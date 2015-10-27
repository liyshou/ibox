// DGTBusTrade.cpp: implementation of the CDGBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DGTBusTrade.h"
#include "Tools.h"
#include "WrLog.h"
#include "Def.h"
#include "WrLog.h"
#include "Des.h"
#include "3DES.h"
#include "DB.h"

#define RLD_PIN_MAK			"4163958262438749"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CDGTBusTrade::~CDGTBusTrade()
{
	
}

CDGTBusTrade::CDGTBusTrade()
{ 
	

}


//��ʼ����ݸͨ��Ϣ
void CDGTBusTrade::initField()
{

	memset(dgtFIELD0,0,sizeof(dgtFIELD0));
	memset(dgtFIELD1,0,sizeof(dgtFIELD1));
	memset(dgtFIELD2,0,sizeof(dgtFIELD2));
	memset(dgtFIELD3,0,sizeof(dgtFIELD3));
	memset(dgtFIELD4,0,sizeof(dgtFIELD4));
	memset(dgtFIELD5,0,sizeof(dgtFIELD5));
	memset(dgtFIELD6,0,sizeof(dgtFIELD6));
	memset(dgtFIELD7,0,sizeof(dgtFIELD7));
	memset(dgtFIELD8,0,sizeof(dgtFIELD8));
	memset(dgtFIELD9,0,sizeof(dgtFIELD9));
	memset(dgtFIELD10,0,sizeof(dgtFIELD10));
	memset(dgtFIELD11,0,sizeof(dgtFIELD11));
	memset(dgtFIELD12,0,sizeof(dgtFIELD12));
	memset(dgtFIELD13,0,sizeof(dgtFIELD13));
	memset(dgtFIELD14,0,sizeof(dgtFIELD14));
	memset(dgtFIELD15,0,sizeof(dgtFIELD15));
	memset(dgtFIELD16,0,sizeof(dgtFIELD16));
	memset(dgtFIELD17,0,sizeof(dgtFIELD17));
	memset(dgtFIELD18,0,sizeof(dgtFIELD18));
	memset(dgtFIELD19,0,sizeof(dgtFIELD19));
	memset(dgtFIELD20,0,sizeof(dgtFIELD20));
	memset(dgtFIELD21,0,sizeof(dgtFIELD21));
	memset(dgtFIELD22,0,sizeof(dgtFIELD22));
	memset(dgtFIELD23,0,sizeof(dgtFIELD23));
	memset(dgtFIELD24,0,sizeof(dgtFIELD24));
	memset(dgtFIELD25,0,sizeof(dgtFIELD25));
	memset(dgtFIELD26,0,sizeof(dgtFIELD26));
	memset(dgtFIELD27,0,sizeof(dgtFIELD27));
	memset(dgtFIELD28,0,sizeof(dgtFIELD28));
	memset(dgtFIELD29,0,sizeof(dgtFIELD29));
	memset(dgtFIELD30,0,sizeof(dgtFIELD30));
	memset(dgtFIELD31,0,sizeof(dgtFIELD31));
	memset(dgtFIELD32,0,sizeof(dgtFIELD32));
	memset(dgtFIELD33,0,sizeof(dgtFIELD33));
	memset(dgtFIELD34,0,sizeof(dgtFIELD34));
	memset(dgtFIELD35,0,sizeof(dgtFIELD35));
	memset(dgtFIELD36,0,sizeof(dgtFIELD36));
	memset(dgtFIELD37,0,sizeof(dgtFIELD37));
	memset(dgtFIELD38,0,sizeof(dgtFIELD38));
	memset(dgtFIELD39,0,sizeof(dgtFIELD39));
	memset(dgtFIELD40,0,sizeof(dgtFIELD40));
	memset(dgtFIELD41,0,sizeof(dgtFIELD41));
}


//��ݸͨ���
//������:   packdgtFunc
//���룺	
//�����	data_source
//���أ�	������
int CDGTBusTrade::packdgtFunc(CGlobal *pgbl, char *transMessage, char*tranType,char *sendData,int *sendLen)	//���
{
	int 	i;
	int		iRet;
	int 	TempLen , Send_Length;
	int max_count=0;
	int pack_len=0;// ���ĳ���
	char syn_info[12+1]; //ͬ����Ϣ
	char zip_flag='0'; //ѹ������
	char encrypt_flag='0'; //������
	char message_type[4+1];
	char version[2+1];
	char keyField[500];
	char packDatas[1024];
	char biz_name[20];
	int keyFiledLen=0;
	char CenterSeqno_field[10+1];
	char cardFaceid[16+1];//
	char cardvaldate[8+1]; //����Ч��
	char deposit[8+1];
	char EDCARDID_field[16+1];

	bbzero(EDCARDID_field);
	bbzero(deposit);
	bbzero(cardvaldate);
	bbzero(cardFaceid);
	bbzero(CenterSeqno_field);

	char body_ver[2+1];
	bbzero(biz_name);
	bbzero(keyField);

	char body_messageType[4+1];
	char body_MessageDateTime[14+1];
	char body_MAC[8+1]; //ͨѶMAC
	char body_rspcode[2+1]; //��Ӧ��

	bbzero(packDatas);
	bbzero(body_ver);
	bbzero(keyField);
	bbzero(body_messageType);
	bbzero(body_MessageDateTime);
	bbzero(body_MAC);
	bbzero(body_rspcode);

	bbzero(version);
	bbzero(syn_info);
	bbzero(message_type);
	bbzero(body_ver);

	strcpy(message_type,"5000");
	strcpy(version,"01");
	getSyncinfo(syn_info);
	wrLog("ͬ����Ϣ:%s",syn_info);
	sprintf(EDCARDID_field,"%016s",pgbl->busi_id);

	initField();
	bbzero(packDatas);
	Send_Length=0;


	memcpy( packDatas, syn_info, 12); //ͬ����Ϣ
	Send_Length +=12;
	memcpy(packDatas+Send_Length,"00015000",8);
	Send_Length+=8;

	//�������е�ͨѶ���� ͨѶ���� �̶�������30
	memcpy(body_ver,"10",2);
	bbzero(body_messageType);
	strcpy(body_messageType,transMessage); //�̶�ֵ

	//��ȡʱ��
	CTools::get_termTime(body_MessageDateTime);

	bbzero(pgbl->dgtCard.MESSAGEDATETIME);
	strcpy(pgbl->dgtCard.MESSAGEDATETIME,body_MessageDateTime);
	bbzero(pgbl->dgtCard.tradeDate);
	bbzero(pgbl->dgtCard.tradeTime);
	memcpy(pgbl->dgtCard.tradeDate,pgbl->dgtCard.MESSAGEDATETIME,8);
	memcpy(pgbl->dgtCard.tradeTime,pgbl->dgtCard.MESSAGEDATETIME+8,6);

	bbzero(body_rspcode);
	strcpy(body_rspcode,"00");

	//����Ѻ��
	bbzero(deposit);
	char depositAmount[4+1];
	bbzero(depositAmount);


	//����ǻ������룬Ѻ����M1�Ŀ�ƬѺ��
	if (strcmp(transMessage,"3458")==0 ||  strcmp(transMessage,"3481")==0) //����ǿ�����
	{
		bbzero(pgbl->dgtM1Card.city_code);
		strcpy(pgbl->dgtM1Card.city_code,"5230");
		strcpy(deposit,pgbl->dgtM1Card.deposit);
	}
	else if ( strcmp(transMessage,"3480")==0 ) //����ǽ����ȡ���� ȡCPU����Ϣ
	{

		bbzero(pgbl->dgtM1Card.city_code);
		strcpy(pgbl->dgtM1Card.city_code,"5230");
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.deposit,(BYTE *) depositAmount,2);
		
		if (strcmp(depositAmount,"FFFF")==0 ) //���û��Ѻ��
		{
			strcpy(deposit,"000000");
			
		}
		else
		{
			char deposit_temp[2+1];
			bbzero(deposit_temp);
			memcpy(deposit_temp,depositAmount+2,2);
			sprintf(deposit,"%06d",(atoi(deposit_temp)*100));
			
		}
	
	}
	else //����������CPU������Ϣ
	{
	
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.deposit,(BYTE *) depositAmount,2);
		
		if (strcmp(depositAmount,"FFFF")==0 ) //���û��Ѻ��
		{
			strcpy(deposit,"000000");
			
		}
		else
		{
			char deposit_temp[2+1];
			bbzero(deposit_temp);
			memcpy(deposit_temp,depositAmount+2,2);
			sprintf(deposit,"%06d",(atoi(deposit_temp)*100));
			
		}

	}


	init_dgt_pack(transMessage,tranType,pgbl);

	//����MAC
	switch (atoi(transMessage))
	{

	case 2051:
		strcpy(biz_name,"ǩ��");
		max_count=17; //����ǩ��
		bbzero(body_MAC);
		strcpy(body_MAC,"00000000");

		break;
	case 2061: //����������
		strcpy(biz_name,"����������");
		max_count=42;

		//����ͨѶMAC���ؼ���
		//messagedatetime���ķ���ʱ��(14) + transtypeҵ������(2) + posid(12) + possequence(10)+edcardid(16)
		
		memcpy(keyField,body_MessageDateTime,14);
		strcat(keyField,tranType);
		strcat(keyField,pgbl->posId);

		char posSerial[10+1];
		bbzero(posSerial);
		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcat(keyField,posSerial); //ǰ��0
	
		//ecardid(16)
		strcat(keyField,EDCARDID_field);
		
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
		
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //ͨѶ����
		}

		break;
	case 2063: //�ۿ�
		strcpy(biz_name,"�ۿ�");
		max_count=30;
		//messagedatetime���ķ���ʱ��(14) + transmessage��������(4) + posid(12) + possequence(10λ) +edcardid(16)+ cardsalemode(2) + deposit(8) + befbalance(8) + txnamt(8) + txndate(8) + txntime(6) + cardid(16)
		memcpy(keyField,body_MessageDateTime,14);
		strcat(keyField,transMessage);
		strcat(keyField,pgbl->posId);
		bbzero(posSerial);
		wrLog("�ն���ˮ��:%s",pgbl->dgt_posSerial);

		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcat(keyField,posSerial); //ǰ��0
		
		strcat(keyField,EDCARDID_field);
		strcat(keyField,pgbl->dgtCard.sale_type);  //�ۿ���ʽ Ѻ����2��������Ϊ1

		//deposit(8) + befbalance(8) + txnamt(8) 
		//
		char deposit_buffer[8+1];
		bbzero(deposit_buffer);
		CTools::addZero(deposit,8,1,deposit_buffer);
		strcat(keyField,deposit_buffer); //ǰ��0

		strcat(keyField,pgbl->dgtCard.tradeBeForeMny); //����ǰ���
		strcat(keyField,pgbl->dgtCard.tradeMny);
		strcat(keyField,pgbl->dgtCard.tradeDate);
		strcat(keyField,pgbl->dgtCard.tradeTime);
		strcat(keyField,pgbl->dgtCard.cardShowId);
		
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
		
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //ͨѶ����
		}

		break;
	case 3458: //�������� 
		//����ͨѶmac���ؼ���
	case 3480: //�����ȡ
	case 3481://�����ȡ����

		if (strcmp(transMessage,"3458")==0)
		{
			strcpy(biz_name,"��������");
			bbzero(CenterSeqno_field);
			CTools::addZero(CenterSeqno_field,10,0,CenterSeqno_field);

		}
		else if (strcmp(transMessage,"3480")==0)
		{
			 bbzero(CenterSeqno_field);
			memcpy(CenterSeqno_field,pgbl->Centerseqno+8,10);
			strcpy(biz_name,"�����ȡ");
		}
		else
		{
			memcpy(CenterSeqno_field,pgbl->Centerseqno+8,10);
			strcpy(biz_name,"�����ȡ����");

		}
	
		max_count=39;
		memcpy(keyField,transMessage,4);
		char cardno[16+1];
		bbzero(cardno);
		char bus_mny[8+1];
		bbzero(bus_mny);

		CTools::addZero(pgbl->dgtM1Card.bus_showCard,16,1,cardno);

		bbzero(cardFaceid);
		CTools::addZero(pgbl->dgtM1Card.bus_showCard,16,1,cardFaceid);
		strcpy(bus_mny,pgbl->dgtM1Card.bus_amt);

		strcat(keyField,body_MessageDateTime);
		strcat(keyField,CenterSeqno_field);
		strcat(keyField,pgbl->posId);
		bbzero(posSerial);
		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcat(keyField,posSerial); //ǰ��0
			
		//edcardid
		strcat(keyField,EDCARDID_field);

		strcat(keyField,cardFaceid);
		strcat(keyField,cardno);
		//strcat(keyField,gbl.dgtM1Card.city_code);
		strcat(keyField,pgbl->dgtM1Card.city_code);
		strcat(keyField,pgbl->dgtM1Card.card_parent_type);
		strcat(keyField,pgbl->dgtM1Card.card_child_type);
		strcat(keyField,pgbl->dgtM1Card.cardVadate); //����Ч�� 8λ��
			
		strcat(keyField,bus_mny);
		strcat(keyField,deposit);

		wrLog("�������MAC����");
		wrLog("bus_amt:%s",bus_mny);
		wrLog("cardno:%s",cardno);
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
	
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //ͨѶ����
		}
		
		break;

	case 3460://�������
		max_count=39;
		//mac = messagetype��������(4) + messagedatetime(14) + centerseqno(10) + 
		//+posid(12) + possequence(10) + edcardid(16) + cardfaceid(16) + cardid(16) 
		//+ citycode(4) + cardmkind(2) + cardskind(2) + cardvaldate(8) 
		//+ befbalance(8) + depositamount(6)
		strcpy(biz_name,"�������");
		char city_code[4+1];
		char card_parentType[2+1];
		char card_childType[2+1];
		char effectiveDate[8+1];
		char ecardid[16+1];

		bbzero(card_childType);
		bbzero(card_parentType);
		bbzero(city_code);
		bbzero(effectiveDate);
		bbzero(posSerial);
		bbzero(ecardid);
		bbzero(cardFaceid);

		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.CItyCode,(BYTE *) city_code,2);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardParent_type,(BYTE *) card_parentType,1);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardChild_type,(BYTE *) card_childType,1);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *) effectiveDate,4);
	
		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcpy(cardFaceid,pgbl->dgtCard.cardShowId);

		bbzero(CenterSeqno_field);
		memcpy(CenterSeqno_field,pgbl->Centerseqno+8,10);

		memcpy(keyField,transMessage,4);
		strcat(keyField,body_MessageDateTime);
		strcat(keyField,CenterSeqno_field); //���ĺ�����λ
		strcat(keyField,pgbl->posId);
		strcat(keyField,posSerial); //ǰ��0
		strcat(keyField,EDCARDID_field);
		strcat(keyField,cardFaceid);
		strcat(keyField,pgbl->dgtCard.cardShowId);
		strcat(keyField,city_code);
		strcat(keyField,card_parentType);
		strcat(keyField,card_childType);
		strcat(keyField,effectiveDate);
		
		strcat(keyField,pgbl->dgtCard.tradeBeForeMny);
		strcat(keyField,deposit);
		
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
		
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //ͨѶ����
		}

		break;
	default:
		return -1;
	}

	strcpy(body_rspcode,"00"); //���ͱ��ĵ���Ӧ����00
	strcat(packDatas,body_ver);
	strcat(packDatas,body_messageType);
	strcat(packDatas,body_MessageDateTime);
	strcat(packDatas,body_MAC);
	strcat(packDatas,body_rspcode);
	Send_Length+=30;

	wrLog("��ʼ���[%s]����",biz_name);


	for ( i = 0; i < max_count; i++)
	{	

			//����Ƕ�����ʵ�ʳ��Ⱦ�����󳤶ȡ��������Ƕ�����ʵ��ʵ����real_length
			switch (Pack_dgt_table[i].Format)
				{
					case FIXED:
						Pack_dgt_table[i].Real_Length=Pack_dgt_table[i].Max_Length;
						break;
					case LLLVAR:
						break;
						Pack_dgt_table[i].Real_Length=atoi(Pack_dgt_table[i-1].Send_Data); //ȡ��һ���ֶε�ֵ

					default:
						Pack_dgt_table[i].Real_Length=Pack_dgt_table[i].Max_Length;

				}
			

				TempLen = Pack_dgt_table[i].Real_Length;
		
				strcat(packDatas,Pack_dgt_table[i].Send_Data);
				Send_Length += TempLen;

			if( DEBUG_LEVEL <= 3)
			{
				wrLog("[%d]-----%s[%d]:  %s",i,Pack_dgt_table[i].name,Pack_dgt_table[i].Real_Length,Pack_dgt_table[i].Send_Data);  //��������
			}

	}

	*sendLen=(Send_Length+4); //�ӱ��ĳ���
	bbzero(sendData);
	sprintf(sendData,"%04d%s",Send_Length,packDatas);

	wrLog("����%s���ݰ�:%s",biz_name,sendData);
	return  0;

}

//��ݸͨ���
//������:   unpackFunc
//����������8583�������
//���룺	data_source��data_length
//�����	��
//���أ�	TRUE���ɹ�
int CDGTBusTrade::undgtpackFunc(CGlobal *pgbl, char *transMessage,char *transType,char *rcvData,int rcvLen,char *rsp_code)
{
	BYTE	i;
	int	    offset=0;
	int     Real_Length = 0;
	int max_count=0;
	int unpackLen=0;
	char commuMac[8+1];
	char bizName[100];
	char trans_code[2+1];

	bbzero(rsp_code);
	bbzero(commuMac);
	bbzero(bizName);
	bbzero(rsp_code);
	bbzero(trans_code);

	wrLog("���ձ��ĳ���:%d",rcvLen);
	initField();
	
	memcpy(commuMac,rcvData+24+20,8); //ͨѶmac
	memcpy(trans_code,rcvData+24+28,2); //ͨѶӦ��
	wrLog("ͨѶMAC:%s",commuMac);
	wrLog("Ӧ����Ӧ��:%s",trans_code);
	//��ȡ��Ӧ��

	if (strcmp(trans_code,"00")!=0) //�����Ӧʧ��
	{
		bbzero(rsp_code);
		strcpy(rsp_code,trans_code);
		return -1;// ͨѶ������Ӧʧ��
	}

	//��ȡrsp_code
	memcpy(rsp_code,rcvData+rcvLen-5,5);
	if (strcmp(rsp_code,"00000")!=0) //�����Ӧ�벻��00000
	{
		return -2;
	}

	wrLog("%s�����ʼ����",bizName);

	offset =54; //ֱ�ӻ�ȡ����������
	

	switch (atoi(transMessage))
	{

	case 2051:
		strcpy(bizName,"ǩ��");
		max_count=17; //����ǩ��
		break;
	case 2061: //����������
		strcpy(bizName,"����������");
		max_count=42;
		//����ͨѶMAC���ؼ���
		//messagedatetime���ķ���ʱ��(14) + transtypeҵ������(2) + posid(12) + possequence(10)+edcardid(16)
		//	getCommuMac();
		
		break;
	case 2063: //�ۿ�
		max_count=30;
		strcpy(bizName,"�ۿ�");
		//getCommuMac();
		//����ͨ��MAC,�ؼ���
		break;
	case 3458: //��������
		max_count=39;
		strcpy(bizName,"��������");

		//	getCommuMac();
		//����ͨѶmac���ؼ���
	case 3480: //�����ȡ
		//����ͨѶMAC,�ؼ���
		max_count=39;
		strcpy(bizName,"�����ȡ");
		//	getCommuMac();
		
	case 3481://�����ȡ����
		max_count=39;
		strcpy(bizName,"�����ȡ����");
		//	getCommuMac();
		break;
	case 3460://�������
		strcpy(bizName,"�������");
		max_count=39;
		//	getCommuMac();
		break;
	default:
		break;
	}


	for ( i = 0; i <max_count; i++)
	{

			memset( Pack_dgt_table[i].Rev_Data, 0, sizeof(Pack_dgt_table[i].Rev_Data) ); 

			switch ( Pack_dgt_table[i].Format) //����Ƕ���
			{
			case FIXED:
				{
					Pack_dgt_table[i].Real_Length=Pack_dgt_table[i].Max_Length;
					memcpy(Pack_dgt_table[i].Rev_Data,rcvData+offset,Pack_dgt_table[i].Max_Length);
					offset += Pack_dgt_table[i].Max_Length;				
					break;
				}
				
			case LLLVAR: //������Ƕ���
				{

					//ȡ��ʵ����
					
					int real_length=Pack_dgt_table[i].Real_Length;
				
					memcpy(Pack_dgt_table[i].Rev_Data,rcvData+offset,real_length);
					offset+=real_length;

					
				}//****case LLLVAR***
			}//end switch(format)

			if( DEBUG_LEVEL <= 2)
			{
				wrLog("[%d]----%s[%d]:  %s",i,Pack_dgt_table[i].name,Pack_dgt_table[i].Real_Length,Pack_dgt_table[i].Rev_Data);
			}
		
	}//end for


	//��ȡ��Ϣ
	switch (atoi(transMessage))
	{
		
	case 2051:
		strcpy(rsp_code,dgtFIELD16);

		break;
	case 2061: //����������

		//��ȡ��Ȩ��ˮ��
		bbzero(pgbl->dgtCard.AUTHSEQ);
		bbzero(pgbl->dgtCard.LIMITEAUTHSEQL);
		strcpy(pgbl->dgtCard.AUTHSEQ,dgtFIELD38);
		strcpy(pgbl->dgtCard.LIMITEAUTHSEQL,dgtFIELD39);
		bbzero(pgbl->dgtCard.SETDATE);
		strcpy(pgbl->dgtCard.SETDATE,dgtFIELD36);
		bbzero(pgbl->dgtCard.BATCHNO);

		
		//������ۿ���ȡdicmac ����ǳ�ֵ��ȡMAC2
		if (strcmp(transType,"80")==0 || strcmp(transType,"52")==0) //������ۿ�
		{
			
			bbzero(pgbl->dgtCard.DICMAC);
			memcpy(pgbl->dgtCard.DICMAC,dgtFIELD34,8);
			
		}
		else
		{
			bbzero(pgbl->dgtCard.MAC2);
			strcpy(pgbl->dgtCard.MAC2,dgtFIELD25);
			wrLog("��ݸͨmac2��%s",pgbl->dgtCard.MAC2);
			
		}

		break;
	case 2063: //�ۿ�

		break;
	case 3458: //��������
	case 3480: //�����ȡ
	case 3481://�����ȡ����

		if (strcmp(transMessage,"3458")==0 )  //�ڻ��������ʱ�򷵻� 
		{

			bbzero(pgbl->Centerseqno);
			strcpy(pgbl->Centerseqno,dgtFIELD3);

			bbzero(pgbl->ReturnAmt);
			strcpy(pgbl->ReturnAmt,dgtFIELD24);
			
			bbzero(pgbl->ReturnDeposit);
			strcpy(pgbl->ReturnDeposit,dgtFIELD25);

			bbzero(pgbl->MonthFeeFlag);
			strcpy(pgbl->MonthFeeFlag,dgtFIELD26);

			bbzero(pgbl->FeeAmtFlag);
			strcpy(pgbl->FeeAmtFlag,dgtFIELD27);

			bbzero(pgbl->MonthFeeAmt);
			strcpy(pgbl->MonthFeeAmt,dgtFIELD28);

			bbzero(pgbl->FeeAmt);
			strcpy(pgbl->FeeAmt,dgtFIELD29);

			bbzero(pgbl->dgtCard.AUTHSEQ);
			strcpy(pgbl->dgtCard.AUTHSEQ,dgtFIELD32);
			
			bbzero(pgbl->dgtCard.BATCHNO);
			strcpy(pgbl->dgtCard.BATCHNO,dgtFIELD31);
			bbzero(pgbl->dgtCard.SETDATE);
			strcpy(pgbl->dgtCard.SETDATE,dgtFIELD30);

			wrLog("������ˮ��:%s",pgbl->Centerseqno);
			wrLog("gbl.dgtCard.AUTHSEQ:%s",pgbl->dgtCard.AUTHSEQ);
			wrLog("gbl.FeeAmt:%s",pgbl->FeeAmt);
			wrLog("gbl.MonthFeeAmt:%s",pgbl->MonthFeeAmt);
			wrLog("gbl.ReturnDeposit:%s",pgbl->ReturnDeposit);

		}

		break;
	case 3460://�������
		
		break;
	default:
		break;
	}

	return 0;
}


