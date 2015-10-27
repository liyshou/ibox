// ADOConn.h: interface for the CADOConn class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4146)
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")

#if !defined(AFX_ADOCONN_H__D27A7A15_B166_4079_AD21_8D2EAFDCCAB7__INCLUDED_)
#define AFX_ADOCONN_H__D27A7A15_B166_4079_AD21_8D2EAFDCCAB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CADOConn  
{
// �������
private:
	//���һ��ָ��Connection�����ָ��:
	_ConnectionPtr m_pConnection;
	//���һ��ָ��Recordset�����ָ��:
	_RecordsetPtr m_pRecordset;
	int ctsi_type;

public:
	CADOConn();
	virtual ~CADOConn();

	// ��ʼ��-�������ݿ�
	int  OnInitADOConn();
	// ִ�в�ѯ
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL, int &ret);
	// ִ��SQL��䣬Insert Update _variant_t
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();

};

#endif // !defined(AFX_ADOCONN_H__D27A7A15_B166_4079_AD21_8D2EAFDCCAB7__INCLUDED_)
