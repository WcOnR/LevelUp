// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	bIsDead = false;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (IsValid(Owner))
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeAnyDamage);
	}
}

void UHealthComponent::TakeAnyDamage(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast();
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		bIsDead = true;
		OnDeath.Broadcast();
		if (DestroyOwnerOnDeath)
		{
			GetOwner()->Destroy();
		}
	}
}
