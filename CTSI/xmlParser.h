// xmlParser.h: interface for the CxmlParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_xmlParser_H__045D6E06_D393_49AB_B98D_3EBBCB42DAE5__INCLUDED_)
#define AFX_xmlParser_H__045D6E06_D393_49AB_B98D_3EBBCB42DAE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Global.h"
#define MAXAMOLEN 15

#include "tinystr.h"
#include "tinyxml.h"

#include <string>
#include <map>
#include <list>
using namespace std;



typedef struct field_cfg
{
	int m_iIsCN; //�Ƿ��������ַ�
    char *m_strKey; //mapTransInfo�е�key
    char *m_strAttr;//xml�е������� 
    char * m_strSelf;//xml�еĽڵ���
    char *m_strParent;//xml�еĸ��ڵ���
    char *m_strGrant;//xml�е��游�ڵ���
}field_cfg;


class CxmlParser  
{

public:
	CxmlParser();
	virtual ~CxmlParser();

	/************************************************************************/
	/* packAreaQryxml
	OUTTMNNUMNO �ⲿ�ն˺� 8 
	ACCEPTAREACODE ����������� 6 
	ACCEPTDATE ����ʱ��  14 YYYYMMDDhhmmss                                                                      
	/************************************************************************/

	int static packAreaQryxml(CGlobal gbl,char *OUTTMNNUMNO,char * areaCode,char *ACCEPTDATE, string &strReq0,string &strReq1,char * WEBSVRNAME,char *WEBSVRCODE); 

	/************************************************************************/
	/*
	packSDMQryxml��packSDMQryxml
	ORDERSEQ ������ 32 �ο�����:������+�ն˺�+YYMMDDHHMMSS+�����
	OUTTMNNUMNO �ⲿ�ն˺� 8 
	SELECTTYPE ��ѯ���� ��ѯ���ͣ�001:�û��� 002:������
	ACCEPTDATE ����ʱ��  YYYYMMDDhhmmss
	ACCEPTAREACODE ����������� 6 
	ADDITEM1 16 �ɷ�ʡ���루6λ��+�ɷѳ��д��루6λ��+�ɷ���Ŀ��ţ�4λ��
	ADDITEM2 ��ѯ���  �Ϻ�ˮ��ú��Ҫ��������� 
                
	/************************************************************************/
	int static  packSDMQryxml(CGlobal gbl,char * WEBSVRNAME,char *WEBSVRCODE,char *OUTTMNNUMNO,char *SELECTTYPE,char *SELECTVALUE,char *ACCEPTDATE,char *ACCEPTAREACODE,char *ADDITEM1,char *ADDITEM2,string &strReq0,string &strReq1 ) ;//�Ϻ�ˮ�Ѻ�ú��ʹ��

	/************************************************************************/
	/* packSDMPayxml
	ORDERSEQ 32 ���׷�����ˮ�� 
	OUTTMNNUMNO 8 
	ACCEPTDATE ����ʱ�� 14  YYYYMMDDhhmmss 
	SYSTEMNO ϵͳ�ο��� 15  
	TXNAMOUNT ���׽�� 14 
	CASHTYPE ���˵������� 0-���� 1-�ɷѵ��� 
	PAYTYPE ֧����ʽ 0 ��ҵ�˻� 1 POS���ո�Ĭ����ҵ�˻� 0
	OPERUSER ��ҵ�˻�����Ա 64 ֧����ʽΪ��ҵ�˻�ʱ��д 
	OPERPASSWORD ��ҵ�˻�֧������ 128 ֧����ʽΪ��ҵ�˻�ʱ��д                                                                    
	/************************************************************************/
	int static packSDMPayxml(CGlobal gbl,char *OUTTMNNUMNO,char *WEBSVRNAME,char *WEBSVRCODE,char *ACCEPTDATE,char *SYSTEMNO,char *TXNAMOUNT,int CASHTYPE,char * CASHORDER,char *CASHNUMBER,int PAYTYPE,char *OPERUSER,char *OPERPASSWORD,string &strReq0,string &strReq1);
	/************************************************************************/
	/*  packSDMPayRevxml          
	RESPONSECODE ��Ӧ�� 6 ��Ӧ��Ϊ000000��ʾ�ɹ�������Ϊʧ��
	RESPONSECONTENT ��Ӧ������ 
	OUTTMNNUMNO �ⲿ�ն˺� 
	ORDERSEQ ������ �������ɵ���ˮ�� 
	TRANSSEQ ������ˮ�� 15
	SYSTEMNO ϵͳ�ο���  15
	TXNAMOUNT ���׽�� �Է�Ϊ��λ,�˵��Ľ�� 
	CASHTYPE ���˵������� 0-����1-�ɷѵ���
	/************************************************************************/
	int static packSDMPayRevxml(string strReq0,string strReq1,char *ORDERSEQ,char *TRANSSEQ,char *TXNAMOUNT,char *CASHTYPE);
	void static  UTF8toANSI(string &strUTF8);
	void static  ANSItoUTF8(string &strAnsi);
	int  static unpack(const string &_strSrc,string m_strTransCode, map<string, string> &_mapData, string &_strErr);
	const  field_cfg * get_cfg(int _iReqOrRsp,string m_strTransCode); 
	int static getAreaDatas( string datas); //��ȡ����
	int    static getAreaDatas( string datas,CGlobal gbl);
	int static getBills( string datas,CGlobal gbl) ; //��ȡ�û��˵���Ϣ
	int  static getBillDatas(CGlobal *gbl, string datas); //��ȡ�˵���Ϣ
	int static pack(string m_strTransCode,string &strReq1, map<string, string> &_mapData, string &_strErr);
	int static getCardTaxBills( string datas,CGlobal gbl);
public:

	string strReq0; //����1
	string strReq1; //����2


private:
	string m_strTransCode;//������
	CGlobal gbl;


};

#endif // !defined(AFX_xmlParser_H__EF6864A0_8E88_4116_A187_B59072B3390D__INCLUDED_)
