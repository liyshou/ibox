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


//CGlobal gbl;	//�߳�ȫ�ֱ���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuHdl::CMenuHdl()
{
	sd = 0;
	memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
	wrLog("��ʱ��ͳ�ơ���ʼ");
}

CMenuHdl::~CMenuHdl()
{
	wrLog("��ʱ��ͳ�ơ�����");

//	wrLog("�ر�SOCKET,�����߳��˳�...");
	TS_HangUp(gbl.dPosInfo);

	//closesocket(sd);
	wrLog("��~CMenuHdl��");
	
}

int CMenuHdl::yct3_flag = 0;

// ���أ�0, �޲��������ף�������ʧ��
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
// 	if( i == 0)  //�в���������
// 	{
// 		yct.setStan( yctStan );
// 		bbzero(stat);
// 		ret = yct.notFullHdl(stat);
// 		if( ret == 0 )  //��ֵ�ɹ�
// 		{
// 			CDB::setYctStat(1, seq); //��Ϊδȷ��״̬
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
// 			if( yct.YCTAckTrade(yctCard, stat, hexAmo)==0 ) //ȷ��
// 				CDB::setYctStat(0, seq);			
// 			TS_OutPutMsg(gbl.dPosInfo, "  \n\n     ��ֵ�ɹ���", 1, iSelect);
// 			return 0;
// 		}
// 		else if( ret == 1 ) //��ֵʧ��
// 		{
// 			CDB::setYctStat(3, seq); //��Ϊδ����״̬
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
// 			TS_OutPutMsg(gbl.dPosInfo, "���粻�ȶ������ֵʧ�ܣ�\n���Ժ�����!\n ������׽������ȷ���� ���һ�ʽ��ײ�ѯ", 1, iSelect);
// 			return 0;
// 		}
// 		else  //����������ʧ�ܣ�����������������
// 		{
// 			TS_OutPutMsg(gbl.dPosInfo, "���粻�ȶ����������Ľ��ף�\n���Ժ�����!\n ������׽������ȷ���� ���һ�ʽ��ײ�ѯ", 1, iSelect);
// 			return -1;
// 		}
// 	}
// 	else if( i == 1)
// 		return 0;
// 	else
// 	{
// 		TS_OutPutMsg(gbl.dPosInfo, " \n���粻�ȶ�,���ݿ����\n���Ժ�����!\n ������׽������ȷ���� ���һ�ʽ��ײ�ѯ", 1,iSelect);
// 		return -1;
// 	}
// 	
// }

int CMenuHdl::Menu_main_hdl(Coninfo coninfo)
{
	TSMENULIST menuList;	//�˵��ṹ��
	CTrade	trade;
	int list[30] = {0};		//ÿ���·��˵��ı������
	int	par_menu;			//���˵�
	int cur_menu;			//��ǰ�˵�
	int iRet = 0;
	int iRet2=0;			//	�û��Ƿ��Ѿ�ͬ�⡣
	int iSelect = 0;		//�˵�ѡ��
	int loop = 1;			//ѭ����ʶ
	int enc_logon_lag = 0;
	int menu_id;			//���ز˵����͵Ĳ˵�ID
	int main_group = 1;			//���˵�������
	char main_ver[8+1];		//���˵��İ汾��
	char menu_ver[8+1];     //�˵��汾��
	char usbid[16+1];

	sd = coninfo.sd;		//ȡ���ն�socket
	bbzero(gbl.ip);
	sprintf(gbl.ip, "%d.%d.%d.%d", coninfo.s_b1, coninfo.s_b2, coninfo.s_b3, coninfo.s_b4);

	bbzero(main_ver);
	memset(&gbl.posInfo, 0, sizeof(gbl.posInfo));
	memset(&gbl.dPosInfo, 0, sizeof(gbl.dPosInfo));
	iRet = TS_GetPosInfo(sd, &gbl.posInfo, &gbl.dPosInfo, gbl.askType, menu_id, CARD_TYPE);	//��ȡ�ն���Ϣ
	
	if( iRet )
	{
		CPack pack;
		wrLog("��ȡ�ն���Ϣ����");
		wrMonitorLog(10, 10, iRet, gbl, pack);	//�����־
		TS_OutPutMsg(gbl.dPosInfo, "\n\n   ��ȡ�ն���Ϣ����\n���Ժ�����!", 1,iSelect);

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
	strcpy( log.g_pos_no, gbl.pos_no );	//�����̵߳��ն˺ű���(���̶߳�����������־ʶ��)
	log.log_sd = sd;
				 
	iRet2=CDB::getAgreeFlag(&gbl.agreeFlag,gbl.pos_no);
	
	if(iRet2)
	{
		
		wrLog("session thread");
		return -1;
	}
    
	
	wrLog("-------------------------");
	wrLog("USBID[%s]", usbid);
	wrLog("�绰����[%s]", gbl.call_tel);
	wrLog("����MD5[%s]", thisPrmMD5);
	wrLog("�ն˺�[%s]", gbl.pos_no);
	wrLog("IP[%s]", gbl.ip);
	wrLog("��������[%d]", gbl.askType);
	wrLog("�󴮿��豸[%d]", gbl.posInfo.leftType);
	wrLog("�Ҵ����豸[%d]", gbl.posInfo.rightType);
	wrLog("�����豸[%d]", gbl.posInfo.inType);
	wrLog("����汾[%s]", gbl.posInfo.prmVer);
	wrLog("Ӧ�ð汾[%s]", gbl.posInfo.funVer);
	wrLog("��������[%d]", gbl.posInfo.enyFlag);
    wrLog("�û��Ƿ�ȷ��[gbl.agreeFlag]:%d",gbl.agreeFlag);

	
	//bbzero(gbl.DIY_URL);
	bbzero(gbl.CS_URL);
	//DCN��������
	if(!memcmp(gbl.pos_no+4, "99", 2) || strcmp(gbl.ip,"132.97.45.31")==0)
	{
		//DIY http://132.97.45.31:1301
		//�ͷ���վ http://132.97.45.31:1458
		//strcpy(gbl.DIY_URL, DIY_DCN_URL);
		strcpy(gbl.CS_URL, CS_DCN_URL);
	}
	else
	{
		//�ͷ�http://121.33.197.198:1458
		//diy  http://diy.payeasy.cn
	//	strcpy(gbl.DIY_URL, DIY_GBL_URL);
		strcpy(gbl.CS_URL, CS_GBL_URL);
	}
	
    int encrypt_type=0;
	iRet = CDB::getEncryptType(&encrypt_type,gbl.pos_no);
	wrLog("���ݿ��м�¼�ļ�������[%d]��iRet[%d]", encrypt_type,iRet);
	if(iRet==-1)
	{
		if(gbl.posInfo.enyFlag==0)
          CDB::setEncryptType(gbl.posInfo.enyFlag,0,gbl.pos_no);
	}else if(iRet==0){
		if(gbl.posInfo.enyFlag==0||(gbl.posInfo.enyFlag==1&&encrypt_type==0))
          CDB::setEncryptType(gbl.posInfo.enyFlag,1,gbl.pos_no);
	}


	bbzero(gbl.server_ip); //��ȡ������ip
	CConnect::getIP(gbl.server_ip);
	wrLog("������IP:[%s]",gbl.server_ip);



	if(memcmp(gbl.pos_no, "68", 2)==0 && menu_id==121)
	{
		menu_id = 110;
	}

	if(atoi(gbl.dPosInfo.prmVer)>=9000)
	{
		int prtCom=0, prtType=0;
		if( CPrint::getPrintPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &prtCom, &prtType)!=0 )//���IPOS��û�д�ӡ��
		{
			gbl.posInfo.leftType = 2;//�������״�
		}
	}

	//��ȡ�ն���Ϣ
	bbzero(gbl.curworkid);
	CPack pack;
	iRet = CDB::getEposTerm(gbl.pos_no, gbl.call_tel, &gbl.tgroup, &gbl.group_type, &gbl.phs_ticket, &gbl.get_card, gbl.mer_ecard, &enc_logon_lag, gbl.welcome, gbl.curworkid, &gbl.dPosInfo, usbid, &gbl.dayAmo, gbl.city_no, gbl.acc_city_no, &gbl.title_flag, gbl.pki_card, gbl.signin_serial, &gbl.DianXin_Turn, &gbl.Reversal_count, &gbl.term_level,gbl.update_messag, &gbl.input_times, &gbl.print_paper, &gbl.commission_free,&gbl.HID_TYPE,&gbl.bus_trade_flag);	
	wrLog("��ݸ�廪ͬ���������豸[%d]",gbl.HID_TYPE);

	//ϵͳ��ͣ
	char service_Hour[6+1];
	char service_time[14+1];
	bbzero(service_Hour);
	bbzero(service_time);
	CDB::getDBtime(service_time);
	memcpy(service_Hour,service_time+8,6);
	wrLog("���ݿ�ʱ��Ϊ:%s",service_Hour);


	if(atoi(service_Hour)>=service_stop ||atoi(service_Hour)<=service_start)
	{
		TRADE_STAT=1; //��ͣ����	
	}
	else
	{
	
		TRADE_STAT=0;
	}

	if(TRADE_STAT)  
	{
		char stips[301], sflag[2];
		wrLog("�������л��У��߳��˳�1��");
	
		 if(atoi(service_Hour)>=service_stop ||atoi(service_Hour)<=service_start)
			
		 {
			TS_OutPutMsg(gbl.dPosInfo, "ϵͳ�ս��У����Ժ�����", 1, iSelect);
		 }
		 else
		 {

			 bbzero(stips);
			 get_epos_tips(gbl.pos_no, 0, sflag, stips);
			 if(strlen(stips) > 10) //����С��10����Ϊ����Ч��ʾ��
			 {
				 
				 TS_OutPutMsg(gbl.dPosInfo, stips, 1, iSelect);
			 }
			 
			 else
			 {
				 TS_OutPutMsg(gbl.dPosInfo, "ϵͳά���У����Ժ�����...", 1, iSelect);
				 
			 }

	
		}

		return 0;

	}


	//���ؽ��ڲ˵��������������������
	if( gbl.askType == 9 ){
		gbl.get_card = 1;//���Ϊ���ؽ��ڲ˵�������ˢ����־����Ϊ���ˢ��
		TSTradeInfoSend tradeInfoSend;//���ý��ڲ˵���Ӧ����������
		memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
		memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
		tradeInfoSend.uTotalContents = 3;
		tradeInfoSend.Contents[0].uCommand = TS_INPUT_MONEY;	//��ȡ���
		tradeInfoSend.Contents[1].uCommand = TS_MAG_CARD;		//ˢ��
		tradeInfoSend.Contents[2].uCommand = TS_INPUT_PIN;		//������
		iRet = TS_ReadLocalCont(gbl.dPosInfo, &tradeInfoSend, &tradeInfoRecv);//����Ϣ��FSK��֪ͨFSKȥ�����ն˵ı���
		if( iRet )
		{
			TS_OutPutMsg(gbl.dPosInfo, " \n\n  ��ȡ�û�������Ϣʧ�ܣ�ת�ʲ��ɹ���\n���Ժ�����!\n ������׽������ȷ���� ���һ�ʽ��ײ�ѯ", 1,iSelect);
			if(iRet<0)
				iRet = 7;
			wrMonitorLog(10, 10, iRet, gbl, pack);	//�����־		
			
			return -1;
		}
	}

	if( iRet!=0 )
	{
		wrLog("�Ƿ����ն���Ϣ iRet=[%d]", iRet);
		TS_OutPutMsg(gbl.dPosInfo, " �Ƿ����ն���Ϣ��\n���Ժ�����!\n�򰴡�Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ", 1,iSelect);
		if(iRet!=0)
			iRet = 7;	
		wrMonitorLog(10, 10, iRet, gbl, pack);//�����־
		
		return -1;
	}
	else
		wrMonitorLog(10, 0, 0, gbl, pack);		//�����־

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

	if(atoi(gbl.dPosInfo.prmVer)>=9000 && (atoi(prmVer)>atoi(gbl.dPosInfo.prmVer) || strstr(prmMD5, thisPrmMD5)==NULL && strstr(prmMD5, "0000000000000000")==NULL))//��Ҫ���³���
	{
		if(	strlen(gbl.update_messag) != 0 )
		{//����и��� ����ʾ��������Ҫע�������
			bbzero(tmp);
			strcat(tmp,gbl.update_messag);
			TS_OutPutMsg(gbl.dPosInfo,tmp, 1, iSelect);			
		}
		UpdateFiles();
	}

// 	//�ж��ն��Ƿ������ͨ����������
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

	//��鲢�������ͨδ֪�������
	if(gbl.askType == 10)
	{
		if( CYCTYPTrade::NotFinishChk(gbl, gbl.pos_no, gbl.pki_card, gbl.signin_serial)!=0 )
		{
			return -1;
		}
	}



	/*********��һ��ǩ��********/
	wrLog("logon_flag = %d", logon_flag);
	trade.set_in_par(gbl); //���ݲ���
	if(logon_flag == 0 && !gbl.posInfo.enyFlag)	//���ܸ��죬�ɼ����ն�ǩ��
	{
		if( trade.logonHdl() == 0)  //20070206, ��ǩ���ɹ�����ǩ����־Ϊ1
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
				//closesocket(sd);				//���һ��
				return -2;
			}
			char mac16[16+1];
			bbzero(mac16);
			iRet = TS_GetMacKey(gbl.dPosInfo, mac16);
			if(iRet)
			{
				//TS_HangUp(gbl.dPosInfo);
				//closesocket(sd);			//���һ��
				return -3;
			}else
			{
				memcpy(gbl.dPosInfo.macKey, mac16, sizeof(gbl.dPosInfo.macKey)-1);
			}		
		}
	}	

	if(atoi(gbl.dPosInfo.prmVer)<9000)//�����ն�
	{
		trade.getEcardMsg("e����ֵ��Ϣ"); //��ʾe����ֵ��ʾ
	}

	if(gbl.get_card	== MNG_ONE || gbl.get_card == MNG_ONE_BIND || gbl.get_card == MNG_ONE_BIND_E)	//һ��ˢ���ĵ�һ��ˢ��
	{
		iRet=1;
		trade.set_in_par(gbl); //���ݲ���	
		while(iRet > 0)	//ˢ����������ˢ��
		{
			char card_info[200];
			bbzero(gbl.fee_type);
			memcpy(gbl.fee_type, "-1", 2);
			bbzero(card_info);
			iRet = trade.getCardInfo("������", card_info, MNG_OR_INPUT, gbl.get_card);//ˢ��ȡ�ÿ���Ϣ
			if( iRet == 0 )
				iRet = trade.getCardNum(card_info, MNG_OR_INPUT);//ȡ�ÿ���
			if(iRet == RE_MNG_CARD)
				continue;
			else if(iRet != 0)
				return -1;
			trade.get_gbl( gbl );  //ȡ����CTrade���л�õĹ��ñ���
		}
	}
	
	iRet = CDB::getMenu(gbl.tgroup, gbl.askType, gbl.bank, &gbl.menu_group, &main_group, main_ver, menu_ver);
	if( iRet )
	{
		TS_OutPutMsg(gbl.dPosInfo, " \n\n  �޷��ҵ����ն˲˵���\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ", 1, iSelect);
		return -1;
	}else
	{
		if( gbl.askType>10){
			main_group = gbl.menu_group;
			strcpy(main_ver,menu_ver);
		}
		wrLog("��gbl.menu_group����%d��", gbl.menu_group);
		wrLog("��main_group����%d��", main_group);
		wrLog("��main_ver����%s��", main_ver);
		//��gbl���´���trade
		trade.set_in_par(gbl);
	}

	//�¼��ܵ����°汾

	if( gbl.posInfo.enyFlag==1 && ( memcmp(main_ver, gbl.posInfo.funVer, 3)!=0 || memcmp(main_ver+3, gbl.posInfo.funVer+3, 5)>0 ) && atoi(gbl.dPosInfo.prmVer)!=1001 && atoi(gbl.dPosInfo.prmVer)!=1002 && atoi(gbl.dPosInfo.prmVer)>=200 )
	{
		//���¸��ն˵����˵����
		iRet = trade.UpdateMenus(main_group, main_ver[0]);
		if(iRet==0)
		{
			//���²˵��汾
			TSPosPar tsposPar;
			tsposPar.num=1;
			tsposPar.parCont[0].parId=8;
			tsposPar.parCont[0].parLen=4;
			bbzero(tsposPar.parCont[0].parCont);
			CTools::hex_to_byte((const unsigned char *)main_ver, (unsigned char *)tsposPar.parCont[0].parCont, 8);
			iRet = TS_UpdatePosPar(gbl.dPosInfo, &tsposPar);
			if(iRet)
			{
				wrLog("���汾�Ÿ���Ϊ[%s]ʧ��", main_ver);
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
		if(gbl.group_type==0 && atoi(gbl.dPosInfo.prmVer)<9000)//������̻��Ҳ������ն�
		{
			for(int i=0; i<MAXTRY; i++)
			{
				wrLog("������һ��ز˵�����%d��", menu_id);
				oper_hdl(menu_id);			
				
				TSMENULIST menuList;
				memset(&menuList,0,sizeof(menuList));
				menuList.iTotalNum = 3;
				strcpy(menuList.acTitle, "��ѡ��");
				strcpy(menuList.menuItem[0], epos_menu[menu_id].name);
				strcpy(menuList.menuItem[1], "���˵�");
				strcpy(menuList.menuItem[2], "��    ��");
				//strcpy(menuList.menuItem[2],"�˳�");
				
				iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
				if(iRet)
				{
					wrLog("�·��һ�����");	//�·��һ�����
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
					wrLog("�·��һ�����");	//�·��һ�����
					return CANCLE_KEY;
				}
			}		
		}
		else
		{
			wrLog("������һ��ز˵�����%d��", menu_id);
			oper_hdl(menu_id);
 			wrLog("�·��һ�����");	//�·��һ�����
			
			//wrLog("�����ն˼���̣߳���δ������");

			//TS_HangUp(gbl.dPosInfo);
 			//closesocket(sd);				//���һ��

// 			wrLog("�ն˼�ؿ�������");
// 			HANDLE termMonitor = CreateThread(NULL , 1000 , YCT3Monitor, (LPVOID)&gbl , 0 , NULL);
// 			CloseHandle(termMonitor);

 			return CANCLE_KEY;
		}		
	}

	//ȡ�˵�
	par_menu = 0;  //��ʼΪ���˵�
	cur_menu = 0; 
	iRet = get_menu_list(par_menu, &menuList, list);
	if( iRet )
	{	
		TS_OutPutMsg(gbl.dPosInfo, "\n\nû���ҵ����ն˲˵���\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ", 1, iSelect);
		return -1;
	}


	if(list[2] < 0)	//ֻ��һ��ҵ��Ļ�ֱ�ӽ����ҵ��
	{
		wrLog("������һ��˵�����%d��", list[1]);
		oper_hdl(list[1]);			
		//TS_HangUp(gbl.dPosInfo);
		//closesocket(sd);						//���һ��
		return CANCLE_KEY;
	}

	wrLog("������һ��˵�����%d��", gbl.menu_group);
	wrLog("���·��˵���");
	iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);	//�·��˵�
	/*-------------------------------------*/
	loop = 1;
	while(loop) 
	{
		if(TRADE_STAT)  //ϵͳ��ͣ
		{
			char stips[301], sflag[2];
			wrLog("�������л��У��߳��˳�2��");
				
			
			if(atoi(service_Hour)>=235500 ||atoi(service_Hour)<=500)
				
			{
				TS_OutPutMsg(gbl.dPosInfo, "ϵͳ�ս��У����Ժ�����", 1, iSelect);
			}
			else
			{
				bbzero(stips);
				get_epos_tips(gbl.pos_no, 0, sflag, stips);
				if(strlen(stips) > 10) //��������10����Ϊ����Ч��ʾ��
				{
					TS_OutPutMsg(gbl.dPosInfo, stips, 1, iSelect);
				}
				else
				{
					TS_OutPutMsg(gbl.dPosInfo, "ϵͳά���У����Ժ�����...", 1, iSelect);
				}
			
			}
		
			break;
		}
		switch(iRet) 	//ͨ��iRet���жϲ���
		{
		case SUCC:
			iRet = MenuHdl(list, par_menu, cur_menu, iSelect, menuList);
			wrLog("��SUCC��iRet=%d",iRet);
			break;
			
		case HOTKEY:	//��ݼ�����
			iRet = Menu_HOTKEY_hdl(iSelect, loop, menuList, par_menu, cur_menu, list);
			wrLog("��HOTKEY��iRet=%d",iRet);
			break;
			
		default:
			wrLog("�·��һ�����");	//�·��һ�����
			loop = 0;
			break;
		}
	}
	return 0;
}


