// Copyright


#include "UI/HUD/AuraHUD.h"
#include "UI/WIdget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),  OverlayWidgetClass);
	Widget->AddToViewport();
}
