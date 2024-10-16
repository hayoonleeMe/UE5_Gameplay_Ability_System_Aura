// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Magic Circle Decal"));
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MagicCircleDecal->AddLocalRotation(FRotator(0.f, 0.f, DeltaTime * 8.f));
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}