int CMenuHdl::Menu_HOTKEY_hdl(int &iSelect, int &loop,TSMENULIST &menuList, int &par_menu, int &cur_menu, int *chglist)
{
	int iRet = 0, i;
	wrLog("����ݼ���������");
	wrLog("iSelect:%d",iSelect);
	switch(iSelect)
	{
	case CANCLE_KEY:
		//"ȡ��"
		if(cur_menu == 0)  //���˵�
		{
			//�·��һ�����
			wrLog("�·��һ�����1");
			loop = 0;
		}
		else
		{
			cur_menu = par_menu;
			if( CDB::isLock() ) //ˢ�����ݿ��У��ȴ�6���ˢ�²��ɹ����˳�
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
			if(i == menu_cnt[gbl.menu_group]) //���ҳ���
			{
				loop = 0;
				return -1;
			}
			get_menu_list(cur_menu, &menuList,chglist);
			iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		}
		break;
	default:
		//�·��һ�����
		wrLog("�·��һ�����2");
		loop = 0;
		break;
	}
	return iRet;
}

//���ܣ���ȡcur_menu���Ӳ˵�
//cur_menu����ǰ�˵���
//list���·��Ĳ˵���ı��˳��
int CMenuHdl::get_menu_list(int cur_menu, PTSMENULIST pMenuList, int *chglist)	
{
	int i;
	int	k = 0;
	if( CDB::isLock() ) //ˢ�����ݿ��У��ȴ�6���ˢ�²��ɹ�
		return -1;
	for(i=0; i<menu_cnt[gbl.menu_group]; i++)
	{
		//ȡ���¼��˵�
		if(menu_conf[gbl.menu_group][i].currentm == cur_menu) 
		{
			bbzero(pMenuList->menuItem[k]);
			strcpy(pMenuList->menuItem[k], epos_menu[ menu_conf[gbl.menu_group][i].child ].name);
			chglist[k+1] = menu_conf[gbl.menu_group][i].child; //��1����
			k++;
		}
	}
	if( k == 0) //���¼��˵�
		return 1;
	if(cur_menu == 0)
	{
		bbzero(pMenuList->menuItem[k]);
		strcpy(pMenuList->menuItem[k], "��   ��");
		chglist[k+1] = -1;
		k++;
	}
	else
	{
		bbzero(pMenuList->menuItem[k]);
		strcpy(pMenuList->menuItem[k], "��    ��");
		chglist[k+1] = -2;
		k++;
	}
	pMenuList->iTotalNum = k;
	if(cur_menu == 0 && gbl.tgroup == 5)  //20070831��ʡ��˾��ʾ
	{
		strcpy(pMenuList->acTitle, "�й����ţ�����֧��");
	}
	else if(cur_menu == 0 && gbl.tgroup == 9)  //20071119������ҵ��
	{
		strcpy(pMenuList->acTitle, "����ͨ");
	}
	else
	{
		if(strcmp(gbl.city_no,"0086")==0)

		{
		strcpy(pMenuList->acTitle,"��֧��");
		strcat(pMenuList->acTitle,epos_menu[cur_menu].name);

		}

		else

		{

		strcpy(pMenuList->acTitle, "������--");
		strcat(pMenuList->acTitle, epos_menu[cur_menu].name);

		}
	}
	return 0;
}


//���ܣ��˵�����
//list����ŵ�ǰ�·��˵�������˳��
//par_menu����ǰ�˵��ĸ��˵����
//cur_menu����ǰ�˵��ı��
//iSelect���û�ѡ���iSelect���˵���
//menuList���·��Ĳ˵��ṹ��
int CMenuHdl::MenuHdl(int *chglist, int &par_menu, int &cur_menu,int &iSelect, TSMENULIST &menuList)
{
	int iRet, cnt=1, i;

	gbl.init_global();	//��ʼ��ȫ����
	while(chglist[cnt++]);	//��ȡ�˵�����
	if(iSelect > 0 && iSelect < cnt) //ѡ������һ���˵�
	{
		par_menu = cur_menu;  //���˵���Ϊ��ǰ�˵�
		cur_menu = chglist[iSelect]; //��ǰ�˵���Ϊ�û�ѡ��Ĳ˵���
		iRet = get_menu_list(cur_menu, &menuList, chglist);

		if( !iRet ) //���¼��˵�
		{
			iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		}
		else if( iRet == -1 ) //�˳�
		{
			return HANG_UP_KEY;
		}
		else  //�û�ѡ��Ĳ˵������Ӳ˵�������ҵ����
		{
			if(cur_menu == -1)	//�˳�
			{
				return HANG_UP_KEY;
			}else if(cur_menu == -2)  //����
			{
				if( CDB::isLock() )
					return HANG_UP_KEY;
				cur_menu = par_menu;	//ȡ��ԭ�˵�����
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
				oper_hdl(chglist[iSelect]);  //ҵ����
			}			
			cur_menu = par_menu;	//������ϣ�ȡ��ԭ�˵�����
			if( CDB::isLock() )
				return HANG_UP_KEY;
			for(i=0; i<menu_cnt[gbl.menu_group]; i++) //ȡ��ԭ�˵��ĸ��˵����
			{
				if(menu_conf[gbl.menu_group][i].currentm == cur_menu)
				{
					par_menu = menu_conf[gbl.menu_group][i].parent;
					break;
				}
			}
			if( i == menu_cnt[gbl.menu_group] ) //���Ҵ���
				return HANG_UP_KEY;
			get_menu_list(cur_menu, &menuList, chglist); //��ȡԭ�˵�
			iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);	//�·�ԭ�˵�
		}
	}
	else //�û�ȡ��
		iRet = TS_Menu(gbl.dPosInfo, &menuList, iSelect, MENU_TIME_OUT);	//�·�ԭ�˵�
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


