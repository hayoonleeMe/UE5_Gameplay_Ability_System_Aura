// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	OnSpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
	{
		// 현재 선택한 Spell Globe의 Ability Status가 업데이트됐을 때
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			
			bool bEnableSpendPointButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
			FString Description, NextLevelDescription;
			GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 Points)
	{
		OnSpellPointsChangedDelegate.Broadcast(Points);
		CurrentSpellPoints = Points;
		
		bool bEnableSpendPointButton = false;
		bool bEnableEquipButton = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
		FString Description, NextLevelDescription;
		GetAuraASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).TypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayTag AbilityStatusTag;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag); 
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatusTag = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatusTag = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);		
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatusTag;
	
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	bool bEnableSpendPointButton = false;
	bool bEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatusTag, SpellPoints, bEnableSpendPointButton, bEnableEquipButton);
	FString Description, NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).TypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityTypeTag = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).TypeTag;
	WaitForEquipDelegate.Broadcast(AbilityTypeTag);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection)
	{
		return;
	}
	// Check selected ability against the slot's type. (don't equip an offensive spell in a passive slot and vice versa);
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).TypeTag;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
	{
		return;
	}

	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot,
	const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast Empty Info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell.
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(Info.TypeTag);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatusTag, int32 SpellPoints, bool& bShouldEnableSpendPointButton,
                                                     bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if (AbilityStatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableEquipButton = false;
		bShouldEnableSpendPointButton = false;
	}
}
