// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(DebuffTag).AddUObject(this, &ThisClass::DebuffTagChanged);		
	}
	// BeginPlay에서 ASC가 유효하지 않다면, ASC가 유효해질 때 ASC를 브로드캐스트해주는 델레게이트를 이용해 이벤트를 바인드해준다.
	else if (CombatInterface)
	{
		// AddWeakLambda를 사용하면 해당 람다 함수에서 객체들을 캡쳐할 때(여기서는 this) reference counting하지 않아 해당 객체들이 garbage collected 될 수 있다.
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC) {
			InASC->RegisterGameplayTagEvent(DebuffTag).AddUObject(this, &ThisClass::DebuffTagChanged);
		});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	
	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
