// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
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
		LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
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
		if (LoadScreenSaveGameClass)
		{
			SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
		}
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

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGameInstance);

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
	{
		// 다른 맵으로 이동하는 경우 이동하고자 하는 맵의 MapAssetName, MapName을 함께 저장한다.
		if (DestinationMapAssetName != FString())
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		FSavedMap SavedMap;
		int32 SavedMapIndex;
		// 해당 월드 정보가 없으면 새로 추가
		if (!SaveGame->GetSavedMapWithMapName(WorldName, SavedMap, SavedMapIndex))
		{
			SavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Emplace(SavedMap);
			SavedMapIndex = SaveGame->SavedMaps.Num() - 1;
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
		if (SaveGame->SavedMaps.IsValidIndex(SavedMapIndex))
		{
			SaveGame->SavedMaps[SavedMapIndex] = SavedMap;
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGameInstance);

	if (UGameplayStatics::DoesSaveGameExist(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex));
		if (!SaveGame)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
			return;
		}
	
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())
			{
				continue;
			}
			FSavedMap SavedMap;
			if (SaveGame->GetSavedMapWithMapName(WorldName, SavedMap))
			{
				for (FSavedActor SavedActor : SavedMap.SavedActors)
				{
					if (SavedActor.ActorName == Actor->GetFName())
					{
						if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
						{
							Actor->SetActorTransform(SavedActor.Transform);
						}

						FMemoryReader MemoryReader(SavedActor.Bytes);
						FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
						Archive.ArIsSaveGame = true;
						Actor->Serialize(Archive);	// converts binary bytes back into variables.
						
						ISaveInterface::Execute_LoadActor(Actor);
					}
				}
			}
		}
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& InMapAssetName) const
{
	for (auto Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == InMapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
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

void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	if (ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData())
	{
		UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Maps.Emplace(DefaultMapName, DefaultMap);
}
