// Connect.h: interface for the CConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECT_H__D2B97D30_FAD4_40A4_8540_F845D73E2B18__INCLUDED_)
#define AFX_CONNECT_H__D2B97D30_FAD4_40A4_8540_F845D73E2B18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>

class CConnect  

{

private:
	char HOSTNAME[20];
	int  PORT;
private:

public:
	CConnect();
	virtual ~CConnect();

	int rcvMsg(SOCKET sd, char * recvData, int *recvLen, int recvMaxLen, int timeOut=0);
	int sndMsg(SOCKET sd,char *sendData,int sendLen, int timeOut=0);
	int conToHost(char *myHostName, short myPort, SOCKET *sd);
	int HostSendRecvData(char *sendData,int sendLen,char * recvData,int recvLen,int timeOut=0);
	int xltHostSendRecvData(char *sendData,int sendLen,char * recvData,int recvLen,char *myIP, int myPort);
	int srvListen(SOCKET &listen_sd, unsigned int port);
	int static getIP(char * ip);
	int static GetLocalHostName(char *sHostName);
	int static GetIPAddress(char *sHostName, char *sIPAddress);
	int static CleanUp();
	int  static StartUp();
};

#endif // !defined(AFX_CONNECT_H__D2B97D30_FAD4_40A4_8540_F845D73E2B18__INCLUDED_)
