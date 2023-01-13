// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"

class UHealthComponent;

UCLASS()
class LEVELUP_API ABreakableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UHealthComponent* HealthComponent;
};
