#pragma once

#include "CoreMinimal.h"
#include "ColorThemeAsset.h"
#include "Toolkits/AssetEditorToolkit.h"

class FColorThemeEditorToolkit : public FAssetEditorToolkit
{
public:
	void InitEditor(const TArray<UObject*>& InObjects);

	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	FName GetToolkitFName() const override { return "ColorThemeEditor"; }
	FText GetBaseToolkitName() const override { return INVTEXT("Color Theme Editor"); }
	FString GetWorldCentricTabPrefix() const override { return "Color Theme"; }
	FLinearColor GetWorldCentricTabColorScale() const override { return {}; }


protected:
	void OnSetColorFromColorPicker(FLinearColor NewColor);

private:
	UColorThemeAsset* ColorTheme;
};