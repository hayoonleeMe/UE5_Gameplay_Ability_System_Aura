// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	FLootItem()
		: ChanceToSpawn(0.f), MaxNumToSpawn(0), bLootLevelOverride(true)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loot Tiers|Spawning")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere, Category="Loot Tiers|Spawning")
	float ChanceToSpawn;

	UPROPERTY(EditAnywhere, Category="Loot Tiers|Spawning")
	int32 MaxNumToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loot Tiers|Spawning")
	bool bLootLevelOverride;
};

/**
 * 
 */
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Loot Tiers|Spawning")
	TArray<FLootItem> LootItems;

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
};
