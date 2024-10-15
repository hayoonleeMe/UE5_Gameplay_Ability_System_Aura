// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndexDelegate;

	void InitializeSlot();

	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	ESaveSlotStatus SlotStatus;

	/*
	 *	Field Notifies
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString PlayerName;

	void SetPlayerName(const FString& InPlayerName);

	FString GetPlayerName() const { return PlayerName; }
};
