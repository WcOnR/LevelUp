// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

class AHUD;
class SWeaponInterfaceWidget;

class LEVELUP_API SGameHUDWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SGameHUDWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnHUD)
	SLATE_ARGUMENT(FSlateBrush, CrosshairImg)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	bool SupportsKeyboardFocus() const override { return false; }

protected:
	void ShowWeaponInterface();

protected:
	TWeakObjectPtr<AHUD> OwnHUD;

	TSharedPtr<SOverlay> MainOverlay;

	FSlateBrush CrosshairBrush;
	TSharedPtr<SWeaponInterfaceWidget> WeaponInterface;
};
