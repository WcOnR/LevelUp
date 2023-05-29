// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlueprintCompileTestActor.generated.h"

UCLASS()
class LEVELUP_API ABlueprintCompileTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlueprintCompileTestActor();

	UFUNCTION(BlueprintCallable)
	int32 GetValue() const { return Value; }

protected:

	int32 Value = 10;

};
