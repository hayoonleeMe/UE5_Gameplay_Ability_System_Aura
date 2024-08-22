// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UAuraSummonAbility();
	
	UFUNCTION(BlueprintCallable, Category="Aura|Summoning")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category="Aura|Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category="Aura|Summoning")
	int32 NumMinions;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Summoning")
	float MinSpawnDistance;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Summoning")
	float MaxSpawnDistance;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Summoning")
	float SpawnSpread;
};