//ҵ����
//id��ҵ����
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
		strcpy(type, "���ֵ��ӽɷ�");
	else if( atoi(gbl.fee_type) == 112)
		strcpy(type, "�齭��Ƶ�ɷ�");
	else if( atoi(gbl.fee_type) == 114)
		strcpy(type, "ģ�����(�޻�����)");
	else if( atoi(gbl.fee_type) == 172)
		strcpy(type, "�齭��Ƶ�Ż�");

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
		wrLog("�й��ƶ�");
		strcpy(gbl.fee_type, "3012");
		trade.set_in_par(gbl);
		trade.mblTrade();
		break;

	case 10:  //���ó���ǿ��
	
		trade.set_in_par(gbl);
		trade.piccTrade();
		break;
	case 34:  //���ߵ��ӽɷ�
		strcpy(gbl.fee_type,"0114");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 35:  //�齭����ɷ�
		strcpy(gbl.fee_type,"0100");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 36:  //�齭��Ƶ�ɷ�
		strcpy(gbl.fee_type,"0112");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 167: //�齭��Ƶ�Ż�
		strcpy(gbl.fee_type,"0172");
		trade.set_in_par(gbl);
		trade.CATVTrade(type);
		break;
	case 41:

		strcpy(gbl.fee_type,"0076");
		trade.set_in_par(gbl);
		trade.QQTrade("Q�ҳ�ֵ");

		/*
				
		if (atoi(gbl.dPosInfo.prmVer) >9000) //�����ipos �л����µĹ�»�ӿ�
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015"); //glQQ
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027"); //glQQ

			trade.GLQQTrade("Q�ҳ�ֵ");  //�Ѿ�����Ϊ��»qq����Ѷqq��ͣ


		}
		else
		{
		
			trade.QQTrade("Q�ҳ�ֵ");
		}

		*/

		break;
	case 42:
		strcpy(gbl.fee_type,"0077");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ��Ա����");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ��Ա����");
		}
		else
		{
			trade.QQTrade("QQ��Ա����");

		}

		*/	
		
		break;
	/*---------------------------------*/
	case 70:
		wrLog("���ɻ����ʺų�ֵ"); 
		strcpy(gbl.fee_type,"0143");
		trade.set_in_par(gbl);
		trade.telTrade("���ɻ����ʺų�ֵ");
		break;
	case 71:
		wrLog("�̻�/С��ͨ����");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.telTrade("�̻�/С��ͨ����");
		break;
	case 72:
		wrLog("Ԥ���ѹ̻���ֵ");
		strcpy(gbl.fee_type,"0098");
		trade.set_in_par(gbl);
		trade.telTrade("Ԥ���ѹ̻���ֵ");
		break;
	case 73:
		wrLog("С��ͨ����");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.telTrade("С��ͨ����");
		break;
	case 74:
		wrLog("Ԥ����С��ͨ��ֵ");
		strcpy(gbl.fee_type,"0064");
		trade.set_in_par(gbl);
		trade.telTrade("Ԥ����С��ͨ��ֵ");
		break;
	case 75:
		wrLog("ADSL����");
		strcpy(gbl.fee_type,"0013");
		trade.set_in_par(gbl);
		trade.telTrade("ADSL����");
		break;
	case 76: 
		wrLog("ADSLԤ�ɻ�������");	//��ҵ����롰71��
		strcpy(gbl.fee_type,"0071");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"71");
		trade.set_in_par(gbl);
		trade.telTrade("ADSLԤ�ɻ�������");
		break;
	case 77:
		wrLog("Ԥ�ɷѿ���ֵ��������");
		strcpy(gbl.fee_type,"0046");
		trade.set_in_par(gbl);
		trade.telTrade("Ԥ�ɷѿ���ֵ��������");
		break;
	case 78:
		wrLog("ITVǷ��");   //���Ų˵����
		strcpy(gbl.fee_type,"0137");
		trade.set_in_par(gbl);
		trade.telTrade("ITVǷ��");
		break;
	case 79:
		wrLog("Ԥ����ADSL��ֵ");
		strcpy(gbl.fee_type,"0140");
		trade.set_in_par(gbl);
		trade.telTrade("Ԥ����ADSL��ֵ");
		break;
	/*---------------------------------*/
	case 80: 
		//130/131�󸶷�
		
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"01");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"0");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	case 81: 
		//130/131Ԥ����
	
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"01");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"1");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	case 84: 
		//193/17911�󸶷�
	
		strcpy(gbl.fee_type,"0049");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"03");
		memset(gbl.uni_flag,0,sizeof(gbl.uni_flag));
		strcpy(gbl.uni_flag,"0");
		trade.set_in_par(gbl);
		trade.uniTrade();
		break;
	case 85: 
		//193/17911Ԥ����
		
	
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
// 		wrLog("С��ͨ�����׿���>Ԥ����ADSL��ֵ");
// 		strcpy(gbl.fee_type,"0140");
// 		trade.set_in_par(gbl);
// 		ret = trade.xltTrade("Ԥ����ADSL��ֵ");
// 		if( !ret )
// 			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
// 		break;
		wrLog("������ѯ�ĵ���ֱ�䡪��Ԥ����ADSL��ֵ��");
		trade.set_in_par(gbl);
		trade.newAirPay("Ԥ����ADSL��ֵ",2);
		break;
	case 87: 
		wrLog("С��ͨ�����׿���>ITVǷ��");
		strcpy(gbl.fee_type,"0137");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("ITVǷ��");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 88: 
		wrLog("ITVԤ��");
		strcpy(gbl.fee_type,"0138");
		trade.set_in_par(gbl);
		ret = trade.telTrade("ITVԤ��");
		break;
	case 89: 
		wrLog("С��ͨ�����׿���>���ɻ����ʺų�ֵ");
		strcpy(gbl.fee_type,"0143");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("���ɻ����ʺų�ֵ");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 90: 
		wrLog("С��ͨ�����׿���>�̻�/С��ͨ����");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("�̻�/С��ͨ����");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 91: 
		wrLog("�̻�/С��ͨ����Ԥ��");
		strcpy(gbl.fee_type,"0096");
		trade.set_in_par(gbl);
		ret = trade.telTrade("�̻�/С��ͨ����Ԥ��");
		break;
	case 92: 
		wrLog("С��ͨ�����׿���>Ԥ���ѹ̻���ֵ");
		strcpy(gbl.fee_type,"0098");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("Ԥ���ѹ̻���ֵ");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 93:
		wrLog("С��ͨ�����׿���>С��ͨ����");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("С��ͨ����");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 94: 
		wrLog("С��ͨ�����׿���>Ԥ����С��ͨ��ֵ");
		strcpy(gbl.fee_type,"0064");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("Ԥ����С��ͨ��ֵ");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 95: 
		wrLog("С��ͨ����Ԥ����");
		strcpy(gbl.fee_type,"0096");
		trade.set_in_par(gbl);
		ret = trade.telTrade("С��ͨ����Ԥ����");
		break;
	case 96: 
		wrLog("С��ͨ�����׿���>ADSL����");
		strcpy(gbl.fee_type,"0013");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("ADSL����");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 97: 
		wrLog("ADSL����Ԥ��");
		strcpy(gbl.fee_type,"0097");
		trade.set_in_par(gbl);
		ret = trade.telTrade("ADSL����Ԥ��");
		break;
	case 98: 
		wrLog("С��ͨ�����׿���>ADSLԤ�ɻ�������");
		strcpy(gbl.fee_type,"0071");
		memset(gbl.sub_code,0,sizeof(gbl.sub_code));
		strcpy(gbl.sub_code,"71");
		trade.set_in_par(gbl);
		ret = trade.xltTrade("ADSLԤ�ɻ�������");
		if( !ret )
			trade.get_xlt_info(gbl.card_no, gbl.track2_len, gbl.track2);
		break;
	case 99:
		wrLog("С��ͨ�����׿���>��ѯ���");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0094", 4);
		trade.set_in_par(gbl);
		trade.cardInqHdl();
		break;
	case 100: 
		wrLog("С��ͨ�����׿���>�޸�����");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0095", 4);
		trade.set_in_par(gbl);
		trade.cardChgPwd();
		break;
	/*---------------------------------*/
	case 101:

		wrLog("���ݳ���˰");
		strcpy(gbl.fee_type,"0065");
		trade.set_in_par(gbl);
		trade.taxTrade("���ݳ���˰");
		break;
	/*---------------------------------*/
	case 110:
		//IPOS��½��ʾ��ʾ��
		if(	strlen(gbl.welcome) != 0 )
		{

			bbzero(temp_buff);
			strcat(temp_buff,gbl.welcome);
			TS_OutPutMsg(gbl.dPosInfo,temp_buff, 1, iSelect);
		}

		wrLog("e����>��ѯ���");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		char amount[12+1];
		trade.cardInqHdl(amount);
// 		if(atoi(gbl.dPosInfo.prmVer)>=9000 && (atoi(prmVer)>atoi(gbl.dPosInfo.prmVer) || strstr(prmMD5, thisPrmMD5)==NULL && strstr(prmMD5, "0000000000000000")==NULL))//��Ҫ���³���
// 		{
// 			UpdateFiles();
// 		}
		break;
	case 111:
	case 535:
		wrLog("e����>�޸�����");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0084", 4);
		trade.set_in_par(gbl);
		trade.cardChgPwd();				
		break;	
	/*---------------------------------*/
	case 112:
		wrLog("����ͨѶ�ʷѷ�Ʊ");
		strcpy(gbl.fee_type,"0012");
		trade.set_in_par(gbl);
		trade.rePrintHdl(1);
		break;
	case 113:
		wrLog("���򳵴�ʹ��˰��Ʊ");
		strcpy(gbl.fee_type,"3065");
		trade.set_in_par(gbl);
		trade.rePrintHdl(2);
		break;
	/*---------------------------------*/
	case 114: 
		wrLog("С��ͨ��ֵ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "010");
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��");
		break;
	case 115: 
		wrLog("С��ͨ�����׿�");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "011");
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ�����׿�");
		break;
	case 116:
		wrLog("�ƶ���ֵ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��");
		break;
	case 117:
		wrLog("��ͨһ����");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨһ����");
		break;	
	case 120:
		wrLog("����ϴ��");
		trade.set_in_par(gbl);
		trade.merTrade();
		break;
	case 121:
	case 495:
	case 534:
		wrLog("�ڱ궨λ");
		//prt.set_in_par(gbl, pack);
		prt.set_in_par(gbl);
		prt.print_heibiao();
		break;
// 	case 131:
// 		{
// 			CYCTTrade yct;
// 			yct.setInPar(gbl);
// 			yct.YCTPayTrade();  //��ѯ����
// 		}
// 		break;
	case 132:
		wrLog("���ص����ۺ��շ�");		
		trade.set_in_par(gbl);
		trade.foshanTrade(epos_menu[id].name, "0");
		break;
	case 145:
		wrLog("�̻��ս��ײ�ѯ");
		trade.set_in_par(gbl);
		//trade.merInqTrade(1);
        trade.drcx_online(0);
		break;
	case 146:
		wrLog("�̻����½��ײ�ѯ");
		trade.set_in_par(gbl);
		//trade.merInqTrade(2);
		trade.last_mnth_online(0);
		break;
	case 147:
		//�����׿�2������ѯ
		trade.set_in_par(gbl);
		trade.xltInqTrade("����ѯ");
		break;
	case 148:
		//�����׿�2�ڳ�ֵ
		trade.set_in_par(gbl);
		trade.xltMnyIn("�����׿���ֵ");
		break;
	case 149:
		//�����׿�2��ת��
		trade.set_in_par(gbl);
		trade.xltAccTrans("�����׿�ת��");
		break;
	case 150:
		//�ƶ����г�ֵ
		trade.set_in_par(gbl);
		trade.mblAirPay("�ƶ�����ֱ��", 2);
		break;
	case 151:
		//e����ֵ
		trade.set_in_par(gbl);
		trade.ecardMnyIn("�������˻���ֵ");
		TS_HangUp(gbl.dPosInfo);
		closesocket(sd);
		break;
	case 152:
		//e����ֵ�Ǽ�
		trade.set_in_par(gbl);
		trade.ecardAppli("�˻���ֵ����");
		break;
	case 153:
		//���µ�Ӱ�ۿ�Ʊ
		trade.set_in_par(gbl);
		trade.filmPay("���µ�Ӱ�ۿ�Ʊ");
		break;
	case 154:
		//���п�����ѯ
		trade.set_in_par(gbl);
		trade.getBankMny("���п�����ѯ");
		break;
	case 155:
		//���һ�ʽ��ײ�ѯ
		trade.set_in_par(gbl);
		iRet=trade.merInqTrade(3);
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e����>���һ�ʽ��ײ�ѯ���ѯ���");
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
		wrLog("ǩ��");
		/**/
		if( gbl.askType != 4)
			TS_OutPutMsg(gbl.dPosInfo, "��ʾ��Ϊ�������ġ�ǩ�������������������ֱ�Ӱ��������Ϸ��ġ����ء���ݼ���ɣ���ѯ��400-80-11888", 1, ret);
		
		trade.set_in_par(gbl);
		trade.banInqTrade(1);
		break;
	case 157:
		//ת��
		trade.set_in_par(gbl);
		trade.banTransfer(epos_menu[id].name);
		break;
	case 158:
		wrLog("����ѯ");
		strcpy(gbl.fee_type, "0185");
		trade.set_in_par(gbl);	
		trade.banInqMoneyTrade(epos_menu[id].name);
		break;
	case 159:
		wrLog("�տ��˻�����");
		trade.set_in_par(gbl);
		trade.banInqTrade(2);
		break;
	case 160:
		wrLog("���һ�ʽ���");
		trade.set_in_par(gbl);
		trade.banInqLast();
		break;
	case 161:
		wrLog("���ս�����ϸ");
		trade.set_in_par(gbl);
		//trade.banInqHistory(1);
		trade.drcx_online(0);
		break;
	case 162:
		wrLog("��ʷ������ϸ");
		trade.set_in_par(gbl);
		trade.banInqHistory(2);
		break;
	case 163:
		wrLog("��ӡ����");
		prt.set_in_par(gbl);
		prt.print_heibiao(1);
		break;
	case 168:
		wrLog("ȫʡ�����ۺ��շ�");
		trade.set_in_par(gbl);
		trade.foshanTrade(epos_menu[id].name, "0");
		break;
	/*--------------------------*/
	case 199:  //������ҳ��Ϣ
		TS_UpdateHeadPage(gbl.dPosInfo, "��ӭʹ�ý�ͨ���н���֧ͨ��ϵͳ");
//		trade.set_in_par(gbl);
//		trade.updateConInfo();
		break;
	case 200:
		//���Ͷ���
		trade.set_in_par(gbl);
		trade.sendMsg();
		break;
	case 205:
		wrLog("�̻�+ADSL����");
		strcpy(gbl.fee_type,"0047");
		trade.set_in_par(gbl);
		trade.telTrade("�̻�+ADSL����");
		break;
	case 206:
		wrLog("�ش�ӡ��Ʊ");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_fail("�ش�ӡ��Ʊ");
		break;
	case 207:
		wrLog("����ת�ʱ��ػ�");
		trade.set_in_par(gbl);
		trade.banTransLocal("����ת�ʱ��ػ�", &tradeInfoRecv);
		break;
	case 208:
		wrLog("ħ������300��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0801");
		trade.set_in_par(gbl);
		trade.sellCardTrade("ħ������300��");
		break;
	case 209:
		wrLog("ħ������600��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0801");
		trade.set_in_par(gbl);
		trade.sellCardTrade("ħ������600��");
		break;
	case 210:
		wrLog("�Ѻ�15Ԫ");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0802");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�Ѻ�����һ��ͨ300��");
		break;
	case 211:
		wrLog("�Ѻ�30Ԫ");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0803");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�Ѻ�����һ��ͨ600��");
		break;
	case 213:
		wrLog("����͸�����ܼ��");
		trade.set_in_par(gbl);
		trade.testIcReaderCommand("����͸�����ܼ��");
		break;
	case 215:
		wrLog("�����ش�ӡ��Ʊ");
		strcpy(gbl.fee_type,"0218");
		trade.set_in_par(gbl);
		trade.rePrintTel("���Ų���Ʊ");
		break;
	case 216:
		wrLog("��e�������ʳ�ֵ��");
		trade.set_in_par(gbl);
		trade.getEcardMsg("e�������ʳ�ֵ", 1);
		break;
	case 217:
		wrLog("�����Ĳ��ԡ�");
		trade.set_in_par(gbl);
		trade.testFSKPack("���Ĳ���");
		break;
