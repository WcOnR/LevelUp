// Copyright Epic Games, Inc. All Rights Reserved.
#include "ColorThemeAssetTool.h"

#include "Actions/ColorThemeActions.h"

#define LOCTEXT_NAMESPACE "FColorThemeAssetTool"

void FColorThemeAssetTool::OnStartupModule()
{
	ColorThemeActions = MakeShared<FColorThemeAssetTypeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ColorThemeActions.ToSharedRef());
}

void FColorThemeAssetTool::OnShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		return;
	}
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(ColorThemeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
