// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

UAuraSummonAbility::UAuraSummonAbility()
{
	NumMinions = 5;
	MinSpawnDistance = 150.f;
	MaxSpawnDistance = 400.f;
	SpawnSpread = 90.f;
}

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	TArray<FVector> SpawnLocations;
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	for (int32 Index = 0; Index < NumMinions; ++Index)
	{
		const FVector Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * Index, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		if (IsValid(GetWorld()) && GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility))
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Emplace(ChosenSpawnLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	return MinionClasses[FMath::RandRange(0, MinionClasses.Num() - 1)];
}
