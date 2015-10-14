
#pragma once
//#include "stdafx.h"
#ifdef CCOMDLL_EXPORTS
#define CCOMDLL_API __declspec(dllexport)
#else
#define CCOMDLL_API __declspec(dllimport)
#endif
// CCOMDll

class CCOMDll : public CWnd
{
	DECLARE_DYNAMIC(CCOMDll)

public:
	CCOMDLL_API CCOMDll();
	CCOMDLL_API virtual ~CCOMDll();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	HANDLE m_hComm;
	bool SetRWTimeout(int msRITO, int msRTTOM, int msRTTOC, int msWTTOM, int msWTTOC);
public:
	
	//LOCKERDLL_API int GetDeviceLastError();

	CCOMDLL_API CString GetDLLVersion();
	CCOMDLL_API int ReadCom(BYTE* recv,DWORD &len,DWORD iTimeOut,int iUseFinishFlag=0,BYTE cFinish=0);
	CCOMDLL_API int WriteCom(BYTE* send,DWORD len,DWORD iTimeOut);
	CCOMDLL_API int WriteAndRead(BYTE* send,DWORD sLen,BYTE* recv,DWORD &rLen,DWORD iWriteTimeOut,DWORD iReadTimeOut,int iUseFinishFlag=0,BYTE cFinish=0);
	CCOMDLL_API int InitComm(int iPort,int iBaudRate=CBR_9600,int iByteSize=8,int iParity=EVENPARITY,int iStopBits=ONESTOPBIT);
	CCOMDLL_API bool IsInit();
	CCOMDLL_API int CloseComm();
	CCOMDLL_API void SetLog(int iWriteLog);
};