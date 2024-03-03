// Copyright 2024 Xiao Gang. All Rights Reserved.

#include "XGXunFeiSDKWakeupSubsystem.h"

#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "LogXGXunFeiSDK.h"
#include "XGXunFeiSDKWakeRunnable.h"

UXGXunFeiSDKWakeupSubsystem* UXGXunFeiSDKWakeupSubsystem::XunFeiSDKWakeupSubsystem = nullptr;
FThreadSafeBool UXGXunFeiSDKWakeupSubsystem::bListenForWakeup = false;
bool UXGXunFeiSDKWakeupSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UXGXunFeiSDKWakeupSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	XunFeiSDKWakeupSubsystem = this;
	bListenForWakeup = false;
}

void UXGXunFeiSDKWakeupSubsystem::Deinitialize()
{
	StopToListenForWakeup();

	XunFeiSDKWakeupSubsystem = nullptr;

	Super::Deinitialize();

}

UXGXunFeiSDKWakeupSubsystem* UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()
{
	return XunFeiSDKWakeupSubsystem;
}

void UXGXunFeiSDKWakeupSubsystem::StartToListenForWakeup(FString InAppID)
{
	if (!bListenForWakeup)
	{
		bListenForWakeup = true;

		WakeupRunnable = MakeShared<FXGXunFeiSDKWakeupRunnable>(TEXT("XGXunFeiSDKWakeRunnable"), InAppID);

		WakeupThread = FRunnableThread::Create(WakeupRunnable.Get(), *(WakeupRunnable->ThreadName));

		FString InitMessage = FString::Printf(TEXT("[%s]:[%s]"), *FString(__FUNCTION__), TEXT("Listen for wake up begin to init !"));

		UE_LOG(LogXGXunFeiSDKWakeup, Display, TEXT("%s"), *InitMessage);

	}
	else
	{


		FString InitMessage = FString::Printf(TEXT("[%s]:[%s]"), *FString(__FUNCTION__), TEXT("Listen for wake up is running !"));

		UE_LOG(LogXGXunFeiSDKWakeup, Warning, TEXT("%s"), *InitMessage);

		CallInitDelegate(false, -1, InitMessage);

	}





}

void UXGXunFeiSDKWakeupSubsystem::StopToListenForWakeup()
{

	if (WakeupRunnable.IsValid())
	{
		WakeupRunnable->Stop();
	}

	if (WakeupThread)
	{
		WakeupThread->WaitForCompletion();

		delete WakeupThread;

		WakeupThread = nullptr;

	}


}

bool UXGXunFeiSDKWakeupSubsystem::IsLisenForWakeup()
{
	return bListenForWakeup;
}

void UXGXunFeiSDKWakeupSubsystem::CallInitDelegate(bool bInit, int32 Code, FString Message)
{
	FXGXunFeiSDKWakeupInitDelegate TempDeleage = InitDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDeleage.Broadcast(bInit, Code, Message);
		});

}

void UXGXunFeiSDKWakeupSubsystem::CallListenErrorDelegate(int32 Code, FString Message)
{
	FXGXunFeiSDKWakeupListenErrorDelegate TempDeleage = ListenErrorDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDeleage.Broadcast(Code, Message);
		});

}

void UXGXunFeiSDKWakeupSubsystem::CallActiveDelegate(FXGXunFeiSDKWakeupActiveInfo ActiveInfo)
{
	FXGXunFeiSDKWakeupActiveDelegate TempDeleage = ActiveDelegate;
	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDeleage.Broadcast(ActiveInfo);
		});

}

void UXGXunFeiSDKWakeupSubsystem::CallCloseListForWakeup()
{

	AsyncTask(ENamedThreads::GameThread, []() {

		FString CloseMessage = FString::Printf(TEXT("[%s]:[%s]"), *FString(__FUNCTION__), TEXT("Listen for wake up quited !"));

		UE_LOG(LogXGXunFeiSDKWakeup, Display, TEXT("%s"), *CloseMessage);

		bListenForWakeup = false;

		});

}



