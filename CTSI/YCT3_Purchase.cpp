#include "YCT3_Purchase.h"

/****************卡扣费流程*********************/
int CYCTPurchase::Purchase_Handle(CGlobal *pgbl,PYCT_DATA pyct_data,BYTE *app_type,char *purchase_amo,BYTE *extension_stan,BYTE* party, BYTE* rspcode)
{
	int iRet,iSelect = 0;
	int com=0, comType=0;
	BYTE pysical_info [51]; //卡片物理信息
	BYTE card_pycicalInfo1[32+1];//卡片物理信息1
	BYTE SW1SW2[2];
	BYTE SAK[1];
	CPack pack;	
	BYTE query_info[96+1];//查询信息
	BYTE wallet_type[1+1];//钱包类型
	BYTE count_info[16+1];//计数信息
	BYTE card_appinfo[40+1];//卡应用信息
	BYTE consume_tradeinfo[64+1];//消费交易信息
	BYTE consume_info[144+1];//消费信息
	BYTE status[1+1];
	BYTE consume_cmd[48+1];//消费指令
	BYTE MAC2[4+1];
	BYTE ReaderStatus[1+1];//读卡器返回状态
	int try_times = 1;
	BYTE followerup_oper[1+1];//后续操作
	char ticket_no[16+1];
	char show_ticket_no[16+1];

	bbzero(ticket_no);
	bbzero(show_ticket_no);
	
	wrLog("【Purchase_Handle】");

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
// 		return 5;// 5：扣费前失败
// 	}
// 	
// 	if((pgbl->posInfo).rightType == 0x08)	//索天读卡器
// 	{
// 		//memset(pyct_data,0,sizeof(pyct_data));
// 		comType = 0x05;
// 		iRet=CYCTYPTrade::ST_initReader3(pgbl->dPosInfo,com,comType,pyct_data); //索天读卡器第三代羊城通充值读卡器初始化
// 		if (CYCTYPTrade::yct_SignIn(pgbl->dPosInfo,com,comType,pyct_data,party,rspcode)==0)
// 		{
// 			wrLog("签到成功");
// 		}
// 		
// 		if(iRet)
// 		{
// 			*party = FROMCTSI;
// 			*rspcode = CONNCOMFAIL;
// 			return 5;
// 		}
// 	}
// 	//暂时屏蔽明华读卡器交易
// 	else
// 	{
// 		TS_OutPutMsg(pgbl->dPosInfo, "无效硬件，无法继续交易，请重新设置，如有疑问请联系交费易客服", 0, iSelect);
// 		*party = FROMCTSI;
// 		*rspcode = CONNCOMFAIL;
// 		return 5;
// 	}
    com = pgbl->com;
	comType = pgbl->comType;

	//R_PUB_QRY_PHYSICS_INFO()
	//.终端向读卡器重新发送查询卡物理信息的指令
	iRet=CYCTYPTrade::R_PUB_QRY_PHYSICS_INFO(pgbl->dPosInfo,com,comType, pysical_info);
	
	if(iRet==0)
	{
		bbzero(SW1SW2);
		CTools::byte_to_hex((BYTE *)pysical_info,(BYTE *)pyct_data->ticket_physicalNo,8);//获取物理卡号
		CTools::byte_to_hex((BYTE *)pysical_info+8,(BYTE *)pyct_data->ticket_no,8);//取得逻辑卡号
		memcpy(SAK,pysical_info+16,1); //sSAK
		bbzero(card_pycicalInfo1);
		memcpy(card_pycicalInfo1,pysical_info+17,32); //卡片物理信息1
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
			sprintf(outPut_Msg,"返回响应卡状态码错误：%s",errMsg);
			TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1：与读卡器交互失败
		}

		wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
		//wrLog("逻辑卡号:%s",pyct_data->ticket_no);
		CTools::shildWords(pyct_data->ticket_no,7,5,ticket_no);
		wrLog("逻辑卡号:%s",ticket_no);

		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32);	
	}
	else if (iRet != 0)
	{
		return 1;// 1：与读卡器交互失败
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
		//对比扣费前后余额减值是否正确
		iRet = balance_compare(pyct_data,purchase_amo);
		if (iRet == 0)
		{
			wrLog("扣费成功");
			bbzero(pyct_data->out_balance_aft);
			memcpy(pyct_data->out_balance_aft,pyct_data->ticket_amo_aft,12);
			//打印扣费成功单据
		}
		else
		{
			wrLog("扣费失败");
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
		//还有相关操作，需要执行扩展应用操作
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
	
	wrLog("【P_PURCHASE_QRY】");

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);

	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki卡号
	len+=4;

	//memcpy(sndData+len,(BYTE *)pyct_data->ticket_physicalNo,8);	 //物理卡号
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,sndData+len,16);
	wrHex("物理卡号: ",sndData+len,8);
	len+=8;

	//memcpy(sndData+len,(BYTE *)pyct_data->ticket_no,8);	 //逻辑卡号
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,sndData+len,16);
//	wrHex("逻辑卡号: ",sndData+len,8);
	len+=8;

	
	memcpy(sndData+len,app_type,1);	 //应用类型
	wrHex("应用类型: ",sndData+len,1);
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
	wrHex("扣费金额: ",sndData+len,4);//扣费金额
	len+=4;

	memcpy(sndData+len,(BYTE *)card_pycicalInfo1,32);	 //卡物理信息1
	wrHex("卡物理信息1: ",sndData+len,32);
	len+=32;

	if (app_type[0] == 0x01 || app_type[0] == 0x02)
	{
		memcpy(sndData+len,(BYTE *)extension_stan,8);	 //扩展应用流水
	}
	else
	{
		memcpy(sndData+len,(BYTE *)0x00,8);	 //扩展应用流水
	}
	wrHex("扩展应用流水: ",sndData+len,8);
	len+=8;

	memcpy(sndData+len,(BYTE *)RFU,39);	 //RFU
	wrHex("RFU: ",sndData+len,39);
	len+=39;

	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E011" , (BYTE*)"0001" , 0x02 , 0x80 , \
		sndData , len , rcvData , &rcvLen);
	
	len = 8;
    wrHex("握手流水号: ",rcvData + len,8);
	len+=8;
	memcpy(pyct_data->yct_stan,rcvData + len,8);//用于后续扣费操作的扩展应用流水
	wrHex("返回系统流水号: ",rcvData + len,8);
	len+=8;
	//memcpy(rsp_code,rcvData + len,4)
	CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4);//返回码
	wrHex("返回码: ",rcvData + len,4);
	len+=4;
	
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("交易返回码:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//错误信息描述
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("错误信息描述: %s",byte_errinfo);	
		bbzero(Output_Msg);
		strcat(Output_Msg,byte_errinfo);
		TS_OutPutMsg(dPosInfo,byte_errinfo,1,iSelect);
		return 1;
	}
	
	bbzero(consume_tradeinfo);
	memcpy(consume_tradeinfo,rcvData + len,64);
	wrHex("消费交易信息: ",rcvData + len,64);

	return 0;
}

