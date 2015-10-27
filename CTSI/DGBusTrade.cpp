// DGBusTrade.cpp: implementation of the CDGBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "DGBusTrade.h"
#include "Def.h"
#include "Global.h"
#include <ctime> 
#include <winsock2.h>
#include "string.h"
#include "TSCTSI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDGBusTrade::CDGBusTrade()
{ 
	bbzero(HOSTNAME);
	PORT = 0;
	strcpy(HOSTNAME, DGBUS_IP);		//����
	PORT = DGBUS_PORT;
	m_time = 90;
}

CDGBusTrade::~CDGBusTrade()
{

}

int CDGBusTrade::conToHost()
{
	int sd;
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
	
    wrLog("��������%s,%d����",HOSTNAME,PORT);
	
    //��������
	if ( (	sd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) == INVALID_SOCKET )
	{
        wrLog("socket��ʼ��ʧ��");
        return -1;
	}
	
	
	if ((hostEnt = gethostbyname(HOSTNAME)) == NULL)
	{ /* get the host info */
		wrLog("��ȡ��������Ϣ����");
		closesocket(sd);
		return -1;
	}
	
	their_addr.sin_family = AF_INET;	
	their_addr.sin_port = htons(PORT);	
	their_addr.sin_addr = *((struct in_addr *)hostEnt->h_addr);
	memset( their_addr.sin_zero , 0, 8 );//sizeof(their_addr.sin_zero)	
	
	if ( connect(sd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr) ) == -1) 
	{
		wrLog("���ӷ���������");
		closesocket(sd);
		return -1;
	}
	wrLog("Connect OK!");
	return sd;
}

//---------------------------------------------------------------------------
//������:       BusHostSendRecvData
//����������    �򹫽������������󣬲�����Ӧ����Ϣ
//���룺	sendData�����ͱ������ݣ�sendLen�����ͱ��ĳ��ȣ�recvLen�����ձ��ĳ���
//�����	recvData�����ձ���
//���أ�	0���ɹ�������������
//����SOCK��ͬ�������ӷ�ʽ������ͨѶ��������

