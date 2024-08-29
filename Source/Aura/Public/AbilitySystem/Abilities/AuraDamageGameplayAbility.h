// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "Types/AuraAbilityTypes.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UAuraDamageGameplayAbility();
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	float DebuffChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	float DebuffDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	float DebuffFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	float DebuffDuration;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
