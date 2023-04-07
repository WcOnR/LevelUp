// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadGEExeCalculation.h"

#include "WeaponAttributeSet.h"

struct FAmmoStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxAmmoInMag);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AmmoInMag);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AmountOfAmmo);

	FAmmoStatic() 
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, MaxAmmoInMag, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, AmmoInMag, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWeaponAttributeSet, AmountOfAmmo, Target, false);
	}
};
static const FAmmoStatic& AmmoStatic()
{
	static FAmmoStatic Ammo;
	return Ammo;
}

UReloadGEExeCalculation::UReloadGEExeCalculation()
{
	RelevantAttributesToCapture.Add(AmmoStatic().MaxAmmoInMagDef);
	RelevantAttributesToCapture.Add(AmmoStatic().AmmoInMagDef);
	RelevantAttributesToCapture.Add(AmmoStatic().AmountOfAmmoDef);
}

void UReloadGEExeCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
													 FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float MaxAmmoInMag = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AmmoStatic().MaxAmmoInMagDef, EvaluateParameters, MaxAmmoInMag);

	float AmmoInMagDef = 0.0f;
	float AmmoInMagOld = 0.0f;
	float AmountOfAmmoDef = 0.0f;
	float AmountOfAmmoOld = 0.0f;
	for (const auto& ModifiedAttr : Spec.ModifiedAttributes)
	{
		if (ModifiedAttr.Attribute == UWeaponAttributeSet::GetAmmoInMagAttribute())
		{
			AmmoInMagDef = ModifiedAttr.TotalMagnitude;
			float AmmoInMag = 0.0f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AmmoStatic().AmmoInMagDef, EvaluateParameters, AmmoInMag);
			AmmoInMagOld = AmmoInMag - AmmoInMagDef;
		}
		else if (ModifiedAttr.Attribute == UWeaponAttributeSet::GetAmountOfAmmoAttribute())
		{
			AmountOfAmmoDef = ModifiedAttr.TotalMagnitude;
			float AmountOfAmmo = 0.0f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AmmoStatic().AmountOfAmmoDef, EvaluateParameters, AmountOfAmmo);
			AmountOfAmmoOld = AmountOfAmmo - AmountOfAmmoDef;
		}
	}
	float AmmoToAdd = FMath::Min(AmountOfAmmoOld, MaxAmmoInMag - AmmoInMagOld);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(AmmoStatic().AmmoInMagProperty, EGameplayModOp::Override, AmmoInMagOld + AmmoToAdd));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(AmmoStatic().AmountOfAmmoProperty, EGameplayModOp::Override, AmountOfAmmoOld - AmmoToAdd));

}
