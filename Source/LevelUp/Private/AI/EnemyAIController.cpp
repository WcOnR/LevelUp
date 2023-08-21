// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

static const FName PlayerKey("Player");
static const FName LastPlayerLocationKey("LastPlayerLocation");
static const FName SpawnLocationKey("SpawnLocation");
static const FName SpawnLookAtKey("SpawnLookAt");

AEnemyAIController::AEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
	InitPosintion();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InitPosintion();
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsObject(PlayerKey, Actor);
		Blackboard->ClearValue(LastPlayerLocationKey);
	}
	else
	{
		Blackboard->ClearValue(PlayerKey);
		Blackboard->SetValueAsVector(LastPlayerLocationKey, Actor->GetActorLocation());
	}
}

void AEnemyAIController::InitPosintion()
{
	if (Blackboard && GetPawn())
	{
		APawn* TmpPawn = GetPawn();
		Blackboard->SetValueAsVector(SpawnLocationKey, TmpPawn->GetActorLocation());
		Blackboard->SetValueAsVector(SpawnLookAtKey, TmpPawn->GetActorLocation() + TmpPawn->GetActorForwardVector() * 100.0f);
	}
}
