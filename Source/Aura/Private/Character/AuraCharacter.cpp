// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
}