int CDGBusTrade::BusHostSendRecvData(int overTime)
{
    int iRet,i,j;
	int sd;	
	char recvData[1024];
	char stan[12+1];
	int recvLen=0,length,sendLen=0;
	time_t now;
	int timediff = 0;
	int sign_flag=0;
	CTools tool;
	char dtime[6+1], date[8+1], data_front[128], data_back[128];
	char data[512], temp_buff[1024],tmp_buff[1024],send_buff[1024];
	char respond_code[4+1];
	char crc16[4+1];
	int index=1;
	for (i=1;i<MAX_BUS_ORDER;i++)
	{
		bus_order[i].flag = 0;
		bus_order[i].data_len = 0;
		bbzero(bus_order[i].data);
		bbzero(bus_order[i].stan);
	}
	while (1)
	{
		if (sign_flag == 0)
		{			
			bbzero(dtime);
			bbzero(data);
			bbzero(data_front);
			bbzero(data_back);
			bbzero(data);
			bbzero(temp_buff);
			tool.get_date8(date);
			tool.get_time(dtime);
			strcpy(data_front,"7E7E7E0000000180A8000000520031076900010001000001");
			strcpy(data_back,"01030000000000000000000000000000000000000000000000000000000000");
			sprintf(data,"%s%s%s%s",data_front,date,dtime,data_back);
			length = strlen(data);
			length = tool.hex_to_byte((const BYTE *)data,(BYTE *)temp_buff,length);
			tool.crc_cu_16((BYTE *)temp_buff,length);
			bbzero(bus_order[0].data);
			tool.byte_to_hex((const BYTE *)temp_buff,(BYTE *)bus_order[0].data,length+2);
			length = strlen(bus_order[0].data);
	        bus_order[0].data_len = length;
			bus_order[0].flag = 1;
			bbzero(bus_order[0].stan);
			m_time = overTime;
			if( (sd = conToHost()) < 0)
			{
				g_socket = sd;
				sign_flag = 0;
				closesocket(g_socket);
				wrDGBLog("��ݸ��������������ʧ��");
				Sleep(1000*5);
			}
			else{
				g_socket = sd;
				bbzero(send_buff);
				tool.hex_to_byte((const BYTE *)(bus_order[0].data),(BYTE *)send_buff,bus_order[0].data_len);
				sendLen = ((bus_order[0].data_len)+(bus_order[0].data_len)%2)/2;
				//wrDGBLog("����ǩ������[%d]��%s",bus_order[0].data_len,bus_order[0].data);
				iRet = BussndMsg(sd,send_buff,sendLen);
				if (iRet<=0)
				{
					sign_flag = 0;
					closesocket(g_socket);
					wrDGBLog("��ݸ����ǩ�����ݷ���ʧ��");
					Sleep(1000*5);
				}
				else{
					Sleep(1);
					bbzero(recvData);
					iRet = sign_BusrcvMsg(sd,recvData,sizeof(recvData));
					if (iRet<=0)
					{
						sign_flag = 0;
						closesocket(g_socket);
						wrDGBLog("��ݸ����ǩ�����ݽ���ʧ��");
						Sleep(1000*5);
					}
					else{
						bbzero(bus_order[0].data);
						tool.byte_to_hex((const BYTE *)recvData,(BYTE *)(bus_order[0].data),iRet);
						length = strlen(bus_order[0].data);
						bus_order[0].data_len = length;
						bus_order[0].flag = 3;
						bbzero(bus_key.bus_charge_key);
						bbzero(bus_key.bus_discharge_key);
						bbzero(respond_code);
						bbzero(crc16);
						bbzero(temp_buff);
						memcpy(temp_buff,bus_order[0].data,62);
						memcpy(crc16,bus_order[0].data+62,4);
						//wrDGBLog("����ǩ������[%d]��%s",length,bus_order[0].data);
						if (crc_checkout(temp_buff,crc16)<0)
						{
							sign_flag = 0;
							closesocket(g_socket);
							wrDGBLog("��ݸ����ǩ�����ݽ���CRC16У�����");
							Sleep(1000*5);
						}else{
							memcpy(respond_code,bus_order[0].data+58,4);
							if (memcmp(respond_code,"9000",4)==0)
							{
								
								sign_flag = 1;
								memcpy(bus_key.bus_charge_key,bus_order[0].data+34,8);
								memcpy(bus_key.bus_discharge_key,bus_order[0].data+42,16);
								wrDGBLog("��ݸ����ǩ���ɹ�");
							}else
							{
								sign_flag = 0;
								closesocket(g_socket);
								wrDGBLog("��ݸ����ǩ��ʧ��");
								Sleep(1000*5);
							}
						}						
					}					
				}				
			}			
		}else{
			for (i=1;i<MAX_BUS_ORDER;i++)
			{
				time(&now);
				if (bus_order[index].flag != 0)
				{
					timediff = static_cast<int> ( difftime(now,bus_order[index].inTime) );
				}else
					timediff = 0;
				
				if ((timediff > 90)&&(bus_order[index].flag != 0))
				{
					bus_order[index].flag = 0;
					bus_order[index].data_len = 0;
					bbzero(bus_order[index].data);
					bbzero(bus_order[index].stan);
				}else{
					if ((bus_order[index].flag == 1)&&(bus_order[index].data_len>0))
					{
						bbzero(send_buff);
						sendLen = tool.hex_to_byte((const BYTE *)(bus_order[index].data),(BYTE *)send_buff,bus_order[index].data_len); 
						iRet = BussndMsg(sd,send_buff,sendLen);
						if (iRet>0)
						{
							wrDGBLog("�������ݣ����ڴ��ţ�%d����STAN��%s����DATA[%d]��%s��",index,bus_order[index].stan,bus_order[index].data_len,bus_order[index].data);
							bus_order[index].flag = 2;
							time(&(bus_order[index].inTime));

							index = index%799 + 1;
							Sleep(1);
							break;
						}else
						{
							sign_flag = 0;
							wrDGBLog("��������ʧ�ܣ����ڴ��ţ�%d����STAN��%s����DATA[%d]��%s��",index,bus_order[index].stan,bus_order[index].data_len,bus_order[index].data);
							closesocket(g_socket);
							break;
						}
					}else
					{
						;
					}
				}
				index = index%799 + 1;			
			}
			iRet = BusrcvMsg( sd, recvData, sizeof(recvData)); 
			recvLen = iRet;
			if (iRet > 0)
			{
				bbzero(stan);
				bbzero(respond_code);
				bbzero(crc16);
				bbzero(temp_buff);
				bbzero(tmp_buff);
				tool.byte_to_hex((const BYTE *)recvData,(BYTE *)tmp_buff,recvLen);
				memcpy(temp_buff,tmp_buff,recvLen*2-4);
				memcpy(crc16,tmp_buff+recvLen*2-4,4);
				if (crc_checkout(temp_buff,crc16)==0)
				{
					memcpy(stan,temp_buff+26,12);
					for (j=1;j<MAX_BUS_ORDER;j++)
					{
						if (memcmp(bus_order[j].stan,stan,12)==0)
						{
							bbzero(bus_order[j].data);
							strcpy(bus_order[j].data,tmp_buff);
							bus_order[j].data_len = strlen(bus_order[j].data);
							bus_order[j].flag = 3;
							wrDGBLog("�������ݣ����ڴ��ţ�%d����STAN��%s����DATA[%d]��%s��",j,bus_order[j].stan,bus_order[j].data_len,bus_order[j].data);
						}else
						{
							;
						}
					}
				}else
				{
					wrDGBLog("���ڴ��ţ�%d����STAN��%s�� CRC16У�����",j,bus_order[j].stan);
				}
			}else if (iRet ==  0)
			{
				sign_flag = 0;
				closesocket(g_socket);
				Sleep(1);
			}else if (iRet == -1 )
			{
				sign_flag = 0;
				closesocket(g_socket);
				Sleep(1);
			}else
			{
				Sleep(1);
			}
		}
		
	}
	closesocket(g_socket);
	return 0;
}


