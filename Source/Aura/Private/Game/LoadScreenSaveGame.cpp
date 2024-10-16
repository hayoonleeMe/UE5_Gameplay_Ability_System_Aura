// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LoadScreenSaveGame.h"

ULoadScreenSaveGame::ULoadScreenSaveGame()
{
	PlayerName = FString(TEXT("Default Name"));
	PlayerLevel = 1;
	bFirstTimeLoadIn = true;
}
