// Copyright Epic Games, Inc. All Rights Reserved.
#include "ColorThemeEditor.h"

#include "Tools/ColorThemeTool.h"
#include "Tools/ColorThemeAssetTool.h"

#define LOCTEXT_NAMESPACE "FColorThemeEditorModule"

void FColorThemeEditorModule::StartupModule() 
{ 
	if (!GIsEditor || IsRunningCommandlet())
	{
		return; 
	}
	CreateTools();
	for (TSharedPtr<IToolInterface> Tool : Tools)
	{
		Tool->OnStartupModule();
	}
}

void FColorThemeEditorModule::ShutdownModule()
{
	Algo::Reverse(Tools);
	for (TSharedPtr<IToolInterface> Tool : Tools)
	{
		Tool->OnShutdownModule();
	}
	Tools.Empty();
}

void FColorThemeEditorModule::CreateTools()
{
	Tools.Add(MakeShared<FColorThemeTool>());
	Tools.Add(MakeShared<FColorThemeAssetTool>());
}

#undef LOCTEXT_NAMESPACE
