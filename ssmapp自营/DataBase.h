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
	BOOL InitDB();  //清空数据库句柄
	BOOL ConnectDB(const char* host, const char* user, const char* password, const char* databaseName, UINT port);  //链接数据库
	BOOL ExeSQLQuery(const char* sqlString);  //执行语句
	BOOL StoreResult();//保存查询的结果
	void FreeResultSet();//释放结果内存，与StoreResult()搭配用
	void DisconnectDB(); 
	BOOL AutoCommit(BOOL bEnable=TRUE); //是否自动提交（若条件要求是：多条语句必须都执行成功，否则不执行，可先置为false，结束时再置回true）
	BOOL Commit(); //与AutoCommit(FALSE)搭配使用
	BOOL Rollback(); //回滚，在提交之前使用，置之前的执行语句无效
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
