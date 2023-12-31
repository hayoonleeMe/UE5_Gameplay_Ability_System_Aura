// Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:
	// 레퍼런스를 반환하여 구조체 내에 하나만 존재하는 정적 변수 GameplayTags 자체에만 접근시킨다.
	// -> 싱글톤 (단 하나의 인스턴스만 존재, 사용)
	static const FAuraGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

	/*
	 *	Primary Attributes
	 */
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/*
	 *	Secondary Attributes
	 */
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	/*
	 *	Meta Attributes
	 */
	FGameplayTag Attributes_Meta_IncomingXP;
	
	/*
	 *	Input Tags
	 */
	FGameplayTag InputTag_LMB;	
	FGameplayTag InputTag_RMB;	
	FGameplayTag InputTag_1;	
	FGameplayTag InputTag_2;	
	FGameplayTag InputTag_3;	
	FGameplayTag InputTag_4;

	/*
	 * Damage Types
	 */
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	/*
	 * Abilities
	 */
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_Fire_FireBolt;

	/*
	 * Cooldown
	 */
	FGameplayTag Cooldown_Fire_FireBolt;
	
	/*
	 * Sockets
	 */
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	/*
	 * Montages
	 */
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	/*
	 * Effects
	 */
	FGameplayTag Effects_HitReact;
	
private:
	static FAuraGameplayTags GameplayTags;
};