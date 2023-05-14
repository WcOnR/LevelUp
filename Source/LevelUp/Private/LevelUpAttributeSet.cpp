// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LevelUpCharacter.h"

ULevelUpAttributeSet::ULevelUpAttributeSet()
{
}

void ULevelUpAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULevelUpAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULevelUpAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULevelUpAttributeSet, MovingSpeed, COND_None, REPNOTIFY_Always);
}

void ULevelUpAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		float OldValue = GetHealth();
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, GetMaxHealth()));
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, GetMaxHealth()));
		if (!FMath::IsNearlyEqual(OldValue, NewValue) && FMath::IsNearlyZero(NewValue))
		{
			OnDeath.Broadcast();
		}
	}
}

void ULevelUpAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMovingSpeedAttribute())
	{
		if (ALevelUpCharacter* TargetCharacter = Cast<ALevelUpCharacter>(GetOwningActor()))
		{
			UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(TargetCharacter->GetMovementComponent());
			Movement->MaxWalkSpeed = NewValue;
		}
	}
	else if (!FMath::IsNearlyEqual(OldValue, NewValue) && Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast();
	}
}

GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(ULevelUpAttributeSet, MaxHealth)
GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(ULevelUpAttributeSet, Health)
GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(ULevelUpAttributeSet, MovingSpeed)

