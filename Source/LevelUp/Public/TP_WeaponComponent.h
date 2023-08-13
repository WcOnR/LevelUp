// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "LevelUpProjectile.h"

#include "TP_WeaponComponent.generated.h"

class ALevelUpCharacter;
class UInputMappingContext; 
class UNiagaraSystem;
class UNiagaraComponent;

USTRUCT()
struct LEVELUP_API FLaunchRay
{
	GENERATED_BODY()

	FLaunchRay() = default;
	FLaunchRay(const FVector& O, const FVector& D) : Origin(O), Dir(D) {}

	FVector Origin = FVector::ZeroVector;
	FVector Dir = FVector::XAxisVector;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template <>
struct LEVELUP_API TStructOpsTypeTraits<FLaunchRay> : public TStructOpsTypeTraitsBase2<FLaunchRay>
{
	enum
	{
		WithNetSerializer = true,
	};
};

USTRUCT()
struct LEVELUP_API FClientProjectileData
{
	GENERATED_BODY()

	UPROPERTY()
	FProjectileData Data;
	UPROPERTY()
	TSubclassOf<ALevelUpProjectile> ProjectileClass;
	UPROPERTY()
	FLaunchRay LaunchRay;
	UPROPERTY()
	int64 ProjectilePtr = 0;
	UPROPERTY()
	float MaxLifeTime = 0.0f;
	UPROPERTY()
	float TimeStamp = 0.0f;
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
	void Shoot();
	UFUNCTION(BlueprintCallable)
	void StopShoot();
	UFUNCTION(BlueprintCallable)
	void AltShoot();
	UFUNCTION(BlueprintCallable)
	void StopAltShoot();
	
	void ThrowProjectile(TSubclassOf<ALevelUpProjectile> ProjClass);

	UFUNCTION(Server, reliable)
	void Server_ThrowProjectile(const FClientProjectileData& ProjectileData);

	UFUNCTION(Client, reliable)
	void Client_DestroyFakeProjectile(int64 ProjectilePtr);

protected:
	virtual void BeginPlay() override;
	void OnPositionUpdated();

	bool GetTargetHit(FHitResult& OutHit);
	void ApplyHitEffect(const FHitResult& OutHit);
	FVector GetTargetDirection(const FVector& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<ALevelUpProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UNiagaraSystem* ShootingEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY()
	UNiagaraComponent* ShootingFX;
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FireMappingContext;
};
