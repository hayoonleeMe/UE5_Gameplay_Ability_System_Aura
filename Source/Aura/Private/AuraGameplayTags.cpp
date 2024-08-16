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

	/* Input Tags */
	GameplayTags.InputTag_LMB = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.LMB")), FString(TEXT("Input Tag for Left Mouse Button")));
	GameplayTags.InputTag_RMB = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.RMB")), FString(TEXT("Input Tag for Right Mouse Button")));
	GameplayTags.InputTag_1 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.1")), FString(TEXT("Input Tag for 1 key")));
	GameplayTags.InputTag_2 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.2")), FString(TEXT("Input Tag for 2 key")));
	GameplayTags.InputTag_3 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.3")), FString(TEXT("Input Tag for 3 key")));
	GameplayTags.InputTag_4 = Manager.AddNativeGameplayTag(FName(TEXT("InputTag.4")), FString(TEXT("Input Tag for 4 key")));
}
