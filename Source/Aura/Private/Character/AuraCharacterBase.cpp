// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh Component"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), FName(TEXT("WeaponHandSocket")));
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
