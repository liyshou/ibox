#include "YCT3_BalanceTransfer.h"

/****************卡间余额转移*********************/

int CYCTBlnTransfer::BalanceTransfer_TradeHdl(CGlobal gbl,YCT_DATA yct_data,char *title)
{
	int iRet,iSelect = 0;
	int com = 0;
	int comType = 0;
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	char m_title[1024];
	char priCont[1024];
	char transfer_amo[12+1];//转移金额
	BYTE transqry_type[1+1];
	char in_mny[20+1];
	CPack pack;
	TSTradeInfoSend tradeInfoSend;
	TSTradeInfoRecv tradeInfoRecv;
	char temp_buff[1024];
	char amo[12+1];
	char out_amo[12+1];
	char in_amo[12+1];
	char temp_amo[4];
	BYTE extension_stan[8+1];//扩展应用流水
	BYTE app_type[1+1];//应用类型
	BYTE in_wallet_type[1+1];//转入卡钱包类型
	BYTE out_wallet_type[1+1];//转出卡钱包类型
	CPrint prt;
	char purchase_stan[16+1];//卡扣费流水
	char load_stan[16+1];//卡圈存流水
	char out_cardno[16+1];

	bbzero(m_title);
	bbzero(priCont);
	bbzero(transqry_type);
	bbzero(temp_buff);
	bbzero(amo);
	bbzero(out_amo);
	bbzero(in_amo);
	bbzero(in_wallet_type);
	bbzero(out_wallet_type);
	bbzero(purchase_stan);
	bbzero(load_stan);
	bbzero(out_cardno);

	
	CDB::update_epos_term(gbl.pos_no, "1");//将终端状态设为"公交扣费中"
	
	memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
	memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
	
	tradeInfoSend.uHasTradeID = 1;
	tradeInfoSend.uTotalContents = 1;
	
	//转移金额输入
	strcpy(tradeInfoSend.Contents[0].acTitle, "请输入转移金额：");	
	tradeInfoSend.Contents[0].uCommand = TS_INPUT_MONEY;	//金额
	tradeInfoSend.Contents[0].uCount = 0;		//输入1次
	tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME+10;	//超时40秒
	tradeInfoSend.Contents[0].uMinLen = 1;	
	tradeInfoSend.Contents[0].uMaxLen = 12;	
	
	iRet = TS_TradeControl(&gbl.dPosInfo, &tradeInfoSend, &tradeInfoRecv);
	if(iRet != 0)
	{
		wrLog("TS_TradeControl返回错误[%d]", iRet);
		wrMonitorLog(1, 1, iRet, gbl, pack);	//监控日志
		return iRet;
	}
	
	bbzero(transfer_amo);
	strcpy(transfer_amo,(char *)tradeInfoRecv.Contents[0].uMsg);
	bbzero(gbl.amount);
	strcpy(gbl.amount,transfer_amo);
	bbzero(yct_data.amount);
	strcpy(yct_data.amount,transfer_amo);

	iRet = TS_OutPutMsg(gbl.dPosInfo, "请指引客户将转入卡正确放置在读卡器上，且勿移动它。\n按【确认】继续 按【返回】取消\n\n\n\n【温馨提示】该功能暂不支持不同区域间卡片的余额转移，比如，暂不支持广州的卡片与佛山的卡片间的余额转移。", 0, iSelect);
	if( iRet==0 && iSelect==0 )
	{
		NULL;
	}
	else
	{
		return 5;// 5：扣费前失败
	}

	//初始化串口号，串口类型,签到获取YCT_DATA结构
// 	if (CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data) != 0)
// 	{
// 		return 1;
// 	}

	char cur_signTime[30+1];
	CDB::getYct3SignInfo(gbl.pos_no,&yct_data,cur_signTime);

	transqry_type[0] = 0x01;
	iRet = Balance_Transfer_QRY(&gbl, &yct_data,transfer_amo,transqry_type,in_wallet_type,&m_party,&m_rspcode,priCont);
	if (iRet == 2 || iRet == 1)
	{
		CYCTYPTrade::showErrMsg(gbl,m_party,m_rspcode);
		return 2;// 
	}
	//显示转入卡余额
	//提示余额
	bbzero(in_mny);
	bbzero(temp_amo);
	memcpy(in_amo,yct_data.in_balance,12);
	bbzero(yct_data.in_balance_bef);
	memcpy(yct_data.in_balance_bef,yct_data.in_balance,12);
//	CTools::byte_to_hex((BYTE *)yct_data.in_balance,(BYTE *)in_amo,4);
	//CTools::HexToDec(temp_amo,in_amo);
	CTools::chgToMny(yct_data.in_balance, in_mny);
	wrLog("转入卡余额: %s", in_mny);
	wrMonitorLog(12, 0, 0, gbl, pack);	//监控日志
	bbzero( temp_buff );
	sprintf(temp_buff, "转入卡的余额为：%s\n按【确认】继续\n按【返回】取消", in_mny);
	TS_OutPutMsg(gbl.dPosInfo, temp_buff, 0, iSelect);
	if( iSelect!=0 )
	{
		return 0;
	}


	iRet = TS_OutPutMsg(gbl.dPosInfo, "请指引客户将转出卡正确放置在读卡器上进行扣费，且勿移动它。\n按【确认】继续 按【返回】取消", 0, iSelect);
	if( iRet==0 && iSelect==0 )
	{
		NULL;
	}
	else
	{
		return 5;// 5：扣费前失败
	}

	bbzero(transqry_type);
	transqry_type[0] = 0x02;
	iRet = Balance_Transfer_QRY(&gbl, &yct_data,transfer_amo,transqry_type,out_wallet_type,&m_party,&m_rspcode,priCont);
	if (iRet != 0 )
	{
		CYCTYPTrade::showErrMsg(gbl,m_party,m_rspcode);
		return 2;// 
	}


	//转出卡扣费
    //Purchase_Handle();
	memcpy(out_cardno,yct_data.out_logicalcard,8);
	bbzero(extension_stan);
	memcpy(extension_stan,yct_data.yct_stan,8);
	bbzero(yct_data.transfer_stan);
	memcpy(yct_data.transfer_stan,yct_data.yct_stan,8);
	bbzero(app_type);
	app_type[0] = 0x01;
	iRet = CYCTPurchase::Purchase_Handle(&gbl, &yct_data,(BYTE *)app_type,transfer_amo,extension_stan,&m_party,&m_rspcode);
	if (iRet != 0)
	{
		CYCTYPTrade::showErrMsg(gbl,m_party,m_rspcode);
		return 2;// 
	}

	iRet = TS_OutPutMsg(gbl.dPosInfo, "转出卡扣费成功!\n\n按【确认】打印转出卡圈提单据", 0, iSelect);
    memcpy(purchase_stan,yct_data.yct_stan,8);
	wrHex("扣费流水号：",(BYTE *)purchase_stan,8);

	set_blntransfer_priCont(0,&yct_data,priCont);
	prt.set_in_par(gbl);
	prt.print_yctthird_detail("羊城通余额转出",priCont);

	iRet = TS_OutPutMsg(gbl.dPosInfo, "请指引客户再次将转入卡正确放置在读卡器上，且勿移动它。\n按【确认】继续 按【返回】取消", 0, iSelect);
	if( iRet==0 && iSelect==0 )
	{
		NULL;
	}
	else
	{
		return 6;// 6：扣费后失败
	}

    //传入圈存时写数据库需要的参数
	bbzero(yct_data.biz_type);
	bbzero(yct_data.term_id);
	bbzero(yct_data.term_serial);
	bbzero(yct_data.bank);
	strncpy(yct_data.biz_type, gbl.fee_type, strlen(gbl.fee_type));
	strncpy(yct_data.term_id, gbl.pos_no, strlen(gbl.pos_no));
	bbzero(gbl.pos_serial);
	CTools::get_time(gbl.pos_serial);
	strncpy(yct_data.term_serial, gbl.pos_serial, sizeof(yct_data.term_serial)-1);
	strncpy(yct_data.bank, gbl.bank,  strlen(gbl.bank));
	strncpy(yct_data.card_no, gbl.card_no, strlen(gbl.card_no));
    
	//转入卡圈存 
	if (in_wallet_type[0] == 0x01)
	{
		wrLog("M1钱包(查询余额，圈存和撤销)");
		iRet = CYCTYPTrade::excute_M1_LOAD(gbl.dPosInfo,com,comType,&yct_data,0, &m_party, &m_rspcode,transfer_amo,0x30);
	}
	else if (in_wallet_type[0] == 0x02)
	{
		wrLog("CPU钱包充值（查询余额，圈存和撤销）");
		BYTE query_info[48+1];
		bbzero(query_info);
		iRet = CYCTYPTrade::CPU_EnqPay(gbl.dPosInfo, com, comType,0,&yct_data,transfer_amo,0x30,query_info,&m_party, &m_rspcode);
	}
	//CYCTYPTrade::init_sign(&gbl,com,comType,yct_data);
	//iRet = CYCTYPTrade::ST_NormalEnqPay(gbl.dPosInfo, gbl.com, gbl.comType, NORMALPAY, &yct_data, transfer_amo, &m_party, &m_rspcode);
	bbzero(yct_data.in_balance_aft);
	memcpy(yct_data.in_balance_aft,yct_data.ticket_amo_aft,12);
	if (m_party != FROMSUCC)
	{
		
		iRet = TS_OutPutMsg(gbl.dPosInfo, "余额转移失败!\n\n请联系在线客服中心【Ctrl+H】  400-80-11888", 0, iSelect);
	}

	//写数据库
	strcpy(load_stan,yct_data.yct_stan);
	//strncpy(yct_data.stan, gbl.stan_no, sizeof(yct_data.stan)-1);
	strcpy(yct_data.stan,load_stan);
	CTools::byte_to_hex((BYTE *)purchase_stan,(BYTE *)yct_data.yct_serial,8);
	//strcpy(yct_data.yct_serial,purchase_stan);
	bbzero(yct_data.txn_date);
	bbzero(yct_data.txn_time);
	CTools::getDate(yct_data.txn_date);
	CTools::getTime(yct_data.txn_time);
	//strncpy(yct_data.card_no, gbl.card_no, sizeof(yct_data.card_no)-1);
	bbzero(yct_data.card_no);
	bbzero(yct_data.ticket_no);
	CTools::byte_to_hex((BYTE *)out_cardno,(BYTE *)yct_data.card_no,8);
	//strcpy(yct_data.card_no,(char *)out_cardno);
	CTools::byte_to_hex((BYTE *)yct_data.in_logicardno,(BYTE *)yct_data.ticket_no,8);
	//strcpy(yct_data.ticket_no,(char *)yct_data.in_logicardno);
	strncpy(yct_data.amount, gbl.amount, sizeof(yct_data.amount)-1);

	yct_data.status = 2;
	//记录羊城通交易数据
//	CDB::setYctYPData(yct_data);
	CDB::insetYctYPtxnDATA(&yct_data,yct_data.stan,2);

	iRet = TS_OutPutMsg(gbl.dPosInfo, "余额转移成功!\n\n按【确认】打印转入卡圈存单据", 0, iSelect);
	//打印圈存单据
	set_blntransfer_priCont(1,&yct_data,priCont);
	prt.set_in_par(gbl);
	prt.print_yctthird_detail("羊城通余额转入",priCont);

	return 0;
}//BalanceTransfer_TradeHdl

