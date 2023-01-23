// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponPickUp);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmountOfAmmoChanged, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEVELUP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	FOnWeaponPickUp OnWeaponPickUp;
	FOnAmountOfAmmoChanged OnAmountOfAmmoChanged;

	void SetRifle(AActor* NewRifle);
	AActor* GetRifle() const;

	void AddAmmo(int32 Amount);
	int32 PopAmmo(int32 Amount);
	int32 GetAmountOfAmmo() const { return AmountOfAmmo; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	int32 MaxAmountOfAmmo = 20;

	UPROPERTY()
	AActor* Rifle;
	int32 AmountOfAmmo;
};
