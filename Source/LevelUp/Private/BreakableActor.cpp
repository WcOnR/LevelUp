// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"
#include "AbilitySystemComponent.h"
#include "LevelUpAttributeSet.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	LevelUpAttributeSet = CreateDefaultSubobject<ULevelUpAttributeSet>(TEXT("CommonAttributeSet"));

	HealthPercentage = 1.0f;
}

void ABreakableActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABreakableActor, HealthPercentage);
}

void ABreakableActor::InitializeAttributes()
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (const TSubclassOf<UGameplayEffect>& AttributeEffect : DefaultAttributeEffects)
		{
			if (AttributeEffect)
			{
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributeEffect, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
	if (HasAuthority())
	{
		LevelUpAttributeSet->OnDeath.AddDynamic(this, &ABreakableActor::OnDeath);
		LevelUpAttributeSet->OnHealthChanged.AddDynamic(this, &ABreakableActor::OnHealthChanged);
	}

	if (IsValid(HealthMaterial))
	{
		DynamicHealthMaterial = UMaterialInstanceDynamic::Create(HealthMaterial, this);
		UpdateHealthMaterial();
	}
}

UAbilitySystemComponent* ABreakableActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABreakableActor::UpdateHealthMaterial()
{
	if (IsValid(DynamicHealthMaterial))
	{
		DynamicHealthMaterial->SetScalarParameterValue("Percentage", HealthPercentage);
		Mesh->SetMaterial(0, DynamicHealthMaterial);
	}
}

void ABreakableActor::OnHealthChanged()
{
	HealthPercentage = 1.0f;
	if (LevelUpAttributeSet->GetMaxHealth() > 0.0f)
	{
		HealthPercentage = LevelUpAttributeSet->GetHealth() / LevelUpAttributeSet->GetMaxHealth();
	}
	OnRep_HealthPercentage();
}

void ABreakableActor::OnDeath()
{
	Destroy();
}

void ABreakableActor::OnRep_HealthPercentage()
{
	UpdateHealthMaterial();
}