int CYCTBlnTransfer::Balance_Transfer_QRY(CGlobal *pgbl, PYCT_DATA pyct_data,char *trans_amo,\
								  BYTE *transqry_type,BYTE *wallet_type,BYTE* party, BYTE* rspcode, char *priCont)
{
	int iRet,iSelect = 0;
	int com=0, comType=0;
	BYTE pysical_info [51]; //卡片物理信息
	BYTE card_pycicalInfo1[32+1];//卡片物理信息1
	BYTE SW1SW2[2];
	BYTE SAK[1];
	CPack pack;	
	BYTE query_info[96+1];//查询信息
	BYTE last_transtan[8+1];//上次卡转流水
	BYTE count_info[16+1];//计数信息
	BYTE card_appinfo[40+1];//卡应用信息
	BYTE out_deductamo[8+1];//转出卡扣减金额
	BYTE in_chargeamo[8+1];//转入卡充值金额
	BYTE ReaderStatus[1+1];//读卡器状态
	char outPut_Msg[1024];
	char show_ticketNo[16+1];

	bbzero(show_ticketNo);

	wrLog("【Balance_Transfer】");

	bbzero(query_info);
	bbzero(last_transtan);
	bbzero(count_info);
	bbzero(out_deductamo);
	bbzero(in_chargeamo);

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
		bbzero(pyct_data->SW1SW2);
		memcpy(pyct_data->SW1SW2,pysical_info+49,2);

		wrLog("SW1SW2:%s",SW1SW2);
		if (memcmp(SW1SW2,"9000",4) != 0)
		{
			char errMsg[1024];
			get_cardStatus_errMsg((BYTE *)temp_SW1SW2,errMsg);
			bbzero(outPut_Msg);
			sprintf(outPut_Msg,"返回响应卡状态码错误：%s",errMsg);
			TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1：与读卡器交互失败
		}

		wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
		CTools::shildWords(pyct_data->ticket_no,7,5,show_ticketNo);
		wrLog("逻辑卡号:%s",show_ticketNo);
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32);	
	}
	else if (iRet != 0)
	{
		*party = FROMIC;
		*rspcode = CARDNOTFOUND;
		return 1;// 1：与读卡器交互失败
	}
	
	//P_TRANSFOR_QRY
	//
	iRet = P_TRANSFOR_QRY(pgbl->dPosInfo, com, comType,pyct_data,card_pycicalInfo1,transqry_type,query_info,last_transtan,wallet_type,party, rspcode);
	if (iRet != 0)
	{
		*party = FROMIC;
		*rspcode = TRANSFER_ERROR;
		return 1;
	}
	BYTE tmp_query_info[96+1];
	bbzero(tmp_query_info);
	memcpy(tmp_query_info,query_info,48);
	if (*rspcode == LASTTRANSFER_NOTDONE)
	{
		//该卡上次转移的充值没成功，转到充值的流程
		TS_OutPutMsg(pgbl->dPosInfo,"该转入卡有未转移完成的充值记录，直接做转入卡充值...",2,iSelect,3);
		bbzero(pyct_data->transfer_stan);
		memcpy(pyct_data->transfer_stan,last_transtan,8);
		if (wallet_type[0] == 0x01)
		{
			wrLog("M1钱包(查询余额，圈存和撤销)");
			iRet = CYCTYPTrade::excute_M1_LOAD(pgbl->dPosInfo,com,comType,pyct_data,0,party,rspcode,trans_amo,0x30);
		}
		else if (wallet_type[0] == 0x02)
		{
			wrLog("CPU钱包充值（查询余额，圈存和撤销）");			
			iRet = CYCTYPTrade::CPU_EnqPay(pgbl->dPosInfo, com, comType,0,pyct_data,trans_amo,0x30,query_info,party,rspcode);
		}

// 		TS_OutPutMsg(pgbl->dPosInfo,"上次未完成交易已成功执行圈存\n\n按【Enter】返回",1,iSelect);
// 		return 0;
	}
	
	if (iRet != 0)
	{
		*party = FROMIC;
		*rspcode = TRANSFER_ERROR;
		return 1;
	}

	//R_PUB_SET_READCARDINFO(设置读卡信息)
	CYCTYPTrade::R_PUB_SET_READCARDINFO(pgbl->dPosInfo, com, comType,(char *)pyct_data->ticket_no,tmp_query_info,(char *)SW1SW2);

    //R_PUB_QRY_APP_INFO
	iRet = R_PUB_QRY_APP_INFO(pgbl->dPosInfo, com, comType,pyct_data,card_appinfo,ReaderStatus);
	if (iRet == 2)
	{
		*party = FROMIC;
		*rspcode = TRANSFER_ERROR;
		return 2;// 
	}
	CTools::byte_to_hex((BYTE *)pyct_data->SW1SW2,SW1SW2,2);
	wrLog("SW1SW2:%s",SW1SW2);
	if (memcmp(SW1SW2,"9000",4) != 0)
