// Copyright Epic Games, Inc. All Rights Reserved.
#include "Containers/Array.h"
#include "IToolInterface.h"

class UColorThemeAsset;

class FColorThemeTool : public IToolInterface, public TSharedFromThis<FColorThemeTool>
{
public:

	void OnStartupModule() override;
	void OnShutdownModule() override;

	void AddMenuExtension(const FMenuExtensionDelegate& extensionDelegate,
						  FName extensionHook,
						  const TSharedPtr<FUICommandList>& CommandList = NULL,
						  EExtensionHook::Position position = EExtensionHook::Before);

protected:
	void BindCommands();

	void MakePulldownMenu(FMenuBarBuilder& menuBuilder);
	void FillPulldownMenu(FMenuBuilder& menuBuilder);

	void OpenColorThemeEditorWindow();
	TSharedRef<SDockTab> SpawnColorThemeEditorTab(const FSpawnTabArgs& TabSpawnArgs);
	
	static bool IsColorThemeEditorAvailable();
	static UColorThemeAsset* GetSelectedColorThemeAsset();

private:
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;
	TSharedPtr<FExtender> MenuExtender;
	TSharedPtr<FUICommandList> ColorThemeCommandList;

	static TSharedRef<FWorkspaceItem> MenuRoot;

	TWeakPtr<SWindow> ColorThemeWindow;
};

