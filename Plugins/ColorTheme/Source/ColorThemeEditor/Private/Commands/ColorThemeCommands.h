// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/InputChord.h"

#define LOCTEXT_NAMESPACE "ColorThemeCommands"

class FColorThemeCommands : public TCommands<FColorThemeCommands>
{
public:
	FColorThemeCommands()
		: TCommands<FColorThemeCommands>(
			  FName("ColorThemeCommands"), FText::FromString("Color Theme Tool"), NAME_None, FCoreStyle::Get().GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(OpenColorThemeEditorWindow,
				   "Open Color Theme Editor Window",
				   "Open the ColorTheme editor window",
				   EUserInterfaceActionType::Button,
				   FInputGesture());
	}

	TSharedPtr<FUICommandInfo> OpenColorThemeEditorWindow;
};

#undef LOCTEXT_NAMESPACE
