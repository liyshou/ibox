// Connect.cpp: implementation of the CConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "Connect.h"
#include "packet.h"
#include "Def.h"
#include <winsock2.h>
#include "string.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXQUEUE 512

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnect::CConnect()
{
	bbzero(HOSTNAME);
	if(TRADE_FLAG)  //����
		strcpy(HOSTNAME, "132.97.117.210");
	else   //����
		strcpy(HOSTNAME, "132.97.42.24");
	PORT = 4100;
}

CConnect::~CConnect()
{
	
}


//---------------------------------------------------------------------------
//������:       HostSendRecvData
//����������    ���������ͽ������󣬲�����Ӧ����Ϣ
//���룺	sendData�����ͱ������ݣ�sendLen�����ͱ��ĳ��ȣ�recvLen�����ձ��ĳ���
//�����	recvData�����ձ���
//���أ�	0���ɹ�������������
//����SOCK��ͬ�������ӷ�ʽ������ͨѶ��������

int CConnect::HostSendRecvData(char *sendData,int sendLen,char * recvData,int recvLen,int timeOut)
{
    int iRet;
	SOCKET sd;
	char pack_buff[4+1];
	int rev_len=0, pack_len=0;

	if(strcmp(PE_IP, ""))	//��Ϊ��
		strcpy(HOSTNAME, PE_IP);
	if(PE_PORT)				//��0
		PORT = PE_PORT;

	if( conToHost(HOSTNAME, PORT, &sd)==0 )
	{
		NULL;
	}
	else
	{
		return 1;
	}
	iRet = sndMsg(sd, sendData, sendLen);
	if(iRet == -1)
		return 2;
	else if(iRet == -2)
		return 3;
	if(!timeOut)
	   iRet = rcvMsg(sd, recvData, &rev_len, 1024);
	else
       iRet = rcvMsg(sd, recvData, &rev_len, 1024,timeOut);
	closesocket(sd);
	if(iRet == -1)
		return 4;
	else if(iRet == -3)
		return 5;

	CTools::byte_to_hex((BYTE *)recvData, (BYTE *)pack_buff, 2);
	pack_len =atol(pack_buff);
	if(pack_len != (rev_len - 2))	//�յ��������İ� 
		return 6;
	return 0;
}


int CConnect::conToHost(char *myHostName, short myPort, SOCKET *sd)
{
	struct sockaddr_in their_addr;
	struct hostent *hostEnt = NULL;

	WSADATA   wsaData;   
	WORD   version   =   MAKEWORD(2,0);   
	int   ret   =   WSAStartup(version,&wsaData);   
	if(ret   !=   0)   
	{   
		wrLog("��ʼ��ʧ��");   
		return -1;
	} 
    	
    wrLog("��������%s,%d����",myHostName,myPort);
	
    //��������
	if ( (	*sd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) == INVALID_SOCKET )
	{
        wrLog("socket��ʼ��ʧ��");
        return -1;
	}
	
	
	if ((hostEnt = gethostbyname(myHostName)) == NULL)
	{ /* get the host info */
		wrLog("��ȡ��������Ϣ����");
		return -1;
	}
	
	their_addr.sin_family = AF_INET;	
	their_addr.sin_port = htons(myPort);
	their_addr.sin_addr = *((struct in_addr *)hostEnt->h_addr);
	memset( their_addr.sin_zero , 0, 8 );//sizeof(their_addr.sin_zero)	

	if ( connect(*sd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr) ) == -1) 
	{
		wrLog("���ӷ���������");
		return -1;
	}

	wrLog("Connect OK!");
	return 0;
}


int CConnect::sndMsg(SOCKET sd,char *sendData,int sendLen, int timeOut)
{
	int iRet;
	fd_set  fdW;
	struct timeval timeout;
	if(!timeOut)
		timeout.tv_sec = 90;	//���ó�ʱ�˳�ʱ��Ϊ90��
	else
		timeout.tv_sec = timeOut;
	timeout.tv_usec = 0;
	FD_ZERO(&fdW);
	FD_SET(sd, &fdW);
	
	switch (select(sd+1, NULL, &fdW, NULL,&timeout)) 
	{ 
	case -1: 
		{
			wrLog("����ʧ��");
			closesocket(sd);
			return -1;//error handled by u; 
		}
	case 0: 
		{
			wrLog("���ͳ�ʱ");
			closesocket(sd);
			return -2;//timeout hanled by u; 
		}
	default: 
		if( FD_ISSET(sd, &fdW) )
		{
			iRet=send( sd, sendData, sendLen, 0 ) ;
			wrLog("��������[%d]",iRet);
			//wrHex("���Ͱ�����", (BYTE*)sendData, iRet);
			return 0;
		}
		else
			return -3;
	} 
}

