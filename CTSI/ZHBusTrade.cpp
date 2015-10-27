// ZHBusTrade.cpp: implementation of the ZHBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "ZHBusTrade.h"
#include "Connect.h"
#include "DB.h"
#include "Print.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZHBusTrade::ZHBusTrade()
{

}

ZHBusTrade::~ZHBusTrade()
{

}


// int ZHBusTrade::zhBusEnquiry()
// {
// 
// }
// 
// int ZHBusTrade::zhBusCharge()
// {
// 
// }
// 
// int ZHBusTrade::zhBusRegister(CGlobal gbl)
// {
// 	//����ע��
// 	bbzero(theCommand);
// 	strcpy(theCommand, "�麣��������|����ע��|");
// 	iRet = TS_SendCommand(gbl.dPosInfo, theCommand);
// 	if(!iRet)
// 	{
// 		wrLog("����ע��ɹ�");
// 	}
// 	else
// 	{
// 		wrLog("����ע��ʧ��:%s", theCommand);
// 		
// 		bbzero(theCommand);
// 		strcpy(theCommand, "�麣��������|������Ϣ|");
// 		iRet = TS_SendCommand(gbl.dPosInfo, theCommand);
// 		if(!iRet)
// 		{
// 			wrLog(theCommand);
// 			iRet = CTools::findChar(theCommand, '|', loc);
// 			if(iRet > loc)//������Ϣ
// 			{
// 				char temp2[100];
// 				bbzero(temp);
// 				memcpy(temp2, theCommand + loc, iRet - loc);
// 				sprintf(temp, "ע��ʧ�ܣ�%s�����Ժ����ԡ�\n����ȷ�ϡ��˳�", temp2);
// 				iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
// 				return -1;
// 			}
// 		}
// 		else
// 		{
// 			wrLog("��ֵʧ��:%s", theCommand);
// 			//��������������������������������������E������������������������������������������������������
// 			iRet = TS_OutPutMsg(gbl.dPosInfo, "������ֵʧ�ܣ����Ժ����ԡ�\n����ȷ�ϡ��˳�", 0, iSelect);
// 			return -1;
// 		}
// 	}
// }
// 
// int ZHBusTrade::zhBusLogin()
// {
// 
// }
// 
// int ZHBusTrade::zhBusGetError()
// {
// 	bbzero(theCommand);
// 	strcpy(theCommand, "�麣��������|������Ϣ|");
// 	iRet = TS_SendCommand(gbl.dPosInfo, theCommand);
// 	if(!iRet)
// 	{
// 		wrLog(theCommand);
// 		iRet = CTools::findChar(theCommand, '|', loc);
// 		if(iRet > loc)//������Ϣ
// 		{
// 			char temp2[100];
// 			bbzero(temp);
// 			memcpy(temp2, theCommand + loc, iRet - loc);
// 			sprintf(temp, "ע��ʧ�ܣ�%s�����Ժ����ԡ�\n����ȷ�ϡ��˳�", temp2);
// 			iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
// 			return -1;
// 		}
// 	}
// }
// 
// int ZHBusTrade::zhBusChargeReverse()
// {
// 
// }