CDGTBusTrade::init_dgt_pack( char *transMessage, char *tranType,CGlobal *pgbl)
{
	int j;
	int max_count=0;
	char AuthSeq_field[18+1];
	char deposit_field[8+1];
	char EDCARDID_field[16+1];
	char depositAmount[4+1];
	bbzero(depositAmount);
	int COMMAND_LEN;

	bbzero(EDCARDID_field);
	bbzero(deposit_field);
	bbzero(AuthSeq_field);
	sprintf(AuthSeq_field,"%s%s",pgbl->dgtCard.SETDATE,pgbl->dgtCard.AUTHSEQ);
	sprintf(EDCARDID_field,"%016s",pgbl->busi_id); //����Ԫ���ţ�������Ԫ��һ��

	memset(&Pack_dgt_table, 0, sizeof(Pack_dgt_table));

	if (strcmp(transMessage,"3458")==0  || strcmp(transMessage,"3481")==0) //����ǿ����� 
	{
			strcpy(deposit_field,pgbl->dgtM1Card.deposit); //ȡѺ��
	}

	else  //3480 ��ʾ�����ȡ���� Ѻ����cpu����Ѻ��
	{
		
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.deposit,(BYTE *) depositAmount,2);
		
		if (strcmp(depositAmount,"FFFF")==0 ) //���û��Ѻ��
		{
			strcpy(deposit_field,"00000000");
			
		}
		else
		{
			//���Ѻ��
			char deposit_buffer[2+1];
			bbzero(deposit_buffer);
			memcpy(deposit_buffer,depositAmount+2,2);
			bbzero(deposit_field);
			sprintf(deposit_field,"%d",(atoi(deposit_buffer)*100));
			
		}


	}



	switch (atoi(transMessage))
	{

	case 2051: //ǩ��

	//ǩ������
	Pack_dgt_table[0].Type = NUMERIC;			Pack_dgt_table[0].Format = FIXED;			Pack_dgt_table[0].Max_Length = 8;		Pack_dgt_table[0].optionFlag='M';	//Unitid
	Pack_dgt_table[1].Type = NUMERIC;			Pack_dgt_table[1].Format = FIXED;			Pack_dgt_table[1].Max_Length = 2;		Pack_dgt_table[1].optionFlag='M';	//���׷�ʽ
	Pack_dgt_table[2].Type = NUMERIC;			Pack_dgt_table[2].Format = FIXED;			Pack_dgt_table[2].Max_Length = 16;		Pack_dgt_table[2].optionFlag='C';	//SAM����
	Pack_dgt_table[3].Type = NUMERIC;			Pack_dgt_table[3].Format = FIXED;			Pack_dgt_table[3].Max_Length = 16;		Pack_dgt_table[3].optionFlag='C';	//SAM����Ȩ��Ϣ
	Pack_dgt_table[4].Type = NUMERIC;			Pack_dgt_table[4].Format = FIXED;			Pack_dgt_table[4].Max_Length = 16;		Pack_dgt_table[4].optionFlag='C';	//ED����Ȩ��Ϣ
	Pack_dgt_table[5].Type = NUMERIC;			Pack_dgt_table[5].Format = FIXED;			Pack_dgt_table[5].Max_Length = 12;		Pack_dgt_table[5].optionFlag='M';	//�����豸����
	Pack_dgt_table[6].Type = NUMERIC; 			Pack_dgt_table[6].Format = FIXED;			Pack_dgt_table[6].Max_Length = 12;		Pack_dgt_table[6].optionFlag='M';	//��ҵ�豸����
	Pack_dgt_table[7].Type = NUMERIC; 			Pack_dgt_table[7].Format = FIXED;			Pack_dgt_table[7].Max_Length = 16;		Pack_dgt_table[7].optionFlag='M';	//����Ա��
	Pack_dgt_table[8].Type = NUMERIC;			Pack_dgt_table[8].Format = FIXED;  			Pack_dgt_table[8].Max_Length = 16;		Pack_dgt_table[8].optionFlag='C';	//������������
	Pack_dgt_table[9].Type = NUMERIC;			Pack_dgt_table[9].Format = FIXED;			Pack_dgt_table[9].Max_Length = 8;		Pack_dgt_table[9].optionFlag='C';	//�������� ȫ0
	Pack_dgt_table[10].Type = NUMERIC;			Pack_dgt_table[10].Format = FIXED;			Pack_dgt_table[10].Max_Length = 6;		Pack_dgt_table[10].optionFlag='C';	//���κ�
	Pack_dgt_table[11].Type = NUMERIC;			Pack_dgt_table[11].Format = FIXED;			Pack_dgt_table[11].Max_Length = 14;		Pack_dgt_table[11].optionFlag='C';	//����ʱ�䣬�������豸У��ʱ��,ȫ��0
	Pack_dgt_table[12].Type = NUMERIC;			Pack_dgt_table[12].Format = FIXED;			Pack_dgt_table[12].Max_Length = 24;		Pack_dgt_table[12].optionFlag='C';	//��Ȩ��֤�루M1��
	Pack_dgt_table[13].Type = NUMERIC;			Pack_dgt_table[13].Format = FIXED;			Pack_dgt_table[13].Max_Length = 32;		Pack_dgt_table[13].optionFlag='C';	//�������±�־λ
	Pack_dgt_table[14].Type = ALPHANUM;			Pack_dgt_table[14].Format = FIXED;			Pack_dgt_table[14].Max_Length = 32;		Pack_dgt_table[14].optionFlag='C';	//��Կ(MAC��Կ)(����)
	Pack_dgt_table[15].Type = ALPHANUM;			Pack_dgt_table[15].Format = FIXED;			Pack_dgt_table[15].Max_Length = 20;		Pack_dgt_table[15].optionFlag='C';	//������
	Pack_dgt_table[16].Type = NUMERIC;			Pack_dgt_table[16].Format = FIXED;			Pack_dgt_table[16].Max_Length = 5;		Pack_dgt_table[16].optionFlag='C';	//����Ӧ����
	
	strcpy(Pack_dgt_table[0].name,"Unitid");
	strcpy(Pack_dgt_table[1].name,"TxnMode");
	strcpy(Pack_dgt_table[2].name,"SAMID");
	strcpy(Pack_dgt_table[3].name,"SAMAuthInfo");
	strcpy(Pack_dgt_table[4].name,"EDAuthInfo");
	strcpy(Pack_dgt_table[5].name,"Posid");
	strcpy(Pack_dgt_table[6].name,"Termid");
	strcpy(Pack_dgt_table[7].name,"Operid");
	strcpy(Pack_dgt_table[8].name,"EDCardId");
	strcpy(Pack_dgt_table[9].name,"SettDate");
	strcpy(Pack_dgt_table[10].name,"BatchNo");
	strcpy(Pack_dgt_table[11].name,"SysDatetime");
	strcpy(Pack_dgt_table[12].name,"Authcode");
	strcpy(Pack_dgt_table[13].name,"ParamBit");
	strcpy(Pack_dgt_table[14].name,"KeySet");
	strcpy(Pack_dgt_table[15].name,"Reserved");
	strcpy(Pack_dgt_table[16].name,"RSPCODE");

	initField();
	strcpy(dgtFIELD0,DGT_Unitid);
	strcpy(dgtFIELD1,"01");//���׷�ʽ��01��ʾ������00��ʾ�ѻ�
	strcpy(dgtFIELD5,pgbl->posId);
	strcpy(dgtFIELD6,pgbl->busi_id);
	strcpy(dgtFIELD7,pgbl->busi_id);


	Pack_dgt_table[0].Rev_Data = dgtFIELD0;			
	Pack_dgt_table[0].Send_Data = dgtFIELD0;

	Pack_dgt_table[1].Rev_Data = dgtFIELD1;			Pack_dgt_table[1].Send_Data = dgtFIELD1;
	Pack_dgt_table[2].Rev_Data = dgtFIELD2;			Pack_dgt_table[2].Send_Data = dgtFIELD2;
	Pack_dgt_table[3].Rev_Data = dgtFIELD3;			Pack_dgt_table[3].Send_Data = dgtFIELD3;
	Pack_dgt_table[4].Rev_Data = dgtFIELD4;			Pack_dgt_table[4].Send_Data= dgtFIELD4;
	Pack_dgt_table[5].Rev_Data = dgtFIELD5;			Pack_dgt_table[5].Send_Data = dgtFIELD5;
	Pack_dgt_table[6].Rev_Data = dgtFIELD6;			Pack_dgt_table[6].Send_Data = dgtFIELD6;
	Pack_dgt_table[7].Rev_Data = dgtFIELD7;			Pack_dgt_table[7].Send_Data = dgtFIELD7;
	Pack_dgt_table[8].Rev_Data = dgtFIELD8;			Pack_dgt_table[8].Send_Data = dgtFIELD8;
	Pack_dgt_table[9].Rev_Data = dgtFIELD9;			Pack_dgt_table[9].Send_Data = dgtFIELD9;
	Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
	Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
	Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
	Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
	Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
	Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
	Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;

	break;

	case 2061: //����������

		//���������� �в�����ֵ
		max_count=42;
		initField();

		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->posId);
		strcpy(dgtFIELD3,pgbl->dgt_posSerial);//PosSequence
		strcpy(dgtFIELD4,pgbl->busi_id);//Termid
		strcpy(dgtFIELD5,pgbl->busi_id);//Operid
		strcpy(dgtFIELD6,EDCARDID_field); 

		if (strcmp(tranType,"53")==0) //ת����ֵ
		{
			strcpy(dgtFIELD7,"3460");

		}
		else if (strcmp(tranType,"52") ==0 || strcmp(tranType,"80")) // �����ۿ�����ͨ�ۿ�
		{

			strcpy(dgtFIELD7,"2063");

		}
		else  //���� 02
		{
		
			strcpy(dgtFIELD7,"2062");
		}

		CTools::byte_to_hex((BYTE *)pgbl->dgtCard.CItyCode,(BYTE *)dgtFIELD8,2);
		strcpy(dgtFIELD9,pgbl->dgtCard.cardShowId);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardParent_type,(BYTE *)dgtFIELD10,1);
		CTools::byte_to_hex((BYTE *)pgbl->dgtCard.cardChild_type,(BYTE *)dgtFIELD11,1);
		strcpy(dgtFIELD12,"1"); //1��ʾCPU��
		strcpy(dgtFIELD13,tranType); //���������� Ĭ��δ����

		strcpy(dgtFIELD21,deposit_field);
		strcpy(dgtFIELD37,pgbl->dgtSignInfo.BATCHNO); //�޸ĳ�ǩ�����ص����κ�
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD24,4);
		memcpy(dgtFIELD26,pgbl->dgtCard.cardMny+4,8);
		strcpy(dgtFIELD30,pgbl->dgtCard.CARDRAND);
	
		if (strcmp(tranType,"02")==0 || strcmp(tranType,"53")==0) //��ͨ��ֵ�ͻ���ת���ʽ��ֵ
		{
			//����ǳ�ֵ
			strcpy(dgtFIELD27,pgbl->dgtCard.CARDSEQ);
			strcpy(dgtFIELD28,pgbl->dgtCard.KEYVER);
			strcpy(dgtFIELD29,pgbl->dgtCard.ALGIND);
			strcpy(dgtFIELD31,pgbl->dgtCard.MAC1);
			strcpy(dgtFIELD22,pgbl->dgtCard.tradeMny); //ȡ��8λ��
			strcpy(dgtFIELD23,pgbl->dgtCard.tradeMny);
			CTools::byte_to_hex((BYTE *)pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD24,4);

			strcpy(dgtFIELD28,pgbl->dgtCard.KEYVER);
			strcpy(dgtFIELD29,pgbl->dgtCard.ALGIND);
			//��Ƭcommand ������0
			strcpy(dgtFIELD32,"000");
			bbzero(pgbl->dgtCard.CHARGE_DIVDATA);
			CTools::byte_to_hex((BYTE *) pgbl->dgtCard.appSerial,(BYTE *)pgbl->dgtCard.CHARGE_DIVDATA,8); //��ֵ��ʱ����ɢ�����ǿ���
			strcpy(dgtFIELD35,pgbl->dgtCard.CHARGE_DIVDATA);
			strcpy(dgtFIELD40,pgbl->dgtCard.CHARGE_DIVDATA);

		}
		else
		{
			//����ǿ���		
			COMMAND_LEN=strlen(pgbl->dgtCard.APDU);
			sprintf(dgtFIELD32,"%03d",COMMAND_LEN);
			strcpy(dgtFIELD33,pgbl->dgtCard.APDU);
			strcpy(dgtFIELD35,pgbl->dgtCard.CARDSALE_DIVDATA);
			strcpy(dgtFIELD40,pgbl->dgtCard.CARDSALE_DIVDATA);
			memcpy(dgtFIELD22,pgbl->amount+4,8); //ȡ��8λ�� //Ӧ�ս��
			strcpy(dgtFIELD23,pgbl->dgtCard.tradeMny);
			CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD24,4);


		}
	
		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;		Pack_dgt_table[0].Max_Length = 8;       Pack_dgt_table[0].optionFlag='M';	//Unitid
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;		Pack_dgt_table[1].Max_Length = 12;		Pack_dgt_table[1].optionFlag='M';	//Posid
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;		Pack_dgt_table[2].Max_Length = 16;		Pack_dgt_table[2].optionFlag='C';	//SamId
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;		Pack_dgt_table[3].Max_Length = 9;		Pack_dgt_table[3].optionFlag='M';	//POS����ˮ��
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;		Pack_dgt_table[4].Max_Length = 12;		Pack_dgt_table[4].optionFlag='M';	//��ҵ�豸����
		Pack_dgt_table[5].Type = NUMERIC;		Pack_dgt_table[5].Format = FIXED;		Pack_dgt_table[5].Max_Length = 16;		Pack_dgt_table[5].optionFlag='M';	//����Ա��
		Pack_dgt_table[6].Type = NUMERIC; 		Pack_dgt_table[6].Format = FIXED;		Pack_dgt_table[6].Max_Length = 16;		Pack_dgt_table[6].optionFlag='C';	//������������
		Pack_dgt_table[7].Type = NUMERIC; 		Pack_dgt_table[7].Format = FIXED;		Pack_dgt_table[7].Max_Length = 4;		Pack_dgt_table[7].optionFlag='M';	//��������������
		Pack_dgt_table[8].Type = NUMERIC;	    Pack_dgt_table[8].Format = FIXED;  		Pack_dgt_table[8].Max_Length = 4;		Pack_dgt_table[8].optionFlag='M';	//���д���
		Pack_dgt_table[9].Type = NUMERIC;	    Pack_dgt_table[9].Format = FIXED;	    Pack_dgt_table[9].Max_Length = 16;		Pack_dgt_table[9].optionFlag='M';	//���ں�
	
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;		Pack_dgt_table[10].Max_Length = 2;		Pack_dgt_table[10].optionFlag='M';	//��������
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;		Pack_dgt_table[11].Max_Length = 2;		Pack_dgt_table[11].optionFlag='M';	//�ӿ�����
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;		Pack_dgt_table[12].Max_Length = 1;		Pack_dgt_table[12].optionFlag='M';	//���ͣ� 1-CPU��
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;		Pack_dgt_table[13].Max_Length = 2;		Pack_dgt_table[13].optionFlag='M';	//ҵ������
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;		Pack_dgt_table[14].Max_Length = 16;		Pack_dgt_table[14].optionFlag='C';	//�������ʾ���
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;		Pack_dgt_table[15].Max_Length = 8;		Pack_dgt_table[15].optionFlag='C';	//��У���루MF1��
		Pack_dgt_table[16].Type = NUMERIC;		Pack_dgt_table[16].Format = FIXED;	Pack_dgt_table[16].Max_Length = 14;			Pack_dgt_table[16].optionFlag='C';	//����ʱ�䣬�������豸У��ʱ��
		Pack_dgt_table[17].Type = NUMERIC;		Pack_dgt_table[17].Format = FIXED;	Pack_dgt_table[17].Max_Length = 2;			Pack_dgt_table[17].optionFlag='C';	//�·�key������(M1)
		Pack_dgt_table[18].Type = NUMERIC;		Pack_dgt_table[18].Format = FIXED;	Pack_dgt_table[18].Max_Length = 4;			Pack_dgt_table[18].optionFlag='C';	//SectorID
		Pack_dgt_table[19].Type = NUMERIC;		Pack_dgt_table[19].Format = FIXED;	Pack_dgt_table[19].Max_Length = 64;			Pack_dgt_table[19].optionFlag='C';	//KeySet ��Կ(KeyB)(M1)
		
		Pack_dgt_table[20].Type = NUMERIC;		Pack_dgt_table[20].Format = FIXED;    Pack_dgt_table[20].Max_Length = 32;		Pack_dgt_table[20].optionFlag='C';	//TACKeySet
		Pack_dgt_table[21].Type = NUMERIC;		Pack_dgt_table[21].Format = FIXED;	Pack_dgt_table[21].Max_Length = 8;			Pack_dgt_table[21].optionFlag='M';	//Deposit
		Pack_dgt_table[22].Type = NUMERIC;		Pack_dgt_table[22].Format = FIXED ;	Pack_dgt_table[22].Max_Length = 8;			Pack_dgt_table[22].optionFlag='M';	//OrigAmt
		Pack_dgt_table[23].Type = NUMERIC;		Pack_dgt_table[23].Format = FIXED;	Pack_dgt_table[23].Max_Length = 8;			Pack_dgt_table[23].optionFlag='M';	//ReloadBal
		Pack_dgt_table[24].Type = NUMERIC;		Pack_dgt_table[24].Format = FIXED;	Pack_dgt_table[24].Max_Length = 8;			Pack_dgt_table[24].optionFlag='M';	//CardValDate
		Pack_dgt_table[25].Type = ALPHANUMS;	Pack_dgt_table[25].Format = FIXED;	Pack_dgt_table[25].Max_Length = 8;			Pack_dgt_table[25].optionFlag='C';	//MAC2
		Pack_dgt_table[26].Type = NUMERIC;		Pack_dgt_table[26].Format = FIXED;	Pack_dgt_table[26].Max_Length = 8;			Pack_dgt_table[26].optionFlag='M';	//SrcBal
		Pack_dgt_table[27].Type = NUMERIC;	 Pack_dgt_table[27].Format = FIXED;		Pack_dgt_table[27].Max_Length = 6;				Pack_dgt_table[27].optionFlag='M';	//CardSeq
		Pack_dgt_table[28].Type = NUMERIC;	 Pack_dgt_table[28].Format = FIXED;	Pack_dgt_table[28].Max_Length = 2;				Pack_dgt_table[28].optionFlag='M';	//KeyVer
		Pack_dgt_table[29].Type = NUMERIC;	Pack_dgt_table[29].Format = FIXED;	Pack_dgt_table[29].Max_Length = 2;				Pack_dgt_table[29].optionFlag='M';	//AlgInd
		
		Pack_dgt_table[30].Type = NUMERIC;  Pack_dgt_table[30].Format = FIXED;	Pack_dgt_table[30].Max_Length = 8;				Pack_dgt_table[30].optionFlag='M';	//CardRand
		Pack_dgt_table[31].Type = NUMERIC;	Pack_dgt_table[31].Format = FIXED;	Pack_dgt_table[31].Max_Length = 8;				Pack_dgt_table[31].optionFlag='M';	//MAC1
		Pack_dgt_table[32].Type = NUMERIC;  Pack_dgt_table[32].Format = FIXED;	Pack_dgt_table[32].Max_Length = 3;				Pack_dgt_table[32].optionFlag='C';	//CommandLen
		Pack_dgt_table[33].Type = ALPHANUMS;	Pack_dgt_table[33].Format = LLLVAR;	Pack_dgt_table[33].Max_Length = 999;		Pack_dgt_table[33].optionFlag='C';	//Command
		Pack_dgt_table[34].Type = ALPHANUMS;  Pack_dgt_table[34].Format = FIXED;	Pack_dgt_table[34].Max_Length = 16;				Pack_dgt_table[34].optionFlag='C';	//DicMAC
		Pack_dgt_table[35].Type = ALPHANUMS;	Pack_dgt_table[35].Format = FIXED;	Pack_dgt_table[35].Max_Length = 18;		Pack_dgt_table[35].optionFlag='C';	//DivData
		Pack_dgt_table[36].Type = ALPHANUMS;  Pack_dgt_table[36].Format = FIXED;	Pack_dgt_table[36].Max_Length = 8;				Pack_dgt_table[36].optionFlag='C';	//SettDate
		Pack_dgt_table[37].Type = ALPHANUMS;	Pack_dgt_table[37].Format = FIXED;	Pack_dgt_table[37].Max_Length = 6;		Pack_dgt_table[37].optionFlag='M';	//BatchNo
		Pack_dgt_table[38].Type = NUMERIC;	Pack_dgt_table[38].Format = FIXED;		Pack_dgt_table[38].Max_Length = 10;		Pack_dgt_table[38].optionFlag='C';	//��Կ��Ȩ��ˮ��
		Pack_dgt_table[39].Type = NUMERIC;	Pack_dgt_table[39].Format = FIXED;		Pack_dgt_table[39].Max_Length = 10;		Pack_dgt_table[39].optionFlag='C';	//�����Ȩ��ˮ��
	
		Pack_dgt_table[40].Type = ALPHANUMS;	Pack_dgt_table[40].Format = FIXED;	Pack_dgt_table[40].Max_Length = 20;		Pack_dgt_table[40].optionFlag='C';	//������
		Pack_dgt_table[41].Type = ALPHANUMS;	Pack_dgt_table[41].Format = FIXED;	Pack_dgt_table[41].Max_Length = 5;		Pack_dgt_table[41].optionFlag='C';	//����Ӧ����
	
		Pack_dgt_table[0].Rev_Data = dgtFIELD0;		Pack_dgt_table[0].Send_Data = dgtFIELD0;
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;		Pack_dgt_table[1].Send_Data = dgtFIELD1;
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;		Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;		Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;		Pack_dgt_table[4].Send_Data= dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;		Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;		Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;		Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;		Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;		Pack_dgt_table[9].Send_Data = dgtFIELD9;

		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;		Pack_dgt_table[19].Send_Data = dgtFIELD19;
		
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;	Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;	Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;	Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;	Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;	Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;	Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;	Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;	Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;	Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;	Pack_dgt_table[29].Send_Data = dgtFIELD29;

		Pack_dgt_table[30].Rev_Data = dgtFIELD30;	Pack_dgt_table[30].Send_Data = dgtFIELD30;
		Pack_dgt_table[31].Rev_Data = dgtFIELD31;	Pack_dgt_table[31].Send_Data = dgtFIELD31;
		Pack_dgt_table[32].Rev_Data = dgtFIELD32;	Pack_dgt_table[32].Send_Data = dgtFIELD32;
		Pack_dgt_table[33].Rev_Data = dgtFIELD33;	Pack_dgt_table[33].Send_Data = dgtFIELD33;
		Pack_dgt_table[34].Rev_Data=dgtFIELD34;		Pack_dgt_table[34].Send_Data=	dgtFIELD34;
		Pack_dgt_table[35].Rev_Data=dgtFIELD35;		Pack_dgt_table[35].Send_Data=	dgtFIELD35;
		Pack_dgt_table[36].Rev_Data=dgtFIELD36;		Pack_dgt_table[36].Send_Data=	dgtFIELD36;
		Pack_dgt_table[37].Rev_Data=dgtFIELD37;		Pack_dgt_table[37].Send_Data=	dgtFIELD37;
		Pack_dgt_table[38].Rev_Data=dgtFIELD38;		Pack_dgt_table[38].Send_Data=	dgtFIELD38;
		Pack_dgt_table[39].Rev_Data=dgtFIELD39;		Pack_dgt_table[39].Send_Data=	dgtFIELD39;
	
		Pack_dgt_table[40].Rev_Data=dgtFIELD40;		Pack_dgt_table[40].Send_Data=	dgtFIELD40;
		Pack_dgt_table[41].Rev_Data=dgtFIELD41;		Pack_dgt_table[41].Send_Data=	dgtFIELD41;

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"Posid");
		strcpy(Pack_dgt_table[2].name,"SamId");
		strcpy(Pack_dgt_table[3].name,"PosSequence");
		strcpy(Pack_dgt_table[4].name,"Termid");
		strcpy(Pack_dgt_table[5].name,"Operid");
		strcpy(Pack_dgt_table[6].name,"EDCardId");
		strcpy(Pack_dgt_table[7].name,"CardOprType");
		strcpy(Pack_dgt_table[8].name,"CItyCode");
		strcpy(Pack_dgt_table[9].name,"CardId");

		strcpy(Pack_dgt_table[10].name,"CardMKnd");
		strcpy(Pack_dgt_table[11].name,"CardSKnd");
		strcpy(Pack_dgt_table[12].name,"CardModel");
		strcpy(Pack_dgt_table[13].name,"TransType");
		strcpy(Pack_dgt_table[14].name,"CardMac");
		strcpy(Pack_dgt_table[15].name,"CardCSN");
		strcpy(Pack_dgt_table[16].name,"SysDatetime");
		strcpy(Pack_dgt_table[17].name,"KeyCnt");
		strcpy(Pack_dgt_table[18].name,"SectorID");
		strcpy(Pack_dgt_table[19].name,"KeySet");

		strcpy(Pack_dgt_table[20].name,"TACKeySet");
		strcpy(Pack_dgt_table[21].name,"Deposit");
		strcpy(Pack_dgt_table[22].name,"OrigAmt");
		strcpy(Pack_dgt_table[23].name,"ReloadBal");
		strcpy(Pack_dgt_table[24].name,"CardValDate");
		strcpy(Pack_dgt_table[25].name,"MAC2");
		strcpy(Pack_dgt_table[26].name,"SrcBal");
		strcpy(Pack_dgt_table[27].name,"CardSeq");
		strcpy(Pack_dgt_table[28].name,"KeyVer");
		strcpy(Pack_dgt_table[29].name,"AlgInd");

		strcpy(Pack_dgt_table[30].name,"CardRand");
		strcpy(Pack_dgt_table[31].name,"MAC1");
		strcpy(Pack_dgt_table[32].name,"CommandLen");
		strcpy(Pack_dgt_table[33].name,"Command");
		strcpy(Pack_dgt_table[34].name,"DicMAC");
		strcpy(Pack_dgt_table[35].name,"DivData");
		strcpy(Pack_dgt_table[36].name,"SettDate");
		strcpy(Pack_dgt_table[37].name,"BatchNo");
		strcpy(Pack_dgt_table[38].name,"AuthSeq");
		strcpy(Pack_dgt_table[39].name,"LimitedAuthSeql");

		strcpy(Pack_dgt_table[40].name,"Reserved");
		strcpy(Pack_dgt_table[41].name,"rspcode");
		wrLog("pack_dgt_table:%s",Pack_dgt_table[0].Send_Data);

		break;
	case 2063:
		//�ۿ�
		
		max_count=30;

		initField();
		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->dgt_LocalSequence); //��ˮ��
		strcpy(dgtFIELD2,tranType);
		strcpy(dgtFIELD3,pgbl->posId);
		strcpy(dgtFIELD5,pgbl->dgt_posSerial);
		strcpy(dgtFIELD6,pgbl->busi_id);
		strcpy(dgtFIELD7,pgbl->busi_id);
		strcpy(dgtFIELD8,EDCARDID_field);

		strcpy(dgtFIELD9,pgbl->dgtCard.cardShowId);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardParent_type,(BYTE *)dgtFIELD11,1);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardChild_type,(BYTE *)dgtFIELD12,1);

		strcpy(dgtFIELD13,"1");
		strcpy(dgtFIELD14,pgbl->dgtCard.sale_type); //�ۿ�����

		strcpy(dgtFIELD15,deposit_field);
		strcpy(dgtFIELD16,pgbl->dgtCard.tradeBeForeMny);
		strcpy(dgtFIELD17,pgbl->dgtCard.tradeMny);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD18,4);
		strcpy(dgtFIELD19,pgbl->dgtCard.tradeDate);
		strcpy(dgtFIELD20,pgbl->dgtCard.tradeTime);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.CItyCode,(BYTE *) dgtFIELD21,2);
		strcpy(dgtFIELD23,pgbl->dgtCard.SETDATE);
		strcpy(dgtFIELD24,pgbl->dgtSignInfo.BATCHNO); //�޸�Ϊǩ�����ص����κ�
		strcpy(dgtFIELD25,AuthSeq_field);
		strcpy(dgtFIELD26,pgbl->dgtCard.LIMITEAUTHSEQL);

		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;		Pack_dgt_table[0].Max_Length = 8;		Pack_dgt_table[0].optionFlag='M';	//Unitid
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;		Pack_dgt_table[1].Max_Length = 10;		Pack_dgt_table[1].optionFlag='M';	//LocalSequence
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;		Pack_dgt_table[2].Max_Length = 2;		Pack_dgt_table[2].optionFlag='M';	//TransType
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;		Pack_dgt_table[3].Max_Length = 12;		Pack_dgt_table[3].optionFlag='M';	//PosId
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;		Pack_dgt_table[4].Max_Length = 16;		Pack_dgt_table[4].optionFlag='C';	//SamId
		Pack_dgt_table[5].Type = NUMERIC;		Pack_dgt_table[5].Format = FIXED;		Pack_dgt_table[5].Max_Length = 9;		Pack_dgt_table[5].optionFlag='M';	//PosSequence
		Pack_dgt_table[6].Type = NUMERIC; 		Pack_dgt_table[6].Format = FIXED;	    Pack_dgt_table[6].Max_Length = 12;		Pack_dgt_table[6].optionFlag='M';	//TermId
		Pack_dgt_table[7].Type = NUMERIC; 		Pack_dgt_table[7].Format = FIXED;	    Pack_dgt_table[7].Max_Length = 16;		Pack_dgt_table[7].optionFlag='M';	//OperId
		Pack_dgt_table[8].Type = NUMERIC;	    Pack_dgt_table[8].Format = FIXED;		Pack_dgt_table[8].Max_Length = 16;		Pack_dgt_table[8].optionFlag='C';	//EdCardId
		Pack_dgt_table[9].Type = NUMERIC;	    Pack_dgt_table[9].Format = FIXED;	    Pack_dgt_table[9].Max_Length = 16;		Pack_dgt_table[9].optionFlag='M';	//CardId
	
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;		Pack_dgt_table[10].Max_Length = 6;		Pack_dgt_table[10].optionFlag='M';	//CardCnt
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;		Pack_dgt_table[11].Max_Length = 2;		Pack_dgt_table[11].optionFlag='M';	//CardMKnd
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;		Pack_dgt_table[12].Max_Length = 2;		Pack_dgt_table[12].optionFlag='M';	//CardSKind
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;		Pack_dgt_table[13].Max_Length = 1;		Pack_dgt_table[13].optionFlag='M';	//CardModel
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;		Pack_dgt_table[14].Max_Length = 2;		Pack_dgt_table[14].optionFlag='M';	//SaleMode
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;		Pack_dgt_table[15].Max_Length = 8;		Pack_dgt_table[15].optionFlag='M';	//Deposit
		Pack_dgt_table[16].Type = NUMERIC;	Pack_dgt_table[16].Format = FIXED;			Pack_dgt_table[16].Max_Length = 8; 		Pack_dgt_table[16].optionFlag='M';	//BefBalance
		Pack_dgt_table[17].Type = NUMERIC;	Pack_dgt_table[17].Format = FIXED;			Pack_dgt_table[17].Max_Length = 8;		Pack_dgt_table[17].optionFlag='M';	//TxnAmt
		Pack_dgt_table[18].Type = NUMERIC;	Pack_dgt_table[18].Format = FIXED;			Pack_dgt_table[18].Max_Length = 8; 		Pack_dgt_table[18].optionFlag='M';	//CardValDate
		Pack_dgt_table[19].Type = NUMERIC;	Pack_dgt_table[19].Format = FIXED;			Pack_dgt_table[19].Max_Length = 8; 		Pack_dgt_table[19].optionFlag='M';	//TxnDate
		
		Pack_dgt_table[20].Type = NUMERIC;	Pack_dgt_table[20].Format = FIXED;			Pack_dgt_table[20].Max_Length = 6;		Pack_dgt_table[20].optionFlag='M';	//TxnTime
		Pack_dgt_table[21].Type = NUMERIC;	Pack_dgt_table[21].Format = FIXED;			Pack_dgt_table[21].Max_Length = 4;		Pack_dgt_table[21].optionFlag='M';	//CityCode
		Pack_dgt_table[22].Type = NUMERIC;	Pack_dgt_table[22].Format = FIXED ;			Pack_dgt_table[22].Max_Length = 2;		Pack_dgt_table[22].optionFlag='M';	//CardVerNo
		Pack_dgt_table[23].Type = NUMERIC;	Pack_dgt_table[23].Format = FIXED;			Pack_dgt_table[23].Max_Length = 8;		Pack_dgt_table[23].optionFlag='M';	//SettDate
		Pack_dgt_table[24].Type = NUMERIC;  Pack_dgt_table[24].Format = FIXED;			Pack_dgt_table[24].Max_Length = 6;		Pack_dgt_table[24].optionFlag='M';	//BatchNo
		Pack_dgt_table[25].Type = NUMERIC;		Pack_dgt_table[25].Format = FIXED;		Pack_dgt_table[25].Max_Length = 18;		Pack_dgt_table[25].optionFlag='M';	//AuthSeq
		Pack_dgt_table[26].Type = NUMERIC;	Pack_dgt_table[26].Format = FIXED;			Pack_dgt_table[26].Max_Length = 10;		Pack_dgt_table[26].optionFlag='M';	//LimitedAuthSeq
		Pack_dgt_table[27].Type = ALPHANUMS;  Pack_dgt_table[27].Format = FIXED;		Pack_dgt_table[27].Max_Length = 20;		Pack_dgt_table[27].optionFlag='M';	//Reserved
		Pack_dgt_table[28].Type = NUMERIC;  Pack_dgt_table[28].Format = FIXED;			Pack_dgt_table[28].Max_Length = 8;		Pack_dgt_table[28].optionFlag='M';	//TAC
		Pack_dgt_table[29].Type = NUMERIC;  Pack_dgt_table[29].Format = FIXED;			Pack_dgt_table[29].Max_Length = 5;		Pack_dgt_table[29].optionFlag='M';	//Response Code
		

		Pack_dgt_table[0].Rev_Data = dgtFIELD0;			Pack_dgt_table[0].Send_Data = dgtFIELD0;
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;			Pack_dgt_table[1].Send_Data = dgtFIELD1;
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;			Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;			Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;			Pack_dgt_table[4].Send_Data = dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;			Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;			Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;			Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;			Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;			Pack_dgt_table[9].Send_Data = dgtFIELD9;
		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;	Pack_dgt_table[19].Send_Data = dgtFIELD19;
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;	Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;	Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;	Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;	Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;	Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;	Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;	Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;	Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;	Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;	Pack_dgt_table[29].Send_Data = dgtFIELD29;

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"LocalSequence");
		strcpy(Pack_dgt_table[2].name,"TransType");
		strcpy(Pack_dgt_table[3].name,"PosId");
		strcpy(Pack_dgt_table[4].name,"SamId");
		strcpy(Pack_dgt_table[5].name,"PosSequence");
		strcpy(Pack_dgt_table[6].name,"TermId");
		strcpy(Pack_dgt_table[7].name,"OperId");
		strcpy(Pack_dgt_table[8].name,"EdCardId");
		strcpy(Pack_dgt_table[9].name,"CardId");
		strcpy(Pack_dgt_table[10].name,"CardCnt");
		strcpy(Pack_dgt_table[11].name,"CardMKnd");
		strcpy(Pack_dgt_table[12].name,"CardSKind");
		strcpy(Pack_dgt_table[13].name,"CardModel");
		strcpy(Pack_dgt_table[14].name,"SaleMode");
		strcpy(Pack_dgt_table[15].name,"Deposit");
		strcpy(Pack_dgt_table[16].name,"BefBalance");
		strcpy(Pack_dgt_table[17].name,"TxnAmt");
		strcpy(Pack_dgt_table[18].name,"CardValDate");
		strcpy(Pack_dgt_table[19].name,"TxnDate");
		strcpy(Pack_dgt_table[20].name,"TxnTime");
		strcpy(Pack_dgt_table[21].name,"CityCode");
		strcpy(Pack_dgt_table[22].name,"CardVerNo");
		strcpy(Pack_dgt_table[23].name,"SettDate");
		strcpy(Pack_dgt_table[24].name,"BatchNo");
		strcpy(Pack_dgt_table[25].name,"AuthSeq");
		strcpy(Pack_dgt_table[26].name,"LimitedAuthSeq");
		strcpy(Pack_dgt_table[27].name,"Reserved");
		strcpy(Pack_dgt_table[28].name,"TAC");
		strcpy(Pack_dgt_table[29].name,"RSPCODE");

		break;

	case 3458: //�������� 
	case 3480: //�����ȡ
	case 3481: //�����ȡ����

		//��Ϊ������ ����

		max_count=39;
		
		initField();
		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->dgt_LocalSequence);
		strcpy(dgtFIELD2,tranType);
		strcpy(dgtFIELD4,pgbl->posId);
		strcpy(dgtFIELD5,pgbl->dgt_posSerial);
		strcpy(dgtFIELD6,pgbl->busi_id);
		strcpy(dgtFIELD7,pgbl->busi_id);
		strcpy(dgtFIELD8,EDCARDID_field);
		strcpy(dgtFIELD10,pgbl->dgtM1Card.bus_showCard);
	
		CTools::addZero(pgbl->dgtM1Card.bus_showCard,16,1,dgtFIELD11);
		strcpy(dgtFIELD12,pgbl->dgtM1Card.city_code);
		strcpy(dgtFIELD13,pgbl->dgtM1Card.card_parent_type);
		strcpy(dgtFIELD14,pgbl->dgtM1Card.card_child_type);
		strcpy(dgtFIELD15,"3"); //��Ƭ���͡���3 M1
		strcpy(dgtFIELD16,pgbl->dgtM1Card.openDate); //����ʱ��
		strcpy(dgtFIELD18,pgbl->dgtM1Card.cardVadate);
		strcpy(dgtFIELD20,pgbl->dgtM1Card.bus_amt);
		strcpy(dgtFIELD21,deposit_field);
		strcpy(dgtFIELD22,"0");
		strcpy(dgtFIELD23,"0");
		strcpy(dgtFIELD31,pgbl->dgtSignInfo.BATCHNO); //�޸�Ϊǩ����ȡ�����κ�
		strcpy(dgtFIELD34,pgbl->dgtCard.tradeDate);
		strcpy(dgtFIELD35,pgbl->dgtCard.tradeTime);

		if (strcmp(transMessage,"3458")!=0) //����ǻ�������
		{

			strcpy(dgtFIELD3,pgbl->Centerseqno);
			strcpy(dgtFIELD24,pgbl->ReturnAmt);
			strcpy(dgtFIELD25,pgbl->ReturnDeposit);
			
			strcpy(dgtFIELD26,pgbl->MonthFeeFlag);
			strcpy(dgtFIELD27,pgbl->FeeAmtFlag);
			strcpy(dgtFIELD28,pgbl->MonthFeeAmt);
			strcpy(dgtFIELD29,pgbl->FeeAmt);
			strcpy(dgtFIELD32,AuthSeq_field);

		}

		if (strcmp(transMessage,"3480")==0) //����Ƿ������ȡ���ģ�������д3458(�����뱨�ĵĽ�������)����16���ո�
		{		
			bbzero(dgtFIELD36);
			CTools::addSpace("3458",20,0,dgtFIELD36); //���油16���ո�

		}
	
		//ANS16 CardCSN���Ҳ��ո�3480�����ȡ����ʱ�������뽻�����ͣ��磺�ڻ�����ɽ����������ȡ���������д�3458�����Ҳ��ո�


		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;	    Pack_dgt_table[0].Max_Length = 8;	Pack_dgt_table[0].optionFlag='M';	//Unitid	
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;	    Pack_dgt_table[1].Max_Length = 10;	Pack_dgt_table[1].optionFlag='M';	//LocalSequence	
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;	    Pack_dgt_table[2].Max_Length = 2;	Pack_dgt_table[2].optionFlag='M';	//TransType	
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;	    Pack_dgt_table[3].Max_Length = 18;	Pack_dgt_table[3].optionFlag='C';	//CenterSeqno	
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;	    Pack_dgt_table[4].Max_Length = 12;	Pack_dgt_table[4].optionFlag='M';	//Posid	
		Pack_dgt_table[5].Type = NUMERIC;		Pack_dgt_table[5].Format = FIXED;	    Pack_dgt_table[5].Max_Length = 9;	Pack_dgt_table[5].optionFlag='M';	//PosSequence	
		Pack_dgt_table[6].Type = NUMERIC;		Pack_dgt_table[6].Format = FIXED;	    Pack_dgt_table[6].Max_Length = 12;	Pack_dgt_table[6].optionFlag='M';	//TremId	
		Pack_dgt_table[7].Type = NUMERIC;		Pack_dgt_table[7].Format = FIXED;	    Pack_dgt_table[7].Max_Length = 16;	Pack_dgt_table[7].optionFlag='M';	//Operid	
		Pack_dgt_table[8].Type = NUMERIC;		Pack_dgt_table[8].Format = FIXED;	    Pack_dgt_table[8].Max_Length = 16;	Pack_dgt_table[8].optionFlag='M';	//EdCardId	
		Pack_dgt_table[9].Type = NUMERIC;		Pack_dgt_table[9].Format = FIXED;	    Pack_dgt_table[9].Max_Length = 16;	Pack_dgt_table[9].optionFlag='C';	//SamId	
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;	    Pack_dgt_table[10].Max_Length = 16;	Pack_dgt_table[10].optionFlag='M';	//CardFaceId	
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;	    Pack_dgt_table[11].Max_Length = 16;	Pack_dgt_table[11].optionFlag='M';	//CardId	
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;	    Pack_dgt_table[12].Max_Length = 4;	Pack_dgt_table[12].optionFlag='M';	//CityCode	
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;	    Pack_dgt_table[13].Max_Length = 2;	Pack_dgt_table[13].optionFlag='M';	//CardMKnd	
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;	    Pack_dgt_table[14].Max_Length = 2;	Pack_dgt_table[14].optionFlag='M';	//CardSKind	
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;	    Pack_dgt_table[15].Max_Length = 1;	Pack_dgt_table[15].optionFlag='M';	//CardModel	
		Pack_dgt_table[16].Type = NUMERIC;		Pack_dgt_table[16].Format = FIXED;	    Pack_dgt_table[16].Max_Length = 8;	Pack_dgt_table[16].optionFlag='M';	//CardSellDate	
		Pack_dgt_table[17].Type = ALPHANUM;		Pack_dgt_table[17].Format = FIXED;	    Pack_dgt_table[17].Max_Length = 8;	Pack_dgt_table[17].optionFlag='C';	//CardBusInessBit	
		Pack_dgt_table[18].Type = NUMERIC;		Pack_dgt_table[18].Format = FIXED;	    Pack_dgt_table[18].Max_Length = 8;	Pack_dgt_table[18].optionFlag='M';	//CardVal	
		Pack_dgt_table[19].Type = NUMERIC;		Pack_dgt_table[19].Format = FIXED;	    Pack_dgt_table[19].Max_Length = 6;	Pack_dgt_table[19].optionFlag='M';	//CardCnt	
		Pack_dgt_table[20].Type = NUMERIC;		Pack_dgt_table[20].Format = FIXED;	    Pack_dgt_table[20].Max_Length = 10;	Pack_dgt_table[20].optionFlag='M';	//Balance before	
		Pack_dgt_table[21].Type = NUMERIC;		Pack_dgt_table[21].Format = FIXED;	    Pack_dgt_table[21].Max_Length = 8;	Pack_dgt_table[21].optionFlag='M';	//Deposit amout	
		Pack_dgt_table[22].Type = NUMERIC;		Pack_dgt_table[22].Format = FIXED;	    Pack_dgt_table[22].Max_Length = 1;	Pack_dgt_table[22].optionFlag='M';	//ChipBadType
		Pack_dgt_table[23].Type = NUMERIC;		Pack_dgt_table[23].Format = FIXED;	    Pack_dgt_table[23].Max_Length = 1;	Pack_dgt_table[23].optionFlag='M';	//CardFaceBadType
		Pack_dgt_table[24].Type = NUMERIC;		Pack_dgt_table[24].Format = FIXED;	    Pack_dgt_table[24].Max_Length = 10;	Pack_dgt_table[24].optionFlag='C';	//Return amount
		Pack_dgt_table[25].Type = NUMERIC;		Pack_dgt_table[25].Format = FIXED;	    Pack_dgt_table[25].Max_Length = 8;	Pack_dgt_table[25].optionFlag='C';	//Return Deposit
		Pack_dgt_table[26].Type = NUMERIC;		Pack_dgt_table[26].Format = FIXED;	    Pack_dgt_table[26].Max_Length = 1;	Pack_dgt_table[26].optionFlag='C';	//Month Fee flag
		Pack_dgt_table[27].Type = NUMERIC;		Pack_dgt_table[27].Format = FIXED;	    Pack_dgt_table[27].Max_Length = 1;	Pack_dgt_table[27].optionFlag='C';	//Fee amount flag
		Pack_dgt_table[28].Type = NUMERIC;		Pack_dgt_table[28].Format = FIXED;	    Pack_dgt_table[28].Max_Length = 8;	Pack_dgt_table[28].optionFlag='C';	//Month Fee amount
		Pack_dgt_table[29].Type = NUMERIC;		Pack_dgt_table[29].Format = FIXED;	    Pack_dgt_table[29].Max_Length = 8;	Pack_dgt_table[29].optionFlag='C';	//Fee amount
		Pack_dgt_table[30].Type = NUMERIC;		Pack_dgt_table[30].Format = FIXED;	    Pack_dgt_table[30].Max_Length = 8;	Pack_dgt_table[30].optionFlag='C';	//settime
		Pack_dgt_table[31].Type = NUMERIC;		Pack_dgt_table[31].Format = FIXED;	    Pack_dgt_table[31].Max_Length = 6;	Pack_dgt_table[31].optionFlag='M';	//BatchNo
		Pack_dgt_table[32].Type = NUMERIC;		Pack_dgt_table[32].Format = FIXED;	    Pack_dgt_table[32].Max_Length = 18;	Pack_dgt_table[32].optionFlag='M';	//AuthSeq
		Pack_dgt_table[33].Type = NUMERIC;		Pack_dgt_table[33].Format = FIXED;	    Pack_dgt_table[33].Max_Length = 1;	Pack_dgt_table[33].optionFlag='C';	//DelayFlag
		Pack_dgt_table[34].Type = NUMERIC;		Pack_dgt_table[34].Format = FIXED;	    Pack_dgt_table[34].Max_Length = 8;	Pack_dgt_table[34].optionFlag='M';	//TxnDate
		Pack_dgt_table[35].Type = NUMERIC;		Pack_dgt_table[35].Format = FIXED;	    Pack_dgt_table[35].Max_Length = 6;	Pack_dgt_table[35].optionFlag='M';	//TxnTime
		Pack_dgt_table[36].Type = ALPHANUM;		Pack_dgt_table[36].Format = FIXED;	    Pack_dgt_table[36].Max_Length = 20;	Pack_dgt_table[36].optionFlag='C';	//Reserved
		Pack_dgt_table[37].Type = ALPHANUM;		Pack_dgt_table[37].Format = FIXED;	    Pack_dgt_table[37].Max_Length = 50;	Pack_dgt_table[37].optionFlag='C';	//MONTHRESERVED  M1������������
		Pack_dgt_table[38].Type = NUMERIC;		Pack_dgt_table[38].Format = FIXED;	    Pack_dgt_table[38].Max_Length = 5;	Pack_dgt_table[38].optionFlag='C';	//RSP_CODE
		
		Pack_dgt_table[0].Rev_Data = dgtFIELD0;		Pack_dgt_table[0].Send_Data = dgtFIELD0;
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;		Pack_dgt_table[1].Send_Data = dgtFIELD1;
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;		Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;		Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;		Pack_dgt_table[4].Send_Data= dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;		Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;		Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;		Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;		Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;		Pack_dgt_table[9].Send_Data = dgtFIELD9;
		
		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;		Pack_dgt_table[19].Send_Data = dgtFIELD19;
		
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;	Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;	Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;	Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;	Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;	Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;	Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;	Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;	Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;	Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;	Pack_dgt_table[29].Send_Data = dgtFIELD29;
		
		Pack_dgt_table[30].Rev_Data = dgtFIELD30;	Pack_dgt_table[30].Send_Data = dgtFIELD30;
		Pack_dgt_table[31].Rev_Data = dgtFIELD31;	Pack_dgt_table[31].Send_Data = dgtFIELD31;
		Pack_dgt_table[32].Rev_Data = dgtFIELD32;	Pack_dgt_table[32].Send_Data = dgtFIELD32;
		Pack_dgt_table[33].Rev_Data = dgtFIELD33;	Pack_dgt_table[33].Send_Data = dgtFIELD33;
		Pack_dgt_table[34].Rev_Data=dgtFIELD34;		Pack_dgt_table[34].Send_Data=	dgtFIELD34;
		Pack_dgt_table[35].Rev_Data=dgtFIELD35;		Pack_dgt_table[35].Send_Data=	dgtFIELD35;
		Pack_dgt_table[36].Rev_Data=dgtFIELD36;		Pack_dgt_table[36].Send_Data=	dgtFIELD36;
		Pack_dgt_table[37].Rev_Data=dgtFIELD37;		Pack_dgt_table[37].Send_Data=	dgtFIELD37;
		Pack_dgt_table[38].Rev_Data=dgtFIELD38;		Pack_dgt_table[38].Send_Data=	dgtFIELD38;

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"LocalSequence");
		strcpy(Pack_dgt_table[2].name,"TransType");
		strcpy(Pack_dgt_table[3].name,"CenterSeqno");
		strcpy(Pack_dgt_table[4].name,"Posid");
		strcpy(Pack_dgt_table[5].name,"PosSequence");
		strcpy(Pack_dgt_table[6].name,"termid");
		strcpy(Pack_dgt_table[7].name,"Operid");
		strcpy(Pack_dgt_table[8].name,"EdCardId");
		strcpy(Pack_dgt_table[9].name,"SamId");
		strcpy(Pack_dgt_table[10].name,"CardFaceId");
		strcpy(Pack_dgt_table[11].name,"CardId");
		strcpy(Pack_dgt_table[12].name,"CityCode");
		strcpy(Pack_dgt_table[13].name,"CardMKnd");
		strcpy(Pack_dgt_table[14].name,"CardSKind");
		strcpy(Pack_dgt_table[15].name,"CardModel");
		strcpy(Pack_dgt_table[16].name,"CardSellDate");
		strcpy(Pack_dgt_table[17].name,"CardBusInessBit");
		strcpy(Pack_dgt_table[18].name,"CardVal");
		strcpy(Pack_dgt_table[19].name,"CardCnt");
		strcpy(Pack_dgt_table[20].name,"BalanceBef");
		strcpy(Pack_dgt_table[21].name,"DepositAmt");
		strcpy(Pack_dgt_table[22].name,"ChipBadType");
		strcpy(Pack_dgt_table[23].name,"CardFaceBadType");
		strcpy(Pack_dgt_table[24].name,"ReturnAmt");
		strcpy(Pack_dgt_table[25].name,"ReturnDeposit");
		strcpy(Pack_dgt_table[26].name,"MonthFeeFlag");
		strcpy(Pack_dgt_table[27].name,"FeeAmtFlag");
		strcpy(Pack_dgt_table[28].name,"MonthFeeAmt");
		strcpy(Pack_dgt_table[29].name,"FeeAmt");
		strcpy(Pack_dgt_table[30].name,"setTime");
		strcpy(Pack_dgt_table[31].name,"BatchNo");
		strcpy(Pack_dgt_table[32].name,"AuthSeq");
		strcpy(Pack_dgt_table[33].name,"DelayFlag");
		strcpy(Pack_dgt_table[34].name,"TxnDate");
		strcpy(Pack_dgt_table[35].name,"TxnTime");
		strcpy(Pack_dgt_table[36].name,"Reserved");
		strcpy(Pack_dgt_table[37].name,"MONTHRESERVED");
		strcpy(Pack_dgt_table[38].name,"RSPCODE");
		
		break;
	case 3460: //�������
		max_count=39;
		
		char openCardDate[8+1];
		bbzero(openCardDate);

		initField();
		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->dgt_LocalSequence); //��ˮ��
		strcpy(dgtFIELD2,tranType);
		strcpy(dgtFIELD3,pgbl->Centerseqno);
		strcpy(dgtFIELD4,pgbl->posId);
		strcpy(dgtFIELD5,pgbl->dgt_posSerial);
		strcpy(dgtFIELD6,pgbl->busi_id);
		strcpy(dgtFIELD7,pgbl->busi_id);
		strcpy(dgtFIELD8,EDCARDID_field);
		strcpy(dgtFIELD10,pgbl->dgtCard.cardShowId);

		strcpy(dgtFIELD11,pgbl->dgtCard.cardShowId);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.CItyCode,(BYTE *)dgtFIELD12,2);
		CTools::byte_to_hex((BYTE *)pgbl->dgtCard.cardParent_type,(BYTE *) dgtFIELD13,1);
		CTools::byte_to_hex((BYTE  *) pgbl->dgtCard.cardChild_type,(BYTE *) dgtFIELD14,1);
		strcpy(dgtFIELD15,"1");

		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.openDate,(BYTE *) dgtFIELD16,4);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *) dgtFIELD18,4);

		strcpy(dgtFIELD19,pgbl->dgtCard.CARDSEQ);
		strcpy(dgtFIELD20,pgbl->dgtCard.tradeBeForeMny);
		strcpy(dgtFIELD21,deposit_field);
		strcpy(dgtFIELD22,"0"); //��0-�ɶ���1-���ɶ�
		strcpy(dgtFIELD23,"0"); //0-������ã�1-������
		strcpy(dgtFIELD24,pgbl->ReturnAmt); // 
		strcpy(dgtFIELD25,pgbl->ReturnDeposit);
		strcpy(dgtFIELD26,pgbl->MonthFeeFlag);
		strcpy(dgtFIELD27,pgbl->FeeAmtFlag);
		strcpy(dgtFIELD28,pgbl->MonthFeeAmt);
		strcpy(dgtFIELD29,pgbl->FeeAmt);
		strcpy(dgtFIELD31,pgbl->dgtSignInfo.BATCHNO); //�޸�Ϊǩ����ȡ�����κ�
		strcpy(dgtFIELD32,AuthSeq_field);
		strcpy(dgtFIELD33,"1"); //�ӳ�
		strcpy(dgtFIELD34,pgbl->dgtCard.tradeDate);
		strcpy(dgtFIELD35,pgbl->dgtCard.tradeTime);
	//	strcpy(dgtFIELD36,pgbl->dgtCard.CHARGE_DIVDATA);
		
		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;	    Pack_dgt_table[0].Max_Length = 8;	Pack_dgt_table[0].optionFlag='M';	//Unitid	
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;		Pack_dgt_table[1].Max_Length = 10; 	Pack_dgt_table[1].optionFlag='M'; //LocalSequence
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;		Pack_dgt_table[2].Max_Length = 2;	Pack_dgt_table[2].optionFlag='M'; //TransType
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;		Pack_dgt_table[3].Max_Length = 18;	Pack_dgt_table[3].optionFlag='C'; //���ĵǼ���ˮ��
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;		Pack_dgt_table[4].Max_Length = 12;	Pack_dgt_table[4].optionFlag='M'; //Posid
		Pack_dgt_table[5].Type = NUMERIC; 		Pack_dgt_table[5].Format = FIXED;	    Pack_dgt_table[5].Max_Length = 9;	Pack_dgt_table[5].optionFlag='M'; //PosSeq
		Pack_dgt_table[6].Type = NUMERIC;		Pack_dgt_table[6].Format = FIXED;	    Pack_dgt_table[6].Max_Length = 12;	Pack_dgt_table[6].optionFlag='M'; //��ҵ�豸����
		Pack_dgt_table[7].Type = NUMERIC;	    Pack_dgt_table[7].Format = FIXED;  		Pack_dgt_table[7].Max_Length = 16;	Pack_dgt_table[7].optionFlag='M'; //Operid
		Pack_dgt_table[8].Type = NUMERIC;	    Pack_dgt_table[8].Format = FIXED;	    Pack_dgt_table[8].Max_Length = 16;	Pack_dgt_table[8].optionFlag='C'; //EdCardId
		Pack_dgt_table[9].Type = NUMERIC;		Pack_dgt_table[9].Format = FIXED;		Pack_dgt_table[9].Max_Length = 16;	Pack_dgt_table[9].optionFlag='C'; //SamId
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;		Pack_dgt_table[10].Max_Length = 16;	Pack_dgt_table[10].optionFlag='M'; //CardFaceId
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;		Pack_dgt_table[11].Max_Length = 16;	Pack_dgt_table[11].optionFlag='M'; //CardId
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;		Pack_dgt_table[12].Max_Length = 4;	Pack_dgt_table[12].optionFlag='M'; //CityCode
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;		Pack_dgt_table[13].Max_Length = 2;	Pack_dgt_table[13].optionFlag='M';//CardMKnd
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;		Pack_dgt_table[14].Max_Length = 2;	Pack_dgt_table[14].optionFlag='M';//CardSKind
		
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;		Pack_dgt_table[15].Max_Length = 1;	Pack_dgt_table[15].optionFlag='M'; //CardModel
		Pack_dgt_table[16].Type = NUMERIC;		Pack_dgt_table[16].Format = FIXED;		Pack_dgt_table[16].Max_Length = 8;	Pack_dgt_table[16].optionFlag='M'; //CardSellDate
		
		Pack_dgt_table[17].Type = ALPHANUM;	Pack_dgt_table[17].Format = FIXED;		Pack_dgt_table[17].Max_Length = 8;		Pack_dgt_table[17].optionFlag='C'; //Ǯ����ͨҵ��λͼ
		Pack_dgt_table[18].Type = NUMERIC;	Pack_dgt_table[18].Format = FIXED;	Pack_dgt_table[18].Max_Length = 8;		Pack_dgt_table[18].optionFlag='M'; //����Ч��
		
		Pack_dgt_table[19].Type = NUMERIC;	Pack_dgt_table[19].Format = FIXED;		Pack_dgt_table[19].Max_Length = 6;		Pack_dgt_table[19].optionFlag='M'; //��������
		Pack_dgt_table[20].Type = NUMERIC;	Pack_dgt_table[20].Format = FIXED;		Pack_dgt_table[20].Max_Length = 10;		Pack_dgt_table[20].optionFlag='M'; //�����
		Pack_dgt_table[21].Type = NUMERIC;	Pack_dgt_table[21].Format = FIXED ;		Pack_dgt_table[21].Max_Length = 8;		Pack_dgt_table[21].optionFlag='M'; //Ѻ��
		Pack_dgt_table[22].Type = NUMERIC;	Pack_dgt_table[22].Format = FIXED;		Pack_dgt_table[22].Max_Length = 1;		Pack_dgt_table[22].optionFlag='C'; //оƬ�����
		Pack_dgt_table[23].Type = NUMERIC;	Pack_dgt_table[23].Format = FIXED;		Pack_dgt_table[23].Max_Length = 1;		Pack_dgt_table[23].optionFlag='M'; //����������
		Pack_dgt_table[24].Type = NUMERIC;			Pack_dgt_table[24].Format = FIXED;		Pack_dgt_table[24].Max_Length = 10;		Pack_dgt_table[24].optionFlag='M';//д���¿����
		Pack_dgt_table[25].Type = NUMERIC;		Pack_dgt_table[25].Format = FIXED;		Pack_dgt_table[25].Max_Length = 8;		Pack_dgt_table[25].optionFlag='M'; //���û�Ѻ��
		Pack_dgt_table[26].Type = NUMERIC;	Pack_dgt_table[26].Format = FIXED;		Pack_dgt_table[26].Max_Length = 1;		Pack_dgt_table[26].optionFlag='M'; //�·���ȡ��־
		Pack_dgt_table[27].Type = NUMERIC;	Pack_dgt_table[27].Format = FIXED;		Pack_dgt_table[27].Max_Length = 1;		Pack_dgt_table[27].optionFlag='M'; //��������ȡ��־
		Pack_dgt_table[28].Type = NUMERIC;	Pack_dgt_table[28].Format = FIXED;		Pack_dgt_table[28].Max_Length = 8;		Pack_dgt_table[28].optionFlag='M'; //���ɱ����·�
		
		Pack_dgt_table[29].Type = NUMERIC;	Pack_dgt_table[29].Format = FIXED;	Pack_dgt_table[29].Max_Length = 8;	Pack_dgt_table[29].optionFlag='M';  //������
		Pack_dgt_table[30].Type = NUMERIC;	Pack_dgt_table[30].Format = FIXED;	Pack_dgt_table[30].Max_Length = 8;	Pack_dgt_table[30].optionFlag='M'; //�������� ȫ0
		Pack_dgt_table[31].Type = NUMERIC;	Pack_dgt_table[31].Format = FIXED;	Pack_dgt_table[31].Max_Length = 6;	Pack_dgt_table[31].optionFlag='M'; //���κ�
		Pack_dgt_table[32].Type = NUMERIC;	Pack_dgt_table[32].Format = FIXED;	Pack_dgt_table[32].Max_Length = 18;	Pack_dgt_table[32].optionFlag='M'; //��Ȩ��ˮ��
		Pack_dgt_table[33].Type = NUMERIC;	Pack_dgt_table[33].Format = FIXED;	Pack_dgt_table[33].Max_Length = 1;	Pack_dgt_table[33].optionFlag='M'; //��ʱ�����־λ
		Pack_dgt_table[34].Type = NUMERIC;	Pack_dgt_table[34].Format = FIXED;	Pack_dgt_table[34].Max_Length = 8;	Pack_dgt_table[34].optionFlag='M'; //��������
		Pack_dgt_table[35].Type = NUMERIC;	Pack_dgt_table[35].Format = FIXED;	Pack_dgt_table[35].Max_Length = 6;	Pack_dgt_table[35].optionFlag='M'; //����ʱ��
		Pack_dgt_table[36].Type = ALPHANUMS;	Pack_dgt_table[36].Format = FIXED;	Pack_dgt_table[36].Max_Length = 20;	Pack_dgt_table[36].optionFlag='M'; //������ ȫ0
		Pack_dgt_table[37].Type = ALPHANUMS;	Pack_dgt_table[37].Format = FIXED;	Pack_dgt_table[37].Max_Length = 50;	Pack_dgt_table[37].optionFlag='M'; //����ʹ��
		Pack_dgt_table[38].Type = NUMERIC;	Pack_dgt_table[38].Format = FIXED;	Pack_dgt_table[38].Max_Length = 5;	Pack_dgt_table[38].optionFlag='M'; //����Ӧ����
		
		Pack_dgt_table[0].Rev_Data = dgtFIELD0;	 		Pack_dgt_table[0].Send_Data = dgtFIELD0; 	
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;			Pack_dgt_table[1].Send_Data = dgtFIELD1; 	
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;			Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;			Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;			Pack_dgt_table[4].Send_Data = dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;			Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;			Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;			Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;			Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;			Pack_dgt_table[9].Send_Data = dgtFIELD9;
		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;		Pack_dgt_table[19].Send_Data = dgtFIELD19;
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;		Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;		Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;		Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;		Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;		Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;		Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;		Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;		Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;		Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;		Pack_dgt_table[29].Send_Data = dgtFIELD29;
		Pack_dgt_table[30].Rev_Data = dgtFIELD30;		Pack_dgt_table[30].Send_Data = dgtFIELD30;
		Pack_dgt_table[31].Rev_Data = dgtFIELD31;		Pack_dgt_table[31].Send_Data = dgtFIELD31;
		Pack_dgt_table[32].Rev_Data = dgtFIELD32;		Pack_dgt_table[32].Send_Data = dgtFIELD32;
		Pack_dgt_table[33].Rev_Data = dgtFIELD33;		Pack_dgt_table[33].Send_Data = dgtFIELD33;
		Pack_dgt_table[34].Rev_Data = dgtFIELD34;		Pack_dgt_table[34].Send_Data = dgtFIELD34;
		Pack_dgt_table[35].Rev_Data = dgtFIELD35;		Pack_dgt_table[35].Send_Data = dgtFIELD35;
		Pack_dgt_table[36].Rev_Data = dgtFIELD36;		Pack_dgt_table[36].Send_Data = dgtFIELD36;
		Pack_dgt_table[37].Rev_Data = dgtFIELD37;		Pack_dgt_table[37].Send_Data = dgtFIELD37;
		Pack_dgt_table[38].Rev_Data = dgtFIELD38;		Pack_dgt_table[38].Send_Data = dgtFIELD38;

		//������� ����

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"LocalSequence");
		strcpy(Pack_dgt_table[2].name,"TransType");
		strcpy(Pack_dgt_table[3].name,"CenterSeqno");
		strcpy(Pack_dgt_table[4].name,"Posid");
		strcpy(Pack_dgt_table[5].name,"PosSeq");
		strcpy(Pack_dgt_table[6].name,"TermId");
		strcpy(Pack_dgt_table[7].name,"Operid");
		strcpy(Pack_dgt_table[8].name,"EdCardId");
		strcpy(Pack_dgt_table[9].name,"SamId");
		strcpy(Pack_dgt_table[10].name,"CardFaceId");
		strcpy(Pack_dgt_table[11].name,"CardId");
		strcpy(Pack_dgt_table[12].name,"CityCode");
		strcpy(Pack_dgt_table[13].name,"CardMKnd");
		strcpy(Pack_dgt_table[14].name,"CardSKind");
		strcpy(Pack_dgt_table[15].name,"CardModel");
		strcpy(Pack_dgt_table[16].name,"CardSellDate");
		strcpy(Pack_dgt_table[17].name,"CardBusInessBit");
		strcpy(Pack_dgt_table[18].name,"CardVal");
		strcpy(Pack_dgt_table[19].name,"CardCnt");
		strcpy(Pack_dgt_table[20].name,"BalanceBef");
		strcpy(Pack_dgt_table[21].name,"Deposit");
		strcpy(Pack_dgt_table[22].name,"ChipBadType");
		strcpy(Pack_dgt_table[23].name,"CardFaceBadType");
		strcpy(Pack_dgt_table[24].name,"ReturnAmt");
		strcpy(Pack_dgt_table[25].name,"ReturnDeposit");
		strcpy(Pack_dgt_table[26].name,"MonthFeeflag");
		strcpy(Pack_dgt_table[27].name,"FeeAmtFlag");
		strcpy(Pack_dgt_table[28].name,"MonthFeeAmt");
		strcpy(Pack_dgt_table[29].name,"FeeAmt");
		strcpy(Pack_dgt_table[30].name,"setTime");
		strcpy(Pack_dgt_table[31].name,"BatchNo");
		strcpy(Pack_dgt_table[32].name,"AuthSeq");
		strcpy(Pack_dgt_table[33].name,"LagFlag");
		strcpy(Pack_dgt_table[34].name,"TxnDate");
		strcpy(Pack_dgt_table[35].name,"TxnTime");
		strcpy(Pack_dgt_table[36].name,"Reserved");
		strcpy(Pack_dgt_table[37].name,"MONTHRESERVED");
		strcpy(Pack_dgt_table[38].name,"RSPCODE");

		break;

	default:
		return -1;
	}

	//���г�ʼ��

	for ( j =0;j<max_count;j++)
	{
		Pack_dgt_table[j].Real_Length = 0; 
		char sendDataField[100];
		bbzero(sendDataField);
		strcpy(sendDataField,Pack_dgt_table[j].Send_Data);
		wrLog("%s:%s",Pack_dgt_table[j].name,Pack_dgt_table[j].Send_Data);

		switch (Pack_dgt_table[j].optionFlag) 
		{
		case 'M': //����Ǳ���
			
			if (Pack_dgt_table[j].Type==NUMERIC) //��������֡�
			{
				int fix_length=Pack_dgt_table[j].Max_Length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Max_Length,1,Pack_dgt_table[j].Send_Data);
			
				
			}
			else //�����ansc
			{
				
				int fix_length=Pack_dgt_table[j].Max_Length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Max_Length,0,Pack_dgt_table[j].Send_Data);
				
			}
			break;
		case 'C': //�����ѡ��ѡ�� 

			if (Pack_dgt_table[j].Format==FIXED) //����Ƕ�����
			{
				int fix_length=Pack_dgt_table[j].Max_Length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Max_Length,0,Pack_dgt_table[j].Send_Data);
				
			}
			else //����ǿɱ䳤�ȣ���ȡ��һ���ֶε�ֵ��Ϊ����
			{
				int real_length=atoi(Pack_dgt_table[j-1].Send_Data);
				Pack_dgt_table[j].Real_Length=real_length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Real_Length,0,Pack_dgt_table[j].Send_Data);
			}
			
			break;
			
		}
	}





	return 0;
}



