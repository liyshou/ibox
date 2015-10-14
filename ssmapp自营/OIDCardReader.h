#include "stdafx.h"

#pragma once

class CIDCardData
{
public:
	CString name;
	CString sex;
	CString nation;
	CString birthday;
	CString address;
	CString idcode;
	CString department;
	CString start;
	CString end;

	void ClearData()
	{
		name.Empty();
		sex.Empty();
		nation.Empty();
		birthday.Empty();
		address.Empty();
		idcode.Empty();
		department.Empty();
		start.Empty();
		end.Empty();
	}
};

#ifdef OIDCARDREADER_EXPORTS
#define OIDCARDREADER_API __declspec(dllexport)
#else
#define OIDCARDREADER_API __declspec(dllimport)
#endif

class OIDCardReader : public CWnd
{
	DECLARE_DYNAMIC(OIDCardReader)

public:
	OIDCARDREADER_API OIDCardReader();
	OIDCARDREADER_API virtual ~OIDCardReader();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	int m_iLastDeviceError;
	int m_iDeviceType;
	int m_iPort;
	int m_iCreatePhoto;
	CString m_csDllDirectory;
	CString m_csDLLPathName;
	CString m_csPhotoPath;
	int ConvertErrCode(int iErr,CString csFunc="");
public:
	OIDCARDREADER_API CString GetDLLVersion();
	OIDCARDREADER_API CString GetDeviceModel();
	OIDCARDREADER_API int SetPort(int iPort);
	OIDCARDREADER_API int SetDeviceType(int iType);
	OIDCARDREADER_API int SetDllPathName(CString csPathName);
	OIDCARDREADER_API int SetPhotoPath(CString csPathName);
	OIDCARDREADER_API int SetCreatePhoto(int iCreate=1);
	OIDCARDREADER_API int GetLastDeviceError();
	//
	OIDCARDREADER_API int InitComm();
	OIDCARDREADER_API int CloseComm();
	OIDCARDREADER_API int Authenticate();
	OIDCARDREADER_API int ReadContent(CIDCardData& IDCardData);
	OIDCARDREADER_API bool IsCardOn();
	OIDCARDREADER_API int ICFindCard();
	OIDCARDREADER_API CString ICReadCardSN();
	OIDCARDREADER_API int ICReadCard(int SID,int BID,int KeyType,BYTE* Key,BYTE* data);
	OIDCARDREADER_API int ICWriteCard(int SID,int BID,int KeyType,BYTE* Key,BYTE* data);
	
};