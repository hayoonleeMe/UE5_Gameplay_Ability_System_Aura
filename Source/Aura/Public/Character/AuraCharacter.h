// Copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class AAuraPlayerState;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	/* End Player Interface */
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */

private:
	virtual void InitAbilityActorInfo() override;
};
