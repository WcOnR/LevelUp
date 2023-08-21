// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ClearBlackboardValue.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->ClearValue(BlackboardKey.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}
