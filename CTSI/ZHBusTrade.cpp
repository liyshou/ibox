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
// 	//开机注册
// 	bbzero(theCommand);
// 	strcpy(theCommand, "珠海公交命令|开机注册|");
// 	iRet = TS_SendCommand(gbl.dPosInfo, theCommand);
// 	if(!iRet)
// 	{
// 		wrLog("开机注册成功");
// 	}
// 	else
// 	{
// 		wrLog("开机注册失败:%s", theCommand);
// 		
// 		bbzero(theCommand);
// 		strcpy(theCommand, "珠海公交命令|错误信息|");
// 		iRet = TS_SendCommand(gbl.dPosInfo, theCommand);
// 		if(!iRet)
// 		{
// 			wrLog(theCommand);
// 			iRet = CTools::findChar(theCommand, '|', loc);
// 			if(iRet > loc)//错误信息
// 			{
// 				char temp2[100];
// 				bbzero(temp);
// 				memcpy(temp2, theCommand + loc, iRet - loc);
// 				sprintf(temp, "注册失败，%s，请稍候再试。\n按【确认】退出", temp2);
// 				iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
// 				return -1;
// 			}
// 		}
// 		else
// 		{
// 			wrLog("充值失败:%s", theCommand);
// 			//！！！！！！！！！！！！！！！！！冲正E卡！！！！！！！！！！！！！！！！！！！！！！！！！！
// 			iRet = TS_OutPutMsg(gbl.dPosInfo, "公交充值失败，请稍候再试。\n按【确认】退出", 0, iSelect);
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
// 	strcpy(theCommand, "珠海公交命令|错误信息|");
// 	iRet = TS_SendCommand(gbl.dPosInfo, theCommand);
// 	if(!iRet)
// 	{
// 		wrLog(theCommand);
// 		iRet = CTools::findChar(theCommand, '|', loc);
// 		if(iRet > loc)//错误信息
// 		{
// 			char temp2[100];
// 			bbzero(temp);
// 			memcpy(temp2, theCommand + loc, iRet - loc);
// 			sprintf(temp, "注册失败，%s，请稍候再试。\n按【确认】退出", temp2);
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


//包处理函数:打包、发包、收包和解包
// 入参：flag:2:交费包;4:确认包;5:冲正包
// 出参：无
// 返回值
// 0：成功
// 1：打包失败，扣费失败
// 2：超时，结果未知
// 3：解包失败，结果未知
int ZHBusTrade::packHdl(CGlobal gbl, CPack *ppack, int flag)
{
	BYTE temp_buff[1024];
	char send_buff[1024], rev_buff[1024+1], len_buff[4+1];
	char seq[10+1]; //数据库操作时产生的序列号
	int send_len, iRet, rev_len;
	int i, type, stat;
	CConnect con;	//连接类
	//打包   
	memset(temp_buff,0,sizeof(temp_buff));
	
	send_len = ppack->Comm_Pack(flag, temp_buff);
	
	sprintf(len_buff,"%04d",send_len);
	if (send_len <=0 || send_len>1024)
	{
		wrLog("8583包打包错误\n");
		return 1;// 1：打包失败
	}
	memset(send_buff,0,sizeof(send_buff));
	CTools::hex_to_byte((BYTE*)len_buff, (BYTE*)send_buff, 4);
	memcpy(send_buff+2,temp_buff,send_len);
	send_len +=2;
	
	//插入记录
	bbzero(seq);
	i = 0;
	if(flag < 4)  //确认和冲正不写数据库，2007-3-15
	{
		while( CDB::setEposTxn(flag, gbl, *ppack, seq) && i<3 )  //错误重试3次
			i++;
	}
	if (flag==2) //edit by liuxiao 
	{
		bbzero(gbl.order_time);
		CDB::getDBtime(gbl.order_time);
		wrLog("订单发起的时间为:%s",gbl.order_time);
	}
	//上送数据包
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
		wrMonitorLog(type, 2, iRet, gbl, *ppack);		//监控日志
		return 2;// 2：超时
	}
	bbzero(len_buff);
	CTools::byte_to_hex((BYTE *)rev_buff, (BYTE *)len_buff, 2);
	rev_len =atol(len_buff);
	
	//解包
	if( ppack->unpackFunc((BYTE *)rev_buff,rev_len) != TRUE)
		return 3;// 3：解包失败
	//更新记录
	if( strcmp(seq, "") )
	{
		if(flag == 2)
			CDB::setEposTxn(22, gbl, *ppack, seq);
	}
	//监控日志
	if(atoi((char *)ppack->FIELD39))
		stat = 3;
	else
		stat = 0;
	if(type>0)
		wrMonitorLog(type, stat, atoi((char *)ppack->FIELD39), gbl, *ppack);
	return 0;// 0：成功
}