//����ͨѶmac 3des����

int CDGTBusTrade::getCommuMac(char *poskey,char *keyset,char *keyField,char *commuMac)
{
	char uncipherkeySet[32+1];
	unsigned char key_field_byte[1024];
	unsigned char  uncipherkeySet_byte[16+1];
	unsigned char iv[8]={0};
	int i=0;
	int totalLen=0;
	int iSrcLen=0;
	unsigned char commuMac_temp[4+1];

	bbzero(commuMac_temp);
	bbzero(commuMac);
	bbzero(uncipherkeySet);
	bbzero(uncipherkeySet_byte);
	bbzero(key_field_byte);
	
	wrLog("poskey:%s",poskey);
	wrLog("keyset:%s",keyset);
	wrLog("���ܹؼ���:%s",keyField);
	
	bbzero(uncipherkeySet);
	//���ն���Կ����keyset
	if (strlen(keyField) %2 !=0) //�������2�ı���
	{
		wrLog("�ؼ�����2�ı���");
		return -1; //
	}
	
	CDes::TripleDesDec((char*)poskey , keyset , uncipherkeySet);
	wrLog("uncipherkeySet:%s",uncipherkeySet);
	wrLog("keyFiled:%s",keyField);
	
	CTools::hex_to_byte((BYTE *) uncipherkeySet,(BYTE *)uncipherkeySet_byte,strlen(uncipherkeySet));
	CTools::hex_to_byte((BYTE *) keyField,(BYTE *) key_field_byte,strlen(keyField));
	//�Թؼ�����м���
	
	iSrcLen=strlen(keyField)/2;
	MAC16(uncipherkeySet_byte, key_field_byte,  iSrcLen, iv, commuMac_temp);
	CTools::byte_to_hex( commuMac_temp,(BYTE *)commuMac,4);
	wrLog("comMac:%s",commuMac);
	if (strlen(commuMac)!=8)
	{
		return -1;//����ʧ��
	}
	
	return 0;
}


