// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	FSavedAbility()
		: AbilityLevel(0)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Class Defaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel;
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

	UPROPERTY()
	bool bFirstTimeLoadIn;

	/*
	 *	Player
	 */
	UPROPERTY()
	int32 PlayerLevel;

	UPROPERTY()
	int32 XP;

	UPROPERTY()
	int32 SpellPoints;

	UPROPERTY()
	int32 AttributePoints;

	/*
	 *	Attributes
	 */
	UPROPERTY()
	float Strength;

	UPROPERTY()
	float Intelligence;

	UPROPERTY()
	float Resilience;

	UPROPERTY()
	float Vigor;

	/*
	 *	Abilities
	 */
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
};
