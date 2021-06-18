// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Module5Proj : ModuleRules
{
	public Module5Proj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
