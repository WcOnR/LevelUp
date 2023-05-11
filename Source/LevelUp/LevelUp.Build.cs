// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LevelUp : ModuleRules
{
	public LevelUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput" 
		});
		
		PrivateDependencyModuleNames.AddRange(new string[] { 
			"SlateCore", 
			"Slate", 
			"UMG", 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"OnlineSubsystem",
			"OnlineSubsystemSteam"
		});
	}
}
