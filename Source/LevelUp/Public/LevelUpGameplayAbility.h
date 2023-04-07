// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputAction.h"

#include "LevelUpGameplayAbility.generated.h"

UCLASS()
class LEVELUP_API ULevelUpGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULevelUpGameplayAbility() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* Input;
};