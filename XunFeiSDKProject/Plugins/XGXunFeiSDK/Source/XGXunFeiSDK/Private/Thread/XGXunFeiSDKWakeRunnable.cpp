// Copyright 2024 Xiao Gang. All Rights Reserved.

#include "XGXunFeiSDKWakeRunnable.h"


#include "Containers/StringConv.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "JsonObjectConverter.h"

#include "LogXGXunFeiSDK.h"
#include "XGXunFeiSDK.h"
#include "XGXunFeiSDKWakeupSubsystem.h"
#include "XGXunFeiSDKType.h"



#include "XGXunFeiSDKLibrary/msp_cmn.h"
#include "XGXunFeiSDKLibrary/msp_errors.h"
#include "XGXunFeiSDKLibrary/msp_types.h"
#include "XGXunFeiSDKLibrary/qivw.h"


#include <windows.h>
//#include <WinDef.h>
//#include "stdlib.h"
//#include "stdio.h"
//#include <conio.h>
//#include <errno.h>


//16k�����ʵ�16bit��Ƶ��һ֡�Ĵ�СΪ640B, ʱ��20ms
#define XGXUNFEIFRAME_LEN	640 




int32 FXGXunFeiSDKWakeupRunnable::WakeupFlage = 0;

FThreadSafeBool FXGXunFeiSDKWakeupRunnable::bIsRunning =true;
FXGXunFeiSDKWakeupRunnable::FXGXunFeiSDKWakeupRunnable(FString InThreadName, FString InAppID)
	:ThreadName(InThreadName)
{
	AppID = InAppID;
	WorkBin = FXGXunFeiSDKModule::WakeupBinPath;

}

FXGXunFeiSDKWakeupRunnable::~FXGXunFeiSDKWakeupRunnable()
{

}

bool FXGXunFeiSDKWakeupRunnable::Init()
{

	bIsRunning = true;
	WakeupFlage = 0;
	return true;
}

uint32 FXGXunFeiSDKWakeupRunnable::Run()
{
	FString LgiParam = FString::Printf(TEXT("appid = %s, work_dir =%s"), *AppID, *WorkBin);

	auto LgiParamChar = StringCast<ANSICHAR>(*LgiParam);

	int         ret = MSP_SUCCESS;

	const char* lgi_param = LgiParamChar.Get();

	const char* ssb_param = "ivw_threshold=0:1450,sst=wakeup,ivw_res_path =fo|res/ivw/wakeupresource.jet";

	ret = MSPLogin(NULL, NULL, lgi_param);

	if (MSP_SUCCESS != ret)
	{

		FString ErrorMessage = FString::Printf(TEXT("MSPLogin failed,ErrorCode =%d"), ret);

		PrintError(ErrorMessage);

		UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallInitDelegate(false, ret, ErrorMessage);

		return 0;
	}

	FString InitMessage = TEXT("MSPLogin Success");

	PrintDisplay(InitMessage);

	UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallInitDelegate(true, 0, InitMessage);

	while (bIsRunning)
	{
		RunListen(NULL, ssb_param);
	}
	return 0;
}

void FXGXunFeiSDKWakeupRunnable::Stop()
{

	bIsRunning = false;

}

void FXGXunFeiSDKWakeupRunnable::Exit()
{
	//�˳���¼
	MSPLogout();

	UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallCloseListForWakeup();

}

int FXGXunFeiSDKWakeupRunnable::WakeUpCallBack(const char* sessionID, int msg, int param1, int param2, const void* info, void* userData)
{
	if (MSP_IVW_MSG_ERROR == msg)
	{
		bIsRunning =false;
		//���ѳ�����Ϣ
		int32 ErrorCode = param1;

		FString ErrorMessage =FString::Printf(TEXT("MSP_IVW_MSG_ERROR ErrorCode = %d"), ErrorCode);

		PrintError(ErrorMessage);

		UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallListenErrorDelegate(param1, TEXT("MSP_IVW_MSG_ERROR"));

	}
	else if (MSP_IVW_MSG_WAKEUP == msg)
	{
		//���ѳɹ���Ϣ
		WakeupFlage = 1;

		auto WakeupInfo = StringCast<TCHAR>((char*)info);

		FString Result = WakeupInfo.Get();

		FXGXunFeiSDKWakeupActiveInfo ActiveInfo;

		FJsonObjectConverter::JsonObjectStringToUStruct(Result,&ActiveInfo);

		FString ActiveMessage = FString::Printf(TEXT("MSP_IVW_MSG_WAKEUP Result = [%s]"), *Result);

		PrintLog(ActiveMessage);

		UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallActiveDelegate(ActiveInfo);




	}


	return 0;

}