//��ȡͬ����ϢANS12 ʹ�������114523
int CDGTBusTrade::getSyncinfo(char *syncInfo)
{
	char currentTime[9+1];
	char sub_currentTime[6+1];
	char randomNum[6+1];

	bbzero(currentTime);
	bbzero(randomNum);

	bbzero(sub_currentTime);
	CTools::getTime(currentTime,1); //��ȡ������
	memcpy(sub_currentTime,currentTime+3,6);
	CTools::generate_random_num(randomNum,6);
	bbzero(syncInfo);
	sprintf(syncInfo,"%s%s",sub_currentTime,randomNum);
	return 0;
}




//�ֻ����� by liuxiao 20130401 ��ݸHID�豸

UCHAR CDGTBusTrade::getXor(UCHAR *Data, int DataLen)
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

int CDGTBusTrade::Package0203(UCHAR *sData,UCHAR *dData,int DataLen)
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
int CDGTBusTrade::unPackage0203(UCHAR *sData,UCHAR *dData,int *DataLen) 
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


//��ݸ�������Ľ�����Ϣ
int CDGTBusTrade::dgtSndRcvHID_IcData(const TSDLLPosInfo dPosInfo,const BYTE *downData, const int downLen, BYTE *upData, int &upLen,char *errMsg)
{
	int ret;
	TSHID_IcReaderCommand icCmd;
	memset(&icCmd, 0, sizeof(icCmd));
	//strcpy(icCmd.acTitle, title);
	icCmd.uDownLen=downLen+5;
	//�������
	ret=Package0203((BYTE *)icCmd.uDownData,(BYTE *)downData,downLen);
	if (ret!=0)
	{
		return 1;
	}

	wrHex("�·�����������", icCmd.uDownData, icCmd.uDownLen); //�´��������ı��ļ����ĳ���
	ret = TS_HID_IcReaderCommand(dPosInfo, &icCmd);  
	if( ret ) //���󣬷��س�����Ϣ
	{
		upLen = 0;
		wrLog("���󷵻�ֵ: [%d]", ret);
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽������������Ӻö�����");
		//TS_OutPutMsg(gbl.dPosInfo, " \n ���ն˽�������!", 1, iSelect);
		return ret;
	}
	
	wrHex("��������������", icCmd.uUpData, icCmd.uUpLen);
	if( icCmd.uUpLen < 6 )
	{
		wrLog("��������������С��6");
		return 7;
	}
	//�������
	ret=unPackage0203(icCmd.uUpData,upData,&upLen);
	if(ret==-1)
	{
		bbzero(errMsg);
		strcmp(errMsg,"���ĸ�ʽ����");
		wrLog("����������");
		return 9;
		
	}
	if(ret==-2)
	{
		wrLog("У�������");
		bbzero(errMsg);
		strcpy(errMsg,"�������У�����");
		//TS_OutPutMsg(gbl.dPosInfo, " \n �������У�����!", 1, iSelect);
		return 8;
	}
	
	return 0;
}



//��Ƭ�ϵ�
int CDGTBusTrade::getCardType(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg)
{
	char rcvData[256];
	int rcvLen;
	char cardType[2+1];

	bbzero(rsp_code);
	bbzero(rcvData);
	bbzero(cardType);
	//cpu����ֵ֮ǰ��Ҫ����Ѱ������
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��Ƭ����ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)cardType,1);

	//0A //cpu �� 1A //M1��
	if (strcmp(cardType,"1A")==0) //�����M1��
	{
		return 1;
	}
	else if (strcmp(cardType,"0A")==0) //�����cpu��
	{
		return 0;
	}
	else
	{
	
		return -1; //��ѯ����
	}

}

//�ǽӴ����ϵ�Ѱ�� Ѱ��ʧ���������ز���ʧ��
//����ֵ 0����ִ��ָ��ɹ������ؿ���Ϣ ��Ƭִ��ָ��ɹ������ش�����Ϣ��
/**********************************************************************
���Σ�cardType:������
cardInfo:��Ƭ��Ϣ
otherData:������Ƭ��Ϣ�������A����
//flag=3 ��������Ϣ

**********************************************************************/
int CDGTBusTrade::cardSearch(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg,PDGTCard pDgtCard ,int flag)
{
	char rcvData[256];
	int rcvLen;
	char cardType[2+1];
	//��ȡ������Ϣ
	int infoLen=0; //��Ƭ��Ϣ����
	char cardInfo[255]; //��Ƭ��Ϣ
	int otherLen=0; //������Ϣ����
	char otherData[255]; //������Ϣ(A�����ص�)
	int count=0;
	char len_temp_10[2];

	bbzero(rsp_code);
	bbzero(rcvData);
	bbzero(cardInfo);
	bbzero(otherData);
	bbzero(cardType);
	
	//cpu����ֵ֮ǰ��Ҫ����Ѱ������
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��Ƭ����ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	bbzero(len_temp_10);
	bbzero(cardInfo);
	bbzero(otherData);
	
	char len_temp[2];
	bbzero(len_temp);
	CTools::byte_to_hex((BYTE *)rcvData+2,(BYTE *)cardType,1);
	//�����m1������ʾ�û���Ƭ���ʹ��󡣽�֧�ֶ�ݸͨ��Ƭ��ֵ
	if (strcmp(cardType,"1A") ==0)
	{
		wrLog("��Ƭ���ʹ���,��CPU��");
		bbzero(errMsg);
		strcpy(errMsg,"\n��ҵ���֧�ֶ�ݸͨ������ͨ������붫ݸ����ҵ��\n\n�밴��Enter������");
		return -2;
	}

	CTools::byte_to_hex((BYTE *) rcvData+3,(BYTE *)len_temp,1);
	CTools::sixten_toten(len_temp,len_temp_10);
	infoLen=atoi(len_temp_10);
	memcpy(cardInfo,rcvData+4,infoLen);
	count+=infoLen+4;

	bbzero(len_temp);
	bbzero(len_temp_10);
	CTools::byte_to_hex((BYTE *)rcvData+count,(BYTE *)len_temp,1);
	CTools::sixten_toten(len_temp,len_temp_10);
	otherLen=atoi(len_temp_10);
	count+=1;

	if (otherLen!=0) //�������������Ϣ��Ϊ0
	{
		bbzero(otherData);
		memcpy(otherData,rcvData+count,otherLen);
		count+=otherLen;
	}


	if (flag!=3) //��������ڽ��׹����в�ѯ���
	{

		bbzero(pDgtCard->cardInfo);
		bbzero(pDgtCard->otherData);
		bbzero(pDgtCard->cardType);
		memcpy(pDgtCard->cardInfo,cardInfo,infoLen);
		memcpy(pDgtCard->otherData,otherData,otherLen);
		memcpy(pDgtCard->cardType,cardType,2);
		
		bbzero(pDgtCard->CARDSALE_DIVDATA); //�ۿ���ɢ����
		CTools::byte_to_hex((BYTE *) otherData+8,(BYTE *)pDgtCard->CARDSALE_DIVDATA, 8);

	}

	//�����Ϣ
	wrLog("cpu������ɹ�!");
	wrLog("������:%s",cardType);
	wrHex("����Ϣ:",(BYTE *)cardInfo,infoLen);
	wrHex("������Ϣ(A����Ϣ����):",(BYTE *)otherData,otherLen);
	//wrLog("�ۿ���ɢ����:%s",pDgtCard->CARDSALE_DIVDATA);
	
	return 0;
}


//��ȡM1����Ϣ (M1�ϵ���Ϣ

