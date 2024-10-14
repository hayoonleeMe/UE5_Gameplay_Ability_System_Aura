// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

// 정적 변수를 메모리에 할당하기 위해 구조체 외부에서 외부 정의를 반드시 수행해야 한다.
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	
	/* Primary Attributes */
	GameplayTags.Attributes_Primary_Strength = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Primary.Strength")), FString(TEXT("Increases physical damage")));
	
	GameplayTags.Attributes_Primary_Intelligence = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Primary.Intelligence")), FString(TEXT("Increases magical damage")));
	
	GameplayTags.Attributes_Primary_Resilience = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Primary.Resilience")), FString(TEXT("Increases Armor and Armor Penetration")));
	
	GameplayTags.Attributes_Primary_Vigor = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Primary.Vigor")), FString(TEXT("Increases Health")));

	/* Secondary Attributes */
	GameplayTags.Attributes_Secondary_Armor = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.Armor")), FString(TEXT("Reduces damage taken, improves Block Chance")));

	GameplayTags.Attributes_Secondary_ArmorPenetration = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.ArmorPenetration")), FString(TEXT("Ignored Percentage of enemy Armor, increases Critical Hit Chance")));

	GameplayTags.Attributes_Secondary_BlockChance = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.BlockChance")), FString(TEXT("Chance to cut incoming damage in half")));

	GameplayTags.Attributes_Secondary_CriticalHitChance = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.CriticalHitChance")), FString(TEXT("Chance to double damage plus critical hit bonus")));

	GameplayTags.Attributes_Secondary_CriticalHitDamage = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.CriticalHitDamage")), FString(TEXT("Bonus damage added when a critical hit is scored")));

	GameplayTags.Attributes_Secondary_CriticalHitResistance = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.CriticalHitResistance")), FString(TEXT("Reduces Critical Hit Chance of attacking enemies")));

	GameplayTags.Attributes_Secondary_HealthRegeneration = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.HealthRegeneration")), FString(TEXT("Amount of Health regenerated every 1 second")));

	GameplayTags.Attributes_Secondary_ManaRegeneration = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.ManaRegeneration")), FString(TEXT("Amount of Mana regenerated every 1 second")));

	GameplayTags.Attributes_Secondary_MaxHealth = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.MaxHealth")), FString(TEXT("Maximum amount of Health obtainable")));
	
	GameplayTags.Attributes_Secondary_MaxMana = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Secondary.MaxMana")), FString(TEXT("Maximum amount of Mana obtainable")));

	/* Vital Attributes */
	GameplayTags.Attributes_Vital_Health = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Vital.Health")), FString(TEXT("Amount of damage a player can take before death")));
	GameplayTags.Attributes_Vital_Mana = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Vital.Mana")), FString(TEXT("Amount of energy a player can use to cast spells")));

	/* Meta Attributes */
	GameplayTags.Attributes_Meta_IncomingXP = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Meta.IncomingXP")), FString(TEXT("Incoming XP Meta Attribute")));
	
	/* Input Tags */
	GameplayTags.InputTag_LMB = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.LMB")), FString(TEXT("Input Tag for Left Mouse Button")));
	GameplayTags.InputTag_RMB = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.RMB")), FString(TEXT("Input Tag for Right Mouse Button")));
	GameplayTags.InputTag_1 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.1")), FString(TEXT("Input Tag for 1 key")));
	GameplayTags.InputTag_2 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.2")), FString(TEXT("Input Tag for 2 key")));
	GameplayTags.InputTag_3 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.3")), FString(TEXT("Input Tag for 3 key")));
	GameplayTags.InputTag_4 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.4")), FString(TEXT("Input Tag for 4 key")));
	GameplayTags.InputTag_Passive_1 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.Passive.1")), FString(TEXT("Input Tag Passive Ability 1")));
	GameplayTags.InputTag_Passive_2 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.Passive.2")), FString(TEXT("Input Tag Passive Ability 2")));

	/* Damage Type Tags */
	GameplayTags.Damage = Manager.AddNativeGameplayTag(FName(TEXT("Damage")), FString(TEXT("Damage")));
	GameplayTags.Damage_Fire = Manager.AddNativeGameplayTag(FName(TEXT("Damage.Fire")), FString(TEXT("Fire Damage Type")));
	GameplayTags.Damage_Lightning = Manager.AddNativeGameplayTag(FName(TEXT("Damage.Lightning")), FString(TEXT("Lightning Damage Type")));
	GameplayTags.Damage_Arcane = Manager.AddNativeGameplayTag(FName(TEXT("Damage.Arcane")), FString(TEXT("Arcane Damage Type")));
	GameplayTags.Damage_Physical = Manager.AddNativeGameplayTag(FName(TEXT("Damage.Physical")), FString(TEXT("Physical Damage Type")));

	/* Resistance Attributes */
	GameplayTags.Attributes_Resistance_Fire = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Resistance.Fire")), FString(TEXT("Resistance to Fire Damage")));
	GameplayTags.Attributes_Resistance_Lightning = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Resistance.Lightning")), FString(TEXT("Resistance to Lightning Damage")));
	GameplayTags.Attributes_Resistance_Arcane = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Resistance.Arcane")), FString(TEXT("Resistance to Arcane Damage")));
	GameplayTags.Attributes_Resistance_Physical = Manager.AddNativeGameplayTag(FName(TEXT("Attributes.Resistance.Physical")), FString(TEXT("Resistance to Physical Damage")));

	/* Map of Damage Types to Resistances */
	GameplayTags.DamageTypesToResistances.Emplace(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Emplace(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Emplace(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Emplace(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	/* Debuffs Tags */
	GameplayTags.Debuff_Burn = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Burn")), FString(TEXT("Debuff for Fire Damage")));
	GameplayTags.Debuff_Stun = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Stun")), FString(TEXT("Debuff for Lightning Damage")));
	GameplayTags.Debuff_Arcane = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Arcane")), FString(TEXT("Debuff for Arcane Damage")));
	GameplayTags.Debuff_Physical = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Phsycial")), FString(TEXT("Debuff for Physical Damage")));

	GameplayTags.Debuff_Chance = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Chance")), FString(TEXT("Debuff Chance")));
	GameplayTags.Debuff_Damage = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Damage")), FString(TEXT("Debuff Damage")));
	GameplayTags.Debuff_Frequency = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Frequency")), FString(TEXT("Debuff Frequency")));
	GameplayTags.Debuff_Duration = Manager.AddNativeGameplayTag(FName(TEXT("Debuff.Duration")), FString(TEXT("Debuff Duration")));

	/* Map of Damage Types to Debuffs */
	GameplayTags.DamageTypesToDebuffs.Emplace(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Emplace(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Emplace(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Emplace(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);

	/* Abilities (Active) */
	GameplayTags.Abilities_None = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.None")), FString(TEXT("No Ability - like the nullptr for Ability Tags")));
	
	GameplayTags.Abilities_Attack = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Attack")), FString(TEXT("Attack Ability Tag")));
	GameplayTags.Abilities_Summon = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Summon")), FString(TEXT("Summon Ability Tag")));
	GameplayTags.Abilities_HitReact = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.HitReact")), FString(TEXT("HitReact Ability Tag")));

	GameplayTags.Abilities_Status_Locked = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Status.Locked")), FString(TEXT("Locked Status Tag")));
	GameplayTags.Abilities_Status_Eligible = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Status.Eligible")), FString(TEXT("Eligible Status Tag")));
	GameplayTags.Abilities_Status_Unlocked = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Status.Unlocked")), FString(TEXT("Unlocked Status Tag")));
	GameplayTags.Abilities_Status_Equipped = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Status.Equipped")), FString(TEXT("Equipped Status Tag")));
	
	GameplayTags.Abilities_Type_Offensive = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Type.Offensive")), FString(TEXT("Type Offensive")));
	GameplayTags.Abilities_Type_Passive = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Type.Passive")), FString(TEXT("Type Passive")));
	GameplayTags.Abilities_Type_None = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Type.None")), FString(TEXT("Type None")));
	
	GameplayTags.Abilities_Fire_FireBolt = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Fire.FireBolt")), FString(TEXT("FireBolt Ability Tag")));
	GameplayTags.Abilities_Fire_FireBlast = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Fire.FireBlast")), FString(TEXT("FireBlast Ability Tag")));
	GameplayTags.Abilities_Lightning_Electrocute = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Lightning.Electrocute")), FString(TEXT("Electrocute Ability Tag")));
	GameplayTags.Abilities_Arcane_ArcaneShards = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Arcane.ArcaneShards")), FString(TEXT("ArcaneShards Ability Tag")));

	/* Abilities (Passive) */
	GameplayTags.Abilities_Passive_HaloOfProtection = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Passive.HaloOfProtection")), FString(TEXT("Halo Of Protection")));
	GameplayTags.Abilities_Passive_LifeSiphon = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Passive.LifeSiphon")), FString(TEXT("Life Siphon")));
	GameplayTags.Abilities_Passive_ManaSiphon = Manager.AddNativeGameplayTag(FName(TEXT("Abilities.Passive.ManaSiphon")), FString(TEXT("Mana Siphon")));

	/* Cooldown */
	GameplayTags.Cooldown_Fire_FireBolt = Manager.AddNativeGameplayTag(FName(TEXT("Cooldown.Fire.FireBolt")), FString(TEXT("FireBolt Cooldown Tag")));
	
	/* Sockets */
	GameplayTags.CombatSocket_Weapon = Manager.AddNativeGameplayTag(FName(TEXT("CombatSocket.Weapon")), FString(TEXT("Weapon")));
	GameplayTags.CombatSocket_RightHand = Manager.AddNativeGameplayTag(FName(TEXT("CombatSocket.RightHand")), FString(TEXT("Right Hand")));
	GameplayTags.CombatSocket_LeftHand = Manager.AddNativeGameplayTag(FName(TEXT("CombatSocket.LeftHand")), FString(TEXT("Left Hand")));
	GameplayTags.CombatSocket_Tail = Manager.AddNativeGameplayTag(FName(TEXT("CombatSocket.Tail")), FString(TEXT("Tail")));

	/* Montages */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString(TEXT("Attack 1")));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString(TEXT("Attack 2")));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString(TEXT("Attack 3")));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString(TEXT("Attack 4")));

	/* Effect Tags */
	GameplayTags.Effects_HitReact = Manager.AddNativeGameplayTag(FName(TEXT("Effects.HitReact")), FString(TEXT("Tag granted when Hit Reacting")));

	/* Player Tags */
	GameplayTags.Player_Block_InputPressed = Manager.AddNativeGameplayTag(FName(TEXT("Player.Block.InputPressed")), FString(TEXT("Block input pressed callback for input")));
	GameplayTags.Player_Block_InputHeld = Manager.AddNativeGameplayTag(FName(TEXT("Player.Block.InputHeld")), FString(TEXT("Block input held callback for input")));
	GameplayTags.Player_Block_InputReleased = Manager.AddNativeGameplayTag(FName(TEXT("Player.Block.InputReleased")), FString(TEXT("Block input released callback for input")));
	GameplayTags.Player_Block_CursorTrace = Manager.AddNativeGameplayTag(FName(TEXT("Player.Block.CursorTrace")), FString(TEXT("Block tracing under the cursor")));

	/* Gameplay Cues */
	GameplayTags.GameplayCue_FireBlast = Manager.AddNativeGameplayTag(FName(TEXT("GameplayCue.FireBlast")), FString(TEXT("FireBlast GameplayCue Tag")));
}
