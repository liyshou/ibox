// MenuHdl.cpp: implementation of the CMenuHdl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "MenuHdl.h"
#include "DB.h"
#include "TSCTSI.h"
#include "Trade.h"
#include "Pack.h"
#include "Print.h"
#include "TissonCTSIDlg.h"
#include "YCTYPTrade.h"
#include "YCT3_BalanceTransfer.h"
#include "YCT3Trade.h"
#include "ZHBusTrade.h"
#include "AES.h"
#include "md5.h"


#include <process.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//CGlobal gbl;	//线程全局变量

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuHdl::CMenuHdl()
{
	sd = 0;
	memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
	wrLog("【时间统计】开始");
}

CMenuHdl::~CMenuHdl()
{
	wrLog("【时间统计】结束");

//	wrLog("关闭SOCKET,服务线程退出...");
	TS_HangUp(gbl.dPosInfo);

	//closesocket(sd);
	wrLog("【~CMenuHdl】");
	
}

int CMenuHdl::yct3_flag = 0;

// 返回：0, 无不完整交易，其他，失败
// int CMenuHdl::isYctNotFull(char *pos_no)
// {
// 	CYCTTrade yct;
// 	char yctStan[8+1], seq[10+1];
// 	char hexAmo[8+1], stat[4+1], yctCard[16+1];
// 	int i, ret, iSelect;
// 	CGlobal tmpGbl=gbl;
// 	
// 	bbzero(seq);
// 	bbzero(yctStan);
// 	yct.setInPar(tmpGbl);
// 	memcpy(&tmpGbl, &gbl, sizeof(tmpGbl));
// 	i = CDB::getYctStat(pos_no, yctStan, seq, tmpGbl.stan_no);
// 	if( i == 0)  //有不完整交易
// 	{
// 		yct.setStan( yctStan );
// 		bbzero(stat);
// 		ret = yct.notFullHdl(stat);
// 		if( ret == 0 )  //充值成功
// 		{
// 			CDB::setYctStat(1, seq); //设为未确认状态
// 			CDB::updateYctAck(seq,stat);
// 			CPack pack;
// 			bbzero(pack.FIELD62);
// 			memcpy(pack.FIELD37, tmpGbl.stan_no, 12);
// 			wrMonitorLog(14, 0, 0, gbl, pack);
// 			bbzero(hexAmo);
// 			bbzero(stat);
// 			bbzero(yctCard);
// 			bbzero(seq);
// 			if( CDB::getYctAck(tmpGbl.stan_no, tmpGbl.pos_no, tmpGbl.pos_serial, tmpGbl.amount, hexAmo, yctCard, stat, seq) )
// 				return 0;
// 			yct.setInPar(tmpGbl);
// 			if( yct.YCTAckTrade(yctCard, stat, hexAmo)==0 ) //确认
// 				CDB::setYctStat(0, seq);			
// 			TS_OutPutMsg(gbl.dPosInfo, "  \n\n     充值成功！", 1, iSelect);
// 			return 0;
// 		}
// 		else if( ret == 1 ) //充值失败
// 		{
// 			CDB::setYctStat(3, seq); //设为未冲正状态
// 			CPack pack;
// 			bbzero(pack.FIELD62);
// 			memcpy(pack.FIELD37, tmpGbl.stan_no, 12);
// 			wrMonitorLog(14, 1, 6, gbl, pack);
// 			bbzero(seq);
// 			bbzero(tmpGbl.pos_no);
// 			bbzero(tmpGbl.pos_serial);
// 			bbzero(tmpGbl.card_no);
// 			bbzero(tmpGbl.amount);
// 			bbzero(yctCard);			
// 			if( CDB::getYctRev(seq, tmpGbl.pos_no, tmpGbl.pos_serial, tmpGbl.card_no, tmpGbl.amount, yctCard) )
// 				return 0;
// 			tmpGbl.card_no_len = strlen(tmpGbl.card_no);
// 			yct.setInPar(tmpGbl);
// 			if( yct.YCTRevTrade(yctCard) )
// 				NULL;
// 			else
// 				CDB::setYctStat(2, seq);			
// 			TS_OutPutMsg(gbl.dPosInfo, "网络不稳定引起充值失败！\n请稍候再试!\n 如果交易结果不明确请做 最后一笔交易查询", 1, iSelect);
// 			return 0;
// 		}
// 		else  //不完整交易失败，需再做不完整交易
// 		{
// 			TS_OutPutMsg(gbl.dPosInfo, "网络不稳定引起不完整的交易！\n请稍候再试!\n 如果交易结果不明确请做 最后一笔交易查询", 1, iSelect);
// 			return -1;
// 		}
// 	}
// 	else if( i == 1)
// 		return 0;
// 	else
// 	{
// 		TS_OutPutMsg(gbl.dPosInfo, " \n网络不稳定,数据库错误\n请稍候再试!\n 如果交易结果不明确请做 最后一笔交易查询", 1,iSelect);
// 		return -1;
// 	}
// 	
// }

int CMenuHdl::Menu_main_hdl(Coninfo coninfo)
{
	TSMENULIST menuList;	//菜单结构体
	CTrade	trade;
	int list[30] = {0};		//每次下发菜单的编号排列
	int	par_menu;			//父菜单
	int cur_menu;			//当前菜单
	int iRet = 0;
	int iRet2=0;			//	用户是否已经同意。
	int iSelect = 0;		//菜单选项
	int loop = 1;			//循环标识
	int enc_logon_lag = 0;
	int menu_id;			//本地菜单上送的菜单ID
	int main_group = 1;			//主菜单的组别号
	char main_ver[8+1];		//主菜单的版本号
	char menu_ver[8+1];     //菜单版本号
	char usbid[16+1];

	sd = coninfo.sd;		//取得终端socket
	bbzero(gbl.ip);
	sprintf(gbl.ip, "%d.%d.%d.%d", coninfo.s_b1, coninfo.s_b2, coninfo.s_b3, coninfo.s_b4);

	bbzero(main_ver);
	memset(&gbl.posInfo, 0, sizeof(gbl.posInfo));
	memset(&gbl.dPosInfo, 0, sizeof(gbl.dPosInfo));
	iRet = TS_GetPosInfo(sd, &gbl.posInfo, &gbl.dPosInfo, gbl.askType, menu_id, CARD_TYPE);	//获取终端信息
	
	if( iRet )
	{
		CPack pack;
		wrLog("获取终端信息错误！");
		wrMonitorLog(10, 10, iRet, gbl, pack);	//监控日志
		TS_OutPutMsg(gbl.dPosInfo, "\n\n   获取终端信息错误！\n请稍候再试!", 1,iSelect);

		return -1;
	}


	bbzero(gbl.pos_no);
	bbzero(gbl.call_tel);
	bbzero(usbid);
	bbzero(thisPrmMD5);
	strcpy(gbl.pos_no, gbl.posInfo.posNum);
	if(atoi(gbl.dPosInfo.prmVer)>=9000)
	{
		memcpy(usbid, gbl.posInfo.telNum, 16);
		memcpy(gbl.call_tel, gbl.posInfo.telNum+16, 16);
		memcpy(thisPrmMD5, gbl.posInfo.telNum+32, 16);
	}
	else
	{
		strcpy(usbid, "0000000000000000");
		strcpy(gbl.call_tel, gbl.posInfo.telNum);
		strcpy(thisPrmMD5, "0000000000000000");
	}
	strcpy( log.g_pos_no, gbl.pos_no );	//设置线程的终端号变量(各线程独立，用于日志识别)
	log.log_sd = sd;
				 
	iRet2=CDB::getAgreeFlag(&gbl.agreeFlag,gbl.pos_no);
	
	if(iRet2)
	{
		
		wrLog("session thread");
		return -1;
	}
    
	
	wrLog("-------------------------");
	wrLog("USBID[%s]", usbid);
	wrLog("电话号码[%s]", gbl.call_tel);
	wrLog("程序MD5[%s]", thisPrmMD5);
	wrLog("终端号[%s]", gbl.pos_no);
	wrLog("IP[%s]", gbl.ip);
	wrLog("请求类型[%d]", gbl.askType);
	wrLog("左串口设备[%d]", gbl.posInfo.leftType);
	wrLog("右串口设备[%d]", gbl.posInfo.rightType);
	wrLog("内置设备[%d]", gbl.posInfo.inType);
	wrLog("程序版本[%s]", gbl.posInfo.prmVer);
	wrLog("应用版本[%s]", gbl.posInfo.funVer);
	wrLog("加密类型[%d]", gbl.posInfo.enyFlag);
    wrLog("用户是否确认[gbl.agreeFlag]:%d",gbl.agreeFlag);

	
	//bbzero(gbl.DIY_URL);
	bbzero(gbl.CS_URL);
	//DCN内网链接
	if(!memcmp(gbl.pos_no+4, "99", 2) || strcmp(gbl.ip,"132.97.45.31")==0)
	{
		//DIY http://132.97.45.31:1301
		//客服网站 http://132.97.45.31:1458
		//strcpy(gbl.DIY_URL, DIY_DCN_URL);
		strcpy(gbl.CS_URL, CS_DCN_URL);
	}
	else
	{
		//客服http://121.33.197.198:1458
		//diy  http://diy.payeasy.cn
	//	strcpy(gbl.DIY_URL, DIY_GBL_URL);
		strcpy(gbl.CS_URL, CS_GBL_URL);
	}
	
    int encrypt_type=0;
	iRet = CDB::getEncryptType(&encrypt_type,gbl.pos_no);
	wrLog("数据库中记录的加密类型[%d]，iRet[%d]", encrypt_type,iRet);
	if(iRet==-1)
	{
		if(gbl.posInfo.enyFlag==0)
          CDB::setEncryptType(gbl.posInfo.enyFlag,0,gbl.pos_no);
	}else if(iRet==0){
		if(gbl.posInfo.enyFlag==0||(gbl.posInfo.enyFlag==1&&encrypt_type==0))
          CDB::setEncryptType(gbl.posInfo.enyFlag,1,gbl.pos_no);
	}


	bbzero(gbl.server_ip); //获取服务器ip
	CConnect::getIP(gbl.server_ip);
	wrLog("服务器IP:[%s]",gbl.server_ip);



	if(memcmp(gbl.pos_no, "68", 2)==0 && menu_id==121)
	{
		menu_id = 110;
	}

	if(atoi(gbl.dPosInfo.prmVer)>=9000)
	{
		int prtCom=0, prtType=0;
		if( CPrint::getPrintPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &prtCom, &prtType)!=0 )//如果IPOS，没有打印机
		{
			gbl.posInfo.leftType = 2;//热敏非套打
		}
	}

	//获取终端信息
	bbzero(gbl.curworkid);
	CPack pack;
	iRet = CDB::getEposTerm(gbl.pos_no, gbl.call_tel, &gbl.tgroup, &gbl.group_type, &gbl.phs_ticket, &gbl.get_card, gbl.mer_ecard, &enc_logon_lag, gbl.welcome, gbl.curworkid, &gbl.dPosInfo, usbid, &gbl.dayAmo, gbl.city_no, gbl.acc_city_no, &gbl.title_flag, gbl.pki_card, gbl.signin_serial, &gbl.DianXin_Turn, &gbl.Reversal_count, &gbl.term_level,gbl.update_messag, &gbl.input_times, &gbl.print_paper, &gbl.commission_free,&gbl.HID_TYPE,&gbl.bus_trade_flag);	
	wrLog("东莞清华同方读卡器设备[%d]",gbl.HID_TYPE);

	//系统暂停
	char service_Hour[6+1];
	char service_time[14+1];
	bbzero(service_Hour);
	bbzero(service_time);
	CDB::getDBtime(service_time);
	memcpy(service_Hour,service_time+8,6);
	wrLog("数据库时间为:%s",service_Hour);


	if(atoi(service_Hour)>=service_stop ||atoi(service_Hour)<=service_start)
	{
		TRADE_STAT=1; //暂停服务	
	}
	else
	{
	
		TRADE_STAT=0;
	}

	if(TRADE_STAT)  
	{
		char stips[301], sflag[2];
		wrLog("【程序切换中，线程退出1】");
	
		 if(atoi(service_Hour)>=service_stop ||atoi(service_Hour)<=service_start)
			
		 {
			TS_OutPutMsg(gbl.dPosInfo, "系统日结中，请稍后再试", 1, iSelect);
		 }
		 else
		 {

			 bbzero(stips);
			 get_epos_tips(gbl.pos_no, 0, sflag, stips);
			 if(strlen(stips) > 10) //长度小于10，认为是无效提示语
			 {
				 
				 TS_OutPutMsg(gbl.dPosInfo, stips, 1, iSelect);
			 }
			 
			 else
			 {
				 TS_OutPutMsg(gbl.dPosInfo, "系统维护中，请稍候再试...", 1, iSelect);
				 
			 }

	
		}

		return 0;

	}


	//本地金融菜单－操作码包含发送数据
	if( gbl.askType == 9 ){
		gbl.get_card = 1;//如果为本地金融菜单上来，刷卡标志定死为多次刷卡
		TSTradeInfoSend tradeInfoSend;//设置金融菜单对应的输入数据
		memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
		memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
		tradeInfoSend.uTotalContents = 3;
		tradeInfoSend.Contents[0].uCommand = TS_INPUT_MONEY;	//读取金额
		tradeInfoSend.Contents[1].uCommand = TS_MAG_CARD;		//刷卡
		tradeInfoSend.Contents[2].uCommand = TS_INPUT_PIN;		//输密码
		iRet = TS_ReadLocalCont(gbl.dPosInfo, &tradeInfoSend, &tradeInfoRecv);//发消息给FSK，通知FSK去接收终端的报文
		if( iRet )
		{
			TS_OutPutMsg(gbl.dPosInfo, " \n\n  提取用户操作信息失败，转帐不成功！\n请稍候再试!\n 如果交易结果不明确请做 最后一笔交易查询", 1,iSelect);
			if(iRet<0)
				iRet = 7;
			wrMonitorLog(10, 10, iRet, gbl, pack);	//监控日志		
			
			return -1;
		}
	}

	if( iRet!=0 )
	{
		wrLog("非法的终端信息 iRet=[%d]", iRet);
		TS_OutPutMsg(gbl.dPosInfo, " 非法的终端信息！\n请稍候再试!\n或按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询", 1,iSelect);
		if(iRet!=0)
			iRet = 7;	
		wrMonitorLog(10, 10, iRet, gbl, pack);//监控日志
		
		return -1;
	}
	else
		wrMonitorLog(10, 0, 0, gbl, pack);		//监控日志

 	if (gbl.askType == 10)
	{
		iRet = get_menu_group_mbl_flag(gbl.tgroup,0,&gbl.menu_group_mbl_flag);
	}else
		iRet = get_menu_group_mbl_flag(gbl.tgroup,gbl.askType,&gbl.menu_group_mbl_flag);
	if (iRet<0)
	{
		gbl.menu_group_mbl_flag = 0;
	}
	wrLog("gbl.menu_group_mbl_flag:[%d]",gbl.menu_group_mbl_flag);
	char tmp[1024];
	
	if(	strlen(gbl.welcome) != 0 )
	{
		if (atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			NULL;
		}else{
			bbzero(tmp);
			strcat(tmp,gbl.welcome);
			TS_OutPutMsg(gbl.dPosInfo,tmp, 1, iSelect);
		}
		
	}

	if(atoi(gbl.dPosInfo.prmVer)>=9000 && (atoi(prmVer)>atoi(gbl.dPosInfo.prmVer) || strstr(prmMD5, thisPrmMD5)==NULL && strstr(prmMD5, "0000000000000000")==NULL))//需要更新程序
	{
		if(	strlen(gbl.update_messag) != 0 )
		{//如果有更新 则提示更新所需要注意的事情
			bbzero(tmp);
			strcat(tmp,gbl.update_messag);
			TS_OutPutMsg(gbl.dPosInfo,tmp, 1, iSelect);			
		}
		UpdateFiles();
	}

