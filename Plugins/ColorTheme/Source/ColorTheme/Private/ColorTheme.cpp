// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FColorThemeModule"

class FColorThemeModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FColorThemeModule, ColorTheme)