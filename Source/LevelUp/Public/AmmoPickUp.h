// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickUp.generated.h"

class ALevelUpCharacter;
class URotatingMovementComponent;
class UTP_PickUpComponent;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
static TAutoConsoleVariable<int32> CVarAmmoPickUpAmount(
    TEXT("AmmoPickUp.Amount"),
    0,
    TEXT("Defines amount of ammo in all ammo PickUps.\n")
    TEXT("<=0: off\n"),
    ECVF_Scalability | ECVF_RenderThreadSafe);
#endif

UCLASS()
class LEVELUP_API AAmmoPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickUp();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickUp(ALevelUpCharacter* PickUpCharacter);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UTP_PickUpComponent* PickUpComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	URotatingMovementComponent* RotatingMovement;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ammo)
	int32 AmountOfAmmo = 5;
};
