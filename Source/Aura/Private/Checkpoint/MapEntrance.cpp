// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SphereComponent->SetupAttachment(MoveToComponent);
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a Map Entrance.
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMeshComponent->SetRenderCustomDepth(true);
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
