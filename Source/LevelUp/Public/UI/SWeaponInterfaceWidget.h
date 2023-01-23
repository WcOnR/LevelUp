// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

class AHUD;

class LEVELUP_API SWeaponInterfaceWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SWeaponInterfaceWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnHUD)
	SLATE_ARGUMENT(FSlateBrush, CrosshairImg)
	SLATE_END_ARGS()

	SWeaponInterfaceWidget();

	void Construct(const FArguments& InArgs);
	bool SupportsKeyboardFocus() const override { return false; }

protected:
	void UpdateAmmoInMag(int32 CurrentAmmo);
	void UpdateAmmoInBag(int32 CurrentAmmo);
	void UpdateAmmoText();

protected:
	TWeakObjectPtr<AHUD> OwnHUD;

	FSlateBrush CrosshairBrush;
	TSharedPtr<STextBlock> AmmoText;

	int32 AmmoInMag;
	int32 AmmoInBag;
};
