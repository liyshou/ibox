#include "YCT3_Purchase.h"

/****************���۷�����*********************/
int CYCTPurchase::Purchase_Handle(CGlobal *pgbl,PYCT_DATA pyct_data,BYTE *app_type,char *purchase_amo,BYTE *extension_stan,BYTE* party, BYTE* rspcode)
{
	int iRet,iSelect = 0;
	int com=0, comType=0;
	BYTE pysical_info [51]; //��Ƭ������Ϣ
	BYTE card_pycicalInfo1[32+1];//��Ƭ������Ϣ1
	BYTE SW1SW2[2];
	BYTE SAK[1];
	CPack pack;	
	BYTE query_info[96+1];//��ѯ��Ϣ
	BYTE wallet_type[1+1];//Ǯ������
	BYTE count_info[16+1];//������Ϣ
	BYTE card_appinfo[40+1];//��Ӧ����Ϣ
	BYTE consume_tradeinfo[64+1];//���ѽ�����Ϣ
	BYTE consume_info[144+1];//������Ϣ
	BYTE status[1+1];
	BYTE consume_cmd[48+1];//����ָ��
	BYTE MAC2[4+1];
	BYTE ReaderStatus[1+1];//����������״̬
	int try_times = 1;
	BYTE followerup_oper[1+1];//��������
	char ticket_no[16+1];
	char show_ticket_no[16+1];

	bbzero(ticket_no);
	bbzero(show_ticket_no);
	
	wrLog("��Purchase_Handle��");

	bbzero(query_info);
	bbzero(count_info);

	
// 	iRet = CTrade::getComPar(pgbl->posInfo.leftType, pgbl->posInfo.rightType, pgbl->posInfo.inType, &com, &comType);
// 	if( iRet==0 )
// 	{
// 		NULL;
// 	}
// 	else
// 	{
// 		*party = FROMCTSI;
// 		*rspcode = COMNOTFOUND;
// 		return 5;// 5���۷�ǰʧ��
// 	}
// 	
// 	if((pgbl->posInfo).rightType == 0x08)	//���������
// 	{
// 		//memset(pyct_data,0,sizeof(pyct_data));
// 		comType = 0x05;
// 		iRet=CYCTYPTrade::ST_initReader3(pgbl->dPosInfo,com,comType,pyct_data); //������������������ͨ��ֵ��������ʼ��
// 		if (CYCTYPTrade::yct_SignIn(pgbl->dPosInfo,com,comType,pyct_data,party,rspcode)==0)
// 		{
// 			wrLog("ǩ���ɹ�");
// 		}
// 		
// 		if(iRet)
// 		{
// 			*party = FROMCTSI;
// 			*rspcode = CONNCOMFAIL;
// 			return 5;
// 		}
// 	}
// 	//��ʱ������������������
// 	else
// 	{
// 		TS_OutPutMsg(pgbl->dPosInfo, "��ЧӲ�����޷��������ף����������ã�������������ϵ�����׿ͷ�", 0, iSelect);
// 		*party = FROMCTSI;
// 		*rspcode = CONNCOMFAIL;
// 		return 5;
// 	}
    com = pgbl->com;
	comType = pgbl->comType;

	//R_PUB_QRY_PHYSICS_INFO()
	//.�ն�����������·��Ͳ�ѯ��������Ϣ��ָ��
	iRet=CYCTYPTrade::R_PUB_QRY_PHYSICS_INFO(pgbl->dPosInfo,com,comType, pysical_info);
	
	if(iRet==0)
	{
		bbzero(SW1SW2);
		CTools::byte_to_hex((BYTE *)pysical_info,(BYTE *)pyct_data->ticket_physicalNo,8);//��ȡ������
		CTools::byte_to_hex((BYTE *)pysical_info+8,(BYTE *)pyct_data->ticket_no,8);//ȡ���߼�����
		memcpy(SAK,pysical_info+16,1); //sSAK
		bbzero(card_pycicalInfo1);
		memcpy(card_pycicalInfo1,pysical_info+17,32); //��Ƭ������Ϣ1
		CTools::byte_to_hex(pysical_info+49,SW1SW2,2);
		
		BYTE temp_SW1SW2[2+1];
		bbzero(temp_SW1SW2);
		memcpy(temp_SW1SW2,pysical_info+49,2);
		
		wrLog("SW1SW2:%s",SW1SW2);
		if (memcmp(SW1SW2,"9000",4) != 0)
		{
			char errMsg[512];
			char outPut_Msg[512];
			get_cardStatus_errMsg((BYTE *)temp_SW1SW2,errMsg);
			bbzero(outPut_Msg);
			sprintf(outPut_Msg,"������Ӧ��״̬�����%s",errMsg);
			TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1�������������ʧ��
		}

		wrLog("������:%s",pyct_data->ticket_physicalNo);
		//wrLog("�߼�����:%s",pyct_data->ticket_no);
		CTools::shildWords(pyct_data->ticket_no,7,5,ticket_no);
		wrLog("�߼�����:%s",ticket_no);

		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32);	
	}
	else if (iRet != 0)
	{
		return 1;// 1�������������ʧ��
	}

	//P_PURCHASE_QRY();
	iRet = P_PURCHASE_QRY(pgbl->dPosInfo, com, comType,pyct_data,app_type,purchase_amo,card_pycicalInfo1,extension_stan,consume_tradeinfo);
	if (iRet != 0)
	{
		*party = FROMCTSI;
		*rspcode = PURCHASE_FAIL;
		return 1;
	}

	//R_PURCHASE_INIT();
	iRet = R_PURCHASE_INIT(pgbl->dPosInfo, com, comType,pyct_data,consume_tradeinfo,consume_info,status);

	//P_PURCHASE()
	iRet = P_PURCHASE(pgbl->dPosInfo, com, comType,pyct_data,consume_info,status,consume_cmd);

	//R_PURCHASE()
	iRet = R_PURCHASE(pgbl->dPosInfo, com, comType,pyct_data,consume_cmd,wallet_type,MAC2);

	//R_PUB_QRY_APP_INFO
	while (try_times <= 3)
	{
		iRet = CYCTYPTrade::R_PUB_QRY_APP_INFO(pgbl->dPosInfo, com, comType,pyct_data,card_appinfo,ReaderStatus,1);
		if (iRet == 0)
		{
			break;
		}
		if (iRet == 2)
		{
			*party = FROMIC;
			*rspcode = PURCHASE_FAIL;
			break;// 
		}
		try_times++;
	}

	if (ReaderStatus[0] == 0x00)
	{
		//�Աȿ۷�ǰ������ֵ�Ƿ���ȷ
		iRet = balance_compare(pyct_data,purchase_amo);
		if (iRet == 0)
		{
			wrLog("�۷ѳɹ�");
			bbzero(pyct_data->out_balance_aft);
			memcpy(pyct_data->out_balance_aft,pyct_data->ticket_amo_aft,12);
			//��ӡ�۷ѳɹ�����
		}
		else
		{
			wrLog("�۷�ʧ��");
		}
	}

	//P_PURCHASE_SUBMIT
	try_times = 1;
	while (try_times <= 3)
	{
		iRet = P_PURCHASE_SUBMIT(pgbl->dPosInfo, com, comType,pyct_data,wallet_type,MAC2,ReaderStatus,card_appinfo,followerup_oper);
		if (iRet == 0)
		{
			break;
		}
		try_times++;
	}
	
	if (followerup_oper[0] != 0x00)
	{
		//������ز�������Ҫִ����չӦ�ò���
		*party = FROMSUCC;
		*rspcode = SUCC_NEEDEXAPPOPR;	
	}
	else
	{
		*party = FROMSUCC;
		*rspcode = SUCC_PURCHASE;
	}
	return 0;
}


