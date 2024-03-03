// Copyright 2024 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HAL/ThreadSafeBool.h"

#include "XGXunFeiSDKType.h"

#include "XGXunFeiSDKWakeupSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FXGXunFeiSDKWakeupInitDelegate,bool,bInit,int32,Code,FString,Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FXGXunFeiSDKWakeupListenErrorDelegate,int32, Code, FString, Message);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXGXunFeiSDKWakeupActiveDelegate, FXGXunFeiSDKWakeupActiveInfo, ActiveInfo);


class FXGXunFeiSDKWakeupRunnable;

UCLASS()
class  XGXUNFEISDK_API UXGXunFeiSDKWakeupSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class FXGXunFeiSDKWakeupRunnable;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	
	virtual void Deinitialize()override;

	static UXGXunFeiSDKWakeupSubsystem* GetWakeupSubsystem();

public:
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XunFeiStartToListenForWakeup", KeyWords = "XG XunFei Start Listen Wakeup"), 
	Category = "XGXunFeiSDK|Wakeup")
	void StartToListenForWakeup(FString InAppID);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XunFeiStopToListenForWakeup", KeyWords = "XG XunFei Stop Listen Wakeup"),
		Category = "XGXunFeiSDK|Wakeup")
	void StopToListenForWakeup();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "XunFeiIsLisenForWakeup", KeyWords = "XG XunFei Stop Listen Wakeup"),
		Category = "XGXunFeiSDK|Wakeup")
	bool IsLisenForWakeup();

protected:

	void CallInitDelegate(bool bInit, int32 Code, FString Message);

	void CallListenErrorDelegate(int32 Code, FString Message);

	void CallActiveDelegate(FXGXunFeiSDKWakeupActiveInfo ActiveInfo);

	void CallCloseListForWakeup();

public:

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiSDKWakeupInitDelegate InitDelegate;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiSDKWakeupListenErrorDelegate ListenErrorDelegate;

	UPROPERTY(BlueprintAssignable)
	FXGXunFeiSDKWakeupActiveDelegate ActiveDelegate;

protected:

	static UXGXunFeiSDKWakeupSubsystem* XunFeiSDKWakeupSubsystem;

	static FThreadSafeBool bListenForWakeup ;

	TSharedPtr<FXGXunFeiSDKWakeupRunnable> WakeupRunnable;

	FRunnableThread* WakeupThread;




};








