// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Containers/Array.h"
#include "IToolInterface.h"

class FColorThemeAssetTypeActions;

class FColorThemeAssetTool : public IToolInterface
{
public:

	void OnStartupModule() override;
	void OnShutdownModule() override;

private:
	TSharedPtr<FColorThemeAssetTypeActions> ColorThemeActions;
};

