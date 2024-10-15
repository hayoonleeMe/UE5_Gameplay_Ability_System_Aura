// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	check(LoadSlotViewModelClass);
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SlotName = FString(TEXT("LoadSlot_0"));
	LoadSlots.Emplace(LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SlotName = FString(TEXT("LoadSlot_1"));
	LoadSlots.Emplace(LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SlotName = FString(TEXT("LoadSlot_2"));
	LoadSlots.Emplace(LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	if  (LoadSlots.IsValidIndex(Index))
	{
		return LoadSlots[Index];
	}
	return nullptr;
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		LoadSlots[Slot]->SetPlayerName(EnteredName);
		AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
}	
