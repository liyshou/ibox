// YCTYPTrade.cpp: implementation of the CYCTYPTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "YCTYPTrade.h"
#include "Connect.h"
#include "DB.h"
#include "Print.h"
#include "Des.h"
#include "AES.h"
#include "md5.h"
#include "Trade.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

 static int moni=0;//模拟成功
 static char char_fre[5];
 static char statusflag[10];
 static char ornotpassword[10];
 static char money_limit[10];
 static int cpurspStatus=7;
 static int flag1;
 static char a[10],des_lim[10];
 static BYTE down_limit[1+1];
 static char show_money[20];
 static float Display_credit;
 static int M1_CPU=1;//判断是M1还是CPU卡
 static int atuo_charge;//自动充值标志
 static char mny[20];
 static int change;
 static char auto_show[1024];//自动充值显示
 static int put_card;
 static int bank_signed;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYCTYPTrade::CYCTYPTrade()
{
	
}

CYCTYPTrade::~CYCTYPTrade()
{
	
}

// 检查是否有未知交易
// 入参：term_id:终端号；pki_card：现签到的PKI卡卡号；signin_serial：握手流水号
// 出参：pyct_data:羊城通交易信息；pki_card：现签到的PKI卡卡号；signin_serial：握手流水号
// 返回值：
// 0:检查通过
// 1:处理"未知结果交易"出错
// 2:签到的pki卡卡号与原交易不一致
// 3:获取串口信息失败
int CYCTYPTrade::NotFinishChk(CGlobal gbl, char *term_id, char *pki_card, char *signin_serial)
{
	int iRet=0, com=0, comType=0, iSelect=0;
	YCT_DATA yct_data;
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	memset(&yct_data, 0, sizeof(yct_data));
	
	iRet = CDB::getYctNotFinish(term_id, &yct_data);
	if( iRet==0 )//如果存在没有确认的公交交易
	{

		wrYctDataLog("getYctNotFinish", yct_data);
		strncpy(gbl.fee_type, yct_data.biz_type, sizeof(gbl.fee_type)-1);
		strncpy(gbl.pos_no, yct_data.term_id, sizeof(gbl.pos_no)-1);
		strncpy(gbl.pos_serial, yct_data.term_serial, sizeof(gbl.pos_serial)-1);
		strncpy(gbl.bank, yct_data.bank, sizeof(gbl.bank)-1);
		strncpy(gbl.card_no, yct_data.card_no, sizeof(gbl.card_no)-1);
		strncpy(gbl.amount, yct_data.amount, sizeof(gbl.amount)-1);
		strncpy(gbl.stan_no, yct_data.stan, sizeof(gbl.stan_no)-1);
		gbl.card_no_len = strlen(gbl.card_no);
		
		iRet = CTrade::getComPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &com, &comType);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			m_party = FROMCTSI;
			m_rspcode = COMNOTFOUND;
			showErrMsg(gbl, m_party, m_rspcode);
			return 3;// 3：获取串口信息失败
		}
		
		// 		if( memcmp(pki_card, yct_data.pki_card, 8)==0 )//现在已签到的pki卡卡号与原交易一致
		// 		{
		iRet = TS_OutPutMsg(gbl.dPosInfo, "现在正在确认上一笔羊城通交易\n请连接好读卡器", 0, iSelect);
		if(memcmp(yct_data.biz_type, "0056", 4)==0)//普通钱包
		{
			//判断读卡器类型
			if(gbl.posInfo.rightType == 0x08)
			{
				comType = 0x05;
				if (YCT_VER==3)
				{

					iRet = ST_initReader3(gbl.dPosInfo, com, comType,&yct_data); //第三代羊城通充值读卡器初始化操作
			
				}
				else 
				{
				
					iRet = ST_initReader(gbl.dPosInfo, com, comType); //读取第二代羊城通读卡器初始化操作
				}
			
				
				if( iRet==0 )//成功
				{

					if(strlen(yct_data.result_message) < 3)
					{
						// 							memset(yct_data.result_message, 0, 33);
						// 							memcpy(yct_data.result_message, yct_data.pki_card, 8);
						// 							memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
						// 							memcpy(yct_data.result_message+16, "FF00000000000000", 16);//超时拿不到此信息
						m_party = FROMCTSI_N;
						m_rspcode = NORESULT;
						if (YCT_VER==3)
						{
							yct_data.yct_ack=1;
						}

						yct_data.status = 7;//无法获知充值结果
						CDB::updateYctYPData(yct_data);
						showErrMsg(gbl, m_party, m_rspcode);
						//	return 1;
					}
					else if (YCT_VER==3 && strlen(yct_data.result_message) > 64 )
					{
						//第三代羊城通不完整交易处理
						if(yct_data.yct_ack==0)
						{
						
						iRet = ST_YctThird_NotFinishHdl(gbl.dPosInfo, com, comType, &yct_data,yct_data.yct_serial,\
						yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
					
						}
					
					}
					else
					{
						iRet = ST_NormalNotFinish(gbl.dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, \
						yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
					}
				}
				else
				{
					m_party = FROMCTSI_N;
					m_rspcode = CONNCOMFAIL;
					return 1;// 1：与读卡器交互失败，结果未知
				}
			}
			else
			{
				iRet = NormalNotFinish(gbl.dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);
			}
		}
		else//月票钱包
		{
			iRet = MonthNotFinish(gbl.dPosInfo, com, comType, signin_serial, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);
			
		}
		
	/*
		if(iRet==0)//成功
		{

			//m_party = FROMSUCC;
			//m_rspcode = SUCC_MSG_LAST;
			//yct_data.status = 1;//充值成功未确认 edit by liuxiao
			//更新交易总额
		//	CDB::setSumDayAmo(atoi(gbl.amount), gbl.pos_no);
		//	showErrMsg(gbl, m_party, m_rspcode);
			yct_data.yct_ack=1; //羊城通充值确认
		}
		else if(iRet==5)//失败
		{
			/* edit by liuxiao
			m_party = FROMCTSI;
			m_rspcode = FAIL_LAST;
			yct_data.status = 4;//充值失败未冲正
			showErrMsg(gbl, m_party, m_rspcode);
			*/
	//		yct_data.yct_ack=0; //羊城通充值未确认
	//	}
		
		/*
		else//未知
		{
			//m_party,m_rspcode不变
			/* edit by liuxiao 
			yct_data.status = 0;//已扣费结果未知
			//更新数据库
			showErrMsg(gbl, m_party, m_rspcode);
			*/
		//	yct_data.yct_ack=0; //羊城通充值未确认
			//return 1;
		//}
		if (yct_data.status==4 ||yct_data.status==9)
		{
			TS_OutPutMsg(gbl.dPosInfo,"您有一笔羊城通交易失败，需退费，请按【ENTER】,并留意账户余额变动",0,iSelect);
		}
		else if (yct_data.status==2)
		{

			TS_OutPutMsg(gbl.dPosInfo,"上一笔公交交易成功，按【Enter】继续交易",0,iSelect);
		}
	
		wrYctDataLog("after NotFinish", yct_data);
		//PE确认与冲正
		yct_data.status = AckRevTrade(gbl, yct_data); //羊城通最终交易状态

		if (yct_data.status==5)
		{
			yct_data.status=AckRevTrade(gbl,yct_data);
		}
		//更新数据库
		CDB::updateYctYPData(yct_data);
		if (yct_data.status==5)
		{
			TS_OutPutMsg(gbl.dPosInfo,"退费成功，请留意账户余额变动，继续交易请按【Enter】",0,iSelect);
		}
		else if(yct_data.status==6)
		{

			char tmp[120];
			bbzero(tmp);
			sprintf(tmp,"退费失败，交易系统参考号为：%s\n请联系在线客服或者拨打客服电话400-80-11888，继续交易请按【Enter】",yct_data.stan);
			TS_OutPutMsg(gbl.dPosInfo,tmp,0,iSelect);
		}
		return 0;

	}
	else
	{
		return 0;
	}
}

// 普通钱包的不完整交易
// 入参：yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
// 出参：ticket_amo_aft(12):票卡充值后余额; errCode:出错代码
// 返回值
// 0：成功
// 1：与读卡器交互失败，结果未知
// 2：读卡器返回的错误，结果未知
// 3：与羊城通交互失败，结果未知
// 4：PKI卡被更换，结果未知
// 5：失败
int CYCTYPTrade::NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	SOCKET sd=0;
	CConnect con;//连接类
	
	wrLog("【NormalNotFinish】");
	
	iRet = connTest(dPosInfo, com, comType);//连接测试，同时也断开上笔交易，初始化读卡器
	if( iRet==0 )//成功
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = CONNCOMFAIL;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	wrLog("【1: commuWithIC】");
	bbzero(icData);
	CTools::hex_to_byte((BYTE*)yct_serial+2, (BYTE*)icData, 8);
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, NORMAL_NOTFINISH_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE*)pki_card_this, 4);//管理卡逻辑卡号
			wrLog("管理卡逻辑卡号pki_card_this [%s]", pki_card_this);
			if( memcmp(pki_card_this, pki_card, 8)==0 )
			{
				sndLen = rcvLen-6-20-1;
				memcpy(sndData, rcvData+6+20, sndLen);
			}
			else
			{
				wrLog("与原交易管理卡逻辑卡号不一致pki_card [%s]", pki_card);
				*party = FROMCTSI_N;
				*rspcode = PKICHGNOTFINISH;
				return 4;// 4：PKI卡被更换，结果未知
			}			
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCD )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			return 0;// 0：成功
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCE )
		{
			*party = FROMCTSI;
			*rspcode = FAIL_LAST;
			return 5;// 5：失败
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败，结果未知
	}
	
	wrLog("【2: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, NORMAL_NOTFINISH_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCD )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			closesocket(sd);
			return 0;// 0：成功
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCE )
		{
			*party = FROMCTSI;
			*rspcode = FAIL_LAST;
			closesocket(sd);
			return 5;// 5：失败
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败，结果未知
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	wrLog("【3: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 3, NORMAL_NOTFINISH_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			if( rcvData[6]==0x00 )
			{
				*party = FROMSUCC;
				*rspcode = SUCC_MSG;
				closesocket(sd);
				return 0;// 0：成功
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = FAIL_LAST;
				closesocket(sd);
				return 5;// 5：失败
			}
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCD )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			closesocket(sd);
			return 0;// 0：成功
		}
		else if( rcvData[4]==0xA1 && (rcvData[5]==0xCE || rcvData[5]==0xCA) )
		{
			*party = FROMCTSI;
			*rspcode = FAIL_LAST;
			closesocket(sd);
			return 5;// 5：失败
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败，结果未知
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败，结果未知
	}
}

// 月票钱包的不完整交易
// 入参：signin_serial(16):握手流水号;yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
// 出参：ticket_amo_aft(12):票卡余额; errCode:出错代码; signin_serial(16):握手流水号;yct_serial(8):交易流水号;
// 返回值
// 0：成功
// 1：与读卡器交互失败，结果未知
// 2：读卡器返回的错误，结果未知
// 3：与羊城通交互失败，结果未知
// 4：PKI卡被更换，结果未知
// 5：失败
// 6：签到失败，结果未知
// 7：羊城通返回的错误， 结果未知
int CYCTYPTrade::MonthNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *signin_serial, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	SOCKET sd=0;
	CConnect con;//连接类
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	
	wrLog("【MonthNotFinish】");
	
	if( memcmp(signin_serial, "0000000000000000", 16)==0 )
	{
		iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
		if( iRet==0 )//成功
		{
			NULL;
		}
		else
		{
			switch(m_party)
			{
			case FROMIC:
				*party = FROMIC_N;
			case FROMYCT:
				*party = FROMYCT_N;
			case FROMCTSI:
				*party = FROMCTSI_N;
			default:
				*party = m_party;
			}
			*rspcode = m_rspcode;
			return 6;// 6：签到失败，结果未知
		}
	}
	else
	{
		NULL;
	}
	
	iRet = connTest(dPosInfo, com, comType);//连接测试，同时也断开上笔交易，初始化读卡器
	if( iRet==0 )//成功
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = CONNCOMFAIL;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	wrLog("【1: commuWithIC】");
	bbzero(icData);
	CTools::hex_to_byte((BYTE*)yct_serial, (BYTE*)icData, 10);
	CTools::hex_to_byte((BYTE*)signin_serial, (BYTE*)icData+5, 16);
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, MONTH_NOTFINISH_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6+8, (BYTE*)pki_card_this, 4);//管理卡逻辑卡号
			wrLog("管理卡逻辑卡号pki_card_this [%s]", pki_card_this);
			if( memcmp(pki_card_this, pki_card, 8)==0 )
			{
				sndLen = rcvLen-6-20-1;
				memcpy(sndData, rcvData+6+20, sndLen);
			}			
			else
			{
				wrLog("与原交易管理卡逻辑卡号不一致pki_card [%s]", pki_card);
				*party = FROMCTSI_N;
				*rspcode = PKICHGNOTFINISH;
				return 4;// 4：PKI卡被更换，结果未知
			}			
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败，结果未知
	}
	
	wrLog("【2: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, MONTH_NOTFINISH_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			if( rcvData[6+5]==0x00 )
			{
				*party = FROMSUCC;
				*rspcode = SUCC_MSG;
				closesocket(sd);
				return 0;// 0：成功
			}
			else if( rcvData[6+5]==0xA1 || rcvData[6+5]==0xFF )
			{
				*party = FROMCTSI;
				*rspcode = FAIL_LAST;
				closesocket(sd);
				return 5;// 5：失败
			}
			else
			{
				*party = FROMYCT_N;
				*rspcode = rcvData[6+5];
				closesocket(sd);
				return 7;// 7：羊城通返回的错误， 结果未知
			}
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败，结果未知
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败，结果未知
	}
}

void CYCTYPTrade::setInPar(CGlobal inGbl) //设置传入参数
{
	gbl = inGbl;
	getComPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, m_com, m_comType);
}
int CYCTYPTrade::getComPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int &com, int &comType)
{
	int flag = 0; //初始化
	//内置串口
	if(inType==5 || inType==8)
	{
		comType = inType;
		com  = 0x03;
		flag = 1;
	}
	//左串口
	else if(leftType==5 || leftType==8)
	{
		comType = leftType;
		com  = 0x01;
		flag = 1;
	}
	//右串口
	else if(rightType==5 || rightType==8)
	{
		comType = rightType;
		com  = 0x02;
		flag = 1;
	}
	if(flag)
		return 0;
	else
		return -1;
}
//确认与冲正
// 入参：yct_data:羊城通交易信息
// 出参：无
// 返回值：
// 2：确认成功
// 3：确认失败
// 5：冲正成功
// 6：冲正失败
int CYCTYPTrade::AckRevTrade(CGlobal gbl, YCT_DATA yct_data)
{
	int iRet=0;
	if(yct_data.status==1)//成功
	{
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		CTrade trade;
		trade.set_in_par(gbl);
		trade.cardInq_yctyp();
		iRet=AckTrade(gbl, yct_data);//确认
		wrLog("羊城通交易状态【%d】",iRet);
	}
	else if (yct_data.status==5)
	{
		CTrade trade;
		trade.set_in_par(gbl);
		iRet=AckTrade(gbl, yct_data);//确认
		wrLog("羊城通交易状态【%d】",iRet);
	}

	else if(yct_data.status==4 || yct_data.status==9) //退费交易
	{
		iRet=RevTrade(gbl, yct_data);//冲正
	}
	else
	{
		iRet = yct_data.status;
	}
	return iRet;
}


//主函数
//payType:1:非月票先查询 2:非月票直接充值 3:公交90次本月生效 4:公交90次下月生效 5:公交定线 6:地铁限次 7:现金充值 8:余额查询
// 返回值：
// 0：成功已确认
// 1：成功未确认
// 2：失败已冲正
// 3：失败未冲正
// 4：未知结果
// 5：扣费前失败
int CYCTYPTrade::TradeHdl(CGlobal gbl, char *title, int payType, char *pki_card, char *signin_serial)
{
	int iRet=0, iSelect=0;
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	char m_title[1024];
	char priCont[2048];
	bbzero(m_title);
	bbzero(priCont);
	
	strcpy(m_title, title);
	
	CDB::update_epos_term(gbl.pos_no, "8");//将终端状态设为"公交扣费中"
	
	iRet = TS_OutPutMsg(gbl.dPosInfo, "请指引客户将公交卡正确放置在读卡器上，且勿移动它。\n若是天翼羊城通/天翼广佛通充值，请首先关机后再放置在读卡器上，切勿随便挪动。\n按【确认】继续 按【返回】取消", 0, iSelect);
	if( iRet==0 && iSelect==0 )
	{
		NULL;
	}
	else
	{
		return 5;// 5：扣费前失败
	}
	
	if( payType==1 || payType==2 || payType==7 || payType==8)
	{
		strcpy(gbl.fee_type, "0056");
		iRet = YCT_NormalTrade(&gbl, payType, &m_party, &m_rspcode, priCont);
		if (payType==7)
		{
			strcpy(m_title,"羊城通现金充值");
		}
		else
		{
			strcpy(m_title, "羊城通普通钱包充值");
		}
	}
	else if( payType==3 || payType==4 || payType==5 || payType==6 )
	{
		strcpy(gbl.fee_type, "0247");//月票
		iRet = MonthTrade(&gbl, payType, pki_card, signin_serial, &m_party, &m_rspcode, priCont);
	}
	else
	{
		m_party = FROMCTSI;
		m_rspcode = FATALERR;
		iRet = 5;                                                                                                                                                                                                                                                                                                                                                                                                                            
		wrLog("payType error");
	}

	CDB::update_epos_term(gbl.pos_no, "1");//将终端状态设为"正常"

	if (payType==8 &&(m_party==FROMSUCC && (m_rspcode==SUCC_MSG || m_rspcode==SUCC_NOMSG)))
	{
		return 0;
	}
	showErrMsg(gbl, m_party, m_rspcode);
	if( m_party==FROMSUCC && (m_rspcode==SUCC_MSG || m_rspcode==SUCC_NOMSG) )  //成功打印凭条
	{
		CPrint prt;
		CPack pack;
		memset(&pack, 0, sizeof(pack));
		bbzero(pack.FIELD37);
		memcpy(pack.FIELD37,gbl.stan_no,12);
		prt.set_in_par(gbl);
// 		if (payType == 7)
// 		{
// 			prt.print_yctthird_detail(0, priCont);
// 		}
// 		else
// 		{
			prt.print_detail(20, m_title, "", "", priCont);
/*		}*/
	}
	else
	{
		NULL;
	}
	return iRet;
}

int CYCTYPTrade::NormalSelectAmo(CGlobal* pgbl)
{
	int iRet,iSelect, nowAmo;
	TSMENULIST menuList;
	memset(&menuList,0,sizeof(menuList));
	if(TRADE_FLAG==1)
	{
		menuList.iTotalNum = 4;
		strcpy(menuList.acTitle, "请选择您要充值的金额");
		strcpy(menuList.menuItem[0], "50元");
		strcpy(menuList.menuItem[1], "100元");
		strcpy(menuList.menuItem[2], "150元");
		strcpy(menuList.menuItem[3], "200元");


	}
	else
	{
		menuList.iTotalNum = 5;
		strcpy(menuList.acTitle, "请选择您要充值的金额");
		strcpy(menuList.menuItem[0], "50元");
		strcpy(menuList.menuItem[1], "100元");
		strcpy(menuList.menuItem[2], "150元");
		strcpy(menuList.menuItem[3], "200元");
		//	strcpy(menuList.menuItem[4],"退押金(10元)"); //押金
		strcpy(menuList.menuItem[4], "1分钱");

	}

	
	iRet = TS_Menu(pgbl->dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
	if(iRet==HOTKEY)
	{
		return CANCLE_KEY;
	}
	else if(iRet!=0)
	{
		return iRet;
	}
	else
	{
		NULL;
	}
	
	strcpy(pgbl->amount, "");

	if (TRADE_FLAG==1)
	{

		switch(iSelect)
		{
			
			
		case 1:
			strcpy(pgbl->amount,"000000005000");
			return 0;
		case 2:
			strcpy(pgbl->amount,"000000010000");
			return 0;
		case 3:
			strcpy(pgbl->amount,"000000015000");
			return 0;
		case 4:
			strcpy(pgbl->amount,"000000020000");
			return 0;
			//	case 5:
			//		strcpy(pgbl->amount,"000000001000"); //退押金（10元），不需要E卡充值
			//		return 0;
					
		default: 
			return CANCLE_KEY;
	}

	}
	else
	{
	
		switch(iSelect)
		{
			
			
				case 1:
					strcpy(pgbl->amount,"000000005000");
					return 0;
				case 2:
					strcpy(pgbl->amount,"000000010000");
					return 0;
				case 3:
					strcpy(pgbl->amount,"000000015000");
					return 0;
				case 4:
					strcpy(pgbl->amount,"000000020000");
					return 0;
					//	case 5:
					//		strcpy(pgbl->amount,"000000001000"); //退押金（10元），不需要E卡充值
					//		return 0;
				case 5:
					strcpy(pgbl->amount,"000000000001");
					return 0;
				default: 
					return CANCLE_KEY;
		}
				

		}

	
	//判断余额是否充足
	if(CDB::getDayAmo(pgbl->dPosInfo.pos_no, &nowAmo)==0)
	//20120105修改-yyx
	//if (CDB::pep_getDayAmo(pgbl->pos_no, &nowAmo) == 0)
	{
		if( pgbl->dayAmo!=0 && nowAmo+atoi(pgbl->amount)>pgbl->dayAmo )
		{
			TS_OutPutMsg(pgbl->dPosInfo, "已超出当天的最大消费金额，交易取消", 1, iSelect);
			return -1;
		}
	}
	else
	{
		TS_OutPutMsg(pgbl->dPosInfo, "数据库操作出错，交易取消\n请稍候再试!\n 如果交易结果不明确请做 最后一笔交易查询", 1, iSelect);
		return -1;
	}
}

// 普通钱包的主函数
// 入参：无
// 出参：无
// 返回值：
// 0：成功已确认
// 1：成功未确认
// 2：失败已冲正
// 3：失败未冲正
// 4：未知结果
// 5：扣费前失败
int CYCTYPTrade::NormalTrade(CGlobal *pgbl, int payType, BYTE* party, BYTE* rspcode, char *priCont)
{
	int iRet=0, iSelect=0, com=0, comType=0;
	char temp_buff[512];
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	YCT_DATA yct_data;
	char card_physics_info[100];
	char ticket_no[20];
	CPack pack; 
	CTrade trade;
	int query_iRet;

	bbzero(ticket_no);

	iRet = CTrade::getComPar(pgbl->posInfo.leftType, pgbl->posInfo.rightType, pgbl->posInfo.inType, &com, &comType);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = COMNOTFOUND;
		return 5;// 5：扣费前失败
	}
	
	if((pgbl->posInfo).rightType == 0x08)	//索天读卡器
	{
		comType = 0x05;
		if (YCT_VER==3)
		{
			char cur_signTime[30+1];
			init_yct_data(&yct_data);
			CDB::getYct3SignInfo(pgbl->pos_no,&yct_data,cur_signTime);
			strcpy(yct_data.card_no,pgbl->card_no);


		}
		else
		{
			memset(&yct_data, 0, sizeof(yct_data));
			iRet = ST_initReader(pgbl->dPosInfo, com, comType);
		}
	
		if(iRet)
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 5;
		}
	}
	//暂时屏蔽明华读卡器交易
	else
	{
		TS_OutPutMsg(pgbl->dPosInfo, "无效硬件，无法继续交易，请重新设置，如有疑问请联系交费易客服", 0, iSelect);
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 5;
	}


	if(payType==1 || payType==7 || payType==8)//先查询
	{
		//判断读卡器类型
		//iRet = ST_initReader(pgbl->dPosInfo, com, comType);
		if((pgbl->posInfo).rightType == 0x08)
		{
			if(YCT_VER==3)
			iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			else
			iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			//进行交易查询
		}
		else
		{
			iRet = NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
		}

		if (m_rspcode == BLACKLIST)
		{
			*party = m_party;
			*rspcode = m_rspcode;
			TS_OutPutMsg(pgbl->dPosInfo,"此卡为黑名单卡，交易不允许!",1,iSelect);
			return 0;
		}
		
		if( iRet==0 )
		{
			if (M1_CPU == 1)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//提示余额
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
//    			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				Display_credit = atof(mny)/100;
				sprintf(show_money,"%.2f",Display_credit);
			}
			if (M1_CPU == 2)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//提示余额
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
				//			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				wrLog("余额下限:%.2f元",atof(des_lim));
				Display_credit = atof(mny)/100 - atof(des_lim);
//				wrLog("钱包显示的余额:%.2f",Display_credit);
				sprintf(show_money,"%.2f",Display_credit);
//				wrLog("钱包显示余额:%.2f",atof(show_money));
			}

			wrMonitorLog(12, 0, 0, *pgbl, pack);	//监控日志
			bbzero( temp_buff );
			if (payType == 8)
			{
				sprintf(temp_buff, "您的余额为：%s元\n按【Esc】返回", show_money);
			}
			else
			{
				sprintf(temp_buff, "您的余额为：%s元\n按【确认】充值\n按【返回】取消", show_money);
			}
			TS_OutPutMsg(pgbl->dPosInfo, temp_buff, 0, iSelect);
			if( iSelect==0 )
			{
				NULL;
			}
			else
			{
				if(iRet==0)
				{
					*party = FROMCTSI;
					*rspcode = USERCANCEL;
				}
				else
				{
					*party = FROMCTSI;
					*rspcode = ICTIMEOUT;
				}
				return 5;// 5：扣费前失败
			}
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5：扣费前失败
		}
	}
	else
	{
		NULL;
	}

	if (payType == 8)
	{
		//只查询
		return 0;
	}
	//选金额
	iRet = NormalSelectAmo(pgbl);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		if( iRet==CANCLE_KEY )
		{
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
		}
		return 5;// 5：扣费前失败
	}
	
	bbzero( temp_buff );
	if(payType == 1 || payType==7)
		sprintf(temp_buff, "交易前余额：%s元\n充值金额：%.2f元\n交易后金额：%.2f元\n按【确认】充值\n按【返回】取消", 
		show_money, atof(pgbl->amount)/100, atof(pgbl->amount)/100 + atof(show_money));
	else if(payType == 2)
		sprintf(temp_buff, "充值金额：%.2f元\n按【确认】充值\n按【返回】取消", 
		atof(pgbl->amount)/100);

/*
	if (atof(pgbl->amount)/100 + money - 1000.00 >0.001)
	{
		TS_OutPutMsg(pgbl->dPosInfo," \n余额充值后超过余额上限。请按【确认】返回",0,iSelect);
		if (iSelect == 0)
		{
			*party = FROMIC;
			*rspcode = BALANCE_EXCEED;
			return 5;
		}
	}
*/
	iRet = TS_OutPutMsg(pgbl->dPosInfo, temp_buff, 0, iSelect);
	
	if( iSelect==0 )
	{
		NULL;
	}
	else
	{
		if(iRet==0)
		{
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
		}
		return 5;// 5：扣费前失败
	}
	

	//扣费的时候读取一次卡信息。如果出现读卡的羊城通卡号和交易的卡号不一致，则取消交易，三次提示用户继续读卡，三次均不成功，则取消该次交易
	//获取卡片的物理信息,返回卡片的物理信息
	/***************************begin*********************************************/
	for (int i = 0; i<3; i++)
	{
		bbzero(card_physics_info);
		query_iRet=R_PUB_QRY_PHYSICS_INFO(pgbl->dPosInfo,com,comType,(BYTE *)card_physics_info);
		if (query_iRet == 0)
		{
			char SW1SW2[4+1];
			bbzero(SW1SW2);
			CTools::byte_to_hex((BYTE *)card_physics_info+8,(BYTE *)ticket_no,8);//取得逻辑卡号
			CTools::byte_to_hex((BYTE *)card_physics_info+49,(BYTE *)SW1SW2,2);
			
			BYTE temp_SW1SW2[2+1];
			bbzero(temp_SW1SW2);
			memcpy(temp_SW1SW2,card_physics_info+49,2);
			if (memcmp(SW1SW2,"9000",4) != 0)
			{
				int iSelect = 0;
				char errMsg[512];
				char outPut_Msg[512];
				get_cardStatus_errMsg((BYTE *)temp_SW1SW2,errMsg);
				bbzero(outPut_Msg);
				sprintf(outPut_Msg,"请放好卡片，重新读卡,返回响应卡状态码错误：%s",errMsg);
				TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
				
			}
			else
			{
				if (strcmp(yct_data.ticket_no,ticket_no) !=0 ) //判断卡片是否与读卡器之前查询的卡片一致
				{
					TS_OutPutMsg(pgbl->dPosInfo,"卡片与读卡的卡片信息不一致，请重试",1,iSelect);
				}
				
				else
				{
					
					break;
					
				}
				
			}
			
			
			
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo,"读取卡片信息错误，请重新放卡",1,iSelect);
			
		}
	}
	
	if (query_iRet!=0 )
	{
		TS_OutPutMsg(pgbl->dPosInfo,"读取卡片信息失败，交易失败",1,iSelect);
		*party = FROMCTSI;
		*rspcode = USERCANCEL;
		return 5;
	}
	else
	{
		if (strcmp(yct_data.ticket_no,ticket_no) !=0)
		{
			TS_OutPutMsg(pgbl->dPosInfo,"读取卡片信息失败，交易失败",1,iSelect);
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
			return 5;
			
		}
		
	}
	
	
	/***************end*of*readcardinfo*******************************************************/
	

	/***********************end*of*readcardinfo***********************************************/


	if(atoi(pgbl->amount)==1000) //扣押金pe不进行扣费
		
	{
		TS_OutPutMsg(pgbl->dPosInfo,"\n本业务仅限于羊城通押金卡，可通过退押金进行充值，无需刷卡\n 按【确认】进入，按【ESC】退出",0,iSelect);
		//记录羊城通交易数据 
		
		CTools::getDate(yct_data.txn_date);
		CTools::getTime(yct_data.txn_time);
		//生成羊城通充值终端号
		CTools::getYctStan(pgbl->stan_no); //获取羊城通系统参考号
		CTools::getYct_posSerial(pgbl->pos_serial);
		
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
		strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
		strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
		strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
		strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
		
		yct_data.status = 6; //默认为充值失败.冲正失败
		CDB::setYctYPData(yct_data); 
	}
    else
	{
		//刷卡输密码PE扣费

		iRet = PayPe(pgbl, yct_data, &m_party, &m_rspcode);
		
		if( iRet==0 )//成功
		{
			strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
			bbzero(yct_data.txn_date);
			CTools::getDate(yct_data.txn_date);
			bbzero(yct_data.txn_time);
			CTools::getTime(yct_data.txn_time);
			bbzero(yct_data.biz_type);
			strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
			bbzero(yct_data.term_id);
			strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
			bbzero(yct_data.term_serial);
			strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
			bbzero(yct_data.bank);
			strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
			bbzero(yct_data.card_no);
			strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
			bbzero(yct_data.amount);
			strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
			bbzero(yct_data.yct_serial);
			bbzero(yct_data.result_message);
			bbzero(yct_data.ticket_amo_aft);
			//在获取充值密钥之前，默认充值失败
			yct_data.status = 4;
			//记录羊城通交易数据
			CDB::setYctYPData(yct_data);
			
			//更新交易总额
			//CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
		}else if (iRet == 2) 
		{
			strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
			bbzero(yct_data.txn_date);
			bbzero(yct_data.txn_time);
			CTools::getDate(yct_data.txn_date);
			CTools::getTime(yct_data.txn_time);
			bbzero(yct_data.biz_type);
			strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
			bbzero(yct_data.term_id);
			strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
			bbzero(yct_data.term_serial);
			strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
			bbzero(yct_data.bank);
			strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
			bbzero(yct_data.card_no);
			strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
			bbzero(yct_data.amount);
			strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
			bbzero(yct_data.yct_serial);
			bbzero(yct_data.result_message);
			//记录羊城通交易数据
			yct_data.status = 4;
			CDB::setYctYPData(yct_data);
			
			//PE确认与冲正
			trade.set_in_par(*pgbl);
			yct_data.status = trade.YCT_AckRevTrade(yct_data,yct_data.status); //扣费前失败
			//发送充值确认
			if (yct_data.status==5)
			{
				iRet=trade.YCT_AckRevTrade(yct_data,yct_data.status); //冲正成功才发送确认
			}
			//更新数据库
			CDB::updateYctYPData(yct_data);
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 4;// 5：扣费前失败
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 5;// 5：扣费前失败
		}

	}
	
	try
	{
		//充值
		if((pgbl->posInfo).rightType == 0x08)
		{
			if(YCT_VER==3)
			iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALPAY, &yct_data, pgbl->amount, &m_party, &m_rspcode);
			else 
			iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALPAY, &yct_data, pgbl->amount, &m_party, &m_rspcode);
		}
		else
		{
			iRet = NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALPAY, &yct_data, pgbl->amount, &m_party, &m_rspcode);
		}
	}
	catch (...)
	{
		wrLog("szCause");
		TS_OutPutMsg(pgbl->dPosInfo, " \n\n\n羊城通充值发生异常，请再次查询余额核对充值是否成功，如有疑问请按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询", 1,iSelect);
	}
	wrYctDataLog("NormalPay", yct_data);
	
	if( iRet==0 )
	{
		if(atoi(pgbl->amount)==1000)
		{
			
			yct_data.status = 2; //退押金成功，不需要pe 进行确认，羊城通终端交易状态直接修改为2
			CDB::updateYctYPData(yct_data);
			
		}
		else
		{
			yct_data.status = 1;
		}
	

	}
	else if( iRet==1 || iRet==2 || iRet==3 || iRet==4 || iRet==8 )
	{
		if(atoi(pgbl->amount)==1000) //如果是退押金
		{
			
			yct_data.status=6;
		}
		
		else
		{
			yct_data.status = 4;
		}
	}
	else if(iRet==10)//写卡时超时，不明确卡是否充值成功
	{
		if((pgbl->posInfo).rightType == 0x08)
		{
			int i = 0;
			YCT_DATA yct_data_check;
			memset(&yct_data_check, 0, sizeof(yct_data_check));
			iRet = 1;
			while(i<3 && iRet){
				if(YCT_VER==3)
				{
			
					iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data_check, "000000000000", &m_party, &m_rspcode);
				}
				else 
				{
					iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data_check, "000000000000", &m_party, &m_rspcode);
				}
				
				i++;
				Sleep(1000*3);
			}
			if(!iRet)
			{
				if(strcmp(yct_data.ticket_amo_bef, yct_data_check.ticket_amo_bef) && !strcmp(yct_data.ticket_no, yct_data_check.ticket_no))
				{
					if(atoi(pgbl->amount) == (atoi(yct_data_check.ticket_amo_bef) - atoi(yct_data.ticket_amo_bef)))
					{
						memset(yct_data.result_message, 0, 33);
						memcpy(yct_data.result_message, yct_data.pki_card, 8);
						memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
						memcpy(yct_data.result_message+16, "0003000000000000", 16);
						iRet = ST_NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
						
						wrYctDataLog("after NormalNotFinish", yct_data);
						if(iRet==0)//成功
						{

							if(atoi(pgbl->amount)==1000)

								{
								//如果是押金充值，充值成功后不用pe 确认，直接修改交易状态
								//	CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
								yct_data.status = 2;
								CDB::updateYctYPData(yct_data);
								}
							
							else
							{
							//更新交易总额
							CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
							yct_data.status = 1;//充值成功未确认
							}
						}
						else//未知
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//更新数据库
							CDB::updateYctYPData(yct_data);
							return 4;// 4：未知结果
						}
					}
					else if(atoi(yct_data_check.ticket_amo_bef) == atoi(yct_data.ticket_amo_bef))
					{
						memset(yct_data.result_message, 0, 33);
						memcpy(yct_data.result_message, yct_data.pki_card, 8);
						memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
						memcpy(yct_data.result_message+16, "FF00000000000000", 16);
						iRet = ST_NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
						
						wrYctDataLog("after NormalNotFinish", yct_data);
						if(iRet==5)//失败
						{
							if (atoi(pgbl->amount)==1000)
							{
								yct_data.status = 6;//充值失败冲正失败
							}
							else
							{
								
								yct_data.status = 4;//充值失败未冲正
								
							}
							
						}
						else//未知
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//更新数据库
							CDB::updateYctYPData(yct_data);
							return 4;// 4：未知结果
						}
					}
				}
			}
			else
			{
				m_party = FROMCTSI_N;
				m_rspcode = NORESULT;
				yct_data.status = 7;
			}
		}
		else
		{
			yct_data.status = 4;
		}
	}
	else//需要做不完整交易
	{
		if((pgbl->posInfo).rightType == 0x08)
		{
			if(iRet == 11)
			{
				memset(yct_data.result_message, 0, 33);
				memcpy(yct_data.result_message, yct_data.pki_card, 8);
				memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
				memcpy(yct_data.result_message+16, "FF00000000000000", 16);
			}
			else if(strlen(yct_data.result_message) < 3)
			{
				// 				memset(yct_data.result_message, 0, 33);
				// 				memcpy(yct_data.result_message, yct_data.pki_card, 8);
				// 				memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
				// 				memcpy(yct_data.result_message+16, "FF00000000000000", 16);
				m_party = FROMCTSI_N;
				m_rspcode = NORESULT;
				yct_data.status = 7;//无法获知充值结果
				CDB::updateYctYPData(yct_data);
				return 4;
			}
			iRet = ST_NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
		}
		else
		{
			iRet = NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);	
		}
		
		wrYctDataLog("after NormalNotFinish", yct_data);
		if(iRet==0)//成功
		{
	
			if(atoi(pgbl->amount)==1000)
			{
				//如果是押金充值，不需要进行pe确认，直接修改最后的交易状态
				//CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 2;
				CDB::updateYctYPData(yct_data);
			}
			else
			{
				//更新交易总额
				CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 1;//充值成功未确认	
				
			}		
		}
		else if(iRet==5)//失败
		{
			if (atoi(pgbl->amount)==1000)
			{
				yct_data.status = 6;//充值失败冲正失败
			}
			else
			{
				yct_data.status = 4;//充值失败未冲正
			}
		}
		else//未知
		{
			yct_data.status = 0;
			*party = m_party;
			*rspcode = m_rspcode;
			//更新数据库
			CDB::updateYctYPData(yct_data);
			return 4;// 4：未知结果
		}
	}
	CDB::updateYctYPData(yct_data);
	//PE确认与冲正
	yct_data.status = AckRevTrade(*pgbl, yct_data);
	if (yct_data.status == 3)
	{
		*party = FROMPE;
		*rspcode = 99;
		return 1;
	}
	//更新数据库
	CDB::updateYctYPData(yct_data);
	CDB::update_epos_term(pgbl->pos_no, "1");//将终端状态设为"正常"

	if (payType == 7)
	{
		set_yctthird_priCont(0,&yct_data,priCont);
	}
	else
	{
		strcat(priCont, "羊城通卡号：");
		strcat(priCont, yct_data.ticket_no);
		strcat(priCont, "\n羊城通流水号：");
		strcat(priCont, yct_data.yct_serial+2);
		sprintf(priCont+strlen(priCont), "\n原余额: %.2f元", atof(yct_data.ticket_amo_bef)/100);
		sprintf(priCont+strlen(priCont), "\n充值金额: %.2f元", atof(yct_data.amount)/100);
		sprintf(priCont+strlen(priCont), "\n现余额: %.2f元", atof(yct_data.ticket_amo_bef)/100 + atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n充值金额: %.2f元", atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n现余额: %.2f元", atof(yct_data.ticket_amo_aft)/100);
	}
	strcpy(pgbl->yct_disp,priCont);
	
	*party = m_party;
	*rspcode = m_rspcode;
	return 0;
}


//改造函数
// 普通钱包的主函数
// 入参：无
// 出参：无
// 返回值：
// 0：成功已确认
// 1：成功未确认
// 2：失败已冲正
// 3：失败未冲正
// 4：未知结果
// 5：扣费前失败
int CYCTYPTrade::YCT_NormalTrade(CGlobal *pgbl, int payType, BYTE* party, BYTE* rspcode, char *priCont)
{
	int iRet=0, iSelect=0, com=0, comType=0;
	char temp_buff[512];
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	YCT_DATA yct_data;
	CPack pack; 
	CTrade trade;
	int  query_iRet;
	char card_physics_info[100];
	char ticket_no[20];
	bbzero(ticket_no);
	bbzero(card_physics_info);

	iRet = trade.getComPar(pgbl->posInfo.leftType, pgbl->posInfo.rightType, pgbl->posInfo.inType, &com, &comType);
	if( iRet==0 )
	{
		NULL;
	}

	else
	{

			*party = FROMCTSI;
			*rspcode = COMNOTFOUND;
			return 5;// 5：扣费前失败
	}

	
	if(pgbl->posInfo.rightType == 0x08)	//索天读卡器
	{
		comType = 0x05;
		if (YCT_VER==3)
		{
			char cur_signTime[30+1];
			init_yct_data(&yct_data);
			CDB::getYct3SignInfo(pgbl->pos_no,&yct_data,cur_signTime);
			strcpy(yct_data.card_no,pgbl->card_no);


		}
		else
		{
			memset(&yct_data, 0, sizeof(yct_data));
			iRet = ST_initReader(pgbl->dPosInfo, com, comType);
		}
	
		if(iRet)
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 5;
		}
	}
	//暂时屏蔽明华读卡器交易
	else
	{
		TS_OutPutMsg(pgbl->dPosInfo, "无效硬件，无法继续交易，请重新设置，如有疑问请联系交费易客服", 0, iSelect);
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 5;
	}


	if(payType==1 || payType==7 || payType==8)//先查询
	{
		//判断读卡器类型
		//iRet = ST_initReader(pgbl->dPosInfo, com, comType);
		if(pgbl->posInfo.rightType == 0x08)
		{
			if(YCT_VER==3)
			iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			else
			iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			//进行交易查询
		}
		else
		{
			iRet = NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
		}

		if (m_rspcode == BLACKLIST)
		{
			*party = m_party;
			*rspcode = m_rspcode;
			TS_OutPutMsg(pgbl->dPosInfo,"此卡为黑名单卡，交易不允许!",1,iSelect);
			return 0;
		}
		
		if( iRet==0 )
		{
			if (M1_CPU == 1)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//提示余额
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
//    			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				Display_credit = atof(mny)/100;
				sprintf(show_money,"%.2f",Display_credit);
			}
			if (M1_CPU == 2)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//提示余额
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
				//			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				wrLog("余额下限:%.2f元",atof(des_lim));
				Display_credit = atof(mny)/100 - atof(des_lim);
//				wrLog("钱包显示的余额:%.2f",Display_credit);
				sprintf(show_money,"%.2f",Display_credit);
//				wrLog("钱包显示余额:%.2f",atof(show_money));
			}

			wrMonitorLog(12, 0, 0, *pgbl, pack);	//监控日志
			bbzero( temp_buff );
			if (payType == 8)
			{
				sprintf(temp_buff, "您的余额为：%s元\n按【Esc】返回", show_money);
			}
			else
			{
				sprintf(temp_buff, "您的余额为：%s元\n按【确认】充值\n按【返回】取消", show_money);
			}
			TS_OutPutMsg(pgbl->dPosInfo, temp_buff, 0, iSelect);
			if( iSelect==0 )
			{
				NULL;
			}
			else
			{
				if(iRet==0)
				{
					*party = FROMCTSI;
					*rspcode = USERCANCEL;
				}
				else
				{
					*party = FROMCTSI;
					*rspcode = ICTIMEOUT;
				}
				return 5;// 5：扣费前失败
			}
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5：扣费前失败
		}
	}
	else
	{
		NULL;
	}

	if (payType == 8)
	{
		//只查询
		return 0;
	}
	//选金额
	iRet = NormalSelectAmo(pgbl);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		if( iRet==CANCLE_KEY )
		{
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
		}
		return 5;// 5：扣费前失败
	}
	
	
	bbzero( temp_buff );
	if(payType == 1 || payType==7)
		sprintf(temp_buff, "交易前余额：%s元\n充值金额：%.2f元\n交易后金额：%.2f元\n按【确认】充值\n按【返回】取消", 
		show_money, atof(pgbl->amount)/100, atof(pgbl->amount)/100 + atof(show_money));
	else if(payType == 2)
		sprintf(temp_buff, "充值金额：%.2f元\n按【确认】充值\n按【返回】取消", 
		atof(pgbl->amount)/100);

/*
	if (atof(pgbl->amount)/100 + money - 1000.00 >0.001)
	{
		TS_OutPutMsg(pgbl->dPosInfo," \n余额充值后超过余额上限。请按【确认】返回",0,iSelect);
		if (iSelect == 0)
		{
			*party = FROMIC;
			*rspcode = BALANCE_EXCEED;
			return 5;
		}
	}
*/
	iRet = TS_OutPutMsg(pgbl->dPosInfo, temp_buff, 0, iSelect);
	
	if( iSelect==0 )
	{
		NULL;
	}
	else
	{
		if(iRet==0)
		{
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
		}
		return 5;// 5：扣费前失败
	}
	
	//扣费的时候读取一次卡信息。如果出现读卡的羊城通卡号和交易的卡号不一致，则取消交易，三次提示用户继续读卡，三次均不成功，则取消该次交易
	//获取卡片的物理信息,返回卡片的物理信息
	/***************************begin*********************************************/
	for (int i = 0; i<3; i++)
	{
		bbzero(card_physics_info);
		query_iRet=R_PUB_QRY_PHYSICS_INFO(pgbl->dPosInfo,com,comType,(BYTE *)card_physics_info);
		if (query_iRet == 0)
		{
			char SW1SW2[4+1];
			bbzero(SW1SW2);
			CTools::byte_to_hex((BYTE *)card_physics_info+8,(BYTE *)ticket_no,8);//取得逻辑卡号
			CTools::byte_to_hex((BYTE *)card_physics_info+49,(BYTE *)SW1SW2,2);
			
			BYTE temp_SW1SW2[2+1];
			bbzero(temp_SW1SW2);
			memcpy(temp_SW1SW2,card_physics_info+49,2);
			if (memcmp(SW1SW2,"9000",4) != 0)
			{
				int iSelect = 0;
				char errMsg[512];
				char outPut_Msg[512];
				get_cardStatus_errMsg((BYTE *)temp_SW1SW2,errMsg);
				bbzero(outPut_Msg);
				sprintf(outPut_Msg,"请放好卡片，重新读卡,返回响应卡状态码错误：%s",errMsg);
				TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
				
			}
			else
			{
				if (strcmp(yct_data.ticket_no,ticket_no) !=0 ) //判断卡片是否与读卡器之前查询的卡片一致
				{
					TS_OutPutMsg(pgbl->dPosInfo,"卡片与读卡的卡片信息不一致，请重试",1,iSelect);
				}
				
				else
				{
					
					break;
					
				}
				
			}
			
			
			
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo,"读取卡片信息错误，请重新放卡",1,iSelect);

		}
	}
	
	if (query_iRet!=0 )
	{
		TS_OutPutMsg(pgbl->dPosInfo,"读取卡片信息失败，交易失败",1,iSelect);
		*party = FROMCTSI;
		*rspcode = USERCANCEL;
		return 5;
	}
	else
	{
		if (strcmp(yct_data.ticket_no,ticket_no) !=0)
		{
			TS_OutPutMsg(pgbl->dPosInfo,"读取卡片信息失败，交易失败",1,iSelect);
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
			return 5;

		}

	}


	/***************end*of*readcardinfo*******************************************************/


	if(atoi(pgbl->amount)==1000) //扣押金pe不进行扣费
		
	{
		TS_OutPutMsg(pgbl->dPosInfo,"\n本业务仅限于羊城通押金卡，可通过退押金进行充值，无需刷卡\n 按【确认】进入，按【ESC】退出",0,iSelect);
		//记录羊城通交易数据 
		
		CTools::getDate(yct_data.txn_date);
		CTools::getTime(yct_data.txn_time);
		//生成羊城通充值终端号
		CTools::getYctStan(pgbl->stan_no); //获取羊城通系统参考号
		CTools::getYct_posSerial(pgbl->pos_serial);
		
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
		strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
		strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
		strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
		strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
		
		yct_data.status = 6; //默认为充值失败.冲正失败
		CDB::setYctYPData(yct_data); 
	}
    else
	{
		//刷卡输密码PE扣费
		char pos_serial[7+1];
		bbzero(pos_serial);
		trade.set_in_par(*pgbl);
		iRet = trade.YCT_PayPe(yct_data, &m_party, &m_rspcode,pgbl->stan_no,pos_serial);
	
		if( iRet==0 )//成功
		{
			strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
			bbzero(yct_data.txn_date);
			CTools::getDate(yct_data.txn_date);
			bbzero(yct_data.txn_time);
			CTools::getTime(yct_data.txn_time);
			bbzero(yct_data.biz_type);
			strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
			bbzero(yct_data.term_id);
			strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
			bbzero(yct_data.term_serial);
			strncpy(yct_data.term_serial, pos_serial, sizeof(yct_data.term_serial)-1); //此处要求跟扣费时候的终端流水号保持一致
			bbzero(yct_data.bank);
			strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
			bbzero(yct_data.card_no);
			strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
			bbzero(yct_data.amount);
			strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
			bbzero(yct_data.yct_serial);
			bbzero(yct_data.result_message);
			bbzero(yct_data.ticket_amo_aft);
			//在获取充值密钥之前，默认充值失败
			yct_data.status = 4;
			//记录羊城通交易数据
			CDB::setYctYPData(yct_data);
			
			//更新交易总额
			//CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
		}else if (iRet == 2)
		{
			strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
			bbzero(yct_data.txn_date);
			bbzero(yct_data.txn_time);
			CTools::getDate(yct_data.txn_date);
			CTools::getTime(yct_data.txn_time);
			bbzero(yct_data.biz_type);
			strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
			bbzero(yct_data.term_id);
			strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
			bbzero(yct_data.term_serial);
			strncpy(yct_data.term_serial,pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
			bbzero(yct_data.bank);
			strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
			bbzero(yct_data.card_no);
			strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
			bbzero(yct_data.amount);
			strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
			bbzero(yct_data.yct_serial);
			bbzero(yct_data.result_message);
			//记录羊城通交易数据
			yct_data.status = 4;
			CDB::setYctYPData(yct_data);
			//PE确认与冲正
			yct_data.status = trade.YCT_AckRevTrade(yct_data,yct_data.status); //羊城通冲正
			if (yct_data.status==5)
			{
				yct_data.status=trade.YCT_AckRevTrade(yct_data,yct_data.status);
			}
			//更新数据库
			CDB::updateYctYPData(yct_data);
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 4;// 5：扣费前失败
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 5;// 5：扣费前失败
		}

	}
	
	try
	{
		//充值
		if(pgbl->posInfo.rightType == 0x08)
		{
			if(YCT_VER==3)
			iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALPAY, &yct_data, pgbl->amount, &m_party, &m_rspcode);
			else 
			iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALPAY, &yct_data, pgbl->amount, &m_party, &m_rspcode);
		}
		else
		{
			iRet = NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALPAY, &yct_data, pgbl->amount, &m_party, &m_rspcode);
		}
	}
	catch (...)
	{
		wrLog("szCause");
		TS_OutPutMsg(pgbl->dPosInfo, " \n\n\n羊城通充值发生异常，请再次查询余额核对充值是否成功，如有疑问请按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询", 1,iSelect);
	}
	wrYctDataLog("NormalPay", yct_data);
	
	if( iRet==0  || iRet==-1) //-1 是超时情况
	{
		if(atoi(pgbl->amount)==1000)
		{
			
			yct_data.status = 2; //退押金成功，不需要pe 进行确认，羊城通终端交易状态直接修改为2
			CDB::updateYctYPData(yct_data);
			
		}
		else
		{
			yct_data.status = 1;
		}
	

	}
	else if( iRet==1 || iRet==2 || iRet==3 || iRet==4 || iRet==8 )
	{
		if(atoi(pgbl->amount)==1000) //如果是退押金
		{
			
			yct_data.status=6;
		}
		
		else
		{
			yct_data.status = 4;
		}
	}
	else if(iRet==10)//写卡时超时，不明确卡是否充值成功
	{
		if((pgbl->posInfo).rightType == 0x08)
		{
			int i = 0;
			YCT_DATA yct_data_check;
			memset(&yct_data_check, 0, sizeof(yct_data_check));
			iRet = 1;
			while(i<3 && iRet){
				if(YCT_VER==3)
				{
			
					iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data_check, "000000000000", &m_party, &m_rspcode);
				}
				else 
				{
					iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data_check, "000000000000", &m_party, &m_rspcode);
				}
				
				i++;
				Sleep(1000*3);
			}
			if(!iRet)
			{
				if(strcmp(yct_data.ticket_amo_bef, yct_data_check.ticket_amo_bef) && !strcmp(yct_data.ticket_no, yct_data_check.ticket_no))
				{
					if(atoi(pgbl->amount) == (atoi(yct_data_check.ticket_amo_bef) - atoi(yct_data.ticket_amo_bef)))
					{
						memset(yct_data.result_message, 0, 33);
						memcpy(yct_data.result_message, yct_data.pki_card, 8);
						memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
						memcpy(yct_data.result_message+16, "0003000000000000", 16);
						iRet = ST_NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
						
						wrYctDataLog("after NormalNotFinish", yct_data);
						if(iRet==0)//成功
						{

							if(atoi(pgbl->amount)==1000)

								{
								//如果是押金充值，充值成功后不用pe 确认，直接修改交易状态
								//	CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
								yct_data.status = 2;
								CDB::updateYctYPData(yct_data);
								}
							
							else
							{
							//更新交易总额
							CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
							yct_data.status = 1;//充值成功未确认
							}
						}
						else//未知
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//更新数据库
							CDB::updateYctYPData(yct_data);
							return 4;// 4：未知结果
						}
					}
					else if(atoi(yct_data_check.ticket_amo_bef) == atoi(yct_data.ticket_amo_bef))
					{
						memset(yct_data.result_message, 0, 33);
						memcpy(yct_data.result_message, yct_data.pki_card, 8);
						memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
						memcpy(yct_data.result_message+16, "FF00000000000000", 16);
						iRet = ST_NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
						
						wrYctDataLog("after NormalNotFinish", yct_data);
						if(iRet==5)//失败
						{
							if (atoi(pgbl->amount)==1000)
							{
								yct_data.status = 6;//充值失败冲正失败
							}
							else
							{
								
								yct_data.status = 4;//充值失败未冲正
								
							}
							
						}
						else//未知
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//更新数据库
							CDB::updateYctYPData(yct_data);
							return 4;// 4：未知结果
						}
					}
				}
			}
			else
			{
				m_party = FROMCTSI_N;
				m_rspcode = NORESULT;
				yct_data.status = 7;
			}
		}
		else
		{
			yct_data.status = 4;
		}
	}
	else//需要做不完整交易
	{
		if((pgbl->posInfo).rightType == 0x08)
		{

			if(iRet == 11)
			{
				memset(yct_data.result_message, 0, 33);
				memcpy(yct_data.result_message, yct_data.pki_card, 8);
				memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
				memcpy(yct_data.result_message+16, "FF00000000000000", 16);
			}
			else if(strlen(yct_data.result_message) < 3)
			{
				// 				memset(yct_data.result_message, 0, 33);
				// 				memcpy(yct_data.result_message, yct_data.pki_card, 8);
				// 				memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
				// 				memcpy(yct_data.result_message+16, "FF00000000000000", 16);
				m_party = FROMCTSI_N;
				m_rspcode = NORESULT;
				yct_data.status = 7;//无法获知充值结果
				CDB::updateYctYPData(yct_data);
				return 4;
			}
			iRet = ST_NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, yct_data.result_message, &m_party, &m_rspcode);
		}
		else
		{
			iRet = NormalNotFinish(pgbl->dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);	
		}
		
		wrYctDataLog("after NormalNotFinish", yct_data);
		if(iRet==0)//成功
		{
	
			if(atoi(pgbl->amount)==1000)
			{
				//如果是押金充值，不需要进行pe确认，直接修改最后的交易状态
				//CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 2;
				CDB::updateYctYPData(yct_data);
			}
			else
			{
				//更新交易总额
				CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 1;//充值成功未确认	
				
			}		
		}
		else if(iRet==5)//失败
		{
			if (atoi(pgbl->amount)==1000)
			{
				yct_data.status = 6;//充值失败冲正失败
			}
			else
			{
				yct_data.status = 4;//充值失败未冲正
			}
		}
		else//未知
		{
			yct_data.status = 0;
			*party = m_party;
			*rspcode = m_rspcode;
			//更新数据库
			CDB::updateYctYPData(yct_data);
			return 4;// 4：未知结果
		}
	}
	CDB::updateYctYPData(yct_data);
	//PE确认与冲正
	trade.set_in_par(*pgbl);
	yct_data.status = trade.YCT_AckRevTrade(yct_data,yct_data.status);

	if (yct_data.status==5)
	{
		yct_data.status=trade.YCT_AckRevTrade(yct_data,yct_data.status);
	}

	if (yct_data.status == 3)
	{
		*party = FROMPE;
		*rspcode = 99;
		return 1;
	}
	//更新数据库
	CDB::updateYctYPData(yct_data);
	CDB::update_epos_term(pgbl->pos_no, "1");//将终端状态设为"正常"

	if (payType == 7)
	{
		set_yctthird_priCont(0,&yct_data,priCont);
	}
	else
	{
		strcat(priCont, "羊城通卡号：");
		strcat(priCont, yct_data.ticket_no);
		strcat(priCont, "\n羊城通流水号：");
		strcat(priCont, yct_data.yct_serial+2);
		sprintf(priCont+strlen(priCont), "\n原余额: %.2f元", atof(yct_data.ticket_amo_bef)/100);
		sprintf(priCont+strlen(priCont), "\n充值金额: %.2f元", atof(yct_data.amount)/100);
		sprintf(priCont+strlen(priCont), "\n现余额: %.2f元", atof(yct_data.ticket_amo_bef)/100 + atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n充值金额: %.2f元", atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n现余额: %.2f元", atof(yct_data.ticket_amo_aft)/100);
	}
	strcpy(pgbl->yct_disp,priCont);
	
	*party = m_party;
	*rspcode = m_rspcode;
	return 0;
}


// 计算校验码
// 入参：data:数据; dataLen:数据长度
// 出参：无
// 返回值
// 0：一致
// 1：不符
int CYCTYPTrade::isXor(const BYTE *data, int dataLen)
{
	int i;
	BYTE c = data[0];
	for(i=1; i<dataLen-1; i++)	
		c ^= data[i];
	if( c != data[dataLen-1] )
		return -1;
	return 0;
}

// 向读卡器发送和接收数据
// 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型;
// 出参：无
// 返回值
// 0：成功
// 1：失败
int CYCTYPTrade::SndRcvIcData(const TSDLLPosInfo dPosInfo, const int com, const int comType, const char *title, const BYTE *downData, const int downLen, BYTE *upData, int *upLen, int baudRate)
{
	int ret, iSelect;
	TSIcReaderCommand icCmd;
	
	memset(&icCmd, 0, sizeof(icCmd));
	strcpy(icCmd.acTitle, title);
	icCmd.com = com;
	icCmd.comType = comType;
	icCmd.uDownLen = downLen;	//下发数据的长度
	
	memcpy(icCmd.uDownData, downData, downLen);	//下发的数据, 与提示串的总长度不得超过 240 字节
	wrHex("下发读卡器数据", icCmd.uDownData, icCmd.uDownLen);
	ret = TS_IcReaderCommand(dPosInfo, &icCmd, baudRate);  //下发命令
	if( ret || icCmd.uUpLen==0 ) //错误，返回出错信息
	{
		*upLen = 0;
		wrLog("错误返回值: [%d]", ret);
		wrHex("读卡器返回数据", icCmd.uUpData, icCmd.uUpLen);
		if(ret == 7)
			TS_OutPutMsg(dPosInfo, " \n 与读卡器交互出错!\n请连接好读卡器再试!", 0, iSelect);
		else
			TS_OutPutMsg(dPosInfo, " \n  与终端交互出错!\n请连接好读卡器再试!", 0, iSelect);
		return 1;
	}
	
	wrHex("读卡器返回数据", icCmd.uUpData, icCmd.uUpLen);
	
	memcpy(upData, icCmd.uUpData, icCmd.uUpLen);	//上送的数据
	*upLen = icCmd.uUpLen;	//上送数据的长度
	
	if( isXor(upData, *upLen) ) //校验
	{
		wrLog("校验码错误");
		return 1;
	}	
	return 0;
}


// 对数据进行打包
// 入参：tag:报文标识; ver:版本; num:报文顺序号; len:报文长度(cmd,stat,data的长度); cmd:命令标识; stat:状态标识; data:数据;
// 出参：sndData:换包后的数据
// 返回值：包长度
int CYCTYPTrade::packData(int tag, int ver, int num, int len, int cmd, int stat, const BYTE *data, BYTE *sndData)
{
	int sndLen;
	BYTE c;  //校验码
	int dataLen = len - 2;
	
	sndData[0] = tag;
	sndData[1] = ver;
	sndData[2] = num;
	sndData[3] = len;
	sndData[4] = cmd;
	sndData[5] = stat;
	sndLen = 6;
	memcpy(sndData + sndLen, data, dataLen);
	sndLen += dataLen;
	c = sndData[0];
	for(int i=1; i<sndLen; i++)
		c ^= sndData[i];
	sndData[sndLen] = c;
	sndLen += 1;
	return sndLen;
}

// 连接测试
// 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型;
// 出参：无
// 返回值
// 0：成功
// 1：失败
int CYCTYPTrade::connTest(const TSDLLPosInfo dPosInfo, const int com, const int comType)
{
	char sndData[256], rcvData[256], data[2];
	int sndLen, rcvLen;
	int len, num, cmd;
	
	wrLog("【连接测试】");
	num = 0;
	cmd = 0x10;
	len = 2;
	
	bbzero(sndData);
	bbzero(data);
	sndLen = packData(TAG, VER, num, len, cmd, STAT, (BYTE *)data, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if( SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen) )
	{
		wrHex("读卡器返回数据", (BYTE *)rcvData, rcvLen);
		return 1;
	}
	
	return 0;
}

// 把12位金额化为4位hex形式
// 入参：amo:12位的金额
// 出参：hexAmo:4位hex形式的金额
// 返回值：无
void CYCTYPTrade::getHexAmo(const char *amo, BYTE *hexAmo)
{
	unsigned long lamo = atoi(amo);
	hexAmo[0] = (BYTE)(lamo / 0x1000000);
	hexAmo[1] = (BYTE)(lamo % 0x1000000 /0x10000);
	hexAmo[2] = (BYTE)(lamo % 0x10000 /0x100);
	hexAmo[3] = (BYTE)(lamo % 0x100);
}

// num位hex形式化为把12位金额
// 入参：hexAmo:4位hex形式的金额 num:位数
// 出参：amo:12位的金额
// 返回值：无
void CYCTYPTrade::getAsiAmo(const BYTE *hexAmo, int num, char *amo)
{
	unsigned long lamo = 0; 
	for(int i=0; i<num; i++)
	{
		lamo = lamo * 256;
		lamo = lamo + hexAmo[i];

	}
	sprintf(amo, "%012ld", lamo);
}



// num位hex形式化为把12位金额
// 入参：hexAmo:4位hex形式的金额 num:位数
// 出参：amo:12位的金额
// 返回值：无
//其中返回数据低位在先

void CYCTYPTrade::getAsiAmo3(const BYTE *hexAmo, int num, char *amo)
{
	unsigned long lamo = 0; 
	for(int i=num-1; i>=0; i--)
	{
		lamo = lamo * 256;
		lamo = lamo + hexAmo[i];
		
	}
	sprintf(amo, "%012ld", lamo);
	
}

// 与羊城通交互
// 入参：sd:连接端口; sendData:发送数据; sendLen:发送数据长度
// 出参：recvData:接收数据; recvLen:接收数据长度
// 返回值
// 0：成功
// 1：发送失败
// 2：接收失败
// 3：连接失败
int CYCTYPTrade::SndRcvYctData(const SOCKET sd, char *sendData,int sendLen, char *recvData, int *recvLen, int recvMaxLen)
{
    int iRet=0;
	CConnect con;//连接类

	iRet = con.sndMsg(sd, sendData, sendLen);
	if( iRet!=0 )
	{
		return 1;
	}
	iRet = con.rcvMsg(sd, recvData, recvLen, recvMaxLen);
	if( iRet!=0 )
	{
		return 2;
	}
	
	return 0;
}

// 上传数据包给羊城通服务器并接收回应
// 入参：sdType:0:短连接;1:长连接; sd:端口号; num:第num步操作; ndData:发向羊城通的数据; sndLen:发向羊城通的数据长度
// 出参：rcvData:从读卡器接收的数据; rcvLen:从读卡器接收的数据的长度
// 返回值
// 0：成功
// 1：与羊城通交互失败
// 2：与读卡器交互失败
int CYCTYPTrade::commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen)
{
	int iRet=0, len=0, icLen=0, sndICLen=0;
	BYTE icData[256], sndICData[256];
	
	wrHex("向羊城通发送数据", (BYTE*)sndData, sndLen);
	bbzero(icData);
	iRet = SndRcvYctData(sd, (char *)sndData, sndLen, (char *)icData+20, &icLen, 256);//羊城通返回数据包（最大135字节）
	if( iRet!=0 )
	{
		return 1;
	}
	else
	{
		NULL;
	}
	/*if( cmd==NORMAL_PAY_CMD && num==4 || cmd==NORMAL_NOTFINISH_CMD && num==4 )//kftest
	{
	return 1;
}*/
	wrHex("从羊城通接收数据", (BYTE*)icData+20, icLen);
	icLen = icLen + 20;
	len = icLen + 2;
	
	wrLog("【%d: commuWithIC】", num);
	
	bbzero(sndICData);	
	sndICLen = packData(TAG, VER, num*2-1, len, cmd, STAT, icData, sndICData);
	*rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndICData, sndICLen, rcvData, rcvLen);
	if( iRet==0 )
	{
		return 0;// 0：成功
	}
	else
	{
		return 2;// 2：与读卡器交互失败
	}
}

// 普通钱包的查询充值
// 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型; enq:0:要充值; 1:只查询
// 出参：pyct_data:羊城通交易信息; errType:出错类型0:成功1:读卡器返的错误; errCode:出错代码0:成功
// 返回值
// 0：成功(查询或充值)
// 1：与读卡器交互失败
// 2：读卡器返回的错误
// 3：与羊城通交互失败
// 4：羊城通返回的错误
// 5：与羊城通交互失败，需要做不完整交易
// 6：与读卡器交互失败，需要做不完整交易
// 7：没有确认标识，需要做不完整交易
// 8：确认充值失败
// 9：读卡器返回的错误，需要做不完整交易
int CYCTYPTrade::NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256];
	SOCKET sd=0;
	CConnect con;//连接类
	
	wrLog("【NormalEnqPay】");
	
	iRet = connTest(dPosInfo, com, comType);//连接测试，同时也断开上笔交易，初始化读卡器
	if( iRet==0 )//成功
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 1;// 1：与读卡器交互失败
	}
	
	wrLog("【1: commuWithIC】");
	memcpy(pyct_data->amount, amount, 12);
	bbzero(icData);
	getHexAmo(amount, icData);	
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, NORMAL_PAY_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE*)pyct_data->pki_card, 4);//管理卡逻辑卡号
			wrLog("管理卡逻辑卡号pki_card [%s]", pyct_data->pki_card);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	wrLog("【2: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			strcpy(pyct_data->yct_serial, "00");
			CTools::byte_to_hex((BYTE *)rcvData+6, (BYTE *)pyct_data->yct_serial+2, 4);//羊城通交易流水号
			wrLog("羊城通交易流水号yct_serial [%s]", pyct_data->yct_serial);
			CTools::byte_to_hex((BYTE *)rcvData+6+4, (BYTE *)pyct_data->ticket_no, 8);//羊城通票卡卡号
			wrLog("羊城通票卡卡号ticket_no: [%s]", pyct_data->ticket_no);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);			
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
	wrLog("【3: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 3, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			getAsiAmo(rcvData+6, 3, pyct_data->ticket_amo_bef); //取得以分为单位的十进制金额
			wrLog("羊城通票卡余额ticket_amo:[%s]", pyct_data->ticket_amo_bef);//羊城通票卡余额
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			wrLog("【FAIL:4: commuWithYCTIC】");
			rcvLen = 0;
			bbzero(rcvData);
			iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 4, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
			if( iRet==0  )
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				closesocket(sd);			
				return 2;// 2：读卡器返回的错误
			}
			else if( iRet==1  )
			{
				*party = FROMCTSI;
				*rspcode = YCTTIMEOUT;
				closesocket(sd);
				return 3;// 3：与羊城通交互失败
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				closesocket(sd);
				return 1;// 1：与读卡器交互失败
			}
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);			
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
	if(enq==NORMALENQ)//只查询
	{
		*party = FROMSUCC;
		*rspcode = SUCC_NOMSG;
		closesocket(sd);
		return 0;
	}
	else
	{
		NULL;
	}
	
	wrLog("【4: commuWithYCTIC】");
	rcvLen = 0;
	pyct_data->status = 0;
	CDB::updateYctYPData(*pyct_data);
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 4, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			if(  rcvData[5]>=0xC4 && rcvData[5]<=0xC8 )
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				closesocket(sd);
				return 2;// 2：读卡器返回的错误
			}
			else
			{
				*party = FROMIC_N;
				*rspcode = rcvData[5];
				closesocket(sd);
				return 9;// 9：读卡器返回的错误，需要做不完整交易
			}
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 5;// 5：与羊城通交互失败，需要做不完整交易
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 6：与读卡器交互失败，需要做不完整交易
	}
	
	wrLog("【5: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 5, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[12]==0x00 )
		{
			getAsiAmo(rcvData+6+10, 4, pyct_data->ticket_amo_aft); //取得以分为单位的十进制金额
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			closesocket(sd);
			return 0;
		}
		else if( rcvData[12]==0xFF )
		{
			*party = FROMCTSI_N;
			*rspcode = ICNOTSURE;
			closesocket(sd);
			return 7;// 7：没有确认标识，需要做不完整交易
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICSAYFAIL;
			closesocket(sd);
			return 8;// 8：确认充值失败
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 5;// 5：与羊城通交互失败，需要做不完整交易
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 6：与读卡器交互失败，需要做不完整交易
	}
}

//包处理函数:打包、发包、收包和解包
// 入参：flag:2:交费包;4:确认包;5:冲正包
// 出参：无
// 返回值
// 0：成功
// 1：打包失败，扣费失败
// 2：超时，结果未知
// 3：解包失败，结果未知
int CYCTYPTrade::packHdl(CGlobal gbl, CPack *ppack, int flag)
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

// 确认交易
// 入参：yct_data:羊城通交易信息
// 出参：无
// 返回值：
// 2：已确认
// 3：未确认
int CYCTYPTrade::AckTrade(CGlobal gbl, YCT_DATA yct_data)
{
	int b48_len, b63_len, iRet;
	char lenBuff[4+1];
	CPack pack;
	char biz_status[2+1];
	bbzero(biz_status);
	if (yct_data.status==1)
	{
		strcpy(biz_status,"00");
	}
	else
	{
	
		strcpy(biz_status,"01");

	}
	
	for (int i=0;i<3;i++)
	{
		wrLog("【*******PE确认*******】");
		//写确认域
		pack.wrField(4, gbl);
		/************补充48域和63域*********/
		memcpy(pack.FIELD48,gbl.fee_type,4);	//4位信息码
		bbzero(pack.FIELD48);
		b48_len = 4;
		memcpy(pack.FIELD48+b48_len,biz_status,2);
		b48_len+=2;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff,"%02d", 25);	//卡号长度
		memcpy(pack.FIELD48+b48_len, lenBuff, 2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);//羊城通逻辑卡号
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);//羊城PKI卡卡号
		b48_len += 8;
		pack.Packet_Table[48].Real_Length = b48_len;
		
		bbzero(pack.FIELD63);
		memcpy(pack.FIELD63, yct_data.yct_serial+2, 8);
		b63_len = 8;
		memcpy(pack.FIELD63 + b63_len, "0001", 4);
		b63_len += 4;
		memcpy(pack.FIELD63 + b63_len, "0000", 4);
		b63_len += 4;
		memcpy(pack.FIELD63 + b63_len, yct_data.ticket_amo_aft+4, 8);
		b63_len += 8;
		pack.Packet_Table[63].Real_Length = b63_len;
		/***********************************/
		//确认包打包
		iRet = packHdl(gbl, &pack, 4);
		if( iRet==0 && memcmp(pack.FIELD39,"00",2)==0 )
		{
			if (yct_data.status==1)
			{
				return 2;
			}
		
			else
			{
			
				return 5;
			}
		}
		else
		{
			;//确认不成功继续确认，直至三次确认次数满
		}
	}

	if (yct_data.status==1) //成功的确认
	{
		return 3;
	}
	else
		return 5;//失败的确认
	
	
}


// 冲正交易
// 入参：yct_data:羊城通交易信息
// 出参：无
// 返回值：
// 5：已冲正
// 6：未冲正
int CYCTYPTrade::RevTrade(CGlobal gbl, YCT_DATA yct_data)
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
		memcpy(pack.FIELD48,"0056",4);	//4位信息码
		b48_len = 4;
		memcpy(pack.FIELD48+b48_len, gbl.pos_serial, 6);	//交费时6位原终端流水号
		b48_len += 6;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff,"%02d", 25);	//卡号长度
		memcpy(pack.FIELD48+b48_len,lenBuff,2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);//羊城通逻辑卡号
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);//羊城PKI卡卡号
		b48_len += 8;
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
		else if (iRet == 2)
		{
			//超时
			return 6;
		}
	}
	return 6;// 6：未冲正
}

// 冲正交易
// 入参：payType:充值类型
// 出参：pyct_detail:充值明细
// 返回值：
// 0：成功
// 1：无此充值类型
// 2：与终端交互失败
// 0x36 CANCLE_KEY：用户取消
int CYCTYPTrade::MonthSelectWay(const TSDLLPosInfo dPosInfo, int payType, PYCT_DETAIL pyct_detail)
{
	int iRet=0, iSelect=0;
	TSDLLPosInfo m_dPosInfo;
	TSMENULIST menuList;
	TSTradeInfoSend tradeInfoSend;
	TSTradeInfoRecv tradeInfoRecv;
	if( payType==3 )//公交90次本月生效
	{
		pyct_detail->ticket_type = 0x01;
		pyct_detail->birth_date = 0x01;
		pyct_detail->bus_type = 0x01;
		pyct_detail->bus_num = 90;
		return 0;
	}
	else if( payType==4 )//公交90次下月生效
	{
		pyct_detail->ticket_type = 0x01;
		pyct_detail->birth_date = 0x02;
		pyct_detail->bus_type = 0x01;
		pyct_detail->bus_num = 90;
		return 0;
	}
	else if( payType==5 )//公交定线
	{
		//输入金额
		memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
		memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
		
		strcpy(tradeInfoSend.acMainTitle, "公交定线");
		tradeInfoSend.uHasTradeID = 1;
		tradeInfoSend.uTotalContents = 2;
		
		strcpy(tradeInfoSend.Contents[0].acTitle, "请输入第1条公交线路");
		tradeInfoSend.Contents[0].uCommand = TS_INPUT_ALL_CHAR;//公交线路
		tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME;//超时30秒
		tradeInfoSend.Contents[0].uCount = 0;//输入1次
		tradeInfoSend.Contents[0].uMinLen = 1;
		tradeInfoSend.Contents[0].uMaxLen = 5;
		
		strcpy(tradeInfoSend.Contents[1].acTitle, "请输入第2条公交线路");
		tradeInfoSend.Contents[1].uCommand = TS_INPUT_ALL_CHAR;//公交线路
		tradeInfoSend.Contents[1].uTimeOut = EPOS_OVER_TIME;//超时30秒
		tradeInfoSend.Contents[1].uCount = 0;//输入1次
		tradeInfoSend.Contents[1].uMinLen = 1;
		tradeInfoSend.Contents[1].uMaxLen = 5;
		
		memcpy(&m_dPosInfo, &dPosInfo, sizeof(m_dPosInfo));
		iRet = TS_TradeControl(&m_dPosInfo, &tradeInfoSend, &tradeInfoRecv);		
		if( iRet!=0 || tradeInfoRecv.Contents[0].uMsgLen>5 || tradeInfoRecv.Contents[1].uMsgLen>5 )
		{			
			return 2;// 2：与终端交互失败
		}
		else {
			NULL;
		}
		
		pyct_detail->ticket_type = 0x01;
		pyct_detail->bus_type = 0x02;
		memset(pyct_detail->bus_way1, 0x30, 5-tradeInfoRecv.Contents[0].uMsgLen);
		memcpy(pyct_detail->bus_way1+5-tradeInfoRecv.Contents[0].uMsgLen, tradeInfoRecv.Contents[0].uMsg, tradeInfoRecv.Contents[0].uMsgLen);
		memset(pyct_detail->bus_way2, 0x30, 5-tradeInfoRecv.Contents[1].uMsgLen);
		memcpy(pyct_detail->bus_way2+5-tradeInfoRecv.Contents[1].uMsgLen, tradeInfoRecv.Contents[1].uMsg, tradeInfoRecv.Contents[1].uMsgLen);
		
		
		memset(&menuList,0,sizeof(menuList));
		strcpy(menuList.acTitle, "公交定线");
		
		menuList.iTotalNum = 2;
		strcpy(menuList.menuItem[0], "本月生效");
		strcpy(menuList.menuItem[1], "下月生效");		
		iRet = TS_Menu(dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return 2;// 2：与终端交互失败
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			pyct_detail->birth_date = 0x01;
			break;
		case 2:
			pyct_detail->birth_date = 0x02;
			break;
		default:
			return CANCLE_KEY;// 0x36 用户取消
		}
		return 0;
	}
	else if( payType==6 )//地铁限次
	{
		pyct_detail->ticket_type = 0x02;
		pyct_detail->railway_type = 0x01;
		
		memset(&menuList,0,sizeof(menuList));
		strcpy(menuList.acTitle, "地铁限次");
		
		menuList.iTotalNum = 3;
		strcpy(menuList.menuItem[0], "20次");
		strcpy(menuList.menuItem[1], "35次");
		strcpy(menuList.menuItem[2], "50次");
		iRet = TS_Menu(dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return 2;// 2：与终端交互失败
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			pyct_detail->railway_num = 20;
			break;
		case 2:
			pyct_detail->railway_num = 35;
			break;
		case 3:
			pyct_detail->railway_num = 50;
			break;
		default:
			return CANCLE_KEY;// 0x36 用户取消
		}
		
		memset(&menuList,0,sizeof(menuList));
		strcpy(menuList.acTitle, "地铁限次");
		
		menuList.iTotalNum = 2;
		strcpy(menuList.menuItem[0], "本月生效");
		strcpy(menuList.menuItem[1], "下月生效");		
		iRet = TS_Menu(dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return 2;// 2：与终端交互失败
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			pyct_detail->birth_date = 0x01;
			break;
		case 2:
			pyct_detail->birth_date = 0x02;
			break;
		default:
			return CANCLE_KEY;// 0x36 用户取消
		}
		return 0;
	}
	else
	{
		return 1;
	}	
}

// 月票钱包的主函数
// 入参：无
// 出参：无
// 返回值：
// 0：成功已确认
// 1：成功未确认
// 2：失败已冲正
// 3：失败未冲正
// 4：未知结果
// 5：扣费前失败
int CYCTYPTrade::MonthTrade(CGlobal *pgbl, int payType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont)
{
	int iRet=0, iSelect=0, com=0, comType=0;
	YCT_DATA yct_data;
	YCT_DETAIL yct_detail;	
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	char m_priCont[1024];
	bbzero(m_priCont);
	
	iRet = CTrade::getComPar(pgbl->posInfo.leftType, pgbl->posInfo.rightType, pgbl->posInfo.inType, &com, &comType);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = COMNOTFOUND;
		return 5;// 5：扣费前失败
	}
	
	memset(&yct_data, 0, sizeof(yct_data));
	memset(&yct_detail, 0, sizeof(yct_detail));
	
	//选套餐
	iRet = MonthSelectWay(pgbl->dPosInfo, payType, &yct_detail);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		if( iRet==CANCLE_KEY )
		{
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
		}
		return 5;// 5：扣费前失败
	}
	
	//查询月票卡当前信息	
	iRet = MonthEnq(pgbl->dPosInfo, com, comType, yct_detail.ticket_type, yct_data.yct_serial, pgbl->amount, pki_card, signin_serial, &m_party, &m_rspcode);
	if( iRet==0 )//查询成功
	{
		NULL;
	}
	else
	{
		*party = m_party;
		*rspcode = m_rspcode;
		return 5;// 5：扣费前失败
	}
	
	//确认路线
	iRet = MonthPay1_2(pgbl->dPosInfo, com, comType, yct_detail, yct_data.yct_serial, pgbl->amount, pki_card, signin_serial, &m_party, &m_rspcode, m_priCont);
	if( iRet==0 )//确认充值
	{
		//刷卡输密码PE扣费
		iRet = PayPe(pgbl, yct_data, &m_party, &m_rspcode);
		if( iRet==0 )//成功
		{
			strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
			CTools::getDate(yct_data.txn_date);
			CTools::getTime(yct_data.txn_time);
			strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
			strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
			strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
			strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
			strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
			strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
			strncpy(yct_data.pki_card, pki_card, sizeof(yct_data.pki_card));
			wrYctDataLog("after Month PayPe", yct_data);
			//记录羊城通交易数据
			CDB::setYctYPData(yct_data);
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5：扣费前失败
		}
		
		//充值
		iRet = MonthPay3_4(pgbl->dPosInfo, com, comType, &yct_data, &m_party, &m_rspcode);
		wrYctDataLog("after MonthPay3_4", yct_data);
		
		strcat(priCont, "羊城通卡号：");
		strcat(priCont, yct_data.ticket_no);
		strcat(priCont, "\n");
		strcat(priCont, m_priCont);
		
		if( iRet==0 )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			yct_data.status = 1;//充值成功未确认
		}
		else if( iRet==1 || iRet==2 || iRet==3 || iRet==7 )
		{
			*party = m_party;
			*rspcode = m_rspcode;
			yct_data.status = 4;//充值失败未冲正
		}
		else//需要做不完整交易		
		{
			iRet = MonthNotFinish(pgbl->dPosInfo, com, comType, signin_serial, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);
			wrYctDataLog("after MonthNotFinish", yct_data);
			if(iRet==0)//成功
			{
				*party = FROMSUCC;
				*rspcode = SUCC_MSG;
				yct_data.status = 1;//充值成功未确认
				
			}
			else if(iRet==5)//失败
			{
				*party = m_party;
				*rspcode = m_rspcode;
				yct_data.status = 4;//充值失败未冲正
			}
			else//未知
			{
				yct_data.status = 0;//已扣费结果未知
				*party = FROMCTSI_N;
				*rspcode = PAYBUTNOTFINISH;
				//更新数据库
				CDB::updateYctYPData(yct_data);	
				return 4;// 4：未知结果
			}
		}
		//PE确认与冲正
		yct_data.status = AckRevTrade(*pgbl, yct_data);
		//更新数据库
		CDB::updateYctYPData(yct_data);		
		return 0;
	}
	else
	{
		*party = m_party;
		*rspcode = m_rspcode;
		return 5;// 5：扣费前失败
	}
}

// 月票钱包的签到
// 入参：无
// 出参：pki_card(8):PKI卡卡号；signin_serial(16):握手流水号
// 返回值
// 0：成功
// 1：与读卡器交互失败，签到失败
// 2：读卡器返回的错误，签到失败
// 3：与羊城通交互失败，签到失败
// 4：最终签到失败
int CYCTYPTrade::MonthSignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	SOCKET sd=0;
	CConnect con;//连接类
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1], signin_serial_this[16+1];
	
	wrLog("【MonthSignIn】");
	
	iRet = connTest(dPosInfo, com, comType);//连接测试，同时也断开上笔交易，初始化读卡器
	if( iRet==0 )//成功
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	wrLog("【1: commuWithIC】");
	bbzero(icData);
	icLen = 0;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, MONTH_SIGNIN_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			bbzero(pki_card_this);
			CTools::byte_to_hex(rcvData+6, (BYTE*)pki_card_this, 4);//管理卡逻辑卡号
			wrLog("新管理卡逻辑卡号pki_card_this [%s] 旧管理卡逻辑卡号pki_card [%s]", pki_card_this, pki_card);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败，结果未知
	}
	
	wrLog("【2: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, MONTH_SIGNIN_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			bbzero(signin_serial_this);
			CTools::byte_to_hex(rcvData+6, signin_serial_this, 8);
			wrLog("新握手流水 [%s] 旧握手流水pki_card [%s]", signin_serial_this, signin_serial);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败，结果未知
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	wrLog("【3: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 3, MONTH_SIGNIN_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			if( rcvData[6]==0x00 )
			{				
				memcpy(pki_card, pki_card_this, 8);
				memcpy(signin_serial, signin_serial_this, 16);
				CDB::setYctYPSignIn((const char *)dPosInfo.pos_no, pki_card, signin_serial);
				*party = FROMSUCC;
				*rspcode = SUCC_NOMSG;
				closesocket(sd);
				return 0;// 0：成功
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = SIGNINFAIL;
				closesocket(sd);
				return 4;// 4：最终签到失败
			}
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误，结果未知
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败，结果未知
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败，结果未知
	}
}

// 月票查询
// 入参：pki_card:PKI卡卡号; signin_serial(16):握手流水号
// 出参：pki_card:PKI卡卡号; signin_serial(16):握手流水号, 
// 返回值
// 0：成功
// 1：与读卡器交互失败
// 2：读卡器返回的错误
// 3：与羊城通交互失败
// 4：羊城通返回的错误
// 5：签到失败
// 6：用户取消
int CYCTYPTrade::MonthEnq(const TSDLLPosInfo dPosInfo, const int com, const int comType, char ticket_type, char *yct_serial, char *amount, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, i=0, iSelect=0, msgLen=0;
	SOCKET sd=0;
	CConnect con;//连接类
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	char msg[1024];
	bbzero(msg);
	
	wrLog("【MonthEnq】");
	
	if( memcmp(signin_serial, "0000000000000000", 16)==0 )
	{
		iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
		if( iRet==0 )//成功
		{
			NULL;
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5：签到失败
		}
	}
	else
	{
		NULL;
	}
	
	for(i=0; i<2; i++)//循环2次
	{
		iRet = connTest(dPosInfo, com, comType);//连接测试，同时也断开上笔交易，初始化读卡器
		if( iRet==0 )//成功
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 1;// 1：与读卡器交互失败，结果未知
		}
		
		wrLog("【1: commuWithIC】");
		icLen = 0;
		bbzero(icData);
		icData[icLen] = ticket_type;
		icLen = icLen + 1;
		CTools::hex_to_byte((BYTE*)signin_serial, (BYTE*)icData+icLen, 16);
		icLen = icLen + 8;
		len = icLen + 2;
		
		bbzero(sndData);	
		sndLen = packData(TAG, VER, 1, len, MONTH_ENQ_CMD, STAT, icData, sndData);
		rcvLen = 0;
		bbzero(rcvData);
		iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
		if( iRet==0 )
		{
			if( rcvData[4]==0x00 && rcvData[5]==0x00 )
			{
				CTools::byte_to_hex(rcvData+6+8, (BYTE*)pki_card_this, 4);//管理卡逻辑卡号
				wrLog("管理卡逻辑卡号pki_card_this [%s]", pki_card_this);
				if( memcmp(pki_card_this, pki_card, 8)==0 )
				{
					sndLen = rcvLen-6-20-1;
					memcpy(sndData, rcvData+6+20, sndLen);
					break;
				}
				else
				{
					iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
					if( iRet==0 )//成功
					{
						continue;
					}
					else
					{
						*party = m_party;
						*rspcode = m_rspcode;
						return 5;// 5：签到失败
					}
				}			
			}
			else if( rcvData[4]==0xA1 && rcvData[5]==0x36 )
			{
				iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
				if( iRet==0 )//成功
				{
					continue;
				}
				else
				{
					*party = m_party;
					*rspcode = m_rspcode;
					return 5;// 5：签到失败
				}
			}
			else
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				return 2;// 2：读卡器返回的错误
			}
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			return 1;// 1：与读卡器交互失败
		}
	}
	if( i>=2 )
	{
		*party = FROMCTSI;
		*rspcode = FATALERR;
		return 1;// 1：与读卡器交互失败
	}
	else
	{
		NULL;
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	wrLog("【2: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, MONTH_ENQ_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
			if( rcvData[6+11]==0x06 )
			{
				strcat(msg, "月票当前信息：成人卡\n卡号：");
			}
			else
			{
				strcat(msg, "月票当前信息：学生卡\n卡号：");
			}
			CTools::byte_to_hex(rcvData+6+12, (BYTE *)msg+strlen(msg), 8);
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;//羊城通返回的错误
			*rspcode = rcvData[6];
			closesocket(sd);
			return 4;// 4：羊城通返回的错误
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 7;// 7：连接羊城通服务器失败
	}
	
	wrLog("【3: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 3, MONTH_ENQ_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			strcat(msg, "\n");
			memcpy(msg+strlen(msg), rcvData+6, rcvLen-6-1);
			msgLen = strlen(msg);
			if( msg[msgLen-2]=='\n' && msg[msgLen-1]=='\n' )
			{
				NULL;				
			}
			else if( msg[msgLen-1]=='\n' )
			{
				strcat(msg, "\n");
			}
			else
			{
				strcat(msg, "\n\n");
			}
			strcat(msg, "【确认】继续\n【返回】退出");
			iRet = TS_OutPutMsg(dPosInfo, msg, 0, iSelect);
			if( iRet==0 && iSelect==0 )
			{
				*party = FROMSUCC;
				*rspcode = SUCC_NOMSG;
				closesocket(sd);
				return 0;// 0：成功
			}
			else if( iRet==0 )
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				closesocket(sd);
				return 6;// 6：用户取消
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				closesocket(sd);
				return 1;// 1：与读卡器交互失败
			}
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;//羊城通返回的错误
			*rspcode = rcvData[6];
			closesocket(sd);
			return 4;// 4：羊城通返回的错误
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
}

// 月票充值的前1，2步
// 入参：pki_card:PKI卡卡号; signin_serial(16):握手流水号
// 出参：pyct_data:羊城通交易信息; pki_card:PKI卡卡号; signin_serial(16):握手流水号
// 返回值
// 0：成功
// 1：与读卡器交互失败
// 2：读卡器返回的错误
// 3：与羊城通交互失败
// 4：羊城通返回的错误
// 5：签到失败
// 6：用户取消
int CYCTYPTrade::MonthPay1_2(const TSDLLPosInfo dPosInfo, const int com, const int comType, YCT_DETAIL yct_detail, char *yct_serial, char *amount, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, i=0, iSelect=0;
	SOCKET sd=0;
	CConnect con;//连接类
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	char date6[6+1], date10[10+1], numBuf[8+1];
	char msg[1024];
	bbzero(msg);
	
	wrLog("【MonthPay1_2】");
	
	if( memcmp(signin_serial, "0000000000000000", 16)==0 )
	{
		iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
		if( iRet==0 )//成功
		{
			NULL;
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5：签到失败
		}
	}
	else
	{
		NULL;
	}
	
	for(i=0; i<2; i++)//循环2次
	{
		iRet = connTest(dPosInfo, com, comType);//连接测试，同时也断开上笔交易，初始化读卡器
		if( iRet==0 )//成功
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 1;// 1：与读卡器交互失败，结果未知
		}
		
		wrLog("【1: commuWithIC】");
		icLen = 0;
		bbzero(icData);
		memcpy(icData, &yct_detail, sizeof(yct_detail));
		icLen = icLen + sizeof(yct_detail);
		CTools::hex_to_byte((BYTE*)signin_serial, (BYTE*)icData+icLen, 16);
		icLen = icLen + 8;
		len = icLen + 2;
		
		bbzero(sndData);	
		sndLen = packData(TAG, VER, 1, len, MONTH_PAY_CMD, STAT, icData, sndData);
		rcvLen = 0;
		bbzero(rcvData);
		iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
		if( iRet==0 )
		{
			if( rcvData[4]==0x00 && rcvData[5]==0x00 )
			{
				CTools::byte_to_hex(rcvData+6+8, (BYTE*)pki_card_this, 4);//管理卡逻辑卡号
			//	wrLog("管理卡逻辑卡号pki_card_this [%s]", pki_card_this);
				if( memcmp(pki_card_this, pki_card, 8)==0 )
				{
					sndLen = rcvLen-6-20-1;
					memcpy(sndData, rcvData+6+20, sndLen);
					break;
				}
				else
				{
					iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
					if( iRet==0 )//成功
					{
						continue;
					}
					else
					{
						*party = m_party;
						*rspcode = m_rspcode;
						return 5;// 5：签到失败
					}
				}			
			}
			else if( rcvData[4]==0xA1 && rcvData[5]==0x36 )
			{
				iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
				if( iRet==0 )//成功
				{
					continue;
				}
				else
				{
					*party = m_party;
					*rspcode = m_rspcode;
					return 5;// 5：签到失败
				}
			}
			else
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				return 2;// 2：读卡器返回的错误
			}
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			return 1;// 1：与读卡器交互失败
		}
	}
	if( i>=2 )
	{
		*party = FROMCTSI;
		*rspcode = FATALERR;
		return 1;// 1：与读卡器交互失败
	}
	else
	{
		NULL;
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	wrLog("【2: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, MONTH_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE *)yct_serial, 5);
			
			if( rcvData[6+11]==0x06 )
			{
				strcat(msg, "本次充值信息：成人卡");
				strcat(priCont, "卡类型成人卡");
			}
			else
			{
				strcat(msg, "本次充值信息：学生卡");
				strcat(priCont, "卡类型学生卡");
			}
			strcat(msg, "\n起始日期: ");
			strcat(priCont, " 起始日期:");
			
			bbzero(date6);
			bbzero(date10);
			CTools::byte_to_hex(rcvData+6+12, (BYTE *)date6, 3);
			strcat(date10, "20");
			memcpy(date10+strlen(date10), date6, 2);
			strcat(date10, "-");
			memcpy(date10+strlen(date10), date6+2, 2);
			strcat(date10, "-");
			memcpy(date10+strlen(date10), date6+4, 2);
			
			strcat(msg, date10);
			strcat(priCont, date10);
			
			sprintf(amount, "%010d00", rcvData[21]*256+rcvData[22]);
			sprintf(msg+strlen(msg), "\n充值金额: %d元", atoi(amount)/100);
			sprintf(priCont+strlen(priCont), "\n充值金额: %d元", atoi(amount)/100);
			
			bbzero(numBuf);
			CTools::byte_to_hex(rcvData+23, (BYTE *)numBuf, 4);
			sprintf(msg+strlen(msg), "\n次数: %d次", atoi(numBuf));
			sprintf(priCont+strlen(priCont), " 次数: %d次", atoi(numBuf));
			
			strcat(msg, "\n路线名称1: ");
			strcat(priCont, "\n路线名称1: ");
			strcat(msg, (char *)rcvData+6+21);
			strcat(priCont, (char *)rcvData+6+21);
			strcat(msg, "\n路线名称2: ");
			strcat(priCont, "\n路线名称2: ");
			strcat(msg, (char *)rcvData+6+41);
			strcat(priCont, (char *)rcvData+6+41);
			
			strcat(msg, "\n\n【确认】充值\n【返回】退出");
			
			iRet = TS_OutPutMsg(dPosInfo, msg, 0, iSelect);
			if( iRet==0 && iSelect==0 )
			{
				*party = FROMSUCC;
				*rspcode = SUCC_NOMSG;
				closesocket(sd);
				return 0;// 0：成功
			}
			else if( iRet==0 )
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				closesocket(sd);
				
				wrLog("【3: cancel commuWithIC】");
				bbzero(icData);
				icData[0] = 0x02;
				icLen = 20;
				len = icLen + 2;
				
				bbzero(sndData);
				sndLen = packData(TAG, VER, 3*2-1, len, MONTH_PAY_CMD, STAT, icData, sndData);
				rcvLen = 0;
				bbzero(rcvData);
				iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
				
				return 6;// 6：用户取消
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				closesocket(sd);
				return 1;// 1：与读卡器交互失败
			}
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;//羊城通返回的错误
			*rspcode = rcvData[6];
			closesocket(sd);
			return 4;// 4：羊城通返回的错误
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
}

// 月票充值的前3，4步
// 入参：无
// 出参：pyct_data:羊城通交易信息
// 返回值
// 0：成功
// 1：与读卡器交互失败
// 2：读卡器返回的错误
// 3：羊城通返回的错误
// 4：与羊城通交互失败，需要做恢复
// 5：与读卡器交互失败，需要做恢复
// 6：读卡器返回的错误，需要做恢复
// 7：连接羊城通服务器失败
int CYCTYPTrade::MonthPay3_4(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, i=0, iSelect=0;
	SOCKET sd=0;
	CConnect con;//连接类
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256];
	
	wrLog("【MonthPay3_4】");
	
	wrLog("【3: commuWithIC】");
	bbzero(icData);
	icData[0] = 0x01;
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);
	sndLen = packData(TAG, VER, 3*2-1, len, MONTH_PAY_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE*)pyct_data->ticket_no, 8);//羊城通票卡卡号
			getAsiAmo(rcvData+6+8, 4, pyct_data->ticket_amo_bef);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;//2：读卡器返回的错误
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 7;// 7：连接羊城通服务器失败
	}
	
	wrLog("【4: commuWithYCTIC】");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 4, MONTH_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			getAsiAmo(rcvData+6+6, 4, pyct_data->ticket_amo_aft);
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			closesocket(sd);
			return 0;// 0：成功
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;
			*rspcode = rcvData[6];
			closesocket(sd);
			return 3;// 3：羊城通返回的错误
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 6;// 6：读卡器返回的错误，需要做恢复
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 4;// 4：与羊城通交互失败，需要做恢复
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 5;// 5：与读卡器交互失败，需要做恢复
	}
}

// 刷卡输密码向PE发扣费包
// 入参：biz_type(4):业务代码；amount(12):金额
// 出参：无
// 返回值
// 0：扣费成功
// 1：扣费失败
// 2：超时返回
int CYCTYPTrade::PayPe(CGlobal *pgbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode)
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
			if(iRet == CANCLE_KEY)
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				return 1;
			}
			else if(iRet != 0)
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				return 1;
			}
		}
		trade.get_gbl(*pgbl);	//取得在trade类中获得的信息
		pack.wrField(2, *pgbl);	//写交费域
		/***********修改一些域信息*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,pgbl->fee_type,4);	//4位信息码
		b48_len = 4;
		memset(len_buff,0,sizeof(len_buff));
		sprintf(len_buff,"%02d", 25);	//卡号长度
		memcpy(pack.FIELD48+b48_len,len_buff,2);	
		b48_len +=2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);	//羊城通逻辑卡号
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);	//羊城通物理卡号
		b48_len += 8;
		pack.Packet_Table[48].Real_Length=b48_len;
		/**********************************/
		
		//交费包处理
		iRet = packHdl(*pgbl, &pack, 2);
		if(iRet==0)
		{
			NULL;
		}
		else if(iRet==1)
		{
			*party = FROMCTSI;
			*rspcode = PACKFAIL;
			return iRet;
		}
		else if(iRet==2)
		{
			*party = FROMCTSI_N;
			*rspcode = PAYTIMEOUT;
			return -1;
		}
		else if(iRet==3)
		{
			*party = FROMCTSI_N;
			*rspcode = UNPACKFAIL;
			return iRet;
		}
		
		if (!memcmp(pack.FIELD39,"00",2)) 
		{
			bbzero(pgbl->stan_no);
			memcpy(pgbl->stan_no,(char *)pack.FIELD37,12);
			*party = FROMCTSI;
			*rspcode = SUCC_MSG;
			return 0;
		}
		else if(!memcmp(pack.FIELD39,"55",2))
		{
			flag = 1;
			if(trade.reGetPwd( "羊城通充值" ) == 0) //密码错误，重新输入密码
			{
				continue;
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				return CANCLE_KEY;
			}
		}
		else if(!memcmp(pack.FIELD39,"14",2)||!memcmp(pack.FIELD39,"15",2)||!memcmp(pack.FIELD39,"33",2)||!memcmp(pack.FIELD39,"35",2)||!memcmp(pack.FIELD39,"36",2)||!memcmp(pack.FIELD39,"56",2)||!memcmp(pack.FIELD39,"57",2)||!memcmp(pack.FIELD39,"60",2)||!memcmp(pack.FIELD39,"62",2)||!memcmp(pack.FIELD39,"91",2))
		{
			TS_OutPutMsg(pgbl->dPosInfo,"\n     不支持的卡\n   请换卡重新刷卡", 0, iSelect);
			if (iSelect!=0)
			{
				*party = FROMCTSI;
				*rspcode = CARDNOTFIX;
				return -1;
			}
			else
			{
				continue;
			}
		}
		*party = FROMPE;
		*rspcode = atoi((char *)pack.FIELD39);
		return -1;
	}// end for(int maxTry=0; maxTry<50; maxTry++)
	return -1;
}


//改造函数
// 刷卡输密码向PE发扣费包
// 入参：biz_type(4):业务代码；amount(12):金额
// 出参：无
// 返回值
// 0：扣费成功
// 1：扣费失败
// 2：超时返回
int CYCTYPTrade::PayPe(CGlobal gbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode)
{
	int  iRet, flag,mngCard_count=1,iSelect;
	int b48_len = 0;
	char len_buff[10];
	CPack pack;
	CTrade trade;
	
	wrLog("【*******PE扣费*******】");	
	
	//传递信息,取得刷卡数据
	flag = 0;	//重输密码标识
	/********************请求刷卡****************/
	mngCard_count=1;
	trade.set_in_par(gbl);
	for(int maxTry=0; maxTry<50; maxTry++)	//密码错误可重输
	{
		if(!flag)
		{
			iRet = trade.mngCard_Hdl("羊城通充值");
			if(iRet == CANCLE_KEY)
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				return 1;
			}
			else if(iRet != 0)
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				return 1;
			}
		}

		pack.wrField(2, gbl);	//写交费域
		/***********修改一些域信息*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,gbl.fee_type,4);	//4位信息码
		b48_len = 4;
		memset(len_buff,0,sizeof(len_buff));
		sprintf(len_buff,"%02d", 25);	//卡号长度
		memcpy(pack.FIELD48+b48_len,len_buff,2);	
		b48_len +=2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);	//羊城通逻辑卡号
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);	//羊城通物理卡号
		b48_len += 8;
		pack.Packet_Table[48].Real_Length=b48_len;
		/**********************************/
		
		//交费包处理
		iRet = packHdl(gbl, &pack, 2);
		if(iRet==0)
		{
			NULL;
		}
		else if(iRet==1)
		{
			*party = FROMCTSI;
			*rspcode = PACKFAIL;
			return iRet;
		}
		else if(iRet==2)
		{
			*party = FROMCTSI_N;
			*rspcode = PAYTIMEOUT;
			return -1;
		}
		else if(iRet==3)
		{
			*party = FROMCTSI_N;
			*rspcode = UNPACKFAIL;
			return iRet;
		}
		
		if (!memcmp(pack.FIELD39,"00",2)) 
		{
			bbzero(gbl.stan_no);
			memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
			*party = FROMCTSI;
			*rspcode = SUCC_MSG;
			return 0;
		}
		else if(!memcmp(pack.FIELD39,"55",2))
		{
			flag = 1;
			if(trade.reGetPwd( "羊城通充值" ) == 0) //密码错误，重新输入密码
			{
				continue;
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				return CANCLE_KEY;
			}
		}
		else if(!memcmp(pack.FIELD39,"14",2)||!memcmp(pack.FIELD39,"15",2)||!memcmp(pack.FIELD39,"33",2)||!memcmp(pack.FIELD39,"35",2)||!memcmp(pack.FIELD39,"36",2)||!memcmp(pack.FIELD39,"56",2)||!memcmp(pack.FIELD39,"57",2)||!memcmp(pack.FIELD39,"60",2)||!memcmp(pack.FIELD39,"62",2)||!memcmp(pack.FIELD39,"91",2))
		{
			TS_OutPutMsg(gbl.dPosInfo,"\n     不支持的卡\n   请换卡重新刷卡", 0, iSelect);
			if (iSelect!=0)
			{
				*party = FROMCTSI;
				*rspcode = CARDNOTFIX;
				return -1;
			}
			else
			{
				continue;
			}
		}
		*party = FROMPE;
		*rspcode = atoi((char *)pack.FIELD39);
		return -1;
	}// end for(int maxTry=0; maxTry<50; maxTry++)
	return -1;
}


//设置pin
int CYCTYPTrade::yctSetting(char *pinValue)
{
	char sndData[256], rcvData[256];
	int iSelect;
	int sndLen, rcvLen;
	int stat, len, num, cmd;
	
	wrLog("【设置pin】");
	num = 0;
	cmd = 0x11;
	stat = 0x00;
	len = 33;
	bbzero(sndData);
	sndLen = packData(TAG, VER, num, len, cmd, stat, (BYTE *)pinValue, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	SndRcvIcData(gbl.dPosInfo,m_com,m_comType,"", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen);
	if (rcvData[4] == 0x00 && rcvData[5] == 0x00)
	{
		TS_OutPutMsg(gbl.dPosInfo, " \n\n     设置成功!", 1, iSelect);
	}
	else
	{
		TS_OutPutMsg(gbl.dPosInfo, " \n\n     设置出错!", 1, iSelect);
		return -1;
	}	
	return 0;
}

void CYCTYPTrade::wrYctDataLog(char *msg, YCT_DATA yct_data)
{
	char ticketno[16+1];
	bbzero(ticketno);
	CTools::shildWords(yct_data.ticket_no,7,5,ticketno);

	wrLog("【YctData】【%s】", msg);
	wrLog("stan           [%s]", yct_data.stan          );
	wrLog("txn_date       [%s]", yct_data.txn_date      );
	wrLog("txn_time       [%s]", yct_data.txn_time      );
	wrLog("biz_type       [%s]", yct_data.biz_type      );
	wrLog("status         [%d]", yct_data.status        );
	wrLog("term_id        [%s]", yct_data.term_id       );
	wrLog("term_serial    [%s]", yct_data.term_serial   );
	wrLog("bank           [%s]", yct_data.bank          );
	wrLog("card_no        [%s]", yct_data.card_no       );
	wrLog("amount         [%s]", yct_data.amount        );
	wrLog("yct_serial     [%s]", yct_data.yct_serial    );
	wrLog("pki_card       [%s]", yct_data.pki_card      );
	wrLog("ticket_no      [%s]", ticketno     );
	wrLog("ticket_amo_bef [%s]", yct_data.ticket_amo_bef);
	wrLog("ticket_amo_aft [%s]", yct_data.ticket_amo_aft);
	wrLog("result_message [%s]", yct_data.result_message);
}

int CYCTYPTrade::showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode)
{
	int iRet = 0;
	if((gbl.posInfo).rightType == 0x08)
		iRet = ST_showErrMsg(gbl, party, rspcode);
	else
		iRet = MW_showErrMsg(gbl, party, rspcode);
	return iRet;
}


int CYCTYPTrade::MW_showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode)
{
	int iSelect=0;
	CTrade trade;
	char response_code[2+1], msg[1024];
	bbzero(response_code);
	bbzero(msg);	
	if(party==FROMSUCC)
	{
		switch(rspcode)
		{
		case SUCC_MSG:
			TS_OutPutMsg(gbl.dPosInfo, "羊城通充值成功\n请按【确认】打印凭证", 0, iSelect);
			break;
		case SUCC_NOMSG:
			break;
		case SUCC_MSG_LAST:
			TS_OutPutMsg(gbl.dPosInfo, "上一笔羊城通充值成功", 0, iSelect);
			break;
		default:
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888!\n", party, rspcode);
			TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
			break;
		}
	}
	else if( party==FROMCTSI || party==FROMCTSI_N )
	{
		switch(rspcode)
		{
		case 0x01:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统未能发现有读卡器的配置：您尚未开通此项业务，可直接输入[861]申请开通。\n");
			break;
		case 0x02:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "羊城通充值已取消\n");
			return 0;
			break;
		case 0x03:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "扣费超时\n");
			break;
		case 0x04:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "向读卡器发送接收数据超时\n");
			break;
		case 0x05:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统发现有读卡器有误：请您确定读卡器是否正确连接，然后选择[返回]后重新操作。\n");
			break;
		case 0x06:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "向羊城通发送接收数据超时\n");
			break;
		case 0x07:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "读卡器不能确定交易状态\n");
			break;
		case 0x08:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "读卡器指明交易失败\n");
			break;
		case 0x09:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统包打包出错\n");
			break;
		case 0x0A:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统包解包出错\n");
			break;
		case 0x0B:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不支持的卡\n");
			break;
		case 0x0C:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "PKI卡已更换，无法读取上笔交易结果\n");
			break;
		case 0x0D:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "经过读取上笔交易结果后，确认充值失败\n");
			break;
		case 0x0E:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "已扣费，但充值结果未知\n");
			break;
		case 0x0F:
			sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "签到出错\n");
			break;
		default:
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMCTSI )
		{
			strcat(msg, "羊城通充值失败\n");
		}
		else
		{
			strcat(msg, "羊城通充值结果未知\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}
	else if( party==FROMPE )
	{
		sprintf(response_code, "%02d", rspcode);
		trade.set_in_par(gbl);
		trade.returnFail(response_code);
	}
	else if( party==FROMIC || party==FROMIC_N )
	{
		switch(rspcode)
		{
		case 0x10:
		case 0x11:
		case 0x21:
		case 0x22:
		case 0x23:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "重新启动读写器\n");
			break;
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x39:
		case 0x3A:
		case 0x3B:
		case 0x3C:
		case 0x76:
		case 0x85:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "操作出错\n");
			break;
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x47:
		case 0x48:
		case 0x49:
		case 0x4A:
		case 0x4B:
		case 0x4C:
		case 0x4D:
		case 0x4E:
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
		case 0x58:
		case 0x59:
		case 0x5A:
		case 0x5B:
		case 0x5C:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "请放好卡\n");
			break;
		case 0x71:
		case 0x72:
		case 0x74:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "请放正确的卡\n");
			break;
		case 0x73:
		case 0x78:
		case 0x79:
		case 0x75:
		case 0x7E:
		case 0x7F:
		case 0x80:
		case 0x81:
		case 0x98:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "拒绝交易，或换卡\n");
			break;
		case 0x77:
		case 0x7A:
		case 0x7B:
		case 0x7C:
		case 0x7D:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x88:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "请放好卡\n");
			break;
		case 0x8B:
		case 0x8C:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "操作出错，可再次操作\n");
			break;
		case 0x93:
		case 0x94:
		case 0x95:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "请检查数据\n");
			break;
		case 0xC1:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "非羊城通票卡\n");
			break;
		case 0xC2:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "票卡错误\n");
			break;
		case 0xC4:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "票卡状态异常不能充值\n");
			break;
		case 0xC5:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值金额超过管理卡额度\n");
			break;
		case 0xC6:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值金额超过票卡额度\n");
			break;
		case 0xC7:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值金额违反业务规则\n");
			break;
		case 0xC8:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "读卡器其它错误\n");
			break;
		case 0xC9:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "没有对应的充值记录\n");
			break;
		case 0xCA:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统流水号与系统记录不一致\n");
			break;
		case 0xCB:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "已确认成功的交易记录不允许修改为失败\n");
			break;
		case 0xCC:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "已确认失败的交易记录不允许修改为成功\n");
			break;
		case 0xCD:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "已确认成功\n");
			break;
		case 0xCE:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "已确认失败\n");
			break;
		case 0xCF:
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "羊城通返回的结果不符合规范\n");
			break;
		default:
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMIC )
		{
			strcat(msg, "羊城通充值失败\n");
		}
		else
		{
			strcat(msg, "羊城通充值结果未知\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}
	else if( party==FROMYCT || party==FROMYCT_N )
	{
		switch(rspcode)
		{
		case 0x00:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值成功\n");
			break;
		case 0xFF:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统错误（要冲正）\n");
			break;
		case 0x01:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "管理卡错\n");
			break;
		case 0x02:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "流水号错\n");
			break;
		case 0x03:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "阶段错误\n");
			break;
		case 0x11:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "月票类型不对\n");
			break;
		case 0x12:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "卡类型不对\n");
			break;
		case 0x13:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "管理卡被禁用\n");
			break;
		case 0x14:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "终端禁用或ISAM_ID为空\n");
			break;
		case 0x15:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "管理卡号不存在\n");
			break;
		case 0x16:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "黑名单卡\n");
			break;
		case 0x17:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "票卡超过有效期\n");
			break;
		case 0x20:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不是终端最后一笔记录\n");
			break;
		case 0x21:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "没有附和条件的月票套餐\n");
			break;
		case 0x22:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "线路号错误\n");
			break;
		case 0x2F:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "其他错误\n");
			break;
		case 0x30:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "报文头错\n");
			break;
		case 0x31:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "卡片不一致\n");
			break;
		case 0x32:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "流水号不一致\n");
			break;
		case 0x33:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "MAC错\n");
			break;
		case 0x3F:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "其他错误\n");
			break;
		case 0x41:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "交易报文没有紧跟查询报文\n");
			break;
		case 0x42:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "入参出现错误\n");
			break;
		case 0x43:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "无效票卡\n");
			break;
		case 0x44:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "卡内原有周期信息不完整\n");
			break;
		case 0x45:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "重复充值\n");
			break;
		case 0x46:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值超过两个周期\n");
			break;
		case 0x47:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "重复充值\n");
			break;
		case 0x48:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不在服务时段\n");
			break;
		case 0x49:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "参数不足\n");
			break;
		case 0x50:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "帐号被禁用\n");
			break;
		case 0x51:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "超过终端每天限额\n");
			break;
		case 0x52:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "帐号余额不足\n");
			break;
		case 0x53:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值前后余次无效\n");
			break;
		case 0x54:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "票卡周期和要求撤消周期不对应\n");
			break;
		case 0x55:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不是终端最后一笔记录\n");
			break;
		case 0x5F:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "其他错误\n");
			break;
		case 0xA1:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "月票充值服务器系统错误（要冲正）\n");
			break;
		case 0x99:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统错误\n");
			break;
		default:			
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMYCT )
		{
			strcat(msg, "羊城通充值失败\n");
		}
		else
		{
			strcat(msg, "羊城通充值结果未知\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}	
	else
	{
		sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}
	return 0;
}

// 索天科技新读卡器版本
// 普通钱包的查询充值
// 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型; enq:0:要充值; 1:只查询
// 出参：pyct_data:羊城通交易信息; errType:出错类型0:成功1:读卡器返的错误; errCode:出错代码0:成功
// 返回值
// 0：成功(查询或充值)
// 1：与读卡器交互失败
// 2：读卡器返回的错误
// 3：与羊城通交互失败
// 4：羊城通返回的错误
// 5：与羊城通交互失败，需要做不完整交易
// 6：与读卡器交互失败，需要做不完整交易
// 7：没有确认标识，需要做不完整交易
// 8：确认充值失败
// 9：读卡器返回的错误，需要做不完整交易

int CYCTYPTrade::ST_NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	int iSelect;
	BYTE term_id[7+1];
	char term_id_tmp[14+1];
	BYTE term_time[7];
	BYTE SK[32+1]; //回话密钥
	
	BYTE pysical_card[8+1]; //物理卡号
	BYTE logical_card[8+1]; //逻辑卡号
	char term_time_tmp[14];
	BYTE card_pycicalInfo1[32+1];//卡片物理信息
	BYTE SW1SW2[4+1];
	int status=0;
	BYTE SAK[1+1];
	BYTE card_physics_info[51];
	SOCKET sd=0;
	CConnect con;//连接类
	int baudRate = 57600;
	//报文中的参数；
	BYTE type [2] ; //信息类型码
	BYTE ver [2]; //报文版本号
	BYTE RFU [34];
	BYTE rsp_code[8+1];
	int trade_type = 0;
	BYTE rsp_message[48];
	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE R_CPU_LOAD_STATUS[1+1];
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //报文序号
	BYTE md5_digest[16];
	BYTE query_info[48+1];

	R_CPU_LOAD_STATUS[0]=0xff;
	
	bbzero(rsp_message);
	bbzero(SHID);
	bbzero(term_id);
	bbzero(term_time);
	bbzero(term_id_tmp);
	bbzero(pysical_card);
	bbzero(logical_card);
	bbzero(card_pycicalInfo1);
	bbzero(rsp_code);
	bbzero(SW1SW2);
	bbzero(SHID);
	bbzero(SK);
	bbzero(SAK);
	bbzero(SERVER_TIME);
	bbzero(term_time_tmp);
	bbzero(type);
	bbzero(ver);
	bbzero(RFU); //RFU 保留
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
	bbzero(query_info);
	
	//BYTE payType='0x00';
	//获取卡片的物理信息,返回卡片的物理信息
	iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
	

	if (iRet == 1)
	{
		TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
		for (int i = 0; i<2; i++)
		{
			iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
			if (iRet == 0)
			{
				break;
			}
			TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
		}

	}
//	if (iRet == -1)
//	{
//		*party = FROMCTSI;
//		*rspcode = CARDNOTFOUND;
//		closesocket(sd);
//		return 1;// 1：与读卡器交互失败
//	}
	if(iRet==0)
	{
		CTools::byte_to_hex((BYTE *)card_physics_info,(BYTE *)pyct_data->ticket_physicalNo,8);//获取物理卡号
		CTools::byte_to_hex((BYTE *)card_physics_info+8,(BYTE *)pyct_data->ticket_no,8);//取得逻辑卡号
		memcpy(SAK,card_physics_info+16,1) ;//SAK
		memcpy(card_pycicalInfo1,card_physics_info+17,32); //卡片物理信息1
		CTools::byte_to_hex(card_physics_info+49,SW1SW2,2);
		
		BYTE temp_SW1SW2[2+1];
		bbzero(temp_SW1SW2);
		memcpy(temp_SW1SW2,card_physics_info+49,2);
		
		wrLog("SW1SW2:%s",SW1SW2);
		if (memcmp(SW1SW2,"9000",4) != 0)
		{
			int iSelect = 0;
			char errMsg[512];
			char outPut_Msg[512];
			get_cardStatus_errMsg((BYTE *)temp_SW1SW2,errMsg);
			bbzero(outPut_Msg);
			sprintf(outPut_Msg,"返回响应卡状态码错误：%s",errMsg);
			TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1：与读卡器交互失败
		}
		
		wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
	//	wrLog("逻辑卡号:%s",pyct_data->ticket_no);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32);
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);	
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = CARDNOTFOUND;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}

	
	
	//共用交易查询P_TRAN_GRY 从平台到服务器
		wrLog("【P_TRAN_QRY】");
		change = 0;
HereTRANQRY:	
	iRet = P_TRAN_QRY(dPosInfo, com, comType, pyct_data,card_pycicalInfo1,enq,trade_type,query_info,party,rspcode);
	if (iRet == 0)
	{
//		trade_type = 0x60;
				switch (trade_type)
				{
					
				case(0x00):
					{
						wrLog("M1钱包(查询余额，圈存和撤销)");
						
						//excute_M1_LOAD(dPosInfo,com,comType,pyct_data,party,rspcode);
						iRet = excute_M1_LOAD(dPosInfo,com,comType,pyct_data,enq,party,rspcode,amount,0x00);
						if (iRet != 0)
						{

						}
						M1_CPU = 1;
						break;
					}
				case (0x02):
					{
						wrLog("M1用户账户充值");
						break;
					}
				case (0x20):
					{
						wrLog("CPU钱包充值（查询余额，圈存和撤销）");			
						iRet = CPU_EnqPay(dPosInfo,com,comType,enq,pyct_data,amount,0x00,query_info,party,rspcode);
						M1_CPU = 2;
						break;
					}
				case(0x21):
					{
						wrLog("OC3.CPU圈存(M1余额转移)");
						change = 1;
//					    iRet =	CPU_EnqPay(dPosInfo,com,comType,enq,pyct_data,"000000000000",0x10,query_info,party,rspcode);
//						R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //卡应用同步
						iRet =	CPU_EnqPay(dPosInfo,com,comType,0,pyct_data,"000000000000",0x10,query_info,party,rspcode);
						bbzero(pyct_data->ticket_amo_bef);
						strcpy(pyct_data->ticket_amo_bef,pyct_data->ticket_amo_aft);
						bbzero(pyct_data->ticket_amo_aft);
						wrLog("卡片同步及余额转移已经完成,请继续充值");
						TS_OutPutMsg(dPosInfo,"同步完成！按enter继续充值",1,iSelect);
						M1_CPU = 2;
						break;
					}
				case (0x22):
					{
						wrLog("oc3.cpu圈存（cpu用户账户充值）");
						break;
					}
				case (0x30):
					{
						wrLog("执行扩展应用操作");
						break;
					}
				case (0x31):
					{
						wrLog("执行扩展应用查询");
						break;
					}
				case (0x40):
					{
						wrLog("执行黑名单处理");					
						iRet = EXCUTE_BLACKLIST(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
						break;
					}
				case (0x60):				
					{
						wrLog("执行CPU同步");
						change = 1;
						iRet = EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
						M1_CPU = 2;
						
						if (iRet == 1 && *rspcode == R_APP_CARD_UPGRADE_EXEFAIL)
						{
							wrLog("已经进入重新执行充值类型查询！");
							TS_OutPutMsg(dPosInfo,"请把你的卡放到读卡器上!",1,iSelect);
							//重新执行充值类型查询
							M1_CPU = 1;
							goto HereTRANQRY; 
						}
						if (iRet == 3)
						{
							wrLog("M1钱包(查询余额，圈存和撤销)");
							iRet = excute_M1_LOAD(dPosInfo,com,comType,pyct_data,enq,party,rspcode,amount,0x00);
						}
						break;		
					}
				default:
					wrLog("无法识别的交易类型");
					*party = FROMYCT_O;
					*rspcode = UNRECOGNIZED_TRADETYPE;
					iRet = 1;
					break;
				}
	}
	if(iRet != 0)
	{
		if (iRet==-1) //超时
		{
			return -1;
		}
		else
		{
			return 1;
		}

	}

	return 0;
}


// 索天科技新读卡器版本
// 第二代羊城通普通钱包的查询充值
// 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型; enq:0:要充值; 1:只查询
// 出参：pyct_data:羊城通交易信息; errType:出错类型0:成功1:读卡器返的错误; errCode:出错代码0:成功
// 返回值
// 0：成功(查询或充值)
// 1：与读卡器交互失败
// 2：读卡器返回的错误
// 3：与羊城通交互失败
// 4：羊城通返回的错误
// 5：与羊城通交互失败，需要做不完整交易
// 6：与读卡器交互失败，需要做不完整交易
// 7：没有确认标识，需要做不完整交易
// 8：确认充值失败
// 9：读卡器返回的错误，需要做不完整交易
int CYCTYPTrade::ST_NormalEnqPay2(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256];
	BYTE ontsStatus1;
	SOCKET sd=0;
	CConnect con;//连接类
	int baudRate = 57600;

	//for test
//	wrLog("Test");
//	char temp[1024];
/*	bbzero(sndData);
	bbzero(temp);
	strcpy(temp, "BA0381023A");
	sndLen = CTools::hex_to_byte((BYTE*)temp, sndData, strlen(temp));
	SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen);
	bbzero(sndData);
	bbzero(temp);
// 	for(int i = 1; i< 10000000; i++)
// 		for(int j = 1; j < 200; j++);
	strcpy(temp, "BA89829002000100803A65585AF52147B276AC58841238BBBB523C105FAC43452810085DC9CB81850CF191ECDCD2C6205A46E107B2CED9D3D83404ED3704A91FF0AFD0A792CA675BD1589E46A9F57BC6959F9AAE42915E755AA26E131461D17D4EB0682E8635174CB6091537E5A63CD0F3516C00E57BC0D1478262FDF319654B259D4D91F7EC0EE730BAB1");
	sndLen = CTools::hex_to_byte((BYTE*)temp, sndData, strlen(temp));
	SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen);
	bbzero(sndData);
	bbzero(temp);
// 	for( i = 1; i< 10000000; i++)
// 		for(int j = 1; j < 200; j++);
	strcpy(temp, "BA02833B");
	sndLen = CTools::hex_to_byte((BYTE*)temp, sndData, strlen(temp));
	SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen);

	TS_OutPutMsg(dPosInfo, "test", 0, iRet);
	return 1;
	//========
*/
	wrLog("【ST_NormalEnqPay】");

	//联机充值寻卡
	wrLog("【1: commuWithIC:ONTS REQUEST】");
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	icData[0] = 0x02;
	len += 1;
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_REQUEST, (BYTE *)icData, (BYTE *)sndData);

	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败
	}

	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	//处理联机充值数据（challenge）
	wrLog("【2: commuWithYCTIC:ONTS CHALLENGE】");		//--------------向羊城通服务器发送阶段1报文，返回阶段2报文
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 2, ST_ONTS_CHALLENGE, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)
		{
		//	CTools::byte_to_hex(rcvData+4, (BYTE*)pyct_data->pki_card, 4);//管理卡逻辑卡号
		//	wrLog("管理卡逻辑卡号pki_card [%s]", pyct_data->pki_card);
		//	strcpy(pyct_data->yct_serial, "00");
		//	CTools::byte_to_hex((BYTE *)rcvData+8, (BYTE *)pyct_data->yct_serial+2, 4);//羊城通交易流水号
		//	wrLog("羊城通交易流水号yct_serial [%s]", pyct_data->yct_serial);
		//  Nothing to do...
		}
		else if(rcvData[3]==MOC_ERROR_ONTS_REFUSED)
		{
			//------------羊城通异常返回-------------
			*party = FROMYCT_O;
			*rspcode = rcvData[13];
			closesocket(sd);			
			return 4;// 2：读卡器返回的错误
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}

	//生成联机充值认证数据
	wrLog("【3: commuWithIC:ONTS AUTH PREPARE】");		//--------------向读卡器发生成认证数据命令，返回阶段3报文
	len = 2;
	bbzero(sndData);
	bbzero(icData);
	
	rcvLen = 0;
	bbzero(rcvData);

	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_AUTH_PREPARE, (BYTE *)icData, (BYTE *)sndData);
	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}

	//处理联机充值认证结果数据
	wrLog("【4: commuWithYCTIC:ONTS AUTH EXECUTE】");		//-------------向羊城通服务器发送阶段3报文，返回阶段4报文
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 4, ST_ONTS_AUTH_EXECUTE, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL || rcvData[3]==MOC_ERROR_ONTS_AUTH)
		{
			CTools::byte_to_hex(rcvData+4, (BYTE*)pyct_data->pki_card, 4);//管理卡逻辑卡号
			//wrLog("管理卡逻辑卡号pki_card [%s]", pyct_data->pki_card);
			strcpy(pyct_data->yct_serial, "00");
			CTools::byte_to_hex((BYTE *)rcvData+8, (BYTE *)pyct_data->yct_serial+2, 4);//羊城通交易流水号
			wrLog("羊城通交易流水号yct_serial [%s]", pyct_data->yct_serial);
			ontsStatus1 = rcvData[13];
			//-------------------需要保存ONTS STATUS2,跟对方确认------------------------
 			if(rcvData[12] == 0x00 && rcvData[13] == 0x00 && rcvData[14] == 0x00 && rcvData[15] == 0x00)
 			{
 				CTools::byte_to_hex((BYTE *)rcvData+16+4, (BYTE *)pyct_data->ticket_no, 8);//羊城通票卡卡号
 				wrLog("羊城通票卡卡号ticket_no: [%s]", pyct_data->ticket_no);
 				getAsiAmo(rcvData+16+25, 4, pyct_data->ticket_amo_bef); //取得以分为单位的十进制金额
 				wrLog("羊城通票卡余额ticket_amo:[%s]", pyct_data->ticket_amo_bef);//羊城通票卡余额
 			}
			else if(rcvData[12] == 0x00 && rcvData[13] == 0x43)
			{
				//-----继续下一步，上送锁卡结果-------
				NULL;
			}
 			else
			{
				*party = FROMYCT;
				*rspcode = rcvData[13];			//----------------??
				closesocket(sd);
				return 2;// 2：读卡器返回的错误
			}
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}

	if(enq==NORMALENQ && ontsStatus1 != 0x43)//只查询
	{
		*party = FROMSUCC;
		*rspcode = SUCC_NOMSG;
		closesocket(sd);
		return 0;
	}
	else
	{
		NULL;
	}

	//生成充值交易参数数据
	wrLog("【5: commuWithIC:ONTS CHARGE PREPARE】");		//---------------向读卡器发送生成充值交易参数数据命令，返回阶段5报文
	memcpy(pyct_data->amount, amount, 12);
	bbzero(icData);
	getHexAmo(amount, icData);	
	wrHex("充值金额",icData,4);
	icLen = 4;
	icData[4] = 0x14;				//------------交易类型代码？固定吗？0x14
	icLen += 1;
	len = icLen + 2;
	bbzero(sndData);
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_CHARGE_PREPARE, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL || (rcvData[3]==MOC_ERROR_CARD_NAMELIST || rcvData[3]==MOC_ERROR_CARD_READ))
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
// 		else if(rcvData[3]==MOC_ERROR_CARD_NAMELIST || rcvData[3]==MOC_ERROR_CARD_READ)
// 		{
// 			//------------黑名单，必须使用异常报文进行第5阶段的处理-----------
// 			NULL;
// 		}
// 		else if( rcvData[3]==MOC_ERROR_CARD_READ)
// 		{
// 			//------------充值阶段5的异常报文，具体解释为上一个报文的ONTS STATUS2-------
// 			*party = FROMYCT_O;
// 			*rspcode = ontsStatus1;
// 			closesocket(sd);
// 			return 4;// 2：读卡器返回的错误
// 		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		wrLog("test.ONTS CHARGE PREPARE timeout");
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}

	//==============================================以下部分异常需要不完整交易===================================================

	pyct_data->status = 0;
	CDB::updateYctYPData(*pyct_data);
	
	//执行联机充值交易
	wrLog("【6: commuWithYCTIC:ONTS CHARGE EXECUTE】");		//--向羊城通服务器发送阶段5报文，返回阶段6报文，打包发到读卡器，返回阶段7明文部分
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 6, ST_ONTS_CHARGE_EXECUTE, sndData, sndLen, rcvData, &rcvLen);
	//=========================================================================================
	//在已发请求，但是读卡器无返回的情况如何处理？！
	//=========================================================================================
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL || rcvData[3]==MOC_ERROR_TS_FALL )
		{
			CTools::byte_to_hex(rcvData+4, (BYTE*)pyct_data->result_message, 16);//交易结果数据报文
			wrLog("返回结果数据result_message [%s]", pyct_data->result_message);

			getAsiAmo(rcvData+4+12, 4, pyct_data->ticket_amo_aft); //取得以分为单位的十进制金额
			wrLog("羊城通票卡充值后余额ticket_amo:[%s]", pyct_data->ticket_amo_aft);//羊城通票卡余额

			CDB::updateYctYPData(*pyct_data);

			if(ontsStatus1 == 0x43)
			{
				*party = FROMYCT;
				*rspcode = 0x43;
				closesocket(sd);
				return 4;// 2：读卡器返回的错误
			}
		}
		else if( rcvData[3]==MOC_ERROR_ONTS_REFUSED )//--------------------充值阶段6的异常报文，如何来判断006F？--------
		{
			//------------羊城通异常返回-------------
			*party = FROMYCT_O;
			*rspcode = rcvData[13];
			closesocket(sd);
			return 4;// 2：读卡器返回的错误
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 9;// 9：读卡器返回的错误，需要未完整交易
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 11;// 11：获取密钥超时，需要冲正，发不完整交易
	}
	else
	{
		wrLog("test.ONTS CHARGE EXECUTE timeout");
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 10;// 10：与读卡器交互失败，无法明确是否充值成功，需要查询余额
	}

	if(enq==NORMALENQ)//只查询
	{
		*party = FROMYCT;
		*rspcode = ontsStatus1;
		closesocket(sd);
		return 3;
	}
	else
	{
		NULL;
	}

	wrLog("【7: commuWithIC:ONTS COMMIT】");		//---------------向读卡器发送ONTS_COMMIT命令，返回阶段7报文
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_COMMIT, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)//------------提交充值结果失败如何处理？！
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else if( rcvData[3]==MOC_ERROR_PKI_DES)//
		{
			*party = FROMIC_N;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 9;// 9：读卡器返回的错误，需要未完整交易
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 1：与读卡器交互失败，需要未完整交易
	}

	//处理交易确认结果
	wrLog("【8: commuWithYCTIC:ONTS CONFIRM】");	
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 8, ST_ONTS_CONFIRM, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{

		if( rcvData[3] == 0x00 && rcvData[12]==0x00 )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			closesocket(sd);
			return 0;
		}
		else if( rcvData[3] == 0x00 && rcvData[12]==0xFF )
		{
			*party = FROMCTSI;
			*rspcode = ICSAYFAIL;
			closesocket(sd);
			return 8;// 8：确认充值失败
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 9;// 9：读卡器返回的错误，需要未完整交易
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 5;// 5：与羊城通交互失败，需要未完整交易
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 6：与读卡器交互失败，需要未完整交易
	}
	
	return 0;
}

// 对数据进行打包
// 入参：tag:报文标识; ver:版本; num:报文顺序号; len:报文长度(cmd,stat,data的长度); cmd:命令标识; stat:状态标识; data:数据;
// 出参：sndData:换包后的数据
// 返回值：包长度
int CYCTYPTrade::ST_packData(int head, int len, int cmd, const BYTE *data, BYTE *sndData)
{
	int sndLen;
	BYTE c;  //校验码
	int dataLen = len - 2;
	
	sndData[0] = head;
	sndData[1] = len;
	sndData[2] = cmd;
	
	sndLen = 3;
	memcpy(sndData + sndLen, data, dataLen);
	sndLen += dataLen;
	c = sndData[0];
	for(int i=1; i<sndLen; i++)
		c ^= sndData[i];
	sndData[sndLen] = c;
	sndLen += 1;
	return sndLen;
}


int CYCTYPTrade::ST_commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen)
{
	int iRet=0, len=0, icLen=0, sndICLen=0;
	BYTE icData[256], sndICData[256];
	
	wrHex("向羊城通发送数据", (BYTE*)sndData, sndLen);
	bbzero(icData);
	iRet = SndRcvYctData(sd, (char *)sndData, sndLen, (char *)icData, &icLen, 256);//羊城通返回数据包（最大135字节）
	if( iRet!=0)
	{
		return 1;
	}
	else
	{
		NULL;
	}
	
	wrHex("从羊城通接收数据", (BYTE*)icData, icLen);
	if(icLen == 0)
		return 1;
	//	icLen = icLen + 20;
	len = icLen + 2;
	
	//for test
	// 	if(cmd == ST_ONTS_CHARGE_EXECUTE)
	// 	{
	// 		int iSelect;
	// 		TS_OutPutMsg(dPosInfo, "for test", 0, iSelect);
	// 		return 2;
	// 	}
	
	
	wrLog("【%d: commuWithIC】", num);
	
	bbzero(sndICData);	
	sndICLen = ST_packData(ST_HEAD, len, cmd, icData, sndICData);
	*rcvLen = 0;
	bbzero(rcvData);
	
	iRet = SndRcvIcData(dPosInfo, com, comType, "请稍候...", sndICData, sndICLen, rcvData, rcvLen, 57600);
	if( iRet==0 )
	{
		return 0;// 0：成功
	}
	else
	{
		return 2;// 2：与读卡器交互失败
	}
}


int CYCTYPTrade::ST_initReader(const TSDLLPosInfo dPosInfo, const int com, const int comType)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, cmd, iSelect=0;
	BYTE icData[256], sndData[256], rcvData[256], pin[7];
	int baudRate = 57600;
	
	wrLog("【初始化读卡器】");
	//获取PIN码
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	cmd = 0x61;
	icData[0] = 0x00;
	icData[1] = 0x51;
	len += 2;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==0x00)
		{
			//初始化正常
			wrLog("【索天科技读卡器】");
			NULL;
		}
		else
		{
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		return 1;// 1：读卡器返回的错误
	}
	
	
	bbzero(pin);
	
	iRet = CDB::getYctPIN2(dPosInfo.pos_no, pin);
	if(iRet == 1)
	{
		wrLog("【锁卡状态】");
		TS_OutPutMsg(dPosInfo, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系", 0, iSelect);
	}
	else if(iRet)
	{
		wrLog("【获取PIN失败】");
		TS_OutPutMsg(dPosInfo, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系", 0, iSelect);
		return 3;	//获取PIN失败
	}
	
	wrLog("getYctPIN2: %s", pin);
	
	wrLog("【校验PIN】");
	//获取pin的函数
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	cmd = 0x62;
	icData[0] = 0x02;
	icData[1] = 0x01;
	icData[2] = 0x06;
	memcpy(icData + 3, pin, 6);
	len += 9;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==0x00)
		{
			//初始化正常
			wrLog("【PIN校验成功】");
			NULL;
		}
		else if( rcvData[3]==MOC_ERROR_SAM_PIN )
		{
			wrLog("【SAM卡密码错误】");
			CDB::setYCTPinErr(dPosInfo.pos_no);
			TS_OutPutMsg(dPosInfo, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系", 0, iSelect);
			return 2;// 2：读卡器返回的错误
		}
		else
		{
			wrLog("【PIN校验失败：%x】", rcvData[3]);
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		return 1;// 1：读卡器返回的错误
	}
	
	return 0;
}

int CYCTYPTrade::ST_NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1], bTemp[17];
	SOCKET sd=0;
	CConnect con;//连接类
	int baudRate = 57600;
	
	wrLog("【ST_NormalNotFinish】");
	
	
	//生成未完整交易处理请求数据
	wrLog("【1: commuWithIC:ONTS UREQUEST】");
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	icData[0] = 0x02;
	len += 1;
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_UREQUEST, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)	//截取阶段A的报文
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	//处理未完整交易处理挑战数据（challenge）
	wrLog("【2: commuWithYCTIC:ONTS UCHALLENGE】");		//--------------向羊城通服务器发送阶段A报文，返回阶段B报文
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 2, ST_ONTS_UCHALLENGE, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)
		{
			NULL;	
		}
		else if( rcvData[3]==MOC_ERROR_ONTS_REFUSED)
		{
			*party = FROMYCT_O;
			*rspcode = rcvData[13];
			closesocket(sd);			
			return 4;// 2：读卡器返回的错误
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
	
	//提交未完整交易的状态数据
	wrLog("【3: commuWithIC:ONTS UCOMMIT】");
	len = 2;
	bbzero(sndData);
	bbzero(icData);
	bbzero(bTemp);
	CTools::hex_to_byte((BYTE*)result_message, bTemp, 32);	
	memcpy(icData, bTemp, 16);	
	len += 16;
	
	rcvLen = 0;
	bbzero(rcvData);
	
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_UCOMMIT, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)	//截取阶段C的报文
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2：读卡器返回的错误
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败，结果未知
	}
	
	//处理未完整交易处理确认结果
	wrLog("【4: commuWithYCTIC:ONTS UCONFIRM】");		//--------------向羊城通服务器发送阶段C报文，返回阶段D报文
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 4, ST_ONTS_UCONFIRM, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)	//确认结果
		{
		/*
		//获取充值结果,如何获取?
		if(rcvData[16+4+4+4] == 0x00 && rcvData[16+4+4+4+1] == 0x00)	//是否有阶段D报文？！
		{
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
		closesocket(sd);
		return 0;// 0：成功
		}
		else
		{
		*party = FROMCTSI;
		*rspcode = FAIL_LAST;
		closesocket(sd);
		return 5;// 5：失败
		}
			*/
			CTools::byte_to_hex(rcvData+4, (BYTE*)pki_card_this, 4);//管理卡逻辑卡号
			//wrLog("管理卡逻辑卡号pki_card_this [%s]", pki_card_this);
			if( memcmp(pki_card_this, pki_card, 8)==0 )
			{
				if(rcvData[16] == 0x00 && rcvData[17] == 0x00)
				{
					*party = FROMSUCC;
					*rspcode = SUCC_MSG;
					closesocket(sd);
					return 0;// 0：成功
				}
				else if(rcvData[16] == 0xff && rcvData[17] == 0xff)
				{
					*party = FROMCTSI;
					*rspcode = FAIL_LAST;
					closesocket(sd);
					return 5;// 5：失败
				}
				else
				{
					*party = FROMCTSI_N;
					*rspcode = PKICHGNOTFINISH;
					closesocket(sd);
					return 4;// 4：PKI卡被更换，结果未知
				}
			}
			else
			{
				wrLog("与原交易管理卡逻辑卡号不一致pki_card [%s]", pki_card);
				*party = FROMCTSI_N;
				*rspcode = PKICHGNOTFINISH;
				closesocket(sd);
				return 4;// 4：PKI卡被更换，结果未知
			}	
		}
		else if( rcvData[3]==MOC_ERROR_ONTS_COMMIT)
		{
			*party = FROMYCT_O;
			*rspcode = rcvData[17];
			closesocket(sd);
			//流水号不一致，此交易在羊城通不存在
			if(rcvData[17] == 0xD1)
			{
				wrLog("此流水与系统流水不一致，认为失败");
				return 5;// 5：失败
			}
			else
				return 4;// 2：读卡器返回的错误
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 2;// 2：读卡器返回的错误
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
}


int CYCTYPTrade::ST_showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode)
{
	int iSelect=0;
	CTrade trade;
	char response_code[2+1], msg[1024];
	bbzero(response_code);
	bbzero(msg);	
	if(party==FROMSUCC)
	{
		switch(rspcode)
		{
		case SUCC_MSG:

			if(atoi(gbl.amount)==1000)
			{
				sprintf(msg, "羊城通退押金成功\n%s\n请按【确认】打印凭证", gbl.yct_disp);
			}
			
			else
			{
				sprintf(msg, "羊城通充值成功\n%s\n请按【确认】打印凭证", gbl.yct_disp);
				
			}
			
			TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);

			break;
		case SUCC_NOMSG:
			break;
		case SUCC_MSG_LAST:
			TS_OutPutMsg(gbl.dPosInfo, "上一笔羊城通充值成功", 0, iSelect);
			break;
		default:
			if(atoi(gbl.amount)==1000)
			{
				
				TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金失败，请联系羊城通客服", 0, iSelect);
			}
			else
			{
				sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888!\n", party, rspcode);
				TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
			}

			break;
		}
	}
	else if( party==FROMCTSI || party==FROMCTSI_N )

	{
		if(atoi(gbl.amount)==1000)
		{
			TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金失败，请联系羊城通客服", 0, iSelect);

		}

		else
		{

			switch(rspcode)
			{
			case 0x01:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "系统未能发现有读卡器的配置：您尚未开通此项业务，可直接输入[861]申请开通。\n");
				break;
			case 0x02:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "羊城通充值已取消\n");
				return 0;
				break;
			case 0x03:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "扣费超时\n");
				break;
			case 0x04:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "向读卡器发送接收数据超时\n");
				break;
			case 0x05:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "系统发现有读卡器有误：请您确定读卡器是否正确连接，然后选择[返回]后重新操作。\n");
				break;
			case 0x06:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "向羊城通发送接收数据超时\n");
				break;
			case 0x07:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "读卡器不能确定交易状态\n");
				break;
			case 0x08:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "读卡器指明交易失败\n");
				break;
			case 0x09:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "系统包打包出错\n");
				break;
			case 0x0A:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "系统包解包出错\n");
				break;
			case 0x0B:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "不支持的卡\n");
				break;
			case 0x0C:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "PKI卡已更换，无法读取上笔交易结果\n");
				break;
			case 0x0D:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "经过读取上笔交易结果后，确认充值失败\n");
				break;
			case 0x0E:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "已扣费，但充值结果未知\n");
				break;
			case 0x0F:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "签到出错\n");
				break;
			case 0x10:
				sprintf(msg, "服务器返回的错误码：[%02X%02X] \n", party, rspcode);
				strcat(msg, "非常抱歉，无法获知充值结果，请再次发起查询交易，通过票卡余额核对交易是否成功，如若还有其他疑问，请联系在线客服中心【Ctrl+H】  400-80-11888\n");
				break;
			case 0x11:
				sprintf(msg, "读卡器无法读到卡 [%02X%02X] \n", party, rspcode);
				strcat(msg, "请指引客户将公交卡正确放置在读卡器上，且勿移动它。\n");
				break;
			case 0x15:
				sprintf(msg, "卡为黑名单卡 [%02X%02X] \n", party, rspcode);
				strcat(msg, "请联系在线客服中心【Ctrl+H】  400-80-11888\n");
				break;
			default:
				sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
				break;
		}

		}
	
		if( party==FROMCTSI )
		{
			strcat(msg, "羊城通充值失败\n");
		}
		else
		{
			strcat(msg, "羊城通充值结果未知\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}
	else if( party==FROMPE )
	{
		sprintf(response_code, "%02d", rspcode);
	 	trade.set_in_par(gbl);
		trade.returnFail(response_code);
	}
	else if( party==FROMIC || party==FROMIC_N )

	{
		if(atoi(gbl.amount)==1000)
		{
			TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金失败，请联系羊城通客服", 0, iSelect);

		}
		else
		{
			sprintf(msg, "读卡器返回的错误码：[%02X%02X] \n", party, rspcode);
			switch(rspcode)
			{
			case 0x60:
				strcat(msg, "读卡器没有安装SAM卡，请与客服进行联系\n");
				break;
			case 0x62:
				strcat(msg, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系\n");
				break;
			case 0x80:
				strcat(msg, "读卡器找不到卡，请把卡放到读卡器上\n");
				break;
			case 0x90:
				strcat(msg, "非羊城通卡\n");
				break;
			case 0x91:
				strcat(msg, "卡片已经过期\n");
				break;
			case 0x93:
				strcat(msg, "卡片非法\n");
				break;
			case 0x94:
				strcat(msg, "卡片为黑名单卡\n");
				break;
			case 0x96:
				strcat(msg, "钱包余额充值后超出上限\n");
				break;
		case 0x13:
			sprintf(msg, "余额转移失败 [%02X%02X] \n", party, rspcode);
			strcat(msg, "请返回主菜单，并重新指引客户将公交卡正确放置在读卡器上，且勿移动它。\n");
			break;
		case 0x01:
			strcat(msg, "读卡器返回错误，请联系在线客服中心【Ctrl+H】  400-80-11888\n");
			break;
		default:
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMIC )
		{
			strcat(msg, "羊城通充值失败\n");
		}
		else
		{
			strcat(msg, "羊城通充值结果未知\n");
		}

		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
		
		}
	}
	else if( party==FROMYCT || party==FROMYCT_N )
	{
		if(atoi(gbl.amount)==1000)
		{
			if(rspcode==0x00)
			{
				TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金成功\n", 0, iSelect);
			}
			else
			{
				TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金失败，请联系羊城通客服", 0, iSelect);
				
			}
			
		}

		else
		{
			
				switch(rspcode)
		{
		case 0x00:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值成功\n");
			break;
		case 0xFF:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统错误（要冲正）\n");
			break;
		case 0x01:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "管理卡错\n");
			break;
		case 0x02:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "流水号错\n");
			break;
		case 0x03:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "阶段错误\n");
			break;
		case 0x11:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "月票类型不对\n");
			break;
		case 0x12:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "卡类型不对\n");
			break;
		case 0x13:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "管理卡被禁用\n");
			break;
		case 0x14:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "终端禁用或ISAM_ID为空\n");
			break;
		case 0x15:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "管理卡号不存在\n");
			break;
		case 0x16:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "黑名单卡\n");
			break;
		case 0x17:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "票卡超过有效期\n");
			break;
		case 0x20:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不是终端最后一笔记录\n");
			break;
		case 0x21:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "没有附和条件的月票套餐\n");
			break;
		case 0x22:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "线路号错误\n");
			break;
		case 0x2F:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "其他错误\n");
			break;
		case 0x30:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "报文头错\n");
			break;
		case 0x31:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "卡片不一致\n");
			break;
		case 0x32:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "流水号不一致\n");
			break;
		case 0x33:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "MAC错\n");
			break;
		case 0x3F:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "其他错误\n");
			break;
		case 0x41:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "交易报文没有紧跟查询报文\n");
			break;
		case 0x42:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "入参出现错误\n");
			break;
		case 0x43:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "黑名单卡\n");
			break;
		case 0x44:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "卡内原有周期信息不完整\n");
			break;
		case 0x45:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "重复充值\n");
			break;
		case 0x46:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值超过两个周期\n");
			break;
		case 0x47:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "重复充值\n");
			break;
		case 0x48:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不在服务时段\n");
			break;
		case 0x49:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "参数不足\n");
			break;
		case 0x50:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "帐号被禁用\n");
			break;
		case 0x51:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "超过终端每天限额\n");
			break;
		case 0x52:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "帐号余额不足\n");
			break;
		case 0x53:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "充值前后余次无效\n");
			break;
		case 0x54:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "票卡周期和要求撤消周期不对应\n");
			break;
		case 0x55:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "不是终端最后一笔记录\n");
			break;
		case 0x5F:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "其他错误\n");
			break;
		case 0xA1:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "月票充值服务器系统错误（要冲正）\n");
			break;
		case 0x99:
			sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
			strcat(msg, "系统错误\n");
			break;
		default:			
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			break;
		}
		
		}
		if( party==FROMYCT )
		{
			strcat(msg, "羊城通充值失败\n");
		}
		else
		{
			strcat(msg, "羊城通充值结果未知\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}	
	else if( party==FROMYCT_O )

	{

		if(atoi(gbl.amount)==1000)
		{
			if(rspcode==0x01||rspcode==0x02||rspcode==0x03)
			{
				TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金成功", 0, iSelect);
			}
			
			else
			{
				TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金失败，请联系羊城通客服", 0, iSelect);
			}
			
		}
		else
		{
					sprintf(msg, "羊城通返回的错误码：[%02X%02X] \n", party, rspcode);
		switch(rspcode)
		{
		case 0x21:
			strcat(msg, "管理卡逻辑卡号无效\n");
			break;
		case 0x22:
			strcat(msg, "管理卡版本号无效\n");
			break;
		case 0x23:
			strcat(msg, "管理卡超过有效期\n");
			break;
		case 0x24:
			strcat(msg, "管理卡被停用\n");
			break;
		case 0x25:
			strcat(msg, "管理卡额度不足\n");
			break;
		case 0x26:
			strcat(msg, "管理卡不在服务时限\n");
			break;
		case 0x41:
			strcat(msg, "非羊城通系统票卡\n");
			break;
		case 0x42:
			strcat(msg, "票卡超过有效期\n");
			break;
		case 0x43:
			strcat(msg, "黑名单卡\n");
			break;
		case 0x51:
			strcat(msg, "寻卡错误\n");
			break;
		case 0x52:
			strcat(msg, "消费密钥错误\n");
			break;
		case 0x53:
			strcat(msg, "读票卡公共信息区错误\n");
			break;
		case 0x54:
			strcat(msg, "读票卡钱包区错误\n");
			break;
		case 0x5A:
			strcat(msg, "票卡已经为黑名单卡\n");
			break;
		case 0x5B:
			strcat(msg, "写黑名单标识成功\n");
			break;
		case 0x5C:
			strcat(msg, "写黑名单标识失败\n");
			break;
			//-----------------需要确认-------------------
		case 0x01:
			strcat(msg, "交易成功\n");
			break;
		case 0x02:
			strcat(msg, "交易成功\n");
			break;
		case 0x03:
			strcat(msg, "交易成功\n");
			break;
		case 0xF1:
			strcat(msg, "交易失败\n");
			break;
		case 0xF2:
			strcat(msg, "交易失败\n");
			break;
		case 0xF3:
			strcat(msg, "交易失败\n");
			break;
		case 0xF4:
			strcat(msg, "交易失败\n");
			break;
		case 0xF5:
			strcat(msg, "交易失败\n");
			break;
			//---------------------------------------------
		case 0x61:
			strcat(msg, "票卡状态异常，不能充值\n");
			break;
		case 0x62:
			strcat(msg, "充值金额超过管理卡额度\n");
			break;
		case 0x63:
			strcat(msg, "充值金额超过票卡限定\n");
			break;
		case 0x64:
			strcat(msg, "充值金额违反夜晚规定\n");
			break;
		case 0xFF://-------确认充值失败，可能不用在这里处理----------
			strcat(msg, "充值失败\n");
			break;
		case 0xD1:
			strcat(msg, "系统流水号与系统记录不一致，充值失败\n");
			break;
		case 0xD2:
			strcat(msg, "已确认超过交易不允许修改为失败\n");
			break;
		case 0xD3:
			strcat(msg, "已确认失败交易不允许修改为成功\n");
			break;
		case 0x31:
			strcat(msg, "充值金额违反业务规则，请重新选择金额进行充值\n");
			break;
		case 0x30:
			strcat(msg, "充值后余额超上限，请重新选择金额进行充值\n");
			break;
		case 0x32:
			strcat(msg, "羊城通后台返回错误，无法识别的交易类型\n");
			break;
		default:			
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			break;
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
		
		}

	}
	else
	{
		if (atoi(gbl.amount)==1000)
		{
			TS_OutPutMsg(gbl.dPosInfo, "羊城通退押金失败，请联系羊城通客服", 0, iSelect);
		}
		
		else
		{
			sprintf(msg, "羊城通充值未知错误[%02X%02X]，请联系在线客服中心【Ctrl+H】  400-80-11888\n", party, rspcode);
			TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
			
		}
	}

	return 0;
}

/*************************第三代羊城通充值新增函数 开始*****************************************/

//读写器程序初始化，程序启动后只调用一次。
//索天读卡器初始化函数

int CYCTYPTrade::ST_initReader3(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, cmd, iSelect=0;
	BYTE icData[256], sndData[256], rcvData[256], pin[7];
	int baudRate = 57600;
	BYTE INT_KEY[8];
	
	wrLog("【ST_initReader3】");
	bbzero(pin);
	iRet = CDB::getYctPIN2(dPosInfo.pos_no, pin);
	if(iRet == 1)
	{
		wrLog("【锁卡状态】");
		TS_OutPutMsg(dPosInfo, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系", 0, iSelect);
	}
	else if(iRet)
	{
		wrLog("【获取PIN失败】");
		TS_OutPutMsg(dPosInfo, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系", 0, iSelect);
		return 3;	//获取PIN失败
	}
	
	
	wrLog("getYctPIN2: %s", pin);
	//	return 3;
	
	wrLog("【校验PIN】");
	//获取pin的函数
	bbzero(sndData);
	bbzero(icData);
	bbzero(INT_KEY);
	
	len = 2;
	cmd = 0x10;
	
	icData[0] = 0x02;
	memcpy(icData + 1, pin, 6);
	memcpy(icData + 7,INT_KEY,8);
	len += 15;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			wrHex("GPK_ID", (BYTE *)rcvData, 9);
			//初始化正常
			wrLog("【初始化成功】");
			CTools::byte_to_hex(rcvData+4,(BYTE *) pyct_data->pki_card, 4);//管理卡逻辑卡号
			//wrLog("管理卡逻辑卡号pki_card [%s]", pyct_data->pki_card);
			NULL;
			
		}
		else if( rcvData[3]==MOC_ERROR_SAM_PIN )
		{
			
			wrLog("【SAM卡密码错误】");
			CDB::setYCTPinErr(dPosInfo.pos_no);
			TS_OutPutMsg(dPosInfo, "羊城通SAM卡配置有误，暂时无法使用，请与客服进行联系", 0, iSelect);
			return 2;// 2：读卡器返回的错误
		}
		else
		{
			wrLog("【PIN校验失败：%x】", rcvData[3]);
			return 2;// 2：读卡器返回的错误
		}
	}
	
	else
	{
		return 1;// 1：读卡器返回的错误
	}
	
	return 0;
	
}

//与羊城通服务器交互数据
int CYCTYPTrade::ST_commuWithYCTIC3(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data, const int com, const int comType, const SOCKET sd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen)
{
	int iRet=0;
	
	wrHex("向羊城通发送数据", (BYTE*)sndData, sndLen);
	iRet = SndRcvYctData(sd, (char *)sndData, sndLen, (char *)rcvData,rcvLen, 500);//羊城通返回数据包（最大字节500）
	//监控报文发送后直接返回，无需等待返回数据包
	if (sndData[0] == 0xfd)
	{
		//羊城通报文+1，同时更新yctyp_signin表里的报文序号字段
		return 0;
	}
	if( iRet!=0)
	{
		return 1;
	}
	else
	{
		NULL;
	}
	
	
	wrHex("从羊城通接收数据", (BYTE*)rcvData, *rcvLen);
	//羊城通报文+1，同时更新yctyp_signin表里的报文序号字段
	pyct_data->yct_sequence_NO++;
	wrLog("当前报文序号: %d",pyct_data->yct_sequence_NO);
	CDB::updateYct3SequenceNo((char *)dPosInfo.pos_no,pyct_data->yct_sequence_NO);
	
	if(*rcvLen == 0)
		return 1;
	
	return 0;
	
}

//终端签到
int CYCTYPTrade::yct_SignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * party ,BYTE* rspcode)

{
	
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen,cmd=0;
	SOCKET sd=0;
	int baudRate=57600;
	CConnect con;//连接类
	BYTE icData[256], sndData[256], rcvData[256];
	BYTE term_id[6]; //终端号
	BYTE DE1[136+1];
	BYTE DE2[136+1];
	BYTE DE3[136+1];
	BYTE term_time[12+1];
	BYTE SK[32+1];
	BYTE SERVER_TIME[6+1]; //服务器时间
	BYTE SKEY[32+1];
	bbzero(term_id);
	bbzero(SK);
	bbzero(SKEY);
	bbzero(icData);
	bbzero(sndData);
	bbzero(DE1);
	bbzero(DE2);
	bbzero(DE3);
	
	wrLog("【签到步骤1:R_AC_LOGIN_1】"); 	//终端签到
	CTools::get_termTime((char *)term_time); //获取终端时间
	
	len = 2;
	cmd = 0xC8; //签到命令
	
	CTools::hex_to_byte((BYTE *)pyct_data->pki_card,(BYTE *)icData,8); //终端的管理卡号
	
	memcpy(icData+4,term_id,6);  //终端编号
	CTools::hex_to_byte((BYTE*)term_time,icData+10,14);//终端时间
	len += 17;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			memcpy(DE1,rcvData+4,136);//取数据
			wrHex("读卡器返回签到DE1:%s",rcvData,rcvLen);
			NULL;
		}
		
		else if(rcvData[3]==0x61)
		{
			
			wrLog("SAM卡初始化失败"); 
		}
		
	}
	
	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	wrLog("【将DE1 传给服务器】");		//--------------向羊城通服务器发送阶段1报文，返回阶段2报文
	
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, DE1, 136, rcvData, &rcvLen);
	
	if( iRet==0  )
	{
		
		memcpy(DE2,rcvData,136);
		BYTE de2_tmp[272];
		CTools::byte_to_hex(rcvData,de2_tmp,136);
		wrLog("服务器返回签到数据DE2:%s",de2_tmp);
		
	}
	
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
	wrLog("终端将DE2下发给读卡器");
	
	bbzero(icData);
	bbzero(sndData);
	
	len = 2;
	cmd = 0xC9; //签到2命令
	
	memcpy(icData, DE2, 136); //终端的管理卡号
	len += 136;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			
			CTools::byte_to_hex(rcvData+4,pyct_data->SHID,8); //握手流水号
			memcpy(SK,rcvData+12,32); //sk取前16个字节，作为AES密钥
			memcpy(pyct_data->SKEY,SK,16);
			CTools::byte_to_hex(pyct_data->SKEY,SKEY,16);
			//wrLog("DE3:%s",DE3);
			CTools::byte_to_hex(rcvData+44,SERVER_TIME,6);  //取服务器时间
			bbzero(pyct_data->SignIn_ServerTime);
			strcpy(pyct_data->SignIn_ServerTime,"20");
			strcat(pyct_data->SignIn_ServerTime,(char *)SERVER_TIME);
			memcpy(DE3,rcvData+50,136);//返回签到步骤3数据
			
			wrHex("服务器返回签到数据DE3",DE3,136);
			wrLog("SHID:%s",pyct_data->SHID);
			wrLog("SKEY为：%s",SKEY);
			wrLog("签到服务器时间:server_time:%s",SERVER_TIME);
			
			NULL;
		}
		else if(rcvData[3]==0xFF)
		{
			
			wrLog("MOC_AUTH_FAIL 失败");
			return 3;// 与读卡器交互失败
			
		}
		
	}
	
	wrLog("【3: commuWithYCTIC】");		//--------------向羊城通服务器发送阶段2报文，返回阶段3报文
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data,com, comType, sd, DE3, 136, rcvData, &rcvLen);
	if( iRet==0  )
	{
		
		if(rcvData[8]==0x00)
		{
			wrLog("认证成功");
			pyct_data->yct_sequence_NO=0; //签到之后报文序号重新为0
			CDB::updateYct3SequenceNo((char *)dPosInfo.pos_no,pyct_data->yct_sequence_NO);		
		}
		else if(rcvData[8]==0xFF)
		{
			wrLog("认证失败");
			return 4;  //认证失败
		}
	}
	
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
	
	return 0;
}


//获取卡片的物理信息,返回物理卡号，逻辑卡号，SAK,卡物理信息1，及SW1SW2
int CYCTYPTrade::R_PUB_QRY_PHYSICS_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE*physicsInfo)

{
	int  len=0, icLen=0, sndLen=0, rcvLen=0,cmd=0,iRet=0;//,iSelect;
	BYTE icData[256], sndData[256], rcvData[256];
	
	bbzero(icData);
	bbzero(sndData);
	
	int baudRate = 57600;
	//查询卡的物理信息 
	wrLog("【查询卡的物理信息】");
	
	len = 2; //长度
	cmd = 0x93; 
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			wrHex("返回物理信息：",rcvData,rcvLen);
			memcpy(physicsInfo,rcvData+4,51);
			NULL;
		}
		else if (rcvData[3]==0x80)
		{
			return 1;
		}
		
		//else 见读卡器错误列表
		else
		{
			iRet = -1;
			return iRet;
			
		}
		
	}
	return 0;
	
}

//进行CPU卡的余额查询和圈存

int CYCTYPTrade::CPU_EnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, \
	int enq, PYCT_DATA pyct_data, char *amount, BYTE payType,BYTE *query_info,BYTE* party, BYTE* rspcode)
{
	int len=0;
	BYTE charge_password[16];
	char SW1SW2[4+1];
	char temp[256];
	BYTE card_info[16];
	BYTE count_info [16];
	BYTE RFU[22+1];

	BYTE trade_sequence[2];
	BYTE yct_term_status[2];
	int status = 0;
	BYTE trade_message2[64+1];
	BYTE trade_message1[64+1];
	//BYTE trade_message3[32+1];
	int iRet=0;	
	int baudRate=57600;
	BYTE TAC [8+1];
	BYTE outside_info[16];//外部系统附加信息
	BYTE tuozhan_stan[8];
	char yct_sequence_NO[4+1];
	BYTE len_tmp[2];
	CConnect con;
	int load_status;
	BYTE followup_oper[2+1];
	BYTE charge_authcode[8+1];
	int iSelect;
	int load_suc=0; //圈存成功标志
	
	M1_CPU = 2;
	bbzero(SW1SW2);
	bbzero(outside_info);
	bbzero(tuozhan_stan);
	bbzero(yct_sequence_NO);
	bbzero(len_tmp);
	bbzero(RFU);
	bbzero(charge_password);
    bbzero(yct_term_status);
	bbzero(charge_authcode);
    bbzero(temp);
    bbzero(TAC);
	wrLog("【CPU_EnqPay】");
	CTools::getDate(pyct_data->txn_date);
    CTools::getTime(pyct_data->txn_time);
	//1.P_PUB_SET_READCARDINFO(设置读卡信息)
	R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,query_info,SW1SW2);
	
	//4.终端向读卡器发送读卡指令R_CPU_GET_CARDINFO CPU取卡信息
	flag1 = 0;
	R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,flag1);
//	if (cpurspStatus == 148)
//	{
//			iRet = EXCUTE_BLACKLIST(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
//			*party = FROMCTSI;
//			*rspcode = BLACKLIST;
//			return 1;
//	}

	if (cpurspStatus==0x80 || cpurspStatus==147)
	{
		for (int num = 0; num<2; num++)
		{
			if (cpurspStatus == 0)
			{
				break;
			}
			R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,flag1);
		}
		if (cpurspStatus==0x80 || cpurspStatus==147)
		{
			*party = FROMCTSI;
			*rspcode = CARDNOTFOUND;
//			closesocket(sd);
			return 1;// 1：与读卡器交互失败
		}

	}
	if (cpurspStatus != 0)
	{
		char errMsg[1024];
		bbzero(errMsg);
		get_cardReader_errMsg((BYTE *)cpurspStatus,errMsg);
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		*party=FROMIC;
		*rspcode=ERROR_FROMIC;
		return 1;
	}

	if (enq == 1)
	{
		//只查询
		return 0;
	}

	if (payType == 0x20)
	{
		sprintf(temp,"卡号:%s\n\n充值前卡余额:%.2f元\n\n按【Enter】充值  按【Esc】返回",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100 - atof(des_lim));
		TS_OutPutMsg(dPosInfo,temp,0,iSelect);
		if (iSelect)
		{
			*party=FROMCTSI_N;
			*rspcode=USERCANCEL;
			return 5;
		}
	}
	if (payType == 0x22)
	{
     	//	sprintf(bef,"\n\n%s元",pyct_data->ticket_amo_bef);
		sprintf(temp,"\n\n充值前卡余额:%.2f元\n\n按【Enter】充值  按【Esc】返回",atof(pyct_data->ticket_amo_bef)/100 - atof(des_lim));
		strcat(auto_show,temp);
		TS_OutPutMsg(dPosInfo,auto_show,0,iSelect);
		if (iSelect)
		{
			*party=FROMCTSI_N;
			*rspcode=USERCANCEL;
			return 5;
		}
	}
	
	//终端向充值服务器发送圈存查询请求报文（P_CPU_LOAD_QRY）
	iRet = P_CPU_LOAD_QRY(dPosInfo,com, comType,count_info,pyct_data,amount,payType,trade_message1,party,rspcode);
    if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		return 3;// 3：与羊城通交互失败
	}
	else if (iRet != 0)
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败
	}
	
	//6.终端向读卡器发送圈存初始化指令R_CPU_LOAD_INIT
    R_CPU_LOAD_INIT(dPosInfo,com, comType,pyct_data,trade_message1,trade_sequence,yct_term_status,card_info);
	
	
	//7.圈存初始化成功之后，向充值服务器发送圈存(P_CPU_LOAD)
	iRet = P_CPU_LOAD(dPosInfo,com, comType,pyct_data,card_info,yct_term_status,trade_message2,charge_authcode);
	if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		return 3;// 3：与羊城通交互失败
	}
	else if (iRet != 0)
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1：与读卡器交互失败
	}
	
	put_card = 0;
	//读卡器发送圈存指令 R_CPU_LOAD(cpu钱包圈存) 对cpu卡进行cpu钱包充值 避免出现终端交互错误，查询卡片余额也失败的时候出现问题！！
	iRet = R_CPU_LOAD(dPosInfo,com, comType,pyct_data,trade_message2,TAC,load_status,count_info);
	if (iRet==0)
	{
			load_suc=1; //表示明确圈存成功
	}

	if (change == 1 && iRet == -1) //未知情况
	{
		int iRet2 = 0;
		do {
			iRet2 = EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
			
		} while( iRet2 == 0);
		if (iRet2 == 0)
		{
			return 0;
		}
		else
		{
			TS_OutPutMsg(dPosInfo, "\n同步出错！\n   按【确认】返回", 0,iSelect);
			return 1;
		}
	}

	else if (iRet ==-1) 
	{
		TS_OutPutMsg(dPosInfo, "\n羊城通充值异常，进行冲正！\n   按【确认】返回", 0,iSelect);
		R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,flag1);
		if (cpurspStatus == 128 || cpurspStatus ==147)
		{
			int k=0;
			for (k;k<2;k++)
			{
				R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,1);
				//			TS_OutPutMsg(dPosInfo, "\n请把卡放到读卡器上\n   按【确认】返回", 0,iSelect);
				Sleep(100);
				if (cpurspStatus == 0)
				{
					break;
				}
			}
			if (k >= 2)
			{
				put_card = 1;
			}
		}

		int i = 1;
		while (i<=3)
		{
			iRet = P_CPU_LOAD_SUBMIT(dPosInfo,com, comType,pyct_data,load_status,TAC,yct_term_status,count_info,charge_authcode,followup_oper);
			i++;
			if ((iRet == 0 || i>3)&& put_card != 1)
			{
				break;
			}
		}

//		TS_OutPutMsg(dPosInfo,"羊城通充值失败",1,iSelect);
		if (iRet != 0 && i>3)
		{
			//在下次开机、操作圈存或圈存撤销时，再次自动提交。
			//pyct_data->status = 0;
		//	*party=FROMIC;
		//	*rspcode=ICSAYFAIL;
			pyct_data->yct_ack=0; //需要开机重新提交
		//	return 10;
		}
		*party=FROMIC;
		*rspcode=ICSAYFAIL;
		if (put_card == 1)
		{
			return 11;
		}
		return 1;
	}

	if (iRet == 1) //如果出现读卡器交互出错
	{
		//*party = FROMCTSI;
		//*rspcode = ICTIMEOUT;
		moni=1;
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
		return -1;// -101：与读卡器交互失败
	}
	
	//重新读卡 如果圈存成功 对于明确圈存成功的。可以不进行读卡。
	
	flag1 = 1;

	for (int k=0;k<3;k++)
	{
		R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,flag1);
		Sleep(1000);
		//			TS_OutPutMsg(dPosInfo, "\n请把卡放到读卡器上\n   按【确认】返回", 0,iSelect);
		if (cpurspStatus == 0)
		{
			break;
		}
	}
	
	char amo_bef[12+1];
	char amo_aft[12+1];
//	char amo_amount[12+1];
// 	CTools::HexToDec(pyct_data->ticket_amo_bef, amo_bef);
//	CTools::HexToDec(pyct_data->ticket_amo_aft, amo_aft);	
//	strcpy(amo_bef,pyct_data->ticket_amo_bef);
//	strcpy(amo_aft,pyct_data->ticket_amo_aft);
	sprintf(amo_aft,"%s",pyct_data->ticket_amo_aft);
	sprintf(amo_bef,"%s",pyct_data->ticket_amo_bef);
	bbzero(pyct_data->amount);
	if (change == 1)
	{
	//	memcpy(amount,pyct_data->ticket_amo_aft,12);
		long int a = atoi(pyct_data->ticket_amo_aft) - atoi(pyct_data->ticket_amo_bef);
		sprintf(amount,"%12d",a);
		change = 0;
	}
	else
		memcpy(pyct_data->amount,amount,12);
	sprintf(pyct_data->amount,"%.2f",pyct_data->amount);
//	wrLog("充值前: %s",amo_bef);
//	wrLog("充值金额: %s",amount);
//	wrLog("充值后: %s",amo_aft);
//	if (atuo_charge == 1)
//	{
//		CTools::HexToDec((char *)amount,amount);
//		wrLog("----------------:%s",amount);
//	}
	wrLog("充值前:%d",atoi(amo_bef));
	wrLog("充值金额:%d",atoi(amount));
	wrLog("充值后:%d",atoi(amo_aft));
	if ( cpurspStatus == 0)
	{
		if (load_suc==1  || (atoi(amo_aft) == atoi(amo_bef) + atoi(amount) )) //如果圈存成功或者读卡器前后加值是相等的。
		{

			if (load_suc==1 ) //如果是1的情况 //圈存明确成功
			{
				bbzero(pyct_data->ticket_amo_aft);
				long int a = atoi(pyct_data->amount) + atoi(pyct_data->ticket_amo_bef);
				sprintf(pyct_data->ticket_amo_aft,"%12d",a);

			}
			wrLog("圈存成功");
			//打印圈存成功单据
		}
		else
		{
			wrLog("圈存后，写卡余额前后不正确，进行冲正");
			TS_OutPutMsg(dPosInfo,"圈存失败，进行冲正",1,iSelect);
			pyct_data->status=4;
			int i = 1;
			while (i<=3)
			{
				iRet = P_CPU_LOAD_SUBMIT(dPosInfo,com, comType,pyct_data,load_status,TAC,yct_term_status,count_info,charge_authcode,followup_oper);
				i++;
				if (iRet == 0 || i>3)
				{
					break;
				}
			}
			
			if (iRet != 0 && i>3)
			{
				//在下次开机、操作圈存或圈存撤销时，再次自动提交。
				//pyct_data->status = 0;
			//	*party=FROMIC;
			//	*rspcode=CONNCOMFAIL;
			//	return 10;
				pyct_data->yct_ack=0; 
			}			
			*party=FROMIC;
			*rspcode=CONNCOMFAIL;
			return 1;// 1：与读卡器交互失败
		}
	}

	wrLog("cpurspStatus:%d",cpurspStatus);
		if (cpurspStatus != 0)
		{
			moni = 1;
		}
		else
			moni = 0;
		
		//9.终端向读卡器发送圈存状态提交报文（P_cpu_load_submit圈存提交）
		bbzero(pyct_data->amount);
		memcpy(pyct_data->amount,amount,12);
		int i = 1;
		while (i<=3)
		{
			iRet = P_CPU_LOAD_SUBMIT(dPosInfo,com, comType,pyct_data,load_status,TAC,yct_term_status,count_info,charge_authcode,followup_oper);
			i++;
			if (iRet == 0 || i>3)
			{
				break;
			}
		}
		
		if (iRet != 0 && i>3)
		{
			//在下次开机、操作圈存或圈存撤销时，再次自动提交。
		//	pyct_data->status = 0;
		//	*party = FROMSUCC;
		//	*rspcode = SUCC_MSG;
		//	return 10;
			pyct_data->yct_ack=0; 

	}
	
	if (followup_oper[0] != 0x00)
	{
		*party = FROMCTSI;
		*rspcode = SUCC_NEEDOTHEROPR;	
	}
    if (followup_oper[0] == 0x00)
	{
		pyct_data->status = 2;
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
	}

	//closesocket(sd);
	return 0;	
}//CPU_EnqPay
	
	
	//设置卡信息
	int  CYCTYPTrade::R_PUB_SET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,char* logical_card,BYTE * query_info,char *SW1SW2)
	{
		
		int len=0,cmd=0,sndLen = 0,iRet = 0,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int bandRate=57600;
		bbzero(icData);
		bbzero(sndData);
		bbzero(rcvData);

		wrLog("【R_PUB_SET_READCARDINFO】");

		len = 2;
		cmd = 0x95; 
		CTools::hex_to_byte((BYTE *)logical_card,(BYTE *)icData,16);
		len+=8;
		memcpy(icData + 8,query_info, 48); //查询信息
		len += 48;
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, bandRate)) == 0)		
		{
			if( rcvData[3]==0x00)
			{
				//return 1;
			//	bbzero(SW1SW2);
				BYTE temp[2+1];
				bbzero(temp);
				CTools::byte_to_hex((BYTE *)rcvData+4, (BYTE * )temp, 2);
				memcpy(SW1SW2,(char *)temp,2);
				//CTools::byte_to_hex((BYTE *)rcvData+4, (BYTE * )SW1SW2, 2);//卡状态码
			}
			//else 见读卡器错误定义
			
			else
				
			{
				return -1;
				
			}
			
		}
		
		return 0;
		
	}
	
	//读取cpu卡的卡信息 读取钱包余额及卡片状态
	//flag:  0 - 充值前查询余额, 1 - 充值后查询余额
	void CYCTYPTrade::R_CPU_GET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE *count_info,int flag)
	{
int len,cmd,sndLen,rcvLen,iRet=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		int  iSelect;
		BYTE up_limit[6+1];
	

		bbzero(up_limit);
		bbzero(down_limit);
		bbzero(icData);
		bbzero(sndData);
		
		wrLog("【R_CPU_GET_READCARDINFO】");
		
		len = 2;
		cmd = 0x79; 
		
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			cpurspStatus = rcvData[3];
			wrLog("cpurspStatus:%d",cpurspStatus);
			if( rcvData[3]==0x00)
			{
				BYTE mny[4+1];	
				CTools::byte_to_hex((BYTE *)rcvData+4,(BYTE *)pyct_data->pki_card,4);//pki管理卡号
				CTools::byte_to_hex((BYTE *)rcvData+8,(BYTE *)pyct_data->ticket_physicalNo,8);//物理卡号
				CTools::byte_to_hex((BYTE *)rcvData+16,(BYTE *)pyct_data->ticket_no,8); //逻辑卡号
				CTools::byte_to_hex((BYTE *)rcvData+24,(BYTE *)mny,4); //钱包余额
				
				if (flag == 0)
				{
					//圈存前
					//	getAsiAmo3(rcvData+24, 4, ); //取得以分为单位的十进制金额
					CTools::HexToDec((char *)mny, pyct_data->ticket_amo_bef);
				}
				else if (flag ==1)
				{
					//圈存后
					//CTools::byte_to_hex((BYTE *)rcvData+24,(BYTE *)pyct_data->ticket_amo_aft,4); //钱包余额
					//	getAsiAmo3(rcvData+24, 4, pyct_data->ticket_amo_aft); //取得以分为单位的十进制金额
					CTools::HexToDec((char *)mny, pyct_data->ticket_amo_aft);
				}
				bbzero(pyct_data->downLimit);
				bbzero(pyct_data->upperLimit);
				CTools::byte_to_hex((BYTE *)rcvData+28,(BYTE *)down_limit,1); //钱包下限
				//memcpy(pyct_data->upperLimit,down_limit,1);
				memcpy(pyct_data->downLimit,rcvData+28,1);
				//CTools::byte_to_hex((BYTE *)rcvData+29,(BYTE *)up_limit,3);//羊城通金额上限
				//memcpy(pyct_data->upperLimit,up_limit,3);
				memcpy(pyct_data->upperLimit,rcvData+29,3);
				//CTools::byte_to_hex((BYTE *)rcvData+32,(BYTE *)pyct_data->card_status,1);// 卡片状态
				pyct_data->card_status=rcvData[32];
				//CTools::byte_to_hex((BYTE *)rcvData+33,(BYTE *)count_info,16); //计数信息
				memcpy(count_info,rcvData+33,16);//计数信息
				CTools::byte_to_hex((BYTE *)rcvData+49,(BYTE *)pyct_data->SW1SW2,2); //卡片返回状态码
				CTools::HexToDec((char *)down_limit,des_lim);
				
				switch (pyct_data->card_status) //返回卡片的状态
				{
				case 0x00:
					wrLog("M1卡");
					break;
				case 0x01:
					wrLog("cpu卡，M1功能未关闭");
					break;
				case 0x02:
					wrLog("cpu卡，M1已经关闭");
					break;
				case 0x03:
					wrLog("cpu卡，M1应用已经锁定");
					break;
					
				}
				
				wrLog("读取cpu卡卡信息如下：");
				wrLog("管理卡号:%s",pyct_data->pki_card);
			//	wrLog("逻辑卡号:%s",pyct_data->ticket_no);
				
				wrLog("钱包余额: %s",mny);
				if (flag == 0)
				{
					wrLog("圈存前卡片余额为(以分为单位的十进制金额): %s",pyct_data->ticket_amo_bef);//羊城通余额
				}
				else if (flag == 1)
				{
					wrLog("圈存后卡片余额为(以分为单位的十进制金额): %s",pyct_data->ticket_amo_aft);//羊城通余额
				}	
				wrHex("卡片上限: ",(BYTE *)pyct_data->upperLimit,3);		//羊城通cpu卡上限
				wrHex("卡片下限为：",(BYTE *)pyct_data->downLimit,1); //羊城通cpu卡下限
				wrHex("计数信息：",count_info,16);
				wrLog("羊城通卡卡状态:%s",pyct_data->SW1SW2); //羊城通返回状态码
				
				NULL;
			}
			if (rcvData[3]==0x80 || rcvData[3]==147)
			{
					TS_OutPutMsg(dPosInfo, "\n请把卡放到读卡器上\n   按【确认】返回", 0,iSelect);
			}
		}
			//else 见读卡器错误定义
			else
			{
				wrLog("充值失败"); 		
			}
			
		
	}
	
	
	//黑名单卡处理函数 至黑名单卡初始化 R_SET_BLACKLIST_INIT
	
	int  CYCTYPTrade::R_SET_BLACKLIST_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data )
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		BYTE SW1SW2[2];
		
		wrLog("【R_SET_BLACKLIST_INIT】");
		bbzero(icData);
		bbzero(sndData);
		
		len = 2;
		cmd = 0x21; 
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)		
		{
			if( rcvData[3]==0x00)
			{
				
				bbzero(pyct_data->ticket_physicalNo); //物理卡号
				bbzero(pyct_data->ticket_no);  //逻辑卡号
				
				CTools::byte_to_hex((BYTE *) rcvData+4,(BYTE *)pyct_data->ticket_physicalNo,8);
				CTools::byte_to_hex((BYTE *) rcvData+12,(BYTE *)pyct_data->ticket_no,8);
				CTools::byte_to_hex((BYTE *) rcvData+20,(BYTE *)pyct_data->BLACKLIST_CARDINFO,16); //黑名单
				CTools::byte_to_hex((BYTE *) rcvData+36,(BYTE *)pyct_data->SW1SW2,2); 
				memcpy(SW1SW2,(BYTE *) rcvData+36,2);
				
				wrLog("黑名单卡的物理卡号:%s",pyct_data->ticket_physicalNo);
				wrLog("黑名单的逻辑卡号:%s",pyct_data->ticket_no);
				wrLog("黑名单初始化卡状态:%s",pyct_data->SW1SW2);
				
				NULL;
			}
			//else 见读卡器错误定义
			
			else
			{
				return -1;
				
			}
			
		}
		
		return 0;
		
	}
	
	
	//P_SET_BLACKLIST() // 黑名单 返回trade_info3
	
	int CYCTYPTrade::P_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,\
		BYTE * trade_info3, BYTE * rsp,BYTE * party,BYTE* rspcode)		
	{		
		BYTE sndData[255]; 
		BYTE rcvData[255];


		int len;
		int iRet=0;
		BYTE pack_data[255];

		int sndLen=0;//发送报文长度
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[20];

		int out=0;
		char yct_sequence_NO[8+1];
		BYTE rsp_code[4+1];

		wrLog("【P_SET_BLACKLIST】");
		bbzero(sndData);
		bbzero(rcvData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(yct_sequence_NO);
		bbzero(rsp_code);
		bbzero(pack_data);
		
		len=0;//报文体长度

		CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //管理卡号
		len+=4;
		CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
		len+=8;
		CTools::hex_to_byte((BYTE* )pyct_data->ticket_no,pack_data+len,16); //逻辑卡号
		len+=8;
		CTools::hex_to_byte((BYTE*)pyct_data->BLACKLIST_CARDINFO,pack_data+len,32); //黑名单卡信息
		wrHex("黑名单卡信息: ",pack_data+len,16);
		len+=16;
		memcpy(pack_data+len,RFU,20);
		len+=20;

		iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB05" , (BYTE*)"0001" , 0x02 , 0x80 , \
			pack_data , len , rcvData , &rcvLen);
		if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			return 3;// 3：与羊城通交互失败
		}
		
		len = 8;
		wrHex("握手流水号: ",rcvData + len,8);
		len+=8;
		//memcpy(pyct_data->yct_stan,rcvData + len,8);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)pyct_data->yct_stan,8);//系统流水号
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
			char Output_Msg[1024];
			int iSelect = 0;
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

		bbzero(trade_info3);
		memcpy(trade_info3,rcvData+len,32);
		wrHex("交易信息3: ",trade_info3,32);
		len += 32;
		

		return 0;
		
	}
	
	
	//黑名单卡处理函数  设置黑名单卡R_SET_BLACKList
	
	int  CYCTYPTrade::R_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,BYTE *SW1SW2,BYTE *status)
	{
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;

		wrLog("【R_SET_BLACKLIST】");
		bbzero(icData);
		bbzero(sndData);
		bbzero(SW1SW2);
		bbzero(icData);
		
		len = 2;
		cmd = 0x22; 
		memcpy(icData,trade_info,32);
		len+=32;
		
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			status[0]=rcvData[3]; //返回读卡器状态
	
			if( rcvData[3]==0x00)
			{			
				memcpy(SW1SW2,(BYTE *) rcvData+4,2);
				bbzero(pyct_data->SW1SW2);
				memcpy(pyct_data->SW1SW2,(BYTE *) rcvData+4,2);
			}
			//else 见读卡器错误定义
			else
			{
				return -1;		
			}	
		}
		
		return 0;		
	}
	
	
	//P_BLACKLIST_SUBMIT // 黑名单提交
	
	int CYCTYPTrade::P_BLACKLIST_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,\
		BYTE *status, BYTE * SW1SW2 ,BYTE * party,BYTE* rspcode)
	{
		BYTE sndData [255];
		BYTE rcvData[255];


		int len;
		int iRet=0;
		BYTE pack_data[255];

		int sndLen=0;//发送报文长度
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[20];

		int out=0;
		BYTE rsp_code[4+1];

		wrLog("【P_BLACKLIST_SUBMIT】");
		bbzero(sndData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(rcvData);
		bbzero(rsp_code);
		
		len=0;//报文体长度

		bbzero(pack_data);		
		CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //管理卡号
		len+=4;
		CTools::hex_to_byte((BYTE *) pyct_data->yct_stan,pack_data+len ,16); //对应AB06报文中的系统流水号
		wrHex("系统流水号: ",pack_data+len,8);
		len+=8;
		CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
		len+=8;
		CTools::hex_to_byte((BYTE* )pyct_data->ticket_no,pack_data+len,16); //逻辑卡号
		len+=8;
		memcpy(pack_data+len,(char *)SW1SW2,2); //读卡器指令置黑名单卡R_SET_BLACKLIST返回的SW1SW2
		len+=2;
	//	memcpy(pack_data+len,(char *)status,1); //读卡器返回返回的指令status 
	//	CTools::hex_to_byte((BYTE *)status,pack_data+len,2);
		pack_data[len]=status[0];
		len+=1;
		memcpy(pack_data+len,RFU,25); //RFU
		len+=25;

		iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB07" , (BYTE*)"0001" , 0x02 , 0x80 , \
			pack_data , len , rcvData , &rcvLen);
		if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			return 3;// 3：与羊城通交互失败
		}
		
		len = 8;
		wrHex("握手流水号: ",rcvData + len,8);
		len+=8;
		memcpy(pyct_data->yct_stan,rcvData + len,8);
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
		bbzero(pyct_data->note);		
		memcpy(pyct_data->note,rcvData + len,32);
		wrHex("备注: ",(BYTE *)pyct_data->note,32);
		len+=32;	
		return 0;
	}
	
	//执行黑名单流程
	int CYCTYPTrade:: EXCUTE_BLACKLIST(const TSDLLPosInfo dPosInfo,const int com,const int comType, PYCT_DATA pyct_data,\
		BYTE * query_info,BYTE *party, BYTE *rspcode)
	{
		BYTE rsp_message [48]; //查询信息
		BYTE trade_info3[32];
		BYTE SW1SW2 [2];
		BYTE status[1+1];
		char *rsp = {0};
		int iRet=0;
		
		bbzero(trade_info3);
		bbzero(rsp_message);
		bbzero(status);

		R_PUB_SET_READCARDINFO(dPosInfo,com,comType,pyct_data->ticket_no,query_info,pyct_data->SW1SW2);
		iRet = R_SET_BLACKLIST_INIT(dPosInfo,com,comType,pyct_data); //对黑名单卡初始化
		if (iRet != 0)
		{
			char Msg[512];
			get_cardReader_errMsg((BYTE*)pyct_data->status,Msg);
			wrLog("读卡器错误: %s",Msg);
			*party = FROMCTSI;
			*rspcode = BLACKLIST;
			return 0;	
		}

		iRet = P_SET_BLACKLIST(dPosInfo,com,comType,pyct_data,trade_info3,(BYTE *)rsp,party,rspcode); 
		if (iRet != 0)
		{
			*party = FROMCTSI;
			*rspcode = BLACKLIST;
			return 0;	
		}

		wrLog("继续做后面的流程");
			
		R_SET_BLACKLIST(dPosInfo,com,comType,pyct_data,trade_info3,SW1SW2,status); 
			
		//默认提交三次重复提交的方式，如果三次都不成功，在下次开机，圈存或者撤销的时候，再次提交
		for(int i =0;i<3;i++)
		{		
		   iRet=P_BLACKLIST_SUBMIT(dPosInfo,com,comType,pyct_data,status,SW1SW2,party,rspcode);			
			if (iRet==0)
			{	*party = FROMCTSI;
				*rspcode = BLACKLIST;			
				return 0;		
			}			
		}
		
		if (iRet != 0 )
		{
			//在下次开机、操作圈存或圈存撤销时，再次自动提交。
			pyct_data->status = 0;
		}
// 		else if(strcmp(rsp,"AA010001")==0)
// 		{
// 			
// 			wrLog("已经执行成功，不用继续后续的流程");
// 			return 0;
// 			
// 		}	
		*party = FROMCTSI;
		*rspcode = BLACKLIST;
		return 0;	
	}
	
	
	
	//终端向读卡器发送卡片同步查询的操作
	//rsp_message 是读卡器返回的数据
	
	int  CYCTYPTrade::R_CARD_QRY_UPGRADE(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data, const int com, const int comType,BYTE *random_num,BYTE * rsp_message)
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		bbzero(icData);
		bbzero(sndData);
		
		wrLog("【R_CARD_QRY_UPGRADE】");
		len = 2;
		cmd = 0x25; 
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			cpurspStatus = rcvData[3];
			if( rcvData[3]==0x00)
			{
				len = 4;
				wrHex("PKI管理卡号：",rcvData+len,4);
				len += 4;
				wrHex("物理卡号：",rcvData+len,8);
				CTools::byte_to_hex((BYTE*)rcvData+len,(BYTE*)pyct_data->ticket_physicalNo,8);
				len += 8;
				//wrHex("逻辑卡号：",rcvData+len,8);
				len += 8;
				wrHex("随机数：",rcvData+len,8);
				CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)random_num,8);
				len += 8;
				CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_message,200);
				wrHex("卡信息：",rcvData+len,200);
			}
			//else 见读卡器错误定义		
			else
			{
				return -1;
				
			}
			
		}
		
		return 0;
		
	}
	
	//实现cpu卡同步
	int CYCTYPTrade::P_M1_TO_CPU(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, \
		BYTE * upgrade_info, int  &next_status,BYTE *syn_order,BYTE * party,BYTE* rspcode)		
	{
		
		BYTE sndData [1024];
		BYTE rcvData[1024];
		int len;
		int iRet=0;
		BYTE pack_data[255];
		BYTE rsp[8];
		int sndLen=0;//发送报文长度
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[36];
		BYTE temp_len[10];
		int out=0;
		SOCKET sd=0;
		CConnect con;
		char  yct_sequence_NO[8+1]; //报文序号
		char error_info[128];
		char byte_errinfo[128];
		int iSelect = 0;
		char outmsg[512];
		
		bbzero(sndData);
		bbzero(rcvData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(temp_len);
		bbzero(yct_sequence_NO);
		
		wrLog("【P_M1_TO_CPU】");
		len=0;//报文体长度
		
		bbzero(pack_data);

		CTools::hex_to_byte((BYTE * )pyct_data->pki_card,pack_data+len,8);        //PKI管理卡号
		len+=4;
		CTools::hex_to_byte((BYTE * )pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
		len+=8;
		CTools::hex_to_byte((BYTE * )pyct_data->ticket_no,pack_data+len,16);      //逻辑卡号
		len+=8;
		CTools::hex_to_byte((BYTE * )pyct_data->random_num,pack_data+len,16);  //随机数
		len+=8;
		//memcpy(pack_data,upgrade_info,208);
		CTools::hex_to_byte((BYTE * )upgrade_info,pack_data+len,400);  //卡信息
		len+=200;
		memcpy(pack_data+len,RFU,36);
		len+=36;
		

		iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB01" , (BYTE*)"0001" , 0x02 , 0x80 , \
			pack_data , len , rcvData , &rcvLen);
		if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			return 3;// 3：与羊城通交互失败
		}

		len = 8;
		wrHex("握手流水号: ",rcvData + len,8);
		len+=8;
		memcpy(pyct_data->yct_stan,rcvData + len,8);
		wrHex("返回系统流水号: ",rcvData + len,8);
		len+=8;
		CTools::byte_to_hex((BYTE *) rcvData+len,rsp,4);
		wrLog("返回码：%s",rsp);
		len+=4;

		if(strcmp((char *)rsp,"00000000")==0)
		{		
			next_status=rcvData[len];
			len += 1;
			memcpy(syn_order,rcvData+len,64);//同步指令
			wrHex("同步指令：",syn_order,64);
			len+=64;
			wrHex("备注：",rcvData+len,80);
			len += 80;				
		}
/*		
		else if(strcmp((char *)rsp,"AA01AB01")==0)
		{
					
			wrLog("有未还的自行车记录，同步操作终止");
			TS_OutPutMsg(dPosInfo,"有未还的自行车记录，同步操作终止!",1,iSelect);
			return -1;
		}				
		else if(strcmp((char *)rsp,"AA01AB02"))		
		{
			wrLog("有未还的咪表记录，同步操作终止");
			TS_OutPutMsg(dPosInfo,"有未还的咪表记录，同步操作终止!",1,iSelect);
			return -2;		
		}
*/		
		else if(strcmp((char *)rsp,"AA01F204")==0)
		{
			return -1;
		}
		else
		{
			bbzero(error_info);
			bbzero(byte_errinfo);
			CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//错误信息描述
			CTools::tobyte(error_info,byte_errinfo);
			CTools::trim(byte_errinfo);
			wrLog("错误信息描述: %s",byte_errinfo);	
			bbzero(outmsg);
			sprintf(outmsg,"充值后台返回错误\n\n错误信息描述：%s",byte_errinfo);
			TS_OutPutMsg(dPosInfo,outmsg,1,iSelect);
			return 1;
		}

	return 0;		
}
	
	
	//卡同步应用
	int  CYCTYPTrade::R_APP_CARD_UPGRADE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE* upgrade_info)
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		BYTE SW1SW2[2];
		
		bbzero(icData);
		bbzero(sndData);
		bbzero(SW1SW2);
		wrLog("【R_APP_CARD_UPGRADE】");
		len = 2;
		cmd = 0x26; 
		memcpy(icData,upgrade_info,64);
		len+=64;
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			pyct_data->status = rcvData[3];
			if( rcvData[3]==0x00)
			{
				bbzero(pyct_data->SW1SW2);
				CTools::byte_to_hex((BYTE *)rcvData+4, (BYTE * )pyct_data->SW1SW2, 2);//卡状态码
				
				NULL;
			}
			//else 见读卡器错误定义
			
			else
			{
				return -1;		
			}
			
		}
		
		return 0;
		
	}
	
	
	int  CYCTYPTrade::EXCUTE_M1_TO_CPU(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE *query_info, BYTE *party,BYTE *rspcode)
	{

		int next_status = 0x00;
		char SW1SW2[2+1];
		BYTE rsp_message[400+1];
		BYTE random_num[8+1];
		BYTE syn_order[64+1];
		char physical_no[30+1];
		int iRet=0;
		int iSelect = 0;
        
		wrLog("【EXCUTE_M1_TO_CPU】");

		bbzero(syn_order);
		bbzero(SW1SW2);
		bbzero(pyct_data->ticket_amo_aft);
		bbzero(pyct_data->result_message);
		bbzero(physical_no);
		//strcpy(physical_no,pyct_data->ticket_no);
		wrHex("query_info: ",query_info,48);
	    iRet = R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,query_info,SW1SW2);



		bbzero(rsp_message);
		bbzero(random_num);
		iRet=R_CARD_QRY_UPGRADE(dPosInfo,pyct_data,com,comType,random_num,rsp_message);
		if (cpurspStatus == 147 || cpurspStatus==0x80)
		{
			TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上!",1,iSelect);
			for (int i=0; i<2; i++)
			{
				iRet=R_CARD_QRY_UPGRADE(dPosInfo,pyct_data,com,comType,random_num,rsp_message);
				if (cpurspStatus == 0)
				{
					break;
				}
				TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上!",1,iSelect);
			}
		}
		if (iRet != 0)
		{
			*party = FROMCTSI;
			*rspcode = TRANSFER_ERROR;
			return 1;
		}

		strcpy(pyct_data->random_num,(char *)random_num);
		iRet=P_M1_TO_CPU(dPosInfo,com,comType,pyct_data,rsp_message,next_status,syn_order,party,rspcode);
		if (iRet == -1 )
		{
			return 3;
		}
		//wrHex("后续操作：",(BYTE*)next_status,1);
		if(iRet==0)
		{
			switch (next_status)
			{		
				
			case 0x00:				
				wrLog("卡片同步及余额转移已经完成,请继续充值");
				TS_OutPutMsg(dPosInfo,"同步完成！按enter继续充值",1,iSelect);
				break;
				
			case 0x01:				
				wrLog("还有后续卡片同步操作");
				M1_CPU = 1;
				//先执行同步指令
				iRet = R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //卡应用同步
				if (iRet != 0)
				{
					//执行同步指令失败则执行充值类型查询
					TS_OutPutMsg(dPosInfo,"请把卡放在读卡器上!",1,iSelect);
					char errMsg[1024];
					bbzero(errMsg);
					get_cardReader_errMsg((BYTE *)pyct_data->status,errMsg);
					wrLog("读卡器返回错误信息：%s",errMsg);
					*party = FROMIC;
					*rspcode = R_APP_CARD_UPGRADE_EXEFAIL;
//					return 1;
				}
				iRet = EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
				break;
				
			case 0x02:	
				wrLog("卡片同步已经完成，请终端发起M1余额转移");//
//				R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //卡应用同步
				iRet = R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //卡应用同步
				if (iRet != 0)
				{
					//执行同步指令失败则执行充值类型查询
					TS_OutPutMsg(dPosInfo,"请把卡放在读卡器上!",1,iSelect);
					char errMsg[1024];
					bbzero(errMsg);
					get_cardReader_errMsg((BYTE *)pyct_data->status,errMsg);
					wrLog("读卡器返回错误信息：%s",errMsg);
					*party = FROMIC;
					*rspcode = R_APP_CARD_UPGRADE_EXEFAIL;
//					return 1;
				}

				iRet =	CPU_EnqPay(dPosInfo,com,comType,0,pyct_data,"000000000000",0x10,query_info,party,rspcode);
				bbzero(pyct_data->ticket_amo_bef);
				strcpy(pyct_data->ticket_amo_bef,pyct_data->ticket_amo_aft);
				bbzero(pyct_data->ticket_amo_aft);
				wrLog("卡片同步及余额转移已经完成,请继续充值");
				TS_OutPutMsg(dPosInfo,"同步完成！按enter继续充值",1,iSelect);
				break;
				
			case 0x03:			
				wrLog("卡片同步未完成，异常退出");
				TS_OutPutMsg(dPosInfo,"卡片同步未完成，异常退出!",1,iSelect);
				*party = FROMCTSI;
				*rspcode = TRANSFER_ERROR;
				return 1;			
			default:			
				wrLog("未知原因，异常退出 %d",next_status);
				*party = FROMCTSI;
				*rspcode = TRANSFER_ERROR;
				return 1;			
			}
			
			return 0;			
		}
		else
		{
			wrLog("卡片同步未完成，异常退出！");
			*party = FROMCTSI;
			*rspcode = TRANSFER_ERROR;
			return 1;
		}	
	}
	
	
	
	//进行M1 圈存查询
	//appType:0x00为商户圈存，0x01 为自主圈存.0x20:充值金账户充值;0x30为卡间余额转移;0x40 为异常信息提交;0x50 :扩展应用
	
	int CYCTYPTrade::P_M1_LOAD_QRY(TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, \
		BYTE * trade_money,BYTE *pysical_info1,int appType,BYTE * query_info,BYTE * party,BYTE* rspcode)
	{
		
		BYTE sndData [255];
		BYTE rcvData[255];
		BYTE type[2];
		BYTE ver[2];
		int len;
		int iRet=0;
		BYTE pack_data[255];
		BYTE md5_digest[16+1];
		BYTE rsp[8];
		int sndLen=0;//发送报文长度
		int rcvLen = 0; //接收报文
		BYTE EN_SHID[16];
		BYTE RFU[23];
		BYTE MAC[4+1];
		int out=0;
		SOCKET sd;
		CConnect con;
		BYTE yct_sequence_NO[8];
		BYTE outside_info[16];
		BYTE  expand_serial[8];
		BYTE charge_password[16]; //充值密码
		BYTE hex_amoney[5];
		BYTE len_tmp[4];
		char error_info[125];
		char byte_errinfo[125];
		int iSelect;
		TSTradeInfoSend tradeInfoSend;
		TSTradeInfoRecv tradeInfoRecv;
		memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
		memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
		
		tradeInfoSend.uHasTradeID = 1;
		tradeInfoSend.uTotalContents = 1;
		
		
		bbzero(sndData);
		bbzero(rcvData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(outside_info);
		bbzero(charge_password);
		bbzero(expand_serial);
		bbzero(hex_amoney);
		bbzero(error_info);
		bbzero(byte_errinfo);
		
		len=0;//报文体长度
		sndLen=0;
		
		CTools::hex_to_byte((BYTE *)"AA61",type,4);
		CTools::hex_to_byte((BYTE *)"0001",ver,4);
		
		memcpy(sndData,type,2); //信息类型码
		memcpy(sndData+2,ver,2); //报文版本号
		sndData[4]=0x02; //报文体加密算法
		sndData[5]=0x80; //数值内容字节顺序
		sndLen+=6;
		
		bbzero(pack_data);
		
		CTools::hex_to_byte((BYTE *) pyct_data->SHID,pack_data,16);  //SHID
		len+=8;
		sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
		CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //报文序号//有终端进行管理，每次上传给服务器报文+1
		len+=4;
		CTools::hex_to_byte((BYTE *)pyct_data->pki_card,pack_data+len,8);//PKI 管理卡号
		len+=4;
		CTools::hex_to_byte((BYTE *) pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
		len+=8;
		CTools::hex_to_byte((BYTE *) pyct_data->ticket_no,pack_data+len,16); //逻辑卡号
		len+=8;
		memcpy(pack_data+len,pysical_info1,32);
		len+=32;
		
		pack_data[len]=appType; //应用类型 
		len+=1;
	//	CTools::hex_to_byte((BYTE *)trade_money,pack_data+len,8);
	    getHexAmo((const char *)trade_money,hex_amoney);
		memcpy(pack_data+len,hex_amoney,4);
		
		//CTools::hex_to_byte((BYTE *)trade_money,(BYTE *)pack_data+len,8);//交易金额 ，测试充值1分钱.对于交易30 或者40 填入的充值金额是无效的。
		len+=4;

		if (appType != 0x01)
		{
			CTools::hex_to_byte((BYTE *)"000000000000000000000000000000000000000000000000",(BYTE *) pack_data+len,32);
		}
		else
		{
			CTools::hex_to_byte((BYTE *)pyct_data->card_no,(BYTE *) pack_data+len,32); //自主圈存时必须输入扣款的银行账号
		}

		len+=16;
		CTools::hex_to_byte((BYTE * ) pyct_data->stan,outside_info,16);//添加系统流水号，方便对账
		memcpy(pack_data+len,outside_info,16);//外部系统附加信息外部信息中添加12位系统参考号
		len+=16;
		
		wrHex("outside_info",outside_info,16);
		
		//进行拓展流水.应用类型为0x50 或者0x30的时候，填入扩展应用或者卡转值的流水号，非0x50和0x30全部填入0x00
		if (appType==0x50)
		{
			//填入扩展应用
			memcpy(pack_data+len,pyct_data->extension_stan,8); //扩展流水号
		}		
		else if (appType==0x30)
		{
			//填入卡转的流水号
			memcpy(pack_data+len,pyct_data->transfer_stan,8); //卡转值流水号
		}
		else
		{
			memcpy(pack_data+len,expand_serial,8); //扩展流水号
		}

		len+=8;
		
		if ((bank_signed == 1) && (atoi(ornotpassword)!=0))
		{
			strcpy(tradeInfoSend.Contents[0].acTitle, "请输入银行签约的密码：");	
			tradeInfoSend.Contents[0].uCommand = TS_INPUT_PWD;	//密码
			tradeInfoSend.Contents[0].uCardType = CARD_TYPE;//卡加密
			tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME;//超时30秒
			tradeInfoSend.Contents[0].uMinLen = 6;
			tradeInfoSend.Contents[0].uMaxLen = 6;
			iRet = TS_TradeControl(&dPosInfo, &tradeInfoSend, &tradeInfoRecv);
			//		char charge_sig[16];
			if(iRet != 0)
			{
				wrLog("TS_TradeControl返回错误[%d]", iRet);
				TS_OutPutMsg(dPosInfo, "TS_TradeControl返回错误\n   按【确认】返回", 0,iSelect);
				//		wrMonitorLog(1, 1, iRet, gbl, pack);	//监控日志
				return iRet;
			}
			
			md5_csum((BYTE *)tradeInfoRecv.Contents[0].uMsg,tradeInfoRecv.Contents[0].uMsgLen,charge_password);
			wrHex("charge_password 加密后 :",charge_password,16);
			
		}

//		bank_signed = 0;
		memcpy(pack_data+len,charge_password,16); //充值密码
		len+=16;
		memcpy(pack_data+len,RFU,23); //RFU
		len+=23;
		wrHex("RFU",RFU,23);
		
		//计算mac
		bbzero(md5_digest);
		md5_csum(pack_data,len,md5_digest);
		memcpy(MAC,md5_digest,4); //校验码
		memcpy(pack_data+len,MAC,4);
		len+=4;
		wrLog("len:%d",len);
		wrHex("aa61 MAC",MAC,4);
		wrHex("aa61 pack_data",pack_data,len);
		
		sprintf((char *)len_tmp,"%04X",len);
		CTools::hex_to_byte((BYTE *)len_tmp,sndData+sndLen,4);//报文体长度
		sndLen+=2;
		CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //使用3des对SHID
		CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16); 
		sndLen+=8;
		
		BYTE *enrpt_packdata=AESEncryptEx(pack_data+8,144,pyct_data->SKEY,16,&out); //使用sk通过AES加密
		wrHex("aa61需要加密的数据:",pack_data+8,144);
		
		wrHex("aa61:",enrpt_packdata,out);
		
		memcpy(sndData+sndLen,enrpt_packdata,out);
		sndLen+=out;
		
		//羊城通建立连接
		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3：与羊城通交互失败
		}
		
		bbzero(rcvData);
		iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //将数据发送给羊城通
		
		if( iRet==0  )
		{
			
			BYTE mac_this[4];
			BYTE decrpt_Data[255];
			
			int packLen=0;
			BYTE back_time[14];
			
			bbzero(back_time);
			bbzero(MAC);
			bbzero(EN_SHID);
			bbzero(pyct_data->SHID);
			
			//解密SHID
			CTools::byte_to_hex((BYTE *)rcvData+8,(BYTE *)EN_SHID,8);
			
			CDes::TripleDesDec((char *)CKEY,(char* )EN_SHID, (char *)pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_Data,16);
			packLen+=8;
			
			BYTE * aes_rcvData=AESDecryptEx((BYTE *) rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out); //除了握手流水号之外的报文体内容
			wrHex("aa62 aes_rcvData",aes_rcvData,out);
			memcpy(decrpt_Data+packLen,aes_rcvData,out-4);
			packLen+=out-4;
			memcpy(mac_this,aes_rcvData+(out-4),4); //获取mac_this
			
			//进行mac校验
			bbzero(md5_digest);
			md5_csum(decrpt_Data,packLen,md5_digest);
			memcpy(MAC,md5_digest,4); //校验码
			
			if(memcmp((char*)mac_this,(char *)MAC,4)==0)
			{
				
				wrLog("MAC校验成功");
				bbzero(pyct_data->yct_stan);
				bbzero(rspcode);
				
				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)pyct_data->yct_stan,8); //返回系统流水号
				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)pyct_data->yct_serial,8); //返回系统流水号
				wrLog("羊城通系统流水号:%s",pyct_data->yct_stan);
				CTools::byte_to_hex((BYTE *) aes_rcvData+8,rsp,4); //如果返回码不是0x00000000 ，后面是错误信息描述
				if (strcmp((char *)rsp,"00000000")==0)
				{
					CTools::byte_to_hex((BYTE *) aes_rcvData+12,back_time,7); //后台时间
					memcpy(query_info,aes_rcvData+19,48); // 查询信息
					
					wrHex("query_info",query_info,48);
					wrLog("充值后台时间为：%s",back_time);
					
				}
				else
				{
					if ((strcmp((char *)rsp,"AA01E202")==0) && (bank_signed == 1))
					{
						TS_OutPutMsg(dPosInfo, "账户密码错误，请确认后重试!\n\n\n", 0,iSelect);
					}
					CTools::byte_to_hex((BYTE *)aes_rcvData+12,(BYTE *)error_info,50);//错误信息描述
					CTools::tobyte(error_info,byte_errinfo);
					CTools::trim(byte_errinfo);
					TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				    wrLog("错误信息描述: %s",byte_errinfo);	
					return 3;
				}
				
			}
			
		}
		else if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			closesocket(sd);
			return 3;// 3：与羊城通交互失败
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			closesocket(sd);
			return 1;// 1：与读卡器交互失败
		}
		
		return 0;
		
	}
	
	
	//M1取卡信息
	
	int  CYCTYPTrade::R_M1_GET_CARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,int &rspStatus,int flag)
		
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		BYTE trade_count[4];
		BYTE mny[8];
		int iSelect = 0;
		
		wrLog("【R_M1_GET_CARDINFO】");
		bbzero(icData);
		bbzero(sndData);
		bbzero(trade_count);
		
		len = 2;
		cmd = 0xB9; 
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			rspStatus=rcvData[3];
			wrLog("rspStatus:%d",rspStatus);
			if( rcvData[3]==0x00)
			{
				bbzero(pyct_data->pki_card);
				bbzero(pyct_data->ticket_no);
				CTools::byte_to_hex((BYTE *)rcvData+4,(BYTE *)pyct_data->pki_card,4); 
				CTools::byte_to_hex((BYTE *) rcvData+8,(BYTE *)pyct_data->ticket_physicalNo,8);// 物理卡号
				CTools::byte_to_hex((BYTE *) rcvData+16,(BYTE *)pyct_data->ticket_no,8); // 逻辑卡号
				memcpy(trade_info,(char *)rcvData+24,88); //交易信息
				
				wrHex("trade_info:",trade_info,88);
				CTools::byte_to_hex((BYTE *)rcvData+112,(BYTE *)mny,4);// 卡片余额pyct_data->ticket_amo_bef
				if (flag == 0)
				{
					bbzero(pyct_data->ticket_amo_bef);
                 	getAsiAmo3(rcvData+112, 4, pyct_data->ticket_amo_bef); //取得以分为单位的十进制金额
					wrLog("羊城通票卡余额ticket_amo:[%s]", pyct_data->ticket_amo_bef);//羊城通票卡余额
				}
			    else if (flag == 1)
			    {
					bbzero(pyct_data->ticket_amo_aft);
                    getAsiAmo3(rcvData+112, 4, pyct_data->ticket_amo_aft); //取得以分为单位的十进制金额
					wrLog("羊城通票卡余额ticket_amo:[%s]", pyct_data->ticket_amo_aft);//羊城通票卡余额
			    }
				
				CTools::byte_to_hex((BYTE *)rcvData+116,trade_count,2);
				wrLog("PKI管理卡号为:%s",pyct_data->pki_card);
				wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
				//wrLog("逻辑卡号:%s",pyct_data->ticket_no);
				
				
				wrLog("M1卡片余额是%s",mny); 
				return 0;
				
			}
			//else 见读卡器错误定义
			else
			{
				return -1;			
			}
			
		}
		else
		{
			return -1;
		}
		
		return 0;
		
	}
	
	
	//M1卡进行圈存
	int CYCTYPTrade::P_M1_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_info,int next_action,BYTE * charge_info,BYTE * party,BYTE* rspcode)
		
	{
		
		BYTE sndData [255];
		BYTE rcvData[255];
		BYTE type[2];
		BYTE ver[2];
		int len;
		int iRet=0;
		BYTE pack_data[255];
		BYTE md5_digest[16+1];
		BYTE rsp[8];
		int sndLen=0;//发送报文长度
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[28+1];
		BYTE MAC[4+1];
		BYTE yct_sequence_NO[8];
		int out=0;
		BYTE len_tmp[4];
		
		SOCKET sd=0;
		CConnect con;
		int iSelect;
		bbzero(sndData);
		bbzero(rcvData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(charge_info);
		bbzero(yct_sequence_NO);
		
		len=0;//报文体长度
		
		CTools::hex_to_byte((BYTE *)"AA63",type,4);
		CTools::hex_to_byte((BYTE *)"0001",ver,4);
		
		memcpy(sndData,type,2); //信息类型码
		memcpy(sndData+2,ver,2); //报文版本号
		sndData[4]=0x02; //报文体加密算法
		sndData[5]=0x80; //数值内容字节顺序
		sndLen+=6;
		
		bbzero(pack_data);
		
		CTools::hex_to_byte((BYTE *) pyct_data->SHID,pack_data,16);  //SHID
		len+=8;
		sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
		CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //报文序号//有终端进行管理，每次上传给服务器报文+1
		len+=4;
		CTools::hex_to_byte((BYTE *)pyct_data->pki_card,pack_data+len,8);//PKI 管理卡号
		len+=4;
		CTools::hex_to_byte((BYTE *) pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
		wrLog("物理卡号：%s",pyct_data->ticket_physicalNo);
		len+=8;
		CTools::hex_to_byte((BYTE *) pyct_data->yct_stan,pack_data+len,16) ; //系统流水号
		wrLog("羊城通系统流水号:%s",pyct_data->yct_stan);
		len+=8;
		memcpy(pack_data+len,trade_info,88); //交易信息
		wrHex("交易信息trade_info",trade_info,88);
		len+=88;
		memcpy(pack_data+len,RFU,28);
		len+=28;
		//计算mac
		bbzero(md5_digest);
		md5_csum(pack_data,len,md5_digest);
		memcpy(MAC,md5_digest,4); //校验码
		
		memcpy(pack_data+len,MAC,4);
		len+=4;
		wrHex("AA63 数据包加密之前:",pack_data+8,len-8);
		
		sprintf((char *)len_tmp,"%04X",len);
		wrLog("len_tmp:%s",len_tmp);
		CTools::hex_to_byte((BYTE *)len_tmp,sndData+sndLen,4);//报文体长度
		sndLen+=2;
		CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //使用3des对SHID
		CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16); 
		sndLen+=8;
		BYTE *enrpt_packdata=AESEncryptEx(pack_data+8,144,pyct_data->SKEY,16,&out); //使用sk通过AES加密
		
		memcpy(sndData+sndLen,enrpt_packdata,out);
		sndLen+=out;
		wrHex("aa63 数据包加密之后：",sndData,sndLen);
		
		//羊城通建立连接
		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3：与羊城通交互失败
		}
		
		iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //将数据发送给羊城通
		
		if( iRet==0  )
		{
			
			BYTE mac_this[4];
			//BYTE md5_digest[16];
			BYTE decrpt_Data[255];
			bbzero(MAC);
			bbzero(EN_SHID);
			int packLen=0;
			BYTE trade_time[14]; //充值时间
			
			bbzero(trade_time);
			bbzero(decrpt_Data);
			bbzero(md5_digest);
			bbzero(MAC);
			bbzero(mac_this);
			
			//CTools::byte_to_hex((BYTE *) rcvData+6,(BYTE *)packLen,2); //报文体长度
			
			//解密SHID
			CTools::byte_to_hex((BYTE *)rcvData+8,(BYTE *)EN_SHID,8);
			CDes::TripleDesDec((char *)CKEY,(char* )EN_SHID, (char *)pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_Data,16);
			packLen+=8;
			
			BYTE * aes_rcvData=AESDecryptEx((BYTE *) rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out); //除了握手流水号之外的报文体内容
			wrHex("aes_rcvData",aes_rcvData,out);
			memcpy(decrpt_Data+packLen,aes_rcvData,out-4);
			packLen+=out-4;
			
			memcpy(mac_this,aes_rcvData+(out-4),4);
			wrHex("aa64",mac_this,4);
			
			//进行mac校验
			bbzero(md5_digest);
			md5_csum(decrpt_Data,packLen,md5_digest);
			memcpy(MAC,md5_digest,4); //校验码
			wrHex("aa64 mac",MAC,4);

			if(memcmp((char*)mac_this,(char *)MAC,4)==0)
			{
				
				wrLog("MAC校验成功");
				bbzero(pyct_data->yct_stan);
				bbzero(pyct_data->tradeTime);
				bbzero(pyct_data->TAC);
				bbzero(pyct_data->yct_termid);
				bbzero(pyct_data->trader_id);
				bbzero(pyct_data->trader_name);
				bbzero(pyct_data->out_logicalcard);
				bbzero(pyct_data->note);
				bbzero(rspcode);
				
				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)pyct_data->yct_stan,8); //返回系统流水号
				wrLog("系统参考号:%s",pyct_data->yct_stan);
				CTools::byte_to_hex((BYTE *) aes_rcvData+8,rsp,4); //如果返回码不是0x00000000 ，后面是错误信息描述
				wrLog("rsp:%s",rsp);
				if (strcmp((char *)rsp,"00000000")==0)
				{
					
					memcpy(charge_info,aes_rcvData+12,24); //充值信息
					CTools::byte_to_hex(aes_rcvData+36,(BYTE *)pyct_data->tradeTime,7);//交易日期时间
					CTools::byte_to_hex(aes_rcvData+43,(BYTE *)pyct_data->charge_authcode,4); //充值凭证号
					CTools::byte_to_hex(aes_rcvData+47,(BYTE *)pyct_data->yct_termid,6); // 羊城通终端编号
					CTools::byte_to_hex(aes_rcvData+53,(BYTE *)pyct_data->trader_id,8) ; //商户编号
					CTools::byte_to_hex(aes_rcvData+61,(BYTE *)pyct_data->trader_name,20); //商户简称
					CTools::byte_to_hex(aes_rcvData+81,(BYTE *)pyct_data->out_logicalcard,8) ;//转出卡逻辑卡号 //0x30 卡余额转移的时候有效
					CTools::byte_to_hex(aes_rcvData+89,(BYTE *)pyct_data->note,64); //备注
					
					wrHex("aa64 返回充值信息",charge_info,24);
					wrLog("aa64交易时间:%s",pyct_data->tradeTime);
					wrLog("aa64 充值凭证号:%s",pyct_data->charge_authcode);
					wrLog("aa64 羊城通终端编号:%s",pyct_data->yct_termid);
					wrLog("aa64 羊城通商户编号:%s",pyct_data->trader_id);
					wrLog("aa64 羊城通商户简称：%s",pyct_data->trader_name);
					//wrLog("aa64 转出卡逻辑卡号：%s",pyct_data->out_logicalcard);
					wrLog("aa64 备注:%s",pyct_data->note);
					
					next_action=aes_rcvData[153]; //后续操作
					
					if(next_action==0x00)
					{
						wrLog("操作已经完成");
						return 0;
					}
					
					else
					{
						wrLog("还有相关的操作，需要执行拓展应用的操作");
						//ExtendApp(gbl,)
					}
					
				}
				else
				{
					char error_info[128];
					char byte_errinfo[128];
					wrLog("交易返回码:%s",rsp);
					if (memcmp(rsp,"AA01F001",8) == 0)
					{
						//充值金额违法业务规则
						*party = FROMYCT_O;
						*rspcode = AMOUNTRULE_VIALATION;	
					}
					else if (memcmp(rsp,"AA01F002",8) == 0)
					{
						//充值后余额超上限
						*party = FROMYCT_O;
						*rspcode = AMOUNTRULE_VIALATION;	
					}
					else if (strcmp((char *)rsp,"AA01FD22")==0)
					{
						TS_OutPutMsg(dPosInfo," \n 账户余额不足!\n请按【确认】返回",0,iSelect);
					}
					bbzero(error_info);
					bbzero(byte_errinfo);
					CTools::byte_to_hex((BYTE *)aes_rcvData+12,(BYTE *)error_info,50);//错误信息描述
					CTools::tobyte(error_info,byte_errinfo);
					CTools::trim(byte_errinfo);
					wrLog("错误信息描述: %s",byte_errinfo);
					
					//TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				    return 1;
				}
				
			}
			
		}
		
		else if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			closesocket(sd);
			return 3;// 3：与羊城通交互失败
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			closesocket(sd);
			return 1;// 1：与读卡器交互失败
		}
		
		return 0;
		
	}
	
	
	//对卡进行R_M1_LOAD
	int  CYCTYPTrade::R_M1_LOAD(const TSDLLPosInfo dPosInfo, const int com, int &status,const int comType,PYCT_DATA pyct_data,BYTE * charge_info)
		{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		bbzero(icData);
		bbzero(sndData);
		bbzero(icData);
		
		len = 2;
		cmd = 0xBA; 
		CTools::hex_to_byte((BYTE *) pyct_data->tradeTime,icData,14); //交易时间
		len+=7;
		memcpy(icData+7,charge_info,24); //充值信息
		len+=24;
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("下发数据", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
		{

			status=rcvData[3]; //返回读卡器状态
			if( rcvData[3]==0x00)	
			{
				
				BYTE mny[4+1];
				int  tradeType;
				char trade_count[2+1];
				bbzero(mny);
				
				bbzero(pyct_data->ticket_amo_aft);
				wrLog("M1充值成功");
				memcpy(mny,rcvData+4,4); //余额
				getAsiAmo3(rcvData+4, 4, pyct_data->ticket_amo_aft); //取得以分为单位的十进制金额,高位在前
				tradeType=rcvData[8];
				
				wrLog("tradeType:%d",tradeType); //交易类型
				memcpy(trade_count,rcvData+9,2); //羊城通票卡计数
				pyct_data->trade_count=atoi(trade_count);
				
				wrLog("羊城通票卡计数:%d",pyct_data->trade_count);
				wrLog("充值之后的余额为:%s",pyct_data->ticket_amo_aft);	
				wrHex("充值之之后的票卡余额为",mny,4); 
				
				NULL;
			}
			//else 见读卡器错误定义
			
			else
			{
				return -1;
				
			}
			
		}
		else
			return -1; //读卡器没有返回数据
		
		return 0;
	}
	
	
	//M1卡冲正
	int CYCTYPTrade:: P_M1_ROLLBACK(const TSDLLPosInfo dPosInfo, const int com, const int comType, int status,PYCT_DATA pyct_data,char* reason,char *aa62_stan,BYTE *money,BYTE * TAC,BYTE * trade_info,int next_action,BYTE *party,BYTE *rspcode)
		
	{
		BYTE sndData [255];
		BYTE rcvData[255];
		BYTE type[2];
		BYTE ver[2];
		int len;
		int iRet=0;
		BYTE pack_data[255];
		BYTE md5_digest[16+1];
		int sndLen=0;//发送报文长度
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[27];
		BYTE MAC[10+1];
		BYTE temlen[5];
		char yct_sequence_NO[4+1];
		char hex_status[4];
		BYTE hex_amount[10];
		BYTE error_message[100];
		char message[100];
		char temp_buffer[1024];
		int out=0;
		SOCKET sd=0;
		CConnect con;
		bbzero(sndData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(rcvData);
		bbzero(temlen);
		bbzero(hex_status);
		bbzero(hex_amount);
		bbzero(error_message);
		bbzero(message);
		bbzero(temp_buffer);
		len=0;//报文体长度
		
		CTools::hex_to_byte((BYTE *)"AA65",type,4);
		CTools::hex_to_byte((BYTE *)"0001",ver,4);
		
		memcpy(sndData,type,2); //信息类型码
		memcpy(sndData+2,ver,2); //报文版本号
		sndData[4]=0x02; //报文体加密算法
		sndData[5]=0x80; //数值内容字节顺序
		sndLen+=6;
		
		bbzero(pack_data);
		
		CTools::hex_to_byte((BYTE *) pyct_data->SHID,pack_data,16);  //SHID
		len+=8;
		sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
		CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //报文序号//有终端进行管理，每次上传给服务器报文+1
		len+=4;
		CTools::hex_to_byte((BYTE *) aa62_stan,pack_data+len,16); //对应AA62报文中的系统流水号
		len+=8;
		CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //管理卡号
		len+=4;
		CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
		len+=8;
		CTools::hex_to_byte((BYTE* )pyct_data->ticket_no,pack_data+len,16); //逻辑卡号
		len+=8;
		CTools::hex_to_byte((BYTE *)reason,pack_data+len,2); //冲正原因 其中 01：充值写卡失败；02：未收到AA62报文或者未向读卡器发起充值指令
		len+=1;
		sprintf(hex_status,"%02X",status);
		CTools::hex_to_byte((BYTE *)hex_status,pack_data+len,2);//读卡器指令R_M1_LOAD返回的status结果
		len+=1;
		getHexAmo((const char*)money,hex_amount);
		memcpy((char*)pack_data+len,hex_amount,4);
		//CTools::hex_to_byte((BYTE *)money,pack_data+len,4);
		len+=4;
		CTools::hex_to_byte((BYTE *)pyct_data->charge_authcode,pack_data+len,8);//充值凭证号
		//memcpy(pack_data+len,TAC,4); 
		len+=4;
		memcpy(pack_data+len,trade_info,88);
		len+=88;
		memcpy(pack_data+len,RFU,26);
		len+=26;
		//计算mac
		md5_csum(pack_data,len,md5_digest);
		memcpy(MAC,md5_digest,4); //校验码
		memcpy(pack_data+len,MAC,4); //取校验码前四位
		len+=4;
		
		sprintf((char *)temlen,"%04X",len);
		CTools::hex_to_byte((BYTE *)temlen,sndData+6,4);//报文体长度
		sndLen+=2;
		strcpy(temp_buffer,(char*)sndData);
		CTools::byte_to_hex((BYTE*)sndData,(BYTE *)temp_buffer,8);
		CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //使用3des对SHID
		CTools::hex_to_byte((BYTE *)EN_SHID,sndData+8,16); 
		sndLen+=8;
		strcat(temp_buffer,(char *)pyct_data->SHID);
		//strcat(temp_buffer,(char*)pack_data+8);
		CTools::byte_to_hex((BYTE*)pack_data+8,(BYTE *)temp_buffer + 32,len-8);
		CDB::updateYctRollback(pyct_data,temp_buffer);

		BYTE *enrpt_packdata=AESEncryptEx(pack_data+8,160,pyct_data->SKEY,16,&out); //使用SKEY通过AES 加密
		wrHex("aa65需要加密的数据:",pack_data+8,160);
		wrHex("aa65:",enrpt_packdata,out);
		memcpy(sndData+sndLen,enrpt_packdata,out);
		sndLen+=out;
		bbzero(pyct_data->result_message);
		CTools::byte_to_hex((BYTE*)sndData,(BYTE *)pyct_data->result_message,sndLen);
		
//		memcpy(pyct_data->result_message,temp_buffer + 32,160);
		wrLog("pyct_data->result_message ----------:%s",pyct_data->result_message);

		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3：与羊城通交互失败
		}
		
		iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data,com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //将数据发送给羊城通
		
		if( iRet==0  )
		{
			BYTE mac_this[10];
			BYTE decrpt_Data[255];
		    int packLen=0;

			bbzero(MAC);
			bbzero(EN_SHID);
			bbzero(decrpt_Data);
			
			//报文解密及进行MAC校验
			
			//解密SHID
			CTools::byte_to_hex((BYTE *)rcvData+8,EN_SHID,16);
			
			CDes::TripleDesDec((char *)CKEY,(char *)EN_SHID,(char *)pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_Data,16);
			packLen+=8;
			
			BYTE *des_rcvData=AESDecryptEx((BYTE *) rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out);; //解密报文体的内容
			wrHex("des_rcvData",des_rcvData,out);
			memcpy(decrpt_Data+packLen,des_rcvData,out-4);
			packLen+=out-4;
			memcpy(MAC,des_rcvData+(out-4),4);//校验码
			//CTools::byte_to_hex((BYTE*)des_rcvData+152,(BYTE *)MAC,4);
			//进行mac校验
			bbzero(md5_digest);
			md5_csum(decrpt_Data,packLen,md5_digest);
			memcpy(mac_this,md5_digest,4); //校验码
			wrHex("mac_this",mac_this,4);
			wrHex("aa66 mac",MAC,4);
			if(memcmp((char*)mac_this,(char *)MAC,4)==0)
			{
				
				wrLog("MAC校验成功");
//				bbzero(pyct_data->yct_stan);
//				CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE *)pyct_data->yct_stan,8); //返回系统流水号
				wrLog("--------:%s",pyct_data->yct_stan);
				CTools::byte_to_hex((BYTE *) des_rcvData+8,(BYTE *)pyct_data->SW1SW2,4);//返回码 
				if (memcmp(pyct_data->SW1SW2,"00000000",8) != 0)
				{
                    memcpy(error_message,des_rcvData+12,50);
					wrLog("返回的错误信息：%s",error_message);
					CDB::updateYctuntxn(pyct_data,aa62_stan);
				}
				else
				{
//					TS_OutPutMsg(dPosInfo,"冲正成功",0,iSelect);
					wrLog("冲正成功");
					pyct_data->status=5;
					CDB::updateYctuntxn(pyct_data,aa62_stan);
				}
				CTools::byte_to_hex((BYTE *) des_rcvData,(BYTE *)pyct_data->note,48);//备注信息
				next_action=des_rcvData[60];
				
			}
			
			else
				
			{
				
				wrLog("MAC 校验失败");
			}
			
		}

		else if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			closesocket(sd);
			return 3;// 3：与羊城通交互失败
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			closesocket(sd);
			return 1;// 1：与读卡器交互失败
		}
		
		
		return 0;
	}
	
	
	//执行M1钱包的圈存
	int CYCTYPTrade::excute_M1_LOAD(const TSDLLPosInfo dPosInfo,const int com,const int comType,PYCT_DATA pyct_data,\
		int enq,BYTE *party,BYTE *rspcode,char *tradeamount,int payType)
	{
		int iRet=0,iDb=0;
		int readCard_iRet=0; //读取卡片的返回信息
		BYTE pysical_info [51]; //卡片物理信息
		BYTE card_pycicalInfo1[32];
		BYTE query_info[48];
		BYTE trade_info[88];
		BYTE charge_info[24];
		BYTE SW1SW2[2];
		BYTE SAK[1];
		BYTE TAC[5];
		char czamount[20];
		char limit_money[20];
		BYTE sign_status[20];
		char temp[256];
		char txn_serial[10];
		char bef_balance[20];
		char aft_balance[20];
		char reason[5];
		char aa62_stan[20];

		int next_action = 0;
		int rspStatus;
		int status;
		int iSelect;
		int flag;
		M1_CPU = 1;
		bbzero(limit_money);
		bbzero(sign_status);
		bbzero(temp);
		bbzero(czamount);
		bbzero(pysical_info);
		bbzero(card_pycicalInfo1);
		bbzero(query_info);
		bbzero(charge_info);
		bbzero(TAC);
		bbzero(txn_serial);
		bbzero(bef_balance);
		bbzero(aft_balance);
		bbzero(reason);
		bbzero(aa62_stan);
		CTools::getYct_posSerial(txn_serial);
		//strcpy(pyct_data->term_serial,txn_serial);
		//4.终端向读卡器重新发送查询卡物理信息的指令
		iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType, pysical_info);
		if (iRet == 1)
		{
			TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",0,iSelect);
			for (int i = 0; i<2; i++)
			{
				iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType, pysical_info);
				if (iRet == 0)
				{
					break;
				}
				TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",0,iSelect);
			}
		}
		if(iRet != 0)
		{
			*party=FROMIC;
			*rspcode=ICTIMEOUT;
			TS_OutPutMsg(dPosInfo,"获取卡片的物理信息发生错误",0,iSelect);
			return 1;
		}
		
		if(iRet==0)
		{
			
			bbzero(SW1SW2);
			CTools::byte_to_hex((BYTE *)pysical_info,(BYTE *)pyct_data->ticket_physicalNo,8);//获取物理卡号
			
			CTools::byte_to_hex((BYTE *)pysical_info+8,(BYTE *)pyct_data->ticket_no,8);//取得逻辑卡号
			memcpy(SAK,pysical_info+16,1); //sak
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
				TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
				*party = FROMIC;
				*rspcode = ERROR_FROMIC;
				return 10;// 1：与读卡器交互失败
			}

			wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
		//	wrLog("逻辑卡号:%s",pyct_data->ticket_no);
			CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
			wrHex("SAK",SAK,1);
			wrHex("card_pycicalInfo1",card_pycicalInfo1,32);
			
		}

		
		//pe
		
		//5.终端向充值服务器发送M1充值查询请求报文
		iRet=P_M1_LOAD_QRY(dPosInfo,com,comType,pyct_data,(BYTE *)tradeamount,card_pycicalInfo1,payType,query_info,party,rspcode);//P_M1_LOAD_QRY 圈存查询(商户圈存)
		
		if(iRet==0)
		{
			flag=0;
			bbzero(SW1SW2);
			//如果报文返回是成功，向读卡器发送指令R_PUB_SET_READCARDINFO
			iRet=R_PUB_SET_READCARDINFO(dPosInfo,com,comType,pyct_data->ticket_no,query_info,(char *)SW1SW2); //
            if (iRet == -1)
			{
				*party=FROMIC;
				*rspcode=ICSAYFAIL;
				return 10;
			}
			else if (iRet ==1)
			{
               	*party=FROMIC;
				*rspcode=ICTIMEOUT;
				return 10;
			}
			iRet=R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);//6.终端向读卡器发送M取卡信息指令]
			if (rspStatus == 128 || rspStatus == 132)
			{
				TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
				for (int i = 0;i<2; i++)
				{
					iRet=R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);
					
					if (rspStatus != 128 && rspStatus != 132)
					{
						break;
					}
					TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
				}	
			}
			if (iRet == -1)
			{
				char errMsg[1024];
				bbzero(errMsg);
				get_cardReader_errMsg((BYTE *)rspStatus,errMsg);
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				*party=FROMIC;
				*rspcode=ERROR_FROMIC;
				return 10;
			}
			else if (iRet ==1)
			{
               	*party=FROMIC;
				*rspcode=ICTIMEOUT;
				return 10;
			}

			if (enq == 1)
			{
				return 0;
			}			
			if (payType == 0x20)
			{
				sprintf(temp,"卡号:%s\n\n充值前卡余额:%.2f元\n\n按【Enter】充值  按【Esc】返回",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100);
				TS_OutPutMsg(dPosInfo,temp,0,iSelect);
				if (iSelect)
				{
					*party=FROMCTSI_N;
					*rspcode=USERCANCEL;
					return 5;
				}
			}
			if (payType == 0x22)
			{
//				char bef[12+1];
			//	sprintf(bef,"\n\n%s元",pyct_data->ticket_amo_bef);
				sprintf(temp,"\n\n充值前卡余额:%.2f元\n\n按【Enter】充值  按【Esc】返回",atof(pyct_data->ticket_amo_bef)/100);
				strcat(auto_show,temp);
				TS_OutPutMsg(dPosInfo,auto_show,0,iSelect);
				if (iSelect)
				{
					*party=FROMCTSI_N;
					*rspcode=USERCANCEL;
					return 5;
				}
			}
			
			strcpy(aa62_stan,pyct_data->yct_stan);
            strcpy(bef_balance,pyct_data->ticket_amo_bef);
			bbzero(pyct_data->ticket_amo_aft);
			bbzero(pyct_data->result_message);
			//PE扣费
			CTools::getDate(pyct_data->txn_date);
		    CTools::getTime(pyct_data->txn_time);
			pyct_data->status=0;
			strcpy(pyct_data->biz_type,"0056");
			//strcpy(pyct_data->bank,"000000");
			//strcpy(pyct_data->term_serial,txn_serial);
			strcpy(pyct_data->yct_termid,dPosInfo.pos_no);
			strcpy(pyct_data->amount,tradeamount);
		//	iDb=CDB::insetYctYPtxnDATA(pyct_data,aa62_stan,0);
			//进行M1卡圈存
			iRet=P_M1_LOAD(dPosInfo,com,comType,pyct_data,trade_info,next_action,charge_info,party,rspcode); //7.服务器进行M1圈存
			if (iRet != 0) 
			{
				//向羊城通读卡器发送回滚报文
				strcpy(reason,"02");
				for (int i=0;i<3;i++)
				{
					iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa62_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
					if (iRet == 0)
					{
						break; 
					}
				}

				if (iRet !=0 || i>=3) //如果羊城通冲正失败  
				{
					pyct_data->yct_ack=0; //需要向羊城通服务器发送确认报文
					*party=FROMIC;
					*rspcode=ICSAYFAIL;
					return 1;
				}

				return 1;

			}

		   	iRet=R_M1_LOAD(dPosInfo,com,status,comType,pyct_data,charge_info); //8.终端向充值服务器返回的成功报文后，向读卡器发送圈存指令，
			if (iRet==0 && status==0) //圈存成功
			{
				bbzero(bef_balance);
				bbzero(aft_balance);
				strcpy(bef_balance,pyct_data->ticket_amo_bef) ;
				strcpy(aft_balance,pyct_data->ticket_amo_aft);
			}

			//充值之后三次读卡 通过读卡器的值来确定充值是否成功。
			for (int i = 0;i<3; i++)
				
			{
					readCard_iRet=R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,1);
					
					if (readCard_iRet==0 && rspStatus == 0 )  //如果读卡成功
					{
						bbzero(bef_balance);
						bbzero(aft_balance);
						strcpy(bef_balance,pyct_data->ticket_amo_bef) ;
						strcpy(aft_balance,pyct_data->ticket_amo_aft);
						break; 
					} 
					else
					{
					
						if (rspStatus == 128 || rspStatus == 132)
						{
							TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
						}

						continue;

					}
				
			}
			

			if (iRet ==0 || readCard_iRet == 0 ) //如果读卡和圈存有返回
			{

				if (atoi(aft_balance)== atoi(bef_balance) + atoi(tradeamount))
				{
					pyct_data->status=2;
					//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
					wrLog("充值成功");
					*party = FROMSUCC;
					*rspcode = SUCC_MSG;
				}

				else 
				{
					strcpy(reason,"01");
					//以下进行冲正操作;
					
					for (int i=0;i<3;i++)
					{
						iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa62_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
						if (iRet == 0)
						{
							break;
						}
					}
					if (iRet !=0 || i>=3)
					{
						pyct_data->status=0;
						*party=FROMIC;
						*rspcode=ICSAYFAIL;
						return 1;
					}
//					TS_OutPutMsg(dPosInfo,"充值失败",1,iSelect);
					*party=FROMIC;
					*rspcode=CONNCOMFAIL;
					return 5; 
				}

			}
			/*
			else if (status!=0 ) //如果圈存明确失败 读卡失败
			{

				strcpy(reason,"01");
				//以下进行冲正操作;
				
				for (int i=0;i<3;i++)
				{
					iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa62_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
					if (iRet == 0)
					{
						break;
					}
				}
				if (iRet !=0 || i>=3)
				{
					pyct_data->status=0;
					*party=FROMIC;
					*rspcode=ICSAYFAIL;
					return 1;
				}
				//					TS_OutPutMsg(dPosInfo,"充值失败",1,iSelect);
				*party=FROMIC;
				*rspcode=CONNCOMFAIL;
					return 5; 

			}

			*/
			else  //默认成功
			{
				moni = 1;
				pyct_data->status=2;
				//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
				wrLog("充值成功");
				*party = FROMSUCC;
				*rspcode = SUCC_MSG;
			}

			//8.执行完R_M1_LOAD指令需要重新读取卡片的余额。执行R_M1_GET_CARDINFO指令

/*			if(rspStatus ==0x00)
			{
				flag=1;
				R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);
				wrLog("rspStatus %d",rspStatus);
				if(rspStatus ==0x00)
				{
					flag=0;
					//重新读取充值金账户
					strcpy(aft_balance,pyct_data->ticket_amo_aft);
					if (atoi(aft_balance)== atoi(bef_balance) + atoi(tradeamount))
					{
                        pyct_data->status=2;
						//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
						wrLog("充值成功");
						*party = FROMSUCC;
						*rspcode = SUCC_MSG;
					}
				}
			}
*//*			for(int i=0;i<3;i++)
			{
				flag=1;
				R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);
				wrLog("rspStatus %d",rspStatus);
				if(rspStatus ==0x00)
				{
					flag=0;
					//重新读取充值金账户
					strcpy(aft_balance,pyct_data->ticket_amo_aft);
					if (atoi(aft_balance)== atoi(bef_balance) + atoi(tradeamount))
					{
                        pyct_data->status=2;
						//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
						wrLog("充值成功");
						*party = FROMSUCC;
						*rspcode = SUCC_MSG;
					    break;
					}
			        else
					{
                        wrLog("圈存后，写卡余额前后不正确，进行冲正");
                        pyct_data->status=4;
						//CDB::updateYctuntxn(pyct_data,aa62_stan);
						strcpy(reason,"01");
						//以下进行冲正操作;
						for (int i=0;i<3;i++)
						{
							iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa62_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
							if (iRet == 0)
							{
								break;
							}
						}
						if (iRet !=0)
						{
							pyct_data->status=0;
							//CDB::updateYctuntxn(pyct_data,aa62_stan);
						}
						*party=FROMIC;
						*rspcode=CONNCOMFAIL;
					    return 1; 
					}
				}
				else if(rspStatus==0x80)
				{
					wrLog("请重新放好卡");	
				}		
				else 
				{
		//			wrLog("圈存失败，进行冲正");
		//			pyct_data->status=4;
		//			CDB::updateYctuntxn(pyct_data);
		//			strcpy(reason,"02");
		//			//以下进行冲正操作;
		//			for (int i=0;i<3;i++)
		//			{
		//				iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa65_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
		//				if (iRet == 0)
		//				{
		//					break;
		//				}
		//			}

					*party=FROMIC;
					*rspcode=CONNCOMFAIL;
					return 5;                //疑似成功		
				}
			}
*/
/*			if (flag != 0)
			{
				wrLog("M1卡三次读卡失败");
				//向羊城通后台发起冲正
				strcpy(reason,"01");
				for (int i=0;i<3;i++)
				{
					iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa62_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
					if (iRet == 0)
					{
						break;
					}
				}
				if (iRet !=0)
				{
					pyct_data->status=0;
				}


				*party=FROMIC;
				*rspcode=CONNCOMFAIL;
				return 5;                //充值失败	
			}
*/
		}
		else
		{
			TS_OutPutMsg(dPosInfo,"充值失败",1,iSelect);
			return 1;
			
		}		
		return 0;	
	}
	
	/**********************************************************************/
	/*                             CPU                                    */


//CPU圈存初始化
int  CYCTYPTrade::R_CPU_LOAD_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *trade_message1,BYTE *trade_sequence,BYTE *yct_term_status,BYTE *card_info)
{
	int len=0,cmd=0,sndLen,iRet,rcvLen=0;
	BYTE icData[255],sndData[255],rcvData[255];
	int baudRate=57600;
	BYTE SW1SW2[2];
	bbzero(icData);
	bbzero(sndData);
	bbzero(icData);
	
	wrLog("【R_CPU_LOAD_INIT】");
	
	len = 2;
	cmd = 0x7A; 
	//CTools::byte_to_hex((BYTE *)trade_message1,icData,128);
	memcpy(icData,trade_message1,64);
	len+=64;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		CTools::byte_to_hex((BYTE *)rcvData+3,yct_term_status,1); //羊城通终端状态
		wrHex("操作结果 :",rcvData+3,1);
		if( rcvData[3]==0x00)
		{
			bbzero(SW1SW2);
			CTools::byte_to_hex(rcvData+4,trade_sequence,2); //联机交易序号
			
			memcpy(card_info,rcvData+6,80);//卡信息
			CTools::byte_to_hex((BYTE *)rcvData+86,(BYTE *)pyct_data->SW1SW2,2);//卡状态信息
			
			wrLog("联机交易序号:%s",trade_sequence);
			wrHex("卡信息: ",card_info,80);
			wrLog("CPU圈存返回卡状态为:%s",pyct_data->SW1SW2);
			
			NULL;
		}
		//else 见读卡器错误定义		
		else
		{
			wrLog("读卡器错误，错误代码 [%s], 请查阅文档中错误定义！",yct_term_status);
			// 			*party = FROMCTSI;
			// 			*rspcode = ICTIMEOUT;
			// 			closesocket(sd);
			// 			return 1;// 1：与读卡器交互失败
		}
	}
		return 0;
}//R_CPU_LOAD_INIT()

	
	//CPU圈存查询
	int  CYCTYPTrade::P_CPU_LOAD_QRY( TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE *count_info,\
		PYCT_DATA pyct_data, const char * amount,BYTE appType,\
		BYTE * trade_message1,BYTE *party,BYTE* rspcode)
	{
		int iRet,sndLen,rcvLen,len = 0;
		SOCKET sd;
		BYTE pack_data [256];
		BYTE decrpt_data[512];
		BYTE  sndData[512];
		BYTE charge_password[16];
		BYTE type [2]; //信息类型码
		BYTE ver [2]; //报文版本号
		char MAC[4];
		BYTE mny[12+1];
		BYTE RFU[22+1];
		BYTE rcvData[512];
		BYTE EN_SHID[16+1];
		int out=0;	
		int baudRate=57600;
		BYTE outside_info[16];//外部系统附加信息
		BYTE tuozhan_stan[8];
		char yct_sequence_NO[4+1];
		BYTE len_tmp[2];
		CConnect con;
	    BYTE hexAmo[10];
		BYTE *enrpt_packdata;
		BYTE md5_digest[16] = { 0 };
		int iSelect;

		wrLog("【P_CPU_LOAD_QRY】");
		TSTradeInfoSend tradeInfoSend;
		TSTradeInfoRecv tradeInfoRecv;
		memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
		memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
		
		tradeInfoSend.uHasTradeID = 1;
		tradeInfoSend.uTotalContents = 1;
	bbzero(sndData);
	bbzero(rcvData);
	bbzero(mny);
	bbzero(charge_password);
	bbzero(RFU);
	bbzero(tuozhan_stan);
	bbzero(outside_info);
	bbzero(hexAmo);
	//bbzero(EN_SHID);
	
	sndLen=0;//发送报文长度
	rcvLen = 0;
	
	len=0;//报文体长度
	
	CTools::hex_to_byte((BYTE *)"AA81",type,4);
	CTools::hex_to_byte((BYTE *)"0001",ver,4);
	memcpy(sndData,type,2); //信息类型码
	memcpy(sndData+2,ver,2); //报文版本号
	
	sndData[4]=0x02; //报文体加密算法
	sndData[5]=0x80; //数值内容字节顺序
	sndLen+=6;
	
	bbzero(pack_data);
	bbzero(MAC);
	bbzero(trade_message1);
	
	//memcpy(pack_data,pyct_data->SHID,8); //握手流水号
	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //握手流水号
	len+=8;
	//sprintf(yct_sequence_NO,"%04x",pyct_data->yct_sequence_NO); //报文序号
	sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
	CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //报文序号//有终端进行管理，每次上传给服务器报文+1
	//CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,4);
	//memcpy(pack_data+len,yct_sequence_NO,4);
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card,pack_data+len,8); //pki管理卡号
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
	len+=8;
	
	CTools::hex_to_byte((BYTE*) pyct_data->ticket_no,pack_data+len,16);//逻辑卡号
	len+=8;
	pack_data[len]=pyct_data->card_status; //卡片状态
	len+=1;
	wrHex("count_info :",count_info,16);
	memcpy(pack_data+len,count_info,32); //取卡器指令cpu取卡信息返回的数据
	len+=16;
	
    wrLog("appType : %d",(BYTE *)appType,1);
	pack_data[len]=appType; //商户圈存
	len+=1;

	if (appType == 0x10 || appType == 0x30 || appType == 40)
	{
		getHexAmo("000000000000", hexAmo);		
	}
	else 
	{
		getHexAmo(amount, hexAmo);
	}
    wrHex("hexAmo :",hexAmo,4);
    memcpy(pack_data+len,hexAmo,4);//圈存金额	
  
	len+=4;
	
	if (appType != 0x01)
	{
		CTools::hex_to_byte((BYTE *)"000000000000000000000000000000000000000000000000",(BYTE *) pack_data+len,32);
	}
    else
	{
		CTools::hex_to_byte((BYTE *)pyct_data->card_no,(BYTE *) pack_data+len,32); //自主圈存时必须输入扣款的银行账号
	}
	
	len+=16;
	//memcpy(outside_info,pyct_data->stan,16); 
	CTools::hex_to_byte((BYTE * ) pyct_data->stan,outside_info,16);//添加系统流水号，方便对账
	wrHex("outside_info :",outside_info,16);
	memcpy(pack_data+len,outside_info,16); //外部系统附加信息。合作商户可以录入操作员编号及合作商户的交易流水号等信息。数据保存在对账文件中
	len+=16;
	

	//进行拓展流水.应用类型为0x50 或者0x30的时候，填入扩展应用或者卡转值的流水号，非0x50和0x30全部填入0x00
	if (appType==0x50)
	{
		//填入扩展应用
		wrHex("pyct_data->extension_stan :",(BYTE *)pyct_data->extension_stan,8);
		memcpy(pack_data+len,pyct_data->extension_stan,8); //扩展流水号
	}		
	else if (appType==0x30)
	{
		//填入卡转的流水号
		wrHex("pyct_data->transfer_stan :",(BYTE *)pyct_data->transfer_stan,8);
		memcpy(pack_data+len,pyct_data->transfer_stan,8); //卡转值流水号
	}
	else
	{
		CTools::hex_to_byte((BYTE *)"0000000000000000",(BYTE *) pack_data+len,16);//非0X50 和0x30该域应该全填0x00
	}
	len+=8;
//	wrLog("---------------------------%d",atoi(ornotpassword));
	if ((bank_signed == 1) && (atoi(ornotpassword)!=0))
	{
		strcpy(tradeInfoSend.Contents[0].acTitle, "请输入银行签约的密码：");	
		tradeInfoSend.Contents[0].uCommand = TS_INPUT_PWD;	//密码
		tradeInfoSend.Contents[0].uCardType = CARD_TYPE;//卡加密
		tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME;//超时30秒
		tradeInfoSend.Contents[0].uMinLen = 6;
		tradeInfoSend.Contents[0].uMaxLen = 6;
		iRet = TS_TradeControl(&dPosInfo, &tradeInfoSend, &tradeInfoRecv);
//		char charge_sig[16];
		if(iRet != 0)
		{
			wrLog("TS_TradeControl返回错误[%d]", iRet);
			TS_OutPutMsg(dPosInfo, "TS_TradeControl返回错误\n   按【确认】返回", 0,iSelect);
			//		wrMonitorLog(1, 1, iRet, gbl, pack);	//监控日志
			return iRet;
		}
		bbzero(charge_password);
	
//		char temp[16];
//		strcpy(temp,(char *)tradeInfoRecv.Contents[0].uMsg);
//		strupr(temp);
		//转为十六进制
//		CTools::DecToHex(temp, charge_sig,16);
		md5_csum((BYTE *)tradeInfoRecv.Contents[0].uMsg,tradeInfoRecv.Contents[0].uMsgLen,charge_password);
		wrHex("charge_password 加密后 :",charge_password,16);

	}

//	bank_signed = 0;

//	memcpy(charge_password,(char *)"888888",16);
	memcpy(pack_data+len,charge_password,16);// 充值密码
	len+=16;
	wrHex("RFU :",RFU,22);
	memcpy(pack_data+len,RFU,22); //默认为0x00
	len+=22;

	//计算mac
	//get_md5(pack_data,md5_digest);
	wrHex("报文体（没有MAC）:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4); //校验码
	memcpy(pack_data+len,MAC,4); //取校验码前四位
	len+=4;
	wrHex("mac",(BYTE *)MAC,4);
	wrHex("pack_data:",(BYTE *)pack_data,len);
	
	//SHID 使用3DES加密;
	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID);
	wrLog("加密前的SHID: %s",pyct_data->SHID);
	wrLog("加密后的SHID: %s",EN_SHID);

	enrpt_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,pyct_data->SKEY,16,&out); //除握手流水号以外报文使用sk通过AES加密

	wrHex("AA81需要加密的数据 :",pack_data+8,len-8);
	wrHex("AA81加密后的数据   :",enrpt_packdata,out);
	
	wrLog("报文体总长度: %d",len);
	sprintf((char *)len_tmp,"%04x",len);
	//memcpy(sndData,len_tmp,2); //报文体长度
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //报文体长度
	sndLen+=2;
	CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16);
	sndLen+=8;
	memcpy(sndData+sndLen,enrpt_packdata,out);//发送数据
	sndLen+=out;

//	md5_csum(decrpt_data,sndLen,md5_digest); //进行md5运算

	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}

	iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data,com, comType, sd, sndData,sndLen, rcvData, &rcvLen); //将数据发送给羊城通
	
	if( iRet==0  )
	{
		
		BYTE md5_this[16];
		BYTE mac_this[4];
		bbzero(pack_data);
		bbzero(MAC);
		bbzero(md5_digest);
		bbzero(md5_this);
		int packLen=0;
		out = 0;

		CTools::byte_to_hex((BYTE *)rcvData+8,EN_SHID,8);
		bbzero(pyct_data->SHID);
		CDes::TripleDesDec((char * )CKEY,(char *)EN_SHID,(char *)pyct_data->SHID);//握手流水号
		wrLog("握手流水号是:%s",pyct_data->SHID);
		CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
		packLen+=8;
		
		BYTE *des_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,(BYTE *)pyct_data->SKEY,16,&out); //报文体使用SKEY进行解密
		memcpy(decrpt_data+packLen,des_rcvData,out-4); //
		packLen+=out-4;
		
		wrHex("解密报文体（除握手流水号）",des_rcvData,out);
		memcpy(mac_this,des_rcvData+(out-4),4); //获取MAC
		
		//需要计算md5的数据：
		wrHex("decrpt_data",decrpt_data,packLen);
		//进行mac校验
		bbzero(md5_digest);
		md5_csum(decrpt_data,packLen,md5_digest); //进行md5运算
		memcpy(MAC,md5_digest,4); //校验码
		wrHex("MAC",(BYTE *)MAC,4);
		wrHex("mac_this",mac_this,4);
		
		if(memcmp(MAC,mac_this,4)==0)
		{
			char rsp_code[8+1];
			BYTE sys_time[14+1];
			BYTE NOTE [96+1];
			char error_info[128];
			char byte_errinfo[128];
			
			wrLog("MAC校验成功");

			CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE*)pyct_data->yct_stan,8); //羊城通充值流水号
			CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE*)pyct_data->yct_serial,8); //羊城通充值流水号
			CTools::byte_to_hex((BYTE *)des_rcvData+8,(BYTE *)rsp_code,4);//返回码
			//如果返回码不是0x00000000 ，后面是错误信息描述
			if (strcmp((char *)rsp_code,"00000000")!=0)
			{
			    wrLog("返回码:%s",rsp_code);
				bbzero(error_info);
				bbzero(byte_errinfo);
				CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)error_info,50);//错误信息描述
				CTools::tobyte(error_info,byte_errinfo);
				CTools::trim(byte_errinfo);
				//TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				wrLog("错误信息描述: %s",byte_errinfo);	
				if ((strcmp((char *)rsp_code,"AA01E202")==0) && (bank_signed == 1))
				{
					TS_OutPutMsg(dPosInfo, "账户密码错误，请确认后重试!\n\n\n", 0,iSelect);
				}
				return 1;
			}
			else
			{
				//CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)trade_message1,64);//返回交易信息1
				memcpy(trade_message1,(BYTE *)des_rcvData+12,64);
				CTools::byte_to_hex((BYTE *)des_rcvData+76,(BYTE *)sys_time,7);//返回系统时间			
				CTools::byte_to_hex((BYTE *)des_rcvData+83,(BYTE *)NOTE,48);//圈存查询返回的说明信息		
				wrLog("握手流水号为:%s",pyct_data->SHID);
				wrLog("系统流水号:%s",pyct_data->yct_stan);
				wrLog("返回码:%s",rsp_code);
				wrHex("交易信息1：",trade_message1,64);
				wrLog("圈存说明信息:%s",NOTE);		
				wrLog("系统时间为：%s",sys_time);
				//wrLog("羊城通返回圈存系统参考号为：%s",pyct_data->yct_stan);
			}
		}		
		else
		{
			wrLog("数据包MAC校验出错");	
			return 1;
		}
	}
	
	return iRet;
}//P_CPU_LOAD_QRY()
	

int CYCTYPTrade::R_CPU_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
							BYTE *trade_message2,BYTE *TAC,int &load_status,BYTE *count_info)
{
	BYTE icData[255];
	int sndLen,rcvLen,len,iRet=0;
	BYTE  sndData[512];
	BYTE rcvData[512];
	int cmd;	
	int baudRate=57600;
	
	wrLog("【R_CPU_LOAD】");
	
	bbzero(icData);
	bbzero(sndData);
	
	len = 2;
	cmd = 0x7B; 
	wrHex("trade_message2 :",trade_message2,32);
	//CTools::hex_to_byte(trade_message2,icData,64);
	memcpy(icData,trade_message2,32); //交易消息2
	wrHex("icData :",icData,32);
	len +=32;
	
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("下发数据", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		
		load_status = rcvData[3];
		//memcpy(load_status,rcvData[3],1);
		//CTools::byte_to_hex((BYTE *)rcvData+3,yct_term_status,1); //羊城通终端状态
        wrHex("操作结果 :",rcvData+3,1);
		if( rcvData[3]==0x00) //如果充值状态为00，则圈存成功
		{
			//load_status=0;
			//bbzero(SW1SW2);
			bbzero(pyct_data->SW1SW2);
			CTools::byte_to_hex(rcvData+4,TAC,4); //交易认证码
			CTools::byte_to_hex(rcvData+8,(BYTE *)pyct_data->SW1SW2,2);//卡片返回状态码
			//memcpy(SW1SW2,rcvData+8,2);
			wrLog("交易认证码为:%s",TAC);
			wrLog("卡片返回状态码:%s",pyct_data->SW1SW2);
			
			
			//如果卡片状态返回码是ox9000 对比写卡前后的余额加值正确，表示圈存成功
			if(strcmp(pyct_data->SW1SW2, "9000") == 0)
			{
				
				return 0;
			}
			
			return 0;
		}
		//else 见读卡器错误定义		
		else
		{
//			load_status;
			wrLog("圈存失败");
			return -1;
			//R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,1);
			//需要重新读卡余额后，将结果返回服务器，终端不能打印圈存单据，提示圈存失败	
			
		}
		//如果读卡器执行R_cpu_load指令后读卡器无响应，需要重新读取卡余额，如果余额成功，并且价值正确，表示圈存成功。
	}
	else
	{
	
		return 1;
	}

}//R_CPU_LOAD


int  CYCTYPTrade::P_CPU_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_info,BYTE *yct_term_status,BYTE *trade_message2,BYTE *charge_authcode)
{
	int iRet,sndLen,rcvLen,len,out = 0;
	SOCKET sd;
	BYTE pack_data [256];
	BYTE decrpt_data[512];
	BYTE  sndData[512];
	BYTE type [2]; //信息类型码
	BYTE ver [2]; //报文版本号
	char MAC[4];
	BYTE RFU[32+1];
	BYTE rcvData[512];
	BYTE md5_digest[255];
	BYTE EN_SHID[16+1];
	char txn_serial[20];
	int iSelect;

	char yct_sequence_NO[4+1];
	BYTE len_tmp[2];
	CConnect con;
	BYTE enrpt_packdata[512];


  	wrLog("【P_CPU_LOAD】");

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(txn_serial);
	sndLen=0;//发送报文长度
	rcvLen = 0;
	
	len=0;//报文体长度
	
	CTools::getYct_posSerial(txn_serial);
	//strcpy(pyct_data->term_serial,txn_serial);
	CTools::hex_to_byte((BYTE *)"AA83",type,4);
	CTools::hex_to_byte((BYTE *)"0001",ver,4);
	memcpy(sndData,type,2); //信息类型码
	memcpy(sndData+2,ver,2); //报文版本号
	sndData[4]=0x02; //报文体加密算法
	sndData[5]=0x80; //数值内容字节顺序
	sndLen+=6;
	
	bbzero(pack_data);
	bbzero(MAC);
	bbzero(enrpt_packdata);

	//memcpy(pack_data,pyct_data->SHID,8); //握手流水号
	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //握手流水号
	len+=8;
	//CTools::hex_to_byte((BYTE * )pyct_data->yct_sequence_NO,pack_data,4); //报文序号//有终端进行管理，每次上传给服务器报文+1
	sprintf((char *)yct_sequence_NO,"%08X",(unsigned int )pyct_data->yct_sequence_NO);  //羊城通报文序号（类型为hex）
	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8); 
	len+=4;
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //管理卡号
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //物理卡号
	len+=8;
	CTools::hex_to_byte((BYTE *) pyct_data->ticket_no,pack_data+len,16); //逻辑卡号
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->yct_stan,pack_data+len,16); //系统流水号
	len+=8;
	wrHex("card_info: ",card_info,80);
	memcpy(pack_data+len,card_info,80); //取卡器指令cpu取卡信息返回的数据
	len+=80;
	CTools::hex_to_byte((BYTE *)pyct_data->SW1SW2,pack_data+len,4); //卡片返回码sw1sw2
	len+=2;
	memcpy(pack_data+len,yct_term_status,1) ;//相应读卡器返回0xff
	len+=1;
	memcpy(pack_data+len,RFU,25);//默认为0x00
	len+=25;
	
	//计算mac
	//	CMd5::md5_finish(pack_data,md5_digest); 
	wrHex("报文体（没有MAC）:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4); //校验码
	memcpy(pack_data+len,MAC,4); //取校验码前四位
	len+=4;
	wrHex("mac",(BYTE *)MAC,4);
	wrHex("pack_data:",(BYTE *)pack_data,len);

	//memcpy(MAC,md5_digest,4); //校验码
	//memcpy(pack_data,MAC,4); //取校验码前四位
	//len+=4;

	//加密
	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //使用3des对SHID
	wrLog("加密前的SHID: %s",pyct_data->SHID);
	wrLog("加密后的SHID: %s",EN_SHID);
	
	//enrpt_packdata=AES.AESEncryptEx(pack_data,len,pyct_data->SKEY,16,&out); //使用SKEY通过AES 加密
    BYTE *enc_packdata;
	enc_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,pyct_data->SKEY,16,&out); //除握手流水号以外报文使用sk通过AES加密
	
	wrHex("AA83需要加密的数据 :",pack_data+8,len-8);
	wrHex("AA83加密后的数据   :",enc_packdata,out);
	
// 	CTools::hex_to_byte((BYTE *)len,sndData+6,2);//报文体长度
// 	CTools::hex_to_byte(EN_SHID,sndData+8,16);
// 	CTools::hex_to_byte(enrpt_packdata,sndData+24,out);
// 	sndLen+=out;

	wrLog("报文体总长度: %d",len);
	sprintf((char *)len_tmp,"%04x",len);
	//memcpy(sndData,len_tmp,2); //报文体长度
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //报文体长度
	sndLen+=2;
	CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16);
	sndLen+=8;
	memcpy(sndData+sndLen,enc_packdata,out);//发送数据
	sndLen+=out;

	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		return 3;// 3：与羊城通交互失败
	}
	
	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //将数据发送给羊城通
	
	if( iRet==0  )
	{
		BYTE mac_this[4];
		
		BYTE trade_time[14+1];
		BYTE trade_mny[8+1];	
		BYTE client_no[16+1];
		BYTE client_name[40+1];
		BYTE note [128+1];
		//BYTE charge_authcode[8+1];

		BYTE out_tickect_no[16+1];
		BYTE RFU [78+1];
		BYTE *des_rcvData;
		BYTE rsp_code[8+1];
		int packLen=0;
		BYTE temp[8+1];
		
		
		bbzero(MAC);
		//bbzero(des_rcvData);
		bbzero(EN_SHID);
		bbzero(charge_authcode);
		bbzero(trade_time);
		bbzero(trade_mny);
		bbzero(client_no);
		bbzero(client_name);
		bbzero(note);
		bbzero(out_tickect_no);
		bbzero(temp);
		
		//报文解密及进行MAC校验
		
		//解密SHID
		//CTools::byte_to_hex(rcvData+8,EN_SHID,16);
		CTools::byte_to_hex((BYTE *)rcvData+8,EN_SHID,8);
		bbzero(pyct_data->SHID);
		CDes::TripleDesDec((char * )CKEY,(char *)EN_SHID,(char *)pyct_data->SHID);//握手流水号
		wrLog("握手流水号是:%s",pyct_data->SHID);

// 		//		CDes::TripleDesDec(CKEY,EN_SHID,pyct_data->SHID);
// 		
// 		//		des_rcvData=CAES::aes_decrypt(rcvData+16,len-16,pyct_data->SKEY,16,&out); //解密报文体的内容
// 		CTools::byte_to_hex((BYTE*)des_rcvData+204,(BYTE *)MAC,4);//校验码
// 		//进行mac校验
// 		//		CMd5::md5_finish(rcvData,md5_this);
// 		memcpy((CHAR *)mac_this,md5_this,4);


		CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
		packLen+=8;
		
		des_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out); //报文体使用SKEY进行解密
		memcpy(decrpt_data+packLen,des_rcvData,out-4); //
		packLen+=out-4;
		
		wrHex("解密报文体（除握手流水号）",des_rcvData,out);
		memcpy(mac_this,des_rcvData+(out-4),4); //获取MAC
		
		//需要计算md5的数据：
		wrHex("decrpt_data",decrpt_data,packLen);
		//进行mac校验
		bbzero(md5_digest);
		md5_csum(decrpt_data,packLen,md5_digest); //进行md5运算
		memcpy(MAC,md5_digest,4); //校验码
		wrHex("MAC",(BYTE *)MAC,4);
		wrHex("mac_this",mac_this,4);
		
		if(memcmp(MAC,mac_this,4)==0)
		{
			
			wrLog("MAC校验成功");
			bbzero(pyct_data->yct_stan);
			
			CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE *)pyct_data->yct_stan,8); //返回系统流水号
			wrLog("系统流水号: %s",pyct_data->yct_stan);
			CTools::byte_to_hex((BYTE *)des_rcvData+8,(BYTE *)rsp_code,4);//返回码

			if (strcmp((char *)rsp_code,"00000000")!=0)
			{
				char error_info[128];
				char byte_errinfo[128];
				wrLog("交易返回码:%s",rsp_code);
				if (strcmp((char *)rsp_code,"AA01FD22")==0)
				{
					TS_OutPutMsg(dPosInfo," \n 账户余额不足!\n请按【确认】返回",0,iSelect);
				}
				bbzero(error_info);
				bbzero(byte_errinfo);
				CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)error_info,50);//错误信息描述
				CTools::tobyte(error_info,byte_errinfo);
				CTools::trim(byte_errinfo);
				wrLog("错误信息描述: %s",byte_errinfo);
				//TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				return 1;
			}
			else
			{
				bbzero(trade_message2);
				//CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)trade_message2,64);//返回交易信息2
				memcpy((BYTE *)trade_message2,des_rcvData+12,32);//返回交易信息2

				bbzero(pyct_data->tradeTime);
				CTools::byte_to_hex((BYTE *)des_rcvData+44,(BYTE *)pyct_data->tradeTime,7);//返回交易时间
				
				bbzero(pyct_data->amount);
				CTools::getAsiAmo((BYTE *)des_rcvData+51,4,pyct_data->amount);
				//CTools::byte_to_hex((BYTE *)des_rcvData+51,(BYTE *)pyct_data->amount,4);//返回交易金额
				//memcpy((BYTE *)des_rcvData+51,(BYTE *)trade_mny,4);
				bbzero(charge_authcode);
				CTools::byte_to_hex((BYTE *)des_rcvData+55,(BYTE *)charge_authcode,4); //返回交易凭证号
				bbzero(pyct_data->charge_authcode);
				memcpy(pyct_data->charge_authcode,charge_authcode,8);
				
				bbzero(pyct_data->yct_termid);
				memcpy((BYTE *)pyct_data->yct_termid,des_rcvData+59,6);
				//CTools::byte_to_hex((BYTE *)des_rcvData+59,(BYTE *)pyct_data->yct_termid,6);//终端编号
				
				bbzero(pyct_data->trader_id);
				memcpy((BYTE *)pyct_data->trader_id,des_rcvData+65,8);
				//CTools::byte_to_hex((BYTE *)des_rcvData+65,(BYTE *)pyct_data->trader_id,8);//商户编号
				
				bbzero(pyct_data->trader_name);
				memcpy((BYTE *)pyct_data->trader_name,des_rcvData+73,20);
				//CTools::byte_to_hex((BYTE *)des_rcvData+73,(BYTE *)pyct_data->trader_name,20);//商户简称

				bbzero(out_tickect_no);
				CTools::byte_to_hex((BYTE *)des_rcvData+93,(BYTE *)out_tickect_no,8);//P_CPU_LOAD_QRY中阶段1应用类型是0x30卡间余额转移时有效
				CTools::byte_to_hex((BYTE *)des_rcvData+101,(BYTE *)pyct_data->note,64);//用于填写充值成功的备注
				CTools::byte_to_hex((BYTE *)des_rcvData+165,(BYTE *)RFU,39);//RFU
				
				
				wrLog("交易返回码为：%s",rsp_code);
				wrHex("des_rcvData+12 :",des_rcvData+12,32);
				wrHex("返回交易信息2 :",trade_message2,32);
				wrLog("交易时间：%s",pyct_data->tradeTime);
				wrLog("交易金额: %s",pyct_data->amount);
				wrLog("充值凭证号:%s",charge_authcode);
				wrHex("终端编号: ",(BYTE *)pyct_data->yct_termid,6);
				wrHex("商户编号: ",(BYTE *)pyct_data->trader_id,8);
				wrHex("商户简称: ",(BYTE *)pyct_data->trader_name,20);
				//wrLog("转出卡逻辑卡号:%s",out_tickect_no);
				wrLog("备注:%s",pyct_data->note);
				wrLog("RFU:%s",RFU);
			}
		}
		else
		{
			
			wrLog("数据包MAC校验失败");
		}
		
		return iRet;
	} 
	
	else if( iRet==1  )
	{
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
}//P_CPU_LOAD()


int CYCTYPTrade::P_CPU_LOAD_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		int &load_status,BYTE *TAC,BYTE *yct_term_status,BYTE *count_info,BYTE *charge_authcode,BYTE *followup_oper)
{
	int iRet = 0;
	int sndLen = 0;
	int rcvLen = 0;
	int len = 0;
	int packLen = 0;
	int out = 0;
	
	BYTE pack_data [256];
	BYTE decrpt_data[512];
	BYTE  sndData[512];
	BYTE type [2]; //信息类型码
	BYTE ver [2]; //报文版本号
	BYTE trade_authcode[2+1];//交易凭证号
    BYTE amo[4+1];
	BYTE RFU[64+1];
	BYTE rcvData[512];
    CConnect con;
	char yct_sequence_NO[8+1];
	BYTE loadstatus[2+1];
	BYTE up_limit[6+1];
	BYTE down_limit[1+1];
	
	bbzero(up_limit);
	bbzero(down_limit);

	wrLog("【P_CPU_LOAD_SUBMIT】");

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(trade_authcode);
	bbzero(yct_sequence_NO);
	bbzero(decrpt_data);



	sndLen=0;//发送报文长度
	rcvLen = 0;
	
	len=0;//报文体长度
	
	CTools::hex_to_byte((BYTE *)"AA85",type,4);
	CTools::hex_to_byte((BYTE *)"0001",ver,4);
	memcpy(sndData,type,2); //信息类型码
	sndLen+=2;
	memcpy(sndData+2,ver,2); //报文版本号
	sndLen+=2;
	
	sndData[4]=0x02; //报文体加密算法
	sndData[5]=0x80; //数值内容字节顺序
	sndLen+=2;
	
	bbzero(pack_data);
// 
// 	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //握手流水号
// 	len+=8;	
// 	sprintf(yct_sequence_NO,"%08X",(unsigned int )pyct_data->yct_sequence_NO);  //羊城通报文序号（类型为hex）
// 	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8); 
// 	//CTools::hex_to_byte((BYTE *)pyct_data->yct_sequence_NO,pack_data+len,8);//报文序号
// 	len+=4;


	CTools::hex_to_byte((BYTE*)pyct_data->pki_card,pack_data+len,8); //pki管理卡号
	len+=4;
//	memcpy(pack_data+len,pyct_data->yct_stan,8); //系统流水号 对应的AA82的系统流水号
	wrLog("pyct_data->yct_stan :%s",pyct_data->yct_stan);
	CTools::hex_to_byte((BYTE *)pyct_data->yct_stan,pack_data+len,16); //系统流水号
	len+=8;
	CTools::hex_to_byte((BYTE *) pyct_data->ticket_physicalNo,pack_data+len,16);//物理卡号
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,pack_data+len,16); //逻辑卡号
	len+=8;

	sprintf((char *)loadstatus,"%02X",0); 
	CTools::hex_to_byte((BYTE *)loadstatus,pack_data+len,2); 
	wrLog("loadstatus:  %s",(char *)loadstatus);
	//memcpy(pack_data+len,(BYTE*)load_status,1);//hex ,00正常提交，AA84报文或者未向读卡器发起充值指令
	len+=1;
	CTools::hex_to_byte((BYTE*)TAC,pack_data+len,8); //交易认证码就是TAC码，无法错误TAC时填0x00;
	wrHex("交易认证码: ",pack_data+len,4);
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->SW1SW2,pack_data+len,4); //读卡器指令R_CPU_LOAD返回的SW1SW2
	len+=2;
	//memcpy(pack_data+len,(BYTE*)yct_term_status,1);//读卡器指令R_CPU_LOAD返回status 结果，无响应时oxff
	sprintf((char *)loadstatus,"%02X",load_status); 
	CTools::hex_to_byte((BYTE*)loadstatus,pack_data+len,2);//读卡器指令R_CPU_LOAD返回status 结果，无响应时oxff
	wrHex("读卡器返回码: ",pack_data+len,1);
	len+=1;
	if (memcmp((BYTE *)loadstatus,"01",2) == 0)
	{
        //memcpy(trade_authcode,0x00,1);
		CTools::hex_to_byte((BYTE*)"0x00",pack_data+len,8);//圈存状态为01时候，可填写0x00
	}
	else
	{
		CTools::hex_to_byte((BYTE*)charge_authcode,pack_data+len,8);
		//memcpy(pack_data+len,,4);
	}
	wrHex("充值凭证号: ",pack_data+len,4);
	len+=4;
	
	//重新调用R_CPU_GET_CARDINFO 如果圈存状态为0x01也填0x00，无法重新读到时填0x00
	//调用函数get_cpu_cardInfo();读不到卡需要重新读卡三次
	
	
	getHexAmo(pyct_data->amount,amo);
	wrHex("amo :",amo,4);
	memcpy(pack_data+len,amo,4);//交易金额
	//CTools::hex_to_byte((BYTE *)amo,(BYTE *)pack_data+len,8);
	wrHex("交易金额: ",pack_data+len,4);
	len+=4;

	getHexAmo(pyct_data->ticket_amo_aft,amo);
	wrHex("amo :",amo,4);
	memcpy(pack_data+len,amo,4);//（圈存后）钱包余额
	//memcpy(pack_data,pyct_data->ticket_amo_aft,4);
//	CTools::hex_to_byte((BYTE *)amo,(BYTE *)pack_data+len,8);
	wrHex("钱包余额: ",pack_data+len,4);
	len+=4;
	//memcpy(down_limit,pyct_data->downLimit,1);
	//CTools::hex_to_byte((BYTE *)pyct_data->downLimit,pack_data+len,2); //钱包下限
	
	memcpy(pack_data+len,pyct_data->downLimit,1); //余额下限
	wrHex("钱包下限: ",pack_data+len,1);
	len+=1;
	//memcpy(up_limit,pyct_data->downLimit,3);
	//CTools::hex_to_byte((BYTE *)pyct_data->downLimit,pack_data+len,6); //羊城通金额上限
	
	memcpy(pack_data+len,pyct_data->upperLimit,3);//余额上限
	wrHex("钱包上限: ",pack_data+len,3);
	len+=3;
	wrHex("count_info: ",count_info,16);
	memcpy(pack_data+len,count_info,16);//计数信息
	len+=16;
	//bbzero(RFU);
	memcpy(pack_data+len,RFU,36);//RFU
	len+=36;
	
	  //计算mac
// 	  CMd5::md5_finish(sndData+6,md5_digest);
// 	  memcpy(MAC,md5_digest,4); //校验码
// 	  memcpy(pack_data,MAC,4); //取校验码前四位
// 	  len+=4;
    wrLog("len: %d",len);
	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA85" , (BYTE*)"0001" , 0x02 , 0x80 , \
		 pack_data , len , rcvData , &rcvLen);

	if (iRet != 0)
	{
		wrLog("发送到羊城通服务器失败");				
		return 1;
	}

	BYTE des_rcvData[512];
	BYTE rsp_code[4+1];
	bbzero(rsp_code);
	bbzero(des_rcvData);
	memcpy(des_rcvData,rcvData+8+8,rcvLen-8-8);
	//bbzero(pyct_data->yct_stan);
	
	//CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE *)pyct_data->yct_stan,8); //返回系统流水号
	//wrLog("返回系统流水号: %s",pyct_data->yct_stan);
	wrHex("返回系统流水号: ",des_rcvData,8);
	CTools::byte_to_hex((BYTE *)des_rcvData+8,(BYTE *)rsp_code,4);//返回码
				
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("交易返回码:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)des_rcvData+13,(BYTE *)error_info,50);//错误信息描述
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("错误信息描述: %s",byte_errinfo);				
		return 1;
	}
	else
	{	
		CTools::byte_to_hex(des_rcvData+28,followup_oper,1);//hex ox00表示操作已经完成 0x01时候，还需要进行扩展应用的操作
		memcpy(followup_oper,des_rcvData+28,1);
		wrHex("followup_oper: ",followup_oper,1);
	}

	return 0;
}//P_CPU_LOAD_SUBMIT()

    /*                                                                    */
	/******************************** CPU  ********************************/


int CYCTYPTrade::P_TRAN_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, \
		PYCT_DATA pyct_data,BYTE *card_pycicalInfo1,BYTE oper_code,int &trade_type,BYTE *query_info,BYTE *party, BYTE *rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE SK[32+1]; //回话密钥
	
	BYTE pysical_card[8+1]; //物理卡号
	BYTE logical_card[8+1]; //逻辑卡号
	char term_time_tmp[14];
	//BYTE card_pycicalInfo1[32+1];//卡片物理信息
	BYTE SW1SW2[4+1];
	int status=0;
	BYTE SAK[1+1];
	SOCKET sd=0;
	CConnect con;//连接类
	int baudRate = 57600;
	//报文中的参数；
	BYTE type [2]; //信息类型码
	BYTE ver [2]; //报文版本号
	BYTE RFU [34];
	BYTE  MAC [4];
	BYTE rsp_code[8+1];
	BYTE rsp_message[48];

	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE R_CPU_LOAD_STATUS[1+1];
	BYTE enrpt_SHID[30]; 
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //报文序号
	BYTE md5_digest[16];
	R_CPU_LOAD_STATUS[0]=0xff;
	BYTE mac_this[4];
	char czamount[20];
	BYTE sign_status[10];
	char money_limit[10];
	char tradeamount[20];
	char temp[256];
	BYTE len_tmp[4];
	BYTE pack_data[104];
	BYTE *enrpt_packdata;
	int packLen=0;
	int len2=0;
	
	bbzero(MAC);
	bbzero(mac_this);
	bbzero(SHID);
	bbzero(czamount);
	bbzero(sign_status);
	bbzero(money_limit);
	bbzero(tradeamount);
	bbzero(temp);
	
	bbzero(rsp_message);
	bbzero(SHID);
	bbzero(pysical_card);
	bbzero(logical_card);
	bbzero(card_pycicalInfo1);
	bbzero(rsp_code);
	bbzero(SW1SW2);
	bbzero(SHID);
	bbzero(SK);
	bbzero(SAK);
	bbzero(SERVER_TIME);
	bbzero(term_time_tmp);
	bbzero(type);
	bbzero(ver);
	bbzero(RFU); //RFU 保留
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
     len=0;//报文体长度 
	sndLen=0;//发送报文长度
	rcvLen = 0;

	bbzero(sndData);
	bbzero(rcvData);

	bbzero(pack_data);

	CTools::hex_to_byte((BYTE *)"AA51",sndData,4); //信息类型码
	sndLen+=2;
	CTools::hex_to_byte((BYTE *)"0003",sndData+sndLen,4); //报文版本号
	sndLen+=2;

	sndData[4]=0x02; //报文体加密算法
	sndData[5]=0x80; //数值内容字节顺序
	sndLen+=2; 
	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //流水号
	len+=8;
	sprintf(yct_sequence_NO,"%08X",(unsigned int )pyct_data->yct_sequence_NO);  //羊城通报文序号（类型为hex）
	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8); 
	len+=4;
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)pack_data+len,8); //pki卡号
	len+=4;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,(BYTE *)pack_data+len,16); //物理卡号
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,(BYTE *)pack_data+len,16);// //逻辑卡号
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->SAK,SAK,2);
	memcpy(pack_data+len,SAK,1); 
	len+=1;
	memcpy(pack_data+len,card_pycicalInfo1,32);
	len+=32;
	pack_data[len]=oper_code; //操作码 执行余额查询
	len+=1;
	memcpy(pack_data+len,RFU,34);
	len+=34;
	wrHex("RTU",RFU,34);

	//计算mac 
	wrHex("报文体（没有MAC）:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4);
	memcpy(pack_data+len,MAC,4);
	len+=4;
	wrHex("mac",MAC,4);

	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)enrpt_SHID);
	
	wrLog("加密之前的SHID:%s",pyct_data->SHID);
	wrLog("加密之后的SHID:%s",enrpt_SHID);

	//报文体中除了SHID 其余报文体使用的是AES加密，SHID 使用3DES;
	wrHex("packData:",(BYTE *)pack_data,len);

	enrpt_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,(BYTE *)pyct_data->SKEY,16,&out); //使用sk通过AES加密

	wrHex("aa51需要加密的数据：",pack_data+8,len-8);
	wrHex("aa51 encpt_packdata:",enrpt_packdata,out);
	
	sprintf((char *)len_tmp,"%04X",8+out); //报文体长度
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //报文体长度
	sndLen+=2;
	
	CTools::hex_to_byte((BYTE *)enrpt_SHID,sndData+sndLen,16);  //加密的SHID 
	sndLen+=8;
	memcpy(sndData+sndLen,enrpt_packdata,out);
	sndLen+=out;
                                                                                                                                                                                                                                                                                                                                         
	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}


	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //将数据发送给羊城通	
	if( iRet==0  )
	{
		char  tag[4]; //信息类型码
		BYTE mac_this[4];
		BYTE decrpt_data[255];
		int packLen=0;

		bbzero(MAC);
		bbzero(mac_this);
		bbzero(SHID);

		CTools::byte_to_hex((BYTE *)rcvData,(BYTE *)tag,2);

		if (strcmp(tag,"FE04")==0)
		{
			yct_SignIn(dPosInfo,com,comType,pyct_data,party,(byte*)rsp_code);
			wrLog("FE04报文，需要重新签到");
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
		}
		else		
		{
			CTools::byte_to_hex((BYTE *)rcvData+8,enrpt_SHID,8);
			bbzero(pyct_data->SHID);
			CDes::TripleDesDec((char * )CKEY,(char *)enrpt_SHID,(char *)pyct_data->SHID);//流水号
			wrLog("握手流水号是:%s",pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
			packLen+=8;

			BYTE *aes_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,(BYTE *)pyct_data->SKEY,16,&out); //报文体使用SKEY进行解密
			memcpy(decrpt_data+packLen,aes_rcvData,out-4); //
			packLen+=out-4;

			wrHex("解密报文体（除握手流水号）",aes_rcvData,out);
			memcpy(mac_this,aes_rcvData+(out-4),4); //获取MAC
			
			//需要计算md5的数据：
			wrHex("decrpt_data",decrpt_data,packLen);
			//进行mac校验
			bbzero(md5_digest);
			md5_csum(decrpt_data,packLen,md5_digest); //进行md5运算
			memcpy(MAC,md5_digest,4); //校验码
			wrHex("MAC",MAC,4);
			wrHex("mac_this",mac_this,4);

			if(memcmp((char *)MAC,(char *)mac_this,4)==0) //进行MAC校验
			{	
				wrLog("mac校验成功");

				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)rsp_code,4); //返回码
				trade_type=aes_rcvData[4]; //返回交易类型
				wrLog("aes_rcvData[4] : %d",aes_rcvData[4]);
				memcpy(rsp_message,aes_rcvData+5,48); ////返回查询信息
				bbzero(query_info);
				memcpy(query_info,aes_rcvData+5,48);
				wrLog("返回码为：%s",rsp_code);
				wrLog("返回交易类型为：%d",trade_type);
				wrHex("返回查询信息",query_info,48);
			}

			else
			{
				wrLog("MAC校验出错");
			}
			return 0;
		}	
		return 0;
	}	
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}
}

/**************************第三代羊城通充值新增函数 结束****************************************/
int CYCTYPTrade::P_ACCT_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, int &payType, char *czamount, char *money_limit,char *char_fre,char *ornotpassword,char *statusflag, BYTE *sign_status, BYTE *party, BYTE *rspcode)
{
    int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE SK[32+1]; //回话密钥
	
	BYTE pysical_card[8+1]; //物理卡号
	BYTE logical_card[8+1]; //逻辑卡号
	BYTE card_pycicalInfo1[32+1];//卡片物理信息
	BYTE SW1SW2[4+1];
	int status=0;
	BYTE SAK[1+1];
	SOCKET sd=0;
	CConnect con;//连接类
	int baudRate = 57600;
	//报文中的参数；
	BYTE type [2]; //信息类型码
	BYTE ver [2]; //报文版本号
	BYTE RFU [36];
	BYTE  MAC [4];
	BYTE rsp_code[8+1];
	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE R_CPU_LOAD_STATUS[1+1];
	BYTE enrpt_SHID[30]; 
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //报文序号
	BYTE md5_digest[16];
	R_CPU_LOAD_STATUS[0]=0xff;
	BYTE mac_this[4];
	BYTE decrpt_data[255];
	char tradeamount[20];
	char temp[256];
	BYTE len_tmp[4];
	BYTE pack_data[104];
	
	char hex_amount[20];
	char charge_limit[20];
	BYTE *enrpt_packdata;
	int iSelect = 0;
	int packLen=0;
	int len2=0;
	
	bbzero(MAC);
	bbzero(mac_this);
	bbzero(SHID);
	bbzero(tradeamount);
	bbzero(temp);
	
	bbzero(SHID);
	bbzero(pysical_card);
	bbzero(logical_card);
	bbzero(card_pycicalInfo1);
	bbzero(rsp_code);
	bbzero(SW1SW2);
	bbzero(SHID);
	bbzero(SK);
	bbzero(SAK);
	bbzero(SERVER_TIME);
	bbzero(type);
	bbzero(ver);
	bbzero(RFU); //RFU 保留
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
	bbzero(char_fre);
	bbzero(hex_amount);
	bbzero(charge_limit);

	wrLog("【P_ACCT_QRY】");

	CTools::hex_to_byte((BYTE*)"AB31",sndData,4);                 //信息类型码
	sndLen +=2;
	CTools::hex_to_byte((BYTE*)"0001",sndData+sndLen,4);          //报文版本号
	sndLen +=2;
	sndData[4]=0x02;
	sndData[5]=0x80;
	sndLen +=2;
	CTools::hex_to_byte((BYTE*)pyct_data->SHID,pack_data,16);     //握手流水号
	len +=8;
	sprintf(yct_sequence_NO,"%08x",(unsigned int )pyct_data->yct_sequence_NO);     //报文序号
	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8);
	len +=4;
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)pack_data+len,8);     //pki卡号
	len+=4;
	CTools::hex_to_byte((BYTE *)"02",(BYTE *)pack_data+len,2);                     //终端类型
	len +=1;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,(BYTE *)pack_data+len,16); //物理卡号
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,(BYTE *)pack_data+len,16);    //逻辑卡号
	len+=8;
	memcpy(pack_data+len,RFU,35);
	len+=35;
	wrHex("RTU",RFU,35);



	//计算mac 
	wrHex("报文体（没有MAC）:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4);
	memcpy(pack_data+len,MAC,4);
	len+=4;
	wrHex("mac",MAC,4);
	
	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)enrpt_SHID);
	
	wrLog("加密之前的SHID:%s",pyct_data->SHID);
	wrLog("加密之后的SHID:%s",enrpt_SHID);
	
	//报文体中除了SHID 其余报文体使用的是AES加密，SHID 使用3DES;
	wrHex("packData:",(BYTE *)pack_data,len);
	
	enrpt_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,(BYTE *)pyct_data->SKEY,16,&out); //使用sk通过AES加密
	
	wrHex("AB31需要加密的数据：",pack_data+8,len-8);
	wrHex("AB31 encpt_packdata:",enrpt_packdata,out);
	wrLog("out: %d",out);
	sprintf((char *)len_tmp,"%04X",8+out); //报文体长度
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //报文体长度
	sndLen+=2;
	
	CTools::hex_to_byte((BYTE *)enrpt_SHID,sndData+sndLen,16);  //加密的SHID 
	sndLen+=8;
	memcpy(sndData+sndLen,enrpt_packdata,out);
	sndLen+=out;
	
	//羊城通建立连接
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3：与羊城通交互失败
	}
	
	
	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //将数据发送给羊城通
    if (iRet ==0)
    {
		
		CTools::byte_to_hex((BYTE *)rcvData+8,enrpt_SHID,8);
		bbzero(pyct_data->SHID);
		CDes::TripleDesDec((char * )CKEY,(char *)enrpt_SHID,(char *)pyct_data->SHID);//握手流水号
		wrLog("握手流水号是:%s",pyct_data->SHID);
		CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
		packLen+=8;
		
		BYTE *aes_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,(BYTE *)pyct_data->SKEY,16,&out); //报文体使用SKEY进行解密
		memcpy(decrpt_data+packLen,aes_rcvData,out-4); //
		packLen+=out-4;
		
		wrHex("解密报文体（除握手流水号）",aes_rcvData,out);
		memcpy(mac_this,aes_rcvData+(out-4),4); //获取MAC
		
		//需要计算md5的数据：
		wrHex("decrpt_data",decrpt_data,packLen);
		//进行mac校验
		bbzero(md5_digest);
		md5_csum(decrpt_data,packLen,md5_digest); //进行md5运算
		memcpy(MAC,md5_digest,4); //校验码
		wrHex("MAC",MAC,4);
		wrHex("mac_this",mac_this,4);

		if (memcmp((char *)MAC,(char *)mac_this,4)==0)
		{
			wrLog("mac校验成功");
			len2 +=12;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)rsp_code,4); //返回码
			wrLog("返回码 %s",rsp_code);
			if (memcmp(rsp_code,"00000000",8)!=0)
			{
			   len2 +=4;
               char hex_message[256];
			   char error_message[256];
			   bbzero(hex_message);
			   bbzero(error_message);
			   CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)hex_message,50);
			   CTools::tobyte(hex_message,error_message);
			   TS_OutPutMsg(dPosInfo,error_message,0,iSelect);
			   wrLog("错误信息描述 %s",error_message);
			   
			   *party = FROMYCT;
			   *rsp_code = FROMYCT_N;
			   return 1;
			}
			len2 +=4;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)hex_amount,4);
			CTools::HexToDec(hex_amount,czamount);              //充值金账户余额
			//getAsiAmo3(aes_rcvData+len2,4,czamount); 
			wrLog("充值金账户余额 %.2f 元",atof(czamount)/100);
			len2 +=4;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)ornotpassword,1);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)statusflag,1);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)sign_status,1); //实时扣款签约状态
			wrLog("返回码 %s",sign_status);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)char_fre,1);      //充值频度
			wrLog("充值频度 %s",char_fre);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)money_limit,4); //充值额度
			wrLog("充值额度 %s",money_limit);

		//用文字表示账户是否需要充值密码
			if (strcmp(ornotpassword,"00")==0)
				strcpy(ornotpassword,"否");
			else
				strcpy(ornotpassword,"是");
		
			//用文字表示账户状态标志


			if (strcmp(statusflag,"00")==0)
			{
				strcpy(statusflag,"在用");
			}
			else 
				if (strcmp(statusflag,"01")==0)
			{
					strcpy(statusflag," 已停用");
			}
			else 
				if (strcmp(statusflag,"02")==0)
			{
					strcpy(statusflag,"表示销户");
			}
			else 
				if (strcmp(statusflag,"03")==0)
			{
					strcpy(statusflag,"表示合并销户");
			}
				
			//用文字显示充值频度
				if (strcmp(char_fre,"00")==0)
			{
					strcpy(char_fre,"无限制");
			}



            if (atol((const char *)czamount) > 0)
			{
 		   		payType=0x20;
			}
 			else
			{
				payType=0x22;
			}
		}
    }
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		closesocket(sd);
		return 3;// 3：与羊城通交互失败
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1：与读卡器交互失败
	}

    return 0;
}

int CYCTYPTrade::Yct_AcctHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE *party, BYTE *rspcode)
{
    int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	int iAom;
	
	BYTE pysical_card[8+1]; //物理卡号
	BYTE logical_card[8+1]; //逻辑卡号
	BYTE card_pycicalInfo1[32+1];//卡片物理信息
	BYTE rsp_message[1204];
	char SW1SW2[4+1];
	BYTE SAK[1+1];
	BYTE card_physics_info[51];
	SOCKET sd=0;
	CConnect con;//连接类
	int baudRate = 57600;
	//报文中的参数；
	BYTE type [2]; //信息类型码
	BYTE ver [2]; //报文版本号
	BYTE RFU [34];
	BYTE  MAC [4];
	BYTE rsp_code[8+1];
	int trade_type =0;
	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE SHID[16+1];
	BYTE md5_digest[16];
	BYTE mac_this[4];
	char czamount[20];
	BYTE sign_status[10];
	
	char tradeamount[20];
	char acctamount[20];
	char charge_money[20];
	char temp[256];
	BYTE testmessage[33];


	int iSelect;
	int payType;
	int packLen=0;
	int len2=0;
	
	bbzero(testmessage);
	bbzero(rsp_message);
	bbzero(MAC);
	bbzero(mac_this);
	bbzero(SHID);
	bbzero(czamount);
	bbzero(sign_status);
	bbzero(money_limit);
	bbzero(tradeamount);
	bbzero(temp);
	bbzero(pysical_card);
	bbzero(logical_card);
	bbzero(card_pycicalInfo1);
	bbzero(rsp_code);
	bbzero(SW1SW2);
	bbzero(SHID);
	bbzero(SERVER_TIME);
	bbzero(type);
	bbzero(ver);
	bbzero(RFU); //RFU 保留
	bbzero(md5_digest);
	bbzero(acctamount);
	bbzero(charge_money);
	bbzero(char_fre);
	bbzero(statusflag);
	bbzero(ornotpassword);
	
	//获取卡片的物理信息,返回卡片的物理信息
	iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
	if (iRet == 1)
	{
		TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
		for (int i = 0; i<2; i++)
		{
			iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
			if (iRet == 0)
			{
				break;
			}
			TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
		}

	}
	
	if (iRet ==0)
	{
		CTools::byte_to_hex((BYTE*) card_physics_info,(BYTE*)pyct_data->ticket_physicalNo,8);//获取物理卡号
		CTools::byte_to_hex((BYTE*) card_physics_info+8,(BYTE*)pyct_data->ticket_no,8);      //获取逻辑卡号
		memcpy(SAK,card_physics_info+16,1);                                                  //SAK
		memcpy(card_pycicalInfo1,card_physics_info+17,32);                                   //卡片物理信息1
		CTools::byte_to_hex(card_physics_info+49,(BYTE*)SW1SW2,2);                                  //SW1SW2
		
		wrLog("SW1SW2:%s",SW1SW2);//卡片状态

		if (memcmp(SW1SW2,"9000",4) != 0)
		{
			*party=FROMIC;
			*rspcode=ICTIMEOUT;
			TS_OutPutMsg(dPosInfo,"",0,iSelect);
			return 1;
		}
		wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
		//wrLog("逻辑卡号:%s",pyct_data->ticket_no);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32); 
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
	}
	else
	{
		*party=FROMIC;
		*rspcode=ICTIMEOUT;
		TS_OutPutMsg(dPosInfo,"获取卡片的物理信息发生错误",0,iSelect);
		return 1;
	}
	//P_ACCT_QRY充值金查询
	iRet=P_ACCT_QRY(dPosInfo,com,comType,pyct_data,payType,czamount,money_limit,char_fre,ornotpassword,statusflag,sign_status,party,rspcode);
	if (iRet ==3)
	{
		return 3; // 3：与羊城通交互失败
	}
	else if(iRet ==1)
	{
     	return 1; // 1：与读卡器交互失败
	}	
	else
	{
		wrLog("应用类型 %d",payType);

		sprintf(temp,"充值金账户余额: %.2f元\n使用充值金账户余额充值，按【Enter】选择充值金额 按【Esc】返回",atof(czamount)/100);
		if(atol((const char *)czamount) > 0)
		{
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);
			if(iSelect)
			{
				//用户取消
				*party = FROMCTSI_N;
				*rspcode = USERCANCEL;	
				return 5;
			}
		}
		else
		{
			if (memcmp(sign_status,"02",2) == 0)
			{
				TS_OutPutMsg(dPosInfo,"您的充值金账户余额为0，可进行签约账户充值\n按【Enter】继续充值   按【ESC】返回",0,iSelect);
				if(iSelect)
				{
					//用户取消
					*party = FROMCTSI_N;
					*rspcode = USERCANCEL;	
					return 5;
				}
							//用文字显示实时扣款签约状态
				if (strcmp((char *)sign_status,"00")==0)
				{
					strcpy((char *)sign_status,"未签约");
				}
				else 
					if (strcmp((char *)sign_status,"01")==0)
				{
						strcpy((char *)sign_status," 未激活");
				}
				else 
					if (strcmp((char *)sign_status,"02")==0)
				{
						strcpy((char *)sign_status,"已激活");
				}
				else 
					if (strcmp((char *)sign_status,"03")==0)
				{
					strcpy((char *)sign_status,"暂停");
				}
				else 
					if (strcmp((char *)sign_status,"04")==0)
				{
					strcpy((char *)sign_status,"解约");
				}
					CTools::HexToDec((char *)money_limit,money_limit);
					sprintf(auto_show,"\t\t卡号:%s\n\n充值金账户余额:%.2f元\t账户是否需要充值密码:%s\n\n账户状态:%s\t\t实时扣款签约状态:%s\n\n充值频度:%s天/一次\t充值额度:%.2f元",pyct_data->ticket_no,atof(czamount)/100,ornotpassword,statusflag,sign_status,char_fre,atof(money_limit)/100);
//					TS_OutPutMsg(dPosInfo,auto_show,0,iSelect);
			}
			else
			{
				TS_OutPutMsg(dPosInfo,"您尚未开通充值金账户或签约账户，可选择使用【现金充值】方式进行充值\n按【ESC】返回",0,iSelect);
				*party = FROMCTSI_N;
				*rspcode = USERCANCEL;	
				return 5;
			}
		}
	}
    //获取卡片的物理信息,返回卡片的物理信息
	iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
	if (iRet == 1)
	{
		TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
		for (int i = 0; i<2; i++)
		{
			iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
			if (iRet == 0)
			{
				break;
			}
			TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
		}

	}
	
	
	if (iRet ==0)
	{
		CTools::byte_to_hex((BYTE*) card_physics_info,(BYTE*)pyct_data->ticket_physicalNo,8);//获取物理卡号
		CTools::byte_to_hex((BYTE*) card_physics_info+8,(BYTE*)pyct_data->ticket_no,8);      //获取逻辑卡号
		memcpy(SAK,card_physics_info+16,1);                                                  //SAK
		memcpy(card_pycicalInfo1,card_physics_info+17,32);                                   //卡片物理信息1
		CTools::byte_to_hex(card_physics_info+49,(BYTE*)SW1SW2,2);                                  //SW1SW2
		
		BYTE temp_SW1SW2[2+1];
		bbzero(temp_SW1SW2);
		memcpy(temp_SW1SW2,card_physics_info+49,2);
		
		wrLog("SW1SW2:%s",SW1SW2);
		if (memcmp(SW1SW2,"9000",4) != 0)
		{
			char errMsg[512];
			char outPut_Msg[512];
			get_cardStatus_errMsg((BYTE *)temp_SW1SW2,errMsg);
			bbzero(outPut_Msg);
			sprintf(outPut_Msg,"返回响应卡状态码错误：%s",errMsg);
			TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1：与读卡器交互失败
		}

		wrLog("物理卡号:%s",pyct_data->ticket_physicalNo);
		//wrLog("逻辑卡号:%s",pyct_data->ticket_no);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32); 
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
	}
	else
	{
		*party=FROMIC;
		*rspcode=ICTIMEOUT;
		TS_OutPutMsg(dPosInfo,"获取卡片的物理信息发生错误",0,iSelect);
		return 1; // 1：与读卡器交互失败
	}
	//共用交易查询P_TRAN_QRY 从平台到服务器
TRANQRY: iRet=P_TRAN_QRY(dPosInfo,com,comType,pyct_data,card_pycicalInfo1,0x04,trade_type,rsp_message,party,rspcode);
	if (iRet == 0)
	{    bank_signed = 0;
		switch (trade_type)
		{
			
		case(0x00):
			{
				wrLog("M1钱包(查询余额，圈存和撤销)");
				R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,rsp_message,SW1SW2);
				//判断充值金账户余额/签约账户充值，选择充值金额
				if (payType == 0x20)
				{
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return 3; // 3：与羊城通交互失败
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
					sprintf(temp,"您的签约账户的额度是: %.2f元",atof(money_limit)/100);
//					TS_OutPutMsg(dPosInfo,temp,0,iSelect);
					strcpy(tradeamount,money_limit);
				}
				strcpy(pyct_data->amount,tradeamount);
				iRet=excute_M1_LOAD(dPosInfo,com,comType,pyct_data,2,party,rspcode,tradeamount,payType);
				
				break;
			}
		case (0x02):
			{
				wrLog("M1用户账户充值");
				R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,rsp_message,SW1SW2);
				//判断充值金账户余额/签约账户充值，选择充值金额
				if (payType == 0x20)
				{
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return 3; // 3：与羊城通交互失败
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
					sprintf(temp,"您的签约账户的额度是: %.2f元",atof(money_limit)/100);
//					TS_OutPutMsg(dPosInfo,temp,0,iSelect);
					strcpy(tradeamount,money_limit);
				}
				strcpy(pyct_data->amount,tradeamount);
				iRet=excute_M1_LOAD(dPosInfo,com,comType,pyct_data,2,party,rspcode,tradeamount,payType);
				break;
			}
		case (0x20):
			{
				wrLog("CPU钱包充值（查询余额，圈存和撤销）");
				//判断充值金账户余额/签约账户充值，选择充值金额
				if (payType == 0x20)
				{                                                
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return 3;  // 3：与羊城通交互失败
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
//					sprintf(temp,"您的签约账户额度是: %.2f元",atof(charge_money)/100);
//					//TS_OutPutMsg(dPosInfo,temp,0,iSelect);
					strcpy(tradeamount,money_limit);

				}
				strcpy(pyct_data->amount,tradeamount);
				bbzero(pyct_data->ticket_amo_aft);
				bbzero(pyct_data->result_message);
			//	iRet = CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);
				iRet=CPU_EnqPay(dPosInfo,com,comType,enq,pyct_data,tradeamount,payType,rsp_message,party,rspcode);

				break;
			}
		case(0x21):
			{
				wrLog("OC3.CPU圈存(M1余额转移)");
				
				break;
			}
		case (0x22):
			{
				wrLog("oc3.cpu圈存（cpu用户账户充值）");
				wrLog("CPU钱包充值（查询余额，圈存和撤销）");
				//判断充值金账户余额/签约账户充值，选择充值金额
				if (payType == 0x20)
				{                                                
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return CANCLE_KEY;  // 3：与羊城通交互失败
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
					sprintf(temp,"您的签约账户额度是: %.2f元",atof(charge_money)/100);
					//TS_OutPutMsg(dPosInfo,temp,0,iSelect);
					strcpy(tradeamount,money_limit);
				}
				strcpy(pyct_data->amount,tradeamount);
				bbzero(pyct_data->ticket_amo_aft);
				bbzero(pyct_data->result_message);
				//	iRet = CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);
				iRet=CPU_EnqPay(dPosInfo,com,comType,enq,pyct_data,tradeamount,payType,rsp_message,party,rspcode);

				//	EXCUTE_OC3_CPU(dPosInfo,com,comType,pyct_data,party,rspcode);
				break;
			}
		case (0x30):
			{
				wrLog("执行扩展应用操作");
				break;
			}
		case (0x31):
			{
				wrLog("执行扩展应用查询");
				break;
			}
		case (0x40):
			{
				wrLog("执行黑名单处理");				
				EXCUTE_BLACKLIST(dPosInfo,com,comType,pyct_data,rsp_message,party,rspcode);
				break;
			}
		case (0x60):
			
			{
				wrLog("执行CPU同步");
				change = 1;
				R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,rsp_message,SW1SW2);
				iRet=EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,rsp_message,party,rspcode);
				if (*rspcode == R_APP_CARD_UPGRADE_EXEFAIL)
				{
					wrLog("已经进入重新执行充值类型查询！");
					TS_OutPutMsg(dPosInfo,"请把卡放到读卡器上",1,iSelect);
					//重新执行充值类型查询
					goto TRANQRY; 
				}
				if (iRet == 3)
				{
					M1_CPU = 1;
					iRet=excute_M1_LOAD(dPosInfo,com,comType,pyct_data,2,party,rspcode,tradeamount,payType);					
				}

				break;		
			}
			
			
		}
	}

    if (iRet ==5)
    {
		return 5; 
    }
	if (iRet ==3)
	{
		return 3; // 3：与羊城通交互失败
	}
	if(iRet ==1)
	{
		pyct_data->status = 5;
		bbzero(pyct_data->term_serial);
		bbzero(pyct_data->ticket_amo_aft);
		CTools::getYct_posSerial(pyct_data->term_serial);
		strcpy(pyct_data->yct_serial,pyct_data->yct_stan);
		CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);
		return 1; // 1：与读卡器交互失败
	}
	if (iRet == 11)
	{
		pyct_data->status = 6;
		bbzero(pyct_data->term_serial);
		bbzero(pyct_data->ticket_amo_aft);
		CTools::getYct_posSerial(pyct_data->term_serial);
		strcpy(pyct_data->yct_serial,pyct_data->yct_stan);
		CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);
		return 1; // 1：与读卡器交互失败
	}
	if (iRet != 0)
	{
		pyct_data->status = 5;

	}
	else
	{
		pyct_data->status = 2;
	}

	bbzero(pyct_data->term_serial);
	CTools::getYct_posSerial(pyct_data->term_serial);
	strcpy(pyct_data->yct_serial,pyct_data->yct_stan);
	CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);

	//P_ACCT_QRY充值金查询
	iRet=P_ACCT_QRY(dPosInfo,com,comType,pyct_data,payType,czamount,money_limit,char_fre,ornotpassword,statusflag,sign_status,party,rspcode);
	if (iRet ==3)
	{
		return 3;
	}
	else if(iRet ==1)
	{
		return 1;
	}
	
	else
	{
		if(M1_CPU == 1 && payType == 0x22)
		{
			sprintf(temp,"\n\n卡号:%s\n\n卡充值后余额:%.2f元\n\n按【Enter】充值  按【Esc】返回",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100);
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);

		}
		if (M1_CPU == 1 &&payType == 0x20)
		{
			sprintf(temp,"卡号：%s\n\n充值金账户余额: %.2f元\n\n充值后卡余额:%0.2f元\n\n\n\n\n按【Enter】充值  按【Esc】返回",pyct_data->ticket_no,atof(czamount)/100,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100);
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);
		}
		if(M1_CPU == 2 && payType == 0x22)
		{
			sprintf(temp,"\n\n卡号:%s\n\n卡充值后余额:%.2f元\n\n按【Enter】充值  按【Esc】返回",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100 - atof(des_lim));
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);

		}
		if (M1_CPU == 2 && payType == 0x20)
		{
			sprintf(temp,"卡号：%s\n\n充值金账户余额: %.2f元\n\n充值后卡余额:%0.2f元\n\n\n\n\n按【Enter】充值  按【Esc】返回",pyct_data->ticket_no,atof(czamount)/100,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100 - atof(des_lim));
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);
		}

	}

    if (iRet == 3)
		return 3;
	else if (iRet == 1)
	{
		return 1;
	}
	return 0;
}

int CYCTYPTrade::AutoSelectAmo(const TSDLLPosInfo dposInfo, char *czamo, char *tradeamount)
{
    int iRet=0,iSelect=0,nowAmo=0;
    TSMENULIST menulist;
	memset(&menulist,0,sizeof(menulist));
	nowAmo=atoi(czamo);
	wrLog("余额：%d",nowAmo);
	if (nowAmo >0 && nowAmo <5000)
	{
		menulist.iTotalNum = 1;
		strcpy(menulist.acTitle,"请选择你充值的金额");
		strcpy(menulist.menuItem[0],"一笔充完");
		iRet=TS_Menu(dposInfo,&menulist,iSelect,MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return iRet;
		}
		else
		{
			NULL;
		}
		switch(iSelect)
		{
		case 1:
            strcpy(tradeamount,czamo);
			return 0;
		default: 
		    return CANCLE_KEY;
		}
	}
	else if (nowAmo >=5000 && nowAmo <10000)
	{
		menulist.iTotalNum=2;
		strcpy(menulist.acTitle,"请选择你充值的金额");
		strcpy(menulist.menuItem[0],"50元");
		strcpy(menulist.menuItem[1],"一笔充完");
        iRet=TS_Menu(dposInfo,&menulist,iSelect,MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return iRet;
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			strcpy(tradeamount,"000000005000");
			return 0;
		case 2:
			strcpy(tradeamount,czamo);
			return 0;
		default: 
		    return CANCLE_KEY;
		}
	}
	else if (nowAmo >=10000 && nowAmo <20000)
	{
        menulist.iTotalNum=3;
		strcpy(menulist.acTitle,"请选择你充值的金额");
		strcpy(menulist.menuItem[0],"50元");
		strcpy(menulist.menuItem[1],"100元");
		strcpy(menulist.menuItem[2],"一笔充完");
        iRet=TS_Menu(dposInfo,&menulist,iSelect,MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return iRet;
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			strcpy(tradeamount,"000000005000");
			return 0;
		case 2:
			strcpy(tradeamount,"000000010000");
			return 0;
		case 3:
			strcpy(tradeamount,czamo);
			return 0;
		default: 
			return CANCLE_KEY;
		}
	}
	else if (nowAmo >=20000 && nowAmo<100000)
	{
        menulist.iTotalNum=4;
		strcpy(menulist.acTitle,"请选择你充值的金额");
		strcpy(menulist.menuItem[0],"50元");
		strcpy(menulist.menuItem[1],"100元");
		strcpy(menulist.menuItem[2],"200元");
		strcpy(menulist.menuItem[3],"一笔充完");
//		strcpy(menulist.menuItem[4],"0.1元");
        iRet=TS_Menu(dposInfo,&menulist,iSelect,MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return iRet;
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			strcpy(tradeamount,"000000005000");
			return 0;
		case 2:
			strcpy(tradeamount,"000000010000");
			return 0;
		case 3:
			strcpy(tradeamount,"000000020000");
			return 0;
		case 4:
			strcpy(tradeamount,czamo);
			return 0;
//		case 5:
//			strcpy(tradeamount,"000000000001");
//			return 0;

		default: 
			return CANCLE_KEY;
		}
	}
	else
	{ 
        menulist.iTotalNum=3;
		strcpy(menulist.acTitle,"请选择你充值的金额");
		strcpy(menulist.menuItem[0],"50元");
		strcpy(menulist.menuItem[1],"100元");
		strcpy(menulist.menuItem[2],"200元");
//		strcpy(menulist.menuItem[3],"0.1元");
        iRet=TS_Menu(dposInfo,&menulist,iSelect,MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return iRet;
		}
		else
		{
			NULL;
		}
		switch (iSelect)
		{
		case 1:
			strcpy(tradeamount,"000000005000");
			return 0;
		case 2:
			strcpy(tradeamount,"000000010000");
			return 0;
		case 3:
			strcpy(tradeamount,"000000020000");
			return 0;
//		case 4:
//			strcpy(tradeamount,"000000000001");
//			return 0;

		default: 
			return CANCLE_KEY;
		}
	}
	return 0;
}

int CYCTYPTrade::Yct_AutoTradeHdl(CGlobal pgbl, char *title)
{
    int iRet=0, iSelect=0,com=0,comType=0,itype=0;
	atuo_charge = 1;
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	char m_title[1024];
	char priCont[1024];
	char type[1024];

	bbzero(type);
	bbzero(m_title);
	bbzero(priCont);
	
	strcpy(m_title, title);
	YCT_DATA yct_data;
	CPack pack;
	
	init_yct_data(&yct_data);
 	char cur_signTime[30+1];
 	CDB::getYct3SignInfo(pgbl.dPosInfo.pos_no,&yct_data,cur_signTime);

	//进行充值金账户
	strcpy(yct_data.bank,pgbl.bank);
	strcpy(yct_data.card_no,pgbl.card_no);
	strcpy(yct_data.biz_type,pgbl.fee_type);
	strcpy(yct_data.term_id,pgbl.dPosInfo.pos_no);
	iRet=Yct_AcctHdl(pgbl.dPosInfo,com,comType,NORMALPAY,&yct_data,"000000000000",&m_party,&m_rspcode);
	if (iRet ==1)
	{
		// 1：与读卡器交互失败
		showErrMsg(pgbl,m_party,m_rspcode);
		return 1;
	}
	else if (iRet ==3)
	{
		showErrMsg(pgbl,m_party,m_rspcode);
		return 0;
	}
	else if (iRet ==5)
	{
		showErrMsg(pgbl,m_party,m_rspcode);
		itype=1;
		return 0;
	}
	else if (iRet == 0)
	{
		showErrMsg(pgbl,m_party,m_rspcode);
		CTools::HexToDec(yct_data.yct_stan,pgbl.stan_no,12); //自动充值时票据的系统参考号填羊城通流水号
		itype=0;
		set_yctthird_priCont(itype,&yct_data,priCont);
		CPrint ptr;
		ptr.set_in_par(pgbl);
		ptr.print_yctthird_detail("羊城通自动充值",priCont);
	}

	
	return 0;
}

//查询卡应用信息
//out: card_appinfo - 卡应用信息
int CYCTYPTrade::R_PUB_QRY_APP_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,\
									PYCT_DATA pyct_data,BYTE *card_appinfo,BYTE *ReaderStatus,int flag)
{
    int buflen=0 , len = 0 , iRet = 0,tmp_len = 0;
	int iSelect = 0;
	BYTE inBuf[256+1];
	BYTE outBuf[256+1];
	BYTE mny[8+1];
	char outPut_Msg[1024];
	char errMsg[1024];
	
	wrLog("【R_PUB_QRY_APP_INFO】");
	
	bbzero(inBuf);
	bbzero(outBuf);
	bbzero(outPut_Msg);
	bbzero(errMsg);
	
	iRet = CYCTYPTrade::R_COMMAND(dPosInfo , com , comType , 0x94 , (char*)inBuf , buflen , (char*)outBuf , &len);
	if (iRet != 0 && iRet != 1)
	{
		return 1;
	}

	bbzero(ReaderStatus);
	memcpy(ReaderStatus,outBuf+3,1);

	if (outBuf[3] != 0x00)
	{
		//提示转移失败
		get_cardReader_errMsg(ReaderStatus,errMsg);
		sprintf(outPut_Msg,"读卡器返回错误信息：%s",errMsg);
		TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
		return 2;
	}
	
    tmp_len+=4;
	
	bbzero(pyct_data->ticket_physicalNo);
	wrHex("物理卡号: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_physicalNo,outBuf+tmp_len,8);
	tmp_len+=8;
	
//	wrHex("逻辑卡号: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_no,outBuf+tmp_len,8);
	tmp_len+=8;
	
	wrHex("SAK: ",outBuf+tmp_len,1);
	tmp_len+=1;
	
	bbzero(card_appinfo);
	memcpy(card_appinfo,outBuf+tmp_len,40);
	wrHex("卡应用信息: ",card_appinfo,40);
	tmp_len+=40;
	
    bbzero(mny);
	CTools::byte_to_hex((BYTE *)outBuf+tmp_len,(BYTE *)mny,4); //钱包余额	
	if (flag == 0)
	{
		bbzero(pyct_data->ticket_amo_bef);
		CTools::HexToDec((char *)mny, pyct_data->ticket_amo_bef);
		wrHex("扣费前钱包余额: ",outBuf+tmp_len,4);
	}
	else if (flag == 1)
	{
		bbzero(pyct_data->ticket_amo_aft);
		CTools::HexToDec((char *)mny, pyct_data->ticket_amo_aft);
		wrHex("扣费后钱包余额: ",outBuf+tmp_len,4);
	}
	tmp_len+=4;
	
	bbzero(pyct_data->downLimit);
	memcpy(pyct_data->downLimit,outBuf+tmp_len,1);
	wrHex("金额下限: ",outBuf+tmp_len,1);
	tmp_len+=1;
	
	bbzero(pyct_data->upperLimit);
	memcpy(pyct_data->upperLimit,outBuf+tmp_len,3);
	wrHex("金额上限: ",outBuf+tmp_len,3);
	tmp_len+=3;
	
	wrHex("SW1SW2: ",outBuf+tmp_len,2);
	memcpy(pyct_data->SW1SW2,outBuf+tmp_len,2);
	tmp_len+=2;
	return 0;
}

//设置羊城通充值第三代打印明细内容
//in : type - 单据类型  0:充值成功单据   1:充值疑似成功单据 2:充值撤销单据 
//						3:转出卡圈提单据 4:转入卡圈存单据   5:转入卡圈存疑似成功单据
//out: priCont - 打印明细内容
int CYCTYPTrade::set_yctthird_priCont(int type,PYCT_DATA pyct_data,char *priCont)
{
	char auth_temp[8+1];
	char trader_name[21];
	char note[256];
	char yct_stan_temp[16+1];

	bbzero(pyct_data->trade_type);
	bbzero(trader_name);
	bbzero(note);
	if (type == 0)
	{
		strcpy(pyct_data->trade_type,"充值");
	}
	else if (type == 2)
	{
		strcpy(pyct_data->trade_type,"充值撤销");
	}
    else if (type == 3 )
    {
		strcpy(pyct_data->trade_type,"余额转出");
    }
	else if (type == 4 || type == 5)
	{
		strcpy(pyct_data->trade_type,"余额转入");
	}

		strcat(priCont, "交易卡号: ");
		strcat(priCont, pyct_data->ticket_no);
// 		strcat(priCont, "\n交易类型: ");
// 		strcat(priCont, pyct_data->trade_type);
		strcat(priCont, "\n管 理 卡: ");
		strcat(priCont, pyct_data->pki_card);
		
// 		if (type == 2)
// 		{
// 			strcat(priCont, "\n撤销时间: ");
// 		}
// 		else if (type == 3 || type ==4 || type == 5)
// 		{
// 			strcat(priCont, "\n转移时间: ");
// 		}
// 		else
// 		{
// 			strcat(priCont, "\n交易时间: ");
// 		}
// 		//转换时间
// 
// 		bbzero(fmt_time);
// 		bbzero(fmt_date);
// 		CTools::formatDate(pyct_data->txn_date,fmt_date,3);
// 		CTools::formatTime(pyct_data->txn_time,fmt_time);
// 		sprintf(priCont+strlen(priCont),"%s %s",fmt_date,fmt_time);
// 		
// 		
// 		strcat(priCont, "\n\n");//空两行
		if (type == 0 || type == 1)
		{
			strcat(priCont, "\n羊城通流水号: ");		
			bbzero(yct_stan_temp);
			CTools::HexToDec(pyct_data->yct_stan,yct_stan_temp,8);
			strcat(priCont, yct_stan_temp);
			strcat(priCont, "\n凭 证 号: ");
			bbzero(auth_temp);
			CTools::HexToDec(pyct_data->charge_authcode,auth_temp,8);
			strcat(priCont, auth_temp);
		}
		else if (type == 2)
		{
			strcat(priCont, "\n原系统参考号: ");
			strcat(priCont, pyct_data->stan);

			strcat(priCont, "\n原羊城通充值流水号: ");
			bbzero(yct_stan_temp);
			CTools::HexToDec(pyct_data->org_yctstan,yct_stan_temp,8);
			strcat(priCont, yct_stan_temp);
		}
		if (M1_CPU == 1)
		{
			sprintf(mny,"%s",pyct_data->ticket_amo_bef);
			//    			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
			Display_credit = atof(mny)/100;
			sprintf(show_money,"%.2f",Display_credit);
		}
		if (M1_CPU == 2)
		{
			sprintf(mny,"%s",pyct_data->ticket_amo_bef);
			Display_credit = atof(mny)/100 - atof(des_lim);
//			wrLog("钱包显示的余额:%.2f",Display_credit);
		}

		if (atuo_charge == 1 && M1_CPU == 1)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡原余额: RMB %.2f元", atof(pyct_data->ticket_amo_bef)/100);
		}
		if (atuo_charge == 1 && M1_CPU == 2)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡原余额: RMB %.2f元", atof(pyct_data->ticket_amo_bef)/100 - atof(des_lim));
		}
		if(atuo_charge != 1 && (M1_CPU != 1 || M1_CPU != 2))
			sprintf(priCont+strlen(priCont), "\n羊城通卡原余额: RMB %.2f元", Display_credit );

		if (type == 2)
		{
			sprintf(priCont+strlen(priCont), "\n撤销金额: RMB %.2f元", atof(pyct_data->amount)/100);	
		}
//		if (type == 1)
		if(moni ==1)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡现余额: RMB %.2f(*)元", Display_credit+atof(pyct_data->amount)/100);
		}
		if (type == 2)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡现余额: RMB %.2f元", Display_credit-atof(pyct_data->amount)/100);	
		}
		if (type != 2 && atuo_charge == 1 && M1_CPU == 1 && moni != 1)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡现余额: RMB %.2f元", atof(pyct_data->ticket_amo_aft)/100);
		}
		if (type != 2 && atuo_charge == 1 && M1_CPU == 2 && moni !=1)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡现余额: RMB %.2f元", atof(pyct_data->ticket_amo_aft)/100 - atof(des_lim) );
		}
		if(type != 2 && moni != 1 && atuo_charge != 1)
		{
			sprintf(priCont+strlen(priCont), "\n羊城通卡现余额: RMB %.2f元", Display_credit+atof(pyct_data->amount)/100);
		}

//		if (type == 1)
		if(moni ==1)
		{
			sprintf(priCont+strlen(priCont), "\n交易金额: RMB %.2f(*)元", atof(pyct_data->amount)/100);
		}
		else if (type != 2)
		{
			sprintf(priCont+strlen(priCont), "\n交易金额: RMB %.2f元", atof(pyct_data->amount)/100);	
		}
		CTools::hex_to_byte((BYTE*)pyct_data->note,(BYTE*)note,strlen(pyct_data->note));
		strcat(priCont, "\n备    注: ");
		strcat(priCont, note);

		strcat(priCont, "\n注:本单据仅供参考, 不作结算依据");
		strcat(priCont,"\n此票不作报销凭证");
		strcat(priCont,"\n客服电话400-80-11888");
		strcat(priCont,"\n\n\n\n\n\n");

	return 0;
}

//获取卡响应状态码含义
int CYCTYPTrade::get_cardStatus_errMsg(BYTE *SW1SW2,char *errMsg)
{
	char temp[10+1];
	char outstr[32];
	int  temp_int = 0;

	bbzero(temp);
	bbzero(errMsg);

	if (SW1SW2[0] == 0x61)
	{
		strcpy(errMsg,"正常处理");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"无信息提供");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"回送的数据可能有错");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"文件长度<Le");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x83)
	{
		strcpy(errMsg,"选择的文件无效");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x84)
	{
		strcpy(errMsg,"FC1格式与P2指定的不符");
	}
	else if (SW1SW2[0] == 0x63 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"认证失败");
	}
	else if (SW1SW2[0] == 0x63)
	{
		CTools::byte_to_hex((BYTE *)SW1SW2+1,(BYTE *)temp,2);
		if (memcmp(temp,"C",1) == 0)
		{
			bbzero(outstr);
			CTools::HexToDec(temp+1,outstr);//取出SW2第二位并转为十进制
			temp_int = atoi(outstr);
			sprintf(errMsg,"验证失败，还剩下%d次尝试机会",temp_int);
		}
	}
	else if (SW1SW2[0] == 0x64 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"状态标识位未变");
	}
	else if (SW1SW2[0] == 0x65 && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"内存错误");
	}
	else if (SW1SW2[0] == 0x67 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"长度错误");
	}
	else if (SW1SW2[0] == 0x68 && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"不支持安全报文");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"不能处理");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x01)
	{
		strcpy(errMsg,"命令不接受（无效状态）");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"命令与文件结构不相容");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"不满足安全状态");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x83)
	{
		strcpy(errMsg,"验证方法锁定");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x84)
	{
		strcpy(errMsg,"引用数据无效");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x85)
	{
		strcpy(errMsg,"使用条件不满足");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x86)
	{
		strcpy(errMsg,"不满足命令执行的条件（非当前EF）");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x87)
	{
		strcpy(errMsg,"安全报文数据项丢失");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x88)
	{
		strcpy(errMsg,"安全信息数据对象不正确");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x80)
	{
		strcpy(errMsg,"数据域参数不正确");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"功能不支持");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"未找到文件");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x83)
	{
		strcpy(errMsg,"未找到记录");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x84)
	{
		strcpy(errMsg,"文件中存储空间不够");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x86)
	{
		strcpy(errMsg,"P1、P2参数不正确");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x88)
	{
		strcpy(errMsg,"引用数据找不到");
	}
	else if (SW1SW2[0] == 0x6B && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"参数错误（偏移地址超出了EF）");
	}
	else if (SW1SW2[0] == 0x6C)
	{
		CTools::byte_to_hex((BYTE *)SW1SW2+1,(BYTE *)temp,1);
		bbzero(outstr);
		CTools::HexToDec(temp,outstr);//取出SW2并转为十进制
		temp_int = atoi(outstr);
		sprintf(errMsg,"长度错误，实际长度为：%d",temp_int);
	}
	else if (SW1SW2[0] == 0x6F && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"数据无效");
	}

	wrLog("卡响应状态码含义：%s",errMsg);
	return 0;
}

//获取读卡器错误信息
int CYCTYPTrade::get_cardReader_errMsg(BYTE *status,char *errMsg)
{
	int iStatus;
	bbzero(errMsg);

	iStatus = int(status);
	switch(iStatus)
	{
		case 0x60:
			strcpy(errMsg,"没有安装SAM卡");
			break;
		case 0x61:
			strcpy(errMsg,"SAM卡初始化错误或未初始化");
			break;
		case 0x62:
			strcpy(errMsg,"SAM卡检验PIN错误");
			break;
		case 0x63:
			strcpy(errMsg,"SAM卡类型与交易类型不匹配");
			break;
		case 0x64:
			strcpy(errMsg,"SAM卡选择文件错误");
			break;
		case 0x65:
			strcpy(errMsg,"SAM卡错误");
			break;
		case 0x66:
			strcpy(errMsg,"SAM卡写错误");
			break;
		case 0x67:
			strcpy(errMsg,"SAM卡认证错误");
			break;
		case 0x68:
			strcpy(errMsg,"SAM卡随机数错误");
			break;
		case 0x69:
			strcpy(errMsg,"SAM卡DES计算错误");
			break;
		case 0x6A:
			strcpy(errMsg,"SAM卡生成钱包密钥错误");
			break;
		case 0x71:
			strcpy(errMsg,"PKI卡RSA计算错误");
			break;
		case 0x72:
			strcpy(errMsg,"PKI卡DES计算错误");
			break;
		case 0x7E:
			strcpy(errMsg,"SAM卡执行APDU命令错误");
			break;
		case 0x7F:
			strcpy(errMsg,"SAM卡操作超时");
			break;
		case 0x80:
			strcpy(errMsg,"没有卡");
			break;
		case 0x81:
			strcpy(errMsg,"选择卡片错误");
			break;
		case 0x82:
			strcpy(errMsg,"停用卡片错误");
			break;
		case 0x83:
			strcpy(errMsg,"认证卡片错误");
			break;
		case 0x84:
			strcpy(errMsg,"卡片读操作错误");
			break;
		case 0x85:
			strcpy(errMsg,"卡片写操作错误");
			break;
		case 0x86:
			strcpy(errMsg,"卡片写操作中途中断");
			break;
		case 0x87:
			strcpy(errMsg,"充值卡片无响应");
			break;
		case 0x90:
			strcpy(errMsg,"不是本系统标准的卡片");
			break;
		case 0x91:
			strcpy(errMsg,"卡片超出有效期");
			break;
		case 0x92:
			strcpy(errMsg,"城市代码或应用代码错误");
			break;
		case 0x93:
			strcpy(errMsg,"非法卡");
			break;
		case 0x94:
			strcpy(errMsg,"黑名单卡");
			break;
		case 0x95:
			strcpy(errMsg,"钱包余额不足");
			break;
		case 0x96:
			strcpy(errMsg,"钱包余额超出上限");
			break;
		case 0x97:
			strcpy(errMsg,"钱包未启用");
			break;
		case 0x98:
		case 0x9A:
			strcpy(errMsg,"钱包已停用");
			break;
		case 0x99:
			strcpy(errMsg,"钱包正本被破坏");
			break;
		case 0x9F:
			strcpy(errMsg,"公共信息区被破坏");
			break;
		case 0xAF:
			strcpy(errMsg,"卡片操作超时");
			break;
		case 0xB0:
			strcpy(errMsg,"交易操作中途中断");
			break;
		case 0xB1:
			strcpy(errMsg,"交易中断");
			break;
		case 0xB2:
			strcpy(errMsg,"前一步指令未执行或执行失败");
			break;
		case 0xC1:
			strcpy(errMsg,"联机充值请求被拒绝");
			break;
		case 0xC2:
			strcpy(errMsg,"联机充值认证失败");
			break;
		case 0xC3:
			strcpy(errMsg,"交易结果提交错误");
			break;
		case 0xCE:
			strcpy(errMsg,"联机充值协议错误");
			break;
		case 0xCF:
			strcpy(errMsg,"交易操作超时");
			break;
		case 0xD0:
			strcpy(errMsg,"远程读写器执行错");
			break;
		case 0xE0:
			strcpy(errMsg,"MIFARE硬件初始化错误");
			break;
		case 0xE1:
			strcpy(errMsg,"SAM硬件初始化错误");
			break;
		case 0xE2:
			strcpy(errMsg,"命令错误");
			break;
		case 0xE3:
			strcpy(errMsg,"参数错误");
			break;
		case 0xE4:
			strcpy(errMsg,"检验和错误");
			break;
		case 0xE5:
			strcpy(errMsg,"线路通讯超时");
			break;
		case 0xE6:
			strcpy(errMsg,"内部FLASH写错误");
			break;
		case 0x30:
			strcpy(errMsg,"报文头错");
			break;
		case 0x31:
			strcpy(errMsg,"卡片不一致");
			break;	
		case 0x32:
			strcpy(errMsg,"流水号不一致");
			break;
		case 0x33:
			strcpy(errMsg,"MAC错");
			break;
		case 0x3F:
			strcpy(errMsg,"不支持的命令");
			break;
		default:
			strcpy(errMsg,"错误");
			break;
	}

	wrLog("读卡器返回错误信息：%s",errMsg);
	return 0;
}


int CYCTYPTrade::P_TRAN_QRY3(const TSDLLPosInfo& dPosInfo , PYCT_DATA pyct_data , unsigned char SAK , unsigned char* cardInfo , unsigned char exeFlag , unsigned char* outBuf , int* len )
{
	unsigned char inBuf[512] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = -1,iSelect;
    wrLog("【P_TRAN_QRY3】");
	//PKI
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//物理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//逻辑卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//SAK
	inBuf[bufLen ++] = SAK ;
	//卡物理信息1
	memcpy(inBuf + bufLen , cardInfo , 32);
	bufLen += 32;
	//执行操作
	inBuf[bufLen++] = exeFlag;
	//RFU
	bufLen += 34;
	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA51" , (BYTE*)"0003" ,  0x02 , 0x80 , inBuf , bufLen , outBuf , len);

	if (iRet != 0)
	{
		return 1;
	}
	if (ErrReturn(dPosInfo , outBuf + 16) != 0)
	{
		TS_OutPutMsg(dPosInfo , "撤销失败！" , 0 , iSelect );

		return 1;
	}
	return 0;
}

/************************************************************************/
/* Author: Sun Chengcheng                                               */
/* Data  : 2012-06-06 15:03                                             */
/* 此函数为与读卡器交互基础函数                                         */
/* 输入参数 dPosInfo，终端信息，com、comType，串口号、串口类型          */
/* commandCode：命令报文，inBuf、inLength，数据域及数据域长度           */
/* outBuf，outLength为输出参数，输出==全报文==                          */
/* 注意，此处输入的只是 文档中的 数据域 ，                              */
/* 如文档 8.4.2.3，R_CPU_LOAD，P117中，commandCode 为0x7B，而inBuf则是  */
/* 交易信息2，inLenght应该为32                                          */
/* 而输出参数outBuf则统一参考文档P101页，包括以下内容：                 */
/* Header(1字节) + Len(1字节) + Command(1字节) + Status(1字节) +        */
/* Data(此处则为交易认证码4字节+SW1SW2两字节) + CheckSum(1字节)         */
/* =11 字节 ， 故 *outLength = 11                                       */
/* 另外需要注意，输出为纯内存RAW字节序，需要使用者自行调用              */
/* byte_to_hex等转换为可见序列                                          */
/************************************************************************/
int CYCTYPTrade::R_COMMAND(const TSDLLPosInfo dPosInfo, const int com, const int comType,  \
						   int commandCode , char* inBuf , int inLength , char* outBuf, int* outLength)
{
	int len=0;
	int sndLen = 0;
	int iRet = 0;
	int rcvLen=0;
	BYTE icData [255] = { 0 } ;
	BYTE sndData[255] = { 0 } ;
	int baudRate=57600;

	sndLen = ST_packData(ST_HEAD, inLength + 2 , commandCode, (BYTE *)inBuf, (BYTE *)sndData);	
	iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)outBuf, outLength, baudRate);
	if( iRet != 0)
	{
		return 1;
	}

	if( outBuf[3] != 0x00)
	{
		return 1;	
	}

	return 0;
}

/************************************************************************/
/* Author: Sun Chengcheng                                               */
/* Data  : 2012-06-06 15:03                                             */
/* 此函数为与充值后台交互基础函数                                       */
/* 输入参数 dPosInfo，终端信息，PYCT_DATA ， 羊城通结构信息，           */
/* 使用到了shid、yct_sequence_NO，故不得为空                            */
/* infoType、ver、为信息类型码、报文版本号，                            */
/* 传ASCII码即可，如 (BYTE*)"AA74"                                      */
/* encryptMethod、endian，报文体加密算法、数值内容字节序，              */
/* 十六进制整形传入，目前加密算法只支持0x02                             */
/* inBuf , inLen ，为报文体中除了“握手流水号、报文序号，校验码之外”的 */
/* 其他全部内容极其长度，与R_COMMAND中的inBuf和inLenght类似             */
/* outBuf，outLen为输出参数，输出==全报文== ，与R_COMMAND类似，         */
/* 输出解密后的全报文，包括报文头、报文体，不细述                       */
/* 具体使用可参考诸如P_TRAN_QRY3，P_CPU_CANCLE等范例使用函数            */
/* 另外需要注意，                                                       */
/* 1、输出为纯内存RAW字节序，需要使用者自行调用byte_to_hex等转换        */
/* 为可见序列 2、函数内部自动实现AES加密、MD5校验，                     */
/* 但因为各报文对于返回码放置位置不一，没有封装返回码判断，             */
/* 可自行使用ErrReturn                                                  */
/************************************************************************/
int CYCTYPTrade::P_COMMAND(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data ,const unsigned char* infoType,  \
						   const unsigned char* ver, int encryptMethod , int endian , unsigned char* inBuf , int inLen , \
							unsigned char* outBuf, int* outLen)
{
	int sndLen = 0;
	int Len    = 0;
	int iRet   = 0;
	unsigned char headerData[256] = { 0 };					//报文头
	unsigned char bodyData[256] ={0 };						//报文体
	unsigned char sndData[512] = { 0 };						//待发送报文
	unsigned char md5_dig[16] = { 0 };
	unsigned char byteTmp[512] = { 0 };
	unsigned char debugInfo[256]= { 0 };
	char undecripdata[512] = { 0 };
	char SHID[16+1];

	CTools::hex_to_byte( infoType , headerData , 4);		//信息类型码
	sndLen += 2;
	CTools::hex_to_byte( ver      ,  headerData+sndLen , 4);//报文版本号
	sndLen += 2;
	headerData[4] = encryptMethod;							//报文体加密算法
	headerData[5] = endian;									//数值内容字节序
	sndLen += 2;
	sprintf((char*)byteTmp , "%04X" , 16 + inLen);
	CTools::hex_to_byte( byteTmp , headerData + sndLen , 4);
	sndLen += 2;
	memcpy(sndData , headerData , sndLen);					//报文头完毕

	bbzero(SHID);
	strcpy(SHID,(char *)pyct_data->SHID);
	CTools::hex_to_byte((BYTE *)SHID , bodyData , 16);	//握手流水号
	Len += 8;
	char seq_NO[10] = { 0 };
	sprintf(seq_NO , "%08X" , pyct_data->yct_sequence_NO);	
	CTools::hex_to_byte( (BYTE*)seq_NO ,  bodyData+Len , 8);//报文序号
	Len += 4;
	memcpy(bodyData + Len , inBuf , inLen);					//报文信息域，传入后直接copy，使用此函数需要注意此处
	Len += inLen;

	md5_csum( bodyData , Len ,  md5_dig );					//MD5SUM校验码区
	memcpy(bodyData+Len , md5_dig , 4);
	Len += 4;
	
	memcpy(debugInfo , headerData , sndLen);
	memcpy(debugInfo + sndLen , bodyData , Len);
	wrHex("欲发送报文-未加密:" , debugInfo , sndLen + Len);
	CTools::byte_to_hex((BYTE *)debugInfo,(BYTE *)undecripdata,sndLen + Len);
	wrLog("undecripdata: %s",undecripdata);

	//握手流水号加密
	CDes::TripleDesEncrypt((char*)CKEY, (char*)pyct_data->SHID , (char*)byteTmp );
	CTools::hex_to_byte(byteTmp , sndData + 8 , 16);
	sndLen += 8;
	//报文体其他部分加密
	int encrLen = 0;
	unsigned char* encr_packdata =  AESEncryptEx( bodyData+8 , Len - 8 , pyct_data->SKEY , 16, &encrLen );
	memcpy(sndData + 16 , encr_packdata , encrLen);
	free(encr_packdata);
	encr_packdata = NULL;
	sndLen = 8 + 8 + encrLen ;								//包头+握手流水号+已加密包体部分
	//通讯羊城通服务器
	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(YCT_IP , YCT_PORT , &sd);
	if (iRet != 0 )
	{
		return 1;
	}
	unsigned char rcvData[256] = { 0 };
	int  rcvLen		  = 0 ;

	//保存报文原文到result_message_third,设置交易状态为0 如果交易类型是AA73 M1充值撤销提交，AA95 cpu充值撤销提交，AA85cpu充值提交
	if (strcmp((char *)infoType,"AA73")==0 || strcmp((char *)infoType,"AA95")==0 ||strcmp((char *)infoType,"AA85")==0 )
	{
		bbzero(pyct_data->result_message);
		memcpy(pyct_data->result_message,undecripdata,strlen(undecripdata));
		CDB::updateYctResultMsg(pyct_data,pyct_data->result_message);
	}

	iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data , 0 , 0 ,sd , sndData , sndLen ,  rcvData , &rcvLen);

	unsigned char header_tag[4] = { 0 };
	CTools::byte_to_hex( rcvData , header_tag , 2);
	if (strcmp((char*)header_tag , "FE04") == 0)
	{
		return 2;									//FE04报文，需要重新签到
	}
	if (rcvData[4] != 0x02)
	{
		return 1;									//暂不支持除0x02 AES-128以外的加密方式
	}

	unsigned char bodyDecrypted[256] = { 0 };		//解密后的报文体
	char tmp_shid[16+2] = { 0 };	
	int pack_len = 0;

	CTools::byte_to_hex( rcvData + 8 , byteTmp , 8);		//握手流水号
	CDes::TripleDesDec((char*)CKEY , (char*)byteTmp , tmp_shid);		//解密握手流水号
	CTools::hex_to_byte((BYTE*)tmp_shid , bodyDecrypted , 16);
	pack_len += 8;

	int encr_len;
	BYTE* des_data = AESDecryptEx( rcvData + 16 , rcvLen - 16 , pyct_data->SKEY , 16 , &encr_len);
	memcpy(bodyDecrypted + pack_len , des_data , encr_len - 4);
	pack_len += encr_len - 4;

	char MAC1[20] = { 0 };
	char MAC2[20] = { 0 };
	memcpy(MAC1 , des_data + (encr_len - 4) , 4);
	md5_csum(bodyDecrypted , pack_len , (BYTE*)MAC2);
	MAC2[4] = 0;

	if (memcmp(MAC1 , MAC2 , 4) != 0 )
	{
		wrLog("MAC校验错误");
		return 1;									//服务器返回MAC校验错误
	}

	memcpy(outBuf , rcvData , 8);					//报文头
	memcpy(outBuf + 8 , bodyDecrypted , pack_len);	//报文体不含校验码
	memcpy(outBuf + 8 + pack_len , MAC2 , 4);		//校验码
	*outLen = 8 + pack_len + 4 ;					//返回长度
	wrHex("收到报文-已解密:" , outBuf , *outLen);
	free(des_data);
	des_data = NULL;
	return 0;
}

//充值撤销 flow:原系统流水号，pz：充值凭证号，amount：交易金额，已分为单位
//撤销前就已失败，比如卡片黑名单，找不到凭证号等等，返回1
//成功返回0
int CYCTYPTrade::Charge_Cancle(CGlobal *pgbl ,  const char* flow , 	const char* pz , const char* _amount,YCT_DATA pyct_data )
{
	unsigned char SAK[1+2] = { 0 };
	//YCT_DATA yct_data;
	//PYCT_DATA pyct_data = &yct_data;
	unsigned char card_pycicalInfo1[32+2] = { 0 };
	int iRet = -1 , com=0, comType=0;
	unsigned char card_physics_info[51] = { 0 };
	unsigned char SW1SW2[4+2] = { 0 };
	double amount = atof(_amount);
	char outBuf[1024] = { 0 };
	int  outLen		  = 0;
	int iSelect		  = -1;	
	int charge_type = 0;
	char priCont[1024] = {0};
	CTrade trade;

	//init_yct_data(pyct_data);

	strcpy(pgbl->amount,_amount);
	strcpy(pyct_data.org_yctstan,flow); //羊城通撤销流水号
	com = pgbl->com;
	comType = pgbl->comType;
// 	if (init_sign(pgbl , com , comType , yct_data) != 0)
// 	{
// 		return 1;
// 	}
	
	char cur_signTime[30+1];
	CDB::getYct3SignInfo(pgbl->dPosInfo.pos_no,&pyct_data,cur_signTime);

	//读卡物理信息
	do 
	{		
		iRet = R_PUB_QRY_PHYSICS_INFO2(pgbl->dPosInfo , com , comType , (BYTE*)outBuf , &outLen);
		if (iRet == 0)
		{
			CTools::byte_to_hex((BYTE*)outBuf + 4 , (BYTE*)pyct_data.ticket_physicalNo , 8);
			CTools::byte_to_hex((BYTE*)outBuf + 12, (BYTE*)pyct_data.ticket_no , 8);
			wrLog("物理卡号:%s",pyct_data.ticket_physicalNo);
			//wrLog("逻辑卡号:%s",pyct_data.ticket_no);
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo , "读卡错误！如果没放卡，请把卡放到读卡器上！\n按【Enter】重试，按【ESC】返回。" ,0 , iSelect );
			if (iSelect != 0)
			{
				return iRet ;
			}
		}
	} while (iRet != 0); //取消或者读卡正确
	
	//公用交易类型查询
here:	iRet = P_TRAN_QRY3(pgbl->dPosInfo , &pyct_data , outBuf[20] , (BYTE*)outBuf + 21 , 0x02 , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return 1;
	}
	if (outBuf[20] == 0x00)
	{
		iRet = Charge_Cancle_M1(pgbl , &pyct_data , com , comType ,flow , pz , amount,(BYTE *)outBuf + 21);

	}
	else if (outBuf[20] == 0x20)
	{
		iRet = Charge_Cancle_CPU(pgbl , &pyct_data , com , comType, flow , pz , amount,(BYTE *)outBuf + 21);
	}
	else
	{
		return 1;
	}

	if (iRet ==1)
	{
		//执行交易类型查询
		goto here;
	}
	
	if (iRet==-1) //撤销失败
	{
		return 1;
	}
	
	strcpy(pyct_data.amount,_amount);
	CDB::getChargeType((char *)flow,charge_type,pyct_data.stan,pgbl->pos_serial); //edit by liuxiao cut 
	
	bbzero(pgbl->stan_no);
	strcpy(pgbl->stan_no,pyct_data.stan);
	bbzero(pyct_data.term_serial);
	strcpy(pyct_data.term_serial,pgbl->pos_serial);

	trade.set_in_par(*pgbl);

	if (pyct_data.status ==4)
	{
		//现金充值需PE冲正
		wrYctDataLog("after charge cancel", pyct_data);
		//PE确认与冲正
		
		pyct_data.status = trade.YCT_AckRevTrade( pyct_data,pyct_data.status);
		if (pyct_data.status==5) //如果冲正成功，需要发送确认包
		{
			pyct_data.status=trade.YCT_AckRevTrade(pyct_data,pyct_data.status);
		}

		if (pyct_data.status == 5)
		{		
		
		//	CDB::updateYctTxnStatus((char *)flow,"8"); //更新第三代羊城通数据 edit by liuxiao
			pyct_data.status=8;
			CDB::updateYctYPData(pyct_data);
			//设置打印信息
			TS_OutPutMsg(pgbl->dPosInfo,"羊城通充值撤销成功\n按【确认】打印凭证",1,iSelect);
			set_yctthird_priCont(2,&pyct_data,priCont);
			CPrint prt;
			CPack pack;
			char m_title[64];
			strcpy(m_title,"羊城通充值撤销");
			prt.set_in_par(*pgbl);
			prt.print_yctthird_detail(m_title,priCont);
		}
		else
		{
			//CDB::updateYctTxnStatus((char *)flow,"9");
			pyct_data.status=9; //撤销失败
			CDB::updateYctYPData(pyct_data);
			TS_OutPutMsg(pgbl->dPosInfo , "E卡冲正失败，羊城通充值撤销失败，请联系在线客服【Ctrl+H】" ,0 , iSelect );
		}
	}
	else
	{		
	
		//CDB::updateYctTxnStatus((char *)flow,"8"); //edit by liuxiao
		pyct_data.status=8;
		CDB::updateYctYPData(pyct_data);

		//设置打印信息
		set_yctthird_priCont(2,&pyct_data,priCont);
		TS_OutPutMsg(pgbl->dPosInfo,"羊城通充值撤销成功\n按【确认】打印凭证",1,iSelect);
		CPrint prt;
		CPack pack;
		char m_title[64];
		strcpy(m_title,"羊城通充值撤销");
		prt.set_in_par(*pgbl);
		prt.print_yctthird_detail(m_title,priCont);
	}

	return 0;
}

//CPU卡撤销
int CYCTYPTrade::Charge_Cancle_CPU(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow , const char* pz , double amount,BYTE *query_info)
{
	int iRet = 1 , outLen = 0;
	char outBuf[1024] = { 0 };
	char SW1SW2[64] = { 0 };
	unsigned char byteTmp[64] = { 0 };
	BYTE rcvData[512] = { 0 };
	BYTE temp[14+1] = { 0 };
	BYTE followerup_oper[1+1] =  { 0 };
	int iSelect = 0;
	char priCont[1024] = { 0 };

	//设置准备读卡
	iRet = R_PUB_SET_READCARDINFO(pgbl->dPosInfo, com , comType  , pyct_data->ticket_no , query_info,  (char*)SW1SW2);
	if (iRet != 0)
	{
		return -1;
	}
	
	//读CPU卡信息
	char countInfo[16+2] = { 0 }	;
	//iRet = R_CPU_GET_CARDINFO(pgbl->dPosInfo , com , comType , (BYTE*)outBuf , &outLen);
	flag1 = 0;
	R_CPU_GET_READCARDINFO(pgbl->dPosInfo , com , comType,pyct_data,(BYTE *)countInfo,flag1);
	if (cpurspStatus==0x80 || cpurspStatus==147)
	{
		for (int j = 0; j<2;j++)
		{
			if (cpurspStatus == 0)
			{
				break;
			}
			R_CPU_GET_READCARDINFO(pgbl->dPosInfo , com , comType,pyct_data,(BYTE *)countInfo,flag1);
		}

	}

	if (cpurspStatus != 0)
	{
		return -1;
	}
// 	if (iRet == 0)
// 	{
// 		//设置YCT_DATA，显示卡信息
// 		CTools::byte_to_hex((BYTE *)outBuf+4,(BYTE *)pyct_data->pki_card,4);//pki管理卡号
// 		CTools::byte_to_hex((BYTE *)outBuf+8,(BYTE *)pyct_data->ticket_physicalNo,8);//物理卡号
// 		CTools::byte_to_hex((BYTE *)outBuf+16,(BYTE *)pyct_data->ticket_no,8); //逻辑卡号
// 		CTools::byte_to_hex((BYTE *)outBuf+24,(BYTE *)pyct_data->ticket_amo_bef,4); //钱包余额
// 		CTools::byte_to_hex((BYTE *)outBuf+28,(BYTE *)pyct_data->downLimit,1); //钱包下限
// 		CTools::byte_to_hex((BYTE *)outBuf+29,(BYTE *)pyct_data->upperLimit,3);//羊城通金额上限
// 		CTools::byte_to_hex((BYTE *)outBuf+49,(BYTE *)pyct_data->SW1SW2,2); //卡片返回状态码
// 		pyct_data->card_status=outBuf[32];
// 		memcpy(countInfo,outBuf+33,16);//计数信息
// 		
// 		wrLog("====读卡信息====");
// 		wrLog("管理卡号:%s",pyct_data->pki_card);
// 		wrLog("逻辑卡号:%s",pyct_data->ticket_no);
// 		wrLog("卡片余额为:%s",pyct_data->ticket_amo_bef);//羊城通余额
// 		wrLog("卡片上限:%s",pyct_data->upperLimit);		//羊城通cpu卡上限
// 		wrLog("卡片下限为：%s",pyct_data->downLimit); //羊城通cpu卡下限
// 		wrLog("羊城通卡卡状态:%s",pyct_data->SW1SW2); //羊城通返回状态码
// 		wrLog("====读卡信息====");
// 	}
// 	else
// 	{
// 		return 1;
// 	}

	iRet = P_CPU_CANCLE_QRY(pgbl->dPosInfo , pyct_data , (BYTE*)countInfo , flow , pz , amount , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return -1;
	}
	
	unsigned char curFlow[8 + 2] = { 0 };
	unsigned char retCode[4 + 2] = { 0 };
	unsigned char bizInfo[32+ 2] = { 0 };
	unsigned char RFU    [32+ 2] = { 0 };
	memcpy(curFlow , outBuf + 16 , 8);
	bbzero(pyct_data->yct_stan);
	memcpy(pyct_data->yct_stan,outBuf+16,8);
	memcpy(retCode , outBuf + 24 , 4);
	memcpy(bizInfo , outBuf + 28 , 32);
	memcpy(RFU     , outBuf + 60 , 32);
	
	iRet = R_CPU_CANCLE_INIT(pgbl->dPosInfo , com , comType ,(char*) bizInfo , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return -1;
	}
	unsigned char tjjyxh[2+2] = { 0 };		//脱机交易序号
	unsigned char cardInfo1[128 + 2] = { 0 };
	unsigned char sw1sw2_tmp[2+2] = { 0 };
	unsigned char r_cpu_cancle_init_status = outBuf[3];	//status
	memcpy(tjjyxh , outBuf +4 , 2);
	memcpy(cardInfo1 , outBuf + 6 , 128);
	memcpy(sw1sw2_tmp , outBuf + 134 , 2);
	
	iRet = P_CPU_CANCLE(pgbl->dPosInfo , pyct_data , (char *)curFlow , cardInfo1 , sw1sw2_tmp , (int)r_cpu_cancle_init_status , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return -1;
	}
	unsigned char bizInfo4[32+2] = { 0 };
	memcpy(bizInfo4 , outBuf + 28, 32);
	
	iRet = R_CPU_CANCLE(pgbl->dPosInfo , com , comType , amount , bizInfo4 , (BYTE*)outBuf , &outLen);
	if (iRet != 0 )
	{
		return -1;
	}
	memcpy(sw1sw2_tmp , outBuf + 12 , 2);
	unsigned char r_cpu_cancle_status = outBuf[3];
	unsigned char bal_bef[12+1] = { 0 };
	memcpy(bal_bef , pyct_data->ticket_amo_bef , 13);
	
	//终端重新读卡，获取卡片余额等信息
	unsigned char cardInfo[512] = { 0 };
	//iRet = R_CPU_GET_CARDINFO(pgbl->dPosInfo , com , comType , cardInfo ,&outLen);
	flag1 = 1;
	R_CPU_GET_READCARDINFO(pgbl->dPosInfo , com , comType,pyct_data,(BYTE *)countInfo,flag1);
	int num;
	if (cpurspStatus==0x80 || cpurspStatus==147)
	{
		for ( num=0; num<2;num++)
		{
			if (cpurspStatus == 0)
			{
				break ;
			}
			R_CPU_GET_READCARDINFO(pgbl->dPosInfo , com , comType,pyct_data,(BYTE *)countInfo,flag1);
			
		}

	}

	if (num >= 2 || cpurspStatus != 0)
	{
		return -1;
	}
// 	bbzero(pyct_data->ticket_amo_aft);
// 	char tmp_amo[12+1];
// 	bbzero(tmp_amo);
// 	CTools::byte_to_hex((BYTE*)cardInfo + 24 ,(BYTE *)tmp_amo, 4);
// 	CTools::HexToDec(tmp_amo,pyct_data->ticket_amo_aft);
	//memcpy(pyct_data->ticket_amo_aft,(BYTE*)cardInfo + 24 , 4);
	//wrHex("撤销后钱包余额：" , (BYTE*)cardInfo + 24 , 4);

	//比对前后余额减值
	char amo_bef[12+1];
	char amo_aft[12+1];
	bbzero(amo_aft);
	bbzero(amo_bef);
// 	CTools::HexToDec(pyct_data->ticket_amo_bef,amo_bef);
// 	CTools::HexToDec(pyct_data->ticket_amo_aft,amo_aft);

	memcpy(amo_bef,pyct_data->ticket_amo_bef,12);
	memcpy(amo_aft,pyct_data->ticket_amo_aft,12);
	bbzero(pyct_data->amount);
	char tmp_amo[12+1];
	bbzero(tmp_amo);
	sprintf(tmp_amo,"%12ld",amount);
	strcpy(pyct_data->amount,tmp_amo);
	if (atol(amo_aft) == atol(amo_bef) - long(amount))
	{
		wrLog("撤销成功");
		pyct_data->status='4';
	}
	else
	{
		wrLog("撤销失败");
		TS_OutPutMsg(pgbl->dPosInfo,"撤销失败!",1,iSelect);
		return -1; //撤销失败，界面需要返回错误

	}
	
	for (int i=0 ; i != 3; ++i)
	{
		//默认三次提交，成功即退出
		iRet = P_CPU_CANCLE_SUBMIT(pgbl->dPosInfo , pyct_data , (BYTE*)curFlow , (BYTE*)outBuf + 4 , (BYTE*)outBuf + 8 , \
			(BYTE*)sw1sw2_tmp , r_cpu_cancle_status,  cardInfo + 24 ,cardInfo + 29, cardInfo[28],(BYTE*)countInfo , (BYTE*)outBuf , &outLen);
		if (iRet == 0)
		{
			break;
		}
	}

	memcpy(rcvData,(BYTE *)outBuf,outLen);

	int len = 28;
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

	if (followerup_oper[0] != 0x00)
	{
		return 1;
	}
  
	return 0;
}

//M1卡撤销
int CYCTYPTrade::Charge_Cancle_M1(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow , const char* pz , double amount,BYTE *query_info)
{
	int iRet = 1 ,  outLen = 0 , rspCode = 0 , iSelect = 0;
	char outBuf[1024] = { 0 };
	char SW1SW2[64] = { 0 };
	char trade_info[88] = { 0 };
	BYTE cancel_info[64]={0}; //撤销信息
	char cancle_status; //撤销结果，读卡器返回
	char cancle_stan[8]={0}; //撤销系统流水号
	BYTE followup_oper[2+1];
	int len=28;
	char temp[20+1];

	bbzero(followup_oper);
	bbzero(temp);

	iRet = R_PUB_SET_READCARDINFO(pgbl->dPosInfo, com , comType  , pyct_data->ticket_no , query_info,  (char*)SW1SW2);
	if (iRet != 0)
	{
		return -1;
	}
	
	char countInfo[16+2] = { 0 } ;
	iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,0);
	if (rspCode == 128 || rspCode == 132)
	{
		TS_OutPutMsg(pgbl->dPosInfo,"请把卡放到读卡器上",1,iSelect);
		for (int i = 0;i<2; i++)
		{
			iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,0);
			
			if (rspCode != 128 && rspCode != 132)
			{
				break;
			}
			TS_OutPutMsg(pgbl->dPosInfo,"请把卡放到读卡器上",1,iSelect);
		}	
	}
	if (iRet != 0)
	{
		
		return -1;
	}

	outLen=0;
	bbzero(outBuf);
	iRet = P_M1_CANCLE(pgbl->dPosInfo , pyct_data , flow , amount , (BYTE*)pz , (BYTE*)trade_info , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		//TS_OutPutMsg(pgbl->dPosInfo,"撤销失败！",1,iSelect);
		return -1;
	}

	memcpy(cancle_stan,outBuf+16,8); //撤销系统流水号
	unsigned char bizData[16] = { 0 };//R_M1_CANCLE_INIT、R_M1_CANCLE需要的交易数据，由上一部获得
	memcpy(bizData , outBuf + 28 , 16);

	iRet = R_M1_CANCLE_INIT(pgbl->dPosInfo , com , comType , (char*)bizData , (BYTE*)outBuf , &outLen);//需要获取返回数据的撤销初始化数据
	
	if (iRet != 0)
	{
		return -1;
	}
	
	memcpy(cancel_info,outBuf+4,64); //撤销信息

	bbzero(outBuf);
	outLen=0;
	iRet = R_M1_CANCLE(pgbl->dPosInfo , com , comType , (char*)bizData , (BYTE*)outBuf , &outLen);
	cancle_status=outBuf[3]; //撤销结果
	wrHex("撤销读卡器返回",(BYTE *)outBuf,outLen);

	if (iRet != 0)
	{
		return -1; //如果卡撤销执行错误
	}

	iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,1);
	if (rspCode == 128 || rspCode == 132)
	{
		TS_OutPutMsg(pgbl->dPosInfo,"请把卡放到读卡器上",1,iSelect);
		for (int i = 0;i<2; i++)
		{
			iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,1);
			
			if (rspCode != 128 && rspCode != 132)
			{
				break;
			}
			TS_OutPutMsg(pgbl->dPosInfo,"请把卡放到读卡器上",1,iSelect);
		}	
	}
	
	if (iRet != 0)
	{
		return -1;
	}
	
	for (int i=0 ; i != 3; ++i)
	{
		//默认三次提交，成功即退出
		bbzero(outBuf);
		outLen=0;
		iRet = P_M1_CANCLE_SUBMIT(pgbl->dPosInfo,pyct_data,cancle_stan,cancel_info,(BYTE *)trade_info,cancle_status,(BYTE *)outBuf,&outLen);
		
		if (iRet == 0 || i<3)
		{
			break;
		}

	}
	//更新第三代读卡器数据；

	if (iRet!=0)
	{
		pyct_data->yct_ack=0; //交易需要重发commit包

	}

	bbzero(temp);
	len=28;

	memcpy(temp,outBuf + len,7);
	wrHex("交易日期时间: ",(BYTE *)outBuf + len,7);
	CTools::byte_to_hex((BYTE *)temp,(BYTE *)pyct_data->tradeTime,14);
    len+=7;
	
	bbzero(pyct_data->yct_termid);
	memcpy((BYTE *)pyct_data->yct_termid,outBuf + len,6);
	wrHex("终端编号: ",(BYTE *)pyct_data->yct_termid,6);
	len+=6;
	
	bbzero(pyct_data->trader_id);
	memcpy((BYTE *)pyct_data->trader_id,outBuf + len,8);
	wrHex("商户编号: ",(BYTE *)pyct_data->trader_id,8);
	len+=8;
	
	bbzero(pyct_data->trader_name);
	memcpy((BYTE *)pyct_data->trader_name,outBuf + len,20);
	wrHex("商户简称: ",(BYTE *)pyct_data->trader_name,20);
	len+=20;
	
	bbzero(pyct_data->note);
	memcpy(pyct_data->note,outBuf + len,64);
	wrHex("备注: ",(BYTE *)pyct_data->note,64);
	len+=64;
	
	bbzero(followup_oper);
	memcpy(followup_oper,outBuf + len,1);
	wrHex("后续操作: ",followup_oper,1);

	//判断是否撤销成功 撤销金额等于前后减值
	wrLog("撤销前余额:%.2f",atof(pyct_data->ticket_amo_bef));
	wrLog("撤销后余额:%.2f",atof(pyct_data->ticket_amo_aft));
	wrLog("交易金额:%.2f",amount);
	wrLog("撤销状态:%s",cancle_status);

	
	if (cancle_status=='\x00' && atof(pyct_data->ticket_amo_aft)==atof(pyct_data->ticket_amo_bef)-amount)
	{
		//撤销成功
		pyct_data->status=4;
	}

	else
	{	
		TS_OutPutMsg(pgbl->dPosInfo,"撤销失败!",1,iSelect);
		return CANCLE_KEY; //撤销失败，界面需要返回错误
	}

	if (followup_oper[0] != 0x00)
	{
		return 1;
	}
	
	//end of liuxiao
	return 0;
}

int CYCTYPTrade::P_CPU_CANCLE_QRY(const TSDLLPosInfo dPosInfo, PYCT_DATA pyct_data ,\
								  unsigned char* countInfo , const char* flow, const char* pz,\
								  int amount, unsigned char* outBuf , int* len)
{
	wrLog("【P_CPU_CANCLE_QRY】");
	BYTE hexamo[8+1] = {0};
	unsigned char inBuf[1024] = { 0 } ;
	unsigned char bytetmp[256]= { 0 } ;
	int bufLen = 0 , iRet = 0;
	//PKI管理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf , bytetmp , 4);
	bufLen += 4;
	//物理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//逻辑卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//钱包余额
	CTools::getHexAmo(pyct_data->ticket_amo_bef,(BYTE *)hexamo);
	wrHex("hexamo: ",hexamo,4);
	//CTools::hex_to_byte((BYTE*)hexamo , bytetmp , 8);
	//wrHex("bytetmp: ",bytetmp,4);
	memcpy(inBuf + bufLen , hexamo , 4);
	bufLen += 4;
	//金额下限
	CTools::hex_to_byte((BYTE*)down_limit , bytetmp , 2);
	memcpy(inBuf + bufLen , bytetmp , 1);
	bufLen += 1;
	//计数信息
	memcpy(inBuf + bufLen , countInfo , 16);
	bufLen += 16;
	//交易金额
	char amount_tmp[10] = {0 };
	sprintf(amount_tmp , "%08X" , amount);
	CTools::hex_to_byte((BYTE*)amount_tmp , bytetmp , 8);
	wrHex("交易金额：" , bytetmp , 4);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//系统流水号
	CTools::hex_to_byte((BYTE*)flow , bytetmp , 16);
	wrHex("系统流水号：" , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//充值凭证号
	CTools::hex_to_byte((BYTE*)pz , bytetmp , 8);
	wrHex("充值凭证号：" , bytetmp , 4);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//RFU
	bufLen += 31;	//保留，为空

	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA91" , (BYTE*)"0001" , 0x02 , 0x80 , \
		 inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return iRet;	//发送 or 接收出错
	}
	if (ErrReturn(dPosInfo , outBuf + 24) != 0)
	{
		return 1;		//返回码错误
	}
	return 0;
}

int CYCTYPTrade::R_CPU_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, char* bizInfo3 , unsigned char* outBuf , int* outLen)
{
	wrLog("【R_CPU_CANCLE_INIT】");
	return R_COMMAND(dPosInfo , com , comType , 0x75 , bizInfo3, 32, (char*)outBuf , outLen);
}

int CYCTYPTrade::P_CPU_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , const unsigned char* cardinfo1 , const unsigned char* retCode , int readerCode , unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = 0;
	wrLog("【P_CPU_CANCLE】");

	//系统流水号
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//PKI
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//卡信息1
	memcpy(inBuf + bufLen , cardinfo1 , 128);
	bufLen += 128;
	//卡片返回码
	memcpy(inBuf + bufLen , retCode , 2);
	bufLen += 2;
	//读卡器返回码
	inBuf[bufLen] = readerCode ;
	++bufLen;
	//RFU
	bufLen += 25;
	
	iRet = P_COMMAND(dposinfo , pyct_data , (BYTE*)"AA93" , (BYTE*)"0001" , 0x02 , 0x80 , inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return iRet;
	}
	if (ErrReturn(dposinfo , outBuf + 24) != 0)
	{
		return 1;
	}
	return 0;
}

int CYCTYPTrade::R_CPU_CANCLE(const TSDLLPosInfo dPosInfo , int com , int comtype , double amount , const unsigned char* bizInfo4 , unsigned char* outBuf , int* len)
{
	wrLog("【R_CPU_CANCLE】");
	unsigned char inBuf[255] = { 0 };
	char tmpamount[8+2] = { 0 };
	sprintf(tmpamount , "%08X" , amount);
	CTools::hex_to_byte((BYTE*)tmpamount , inBuf , 8);
	memcpy(inBuf + 4 , bizInfo4 , 32);
	return R_COMMAND(dPosInfo , com , comtype , 0x76 , (char*)inBuf ,36 , (char*)outBuf , len);
}

int CYCTYPTrade::R_PUB_GETVERSION(const TSDLLPosInfo dPosInfo , int com , int comType , unsigned char* outbuf , int* len)
{
	return R_COMMAND(dPosInfo , com , comType , 0x60 , NULL , 0 , (char*)outbuf , len);
}

int CYCTYPTrade::R_PUB_GET_PKISTATE(const TSDLLPosInfo dPosInfo , int com , int comType , unsigned char* outBuf , int* len)
{
	return R_COMMAND(dPosInfo , com , comType , 0x65 , NULL , 0 , (char*)outBuf , len);
}

/*
int CYCTYPTrade::termMonitor(LPVOID arg,PYCT_DATA pyct_data)
{
	CGlobal* pgbl = (CGlobal*)arg;
	unsigned char monitor[114]    = { 0 };
	unsigned char setmonitor1[87] = { 0 };
	unsigned char setmonitor2[256]= { 0 };
	unsigned char byteTmp[256]    = { 0 };
	char termVer[16+2] = { 0 };//终端软件版本号
	int setFlag = 0;
	int bufLen  = 0;
	int iRet	= 0;
	int rcvLen	= 0;
	int upfreq  = 0;
	int com		= 0;
	int comType	= 0;
	int iSelect = 0;
	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(YCT_MONITOR_IP , YCT_MONITOR_PORT , &sd); //第三代羊城通监控服务器
	if (iRet != 0 )
	{
		return 1;
	}
	unsigned char rcvData[256] = { 0 };

	//初始化com ， comtype等信息，且进行签到，获取YCT_DATA结构
//  	if (init_sign(pgbl , com , comType , yct_data) != 0)
//  	{
//  		return 1;
//  	}

	char cur_signTime[30+1];
	CDB::getYct3SignInfo(pgbl->pos_no,pyct_data,cur_signTime);

    com = pgbl->com;
	comType = pgbl->comType;

	//准备P_MONITOR包
	//1、R_PUB_GETVERSION
	unsigned char outBuf[1024]  = { 0 };
	unsigned char ReaderVer[60] = { 0 };
	unsigned char MonitorStat	= 0;
	unsigned char lastBizStat	= 0;
	unsigned char lastBizStan[8]= { 0 };
	int outLen					= 0;
	iRet = R_PUB_GETVERSION(pgbl->dPosInfo , com , comType , outBuf , &outLen);
	if (iRet != 0)
	{
		return 1;
	}
	memcpy(ReaderVer , outBuf + 4 , 60);
	//2、R_PUB_GET_PKISTATE
	iRet = R_PUB_GET_PKISTATE(pgbl->dPosInfo , com , comType , outBuf , &outLen);
	if (iRet != 0)
	{
		return 1;
	}
	MonitorStat = outBuf[4];
	//3、数据库获取最后一次的记录
	CDB::get_Last_Biz(lastBizStat , lastBizStan);
	
	//准备P_MONITOR包
	//包头
	bufLen = 0;
	CTools::hex_to_byte((BYTE*)"FD01" , byteTmp , 4);
	memcpy(monitor + bufLen , byteTmp , 2);
	bufLen += 2;
	CTools::hex_to_byte( (BYTE*)"0002" , byteTmp , 4);
	memcpy(monitor + bufLen , byteTmp , 2);
	bufLen += 2;
	monitor[4] = 0x00;
	monitor[5] = 0x80;
	sprintf( (char*)byteTmp , "%04X" , sizeof(monitor) - 8);
	CTools::hex_to_byte(byteTmp , monitor + bufLen , 4);
	bufLen += 4;
	//包体
	CTools::hex_to_byte((BYTE*)pyct_data->SHID , byteTmp , 16);
	memcpy(monitor + bufLen , byteTmp , 8);
	bufLen += 8;
	bufLen += 1;//管理卡版本号
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , byteTmp , 8);
	wrHex("PKI管理卡号：" , byteTmp , 4);
	memcpy(monitor + bufLen , byteTmp, 4);
	bufLen += 4;
	time_t t = time(0);
	strftime((char*)byteTmp , sizeof(byteTmp) , "%Y%m%d%H%M%%S" , localtime(&t));
	CTools::hex_to_byte(byteTmp , monitor + bufLen , 14);
	bufLen += 7; //时间戳
	monitor[bufLen++] = lastBizStat ; 
	memcpy(monitor + bufLen , lastBizStan , 8);
	bufLen += 8;
	memcpy(monitor + bufLen , ReaderVer , 60);
	bufLen += 60;
	memcpy(monitor + bufLen , termVer , 16);
	bufLen += 16;
	monitor[bufLen++] = MonitorStat ;
	//报文准备完毕
	wrHex("定时监控报文：" , (BYTE*)monitor , bufLen);

	memset(rcvData , 0 , sizeof(rcvData));
	iRet = ST_commuWithYCTIC3(pgbl->dPosInfo, pyct_data ,  com , comType,sd , monitor , bufLen ,  rcvData , &rcvLen);
	if (iRet != 0)
	{
		wrLog("监控报文发送失败!");
	}
	else
	{
		wrLog("监控报文发送成功!");
	}
	return 1;
	} 

*/

int CYCTYPTrade::P_EXTEND_QRY(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, unsigned char* outBuf , int* len)
{
	unsigned char inBuf[128] = { 0 };
	int bufLen = 0 , iRet = 0;
	unsigned char byteTmp[64] = { 0 };

	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , byteTmp , 8);
	memcpy(inBuf , byteTmp , 4);
	bufLen += 4;

	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , byteTmp , 16);
	memcpy(inBuf + 4 , byteTmp , 8);
	bufLen += 8;

	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , byteTmp , 16);
	memcpy(inBuf + 12 , byteTmp , 8);
	bufLen += 8;
	bufLen += 20;
	
	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"E003" , (BYTE*)"0001" , 0x02 , 0x80 , inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return 1;
	}
	if(ErrReturn(dPosInfo , outBuf + 16) != 0)
	{
		return 1;
	}
	return 0;
}

int CYCTYPTrade::ExtendApp(CGlobal *pgbl , int execFlag)
{
	unsigned char SAK[1+2] = { 0 };
	unsigned char card_pycicalInfo1[32+2] = { 0 };
	int iRet	= -1;
	int com		=  0;
	int comType	=  0;
	unsigned char card_physics_info[51] = { 0 };
	unsigned char SW1SW2[4+2] = { 0 };
	YCT_DATA yct_data ;
	PYCT_DATA pyct_data = &yct_data;
	char inBuf[1024]  = { 0 };
	char outBuf[1024] = { 0 };
	int  outLen		  = 0;
	int iSelect  = 0;

	//初始化串口号，串口类型,签到获取YCT_DATA结构
// 	if (init_sign(pgbl , com , comType , yct_data) != 0)
// 	{
// 		return 1;
// 	}

	
	char cur_signTime[30+1];
	CDB::getYct3SignInfo(pgbl->dPosInfo.pos_no,&yct_data,cur_signTime);
    com = pgbl->com;
	comType = pgbl->comType;

	do 
	{
		iRet = R_PUB_QRY_PHYSICS_INFO2(pgbl->dPosInfo , com , comType , (BYTE*)outBuf , &outLen);
		if (iRet == 0)
		{
			//设置卡片基本信息
			CTools::byte_to_hex((BYTE*)outBuf + 4 , (BYTE*)pyct_data->ticket_physicalNo , 8);
			CTools::byte_to_hex((BYTE*)outBuf + 12, (BYTE*)pyct_data->ticket_no , 8);
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo , "读卡信息错误\n按【Enter】重试读卡，按【ESC】取消并返回。" , 0 , iSelect );
			if (iSelect != 0)
			{
				wrLog("iSelect : %d" , iSelect);
				return iRet ;
			}
		}
	} while ( iRet != 0);

	//其下的0x03代表执行扩展应用操作
	iRet = P_TRAN_QRY3(pgbl->dPosInfo ,  pyct_data , outBuf[20] , (BYTE*)outBuf + 21 , 0x03 , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return 1; 
	}

	iRet = R_PUB_SET_READCARDINFO2(pgbl->dPosInfo , com , comType , (BYTE*)pyct_data->ticket_no , \
		(BYTE*) outBuf + 21, (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return 1;
	}

	iRet = P_EXTEND_QRY(pgbl->dPosInfo , pyct_data , (BYTE*)outBuf , &outLen);
	if (iRet != 0)
	{
		return 1;
	}

	//下面根据报文返回提示菜单以供用户选择卡扩展应用
	int bizCount = outBuf[20];
	char bizName[5] = { 0 };
	char bizDesc[31]= { 0 };
	TSMENULIST menuList;

	memset(&menuList,0,sizeof(menuList));
	menuList.iTotalNum = bizCount;
	strcpy(menuList.acTitle, "请选择扩展业务类型\n");

	for (int i=0; i!= bizCount ; ++i)
	{
		//小学加法算术式用以计算业务类型描述偏移
		//bizDesc[0] = 0x31 + i ;
// 		memcpy(bizDesc  , outBuf + 21 + 4 + 34* i , 30);
// 		bizDesc[30] = 0 ;
		strncpy(bizDesc  , outBuf + 21 + 4 + 34* i,30 );
		CTools::trim(bizDesc);
		strcpy(menuList.menuItem[i] , bizDesc);	
	}

	iRet = TS_Menu(pgbl->dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
	if(iRet==HOTKEY)
	{
		return CANCLE_KEY;
	}
	else if(iRet!=0)
	{
		return 1;
	}
	
	memcpy(bizDesc , outBuf + 21 + 34*(iSelect - 1) , 4);	//业务类型
	wrHex("业务类型：" , (BYTE*)bizDesc , 4);
	
	bool wBreak = false;
	int infLen  = 0;
	unsigned char code_length = 0x00;
	unsigned char cur_code    = 0x00;
	unsigned char code[512]   = { 0 };
	unsigned char bef_flow[8] = { 0 };
	unsigned char flow[8]     = { 0 };
	unsigned char byteTmp[256]= { 0 };
	
	do
	{
		int len = 0;
		iRet = R_EXTEND(pgbl->dPosInfo , com , comType , code_length , code , (BYTE*)outBuf , &outLen);
		if (iRet != 0)
		{
			return 1;
		}

		//激活卡信息长度-扩展信息长度
		CTools::byte_to_hex((BYTE*)outBuf + 4 , byteTmp , 2);
		sscanf((char*)byteTmp , "%x" , &len);

		iRet = P_EXTEND(pgbl->dPosInfo , pyct_data , flow , (BYTE*)bizDesc , cur_code, bef_flow , \
			len , (BYTE*)outBuf + 6 , (BYTE*)outBuf , &outLen);
		if (iRet != 0)
		{
			return 1;
		}

		CTools::byte_to_hex((BYTE*)outBuf + 29 , byteTmp , 2);
		sscanf((char*)byteTmp , "%x" , &code_length);
		memcpy(code , outBuf + 31 , code_length);	//将操作指令长度和操作指令取出
		memcpy(flow , outBuf + 16 , 8);				//系统流水号取出

		if (outBuf[28] == 0x01)
		{
			//将数据发送给读卡器后退出
			iRet = R_EXTEND(pgbl->dPosInfo , com , comType , code_length , code , (BYTE*)outBuf , &outLen);
			if (iRet != 0)
			{
				return 1;
			}
			wBreak = false;
		}
		else if (outBuf[28] == 0x02)
		{
			//无后续指令
			wBreak = false;
		}
		else if (outBuf[28] == 0x03)
		{
			//按备注提示用户是否继续执行指令
			char infMessage[128] = { 0 };
			strcpy(infMessage , "服务器需要继续执行以下内容，按【Enter】继续，【ESC】取消。\n" );
			memcpy(infMessage + strlen(infMessage) , outBuf + 31 + code_length , 64);
			TS_OutPutMsg(pgbl->dPosInfo , infMessage , 0 , iSelect);
			if (iSelect != 0)
			{
				return 1;
			}
		}
		else if (outBuf[28] == 0x04)
		{
			//直接执行后续指令
			cur_code = 0x01 ;
			wBreak = true;
		}
		else if (outBuf[28] == 0x05)
		{
			//做充值 //TODO
			wBreak = false;
		}
		else if (outBuf[28] == 0x06)
		{
			//做消费 //TODO
			wBreak = false;
		}
		else
		{
			wBreak = false;
			break;
		}
	}while(wBreak);

	return 0;
}

int CYCTYPTrade::R_M1_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com , const int comType, char* bizInfo , unsigned char* outBuf , int* len)
{
	char inBuf[64] = { 0 };
	unsigned char byteTmp[64] = { 0 };
	memcpy(inBuf , bizInfo , 16);//交易数据
	time_t t = time(0);
	strftime((char*)byteTmp , sizeof(byteTmp) , "%Y%m%d%H%M%%S" , localtime(&t));
	CTools::hex_to_byte(byteTmp , (BYTE*)inBuf + 16 , 14);
	return R_COMMAND(dPosInfo , com , comType , 0xBB , inBuf , 16 + 7 ,(char*) outBuf , len);
}

int CYCTYPTrade::R_M1_CANCLE(const TSDLLPosInfo& dPosInfo , int com , int comType , char* bizInfo , unsigned char* outBuf , int* len)
{
	char inBuf[64] = { 0 };
	unsigned char byteTmp[64] = { 0 };
	memcpy(inBuf , bizInfo , 16);//交易数据
	time_t t = time(0);
	strftime((char*)byteTmp , sizeof(byteTmp) , "%Y%m%d%H%M%%S" , localtime(&t));
	CTools::hex_to_byte(byteTmp , (BYTE*)inBuf + 16 , 14);
	return R_COMMAND(dPosInfo , com , comType , 0xBC , inBuf , 16 + 7 , (char*)outBuf , len);
}

int CYCTYPTrade::P_EXTEND(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data,  \
						  unsigned char* flow , unsigned char* bizType , int firstType , unsigned char* firstFlow , \
						  int infLen , unsigned char* cardInfo ,  unsigned char* outBuf , int* outLen)
{
	unsigned char inBuf[512] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = -1;
	
	//系统流水号
	//CTools::hex_to_byte(flow , bytetmp , 16);
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//PKI
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//业务类型
	memcpy(inBuf + bufLen , bizType , 4);
	bufLen += 4;
	//前次操作类型
	inBuf[bufLen++] = firstType;
	//前次操作流水号
	memcpy(inBuf + bufLen , firstFlow , 8);
	bufLen += 8;
	//扩展信息长度
	sprintf((char*)bytetmp , "%04X" , infLen);
	CTools::hex_to_byte(bytetmp , inBuf + bufLen , 4);
	bufLen += 2;
	//扩展信息
	memcpy(inBuf + bufLen , cardInfo , infLen);
	bufLen += infLen;
	//RFU 保证除握手流水号外是16的倍数
	bufLen += 16 - (bufLen + 8) % 16 ;
	
	iRet = P_COMMAND(dposinfo , pyct_data , (BYTE*)"E001" , (BYTE*)"0001" , 0x02 ,0x80 , inBuf , bufLen , outBuf , outLen);
	if (iRet != 0)
	{
		return 1;
	}
	if (ErrReturn(dposinfo , outBuf + 24) != 0)
	{
		return 1;
	}
	return 0;
}


int  CYCTYPTrade::P_M1_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , double amount , unsigned char* pz , unsigned char* bizInfo, unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	BYTE hexamo[4+1];
	int bufLen = 0 , iRet = 0;

	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf+ bufLen , bytetmp , 4);
	bufLen += 4;
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	long amo = amount;//以分为单位
	sprintf((char*)bytetmp , "%012d" , amo);
	bbzero(hexamo);
	getHexAmo((char*)bytetmp , hexamo);
	memcpy(inBuf + bufLen ,hexamo,4);
	bufLen += 4;
	CTools::hex_to_byte((BYTE *)flow,inBuf + bufLen,16);
	//memcpy(inBuf + bufLen , flow ,8);
	bufLen += 8;
	CTools::hex_to_byte((BYTE *)pz,inBuf + bufLen,8);
	//memcpy(inBuf + bufLen , pz , 4);
	bufLen += 4;
	memcpy(inBuf + bufLen , bizInfo , 88);
	bufLen += 88;
	bufLen += 36;//RFU

	iRet = P_COMMAND(dposinfo , pyct_data , (BYTE*)"AA71" , (BYTE*)"0001" , 0x02 , 0x80 , inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return iRet;
	}
	if (ErrReturn(dposinfo , outBuf + 24) != 0)
	{
		return 1;
	}

	return 0;
}

/*
flow:系统流水号，对应的是撤销阶段2的系统流水号
cancleInfo:撤销信息。该信息是读卡器指令R_M1_CANCLE_INIT返回结果
bizInfo：交易信息，重新调用读卡器指令R_M1_GET_CARDINFO返回结果，信息错误撤销无效
readerCode：读卡器返回数据。读卡器指令R_M1_CANCLE返回的status结果
outBuf:返回报文体长度
*/
int CYCTYPTrade::P_M1_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, const char* flow , unsigned char* cancleInfo , unsigned char* bizInfo , unsigned char readerCode, unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = 0;

	//PKI管理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf+ bufLen , bytetmp , 4);
	bufLen += 4;
	//系统流水号
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//物理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//逻辑卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//撤销信息
	memcpy(inBuf + bufLen , cancleInfo , 64);
	bufLen += 64;
	//交易信息
	memcpy(inBuf + bufLen , bizInfo , 88);
	bufLen += 88;
	//读卡器返回码
	inBuf[bufLen++] = readerCode;
	bufLen += 19;//RFU
	
	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA73" , (BYTE*)"0001" , 0x02 , 0x80 , inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return iRet;
	}
	if (ErrReturn(dPosInfo , outBuf + 24) != 0)
	{
		return 1;
	}

	return 0;
}

int CYCTYPTrade::P_CPU_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, unsigned char* flow, \
									 unsigned char* TAC , unsigned char* MAC2 , unsigned char* cardCode , \
									 unsigned char readerCode, unsigned char* bef , unsigned char* up_bef, \
									 unsigned char down_bef , unsigned char* countInfo, unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	BYTE rcvData[512] = { 0 };
	BYTE temp[14+1] = { 0 };
	int bufLen = 0 , iRet = 0;
	BYTE RFU[25+1];

	bbzero(RFU);

	wrLog("【P_CPU_CANCLE_SUBMIT】");
	
	//PKI管理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf+ bufLen , bytetmp , 4);
	bufLen += 4;
	//系统流水号
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//物理卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//逻辑卡号
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//TAC
	memcpy(inBuf + bufLen , TAC , 4);
	bufLen += 4;
	//MAC2
	memcpy(inBuf + bufLen , MAC2 , 4);
	bufLen += 4;
	//卡片返回码
	memcpy(inBuf + bufLen , cardCode , 2);
	bufLen += 2;
	//读卡器返回码
	inBuf[bufLen] = readerCode ; 
//	memcpy(inBuf + bufLen,(BYTE *)readerCode,1);
	bufLen += 1;
	//钱包余额
	memcpy(inBuf + bufLen , bef , 4);
	bufLen += 4;
	//金额下限
	inBuf[bufLen] = down_bef ;
	//memcpy(inBuf + bufLen,(BYTE *)down_bef,1);
	bufLen += 1;
	//金额上限
	memcpy(inBuf + bufLen , up_bef , 3);
	bufLen += 3;
	//计数信息
	memcpy(inBuf + bufLen , countInfo , 16);
	bufLen += 16;
	//RFU
	memcpy(inBuf + bufLen , RFU , 25);
	bufLen += 25;
	//保存需要提交的数据
	
	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA95" , (BYTE*)"0001" , 0x02 , 0x80 , inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return iRet;
	}

	if (ErrReturn(dPosInfo , outBuf + 24) != 0)
	{
		return 1;
	}

	return 0;
}

int CYCTYPTrade::R_EXTEND(const TSDLLPosInfo dPosInfo , int com , int comtype , int codeLength , \
						  unsigned char* code , unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0;

	sprintf((char*)bytetmp , "%04X" , codeLength);
	CTools::hex_to_byte(bytetmp , inBuf , 4);
	bufLen += 2;
	memcpy(inBuf + bufLen , code , codeLength);
	bufLen += codeLength;

	return R_COMMAND(dPosInfo , com , comtype , 0xCA , (char*)inBuf , bufLen , (char*)outBuf , len);
}

//初始化读卡器，获取全局的 串口号、串口类型等信息，签到获取yct_data结构，引用传值！
int CYCTYPTrade::init_sign(CGlobal *pgbl , int& com , int& comType , YCT_DATA& yct_data)
{
	int iRet = 0, iSign = 0, iSelect = 0, firstSign = 0;
	//char sysTime[30];
	long lInterval = 0;
	char cur_signInTime[30+1];
	char fmtDate[10+1];
	char fmtTime[10+1];

	bbzero(cur_signInTime);

    //羊城通数据结构初始化
	init_yct_data(&yct_data);

	iRet = CTrade::getComPar(pgbl->posInfo.leftType, pgbl->posInfo.rightType, pgbl->posInfo.inType, &com, &comType);
	if( iRet != 0 )
	{
		return 1;
	}

// 	if((pgbl->posInfo).rightType == 0x08)	//索天读卡器
// 	{
// 		comType = 0x05;
// 		iRet = ST_initReader3(pgbl->dPosInfo,com,comType,&yct_data); //索天读卡器第三代羊城通充值读卡器初始化
// 		if(iRet)
// 		{
// 			return 1;
// 		}
// 	}
// 	else //暂时屏蔽明华读卡器交易
// 	{
// 		TS_OutPutMsg(pgbl->dPosInfo, "无效硬件，无法继续交易，请重新设置，如有疑问请联系交费易客服", 0, iSelect);
// 		return 1;
// 	}

	//获取终端签到时间
	iRet = CDB::getYct3SignInfo(pgbl->dPosInfo.pos_no,&yct_data,cur_signInTime);
	if (iRet == -3)
	{
		firstSign = 1; //终端第一次签到
		wrLog("羊城通充值终端第一次签到");
	}

	//若不是第一次签到则比较当前签到时间与系统时间
// 	if(firstSign != 1)
// 	{
// 		CTools::getCurDatetime(sysTime);
// 		
// 		lInterval = CDB::getTimeInterval(cur_signInTime,sysTime);
// 		
// 		if( lInterval <= SKEY_VALIDATE )
// 		{
// 			wrLog("羊城通充值终端一天内无需再次签到");
// 			return 0;
// 		}
// 		wrLog("羊城通充值终端需要重新签到");
// 	}


	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;

	if((pgbl->posInfo).rightType == 0x08)	//索天读卡器
	{
		comType = 0x05;
		if(YCT_VER==3)
		{
			iRet = ST_initReader3(pgbl->dPosInfo,com,comType,&yct_data); //索天读卡器第三代羊城通充值读卡器初始化
		}
		
		else 
		{
			iRet = ST_initReader(pgbl->dPosInfo, com, comType); //索天读卡器第二代羊城通充值读卡器初始化
		}
		if(iRet)
		{
	 		return 1;
	 	}
	}
	else //暂时屏蔽明华读卡器交易
	{
		TS_OutPutMsg(pgbl->dPosInfo, "无效硬件，无法继续交易，请重新设置，如有疑问请联系交费易客服", 0, iSelect);
		return 1;
	}

	iRet = yct_SignIn(pgbl->dPosInfo,com,comType,&yct_data,&m_party,&m_rspcode);
	if (iRet == 0)
	{ 
		wrLog("签到成功");
	}
	else
	{		
		wrLog("签到失败");
		TS_OutPutMsg(pgbl->dPosInfo, "羊城通终端签到失败，请重新打开iPos程序重新进行签到", 0, iSelect);
		return 1;
	}
	
	bbzero(cur_signInTime);
	bbzero(fmtDate);
	bbzero(fmtTime);
	CTools::formatDate(yct_data.SignIn_ServerTime,fmtDate);
	CTools::formatTime(yct_data.SignIn_ServerTime+8,fmtTime);
	strcpy(cur_signInTime,fmtDate);
	strcat(cur_signInTime," ");
	strcat(cur_signInTime,fmtTime);
	if (firstSign == 1)
	{
		//insert
		iSign = CDB::insertYct3SignInfo(pgbl->dPosInfo.pos_no,&yct_data,cur_signInTime);
	}
	else
	{
		//update
		iSign = CDB::updateYct3SignInfo(pgbl->dPosInfo.pos_no,&yct_data,cur_signInTime);
	}
	return 0;
}


int CYCTYPTrade::R_PUB_SET_READCARDINFO2(const TSDLLPosInfo& dPosInfo , int com , int comType , unsigned char* logic_No, unsigned char* qryInfo , unsigned char* outBuf , int* len )
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0;
	wrLog("【R_PUB_SET_READCARDINFO】");
	//逻辑卡号
	CTools::hex_to_byte(logic_No , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//查询信息
	memcpy(inBuf + bufLen , qryInfo , 48);
	bufLen += 48;

	return R_COMMAND(dPosInfo , com , comType , 0x95 , (char*)inBuf , bufLen , (char*)outBuf , len);
}

//原函数用起来不习惯，封装结构不够好，重写之
int CYCTYPTrade::R_CPU_GET_CARDINFO(const TSDLLPosInfo& dPosInfo , int com , int comType , unsigned char* outBuf , int*len)
{
	wrLog("【R_CPU_GET_CARDINFO】");
	return R_COMMAND(dPosInfo , com , comType , 0x79 , NULL , 0 , (char*)outBuf , len);
}

//用于返回码判断，offset 为返回码所在位置的偏移
int CYCTYPTrade::ErrReturn(const TSDLLPosInfo& dPosInfo , const unsigned char* offset)
{
	char zero[4] = { 0 };
	int iSelect = 0;
	if (memcmp(offset , zero , 4) == 0)
	{
		return 0;
	}
//	TS_OutPutMsg(dPosInfo , (char*)offset + 4 , 0 , iSelect );
	TS_OutPutMsg(dPosInfo , "输入参数出错或者没把卡放到读卡器上" , 0 , iSelect );

	return 1;
}

int CYCTYPTrade::R_PUB_QRY_PHYSICS_INFO2(const TSDLLPosInfo& dPosInfo , int com ,int comType , unsigned char* outBuf, int*len)
{
	wrLog("【R_PUB_QRY_PHYSICS_INFO】");
	return R_COMMAND(dPosInfo , com , comType , 0x93 , NULL , 0 , (char*)outBuf , len);
}


// 福州索天科技 羊城通充值第三代版本
// 不完整交易处理
// 入参：yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
// 出参：ticket_amo_aft(12):票卡充值后余额; errCode:出错代码
// 返回值
// 0：成功
// 1：与读卡器交互失败，结果未知
// 2：读卡器返回的错误，结果未知
// 3：与羊城通交互失败，结果未知
// 4：PKI卡被更换，结果未知
// 5：失败 
int CYCTYPTrade::ST_YctThird_NotFinishHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,\
										  char *yct_serial,char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode)
{
	int iRet = 0;
	int len = 0, msg_len = 0;
	int bufLen = 0; 
	int body_len = 0;
	char strbodylen[4];
	char templen[20];
	BYTE inBuf[512];
	BYTE outBuf[512];
    SOCKET sd;
	CConnect con;
	char type[4+1];//信息类型码
	char ver[4+1];//报文版本号
	BYTE encAlgo[2];//报文体加密类型算法
	BYTE endian[2];//数值内容字节顺序

	bbzero(type);
	bbzero(ver);
	bbzero(encAlgo);
	bbzero(endian);
	bbzero(inBuf);
	bbzero(outBuf);
	bbzero(strbodylen);
	bbzero(templen);


		if (yct_SignIn(dPosInfo,com,comType,pyct_data,party,rspcode)==0)
		{
			wrLog("签到成功");
		}
		
		if(iRet) //签到失败
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 5;
		}
		
		memcpy(type,result_message + msg_len,4);
		msg_len+=4;
		memcpy(ver,result_message + msg_len,4);
		msg_len+=4;
		CTools::hex_to_byte((BYTE *)result_message + msg_len,(BYTE *)encAlgo,2);
		msg_len+=2;
		CTools::hex_to_byte((BYTE *)result_message + msg_len,(BYTE *)endian,2);
		msg_len+=2;   
		
		wrHex("报文体加密算法: ",encAlgo,1);
		wrHex("数值内容字节顺序: ",endian,1);
		
		memcpy(strbodylen,result_message + msg_len,4);
		wrLog("strbodylen: %s",strbodylen);
		CTools::HexToDec(strbodylen,templen);
		body_len = atoi(templen);
		msg_len+=4;
		
		CTools::hex_to_byte((BYTE *)result_message + msg_len + 16 + 8,(BYTE *)inBuf,body_len-(16+8+8)/2);
		msg_len+=body_len * 2 -16-8-8;
		
		wrHex("inBuf: ",inBuf,body_len-(16+8+8)/2);
		
		//羊城通建立连接
		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3：与羊城通交互失败
		}
		
		iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)type , (BYTE*)ver , encAlgo[0] , endian[0] , inBuf , body_len-(16+8+8)/2 , outBuf , &len);
		
		if (iRet != 0)
		{
			pyct_data->yct_ack = 0;
			return iRet;
		}
		if (ErrReturn(dPosInfo , outBuf + 24) != 0)
		{
			pyct_data->yct_ack = 0;
			return 1;
		}

		pyct_data->yct_ack = 1; //交易状态为1。确认状态
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
		closesocket(sd);

	return 0;
}


//监控配置
/*
int CYCTYPTrade::P_SET_MONITOR(CGlobal *pgbl,const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data)
{
	unsigned char monitor[114]    = { 0 };
	unsigned char setmonitor1[87] = { 0 };
	unsigned char setmonitor2[256]= { 0 };
	unsigned char byteTmp[256]    = { 0 };
	char termVer[16+2] = { 0 };//终端软件版本号
	int setFlag = 0;
	int bufLen  = 0;
	int iRet	= 0;
	int rcvLen	= 0;
	int upfreq  = 0;
	int iSelect = 0;
	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(YCT_MONITOR_IP , YCT_MONITOR_PORT , &sd); //第三代羊城通监控服务器
	if (iRet != 0 )
	{
		return 1;
	}

	//SET_MONITOR阶段1，包头
	CTools::hex_to_byte( (BYTE*)"FD03" , byteTmp , 4);
	memcpy(setmonitor1 + bufLen , byteTmp , 2);
	bufLen += 2;
	CTools::hex_to_byte( (BYTE*)"0002",  byteTmp, 4);
	memcpy(setmonitor1 + bufLen , byteTmp , 2);
	bufLen += 2;
	setmonitor1[4] = 0x00;
	setmonitor1[5] = 0x80;
	bufLen += 2;
	sprintf((char*)byteTmp , "%04X" , 79);
	CTools::hex_to_byte(byteTmp , setmonitor1 + bufLen  , 4 );
	bufLen += 2;
	//包体
	CTools::hex_to_byte((BYTE*)pyct_data->SHID , byteTmp , 16);
	memcpy(setmonitor1 + bufLen , byteTmp , 8);
	bufLen += 8;
	bufLen += 1;//管理卡版本号取0
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , byteTmp , 8);
	memcpy(setmonitor1 + bufLen , byteTmp , 4);
	bufLen += 4;
	bufLen += 2;//终端类型取0000
	bufLen += 64;//RFU保留
	
	wrHex("监控设置发送报文：" , setmonitor1 , bufLen);

	unsigned char rcvData[256] = { 0 };
	iRet = ST_commuWithYCTIC3(pgbl->dPosInfo, pyct_data , com , comType ,sd , setmonitor1 , bufLen ,  setmonitor2 , &rcvLen);
	if (iRet != 0)
	{
		return 1;
	}
	char zeroQ[4] = { 0 };
	if (memcmp(setmonitor2 + 20 , zeroQ , 4) != 0)
	{
		return 1;	//返回码错误
	}
	wrHex("监控设置返回报文：" , setmonitor2 , rcvLen);
	bbzero(byteTmp);
	CTools::byte_to_hex(setmonitor2 + 24 , byteTmp , 2);
	strcpy(pgbl->monitor_frequency,(char *)byteTmp);
	upfreq = atol((char*)byteTmp);
	bbzero(byteTmp);
	CTools::byte_to_hex(setmonitor2 + 24+2 , byteTmp , 7);
	strcpy(pgbl->next_monitortime,(char *)byteTmp);
	bbzero(byteTmp);
	CTools::byte_to_hex(setmonitor2 + 24+2+7, byteTmp , 64);
	strcpy(pgbl->announce,(char *)byteTmp);
 	memcpy(termVer , setmonitor2 + 97 , 16);
 	strcpy(pgbl->term_ver,(char *)termVer);
	 
	wrYct3MonitorLog((char *)dPosInfo.pos_no,"终端监控设置返回信息");
 	wrYct3MonitorLog((char *)dPosInfo.pos_no,"监控频率：%s",pgbl->monitor_frequency);
 	wrYct3MonitorLog((char *)dPosInfo.pos_no,"下次更新时间：%s",pgbl->next_monitortime);
	char strtemp[1024];
 	bbzero(strtemp);
	CTools::tobyte(pgbl->announce,strtemp);
 	wrYct3MonitorLog((char *)dPosInfo.pos_no,"公告信息：%s",strtemp);
	wrYct3MonitorLog((char *)dPosInfo.pos_no,"终端软件版本号：%s",pgbl->term_ver);

	return 0;
}
*/

/**************************第三代 羊城通scc充值新增函数 结束****************************************/

//初始化羊城通结构体
int CYCTYPTrade::init_yct_data(PYCT_DATA pyct_data)
{
	bbzero(pyct_data->stan);
	bbzero(pyct_data->txn_date);
	bbzero(pyct_data->txn_time);
	bbzero(pyct_data->biz_type);
	pyct_data->status = 0;
	bbzero(pyct_data->term_id);
	bbzero(pyct_data->term_serial);
	bbzero(pyct_data->bank);
	bbzero(pyct_data->card_no);
	bbzero(pyct_data->amount);
	bbzero(pyct_data->yct_serial);
	bbzero(pyct_data->pki_card);
	bbzero(pyct_data->ticket_no);
	bbzero(pyct_data->ticket_amo_bef);
	bbzero(pyct_data->ticket_amo_aft);
	bbzero(pyct_data->result_message);
	pyct_data->yct_ack=1; //默认值为0
	/**************************第三代羊城通新增变量*********************/

    bbzero(pyct_data->ticket_physicalNo);
	bbzero(pyct_data->SKEY);
	bbzero(pyct_data->SHID);
	pyct_data->yct_sequence_NO = 0;
	bbzero(pyct_data->downLimit);
	bbzero(pyct_data->upperLimit);
	bbzero(pyct_data->yct_stan);
	bbzero(pyct_data->org_yctstan);
//	bbzero(pyct_data->card_status);
	bbzero(pyct_data->SW1SW2);
	bbzero(pyct_data->BLACKLIST_CARDINFO);
	bbzero(pyct_data->trader_id);
	bbzero(pyct_data->trader_name);
	bbzero(pyct_data->out_logicalcard);
	bbzero(pyct_data->note);
	bbzero(pyct_data->tradeTime);
	bbzero(pyct_data->TAC);
	bbzero(pyct_data->yct_termid);
	pyct_data->trade_count = 0;
	bbzero(pyct_data->trade_type);
	bbzero(pyct_data->charge_authcode);
	bbzero(pyct_data->result_message_third);
	pyct_data->com = 0;
	pyct_data->comType = 0;
	bbzero(pyct_data->SignIn_ServerTime);
	bbzero(pyct_data->SAK);
	bbzero(pyct_data->random_num);

	/*****************羊城通卡间余额转移*******************/
	bbzero(pyct_data->extension_stan);
	bbzero(pyct_data->transfer_stan);
	bbzero(pyct_data->out_logicardno);
	bbzero(pyct_data->out_balance);
	bbzero(pyct_data->out_balance_bef);
	bbzero(pyct_data->out_balance_aft);
	bbzero(pyct_data->out_upperLimit);
	bbzero(pyct_data->out_downLimit);
	bbzero(pyct_data->out_cardappinfo);

	bbzero(pyct_data->in_logicardno);
	bbzero(pyct_data->in_balance);
	bbzero(pyct_data->in_balance_bef);
	bbzero(pyct_data->in_balance_aft);
	bbzero(pyct_data->in_upperLimit);
	bbzero(pyct_data->in_downLimit);
	bbzero(pyct_data->in_cardappinfo);
	return 0;
}