//��������:������������հ��ͽ��
// ��Σ�flag:2:���Ѱ�;4:ȷ�ϰ�;5:������
// ���Σ���
// ����ֵ
// 0���ɹ�
// 1�����ʧ�ܣ��۷�ʧ��
// 2����ʱ�����δ֪
// 3�����ʧ�ܣ����δ֪
int ZHBusTrade::packHdl(CGlobal gbl, CPack *ppack, int flag)
{
	BYTE temp_buff[1024];
	char send_buff[1024], rev_buff[1024+1], len_buff[4+1];
	char seq[10+1]; //���ݿ����ʱ���������к�
	int send_len, iRet, rev_len;
	int i, type, stat;
	CConnect con;	//������
	//���   
	memset(temp_buff,0,sizeof(temp_buff));
	
	send_len = ppack->Comm_Pack(flag, temp_buff);
	
	sprintf(len_buff,"%04d",send_len);
	if (send_len <=0 || send_len>1024)
	{
		wrLog("8583���������\n");
		return 1;// 1�����ʧ��
	}
	memset(send_buff,0,sizeof(send_buff));
	CTools::hex_to_byte((BYTE*)len_buff, (BYTE*)send_buff, 4);
	memcpy(send_buff+2,temp_buff,send_len);
	send_len +=2;
	
	//�����¼
	bbzero(seq);
	i = 0;
	if(flag < 4)  //ȷ�Ϻͳ�����д���ݿ⣬2007-3-15
	{
		while( CDB::setEposTxn(flag, gbl, *ppack, seq) && i<3 )  //��������3��
			i++;
	}
	if (flag==2) //edit by liuxiao 
	{
		bbzero(gbl.order_time);
		CDB::getDBtime(gbl.order_time);
		wrLog("���������ʱ��Ϊ:%s",gbl.order_time);
	}
	//�������ݰ�
	if(flag == 2)
		type = 13;
	else if(flag == 5)
		type = 3;
	else
		type = -1;
	memset(rev_buff,0,sizeof(rev_buff));
	iRet = con.HostSendRecvData(send_buff,send_len,rev_buff,1024);
	if (iRet)
	{
		wrMonitorLog(type, 2, iRet, gbl, *ppack);		//�����־
		return 2;// 2����ʱ
	}
	bbzero(len_buff);
	CTools::byte_to_hex((BYTE *)rev_buff, (BYTE *)len_buff, 2);
	rev_len =atol(len_buff);
	
	//���
	if( ppack->unpackFunc((BYTE *)rev_buff,rev_len) != TRUE)
		return 3;// 3�����ʧ��
	//���¼�¼
	if( strcmp(seq, "") )
	{
		if(flag == 2)
			CDB::setEposTxn(22, gbl, *ppack, seq);
	}
	//�����־
	if(atoi((char *)ppack->FIELD39))
		stat = 3;
	else
		stat = 0;
	if(type>0)
		wrMonitorLog(type, stat, atoi((char *)ppack->FIELD39), gbl, *ppack);
	return 0;// 0���ɹ�
}

// ˢ����������PE���۷Ѱ�
// ��Σ�biz_type(4):ҵ����룻amount(12):���
// ���Σ���
// ����ֵ
// 0���۷ѳɹ�
// 1���۷�ʧ��
// 2����ʱ����
int ZHBusTrade::PayPe(CGlobal *pgbl, char *card_no)
{
	int  iRet, flag,mngCard_count=1,iSelect;
	int b48_len = 0;
	char len_buff[10];
	CTrade trade;
	CPack pack;
	
	wrLog("��*******PE�۷�*******��");	
	
	//������Ϣ,ȡ��ˢ������
	trade.set_in_par(*pgbl);
	flag = 0;	//���������ʶ
	/********************����ˢ��****************/
	mngCard_count=1;
	for(int maxTry=0; maxTry<50; maxTry++)	//������������
	{
		if(!flag)
		{
			iRet = trade.mngCard_Hdl("���ͨ��ֵ");
			if(iRet == CANCLE_KEY || iRet != 0)
				return 1;
		}
		trade.get_gbl(*pgbl);	//ȡ����trade���л�õ���Ϣ
		pack.wrField(2, *pgbl);	//д������
		/***********�޸�һЩ����Ϣ*********/
		pack.FIELD46[0] = 0x39;
		pack.FIELD46[1] = 0x39;
		pack.Packet_Table[46].Real_Length = 2;
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,pgbl->fee_type,4);	//4λ��Ϣ��
		b48_len = 4;
		memset(len_buff,0,sizeof(len_buff));
		sprintf(len_buff,"%02d", 9);	//���ų���
		memcpy(pack.FIELD48+b48_len,len_buff,2);	
		b48_len +=2;
		memcpy(pack.FIELD48+b48_len, card_no, 9);	//���ͨ�߼�����
		b48_len += 9;
		pack.Packet_Table[48].Real_Length=b48_len;
		/**********************************/
		
		//���Ѱ�����
		iRet = packHdl(*pgbl, &pack, 2);
		if(iRet==0)
		{
			NULL;
		}
		else
			return iRet;
		
		if (!memcmp(pack.FIELD39,"00",2)) 
		{
			bbzero(pgbl->stan_no);
			memcpy(pgbl->stan_no,(char *)pack.FIELD37,12);
			return 0;
		}
		else if(!memcmp(pack.FIELD39,"55",2))
		{
			flag = 1;
			if(trade.reGetPwd( "�麣������ֵ" ) == 0) //�������������������
			{
				continue;
			}
			else
				return CANCLE_KEY;
		}
		else if(!memcmp(pack.FIELD39,"14",2)||!memcmp(pack.FIELD39,"15",2)||!memcmp(pack.FIELD39,"33",2)||!memcmp(pack.FIELD39,"35",2)||!memcmp(pack.FIELD39,"36",2)||!memcmp(pack.FIELD39,"56",2)||!memcmp(pack.FIELD39,"57",2)||!memcmp(pack.FIELD39,"60",2)||!memcmp(pack.FIELD39,"62",2)||!memcmp(pack.FIELD39,"91",2))
		{
			TS_OutPutMsg(pgbl->dPosInfo,"\n     ��֧�ֵĿ�\n   �뻻������ˢ��", 0, iSelect);
			if (iSelect!=0)
				return -1;
			else
			{
				continue;
			}
		}
		else{
			trade.set_in_par(*pgbl);
			trade.returnFail((char *)pack.FIELD39);
		}
		return -1;
	}// end for(int maxTry=0; maxTry<50; maxTry++)
	return -1;
}

