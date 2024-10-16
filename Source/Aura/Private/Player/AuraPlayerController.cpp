// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
	AutoRunAcceptanceRadius = 50.f;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component")); 
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	// 플레이어 입력을 처리한다.
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInstance* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::ClientShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		if (UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass))
		{
			DamageTextComponent->RegisterComponent();
			DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageTextComponent->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스 커서가 뷰포트에 잠기지 않음
	InputModeData.SetHideCursorDuringCapture(false); // 게임이나 UI 요소에서 마우스 입력을 캡처하더라도 커서는 표시됨
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// CastChecked : 캐스팅과 check()를 동시에 수행
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Block Input Pressed
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	const FVector2D InputAxisValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// 회전 행렬에서 X축 단위벡터 추출 (플레이어 전진 방향)
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// 회전 행렬에서 Y축 단위벡터 추출 (플레이어 좌우 방향)

	// Move()는 매 프레임 호출되고 아직 Pawn이 생성되지 않았을 수도 있기 때문에 Assert를 하기 보다 if로 체크함
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisValue.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	// Block Cursor Trace
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(CurrentActor);
		LastActor = nullptr;
		CurrentActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>() ? CursorHit.GetActor() : nullptr;

	if (LastActor != CurrentActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(CurrentActor);
	}
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// Block Input Pressed
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(CurrentActor))
		{
			TargetingStatus = CurrentActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if (GetAuraASC())
	{
		GetAuraASC()->AbilityInputTagPressed(InputTag);
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// Block Input Released
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	// LMB가 아닌 입력 or LMB on Highlightable Object Pressed -> Released (마우스 커서가 특정 오브젝트 위에서 Pressed하고 이후에 Released한다면 특정 어빌리티 활성화)
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if (IsValid(GetAuraASC()))
		{
			GetAuraASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	/* Click to Move */
	// LMB on Non Highlightable Object Pressed -> Released	(마우스 커서가 특정 오브젝트 위에 있지 않으면서 Pressed하고 이후에 Released한다면 AutoRunning)
	const APawn* ControlledPawn = GetPawn();
	// Non Highlightable Object 위로 짧게 Press하면 Auto Running
	if (ControlledPawn && FollowTime <= ShortPressThreshold)
	{
		// Auto run to highlighted actor, no click niagara system
		if (IsValid(CurrentActor) && CurrentActor->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_SetMoveToLocation(CurrentActor, CachedDestination);
		}
		else if (ClickNiagaraSystem && GetAuraASC() && !GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			SplineComponent->ClearSplinePoints();
			for (const FVector& PointLoc : NavPath->PathPoints)
			{
				SplineComponent->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
			}

			if (!NavPath->PathPoints.IsEmpty())
			{
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			}
			bAutoRunning = true;
		}
	}

	FollowTime = 0.f;
	TargetingStatus = ETargetingStatus::NotTargeting;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// Block Input Held
	if (GetAuraASC() && GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	// LMB가 아닌 입력 or LMB on Highlightable Object Pressed -> Held (마우스 커서가 특정 오브젝트 위에 있고 Input Held 중이면 특정 어빌리티 활성화)
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if (IsValid(GetAuraASC()))
	    {
			GetAuraASC()->AbilityInputTagHeld(InputTag);
	    }
		return;
	}

	/* Held to Move */
	// LMB on Non Highlightable Object Pressed -> Held	(마우스 커서가 특정 오브젝트 위에 있지 않고 Input Held 중이면 Click to Move)
	FollowTime += GetWorld()->GetDeltaSeconds();
    
    if (CursorHit.bBlockingHit)
    {
    	CachedDestination = CursorHit.ImpactPoint;
    }
    if (APawn* ControlledPawn = GetPawn())
    {
    	const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
    	ControlledPawn->AddMovementInput(WorldDirection);
    }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
	if (!IsValid(AuraAbilitySystemComponent))
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning)
	{
		return;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}
