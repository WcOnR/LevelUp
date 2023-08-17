// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GrenadeLauncherComponent.generated.h"

class AGrenade;
class UPathPredictComponent;

UCLASS(Blueprintable, BlueprintType)
class LEVELUP_API UGrenadeLauncherComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGrenadeLauncherComponent();

	void BeginPlay() override;
	void OnPositionUpdated();

	UFUNCTION(BlueprintCallable)
	void PredictPath();
	UFUNCTION(BlueprintCallable)
	void ThrowGrenade();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AGrenade> GrenadeClass;
	
	UPROPERTY()
	FPredictProjectilePathParams PreProjParams;
	UPROPERTY()
	UPathPredictComponent* PathPredict;
	bool bShowPath = false;
};