// 刷卡输密码向PE发扣费包
// 入参：biz_type(4):业务代码；amount(12):金额
// 出参：无
// 返回值
// 0：扣费成功
// 1：扣费失败
// 2：超时返回
int ZHBusTrade::PayPe(CGlobal *pgbl, char *card_no)
{
	int  iRet, flag,mngCard_count=1,iSelect;
	int b48_len = 0;
	char len_buff[10];
	CTrade trade;
	CPack pack;
	
	wrLog("【*******PE扣费*******】");	
	
	//传递信息,取得刷卡数据
	trade.set_in_par(*pgbl);
	flag = 0;	//重输密码标识
	/********************请求刷卡****************/
	mngCard_count=1;
	for(int maxTry=0; maxTry<50; maxTry++)	//密码错误可重输
	{
		if(!flag)
		{
			iRet = trade.mngCard_Hdl("羊城通充值");
			if(iRet == CANCLE_KEY || iRet != 0)
				return 1;
		}
		trade.get_gbl(*pgbl);	//取得在trade类中获得的信息
		pack.wrField(2, *pgbl);	//写交费域
		/***********修改一些域信息*********/
		pack.FIELD46[0] = 0x39;
		pack.FIELD46[1] = 0x39;
		pack.Packet_Table[46].Real_Length = 2;
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,pgbl->fee_type,4);	//4位信息码
		b48_len = 4;
		memset(len_buff,0,sizeof(len_buff));
		sprintf(len_buff,"%02d", 9);	//卡号长度
		memcpy(pack.FIELD48+b48_len,len_buff,2);	
		b48_len +=2;
		memcpy(pack.FIELD48+b48_len, card_no, 9);	//羊城通逻辑卡号
		b48_len += 9;
		pack.Packet_Table[48].Real_Length=b48_len;
		/**********************************/
		
		//交费包处理
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
			if(trade.reGetPwd( "珠海公交充值" ) == 0) //密码错误，重新输入密码
			{
				continue;
			}
			else
				return CANCLE_KEY;
		}
		else if(!memcmp(pack.FIELD39,"14",2)||!memcmp(pack.FIELD39,"15",2)||!memcmp(pack.FIELD39,"33",2)||!memcmp(pack.FIELD39,"35",2)||!memcmp(pack.FIELD39,"36",2)||!memcmp(pack.FIELD39,"56",2)||!memcmp(pack.FIELD39,"57",2)||!memcmp(pack.FIELD39,"60",2)||!memcmp(pack.FIELD39,"62",2)||!memcmp(pack.FIELD39,"91",2))
		{
			TS_OutPutMsg(pgbl->dPosInfo,"\n     不支持的卡\n   请换卡重新刷卡", 0, iSelect);
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

// 确认交易
// 入参：yct_data:羊城通交易信息
// 出参：无
// 返回值：
// 2：已确认
// 3：未确认
int ZHBusTrade::AckTrade(CGlobal gbl, char *card_no,char *serial,char *balance,char *deviceId)
{
	int b48_len, b63_len, iRet;
	char lenBuff[4+1];
	CPack pack;

	for (int i=0;i<3;i++)
	{
		wrLog("【*******PE确认*******】");
		//写确认域
		pack.wrField(4, gbl);
		/************补充48域和63域*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,"0358",4);	//4位信息码
		b48_len = 4;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff,"%02d", 9);	//卡号长度
		memcpy(pack.FIELD48+b48_len, lenBuff, 2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, card_no, 9);//羊城通逻辑卡号
		b48_len += 9;
// 		memcpy(pack.FIELD48+b48_len, "|", 1);
// 		b48_len += 1;
// 		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);//羊城PKI卡卡号
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
		//确认包打包
		iRet = packHdl(gbl, &pack, 4);
		if( iRet==0 && memcmp(pack.FIELD39,"00",2)==0 )
		{
			return 2;// 2：已确认
		}
		else
		{
			;//确认不成功继续确认，直至三次确认次数满
		}
	}
	return 3;// 3：未确认


}

// 冲正交易
// 入参：yct_data:羊城通交易信息
// 出参：无
// 返回值：
// 5：已冲正
// 6：未冲正
int ZHBusTrade::RevTrade(CGlobal gbl, char *card_no)
{
	int b48_len, iRet;
	char lenBuff[4+1];
	CPack pack;

	for(int i=0; i<3; i++)//冲正不成功发3次
	{
		wrLog("【*******羊城通冲正[%d]*******】", i);
		//写冲正域
		pack.wrField(5, gbl);
		/************补充48域*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,"0358",4);	//4位信息码
		b48_len = 4;
		memcpy(pack.FIELD48+b48_len, gbl.pos_serial, 6);	//交费时6位原终端流水号
		b48_len += 6;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff, "%02d", 9);	//卡号长度
		memcpy(pack.FIELD48+b48_len,lenBuff,2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, card_no, 9);//羊城通逻辑卡号
		b48_len += 9;
		pack.Packet_Table[48].Real_Length = b48_len;
		/***********************************/
		//确认包打包
		iRet = packHdl(gbl, &pack, 5);
		if( iRet==0 )
		{
			if( memcmp(pack.FIELD39,"00",2)==0 )
			{
				return 5;// 5：已冲正
			}
			else
			{
				return 6;// 6：未冲正
			}
		}
		else
		{
			NULL;
		}
	}
	return 6;// 6：未冲正
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

	//提示信息
	iRet = TS_OutPutMsg(gbl.dPosInfo, "请指引客户将公交卡正确放置在读卡器上，且勿移动它。\n按【确认】继续\n按【返回】退出", 0, iSelect);
	if(!iRet && !iSelect)
	{
		NULL;
	}
	else
	{
		return -1;
	}

	//开机注册
	bbzero(errMsg);
	iRet = TS_ZHBusPosLogin(gbl.dPosInfo,errMsg);
	if(!iRet)
	{
		wrLog("开机注册成功");
	}
	else
	{
		wrLog("开机注册失败:%s", errMsg);
		iRet = TS_OutPutMsg(gbl.dPosInfo, "开机注册失败，请稍候再试。\n按【确认】退出", 0, iSelect);
		return -1;
	}

	//签到
	bbzero(errMsg);
	iRet = TS_ZHBusSignIn(gbl.dPosInfo, "00000660","11111111",errMsg);
	if(!iRet)
	{
		wrLog("签到成功");
	}
	else
	{
		wrLog("签到失败:%s", errMsg);
		iRet = TS_OutPutMsg(gbl.dPosInfo, "公交充值失败，请稍候再试。\n按【确认】退出", 0, iSelect);
		return -1;
	}

	//余额信息
	bbzero(out_in_date);
	bbzero(out_cardType);
	bbzero(errMsg);
	iRet = TS_ZHBusQuery(gbl.dPosInfo, old_balance, card_no, out_in_date, out_cardType, errMsg);
	if(!iRet)
	{
		CTools::longToMny(old_balance, str_old_balance);
 		sprintf(str_card_no, "%09d", card_no);
		wrLog("查询成功,卡号[%s],余额[%s],有效日期[%s],卡类型[%s]", str_card_no, str_old_balance, out_in_date, out_cardType);
	}
	else
	{
		bbzero(temp);
		sprintf(temp,"查询余额失败:%s", errMsg);
		iRet = TS_OutPutMsg(gbl.dPosInfo, temp, 0, iSelect);
		return -1;
	}
	
	bbzero(temp);
	sprintf(temp, "卡号：%s\n余额：%s元\n\n按【确认】充值  按【取消】退出", str_card_no, str_old_balance);

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
	
	strcpy(menuList.acTitle, "请选择充值金额:");
	menuList.iTotalNum = 5;
	
	strcpy(menuList.menuItem[0], "50元");
	strcpy(menuList.menuItem[1], "100元");
	strcpy(menuList.menuItem[2], "150元");
	strcpy(menuList.menuItem[3], "200元");
	strcpy(menuList.menuItem[4], "5元");
	
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
	sprintf(temp, "卡号：%s\n余额：%s元\n充值金额：%.2f元\n\n按【确认】充值  按【取消】退出", str_card_no, str_old_balance, atof(gbl.amount)/100);
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

	//返回数据给终端
	if( iRet==0 )//成功
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
			sprintf(temp,"公交充值失败:%s，请稍候再试。\n按【确认】退出",errMsg);
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
		sprintf(temp, "充值成功！！\n卡号：%s\n充值流水：%s\n原有余额：%s元\n充值金额：%.2f元\n充值后余额：%s元\n\n按【确认】打印发票", 
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

	//PE确认
	//显示充值结果
	//打印

	return 0;
}