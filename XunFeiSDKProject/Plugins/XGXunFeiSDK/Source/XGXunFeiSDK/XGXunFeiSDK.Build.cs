// Copyright 2024 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGXunFeiSDK : ModuleRules
{
	public XGXunFeiSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "$(ModuleDir)/Public/Type",
                "$(ModuleDir)/Public/Subsystem",
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "XGXunFeiSDK/Public/Log",
                "XGXunFeiSDK/Public/Subsystem",
                "XGXunFeiSDK/Public/Thread",
                "XGXunFeiSDK/Public/Type"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"XGXunFeiSDKLibrary",
				"Projects"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Json",
                "JsonUtilities",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
			);
	}
}
