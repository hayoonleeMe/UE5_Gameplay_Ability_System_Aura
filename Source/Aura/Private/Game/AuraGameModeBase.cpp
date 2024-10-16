// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SaveGame.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
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
		LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
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

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

		return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
	}
	return nullptr;
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
		AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
		
		UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
	}
}

void AAuraGameModeBase::SaveWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGameInstance);

	if (ULoadScreenSaveGame* SaveData = GetSaveSlotData(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
	{
		FSavedMap SavedMap;
		int32 SavedMapIndex;
		// 해당 월드 정보가 없으면 새로 추가
		if (!SaveData->GetSavedMapWithMapName(WorldName, SavedMap, SavedMapIndex))
		{
			SavedMap.MapAssetName = WorldName;
			SaveData->SavedMaps.Emplace(SavedMap);
			SavedMapIndex = SaveData->SavedMaps.Num() - 1;
		}

		SavedMap.SavedActors.Empty();
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())
			{
				continue;
			}
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			// 엔진 내장 Archive들을 이용해 Actor의 정보를 TArray<uint8> 배열에 저장한다.
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			SavedMap.SavedActors.Emplace(SavedActor);
		}
		// World State Data 업데이트
		if (SaveData->SavedMaps.IsValidIndex(SavedMapIndex))
		{
			SaveData->SavedMaps[SavedMapIndex] = SavedMap;
		}
		UGameplayStatics::SaveGameToSlot(SaveData, AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->SlotName;
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
		if (Actors.Num() > 0)
		{
			AActor* SelectedActor = Actors[0];	// 태그가 없으면 첫 PlayerStart 선택
			for (AActor* Actor : Actors)
			{
				if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
				{
					if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
					{
						SelectedActor = PlayerStart;
						break;
					}
				}
			}
			return SelectedActor;
		}	
	}
	return nullptr;
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Emplace(DefaultMapName, DefaultMap);
}
