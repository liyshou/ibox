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

//	ctsi_type = 0;  //0�����������ݿ⣻1�����������ݿ�
	
}

CADOConn::~CADOConn()
{
	wrLog("���ݿ������ͷſ�ʼ����");
	ExitConnect();
	wrLog("���ݿ������ͷ���ϡ���");
}


int CADOConn::OnInitADOConn()
{

	// ��ʼ��OLE/COM�⻷�� 
	::CoInitialize(NULL);
 	try
	{
		if(TRADE_FLAG==0)
		{
			wrLog("DB_CON_STR = [%s]", DB_CON_STR);

		}
	
		// ����Connection����
		m_pConnection.CreateInstance("ADODB.Connection");

		// ���������ַ�����������BSTR�ͻ���_bstr_t����
		_bstr_t strConnect;
		strConnect = DB_CON_STR; 
		
		m_pConnection->Open(strConnect,"","",adModeUnknown);
		return 0;
	}
	// ��׽�쳣
	catch(_com_error e)
	{
		// ��ʾ������Ϣ
		wrLog("��OnInitADOConn��");
		wrLog(e.Description());
		return 1;
	}
}

// ִ�в�ѯ
//ע�⣺���쳣�˳�ʱ��һ��Ҫ�ر����ӣ�
_RecordsetPtr&  CADOConn::GetRecordSet(_bstr_t bstrSQL, int &ret)
{
	try
	{
		// �������ݿ⣬���Connection����Ϊ�գ��������������ݿ�
		if(m_pConnection==NULL || !(m_pConnection->State))
		{
			if( OnInitADOConn() )
			{
				ret = 1;
				return m_pRecordset;
			}
		}
	//	wrLog("��ѯ���ݿ⡭��");
		// ������¼������
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// ȡ�ñ��еļ�¼
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		ret = 0;
	}
		// ��׽�쳣
	catch(_com_error e)
	{
		// ��ʾ������Ϣ
		wrLog("��GetRecordSet��");
		wrLog(e.Description());
		ret = 1;
	}
	// ���ؼ�¼��
	return m_pRecordset;
}

// ִ��SQL��䣬Insert Update _variant_t
//ע�⣺���쳣�˳�ʱ��һ��Ҫ�ر����ӣ�
BOOL CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
//	_variant_t RecordsAffected;
	try
	{
		// �Ƿ��Ѿ��������ݿ�
		if( m_pConnection == NULL || !(m_pConnection->State) )
		{
			if( OnInitADOConn() )
				return 1;
		}
		// Connection�����Execute����:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options ) 
		// ����CommandText�������ִ���ͨ����SQL���
		// ����RecordsAffected�ǲ�����ɺ���Ӱ�������, 
		// ����Options��ʾCommandText�����ͣ�adCmdText-�ı����adCmdTable-����
		// adCmdProc-�洢���̣�adCmdUnknown-δ֪
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return 0;
	}
	catch(_com_error e)
	{
		wrLog("��ExecuteSQL��");
		wrLog(e.Description());
		return 1;
	}
}

void CADOConn::ExitConnect()
{
	// �رռ�¼��������
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
	// �ͷŻ���
	::CoUninitialize();
}