int CDGTBusTrade::cardM1Search(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg,PDGTM1Card pDgtM1Card)
{
	char rcvData[256];
	int rcvLen;
	char cardType[2+1];
	//��ȡ������Ϣ
	int infoLen=0; //��Ƭ��Ϣ����
	char cardInfo[255]; //��Ƭ��Ϣ
	int otherLen=0; //������Ϣ����
	char otherData[255]; //������Ϣ(A�����ص�)
	int count=0;
	char M1CardCSN[8+1];//	M1��ƬcardCSN
	int iRet;
	char cardParentType[2+1];
	char cardChildType[2+1];

	bbzero(cardParentType);
	bbzero(cardChildType);
	bbzero(rsp_code);
	bbzero(rcvData);
	bbzero(cardInfo);
	bbzero(otherData);
	bbzero(cardType);
	bbzero(M1CardCSN);
	bbzero(cardType);
	
	//cpu����ֵ֮ǰ��Ҫ����Ѱ������
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��Ƭ����ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //��Ƭָ��ִ�в��ɹ������ش���
	}

	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)cardType,1);
	//��������ʹ��󣬲���Ҫ����
	if (strcmp(cardType,"1A") !=0 ) //��� ����M1��ֱ�ӷ���
	{
		bbzero(errMsg);
		strcpy(errMsg,"�뻻��ͨ���ܿ�����");
		return -3;
	}
	CTools::byte_to_hex((BYTE *) rcvData+4,(BYTE *)M1CardCSN,4);
	wrLog("��ȡM����Ϣ����---------------");
	wrLog("cardSCN:%s",M1CardCSN);
	wrLog("cardType:%s",cardType);

	//��ȡ�����Ϳ�������
	iRet=getCardParentType(dPosInfo,M1CardCSN,cardParentType,cardChildType,errMsg);
	if (iRet!=0)
	{
		return -4; //��ѯ��������Ϣ����
	}

	strcpy(pDgtM1Card->bus_card_type,cardType);
	strcpy(pDgtM1Card->M1CardCSN,M1CardCSN);
	strcpy(pDgtM1Card->card_parent_type,cardParentType);
	strcpy(pDgtM1Card->card_child_type,cardChildType);

	return 0;
}



//ֻ������ȡM1��Ƭ��ȡ���ӺͿ�����Ϣ 

int CDGTBusTrade::getCardParentType(const TSDLLPosInfo dPosInfo,char *cardCSN,char *cardParentType,char *cardChildType,char *errMsg)
{

	char sendData[255];
	char rcvData[255];
	int sendLen=0;
	int rcvLen=0;
	char rsp_code[4+1];
	char parent_type_buff[2+1];
	char child_type_buff[2+1];

	bbzero(sendData);
	bbzero(rcvData);
	bbzero(rsp_code);
	bbzero(parent_type_buff);
	bbzero(child_type_buff);

	memcpy(sendData,"\xC2\x02\x60\x03\xA0\xA1\xA2\xA3\xA4\xA5",10);
	sendLen=10;
	CTools::hex_to_byte((BYTE *) cardCSN,(BYTE *) sendData+sendLen,8);
	sendLen+=4;

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��ȡ��������ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	//�ٷ���ָ��
	bbzero(rcvData);
	rcvLen=0;

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x03\x02",3, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��ȡ������ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	//��ȡ������Ϣ��������Ϣ
	bbzero(cardChildType);
	bbzero(cardParentType);
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)parent_type_buff,1);
	CTools::byte_to_hex((BYTE *) rcvData+3,(BYTE *)child_type_buff,1);
	//ת����Ƭ����
	if (strcmp(parent_type_buff,"50") ==0 && ( strcmp(child_type_buff,"01")==0 ))
	{

		bbzero(cardParentType);
		bbzero(cardChildType);
		strcpy(cardParentType,"00");
		strcpy(cardChildType,"01");

	}
	
	if (strcmp(parent_type_buff,"50")==0 && (strcmp(child_type_buff,"11")==0) )
	{
		
		bbzero(cardParentType);
		bbzero(cardChildType);
		strcpy(cardParentType,"00");
		strcpy(cardChildType,"02");
	}

	return 0;


}



//��ѯM1��Ϣ
int CDGTBusTrade::getM1SBInfo(const TSDLLPosInfo dPosInfo,PDGTM1Card pdgtM1Card,char * errMsg)

{
	char sendData[255];
	char rcvData[255];
	int sendLen=0;
	int rcvLen=0;
	char rsp_code[4+1];
	char parent_type_buff[2+1];
	char child_type_buff[2+1];
	char cardCSN[8+1];
	char cardType[2+1];
	char city_code[2+1];
	char rec_code[4+1];
	char SW1SW2[4+1];
	char keyA[6+1];
	char deposit[2+1];
	char saleDate[6+1];
	
	bbzero(saleDate);
	bbzero(parent_type_buff);
	bbzero(child_type_buff);
	bbzero(deposit);
	bbzero(SW1SW2);
	bbzero(keyA);
	bbzero(rsp_code);
	bbzero(city_code);
	bbzero(sendData);
	bbzero(rcvData);
	bbzero(rsp_code);
	bbzero(parent_type_buff);
	bbzero(child_type_buff);
	bbzero(cardType);
	int offset=0;
	
	//step 1

	wrLog("���зǽӴ����ϵ�---------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��Ƭ����ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //��Ƭָ��ִ�в��ɹ������ش���
	}
	
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)cardType,1);
	//��������ʹ��󣬲���Ҫ����
	if (strcmp(cardType,"1A") !=0 ) //��� ����M1��ֱ�ӷ���
	{
		bbzero(errMsg);
		strcpy(errMsg,"�뻻��ͨ���ܿ�����");
		return -3;
	}

	CTools::byte_to_hex((BYTE *) rcvData+4,(BYTE *)cardCSN,4);
	wrLog("��ȡM����Ϣ����---------------");
	wrLog("cardSCN:%s",cardCSN);
	wrLog("cardType:%s",cardType);
	strcpy(pdgtM1Card->M1CardCSN,cardCSN);

	//step 2

	bbzero(sendData);
	sendLen=0;
	memcpy(sendData,"\xC2\x02\x60\x03\xA0\xA1\xA2\xA3\xA4\xA5",10);
	sendLen=10;

	CTools::hex_to_byte((BYTE *) cardCSN,(BYTE *) sendData+sendLen,8);
	sendLen+=4;
	
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) )
	{
		return -1;
	}

	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��ȡ��������ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}


	//��ȡ��Ч�ڵ���Ϣ
	bbzero(rcvData);
	rcvLen=0;
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x03\x01",3, (BYTE *)rcvData, rcvLen,errMsg) )  //��ȡ���б������֤��
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��ȡ��Ч��ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	//��ȡ��Ϣ��Ч�ڵ���Ϣ
	CTools::byte_to_hex((BYTE *) rcvData+6,(BYTE *)pdgtM1Card->cardVadate,4);
	CTools::byte_to_hex((BYTE *) rcvData+14,(BYTE *) pdgtM1Card->cardStatus,1);
	
	//step 3 ����ָ�� //c20302 ����������ȡ���д������֤��
	bbzero(rcvData);
	rcvLen=0;
	wrLog("��ȡ���б���-----------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x03\x02",3, (BYTE *)rcvData, rcvLen,errMsg) )  //��ȡ���б������֤��
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��ȡ��֤��ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	//ȡ���б������֤��
	memcpy(city_code,rcvData+2+6,2);
	memcpy(rec_code,rcvData+2+10,4);
	wrHex("city_code:",(BYTE *)city_code,2);
	wrHex("rec_code:",(BYTE  *) rec_code,4);

	//step 4 sam�ϵ�  32 22 00 00 10
	bbzero(rcvData);
	rcvLen=0;
	wrLog("SAM���ϵ�-------------------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x22\x00\x00\x10",5, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("SAM���ϵ�ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	bbzero(rcvData);
	rcvLen=0;

	//5.����32 26 10 00 a4 00.... ��������ֽ���9000����
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x26\x10\x00\xa4\x00\x00\x02\x10\x03",10, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("sam����Ӧ����");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	//6 ���������SAM�����Уģ����80FC01 01 10 [���д���][��Ψһ��] 0001[��֤��]0a0b0c0d ����������Կ���ֱ�������1��2��3,10 KeyA��Կ��ȡ����ǰ���ֽ�Ϊ��Կ[RESP(0,6)]>10
	bbzero(sendData);
	sendLen=0;
	bbzero(rcvData);
	rcvLen=0;
	wrLog("��ȡ��ԿkeyA------------------------");
	CTools::hex_to_byte((BYTE *) "32261080FC010110",(BYTE * )sendData,16);
	sendLen+=8;
	memcpy(sendData+sendLen,city_code,2);
	sendLen+=2;

	CTools::hex_to_byte((BYTE  *) cardCSN,(BYTE *) sendData+sendLen,8);
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) "0001",(BYTE *)sendData+sendLen,4);
	sendLen+=2;
	memcpy(sendData+sendLen,rec_code,4);
	sendLen+=4;
	CTools::hex_to_byte((BYTE * )"0a0b0c0d",(BYTE *)sendData+sendLen,8);
	sendLen+=4;
	
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��ȡSAM����Կʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //��Ƭָ��ִ�в��ɹ������ش���
	}

	memcpy(keyA,rcvData+2,6);
	//7.�ٴ�ִ��Ѱ��
	bbzero(rcvData);
	rcvLen=0;
	wrLog("�ٴ�Ѱ��-------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("��Ƭ����ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //��Ƭָ��ִ�в��ɹ������ش���
	}

	//C202 6007[VAR10���ڣ���ȡ�õ���Կ] [��Ψһ��]��

	bbzero(sendData);
	sendLen=0;
	bbzero(rcvData);
	rcvLen=0;
	
	CTools::hex_to_byte((BYTE *) "C2026007",(BYTE * )sendData,8);
	sendLen+=4;
	memcpy(sendData+sendLen,keyA,6);
	sendLen+=6;
	CTools::hex_to_byte((BYTE *) cardCSN,(BYTE *)sendData+sendLen,8);
	sendLen+=4;
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("ִ��ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //��Ƭָ��ִ�в��ɹ������ش���
	}


	wrLog("��ȡѺ�����Ϣ------------------");
	bbzero(rcvData);
	rcvLen=0;
	char deposit_temp[6+1];
	bbzero(deposit_temp);

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xc2\x03\x04",3, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("ִ�з���:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("ִ��ʧ��");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //��Ƭָ��ִ�в��ɹ������ش���
	}


	//��ȡ��Ϣ
	 offset=2;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *)parent_type_buff,1);
	offset+=1;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *)child_type_buff,1);
	offset+=1;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *) pdgtM1Card->appSerial,4);
	offset+=4;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *) saleDate,3);
	offset+=3;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *) pdgtM1Card->openTime,2);
	offset+=2;
	//�ۿ��ص� ��ȡ
	offset+=2;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *)deposit,1);
	CTools::sixten_toten(deposit,deposit_temp); //Ѻ��Ӧ��ȡ�ѷ�Ϊ��λ
	sprintf(pdgtM1Card->deposit,"%06d",atoi(deposit_temp)*100);
	if (strcmp(saleDate,"000000")==0) //�����ȡ������Ϊ��
	{
		bbzero(pdgtM1Card->openDate);
		sprintf(pdgtM1Card->openDate,"00%s",saleDate);
	}
	else
	{
	
		bbzero(pdgtM1Card->openDate);
		CTools::transYYYMMDD(saleDate,pdgtM1Card->openDate);
	}
	
	//ת����Ƭ����
	if (strcmp(parent_type_buff,"50") ==0 && ( strcmp(child_type_buff,"01")==0 ))
	{
		
		bbzero(pdgtM1Card->card_parent_type);
		bbzero(pdgtM1Card->card_child_type);
		strcpy(pdgtM1Card->card_parent_type,"00");
		strcpy(pdgtM1Card->card_child_type,"01");
		
	}
	
	if (strcmp(parent_type_buff,"50")==0 && (strcmp(child_type_buff,"11")==0) )
	{
		
		bbzero(pdgtM1Card->card_parent_type);
		bbzero(pdgtM1Card->card_child_type);
		strcpy(pdgtM1Card->card_parent_type,"00");
		strcpy(pdgtM1Card->card_child_type,"02");
	}

	return 0;

}


//û�н���Ǯ�������صĿ�Ƭ��Ϣ ע������0005�ļ��µ���Ϣ ��ȡѺ����Ϣ

int CDGTBusTrade::readCardInfo(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard, char *errMsg)
{


	char rcvData[256];
	int rcvLen;
	char rsp_code[4+1]; //������

	bbzero(rcvData);
	bbzero(rsp_code);

	wrLog("��ȡ��������Ϣδ����Ǯ��Ŀ¼-----------------------------------");
	char CItyCode[2+1]; //���б���
	char compay_code[2+1]; //��������λ
	char busi_code[2+1]; //��ҵ����
	char RFU[2+1]; //����
	char appSerial[8+1];//Ӧ�����
	char cardParent_type[1+1]; //��������
	char cardChild_type[1+1]; //��������
	char openDate[4+1]; //��������
	char deviceInfo[6+1]; //�����豸��Ϣ
	char appVer[2+1];//Ӧ�ð汾��Ϣ

	int dataLen; //������������

	//��ʼ������
	bbzero(CItyCode);
	bbzero(compay_code);
	bbzero(busi_code);
	bbzero(RFU);
	bbzero(appSerial);
	bbzero(cardChild_type);
	bbzero(cardParent_type);
	bbzero(openDate);
	bbzero(deviceInfo);
	bbzero(appVer);

	
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x00\xB0\x85\x00\x1E",8, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
		
	{
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽���ʧ��,	�����Ӻö�����");
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
		
	}
	
	//������ȡ����Ϣ
	dataLen=2;
	memcpy(compay_code,rcvData+dataLen,2); //����������
	dataLen+=2;
	memcpy(CItyCode,rcvData+dataLen,2); //���б���
	dataLen+=2;
	memcpy(busi_code,rcvData+dataLen,2); //��ҵ����
	dataLen+=2;
	memcpy(RFU,rcvData+dataLen,2);  //RFU
	dataLen+=2;
	memcpy(appSerial,rcvData+dataLen,8); //Ӧ�����к�
	dataLen+=8;
	memcpy(cardParent_type,rcvData+dataLen,1);//��������
	dataLen+=1;
	memcpy(cardChild_type,rcvData+dataLen,1); //��������
	dataLen+=1;
	memcpy(openDate,rcvData+dataLen,4); //��������
	dataLen+=4;
	memcpy(deviceInfo,rcvData+dataLen,6); //�����豸��Ϣ
	memcpy(appVer,rcvData+dataLen,1); //Ӧ�ð汾
	dataLen+=1;

	
	wrLog("�����Ƭ������Ϣ(0005)-------------------------");
	wrHex("������:",(BYTE *)compay_code,2);
	wrHex("���д���:",(BYTE *)CItyCode,2);
	wrHex("��ҵ����:",(BYTE *)busi_code,2);
	wrHex("Ѻ��:",(BYTE *)RFU,2);
	wrHex("Ӧ�ð汾:",(BYTE *)appVer,1);
	wrHex("Ӧ�����к�:",(BYTE *)appSerial,8);
	wrHex("��������:",(BYTE *)openDate,4);
	wrHex("��������:",(BYTE *)cardParent_type,1);
	wrHex("��������:",(BYTE *)cardChild_type,1);
	wrHex("�����豸��Ϣ:",(BYTE *) deviceInfo,6);

	bbzero(pdgtCard->deposit);
	memcpy(pdgtCard->deposit,RFU,2); //��ƬѺ��

	return 0;

}



//��ȡ��Ƭ������Ϣ ��ȡ��Ƭ������Ϣ��������Ǯ��֮�󷵻صĿ�Ƭ������Ϣ��
//ע������0015���ļ��е���Ϣ

int CDGTBusTrade::readCardBasicInfo(const TSDLLPosInfo dPosInfo,char *errMsg,PDGTCard pdgtCard,int flag)
{
	char rcvData[256];
	int rcvLen;
	char rsp_code[4+1]; //������

	bbzero(rcvData);
	bbzero(rsp_code);

	wrLog("��ȡ��������Ϣ-----------------------------------");
	char CItyCode[2+1]; //���б���
	char compay_code[2+1]; //��������λ
	char busi_code[2+1]; //��ҵ����
	char RFU[2+1]; //����
	char appSerial[8+1];//Ӧ�����
	char cardParent_type[2+1]; //��������
	char cardChild_type[2+1]; //��������
	char openDate[4+1]; //��������
	char effectiveDate[4+1]; //��Ч����
	char deviceInfo[6+1]; //�����豸��Ϣ
	char appVer[1+1];//Ӧ�ð汾��Ϣ
	char SW1SW2[4+1]; //��Ƭ��Ӧ��
	int dataLen; //������������
	char appType[1+1]; //Ӧ�����ͱ�־
	char connectCity[2+1]; //������ͨ��־�������뻥ͨ���б�־��
	char BusiBitMap[4+1]; //ҵ��λͼ����
	char card_no[16+1];
	char sale_type[2+1];

	//��ʼ������
	bbzero(sale_type);
	bbzero(appType);
	bbzero(CItyCode);
	bbzero(SW1SW2);
	bbzero(compay_code);
	bbzero(busi_code);
	bbzero(RFU);
	bbzero(appSerial);
	bbzero(cardChild_type);
	bbzero(cardParent_type);
	bbzero(openDate);
	bbzero(deviceInfo);
	bbzero(appVer);
	bbzero(connectCity);
	bbzero(effectiveDate);
	bbzero(BusiBitMap);
	bbzero(card_no);
	
	
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x00\xB0\x95\x00\x22",8, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
		
	{
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽���ʧ�ܣ������Ӻö�����");
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
		
	}
	
	//������ȡ����Ϣ
	dataLen=2;
	memcpy(compay_code,rcvData+dataLen,2); //����������
	dataLen+=2;
	memcpy(CItyCode,rcvData+dataLen,2); //���б���
	dataLen+=2;
	memcpy(busi_code,rcvData+dataLen,2); //��ҵ����
	dataLen+=2;
	memcpy(RFU,rcvData+dataLen,2);  //RFU
	dataLen+=2;

	memcpy(appType,rcvData+dataLen,1); //Ӧ�����ͱ�־��������00,01��δ����
	dataLen+=1;
	memcpy(appVer,rcvData+dataLen,1); //Ӧ�ð汾
	dataLen+=1;
	memcpy(connectCity,rcvData+dataLen,2); //������ͨ��־�������뻥����ͨ�ĳ��У�
	dataLen+=2;
	memcpy(appSerial,rcvData+dataLen,8); //Ӧ�����к�
	dataLen+=8;
	memcpy(openDate,rcvData+dataLen,4); //��������
	dataLen+=4;
	memcpy(effectiveDate,rcvData+dataLen,4); //Ӧ����Ч����
	dataLen+=4;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) cardParent_type,1); //��������
	dataLen+=1;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) cardChild_type,1);  //��������
	dataLen+=1;
	memcpy(BusiBitMap,rcvData+dataLen,4); //ҵ��λͼ
	dataLen+=4;


	//Ѻ�𿨵��ۿ���ʽ��02 ������01
	if (strcmp(cardParent_type,"10") ==0 ) //�����Ѻ��
	{

		bbzero(sale_type);
		strcpy(sale_type,"02");
	}
	else
	{
	
		bbzero(sale_type);
		strcpy(sale_type,"01");

	}
	

	wrLog("�����Ƭ������Ϣ��0015�ļ���-------------------------");
	wrHex("������:",(BYTE *)compay_code,2);
	wrHex("���д���:",(BYTE *)CItyCode,2);
	wrHex("��ҵ����:",(BYTE *)busi_code,2);
	wrHex("RFU:",(BYTE *)RFU,2);
	wrHex("Ӧ�����ͱ�־",(BYTE *) appType,1);
	wrHex("Ӧ�ð汾:",(BYTE *)appVer,1);
	wrHex("������ͨ��־",(BYTE *)connectCity,2);
	wrHex("Ӧ�����к�:",(BYTE *)appSerial,8);
	wrHex("��������:",(BYTE *)openDate,4);
	wrHex("Ӧ����Ч����:",(BYTE *) effectiveDate,4);
	wrLog("��������:%s",cardParent_type);
	wrLog("��������:%s",cardChild_type);
	wrHex("ҵ��λͼ��Ϣ:",(BYTE *)BusiBitMap,4);
	wrLog("�ۿ���ʽ:%s",sale_type);
	

	if (flag!=3) //����ǿ�Ƭ���׹����ж�ȡ������Ҫ������Щ������Ϣ
	{

		bbzero(pdgtCard->cardShowId);
		getCardShowId(appSerial,pdgtCard->cardShowId);
		wrLog("����(ʮ����):%s",pdgtCard->cardShowId);
		
		bbzero(pdgtCard->compay_code);
		memcpy(pdgtCard->compay_code,compay_code,2);
		bbzero(pdgtCard->CItyCode);
		memcpy(pdgtCard->CItyCode,CItyCode,2);
		bbzero(pdgtCard->busi_code);
		memcpy(pdgtCard->busi_code,busi_code,2);
		bbzero(pdgtCard->appFlag);
		memcpy(pdgtCard->appFlag,appType,1);
		memcpy(pdgtCard->appVer,appVer,1);
		bbzero(pdgtCard->connectCity);
		memcpy(pdgtCard->connectCity,connectCity,2);
		bbzero(pdgtCard->appSerial);
		memcpy(pdgtCard->appSerial,appSerial,8);
		bbzero(pdgtCard->cardParent_type);
		CTools::hex_to_byte((BYTE *) cardParent_type,(BYTE *) pdgtCard->cardParent_type,2);
		bbzero(pdgtCard->cardChild_type);
		CTools::hex_to_byte((BYTE *) cardChild_type,(BYTE *) pdgtCard->cardChild_type,2);
		bbzero(pdgtCard->openDate);
		memcpy(pdgtCard->openDate,openDate,4);
		bbzero(pdgtCard->effectiveDate);
		memcpy(pdgtCard->effectiveDate,effectiveDate,4);
		bbzero(pdgtCard->deviceInfo);
		memcpy(pdgtCard->deviceInfo,deviceInfo,6);
		bbzero(pdgtCard->sale_type);
		strcpy(pdgtCard->sale_type,sale_type);

	}

	return 0;

}


int CDGTBusTrade::getCardShowId(char *cardID,char *cardShowID)
{
	char card_first8[8+1];
	char card_last8[8+1];
	char  card_id16[16+1];
	char showId_last8[16+1];

	bbzero(card_first8);
	bbzero(card_last8);
	bbzero(card_id16);
	bbzero(showId_last8);

	CTools::byte_to_hex((BYTE *) cardID,(BYTE *)card_id16,8);
	
	memcpy(card_first8,card_id16,8);
	memcpy(card_last8,card_id16+8,8);

	CTools::HexToDec(card_last8,showId_last8,8);
	bbzero(cardShowID);
	sprintf(cardShowID,"%s%s",card_first8,showId_last8);

	wrLog("��Ƭ�Ŀ���(ʮ����):%s",cardShowID);

	return 0;

}
//����Ǯ��Ŀ¼�����в�ѯ�����ؿ�Ƭ��Ϣ��

int CDGTBusTrade::accessWalletDir(const TSDLLPosInfo dPosInfo,char *errMsg)
{
	
	char rsp_code[4+1];
	char rcvData[255];
	int rcvLen;

	bbzero(rsp_code);
	bbzero(rcvData);

	wrLog("ѡǮ��Ӧ��Ŀ¼-------------------------------------");
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01",17, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽���ʧ�ܣ������Ӻö�����");
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE * )rsp_code,2);
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
		
	}
 
	return 0;
}

//��ȡ��Ƭ���,��Ҫ�Ƚ���Ǯ��Ŀ¼

int CDGTBusTrade::getWalletMny(const TSDLLPosInfo dPosInfo,char *errMsg,PDGTCard pdgtCard)
{

	char rsp_code[4+1];
	int dataLen;
	int rcvLen;
	char rcvData[255];
	char SW1SW2[4+1];

	bbzero(rcvData);
	bbzero(rsp_code);
	bbzero(SW1SW2);

	//��ȡ��Ƭ��
	wrLog("��ȡ��Ƭ���--------------------------------------------");
	
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x80\x5C\x00\x02\x04",8, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽���ʧ�ܣ������Ӻö�����");
		return -1;
	}
	

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	//��ȡ��Ƭ���
	char cardMny[8+1];
	char mny[12+1];
	bbzero(cardMny);
	bbzero(SW1SW2);
	
	wrLog("�����Ƭ�����Ϣ");
	dataLen=2;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE * )cardMny,4);
	dataLen+=4;

	CTools::byte_to_hex((BYTE *)rcvData+dataLen,(BYTE *)SW1SW2,2);
	
	CTools::HexToDec(cardMny,mny);
	wrLog("��Ƭ���:%s",(BYTE *)cardMny);
	wrLog("SW1SW2:%s",SW1SW2);
	
	wrLog("�����Ƭ�����Ϣ");
	wrLog("��Ƭ���:%.2f",atof(mny)/100);

	if (strcmp(SW1SW2,"9000")!=0) //SW1SW2���������9000
	{
		bbzero(errMsg);
		strcpy(errMsg,"��Ƭ��Ӧʧ��");
		return -2;
	}

	//��Ƭ��

	bbzero(pdgtCard->cardMny); //����Ǯ�����
	strcpy(pdgtCard->cardMny,mny);

	return 0;
}

//flag ��ʾ��ѯ�����ͣ�flag ����Ĭ��ֻ��ѯ��flag=1Ҫ��ѯ��������������Ϣ

