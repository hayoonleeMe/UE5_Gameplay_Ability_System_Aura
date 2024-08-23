// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// Typical Parameters for Top Down Game
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 캐릭터 가속 방향으로 RotationRate를 적용해 회전시킴
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;	// 이동을 평면에 제한
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// 시작 시 캐릭터가 평면에 부착됨

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

// 서버에서 호출됨 
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();

	// Grant Ability
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
    InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// PlayerController의 경우 서버에서는 모든 Character에 존재하고, 클라이언트에서는 각 기기의 Locally Controlled Character에만 존재한다.
	// 따라서 InitAbilityActorInfo()가 서버측인 PossessedBy()에서 호출되면 모든 캐릭터에서 PlayerController가 Valid하겠지만,
	// InitAbilityActorInfo()가 OnRep_PlayerState와 같이 클라이언트에서 호출되면 하나의 캐릭터만 PlayerController가 Valid하고 나머지 캐릭터에
	// 대해서는 nullptr인 경우도 존재하므로 nullptr 체크를 하여 Player Controller가 Valid한 경우에만 작업을 수행하도록 한다.
	if (AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>())
	{
		if (AAuraHUD* AuraHUD = AuraPlayerController->GetHUD<AAuraHUD>())
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// AbilitySystemComponent가 유효하게 설정된 뒤에, Attribute 초기화
	InitializeDefaultAttributes();
}