int CDGBusTrade::BussndMsg(const int sd,char *sendData,int sendLen)
{
	int iRet;
	fd_set  fdW;
	struct timeval timeout;
	timeout.tv_sec = 40;	//���ó�ʱ�˳�ʱ��Ϊ40��
	timeout.tv_usec = 0;
	FD_ZERO(&fdW);
	FD_SET((const unsigned int)sd, &fdW);
	
	switch (select(sd+1, NULL, &fdW, NULL,&timeout)) 
	{ 
	case -1: 
		{
			wrDGBLog("����ʧ��");
			return -1;//error handled by u; 
		}
	case 0: 
		{
			wrDGBLog("���ͳ�ʱ");
			return -2;//timeout hanled by u; 
		}
	default: 
		if( FD_ISSET(sd, &fdW) )
		{
			wrDGBusHex("sendData(hex)", (BYTE *)sendData,sendLen );
			iRet=send( sd, sendData, sendLen, 0 ) ;
			wrDGBLog("��������[%d]",iRet);
			return iRet;
		}
		
	} 
	return 0;
}


int CDGBusTrade::sign_BusrcvMsg(const int sd,char * recvData,int recvLen)
{
	int iRet;
	fd_set  fdR;
	struct timeval timeout;
	timeout.tv_sec = m_time;	//���ó�ʱ�˳�ʱ��(��λ����)
	timeout.tv_usec = 0;
	FD_ZERO(&fdR); 
	FD_SET((const unsigned int)sd, &fdR); 
	
	switch (select(sd+1, &fdR, NULL, NULL,&timeout)) 
	{
	case -1: 	//error
		{
			wrDGBLog("����ʧ��");
			return -1; 
		}
	case 0:	//timeout 
		{
		//	wrDGBLog("���ճ�ʱ");
			return -3;
		}
	default: 
		if (  FD_ISSET(sd, &fdR) ) 
		{	
			iRet = recv( sd, recvData, recvLen, 0 ); 
			wrDGBLog("���ճ���[%d]",iRet);		
			if(DEBUG_LEVEL < 2){
				wrDGBusHex("��������(hex)", (BYTE *)recvData, iRet);
			}
			return iRet;
		}
	}
	return -1;
}