int CDGTBusTrade::cardQuery(const TSDLLPosInfo dPosInfo,char *rsp_code,char *errMsg,PDGTCard pdgtCard,int flag)
{
	int iRet;

	//1Ѱ��
	iRet=cardSearch(dPosInfo,rsp_code,errMsg,pdgtCard,flag);
	if (iRet!=0)
	{
		return -1; //Ѱ��ʧ��
	}


	 //��ȡ0005�ļ���Ѻ���ڴ�

	//��ѯ����Ϣ�� //ֱ�Ӷ�ȡ����ʱ�򣬲���Ҫ��ȡ
	iRet=readCardInfo(dPosInfo,pdgtCard,errMsg);
	if (iRet!=0)
	{
		wrLog("��ȡ��Ƭ��Ϣ����");
		return -4;
	}


	//���뿨ƬǮ��Ŀ¼��
	iRet=accessWalletDir(dPosInfo,errMsg);
	if (iRet!=0)
	{
		return -2;
	}

	iRet=getWalletMny(dPosInfo,errMsg,pdgtCard); //����Ǯ�����
	if (iRet!=0)
	{
		return -3; //����Ǯ�����ʧ��

	}


	//��ȡ��Ƭ������Ϣ
	iRet=readCardBasicInfo(dPosInfo,errMsg,pdgtCard,flag);
	
	if (iRet!=0)
		
	{
			wrLog("��ȡ��Ƭ������Ϣʧ��");
	}

	return 0;
}


//��ȡ��ݸͨM1�������Ϣ type 0 ���в�ѯ��� 1���н���
int CDGTBusTrade::getM1BasicInfo(const TSDLLPosInfo dPosInfo,int type,PDGTM1Card pdgtM1Card)
{
	char rcvData[255];
	char sndIcData[255];
	char errMsg[200];
	char rsp_code[4+1];
	bbzero(rcvData);
	char date[8+1];
	char check_date[6+1];
	char cardMny[8+1];
	char outPutMsg[50];
	int iSelect;

	int rcvLen=0;
	int sendLen=0;

	bbzero(sndIcData);
	bbzero(date);
	bbzero(errMsg);
	bbzero(rsp_code);
	bbzero(check_date);
	bbzero(cardMny);
	bbzero(outPutMsg);


	CTools::get_date8(date);
	memcpy(check_date,date+2,6);

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x81",2, (BYTE *)rcvData, rcvLen,errMsg ) )  //��ֵ�鿨�Ƕ�����֮��ġ�
		
	{
		TS_OutPutMsg(dPosInfo," �ն˽�������,�����Ӻö�����!",0,iSelect);
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		return -3;
	}
		

	CTools::byte_to_hex((const BYTE *)rcvData+2,(BYTE*)pdgtM1Card->bus_card,4); //Ʊ����
	CTools::byte_to_hex((BYTE *) rcvData+6,(BYTE *)pdgtM1Card->recogise_code,4); //Ʊ����֤��
	CTools::byte_to_hex((const BYTE *)rcvData+10, (BYTE *)cardMny, 4); //Ʊ������ֵǰ��
	CTools::byte_to_hex((const BYTE *)rcvData+14, (BYTE *)pdgtM1Card->bus_card_type, 1); //Ӧ�������ͣ���Ƭ����
	CTools::byte_to_hex((const BYTE *)rcvData+17, (BYTE *)pdgtM1Card->check_date, 3); //��������

	wrLog("�������ڣ�%s",pdgtM1Card->check_date);

	if (type==1)
	{

		if (((unsigned char)(rcvData[16])==0xAA)) //�¿���0XAA
		{
			TS_OutPutMsg(dPosInfo," \n�ÿ�Ϊ�հ׿������ܽ��н��и�ҵ��!\n   ����ȷ�ϡ�����",0,iSelect);
			return -6;
		}
		
		if (strcmp(pdgtM1Card->bus_card_type,"01")&&strcmp(pdgtM1Card->bus_card_type,"A4"))
		{
			TS_OutPutMsg(dPosInfo," \n�����ʹ�����ѹ���\n    ���ܽ��и�ҵ��!\n   ����ȷ�ϡ�����",0,iSelect);
			return -7;
		}
		if ((atoi(pdgtM1Card->check_date)!=0) && (atoi(check_date)>atoi(pdgtM1Card->check_date)))
		{
			TS_OutPutMsg(dPosInfo,"  \n     �ѹ�������!\n    ���ܽ��и�ҵ��!\n   ����ȷ�ϡ�����",0,iSelect);
			return -8;
		}
	
	}
	
	//��ѯ���
	if (type==0)
	{
		if (((unsigned char)(rcvData[16])==0xAA)) //�¿���0XAA
		{
			TS_OutPutMsg(dPosInfo," \n�ÿ�Ϊ�հ׿�!\n   ����ȷ�ϡ�����",0,iSelect);
			return -6;
		}
	}

	memcpy(pdgtM1Card->dkq_code,rcvData+20,4);
	CTools::byte_to_hex((const BYTE *)rcvData+24, (BYTE *)pdgtM1Card->psam, 8); //psam����
	
	wrHex("��ֵ�鿨���ػ��߱��",(BYTE *)pdgtM1Card->psam,4);
	wrLog("PSAM������:%s",pdgtM1Card->psam);
	//ת����10���ƵĿ���
	CTools::getDGTM1CardId(pdgtM1Card->bus_card,pdgtM1Card->bus_showCard);
	
	//��ն�������Ϣ
	bbzero(rcvData);
	rcvLen=0;
	wrLog("��ն�������ʾ��Ϣ");
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\xF0\x40\x00\x01",4, (BYTE *)rcvData, rcvLen,errMsg )) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
			
		TS_OutPutMsg(dPosInfo," �ն˽������������Ӷ�����!\n����ȷ�ϡ�����",0,iSelect);
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		return -3;
	}
		

	//���Ͷ�������ʾ�����Ϣ
	wrLog("���͸���������ʾ��Ϣ��ָ��");
	char bus_amt[8+1];
	int msg_len=0;
	bbzero(outPutMsg);
	bbzero(pdgtM1Card->bus_amt);
	CTools::sixten_toten(cardMny,bus_amt);
	sprintf(pdgtM1Card->bus_amt,"%08s",bus_amt);

	sprintf(outPutMsg,"���:%.2fԪ",atof(pdgtM1Card->bus_amt)/100); //�ڶ�����������ʾ��
	bbzero(sndIcData);
	sendLen=0;

	memcpy(sndIcData,"\xF0\x41\x01\x01",4); //��ʾһ����Ϣ
	sendLen+=4;
	memcpy(sndIcData+sendLen,outPutMsg,strlen(outPutMsg)); 
	sendLen+=strlen(outPutMsg);
	
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen ,errMsg) )//��ֵ�鿨�Ƕ�����֮��ġ�
	{
		TS_OutPutMsg(dPosInfo,"�ն˽������������Ӻö�����",0,iSelect);
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		return -3;
	}
		

	return 0;


}

//��ѯM1����Ϣ 0 ,��ѯ��1 �Ǳ�ʾ����
int CDGTBusTrade::cardM1Query(const TSDLLPosInfo dPosInfo,char *rsp_code,int type,char *errMsg,PDGTM1Card pdgtM1Card)
{
	int iRet;
	
	//��ѯ��Ƭ��Ч�ڵ���Ϣ
	iRet=getM1SBInfo(dPosInfo,pdgtM1Card,errMsg);
	if (iRet!=0)
	{
		return iRet; //Ѱ��ʧ��
	}

	//��ȡ��Ƭ������Ϣ

	iRet=getM1BasicInfo(dPosInfo,type,pdgtM1Card);
	if (iRet!=0)
	{
		if (iRet==-6)
		{
			return -4; //�Կ�Ƭ�ǿհ׿��������ⷵ���봦��
		}
		else
		{
			return -5;

		}
	
	}

	//����M1��������Ϣ

	wrLog("��Ƭ���:%s",pdgtM1Card->bus_amt);
	wrLog("����:%s",pdgtM1Card->bus_card);
	wrLog("����ʾ����:%s",pdgtM1Card->bus_showCard);
	wrLog("��Ƭ����:%s",pdgtM1Card->bus_card_type);
	wrLog("�ӿ�����:%s",pdgtM1Card->card_child_type);
	wrLog("��������:%s",pdgtM1Card->card_parent_type);
	wrLog("��������:%s",pdgtM1Card->check_date);
	wrLog("cardCSN:%s",pdgtM1Card->M1CardCSN);
	wrLog("��֤��:%s",pdgtM1Card->recogise_code);
	wrLog("Ѻ��:%s",pdgtM1Card->deposit);
	wrLog("��������:%s",pdgtM1Card->openDate);
	wrLog("����ʱ��:%s",pdgtM1Card->openTime);
	wrLog("��Ƭ��Ч��:%s",pdgtM1Card->cardVadate);
	wrLog("��Ƭ״̬:%s",pdgtM1Card->cardStatus);

	return 0;
}


//��������һ��

int CDGTBusTrade::buzzer(const TSDLLPosInfo dPosInfo,char *errMsg)
{

	char rsp_code[4+1];
	char rcvData[255];
	int rcvLen;

	bbzero(rcvData);
	bbzero(rsp_code);

	//��������һ��,��ʾ�ɹ���
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x31\x13\x00\x32\x01",5, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽���ʧ�ܣ������Ӻö�����");
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	return 0;


}

int CDGTBusTrade::clearLEDMsg(const TSDLLPosInfo dPosInfo,char *errMsg)
{

	char rcvData[255];
	int rcvLen;
	char rsp_code[4+1];

	bbzero(rcvData);
	bbzero(rsp_code);

	rcvLen=0;
	wrLog("��ն�������ʾ��Ϣ");
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\xF0\x40\x00\x01",4, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"���ն˽���ʧ�ܣ������Ӻö�����");
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	return 0;

}



int CDGTBusTrade::showLEDMsg(const TSDLLPosInfo dPosInfo,char *showMsg,char *errMsg)
{

	//���Ͷ�������ʾ�����Ϣ
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int iRet;

	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);

	//��Ҫ��ִ�������Ϣ�Ĳ���
	iRet=clearLEDMsg(dPosInfo,errMsg);
	if (iRet!=0)
	{
		return -2; //��ʾ�����ʾ��ʧ��
	}

	wrLog("���͸���������ʾ��Ϣ");
	
	memcpy(sndIcData,"\xF0\x41\x01\x01",4); //��ʾһ����Ϣ
	sendLen+=4;
	
	memcpy(sndIcData+sendLen,showMsg,strlen(showMsg)); 
	sendLen+=strlen(showMsg);
	
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;

	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	return 0;

}


//��������
/************************************************************************/
/*                                                                      */
//openDate :����ʱ��
//effectiveDate:��Чʱ��
//cardStatus:��Ƭ״̬
//cardInfo ����Ƭ������Ϣ 0015�ļ��е�10-20
//dicMac:ָ��mac
/************************************************************************/

int CDGTBusTrade::openCard(const TSDLLPosInfo dPosInfo,char *APDU,char *dicMac,char *errMsg)
{
	
	//��ȡ�����
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char APDU_COMMAND[100];
	char DIC_MAC_COMAND[8+1];
	char SW1SW2[4+1];

	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(APDU_COMMAND);
	bbzero(DIC_MAC_COMAND);
	bbzero(SW1SW2);

	memcpy(DIC_MAC_COMAND,dicMac,8); 
	wrLog("���Ƭ");
	wrLog("apdu_command222:%s",APDU);

	wrHex("DICMAC:",(BYTE *)dicMac,strlen(dicMac));
	memcpy(sndIcData,"\x32\x26\xff ",3); 
	sendLen+=3;
	memcpy(sndIcData+sendLen,APDU,strlen(APDU));
	sendLen+=strlen(APDU);

	CTools::hex_to_byte((BYTE *) DIC_MAC_COMAND,(BYTE *) sndIcData+sendLen,8);
	sendLen+=4;
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ���� ��ȷʧ��
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
	}
	
	//�ۿ�ִ��ִ�гɹ�
	CTools::byte_to_hex((BYTE *)rcvData+2,(BYTE *)SW1SW2,2);
	wrLog("sw1sw2HAHH:%s",SW1SW2);
	if (strcmp(SW1SW2,"9000") !=0) //���sw1sw2������9000 ���ʾ ��ִ��ʧ�� 
	{
		return -2; //д��ʧ�� ��ȷʧ��
	}

	return 0;
	
}


//��������
/************************************************************************/
/*                                                                      */
//openDate :����ʱ��
//effectiveDate:��Чʱ��
//cardStatus:��Ƭ״̬
//cardInfo ����Ƭ������Ϣ 0015�ļ��е�10-20
//dicMac:ָ��mac
/************************************************************************/

int CDGTBusTrade::openCard(const TSDLLPosInfo dPosInfo,char *dicMac,char *errMsg,DGTCard dgtCard)
{
	
	//��ȡ�����
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char APDU_COMMAND[100];
	int APDU_LEN=0;
	char APDU[100];
	char DICMAC_COMMAND[8+1];
	char SW1SW2[4+1];
	char openTime[6+1];
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(APDU_COMMAND);
	bbzero(APDU);
	bbzero(DICMAC_COMMAND);
	bbzero(SW1SW2);
	bbzero(openTime);

	wrLog("���Ƭ");
	char openDate[14+1]; //��������
	char effectiveDate[14+1];//��Ч����
	bbzero(openDate);
	bbzero(effectiveDate);
	//���ÿ�������
	CTools::getDate(openDate);
	CTools::getTime(openTime);
	
	bbzero(dgtCard.openTime);
	strcpy(dgtCard.openTime,openTime);

	//������Ч���� ��Ч���ڹ̶�Ϊ��ʾ��ͨ��
	strcpy(effectiveDate,"20901231");

	//���÷�����д��� ֮ǰ��װAPDU����

	//wrlog("DICMAC:%s",dicMac);
	memcpy(DICMAC_COMMAND,dicMac,8); 
	memcpy(sndIcData,"\x32\x26\xff\x04\xd6\x95\x08\x18 ",8); 
	sendLen+=8;
	memcpy(sndIcData+sendLen,"\x01",1); //��ͨ��Ƭ
	sendLen+=1;
	memcpy(sndIcData+sendLen,dgtCard.appVer,1);
	sendLen+=1;
	memcpy(sndIcData+sendLen,dgtCard.connectCity,2);
	sendLen+=2;
	memcpy(sndIcData+sendLen,dgtCard.appSerial,8); //�����к�
	sendLen+=8;
	CTools::hex_to_byte((BYTE *)openDate,(BYTE *)sndIcData+sendLen,8); //����ʱ��
	sendLen+=4;
	CTools::hex_to_byte((BYTE *)effectiveDate, (BYTE *)sndIcData+sendLen,8);
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) DICMAC_COMMAND,(BYTE *) sndIcData+sendLen,8);
	sendLen+=4;
	
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
	}
	
	//�ۿ�ִ��ִ�гɹ�
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)SW1SW2,2);

	if (strcmp(SW1SW2,"9000")!=0) //��������ص�������9000 test
	{
		//��ִ�д���.
		return -2;

	}
	
	return 0;
	
}


//��ʼ��CPU��
//02 00 00 32 26 ff 805000020B[02��ֵ��Կ����]  00000001  000000000002  10 [xor] 03
//cardAmount :��ʾ��Ƭ��ֵǰ���
//cardCount:��Ƭ������
//initRandom:�����
//��ݸͨ��Ƭ��Կ������:01.
//ִ�гɹ���ʱ�򷵻�

int CDGTBusTrade::initCPUCard(const TSDLLPosInfo dPosInfo,char *POSID,char *payAmount,char *cardSeq,char *keyVer,char *ALGIND,char *initRandom,char *cardAmount,char *MAC1,char *errMsg)
{
	//��ȡ�����
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char initMNY[8+1];
	char  POS_ID_BYTE[6+1];
	char cardCount[4+1];

	bbzero(initMNY);
	bbzero(cardCount);
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(POS_ID_BYTE);

	wrLog("payAmount:%s",payAmount);
	CTools::DecToHex(payAmount, initMNY,8); 
	wrLog("��ȡ�����");
	wrLog("��ȡ�����POSID:%s",POSID);

	memcpy(sndIcData,"\x32\x26\xff\x80\x50\x00\x02\x0B\x01",9); //��Կ������01
	sendLen+=9;
	//���
	CTools::hex_to_byte((BYTE *) initMNY,(BYTE *)sndIcData+sendLen,8);
	sendLen+=4;
	//�ն˱仯
	CTools::hex_to_byte((BYTE *)POSID,(BYTE *)sndIcData+sendLen,strlen(POSID));
	sendLen+=6;
	memcpy(sndIcData+sendLen,POS_ID_BYTE,strlen(POS_ID_BYTE));
	sendLen+=strlen(POS_ID_BYTE);
	CTools::hex_to_byte((BYTE *) "10",(BYTE *)sndIcData+sendLen,2);
	sendLen+=1;
	
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;
		
	}
	
	if (rcvLen<=4) //���������������Ӧ����
	{
		bbzero(errMsg);
		strcpy(errMsg,"CPU����ʼ��ʧ��");
		return -2;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	//��ȡ����ǰ�������������������MAC1
	dataLen=2;
	bbzero(cardAmount);
	bbzero(cardCount);
	bbzero(initRandom);
	bbzero(MAC1);
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)cardAmount,4);
	dataLen+=4;

	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)cardCount,2); //�������
	dataLen+=2;

	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)keyVer,1); //��Կ�汾
	dataLen+=1;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)ALGIND,1); //�㷨��־
	dataLen+=1;

	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)initRandom,4); //�����
	dataLen+=4;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)MAC1,4);
	dataLen+=4;
	
	char CARD_trade_seq_tmp[8+1];
	bbzero(CARD_trade_seq_tmp);
	sprintf(CARD_trade_seq_tmp,"0000%s",cardCount);
	bbzero(cardCount);
	CTools::HexToDec(CARD_trade_seq_tmp,cardCount,8); //ת����10���Ƶ�6λ���֡�
	memcpy(cardSeq,cardCount+2,6);
	

	//�����Ϣ

	wrLog("cpu��ʼ�������Ϣ");

	wrLog("����ǰ���:%s",cardAmount);
	wrLog("��Ƭ�������:%s",cardSeq);
	wrLog("��Կ�汾:%s",keyVer);
	wrLog("�㷨��־:%s",ALGIND);
	wrLog("�������:%s",cardSeq);

	wrLog("�����:%s",initRandom);
	wrLog("MAC1:%s",MAC1);

	return 0;

}


//getPIN2 //����PIN2
int CDGTBusTrade::getPIN2(char *appSerial,char *PIN2)
{

	char szMAK[17] = {0};
	char szCardNo[17] = {0};

	int a = 0;
	int b = 0;

	wrLog("appserial:%s",appSerial);

	memcpy(szMAK, RLD_PIN_MAK, 16);
	memcpy(szCardNo, appSerial + 0, 16);
	
	memset(PIN2, 0, sizeof(PIN2));
	for (int nCur = 0; nCur < 16; nCur++)
	{
		a = szCardNo[nCur] - '0';
		b = szMAK[nCur] - '0';
		PIN2[nCur] = (a + b) % 10 + '0';
	}

	wrLog("pin2:%s",PIN2);

	return 0;
}


//У��PIN2

int CDGTBusTrade::checkPIN2(const TSDLLPosInfo dPosInfo,char *appSerial,char *errMsg )
{
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int iRet;
	int dataLen=0;
	char PIN2[17+1];
	char SW1SW2[4+1];
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(PIN2);
	bbzero(SW1SW2);
	
	//����PIN2:
	iRet=getPIN2(appSerial,PIN2);
	wrLog("У��PIN2");
	
	memcpy(sndIcData,"\x32\x26\xff\x00\x20\x00\x01\x06",8); 
	sendLen+=8;
	//pin2
	CTools::hex_to_byte((BYTE *)PIN2,(BYTE *)sndIcData+sendLen,12);
	sendLen+=6;

	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		wrLog("У��pinʧ��");
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	bbzero(SW1SW2);
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *) SW1SW2,2);
	if (strcmp(SW1SW2,"9000")!=0) //���SW1SW2����9000.��ʾ����Ӧʧ��
	{
		wrLog("У��pinʧ��");
		return -2;

	}

	wrLog("У��PIN�ɹ�");
	return 0;

}


//��ȡ���м�¼
//��ȡ����֮ǰ��Ҫ�ϵ�.

int CDGTBusTrade::readAllTxnLogs(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char* rsp_code,char *errMsg)
{

	char sndIcData[255];
	char rcvData[255];
	char tradeTime[14+1]; 
	int rcvLen=0;
	int sendLen=0;
	int dataLen=0;
	char SW1SW2[4+1];
	char index_temp[2+1];
	int iRet;
	int iSelect;

	TSTxnLogs txnLogs;
	dgtALLTrades dgtALLTxnLogs;
	memset(&dgtALLTxnLogs,0,sizeof(dgtALLTxnLogs));
	memset(&txnLogs,0,sizeof(txnLogs));
	bbzero(tradeTime);
	bbzero(SW1SW2);

	//��Ƭ����

	//���ζ���
	for(int q=0;q<3;q++)
	{
		
		iRet=CDGTBusTrade::cardQuery(dPosInfo,rsp_code,errMsg,pdgtCard,1);
		if (iRet!=0) //�����ȡ����
		{
			if (strcmp(rsp_code,"3005")==0 || strcmp(rsp_code,"3006")==0 ) //�������д����ʱ�����
			{
				
				bbzero(errMsg);
				strcpy(errMsg,"��ѯ������ʧ�ܣ��뽫�����ڶ�����������\n �밴��Enter������");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;
				
			}
			
			
		}
		else //�����ɹ�ֱ���˳�Ѱ��
		{
			
			break;
		}
		
		
	}
	
	if (iRet!=0)
	{
		return -3; //��ѯʧ��
	}


	int j=0;
	for(int i=0;i<10;i++)
	{
		char tradeDate[8+1];
		char tradeTime[6+1];
		bbzero(tradeDate);
		bbzero(tradeTime);

		wrLog("��ȡ��ֵ���׼�¼");
		bbzero(sndIcData);
		bbzero(rcvData);
		rcvLen=0;
		sendLen=0;
		bbzero(index_temp);
		sprintf(index_temp,"%02d",(i+1));
		
		memcpy(sndIcData,"\x32\x26\xff\x00\xB2 ",5);
		sendLen+=5;
		//index
		CTools::hex_to_byte((BYTE *)index_temp,(BYTE *)sndIcData+sendLen,2);
		sendLen+=1;
		CTools::hex_to_byte((BYTE *)"C400",(BYTE *)sndIcData+sendLen,4);//00 ��ʾ��ȡ���ļ�β��
		sendLen+=2;

		bbzero(rcvData);
		bbzero(rsp_code);
		rcvLen=0;
		//����Ϣ���ն� 
		if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
		{
			bbzero(errMsg);
			strcpy(errMsg,"�ն˽������������Ӻö�����");
			return -1;
			
		}
		
		if (rcvLen<=4) //������4������Ӧʧ��
		{
			continue;
		}

		bbzero(rsp_code);
		CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
		
		if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
			
		{
			continue; 
		}
		
		//ִ�гɹ���������ϸ����
		dataLen=2;
		CTools::byte_to_hex((BYTE *)rcvData+18,(BYTE *)tradeDate,4);
		CTools::byte_to_hex((BYTE *) rcvData+22,(BYTE *)tradeTime,3);
		if (atoi(tradeDate)==0 && atoi(tradeTime)==0)
		{
			continue;
		}
		
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtALLTxnLogs.cardAllTradeDetail[j].cardSEQ,2);
		dataLen+=2;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtALLTxnLogs.cardAllTradeDetail[j].limitMny,3);
		dataLen+=3;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].tradeMny,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].tradeType,1);
		dataLen+=1;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtALLTxnLogs.cardAllTradeDetail[j].POS_ID,6);
		dataLen+=6;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].trade_date,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].trade_time,3);
		dataLen+=3;
		j++;
		
	}
	
	dgtALLTxnLogs.nCnt=j;
	if (dgtALLTxnLogs.nCnt==0)
	{
		return -2; //��ȷ�޽��׼�¼
	}

	//��ʾ�б�
	txnLogs.colsnum = 7;
	txnLogs.colsWidth[0] = 100;
	txnLogs.colsWidth[1] = 60;
	txnLogs.colsWidth[2] = 60;
	txnLogs.colsWidth[3] = 60;
	txnLogs.colsWidth[4] = 60;
	txnLogs.colsWidth[5] = 60;
	txnLogs.colsWidth[6] = 60;

	txnLogs.colsNameLen = 200;
	memcpy(txnLogs.colsName, "��������\x00�������\x00͸֧�޶�(Ԫ)\x00���׽��(Ԫ)\x00��������\x00��������\x00����ʱ��", txnLogs.colsNameLen);
	
	for (int m =0 ;m <dgtALLTxnLogs.nCnt;m++)
	{

		char cardSEQ[6+1];
		bbzero(cardSEQ);
		char limitMny[12+1];
		char showLimitMny[10];
		char showTradeMny[10];
		char tradeMny[12+1];
		char tradeType[10];
		char index[2+1];

		bbzero(limitMny);
		bbzero(tradeMny);
		bbzero(showLimitMny);
		bbzero(tradeType);
		bbzero(showTradeMny);
		bbzero(index);

		if( txnLogs.itemsLen+strlen(pdgtCard->cardShowId) < MAX_INTERM_BUF ) //��������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,pdgtCard->cardShowId); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		CTools::HexToDec(dgtALLTxnLogs.cardAllTradeDetail[m].cardSEQ,cardSEQ,6);
		if (strcmp(dgtALLTxnLogs.cardAllTradeDetail[m].tradeType,"02")==0)
		{
			strcpy(tradeType,"��ֵ");
		}
		else
		{
		
			strcpy(tradeType,"����");
		}


		CTools::HexToDec(dgtALLTxnLogs.cardAllTradeDetail[m].tradeMny,tradeMny,12);
		sprintf(showTradeMny,"%.2f",atof(tradeMny)/100);
		CTools::HexToDec(dgtALLTxnLogs.cardAllTradeDetail[m].limitMny,limitMny,12);
		sprintf(showLimitMny,"%.2f",atof(limitMny)/100);
		

		if( txnLogs.itemsLen+strlen(cardSEQ) < MAX_INTERM_BUF ) //�������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,cardSEQ); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(showLimitMny) < MAX_INTERM_BUF ) //͸֧���
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showLimitMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}
		
		if( txnLogs.itemsLen+strlen(showTradeMny) < MAX_INTERM_BUF ) //���׽��
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showTradeMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(tradeType) < MAX_INTERM_BUF ) //��������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,tradeType); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtALLTxnLogs.cardAllTradeDetail[m].trade_date) < MAX_INTERM_BUF ) //��������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtALLTxnLogs.cardAllTradeDetail[m].trade_date); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtALLTxnLogs.cardAllTradeDetail[m].trade_time) < MAX_INTERM_BUF ) //����ʱ��
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtALLTxnLogs.cardAllTradeDetail[m].trade_time); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


	}

	sprintf(txnLogs.bottom," ��ѯ���10�ʹ�����ֵ���� �����밴��Esc��") ;
	iRet=TS_ShowTxnLogs(dPosInfo,&txnLogs);

	if (iRet!=0)
	{
		wrLog("TS_ShowTxnLogs���ش���[%d]", iRet); 
		return -1;
	}

	return iRet;

	
}


