// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Containers/Array.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IToolInterface;

class FColorThemeEditorModule : public IModuleInterface, public TSharedFromThis<FColorThemeEditorModule>
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override { return true; }

protected:
	void CreateTools();

private:
	TArray<TSharedPtr<IToolInterface>> Tools;
};

IMPLEMENT_MODULE(FColorThemeEditorModule, ColorThemeEditor);

