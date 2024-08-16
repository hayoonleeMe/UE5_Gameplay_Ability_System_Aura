// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		// TODO : We are on the server, so listen for target data.
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 해당 블럭 내의 작업이 Predicted 되도록 한다.
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	if (const APlayerController* PC = Ability->GetActorInfo().PlayerController.Get())
	{
		FHitResult CursorHit;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
		Data->HitResult = CursorHit;
		DataHandle.Add(Data);

		AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

		// Ability Task의 Delegate를 Broadcast하기 전에 호출해 Ability가 여전히 Active한지 확인한다.
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			ValidData.Broadcast(DataHandle);
		}
	}	
}
