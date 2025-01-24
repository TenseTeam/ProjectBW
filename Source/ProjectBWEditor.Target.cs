// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectBWEditorTarget : TargetRules
{
	public ProjectBWEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
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
	// 	ExtraModuleNames.AddRange(new string[] { "LGUEDK" });
	// }
}
