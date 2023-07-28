// Copyright


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	// UAuraAssetManager 클래스를 프로젝트의 Asset Manager로 등록하기 위해서 기존 AssetMangaer를 이 클래스로 캐스팅한 것을 반환하는 Get() 메소드를 구현해야 한다.
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();
}
