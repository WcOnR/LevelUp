// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MiniMapMark.generated.h"

class UFollower;

UCLASS()
class LEVELUP_API AMiniMapMark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniMapMark();

protected:
	void BeginPlay() override;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Plane;

	UPROPERTY(VisibleDefaultsOnly)
	UFollower* Follower;
	
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	FLinearColor MainColor;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};
