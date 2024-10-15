// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	check(LoadSlotViewModelClass);
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Emplace(LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Emplace(LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Emplace(LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	if  (LoadSlots.IsValidIndex(Index))
	{
		return LoadSlots[Index];
	}
	return nullptr;
}
