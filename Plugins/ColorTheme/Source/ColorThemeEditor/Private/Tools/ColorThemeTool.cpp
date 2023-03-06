// Copyright Epic Games, Inc. All Rights Reserved.
#include "ColorThemeTool.h"

#include "LevelEditor.h"
#include "Commands/ColorThemeCommands.h"
#include "Widgets/Colors/SColorBlock.h"
#include "ToolKit/SColorThemeApplyer.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ColorThemeAsset.h"

#define LOCTEXT_NAMESPACE "FColorThemeTool"

TSharedRef<FWorkspaceItem> FColorThemeTool::MenuRoot = FWorkspaceItem::NewGroup(FText::FromString("Menu Root"));
static const FName ColorThemeTabName("ColorThemeEditorTab");

void FColorThemeTool::OnStartupModule()
{
	FColorThemeCommands::Register();
	ColorThemeCommandList = MakeShared<FUICommandList>();
	BindCommands();
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
	MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuBarExtension("Tools",
									  EExtensionHook::After,
									  ColorThemeCommandList,
									  FMenuBarExtensionDelegate::CreateRaw(this, &FColorThemeTool::MakePulldownMenu));
	LevelEditorMenuExtensibilityManager->AddExtender(MenuExtender);

	FGlobalTabmanager::Get()
		->RegisterNomadTabSpawner(
			ColorThemeTabName,
			FOnSpawnTab::CreateRaw(this, &FColorThemeTool::SpawnColorThemeEditorTab),
			FCanSpawnTab::CreateLambda([](const FSpawnTabArgs&) -> bool { return FColorThemeTool::IsColorThemeEditorAvailable(); })
		)
		.SetGroup(MenuRoot)
		.SetDisplayName(FText::FromString("Open Color Theme Editor Tab"))
		.SetTooltipText(FText::FromString("Open the ColorTheme editor tab"));
}

void FColorThemeTool::OnShutdownModule()
{
	FColorThemeCommands::Unregister();
}

void FColorThemeTool::AddMenuExtension(const FMenuExtensionDelegate& extensionDelegate,
											   FName extensionHook,
											   const TSharedPtr<FUICommandList>& CommandList,
											   EExtensionHook::Position position)
{
	MenuExtender->AddMenuExtension(extensionHook, position, CommandList, extensionDelegate);
}

void FColorThemeTool::BindCommands()
{
	const FColorThemeCommands& Commands = FColorThemeCommands::Get();

	ColorThemeCommandList->MapAction(
		Commands.OpenColorThemeEditorWindow,
		FExecuteAction::CreateSP(this, &FColorThemeTool::OpenColorThemeEditorWindow),
		FCanExecuteAction::CreateLambda([]() -> bool { return FColorThemeTool::IsColorThemeEditorAvailable(); }));
}

void FColorThemeTool::MakePulldownMenu(FMenuBarBuilder& menuBuilder)
{
	menuBuilder.AddPullDownMenu(FText::FromString("ColorTheme"),
								FText::FromString("Open the Color Theme menu"),
								FNewMenuDelegate::CreateRaw(this, &FColorThemeTool::FillPulldownMenu),
								"ColorTheme",
								FName(TEXT("ColorThemeMenu")));
}
void FColorThemeTool::FillPulldownMenu(FMenuBuilder& menuBuilder)
{
	const FColorThemeCommands& Commands = FColorThemeCommands::Get();
	menuBuilder.BeginSection("ColorThemeSection", FText::FromString("General"));
	menuBuilder.AddMenuEntry(Commands.OpenColorThemeEditorWindow);
	FGlobalTabmanager::Get()->PopulateTabSpawnerMenu(menuBuilder, ColorThemeTabName);
	menuBuilder.EndSection();
}

void FColorThemeTool::OpenColorThemeEditorWindow()
{
	if (ColorThemeWindow.IsValid())
	{
		ColorThemeWindow.Pin()->RequestDestroyWindow();
		ColorThemeWindow.Reset();
	}
	TSharedPtr<SWindow> Window = nullptr;

	FVector2D CursorPos = FSlateApplication::Get().GetCursorPos();
	FSlateRect Anchor(CursorPos.X, CursorPos.Y, CursorPos.X, CursorPos.Y);
	FVector2D AdjustedSummonLocation = FSlateApplication::Get().CalculatePopupWindowPosition(Anchor, FVector2D(), true, FVector2D::ZeroVector, Orient_Horizontal);
	FLinearColor TargetColor = FLinearColor::Black;
	if (UColorThemeAsset* ColorTheme = GetSelectedColorThemeAsset())
	{
		TargetColor = ColorTheme->Color;
	}

	// clang-format off
	Window = SNew(SWindow)
			.AutoCenter(EAutoCenter::None)
			.ScreenPosition(AdjustedSummonLocation)
			.SupportsMaximize(false)
			.SupportsMinimize(false)
			.SizingRule(ESizingRule::Autosized)
			.Title(LOCTEXT("WindowHeader", "Color Theme"))
			[
				SNew(SColorThemeApplyer)
				.Color(TargetColor)
			];
	// clang-format on
	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	ColorThemeWindow = Window;
}

TSharedRef<SDockTab> FColorThemeTool::SpawnColorThemeEditorTab(const FSpawnTabArgs& TabSpawnArgs)
{
	FLinearColor TargetColor = FLinearColor::Black;
	if (UColorThemeAsset* ColorTheme = GetSelectedColorThemeAsset())
	{
		TargetColor = ColorTheme->Color;
	}
	// clang-format off
	return SNew(SDockTab)
			.Label(FText::FromString("Color Theme"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SColorThemeApplyer)
					.Color(TargetColor)
				]
			];
	// clang-format on
}

bool FColorThemeTool::IsColorThemeEditorAvailable()
{
	return IsValid(GetSelectedColorThemeAsset());
}

UColorThemeAsset* FColorThemeTool::GetSelectedColorThemeAsset()
{
	TArray<FAssetData> OutSelectedAssets;
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();
	ContentBrowserSingleton.GetSelectedAssets(OutSelectedAssets);
	FText ToolTipText = LOCTEXT("SelectedColorToolTip", "No color theme selected");

	if (OutSelectedAssets.Num())
	{
		return Cast<UColorThemeAsset>(OutSelectedAssets[0].GetAsset());
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
