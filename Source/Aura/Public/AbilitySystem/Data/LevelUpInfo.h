// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	FAuraLevelUpInfo()
		: LevelUpRequirement(1), AttributePointAward(1), SpellPointAward(1)
	{}

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(const int32 XP) const;
};
