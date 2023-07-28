// Copyright


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

// 정적 변수를 메모리에 할당하기 위해 구조체 외부에서 외부 정의를 반드시 수행해야 한다.
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString(TEXT("Reduces damage taken, improves Block Chance")));
}
