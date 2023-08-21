// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UBehaviorTree;

UCLASS()
class LEVELUP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	FGenericTeamId GetGenericTeamId() const override { return 1; }

protected:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void InitPosintion();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	UAIPerceptionComponent* AIPerception;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	UBehaviorTree* BehaviorTree;
};
