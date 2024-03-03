// Copyright 2024 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class XunFeiSDKProjectEditorTarget : TargetRules
{
	public XunFeiSDKProjectEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("XunFeiSDKProject");
	}
}
