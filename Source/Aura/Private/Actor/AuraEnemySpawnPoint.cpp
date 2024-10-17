// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

AAuraEnemySpawnPoint::AAuraEnemySpawnPoint()
{
	EnemyLevel = 1;
	CharacterClass = ECharacterClass::Warrior;
}

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform()))
	{
		Enemy->SetLevel(EnemyLevel);
		Enemy->SetCharacterClass(CharacterClass);
		Enemy->FinishSpawning(GetActorTransform());
		Enemy->SpawnDefaultController();
	}
}
