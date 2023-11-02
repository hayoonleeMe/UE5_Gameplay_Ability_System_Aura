// Copyright


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const
{
	// LevelUpInformation[n] = Level n Information
	int32 Level = 1;
	for (; Level <= LevelUpInformation.Num() - 1; ++Level)	
		if (XP < LevelUpInformation[Level].LevelUpRequirement)
			break;
	return Level;
}
