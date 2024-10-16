// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};


/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	ULoadScreenSaveGame();
	
	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString MapName;

	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	ESaveSlotStatus SaveSlotStatus;
};
