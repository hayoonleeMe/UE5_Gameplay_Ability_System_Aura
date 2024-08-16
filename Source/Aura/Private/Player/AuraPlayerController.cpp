// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"

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
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
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
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != CurrentActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (CurrentActor)
		{
			CurrentActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurrentActor != nullptr;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// LMB가 아닌 입력
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(GetAuraASC()))
		{
			GetAuraASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	/* LMB Input */
	// Highlightable Object Pressed -> Released (마우스 커서가 특정 오브젝트 위에서 Pressed하고 이후에 Released한다면 특정 어빌리티 활성화)
	if (bTargeting)
	{
		if (GetAuraASC())
		{
			GetAuraASC()->AbilityInputTagReleased(InputTag);
		}
	}
	// Non Highlightable Object Pressed -> Released	(마우스 커서가 특정 오브젝트 위에 있지 않으면서 Pressed하고 이후에 Released한다면 AutoRunning)
	else
	{
		const APawn* ControlledPawn = GetPawn();
		// Non Highlightable Object 위로 짧게 Press하면 Auto Running
		if (ControlledPawn && FollowTime <= ShortPressThreshold)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				SplineComponent->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					SplineComponent->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				bAutoRunning = true;
			}
		}

		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// LMB가 아닌 입력
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(GetAuraASC()))
	    {
			GetAuraASC()->AbilityInputTagHeld(InputTag);
	    }
		return;
	}

	/* LMB Input */
	// Highlightable Object Pressed -> Held (마우스 커서가 특정 오브젝트 위에 있고 Input Held 중이면 특정 어빌리티 활성화)
	if (bTargeting)
	{
		if (GetAuraASC())
		{
			GetAuraASC()->AbilityInputTagHeld(InputTag);
		}
	}
	// Non Highlightable Object Pressed -> Held	(마우스 커서가 특정 오브젝트 위에 있지 않고 Input Held 중이면 Click to Move)
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
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
