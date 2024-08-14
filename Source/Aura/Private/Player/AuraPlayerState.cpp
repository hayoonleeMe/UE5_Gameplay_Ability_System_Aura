// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	// 서버가 클라이언트를 업데이트하는 빈도
	// 서버에서 Player State를 변경하면 서버는 동기화를 위해 클라이언트를 업데이트시킴
	// 기본적으로 Player State의 Frequency는 낮기 때문에 Player State에서 Ability System Component와 Attribute Set을 가지기 위해선 빈도를 높여야 함
	NetUpdateFrequency = 100.f;
}
