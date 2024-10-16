// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint/Checkpoint.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckpointMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Checkpoint Mesh Component"));
	CheckpointMeshComponent->SetupAttachment(GetRootComponent());
	CheckpointMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CheckpointMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(CheckpointMeshComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                  bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		HandleGlowEffects();
	}
}

void ACheckpoint::HandleGlowEffects()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMeshComponent->GetMaterial(0), this))
	{
		CheckpointMeshComponent->SetMaterial(0, DynamicMaterialInstance);
		CheckpointReached(DynamicMaterialInstance);
	}
}
