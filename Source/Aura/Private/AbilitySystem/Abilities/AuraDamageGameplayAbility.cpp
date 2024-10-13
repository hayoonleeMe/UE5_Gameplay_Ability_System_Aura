// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

UAuraDamageGameplayAbility::UAuraDamageGameplayAbility()
{
	DebuffChance = 20.f;
	DebuffDamage = 5.f;
	DebuffFrequency = 1.f;
	DebuffDuration = 5.f;
	DeathImpulseMagnitude = 1000.f;
	KnockbackForceMagnitude = 500.f;
}

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	// DamageEffectClass에 저장된 GameplayEffect를 TargetActor에 적용한다.
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, FVector InRadialDamageOrigin,
	bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverrideDeathImpulse, FVector DeathImpulseDirectionOverride,
	bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DebuffDuration = DebuffDuration;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;

	const bool bCanKnockback = FMath::RandRange(1, 100) < KnockbackChance;
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if (bCanKnockback)
		{
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
	}
	if (bCanKnockback && bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
		if (bOverridePitch)
		{
			KnockbackRotation.Pitch = PitchOverride;
		}
		Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
	}
	if (bOverrideDeathImpulse)
	{
		DeathImpulseDirectionOverride.Normalize();
		FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
		if (bOverridePitch)
		{
			DeathImpulseRotation.Pitch = PitchOverride;
		}
		Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
	}
	
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
	}
	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
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