// ȷ�Ͻ���
// ��Σ�yct_data:���ͨ������Ϣ
// ���Σ���
// ����ֵ��
// 2����ȷ��
// 3��δȷ��
int ZHBusTrade::AckTrade(CGlobal gbl, char *card_no,char *serial,char *balance,char *deviceId)
{
	int b48_len, b63_len, iRet;
	char lenBuff[4+1];
	CPack pack;

	for (int i=0;i<3;i++)
	{
		wrLog("��*******PEȷ��*******��");
		//дȷ����
		pack.wrField(4, gbl);
		/************����48���63��*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,"0358",4);	//4λ��Ϣ��
		b48_len = 4;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff,"%02d", 9);	//���ų���
		memcpy(pack.FIELD48+b48_len, lenBuff, 2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, card_no, 9);//���ͨ�߼�����
		b48_len += 9;
// 		memcpy(pack.FIELD48+b48_len, "|", 1);
// 		b48_len += 1;
// 		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);//���PKI������
// 		b48_len += 8;
		pack.Packet_Table[48].Real_Length = b48_len;
		
		bbzero(pack.FIELD63);
		memcpy(pack.FIELD63,"000",3);
		memcpy(pack.FIELD63+3, serial, 9);
		b63_len = 12;
		memcpy(pack.FIELD63 + b63_len, "00", 2);
		b63_len += 2;
		memcpy(pack.FIELD63 + b63_len, "00", 2);
		b63_len += 2;
		memcpy(pack.FIELD63 + b63_len, balance, 12);
		b63_len += 12;
		memcpy(pack.FIELD63 + b63_len,"00000",5);
		b63_len += 5;
		memcpy(pack.FIELD63 + b63_len, deviceId, 5);
		b63_len += 5;
		pack.Packet_Table[63].Real_Length = b63_len;
		/***********************************/
		//ȷ�ϰ����
		iRet = packHdl(gbl, &pack, 4);
		if( iRet==0 && memcmp(pack.FIELD39,"00",2)==0 )
		{
			return 2;// 2����ȷ��
		}
		else
		{
			;//ȷ�ϲ��ɹ�����ȷ�ϣ�ֱ������ȷ�ϴ�����
		}
	}
	return 3;// 3��δȷ��


}

