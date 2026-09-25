// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IllusiRunner : ModuleRules
{
	public IllusiRunner(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"Slate",
            "Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"IllusiRunner",
			"IllusiRunner/Variant_Platforming",
			"IllusiRunner/Variant_Platforming/Animation",
			"IllusiRunner/Variant_Combat",
			"IllusiRunner/Variant_Combat/AI",
			"IllusiRunner/Variant_Combat/Animation",
			"IllusiRunner/Variant_Combat/Gameplay",
			"IllusiRunner/Variant_Combat/Interfaces",
			"IllusiRunner/Variant_Combat/UI",
			"IllusiRunner/Variant_SideScrolling",
			"IllusiRunner/Variant_SideScrolling/AI",
			"IllusiRunner/Variant_SideScrolling/Gameplay",
			"IllusiRunner/Variant_SideScrolling/Interfaces",
			"IllusiRunner/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
