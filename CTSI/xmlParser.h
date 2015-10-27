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
	int m_iIsCN; //是否有中文字符
    char *m_strKey; //mapTransInfo中的key
    char *m_strAttr;//xml中的属性名 
    char * m_strSelf;//xml中的节点名
    char *m_strParent;//xml中的父节点名
    char *m_strGrant;//xml中的祖父节点名
}field_cfg;


class CxmlParser  
{

public:
	CxmlParser();
	virtual ~CxmlParser();

	/************************************************************************/
	/* packAreaQryxml
	OUTTMNNUMNO 外部终端号 8 
	ACCEPTAREACODE 受理区域编码 6 
	ACCEPTDATE 受理时间  14 YYYYMMDDhhmmss                                                                      
	/************************************************************************/

	int static packAreaQryxml(CGlobal gbl,char *OUTTMNNUMNO,char * areaCode,char *ACCEPTDATE, string &strReq0,string &strReq1,char * WEBSVRNAME,char *WEBSVRCODE); 

	/************************************************************************/
	/*
	packSDMQryxml，packSDMQryxml
	ORDERSEQ 订单号 32 参考规则:机构号+终端号+YYMMDDHHMMSS+随机数
	OUTTMNNUMNO 外部终端号 8 
	SELECTTYPE 查询类型 查询类型：001:用户号 002:条形码
	ACCEPTDATE 受理时间  YYYYMMDDhhmmss
	ACCEPTAREACODE 受理区域编码 6 
	ADDITEM1 16 缴费省代码（6位）+缴费城市代码（6位）+缴费项目编号（4位）
	ADDITEM2 查询金额  上海水和煤气要求必须输入 
                
	/************************************************************************/
	int static  packSDMQryxml(CGlobal gbl,char * WEBSVRNAME,char *WEBSVRCODE,char *OUTTMNNUMNO,char *SELECTTYPE,char *SELECTVALUE,char *ACCEPTDATE,char *ACCEPTAREACODE,char *ADDITEM1,char *ADDITEM2,string &strReq0,string &strReq1 ) ;//上海水费和煤气使用

	/************************************************************************/
	/* packSDMPayxml
	ORDERSEQ 32 交易发起方流水号 
	OUTTMNNUMNO 8 
	ACCEPTDATE 受理时间 14  YYYYMMDDhhmmss 
	SYSTEMNO 系统参考号 15  
	TXNAMOUNT 交易金额 14 
	CASHTYPE 销账单号类型 0-条码 1-缴费单号 
	PAYTYPE 支付方式 0 企业账户 1 POS代收付默认企业账户 0
	OPERUSER 企业账户操作员 64 支付方式为企业账户时填写 
	OPERPASSWORD 企业账户支付密码 128 支付方式为企业账户时填写                                                                    
	/************************************************************************/
	int static packSDMPayxml(CGlobal gbl,char *OUTTMNNUMNO,char *WEBSVRNAME,char *WEBSVRCODE,char *ACCEPTDATE,char *SYSTEMNO,char *TXNAMOUNT,int CASHTYPE,char * CASHORDER,char *CASHNUMBER,int PAYTYPE,char *OPERUSER,char *OPERPASSWORD,string &strReq0,string &strReq1);
	/************************************************************************/
	/*  packSDMPayRevxml          
	RESPONSECODE 响应码 6 响应码为000000表示成功，其他为失败
	RESPONSECONTENT 响应码描述 
	OUTTMNNUMNO 外部终端号 
	ORDERSEQ 订单号 发起方生成的流水号 
	TRANSSEQ 交易流水号 15
	SYSTEMNO 系统参考号  15
	TXNAMOUNT 交易金额 以分为单位,账单的金额 
	CASHTYPE 销账单号类型 0-条码1-缴费单号
	/************************************************************************/
	int static packSDMPayRevxml(string strReq0,string strReq1,char *ORDERSEQ,char *TRANSSEQ,char *TXNAMOUNT,char *CASHTYPE);
	void static  UTF8toANSI(string &strUTF8);
	void static  ANSItoUTF8(string &strAnsi);
	int  static unpack(const string &_strSrc,string m_strTransCode, map<string, string> &_mapData, string &_strErr);
	const  field_cfg * get_cfg(int _iReqOrRsp,string m_strTransCode); 
	int static getAreaDatas( string datas); //获取地区
	int    static getAreaDatas( string datas,CGlobal gbl);
	int static getBills( string datas,CGlobal gbl) ; //获取用户账单信息
	int  static getBillDatas(CGlobal *gbl, string datas); //获取账单信息
	int static pack(string m_strTransCode,string &strReq1, map<string, string> &_mapData, string &_strErr);
	int static getCardTaxBills( string datas,CGlobal gbl);
public:

	string strReq0; //参数1
	string strReq1; //参数2


private:
	string m_strTransCode;//交易码
	CGlobal gbl;


};

#endif // !defined(AFX_xmlParser_H__EF6864A0_8E88_4116_A187_B59072B3390D__INCLUDED_)
