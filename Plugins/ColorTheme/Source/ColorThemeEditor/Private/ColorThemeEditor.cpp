// Copyright Epic Games, Inc. All Rights Reserved.
#include "Containers/Array.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

#include "Actions/ColorThemeActions.h"

#define LOCTEXT_NAMESPACE "FColorThemeEditorModule"

class FColorThemeEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override 
	{ 
		//
		RegisterAssetTypeActions();
	}

	virtual void ShutdownModule() override
	{
		//
		UnregisterAssetTypeActions();
	}

	virtual bool SupportsDynamicReloading() override { return true; }

protected:
	void RegisterAssetTypeActions()
	{
		ColorThemeActions = MakeShared<FColorThemeAssetTypeActions>();
		FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ColorThemeActions.ToSharedRef());
	}

	void UnregisterAssetTypeActions()
	{
		if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			return;
		}
		FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(ColorThemeActions.ToSharedRef());
	}

private:
	TSharedPtr<FColorThemeAssetTypeActions> ColorThemeActions;
};

IMPLEMENT_MODULE(FColorThemeEditorModule, ColorThemeEditor);

#undef LOCTEXT_NAMESPACE
