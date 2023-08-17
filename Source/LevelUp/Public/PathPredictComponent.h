// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

#include "PathPredictComponent.generated.h"

class AGrenade;
class USplineMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class LEVELUP_API UPathPredictComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UPathPredictComponent();

	void DrawPath(const FPredictProjectilePathResult& Path);
	void ClearPath();

protected:
	void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshes;
};
