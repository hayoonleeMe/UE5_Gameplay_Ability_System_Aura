// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	UAuraFireBolt();
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Aura|FireBolt")
	float ProjectileSpread;

	UPROPERTY(EditDefaultsOnly, Category="Aura|FireBolt")
	int32 MaxNumProjectiles;

	UPROPERTY(EditDefaultsOnly, Category="Aura|FireBolt")
	float HomingAccelerationMin;

	UPROPERTY(EditDefaultsOnly, Category="Aura|FireBolt")
	float HomingAccelerationMax;

	UPROPERTY(EditDefaultsOnly, Category="Aura|FireBolt")
	bool bLaunchHomingProjectiles;
};
