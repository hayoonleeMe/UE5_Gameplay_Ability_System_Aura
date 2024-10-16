// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadScreenSaveGame.h"

ULoadScreenSaveGame::ULoadScreenSaveGame()
{
	PlayerName = FString(TEXT("Default Name"));
	PlayerLevel = 1;
	bFirstTimeLoadIn = true;
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