int CDGBusTrade::BusrcvMsg(const int sd,char * recvData,int recvLen)
{
	int iRet;
	fd_set  fdR;
	struct timeval timeout;
	timeout.tv_sec = 0;	//���ó�ʱ�˳�ʱ��(��λ����)
	timeout.tv_usec = 100;
	FD_ZERO(&fdR); 
	FD_SET((const unsigned int)sd, &fdR); 
	
	switch (select(sd+1, &fdR, NULL, NULL,&timeout)) 
	{ 
	case -1: 	//error
		{
			wrDGBLog("����ʧ��");
			return -1; 
		}
	case 0:	//timeout 
		{
			//	wrDGBLog("���ճ�ʱ");
			return -3;
		}
	default: 
		if (  FD_ISSET(sd, &fdR) ) 
		{
			iRet = recv( sd, recvData, recvLen, 0 );
			wrDGBLog("���ճ���[%d]",iRet);	
			if(DEBUG_LEVEL < 2){
				if (iRet>0)
				{
					wrDGBusHex("��������(hex)", (BYTE *)recvData, iRet);
				}
			}
			
			return iRet;			
		}
	}
	return -1;
}

int CDGBusTrade::crc_checkout(char *buff,char *crc_Code)
{
	   char t_crc16[4+1],length;
	   char tmpstr[1024],tmp_buff[1024];
	   CTools tool;
	   memset(tmp_buff,0,sizeof(tmp_buff));
	   memset(tmpstr,0,sizeof(tmpstr));
	   length=tool.hex_to_byte((const BYTE *)buff,(BYTE *)tmp_buff,strlen(buff));
	   tool.crc_cu_16((BYTE *)tmp_buff,length);
	   tool.byte_to_hex((const BYTE *)tmp_buff, (BYTE *)tmpstr, length+2);
	   memset(t_crc16,0,sizeof(t_crc16));
	   length=strlen(buff);
	   memcpy(t_crc16,tmpstr+length,4);
	   
	   if(strcmp(t_crc16,crc_Code))
		   return -1;
	   else
		   return 0;	   
}


//�ֻ����� by liuxiao 20130401 ��ݸHID�豸

UCHAR CDGBusTrade::getXor(UCHAR *Data, int DataLen)
{
	UCHAR Chk = 0;
	while (DataLen--)
		Chk ^=Data[DataLen];
	return Chk;

}

/********************************************
//by liuxiao ��ݸHID�豸 
sData�����͵����ݰ�
sDataLen:���͵����ݰ�����
dData: ������
DatatLen: �����򳤶�
********************************************/

int CDGBusTrade::Package0203(UCHAR *sData,UCHAR *dData,int DataLen)
{
	sData[0] = 0x02; //����ͷ
	if (DataLen>512) return -1;
	sData[1] = DataLen / 0x100;  //���ĳ���
	sData[2] = DataLen % 0x100;
	memcpy(sData+3,dData,DataLen);
	sData[3+DataLen] =getXor(sData+3,DataLen); //�������
	sData[4+DataLen] = 0x03;  //����β
	return 0; 
	
}

/****************************************************
*2010-11-09 kevinzhao
*����Э����
*return 0 =sucess,
****************************************************/
int CDGBusTrade::unPackage0203(UCHAR *sData,UCHAR *dData,int *DataLen) 
{
	if (sData[0]!=0x02) return -1;
	*DataLen = sData[1] * 0x100 + sData[2];
	if (*DataLen>1300) return -1;
	if (sData[*DataLen+4]!=0x03) 
		return -1;
	if (sData[*DataLen+3]!=getXor(sData+3,*DataLen)) return -2; //����macУ��
	memcpy(dData,sData+3,*DataLen);
	return 0;
}


