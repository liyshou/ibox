#pragma once

#include "stdafx.h"

#ifdef CODEBARSCANERDLL_EXPORTS
#define CODEBARSCANERDLL_API __declspec(dllexport)
#else
#define CODEBARSCANERDLL_API __declspec(dllimport)
#endif
// CodeBarScanerDll

class CodeBarScanerDll : public CWnd
{
	DECLARE_DYNAMIC(CodeBarScanerDll)

public:
	CODEBARSCANERDLL_API CodeBarScanerDll();
	CODEBARSCANERDLL_API virtual ~CodeBarScanerDll();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	int m_iLastDeviceError;
	int m_iDeviceType;
	int m_iPort;
	int ConvertErrCode(int iErr,CString csFunc="");

	
	CString m_barCode;
	static UINT ReadBarCodeThread(LPVOID lpParam);
	int ReadBarCodeLoop();//读条码循环
	void PauseLoop();
	void ResumeLoop();
	int m_iReadBarCodeStatus;//控制变量
	CCriticalSection critical_section;
public:
	CODEBARSCANERDLL_API CString GetDLLVersion();
	CODEBARSCANERDLL_API CString GetDeviceModel();  //////
	CODEBARSCANERDLL_API int SetDeviceType(int iType);

	CODEBARSCANERDLL_API int InitCom(int port);	
	CODEBARSCANERDLL_API int Setup(char* command,char* response);
	CODEBARSCANERDLL_API int SetWorkMode(int mode);
	CODEBARSCANERDLL_API int LoadDefault();
	CODEBARSCANERDLL_API int AllowRepeat();
	CODEBARSCANERDLL_API int ForbitRepeat();
	CODEBARSCANERDLL_API int SetReadCodeDuration(int level);
	CODEBARSCANERDLL_API int SetPauseDuration(int level);
	CODEBARSCANERDLL_API int Trigger();
	CODEBARSCANERDLL_API int StopTrigger();
	CODEBARSCANERDLL_API int SetVoice(int level);
	CODEBARSCANERDLL_API CString GetFirmwork();
	CODEBARSCANERDLL_API CString GetSerial();
	CODEBARSCANERDLL_API CString GetProductName();
	CODEBARSCANERDLL_API CString GetProdutionDate();
	CODEBARSCANERDLL_API CString GetBarCode();
};