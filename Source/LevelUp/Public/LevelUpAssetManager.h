// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

#include "LevelUpAssetManager.generated.h"

UCLASS()
class LEVELUP_API ULevelUpAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	void StartInitialLoading() override;
};