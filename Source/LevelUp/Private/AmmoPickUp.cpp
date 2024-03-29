// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickUp.h"

#include "TP_PickUpComponent.h"
#include "LevelUpCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
AAmmoPickUp::AAmmoPickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpComponent = CreateDefaultSubobject<UTP_PickUpComponent>(TEXT("PickUpComponent"));
	SetRootComponent(PickUpComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(PickUpComponent);
	
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}

void AAmmoPickUp::BeginPlay()
{
	PickUpComponent->OnPickUp.AddDynamic(this, &AAmmoPickUp::OnPickUp);
	Super::BeginPlay();
}

void AAmmoPickUp::OnPickUp(ALevelUpCharacter* PickUpCharacter)
{
	if (!IsValid(PickUpCharacter))
	{
		return;
	}
	if (UAbilitySystemComponent* AbilitySystemComponent = PickUpCharacter->FindComponentByClass<UAbilitySystemComponent>())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PickUpEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			int32 CheatAmmoValue = 0;
	#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			CheatAmmoValue = CVarAmmoPickUpAmount.GetValueOnGameThread();
	#endif
			SpecHandle.Data->SetSetByCallerMagnitude(PickUpAmmoTag, CheatAmmoValue > 0 ? CheatAmmoValue : AmountOfAmmo);
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	Destroy();
}
