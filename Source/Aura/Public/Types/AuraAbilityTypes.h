#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
		: BaseDamage(0.f), AbilityLevel(1), DebuffChance(0.f), DebuffDamage(0.f), DebuffFrequency(0.f), DebuffDuration(0.f), DeathImpulseMagnitude(0.f), DeathImpulse(FVector::ZeroVector)
	{}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage;
	
	UPROPERTY()
	float AbilityLevel;

	UPROPERTY()
	FGameplayTag DamageType;

	UPROPERTY()
	float DebuffChance;

	UPROPERTY()
	float DebuffDamage;

	UPROPERTY()
	float DebuffFrequency;

	UPROPERTY()
	float DebuffDuration;

	UPROPERTY()
	float DeathImpulseMagnitude;

	UPROPERTY()
	FVector DeathImpulse;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FAuraGameplayEffectContext()
		: bIsBlockedHit(false), bIsCriticalHit(false), bIsSuccessfulDebuff(false), DebuffDamage(0.f), DebuffDuration(0.f), DebuffFrequency(0.f), DeathImpulse(FVector::ZeroVector)
	{}
	
	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	FORCEINLINE bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	FORCEINLINE float GetDebuffDamage() const { return DebuffDamage; }
	FORCEINLINE float GetDebuffDuration() const { return DebuffDuration; }
	FORCEINLINE float GetDebuffFrequency() const { return DebuffFrequency; }
	FORCEINLINE TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FORCEINLINE FVector GetDeathImpulse() const { return DeathImpulse; }
	
	FORCEINLINE void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	FORCEINLINE void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	FORCEINLINE void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	FORCEINLINE void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	FORCEINLINE void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	FORCEINLINE void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	FORCEINLINE void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	FORCEINLINE void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }

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
