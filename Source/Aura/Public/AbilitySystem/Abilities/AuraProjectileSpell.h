// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category="Aura|Projectile")
	void SpawnProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Projectile")
	TSubclassOf<AAuraProjectile> ProjectileClass;
};