//	if (pyct_data->SW1SW2[0] != 0x90 || pyct_data->SW1SW2[1] != 0x00)
	{
		char outMsg[512];
		char errMsg[512];
		get_cardStatus_errMsg((BYTE *)pyct_data->SW1SW2,errMsg);
		sprintf(outMsg,"返回卡状态码错误：%s",errMsg);
		TS_OutPutMsg(pgbl->dPosInfo,outMsg,1,iSelect);
		*party = FROMIC;
		*rspcode = TRANSFER_ERROR;
		return 2;// 
	}

	if (*transqry_type == 0x01)//转入卡
	{
		bbzero(pyct_data->in_logicardno);
		memcpy(pyct_data->in_logicardno,pyct_data->ticket_no,8);
		bbzero(pyct_data->in_balance);
		memcpy(pyct_data->in_balance,pyct_data->ticket_amo_bef,12);
		bbzero(pyct_data->in_downLimit);
		memcpy(pyct_data->in_downLimit,pyct_data->downLimit,1);
		bbzero(pyct_data->in_upperLimit);
		memcpy(pyct_data->in_upperLimit,pyct_data->upperLimit,3);
		bbzero(pyct_data->in_cardappinfo);
		memcpy(pyct_data->in_cardappinfo,card_appinfo,40);
	}
	else if (*transqry_type == 0x02)//转出卡
	{
		if (memcmp(pyct_data->ticket_no,pyct_data->out_logicalcard,8) == 0)
		{
			//转入卡与转出卡相同，返回提示重新放卡
		}

		bbzero(pyct_data->out_logicalcard);
		memcpy(pyct_data->out_logicalcard,(BYTE *)pyct_data->ticket_no,8);
		wrHex("pyct_data->out_logicalcard: ",(BYTE *)pyct_data->out_logicalcard,8);

		bbzero(pyct_data->out_balance);
		memcpy(pyct_data->out_balance,(BYTE *)pyct_data->ticket_amo_bef,12);
		wrLog("pyct_data->out_balance: ",(BYTE *)pyct_data->out_balance,12);

		bbzero(pyct_data->out_downLimit);
		memcpy(pyct_data->out_downLimit,(BYTE *)pyct_data->downLimit,1);
		wrHex("pyct_data->out_downLimit: ",(BYTE *)pyct_data->out_downLimit,1);

		bbzero(pyct_data->out_upperLimit);
		memcpy(pyct_data->out_upperLimit,(BYTE *)pyct_data->upperLimit,3);
		wrHex("pyct_data->out_upperLimit: ",(BYTE *)pyct_data->out_upperLimit,3);

		bbzero(pyct_data->out_cardappinfo);
		memcpy(pyct_data->out_cardappinfo,(BYTE *)card_appinfo,40);
		wrHex("pyct_data->out_cardappinfo: ",(BYTE *)pyct_data->out_cardappinfo,40);
		

		//显示转移信息
		//show_yctthird_transforinfo();
		iRet = show_yctthird_transforinfo(pgbl->dPosInfo,pyct_data,trans_amo);
		if (iRet != 0)
		{
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
			return 5;
		}

		//P_TRANSFOR_CONFIRM_QRY()
		iRet = P_TRANSFOR_COMFIRM_QRY(pgbl->dPosInfo, com, comType,pyct_data,trans_amo,out_deductamo,in_chargeamo);

		if (iRet == 1)
		{
			*party = FROMCTSI;
			*rspcode = TRANSFER_ERROR;
			return 2;
		}
	}

	*party = FROMSUCC;
	*rspcode = SUCC_MSG;
	return 0;
}//Balance_Transfer


