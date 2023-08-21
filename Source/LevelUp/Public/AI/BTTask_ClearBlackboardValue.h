// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearBlackboardValue.generated.h"

struct FBlackboardKeySelector;

UCLASS()
class LEVELUP_API UBTTask_ClearBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()


	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BlackboardKey;
};
