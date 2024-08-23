// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const
{
	// LevelUpInformation[n] = Level n Information
	int32 Level = 1;
	for (; Level <= LevelUpInformation.Num() - 1 && XP >= LevelUpInformation[Level].LevelUpRequirement; ++Level)
	{}
	return Level;
}
