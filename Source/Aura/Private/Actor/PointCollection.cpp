// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Point0 = CreateDefaultSubobject<USceneComponent>(TEXT("Point0"));
	ImmutablePoints.Emplace(Point0);
	SetRootComponent(Point0);

	Point1 = CreateDefaultSubobject<USceneComponent>(TEXT("Point1"));
	ImmutablePoints.Emplace(Point1);
	Point1->SetupAttachment(GetRootComponent());

	Point2 = CreateDefaultSubobject<USceneComponent>(TEXT("Point2"));
	ImmutablePoints.Emplace(Point2);
	Point2->SetupAttachment(GetRootComponent());

	Point3 = CreateDefaultSubobject<USceneComponent>(TEXT("Point3"));
	ImmutablePoints.Emplace(Point3);
	Point3->SetupAttachment(GetRootComponent());
	
	Point4 = CreateDefaultSubobject<USceneComponent>(TEXT("Point4"));
	ImmutablePoints.Emplace(Point4);
	Point4->SetupAttachment(GetRootComponent());

	Point5 = CreateDefaultSubobject<USceneComponent>(TEXT("Point5"));
	ImmutablePoints.Emplace(Point5);
	Point5->SetupAttachment(GetRootComponent());

	Point6 = CreateDefaultSubobject<USceneComponent>(TEXT("Point6"));
	ImmutablePoints.Emplace(Point6);
	Point6->SetupAttachment(GetRootComponent());

	Point7 = CreateDefaultSubobject<USceneComponent>(TEXT("Point7"));
	ImmutablePoints.Emplace(Point7);
	Point7->SetupAttachment(GetRootComponent());

	Point8 = CreateDefaultSubobject<USceneComponent>(TEXT("Point8"));
	ImmutablePoints.Emplace(Point8);
	Point8->SetupAttachment(GetRootComponent());

	Point9 = CreateDefaultSubobject<USceneComponent>(TEXT("Point9"));
	ImmutablePoints.Emplace(Point9);
	Point9->SetupAttachment(GetRootComponent());

	Point10 = CreateDefaultSubobject<USceneComponent>(TEXT("Point10"));
	ImmutablePoints.Emplace(Point10);
	Point10->SetupAttachment(GetRootComponent());
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("Attempted to access ImmutablePoints out of bounds."));

	TArray<USceneComponent*> ArrayCopy;
	for (USceneComponent* Point : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Point != Point0)
		{
			FVector ToPoint = Point->GetComponentLocation() - Point0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(Point0->GetComponentLocation() + ToPoint);
		}
		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - 500.f);
		
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, Point0->GetComponentLocation());
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		FHitResult HitResult;
		//GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName(TEXT("BlockAll")), QueryParams);
		GetWorld()->LineTraceSingleByChannel(HitResult, RaisedLocation, LoweredLocation, ECC_ExcludePlayers, QueryParams);
		
		const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Emplace(Point);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
