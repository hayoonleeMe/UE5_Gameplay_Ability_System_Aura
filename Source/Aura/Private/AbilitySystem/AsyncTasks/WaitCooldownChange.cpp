// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

// WaitCooldownChange Async Task 객체를 생성해주는 static 함수
UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	// Blueprint Async Task는 NewObject로 생성한다.
	if (UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>())
	{
		if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
		{
			WaitCooldownChange->EndTask();
			return nullptr;
		}
		
		// AbilitySystemComponent는 AuraCharacter의 ASC (AuraCharacter에서 AuraHUD를 통해 Overlay를 초기화할 때 AuraCharacter의 ASC를 전달함 -> WidgetController의 ASC가 됨
		WaitCooldownChange->ASC = AbilitySystemComponent;
		WaitCooldownChange->CooldownTag = InCooldownTag;
		
		// WaitForCooldownChange는 static 함수이므로 AddUObject에 this를 사용하지 못한다. 따라서 위에서 생성한 WaitCooldownChange 객체를 사용한다.
		// To know when a cooldown has ended (Cooldown Tag has been removed)
		AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag).AddUObject(WaitCooldownChange, &ThisClass::CooldownTagChanged);

		// To know when a cooldown effect has been applied
		// OnActiveGameplayEffectAddedDelegateToSelf : Duration based GE가 활성화되면 서버, 클라 모두에서 호출됨
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

		return WaitCooldownChange;
	}
	return nullptr;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC))
	{
		return;
	}

	ASC->RegisterGameplayTagEvent(CooldownTag).RemoveAll(this);
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	// Tag has been removed
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

// Duration based GE가 활성화됐을 때 호출됨 -> Cooldown Tag가 설정되어 있다면 -> CooldownStart 델레게이트를 Cooldown GE의 남은 시간 Time Remaining과 함께 브로드캐스팅함
void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		// CooldownTag를 소유하는 GE가 있는지 쿼리
		const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		// Cooldown Tag를 소유하는 모든 GE의 TimeRemaining 값을 받아온다.
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (!TimesRemaining.IsEmpty())
		{
			// 모든 TimeRemaining 값의 최대값을 함께 브로드캐스팅
			float MaxTimeRemaining = TimesRemaining[0];
			for (int32 Index = 1; Index < TimesRemaining.Num(); ++Index)
			{
				MaxTimeRemaining = FMath::Max(MaxTimeRemaining, TimesRemaining[Index]);
			}
			CooldownStart.Broadcast(MaxTimeRemaining);
		}
	}
}
