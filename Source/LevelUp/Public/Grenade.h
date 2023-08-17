// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Grenade.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UGameplayEffect;
class UNiagaraSystem;

UCLASS(config = Game)
class AGrenade : public AActor
{
	GENERATED_BODY()

public:
	AGrenade();

	UStaticMeshComponent* GetMeshComponent() const { return Mesh; }
	UProjectileMovementComponent* GetMovementComponent() const { return Movement; }
	float GetDelay() const { return Delay; }

protected:
	void BeginPlay() override;

	void OnDelayEnd();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USphereComponent* DamageSphere;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* Movement;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* Fx;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float Delay;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float ExplosionImpulse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	TSubclassOf<UGameplayEffect> DamageEffect;
};
