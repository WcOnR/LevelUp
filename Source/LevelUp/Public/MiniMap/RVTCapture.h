// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RVTCapture.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class LEVELUP_API ARVTCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARVTCapture();

protected:
	void BeginPlay() override;
	void OnPositionUpdated();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Plane;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Capture)
	USceneCaptureComponent2D* Capture;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY()
	AController* PlayerController;

	float Size;
};