int CConnect::rcvMsg(SOCKET sd, char * recvData, int *recvLen, int recvMaxLen, int timeOut)
{
	fd_set  fdR;
	struct timeval timeout;
	if(!timeOut)
		timeout.tv_sec = 90;	//���ó�ʱ�˳�ʱ��(��λ����)
	else
		timeout.tv_sec = timeOut;
	timeout.tv_usec = 0;
	FD_ZERO(&fdR); 
	FD_SET(sd, &fdR); 

	switch (select(sd+1, &fdR, NULL, NULL,&timeout)) 
	{ 
	case -1: 
		{
			wrLog("����ʧ��");
			return -1;//error handled by u; 
		}
	case 0: 
		{
			int id_temp = WSAGetLastError();
			wrLog("unknown error id = %d", id_temp); 
	
			wrLog("���ճ�ʱ");
			return -3;//timeout hanled by u; 
		}
	default: 
		if (  FD_ISSET(sd, &fdR) ) 
		{
			*recvLen = recv( sd, recvData, recvMaxLen, 0 ); // �޸�ΪreceiveLEN
			if( *recvLen <= 0 )
			{
				int id_temp = WSAGetLastError();
				wrLog("unknown error id = %d", id_temp); 

				//wrHex("���ذ�����",(BYTE*)recvData, *recvLen);
				return -1;
			}
			else
			{
				//wrHex("���ذ�����",(BYTE*)recvData, *recvLen);
				return 0;
			}			
		}
		else
			return -3;
	}
}


int CConnect::srvListen(SOCKET &listen_sd, unsigned int port)
{
	int bin_sd;
	struct sockaddr_in sev_addr;
	
	WSADATA   wsaData;   
	WORD   version   =   MAKEWORD(2,0);   
	int   ret   =   WSAStartup(version,&wsaData);   
	if(ret   !=   0)   
	{   
	  wrLog("���������ʼ��ʧ��");   
	  return -1;
	} 

//	memset(&sev_addr,0,sizeof(sev_addr));
	if((listen_sd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		wrLog("��������socketʧ��");
		return -2;	
	} 
	else
		wrLog("��������socket�ɹ�");
	
	memset(&sev_addr,0,sizeof(sev_addr));
	sev_addr.sin_family = AF_INET;
	sev_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sev_addr.sin_port = htons(port);
	
	if((bin_sd=bind(listen_sd,(struct sockaddr *)&sev_addr,sizeof(sev_addr)))<0)
	{
		wrLog("�����˿�[%d]ʧ��", port);
		closesocket(listen_sd);
		return -3;
	}
	else
		wrLog("�����˿�[%d]�ɹ�", port);
	listen(listen_sd,MAXQUEUE);	
	return 0;
}


//---------------------------------------------------------------------------
//������:       xltHostSendRecvData
//����������    ���������ͽ������󣬲�����Ӧ����Ϣ
//���룺	sendData�����ͱ������ݣ�sendLen�����ͱ��ĳ��ȣ�recvLen�����ձ��ĳ���
//�����	recvData�����ձ���
//���أ�	0���ɹ�������������
//����SOCK��ͬ�������ӷ�ʽ������ͨѶ��������

int CConnect::xltHostSendRecvData(char *sendData,int sendLen,char * recvData,int recvLen,char *myIP, int myPort)
{
    int iRet=0, rev_len=0;
	SOCKET sd;
	if( conToHost(myIP,myPort,&sd)==0 )
	{
		NULL;		
	}
	else
	{
		return 1;
	}
	iRet = sndMsg(sd,sendData,sendLen);
	if(iRet == -1)
		return 2;
	else if(iRet == -2)
		return 3;
	
	iRet = rcvMsg(sd,recvData, &rev_len,1024);
	closesocket(sd);
	if(iRet == -1)
		return 4;
	else if(iRet == -3)
		return 5;
	
	return 0;
}



int CConnect::getIP(char * ip)
{
	char m_sHostName[100];
	char m_sIPAddress[50];
	bbzero(m_sIPAddress);
	bbzero(m_sHostName);

	int nRetCode;	
	nRetCode = StartUp();
	if(nRetCode==0)
		nRetCode = GetLocalHostName(m_sHostName);
	if(nRetCode==0)
		nRetCode = GetIPAddress(m_sHostName, m_sIPAddress);
	if(nRetCode==0)
		nRetCode = CleanUp();
	if(nRetCode==0)
		memcpy(ip, m_sIPAddress, strlen(m_sIPAddress));
	return nRetCode;
}


int CConnect::GetLocalHostName(char *sHostName)
{
	
	char szHostName[256];
	int  nRetCode;
	bbzero(szHostName);
	nRetCode = gethostname(szHostName, sizeof(szHostName));
	
	if (nRetCode != 0) {
		sHostName = _T("Not available");;
		return WSAGetLastError();
	}
	
	strcpy(sHostName,szHostName);
	return 0;
}


int CConnect::GetIPAddress( char *sHostName, char *sIPAddress)
{
	struct hostent FAR *lpHostEnt = gethostbyname (sHostName);
	
	if (lpHostEnt == NULL) {
		sIPAddress = _T("");
		return WSAGetLastError();
	}
	
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];
	if (lpAddr) {
		struct in_addr  inAddr;
		memmove (&inAddr, lpAddr, 4);
		strcpy(sIPAddress,inet_ntoa (inAddr));
	//	sIPAddress = inet_ntoa (inAddr);
		if (strcmp(sIPAddress,"")==0)
			sIPAddress = _T("Not available");
	}	
	return 0;
}


int CConnect::CleanUp()
{
	int nRetCode;	
	nRetCode = WSACleanup();
	if (nRetCode != 0) {
		return WSAGetLastError();
	}	
	return 0; 
}

int CConnect::StartUp()
{
	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err; 
	
	wVersionRequested = MAKEWORD( 2, 0 ); 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) { 
		return err;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 0 ) {
		WSACleanup( );
		return WSAVERNOTSUPPORTED; 
	}
	return 0;
}
