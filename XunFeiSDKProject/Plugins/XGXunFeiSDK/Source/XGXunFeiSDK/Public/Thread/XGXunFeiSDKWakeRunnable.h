// Copyright 2024 Xiao Gang. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
class FXGXunFeiSDKWakeupRunnable :public FRunnable
{

public:

	FXGXunFeiSDKWakeupRunnable(FString InThreadName,FString InAppID);

	virtual ~FXGXunFeiSDKWakeupRunnable();

	virtual bool Init() override;

	virtual uint32 Run() override;
	
	virtual void Stop() override;
	
	virtual void Exit() override;

	FString ThreadName;

protected:

	static int WakeUpCallBack(const char* sessionID, int msg, int param1, int param2, const void* info, void* userData);

	void RunListen(const char* grammar_list, const char* session_begin_params);

	static void PrintLog(FString InMessage);

	static void PrintDisplay(FString InMessage);

	static void PrintWarning(FString InMessage);

	static void PrintError(FString InMessage);
private:

	FString AppID=TEXT("");
	FString WorkBin = TEXT("");

	static FThreadSafeBool bIsRunning;

	static int32 WakeupFlage ;


};
