// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UMMOTemplate : ModuleRules
{
	public UMMOTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","Sockets"
            //"OnlineSubsystem"
            //"Networking", "Sockets"
        });
	}
}
