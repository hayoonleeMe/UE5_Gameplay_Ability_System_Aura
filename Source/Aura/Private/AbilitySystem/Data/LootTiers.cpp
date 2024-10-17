// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;
	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumToSpawn; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnItems.Emplace(NewItem);
			}
		}
	}
	return ReturnItems;
}
