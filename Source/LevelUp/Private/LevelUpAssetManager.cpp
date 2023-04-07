// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpAssetManager.h"
#include "AbilitySystemGlobals.h"

void ULevelUpAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
