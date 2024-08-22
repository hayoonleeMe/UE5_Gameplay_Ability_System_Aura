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
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

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
