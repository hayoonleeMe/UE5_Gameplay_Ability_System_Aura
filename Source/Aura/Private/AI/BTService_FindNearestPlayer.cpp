// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();
	
	// OwningPawn이 플레이어면 타겟이 Enemy이고, Enemy이면 타겟이 플레이어이다.
	const FName TargetTag = OwningPawn->ActorHasTag(FName(TEXT("Player"))) ? FName(TEXT("Enemy")) : FName(TEXT("Player"));

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);
}
