// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

class AMainGameHUD;

class LEVELUP_API SPauseMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPauseMenuWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<AMainGameHUD>, OwnHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	bool SupportsKeyboardFocus() const override { return true; }

protected:
	FReply OnResume() const;
	FReply OnQuit() const;

protected:
	TWeakObjectPtr<AMainGameHUD> OwnHUD;
};
