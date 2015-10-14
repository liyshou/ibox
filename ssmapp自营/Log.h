// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__08879AEA_2B08_420C_9024_3C4C7BA23B17__INCLUDED_)
#define AFX_LOG_H__08879AEA_2B08_420C_9024_3C4C7BA23B17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxmt.h"

class CLog  
{
public:
	CCriticalSection          mLogLock;

	CString m_csLogPath;

	int			WriteLogEx(int flag,LPCTSTR szFormat,...);
	int         WriteLog(int flag,char *szMsg,int iLen);//����1024�����������ֹ�ضϡ�
	int         FindFolderOrFile(char *szPath);

	UINT GetQueueMessage(LPVOID lpParam);//�Ӷ�����ȡ�����ݰ�
	static UINT __GetQueueMessage(LPVOID lpParam);//�Ӷ�����ȡ�����ݰ�	
	
	CLog();
	virtual ~CLog();
protected:
	int			WriteFile(char* acLog,int iLen,char *acFileName);
};
#endif // !defined(AFX_LOG_H__08879AEA_2B08_420C_9024_3C4C7BA23B17__INCLUDED_)
