// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LevelUpGameMode.generated.h"

UCLASS(minimalapi, Config = Game)
class ALevelUpGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALevelUpGameMode();

protected:
	UFUNCTION(Exec)
	void AddAmmo() const;

protected:
	UPROPERTY(Config)
	int32 CheatAddAmmoAmount = 20;
};
