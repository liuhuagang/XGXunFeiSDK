// Copyright 2024 Xiao Gang. All Rights Reserved.

#include "XGXunFeiSDK.h"

#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"


#define LOCTEXT_NAMESPACE "FXGXunFeiSDKModule"

FString FXGXunFeiSDKModule::MSC_64_LibraryPath =TEXT("");

FString FXGXunFeiSDKModule::WakeupBinPath = TEXT("");

void FXGXunFeiSDKModule::StartupModule()
{

	FString BaseDir = IPluginManager::Get().FindPlugin("XGXunFeiSDK")->GetBaseDir();



#if PLATFORM_WINDOWS

	MSC_64_LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/XGXunFeiSDKLibrary/Win64/msc_x64.dll"));

	WakeupBinPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/XGXunFeiSDKLibrary/Win64/"));

#endif // PLATFORM_WINDOWS

	MSC_64_LibraryHandle = !MSC_64_LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*MSC_64_LibraryPath) : nullptr;

}

void FXGXunFeiSDKModule::ShutdownModule()
{

	FPlatformProcess::FreeDllHandle(MSC_64_LibraryHandle);
	MSC_64_LibraryHandle = nullptr;

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXGXunFeiSDKModule, XGXunFeiSDK)
