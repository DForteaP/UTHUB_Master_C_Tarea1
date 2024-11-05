// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tarea1 : ModuleRules
{
	public Tarea1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
