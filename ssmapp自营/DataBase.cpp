// DataBase.cpp: implementation of the CDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataBase::CDataBase()
{
	g_Log.WriteLogEx(LOG_DATABASE,"CDataBase()");
	m_hMysqlHandle = NULL;
	m_ResultSet = NULL;
	m_Row = NULL;
}

CDataBase::~CDataBase()
{
	g_Log.WriteLogEx(LOG_DATABASE,"~CDataBase()");
}

BOOL CDataBase::InitDB()
{
	m_hMysqlHandle = mysql_init(NULL);
	if(!m_hMysqlHandle)
		g_Log.WriteLogEx(LOG_DATABASE,"InitDB()Ê§°Ü");

	char value = 1;
	int iRet = mysql_options(m_hMysqlHandle, MYSQL_OPT_RECONNECT, (char *)&value);

	return !!m_hMysqlHandle;
}

BOOL CDataBase::ConnectDB( const char* host, const char* user, const char* password, const char* databaseName, UINT port )
{
	m_hMysqlHandle = mysql_real_connect(m_hMysqlHandle, host, user, password, databaseName, port, NULL, 0);
	if(!m_hMysqlHandle)
		g_Log.WriteLogEx(LOG_DATABASE,"ConnectDB()Ê§°Ü");
	return !!m_hMysqlHandle;
}

BOOL CDataBase::ExeSQLQuery( const char* sqlString )
{
	g_Log.WriteLogEx(LOG_DATABASE,"ExeSQLQuery(%s)",sqlString);
	if(!this)
	{
		g_Log.WriteLogEx(LOG_DATABASE,"ExeSQLQuery()Î´ÊµÀý»¯Êý¾Ý¿â");
		return false;
	}
	if(!m_hMysqlHandle)
	{
		g_Log.WriteLogEx(LOG_DATABASE,"ExeSQLQuery()Î´³õÊ¼»¯Êý¾Ý¿â");
		return false;
	}
	
	int result = mysql_query(m_hMysqlHandle, sqlString);	
	if(result)
		g_Log.WriteLogEx(LOG_DATABASE,"ExeSQLQuery()Ê§°Ü");
	return !result;
}

BOOL CDataBase::StoreResult()
{
	m_ResultSet = mysql_store_result(m_hMysqlHandle);
	if(!m_ResultSet)
		g_Log.WriteLogEx(LOG_DATABASE,"StoreResult()Ê§°Ü");
	return !!m_ResultSet;
}

void CDataBase::FreeResultSet()
{
	mysql_free_result(m_ResultSet);
}

const char* CDataBase::GetErrorString()
{
	return mysql_error(m_hMysqlHandle);
}

int CDataBase::GetErrorNo()
{
	return mysql_errno(m_hMysqlHandle);
}

MYSQL* CDataBase::GetMysqlHandle()
{
	return m_hMysqlHandle;
}

MYSQL_RES* CDataBase::GetResultSet()
{
	return m_ResultSet;
}

MYSQL_ROW CDataBase::GetRow()
{
	return (m_Row = mysql_fetch_row(m_ResultSet));
}

UINT CDataBase::GetRowFieldNum()
{
	return mysql_num_fields(m_ResultSet);
}

void CDataBase::DisconnectDB()
{
	mysql_close(m_hMysqlHandle);
}

__int64 CDataBase::GetRowNumOfSelect()
{
	return mysql_num_rows(m_ResultSet);
}

__int64 CDataBase::GetRowNumOfAffected()
{
	return mysql_affected_rows(m_hMysqlHandle);
}

BOOL CDataBase::AutoCommit( BOOL bEnable/*=TRUE*/ )
{
	return (mysql_autocommit(m_hMysqlHandle, bEnable) == 0) ? TRUE : FALSE;
}

BOOL CDataBase::Commit()
{
	return (mysql_commit(m_hMysqlHandle) == 0) ? TRUE : FALSE;
}

BOOL CDataBase::Rollback()
{
	return (mysql_rollback(m_hMysqlHandle) == 0) ? TRUE : FALSE;
}