// ��������
// ��Σ�yct_data:���ͨ������Ϣ
// ���Σ���
// ����ֵ��
// 5���ѳ���
// 6��δ����
int ZHBusTrade::RevTrade(CGlobal gbl, char *card_no)
{
	int b48_len, iRet;
	char lenBuff[4+1];
	CPack pack;

	for(int i=0; i<3; i++)//�������ɹ���3��
	{
		wrLog("��*******���ͨ����[%d]*******��", i);
		//д������
		pack.wrField(5, gbl);
		/************����48��*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,"0358",4);	//4λ��Ϣ��
		b48_len = 4;
		memcpy(pack.FIELD48+b48_len, gbl.pos_serial, 6);	//����ʱ6λԭ�ն���ˮ��
		b48_len += 6;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff, "%02d", 9);	//���ų���
		memcpy(pack.FIELD48+b48_len,lenBuff,2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, card_no, 9);//���ͨ�߼�����
		b48_len += 9;
		pack.Packet_Table[48].Real_Length = b48_len;
		/***********************************/
		//ȷ�ϰ����
		iRet = packHdl(gbl, &pack, 5);
		if( iRet==0 )
		{
			if( memcmp(pack.FIELD39,"00",2)==0 )
			{
				return 5;// 5���ѳ���
			}
			else
			{
				return 6;// 6��δ����
			}
		}
		else
		{
			NULL;
		}
	}
	return 6;// 6��δ����
}


