// Copyright 2024 Xiao Gang. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class XGXunFeiSDKLibrary : ModuleRules
{
	private void CopyDllToPluginBinaries(string InFilePath, ReadOnlyTargetRules Target)
	{
		string TargetDirectory = Path.GetFullPath(Path.Combine(PluginDirectory, "Binaries/ThirdParty", "XGXunFeiSDKLibrary", Target.Platform.ToString()));

        string FileName = Path.GetFileName(InFilePath);

        Console.WriteLine("--SourcFilePath:" + InFilePath);

        Console.WriteLine("--TargetDirectory:" + TargetDirectory + "--FileName:" + FileName);

        if (!Directory.Exists(TargetDirectory))
        {

            Directory.CreateDirectory(TargetDirectory);

        }

        string TargetFilePath = Path.Combine(TargetDirectory, FileName);

        if (!File.Exists(TargetFilePath))
        {
            File.Copy(InFilePath, TargetFilePath, true);
        }


        RuntimeDependencies.Add(TargetFilePath);

    }
    private void CopyWakeupResourceToPluginBinaries(string InFilePath, ReadOnlyTargetRules Target)
    {
        string TargetDirectory = Path.GetFullPath(Path.Combine(PluginDirectory, "Binaries/ThirdParty", "XGXunFeiSDKLibrary", Target.Platform.ToString(), "msc/res/ivw"));

        string FileName = Path.GetFileName(InFilePath);

        Console.WriteLine("--SourcFilePath:" + InFilePath);

        Console.WriteLine("--TargetDirectory:" + TargetDirectory + "--FileName:" + FileName);

        if (!Directory.Exists(TargetDirectory))
        {
            Directory.CreateDirectory(TargetDirectory);
        }

        string TargetFilePath = Path.Combine(TargetDirectory, FileName);

        if (!File.Exists(TargetFilePath))
        {
            File.Copy(InFilePath, TargetFilePath, true);
        }


        RuntimeDependencies.Add(TargetFilePath);

    }



    public XGXunFeiSDKLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
        Type = ModuleType.External;

/*        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
            }
        );*/

        PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
	        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "msc_x64.lib"));

            PublicDelayLoadDLLs.Add("msc_x64.dll");

            CopyDllToPluginBinaries(Path.Combine(ModuleDirectory, "x64", "Release", "msc_x64.dll"), Target);

            CopyWakeupResourceToPluginBinaries(Path.Combine(ModuleDirectory, "x64", "Release", "wakeupresource.jet"), Target);
        }



	}
}
