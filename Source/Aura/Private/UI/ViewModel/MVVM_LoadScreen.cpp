// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	check(LoadSlotViewModelClass);
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SlotName = FString(TEXT("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Emplace(LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SlotName = FString(TEXT("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Emplace(LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SlotName = FString(TEXT("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
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
		LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
		LoadSlots[Slot]->SlotStatus = ESaveSlotStatus::Taken;
		LoadSlots[Slot]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
		
		AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
		LoadSlots[Slot]->InitializeSlot();
		
		if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance()))
		{
			AuraGameInstance->LoadSlotName = LoadSlots[Slot]->SlotName;
			AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
			AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
		}
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SlotStatus = ESaveSlotStatus::EnterName;
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelectedDelegate.Broadcast();
	SelectedSlot = LoadSlots[Slot];
	for (int32 i = 0; i< LoadSlots.Num(); ++i)
	{
		LoadSlots[i]->EnableSelectSlotButtonDelegate.Broadcast(i != Slot);
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->SlotName, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButtonDelegate.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance()))
		{
			AuraGameInstance->LoadSlotName = SelectedSlot->SlotName;
			AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
			AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
		}
		if (IsValid(SelectedSlot))
		{
			AuraGameMode->TravelToMap(SelectedSlot);		
		}
	}
}

void UMVVM_LoadScreen::LoadData()
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		for (int32 i = 0; i < LoadSlots.Num(); ++i)
		{
			if (ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlots[i]->SlotName, i))
			{
				LoadSlots[i]->SetPlayerName(SaveObject->PlayerName);
				LoadSlots[i]->SetMapName(SaveObject->MapName);
				LoadSlots[i]->SlotStatus = SaveObject->SaveSlotStatus;
				LoadSlots[i]->PlayerStartTag = SaveObject->PlayerStartTag;
				LoadSlots[i]->InitializeSlot();
			}
		}
	}
}	
