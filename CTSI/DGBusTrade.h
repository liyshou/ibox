// DGBusTrade.h: interface for the CDGBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGBUSTRADE1_H__47A997F1_DC93_4908_93A0_009B169B27FA__INCLUDED_)
#define AFX_DGBUSTRADE1_H__47A997F1_DC93_4908_93A0_009B169B27FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"

class CDGBusTrade  
{
private:
	int conToHost();
	int BusrcvMsg(const int sd,char * recvData,int recvLen);
	int sign_BusrcvMsg(const int sd,char * recvData,int recvLen);
	int BussndMsg(const int sd,char *sendData,int sendLen);
	int crc_checkout(char *buff,char *crc_Code);
public:
	CDGBusTrade();
	virtual ~CDGBusTrade();
	int BusHostSendRecvData(int overTime=90);
	int srvListen(unsigned int &listen_sd, unsigned int port);
	int static Package0203(UCHAR *sData,UCHAR *dData,int DataLen);
	int static unPackage0203(UCHAR *sData,UCHAR *dData,int *DataLen) ;
	UCHAR  static getXor(UCHAR *Data, int DataLen);
	

private:
	char HOSTNAME[20];
	int  PORT;
	int  m_time;  //³¬Ê±Ê±¼ä
	int g_socket;
};

#endif // !defined(AFX_DGBUSTRADE1_H__47A997F1_DC93_4908_93A0_009B169B27FA__INCLUDED_)