int CYCTBlnTransfer::P_TRANSFOR_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_pycicalInfo1,BYTE *transqry_type,BYTE *query_info,BYTE *last_transtan,BYTE *wallet_type,BYTE *party, BYTE *rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE SK[32+1]; //回话密钥
	int status=0;
	BYTE SAK[1+1];
	SOCKET sd=0;
	CConnect con;//连接类

	//报文中的参数；
	BYTE type [2]; //信息类型码
	BYTE ver [2]; //报文版本号
	BYTE RFU[35+1];
	BYTE  MAC [4];
	BYTE rsp_code[8+1];
	BYTE rsp_message[48];
	int out=0,out1=0;
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //报文序号
	BYTE md5_digest[16];
	BYTE mac_this[4];
	char temp[256];
	BYTE pack_data[104];
	int packLen=0;
	int len2=0;
	char outmsg[512];

	wrLog("【P_TRANSFOR_QRY】");
	
	bbzero(MAC);
	bbzero(mac_this);
	bbzero(temp);	
	bbzero(rsp_message);
	bbzero(rsp_code);
	bbzero(SHID);
	bbzero(SK);
	bbzero(SAK);
	bbzero(type);
	bbzero(ver);
	bbzero(RFU); //RFU 保留
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
	bbzero(wallet_type);
	bbzero(last_transtan);
	bbzero(query_info);

    len=0;//报文体长度 
	sndLen=0;//发送报文长度
	rcvLen = 0;

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(pack_data);

	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)pack_data+len,8); //pki卡号
	len+=4;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,(BYTE *)pack_data+len,16); //物理卡号
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,(BYTE *)pack_data+len,16);// //逻辑卡号
	len+=8;
	memcpy(pack_data+len,(BYTE *)card_pycicalInfo1,32);//卡物理信息1
	wrHex("卡物理信息1: ",pack_data+len,32);
	len+=32;
	wrHex("transqry_type: ",(BYTE *)transqry_type,1);
	memcpy(pack_data+len,(BYTE *)transqry_type,1);//转移类型
	//CTools::hex_to_byte((BYTE *)transqry_type,pack_data+len,2);//转移类型
	wrHex("转移类型: ",pack_data+len,1);
	len+=1;
	memcpy(pack_data+len,RFU,35);
	len+=35;
	wrHex("RTU",RFU,35);


	wrLog("len: %d",len);
	iRet = CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB11" , (BYTE*)"0001" , 0x02 , 0x80 , \
		 pack_data , len , rcvData , &rcvLen);

 		char  tag[4];	//信息类型码

		CTools::byte_to_hex((BYTE *)rcvData,(BYTE *)tag,2);

		if (iRet == 2)
		{
			wrLog("FE04报文，需要重新签到");
			CYCTYPTrade::yct_SignIn(dPosInfo,com,comType,pyct_data,party,(byte*)rsp_code);
			char cur_signInTime[30+1];
			char fmtDate[10+1];
			char fmtTime[10+1];
			bbzero(cur_signInTime);
			bbzero(fmtDate);
			bbzero(fmtTime);
			CTools::formatDate(pyct_data->SignIn_ServerTime,fmtDate);
			CTools::formatTime(pyct_data->SignIn_ServerTime+8,fmtTime);
			strcpy(cur_signInTime,fmtDate);
			strcat(cur_signInTime," ");
			strcat(cur_signInTime,fmtTime);
			CDB::updateYct3SignInfo((char *)dPosInfo.pos_no,pyct_data,cur_signInTime);
			//重新签到后如何处理
			P_TRANSFOR_QRY(dPosInfo, com, comType,pyct_data,card_pycicalInfo1,transqry_type,query_info,last_transtan,wallet_type,party, rspcode);
		}
		else	
		{
			len = 8;
			bbzero(pyct_data->SHID);
			CTools::byte_to_hex((BYTE *)rcvData+len,pyct_data->SHID,8);
			wrLog("握手流水号是: %s",pyct_data->SHID);
			len+=8;
			
			CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_code,4); //返回码
			wrLog("返回码为：%s",rsp_code);
			len+=4;
			if (memcmp(rsp_code,"00000000",8) != 0 && memcmp(rsp_code,"00000001",8) != 0)
			{
					char error_info[128];
					char byte_errinfo[128];
					int iSelect = 0;
					wrLog("交易返回码:%s",rsp_code);
					bbzero(error_info);
					bbzero(byte_errinfo);
					CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//错误信息描述
					CTools::tobyte(error_info,byte_errinfo);
					CTools::trim(byte_errinfo);
					wrLog("错误信息描述: %s",byte_errinfo);	
					bbzero(outmsg);
					sprintf(outmsg,"充值后台返回错误\n\n错误信息描述：%s",byte_errinfo);
					TS_OutPutMsg(dPosInfo,outmsg,1,iSelect);
					*party = FROMYCT;
					*rspcode = TRANSFER_ERROR;
					return 1;
			}

			if (memcmp(rsp_code,"00000001",8) == 0)
			{
				//该卡上次转移的充值没成功，将转到充值流程
				*party = FROMSUCC;
				*rspcode = LASTTRANSFER_NOTDONE;
			}

				//卡钱包类型
				memcpy(wallet_type,rcvData+len,1);//返回钱包类型
				wrHex("钱包类型 : ",wallet_type,1);
				len += 1;
				if (*wallet_type == 0x01 )
				{
					wrLog("M1钱包");
				}
				else
				{
					wrLog("CPU钱包");
				}
		
				bbzero(query_info);
				memcpy(query_info,rcvData+len,48); ////返回查询信息	
				wrHex("des_rcvData+5: ",rcvData+len,48);
				wrHex("返回查询信息为：",query_info,48);
				len+=48;
				memcpy(last_transtan,rcvData+len,8); //返回上次卡转流水（对应充值的扩展流水）
				wrHex("上次卡转流水: ",last_transtan,8);
				len+=8;
		}	

	return 0;
}