//扣费初始化
//in: consume_tradeinfo - 消费交易信息
//out: consume_info - 消费信息, status - 读卡器返回码
int CYCTPurchase::R_PURCHASE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *consume_tradeinfo,BYTE *consume_info,BYTE *status)
{
	int buflen=0 , len = 0 , iRet = 0,tmp_len = 0;
	BYTE inBuf[256+1];
	BYTE outBuf[256+1];
	BYTE trade_sequence[2+1];
	
	wrLog("【R_PURCHASE_INIT】");
	
	bbzero(inBuf);
	bbzero(outBuf);

	memcpy(inBuf,consume_tradeinfo,64);
	wrHex("消费交易信息: ",consume_tradeinfo,64);
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
		//提示失败
		return 2;
	}
	
    tmp_len+=4;
	
	bbzero(pyct_data->ticket_physicalNo);
	wrHex("物理卡号: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_physicalNo,outBuf+tmp_len,8);
	tmp_len+=8;
	
	//wrHex("逻辑卡号: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_no,outBuf+tmp_len,8);
	tmp_len+=8;
	
//	memcpy((BYTE *)pyct_data->yct_sequence_NO,outBuf+tmp_len,8);
	bbzero(trade_sequence);
	CTools::byte_to_hex((BYTE *)outBuf+tmp_len,(BYTE *)trade_sequence,2); //交易序号
	wrHex("交易序号: ",outBuf+tmp_len,2);
	tmp_len+=2;
	
	bbzero(consume_info);
	memcpy(consume_info,outBuf+tmp_len,144);
	wrHex("消费信息: ",outBuf+tmp_len,144);
	tmp_len+=144;

//	bbzero(pyct_data->ticket_amo_bef);
//	memcpy(pyct_data->ticket_amo_bef,outBuf+tmp_len,4);
	wrHex("钱包余额: ",outBuf+tmp_len,4);
	tmp_len+=4;
	
	wrHex("SW1SW2: ",outBuf+tmp_len,2);
	memcpy(pyct_data->SW1SW2,outBuf+tmp_len,2);
	tmp_len+=2;
	return 0;	
}

