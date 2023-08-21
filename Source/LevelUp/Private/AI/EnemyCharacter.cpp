// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI/EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include <GameplayEffectTypes.h>
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "LevelUpAttributeSet.h"
#include "LevelUpGameplayAbility.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<ULevelUpAttributeSet>(TEXT("CommonAttributeSet"));

	USkeletalMeshComponent* SklMesh = GetMesh();
	SklMesh->SetCollisionProfileName("IgnoreOnlyPawn");
	SklMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

FGenericTeamId AEnemyCharacter::GetGenericTeamId() const
{
	if (AAIController* AICtr = Cast<AAIController>(Controller))
	{
		return AICtr->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
	if (HasAuthority())
	{
		AttributeSet->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDeath);
	}
}

 UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		for (TSubclassOf<UGameplayEffect> Effect : DefaultAttributeEffect)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void AEnemyCharacter::InitializeAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (int32 i = 0; i < DefaultAbilities.Num(); ++i)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbilities[i], 1, i, this));
		}
	}
}

void AEnemyCharacter::OnDeath()
{
	if (IsValid(Controller))
	{
		Controller->UnPossess();
	}
	GetCapsuleComponent()->DestroyComponent();
	USkeletalMeshComponent* SklMesh = GetMesh();
	SklMesh->SetSimulatePhysics(true);
	SklMesh->AddImpulseAtLocation(LastReceivedDamage.Value, LastReceivedDamage.Key);
}

void AEnemyCharacter::ReceiveDamage(const FVector& Location, const FVector& Velocity)
{
	LastReceivedDamage.Key = Location;
	LastReceivedDamage.Value = Velocity;
}
