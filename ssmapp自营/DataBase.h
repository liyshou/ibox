// DataBase.h: interface for the CDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__E1CCAB07_CD0A_4FD3_9405_44FAAE88E496__INCLUDED_)
#define AFX_DATABASE_H__E1CCAB07_CD0A_4FD3_9405_44FAAE88E496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataBase  
{
public:
	CDataBase();
	virtual ~CDataBase();

public:
	BOOL InitDB();  //������ݿ���
	BOOL ConnectDB(const char* host, const char* user, const char* password, const char* databaseName, UINT port);  //�������ݿ�
	BOOL ExeSQLQuery(const char* sqlString);  //ִ�����
	BOOL StoreResult();//�����ѯ�Ľ��
	void FreeResultSet();//�ͷŽ���ڴ棬��StoreResult()������
	void DisconnectDB(); 
	BOOL AutoCommit(BOOL bEnable=TRUE); //�Ƿ��Զ��ύ��������Ҫ���ǣ����������붼ִ�гɹ�������ִ�У�������Ϊfalse������ʱ���û�true��
	BOOL Commit(); //��AutoCommit(FALSE)����ʹ��
	BOOL Rollback(); //�ع������ύ֮ǰʹ�ã���֮ǰ��ִ�������Ч
	BOOL Ping();

	const char* GetErrorString();
	int  GetErrorNo();
	MYSQL* GetMysqlHandle();
	MYSQL_RES* GetResultSet();
	MYSQL_ROW GetRow();
	UINT GetRowFieldNum();
	__int64 GetRowNumOfSelect();
	__int64 GetRowNumOfAffected();

private:
	MYSQL* m_hMysqlHandle;
	MYSQL_RES* m_ResultSet;
	MYSQL_ROW m_Row;
};

#endif // !defined(AFX_DATABASE_H__E1CCAB07_CD0A_4FD3_9405_44FAAE88E496__INCLUDED_)
