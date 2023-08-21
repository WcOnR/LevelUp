// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageReceiverInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDamageReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class IDamageReceiverInterface
{
	GENERATED_BODY()

public:
	virtual void ReceiveDamage(const FVector& Location, const FVector& Velocity) = 0;
};