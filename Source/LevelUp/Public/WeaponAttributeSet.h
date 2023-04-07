// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AttributeSetMacro.h"

#include "WeaponAttributeSet.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);

UCLASS()
class LEVELUP_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWeaponAttributeSet();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

public:
	FOnAmmoChanged OnAmmoInMagChanged;
	FOnAmmoChanged OnAmountOfAmmoChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxAmmoInMag)
	FGameplayAttributeData MaxAmmoInMag;
	LEVELUP_ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, MaxAmmoInMag);
	UFUNCTION()
	virtual void OnRep_MaxAmmoInMag(const FGameplayAttributeData& OldMaxAmmoInMag);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AmmoInMag)
	FGameplayAttributeData AmmoInMag;
	LEVELUP_ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, AmmoInMag);
	UFUNCTION()
	virtual void OnRep_AmmoInMag(const FGameplayAttributeData& OldAmmoInMag);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AmountOfAmmo)
	FGameplayAttributeData AmountOfAmmo;
	LEVELUP_ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, AmountOfAmmo);
	UFUNCTION()
	virtual void OnRep_AmountOfAmmo(const FGameplayAttributeData& OldAmountOfAmmo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_ProjectileDamage)
	FGameplayAttributeData ProjectileDamage;
	LEVELUP_ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, ProjectileDamage);
	UFUNCTION()
	virtual void OnRep_ProjectileDamage(const FGameplayAttributeData& OldProjectileDamage);
};