//转移确认查询
//in:  trans_amo     - 转移金额
//out: out_deductamo - 转出卡扣减金额
//     in_chargeamo  - 转入卡充值金额
int CYCTBlnTransfer::P_TRANSFOR_COMFIRM_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		char *trans_amo,BYTE *out_deductamo,BYTE *in_chargeamo)
{
	int iRet=0, len=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE RFU[64+1];
	BYTE rsp_code[4+1];
	BYTE temp[20+1];
    int iSelect = 0;
	char ticket_no[16+1];
	char showTicket_no[16+1];
	char out_ticketno[16+1];
	char out_showticketno[16+1];
	
	bbzero(out_ticketno);
	bbzero(out_showticketno);

	bbzero(ticket_no);
	bbzero(showTicket_no);

	wrLog("【P_TRANSFOR_COMFIRM_QRY】");
	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(rsp_code);
	bbzero(temp);

	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)sndData+len,8); //pki卡号
	len+=4;

	CTools::getHexAmo(trans_amo,(BYTE *)sndData+len);
	//CTools::hex_to_byte((BYTE *)trans_amo,8);
	wrHex("转移金额: ",sndData+len,4);//转移金额
	len+=4;

	//CTools::byte_to_hex((BYTE *)pyct_data->out_logicalcard,(BYTE *)temp,8);
