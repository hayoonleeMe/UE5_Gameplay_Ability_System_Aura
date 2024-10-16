// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadScreenSaveGame.h"

ULoadScreenSaveGame::ULoadScreenSaveGame()
{
	PlayerName = FString(TEXT("Default Name"));
	PlayerLevel = 1;
	bFirstTimeLoadIn = true;
}

bool ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName, FSavedMap& OutSavedMap, int32& OutIndex)
{
	for (int32 i = 0; i < SavedMaps.Num(); ++i)
	{
		const FSavedMap& Map = SavedMaps[i];
		if (Map.MapAssetName == InMapName)
		{
			OutSavedMap = Map;
			OutIndex = i;
			return true;
		}
	}
	return false;
}

bool ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName, FSavedMap& OutSavedMap)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			OutSavedMap = Map;
			return true;
		}
	}
	return false;
}
