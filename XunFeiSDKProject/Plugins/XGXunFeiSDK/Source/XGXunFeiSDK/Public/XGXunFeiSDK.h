// Copyright 2024 Xiao Gang. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FXGXunFeiSDKModule : public IModuleInterface
{

public:


	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

public:
	
	static FString MSC_64_LibraryPath;

	static FString WakeupBinPath;

private:

	void* MSC_64_LibraryHandle;

};
