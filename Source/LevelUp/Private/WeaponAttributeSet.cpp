// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeaponAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"

UWeaponAttributeSet::UWeaponAttributeSet()
{
}

void UWeaponAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, ProjectileDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, MaxAmmoInMag, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, AmmoInMag, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, AmountOfAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, MaxTemperatureCapacity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWeaponAttributeSet, TemperatureCapacity, COND_None, REPNOTIFY_Always);
}

void UWeaponAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (!FMath::IsNearlyEqual(OldValue, NewValue))
	{
		if (Attribute == GetAmmoInMagAttribute())
		{
			OnAmmoInMagChanged.Broadcast(static_cast<int32>(NewValue));
		}
		else if (Attribute == GetAmountOfAmmoAttribute())
		{
			OnAmountOfAmmoChanged.Broadcast(static_cast<int32>(NewValue));
		}
	}
}

void UWeaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetTemperatureCapacityAttribute())
	{
		float OldValue = GetTemperatureCapacity();
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxTemperatureCapacity());
		TemperatureCapacity.SetBaseValue(FMath::Clamp(TemperatureCapacity.GetBaseValue(), 0.0f, GetMaxTemperatureCapacity()));
		TemperatureCapacity.SetCurrentValue(FMath::Clamp(TemperatureCapacity.GetCurrentValue(), 0.0f, GetTemperatureCapacity()));
	}
}

GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(UWeaponAttributeSet, MaxAmmoInMag)
GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(UWeaponAttributeSet, MaxTemperatureCapacity)
GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(UWeaponAttributeSet, TemperatureCapacity)
GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(UWeaponAttributeSet, ProjectileDamage)

GAMEPLAYATTRIBUTE_VALUE_SETTER_IMPL(UWeaponAttributeSet, AmmoInMag)
void UWeaponAttributeSet::OnRep_AmmoInMag(const FGameplayAttributeData& OldAmmoInMag)
{
	if (!FMath::IsNearlyEqual(AmmoInMag.GetCurrentValue(), OldAmmoInMag.GetCurrentValue()))
	{
		OnAmmoInMagChanged.Broadcast(static_cast<int32>(AmmoInMag.GetCurrentValue()));
	}
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, AmmoInMag, OldAmmoInMag);
}

GAMEPLAYATTRIBUTE_VALUE_SETTER_IMPL(UWeaponAttributeSet, AmountOfAmmo)
void UWeaponAttributeSet::OnRep_AmountOfAmmo(const FGameplayAttributeData& OldAmountOfAmmo)
{
	if (!FMath::IsNearlyEqual(AmountOfAmmo.GetCurrentValue(), OldAmountOfAmmo.GetCurrentValue()))
	{
		OnAmountOfAmmoChanged.Broadcast(static_cast<int32>(AmountOfAmmo.GetCurrentValue()));
	}
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWeaponAttributeSet, AmountOfAmmo, OldAmountOfAmmo);
}

