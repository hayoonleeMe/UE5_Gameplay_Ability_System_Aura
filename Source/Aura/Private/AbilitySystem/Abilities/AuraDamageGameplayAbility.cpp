// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UAuraDamageGameplayAbility::UAuraDamageGameplayAbility()
{
	DebuffChance = 20.f;
	DebuffDamage = 5.f;
	DebuffFrequency = 1.f;
	DebuffDuration = 5.f;
}

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	// DamageEffectClass에 저장된 GameplayEffect를 TargetActor에 적용한다.
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UAuraDamageGameplayAbility::GetTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (!TaggedMontages.IsEmpty())
	{
		const int32 RandIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandIndex];
	}
	return FTaggedMontage();
}
