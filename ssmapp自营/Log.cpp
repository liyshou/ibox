// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLog::CLog()
{
	char szTemp[4096]={0};
	if(GetModuleFileName(NULL, szTemp, sizeof(szTemp)))
    {
		m_csLogPath=szTemp;
		m_csLogPath.Format("%s\\Log",m_csLogPath.Left(m_csLogPath.ReverseFind('\\')));
    }

	if(!FindFolderOrFile(m_csLogPath.GetBuffer(0))) //folder doesn't exist
	{
		CreateDirectory(m_csLogPath,NULL);
	}
}

CLog::~CLog()
{

}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
UINT CLog::__GetQueueMessage(LPVOID lpParam)
{
	return g_Log.GetQueueMessage(NULL);
}

/////////////////////////////////////////////////////////////////
//功能：循环取出数据队列中的数据包进行处理
/////////////////////////////////////////////////////////////////
UINT CLog::GetQueueMessage(LPVOID lpParam)
{	
	while(TRUE)
	{
		if(g_QueueLog.IsEmpty())
		{
			Sleep(200);
			continue;
		}
		CString csLog,csFileName;
		int iIsEncrypt;
		g_QueueLog.GetHead(csLog,csFileName,iIsEncrypt);
		if(iIsEncrypt&&!g_PlainTraceLog)
		{
			char* abc=csLog.GetBuffer(0);
			int iLen=csLog.GetLength();
			for(int i=0;i<iLen;i++)
				abc[i]^=127;
			WriteFile(abc,iLen,csFileName.GetBuffer(0));
		}
		else
			WriteFile(csLog.GetBuffer(0),csLog.GetLength(),csFileName.GetBuffer(0));
	}
	return 0;
}

int CLog::WriteLogEx(int flag,LPCTSTR szFormat,...)
{
	va_list vl;
	char bufTmp[8192];
	va_start(vl,szFormat);
	wvsprintf(bufTmp, szFormat, vl);
	va_end(vl);

	if(1024==strlen(bufTmp))
		WriteLogEx(flag,"警告!日志可能被截断.");

	return WriteLog(flag,bufTmp,strlen(bufTmp));
}

int CLog::WriteLog(int flag,char *szMsg,int iLen)
{
	SYSTEMTIME   st={0}; 
	GetLocalTime(&st);
	
	CString strpath;	
	switch(flag)
	{
	case LOG_JOURNAL:
		strpath.Format("%s\\log\\log%04d%02d%02d.txt",g_AppPath,st.wYear,st.wMonth,st.wDay);
		break;
	case LOG_KTVIEW:
		strpath.Format("%s\\log\\ktviewlog%04d%02d%02d.txt",g_AppPath,st.wYear,st.wMonth,st.wDay);
		break;
	case LOG_TRACE:
		strpath.Format("%s\\log\\tracelog%04d%02d%02d",g_AppPath,st.wYear,st.wMonth,st.wDay);
		break;
	case LOG_FLASH:
		strpath.Format("%s\\log\\flashlog%04d%02d%02d.txt",g_AppPath,st.wYear,st.wMonth,st.wDay);
		break;
	case LOG_COMMUNICATION:
		strpath.Format("%s\\log\\commlog%04d%02d%02d.txt",g_AppPath,st.wYear,st.wMonth,st.wDay);
		break;
	case LOG_DATABASE:
		strpath.Format("%s\\log\\dblog%04d%02d%02d.txt",g_AppPath,st.wYear,st.wMonth,st.wDay);
		break;
	default:
		return false;
	}
	int iIsEncrypt=flag&LOG_ENCRYPT;
	CString csLog;
	csLog.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d : %s\r\n",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,szMsg);
	g_QueueLog.AddTail(csLog,strpath,iIsEncrypt);
	return true;
}

int	CLog::WriteFile(char* acLog,int iLen,char* acFileName)
{
	mLogLock.Lock();
	CFile cFile;
	if(PathFileExists(acFileName))
	{
		if(!cFile.Open(acFileName,CFile::modeWrite|CFile::shareExclusive))
		{
			mLogLock.Unlock();
			return false;
		}
	}
	else
	{
		if(!cFile.Open(acFileName,CFile::modeWrite|CFile::shareExclusive|CFile::modeCreate))
		{
			mLogLock.Unlock();
			return false;
		}
	}	
	cFile.SeekToEnd();
	cFile.Write(acLog,iLen);
	cFile.Close();
	mLogLock.Unlock();
	return true;
}

// make certain whether file / directory exists 
int CLog::FindFolderOrFile(char *szPath)
{
	WIN32_FIND_DATA fd;
	int iRet = 0;
	
	HANDLE hFind = FindFirstFile(szPath, &fd);
	if(  hFind != INVALID_HANDLE_VALUE )
	{	
		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  
		{
			iRet = 1;								   //it's a directory
		}
		else
		{
			iRet =  2;                                  //it's a file
		}
	}
	else
		iRet = 0;                                        //it doesn't exist
	
	FindClose(hFind);
	return iRet;	
}
