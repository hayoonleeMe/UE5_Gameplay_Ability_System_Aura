// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	check(LoadScreenViewModelClass);
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	check(LoadScreenWidgetClass);
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();
}