//	CTools::hex_to_byte((BYTE *)pyct_data->out_logicalcard,(BYTE *)temp,16);// //转出卡逻辑卡号
	memcpy(sndData+len,(BYTE *)pyct_data->out_logicalcard,8);
//	CTools::hex_to_byte((BYTE *)pyct_data->out_logicalcard,(BYTE *)sndData+len,16);
	//wrHex("转出卡逻辑卡号: ",sndData+len,8); // by liuxiao
	CTools::byte_to_hex((BYTE * ) sndData+len,(BYTE *) ticket_no,8);
	CTools::shildWords(ticket_no,7,5,showTicket_no);

	wrLog("转出卡:%s",showTicket_no);
	len+=8;

	CTools::getHexAmo(pyct_data->out_balance,sndData+len);
	//memcpy(sndData+len,pyct_data->out_balance,4);
	wrHex("转出卡钱包余额: ",sndData+len,4);
	len+=4;

	memcpy(sndData+len,pyct_data->out_downLimit,1);
	wrHex("转出卡金额下限: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,pyct_data->out_upperLimit,3);
	wrHex("转出卡金额上限: ",sndData+len,3);
	len+=3;

	memcpy(sndData+len,pyct_data->out_cardappinfo,40);
	wrHex("转出卡应用信息: ",sndData+len,40);
	len+=40;

	//bbzero(temp);
//	CTools::byte_to_hex((BYTE *)pyct_data->out_logicalcard,(BYTE *)temp,8);
//	CTools::hex_to_byte((BYTE *)pyct_data->in_logicardno,(BYTE *)temp,16);// //转入卡逻辑卡号
	memcpy(sndData+len,(BYTE *)pyct_data->in_logicardno,8);
//	CTools::hex_to_byte((BYTE *)pyct_data->in_logicardno,(BYTE *)sndData+len,16);
//	wrHex("转入卡逻辑卡号: ",sndData+len,8);
	CTools::byte_to_hex((BYTE *) sndData+len,(BYTE *) out_ticketno,8);
	CTools::shildWords(out_ticketno,7,5,out_showticketno);
	wrLog("转入卡逻辑卡号:%s",out_showticketno);

	len+=8;
		
	CTools::getHexAmo(pyct_data->in_balance,sndData+len);
	//memcpy(sndData+len,pyct_data->in_balance,4);
	wrHex("转入卡钱包余额: ",sndData+len,4);
	len+=4;
	
	memcpy(sndData+len,pyct_data->in_downLimit,1);
	wrHex("转入卡金额下限: ",sndData+len,1);
	len+=1;
	
	memcpy(sndData+len,pyct_data->in_upperLimit,3);
	wrHex("转入卡金额上限: ",sndData+len,3);
	len+=3;
	
	memcpy(sndData+len,pyct_data->in_cardappinfo,40);
	wrHex("转入卡应用信息: ",sndData+len,40);
	len+=40;

	CTools::hex_to_byte((BYTE *)"02",sndData+len,2);
    //memcpy(sndData+len,(BYTE *)"01",1);
	wrHex("手续费类型: ",sndData+len,1);
	len+=1;

	memcpy(sndData+len,RFU,31);
	wrHex("RFU: ",sndData+len,31);
	len+=31;

	CYCTYPTrade::P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB13" , (BYTE*)"0001" , 0x02 , 0x80 , \
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
		char outMsg[512];
		char error_info[128];
		char byte_errinfo[128];
		wrLog("交易返回码:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//错误信息描述
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("错误信息描述: %s",byte_errinfo);	
		sprintf(outMsg,"羊城通后台返回错误\n\n错误信息描述：%s",byte_errinfo);
		TS_OutPutMsg(dPosInfo,outMsg,1,iSelect);
		return 1;
	}

	wrHex("系统时间: ",rcvData + len,7);
	len+=7;

	memcpy(out_deductamo,rcvData + len,4);
	wrHex("转出卡扣减金额: ",rcvData + len,4);
	len+=4;

	memcpy(in_chargeamo,rcvData + len,4);
	wrHex("转入卡充值金额: ",rcvData + len,4);
	len+=4;


	return 0;
}
	
