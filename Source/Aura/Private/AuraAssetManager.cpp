// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

// UAuraAssetManager 클래스를 프로젝트의 Asset Manager로 등록하기 위해서 기존 AssetManager를 이 클래스로 캐스팅한 것을 반환하는 Get() 메소드를 구현해야 한다.
UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Asset Manager의 초기 로딩 시 C++ Native GameplayTags 초기화
	FAuraGameplayTags::InitializeNativeGameplayTags();
	
	// This is required to use Target Data
	// TargetDataScriptStructCache를 초기화 해주는 기능을 포함한다.
	UAbilitySystemGlobals::Get().InitGlobalData();	
}
