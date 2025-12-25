// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZBeta : ModuleRules
{
	public ZBeta(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"GameplayCameras",
			"UMG",
			"Slate", 
			"GameplayAbilities"
		
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTags","GameplayTasks","NavigationSystem","Niagara"
		});

		PublicIncludePaths.AddRange(new string[] {
			"ZBeta",
			"ZBeta/Variant_Platforming",
			"ZBeta/Variant_Platforming/Animation",
			"ZBeta/Variant_Combat",
			"ZBeta/Variant_Combat/AI",
			"ZBeta/Variant_Combat/Animation",
			"ZBeta/Variant_Combat/Gameplay",
			"ZBeta/Variant_Combat/Interfaces",
			"ZBeta/Variant_Combat/UI",
			"ZBeta/Variant_SideScrolling",
			"ZBeta/Variant_SideScrolling/AI",
			"ZBeta/Variant_SideScrolling/Gameplay",
			"ZBeta/Variant_SideScrolling/Interfaces",
			"ZBeta/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
