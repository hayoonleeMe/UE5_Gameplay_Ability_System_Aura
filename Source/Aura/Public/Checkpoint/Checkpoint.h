// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached;

	/* Begin Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	/* End Save Interface */

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMeshComponent;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
};
