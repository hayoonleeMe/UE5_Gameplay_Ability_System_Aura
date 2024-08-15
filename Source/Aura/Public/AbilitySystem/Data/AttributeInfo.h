// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	FAuraAttributeInfo()
		: AttributeTag(), AttributeName(), AttributeDescription(), AttributeValue(0.f)
	{}
	
	// EditDefaultsOnly : 블루프린트에서 기본값만 변경 가능(인스턴스는 불가) (+ 기본값과 인스턴스에서 둘다 변경 가능한 것이 EditAnywhere)
	// BlueprintReadOnly : 블루프린트에서 해당 값을 읽을 수 있음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription;

	// BlueprintReadOnly만 사용했으므로 블루프린트에서 해당 값을 읽을 수만 있고 블루프린트의 Details 패널 등에서 따로 보거나 변경할 수는 없음
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue;
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInfos;
};
