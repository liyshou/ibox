#pragma once

#include "stdafx.h"

#ifdef UPSDLL_EXPORTS
#define UPSDLL_API __declspec(dllexport)
#else
#define UPSDLL_API __declspec(dllimport)
#endif


class UPSDll : public CWnd
{
	DECLARE_DYNAMIC(UPSDll)

public:
	UPSDLL_API UPSDll();
	UPSDLL_API virtual ~UPSDll();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	int ExecCommand(char*command,char* response,DWORD rLen=0);//rLen 控制最大接收响应长度 
	CString m_csData;   // 

	CCriticalSection critical_section;
public:
	UPSDLL_API int InitCom(int port);	
	UPSDLL_API int GetStatus();

	//UPSDLL_API int AllowRepeat();
	//UPSDLL_API int ForbitRepeat();
	//UPSDLL_API int SetReadCodeDuration(int level);
	//UPSDLL_API int SetPauseDuration(int level);
	//UPSDLL_API int Trigger();
	//UPSDLL_API int StopTrigger();
	//UPSDLL_API int SetVoice(int level);
	//UPSDLL_API CString GetFirmwork();
	//UPSDLL_API CString GetSerial();
	//UPSDLL_API CString GetProductName();
	//UPSDLL_API CString GetProdutionDate();
	//UPSDLL_API CString GetDLLVersion();
	//UPSDLL_API CString GetBarCode();
};