// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Rifle = nullptr;
	AmountOfAmmo = 0;
}

inline void UInventoryComponent::SetRifle(AActor* NewRifle)
{
	if (Rifle != NewRifle)
	{
		Rifle = NewRifle;
		OnWeaponPickUp.Broadcast();
	}
}

AActor* UInventoryComponent::GetRifle() const
{
	return Rifle;
}

void UInventoryComponent::AddAmmo(int32 Amount)
{
	int32 OldAmountOfAmmo = AmountOfAmmo;
	AmountOfAmmo = FMath::Min(MaxAmountOfAmmo, AmountOfAmmo + Amount);
	if (AmountOfAmmo != OldAmountOfAmmo)
	{
		OnAmountOfAmmoChanged.Broadcast(AmountOfAmmo);
	}
}

int32 UInventoryComponent::PopAmmo(int32 Amount)
{
	int32 Result = FMath::Min(Amount, AmountOfAmmo);
	AmountOfAmmo -= Result;
	if (Result > 0)
	{
		OnAmountOfAmmoChanged.Broadcast(AmountOfAmmo);
	}
	return Result;
}