//显示转移信息
int CYCTBlnTransfer::show_yctthird_transforinfo(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data,char *transfer_amo)
{
	int iSelect = 0;
	char temp_buff[1024];
	char amo[12+1];
	char out_amo[12+1];
	char in_amo[12+1];
	char temp_amo[4];	
	char mny[20+1];
	char out_mny[20+1];
	char in_mny[20+1];
	
	bbzero(temp_buff);
	bbzero(amo);
	bbzero(out_amo);
	bbzero(in_amo);

	//显示余额
	//显示转出卡余额
	//提示余额
	bbzero(out_mny);
	bbzero(temp_amo);
	memcpy(out_amo,pyct_data->out_balance,12);
	bbzero(pyct_data->out_balance_bef);
	memcpy(pyct_data->out_balance_bef,pyct_data->out_balance,12);
//	CTools::byte_to_hex((BYTE *)pyct_data->out_balance,(BYTE *)out_amo,4);
//	CTools::HexToDec(temp_amo,out_amo);
	CTools::chgToMny(out_amo, out_mny);
	wrLog("转出卡余额: %s", out_mny);
	bbzero( temp_buff );
	sprintf(temp_buff, "转出卡的余额为：%s\n按【确认】继续\n按【返回】取消", out_mny);
	TS_OutPutMsg(dPosInfo, temp_buff, 0, iSelect);
	if( iSelect!=0 )
	{
		return 1;// 5：扣费前失败
	}
	
	bbzero(in_mny);
	bbzero(temp_amo);
	memcpy(in_amo,pyct_data->in_balance,12);
//	CTools::byte_to_hex((BYTE *)pyct_data->in_balance,(BYTE *)in_amo,4);
	//CTools::HexToDec(temp_amo,in_amo);
	CTools::chgToMny(in_amo, in_mny);

	bbzero(mny);
	CTools::chgToMny(transfer_amo, mny);

	bbzero( temp_buff );
	sprintf(temp_buff, "转移前\n转入卡余额为：%s\n转出卡的余额为：%s\n\n转移金额为：%s\n\n", in_mny,out_mny,mny);
	
	
	bbzero(temp_amo);
	sprintf(temp_amo,"%12ld",atol(in_amo)+atol(transfer_amo));
	CTools::chgToMny(temp_amo, in_mny);
	
	bbzero(temp_amo);
	sprintf(temp_amo,"%12ld",atol(out_amo)-atol(transfer_amo));
	CTools::chgToMny(temp_amo, out_mny);
	
	sprintf(temp_buff+strlen(temp_buff),"转移后\n转入卡余额为：%s\n转出卡的余额为：%s\n按【确认】转移\t按【返回】取消",in_mny,out_mny);
	
	TS_OutPutMsg(dPosInfo, temp_buff, 0, iSelect);
	if( iSelect!=0 )
	{
		return 1;// 5：扣费前失败
	}

    return 0;
}