int CYCTPurchase::P_PURCHASE_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
			BYTE *app_type,char *purchase_amo,BYTE *card_pycicalInfo1,BYTE *extension_stan,BYTE *consume_tradeinfo)
{
	int iRet=0, len=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE RFU[64+1];
	BYTE rsp_code[4+1];
	BYTE temp[20+1];
	char Output_Msg[128];
	int iSelect = 0;
	char show_ticketno[16+1];
	bbzero(show_ticketno);
	
	wrLog("��P_PURCHASE_QRY��");

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);

	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki����
	len+=4;

	//memcpy(sndData+len,(BYTE *)pyct_data->ticket_physicalNo,8);	 //������
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,sndData+len,16);
	wrHex("������: ",sndData+len,8);
	len+=8;

	//memcpy(sndData+len,(BYTE *)pyct_data->ticket_no,8);	 //�߼�����
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,sndData+len,16);
//	wrHex("�߼�����: ",sndData+len,8);
	len+=8;

	
	memcpy(sndData+len,app_type,1);	 //Ӧ������
	wrHex("Ӧ������: ",sndData+len,1);
	len+=1;

	if (app_type[0] == 0x01 || app_type[0] == 0x02)
	{
		CTools::getHexAmo("000000000000",(BYTE *)sndData+len);
	}
	else
	{
		CTools::getHexAmo(purchase_amo,(BYTE *)sndData+len);
	}
	//CTools::hex_to_byte((BYTE *)trans_amo,8);
	wrHex("�۷ѽ��: ",sndData+len,4);//�۷ѽ��
	len+=4;

	memcpy(sndData+len,(BYTE *)card_pycicalInfo1,32);	 //��������Ϣ1
	wrHex("��������Ϣ1: ",sndData+len,32);
	len+=32;

	if (app_type[0] == 0x01 || app_type[0] == 0x02)
	{
		memcpy(sndData+len,(BYTE *)extension_stan,8);	 //��չӦ����ˮ
	}
	else
	{
		memcpy(sndData+len,(BYTE *)0x00,8);	 //��չӦ����ˮ
	}
	wrHex("��չӦ����ˮ: ",sndData+len,8);
	len+=8;

	memcpy(sndData+len,(BYTE *)RFU,39);	 //RFU
	wrHex("RFU: ",sndData+len,39);
	len+=39;

	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E011" , (BYTE*)"0001" , 0x02 , 0x80 , \
		sndData , len , rcvData , &rcvLen);
	
	len = 8;
    wrHex("������ˮ��: ",rcvData + len,8);
	len+=8;
	memcpy(pyct_data->yct_stan,rcvData + len,8);//���ں����۷Ѳ�������չӦ����ˮ
	wrHex("����ϵͳ��ˮ��: ",rcvData + len,8);
	len+=8;
	//memcpy(rsp_code,rcvData + len,4)
	CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4);//������
	wrHex("������: ",rcvData + len,4);
	len+=4;
	
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("���׷�����:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//������Ϣ����
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("������Ϣ����: %s",byte_errinfo);	
		bbzero(Output_Msg);
		strcat(Output_Msg,byte_errinfo);
		TS_OutPutMsg(dPosInfo,byte_errinfo,1,iSelect);
		return 1;
	}
	
	bbzero(consume_tradeinfo);
	memcpy(consume_tradeinfo,rcvData + len,64);
	wrHex("���ѽ�����Ϣ: ",rcvData + len,64);

	return 0;
}

