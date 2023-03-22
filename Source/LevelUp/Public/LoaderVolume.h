// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LoaderVolume.generated.h"

class UBoxComponent;

UCLASS(config = Game)
class ALoaderVolume : public AActor
{
	GENERATED_BODY()

public:
	ALoaderVolume();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
						  AActor* OtherActor,
						  UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex,
						  bool bFromSweep,
						  const FHitResult& SweepResult);

protected:
	void BeginPlay() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath MeshPath;

	UPROPERTY()
	UStaticMesh* StaticMesh;
};
