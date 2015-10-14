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
	int         WriteLog(int flag,char *szMsg,int iLen);//大于1024，用这个，防止截断。
	int         FindFolderOrFile(char *szPath);

	UINT GetQueueMessage(LPVOID lpParam);//从队列中取出数据包
	static UINT __GetQueueMessage(LPVOID lpParam);//从队列中取出数据包	
	
	CLog();
	virtual ~CLog();
protected:
	int			WriteFile(char* acLog,int iLen,char *acFileName);
};
#endif // !defined(AFX_LOG_H__08879AEA_2B08_420C_9024_3C4C7BA23B17__INCLUDED_)