void FXGXunFeiSDKWakeupRunnable::RunListen(const char* grammar_list, const char* session_begin_params)
{

	const char* session_id = NULL;
	int err_code = MSP_SUCCESS;
	long audio_size = 0;
	long audio_count = 0;
	int count = 0;
	int audio_stat = MSP_AUDIO_SAMPLE_CONTINUE;
	char sse_hints[128];
	DWORD bufsize;
	long len = 0;

	//�����豸
	HWAVEIN hWaveIn; 

	//�ɼ���Ƶ�ĸ�ʽ���ṹ��
	WAVEFORMATEX waveform; 

	//�ɼ���Ƶʱ�����ݻ���
	BYTE* pBuffer;

	//�ɼ���Ƶʱ�������ݻ���Ľṹ��
	WAVEHDR wHdr; 
	HANDLE          wait;

	//������ʽΪPCM
	waveform.wFormatTag = WAVE_FORMAT_PCM;

	//��Ƶ������
	waveform.nSamplesPerSec = 16000;

	//��������
	waveform.wBitsPerSample = 16;

	//����������
	waveform.nChannels = 1;

	//ÿ���������
	waveform.nAvgBytesPerSec = 16000;

	//һ����Ĵ�С������bit���ֽ�������������
	waveform.nBlockAlign = 2;

	//һ��Ϊ0
	waveform.cbSize = 0;

	wait = CreateEvent(NULL, 0, 0, NULL);

	//ʹ��waveInOpen����������Ƶ�ɼ�
	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//�����ʵ���С���ڴ�洢��Ƶ���ݣ����ʵ������ڴ��С������¼��ʱ�䣬���ȡ�������ڴ������
	bufsize = 1024 * 500;

	session_id = QIVWSessionBegin(grammar_list, session_begin_params, &err_code);

	if (err_code != MSP_SUCCESS)
	{
		bIsRunning =false;

		FString ErrorMessage =FString::Printf(TEXT("QIVWSessionBegin failed! error code:%d\n"), err_code);

		PrintError(ErrorMessage);

		UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallListenErrorDelegate(err_code, ErrorMessage);
		
		goto exit;
	}

	err_code = QIVWRegisterNotify(session_id, WakeUpCallBack, NULL);

	if (err_code != MSP_SUCCESS)
	{
		bIsRunning = false;

		FString ErrorMessage = FString::Printf(TEXT("QIVWRegisterNotify failed! error code:%d\n"), err_code);

		PrintError(ErrorMessage);

		UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallListenErrorDelegate(err_code, ErrorMessage);

		goto exit;
	}

	pBuffer = (BYTE*)malloc(bufsize);
	wHdr.lpData = (LPSTR)pBuffer;
	wHdr.dwBufferLength = bufsize;
	wHdr.dwBytesRecorded = 0;
	wHdr.dwUser = 0;
	wHdr.dwFlags = 0;
	wHdr.dwLoops = 1;

	//׼��һ���������ݿ�ͷ����¼��
	waveInPrepareHeader(hWaveIn, &wHdr, sizeof(WAVEHDR));

	//ָ���������ݿ�Ϊ¼�����뻺��
	waveInAddBuffer(hWaveIn, &wHdr, sizeof(WAVEHDR));

	//��ʼ¼��
	waveInStart(hWaveIn);

	//���λ���
	//while (audio_count< bufsize && wakeupFlage!=1&& bIsRunning)

	while (audio_count < (long)bufsize&& bIsRunning)//��������
	{
		//�ȴ�����¼��5s
		Sleep(200);
		
		//16k��Ƶ��10֡ ��ʱ��200ms��
		len = 10 * XGXUNFEIFRAME_LEN;

		audio_stat = MSP_AUDIO_SAMPLE_CONTINUE;
		
		if (audio_count >= (long)wHdr.dwBytesRecorded)
		{
			len = audio_size;
			//���һ��
			audio_stat = MSP_AUDIO_SAMPLE_LAST; 
		}
		
		if (0 == audio_count)
		{
			audio_stat = MSP_AUDIO_SAMPLE_FIRST;
		}
	
		
		FString LogMesssage = FString::Printf(TEXT("csid=%s,count=%d,aus=%d\n"), ANSI_TO_TCHAR(session_id), count++, audio_stat);
		
		PrintLog(LogMesssage);
		
		err_code = QIVWAudioWrite(session_id, (const void*)&pBuffer[audio_count], len, audio_stat);
		
		if (MSP_SUCCESS != err_code)
		{

			bIsRunning = false;

			FString ErrorMessage = FString::Printf(TEXT("QIVWAudioWrite failed! error code:%d\n"), err_code);

			PrintError(ErrorMessage);

			UXGXunFeiSDKWakeupSubsystem::GetWakeupSubsystem()->CallListenErrorDelegate(err_code, ErrorMessage);

			goto exit;
		}
		if (MSP_AUDIO_SAMPLE_LAST == audio_stat)
		{

			break;
		}
		audio_count += len;
	}
	//ֹͣ¼��
	waveInReset(hWaveIn);
	
	if (NULL != pBuffer)
	{
		free(pBuffer);
	}

exit:

	if (NULL != session_id)
	{
		QIVWSessionEnd(session_id, sse_hints);
	}

}

void FXGXunFeiSDKWakeupRunnable::PrintLog(FString InMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {

		UE_LOG(LogXGXunFeiSDKWakeup, Log, TEXT("%s"), *InMessage);

		});
}

void FXGXunFeiSDKWakeupRunnable::PrintDisplay(FString InMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {

		UE_LOG(LogXGXunFeiSDKWakeup, Display, TEXT("%s"), *InMessage);

		});
}

void FXGXunFeiSDKWakeupRunnable::PrintWarning(FString InMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {

		UE_LOG(LogXGXunFeiSDKWakeup, Warning, TEXT("%s"), *InMessage);

		});
}

void FXGXunFeiSDKWakeupRunnable::PrintError(FString InMessage)
{
	AsyncTask(ENamedThreads::GameThread, [=]() {

		UE_LOG(LogXGXunFeiSDKWakeup, Error, TEXT("%s"), *InMessage);

		});
}