// 	//判断终端是否有羊城通不完整交易
// 	if( isYctNotFull(gbl.pos_no) )
// 	{
// 		return -1;
// 	}
 	
	if(gbl.posInfo.rightType == 8)
	{
		if(YCT_VER==3)
		CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data);	
//		CYCTYPTrade::termMonitor(&gbl, &yct_data);
	}

	//检查并处理羊城通未知结果交易
	if(gbl.askType == 10)
	{
		if( CYCTYPTrade::NotFinishChk(gbl, gbl.pos_no, gbl.pki_card, gbl.signin_serial)!=0 )
		{
			return -1;
		}
	}



	/*********第一次签到********/
	wrLog("logon_flag = %d", logon_flag);
	trade.set_in_par(gbl); //传递参数
	if(logon_flag == 0 && !gbl.posInfo.enyFlag)	//加密改造，旧加密终端签到
	{
		if( trade.logonHdl() == 0)  //20070206, 若签到成功，置签到标志为1
			logon_flag = 1;
		else
			return -1;
	}

	if(atoi(gbl.dPosInfo.prmVer)>=9000)
	{
		NULL;
	}
	else
	{
		if(gbl.posInfo.enyFlag && enc_logon_lag != 0 ){
			if(trade.logonHdl(enc_logon_lag)==0)
			{
				CDB::setEposLogonStatus(1, gbl.pos_no);
			}
			else
			{
				//TS_HangUp(gbl.dPosInfo);
				//closesocket(sd);				//标记一下
				return -2;
			}
			char mac16[16+1];
			bbzero(mac16);
			iRet = TS_GetMacKey(gbl.dPosInfo, mac16);
			if(iRet)
			{
				//TS_HangUp(gbl.dPosInfo);
				//closesocket(sd);			//标记一下
				return -3;
			}else
			{
				memcpy(gbl.dPosInfo.macKey, mac16, sizeof(gbl.dPosInfo.macKey)-1);
			}		
		}
	}	

	if(atoi(gbl.dPosInfo.prmVer)<9000)//非软终端
	{
		trade.getEcardMsg("e卡充值信息"); //显示e卡充值提示
	}

	if(gbl.get_card	== MNG_ONE || gbl.get_card == MNG_ONE_BIND || gbl.get_card == MNG_ONE_BIND_E)	//一次刷卡的第一次刷卡
	{
		iRet=1;
		trade.set_in_par(gbl); //传递参数	
		while(iRet > 0)	//刷卡错误，重新刷卡
		{
			char card_info[200];
			bbzero(gbl.fee_type);
			memcpy(gbl.fee_type, "-1", 2);
			bbzero(card_info);
			iRet = trade.getCardInfo("交费易", card_info, MNG_OR_INPUT, gbl.get_card);//刷卡取得卡信息
			if( iRet == 0 )
				iRet = trade.getCardNum(card_info, MNG_OR_INPUT);//取得卡号
			if(iRet == RE_MNG_CARD)
				continue;
			else if(iRet != 0)
				return -1;
			trade.get_gbl( gbl );  //取得在CTrade类中获得的公用变量
		}
	}
	
	iRet = CDB::getMenu(gbl.tgroup, gbl.askType, gbl.bank, &gbl.menu_group, &main_group, main_ver, menu_ver);
	if( iRet )
	{
		TS_OutPutMsg(gbl.dPosInfo, " \n\n  无法找到该终端菜单！\n请按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询", 1, iSelect);
		return -1;
	}else
	{
		if( gbl.askType>10){
			main_group = gbl.menu_group;
			strcpy(main_ver,menu_ver);
		}
		wrLog("【gbl.menu_group】【%d】", gbl.menu_group);
		wrLog("【main_group】【%d】", main_group);
		wrLog("【main_ver】【%s】", main_ver);
		//将gbl重新传入trade
		trade.set_in_par(gbl);
	}

	//新加密的最新版本

	if( gbl.posInfo.enyFlag==1 && ( memcmp(main_ver, gbl.posInfo.funVer, 3)!=0 || memcmp(main_ver+3, gbl.posInfo.funVer+3, 5)>0 ) && atoi(gbl.dPosInfo.prmVer)!=1001 && atoi(gbl.dPosInfo.prmVer)!=1002 && atoi(gbl.dPosInfo.prmVer)>=200 )
	{
		//更新该终端的主菜单组号
		iRet = trade.UpdateMenus(main_group, main_ver[0]);
		if(iRet==0)
		{
			//更新菜单版本
			TSPosPar tsposPar;
			tsposPar.num=1;
			tsposPar.parCont[0].parId=8;
			tsposPar.parCont[0].parLen=4;
			bbzero(tsposPar.parCont[0].parCont);
			CTools::hex_to_byte((const unsigned char *)main_ver, (unsigned char *)tsposPar.parCont[0].parCont, 8);
			iRet = TS_UpdatePosPar(gbl.dPosInfo, &tsposPar);
			if(iRet)
			{
				wrLog("将版本号更新为[%s]失败", main_ver);
			}
			else
			{
				NULL;
			}
		}
		else
		{
			NULL;
		}
	}

	if( gbl.askType>=9 )
	{
		if(gbl.group_type==0 && atoi(gbl.dPosInfo.prmVer)<9000)//如果是商户且不是软终端
		{
			for(int i=0; i<MAXTRY; i++)
			{
				wrLog("【进入一项本地菜单】【%d】", menu_id);
				oper_hdl(menu_id);			
				
				TSMENULIST menuList;
				memset(&menuList,0,sizeof(menuList));
				menuList.iTotalNum = 3;
				strcpy(menuList.acTitle, "请选择");
				strcpy(menuList.menuItem[0], epos_menu[menu_id].name);
				strcpy(menuList.menuItem[1], "主菜单");
				strcpy(menuList.menuItem[2], "退    出");
				//strcpy(menuList.menuItem[2],"退出");
				
				iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
				if(iRet)
				{
					wrLog("下发挂机命令");	//下发挂机命令
					return -1;
				}
				
				if(iSelect==1)
				{
					continue;
				}else if(iSelect==2)
				{
					gbl.askType = 0;
					break;
				}else
				{
					wrLog("下发挂机命令");	//下发挂机命令
					return CANCLE_KEY;
				}
			}		
		}
		else
		{
			wrLog("【进入一项本地菜单】【%d】", menu_id);
			oper_hdl(menu_id);
 			wrLog("下发挂机命令");	//下发挂机命令
			
			//wrLog("启动终端监控线程，暂未启动！");

			//TS_HangUp(gbl.dPosInfo);
 			//closesocket(sd);				//标记一下

// 			wrLog("终端监控开启……");
// 			HANDLE termMonitor = CreateThread(NULL , 1000 , YCT3Monitor, (LPVOID)&gbl , 0 , NULL);
// 			CloseHandle(termMonitor);

 			return CANCLE_KEY;
		}		
	}

	//取菜单
	par_menu = 0;  //初始为根菜单
	cur_menu = 0; 
	iRet = get_menu_list(par_menu, &menuList, list);
	if( iRet )
	{	
		TS_OutPutMsg(gbl.dPosInfo, "\n\n没有找到本终端菜单！\n请按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询", 1, iSelect);
		return -1;
	}


	if(list[2] < 0)	//只有一项业务的话直接进入该业务
	{
		wrLog("【进入一项菜单】【%d】", list[1]);
		oper_hdl(list[1]);			
		//TS_HangUp(gbl.dPosInfo);
		//closesocket(sd);						//标记一下
		return CANCLE_KEY;
	}

	wrLog("【进入一组菜单】【%d】", gbl.menu_group);
	wrLog("【下发菜单】");
	iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);	//下发菜单
	/*-------------------------------------*/
	loop = 1;
	while(loop) 
	{
		if(TRADE_STAT)  //系统暂停
		{
			char stips[301], sflag[2];
			wrLog("【程序切换中，线程退出2】");
				
			
			if(atoi(service_Hour)>=235500 ||atoi(service_Hour)<=500)
				
			{
				TS_OutPutMsg(gbl.dPosInfo, "系统日结中，请稍后再试", 1, iSelect);
			}
			else
			{
				bbzero(stips);
				get_epos_tips(gbl.pos_no, 0, sflag, stips);
				if(strlen(stips) > 10) //长度少于10，认为是无效提示语
				{
					TS_OutPutMsg(gbl.dPosInfo, stips, 1, iSelect);
				}
				else
				{
					TS_OutPutMsg(gbl.dPosInfo, "系统维护中，请稍候再试...", 1, iSelect);
				}
			
			}
		
			break;
		}
		switch(iRet) 	//通过iRet来判断操作
		{
		case SUCC:
			iRet = MenuHdl(list, par_menu, cur_menu, iSelect, menuList);
			wrLog("【SUCC】iRet=%d",iRet);
			break;
			
		case HOTKEY:	//快捷键处理
			iRet = Menu_HOTKEY_hdl(iSelect, loop, menuList, par_menu, cur_menu, list);
			wrLog("【HOTKEY】iRet=%d",iRet);
			break;
			
		default:
			wrLog("下发挂机命令");	//下发挂机命令
			loop = 0;
			break;
		}
	}
	return 0;
}


int CMenuHdl::Menu_HOTKEY_hdl(int &iSelect, int &loop,TSMENULIST &menuList, int &par_menu, int &cur_menu, int *chglist)
{
	int iRet = 0, i;
	wrLog("【快捷键处理……】");
	wrLog("iSelect:%d",iSelect);
	switch(iSelect)
	{
	case CANCLE_KEY:
		//"取消"
		if(cur_menu == 0)  //根菜单
		{
			//下发挂机命令
			wrLog("下发挂机命令1");
			loop = 0;
		}
		else
		{
			cur_menu = par_menu;
			if( CDB::isLock() ) //刷新数据库中，等待6秒后还刷新不成功则退出
			{
				loop = 0;
				return -1;
			}
			for(i=0; i<menu_cnt[gbl.menu_group]; i++)
			{
				if(menu_conf[gbl.menu_group][i].currentm == cur_menu)
				{
					par_menu = menu_conf[gbl.menu_group][i].parent;
					break;
				}
			}
			if(i == menu_cnt[gbl.menu_group]) //查找出错
			{
				loop = 0;
				return -1;
			}
			get_menu_list(cur_menu, &menuList,chglist);
			iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		}
		break;
	default:
		//下发挂机命令
		wrLog("下发挂机命令2");
		loop = 0;
		break;
	}
	return iRet;
}

//功能：获取cur_menu的子菜单
//cur_menu：当前菜单项
//list：下发的菜单项的编号顺序
int CMenuHdl::get_menu_list(int cur_menu, PTSMENULIST pMenuList, int *chglist)	
{
	int i;
	int	k = 0;
	if( CDB::isLock() ) //刷新数据库中，等待6秒后还刷新不成功
		return -1;
	for(i=0; i<menu_cnt[gbl.menu_group]; i++)
	{
		//取得下级菜单
		if(menu_conf[gbl.menu_group][i].currentm == cur_menu) 
		{
			bbzero(pMenuList->menuItem[k]);
			strcpy(pMenuList->menuItem[k], epos_menu[ menu_conf[gbl.menu_group][i].child ].name);
			chglist[k+1] = menu_conf[gbl.menu_group][i].child; //由1算起
			k++;
		}
	}
	if( k == 0) //无下级菜单
		return 1;
	if(cur_menu == 0)
	{
		bbzero(pMenuList->menuItem[k]);
		strcpy(pMenuList->menuItem[k], "退   出");
		chglist[k+1] = -1;
		k++;
	}
	else
	{
		bbzero(pMenuList->menuItem[k]);
		strcpy(pMenuList->menuItem[k], "返    回");
		chglist[k+1] = -2;
		k++;
	}
	pMenuList->iTotalNum = k;
	if(cur_menu == 0 && gbl.tgroup == 5)  //20070831，省公司演示
	{
		strcpy(pMenuList->acTitle, "中国电信－固网支付");
	}
	else if(cur_menu == 0 && gbl.tgroup == 9)  //20071119，交行业务
	{
		strcpy(pMenuList->acTitle, "交易通");
	}
	else
	{
		if(strcmp(gbl.city_no,"0086")==0)

		{
		strcpy(pMenuList->acTitle,"翼支付");
		strcat(pMenuList->acTitle,epos_menu[cur_menu].name);

		}

		else

		{

		strcpy(pMenuList->acTitle, "交费易--");
		strcat(pMenuList->acTitle, epos_menu[cur_menu].name);

		}
	}
	return 0;
}


