// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectBWTarget : TargetRules
{
	public ProjectBWTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		
		ExtraModuleNames.AddRange( new string[]
		{
			"ProjectBW",
			"VUNDK",
			"GVUEDK",
			"LGUEDK"
		} );
		// RegisterModulesCreatedByRider();
	}

	// private void RegisterModulesCreatedByRider()
	// {
	// 		ExtraModuleNames.AddRange(new string[] { "LGUEDK" });
	// }
}