//��ֵ��¼ 
//��ȡ��ֵ��¼֮ǰ��Ҫ����Ѱ���������ҽ���Ǯ��Ŀ¼��

int CDGTBusTrade::readChargeLogs(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char *rsp_code,char *errMsg)
{

	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	int dataLen=0;
	char SW1SW2[4+1];
	char index_temp[2+1];
	int iRet;
	TSTxnLogs txnLogs;
	dgtChargeTrades dgtChargeLogs;
	int iSelect;
	char tradeTime[8+1];
	char tradeDate[6+1];

	dgtChargeLogs.nCnt=0;
	memset(&txnLogs,0, sizeof(txnLogs));
	memset(&dgtChargeLogs,0,sizeof(dgtChargeLogs));
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(tradeTime);
	bbzero(SW1SW2);
	bbzero(tradeTime);
	bbzero(tradeDate);

	//���ζ���
	for(int q=0;q<3;q++)
	{
	
		iRet=CDGTBusTrade::cardQuery(dPosInfo,rsp_code,errMsg,pdgtCard,1);
		if (iRet!=0) //�����ȡ����
		{
			if (strcmp(rsp_code,"3005")==0 || strcmp(rsp_code,"3006")==0 ) //�������д����ʱ�����
			{

				bbzero(errMsg);
				strcpy(errMsg,"��ѯ������ʧ�ܣ��뽫�����ڶ�����������\n �밴��Enter������");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;

			}
			
		
		}
		else //�����ɹ�ֱ���˳�Ѱ��
		{
			
			break;
		}

		
	}

	if (iRet!=0)
	{
		return -3; //��ѯʧ��
	}

	memset(&dgtChargeLogs,0,sizeof(dgtChargeLogs));

	int j=0;

	for(int i=0;i<4;i++)
	{
		wrLog("��ȡ��ֵ���׼�¼");
		bbzero(index_temp);
		bbzero(sndIcData);
		bbzero(rcvData);
		rcvLen=0;
		sendLen=0;

		sprintf(index_temp,"%02d",(i+1));
		memcpy(sndIcData,"\x32\x26\xff\x00\xB2",5);
		sendLen+=5;
		//index
		CTools::hex_to_byte((BYTE *)index_temp,(BYTE *)sndIcData+sendLen,2);
		sendLen+=1;
		CTools::hex_to_byte((BYTE *)"DC00",(BYTE *)sndIcData+sendLen,4); //00��ʾ֪���ļ�����Ϊֹ
		sendLen+=2;
		
		//����Ϣ���ն� 
		if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) )//��ֵ�鿨�Ƕ�����֮��ġ�
		{
			bbzero(errMsg);
			strcpy(errMsg,"�ն˽������������Ӻö�����");
			return -1;
			
		}
		
		if (rcvLen<=4) //���С���ĸ��ֽ� //�޷�������
		{
			continue;
		}


		bbzero(rsp_code);
		CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

		if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
			
		{
			//bbzero(errMsg);
			//returnHIDGTFail(rsp_code,errMsg);  
			continue; 
		}

		//���ִ�в��ɹ��Ļ������ݳ�����2���ֽڶ�������Ӧ+2���ֽ�SW1SW2 =4���ֽ�
		//ִ�гɹ���������ϸ����
		dataLen=2;
		//	�жϽ���ʱ��ͽ�������Ϊ0��˵���ǿռ�¼
		
		CTools::byte_to_hex((BYTE *)rcvData+18,(BYTE *)tradeDate,4);
		CTools::byte_to_hex((BYTE *) rcvData+22,(BYTE *)tradeTime,3);
		if (atoi(tradeDate)==0 && atoi(tradeTime)==0)
		{
			continue;
		}
		

		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].cardSEQ,2);
		dataLen+=2;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].tradeAfterMny,3);
		dataLen+=3;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].tradeMny,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].tradeType,1);
		dataLen+=1;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].POS_ID,6);
		dataLen+=6;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].trade_date,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].trade_time,3);
		dataLen+=3;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].TAC,4);
		dataLen+=4;
		j++;
	
	}

	dgtChargeLogs.nCnt=j;

	if (dgtChargeLogs.nCnt==0)
	{
		return -2; //��ȷ�޽��׼�¼
	}

	//��ʾ�б�
	txnLogs.colsnum = 7;
	txnLogs.colsWidth[0] = 100;
	txnLogs.colsWidth[1] = 60;
	txnLogs.colsWidth[2] = 60;
	txnLogs.colsWidth[3] = 60;
	txnLogs.colsWidth[4] = 60;
	txnLogs.colsWidth[5] = 60;
	txnLogs.colsWidth[6] = 60;
	txnLogs.colsNameLen = 200;
	memcpy(txnLogs.colsName, "��������\x00�������\x00���׺����(Ԫ)\x00���׽��(Ԫ)\x00��������\x00��������\x00����ʱ��", txnLogs.colsNameLen);
	
	for (int m =0 ;m <dgtChargeLogs.nCnt;m++)
	{

		char cardSEQ[6+1];
		bbzero(cardSEQ);
		char tradeAfterMny[12+1];
		char showCardMny[10];
		char showTradeMny[10];
		bbzero(showCardMny);
		char tradeMny[12+1];
		bbzero(tradeMny);
		bbzero(showCardMny);
		char tradeType[10];
		bbzero(tradeType);
		bbzero(showTradeMny);
		char index[2+1];
		bbzero(index);

		if( txnLogs.itemsLen+strlen(pdgtCard->cardShowId) < MAX_INTERM_BUF ) //��������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,pdgtCard->cardShowId); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		CTools::HexToDec(dgtChargeLogs.cardChargeDetail[m].cardSEQ,cardSEQ,6);
		if (strcmp(dgtChargeLogs.cardChargeDetail[m].tradeType,"02")==0)
		{
			strcpy(tradeType,"��ֵ");
		}
		else
		{
		
			strcpy(tradeType,"����");
		}


		CTools::HexToDec(dgtChargeLogs.cardChargeDetail[m].tradeAfterMny,tradeAfterMny,12);
		sprintf(showCardMny,"%.2f",atof(tradeAfterMny)/100);
		
		CTools::HexToDec(dgtChargeLogs.cardChargeDetail[m].tradeMny,tradeMny,12);
		sprintf(showTradeMny,"%.2f",atof(tradeMny)/100);

		if( txnLogs.itemsLen+strlen(cardSEQ) < MAX_INTERM_BUF ) //�������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,cardSEQ); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(showCardMny) < MAX_INTERM_BUF ) //���׺����
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showCardMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}
		
		if( txnLogs.itemsLen+strlen(showTradeMny) < MAX_INTERM_BUF ) //���׽��
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showTradeMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(tradeType) < MAX_INTERM_BUF ) //��������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,tradeType); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtChargeLogs.cardChargeDetail[m].trade_date) < MAX_INTERM_BUF ) //��������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtChargeLogs.cardChargeDetail[m].trade_date); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtChargeLogs.cardChargeDetail[m].trade_time) < MAX_INTERM_BUF ) //����ʱ��
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtChargeLogs.cardChargeDetail[m].trade_time); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


	}

	sprintf(txnLogs.bottom," ��ѯ���������4�ʳ�ֵ���� �����밴��Esc��") ;
	iRet=TS_ShowTxnLogs(dPosInfo,&txnLogs);

	if (iRet!=0)
	{
		wrLog("TS_ShowTxnLogs���ش���[%d]", iRet); 
		return -1;
	}

	return iRet;

}

//д��ֵ��¼

int CDGTBusTrade::wrTxnLogs(const TSDLLPosInfo dPosInfo,char *posId,DGTCard dgtCard,char *tradeType,char *errMsg)
{

	int iRet;
	bbzero(errMsg);
	char appSerial[16+1];
	bbzero(appSerial);
	CTools::byte_to_hex((BYTE *) dgtCard.appSerial,(BYTE *)appSerial,8);

	//���ȼ���PIN2
	
	iRet=checkPIN2(dPosInfo,appSerial,errMsg);
	if (iRet!=0)
	{
		bbzero(errMsg);
		strcpy(errMsg,"PIN2У�����");
		wrLog("PIN2У�����");
		return -1;
	}

  
	iRet=wrCardTxnLog(dPosInfo,posId,dgtCard,tradeType,errMsg);
	if (iRet!=0)
	{
		bbzero(errMsg);
		strcpy(errMsg,"д��ʧ��");
		return -2; //д��ʧ��
	}

	wrLog("д����ֵ��¼�ɹ�");

	return 0;

}

//�����׽��д����Ƭ����
//tradeType����������
int CDGTBusTrade::wrCardTxnLog(const TSDLLPosInfo dPosInfo,char * posId,DGTCard dgtCard,char *tradeType,char *errMsg)
{

	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char SW1SW2[4+1];
	char tradeAfterMny[6+1]; //���׺���
	char tradeMny[8+1]; //���׽��
	char cardCNT[4+1];

	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(SW1SW2);
	bbzero(tradeAfterMny);
	bbzero(tradeMny);
	bbzero(cardCNT);
	
	CTools::DecToHex(dgtCard.CARDCNT,cardCNT,4);
	CTools::DecToHex(dgtCard.cardMny,tradeAfterMny,6);
	CTools::DecToHex(dgtCard.tradeMny,tradeMny,8);
	wrLog("cardCNT:%s",cardCNT);
	wrLog("tradeAfterMny:%s",tradeAfterMny);
	wrLog("tradeMny:%s",tradeMny);

	memcpy(sndIcData,"\x32\x26\xff\x00\xE2\x00\xD8\x1F",8); 
	sendLen+=8;
	//��ӽ��׼�¼
	CTools::hex_to_byte((BYTE *) cardCNT,(BYTE *)sndIcData+sendLen,4);
	sendLen+=2;
	CTools::hex_to_byte((BYTE *)tradeAfterMny,(BYTE *) sndIcData+sendLen,6 ); //��ֵ����
	sendLen+=3;
	CTools::hex_to_byte((BYTE *) tradeMny,(BYTE *) sndIcData+sendLen,8); //���׽�� 
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) tradeType,(BYTE *) sndIcData+sendLen,2); //��������
	sendLen+=1;

	CTools::hex_to_byte((BYTE *) posId,(BYTE *) sndIcData+sendLen,12); //�����ն˺�
	sendLen+=6;

	CTools::hex_to_byte((BYTE *) dgtCard.tradeDate,(BYTE *) sndIcData+sendLen,8); //��������
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) dgtCard.tradeTime,(BYTE *) sndIcData+sendLen,6); //����ʱ��
	sendLen+=3;

	CTools::hex_to_byte((BYTE *) dgtCard.TAC,(BYTE *)sndIcData+sendLen,8);
	sendLen+=4;

	memcpy(sndIcData+sendLen,"\x00\x00\x00\x00",4);
	sendLen+=4;

	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *) SW1SW2,2);

	if (strcmp(SW1SW2,"9000")!=0) //�����״̬��Ӧ����9000
	{
		return -2;
	}


	return 0;
	
}


//Ȧ��
//02 00 14 32 26 FF 80 52 00 00 0B [20131213 15 10 09��������][xx xx xx xx MAC2]04 [xor] 03
//02 00 08 00 00 [xx xx xx xx TAC 4�ֽ�]9000 [xor]03

int CDGTBusTrade::cpuCard_load(const TSDLLPosInfo dPosInfo,char *tradeTime,char *MAC2,char *TAC,char *errMsg )
{
	
	char sndIcData[1024];
	char rcvData[1024];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char SW1SW2[4+1];
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(SW1SW2);
	wrLog("cpuǮ��Ȧ��");

	memcpy(sndIcData+sendLen,"\x32\x26\xFF\x80\x52\x00\x00\x0B",8); 
	sendLen+=8;
	//��������
	CTools::hex_to_byte((BYTE *)tradeTime,(BYTE *)sndIcData+sendLen,14);
	sendLen+=7;
	//MAC2
	CTools::hex_to_byte((BYTE *)MAC2,(BYTE *)sndIcData+sendLen,8);
	sendLen+=4;

	sndIcData[sendLen]='\x04';
	sendLen+=1;
	
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1; //��ʱ�����
		
	}
	
	wrLog("rcvLen��%d",rcvLen);
	if (rcvLen<4) // ��Ӧ��2���ֽڣ���Ƭ��Ӧ 2���ֽ� ��tac ��4���ֽڣ�
	{
		return -2;//��ֵʧ��
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

	if (strcmp(rsp_code,"0000")!=0)
	{
		return -2;//��������Ӧ����,��ȷʧ�ܵ����
	}

	//TAC������֤��

	bbzero(TAC);
	dataLen=2;

	CTools::byte_to_hex((BYTE *)rcvData+dataLen,(BYTE *)TAC,4);
	dataLen+=4;

	bbzero(SW1SW2);
	CTools::byte_to_hex((BYTE *)rcvData+dataLen,(BYTE *) SW1SW2,2);
	if (strcmp(SW1SW2,"9000")!=0) //���sw1sw2����9000
	{
		return -2; // ���������ش��� ��ȷʧ��

	}
	
	return 0;

}

//��ȡ�����Random

int CDGTBusTrade::getRandom(const TSDLLPosInfo dPosInfo,char *errMsg,char *Random)
{

	//��ȡ�����
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	
	wrLog("��ȡ�����");
	
	memcpy(sndIcData,"\x32\x26\xff\x00\x84\x00\x00\x04 ",8); //��ʾһ����Ϣ
	sendLen+=8;
	
	//����Ϣ���ն� 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //��ֵ�鿨�Ƕ�����֮��ġ�
	{
		bbzero(errMsg);
		strcpy(errMsg,"�ն˽������������Ӻö�����");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //ִ��ָ��ɹ�,��ʾ����
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	//��ȡrandom
	bbzero(Random);
	dataLen=2;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)Random,4);
	return 0;

}


//��ʾ������������Ϣ
void CDGTBusTrade::returnHIDGTFail(char *rsp_code,char *errMsg) //��ʾ������ʾ
{
	switch(atoi(rsp_code))
	{
		
	case 3001:
		bbzero(errMsg);
		strcpy(errMsg,"��֧�ַǽӴ��û���!");
		break;
	case 3005:
		bbzero(errMsg);
		strcpy(errMsg,"�ǽӴ��û�������ʧ��");
		break;
	case 3006:
		bbzero(errMsg);
		strcpy(errMsg,"�ȴ��������Ӧ����ʱ");
		break;
	case 3009:
		bbzero(errMsg);
		strcpy(errMsg,"�ж��ſ��ڸ�Ӧ��");
		break;
	default:
		bbzero(errMsg);
		strcpy(errMsg,"δ֪����");
		break;
	}
	
}


//��ʼ�����ͨ�ṹ�� ������ʱ����Ҫ���¿�Ƭ������Ϣ
int CDGTBusTrade::init_dgtCard_data(PDGTCard pdgtCard)
{
	bbzero(pdgtCard->cardType); //��Ƭ����
	bbzero(pdgtCard->cardInfo); //��Ƭ��Ϣ
	bbzero(pdgtCard->otherData);  //��Ƭ������Ϣ
	bbzero(pdgtCard->CItyCode);  //���б���
	bbzero(pdgtCard->compay_code); //�������б���
	bbzero(pdgtCard->busi_code);  
	bbzero(pdgtCard->deposit); //Ѻ��
	bbzero(pdgtCard->cardParent_type); //����
	bbzero(pdgtCard->cardChild_type);   //����
	bbzero(pdgtCard->openDate);   //����ʱ��
	bbzero(pdgtCard->effectiveDate); //��Ч��
	bbzero(pdgtCard->deviceInfo); //�豸��Ϣ
	bbzero(pdgtCard->appVer);   //��Ƭ�汾
	bbzero(pdgtCard->cardMny);  //��Ƭ���
	bbzero(pdgtCard->KEYVER);   //��Ƭ��Կ�汾
	bbzero(pdgtCard->ALGIND);    //��Ƭ�㷨
	bbzero(pdgtCard->cardShowId); //����
	bbzero(pdgtCard->CHARGE_DIVDATA);// ��ɢ����
	bbzero(pdgtCard->CARDSALE_DIVDATA); //��ɢ����
	bbzero(pdgtCard->CARDID); 
	bbzero(pdgtCard->CARDVERNO);  //���ں�
	bbzero(pdgtCard->EDCARDID);   
	bbzero(pdgtCard->connectCity);
	return 0;
}



//��ʼ����ݸͨM1����Ϣ
int CDGTBusTrade::init_dgtM1Card_data(PDGTM1Card pdgtM1Card)
{
	bbzero(pdgtM1Card->bus_card) ;//��������
	bbzero(pdgtM1Card->bus_amt);
	bbzero(pdgtM1Card->bus_card_type); //Ӧ���ӿ�����
	bbzero(pdgtM1Card->check_date) ;//��������
	bbzero(pdgtM1Card->psam); //psam����
	bbzero(pdgtM1Card->recogise_code); //��֤��
	bbzero(pdgtM1Card->dkq_code);
	bbzero(pdgtM1Card->card_parent_type);
	bbzero(pdgtM1Card->card_child_type);
	bbzero(pdgtM1Card->M1CardCSN);
	bbzero(pdgtM1Card->bus_showCard);
	bbzero(pdgtM1Card->city_code);
	bbzero(pdgtM1Card->cardVadate); //��Ч��
	bbzero(pdgtM1Card->appSerial); //Ӧ�ÿ����
	bbzero(pdgtM1Card->openDate) ; //��������
	bbzero(pdgtM1Card->openTime) ; //����ʱ��
	bbzero(pdgtM1Card->deposit) ; //Ѻ��

	return 0;
}


//��ݸ����ͨѶ����
int CDGTBusTrade::commuWithDGT(const TSDLLPosInfo dPosInfo, char *sndData, int sndLen, char *rcvData, int *rcvLen)
{
	int iRet=0;
	*rcvLen=0;

	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(DGTBUS_IP , DGTBUS_PORT , &sd);
	if (iRet != 0 )
	{
		return 1;
	}

	wrLog("��ݸͨ��������:%s", sndData);
	
	iRet = con.sndMsg(sd, sndData, sndLen);

	if( iRet!=0 )
	{
		return 1;
	}

	iRet = con.rcvMsg(sd, rcvData, rcvLen,1024);
	if( iRet!=0 )
	{
		closesocket(sd);
		return 2;
	}
	
	wrLog("�Ӷ�ݸͨ�������ݽ�������:%s",rcvData);

	closesocket(sd);

	if(*rcvLen == 0)
		return 1;

	return 0;
	
}


//type:�������� �޸ļ�¼���ͱ���
int CDGTBusTrade::dgt_trade(CGlobal *pgbl, char *tranMessage, char *tranType,char *rsp_code)
{
	int iRet;
	char sndData[1024]={0};
	char rcvData[1024] = { 0 };

	bbzero(sndData);
	bbzero(rcvData);

	int rcvLen;
	int sndLen=0;

	iRet=packdgtFunc(pgbl,tranMessage,tranType,sndData,&sndLen);	//���
	if (iRet!=0)
	{

		//��������
		return -1;

	}

	if (strcmp(tranMessage,"3460")==0  || strcmp(tranMessage,"3481") ==0 || strcmp(tranMessage,"2063")==0 || strcmp(tranMessage,"2480") ==0 || strcmp(tranMessage,"2481") ==0 \
		|| strcmp(tranMessage,"2062") ==0  ) //����ǿ���ɻ��߿����� �ۿ�����ֵ��ȷ�� �ڳ������������ʱ����Ҫ���汨������
	{
		
		CDB::setdgtResubmit(pgbl->Centerseqno,sndData); //resubmit 
		
	}

	for(int i=0;i<3;i++) //����ύ���ɹ������ü�¼��Ϣ��������״̬�������´ν���ҵ���ʱ��ȷ�ϡ�
	{
	
		iRet=commuWithDGT( pgbl->dPosInfo,sndData,sndLen,rcvData, &rcvLen); //ͨѶ //�����ΪͨѶ����û���յ����ذ���
		if (iRet!=0)
		{
			continue;
		}
		else
		{
			break;
		}

	}

	//�������ִ�з��أ�����ȷ�ϣ������ͻ�����ɣ���Ҫ��¼�����ݿ���ִ�е�����

	if(iRet!=0)
	{
		
		return -2;

	}

	//ͨѶ����

	 iRet=undgtpackFunc(pgbl,tranMessage,tranType,rcvData,rcvLen,rsp_code); //���
	 if (iRet!=0)
	 {
		 return -3; //�������
	 }

	 return 0;

}


//������
int CDGTBusTrade::cardManagement(CGlobal *pgbl, char *tranmessage, char *tranType,char *rsp_code,char * rsp_message)
{
	int iRet;
	char showMsg[500];
	bbzero(rsp_code);
	bbzero(rsp_message);
	bbzero(showMsg);

	iRet=dgt_trade(pgbl,tranmessage,tranType,rsp_code);
	if (iRet!=0)
	{
		//��������緱æ
		if (iRet== -2)
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"���緱æ�����Ժ�����\n����ϵ���߿ͷ����µ�400-80-11888��ѯ!");
			return -2;
		}
		if (strlen(rsp_code)==2)
		{
			
			iRet=getCommuErr(rsp_code,rsp_message);
			
			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"���緱æ�����Ժ�����\n����ϵ���߿ͷ����µ�400-80-11888��ѯ!");
				
			}

			
		}
		else if (strlen(rsp_code)==5)
		{
			//���ڱ����г���20002 ������ȫ�ɹ����ǳɹ�����˼
			if (strcmp(rsp_code,"20002") ==0 )
			{
				return 0;
			}
			else
			{
			
				iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
				if (iRet!=0)
				{
					bbzero(rsp_message);
					strcpy(rsp_message,"���緱æ�����Ժ�����!");
					
				}
			}
		
			
		}
		
		else
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"���緱æ�����Ժ�����!");
			
		}

		return -1; //���;ɿ���Ϣʧ��
		
	}
	
	return 0;
	
}


//������ɱ��� ���Զ���ύ��
int CDGTBusTrade::cardTransFinish(CGlobal *pgbl, char *tranmessage, char *tranType,char * rsp_code,char * rsp_message)
{
	int iRet;

	bbzero(rsp_code);
	bbzero(rsp_message);
	
	iRet=dgt_trade(pgbl,tranmessage,tranType,rsp_code);
	if (iRet!=0)
	{

		if (iRet==-2) //��������
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"���緱æ�����Ժ�����!");
			return -2;
		}

		if (strlen(rsp_code)==2)
		{
			
			iRet=getCommuErr(rsp_code,rsp_message);
			
			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"���緱æ�����Ժ�����!");
				
			}
			
		}
		else if (strlen(rsp_code)==5)
		{
			if (strcmp(rsp_code,"20002") ==0) //���ڱ����з���20002�Ľ��д��� �ǳɹ�����˼
			{
				return 0;
			}
			else
			{
				iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
				if (iRet!=0)
				{
					bbzero(rsp_message);
					strcpy(rsp_message,"���緱æ�����Ժ�����!");
					
				}

			}
		
			
		}
		
		else
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"���緱æ�����Ժ�����!");
			
		}
		
		return -1; //���;ɿ���Ϣʧ��
		
	}
	
	return 0;
	
}


//�ۿ����� 
int CDGTBusTrade::cardSaleTrade(CGlobal *pgbl,char *tranMessage,char *tranType)
{
	int iRet;
	char rsp_code[5+1];
	char rsp_message[500];
	int iSelect;
	
	bbzero(rsp_code);
	bbzero(rsp_message);
	
	iRet=dgt_trade(pgbl,tranMessage,tranType,rsp_code);

	if (iRet!=0)
	{
		if (iRet== -2)
		{
	       TS_OutPutMsg(pgbl->dPosInfo,"���緱æ�����Ժ�����!",1,iSelect);
		   return  -2;

		}
		if (strlen(rsp_code)==2)
		{
			
			iRet=getCommuErr(rsp_code,rsp_message);
			
			if (iRet!=0)
			{
				TS_OutPutMsg(pgbl->dPosInfo,"���緱æ�����Ժ�����!",1,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(pgbl->dPosInfo,rsp_message,1,iSelect);
				
			}
			
		}
		else if (strlen(rsp_code)==5)
		{
			iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
			if (iRet!=0)
			{
				TS_OutPutMsg(pgbl->dPosInfo,"���緱æ�����Ժ�����!",1,iSelect);
				
			}
			else
			{
				//������ȫ�ɹ������ǳɹ�
				if (strcmp(rsp_code,"20002") ==0 ) //���ڿ��������г��ֱ�����ȫ�ɹ�����������ǳɹ�������Ҫ�ٷ���
				{
					return 0;

				}
				else
				{
					TS_OutPutMsg(pgbl->dPosInfo,rsp_message,1,iSelect);
					return -1;
				}
				
			
			}
			
		}
		
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo,"���緱æ�����Ժ�����!",0,iSelect);
			
		}

		return -1; //���;ɿ���Ϣʧ��
		
	}
	
	return 0;
	
}

