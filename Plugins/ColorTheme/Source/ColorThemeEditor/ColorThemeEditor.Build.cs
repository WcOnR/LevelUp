// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ColorThemeEditor : ModuleRules
{
	public ColorThemeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"MainFrame",
			});

		PrivateIncludePaths.AddRange(
			new string[] {
				"ColorThemeEditor/Private",
			});

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"ContentBrowser",
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ColorTheme",
				"UnrealEd",
				"EditorStyle",
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Engine",
				"AssetTools",
				"UnrealEd",
			});
	}
}
