// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ReloadGEExeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class LEVELUP_API UReloadGEExeCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UReloadGEExeCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
