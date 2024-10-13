#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
		: BaseDamage(0.f), AbilityLevel(1), DebuffChance(0.f), DebuffDamage(0.f), DebuffFrequency(0.f), DebuffDuration(0.f), DeathImpulseMagnitude(0.f), DeathImpulse(FVector::ZeroVector), KnockbackForceMagnitude(0.f), KnockbackChance(0.f), KnockbackForce(FVector::ZeroVector), bIsRadialDamage(false), RadialDamageInnerRadius(0.f), RadialDamageOuterRadius(0.f), RadialDamageOrigin(FVector::ZeroVector)
	{}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage;
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType;

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius;
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FAuraGameplayEffectContext()
		: bIsBlockedHit(false), bIsCriticalHit(false), bIsSuccessfulDebuff(false), DebuffDamage(0.f), DebuffDuration(0.f), DebuffFrequency(0.f), DeathImpulse(FVector::ZeroVector), KnockbackForce(FVector::ZeroVector), bIsRadialDamage(false), RadialDamageInnerRadius(0.f), RadialDamageOuterRadius(0.f), RadialDamageOrigin(FVector::ZeroVector)
	{}
	
	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	FORCEINLINE bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	FORCEINLINE float GetDebuffDamage() const { return DebuffDamage; }
	FORCEINLINE float GetDebuffDuration() const { return DebuffDuration; }
	FORCEINLINE float GetDebuffFrequency() const { return DebuffFrequency; }
	FORCEINLINE TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FORCEINLINE FVector GetDeathImpulse() const { return DeathImpulse; }
	FORCEINLINE FVector GetKnockbackForce() const { return KnockbackForce; }
	FORCEINLINE bool IsRadialDamage() const { return bIsRadialDamage; }
	FORCEINLINE float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	FORCEINLINE float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FORCEINLINE FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	FORCEINLINE void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	FORCEINLINE void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	FORCEINLINE void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	FORCEINLINE void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	FORCEINLINE void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	FORCEINLINE void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	FORCEINLINE void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	FORCEINLINE void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	FORCEINLINE void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	FORCEINLINE void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	FORCEINLINE void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	FORCEINLINE void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	FORCEINLINE void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockedHit;

	UPROPERTY() 
	bool bIsCriticalHit;

	UPROPERTY()
	bool bIsSuccessfulDebuff;

	UPROPERTY()
	float DebuffDamage;

	UPROPERTY()
	float DebuffDuration;

	UPROPERTY()
	float DebuffFrequency;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse;

	UPROPERTY()
	FVector KnockbackForce;

	UPROPERTY()
	bool bIsRadialDamage;

	UPROPERTY()
	float RadialDamageInnerRadius;
	
	UPROPERTY()
	float RadialDamageOuterRadius;

	UPROPERTY()
	FVector RadialDamageOrigin;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
