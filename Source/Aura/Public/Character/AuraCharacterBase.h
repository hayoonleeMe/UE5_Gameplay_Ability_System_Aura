// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UDebuffNiagaraComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* Begin Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeathDelegate& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeaponMeshComponent_Implementation() override;
	/* End Combat Interface */

	FOnASCRegistered OnAscRegistered;
	FOnDeathDelegate OnDeathDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_IsStunned, BlueprintReadOnly)
	bool bIsStunned;

	UFUNCTION()
	virtual void OnRep_IsStunned();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	virtual void HideHealthBar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Character Class Defaults")
	ECharacterClass CharacterClass;

	/*
	 *	Combat
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	FName TailSocketName;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	TObjectPtr<USoundBase> DeathSound;

	bool bDead = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Combat")
	float BaseWalkSpeed;

	/*
	 *	Minions
	 */
	int32 MinionCount = 0;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level = 1.f) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	/*
	 *	Dissolve Effects
	 */
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Dissolve Effects")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Dissolve Effects")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/*
	 *	Debuff
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;
	
private:
	UPROPERTY(EditAnywhere, Category="Aura|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category="Aura|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> PassiveStartupAbilities;

	UPROPERTY(EditAnywhere, Category="Aura|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
