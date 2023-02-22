#include "Actions/ColorThemeActions.h"
#include "ToolKit/ColorThemeEditorToolkit.h"
#include "ColorThemeAsset.h"

UClass* FColorThemeAssetTypeActions::GetSupportedClass() const
{
	return UColorThemeAsset::StaticClass();
}

FText FColorThemeAssetTypeActions::GetName() const
{
	return INVTEXT("Color Theme");
}

FColor FColorThemeAssetTypeActions::GetTypeColor() const
{
	return FColor::Orange;
}

uint32 FColorThemeAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

void FColorThemeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	MakeShared<FColorThemeEditorToolkit>()->InitEditor(InObjects);
}
