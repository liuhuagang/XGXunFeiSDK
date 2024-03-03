// Copyright 2024 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XGXunFeiSDKType.generated.h"

USTRUCT(BlueprintType)
struct XGXUNFEISDK_API FXGXunFeiSDKWakeupActiveInfo
{
	GENERATED_USTRUCT_BODY()

public:
	/**
	 * @brief 本次业务标识：wakeup表示语音唤醒；oneshot表示唤醒+识别；
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGXunFeiSDK|Wakeup")
	FString sst = TEXT("");
	/**
	 * @brief 当前唤醒词的id
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGXunFeiSDK|Wakeup")
	int32 id = -1;
	/**
	 * @brief 当前唤醒得分，只有当分数大于等于设置的门限值时才会回调唤醒结果
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGXunFeiSDK|Wakeup")
	int32 score = -1;
	/**
	 * @brief 当前唤醒音频的前端点，即当前唤醒音频在写入的总音频中的开始时间位置，单位:ms
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGXunFeiSDK|Wakeup")
	int32 bos = -1;
	/**
	 * @brief 当前唤醒音频的尾端点，即当前唤醒音频在写入的总音频中的结束时间位置，单位:ms
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGXunFeiSDK|Wakeup")
	int32 eos = -1;
	/**
	 * @brief 当前唤醒词，若是中文唤醒词会自动以拼音形式显示
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "XGXunFeiSDK|Wakeup")
	FString keyword = TEXT("");

};