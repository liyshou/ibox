// ADOConn.cpp: implementation of the CADOConn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "ADOConn.h"
#include "Def.h"
#include "WrLog.h"
#include "Des.h"
#include "Tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADOConn::CADOConn()
{
//	m_pConnection = NULL;

//	ctsi_type = 0;  //0，连测试数据库；1，连生产数据库
	
}

CADOConn::~CADOConn()
{
	wrLog("数据库连接释放开始……");
	ExitConnect();
	wrLog("数据库连接释放完毕……");
}


int CADOConn::OnInitADOConn()
{

	// 初始化OLE/COM库环境 
	::CoInitialize(NULL);
 	try
	{
		if(TRADE_FLAG==0)
		{
			wrLog("DB_CON_STR = [%s]", DB_CON_STR);

		}
	
		// 创建Connection对象
		m_pConnection.CreateInstance("ADODB.Connection");

		// 设置连接字符串，必须是BSTR型或者_bstr_t类型
		_bstr_t strConnect;
		strConnect = DB_CON_STR; 
		
		m_pConnection->Open(strConnect,"","",adModeUnknown);
		return 0;
	}
	// 捕捉异常
	catch(_com_error e)
	{
		// 显示错误信息
		wrLog("【OnInitADOConn】");
		wrLog(e.Description());
		return 1;
	}
}

// 执行查询
//注意：在异常退出时，一定要关闭连接！
_RecordsetPtr&  CADOConn::GetRecordSet(_bstr_t bstrSQL, int &ret)
{
	try
	{
		// 连接数据库，如果Connection对象为空，则重新连接数据库
		if(m_pConnection==NULL || !(m_pConnection->State))
		{
			if( OnInitADOConn() )
			{
				ret = 1;
				return m_pRecordset;
			}
		}
	//	wrLog("查询数据库……");
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		ret = 0;
	}
		// 捕捉异常
	catch(_com_error e)
	{
		// 显示错误信息
		wrLog("【GetRecordSet】");
		wrLog(e.Description());
		ret = 1;
	}
	// 返回记录集
	return m_pRecordset;
}

// 执行SQL语句，Insert Update _variant_t
//注意：在异常退出时，一定要关闭连接！
BOOL CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
//	_variant_t RecordsAffected;
	try
	{
		// 是否已经连接数据库
		if( m_pConnection == NULL || !(m_pConnection->State) )
		{
			if( OnInitADOConn() )
				return 1;
		}
		// Connection对象的Execute方法:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options ) 
		// 其中CommandText是命令字串，通常是SQL命令。
		// 参数RecordsAffected是操作完成后所影响的行数, 
		// 参数Options表示CommandText的类型：adCmdText-文本命令；adCmdTable-表名
		// adCmdProc-存储过程；adCmdUnknown-未知
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return 0;
	}
	catch(_com_error e)
	{
		wrLog("【ExecuteSQL】");
		wrLog(e.Description());
		return 1;
	}
}

void CADOConn::ExitConnect()
{
	// 关闭记录集和连接
	if (m_pRecordset != NULL)
	{
		if(m_pRecordset->State)
		{
			m_pRecordset->Close();
			m_pRecordset.Release();
		}
	}
	/*-------------------------*/
	if (m_pConnection != NULL)
	{
		if(m_pConnection->State)
		{
			m_pConnection->Close();
			m_pConnection.Release();
			m_pConnection = NULL;
		}
	}
	// 释放环境
	::CoUninitialize();
}