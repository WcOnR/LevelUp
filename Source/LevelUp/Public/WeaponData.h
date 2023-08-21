// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelUpProjectile.h"
#include "GameplayEffect.h"
#include "NiagaraSystem.h"
#include "LevelUpGameplayAbility.h"

#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct LEVELUP_API FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSoftClassPtr<ALevelUpProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSoftObjectPtr<UNiagaraSystem> RayTraceFx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	TSoftClassPtr<ULevelUpGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	TSoftClassPtr<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Impact;
};
