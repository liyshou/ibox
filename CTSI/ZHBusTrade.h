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
// 	char m_ZHBusCard[9+1]; //�麣����Ʊ���߼�����
// 	char m_stan[8+1]; //ϵͳ��ˮ��
// 	char m_balance[12]; //���ͨ���
// 	char m_stat[4+1];	//����״̬��ʶ
// 	char timeStamp[6+1]; //У��ʱ���

	// ˢ����������PE���۷Ѱ�
	// ��Σ�biz_type(4):ҵ����룻amount(12):���
	// ���Σ���
	// ����ֵ
	// 0���۷ѳɹ�
	// 1���۷�ʧ��
	// 2����ʱ����
	int static PayPe(CGlobal *pgbl, char *card_no);

	//��������:������������հ��ͽ��
	// ��Σ�flag:2:���Ѱ�;4:ȷ�ϰ�;5:������
	// ���Σ���
	// ����ֵ
	// 0���ɹ�
	// ��0��ʧ��
	int static packHdl(CGlobal gbl, CPack *ppack, int flag);

	// ȷ�Ͻ���
	// ��Σ�yct_data:���ͨ������Ϣ
	// ���Σ���
	// ����ֵ��
	// 0����ȷ��
	// 1��δȷ��
	int static AckTrade(CGlobal gbl, char *card_no,char *serial,char *balance,char *deviceId);
	
	// ��������
	// ��Σ�yct_data:���ͨ������Ϣ
	// ���Σ���
	// ����ֵ��
	// 0���ѳ���
	// 1��δ����
	int static RevTrade(CGlobal gbl, char *card_no);

};

#endif // !defined(AFX_ZHBUSTRADE_H__025FD6E2_7134_4CAF_9858_975D920BD106__INCLUDED_)
