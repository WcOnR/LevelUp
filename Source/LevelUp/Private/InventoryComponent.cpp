// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bHasRifle = false;
	AmountOfAmmo = 0;
}

inline void UInventoryComponent::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool UInventoryComponent::GetHasRifle() const
{
	return bHasRifle;
}

void UInventoryComponent::AddAmmo(int32 Amount)
{
	AmountOfAmmo = FMath::Min(MaxAmountOfAmmo, AmountOfAmmo + Amount);
}

int32 UInventoryComponent::PopAmmo(int32 Amount)
{
	int32 Result = FMath::Min(Amount, AmountOfAmmo);
	AmountOfAmmo -= Result;
	return Result;
}