// 	case 218:
// 		wrLog("����ͨ���");
// 		strcpy(gbl.fee_type,"0099");
// 		trade.set_in_par(gbl);
// 		trade.trafficTrade("��ͨ����");
// 		break;
// 	case 220:
// 		{
// 			CYCTTrade yct;
// 			yct.setInPar(gbl);
// 			yct.YCTPayTrade(0);//����
// 		}
// 		break;		
    case 221:
		wrLog("����ͨ�����ƾ֤��");
		strcpy(gbl.fee_type,"0012");
		trade.set_in_par(gbl);
		trade.rePrintTra("��ͨ�����ƾ֤");
		break;
	case 223:
		wrLog("���ֹ�����PIN��");
		trade.set_in_par(gbl);
		trade.SetPIN(1);
		break;
	case 224:
		wrLog("�����ն˺��Զ�����PIN��");
		trade.set_in_par(gbl);
		trade.SetPIN(2);
		break;
	case 228:
		wrLog("CDMA�󸶷�");
		strcpy(gbl.fee_type,"0241");
		trade.set_in_par(gbl);
		trade.telTrade("CDMA�󸶷�");
		break;
	case 229:
		wrLog("CDMAԤ����");
		strcpy(gbl.fee_type,"0242");
		trade.set_in_par(gbl);
		trade.telTrade("CDMAԤ����");
		break;
	//case 229: //20130105 
		//���ݵ��ſ��г�ֵ
		//trade.set_in_par(gbl);
	//	trade.mblAirPay("���Ż���ֱ��", 1);
	//	break;
	case 230:
		//����ת��
		wrLog("������ת��(ת��)��");
		trade.set_in_par(gbl);
		trade.UnionTransfer_in(epos_menu[id].name);
		break;
	case 231:
		//����ת��
		wrLog("������ת��(ת��)��");
		trade.set_in_par(gbl);
		trade.UnionTransfer_out(epos_menu[id].name);
		break;
	case 232:
		wrLog("�������û��Ǽǡ�");
		trade.set_in_par(gbl);
		trade.ysqCatv();
		break;
	case 233:
		//�������п�����ѯ
		wrLog("�����п�����ѯ��");
		trade.set_in_par(gbl);
		trade.UnionbanInqMoney(epos_menu[id].name);
		break;
	case 234:
		//���һ�ν��ײ�ѯ
		wrLog("����һ�ν��ײ�ѯ��");
		trade.set_in_par(gbl);
		trade.UnionbanInqLast();
		break;
	case 235:
		wrLog("�����ս��ײ�ѯ��");
		trade.set_in_par(gbl);
		//trade.UnionbanInqHistory(1);
		trade.drcx_online(0);
		break;
	case 236:
		wrLog("����ʷ���ײ�ѯ��");
		trade.set_in_par(gbl);
		trade.UnionbanInqHistory(2);
		break;
	case 237:
		wrLog("����ͨ����ɡ�");

		bbzero(cTime);			//��ͨ����ÿ��21:00����00:00����ͣ����20081128
		tool.get_time(cTime);
		if(atoi(cTime) >= 210000 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n   ����ϵͳ�ս���\n��24:00���ٽ��н���\n   ����ȷ�ϡ�����",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0099");
		trade.set_in_par(gbl);
		trade.trafficTrade("��ͨ�����");
		break;
	case 238:
		wrLog("��С�ͳ�Υ�²�ѯ��");
		trade.set_in_par(gbl);
		trade.carnum_trafficTrade("С�ͳ�Υ�²�ѯ");
		break;
	case 240:
		wrLog("����ѯ");
		strcpy(gbl.fee_type, "0238");
//		strcpy(gbl.fee_type, "0333");
		trade.set_in_par(gbl);
		trade.banInqMoneyTrade(epos_menu[id].name);
		break;
	case 266:
		wrLog("������ǩ��");
		trade.set_in_par(gbl);
		bbzero(workid);
		iRet = trade.signWorkID(workid);
		if (iRet <0)
		{
			bbzero(temp_buff);
			sprintf(temp_buff,"\n\n����ǩ������!\n���Ժ�����!\n  �밴��ȷ��������");
			TS_OutPutMsg(gbl.dPosInfo, temp_buff, 1,iSelect);
		}else{
			bbzero(gbl.curworkid);
			strcpy(gbl.curworkid,workid);
			bbzero(temp_buff);
			sprintf(temp_buff,"\n\n    ����ǩ���ɹ�!\n���Ժ�����!\n  �밴��ȷ��������");
			TS_OutPutMsg(gbl.dPosInfo, temp_buff, 1,iSelect);
		}
		break;
	case 271:
		wrLog("�̻�����(���л���)");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.BankHK("�̻�����");
		break;
	case 272:
		wrLog("ADSL����(���л���)");
		strcpy(gbl.fee_type,"0013");
		trade.set_in_par(gbl);
		trade.BankHK("ADSL����");
		break;		
	case 273:		
		wrLog("С��ͨ����(���л���)");
		strcpy(gbl.fee_type,"0001");
		trade.set_in_par(gbl);
		trade.BankHK("С��ͨ����");
		break;
	case 278:
		wrLog("Ԥ���Ż�");
		strcpy(gbl.fee_type,"0064");
		trade.set_in_par(gbl);
		trade.YH_xlt("Ԥ���Ż�");
		break;