//功能：菜单处理
//list：存放当前下发菜单的排列顺序
//par_menu：当前菜单的父菜单编号
//cur_menu：当前菜单的编号
//iSelect：用户选择第iSelect个菜单项
//menuList：下发的菜单结构体
int CMenuHdl::MenuHdl(int *chglist, int &par_menu, int &cur_menu,int &iSelect, TSMENULIST &menuList)
{
	int iRet, cnt=1, i;

	gbl.init_global();	//初始化全局类
	while(chglist[cnt++]);	//获取菜单条数
	if(iSelect > 0 && iSelect < cnt) //选择其中一条菜单
	{
		par_menu = cur_menu;  //父菜单变为当前菜单
		cur_menu = chglist[iSelect]; //当前菜单变为用户选择的菜单项
		iRet = get_menu_list(cur_menu, &menuList, chglist);

		if( !iRet ) //有下级菜单
		{
			iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		}
		else if( iRet == -1 ) //退出
		{
			return HANG_UP_KEY;
		}
		else  //用户选择的菜单项无子菜单，已是业务项
		{
			if(cur_menu == -1)	//退出
			{
				return HANG_UP_KEY;
			}else if(cur_menu == -2)  //返回
			{
				if( CDB::isLock() )
					return HANG_UP_KEY;
				cur_menu = par_menu;	//取得原菜单参数
				for(i=0; i<menu_cnt[gbl.menu_group]; i++)
				{
					if(menu_conf[gbl.menu_group][i].currentm == cur_menu)
					{
						par_menu = menu_conf[gbl.menu_group][i].parent;
						break;
					}
				}
			}else
			{
				oper_hdl(chglist[iSelect]);  //业务处理
			}			
			cur_menu = par_menu;	//操作完毕，取得原菜单参数
			if( CDB::isLock() )
				return HANG_UP_KEY;
			for(i=0; i<menu_cnt[gbl.menu_group]; i++) //取得原菜单的父菜单编号
			{
				if(menu_conf[gbl.menu_group][i].currentm == cur_menu)
				{
					par_menu = menu_conf[gbl.menu_group][i].parent;
					break;
				}
			}
			if( i == menu_cnt[gbl.menu_group] ) //查找错误
				return HANG_UP_KEY;
			get_menu_list(cur_menu, &menuList, chglist); //获取原菜单
			iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);	//下发原菜单
		}
	}
	else //用户取消
		iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);	//下发原菜单
	return iRet;
}


int CMenuHdl::get_menu_group_mbl_flag(int tgroup,int askType,int* menu_group_mbl_flag)
{
	int i,my_menu_group,flag=0;
	for (i = 0;i < epos_menu_get.size(); i++)
	{
		if (epos_menu_get[i].tgroup==tgroup && epos_menu_get[i].asktype==askType && memcmp(epos_menu_get[i].bank,"000000",6)==0)
		{
			flag = 1;
			my_menu_group = epos_menu_get[i].menu_group;
			break;
		}
	}
	if (flag==0)
	{
		return -1;
	}
	flag = 0;
	for (i=0;i<MAX_MENU;i++)
	{
		if (epos_menu_group[i].menu_group == my_menu_group)
		{
			flag =1;
			*menu_group_mbl_flag = epos_menu_group[i].mbl_flag;
			break;
		}
	}
	if (flag == 0)
	{
		return -1;
	}
	return 0;
}


int CMenuHdl::get_epos_tips(char*pos_no,int menu_id,char*return_flag,char*tips)
{
	int i;
	char pos_flag[1+1];
	bbzero(pos_flag);
	if (memcmp(pos_no,"7",1)==0)
	{
		strcpy(pos_flag,"1");
	}else if (memcmp(pos_no,"6",1)==0)
	{
		strcpy(pos_flag,"2");
	}else
		return -1;
	for (i = 0; i < epos_tips.size(); i++)
	{
		if (epos_tips[i].menu_id == menu_id)
		{
			if (memcmp(epos_tips[i].pos_flag,"0",1)==0)
			{
				strcpy(return_flag,epos_tips[i].return_flag);
				strcpy(tips,epos_tips[i].tips);
				return 0;
			}else{
				if (memcmp(epos_tips[i].pos_flag,pos_flag,1)==0)
				{
					strcpy(return_flag,epos_tips[i].return_flag);
					strcpy(tips,epos_tips[i].tips);
					return 0;
				}				
			}
		}
	}
	return -1;
	
}


//业务处理
//id：业务编号
void CMenuHdl::oper_hdl(int id)
{
	CTrade	trade;
	CPrint prt;
	CPack pack;
	CTools tool;
	int ret,iRet,iSelect;
	char type[15];
	char workid[8+1];
	char temp_buff[1024];
	char cTime[6+1];
	char tips[300+1],return_flag[1+1];

	bbzero(type);
	if( atoi(gbl.fee_type) == 100)
		strcpy(type, "数字电视缴费");
	else if( atoi(gbl.fee_type) == 112)
		strcpy(type, "珠江宽频缴费");
	else if( atoi(gbl.fee_type) == 114)
		strcpy(type, "模拟电视(无机顶盒)");
	else if( atoi(gbl.fee_type) == 172)
		strcpy(type, "珠江宽频优惠");

	bbzero(tips);
	bbzero(return_flag);

	iRet = get_epos_tips(gbl.pos_no,id,return_flag,tips);
	if (iRet == 0)
	{   		
		if (strlen(tips) != 0)
		{   
			if(id==636||id==637||id==638||id==639){
               TS_OutPutMsg(gbl.dPosInfo,tips,0,iSelect,5,120);
			}else
			   TS_OutPutMsg(gbl.dPosInfo,tips,0,iSelect);
			if (memcmp(return_flag,"1",1)==0)
			{
				return ;
			}
		}
	}
	
	switch( id )
	{
	case 2:
		wrLog("中国移动");
		strcpy(gbl.fee_type, "3012");
		trade.set_in_par(gbl);
		trade.mblTrade();
		break;

	case 10:  //家用车车强险
	
		trade.set_in_par(gbl);
		trade.piccTrade();
		break;
	case 34:  //有线电视缴费
		strcpy(gbl.fee_type,"0114");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 35:  //珠江数码缴费
		strcpy(gbl.fee_type,"0100");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 36:  //珠江宽频缴费
		strcpy(gbl.fee_type,"0112");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 167: //珠江宽频优惠
		strcpy(gbl.fee_type,"0172");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 41:

		strcpy(gbl.fee_type,"0076");
		trade.set_in_par(gbl);
		trade.QQTrade("Q币充值");

		/*
				
		if (atoi(gbl.dPosInfo.prmVer) >9000) //如果是ipos 切换到新的广禄接口
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015"); //glQQ
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027"); //glQQ

			trade.GLQQTrade("Q币充值");  //已经更换为广禄qq，腾讯qq暂停


		}
		else
		{
		
			trade.QQTrade("Q币充值");
		}

		*/

		break;
	case 42:
		strcpy(gbl.fee_type,"0077");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ会员包月");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ会员包月");
		}
		else
		{
			trade.QQTrade("QQ会员包月");

		}

		*/	
		
		break;
	/*---------------------------------*/
	case 70:
		wrLog("自由互联帐号充值"); 
		strcpy(gbl.fee_type,"0143");
		trade.set_in_par(gbl);
		trade.telTrade("自由互联帐号充值");
		break;
	case 71:
		wrLog("固话/小灵通话费");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.telTrade("固话/小灵通话费");
		break;
	case 72:
		wrLog("预付费固话充值");
		strcpy(gbl.fee_type,"0098");
		trade.set_in_par(gbl);
		trade.telTrade("预付费固话充值");
		break;
	case 73:
		wrLog("小灵通话费");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.telTrade("小灵通话费");
		break;
	case 74:
		wrLog("预付费小灵通充值");
		strcpy(gbl.fee_type,"0064");
		trade.set_in_par(gbl);
		trade.telTrade("预付费小灵通充值");
		break;
	case 75:
		wrLog("ADSL网费");
		strcpy(gbl.fee_type,"0013");
		trade.set_in_par(gbl);
		trade.telTrade("ADSL网费");
		break;
	case 76: 
		wrLog("ADSL预缴基本月租");	//子业务代码“71”
		strcpy(gbl.fee_type,"0071");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"71");
		trade.set_in_par(gbl);
		trade.telTrade("ADSL预缴基本月租");
		break;
	case 77:
		wrLog("预缴费卡充值（蓝卡）");
		strcpy(gbl.fee_type,"0046");
		trade.set_in_par(gbl);
		trade.telTrade("预缴费卡充值（蓝卡）");
		break;
	case 78:
		wrLog("ITV欠费");   //电信菜单入口
		strcpy(gbl.fee_type,"0137");
		trade.set_in_par(gbl);
		trade.telTrade("ITV欠费");
		break;
	case 79:
		wrLog("预付费ADSL充值");
		strcpy(gbl.fee_type,"0140");
		trade.set_in_par(gbl);
		trade.telTrade("预付费ADSL充值");
		break;
	/*---------------------------------*/
	case 80: 
		//130/131后付费
		
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"01");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"0");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	case 81: 
		//130/131预付费
	
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"01");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"1");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	case 84: 
		//193/17911后付费
	
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"03");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"0");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	case 85: 
		//193/17911预付费
		
	
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"03");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"1");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	/*---------------------------------*/
	case 86:
// 		wrLog("小灵通付费易卡－>预付费ADSL充值");
// 		strcpy(gbl.fee_type,"0140");
// 		trade.set_in_par(gbl);
// 		ret = trade.xltTrade("预付费ADSL充值");
// 		if( !ret )
// 			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
// 		break;
		wrLog("【含查询的电信直充——预付费ADSL充值】");
		trade.set_in_par(gbl);
		trade.newAirPay("预付费ADSL充值",2);
		break;
	case 87: 
		wrLog("小灵通付费易卡－>ITV欠费");
		strcpy(gbl.fee_type,"0137");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("ITV欠费");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 88: 
		wrLog("ITV预存");
		strcpy(gbl.fee_type,"0138");
		trade.set_in_par(gbl);
		ret = trade.telTrade("ITV预存");
		break;
	case 89: 
		wrLog("小灵通付费易卡－>自由互联帐号充值");
		strcpy(gbl.fee_type,"0143");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("自由互联帐号充值");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 90: 
		wrLog("小灵通付费易卡－>固话/小灵通话费");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("固话/小灵通话费");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 91: 
		wrLog("固话/小灵通话费预存");
		strcpy(gbl.fee_type,"0096");
		trade.set_in_par(gbl);
		ret = trade.telTrade("固话/小灵通话费预存");
		break;
	case 92: 
		wrLog("小灵通付费易卡－>预付费固话充值");
		strcpy(gbl.fee_type,"0098");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("预付费固话充值");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 93:
		wrLog("小灵通付费易卡－>小灵通话费");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("小灵通话费");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 94: 
		wrLog("小灵通付费易卡－>预付费小灵通充值");
		strcpy(gbl.fee_type,"0064");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("预付费小灵通充值");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 95: 
		wrLog("小灵通话费预付金");
		strcpy(gbl.fee_type,"0096");
		trade.set_in_par(gbl);
		ret = trade.telTrade("小灵通话费预付金");
		break;
	case 96: 
		wrLog("小灵通付费易卡－>ADSL网费");
		strcpy(gbl.fee_type,"0013");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("ADSL网费");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 97: 
		wrLog("ADSL网费预存");
		strcpy(gbl.fee_type,"0097");
		trade.set_in_par(gbl);
		ret = trade.telTrade("ADSL网费预存");
		break;
	case 98: 
		wrLog("小灵通付费易卡－>ADSL预缴基本月租");
		strcpy(gbl.fee_type,"0071");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"71");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("ADSL预缴基本月租");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 99:
		wrLog("小灵通付费易卡－>查询余额");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0094", 4);
		trade.set_in_par(gbl);
		trade.cardInqHdl();
		break;
	case 100: 
		wrLog("小灵通付费易卡－>修改密码");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0095", 4);
		trade.set_in_par(gbl);
		trade.cardChgPwd();
		break;
	/*---------------------------------*/
	case 101:

		wrLog("广州车船税");
		strcpy(gbl.fee_type,"0065");
		trade.set_in_par(gbl);
		trade.taxTrade("广州车船税");
		break;
	/*---------------------------------*/
	case 110:
		//IPOS登陆显示提示语
		if(	strlen(gbl.welcome) != 0 )
		{

			bbzero(temp_buff);
			strcat(temp_buff,gbl.welcome);
			TS_OutPutMsg(gbl.dPosInfo,temp_buff, 1, iSelect);
		}

		wrLog("e卡－>查询余额");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		char amount[12+1];
		trade.cardInqHdl(amount);
// 		if(atoi(gbl.dPosInfo.prmVer)>=9000 && (atoi(prmVer)>atoi(gbl.dPosInfo.prmVer) || strstr(prmMD5, thisPrmMD5)==NULL && strstr(prmMD5, "0000000000000000")==NULL))//需要更新程序
// 		{
// 			UpdateFiles();
// 		}
		break;
	case 111:
	case 535:
		wrLog("e卡－>修改密码");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0084", 4);
		trade.set_in_par(gbl);
		trade.cardChgPwd();				
		break;	
	/*---------------------------------*/
	case 112:
		wrLog("补打通讯资费发票");
		strcpy(gbl.fee_type,"0012");
		trade.set_in_par(gbl);
		trade.rePrintHdl(1);
		break;
	case 113:
		wrLog("补打车船使用税发票");
		strcpy(gbl.fee_type,"3065");
		trade.set_in_par(gbl);
		trade.rePrintHdl(2);
		break;
	/*---------------------------------*/
	case 114: 
		wrLog("小灵通充值卡");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "010");
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡");
		break;
	case 115: 
		wrLog("小灵通付费易卡");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "011");
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通付费易卡");
		break;
	case 116:
		wrLog("移动充值卡");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡");
		break;
	case 117:
		wrLog("联通一卡充");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通一卡充");
		break;	
	case 120:
		wrLog("天天洗衣");
		trade.set_in_par(gbl);
		trade.merTrade();
		break;
	case 121:
	case 495:
	case 534:
		wrLog("黑标定位");
		//prt.set_in_par(gbl, pack);
		prt.set_in_par(gbl);
		prt.print_heibiao();
		break;
