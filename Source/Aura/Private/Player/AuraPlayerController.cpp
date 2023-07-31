// Copyright


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * Line trace from cursor. There are several scenarios:
	 *	A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		// both actors are valid
		else
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// () 내의 조건이 false이면 실행 중지 (이 코드에서 Crash 발생)
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// GetLocalPlayer()가 Valid해야만 Subsystem도 Valid하다.
	// 하지만 GetLocalPlayer()가 유효하려면 해당 기기에서 Locally Controlled Player인 하나의 캐릭터만 유효하고 나머지 캐릭터에 대해서는 유효하지 않다.
	// 따라서 기존의 assert(Subsystem) 대신 Subsystem에 대해 nullptr 체크를 수행하도록 변경한다.
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	// 마우스 커서가 뷰포트에 잠기지 않음
	InputModeData.SetHideCursorDuringCapture(false);	// 게임이나 UI 요소에서 마우스 입력을 캡처하더라도 커서는 표시됨
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
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();	// GetControlRotation() : 플레이어 컨트롤러의 회전값 반환
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// 회전 행렬에서 X축 단위벡터 추출 (플레이어 전진 방향)
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// 회전 행렬에서 Y축 단위벡터 추출 (플레이어 좌우 방향)

	// Move()는 매 프레임 호출되고 아직 Pawn이 생성되지 않았을 수도 있기 때문에 Assert를 하기 보다 if로 체크함
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);	// 앞뒤 이동이므로 Y축 값
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);	// 좌우 이동이므로 X축 값
	}
}
