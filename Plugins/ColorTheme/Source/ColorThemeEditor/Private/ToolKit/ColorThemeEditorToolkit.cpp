#include "ToolKit/ColorThemeEditorToolkit.h"
#include "ToolKit/SColorThemePicker.h"
#include "Widgets/Docking/SDockTab.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Colors/SColorGradingWheel.h"

void FColorThemeEditorToolkit::InitEditor(const TArray<UObject*>& InObjects)
{
	ColorTheme = Cast<UColorThemeAsset>(InObjects[0]);
	// clang-format off
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("ColorThemeEditorLayout")->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split
		(
			FTabManager::NewSplitter()->SetSizeCoefficient(0.6f)->SetOrientation(Orient_Horizontal)->Split
			(
				FTabManager::NewStack()->SetSizeCoefficient(0.8f)->AddTab("ColorThemeTab", ETabState::OpenedTab)
			)
		)
	);
	// clang-format on
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "ColorThemeEditor", Layout, true, true, InObjects);
}

void FColorThemeEditorToolkit::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Color Theme Editor"));

	InTabManager->RegisterTabSpawner("ColorThemeTab", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	{
		float ColorGradingWheelExponent = 2.4f;
		// clang-format off
		return SNew(SDockTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SColorThemePicker)
					.OnColorCommitted(FOnLinearColorValueChanged::CreateSP(this, &FColorThemeEditorToolkit::OnSetColorFromColorPicker))
					.TargetColorAttribute(ColorTheme->Color)
				
			]
		];
		// clang-format on
	}))
	.SetDisplayName(INVTEXT("ColorTheme"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FColorThemeEditorToolkit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("ColorThemeTab");
}

void FColorThemeEditorToolkit::OnSetColorFromColorPicker(FLinearColor NewColor)
{
	ColorTheme->Modify();
	ColorTheme->Color = NewColor;
}