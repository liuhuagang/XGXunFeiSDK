// Copyright 2024 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class XunFeiSDKProjectTarget : TargetRules
{
	public XunFeiSDKProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("XunFeiSDKProject");
	}
}