// 	case 131:
// 		{
// 			CYCTTrade yct;
// 			yct.setInPar(gbl);
// 			yct.YCTPayTrade();  //查询交费
// 		}
// 		break;
	case 132:
		wrLog("本地电信综合收费");		
		trade.set_in_par(gbl);
		trade.foshanTrade(epos_menu[id].name, "0");
		break;
	case 145:
		wrLog("商户日交易查询");
		trade.set_in_par(gbl);
		//trade.merInqTrade(1);
        trade.drcx_online(0);
		break;
	case 146:
		wrLog("商户上月交易查询");
		trade.set_in_par(gbl);
		//trade.merInqTrade(2);
		trade.last_mnth_online(0);
		break;
	case 147:
		//付费易卡2期余额查询
		trade.set_in_par(gbl);
		trade.xltInqTrade("余额查询");
		break;
	case 148:
		//付费易卡2期充值
		trade.set_in_par(gbl);
		trade.xltMnyIn("付费易卡充值");
		break;
	case 149:
		//付费易卡2期转帐
		trade.set_in_par(gbl);
		trade.xltAccTrans("付费易卡转帐");
		break;
	case 150:
		//移动空中充值
		trade.set_in_par(gbl);
		trade.mblAirPay("移动话费直充", 2);
		break;
	case 151:
		//e卡充值
		trade.set_in_par(gbl);
		trade.ecardMnyIn("交费易账户充值");
		TS_HangUp(gbl.dPosInfo);
		closesocket(sd);
		break;
	case 152:
		//e卡充值登记
		trade.set_in_par(gbl);
		trade.ecardAppli("账户充值申请");
		break;
	case 153:
		//最新电影折扣票
		trade.set_in_par(gbl);
		trade.filmPay("最新电影折扣票");
		break;
	case 154:
		//银行卡余额查询
		trade.set_in_par(gbl);
		trade.getBankMny("银行卡余额查询");
		break;
	case 155:
		//最后一笔交易查询
		trade.set_in_par(gbl);
		iRet=trade.merInqTrade(3);
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e卡－>最后一笔交易查询后查询余额");
			memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
			memcpy(gbl.fee_type, "0083", 4);
			trade.set_in_par(gbl);
			{
				char amount[12+1];
				bbzero(amount);
				strcpy(amount,"1");
				trade.cardInqHdl(amount);
			}		
		}
		break;
	case 156:
		wrLog("签到");
		/**/
		if( gbl.askType != 4)
			TS_OutPutMsg(gbl.dPosInfo, "提示：为方便您的“签到”操作，即日起可以直接按机身左上方的“返回”快捷键完成，详询：400-80-11888", 1, ret);
		
		trade.set_in_par(gbl);
		trade.banInqTrade(1);
		break;
	case 157:
		//转账
		trade.set_in_par(gbl);
		trade.banTransfer(epos_menu[id].name);
		break;
	case 158:
		wrLog("余额查询");
		strcpy(gbl.fee_type, "0185");
		trade.set_in_par(gbl);	
		trade.banInqMoneyTrade(epos_menu[id].name);
		break;
	case 159:
		wrLog("收款账户卡号");
		trade.set_in_par(gbl);
		trade.banInqTrade(2);
		break;
	case 160:
		wrLog("最后一笔交易");
		trade.set_in_par(gbl);
		trade.banInqLast();
		break;
	case 161:
		wrLog("当日交易明细");
		trade.set_in_par(gbl);
		//trade.banInqHistory(1);
		trade.drcx_online(0);
		break;
	case 162:
		wrLog("历史交易明细");
		trade.set_in_par(gbl);
		trade.banInqHistory(2);
		break;
	case 163:
		wrLog("打印测试");
		prt.set_in_par(gbl);
		prt.print_heibiao(1);
		break;
	case 168:
		wrLog("全省电信综合收费");
		trade.set_in_par(gbl);
		trade.foshanTrade(epos_menu[id].name, "0");
		break;
	/*--------------------------*/
	case 199:  //更新首页信息
		TS_UpdateHeadPage(gbl.dPosInfo, "欢迎使用交通银行交易通支付系统");
//		trade.set_in_par(gbl);
//		trade.updateConInfo();
		break;
	case 200:
		//发送短信
		trade.set_in_par(gbl);
		trade.sendMsg();
		break;
	case 205:
		wrLog("固话+ADSL网费");
		strcpy(gbl.fee_type,"0047");
		trade.set_in_par(gbl);
		trade.telTrade("固话+ADSL网费");
		break;
	case 206:
		wrLog("重打印发票");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_fail("重打印发票");
		break;
	case 207:
		wrLog("交行转帐本地化");
		trade.set_in_par(gbl);
		trade.banTransLocal("交行转帐本地化", &tradeInfoRecv);
		break;
	case 208:
		wrLog("魔兽世界300点");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0801");
		trade.set_in_par(gbl);
		trade.sellCardTrade("魔兽世界300点");
		break;
	case 209:
		wrLog("魔兽世界600点");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0801");
		trade.set_in_par(gbl);
		trade.sellCardTrade("魔兽世界600点");
		break;
	case 210:
		wrLog("搜狐15元");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0802");
		trade.set_in_par(gbl);
		trade.sellCardTrade("搜狐畅游一卡通300点");
		break;
	case 211:
		wrLog("搜狐30元");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0803");
		trade.set_in_par(gbl);
		trade.sellCardTrade("搜狐畅游一卡通600点");
		break;
	case 213:
		wrLog("串口透传功能检测");
		trade.set_in_par(gbl);
		trade.testIcReaderCommand("串口透传功能检测");
		break;
	case 215:
		wrLog("电信重打印发票");
		strcpy(gbl.fee_type,"0218");
		trade.set_in_par(gbl);
		trade.rePrintTel("电信补打发票");
		break;
	case 216:
		wrLog("【e卡最近五笔充值】");
		trade.set_in_par(gbl);
		trade.getEcardMsg("e卡最近五笔充值", 1);
		break;
	case 217:
		wrLog("【报文测试】");
		trade.set_in_par(gbl);
		trade.testFSKPack("报文测试");
		break;
// 	case 218:
// 		wrLog("【交通罚款】");
// 		strcpy(gbl.fee_type,"0099");
// 		trade.set_in_par(gbl);
// 		trade.trafficTrade("交通罚款");
// 		break;
// 	case 220:
// 		{
// 			CYCTTrade yct;
// 			yct.setInPar(gbl);
// 			yct.YCTPayTrade(0);//交费
// 		}
// 		break;		
    case 221:
		wrLog("【交通罚款补打凭证】");
		strcpy(gbl.fee_type,"0012");
		trade.set_in_par(gbl);
		trade.rePrintTra("交通罚款补打凭证");
		break;
	case 223:
		wrLog("【手工设置PIN】");
		trade.set_in_par(gbl);
		trade.SetPIN(1);
		break;
	case 224:
		wrLog("【按终端号自动设置PIN】");
		trade.set_in_par(gbl);
		trade.SetPIN(2);
		break;
	case 228:
		wrLog("CDMA后付费");
		strcpy(gbl.fee_type,"0241");
		trade.set_in_par(gbl);
		trade.telTrade("CDMA后付费");
		break;
	case 229:
		wrLog("CDMA预付费");
		strcpy(gbl.fee_type,"0242");
		trade.set_in_par(gbl);
		trade.telTrade("CDMA预付费");
		break;
	//case 229: //20130105 
		//广州电信空中充值
		//trade.set_in_par(gbl);
	//	trade.mblAirPay("电信话费直充", 1);
	//	break;
	case 230:
		//银联转账
		wrLog("【银行转账(转入)】");
		trade.set_in_par(gbl);
		trade.UnionTransfer_in(epos_menu[id].name);
		break;
	case 231:
		//银联转账
		wrLog("【银行转账(转出)】");
		trade.set_in_par(gbl);
		trade.UnionTransfer_out(epos_menu[id].name);
		break;
	case 232:
		wrLog("【有线用户登记】");
		trade.set_in_par(gbl);
		trade.ysqCatv();
		break;
	case 233:
		//银联银行卡余额查询
		wrLog("【银行卡余额查询】");
		trade.set_in_par(gbl);
		trade.UnionbanInqMoney(epos_menu[id].name);
		break;
	case 234:
		//最后一次交易查询
		wrLog("【上一次交易查询】");
		trade.set_in_par(gbl);
		trade.UnionbanInqLast();
		break;
	case 235:
		wrLog("【当日交易查询】");
		trade.set_in_par(gbl);
		//trade.UnionbanInqHistory(1);
		trade.drcx_online(0);
		break;
	case 236:
		wrLog("【历史交易查询】");
		trade.set_in_par(gbl);
		trade.UnionbanInqHistory(2);
		break;
	case 237:
		wrLog("【交通罚款交纳】");

		bbzero(cTime);			//交通罚款每日21:00分至00:00分暂停交易20081128
		tool.get_time(cTime);
		if(atoi(cTime) >= 210000 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n   交罚系统日结中\n请24:00后再进行交易\n   按【确认】返回",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0099");
		trade.set_in_par(gbl);
		trade.trafficTrade("交通罚款交纳");
		break;
	case 238:
		wrLog("【小型车违章查询】");
		trade.set_in_par(gbl);
		trade.carnum_trafficTrade("小型车违章查询");
		break;
	case 240:
		wrLog("余额查询");
		strcpy(gbl.fee_type, "0238");
//		strcpy(gbl.fee_type, "0333");
		trade.set_in_par(gbl);
		trade.banInqMoneyTrade(epos_menu[id].name);
		break;
	case 266:
		wrLog("按工号签到");
		trade.set_in_par(gbl);
		bbzero(workid);
		iRet = trade.signWorkID(workid);
		if (iRet <0)
		{
			bbzero(temp_buff);
			sprintf(temp_buff,"\n\n工号签到出错!\n请稍候再试!\n  请按【确定】返回");
			TS_OutPutMsg(gbl.dPosInfo, temp_buff, 1,iSelect);
		}else{
			bbzero(gbl.curworkid);
			strcpy(gbl.curworkid,workid);
			bbzero(temp_buff);
			sprintf(temp_buff,"\n\n    工号签到成功!\n请稍候再试!\n  请按【确定】返回");
			TS_OutPutMsg(gbl.dPosInfo, temp_buff, 1,iSelect);
		}
		break;
	case 271:
		wrLog("固话费用(银行划扣)");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.BankHK("固话费用");
		break;
	case 272:
		wrLog("ADSL网费(银行划扣)");
		strcpy(gbl.fee_type,"0013");
		trade.set_in_par(gbl);
		trade.BankHK("ADSL网费");
		break;		
	case 273:		
		wrLog("小灵通费用(银行划扣)");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.BankHK("小灵通费用");
		break;
	case 278:
		wrLog("预存优惠");
		strcpy(gbl.fee_type,"0064");
		trade.set_in_par(gbl);
		trade.YH_xlt("预存优惠");
		break;