//////////////////*���Թ��ܺ���*/////////////////////////
	case 282:
		wrLog("�������������롿");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(8);
		break;
	case 283:
		wrLog("���������¡�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(7);
		break;
	case 284:
		wrLog("���������ڡ�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(6);
		break;
	case 285:
		wrLog("����������ȫ�ַ���");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(5);
		break;
	case 286:
		wrLog("�������������֡�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(4);
		break;
	case 287:
		wrLog("�����������");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(3);
		break;
	case 288:
		wrLog("������ˢ�����ܡ�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(2);
		break;
	case 289:
		wrLog("���������俨�š�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(1);
		break;
	case 290:
		wrLog("����ʾ�ն˵绰���롿");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(1);
		break;
	case 291:
		wrLog("����ʾ�ն˺š�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(2);
		break;
	case 292:
		wrLog("����ʾ�ն��󴮿��豸��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(3);
		break;
	case 293:
		wrLog("����ʾ�ն��Ҵ����豸��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(4);
		break;
	case 294:
		wrLog("����ʾ�ն������豸��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_GetPosInfo(5);
		break;
	case 295:
		wrLog("���·���ʾ��Ϣ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_OutPutMsg(1);
		break;
	case 296:
		wrLog("���·���ȫ��ʾ��Ϣ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_OutPutMsg(2);
		break;
	case 297:
		wrLog("���·�������ʾ��Ϣ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_OutPutMsg(3);
		break;
	case 298:
		wrLog("����ӡ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_B4Print(1);
		break;
	case 299:
		wrLog("����ȫ��ӡ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_B4Print(2);
		break;
	case 300:
		wrLog("��֪ͨ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(1);
		break;
	case 301:
		wrLog("�����׽����");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(2);
		break;
	case 302:
		wrLog("���˵����׽����");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(3);
		break;
	case 303:
		wrLog("���˵���");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(4);
		break;
	case 304:
		wrLog("���汾�š�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(5);
		break;
	case 305:
		wrLog("���ն����кš�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_SendMsg(6);
		break;
	case 306:
		wrLog("��������ʾ��Ϣ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(1);
		break;
	case 307:
		wrLog("�������ն˲�����");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(2);
		break;
	case 308:
		wrLog("��������ҳ��Ϣ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(3);
		break;
	case 309:
		wrLog("�����±��ز˵���");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(4);
		break;
	case 310:
		wrLog("������PSAM����Կ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(5);
		break;
	case 311:
// 		wrLog("��PSAM���š�");
// 		trade.set_in_par(gbl);
// 		iRet = trade.Test_TS_PSAM(1);
		break;
	case 312:
// 		wrLog("��PSAM���������");
// 		trade.set_in_par(gbl);
// 		iRet = trade.Test_TS_PSAM(2);
		break;
	case 313:
		wrLog("������������ĸ��");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(9);
		break;
	case 314:
		wrLog("���������뺺�֡�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(10);
		break;
	case 323:
		wrLog("��ˢ�������俨�š�");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_TradeControl(11);
		break;

	case 324:
		wrLog("��С��ͨѡ�š�");
		trade.set_in_par(gbl);
		iRet = trade.selectxlt();
		break;
	case 325:
		wrLog("�����л�������");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(1);
		break;
	case 327:
		wrLog("�����й̻���װADSL��");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(2);
		break;
	case 328:
		wrLog("������e8�ײ͡�");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(3);
		break;
	case 329:
		wrLog("��e8���������ײ͡�");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(4);
		break;
	case 331:
		wrLog("������e6�ײ͡�");
		trade.set_in_par(gbl);
		iRet = trade.sx_ysl(5);
		break;
	case 333:
		wrLog("����ͨ�߲�������");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "001", "3626");
		break;
	case 334:
		wrLog("�������߲�������");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "002", "3626");
		break;
	case 335:
		wrLog("����ͨ�������䡿");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "001", "284");
		break;
	case 336:
		wrLog("�������������䡿");
		trade.set_in_par(gbl);
		iRet = trade.telIBSS(epos_menu[id].name, "002", "284");
		break;
	case 339:
		wrLog("�������������䡿");
		trade.set_in_par(gbl);
		iRet = trade.selectcdma();
		break;

	case 340:
		wrLog("�����±��ؽ��ڲ˵���");
		trade.set_in_par(gbl);
		iRet = trade.Test_TS_UpdateMsg(6);
		break;
	case 341:
		wrLog("��������ǩԼ��");
		trade.set_in_par(gbl);
		trade.ysqCatv_new();
		break;
	case 342:
		wrLog("���̻���װADSL��");
		trade.set_in_par(gbl);
		trade.add_ADSL("�̻���װADSL");
		break;
	case 343:
// 		wrLog("���ǿ����˵㿨���ۡ�");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("�ǿ����˵㿨����");
		wrLog("����ҵ������ͣ��");
		break;
	case 345:
		wrLog("�������ɹ����");
		trade.set_in_par(gbl);
		trade.sell_pos(1);
		break;
	case 346:
		wrLog("����Ʊ��ƾ��������");
		trade.set_in_par(gbl);
		trade.sell_pos(2);
		break;
	case 347:
		wrLog("�����п�ת�ˡ�");
		trade.set_in_par(gbl);
		trade.Bankc_zz("���п�ת��");
		break;
	case 348:
		wrLog("��288�����");
		trade.set_in_par(gbl);
		trade.sell_pos(3);
		break;
	case 353:
		//���ݵ��ſ��г�ֵ
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1);
		break;
	case 354:
		//ȫʡ���ſ��г�ֵ
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1);
		break;
	case 368:
		//һ�����շ� �绰����
		trade.set_in_par(gbl);
		trade.payonce(1, epos_menu[id].name);
		break;
	case 369:
		//һ�����շ� ���뵥���
		trade.set_in_par(gbl);
		trade.payonce(2, epos_menu[id].name);
		break;
	case 370:
		//һ�����շ� �������
		trade.set_in_par(gbl);
		trade.payonce(3, epos_menu[id].name);
		break;
	case 371:
		trade.set_in_par(gbl);
		trade.dxysl_trad( epos_menu[id].name);
		break;
	case 372:
		wrLog("�̻������½��ײ�ѯ");
		trade.set_in_par(gbl);
		//trade.merInqTrade(4);
		trade.yjycx_online(0);
		break;
	case 382:
		wrLog("�����ͳ�Υ�²�ѯ��");
		trade.set_in_par(gbl);
		trade.carnum_trafficTrade("���ͳ�Υ�²�ѯ",1);
		break;
	case 383:
		wrLog("����ͨ����ɡ�");

		bbzero(cTime);			//��ͨ����ÿ��21:00����00:00����ͣ����20081128
		tool.get_time(cTime);
		if(atoi(cTime) >= 210000 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n   ����ϵͳ�ս���\n��24:00���ٽ��н���\n   ����ȷ�ϡ�����",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0099");
		trade.set_in_par(gbl);
		trade.trafficTrade("��ͨ�����",1);
		break;
	case 384:
// 		wrLog("��ħ������600�㿨��");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("ħ������600�㿨",1);
		wrLog("����ҵ������ͣ��");
		break;
	case 385:
// 		wrLog("��ʢ�󻥶����ֿ���");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("ʢ�󻥶����ֿ�",2);
		wrLog("����ҵ������ͣ��");
		break;
	case 386:
// 		wrLog("���Ѻ�һ��ͨ��");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("�Ѻ�һ��ͨ",3);
		wrLog("����ҵ������ͣ��");
		break;
	case 387:
// 		wrLog("������һ��ͨ��");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("����һ��ͨ",4);
		wrLog("����ҵ������ͣ��");
		break;
	case 388:
// 		wrLog("������һ��ͨ��");
// 		strcpy(gbl.fee_type,"0245");
// 		trade.set_in_par(gbl);
// 		trade.sell_xmcard("����һ��ͨ",5);
		wrLog("����ҵ������ͣ��");
		break;
	case 389:
		wrLog("��ȫʡ����ͨ��Ϸ����");
		strcpy(gbl.fee_type,"0089");
		trade.set_in_par(gbl);
		//ʹ���º���
		trade.sell_txtgame_card("����ͨ��Ϸ��");
		//trade.sell_xmcard("��Ϸ��");
		break;
	case 391:
		wrLog("����ͨ��");
		strcpy(gbl.fee_type,"0089");
		trade.set_in_par(gbl);
		//trade.sellZWgameCard("����ͨ��");
		trade.sellZWgameCard("����ͨ��",0,1);
		break;
	case 393:
		wrLog("��������Ʊ��ѯ����6�����¡�");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("������Ʊ����",1);
		break;
	case 394:
		wrLog("��������Ʊ��ѯ����6�������ϡ�");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("������Ʊ����",2);
		break;
	case 395:
		wrLog("��������Ʊ��ѯ����6�����¡�");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("������Ʊ����",3);
		break;
	case 396:
		wrLog("��������Ʊ��ѯ����6�������ϡ�");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("������Ʊ����",4);
		break;
	case 397:
		wrLog("��������Ʊ��ѯ����6�����¡�");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("������Ʊ����",5);
		break;
	case 398:
		wrLog("��������Ʊ��ѯ����6�������ϡ�");
		trade.set_in_par(gbl);
		trade.nianpiaoTrade("������Ʊ����",6);
		break;
	case 399:
		wrLog("ũ��ת�ˡ���ʾ�桿");
		trade.set_in_par(gbl);
		trade.NH_Transfer("ũ��ת��");
		break;
	case 400:
		//��ͨ���г�ֵ
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3);
		break;
	case 401: 
		wrLog("��С��ͨ��ֵ��50��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "010");
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��", 1);
		break;
	case 402: 
		wrLog("��С��ͨ��ֵ��100��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "010");
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��", 2);
		break;
	case 403:
		wrLog("���ƶ���ֵ��50��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��", 2);
		break;
	case 404:
		wrLog("���ƶ���ֵ��100��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��", 3);
		break;
	case 405:
		wrLog("����ͨһ����30��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨһ����", 1);
		break;
	case 406:
		wrLog("����ͨһ����50��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨһ����", 2);
		break;
	case 407:
		wrLog("����ͨһ����100��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨһ����", 3);
		break;
	case 408:
		wrLog("�����ݵ��ſ��г�ֵ30��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 30);
		break;
	case 409:
		wrLog("�����ݵ��ſ��г�ֵ50��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 50);
		break;
	case 410:
		wrLog("�����ݵ��ſ��г�ֵ100��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 100);
		break;
	case 411:
		wrLog("�����ݵ��ſ��г�ֵ200��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 200);
		break;
	case 412:
		wrLog("�����ݵ��ſ��г�ֵ300��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 300);
		break;
	case 413:
		wrLog("�����ݵ��ſ��г�ֵ500��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 500);
		break;
	case 414:
		wrLog("���ƶ����г�ֵ30��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 30);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",30);
		break;
	case 415:
		wrLog("���ƶ����г�ֵ50��");

		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 50);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",50);
		break;
	case 416:
		wrLog("���ƶ����г�ֵ100��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 100);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 100);
		break;
	case 417:
		wrLog("���ƶ����г�ֵ300��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 300);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 300);
		break;
	case 418:
		wrLog("���ƶ����г�ֵ500��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 500);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 500);
		break;
	case 424:
	case 543:
		wrLog("���̻�ǩԼ��");
		trade.set_in_par(gbl);
		trade.customer_sign();
		break;
	case 425:
		wrLog("����Զ����Ԥ����ҵ��");
		trade.set_in_par(gbl);
		trade.qy_dxysl_trad( epos_menu[id].name,1);
		break;
	case 426:
		wrLog("����Զ����Ԥ����ҵ��");
		trade.set_in_par(gbl);
		trade.qy_dxysl_trad( epos_menu[id].name,2);
		break;
	case 427:
		wrLog("����Զ����Ԥ����ҵ��");
		trade.set_in_par(gbl);
		trade.qy_dxysl_trad( epos_menu[id].name,3);
		break;
	case 428:
		wrLog("��������Ʊ��");
		trade.set_in_par(gbl);
		trade.car_np_Trade("������Ʊ");
		break;
	case 429:
		wrLog("e����>��ѯ���"); // by liuxiao 20130408 
		if(atoi(gbl.dPosInfo.prmVer)>=9000)
		{

		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.yj_InqHdl();

		}
		else
		{
			TS_OutPutMsg(gbl.dPosInfo,"��δ��ͨ\n  �밴��ȷ�ϡ�����" ,1, iSelect);
		
		}

		break;
	case 430:
		wrLog("���޸��ϰ����롿");
		trade.set_in_par(gbl);
		trade.chgPEPwd(epos_menu[id].name);
		break;
	////////////////////////////////�ҵĳ�ˬ������START////////////////////////////////////////
	case 435:
		wrLog("����ˬ�����ס�");
		trade.set_in_par(gbl);
		trade.MyCoolPayEasy( epos_menu[id].name );
		break;
	////////////////////////////////�ҵĳ�ˬ������END//////////////////////////////////////////
	case 437:
		wrLog("���޸�POS������");
		trade.set_in_par(gbl);
		trade.updatePosPar( epos_menu[id].name );
		break;
	case 438:
		wrLog("���޸�IPOS�������뷽ʽ��");
		trade.set_in_par(gbl);
		trade.chgEPwdType( epos_menu[id].name );
		break;
	case 441:
		wrLog("������ѡ��-153��������");
		trade.set_in_par(gbl);
		trade.tel_card_select( epos_menu[id].name , 1);
		break;
	case 442:
		wrLog("������ѡ��-153���ڿ���");
		trade.set_in_par(gbl);
		trade.tel_card_select( epos_menu[id].name , 2);
		break;
	case 443:
		wrLog("���޸��ս����޶");
		trade.set_in_par(gbl);
		trade.chgDayAmo( epos_menu[id].name );
		break;
	case 449:
		wrLog("�����ų�ֵ���ѿ���");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ����ҵ��");
		break;
	case 450:
		wrLog("��������Ǯ����ֵ��");
		strcpy(gbl.fee_type,"0281");
		trade.set_in_par(gbl);
		trade.PayEasy_MoneyIn( epos_menu[id].name );
		break;
	case 468:
		wrLog("�����ų�ֵ���ѿ�-30Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ����ҵ��", 2);
		break;
	case 469:
		wrLog("�����ų�ֵ���ѿ�-50Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ����ҵ��", 3);
		break;
	case 470:
		wrLog("�����ų�ֵ���ѿ�-100Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ����ҵ��", 4);
		break;
	case 473:
		wrLog("�����ų�ֵ���ѿ�-20Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ����ҵ��", 1);
		break;
	case 480:
		wrLog("��IPOS�˻���ֵ��ѯ��");
		trade.set_in_par(gbl);
		//trade.iposTxnEnq(epos_menu[id].name, 1, 0);
		trade.zjdzcx_online(1);
		break;
	case 483:
		wrLog("���̻����㡿");
		trade.set_in_par(gbl);
		trade.chk_money( epos_menu[id].name );
		break;
	//////////////////////////////////////////////////////////////////////////

	case 451:
		wrLog("����ݸȫ��ͨҵ��-��ݸ�ƶ�ֱ�䡿");
		trade.set_in_par(gbl);
		iRet = trade.mblTrade_pre(epos_menu[id].name);
		break;
	case 452:
		wrLog("����ݸ���ź󸶷ѽɷѡ�");
		bbzero(cTime);			//����ÿ��23:45����00:00����ͣ����20071229
        CTools::get_time(cTime);
		if(atoi(cTime) >= 234500 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n �ս��У����Ժ�����\n   ����ȷ�ϡ�����",0,iSelect);
			return;
		}
		strcpy(gbl.fee_type,"0131");
		trade.set_in_par(gbl);
		iRet = trade.telTrade(epos_menu[id].name);
		break;
	case 453: 
		wrLog("����ݸ�̻���С��ͨ��ֵ��");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��",0);
		break;
	case 454:
		wrLog("����ݸADSL�����ֵ��");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=4;
		trade.set_in_par(gbl);
		trade.sellCardTrade("ADSL�����ֵ��",0);
		break;
	case 455:
		wrLog("����ݸ�ƶ��ۿ���");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��",0);
		break;
	case 456:
		wrLog("����ݸ��ͨ�ۿ���");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨ��ֵ��",0);
		break;
	case 457:
		wrLog("��ݸ����ͨ��");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("����ͨ��");
		break;
	case 458:
		wrLog("����ݸQ�ҳ�ֵ��");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q�ҳ�ֵ");

		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //�����ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015"); //glQQ
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q�ҳ�ֵ");
		}
		else
		{
		
			trade.QQTrade("Q�ҳ�ֵ");
		}
		*/


		break;
	case 459:
		wrLog("����ݸQQ��Ա���¡�");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ��Ա����");

		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //�����ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013"); // qlqq
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026"); // qlqq

			trade.GLQQTrade("QQ��Ա����");
		}
		else
		{
			trade.QQTrade("QQ��Ա����");

		}

		*/
		

		break;
	case 460:
		//��ݸ���ſ��г�ֵ
		wrLog("����ݸ���ſ��г�ֵ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1);
		break;
	case 461:
		//��ݸ�ƶ����г�ֵ
		wrLog("����ݸ�ƶ����г�ֵ��");
		if(gbl.tgroup == 34 || gbl.tgroup == 38)
		{
			TS_OutPutMsg(gbl.dPosInfo, "��ҵ�������ϵͳ������ҵ����ͣ������֮��������ԭ��", 1, iSelect);
			return;
		}
		trade.set_in_par(gbl);
		trade.mblAirPay("�ƶ�����ֱ��", 2);
		break;
	case 462:
		wrLog("����ݸ���ݽ�����");
		bbzero(cTime);			//��ͨ����ÿ��21:00����00:00����ͣ����20081128
		tool.get_time(cTime);
		if(atoi(cTime) >= 210000 && atoi(cTime) <= 235959)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n   ����ϵͳ�ս���\n��24:00���ٽ��н���\n   ����ȷ�ϡ�����",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0232");
		trade.set_in_par(gbl);
		trade.trafficTrade("��ͨ�����",2);
		break;
	case 463:
		wrLog("�������Ѻ��벹��Ʊ��");
		trade.set_in_par(gbl);
		iRet = trade.newrePrtTrade(1);
		break;
	case 464:
		wrLog("����ϵͳ�ο��Ų���Ʊ��");
		trade.set_in_par(gbl);
		iRet = trade.newrePrtTrade(2);
		break;
	case 465:
        wrLog("����ݸ�����ղ�ѯ��");
		trade.set_in_par(gbl);
		trade.merInqTrade(5);
        break;
	case 466:
        wrLog("����ݸ�����²�ѯ��");
		trade.set_in_par(gbl);
		trade.merInqTrade(6);
    	break;
	case 478:
		wrLog("��ݸ�ս��ײ�ѯ");
		trade.set_in_par(gbl);
		//trade.merInqTrade(7);
		trade.drcx_online(0);
		break;
	case 479:
		wrLog("��ݸ�½��ײ�ѯ");
		trade.set_in_par(gbl);
		//trade.merInqTrade(8);
		trade.yjycx_online(0);
		break;
	case 484:
		wrLog("�����ڵ���-�̻��ѡ�");
		trade.set_in_par(gbl);
		trade.sz_dx_trade(epos_menu[id].name);
		break;
	case 485:
		wrLog("����ݸ���ų�ֵ���ѿ��ۿ���");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�",0);
		break;
	case 487:

		if (gbl.bus_trade_flag==1) //�����ݸ����״̬Ϊ9�����ڿ۷�״̬�����ܽ��н��ס�
		{
			TS_OutPutMsg(gbl.dPosInfo,"��ݸ����������δ��ɣ����Ժ�����",0,iSelect);
			break;
		}
	
		wrLog("����ݸ������������");
		strcpy(gbl.fee_type,"0214");
		trade.set_in_par(gbl);

		if(gbl.posInfo.rightType!=5 &&gbl.HID_TYPE==0)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н��ж�ݸ��������������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ��ݸ�������á����������й�����������ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		if(gbl.posInfo.rightType==5 && gbl.HID_TYPE==1)
		{
			TS_OutPutMsg(gbl.dPosInfo," �廪ͬ������������������������ֻ��ѡ��ʹ������֮һ����鿴���������ͣ���ͨ����857���˵����롾�ⲿ�豸���á� ����ƥ��Ķ���������Ϊ��",0,iSelect);
			break;
		}

		//�ж��ն��Ƿ��ж�ݸ��������������
		if( trade.busNotEndHdl("����������������") )
		{	
			TS_OutPutMsg(gbl.dPosInfo,"����ȷ��ʧ�ܣ����Ժ����ԡ���ϵ���߿ͷ����߲���400-80-11888��ѯ",1,iSelect);
			break;

		}

		iRet = trade.busTrade(epos_menu[id].name,3); //��������
		break;

	case 488:

		if (gbl.bus_trade_flag==1) //����ն�״̬���ڿ۷�״̬�����ܽ��н���
		{
			TS_OutPutMsg(gbl.dPosInfo,"��ݸ����������δ��ɣ����Ժ�����",0,iSelect);
			break;
		}

		strcpy(gbl.fee_type,"0211");
		trade.set_in_par(gbl);
		wrLog("����ݸ��������ֵ��");

		if(gbl.posInfo.rightType!=5 && gbl.HID_TYPE==0)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н��ж�ݸ��������������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ��ݸ�������á����������й�����������ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		if(gbl.posInfo.rightType==5 && gbl.HID_TYPE==1)
		{
			TS_OutPutMsg(gbl.dPosInfo," �廪ͬ������������������������ֻ��ѡ��ʹ������֮һ����鿴���������ͣ���ͨ����857���˵����롾�ⲿ�豸���á� ����ƥ��Ķ���������Ϊ��",0,iSelect);
			break;
		}

		//�ж��ն��Ƿ��ж�ݸ��������������
		if( trade.busNotEndHdl("����������������") )
		{
			//���ڲ���ɽ���
			TS_OutPutMsg(gbl.dPosInfo,"����ȷ��ʧ�ܣ����Ժ����ԡ���ϵ���߿ͷ����߲���400-80-11888��ѯ",1,iSelect);
			break;
		}

		iRet = trade.busTrade(epos_menu[id].name,1);
		break;

	case 782: //����ѯ����ʱ�������ˣ�

		wrLog("����ݸ����������ѯ��"); 
		if (gbl.bus_trade_flag==1)// �ն�״̬Ϊ9����ݸ�����۷�״̬�����ܽ��н��ס�
		{
			TS_OutPutMsg(gbl.dPosInfo,"��ݸ����������δ��ɣ����Ժ�����",0,iSelect);
			break;
		}
		strcpy(gbl.fee_type,"0211");
		trade.set_in_par(gbl);

		if(gbl.posInfo.rightType!=5 && gbl.HID_TYPE==0)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н��ж�ݸ��������������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ���Ӧ�Ķ�ݸ�������������á����������й�����������ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		if(gbl.posInfo.rightType==5 && gbl.HID_TYPE==1)
		{
			TS_OutPutMsg(gbl.dPosInfo," �廪ͬ������������������������ֻ��ѡ��ʹ������֮һ����鿴���������ͣ���ͨ����857���˵����롾�ⲿ�豸���á� ����ƥ��Ķ���������Ϊ��",0,iSelect);
			break;
		}
		//�����ݸ�������������״������
		if( trade.busNotEndHdl("����������������") )
		{
			break;
		}
	
		iRet = trade.busTrade(epos_menu[id].name,0);
		
		break;


	case 785: //��ݸͨ������ֵ cpu�� ֻ����ʹ���廪ͬ����������
		
		wrLog("����ݸͨ������ֵ��");
		
		strcpy(gbl.fee_type,"0371"); //��ݸͨ������ֵҵ����롣
		trade.set_in_par(gbl);

		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //ֻ�����廪ͬ��������ʹ��
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ��ݸ�������������ô��󣬱�ҵ���֧���廪ͬ������������ȷ�������廪ͬ����������\n ��ͨ����857���˵����롾�ⲿ�豸���á�ѡ��ݸ�������á������������廪ͬ������������������Ϊ��������������ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		iRet = trade.dgtBusTrade(epos_menu[id].name,2); //ݸ��ͨ��ֵ

		break;

	case 801: //��ݸͨ����ѯ cpu�� ��m1����ֻ����ʹ���廪ͬ�������� 
		
		wrLog("����ݸ����������ѯ��"); 
		trade.set_in_par(gbl);	
		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //�������������������г�ֵ
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ��ҵ���֧���廪ͬ�����������������廪ͬ������������ͨ����857���˵����롾�ⲿ�豸���á�ѡ���廪ͬ��������������������Ϊ�У�ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		
		iRet = trade.dgtCardsQuery();
		break;

	case 803:
		wrLog("��ݸ��������");
		strcpy(gbl.fee_type,"0372");
		trade.set_in_par(gbl);

		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //�������������������г�ֵ
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ��ҵ���֧���廪ͬ�����������������廪ͬ������������ͨ����857���˵����롾�ⲿ�豸���á�ѡ���廪ͬ��������������������Ϊ�У�ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		iRet=trade.dgtCardSale(epos_menu[id].name);
		if (iRet==0)
		{
			trade.ecardInqHdl(); //��ѯ�˻����
		}
		
		break;

	case 804://��ݸ����ǩ��
		
		wrLog("��ݸ����ǩ��");
		trade.set_in_par(gbl);
		trade.dgtSignIn();
		break;

	case 805:
		wrLog("��ݸ����ǩ��");
		trade.set_in_par(gbl);
		trade.dgtSignOut();
		break;

	case 802: //��ݸͨ�ɿ����¿�ҵ��  ��ֻ����ʹ���廪ͬ��������

		wrLog("����ݸ�����Ծɻ��¹��ܡ�"); 
		strcpy(gbl.fee_type,"0373");
		trade.set_in_par(gbl);
		
		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //�������������������г�ֵ
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ��ҵ���֧���廪ͬ�����������������廪ͬ������������ͨ����857���˵����롾�ⲿ�豸���á�ѡ���廪ͬ��������������������Ϊ�У�ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		iRet = trade.dgtCardTransfer(epos_menu[id].name); //��ݸ�����Ծɻ��¹��� 

		break;

	case 807:
		wrLog("��ݸͨ��������ֵ�����Ѳ�ѯ");
		trade.set_in_par(gbl);
		trade.showDGTALLTrade();
		break;

	case 810:
		wrLog("����ݸͨ���������ѯ��"); 
		trade.set_in_par(gbl);
		
		if(gbl.HID_TYPE==0 || gbl.posInfo.rightType==5) //�������������������г�ֵ
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ��ҵ���֧���廪ͬ�����������������廪ͬ������������ͨ����857���˵����롾�ⲿ�豸���á�ѡ���廪ͬ��������������������Ϊ�У�ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		
		iRet = trade.getCardStatusResult();
		break;

	case 808:
		wrLog("��ݸͨ��������ֵ���ײ�ѯ");
		trade.set_in_par(gbl);
		trade.showDGTChargeTrade();
		break;

	case 809:
		wrLog("������ֱ��(ȫ��)��");
		strcpy(gbl.fee_type,"0367");//ȫ������
		trade.set_in_par(gbl);
		trade.phone_pay_trade(epos_menu[id].name);
		break;

	case 500:
		wrLog("����ݸ�ƶ��ۿ�30��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��",1);
		break;
	case 501:
		wrLog("����ݸ�ƶ��ۿ�50��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��",2);
		break;
	case 502:
		wrLog("����ݸ�ƶ��ۿ�100��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��",3);
		break;
	case 504:
		wrLog("����ݸ�ƶ����г�ֵ30��");
		trade.set_in_par(gbl);
		//trade.mblAirPay("�ƶ�����ֱ��", 2, 30);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",30);
		break;
	case 505:
		wrLog("����ݸ�ƶ����г�ֵ50��");
		trade.set_in_par(gbl);
		//trade.mblAirPay("�ƶ�����ֱ��", 2, 50);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",50);
		break;
	case 506:
		wrLog("����ݸ�ƶ����г�ֵ100��");
		trade.set_in_par(gbl);
		//trade.mblAirPay("�ƶ�����ֱ��", 2, 100);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",100);
		break;
	case 507:
		wrLog("����ݸ�ƶ����г�ֵ300��");
		trade.set_in_par(gbl);
		//trade.mblAirPay("�ƶ�����ֱ��", 2, 300);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",300);
		break;
	case 508:
		wrLog("����ݸ�ƶ����г�ֵ500��");
		trade.set_in_par(gbl);
		//trade.mblAirPay("�ƶ�����ֱ��", 2, 500);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��",500);
		break;
	case 510: 
		wrLog("����ݸ�̻���С��ͨ��ֵ30��");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��",1);
		break;
	case 511: 
		wrLog("����ݸ�̻���С��ͨ��ֵ50��");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��",2);
		break;
	case 512: 
		wrLog("����ݸ�̻���С��ͨ��ֵ100��");
		strcpy(gbl.fee_type, "0153");
		gbl.card_flag=3;
		trade.set_in_par(gbl);
		trade.sellCardTrade("С��ͨ��ֵ��",3);
		break;
	case 518:
		wrLog("����ݸ���ſ��г�ֵ30��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 30);
		break;
	case 519:
		wrLog("����ݸ���ſ��г�ֵ50��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 50);
		break;
	case 520:
		wrLog("����ݸ���ſ��г�ֵ100��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 100);
		break;
	case 522:
		wrLog("����ݸ��ͨ�ۿ�30��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨ��ֵ��",1);
		break;
	case 523:
		wrLog("����ݸ��ͨ�ۿ�50��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨ��ֵ��",2);
		break;
	case 524:
		wrLog("����ݸ��ͨ�ۿ�100��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "030");
		trade.set_in_par(gbl);
		trade.sellCardTrade("��ͨ��ֵ��",3);
		break;
	case 526:
		wrLog("����ͨ��10");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("����ͨ��",1);
		break;
	case 527:
		wrLog("��ݸ����ͨ��15");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("����ͨ��",2);
		break;
	case 528:
		wrLog("��ݸ����ͨ��30");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sellZWgameCard("����ͨ��",3);
		break;
	case 530:
		wrLog("����ݸQ�ҳ�ֵ10��");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q�ҳ�ֵ",1);

		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q�ҳ�ֵ",1);
		}
		else
		{
			trade.QQTrade("Q�ҳ�ֵ",1);
		}

		*/
		

		break;
	case 531:
		wrLog("����ݸQ�ҳ�ֵ30��");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q�ҳ�ֵ",2);

		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q�ҳ�ֵ",2);
		}
		else
		{
			trade.QQTrade("Q�ҳ�ֵ",2);

		}
		*/


		break;
	case 532:
		wrLog("����ݸQ�ҳ�ֵ50��");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q�ҳ�ֵ",3);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q�ҳ�ֵ",3);
		}
		else
		{
		
			trade.QQTrade("Q�ҳ�ֵ",3);
		}
		
		*/

		break;
	case 533:
		wrLog("����ݸQ�ҳ�ֵ100��");
		strcpy(gbl.fee_type,"0167");
		trade.set_in_par(gbl);
		trade.QQTrade("Q�ҳ�ֵ",4);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1015");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1027");
			trade.GLQQTrade("Q�ҳ�ֵ",4);
		}
		else
		{
			trade.QQTrade("Q�ҳ�ֵ",4);

		}
		
		 */

		break;
	case 537:
		wrLog("����ݸQQ��Ա1���¡�");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ��Ա����",1);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ��Ա����",1);
		}
		else
		{
		
			trade.QQTrade("QQ��Ա����",1);
		}

		*/
		

		break;
	case 538:
		wrLog("����ݸQQ��Ա3���¡�");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ��Ա����",2);

		/*
	
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ��Ա����",2);
		}
		else
		{
		
			trade.QQTrade("QQ��Ա����",2);
		}

        */
		

		break;
	case 539:
		wrLog("����ݸQQ��Ա6���¡�");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ��Ա����",3);

		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ��Ա����",3);
		}
		else
		{
		
			trade.QQTrade("QQ��Ա����",3);

		}
		*/
		

		break;
	case 540:
		wrLog("����ݸQQ��Ա12���¡�");
		strcpy(gbl.fee_type,"0168");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ��Ա����",4);
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1013");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1026");
			trade.GLQQTrade("QQ��Ա����",4);
		}
		else
		{
			trade.QQTrade("QQ��Ա����",4);
		}
		
		*/

		break;
	case 546:
		wrLog("����ݸ���ų�ֵ���ѿ�30��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�",2);
		break;
	case 547:
		wrLog("����ݸ���ų�ֵ���ѿ�50��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�",3);
		break;
	case 548:
		wrLog("����ݸ���ų�ֵ���ѿ�100��");
		strcpy(gbl.fee_type, "0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�",4);
		break;
	case 542:
		//��ݸe����ֵ
		wrLog("����ݸe����ֵ��");
		trade.set_in_par(gbl);
		trade.ecardMnyIn("�������˻�ת��",1);
		TS_HangUp(gbl.dPosInfo);
		closesocket(sd);
		break;
	case 549:
		//��ֱͨ�� ȫʡ�ӿ�
		wrLog("����ֱͨ�� ȫʡ�ӿڡ�");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 4);
		break;
	case 550:
		//�װ������ÿ�����
		wrLog("�����ÿ����");
		bbzero(gbl.fee_type);
		strcpy(gbl.fee_type,"0284");
		trade.set_in_par(gbl);
		trade.Credit_Card_Repayments("���ÿ�����");
		break;
	case 551:
		wrLog("������������Ʊ��");
		trade.set_in_par(gbl);
		trade.car_np_Trade("����������Ʊ");
		break;
	case 552:
		wrLog("������e������ת�ˡ�");
		strcpy(gbl.fee_type, "0074");
		trade.set_in_par(gbl);
		trade.eCard_Trans("e������ת��");
		break;
	case 553:
		wrLog("����ݸe������ת�ˡ�");
		strcpy(gbl.fee_type, "0075");
		trade.set_in_par(gbl);
		trade.eCard_Trans("e������ת��");
		break;
	case 556:
		wrLog("���ƶ���ֵ��30��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��", 1);
		break;
	case 557:
		wrLog("�����ų�ֵ���ѿ�200Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 5);
		break;
	case 558:
		wrLog("�����ų�ֵ���ѿ�300Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 6);
		break;
	case 559:
		wrLog("�����ų�ֵ���ѿ�500Ԫ��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 7);
		break;
	case 560:
		//��ֱͨ�� ȫʡ�ӿ�
		wrLog("����ֱͨ��30Ԫ ȫʡ�ӿڡ�");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 4, 30);
		break;
	case 561:
		//��ֱͨ�� ȫʡ�ӿ�
		wrLog("����ֱͨ��50Ԫ ȫʡ�ӿڡ�");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 4, 50);
		break;
	case 562:
		//��ֱͨ�� ȫʡ�ӿ�
		wrLog("����ֱͨ��100Ԫ ȫʡ�ӿڡ�");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 4, 100);
		break;
	case 563:
		//��ֱͨ�� ȫʡ�ӿ�
		wrLog("����ֱͨ��200Ԫ ȫʡ�ӿڡ�");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 4, 200);
		break;
	case 564:
		//��ֱͨ�� ȫʡ�ӿ�
		wrLog("����ֱͨ��500Ԫ ȫʡ�ӿڡ�");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 4, 500);
		break;
	case 565:
		//�������ÿ�����
		wrLog("�����ÿ����");
		bbzero(gbl.fee_type);
		strcpy(gbl.fee_type,"0286");
		trade.set_in_par(gbl);
		trade.Credit_Card_Repayments("���ÿ�����");
		break;
	case 566:
		wrLog("�����ų�ֵ���ѿ�200Ԫ��");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 5);
		break;
	case 567:
		wrLog("�����ų�ֵ���ѿ�300Ԫ��");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 6);
		break;
	case 568:
		wrLog("�����ų�ֵ���ѿ�500Ԫ��");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 7);
		break;
	case 569:
		wrLog("����ݸ���ſ��г�ֵ200��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 200);
		break;
	case 570:
		wrLog("����ݸ���ſ��г�ֵ300��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 300);
		break;
	case 571:
		wrLog("����ݸ���ſ��г�ֵ500��");
		trade.set_in_par(gbl);
		trade.mblAirPay("���Ż���ֱ��", 1, 500);
		break;
	case 572:
		wrLog("������ת��");
		trade.set_in_par(gbl);
		trade.Customer_Trade();
		break;
	case 574:
		wrLog("������ѯ�ĵ���ֱ�䡪����һ���ѡ�");
		trade.set_in_par(gbl);
		trade.newAirPay("����ֱ��",1);
		break;
	case 575:
		wrLog("������ѯ�ĵ���ֱ�䡪��ADSL��");
		trade.set_in_par(gbl);
		trade.newAirPay("����ֱ��",2);
		break;
	case 576:
		wrLog("������ѯ�ĵ���ֱ�䡪������ͬ�š�");
		trade.set_in_par(gbl);
		trade.newAirPay("����ֱ��",3);
		break;
	case 577:
		wrLog("e����>��ѯ���");
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.cardInqHdl();
		break;
	case 581:
		wrLog("���һ�δ�ӡ����");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_last("���һ�δ�ӡ����");
		break;
	case 582:
		wrLog("��IPOS�ϰ��ս��ײ�ѯ��");
		trade.set_in_par(gbl);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 0);
		trade.drcx_online(1);
		break;
	case 583:
		wrLog("������������á�");
		trade.set_in_par(gbl);
		trade.SetTitle();
		break;
	case 584:
		wrLog("�����г�ֵ�������롿");
		trade.set_in_par(gbl);
		trade.revAppli(epos_menu[id].name);
		break;
	case 585:
		wrLog("�����¸�����ʾ��");
		trade.set_in_par(gbl);
		trade.GetNewMessage();
		break;
	case 586:
		wrLog("��ȫʡ - �ƶ���ֵ��30��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��", 1);
		break;
	case 587:
		wrLog("��ȫʡ - �ƶ���ֵ��50��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��", 2);
		break;
	case 588:
		wrLog("��ȫʡ - �ƶ���ֵ��100��");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "020");
		trade.set_in_par(gbl);
		trade.sellCardTrade("�ƶ���ֵ��", 3);
		break;
	case 592:
		wrLog("����ݸ����ײ����桿");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,1);
		break;	
	case 593:
		wrLog("����ݸ����ײ����桿");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,2);
		break;
	case 594:
		wrLog("����ݸ����ײ����桿");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,3);
		break;
	case 595:
		wrLog("����ݸ����ײ����桿");
		trade.set_in_par(gbl);
		trade.DG_ADSL_xc(epos_menu[id].name,4);
		break;
	case 596:
		wrLog("������90�α�����Ч��");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 3, gbl.pki_card, gbl.signin_serial);
		break;
	case 597:
		wrLog("�����ͨ����ѯ��");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 8);
		break;
	case 598:
		wrLog("�����ͨ��ͨǮ����ֵ��");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}

		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 1);
	

		break;
	case 599:
		//��ͨ���г�ֵ
		wrLog("��������ֱͨ��30Ԫ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3, 30);
		break;
	case 600:
		//��ͨ���г�ֵ
		wrLog("��������ֱͨ��50Ԫ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3, 50);
		break;
	case 601:
		//��ͨ���г�ֵ
		wrLog("��������ֱͨ��100Ԫ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3, 100);
		break;
	case 612:
		//��ͨ���г�ֵ
		wrLog("��������ֱͨ��200Ԫ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3, 200);
		break;
	case 602:
		//��ͨ���г�ֵ
		wrLog("��������ֱͨ��300Ԫ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3, 300);
		break;
	case 603:
		//��ͨ���г�ֵ
		wrLog("��������ֱͨ��500Ԫ��");
		trade.set_in_par(gbl);
		trade.mblAirPay("��ͨ����ֱ��", 3, 500);
		break;
	case 604:
		wrLog("���ⲿ�豸���á�");
		trade.set_in_par(gbl);
		trade.updatePosPar( epos_menu[id].name,1);
		break;
	case 605:
		wrLog("������90��������Ч��");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 4, gbl.pki_card, gbl.signin_serial);
		break;
	case 606:
		wrLog("���������ߡ�");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 5, gbl.pki_card, gbl.signin_serial);
		break;
	case 607:
		wrLog("�������޴Ρ�");
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 6, gbl.pki_card, gbl.signin_serial);
		break;
	case 608:
		wrLog("��ֱ�䲹��Ʊ��");
		trade.set_in_par(gbl);
		trade.newrePrtTrade(1);
		break;
	case 609:
		wrLog("�麣���ź󸶷�");		
		trade.set_in_par(gbl);
		trade.foshanTrade(epos_menu[id].name, "0756");
		break;
	case 610:
		wrLog("������˻���ֵ��");
		strcpy(gbl.fee_type,"0243");
		trade.set_in_par(gbl);
		trade.RaffleTrade("����˻���ֵ");
		break;
	case 615:
		wrLog("��ͻ����ύ����");		
		trade.set_in_par(gbl);
		trade.Submission(epos_menu[id].name);
		break;
	case 616:
		wrLog("��ͻ����鿴��ֵ��");		
		trade.set_in_par(gbl);
		trade.Checkmission(epos_menu[id].name);
		break;
	case 617:
		wrLog("�������");		
		trade.set_in_par(gbl);
		trade.new_DG_ADSL_XC(epos_menu[id].name);
		break;
	case 618:
		wrLog("���ƶ��Żݳ�ֵ��");
		trade.set_in_par(gbl);
		trade.mblAirPay(epos_menu[id].name,2,0,2);
		break;
	case 619:
		wrLog("����ͨ�Żݳ�ֵ��");
		trade.set_in_par(gbl);
		trade.mblAirPay(epos_menu[id].name,3,0,2);
		break;
	case 620:
		wrLog("��ͻ���ȷ�Ͻ���");		
		trade.set_in_par(gbl);
		trade.Paymission(epos_menu[id].name);
		break;
	case 621:
		wrLog("��������e����ĸ��ת�ˡ�");
		trade.set_in_par(gbl);
		trade.cashTransfer_online(1); //�޸�Ϊֱ�Ӵ�ҳ��������ת���С�
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e����>������e����ĸ��ת�˲�ѯ���");
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
		//���һ�ʽ��ײ�ѯ
		trade.set_in_par(gbl);
		iRet=trade.merInqTrade(9);
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e����>���һ�ʽ��ײ�ѯ���ѯ���");
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
		wrLog("�ƶ���-���һ�δ�ӡ����");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_last("���һ�δ�ӡ����",1);
		break;
	case 624:
		wrLog("e����>��ѯ���");
		if(atoi(gbl.dPosInfo.prmVer)>=9000)
		{

		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.yj_InqHdl();

		}
		else
		{
			TS_OutPutMsg(gbl.dPosInfo,"��δ��ͨ\n  �밴��ȷ�ϡ�����" ,1, iSelect);
		
		}

		break;
	case 625:
		wrLog("���ƶ���-IPOS�ϰ��ս��ײ�ѯ��");
		trade.set_in_par(gbl);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 0, 1);
		trade.drcx_online(1);
		break;
	case 627:
		//���һ�ʽ��ײ�ѯ
		trade.set_in_par(gbl);
		iRet=trade.merInqTrade(10);
		if(iRet==0&&atoi(gbl.dPosInfo.prmVer)>=9000)
		{
			wrLog("e����>���һ�ʽ��ײ�ѯ���ѯ���");
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
		wrLog("���ƶ���-���һ�δ�ӡ����");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_last("���һ�δ�ӡ����",2);
		break;
	case 629:

		wrLog("e����>��ѯ���");
		if(atoi(gbl.dPosInfo.prmVer)>=9000)
		{

		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		trade.set_in_par(gbl);
		trade.yj_InqHdl();

		}
		else
		{
			TS_OutPutMsg(gbl.dPosInfo,"��δ��ͨ\n  �밴��ȷ�ϡ�����" ,1, iSelect);
		
		}

		break;
	case 630:
		wrLog("�����ƶ���-IPOS�ϰ��ս��ײ�ѯ��");
		trade.set_in_par(gbl);
        trade.mxcx_online(1);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 0, 2);
		break;
	case 631:
		wrLog("���ƶ����г�ֵ30��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 30,0,1);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 30,0,1);
		break;
	case 632:
		wrLog("���ƶ����г�ֵ50��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 50,0,1);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 50,0,1);
		break;
	case 633:
		wrLog("���ƶ����г�ֵ100��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 100,0,1);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 100,0,1);
		break;
	case 634:
		wrLog("���ƶ����г�ֵ300��");
		trade.set_in_par(gbl);
	//	trade.mblAirPay("��  ֵ", 2, 300,0,1);
		trade.mblAirPayForGDYD("�ƶ�����ֱ��", 300,0,1);
		break;
	case 636:
		wrLog("ȫʡ������С��������");

		memcpy(gbl.car_type, "01", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("ȫʡ��������");
		break;
	case 637: 
		wrLog("ȫʡ����������������");
		memcpy(gbl.car_type, "02", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("ȫʡ��������");
		break;	
	case 638: 	
		wrLog("ȫʡ�������⼮������");
		memcpy(gbl.car_type, "03", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("ȫʡ��������");
		break;
	case 639:	
		wrLog("ȫʡ�������ҳ���");
		memcpy(gbl.car_type, "04", 2);
		trade.set_in_par(gbl);
		trade.provinceTrafficTrade("ȫʡ��������");
		break;
	case 641:
		wrLog("�ͷ�����-���߿ͷ�");		
		trade.set_in_par(gbl);
		trade.OpenKF(1);
		break;
	case 642:
		wrLog("�ͷ�����-��������");		
		trade.set_in_par(gbl);
		trade.OpenKF(0);
		break;
	case 643:
		wrLog("��QQ���꡿");
		strcpy(gbl.fee_type,"0274");
		trade.set_in_par(gbl);
		trade.QQTrade("����������");

		/*
		
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1014");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1022");
			trade.GLQQTrade("����������");
		}
		else
		{
			trade.QQTrade("����������");

		}

		*/

		break;
	case 644:
		wrLog("��QQ���꡿");
		strcpy(gbl.fee_type,"0087");
		trade.set_in_par(gbl);
		trade.QQTrade("����������");


		/*
		
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1011");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1024");
			trade.GLQQTrade("����������");

		}
		else
		{
			trade.QQTrade("����������");

		}

		*/
		

		break;
	case 645:
		wrLog("��QQ���꡿");
		strcpy(gbl.fee_type,"0088");
		trade.set_in_par(gbl);
		trade.QQTrade("����������");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1010");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1023");
			trade.GLQQTrade("����������");

		}
		else
		{
			trade.QQTrade("����������");

		}

		*/
		

		break;
	case 646:
		wrLog("��QQ���꡿");
		strcpy(gbl.fee_type,"0273");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ���������");

		/*
		
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
		//	strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1012");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1025");
			trade.GLQQTrade("QQ���������");
		}
		else
		{
		
			trade.QQTrade("QQ���������");
		}
		*/

		break;
	case 647:
		wrLog("����ݸQQ���꡿");
		strcpy(gbl.fee_type,"0278");
		trade.set_in_par(gbl);
		trade.QQTrade("����������");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) > 9000) 
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1014");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1022");
			trade.GLQQTrade("����������");
		}
		else
		{
			trade.QQTrade("����������");

		}
		*/
		

		break;
	case 648:
		wrLog("����ݸQQ���꡿");
		strcpy(gbl.fee_type,"0293");
		trade.set_in_par(gbl);
		trade.QQTrade("����������");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1011");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1024");
			trade.GLQQTrade("����������");
		}
		else
		{
		
			trade.QQTrade("����������");
		}
		*/

		break;
	case 649:
		wrLog("����ݸQQ���꡿");
		strcpy(gbl.fee_type,"0294");
		trade.set_in_par(gbl);
		trade.QQTrade("����������");
		/*
		
		if (atoi(gbl.dPosInfo.prmVer) > 9000)
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1010");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1023");
			trade.GLQQTrade("����������");
		}
		else
		{
			trade.QQTrade("����������");

		}

		*/
		
		break;
	case 650:
		wrLog("����ݸQQ���꡿");
		strcpy(gbl.fee_type,"0277");
		trade.set_in_par(gbl);
		trade.QQTrade("QQ���������");
		
		/*
		if (atoi(gbl.dPosInfo.prmVer) >9000) //ipos
		{
			bbzero(gbl.glQQPayInfo.PRODUCTCODE);
			//strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1012");
			strcpy(gbl.glQQPayInfo.PRODUCTCODE,"1025");
			trade.GLQQTrade("QQ���������");
		}
		else
		{

			trade.QQTrade("QQ���������");
		}
		*/
	
		break;
	case 651:
		wrLog("��������ʮ Q��������ȡ��");
		trade.set_in_par(gbl);
		trade.Present_handsel(epos_menu[id].name);
		break;
	case 652:
		wrLog("��Υ�·�����ѯ��");
		TS_OpenIE(gbl.dPosInfo, "http://www.payeasy.com.cn/payeasy/trafficQuery/checkCar1.jsp");
		Sleep(5000);
		break;
	case 653:
		wrLog("�ش�ӡ��Ʊ_��ϵͳ�ο��Ų���");
		strcpy(gbl.fee_type,"9001");
		trade.set_in_par(gbl);
		trade.print_fail("��ϵͳ�ο��Ų���Ʊ");
		break;
	case 654:
		wrLog("�����Ӵ���ĵ���ֱ�䡪����һ���ѡ�");
		trade.set_in_par(gbl);
		trade.DZDBAirPay("����ֱ��",1,0);
		break;
	case 655:
		wrLog("�����Ӵ���ĵ���ֱ�䡪��ADSL��");
		trade.set_in_par(gbl);
		trade.DZDBAirPay("����ֱ��",2,0);
		break;
	case 656:
		wrLog("�����Ӵ���ĵ���ֱ�䡪������ͬ�š�");
		trade.set_in_par(gbl);
		trade.DZDBAirPay("����ֱ��",3,0);
		break;
	case 658:
		wrLog("�����зźš����׿����ۡ�");
		trade.set_in_par(gbl);
		trade.Sell_blank_phonecard("�׿��ɹ�", 0);
		break;
	case 659:
		wrLog("�����зźš����׿�����ȷ��1��");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard("�׿�����ȷ��", 0);
		break;
	case 660:
		wrLog("�����зźš�����������1��");
		trade.set_in_par(gbl);
		trade.Sell_phoneno("��������");
		break;
	case 661:
		wrLog("�����зźš����׿�����ȷ��2��");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard_hdl("�׿�����ȷ��", 0);
		break;
	case 662:
		wrLog("�����зźš�����������2��");
		trade.set_in_par(gbl);
		trade.Sell_phoneno_hdl("��������");
		break;
	case 663:
		wrLog("����ͨ��(����)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("������Ϸ��",0,1);
		break;
	case 664:
		wrLog("����ͨ��(�Ѻ�)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("�Ѻ���Ϸ��");
		break;
	case 665:
		wrLog("����ͨ��(ʢ��)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("ʢ����Ϸ��");
		break;
	case 666:
		wrLog("����ͨ��(�������)");
		if(strcmp(gbl.city_no, "0769") == 0)
		{
			strcpy(gbl.fee_type,"0153");
		}
		else
		{
			strcpy(gbl.fee_type,"0089");
		}
		trade.set_in_par(gbl);
		trade.sellZWgameCard("���������Ϸ��");
		break;
	case 671:
		wrLog("�����ƶ���ֵ50");
		trade.set_in_par(gbl);
		trade.mblAirPay("��  ֵ", 10, 50);
		break;
	case 672:
		wrLog("�����ƶ���ֵ100");
		trade.set_in_par(gbl);
		trade.mblAirPay("��  ֵ", 10, 100);
		break;
	case 673:
		wrLog("�����ƶ���ֵ300");
		trade.set_in_par(gbl);
		trade.mblAirPay("��  ֵ", 10, 300);
		break;
	case 674:
		wrLog("�����ƶ���ֵ500");
		trade.set_in_par(gbl);
		trade.mblAirPay("��  ֵ", 10, 500);
		break;
	case 676:
		wrLog("���޸�ת�����롿");
		trade.set_in_par(gbl);
		trade.chgTransferPwd( epos_menu[id].name );
		break;
	case 678:
		wrLog("��ɫ��������");
		trade.set_in_par(gbl);
		trade.sell_pos(9);
		break;
	case 679:
		wrLog("�������׺���������");
		trade.set_in_par(gbl);
		trade.sell_pos(5);
		break;
	case 680:
		wrLog("�������ױ�׼���ƶ�����");
		trade.set_in_par(gbl);
		trade.sell_pos(6);
		break;
	case 681:
		wrLog("��ά�����񶩹���");
		trade.set_in_par(gbl);
		trade.sell_pos(7);
		break;
	case 682:
		wrLog("����ӡ��������");
		trade.set_in_par(gbl);
		trade.sell_pos(8);
		break;
	case 683:
		wrLog("������ҵ��Ԥ����");
		trade.set_in_par(gbl);
		trade.telaffair_preaccept("����ҵ��Ԥ����");
		break;
	case 684:
		wrLog("���б��ս᡿");
		//trade.set_in_par(gbl);
		//trade.pep_distribute();
		break;
	case 687:
		wrLog("����ݸ����ͨ��Ϸ����");
		strcpy(gbl.fee_type,"0153");
		trade.set_in_par(gbl);
		trade.sell_txtgame_card("����ͨ��Ϸ��");
		break;
	case 691:
		wrLog("����Ϸֱ�����б�");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_showAmount("��Ϸֱ�����б�");
		break;
	case 692:
		wrLog("����Ϸֱ��1��");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_do1("��Ϸֱ���ֵ1");
		break;
	case 693:
		wrLog("����Ϸֱ��2��");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_do2("��Ϸֱ���ֵ2");
		break;
	case 694:
		wrLog("����Ϸֱ����Ϸ�б�-ȫ����");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_showGames("��Ϸֱ����Ϸ�б�", 0);
		break;
	case 695:
		wrLog("����Ϸֱ����Ϸ�б�-�ؼ��֡�");
		strcpy(gbl.fee_type, "0341");
		trade.set_in_par(gbl);
		trade.game_recharge_showGames("��Ϸֱ����Ϸ�б�", 1);
		break;
	case 705:
		wrLog("����ͨ���зźš����׿����ۡ�");
		trade.set_in_par(gbl);
		trade.Sell_blank_phonecard("�׿��ɹ�", 1);
		break;
	case 706:
		wrLog("����ͨ���зźš����׿�����ȷ��1��");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard("�׿�����ȷ��", 1);
		break;
	case 707:
		wrLog("����ͨ���зźš�����������1��");
		trade.set_in_par(gbl);
		trade.Unicom_Sell_Phoneno("��������");
		break;
	case 708:
		wrLog("����ͨ���зźš����׿�����ȷ��2��");
		trade.set_in_par(gbl);
		trade.Confirm_blank_phonecard_hdl("�׿�����ȷ��", 1);
		break;
	case 709:
		wrLog("����ͨ���зźš�����������2��");
		trade.set_in_par(gbl);
		trade.Unicom_Sell_Phoneno_Hdl1("��������");
		break;
	case 710:
		wrLog("����ͨ���зźš�����������3��");
		trade.set_in_par(gbl);
		trade.Unicom_Sell_Phoneno_Hdl2("��������");
		break;
	case 711:
		wrLog("�����ų�ֵ���ѿ�20Ԫ��");
		strcpy(gbl.fee_type,"0153");
		strcpy(gbl.sell_card_type, "040");
		trade.set_in_par(gbl);
		trade.sellCardTrade("���ų�ֵ���ѿ�", 1);
		break;
	case 712:

		wrLog("����;����Ʊѡվ��");
		strcpy(gbl.fee_type, "0346");
		trade.set_in_par(gbl);
		trade.coachTicketTradeGetInfo();
		break;
	case 713:

		wrLog("����;����Ʊ��ѯ��");
		strcpy(gbl.fee_type, "0346");
		trade.set_in_par(gbl);
		trade.coachTicketTradeQuery();
		break;
	case 714:

		wrLog("����;����ƱԤ����");
		strcpy(gbl.fee_type, "0346");
		trade.set_in_par(gbl);
		trade.coachTicketTradePay();
		break;
	case 721:
		wrLog("�����ô�ӡֽ���ȡ�");
		trade.setPrintPaper();
		break;
	case 723:
		wrLog("�����24Сʱ���ײ�ѯ��");
		trade.set_in_par(gbl);
		trade.cxcz_online(0);
		//trade.iposTxnEnq( epos_menu[id].name, 0, 1, 0, 1);
		break;
	case 724:
		wrLog("�����24Сʱ���ײ�ѯ-���ƶ���");
		trade.set_in_par(gbl);
        trade.cxcz_online(0);
		//trade.online_ser("cxcz");//��ѯ��������ת��Ϊ���߷���
        //trade.iposTxnEnq( epos_menu[id].name, 0, 1, 2, 1);
		break;
	case 725:
		wrLog("�����24Сʱ���ײ�ѯ-�ƶ���");
		trade.set_in_par(gbl);
        trade.cxcz_online(0);
        //trade.online_ser("cxcz");//��ѯ��������ת��Ϊ���߷���
		//trade.iposTxnEnq( epos_menu[id].name, 0, 1, 1, 1);
		break;
	case 729:
		wrLog("��Ժ��ͨ������Ӱ�һ�ȯ��");
	
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "0900");
		trade.set_in_par(gbl);
		trade.sellCardTrade("Ժ��ͨ��Ӱ�һ�ȯ", 1);
		break;
	case 730:
		wrLog("��Ժ��ͨ������ӰƱ��ѯ��");	
	
		TS_OpenIE(gbl.dPosInfo, "http://gd.118114.cn/movie");
		break;
	case 735:
		wrLog("���麣������ֵ��");
// 		trade.set_in_par(gbl);
// 		trade.ZHTrafficCardCharge("�麣��������ֵ");
		ZHBusTrade::ZHTrafficCardCharge(gbl);
		break;
	case 736:
		wrLog("�����ͨ�ɹ����ײ�ѯ��");
		trade.set_in_par(gbl);
		trade.YctSuccTxnEnq("��ӡ�ս����嵥");
		break;
	case 737:
		wrLog("�����ͨ�Ϳ�Ǽǡ�");
		trade.set_in_par(gbl);
		trade.printYctBill("�Ϳ�Ǽ�");
		break;
	case 741:
		wrLog("[��֧���˻���ֵ]");
		strcpy(gbl.fee_type,"0361");//��֧��ҵ�����0361
		trade.set_in_par(gbl);
		trade.bestPay_rechargeHdl("��֧���˻���ֵ",0);
		break;

	case 742:
		wrLog("��ѶQQ��ֵ(����ͨ�ӿ�)");
		strcpy(gbl.fee_type,"0341");
		trade.set_in_par(gbl);
		trade.qqGame_recharge_do("��ѶQQ��ֵ");//�ɳ�ȫ��������ͨ�ӿ�
		break;
	case 747:
		wrLog("н��н������Ȩ������");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_checkauthorcfg_hdl("н��н������Ȩ������");//н��н������Ȩ������
		break;
    case 748:
		wrLog("н��н����ʽ��˲�ѯ");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_pymreceived_InqHdl("н��н����ʽ��˲�ѯ");//н��н�ʽ𻮲����ײ�ѯ�ӿ�
		break;
	case 750:
		wrLog("н��н��ƶ����������");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_LLcheckcfg_hdl("н��н��ƶ����������");//н��н��ƶ����������
		break;
	case 751:
		wrLog("н��н��������������");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_LLLcheckcfg_hdl("н��н��������������");//н��н��������������
		break;
	case 760:
		wrLog("н��н��ƻ����˺�����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountcfg_hdl(0,"н��н��ƻ����˺�����");//н��н��ƻ����˺�����
		break;
	case 761:
		wrLog("н��н��ƻ����˺�����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountcfg_hdl(1,"н��н��ƻ����˺�����");//н��н��ƻ����˺�����
		break;
	case 762:
		wrLog("н��н��ƶ���������ò�ѯ");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_checkconfig_InqHdl(0,"н��н��ƶ���������ò�ѯ");//н��н��ƶ���������ò�ѯ
		break;
	case 763:
		wrLog("н��н�������������ò�ѯ");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_checkconfig_InqHdl(1,"н��н�������������ò�ѯ");//н��н�������������ò�ѯ
		break;
	case 764:
		wrLog("н��н��ƻ����˺����ò�ѯ");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountconfig_InqHdl(0,"н��н��ƻ����˺����ò�ѯ");//н��н��ƻ����˺����ò�ѯ
		break;
	case 765:
		wrLog("н��н��ƻ����˺����ò�ѯ");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_accountconfig_InqHdl(1,"н��н��ƻ����˺����ò�ѯ");//н��н��ƻ����˺����ò�ѯ
		break;
	case 766:
		wrLog("��ѯ�ɹ�����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_txn_InqHdl(0,"��ѯ�ɹ�����");//н��н�ʽ𻮲���ѯ�ɹ�����
		break;
	case 767:
		wrLog("��ѯ�ɹ�����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_txn_InqHdl(1,"��ѯ���н���");//н��н�ʽ𻮲���ѯ���н���
		break;
	case 769:
		wrLog("�޸Ĺ���Ա����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_updatepwd_Hdl(0,"�޸Ĺ���Ա����");//н��н�ʽ𻮲��޸Ĺ���Ա����
		break;
	case 770:
		wrLog("н��н�ʽ𻮲�¼��");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_funds_transferhdl("н��н�ʽ𻮲�¼��");//н��н�ʽ𻮲�¼��
		break;
	case 771:
		wrLog("�޸�¼��Ա����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_updatepwd_Hdl(1,"�޸�¼������");//н��н�ʽ𻮲��޸�¼��Ա����
		break;
	case 772:
		wrLog("н��н�ʽ𻮲����");
		strcpy(gbl.fee_type,"0368");
		trade.set_in_par(gbl);
		trade.xjx_txn_checkHdl("н��н�ʽ𻮲����");//н��н�ʽ𻮲����
		break;
	case 774:
		wrLog("�����ͨ��������");
		strcpy(gbl.fee_type,"0248");
		trade.set_in_par(gbl);
		trade.Yct_ICReader_purchaseHdl("���ͨ������");//���ͨ�׿�/�ײ͹���
		break;
	case 776:
		wrLog("�����ͨ�ֽ��ֵ��");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		CYCTYPTrade::TradeHdl(gbl, epos_menu[id].name, 7);
		break;
	case 777:
		wrLog("�����ͨ�Զ���ֵ��");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		CYCTYPTrade::Yct_AutoTradeHdl(gbl,epos_menu[id].name);    //���ͨ��ֵ���ֵ/ǩԼ�˻���ֵ
		break;
	case  779:
		wrLog("�����ͨ��ֵ������");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		trade.set_in_par(gbl);
		trade.Yct_charge_cancel("���ͨ��ֵ����");//�����ͨ��ֵ������
		break;
	case 780:
		wrLog("�����ͨ����չӦ�á�");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		trade.set_in_par(gbl);
		trade.Yct_Card_Extend("���ͨ����չӦ��");//�����ͨ��ֵ������
		break;
	case 778:
		wrLog("�����ͨ�������ת�ơ�");
		strcpy(gbl.fee_type,"0056");
		if(gbl.posInfo.rightType!=8)
		{
			TS_OutPutMsg(gbl.dPosInfo," \n ����û�н������ͨ����������\n��ͨ����857���˵����롾�ⲿ�豸���á�ѡ�����ͨ���á��������������ͨ��ȷ�Ϻ���������\n�밴��ȷ�ϡ�����",0,iSelect);
			break;
		}
		CYCTBlnTransfer::BalanceTransfer_TradeHdl(gbl,yct_data,"���ͨ�������ת��");
		break;

	case 783:

		strcpy(gbl.fee_type,"0365");
		wrLog("ȫ��ˮ��úҵ��");
		trade.set_in_par(gbl);
		trade.sdm_district_service("ˮ��ú�ɷ�"); //���в�ѯ
		break;

	case 784:
		strcpy(gbl.fee_type,"0365");
		wrLog("ȫ��ˮ��úҵ�񣬲�ѯ���ѳ��еķ������");
		trade.set_in_par(gbl);
		trade.showSMD_cityList("ˮ��úҵ���ѯ"); //����ʡ��������ѯ�õ�����ͨ�ĳ���
		break;
	case 786:
		strcpy(gbl.fee_type,"0365");
		trade.set_in_par(gbl);
		trade.sdm_trade("ˮ��ú���ɷ�");// �۷ѵ�����ѯ
		break;

	case 787:
		int iRet;
		strcpy(gbl.fee_type,"0365");
		trade.set_in_par(gbl);
		trade.sdm_handle("ˮ��ú���ɷ�"); 
		break;
	
	case 788:

		memset(gbl.fee_type,0,sizeof(gbl.fee_type));
		strcpy(gbl.fee_type,"0366");
		trade.set_in_par(gbl);
		iRet=trade.g3_trade("3G������ֱ��");
		if (iRet==0)
		{
			trade.ecardInqHdl(); //��ѯ�˻����
		}

		break;
	case  789: //��ͨȫ�����ѳ�ֵ

		strcpy(gbl.fee_type,"0370");
		trade.set_in_par(gbl);
		trade.phone_pay_trade(epos_menu[id].name); 
		break;
	case 790: //�ƶ���ֵȫ��

		strcpy(gbl.fee_type,"0369");
		trade.set_in_par(gbl);
		trade.phone_pay_trade(epos_menu[id].name); 

		break;
	case 800:
		wrLog("��֧����");
		strcpy(gbl.fee_type,"0089");
		strcpy(gbl.sell_card_type, "1000"); //����ǰ��λ��10 ��������֧����
		trade.set_in_par(gbl);
		//trade.sellCardTrade("��֧����");
		trade.sellCardTrade("��֧����");
		break;

	default:
		TS_OutPutMsg(gbl.dPosInfo, " \n\n\n������ͨ�������ע��", 1,iSelect);
		break;
	}
}

//�һ�
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
// 	int menu_id;			//���ز˵����͵Ĳ˵�ID
// 	int main_group = 1;			//���˵�������
// 	char main_ver[8+1];		//���˵��İ汾��
// 	char menu_ver[8+1];     //�˵��汾��
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
// 	sd = coninfo.sd;		//ȡ���ն�socket
// 	bbzero(gbl.ip);
// 	sprintf(gbl.ip, "%d.%d.%d.%d", coninfo.s_b1, coninfo.s_b2, coninfo.s_b3, coninfo.s_b4);
// 	
// 	bbzero(main_ver);
// 	memset(&gbl.posInfo, 0, sizeof(gbl.posInfo));
// 	memset(&gbl.dPosInfo, 0, sizeof(gbl.dPosInfo));
// 	
// 	iRet = TS_GetPosInfo(sd, &gbl.posInfo, &gbl.dPosInfo, gbl.askType, menu_id, CARD_TYPE);	//��ȡ�ն���Ϣ
// 	
//  	strcpy( log.g_pos_no, gbl.dPosInfo.pos_no);	//�����̵߳��ն˺ű���(���̶߳�����������־ʶ��)
// 	log.log_sd = sd;
// 	
// 	wrYct3MonitorLog("===========================");
//  	wrYct3MonitorLog("�����ն˼��");
// 	
// 	if (CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data) != 0)
// 	{
// 		return 1;
// 	}
// 
// 	CYCTYPTrade::P_SET_MONITOR(&gbl,gbl.dPosInfo, gbl.com, gbl.comType,&yct_data);
// 	
// 	int iMonFrq = atoi(gbl.monitor_frequency);//���Ƶ��
// 	
// 	while(1)
// 	{
// 		CYCTYPTrade::termMonitor(&gbl);
// 		Sleep(iMonFrq * 60 * 60);
// 	}
// 	
// 	wrYct3MonitorLog("�ر��ն˼��");
// 	return 0;
// }