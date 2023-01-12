// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEVELUP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	void SetHasRifle(bool bNewHasRifle);
	bool GetHasRifle() const;

	void AddAmmo(int32 Amount);
	int32 PopAmmo(int32 Amount);
	int32 GetAmountOfAmmo() const { return AmountOfAmmo; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	int32 MaxAmountOfAmmo = 20;

	bool bHasRifle;
	int32 AmountOfAmmo;
};
