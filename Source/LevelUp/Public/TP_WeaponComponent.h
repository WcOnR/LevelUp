// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "LevelUpProjectile.h"

#include "TP_WeaponComponent.generated.h"

class ALevelUpCharacter;
class UInputMappingContext; 

USTRUCT()
struct LEVELUP_API FClientProjectileData
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileData Data;
	UPROPERTY()
	TSubclassOf<ALevelUpProjectile> ProjectileClass;
	UPROPERTY()
	FVector StartPos;
	UPROPERTY()
	FVector Dir;
	UPROPERTY()
	int64 ProjectilePtr;
	UPROPERTY()
	float MaxLifeTime;
	UPROPERTY()
	float TimeStamp;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEVELUP_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	void AttachWeapon(ALevelUpCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void Shoot(TSubclassOf<ALevelUpProjectile> ProjectileClass);

	UFUNCTION(Server, reliable)
	void Server_Fire(const FClientProjectileData& ProjectileData);

	UFUNCTION(Client, reliable)
	void Client_DestroyFakeProjectile(int64 ProjectilePtr);

public:
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FireMappingContext;
};
