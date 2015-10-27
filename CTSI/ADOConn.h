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
// 定义变量
private:
	//添加一个指向Connection对象的指针:
	_ConnectionPtr m_pConnection;
	//添加一个指向Recordset对象的指针:
	_RecordsetPtr m_pRecordset;
	int ctsi_type;

public:
	CADOConn();
	virtual ~CADOConn();

	// 初始化-连接数据库
	int  OnInitADOConn();
	// 执行查询
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL, int &ret);
	// 执行SQL语句，Insert Update _variant_t
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();

};

#endif // !defined(AFX_ADOCONN_H__D27A7A15_B166_4079_AD21_8D2EAFDCCAB7__INCLUDED_)