//�۷ѳ�ʼ��
//in: consume_tradeinfo - ���ѽ�����Ϣ
//out: consume_info - ������Ϣ, status - ������������
int CYCTPurchase::R_PURCHASE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *consume_tradeinfo,BYTE *consume_info,BYTE *status)
{
	int buflen=0 , len = 0 , iRet = 0,tmp_len = 0;
	BYTE inBuf[256+1];
	BYTE outBuf[256+1];
	BYTE trade_sequence[2+1];
	
	wrLog("��R_PURCHASE_INIT��");
	
	bbzero(inBuf);
	bbzero(outBuf);

	memcpy(inBuf,consume_tradeinfo,64);
	wrHex("���ѽ�����Ϣ: ",consume_tradeinfo,64);
	buflen += 64;
	
	iRet = CYCTYPTrade::R_COMMAND(dPosInfo , com , comType , 0x2B , (char*)inBuf , buflen , (char*)outBuf , &len);
	if (iRet != 0 && iRet != 1)
	{
		return 1;
	}
	
	memcpy(status,outBuf+3,1);
	wrHex("status: ",status,1);

	if (outBuf[3] != 0x00)
	{
		//��ʾʧ��
		return 2;
	}
	
    tmp_len+=4;
	
	bbzero(pyct_data->ticket_physicalNo);
	wrHex("������: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_physicalNo,outBuf+tmp_len,8);
	tmp_len+=8;
	
	//wrHex("�߼�����: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_no,outBuf+tmp_len,8);
	tmp_len+=8;
	
//	memcpy((BYTE *)pyct_data->yct_sequence_NO,outBuf+tmp_len,8);
	bbzero(trade_sequence);
	CTools::byte_to_hex((BYTE *)outBuf+tmp_len,(BYTE *)trade_sequence,2); //�������
	wrHex("�������: ",outBuf+tmp_len,2);
	tmp_len+=2;
	
	bbzero(consume_info);
	memcpy(consume_info,outBuf+tmp_len,144);
	wrHex("������Ϣ: ",outBuf+tmp_len,144);
	tmp_len+=144;

//	bbzero(pyct_data->ticket_amo_bef);
//	memcpy(pyct_data->ticket_amo_bef,outBuf+tmp_len,4);
	wrHex("Ǯ�����: ",outBuf+tmp_len,4);
	tmp_len+=4;
	
	wrHex("SW1SW2: ",outBuf+tmp_len,2);
	memcpy(pyct_data->SW1SW2,outBuf+tmp_len,2);
	tmp_len+=2;
	return 0;	
}

//�۷�
//in: consume_info - ������Ϣ, status - ������������.
//out: BYTE *consume_cmd - ����ʱ��
int CYCTPurchase::P_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *consume_info,BYTE *status,BYTE *consume_cmd)
{
	int iRet=0, len=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE RFU[64+1];
	BYTE rsp_code[4+1];
	BYTE temp[20+1];
	
	wrLog("��P_PURCHASE��");
	
	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);
	
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki����
	len+=4;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_physicalNo,8);	 //������
//	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,sndData+len,16);
	wrHex("������: ",sndData+len,8);
	wrHex("pyct_data->ticket_physicalNo: ",(BYTE *)pyct_data->ticket_physicalNo,8);
	len+=8;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_no,8);	 //�߼�����
//	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,sndData+len,16);
	wrLog("pyct_data->ticket_no: %s",pyct_data->ticket_no);
	//wrHex("�߼�����: ",sndData+len,8);
	len+=8;	
	
	memcpy(sndData+len,pyct_data->yct_stan,8);	 //ϵͳ��ˮ��
	wrHex("ϵͳ��ˮ��: ",sndData+len,8);
	len+=8;
		
	memcpy(sndData+len,(BYTE *)consume_info,144);	 //������Ϣ
	wrHex("������Ϣ: ",sndData+len,144);
	len+=144;
	
	memcpy(sndData+len,(BYTE *)pyct_data->SW1SW2,2);	 //��Ƭ������
	wrHex("��Ƭ������: ",sndData+len,2);
	len+=2;
	
	memcpy(sndData+len,(BYTE *)status,1);	 //������������
	wrHex("������������: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,(BYTE *)RFU,25);	 //RFU
	wrHex("RFU: ",sndData+len,25);
	len+=25;
	
	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E013" , (BYTE*)"0001" , 0x02 , 0x80 , \
		sndData , len , rcvData , &rcvLen);
	
	len = 8;
    wrHex("������ˮ��: ",rcvData + len,8);
	len+=8;
	memcpy(pyct_data->yct_stan,rcvData + len,8);//���ں����۷Ѳ�������չӦ����ˮ
	wrHex("����ϵͳ��ˮ��: ",rcvData + len,8);
	len+=8;
	//memcpy(rsp_code,rcvData + len,4)
	CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4);//������
	wrHex("������: ",rcvData + len,4);
	len+=4;
	
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("���׷�����:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//������Ϣ����
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("������Ϣ����: %s",byte_errinfo);				
		return 1;
	}
	
	bbzero(consume_cmd);
	memcpy(consume_cmd,rcvData + len,48);
	wrHex("����ָ��: ",rcvData + len,48);
	len+=48;

	bbzero(temp);
	memcpy(temp,rcvData + len,7);
	wrHex("����ʱ��: ",rcvData + len,7);
	CTools::byte_to_hex(temp,(BYTE *)pyct_data->tradeTime,14);

	return 0;
}