//扣费
//in: consume_info - 消费信息, status - 读卡器返回码.
//out: BYTE *consume_cmd - 消费时间
int CYCTPurchase::P_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *consume_info,BYTE *status,BYTE *consume_cmd)
{
	int iRet=0, len=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE RFU[64+1];
	BYTE rsp_code[4+1];
	BYTE temp[20+1];
	
	wrLog("【P_PURCHASE】");
	
	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);
	
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki卡号
	len+=4;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_physicalNo,8);	 //物理卡号
//	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,sndData+len,16);
	wrHex("物理卡号: ",sndData+len,8);
	wrHex("pyct_data->ticket_physicalNo: ",(BYTE *)pyct_data->ticket_physicalNo,8);
	len+=8;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_no,8);	 //逻辑卡号
//	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,sndData+len,16);
	wrLog("pyct_data->ticket_no: %s",pyct_data->ticket_no);
	//wrHex("逻辑卡号: ",sndData+len,8);
	len+=8;	
	
	memcpy(sndData+len,pyct_data->yct_stan,8);	 //系统流水号
	wrHex("系统流水号: ",sndData+len,8);
	len+=8;
		
	memcpy(sndData+len,(BYTE *)consume_info,144);	 //消费信息
	wrHex("消费信息: ",sndData+len,144);
	len+=144;
	
	memcpy(sndData+len,(BYTE *)pyct_data->SW1SW2,2);	 //卡片返回码
	wrHex("卡片返回码: ",sndData+len,2);
	len+=2;
	
	memcpy(sndData+len,(BYTE *)status,1);	 //读卡器返回码
	wrHex("读卡器返回码: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,(BYTE *)RFU,25);	 //RFU
	wrHex("RFU: ",sndData+len,25);
	len+=25;
	
	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E013" , (BYTE*)"0001" , 0x02 , 0x80 , \
		sndData , len , rcvData , &rcvLen);
	
	len = 8;
    wrHex("握手流水号: ",rcvData + len,8);
	len+=8;
	memcpy(pyct_data->yct_stan,rcvData + len,8);//用于后续扣费操作的扩展应用流水
	wrHex("返回系统流水号: ",rcvData + len,8);
	len+=8;
	//memcpy(rsp_code,rcvData + len,4)
	CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4);//返回码
	wrHex("返回码: ",rcvData + len,4);
	len+=4;
	
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("交易返回码:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//错误信息描述
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("错误信息描述: %s",byte_errinfo);				
		return 1;
	}
	
	bbzero(consume_cmd);
	memcpy(consume_cmd,rcvData + len,48);
	wrHex("消费指令: ",rcvData + len,48);
	len+=48;

	bbzero(temp);
	memcpy(temp,rcvData + len,7);
	wrHex("消费时间: ",rcvData + len,7);
	CTools::byte_to_hex(temp,(BYTE *)pyct_data->tradeTime,14);

	return 0;
}


//扣费
//in: BYTE *consume_cmd - 消费指令
//out: BYTE *TAC - 交易认证码,BYTE *MAC2 - MAC2 
int CYCTPurchase::R_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *consume_cmd,BYTE *wallet_type,BYTE *MAC2)
{
	int buflen=0 , len = 0 , iRet = 0,tmp_len = 0;
	BYTE inBuf[256+1];
	BYTE outBuf[256+1];
	
	wrLog("【R_PURCHASE】");
	
	bbzero(inBuf);
	bbzero(outBuf);
	
	memcpy(inBuf,consume_cmd,48);
	wrHex("消费交易信息: ",consume_cmd,48);
	buflen += 48;
	
	iRet = CYCTYPTrade::R_COMMAND(dPosInfo , com , comType , 0x2C , (char*)inBuf , buflen , (char*)outBuf , &len);
	if (iRet != 0 && iRet != 1)
	{
		return 1;
	}
	
	if (outBuf[3] != 0x00)
	{
		//提示失败，错误信息参考读卡器错误定义
		return 2;
	}
	
    tmp_len+=4;

	bbzero(wallet_type);
	memcpy(wallet_type,outBuf+tmp_len,1);
	wrHex("钱包类型: ",outBuf+tmp_len,1);
	if (outBuf[tmp_len] == 0x01)
	{
		wrLog("M1钱包");
	}
	else if (outBuf[tmp_len] == 0x02)
	{
		wrLog("CPU钱包");
	}
	tmp_len+=1;
	
	wrHex("交易认证码: ",outBuf+tmp_len,4);
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

//扣费提交
int CYCTPurchase::P_PURCHASE_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
				BYTE *wallet_type,BYTE *MAC2,BYTE *status,BYTE *card_appInfo,BYTE *followerup_oper)
{
	int iRet=0, len=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE RFU[64+1];
	BYTE rsp_code[4+1];
	BYTE temp[20+1];
	
	wrLog("【P_PURCHASE_SUBMIT】");
	
	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);
	
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki卡号
	len+=4;

	memcpy(sndData+len,pyct_data->yct_stan,8);	 //系统流水号
	wrHex("系统流水号: ",sndData+len,8);
	len+=8;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_physicalNo,8);	 //物理卡号
	//CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,sndData+len,16);
	wrHex("pyct_data->ticket_physicalNo: ",(BYTE *)pyct_data->ticket_physicalNo,8);
	wrHex("物理卡号: ",sndData+len,8);
	len+=8;
	
	memcpy(sndData+len,(BYTE *)pyct_data->ticket_no,8);	 //逻辑卡号
	//CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,sndData+len,16);
	//wrLog("pyct_data->ticket_no: %s",pyct_data->ticket_no);
	//wrHex("pyct_data->ticket_no: ",(BYTE *)pyct_data->ticket_no,8);