int ZHBusTrade::ZHTrafficCardCharge(CGlobal gbl)
{
	int iRet, iSelect;
	long old_balance, card_no;
	long out_balance, out_tradeId, out_asn, out_operUniqueId, out_Time;
	char temp[200], errMsg[100];
	char str_old_balance[13], str_new_balance[13], str_card_no[10], str_charge_serial[20];
	char out_in_date[20], out_cardType[10], out_devUniqueId[20], new_str_card_no[10];
	
	bbzero(str_old_balance);
	bbzero(new_str_card_no);
	bbzero(gbl.fee_type);
	strcpy(gbl.fee_type,"0358");
	
	TSMENULIST menuList;

	//��ʾ��Ϣ
	iRet = TS_OutPutMsg(gbl.dPosInfo, "��ָ���ͻ�����������ȷ�����ڶ������ϣ������ƶ�����\n����ȷ�ϡ�����\n�������ء��˳�", 0, iSelect);
	if(!iRet && !iSelect)
	{
		NULL;
	}
	else
	{
		return -1;
	}

	//����ע��
	bbzero(errMsg);
	iRet = TS_ZHBusPosLogin(gbl.dPosInfo,errMsg);
	if(!iRet)
	{
		wrLog("����ע��ɹ�");
	}
	else
	{
		wrLog("����ע��ʧ��:%s", errMsg);
		iRet = TS_OutPutMsg(gbl.dPosInfo, "����ע��ʧ�ܣ����Ժ����ԡ�\n����ȷ�ϡ��˳�", 0, iSelect);
		return -1;
	}

	//ǩ��
	bbzero(errMsg);
	iRet = TS_ZHBusSignIn(gbl.dPosInfo, "00000660","11111111",errMsg);
	if(!iRet)
	{
		wrLog("ǩ���ɹ�");
	}
	else
	{
		wrLog("ǩ��ʧ��:%s", errMsg);
		iRet = TS_OutPutMsg(gbl.dPosInfo, "������ֵʧ�ܣ����Ժ����ԡ�\n����ȷ�ϡ��˳�", 0, iSelect);
		return -1;
	}

	//�����Ϣ
	bbzero(out_in_date);
	bbzero(out_cardType);
	bbzero(errMsg);
	iRet = TS_ZHBusQuery(gbl.dPosInfo, old_balance, card_no, out_in_date, out_cardType, errMsg);
	if(!iRet)
	{
		CTools::longToMny(old_balance, str_old_balance);
 		sprintf(str_card_no, "%09d", card_no);
		wrLog("��ѯ�ɹ�,����[%s],���[%s],��Ч����[%s],������[%s]", str_card_no, str_old_balance, out_in_date, out_cardType);
	}
	else
	{
		bbzero(temp);
		sprintf(temp,"��ѯ���ʧ��:%s", errMsg);
		iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
		return -1;
	}
	
	bbzero(temp);
	sprintf(temp, "���ţ�%s\n��%sԪ\n\n����ȷ�ϡ���ֵ  ����ȡ�����˳�", str_card_no, str_old_balance);

	iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
	if(!iRet && !iSelect)
	{
		NULL;
	}
	else
	{
		return -1;
	}

	memset(&menuList, 0, sizeof(menuList));
	
	strcpy(menuList.acTitle, "��ѡ���ֵ���:");
	menuList.iTotalNum = 5;
	
	strcpy(menuList.menuItem[0], "50Ԫ");
	strcpy(menuList.menuItem[1], "100Ԫ");
	strcpy(menuList.menuItem[2], "150Ԫ");
	strcpy(menuList.menuItem[3], "200Ԫ");
	strcpy(menuList.menuItem[4], "5Ԫ");
	
	iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect);

	if(iRet)
	{
		return -1;
	}
	
	switch(iSelect)
	{
	case 1:
		strcpy(gbl.amount, "000000005000");
		break;
	case 2:
		strcpy(gbl.amount, "000000010000");
		break;
	case 3:
		strcpy(gbl.amount, "000000015000");
		break;
	case 4:
		strcpy(gbl.amount, "000000020000");
		break;
	case 5:
		strcpy(gbl.amount, "000000000500");
		break;
	}

	bbzero(temp);
	sprintf(temp, "���ţ�%s\n��%sԪ\n��ֵ��%.2fԪ\n\n����ȷ�ϡ���ֵ  ����ȡ�����˳�", str_card_no, str_old_balance, atof(gbl.amount)/100);
	iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
	if(!iRet && !iSelect)
	{
		NULL;
	}
	else
	{
		return -1;
	}

	iRet = PayPe(&gbl, str_card_no);

	//�������ݸ��ն�
	if( iRet==0 )//�ɹ�
	{
		bbzero(errMsg);
		bbzero(out_cardType);
		bbzero(out_cardType);
		iRet = TS_ZHBusTopUp(gbl.dPosInfo, 
			atol(gbl.amount), out_balance, out_tradeId, out_asn, old_balance, out_operUniqueId, 
			out_cardType, out_devUniqueId, out_Time, errMsg);
		if(iRet)
		{
			bbzero(temp);
			sprintf(temp,"������ֵʧ��:%s�����Ժ����ԡ�\n����ȷ�ϡ��˳�",errMsg);
			iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
			RevTrade(gbl,(char *)new_str_card_no);
			return -1;
		}
	
		wrLog("out_tradeId[%ld],out_operUniqueId[%ld],out_cardType[%s],out_devUniqueId[%s],out_Time[%ld]",out_tradeId,out_operUniqueId,out_cardType,out_devUniqueId,out_Time);
		bbzero(temp);
		bbzero(str_charge_serial);
		sprintf(new_str_card_no, "%09d", out_asn);
		sprintf(str_charge_serial, "%ld", out_tradeId);
		CTools::longToMny(out_balance, str_new_balance);
		sprintf(temp, "��ֵ�ɹ�����\n���ţ�%s\n��ֵ��ˮ��%s\nԭ����%sԪ\n��ֵ��%.2fԪ\n��ֵ����%sԪ\n\n����ȷ�ϡ���ӡ��Ʊ", 
			new_str_card_no, str_charge_serial, str_old_balance, atof(gbl.amount)/100, str_new_balance);
	
		iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);

		if(!iRet && !iSelect)
		{
			NULL;
		}
		else
		{
			return -1;
		}
	
		sprintf(str_new_balance,"%012ld",out_balance);
		AckTrade(gbl, (char *)new_str_card_no, str_charge_serial, str_new_balance, out_devUniqueId);

	}
	else if(iRet==2)
	{
		
	}

	//PEȷ��
	//��ʾ��ֵ���
	//��ӡ

	return 0;
}