//�۷�
//in: BYTE *consume_cmd - ����ָ��
//out: BYTE *TAC - ������֤��,BYTE *MAC2 - MAC2 
int CYCTPurchase::R_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *consume_cmd,BYTE *wallet_type,BYTE *MAC2)
{
	int buflen=0 , len = 0 , iRet = 0,tmp_len = 0;
	BYTE inBuf[256+1];
	BYTE outBuf[256+1];
	
	wrLog("��R_PURCHASE��");
	
	bbzero(inBuf);
	bbzero(outBuf);
	
	memcpy(inBuf,consume_cmd,48);
	wrHex("���ѽ�����Ϣ: ",consume_cmd,48);
	buflen += 48;
	
	iRet = CYCTYPTrade::R_COMMAND(dPosInfo , com , comType , 0x2C , (char*)inBuf , buflen , (char*)outBuf , &len);
	if (iRet != 0 && iRet != 1)
	{
		return 1;
	}
	
	if (outBuf[3] != 0x00)
	{
		//��ʾʧ�ܣ�������Ϣ�ο�������������
		return 2;
	}
	
    tmp_len+=4;

	bbzero(wallet_type);
	memcpy(wallet_type,outBuf+tmp_len,1);
	wrHex("Ǯ������: ",outBuf+tmp_len,1);
	if (outBuf[tmp_len] == 0x01)
	{
		wrLog("M1Ǯ��");
	}
	else if (outBuf[tmp_len] == 0x02)
	{
		wrLog("CPUǮ��");
	}
	tmp_len+=1;
	
	wrHex("������֤��: ",outBuf+tmp_len,4);
	bbzero(pyct_data->TAC);
	memcpy(pyct_data->TAC,outBuf+tmp_len,4);
	tmp_len+=4;
	
	bbzero(MAC2);
	memcpy(MAC2,outBuf+tmp_len,4);
	wrHex("MAC2: ",outBuf+tmp_len,4);
	tmp_len+=4;
	
	wrHex("SW1SW2: ",outBuf+tmp_len,2);
	memcpy(pyct_data->SW1SW2,outBuf+tmp_len,2);
	tmp_len+=2;
	return 0;	
}

//�۷��ύ
int CYCTPurchase::P_PURCHASE_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *wallet_type,BYTE *MAC2,BYTE *status,BYTE *card_appInfo,BYTE *followerup_oper)
{
	int iRet=0, len=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE RFU[64+1];
	BYTE rsp_code[4+1];
	BYTE temp[20+1];
	
	wrLog("��P_PURCHASE_SUBMIT��");
	
	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);
	
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki����
	len+=4;

	memcpy(sndData+len,pyct_data->yct_stan,8);	 //ϵͳ��ˮ��
	wrHex("ϵͳ��ˮ��: ",sndData+len,8);
	len+=8;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_physicalNo,8);	 //������
	//CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,sndData+len,16);
	wrHex("pyct_data->ticket_physicalNo: ",(BYTE *)pyct_data->ticket_physicalNo,8);
	wrHex("������: ",sndData+len,8);
	len+=8;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_no,8);	 //�߼�����
	//CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,sndData+len,16);
	//wrLog("pyct_data->ticket_no: %s",pyct_data->ticket_no);
	//wrHex("pyct_data->ticket_no: ",(BYTE *)pyct_data->ticket_no,8);
