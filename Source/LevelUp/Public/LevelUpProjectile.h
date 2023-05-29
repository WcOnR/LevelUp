// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStaticsTypes.h"

#include "LevelUpProjectile.generated.h"

class USphereComponent;
class UGameplayEffect;
class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct LEVELUP_API FProjectileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Impact)
	float Mass = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float Radius = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float LaunchVelocity = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
	TSubclassOf<UGameplayEffect> DamageEffect;
};

UCLASS(config = Game)
class ALevelUpProjectile : public AActor
{
	GENERATED_BODY()

public:
	ALevelUpProjectile();

	void SimulatePath(float CreateonTimeStamp);
	static void SimulateHit(const FProjectileData& SimData,
							APawn* Instigator,
							const FVector& Location,
							const FVector& Velocity,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp);

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void Tick(float DeltaSeconds) override;

	const FProjectileData& GetData() const { return Data; }

protected:
	void BeginPlay() override;

	UFUNCTION()
	void OnRep_TrueLocation();

	void PredictPath(float LifeTime);

	void HandleHit(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	void OnLifeTimeEnd();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FProjectileData Data;

	UPROPERTY(ReplicatedUsing = OnRep_TrueLocation)
	FVector TrueLocation;

	FVector ClientStartLocation;
	float ClientSinceUpdate;
	float ClientSinceLastUpdate;

	UPROPERTY()
	TArray<FPredictProjectilePathPointData> PathData;

	UPROPERTY()
	float BeginPlayTimeStamp;

	UPROPERTY()
	AGameStateBase* GameState;
};
