#include "YCT3Trade.h"
//���ͨ��ֵ���������뽻��ǰ��׼��
//���� ��������ʼ�����ն�ǩ����֤�����ͼ������
int CYCT3Trade::Yct3_Init(CGlobal gbl,YCT_DATA yct_data)
{
	if (CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data) != 0)
	{
		return 1;
	}

	//���ͼ������
// 	unsigned char monitor[114]    = { 0 };
// 	unsigned char setmonitor1[87] = { 0 };
// 	unsigned char setmonitor2[256]= { 0 };
// 	unsigned char byteTmp[256]    = { 0 };
// 	char termVer[16+2] = { 0 };//�ն�����汾��
// 	int setFlag = 0;
// 	int bufLen  = 0;
// 	int iRet	= 0;
// 	int rcvLen	= 0;
// 	int upfreq  = 0;
// 	int iSelect = 0;
// 	
// 	//SET_MONITOR�׶�1����ͷ
// 	CTools::hex_to_byte( (BYTE*)"FD03" , byteTmp , 4);
// 	memcpy(setmonitor1 + bufLen , byteTmp , 2);
// 	bufLen += 2;
// 	CTools::hex_to_byte( (BYTE*)"0002",  byteTmp, 4);
// 	memcpy(setmonitor1 + bufLen , byteTmp , 2);
// 	bufLen += 2;
// 	setmonitor1[4] = 0x00;
// 	setmonitor1[5] = 0x80;
// 	bufLen += 2;
// 	sprintf((char*)byteTmp , "%04X" , 79);
// 	CTools::hex_to_byte(byteTmp , setmonitor1 + bufLen  , 4 );
// 	bufLen += 2;
// 	//����
// 	CTools::hex_to_byte((BYTE*)yct_data.SHID , byteTmp , 16);
// 	memcpy(setmonitor1 + bufLen , byteTmp , 8);
// 	bufLen += 8;
// 	bufLen += 1;//�����汾��ȡ0
// 	CTools::hex_to_byte((BYTE*)yct_data.pki_card , byteTmp , 8);
// 	memcpy(setmonitor1 + bufLen , byteTmp , 4);
// 	bufLen += 4;
// 	bufLen += 2;//�ն�����ȡ0000
// 	bufLen += 64;//RFU����
// 	
// 	wrHex("������÷��ͱ��ģ�" , setmonitor1 , bufLen);
// 	//ͨѶ���ͨ������
// 	CConnect con;
// 	SOCKET sd;
// 	iRet = con.conToHost(YCT_IP , 1111 , &sd);
// 	if (iRet != 0 )
// 	{
// 		return 1;
// 	}
// 	unsigned char rcvData[256] = { 0 };
// 	iRet = CYCTYPTrade::ST_commuWithYCTIC3(gbl.dPosInfo, &yct_data , gbl.com , gbl.comType ,sd , setmonitor1 , bufLen ,  setmonitor2 , &rcvLen);
// 	if (iRet != 0)
// 	{
// 		return 1;
// 	}
// 	char zeroQ[4] = { 0 };
// 	if (memcmp(setmonitor2 + 20 , zeroQ , 4) != 0)
// 	{
// 		return 1;	//���������
// 	}
// 	wrHex("������÷��ر��ģ�" , setmonitor2 , rcvLen);
// 
// 	bbzero(byteTmp);
// 	CTools::byte_to_hex(setmonitor2 + 24 , byteTmp , 2);
// 	strcpy(gbl.monitor_frequency,(char *)byteTmp);
// 	upfreq = atol((char*)byteTmp);
// 	bbzero(byteTmp);
// 	CTools::byte_to_hex(setmonitor2 + 24+2 , byteTmp , 7);
// 	strcpy(gbl.next_monitortime,(char *)byteTmp);
// 	bbzero(byteTmp);
// 	CTools::byte_to_hex(setmonitor2 + 24+2+7, byteTmp , 64);
// 	strcpy(gbl.announce,(char *)byteTmp);
// 	memcpy(termVer , setmonitor2 + 97 , 16);
// 	strcpy(gbl.term_ver,(char *)termVer);
// 
// 	wrYct3MonitorLog("�ն˼�����÷�����Ϣ");
// 	wrYct3MonitorLog("���Ƶ�ʣ�%s",gbl.monitor_frequency);
// 	wrYct3MonitorLog("�´θ���ʱ�䣺%s",gbl.next_monitortime);
// 	char strtemp[1024];
// 	bbzero(strtemp);
// 	CTools::tobyte(gbl.announce,strtemp);
// 	wrYct3MonitorLog("������Ϣ��%s",strtemp);
// 	wrYct3MonitorLog("�ն�����汾�ţ�%s",gbl.term_ver);
	return 0;
}

//������
//tradeType: 0:�Զ���ֵ 1:���ת�� 2:��ֵ���� 3:��Ƭ��չӦ��
// ����ֵ��
// 0���ɹ�
// 1��ʧ��
int CYCT3Trade::Yct3_TradeHdl(CGlobal gbl,  YCT_DATA yct_data, char *title, int tradeType)
{
	//
	return 0 ;
}





