//	wrHex("�߼�����: ",sndData+len,8);
	len+=8;	
	
	memcpy(sndData+len,(BYTE *)wallet_type,1);	 //Ǯ������
	wrHex("Ǯ������: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,(BYTE *)pyct_data->TAC,4);	 //TAC
	wrHex("TAC: ",sndData+len,4);
	len+=4;
	
	memcpy(sndData+len,(BYTE *)MAC2,4);	 //MAC2
	wrHex("MAC2: ",sndData+len,4);
	len+=4;

	memcpy(sndData+len,(BYTE *)pyct_data->SW1SW2,2);	 //��Ƭ������
	wrHex("��Ƭ������: ",sndData+len,2);
	len+=2;
	
	memcpy(sndData+len,(BYTE *)status,1);	 //������������
	wrHex("������������: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,(BYTE *)card_appInfo,40);	 //Ӧ����Ϣ
	wrHex("Ӧ����Ϣ: ",sndData+len,40);
	len+=40;
	
	memcpy(sndData+len,(BYTE *)RFU,40);	 //RFU
	wrHex("RFU: ",sndData+len,40);
	len+=40;
	
	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E015" , (BYTE*)"0001" , 0x02 , 0x80 , \
		sndData , len , rcvData , &rcvLen);
	
	len = 8;
    wrHex("������ˮ��: ",rcvData + len,8);
	len+=8;
	//memcpy(pyct_data->yct_stan,rcvData + len,8);//���ں����۷Ѳ�������չӦ����ˮ
	wrHex("����ϵͳ��ˮ��: ",rcvData + len,8);
	len+=8;
	//memcpy(rsp_code,rcvData + len,4)
	CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4);//������
	wrHex("������: ",rcvData + len,4);
	len+=4;
	
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("���׷�����:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//������Ϣ����
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("������Ϣ����: %s",byte_errinfo);				
		return 1;
	}
		
	bbzero(temp);
	memcpy(temp,rcvData + len,7);
	wrHex("��������ʱ��: ",rcvData + len,7);
	CTools::byte_to_hex(temp,(BYTE *)pyct_data->tradeTime,14);
    len+=7;

	bbzero(pyct_data->yct_termid);
	memcpy((BYTE *)pyct_data->yct_termid,rcvData + len,6);
	wrHex("�ն˱��: ",(BYTE *)pyct_data->yct_termid,6);
	len+=6;

	bbzero(pyct_data->trader_id);
	memcpy((BYTE *)pyct_data->trader_id,rcvData + len,8);
	wrHex("�̻����: ",(BYTE *)pyct_data->trader_id,8);
	len+=8;

	bbzero(pyct_data->trader_name);
	memcpy((BYTE *)pyct_data->trader_name,rcvData + len,20);
	wrHex("�̻����: ",(BYTE *)pyct_data->trader_name,20);
	len+=20;

	bbzero(pyct_data->note);
	memcpy(pyct_data->note,rcvData + len,64);
	wrHex("��ע: ",(BYTE *)pyct_data->note,64);
	len+=64;

	bbzero(followerup_oper);
	memcpy(followerup_oper,rcvData + len,1);
	wrHex("��������: ",followerup_oper,1);
	len+=1;

	return 0;
}

//�ȽϿ۷�ǰ�����
//in: PYCT_DATA pyct_data
//out: 0 - �۷�ǰ������ֵ��ȷ��1 - �۷�ǰ������ֵ����
int CYCTPurchase::balance_compare(PYCT_DATA pyct_data,char *trade_amount)	
{
	char amo_bef[12+1];
	char amo_aft[12+1];
	bbzero(amo_aft);
	bbzero(amo_bef);
	strncpy(amo_bef,pyct_data->ticket_amo_bef,12);
	strncpy(amo_aft,pyct_data->ticket_amo_aft,12);
// 	CTools::HexToDec(pyct_data->ticket_amo_bef, amo_bef);
// 	CTools::HexToDec(pyct_data->ticket_amo_aft, amo_aft);	
	if ((atol(amo_aft) == (atol(amo_bef) - atol(trade_amount))) )
	{
		wrLog("�۷�ǰ: %s",amo_bef);
		wrLog("�۷ѽ��: %s",trade_amount);
		wrLog("�۷Ѻ�: %s",amo_aft);
		return 0;
	}
	else
	{
		return 1;
	}

}
/****************���۷�����*********************/