//type - 单据类型 0 - 转出卡圈提单据，1 - 转入卡圈存单据，2 - 转入卡圈存疑似成功单据
int CYCTBlnTransfer::set_blntransfer_priCont(int type,PYCT_DATA pyct_data,char *priCont)
{
	char fmt_date[10+1];
	char fmt_time[10+1];
	char trader_name[21];
	char note[256];
	char temp[64];
 
// 	bbzero(priCont);
// 	strcat(priCont, "=============================");
// 	strcat(priCont, "\n        岭南通单据        ");

	bbzero(pyct_data->trade_type);
	bbzero(trader_name);
	bbzero(note);
	bbzero(temp);
	bbzero(priCont);

    if (type == 0)
    {
		strcpy(pyct_data->trade_type,"余额转出");
    }
	else if (type == 1 || type == 2)
	{
		strcpy(pyct_data->trade_type,"余额转入");
	}


		
// 		strcat(priCont, "\n商户名称: ");
// 	//	CTools::hex_to_byte((BYTE *)pyct_data->trader_name,(BYTE*)trader_name,strlen(pyct_data->trader_name));
// 		strcat(priCont, pyct_data->trader_name);
// 		strcat(priCont, "\n商户编号: ");
// 		CTools::byte_to_hex((BYTE *)pyct_data->trader_id,(BYTE *)temp,8);
// 		strcat(priCont, temp);
// 		strcat(priCont, "\n终端编号: ");
// 		bbzero(temp);
// 		CTools::byte_to_hex((BYTE *)pyct_data->yct_termid,(BYTE *)temp,6);
// 		strcat(priCont, temp);
		strcat(priCont, "转出卡号: ");
		bbzero(temp);
		CTools::byte_to_hex((BYTE *)pyct_data->out_logicalcard,(BYTE *)temp,8);
		strcat(priCont, temp);
		strcat(priCont, "\n转入卡号: ");
		bbzero(temp);
		CTools::byte_to_hex((BYTE *)pyct_data->in_logicardno,(BYTE *)temp,8);
		strcat(priCont, temp);
		strcat(priCont, "\n交易类型: ");
		strcat(priCont, pyct_data->trade_type);
		strcat(priCont, "\n管 理 卡: ");
	//	bbzero(temp);
	//	CTools::byte_to_hex((BYTE *)pyct_data->pki_card,(BYTE *)temp,4);
		strcat(priCont, pyct_data->pki_card);

		//转换时间
		strcat(priCont, "\n转移时间: ");
		bbzero(fmt_time);
		bbzero(fmt_date);
		CTools::formatDate(pyct_data->tradeTime,fmt_date,3);
		CTools::formatTime(pyct_data->tradeTime+8,fmt_time);
		sprintf(priCont+strlen(priCont),"%s %s",fmt_date,fmt_time);
		
// 		strcat(priCont, "\n系统流水号: ");	
// 		bbzero(stan_temp);
// 		bbzero(temp);
// 		CTools::byte_to_hex((BYTE *)pyct_data->transfer_stan,(BYTE *)temp,8);
// 		CTools::HexToDec(temp,stan_temp,8);
// 		strcat(priCont, stan_temp);		

		if (type == 0)
		{
			sprintf(priCont+strlen(priCont), "\n转出卡原余额: RMB %.2f", atof(pyct_data->out_balance_bef)/100);
			sprintf(priCont+strlen(priCont), "\n转移金额: RMB %.2f", atof(pyct_data->amount)/100);
			sprintf(priCont+strlen(priCont), "\n转出卡余额: RMB %.2f", atof(pyct_data->out_balance_aft)/100);
		}
		else if (type == 1)
		{
			sprintf(priCont+strlen(priCont), "\n转入卡原余额: RMB %.2f", atof(pyct_data->in_balance_bef)/100);
			sprintf(priCont+strlen(priCont), "\n转入金额: RMB %.2f", atof(pyct_data->amount)/100);
			sprintf(priCont+strlen(priCont), "\n转入卡余额: RMB %.2f", atof(pyct_data->in_balance_aft)/100);
		}
		else
		{
			sprintf(priCont+strlen(priCont), "\n转入卡原余额: RMB %.2f", atof(pyct_data->in_balance_bef)/100);
			sprintf(priCont+strlen(priCont), "\n转入金额: RMB %.2f(*)", atof(pyct_data->amount)/100);
			sprintf(priCont+strlen(priCont), "\n转入卡余额: RMB %.2f(*)", atof(pyct_data->in_balance_aft)/100);
		}
		CTools::hex_to_byte((BYTE*)pyct_data->note,(BYTE*)note,strlen(pyct_data->note));
		strcat(priCont, "\n备    注: ");
		strncat(priCont, note,64);//备注信息最多32个汉字

		strcat(priCont, "\n注: ");
		strcat(priCont, "本单据仅供参考，不作结算依据\n");

		strcat(priCont,"\n此票不作报销凭证\n客服电话400-80-11888");
		strcat(priCont,"\n\n\n\n\n\n");

	return 0;
}
/****************卡间余额转移*********************/