//��ݸͨ����

int CDGTBusTrade::dgtCardOperate(CGlobal *pgbl,char *tranMessage,char *transType,char *rsp_code,char *rsp_message)
{
	
	int APDU_LEN=0;
	int iRet;
	char openDate[8+1];
	char effectiveDate[8+1];
	char errMsg[200];
	char APDU[70];
	bbzero(APDU);
	
	bbzero(errMsg);
	bbzero(rsp_message);
	bbzero(effectiveDate);
	bbzero(rsp_code);
	
	strcpy(effectiveDate,"20901231");
	bbzero(pgbl->dgtCard.effectiveDate);
	CTools::hex_to_byte((BYTE *) effectiveDate,(BYTE *) pgbl->dgtCard.effectiveDate,8);
	//���ÿ�������
	CTools::getDate(openDate);

	if (strcmp(transType,"80") ==0  || (strcmp(transType,"52")==0)) //������ǳ�ֵ ��Ƭ���׽��Ϊ0
	{
		//����ǿ���
		CTools::hex_to_byte((BYTE *) openDate,(BYTE *) pgbl->dgtCard.openDate,8);

		//���÷�����д��� ֮ǰ��װAPDU����
		bbzero(pgbl->dgtCard.APDU);
		bbzero(APDU);
		//���
		bbzero(pgbl->dgtCard.tradeBeForeMny);
		bbzero(pgbl->dgtCard.tradeMny);
		memcpy(pgbl->dgtCard.tradeBeForeMny,pgbl->dgtCard.cardMny+4,8);
		strcpy(pgbl->dgtCard.tradeMny,"00000000");


		memcpy(APDU+APDU_LEN,"\x04\xd6\x95\x08\x18",5);
		APDU_LEN+=5;
		memcpy(APDU+APDU_LEN,"\x01",1); //��ͨ��Ƭ
		APDU_LEN+=1;
		memcpy(APDU+APDU_LEN,pgbl->dgtCard.appVer,1);
		APDU_LEN+=1;
		memcpy(APDU+APDU_LEN,pgbl->dgtCard.connectCity,2);
		APDU_LEN+=2;
		memcpy(APDU+APDU_LEN,pgbl->dgtCard.appSerial,8); //�����к�
		APDU_LEN+=8;
		CTools::hex_to_byte((BYTE *)openDate,(BYTE *)APDU+APDU_LEN,8); //����ʱ��
		APDU_LEN+=4;
		CTools::hex_to_byte((BYTE *)effectiveDate, (BYTE *)APDU+APDU_LEN,8);
		APDU_LEN+=4;
		
		CTools::byte_to_hex((BYTE *) APDU,(BYTE *)pgbl->dgtCard.APDU,APDU_LEN);
		wrLog("APDU_COMMAND:%s",pgbl->dgtCard.APDU);
		
		//ȡ��Ƭ�����
		bbzero(pgbl->dgtCard.CARDRAND);
		iRet=getRandom(pgbl->dPosInfo,errMsg,pgbl->dgtCard.CARDRAND); //ȡ�����
		wrLog("�ۿ�ȡ��Ƭ�����Ϊ:%s",pgbl->dgtCard.CARDRAND);
		
		if (iRet!=0)
		{
			//TS_OutPutMsg(pgbl->dPosInfo,"����ʧ��",1,iSelect);
			bbzero(rsp_code);
			bbzero(rsp_message);
			strcpy(rsp_code,"00002");
			strcpy(rsp_message,"������ȡ�����ʧ��");
			return -1; //ȡ�����ʧ��
		}

	}

	//��ʼ��װ�����������ġ�
	iRet=dgt_trade(pgbl,tranMessage,transType,rsp_code);


	if (iRet!=0)
	{
		if (iRet==-2)
		{	
			bbzero(rsp_message);
			strcpy(rsp_message,"���緱æ�����Ժ�����!");
			return -2;

		}
		//����rsp_code ��ѯ�������Ʒ���
		if (strlen(rsp_code)==2)
		{

			iRet=getCommuErr(rsp_code,rsp_message);

			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"���緱æ�����Ժ�����!");
				
			}
		}
		else if (strlen(rsp_code)==5)
		{
			iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"���緱æ�����Ժ�����!");

			}

		}

		else

		{
			bbzero(rsp_message);
			strcpy(rsp_message,"���緱æ�����Ժ�����!");

		}

		return -1;
		
	}

	
	return 0;
	
}

int CDGTBusTrade::getCommuErr(char *rsp_code,char *rsp_message)
{
	bbzero(rsp_message);

	switch (atoi(rsp_code))
	{
		case 0x01:
			strcpy(rsp_message,"��Ч�ļ���ѹ���㷨");
			break;
		case 0x02:
			strcpy(rsp_message,"��Ч����Ϣ������");
			break;
		case 0x03:
			strcpy(rsp_message,"��Ч����Ϣ�汾");
			break;
		case 0x04:
			strcpy(rsp_message,"���ĳ�����Ч");
			break;
		case 0x05:
			strcpy(rsp_message,"ͨѶMAC��");
			break;
		case 0x06:
			strcpy(rsp_message,"ͨѶ��ʱ");
			break;
		case 0x07:
			strcpy(rsp_message,"ͨѶĿ��ڵ㲻�ɵ���");
			break;
		case 0x08:
			strcpy(rsp_message,"���ĸ�ʽ����");
			break;
				
		case 0x09:
			strcpy(rsp_message,"����Ա������");
			break;
		case 0x0A:
			strcpy(rsp_message,"����Ա�������");
			break;
		case 0x0B:
			strcpy(rsp_message,"POS���Ų�����");
			break;
		case 0x0C:
			strcpy(rsp_message,"POS���ѽ���");
			break;
		case 0x0D:
			strcpy(rsp_message,"SAM���Ų�����");
			break;
		case 0x0E:
			strcpy(rsp_message,"SAM�����ѽ���");
			break;
		case 0x12:
			strcpy(rsp_message,"����Ա��Ч����SAM��");
			break;
		case 0x13:
			strcpy(rsp_message,"SAM����ED��Ӧ��ϵ��");
			break;
		case 0x14:
			strcpy(rsp_message,"SAM����δǩ��");
			break;
		case 0x15:
			strcpy(rsp_message,"SAM����δǩ��");
			break;
		case 0x16:
			strcpy(rsp_message,"��һ�ճ�ֵ����δ�������");
			break;
		case 0x17:
			strcpy(rsp_message,"ǩ��Ӧ��д�����ʧ��");
			break;
		case 0x18:
			strcpy(rsp_message,"ǩ��Ӧ��д�����ʧ��");
			break;
		case 0x19:
			strcpy(rsp_message,"�ļ����Ѵ���");
			break;
		case 0x1A:
			strcpy(rsp_message,"�ļ���/��дʧ��");
			break;
		case 0x1B:
			strcpy(rsp_message,"�ļ�д��ʧ��");
			break;
		case 0x1C:
			strcpy(rsp_message,"�ļ�������");
			break;
		case 0x1D:
			strcpy(rsp_message,"��Ч�Ĳ�������");
			 break;
		case 0x1E:
			strcpy(rsp_message,"����δ����");
			break;
		case 0x1F:
			strcpy(rsp_message,"�û����Ų�����");
			break;
		case 0x20:
			strcpy(rsp_message,"�û������ѽ���");
			break;
		case 0x21:
			strcpy(rsp_message,"�û�����֤��Ϣ�Ƿ�");
			break;
		case 0x22:
			strcpy(rsp_message,"�������˿�");
			break;
		case 0x23:
			strcpy(rsp_message,"����δ����Ĵ���");
			break;
		case 0x24:
			strcpy(rsp_message,"����δ����");
			break;
		case 0x25:
			strcpy(rsp_message,"��Ч�Ľ���������");
			break;
		case 0x26:
			strcpy(rsp_message,"�����汾����");
			break;
		case 0x27:
			strcpy(rsp_message,"�����汾������");
			break;
		case 0x50:
			strcpy(rsp_message,"ϵͳ����");
			break;
		default:
			return -1;
	}

	return 0;

}


int CDGTBusTrade::cardTrans_OpenCard(CGlobal *pgbl,char *rsp_code,char *rsp_message)
{

	int iRet;
	int iRet1;
	int iSelect;
	int cardStatus_iRet;
	int openCardStatus=0;
	int readCount=0;

	char errMsg[200];
	bbzero(errMsg);
	int read_iRet;

	//����ط������޸� �����ظ��������� ���ڳ��ֿ�״̬����Ĵ�������д��������û�������������
	for (int i=0;i<3;i++)
	{

		//���¶���
		if (readCount >0) //��һ�β���Ҫ���� ��Ϊ�ں�������Ѿ��ȹ�
		{

			memset(&pgbl->dgtCard,0,sizeof(pgbl->dgtCard));
			read_iRet=dgtCardInqHdl(pgbl->dPosInfo,&pgbl->dgtCard,2); //��ѯ��Ƭ������Ϣ
			if (read_iRet!=0) 
			{
				CDB::updateTranChargeProcess(pgbl,"C01");
				wrLog("��ȡ��ݸͨ��Ƭ��ѯ����"); //�����쳣����
				TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��ݸͨ��Ƭ����,����ʧ�� �����ҵ������\n��������,����ѯ���߿ͷ����µ�ͷ��绰400-80-11888",1,iSelect);
				return CANCLE_KEY;
				
			}
		}
		
		iRet=dgtCardOperate(pgbl,"2061","52",rsp_code,rsp_message); //���������� 

		if (iRet!=0)
		{
			//�жϿ�����״̬�Ƿ��ǿ�״̬����
			if (strcmp(rsp_code,"21014") ==0 )
			{
				TS_OutPutMsg(pgbl->dPosInfo,"�뻻�Ŷ�ݸͨ�׿� ���ڶ�����������",1,iSelect);
				if (iSelect!=0)
				{
					return CANCLE_KEY;
				}
				else
				{
					readCount++;
					continue;

				}

			}
			else
			{
			
				break;
			}
		}
		else
		{
		
			break;
		}
	}

	if (iRet!=0) // 
	{

		//����ʧ��
		CDB::updateTranChargeProcess(pgbl,"C01");
		
		if (iRet== -2) //�����Ϊ����ԭ�����𻻿�ʧ��
		{

			bbzero(errMsg);
			sprintf(errMsg,"\n����ʧ�ܣ�%s",rsp_message);

		}
		else
		{
			//����ʧ��
			bbzero(errMsg);
			sprintf(errMsg,"\n����ʧ�ܣ�%s \n�뻻�Ŷ�ݸͨ�׿� �����ҵ������",rsp_message);

		}

		TS_OutPutMsg(pgbl->dPosInfo,errMsg,1,iSelect); 
		return -1;
		
	}
	
	//�����Ƭdicmac ��Ϣ
	//���п��� ���������ֻ����һ��
	//�����Ŀ���������ɹ�֮�󣬷�����ҵ�˻����ɶ���

	iRet=CDGTBusTrade::openCard(pgbl->dPosInfo,pgbl->dgtCard.DICMAC,errMsg,pgbl->dgtCard); //����ʧ�ܵ����Ҫ

	if (iRet!=0) //�������ɹ������г���
	{
	
		//���ζ�ȡ��Ƭ��״̬��Ϣ
		for (int i=0;i<3;i++)
			
		{
			//��ȡ��Ƭ������Ϣ��
			cardStatus_iRet=dgtCardInqHdl(pgbl->dPosInfo,&pgbl->dgtCard,1); //��ѯ��Ƭ������Ϣ
			if (cardStatus_iRet==0)
			{
				//��ȡ��Ƭ��Ϣ
				if (memcmp(pgbl->dgtCard.appFlag,"\x01",1)!=0 ) //��Ƭ����ɹ�

				{
						
					CDB::updateTranChargeProcess(pgbl,"C01");
					TS_OutPutMsg(pgbl->dPosInfo,"����ʧ�ܣ��뻻�Ŷ�ݸͨ�׿� �����ҵ������!",1,iSelect);
					return -2;
				}

					
					break;

			}

		}

		 //������������У������ڶ�ȡ��Ƭ�Ĺ�����Ҳ���ֳ�ʱ��
		if (cardStatus_iRet !=0) 
		
		{
				
			CDB::updateTranChargeProcess(pgbl,"C0T");
			TS_OutPutMsg(pgbl->dPosInfo,"������ʱ ������Ծɻ���ҵ������!",1,iSelect);
			return -5;
		}
	
	}

	//��������ɹ�
	wrLog("�����ɹ�");
	bbzero(pgbl->dgtCard.tradeBeForeMny);
	memcpy(pgbl->dgtCard.tradeBeForeMny,pgbl->dgtCard.cardMny+4,8);
	//����ύ���ɹ��������ύ�������ύ�����ɹ�����������
	CDB::updateTranChargeProcess(pgbl,"C00");

	iRet1=cardSaleTrade(pgbl,"2063","52");  // 52   �ύ������Ϣ �Դ�������ȫ�ظ������жϡ���ȫ�ظ�����Ҫ�ٷ���ȷ����Ϣ

	if (iRet1!=0)
	{
		CDB::updateTranChargeProcess(pgbl,"C0S"); //����ȷ��
		TS_OutPutMsg(pgbl->dPosInfo,"�����ɹ���ȷ��ʧ�ܣ�������Ծɻ���ҵ����н���ȷ��\n �������ʣ�����ѯ���߿ͷ������µ�ͷ��绰400-80-11888",1,iSelect);
		return -3;

	}
	
	CDB::updateTranChargeProcess(pgbl,"C00");
	TS_OutPutMsg(pgbl->dPosInfo,"�����ɹ�,�밴��Enter����������",1,iSelect);

	return 0;

}


//��ȡ��Ƭ��� 	//��ѯ���ͣ�flag 1 ��ѯ���Ȼ�����Ϣ 0 ֻ��ѯ������ڽ�����ʾ ��3���׹����в�ѯ��� �������
//4ֻ�ڿ�����ʱ�����
int CDGTBusTrade::dgtCardInqHdl(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,int flag) //��ȡ��Ƭ������Ϣ

{
	int iRet;
	int iSelect;
	char errMsg[255];
	char reader_rsp[4+1];
	bbzero(reader_rsp); //�������������صĴ�����
	bbzero(errMsg);
		//��ʼ����ݸͨ��
		
	if (flag!=3) //���׹����и��¿�Ƭ������ʼ��
	
	{
	     init_dgtCard_data(pdgtCard); //��ʼ����Ƭ��Ϣ
			
	}
		
		//���ζ���
		for(int i=0;i<3;i++)
		{
			
			iRet=cardQuery(dPosInfo,reader_rsp,errMsg,pdgtCard,flag);
			if (iRet!=0) //�����ȡ����
			{
				
				bbzero(errMsg);
				strcpy(errMsg,"��ѯ������ʧ�ܣ��뽫�����ڶ�����������\n �밴��Enter������");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else //�����ɹ�ֱ���˳�Ѱ��
			{
				
				break;
			}
			
		}
		
		if (iRet!=0) //���ζ�����ʧ��
		{
			return -1;
		}
		
		
		//��ʾ��Ƭ��
		char showMsg[200];
		char showLEDMsg[500];
		char cardMny[12+1];
		
		bbzero(cardMny);
		bbzero(showMsg);
		bbzero(showLEDMsg);
		
		strcpy(cardMny,pdgtCard->cardMny);
		sprintf(showLEDMsg,"���:%.2fԪ",atof(cardMny)/100);
		wrLog("��Ҫ��ʾ����Ϣ:%s",showLEDMsg);
		
		if (flag==0) //���ֻ�ǲ�ѯ ��Ҫ�ڽ���������ʾ���
		{
			
			bbzero(errMsg);
			iRet=CDGTBusTrade::showLEDMsg(dPosInfo,showLEDMsg,errMsg);
			
			if (iRet!=0)
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;
			}
			
			sprintf(showMsg,"��Ƭ���:%.2fԪ �밴��Esc������",atof(cardMny)/100);
			TS_OutPutMsg(dPosInfo,showMsg,1,iSelect);
		}
		
		else
		{
			
			return 0;
			
		}
		
		return 0;
		
}

//ת�ƿ�Ƭ���

int CDGTBusTrade::trans_card_Amt(CGlobal *pgbl,char *rsp_code,char *rsp_message)
{

	char errMsg[200];
	int iRet;
	char cardMNY[8+1];
	bbzero(cardMNY);
	bbzero(errMsg);
	int iRet2;
	int write_iRet;

	//����cpu��ʼ��
	//У��pin2
	char appSerial[16+1];
	bbzero(appSerial);
	CTools::byte_to_hex((BYTE *) pgbl->dgtCard.appSerial,(BYTE *)appSerial,8);
	iRet=CDGTBusTrade::checkPIN2(pgbl->dPosInfo,appSerial,errMsg);
	if (iRet!=0)
	{
		//����������
		bbzero(rsp_code);
		strcpy(rsp_code,"00001");
		bbzero(rsp_message);
		strcpy(rsp_message,errMsg);

		return -1;
	}

	iRet=initCPUCard(pgbl->dPosInfo,pgbl->dgtSignInfo.POSID,pgbl->dgtCard.tradeMny,pgbl->dgtCard.CARDSEQ,pgbl->dgtCard.KEYVER,pgbl->dgtCard.ALGIND,pgbl->dgtCard.CARDRAND,cardMNY,pgbl->dgtCard.MAC1,errMsg);

	if (iRet!=0)
	{
		//����������
		bbzero(rsp_code);
		strcpy(rsp_code,"00002"); //�����������
		bbzero(rsp_message);
		strcpy(rsp_message,errMsg);
		return -1;
	}

	bbzero(pgbl->dgtCard.cardMny);
	CTools::HexToDec(cardMNY,pgbl->dgtCard.cardMny);

	//���ͻ�����ɿ��������� ��ǿ���ⲿ�ֵ��жϣ���ֹ�����ظ���ֵ�������

	iRet=dgtCardOperate(pgbl,"2061","53",rsp_code,rsp_message); //���ͻ�����ɲ��������� transtype=53,cardtype=3460
	if (iRet!=0)
	{
		//����ʧ��

		return -2;
	
	}

	//����Ȧ��ֻ�ܹ�һ�Σ������ֻ��ʹ��һ��

	write_iRet=cpuCard_load(pgbl->dPosInfo,pgbl->dgtCard.MESSAGEDATETIME,pgbl->dgtCard.MAC2,pgbl->dgtCard.TAC,errMsg); 
	//����������������仯������Ȧ�淵�سɹ������׳ɹ���//�Է��س�ʱ����������ر���

	//�ٴβ�ѯ��� ��Ҫ���ζ�ȡ��Ƭ�����Ϣ

	iRet2 = dgtCPUInqHdl(pgbl->dPosInfo,&pgbl->dgtCard,3); //��Ƭ��ֵ�����и�������ѯ����
	
	/************************************************************************/
	/* ��ֵ�ɹ����ȡ��Ƭ��Ϣ�������������Ϣ                               */
	/************************************************************************/
	if (iRet2==0)
	{
		bbzero(pgbl->dgtCard.tradeAfterMny);
		memcpy(pgbl->dgtCard.tradeAfterMny,pgbl->dgtCard.cardMny+4,8);
		
		wrLog("�ȶԳ�ֵǰ����");
		wrLog("��ֵǰ���:%d",atoi(pgbl->dgtCard.tradeBeForeMny));
		wrLog("��ֵ�����:%d",atoi(pgbl->dgtCard.tradeAfterMny));
		wrLog("���׽��:%d",atoi(pgbl->dgtCard.tradeMny)); 
	}

	//�����������
	//д����ʱ������������ζ�����ʧ�ܵ����
	//д���ɹ������ζ�����ʧ�ܵ������

	//��ѯ���
	if (write_iRet==0 || ( iRet2==0 && (atoi(pgbl->dgtCard.tradeAfterMny)==atoi(pgbl->dgtCard.tradeMny)+atoi(pgbl->dgtCard.tradeBeForeMny)) ))//�ɹ�
	{
		//�������� ������سɹ����߶������ɹ�
		
		iRet=CDGTBusTrade::wrTxnLogs(pgbl->dPosInfo,pgbl->dgtSignInfo.POSID,pgbl->dgtCard,"02",errMsg);
		if (iRet!=0)
		{
			wrLog("д����¼ʧ��");
		}
		
		//������ ��ʾ��Ϣ
		char ledMsg[100];
		char errMsg[200];
		bbzero(errMsg);
		bbzero(ledMsg);
		if (write_iRet==0 && iRet2!=0) //�����������������д���ɹ����Ƕ���ʧ�ܵ������Ϊ�ǳɹ�����
		{
			int showMny; //���¿�Ƭ���͸��½��׺����
			showMny=atoi(pgbl->dgtCard.tradeBeForeMny)+atoi(pgbl->dgtCard.tradeMny);
			bbzero(pgbl->dgtCard.tradeAfterMny);
			sprintf(pgbl->dgtCard.tradeAfterMny,"%08d",showMny);
			bbzero(pgbl->dgtCard.cardMny);
			sprintf(pgbl->dgtCard.cardMny,"%012d",showMny);
			sprintf(ledMsg,"���:%.2fԪ",atof(pgbl->dgtCard.cardMny)/100);
		}
		else
		{
			sprintf(ledMsg,"���:%.2fԪ",atof(pgbl->dgtCard.cardMny)/100);
		
		}

		showLEDMsg(pgbl->dPosInfo,ledMsg,errMsg);
		buzzer(pgbl->dPosInfo,errMsg);

		return 0;
	}
	else  //Ȧ�淵��ʧ�ܡ����Ҷ���Ҳʧ��
	{
		//�����ֵ��ʱ�����Ҷ���ʧ�ܵ�������д�����ʾ��ֵ���δ֪������ϵ���߿ͷ�����ʱ���ݿ��е�ʧ�ܴ���
		if (write_iRet==-1 && iRet2 !=0)
		{
		    //��ʱ���ɹ�
			int showMny; //���¿�Ƭ���͸��½��׺����
			showMny=atoi(pgbl->dgtCard.tradeBeForeMny)+atoi(pgbl->dgtCard.tradeMny);
			bbzero(pgbl->dgtCard.tradeAfterMny);
			sprintf(pgbl->dgtCard.tradeAfterMny,"%08d",showMny);
			bbzero(pgbl->dgtCard.cardMny);
			sprintf(pgbl->dgtCard.cardMny,"%012d",showMny);

			return 1;
		}
		else
		{
		
			return -4; //����ʧ��
		}

		
	}
	
	return 0;

}


//CPU����
//��ѯ���ͣ�flag 1 ��ѯ���Ȼ�����Ϣ 0 ֻ��ѯ��� ��3���׹����в�ѯ���

int CDGTBusTrade::dgtCPUInqHdl(TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,int flag) //��ȡ��Ƭ������Ϣ
{

	int iRet;
	int iSelect;
	char errMsg[255];
	char reader_rsp[4+1];
	bbzero(reader_rsp); //�������������صĴ�����
	bbzero(errMsg);
	//��ʼ����ݸͨ��
	
	if (flag!=3) //���׹����и��¿�Ƭ������ʼ��
	{
		init_dgtCard_data(pdgtCard); //��ʼ����Ƭ��Ϣ
		
	}
	
	
	//���ζ���
	for(int i=0;i<3;i++)
	{
		
		iRet=cardQuery(dPosInfo,reader_rsp,errMsg,pdgtCard,flag);
		if (iRet!=0) //�����ȡ����
		{
			if (strcmp(reader_rsp,"3005")==0 || strcmp(reader_rsp,"3006")==0 ) //�������д����ʱ�����
			{
				
				bbzero(errMsg);
				strcpy(errMsg,"��ѯ������ʧ�ܣ��뽫�����ڶ�����������\n �밴��Enter������");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;
				
			}
			
			
		}
		else //�����ɹ�ֱ���˳�Ѱ��
		{
			
			break;
		}
		
	}
	
	//��ʾ��Ƭ��
	char showMsg[200];
	char showLEDMsg[500];
	char cardMny[12+1];
	
	bbzero(cardMny);
	bbzero(showMsg);
	bbzero(showLEDMsg);
	
	strcpy(cardMny,pdgtCard->cardMny);
	sprintf(showLEDMsg,"���:%.2fԪ",atof(cardMny)/100);
	wrLog("��Ҫ��ʾ����Ϣ:%s",showLEDMsg);
	
	if (flag==0) //���ֻ�ǲ�ѯ
	{
		
		bbzero(errMsg);
		iRet=CDGTBusTrade::showLEDMsg(dPosInfo,showLEDMsg,errMsg);
		
		if (iRet!=0)
		{
			TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
			return -1;
		}
		
		sprintf(showMsg,"��Ƭ���:%.2fԪ �밴��Esc������",atof(cardMny)/100);
		TS_OutPutMsg(dPosInfo,showMsg,1,iSelect);
	}
	
	else
	{
		
		return 0;
		
	}
	
	return 0;
	
}