//////////////////*测试功能函数*/////////////////////////
	case 282:
		wrLog("【测试明文密码】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(8);
		break;
	case 283:
		wrLog("【测试年月】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(7);
		break;
	case 284:
		wrLog("【测试日期】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(6);
		break;
	case 285:
		wrLog("【测试输入全字符】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(5);
		break;
	case 286:
		wrLog("【测试输入数字】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(4);
		break;
	case 287:
		wrLog("【测试输入金额】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(3);
		break;
	case 288:
		wrLog("【测试刷卡功能】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(2);
		break;
	case 289:
		wrLog("【测试手输卡号】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(1);
		break;
	case 290:
		wrLog("【显示终端电话号码】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(1);
		break;
	case 291:
		wrLog("【显示终端号】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(2);
		break;
	case 292:
		wrLog("【显示终端左串口设备】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(3);
		break;
	case 293:
		wrLog("【显示终端右串口设备】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(4);
		break;
	case 294:
		wrLog("【显示终端内置设备】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(5);
		break;
	case 295:
		wrLog("【下发提示信息】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_OutPutMsg(1);
		break;
	case 296:
		wrLog("【下发安全提示信息】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_OutPutMsg(2);
		break;
	case 297:
		wrLog("【下发多条提示信息】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_OutPutMsg(3);
		break;
	case 298:
		wrLog("【打印】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_B4Print(1);
		break;
	case 299:
		wrLog("【安全打印】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_B4Print(2);
		break;
	case 300:
		wrLog("【通知】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(1);
		break;
	case 301:
		wrLog("【交易结果】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(2);
		break;
	case 302:
		wrLog("【账单交易结果】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(3);
		break;
	case 303:
		wrLog("【账单】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(4);
		break;
	case 304:
		wrLog("【版本号】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(5);
		break;
	case 305:
		wrLog("【终端序列号】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(6);
		break;
	case 306:
		wrLog("【更新提示信息】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(1);
		break;
	case 307:
		wrLog("【更新终端参数】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(2);
		break;
	case 308:
		wrLog("【更新首页信息】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(3);
		break;
	case 309:
		wrLog("【更新本地菜单】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(4);
		break;
	case 310:
		wrLog("【更新PSAM卡密钥】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(5);
		break;
	case 311:
// 		wrLog("【PSAM卡号】");
// 		trade.set_in_par(gbl);
// 		iRet = trade.Test_TS_PSAM(1);
		break;
	case 312:
// 		wrLog("【PSAM卡随机数】");
// 		trade.set_in_par(gbl);
// 		iRet = trade.Test_TS_PSAM(2);
		break;
	case 313:
		wrLog("【测试输入字母】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(9);
		break;
	case 314:
		wrLog("【测试输入汉字】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(10);
		break;
	case 323:
		wrLog("【刷卡或手输卡号】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(11);
		break;

	case 324:
		wrLog("【小灵通选号】");
		trade.set_in_par(gbl);
		iRet = trade.selectxlt();
		break;
	case 325:
		wrLog("【银行划扣受理】");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(1);
		break;
	case 327:
		wrLog("【已有固话新装ADSL】");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(2);
		break;
	case 328:
		wrLog("【申请e8套餐】");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(3);
		break;
	case 329:
		wrLog("【e8包年升级套餐】");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(4);
		break;
	case 331:
		wrLog("【申请e6套餐】");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(5);
		break;
	case 333:
		wrLog("【开通七彩铃音】");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "001", "3626");
		break;
	case 334:
		wrLog("【撤销七彩铃音】");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "002", "3626");
		break;
	case 335:
		wrLog("【开通语音信箱】");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "001", "284");
		break;
	case 336:
		wrLog("【撤销语音信箱】");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "002", "284");
		break;
	case 339:
		wrLog("【撤销语音信箱】");
		trade.set_in_par(gbl);
		iRet = trade.selectcdma();
		break;

	case 340:
		wrLog("【更新本地金融菜单】");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(6);
		break;
	case 341:
		wrLog("【交费易签约】");
		trade.set_in_par(gbl);
		trade.ysqCatv_new();
		break;
	case 342:
		wrLog("【固话加装ADSL】");
		trade.set_in_par(gbl);
		trade.add_ADSL("固话加装ADSL");
		break;
	case 343:
// 		wrLog("【星空网盟点卡销售】");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("星空网盟点卡销售");
		wrLog("【此业务已暂停】");
		break;
	case 345:
		wrLog("【机器采购付款】");
		trade.set_in_par(gbl);
		trade.sell_pos(1);
		break;
	case 346:
		wrLog("【发票及凭条订购】");
		trade.set_in_par(gbl);
		trade.sell_pos(2);
		break;
	case 347:
		wrLog("【银行卡转账】");
		trade.set_in_par(gbl);
		trade.Bankc_zz("银行卡转账");
		break;
	case 348:
		wrLog("【288礼包】");
		trade.set_in_par(gbl);
		trade.sell_pos(3);
		break;
	case 353:
		//广州电信空中充值
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1);
		break;
	case 354:
		//全省电信空中充值
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1);
		break;
	case 368:
		//一次性收费 电话号码
		trade.set_in_par(gbl);
		trade.payonce(1, epos_menu[id].name);
		break;
	case 369:
		//一次性收费 申请单编号
		trade.set_in_par(gbl);
		trade.payonce(2, epos_menu[id].name);
		break;
	case 370:
		//一次性收费 订单编号
		trade.set_in_par(gbl);
		trade.payonce(3, epos_menu[id].name);
		break;
	case 371:
		trade.set_in_par(gbl);
		trade.dxysl_trad( epos_menu[id].name);
		break;
	case 372:
		wrLog("商户当月月交易查询");
		trade.set_in_par(gbl);
		//trade.merInqTrade(4);
		trade.yjycx_online(0);
		break;
	case 382:
		wrLog("【大型车违章查询】");
		trade.set_in_par(gbl);
		trade.carnum_trafficTrade("大型车违章查询",1);
		break;
	case 383:
		wrLog("【交通罚款交纳】");

		bbzero(cTime);			//交通罚款每日21:00分至00:00分暂停交易20081128
		tool.get_time(cTime);
		if(atoi(cTime) >= 210000 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n   交罚系统日结中\n请24:00后再进行交易\n   按【确认】返回",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0099");
		trade.set_in_par(gbl);
		trade.trafficTrade("交通罚款交纳",1);
		break;
	case 384:
// 		wrLog("【魔兽世界600点卡】");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("魔兽世界600点卡",1);
		wrLog("【此业务已暂停】");
		break;
	case 385:
// 		wrLog("【盛大互动娱乐卡】");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("盛大互动娱乐卡",2);
		wrLog("【此业务已暂停】");
		break;
	case 386:
// 		wrLog("【搜狐一卡通】");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("搜狐一卡通",3);
		wrLog("【此业务已暂停】");
		break;
	case 387:
// 		wrLog("【网易一卡通】");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("网易一卡通",4);
		wrLog("【此业务已暂停】");
		break;
	case 388:
// 		wrLog("【骏网一卡通】");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("骏网一卡通",5);
		wrLog("【此业务已暂停】");
		break;
	case 389:
		wrLog("【全省天下通游戏卡】");
		strcpy(gbl.fee_type,"0089");
		trade.set_in_par(gbl);
		//使用新函数
		trade.sell_txtgame_card("天下通游戏卡");
		//trade.sell_xmcard("游戏卡");
		break;
	case 391:
		wrLog("天下通卡");
		strcpy(gbl.fee_type,"0089");
		trade.set_in_par(gbl);
		//trade.sellZWgameCard("天下通卡");
		trade.sellZWgameCard("天下通卡",0,1);
		break;
	case 393:
		wrLog("【汽车年票查询蓝牌6座以下】");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("汽车年票缴纳",1);
		break;
	case 394:
		wrLog("【汽车年票查询蓝牌6座及以上】");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("汽车年票缴纳",2);
		break;
	case 395:
		wrLog("【汽车年票查询黑牌6座以下】");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("汽车年票缴纳",3);
		break;
	case 396:
		wrLog("【汽车年票查询黑牌6座及以上】");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("汽车年票缴纳",4);
		break;
	case 397:
		wrLog("【汽车年票查询黄牌6座以下】");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("汽车年票缴纳",5);
		break;
	case 398:
		wrLog("【汽车年票查询黄牌6座及以上】");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("汽车年票缴纳",6);
		break;
	case 399:
		wrLog("农行转账【演示版】");
		trade.set_in_par(gbl);
		trade.NH_Transfer("农行转账");
		break;
	case 400:
		//联通空中充值
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3);
		break;
	case 401: 
		wrLog("【小灵通充值卡50】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "010");
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡", 1);
		break;
	case 402: 
		wrLog("【小灵通充值卡100】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "010");
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡", 2);
		break;
	case 403:
		wrLog("【移动充值卡50】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡", 2);
		break;
	case 404:
		wrLog("【移动充值卡100】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡", 3);
		break;
	case 405:
		wrLog("【联通一卡充30】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通一卡充", 1);
		break;
	case 406:
		wrLog("【联通一卡充50】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通一卡充", 2);
		break;
	case 407:
		wrLog("【联通一卡充100】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通一卡充", 3);
		break;
	case 408:
		wrLog("【广州电信空中充值30】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 30);
		break;
	case 409:
		wrLog("【广州电信空中充值50】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 50);
		break;
	case 410:
		wrLog("【广州电信空中充值100】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 100);
		break;
	case 411:
		wrLog("【广州电信空中充值200】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 200);
		break;
	case 412:
		wrLog("【广州电信空中充值300】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 300);
		break;
	case 413:
		wrLog("【广州电信空中充值500】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 500);
		break;
	case 414:
		wrLog("【移动空中充值30】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 30);
		trade.mblAirPayForGDYD("移动话费直充",30);
		break;
	case 415:
		wrLog("【移动空中充值50】");

		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 50);
		trade.mblAirPayForGDYD("移动话费直充",50);
		break;
	case 416:
		wrLog("【移动空中充值100】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 100);
		trade.mblAirPayForGDYD("移动话费直充", 100);
		break;
	case 417:
		wrLog("【移动空中充值300】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 300);
		trade.mblAirPayForGDYD("移动话费直充", 300);
		break;
	case 418:
		wrLog("【移动空中充值500】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 500);
		trade.mblAirPayForGDYD("移动话费直充", 500);
		break;
	case 424:
	case 543:
		wrLog("【商户签约】");
		trade.set_in_par(gbl);
		trade.customer_sign();
		break;
	case 425:
		wrLog("【清远电信预受理业务】");
		trade.set_in_par(gbl);
		trade.qy_dxysl_trad( epos_menu[id].name,1);
		break;
	case 426:
		wrLog("【清远电信预受理业务】");
		trade.set_in_par(gbl);
		trade.qy_dxysl_trad( epos_menu[id].name,2);
		break;
	case 427:
		wrLog("【清远电信预受理业务】");
		trade.set_in_par(gbl);
		trade.qy_dxysl_trad( epos_menu[id].name,3);
		break;
	case 428:
		wrLog("【汽车年票】");
		trade.set_in_par(gbl);
		trade.car_np_Trade("汽车年票");
		break;
	case 429:
		wrLog("e卡－>查询酬金"); // by liuxiao 20130408 
		if(atoi(gbl.dPosInfo.prmVer)>=9000)
		{

		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.yj_InqHdl();

		}
		else
		{
			TS_OutPutMsg(gbl.dPosInfo,"尚未开通\n  请按【确认】返回" ,1, iSelect);
		
		}

		break;
	case 430:
		wrLog("【修改老板密码】");
		trade.set_in_par(gbl);
		trade.chgPEPwd(epos_menu[id].name);
		break;
	////////////////////////////////我的超爽交费易START////////////////////////////////////////
	case 435:
		wrLog("【超爽交费易】");
		trade.set_in_par(gbl);
		trade.MyCoolPayEasy( epos_menu[id].name );
		break;
	////////////////////////////////我的超爽交费易END//////////////////////////////////////////
	case 437:
		wrLog("【修改POS参数】");
		trade.set_in_par(gbl);
		trade.updatePosPar( epos_menu[id].name );
		break;
	case 438:
		wrLog("【修改IPOS输入密码方式】");
		trade.set_in_par(gbl);
		trade.chgEPwdType( epos_menu[id].name );
		break;
	case 441:
		wrLog("【电信选号-153畅听卡】");
		trade.set_in_par(gbl);
		trade.tel_card_select( epos_menu[id].name , 1);
		break;
	case 442:
		wrLog("【电信选号-153大众卡】");
		trade.set_in_par(gbl);
		trade.tel_card_select( epos_menu[id].name , 2);
		break;
	case 443:
		wrLog("【修改日交易限额】");
		trade.set_in_par(gbl);
		trade.chgDayAmo( epos_menu[id].name );
		break;
	case 449:
		wrLog("【电信充值付费卡】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费业务");
		break;
	case 450:
		wrLog("【交费易钱包充值】");
		strcpy(gbl.fee_type,"0281");
		trade.set_in_par(gbl);
		trade.PayEasy_MoneyIn( epos_menu[id].name );
		break;
	case 468:
		wrLog("【电信充值付费卡-30元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费业务", 2);
		break;
	case 469:
		wrLog("【电信充值付费卡-50元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费业务", 3);
		break;
	case 470:
		wrLog("【电信充值付费卡-100元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费业务", 4);
		break;
	case 473:
		wrLog("【电信充值付费卡-20元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费业务", 1);
		break;
	case 480:
		wrLog("【IPOS账户充值查询】");
		trade.set_in_par(gbl);
		//trade.iposTxnEnq(epos_menu[id].name, 1, 0);
		trade.zjdzcx_online(1);
		break;
	case 483:
		wrLog("【商户结算】");
		trade.set_in_par(gbl);
		trade.chk_money( epos_menu[id].name );
		break;
	//////////////////////////////////////////////////////////////////////////

	case 451:
		wrLog("【东莞全球通业务-东莞移动直充】");
		trade.set_in_par(gbl);
		iRet = trade.mblTrade_pre(epos_menu[id].name);
		break;
	case 452:
		wrLog("【东莞电信后付费缴费】");
		bbzero(cTime);			//电信每日23:45分至00:00分暂停交易20071229
        CTools::get_time(cTime);
		if(atoi(cTime) >= 234500 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 日结中，请稍后再试\n   按【确认】返回",0,iSelect);
			return;
		}
		strcpy(gbl.fee_type,"0131");
		trade.set_in_par(gbl);
		iRet = trade.telTrade(epos_menu[id].name);
		break;
	case 453: 
		wrLog("【东莞固话及小灵通充值】");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡",0);
		break;
	case 454:
		wrLog("【东莞ADSL宽带充值】");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=4;
		trade.set_in_par(gbl);
		trade.sellCardTrade("ADSL宽带充值卡",0);
		break;
	case 455:
		wrLog("【东莞移动售卡】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡",0);
		break;
	case 456:
		wrLog("【东莞联通售卡】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通充值卡",0);
		break;
	case 457:
		wrLog("东莞天下通卡");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("天下通卡");
		break;
	case 458:
		wrLog("【东莞Q币充值】");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q币充值");

		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //如果是ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015"); //glQQ
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q币充值");
		}
		else
		{
		
			trade.QQTrade("Q币充值");
		}
		*/


		break;
	case 459:
		wrLog("【东莞QQ会员包月】");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ会员包月");

		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //如果是ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013"); // qlqq
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026"); // qlqq

			trade.GLQQTrade("QQ会员包月");
		}
		else
		{
			trade.QQTrade("QQ会员包月");

		}

		*/
		

		break;
	case 460:
		//东莞电信空中充值
		wrLog("【东莞电信空中充值】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1);
		break;
	case 461:
		//东莞移动空中充值
		wrLog("【东莞移动空中充值】");
		if(gbl.tgroup == 34 || gbl.tgroup == 38)
		{
			TS_OutPutMsg(gbl.dPosInfo, "因业务合作方系统升级，业务暂停。不便之处，敬请原谅", 1, iSelect);
			return;
		}
		trade.set_in_par(gbl);
		trade.mblAirPay("移动话费直充", 2);
		break;
	case 462:
		wrLog("【东莞广州交罚】");
		bbzero(cTime);			//交通罚款每日21:00分至00:00分暂停交易20081128
		tool.get_time(cTime);
		if(atoi(cTime) >= 210000 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n   交罚系统日结中\n请24:00后再进行交易\n   按【确认】返回",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0232");
		trade.set_in_par(gbl);
		trade.trafficTrade("交通罚款交纳",2);
		break;
	case 463:
		wrLog("【按交费号码补打发票】");
		trade.set_in_par(gbl);
		iRet = trade.newrePrtTrade(1);
		break;
	case 464:
		wrLog("【按系统参考号补打发票】");
		trade.set_in_par(gbl);
		iRet = trade.newrePrtTrade(2);
		break;
	case 465:
        wrLog("【东莞交罚日查询】");
		trade.set_in_par(gbl);
		trade.merInqTrade(5);
        break;
	case 466:
        wrLog("【东莞交罚月查询】");
		trade.set_in_par(gbl);
		trade.merInqTrade(6);
    	break;
	case 478:
		wrLog("东莞日交易查询");
		trade.set_in_par(gbl);
		//trade.merInqTrade(7);
		trade.drcx_online(0);
		break;
	case 479:
		wrLog("东莞月交易查询");
		trade.set_in_par(gbl);
		//trade.merInqTrade(8);
		trade.yjycx_online(0);
		break;
	case 484:
		wrLog("【深圳电信-固话费】");
		trade.set_in_par(gbl);
		trade.sz_dx_trade(epos_menu[id].name);
		break;
	case 485:
		wrLog("【东莞电信充值付费卡售卡】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡",0);
		break;
	case 487:

		if (gbl.bus_trade_flag==1) //如果东莞公交状态为9，处于扣费状态。不能进行交易。
		{
			TS_OutPutMsg(gbl.dPosInfo,"东莞公交交易尚未完成，请稍后再试",0,iSelect);
			break;
		}
	
		wrLog("【东莞公交卡发卡】");
		strcpy(gbl.fee_type,"0214");
		trade.set_in_par(gbl);

		if(gbl.posInfo.rightType!=5 &&gbl.HID_TYPE==0)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行东莞公交读卡器设置\n请通过【857】菜单进入【外部设备设置】选择东莞公交设置。进入设置有公交读卡器，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		if(gbl.posInfo.rightType==5 && gbl.HID_TYPE==1)
		{
			TS_OutPutMsg(gbl.dPosInfo," 清华同方读卡器和明华读卡器，您只能选择使用其中之一。请查看读卡器类型，并通过【857】菜单进入【外部设备设置】 将不匹配的读卡器设置为无",0,iSelect);
			break;
		}

		//判断终端是否有东莞公交不完整交易
		if( trade.busNotEndHdl("公交卡不完整交易") )
		{	
			TS_OutPutMsg(gbl.dPosInfo,"交易确认失败，请稍后再试。联系在线客服或者拨打400-80-11888咨询",1,iSelect);
			break;

		}

		iRet = trade.busTrade(epos_menu[id].name,3); //公交开卡
		break;

	case 488:

		if (gbl.bus_trade_flag==1) //如果终端状态处于扣费状态，不能进行交易
		{
			TS_OutPutMsg(gbl.dPosInfo,"东莞公交交易尚未完成，请稍后再试",0,iSelect);
			break;
		}

		strcpy(gbl.fee_type,"0211");
		trade.set_in_par(gbl);
		wrLog("【东莞公交卡充值】");

		if(gbl.posInfo.rightType!=5 && gbl.HID_TYPE==0)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行东莞公交读卡器设置\n请通过【857】菜单进入【外部设备设置】选择东莞公交设置。进入设置有公交读卡器，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		if(gbl.posInfo.rightType==5 && gbl.HID_TYPE==1)
		{
			TS_OutPutMsg(gbl.dPosInfo," 清华同方读卡器和明华读卡器，您只能选择使用其中之一。请查看读卡器类型，并通过【857】菜单进入【外部设备设置】 将不匹配的读卡器设置为无",0,iSelect);
			break;
		}

		//判断终端是否有东莞公交不完整交易
		if( trade.busNotEndHdl("公交卡不完整交易") )
		{
			//存在不完成交易
			TS_OutPutMsg(gbl.dPosInfo,"交易确认失败，请稍后再试。联系在线客服或者拨打400-80-11888咨询",1,iSelect);
			break;
		}

		iRet = trade.busTrade(epos_menu[id].name,1);
		break;

	case 782: //余额查询，暂时（不用了）

		wrLog("【东莞公交卡余额查询】"); 
		if (gbl.bus_trade_flag==1)// 终端状态为9，东莞公交扣费状态，不能进行交易。
		{
			TS_OutPutMsg(gbl.dPosInfo,"东莞公交交易尚未完成，请稍后再试",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0211");
		trade.set_in_par(gbl);

		if(gbl.posInfo.rightType!=5 && gbl.HID_TYPE==0)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行东莞公交读卡器设置\n请通过【857】菜单进入【外部设备设置】选择对应的东莞公交读卡器设置。进入设置有公交读卡器，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		if(gbl.posInfo.rightType==5 && gbl.HID_TYPE==1)
		{
			TS_OutPutMsg(gbl.dPosInfo," 清华同方读卡器和明华读卡器，您只能选择使用其中之一。请查看读卡器类型，并通过【857】菜单进入【外部设备设置】 将不匹配的读卡器设置为无",0,iSelect);
			break;
		}
		//如果东莞公交不完整交易处理完成
		if( trade.busNotEndHdl("公交卡不完整交易") )
		{
			break;
		}
	
		iRet = trade.busTrade(epos_menu[id].name,0);
		
		break;


	case 785: //东莞通公交充值 cpu卡 只允许使用清华同方读卡器。
		
		wrLog("【东莞通公交充值】");
		
		strcpy(gbl.fee_type,"0371"); //东莞通公交充值业务代码。
		trade.set_in_par(gbl);

		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //只允许清华同方读卡器使用
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 东莞公交读卡器配置错误，本业务仅支持清华同方读卡器，请确认连接清华同方读卡器。\n 并通过【857】菜单进入【外部设备设置】选择东莞公交设置。进入设置有清华同方公交读卡器，设置为无明华读卡器，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		iRet = trade.dgtBusTrade(epos_menu[id].name,2); //莞市通充值

		break;

	case 801: //东莞通余额查询 cpu卡 和m1卡，只允许使用清华同方读卡器 
		
		wrLog("【东莞公交卡余额查询】"); 
		trade.set_in_par(gbl);	
		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //不允许明华读卡器进行充值
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 本业务仅支持清华同方读卡器。请连接清华同方读卡器，并通过【857】菜单进入【外部设备设置】选择【清华同方读卡器】。进入设置为有，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		
		iRet = trade.dgtCardsQuery();
		break;

	case 803:
		wrLog("东莞公交开卡");
		strcpy(gbl.fee_type,"0372");
		trade.set_in_par(gbl);

		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //不允许明华读卡器进行充值
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 本业务仅支持清华同方读卡器。请连接清华同方读卡器，并通过【857】菜单进入【外部设备设置】选择【清华同方读卡器】。进入设置为有，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		iRet=trade.dgtCardSale(epos_menu[id].name);
		if (iRet==0)
		{
			trade.ecardInqHdl(); //查询账户余额
		}
		
		break;

	case 804://东莞公交签到
		
		wrLog("东莞公交签到");
		trade.set_in_par(gbl);
		trade.dgtSignIn();
		break;

	case 805:
		wrLog("东莞公交签退");
		trade.set_in_par(gbl);
		trade.dgtSignOut();
		break;

	case 802: //东莞通旧卡换新卡业务  。只允许使用清华同方读卡器

		wrLog("【东莞公交以旧换新功能】"); 
		strcpy(gbl.fee_type,"0373");
		trade.set_in_par(gbl);
		
		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //不允许明华读卡器进行充值
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 本业务仅支持清华同方读卡器。请连接清华同方读卡器，并通过【857】菜单进入【外部设备设置】选择【清华同方读卡器】。进入设置为有，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		iRet = trade.dgtCardTransfer(epos_menu[id].name); //东莞公交以旧换新功能 

		break;

	case 807:
		wrLog("东莞通公交卡充值及消费查询");
		trade.set_in_par(gbl);
		trade.showDGTALLTrade();
		break;

	case 810:
		wrLog("【东莞通开卡激活查询】"); 
		trade.set_in_par(gbl);
		
		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //不允许明华读卡器进行充值
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 本业务仅支持清华同方读卡器。请连接清华同方读卡器，并通过【857】菜单进入【外部设备设置】选择【清华同方读卡器】。进入设置为有，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		
		iRet = trade.getCardStatusResult();
		break;

	case 808:
		wrLog("东莞通公交卡充值交易查询");
		trade.set_in_par(gbl);
		trade.showDGTChargeTrade();
		break;

	case 809:
		wrLog("【电信直充(全国)】");
		strcpy(gbl.fee_type,"0367");//全国电信
		trade.set_in_par(gbl);
		trade.phone_pay_trade(epos_menu[id].name);
		break;

	case 500:
		wrLog("【东莞移动售卡30】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡",1);
		break;
	case 501:
		wrLog("【东莞移动售卡50】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡",2);
		break;
	case 502:
		wrLog("【东莞移动售卡100】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡",3);
		break;
	case 504:
		wrLog("【东莞移动空中充值30】");
		trade.set_in_par(gbl);
		//trade.mblAirPay("移动话费直充", 2, 30);
		trade.mblAirPayForGDYD("移动话费直充",30);
		break;
	case 505:
		wrLog("【东莞移动空中充值50】");
		trade.set_in_par(gbl);
		//trade.mblAirPay("移动话费直充", 2, 50);
		trade.mblAirPayForGDYD("移动话费直充",50);
		break;
	case 506:
		wrLog("【东莞移动空中充值100】");
		trade.set_in_par(gbl);
		//trade.mblAirPay("移动话费直充", 2, 100);
		trade.mblAirPayForGDYD("移动话费直充",100);
		break;
	case 507:
		wrLog("【东莞移动空中充值300】");
		trade.set_in_par(gbl);
		//trade.mblAirPay("移动话费直充", 2, 300);
		trade.mblAirPayForGDYD("移动话费直充",300);
		break;
	case 508:
		wrLog("【东莞移动空中充值500】");
		trade.set_in_par(gbl);
		//trade.mblAirPay("移动话费直充", 2, 500);
		trade.mblAirPayForGDYD("移动话费直充",500);
		break;
	case 510: 
		wrLog("【东莞固话及小灵通充值30】");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡",1);
		break;
	case 511: 
		wrLog("【东莞固话及小灵通充值50】");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡",2);
		break;
	case 512: 
		wrLog("【东莞固话及小灵通充值100】");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("小灵通充值卡",3);
		break;
	case 518:
		wrLog("【东莞电信空中充值30】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 30);
		break;
	case 519:
		wrLog("【东莞电信空中充值50】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 50);
		break;
	case 520:
		wrLog("【东莞电信空中充值100】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 100);
		break;
	case 522:
		wrLog("【东莞联通售卡30】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通充值卡",1);
		break;
	case 523:
		wrLog("【东莞联通售卡50】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通充值卡",2);
		break;
	case 524:
		wrLog("【东莞联通售卡100】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("联通充值卡",3);
		break;
	case 526:
		wrLog("天下通卡10");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("天下通卡",1);
		break;
	case 527:
		wrLog("东莞天下通卡15");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("天下通卡",2);
		break;
	case 528:
		wrLog("东莞天下通卡30");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("天下通卡",3);
		break;
	case 530:
		wrLog("【东莞Q币充值10】");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q币充值",1);

		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q币充值",1);
		}
		else
		{
			trade.QQTrade("Q币充值",1);
		}

		*/
		

		break;
	case 531:
		wrLog("【东莞Q币充值30】");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q币充值",2);

		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q币充值",2);
		}
		else
		{
			trade.QQTrade("Q币充值",2);

		}
		*/


		break;
	case 532:
		wrLog("【东莞Q币充值50】");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q币充值",3);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q币充值",3);
		}
		else
		{
		
			trade.QQTrade("Q币充值",3);
		}
		
		*/

		break;
	case 533:
		wrLog("【东莞Q币充值100】");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q币充值",4);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q币充值",4);
		}
		else
		{
			trade.QQTrade("Q币充值",4);

		}
		
		 */

		break;
	case 537:
		wrLog("【东莞QQ会员1个月】");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ会员包月",1);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ会员包月",1);
		}
		else
		{
		
			trade.QQTrade("QQ会员包月",1);
		}

		*/
		

		break;
	case 538:
		wrLog("【东莞QQ会员3个月】");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ会员包月",2);

		/*
	
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ会员包月",2);
		}
		else
		{
		
			trade.QQTrade("QQ会员包月",2);
		}

        */
		

		break;
	case 539:
		wrLog("【东莞QQ会员6个月】");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ会员包月",3);

		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ会员包月",3);
		}
		else
		{
		
			trade.QQTrade("QQ会员包月",3);

		}
		*/
		

		break;
	case 540:
		wrLog("【东莞QQ会员12个月】");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ会员包月",4);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ会员包月",4);
		}
		else
		{
			trade.QQTrade("QQ会员包月",4);
		}
		
		*/

		break;
	case 546:
		wrLog("【东莞电信充值付费卡30】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡",2);
		break;
	case 547:
		wrLog("【东莞电信充值付费卡50】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡",3);
		break;
	case 548:
		wrLog("【东莞电信充值付费卡100】");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡",4);
		break;
	case 542:
		//东莞e卡充值
		wrLog("【东莞e卡充值】");
		trade.set_in_par(gbl);
		trade.ecardMnyIn("交费易账户转账",1);
		TS_HangUp(gbl.dPosInfo);
		closesocket(sd);
		break;
	case 549:
		//联通直充 全省接口
		wrLog("【联通直充 全省接口】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 4);
		break;
	case 550:
		//易办事信用卡还款
		wrLog("【信用卡还款】");
		bbzero(gbl.fee_type);
		strcpy(gbl.fee_type,"0284");
		trade.set_in_par(gbl);
		trade.Credit_Card_Repayments("信用卡还款");
		break;
	case 551:
		wrLog("【广州汽车年票】");
		trade.set_in_par(gbl);
		trade.car_np_Trade("广州汽车年票");
		break;
	case 552:
		wrLog("【广州e卡卡内转账】");
		strcpy(gbl.fee_type, "0074");
		trade.set_in_par(gbl);
		trade.eCard_Trans("e卡卡内转账");
		break;
	case 553:
		wrLog("【东莞e卡卡内转账】");
		strcpy(gbl.fee_type, "0075");
		trade.set_in_par(gbl);
		trade.eCard_Trans("e卡卡内转账");
		break;
	case 556:
		wrLog("【移动充值卡30】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡", 1);
		break;
	case 557:
		wrLog("【电信充值付费卡200元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 5);
		break;
	case 558:
		wrLog("【电信充值付费卡300元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 6);
		break;
	case 559:
		wrLog("【电信充值付费卡500元】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 7);
		break;
	case 560:
		//联通直充 全省接口
		wrLog("【联通直充30元 全省接口】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 4, 30);
		break;
	case 561:
		//联通直充 全省接口
		wrLog("【联通直充50元 全省接口】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 4, 50);
		break;
	case 562:
		//联通直充 全省接口
		wrLog("【联通直充100元 全省接口】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 4, 100);
		break;
	case 563:
		//联通直充 全省接口
		wrLog("【联通直充200元 全省接口】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 4, 200);
		break;
	case 564:
		//联通直充 全省接口
		wrLog("【联通直充500元 全省接口】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 4, 500);
		break;
	case 565:
		//银联信用卡还款
		wrLog("【信用卡还款】");
		bbzero(gbl.fee_type);
		strcpy(gbl.fee_type,"0286");
		trade.set_in_par(gbl);
		trade.Credit_Card_Repayments("信用卡还款");
		break;
	case 566:
		wrLog("【电信充值付费卡200元】");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 5);
		break;
	case 567:
		wrLog("【电信充值付费卡300元】");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 6);
		break;
	case 568:
		wrLog("【电信充值付费卡500元】");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 7);
		break;
	case 569:
		wrLog("【东莞电信空中充值200】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 200);
		break;
	case 570:
		wrLog("【东莞电信空中充值300】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 300);
		break;
	case 571:
		wrLog("【东莞电信空中充值500】");
		trade.set_in_par(gbl);
		trade.mblAirPay("电信话费直充", 1, 500);
		break;
	case 572:
		wrLog("【酬金结转】");
		trade.set_in_par(gbl);
		trade.Customer_Trade();
		break;
	case 574:
		wrLog("【含查询的电信直充——单一话费】");
		trade.set_in_par(gbl);
		trade.newAirPay("电信直充",1);
		break;
	case 575:
		wrLog("【含查询的电信直充——ADSL】");
		trade.set_in_par(gbl);
		trade.newAirPay("电信直充",2);
		break;
	case 576:
		wrLog("【含查询的电信直充——按合同号】");
		trade.set_in_par(gbl);
		trade.newAirPay("电信直充",3);
		break;
	case 577:
		wrLog("e卡－>查询余额");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.cardInqHdl();
		break;
	case 581:
		wrLog("最后一次打印补打");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_last("最后一次打印补打");
		break;
	case 582:
		wrLog("【IPOS老板日交易查询】");
		trade.set_in_par(gbl);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 0);
		trade.drcx_online(1);
		break;
	case 583:
		wrLog("【界面标题设置】");
		trade.set_in_par(gbl);
		trade.SetTitle();
		break;
	case 584:
		wrLog("【空中充值冲正申请】");
		trade.set_in_par(gbl);
		trade.revAppli(epos_menu[id].name);
		break;
	case 585:
		wrLog("【最新更新提示】");
		trade.set_in_par(gbl);
		trade.GetNewMessage();
		break;
	case 586:
		wrLog("【全省 - 移动充值卡30】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡", 1);
		break;
	case 587:
		wrLog("【全省 - 移动充值卡50】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡", 2);
		break;
	case 588:
		wrLog("【全省 - 移动充值卡100】");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("移动充值卡", 3);
		break;
	case 592:
		wrLog("【东莞宽带套餐续存】");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,1);
		break;	
	case 593:
		wrLog("【东莞宽带套餐续存】");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,2);
		break;
	case 594:
		wrLog("【东莞宽带套餐续存】");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,3);
		break;
	case 595:
		wrLog("【东莞宽带套餐续存】");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,4);
		break;
	case 596:
		wrLog("【公交90次本月生效】");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 3, gbl.pki_card, gbl.signin_serial);
		break;
	case 597:
		wrLog("【羊城通余额查询】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 8);
		break;
	case 598:
		wrLog("【羊城通普通钱包充值】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}

		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 1);
	

		break;
	case 599:
		//联通空中充值
		wrLog("【连欣联通直充30元】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3, 30);
		break;
	case 600:
		//联通空中充值
		wrLog("【连欣联通直充50元】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3, 50);
		break;
	case 601:
		//联通空中充值
		wrLog("【连欣联通直充100元】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3, 100);
		break;
	case 612:
		//联通空中充值
		wrLog("【连欣联通直充200元】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3, 200);
		break;
	case 602:
		//联通空中充值
		wrLog("【连欣联通直充300元】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3, 300);
		break;
	case 603:
		//联通空中充值
		wrLog("【连欣联通直充500元】");
		trade.set_in_par(gbl);
		trade.mblAirPay("联通话费直充", 3, 500);
		break;
	case 604:
		wrLog("【外部设备设置】");
		trade.set_in_par(gbl);
		trade.updatePosPar( epos_menu[id].name,1);
		break;
	case 605:
		wrLog("【公交90次下月生效】");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 4, gbl.pki_card, gbl.signin_serial);
		break;
	case 606:
		wrLog("【公交定线】");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 5, gbl.pki_card, gbl.signin_serial);
		break;
	case 607:
		wrLog("【地铁限次】");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 6, gbl.pki_card, gbl.signin_serial);
		break;
	case 608:
		wrLog("【直充补打发票】");
		trade.set_in_par(gbl);
		trade.newrePrtTrade(1);
		break;
	case 609:
		wrLog("珠海电信后付费");		
		trade.set_in_par(gbl);
		trade.foshanTrade(epos_menu[id].name, "0756");
		break;
	case 610:
		wrLog("【体彩账户充值】");
		strcpy(gbl.fee_type,"0243");
		trade.set_in_par(gbl);
		trade.RaffleTrade("体彩账户充值");
		break;
	case 615:
		wrLog("大客户—提交号码");		
		trade.set_in_par(gbl);
		trade.Submission(epos_menu[id].name);
		break;
	case 616:
		wrLog("大客户—查看充值表");		
		trade.set_in_par(gbl);
		trade.Checkmission(epos_menu[id].name);
		break;
	case 617:
		wrLog("宽带续存");		
		trade.set_in_par(gbl);
		trade.new_DG_ADSL_XC(epos_menu[id].name);
		break;
	case 618:
		wrLog("【移动优惠充值】");
		trade.set_in_par(gbl);
		trade.mblAirPay(epos_menu[id].name,2,0,2);
		break;
	case 619:
		wrLog("【联通优惠充值】");
		trade.set_in_par(gbl);
		trade.mblAirPay(epos_menu[id].name,3,0,2);
		break;
	case 620:
		wrLog("大客户—确认交费");		
		trade.set_in_par(gbl);
		trade.Paymission(epos_menu[id].name);
		break;
	case 621:
		wrLog("【代理商e卡子母卡转账】");
		trade.set_in_par(gbl);
		trade.cashTransfer_online(1); //修改为直接从页面上面跳转进行。
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e卡－>代理商e卡子母卡转账查询余额");
			memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
			memcpy(gbl.fee_type, "0083", 4);
			trade.set_in_par(gbl);
			{
				char amount[12+1];
				bbzero(amount);
				strcpy(amount,"1");
				trade.cardInqHdl(amount);
			}		
		}
		break;
	case 622:
		//最后一笔交易查询
		trade.set_in_par(gbl);
		iRet=trade.merInqTrade(9);
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e卡－>最后一笔交易查询后查询余额");
			memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
			memcpy(gbl.fee_type, "0083", 4);
			trade.set_in_par(gbl);
			{
				char amount[12+1];
				bbzero(amount);
				strcpy(amount,"1");
				trade.cardInqHdl(amount);
			}		
		}
		break;
	case 623:
		wrLog("移动版-最后一次打印补打");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_last("最后一次打印补打",1);
		break;
	case 624:
		wrLog("e卡－>查询酬金");
		if(atoi(gbl.dPosInfo.prmVer)>=9000)
		{

		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.yj_InqHdl();

		}
		else
		{
			TS_OutPutMsg(gbl.dPosInfo,"尚未开通\n  请按【确认】返回" ,1, iSelect);
		
		}

		break;
	case 625:
		wrLog("【移动版-IPOS老板日交易查询】");
		trade.set_in_par(gbl);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 0, 1);
		trade.drcx_online(1);
		break;
	case 627:
		//最后一笔交易查询
		trade.set_in_par(gbl);
		iRet=trade.merInqTrade(10);
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e卡－>最后一笔交易查询后查询余额");
			memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
			memcpy(gbl.fee_type, "0083", 4);
			trade.set_in_par(gbl);
			{
				char amount[12+1];
				bbzero(amount);
				strcpy(amount,"1");
				trade.cardInqHdl(amount);
			}		
		}
		break;
	case 628:
		wrLog("非移动版-最后一次打印补打");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_last("最后一次打印补打",2);
		break;
	case 629:

		wrLog("e卡－>查询酬金");
		if(atoi(gbl.dPosInfo.prmVer)>=9000)
		{

		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.yj_InqHdl();

		}
		else
		{
			TS_OutPutMsg(gbl.dPosInfo,"尚未开通\n  请按【确认】返回" ,1, iSelect);
		
		}

		break;
	case 630:
		wrLog("【非移动版-IPOS老板日交易查询】");
		trade.set_in_par(gbl);
        trade.mxcx_online(1);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 0, 2);
		break;
	case 631:
		wrLog("【移动空中充值30】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 30,0,1);
		trade.mblAirPayForGDYD("移动话费直充", 30,0,1);
		break;
	case 632:
		wrLog("【移动空中充值50】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 50,0,1);
		trade.mblAirPayForGDYD("移动话费直充", 50,0,1);
		break;
	case 633:
		wrLog("【移动空中充值100】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 100,0,1);
		trade.mblAirPayForGDYD("移动话费直充", 100,0,1);
		break;
	case 634:
		wrLog("【移动空中充值300】");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("充  值", 2, 300,0,1);
		trade.mblAirPayForGDYD("移动话费直充", 300,0,1);
		break;
	case 636:
		wrLog("全省交罚（小型汽车）");

		memcpy(gbl.car_type, "01", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("全省交罚代缴");
		break;
	case 637: 
		wrLog("全省交罚（大型汽车）");
		memcpy(gbl.car_type, "02", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("全省交罚代缴");
		break;	
	case 638: 	
		wrLog("全省交罚（外籍汽车）");
		memcpy(gbl.car_type, "03", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("全省交罚代缴");
		break;
	case 639:	
		wrLog("全省交罚（挂车）");
		memcpy(gbl.car_type, "04", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("全省交罚代缴");
		break;
	case 641:
		wrLog("客服中心-在线客服");		
		trade.set_in_par(gbl);
		trade.OpenKF(1);
		break;
	case 642:
		wrLog("客服中心-自助服务");		
		trade.set_in_par(gbl);
		trade.OpenKF(0);
		break;
	case 643:
		wrLog("【QQ黄钻】");
		strcpy(gbl.fee_type,"0274");
		trade.set_in_par(gbl);
		trade.QQTrade("黄钻贵族包月");

		/*
		
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1014");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1022");
			trade.GLQQTrade("黄钻贵族包月");
		}
		else
		{
			trade.QQTrade("黄钻贵族包月");

		}

		*/

		break;
	case 644:
		wrLog("【QQ红钻】");
		strcpy(gbl.fee_type,"0087");
		trade.set_in_par(gbl);
		trade.QQTrade("红钻贵族包月");


		/*
		
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1011");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1024");
			trade.GLQQTrade("红钻贵族包月");

		}
		else
		{
			trade.QQTrade("红钻贵族包月");

		}

		*/
		

		break;
	case 645:
		wrLog("【QQ蓝钻】");
		strcpy(gbl.fee_type,"0088");
		trade.set_in_par(gbl);
		trade.QQTrade("蓝钻贵族包月");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1010");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1023");
			trade.GLQQTrade("蓝钻贵族包月");

		}
		else
		{
			trade.QQTrade("蓝钻贵族包月");

		}

		*/
		

		break;
	case 646:
		wrLog("【QQ紫钻】");
		strcpy(gbl.fee_type,"0273");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ堂紫钻包月");

		/*
		
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1012");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1025");
			trade.GLQQTrade("QQ堂紫钻包月");
		}
		else
		{
		
			trade.QQTrade("QQ堂紫钻包月");
		}
		*/

		break;
	case 647:
		wrLog("【东莞QQ黄钻】");
		strcpy(gbl.fee_type,"0278");
		trade.set_in_par(gbl);
		trade.QQTrade("黄钻贵族包月");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000) 
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1014");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1022");
			trade.GLQQTrade("黄钻贵族包月");
		}
		else
		{
			trade.QQTrade("黄钻贵族包月");

		}
		*/
		

		break;
	case 648:
		wrLog("【东莞QQ红钻】");
		strcpy(gbl.fee_type,"0293");
		trade.set_in_par(gbl);
		trade.QQTrade("红钻贵族包月");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1011");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1024");
			trade.GLQQTrade("红钻贵族包月");
		}
		else
		{
		
			trade.QQTrade("红钻贵族包月");
		}
		*/

		break;
	case 649:
		wrLog("【东莞QQ蓝钻】");
		strcpy(gbl.fee_type,"0294");
		trade.set_in_par(gbl);
		trade.QQTrade("蓝钻贵族包月");
		/*
		
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1010");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1023");
			trade.GLQQTrade("蓝钻贵族包月");
		}
		else
		{
			trade.QQTrade("蓝钻贵族包月");

		}

		*/
		
		break;
	case 650:
		wrLog("【东莞QQ紫钻】");
		strcpy(gbl.fee_type,"0277");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ堂紫钻包月");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1012");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1025");
			trade.GLQQTrade("QQ堂紫钻包月");
		}
		else
		{

			trade.QQTrade("QQ堂紫钻包月");
		}
		*/
	
		break;
	case 651:
		wrLog("【逢万送十 Q币赠送领取】");
		trade.set_in_par(gbl);
		trade.Present_handsel(epos_menu[id].name);
		break;
	case 652:
		wrLog("【违章罚单查询】");
		TS_OpenIE(gbl.dPosInfo, "http://www.payeasy.com.cn/payeasy/trafficQuery/checkCar1.jsp");
		Sleep(5000);
		break;
	case 653:
		wrLog("重打印发票_按系统参考号补打");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_fail("按系统参考号补打发票");
		break;
	case 654:
		wrLog("【电子代办的电信直充——单一话费】");
		trade.set_in_par(gbl);
		trade.DZDBAirPay("电信直充",1,0);
		break;
	case 655:
		wrLog("【电子代办的电信直充——ADSL】");
		trade.set_in_par(gbl);
		trade.DZDBAirPay("电信直充",2,0);
		break;
	case 656:
		wrLog("【电子代办的电信直充——按合同号】");
		trade.set_in_par(gbl);
		trade.DZDBAirPay("电信直充",3,0);
		break;
	case 658:
		wrLog("【空中放号——白卡销售】");
		trade.set_in_par(gbl);
		trade.Sell_blank_phonecard("白卡采购", 0);
		break;
	case 659:
		wrLog("【空中放号——白卡销售确认1】");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard("白卡到货确认", 0);
		break;
	case 660:
		wrLog("【空中放号——号码销售1】");
		trade.set_in_par(gbl);
		trade.Sell_phoneno("号码销售");
		break;
	case 661:
		wrLog("【空中放号——白卡销售确认2】");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard_hdl("白卡到货确认", 0);
		break;
	case 662:
		wrLog("【空中放号——号码销售2】");
		trade.set_in_par(gbl);
		trade.Sell_phoneno_hdl("号码销售");
		break;
	case 663:
		wrLog("天下通卡(网易)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("网易游戏卡",0,1);
		break;
	case 664:
		wrLog("天下通卡(搜狐)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("搜狐游戏卡");
		break;
	case 665:
		wrLog("天下通卡(盛大)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("盛大游戏卡");
		break;
	case 666:
		wrLog("天下通卡(世纪天成)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("世纪天成游戏卡");
		break;
	case 671:
		wrLog("卡充移动充值50");
		trade.set_in_par(gbl);
		trade.mblAirPay("充  值", 10, 50);
		break;
	case 672:
		wrLog("卡充移动充值100");
		trade.set_in_par(gbl);
		trade.mblAirPay("充  值", 10, 100);
		break;
	case 673:
		wrLog("卡充移动充值300");
		trade.set_in_par(gbl);
		trade.mblAirPay("充  值", 10, 300);
		break;
	case 674:
		wrLog("卡充移动充值500");
		trade.set_in_par(gbl);
		trade.mblAirPay("充  值", 10, 500);
		break;
	case 676:
		wrLog("【修改转账密码】");
		trade.set_in_par(gbl);
		trade.chgTransferPwd( epos_menu[id].name );
		break;
	case 678:
		wrLog("【色带订购】");
		trade.set_in_par(gbl);
		trade.sell_pos(9);
		break;
	case 679:
		wrLog("【交费易海报订购】");
		trade.set_in_par(gbl);
		trade.sell_pos(5);
		break;
	case 680:
		wrLog("【交费易标准招牌订购】");
		trade.set_in_par(gbl);
		trade.sell_pos(6);
		break;
	case 681:
		wrLog("【维护服务订购】");
		trade.set_in_par(gbl);
		trade.sell_pos(7);
		break;
	case 682:
		wrLog("【打印机订购】");
		trade.set_in_par(gbl);
		trade.sell_pos(8);
		break;
	case 683:
		wrLog("【电信业务预受理】");
		trade.set_in_par(gbl);
		trade.telaffair_preaccept("电信业务预受理");
		break;
	case 684:
		wrLog("【列表终结】");
		//trade.set_in_par(gbl);
		//trade.pep_distribute();
		break;
	case 687:
		wrLog("【东莞天下通游戏卡】");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sell_txtgame_card("天下通游戏卡");
		break;
	case 691:
		wrLog("【游戏直充金额列表】");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_showAmount("游戏直充金额列表");
		break;
	case 692:
		wrLog("【游戏直充1】");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_do1("游戏直充充值1");
		break;
	case 693:
		wrLog("【游戏直充2】");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_do2("游戏直充充值2");
		break;
	case 694:
		wrLog("【游戏直充游戏列表-全部】");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_showGames("游戏直充游戏列表", 0);
		break;
	case 695:
		wrLog("【游戏直充游戏列表-关键字】");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_showGames("游戏直充游戏列表", 1);
		break;
	case 705:
		wrLog("【联通空中放号——白卡销售】");
		trade.set_in_par(gbl);
		trade.Sell_blank_phonecard("白卡采购", 1);
		break;
	case 706:
		wrLog("【联通空中放号——白卡销售确认1】");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard("白卡到货确认", 1);
		break;
	case 707:
		wrLog("【联通空中放号——号码销售1】");
		trade.set_in_par(gbl);
		trade.Unicom_Sell_Phoneno("号码销售");
		break;
	case 708:
		wrLog("【联通空中放号——白卡销售确认2】");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard_hdl("白卡到货确认", 1);
		break;
	case 709:
		wrLog("【联通空中放号——号码销售2】");
		trade.set_in_par(gbl);
		trade.Unicom_Sell_Phoneno_Hdl1("号码销售");
		break;
	case 710:
		wrLog("【联通空中放号——号码销售3】");
		trade.set_in_par(gbl);
		trade.Unicom_Sell_Phoneno_Hdl2("号码销售");
		break;
	case 711:
		wrLog("【电信充值付费卡20元】");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("电信充值付费卡", 1);
		break;
	case 712:

		wrLog("【长途汽车票选站】");
		strcpy(gbl.fee_type, "0346");
		trade.set_in_par(gbl);
		trade.coachTicketTradeGetInfo();
		break;
	case 713:

		wrLog("【长途汽车票查询】");
		strcpy(gbl.fee_type, "0346");
		trade.set_in_par(gbl);
		trade.coachTicketTradeQuery();
		break;
	case 714:

		wrLog("【长途汽车票预订】");
		strcpy(gbl.fee_type, "0346");
		trade.set_in_par(gbl);
		trade.coachTicketTradePay();
		break;
	case 721:
		wrLog("【设置打印纸长度】");
		trade.setPrintPaper();
		break;
	case 723:
		wrLog("【最近24小时交易查询】");
		trade.set_in_par(gbl);
		trade.cxcz_online(0);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 1, 0, 1);
		break;
	case 724:
		wrLog("【最近24小时交易查询-非移动】");
		trade.set_in_par(gbl);
        trade.cxcz_online(0);
		//trade.online_ser("cxcz");//查询，冲正，转换为在线服务
        //trade.iposTxnEnq( epos_menu[id].name, 0, 1, 2, 1);
		break;
	case 725:
		wrLog("【最近24小时交易查询-移动】");
		trade.set_in_par(gbl);
        trade.cxcz_online(0);
        //trade.online_ser("cxcz");//查询，冲正，转换为在线服务
		//trade.iposTxnEnq( epos_menu[id].name, 0, 1, 1, 1);
		break;
	case 729:
		wrLog("【院线通——电影兑换券】");
	
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0900");
		trade.set_in_par(gbl);
		trade.sellCardTrade("院线通电影兑换券", 1);
		break;
	case 730:
		wrLog("【院线通——电影票查询】");	
	
		TS_OpenIE(gbl.dPosInfo, "http://gd.118114.cn/movie");
		break;
	case 735:
		wrLog("【珠海公交充值】");
// 		trade.set_in_par(gbl);
// 		trade.ZHTrafficCardCharge("珠海公交卡充值");
		ZHBusTrade::ZHTrafficCardCharge(gbl);
		break;
	case 736:
		wrLog("【羊城通成功交易查询】");
		trade.set_in_par(gbl);
		trade.YctSuccTxnEnq("打印日交易清单");
		break;
	case 737:
		wrLog("【羊城通送款单登记】");
		trade.set_in_par(gbl);
		trade.printYctBill("送款单登记");
		break;
	case 741:
		wrLog("[翼支付账户充值]");
		strcpy(gbl.fee_type,"0361");//翼支付业务代码0361
		trade.set_in_par(gbl);
		trade.bestPay_rechargeHdl("翼支付账户充值",0);
		break;

	case 742:
		wrLog("腾讯QQ充值(天下通接口)");
		strcpy(gbl.fee_type,"0341");
		trade.set_in_par(gbl);
		trade.qqGame_recharge_do("腾讯QQ充值");//可充全国，天下通接口
		break;
	case 747:
		wrLog("薪加薪理财审核权限设置");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_checkauthorcfg_hdl("薪加薪理财审核权限设置");//薪加薪理财审核权限设置
		break;
    case 748:
		wrLog("薪加薪理财资金到账查询");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_pymreceived_InqHdl("薪加薪理财资金到账查询");//薪加薪资金划拨交易查询接口
		break;
	case 750:
		wrLog("薪加薪理财二级审核设置");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_LLcheckcfg_hdl("薪加薪理财二级审核设置");//薪加薪理财二级审核设置
		break;
	case 751:
		wrLog("薪加薪理财三级审核设置");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_LLLcheckcfg_hdl("薪加薪理财三级审核设置");//薪加薪理财三级审核设置
		break;
	case 760:
		wrLog("薪加薪理财划出账号设置");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountcfg_hdl(0,"薪加薪理财划出账号设置");//薪加薪理财划出账号设置
		break;
	case 761:
		wrLog("薪加薪理财汇入账号设置");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountcfg_hdl(1,"薪加薪理财汇入账号设置");//薪加薪理财汇入账号设置
		break;
	case 762:
		wrLog("薪加薪理财二级审核设置查询");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_checkconfig_InqHdl(0,"薪加薪理财二级审核设置查询");//薪加薪理财二级审核设置查询
		break;
	case 763:
		wrLog("薪加薪理财三级审核设置查询");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_checkconfig_InqHdl(1,"薪加薪理财三级审核设置查询");//薪加薪理财三级审核设置查询
		break;
	case 764:
		wrLog("薪加薪理财划出账号设置查询");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountconfig_InqHdl(0,"薪加薪理财划出账号设置查询");//薪加薪理财划出账号设置查询
		break;
	case 765:
		wrLog("薪加薪理财汇入账号设置查询");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountconfig_InqHdl(1,"薪加薪理财汇入账号设置查询");//薪加薪理财汇入账号设置查询
		break;
	case 766:
		wrLog("查询成功交易");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_txn_InqHdl(0,"查询成功交易");//薪加薪资金划拨查询成功交易
		break;
	case 767:
		wrLog("查询成功交易");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_txn_InqHdl(1,"查询所有交易");//薪加薪资金划拨查询所有交易
		break;
	case 769:
		wrLog("修改管理员密码");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_updatepwd_Hdl(0,"修改管理员密码");//薪加薪资金划拨修改管理员密码
		break;
	case 770:
		wrLog("薪加薪资金划拨录入");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_funds_transferhdl("薪加薪资金划拨录入");//薪加薪资金划拨录入
		break;
	case 771:
		wrLog("修改录入员密码");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_updatepwd_Hdl(1,"修改录入密码");//薪加薪资金划拨修改录入员密码
		break;
	case 772:
		wrLog("薪加薪资金划拨审核");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_txn_checkHdl("薪加薪资金划拨审核");//薪加薪资金划拨审核
		break;
	case 774:
		wrLog("【羊城通读卡器】");
		strcpy(gbl.fee_type,"0248");
		trade.set_in_par(gbl);
		trade.Yct_ICReader_purchaseHdl("羊城通读卡器");//羊城通白卡/套餐购买
		break;
	case 776:
		wrLog("【羊城通现金充值】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 7);
		break;
	case 777:
		wrLog("【羊城通自动充值】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		CYCTYPTrade::Yct_AutoTradeHdl(gbl,epos_menu[id].name);    //羊城通充值金充值/签约账户充值
		break;
	case  779:
		wrLog("【羊城通充值撤销】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		trade.set_in_par(gbl);
		trade.Yct_charge_cancel("羊城通充值撤销");//【羊城通充值撤销】
		break;
	case 780:
		wrLog("【羊城通卡扩展应用】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		trade.set_in_par(gbl);
		trade.Yct_Card_Extend("羊城通卡扩展应用");//【羊城通充值撤销】
		break;
	case 778:
		wrLog("【羊城通卡间余额转移】");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n 您还没有进行羊城通读卡器设置\n请通过【857】菜单进入【外部设备设置】选择羊城通设置。进入设置有羊城通，确认后重启程序。\n请按【确认】返回",0,iSelect);
			break;
		}
		CYCTBlnTransfer::BalanceTransfer_TradeHdl(gbl,yct_data,"羊城通卡间余额转移");
		break;

	case 783:

		strcpy(gbl.fee_type,"0365");
		wrLog("全国水电煤业务");
		trade.set_in_par(gbl);
		trade.sdm_district_service("水电煤缴费"); //地市查询
		break;

	case 784:
		strcpy(gbl.fee_type,"0365");
		wrLog("全国水电煤业务，查询交费城市的费用情况");
		trade.set_in_par(gbl);
		trade.showSMD_cityList("水电煤业务查询"); //根据省编码来查询该地区开通的城市
		break;
	case 786:
		strcpy(gbl.fee_type,"0365");
		trade.set_in_par(gbl);
		trade.sdm_trade("水电煤气缴费");// 扣费地区查询
		break;

	case 787:
		int iRet;
		strcpy(gbl.fee_type,"0365");
		trade.set_in_par(gbl);
		trade.sdm_handle("水电煤气缴费"); 
		break;
	
	case 788:

		memset(gbl.fee_type,0,sizeof(gbl.fee_type));
		strcpy(gbl.fee_type,"0366");
		trade.set_in_par(gbl);
		iRet=trade.g3_trade("3G流量卡直充");
		if (iRet==0)
		{
			trade.ecardInqHdl(); //查询账户余额
		}

		break;
	case  789: //联通全国话费充值

		strcpy(gbl.fee_type,"0370");
		trade.set_in_par(gbl);
		trade.phone_pay_trade(epos_menu[id].name); 
		break;
	case 790: //移动充值全国

		strcpy(gbl.fee_type,"0369");
		trade.set_in_par(gbl);
		trade.phone_pay_trade(epos_menu[id].name); 

		break;
	case 800:
		wrLog("翼支付卡");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "1000"); //卡号前两位是10 代表是翼支付卡
		trade.set_in_par(gbl);
		//trade.sellCardTrade("翼支付卡");
		trade.sellCardTrade("翼支付卡");
		break;

	default:
		TS_OutPutMsg(gbl.dPosInfo, " \n\n\n即将开通，敬请关注！", 1,iSelect);
		break;
	}
}

//挂机
int CMenuHdl::HangUp(SOCKET in_sd){
	return TS_HangUp(gbl.dPosInfo);
}

int CMenuHdl::UpdateFiles(){
	if(ftpFiles.updateNum==1)
	{
		if(memcmp(gbl.pos_no, "60011067", 8)==0)
		{
			TSFtpFiles tmpftpFiles;
			memcpy(&tmpftpFiles, &ftpFiles, sizeof(TSFtpFiles));
			strcpy(tmpftpFiles.ftpFiles[0].srcFile, "ipos_try.exe.liz");
			return TS_UpdateFiles(gbl.dPosInfo, tmpftpFiles);
		}
		else if(atoi(gbl.dPosInfo.prmVer)>=9000 && atoi(gbl.dPosInfo.prmVer)<=9006)
		{
			TSFtpFiles tmpftpFiles;
			memcpy(&tmpftpFiles, &ftpFiles, sizeof(TSFtpFiles));
			strcpy(tmpftpFiles.ftpFiles[0].srcFile, "ipos.exe");
			return TS_UpdateFiles(gbl.dPosInfo, tmpftpFiles);
		}
		else if(gbl.posInfo.leftType=='\x0E')
		{
			TSFtpFiles tmpftpFiles;
			memcpy(&tmpftpFiles, &ftpFiles, sizeof(TSFtpFiles));

			memcpy(&(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum]), &(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum-1]), sizeof(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum]));
			strcpy(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum-1].srcFile, "print.dll.liz");
			strcpy(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum-1].destFile, "print_new.dll");
			tmpftpFiles.updateNum++;

			memcpy(&(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum]), &(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum-1]), sizeof(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum]));
			strcpy(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum-1].srcFile, "keyboard.dll.liz");
			strcpy(tmpftpFiles.ftpFiles[tmpftpFiles.updateNum-1].destFile, "keyboard_new.dll");
			tmpftpFiles.updateNum++;

			return TS_UpdateFiles(gbl.dPosInfo, tmpftpFiles);
		}
		else
		{
			return TS_UpdateFiles(gbl.dPosInfo, ftpFiles);
		}
	}
	else
	{
		return 0;
	}
}


//UINT YCT3Monitor(LPVOID lpv)
// DWORD WINAPI YCT3Monitor(LPVOID lpv)
// {
// 	int iRet = 0;
// 	int menu_id;			//本地菜单上送的菜单ID
// 	int main_group = 1;			//主菜单的组别号
// 	char main_ver[8+1];		//主菜单的版本号
// 	char menu_ver[8+1];     //菜单版本号
// 	SOCKET sd;
// 	CGlobal gbl;
// 	YCT_DATA yct_data;
// 
// 	memcpy(&gbl, (void *)lpv, sizeof(gbl));
//     
// 	Coninfo coninfo;
// 	memcpy(&coninfo, (void *)lpv, sizeof(Coninfo));
// 	if(coninfo.sd>=CON_MAX)
// 	{
// 		free((Coninfo*)lpv);
// 	}
// 	else
// 	{
// 		NULL;
// 	}
// 	
// 	sd = coninfo.sd;		//取得终端socket
// 	bbzero(gbl.ip);
// 	sprintf(gbl.ip, "%d.%d.%d.%d", coninfo.s_b1, coninfo.s_b2, coninfo.s_b3, coninfo.s_b4);
// 	
// 	bbzero(main_ver);
// 	memset(&gbl.posInfo, 0, sizeof(gbl.posInfo));
// 	memset(&gbl.dPosInfo, 0, sizeof(gbl.dPosInfo));
// 	
// 	iRet = TS_GetPosInfo(sd, &gbl.posInfo, &gbl.dPosInfo, gbl.askType, menu_id, CARD_TYPE);	//获取终端信息
// 	
//  	strcpy( log.g_pos_no, gbl.dPosInfo.pos_no);	//设置线程的终端号变量(各线程独立，用于日志识别)
// 	log.log_sd = sd;
// 	
// 	wrYct3MonitorLog("===========================");
//  	wrYct3MonitorLog("开启终端监控");
// 	
// 	if (CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data) != 0)
// 	{
// 		return 1;
// 	}
// 
// 	CYCTYPTrade::P_SET_MONITOR(&gbl,gbl.dPosInfo, gbl.com, gbl.comType,&yct_data);
// 	
// 	int iMonFrq = atoi(gbl.monitor_frequency);//监控频率
// 	
// 	while(1)
// 	{
// 		CYCTYPTrade::termMonitor(&gbl);
// 		Sleep(iMonFrq * 60 * 60);
// 	}
// 	
// 	wrYct3MonitorLog("关闭终端监控");
// 	return 0;
// }