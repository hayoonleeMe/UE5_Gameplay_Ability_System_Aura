// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
    // 레퍼런스를 반환하여 구조체 내에 하나만 존재하는 정적 변수 GameplayTags 자체에만 접근시킨다.
    // -> 싱글톤 (단 하나의 인스턴스만 존재, 사용)
	static FAuraGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

private:
	static FAuraGameplayTags GameplayTags;
};