//	wrHex("逻辑卡号: ",sndData+len,8);
	len+=8;	
	
	memcpy(sndData+len,(BYTE *)wallet_type,1);	 //钱包类型
	wrHex("钱包类型: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,(BYTE *)pyct_data->TAC,4);	 //TAC
	wrHex("TAC: ",sndData+len,4);
	len+=4;
	
	memcpy(sndData+len,(BYTE *)MAC2,4);	 //MAC2
	wrHex("MAC2: ",sndData+len,4);
	len+=4;

	memcpy(sndData+len,(BYTE *)pyct_data->SW1SW2,2);	 //卡片返回码
	wrHex("卡片返回码: ",sndData+len,2);
	len+=2;
	
	memcpy(sndData+len,(BYTE *)status,1);	 //读卡器返回码
	wrHex("读卡器返回码: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,(BYTE *)card_appInfo,40);	 //应用信息
	wrHex("应用信息: ",sndData+len,40);
	len+=40;
	
	memcpy(sndData+len,(BYTE *)RFU,40);	 //RFU
	wrHex("RFU: ",sndData+len,40);
	len+=40;
	
	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E015" , (BYTE*)"0001" , 0x02 , 0x80 , \
		sndData , len , rcvData , &rcvLen);
	
	len = 8;
    wrHex("握手流水号: ",rcvData + len,8);
	len+=8;
	//memcpy(pyct_data->yct_stan,rcvData + len,8);//用于后续扣费操作的扩展应用流水
	wrHex("返回系统流水号: ",rcvData + len,8);
	len+=8;
	//memcpy(rsp_code,rcvData + len,4)
	CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4);//返回码
	wrHex("返回码: ",rcvData + len,4);
	len+=4;
	
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("交易返回码:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//错误信息描述
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("错误信息描述: %s",byte_errinfo);				
		return 1;
	}
		
	bbzero(temp);
	memcpy(temp,rcvData + len,7);
	wrHex("交易日期时间: ",rcvData + len,7);
	CTools::byte_to_hex(temp,(BYTE *)pyct_data->tradeTime,14);
    len+=7;

	bbzero(pyct_data->yct_termid);
	memcpy((BYTE *)pyct_data->yct_termid,rcvData + len,6);
	wrHex("终端编号: ",(BYTE *)pyct_data->yct_termid,6);
	len+=6;

	bbzero(pyct_data->trader_id);
	memcpy((BYTE *)pyct_data->trader_id,rcvData + len,8);
	wrHex("商户编号: ",(BYTE *)pyct_data->trader_id,8);
	len+=8;

	bbzero(pyct_data->trader_name);
	memcpy((BYTE *)pyct_data->trader_name,rcvData + len,20);
	wrHex("商户简称: ",(BYTE *)pyct_data->trader_name,20);
	len+=20;

	bbzero(pyct_data->note);
	memcpy(pyct_data->note,rcvData + len,64);
	wrHex("备注: ",(BYTE *)pyct_data->note,64);
	len+=64;

	bbzero(followerup_oper);
	memcpy(followerup_oper,rcvData + len,1);
	wrHex("后续操作: ",followerup_oper,1);
	len+=1;

	return 0;
}

//比较扣费前后余额
//in: PYCT_DATA pyct_data
//out: 0 - 扣费前后余额减值正确，1 - 扣费前后余额减值错误
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
		wrLog("扣费前: %s",amo_bef);
		wrLog("扣费金额: %s",trade_amount);
		wrLog("扣费后: %s",amo_aft);
		return 0;
	}
	else
	{
		return 1;
	}

}
/****************卡扣费流程*********************/