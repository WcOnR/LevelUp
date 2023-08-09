// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/SaveGame.h"

#include "LevelUpSaveGame.generated.h"

UCLASS()
class LEVELUP_API ULevelUpSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Player Info")
	FVector PlayerPosition;
	UPROPERTY(BlueprintReadWrite, Category = "Player Info")
	FRotator PlayerRotation;
};