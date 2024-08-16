// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (const APlayerController* PC = Ability->GetActorInfo().PlayerController.Get())
	{
		FHitResult CursorHit;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
		ValidData.Broadcast(CursorHit.ImpactPoint);
	}
}
