// ZHBusTrade.h: interface for the ZHBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHBUSTRADE_H__025FD6E2_7134_4CAF_9858_975D920BD106__INCLUDED_)
#define AFX_ZHBUSTRADE_H__025FD6E2_7134_4CAF_9858_975D920BD106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "Global.h"
#include "Pack.h"
#include "Trade.h"

class ZHBusTrade  
{
public:
	ZHBusTrade();
	virtual ~ZHBusTrade();

	int static ZHTrafficCardCharge(CGlobal gbl);

// 	int static zhBusEnquiry(CGlobal gbl);
// 	int static zhBusCharge();
// 	int static zhBusRegister();
// 	int static zhBusLogin();
// 	int static zhBusGetError();
// 	int static zhBusChargeReverse();

private:
// 	CGlobal gbl;
// 	CPack pack;
// 	CTrade trade;
// 	char m_ZHBusCard[9+1]; //珠海公交票卡逻辑卡号
// 	char m_stan[8+1]; //系统流水号
// 	char m_balance[12]; //羊城通余额
// 	char m_stat[4+1];	//交易状态标识
// 	char timeStamp[6+1]; //校验时间戳

	// 刷卡输密码向PE发扣费包
	// 入参：biz_type(4):业务代码；amount(12):金额
	// 出参：无
	// 返回值
	// 0：扣费成功
	// 1：扣费失败
	// 2：超时返回
	int static PayPe(CGlobal *pgbl, char *card_no);

	//包处理函数:打包、发包、收包和解包
	// 入参：flag:2:交费包;4:确认包;5:冲正包
	// 出参：无
	// 返回值
	// 0：成功
	// 非0：失败
	int static packHdl(CGlobal gbl, CPack *ppack, int flag);

	// 确认交易
	// 入参：yct_data:羊城通交易信息
	// 出参：无
	// 返回值：
	// 0：已确认
	// 1：未确认
	int static AckTrade(CGlobal gbl, char *card_no,char *serial,char *balance,char *deviceId);
	
	// 冲正交易
	// 入参：yct_data:羊城通交易信息
	// 出参：无
	// 返回值：
	// 0：已冲正
	// 1：未冲正
	int static RevTrade(CGlobal gbl, char *card_no);

};

#endif // !defined(AFX_ZHBUSTRADE_H__025FD6E2_7134_4CAF_9858_975D920BD106__INCLUDED_)
