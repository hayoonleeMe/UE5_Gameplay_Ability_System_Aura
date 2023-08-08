// Copyright


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(const ECharacterClass& CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
