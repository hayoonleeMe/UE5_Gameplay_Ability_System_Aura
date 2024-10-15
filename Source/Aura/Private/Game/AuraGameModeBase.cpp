// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	check(LoadScreenSaveGameClass);
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->SlotName, SlotIndex);
	}		
	if (ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass)))
	{
		LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
		LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
		LoadScreenSaveGame->SaveSlotStatus = ESaveSlotStatus::Taken;
		UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->SlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		check(LoadScreenSaveGameClass);
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}	
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->SlotName;
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Emplace(DefaultMapName, DefaultMap);
}
