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

 static int moni=0;//ģ��ɹ�
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
 static int M1_CPU=1;//�ж���M1����CPU��
 static int atuo_charge;//�Զ���ֵ��־
 static char mny[20];
 static int change;
 static char auto_show[1024];//�Զ���ֵ��ʾ
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

// ����Ƿ���δ֪����
// ��Σ�term_id:�ն˺ţ�pki_card����ǩ����PKI�����ţ�signin_serial��������ˮ��
// ���Σ�pyct_data:���ͨ������Ϣ��pki_card����ǩ����PKI�����ţ�signin_serial��������ˮ��
// ����ֵ��
// 0:���ͨ��
// 1:����"δ֪�������"����
// 2:ǩ����pki��������ԭ���ײ�һ��
// 3:��ȡ������Ϣʧ��
int CYCTYPTrade::NotFinishChk(CGlobal gbl, char *term_id, char *pki_card, char *signin_serial)
{
	int iRet=0, com=0, comType=0, iSelect=0;
	YCT_DATA yct_data;
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	memset(&yct_data, 0, sizeof(yct_data));
	
	iRet = CDB::getYctNotFinish(term_id, &yct_data);
	if( iRet==0 )//�������û��ȷ�ϵĹ�������
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
			return 3;// 3����ȡ������Ϣʧ��
		}
		
		// 		if( memcmp(pki_card, yct_data.pki_card, 8)==0 )//������ǩ����pki��������ԭ����һ��
		// 		{
		iRet = TS_OutPutMsg(gbl.dPosInfo, "��������ȷ����һ�����ͨ����\n�����Ӻö�����", 0, iSelect);
		if(memcmp(yct_data.biz_type, "0056", 4)==0)//��ͨǮ��
		{
			//�ж϶���������
			if(gbl.posInfo.rightType == 0x08)
			{
				comType = 0x05;
				if (YCT_VER==3)
				{

					iRet = ST_initReader3(gbl.dPosInfo, com, comType,&yct_data); //���������ͨ��ֵ��������ʼ������
			
				}
				else 
				{
				
					iRet = ST_initReader(gbl.dPosInfo, com, comType); //��ȡ�ڶ������ͨ��������ʼ������
				}
			
				
				if( iRet==0 )//�ɹ�
				{

					if(strlen(yct_data.result_message) < 3)
					{
						// 							memset(yct_data.result_message, 0, 33);
						// 							memcpy(yct_data.result_message, yct_data.pki_card, 8);
						// 							memcpy(yct_data.result_message+8, yct_data.yct_serial+2, 8);
						// 							memcpy(yct_data.result_message+16, "FF00000000000000", 16);//��ʱ�ò�������Ϣ
						m_party = FROMCTSI_N;
						m_rspcode = NORESULT;
						if (YCT_VER==3)
						{
							yct_data.yct_ack=1;
						}

						yct_data.status = 7;//�޷���֪��ֵ���
						CDB::updateYctYPData(yct_data);
						showErrMsg(gbl, m_party, m_rspcode);
						//	return 1;
					}
					else if (YCT_VER==3 && strlen(yct_data.result_message) > 64 )
					{
						//���������ͨ���������״���
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
					return 1;// 1�������������ʧ�ܣ����δ֪
				}
			}
			else
			{
				iRet = NormalNotFinish(gbl.dPosInfo, com, comType, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);
			}
		}
		else//��ƱǮ��
		{
			iRet = MonthNotFinish(gbl.dPosInfo, com, comType, signin_serial, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);
			
		}
		
	/*
		if(iRet==0)//�ɹ�
		{

			//m_party = FROMSUCC;
			//m_rspcode = SUCC_MSG_LAST;
			//yct_data.status = 1;//��ֵ�ɹ�δȷ�� edit by liuxiao
			//���½����ܶ�
		//	CDB::setSumDayAmo(atoi(gbl.amount), gbl.pos_no);
		//	showErrMsg(gbl, m_party, m_rspcode);
			yct_data.yct_ack=1; //���ͨ��ֵȷ��
		}
		else if(iRet==5)//ʧ��
		{
			/* edit by liuxiao
			m_party = FROMCTSI;
			m_rspcode = FAIL_LAST;
			yct_data.status = 4;//��ֵʧ��δ����
			showErrMsg(gbl, m_party, m_rspcode);
			*/
	//		yct_data.yct_ack=0; //���ͨ��ֵδȷ��
	//	}
		
		/*
		else//δ֪
		{
			//m_party,m_rspcode����
			/* edit by liuxiao 
			yct_data.status = 0;//�ѿ۷ѽ��δ֪
			//�������ݿ�
			showErrMsg(gbl, m_party, m_rspcode);
			*/
		//	yct_data.yct_ack=0; //���ͨ��ֵδȷ��
			//return 1;
		//}
		if (yct_data.status==4 ||yct_data.status==9)
		{
			TS_OutPutMsg(gbl.dPosInfo,"����һ�����ͨ����ʧ�ܣ����˷ѣ��밴��ENTER��,�������˻����䶯",0,iSelect);
		}
		else if (yct_data.status==2)
		{

			TS_OutPutMsg(gbl.dPosInfo,"��һ�ʹ������׳ɹ�������Enter����������",0,iSelect);
		}
	
		wrYctDataLog("after NotFinish", yct_data);
		//PEȷ�������
		yct_data.status = AckRevTrade(gbl, yct_data); //���ͨ���ս���״̬

		if (yct_data.status==5)
		{
			yct_data.status=AckRevTrade(gbl,yct_data);
		}
		//�������ݿ�
		CDB::updateYctYPData(yct_data);
		if (yct_data.status==5)
		{
			TS_OutPutMsg(gbl.dPosInfo,"�˷ѳɹ����������˻����䶯�����������밴��Enter��",0,iSelect);
		}
		else if(yct_data.status==6)
		{

			char tmp[120];
			bbzero(tmp);
			sprintf(tmp,"�˷�ʧ�ܣ�����ϵͳ�ο���Ϊ��%s\n����ϵ���߿ͷ����߲���ͷ��绰400-80-11888�����������밴��Enter��",yct_data.stan);
			TS_OutPutMsg(gbl.dPosInfo,tmp,0,iSelect);
		}
		return 0;

	}
	else
	{
		return 0;
	}
}

// ��ͨǮ���Ĳ���������
// ��Σ�yct_serial(8):������ˮ��;pki_card(8):PKI������
// ���Σ�ticket_amo_aft(12):Ʊ����ֵ�����; errCode:�������
// ����ֵ
// 0���ɹ�
// 1�������������ʧ�ܣ����δ֪
// 2�����������صĴ��󣬽��δ֪
// 3�������ͨ����ʧ�ܣ����δ֪
// 4��PKI�������������δ֪
// 5��ʧ��
int CYCTYPTrade::NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	SOCKET sd=0;
	CConnect con;//������
	
	wrLog("��NormalNotFinish��");
	
	iRet = connTest(dPosInfo, com, comType);//���Ӳ��ԣ�ͬʱҲ�Ͽ��ϱʽ��ף���ʼ��������
	if( iRet==0 )//�ɹ�
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = CONNCOMFAIL;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	wrLog("��1: commuWithIC��");
	bbzero(icData);
	CTools::hex_to_byte((BYTE*)yct_serial+2, (BYTE*)icData, 8);
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, NORMAL_NOTFINISH_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE*)pki_card_this, 4);//�����߼�����
			wrLog("�����߼�����pki_card_this [%s]", pki_card_this);
			if( memcmp(pki_card_this, pki_card, 8)==0 )
			{
				sndLen = rcvLen-6-20-1;
				memcpy(sndData, rcvData+6+20, sndLen);
			}
			else
			{
				wrLog("��ԭ���׹����߼����Ų�һ��pki_card [%s]", pki_card);
				*party = FROMCTSI_N;
				*rspcode = PKICHGNOTFINISH;
				return 4;// 4��PKI�������������δ֪
			}			
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCD )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			return 0;// 0���ɹ�
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCE )
		{
			*party = FROMCTSI;
			*rspcode = FAIL_LAST;
			return 5;// 5��ʧ��
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	
	wrLog("��2: commuWithYCTIC��");
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
			return 0;// 0���ɹ�
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCE )
		{
			*party = FROMCTSI;
			*rspcode = FAIL_LAST;
			closesocket(sd);
			return 5;// 5��ʧ��
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	wrLog("��3: commuWithYCTIC��");
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
				return 0;// 0���ɹ�
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = FAIL_LAST;
				closesocket(sd);
				return 5;// 5��ʧ��
			}
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0xCD )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			closesocket(sd);
			return 0;// 0���ɹ�
		}
		else if( rcvData[4]==0xA1 && (rcvData[5]==0xCE || rcvData[5]==0xCA) )
		{
			*party = FROMCTSI;
			*rspcode = FAIL_LAST;
			closesocket(sd);
			return 5;// 5��ʧ��
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
}

// ��ƱǮ���Ĳ���������
// ��Σ�signin_serial(16):������ˮ��;yct_serial(8):������ˮ��;pki_card(8):PKI������
// ���Σ�ticket_amo_aft(12):Ʊ�����; errCode:�������; signin_serial(16):������ˮ��;yct_serial(8):������ˮ��;
// ����ֵ
// 0���ɹ�
// 1�������������ʧ�ܣ����δ֪
// 2�����������صĴ��󣬽��δ֪
// 3�������ͨ����ʧ�ܣ����δ֪
// 4��PKI�������������δ֪
// 5��ʧ��
// 6��ǩ��ʧ�ܣ����δ֪
// 7�����ͨ���صĴ��� ���δ֪
int CYCTYPTrade::MonthNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *signin_serial, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	SOCKET sd=0;
	CConnect con;//������
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	
	wrLog("��MonthNotFinish��");
	
	if( memcmp(signin_serial, "0000000000000000", 16)==0 )
	{
		iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
		if( iRet==0 )//�ɹ�
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
			return 6;// 6��ǩ��ʧ�ܣ����δ֪
		}
	}
	else
	{
		NULL;
	}
	
	iRet = connTest(dPosInfo, com, comType);//���Ӳ��ԣ�ͬʱҲ�Ͽ��ϱʽ��ף���ʼ��������
	if( iRet==0 )//�ɹ�
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = CONNCOMFAIL;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	wrLog("��1: commuWithIC��");
	bbzero(icData);
	CTools::hex_to_byte((BYTE*)yct_serial, (BYTE*)icData, 10);
	CTools::hex_to_byte((BYTE*)signin_serial, (BYTE*)icData+5, 16);
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, MONTH_NOTFINISH_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6+8, (BYTE*)pki_card_this, 4);//�����߼�����
			wrLog("�����߼�����pki_card_this [%s]", pki_card_this);
			if( memcmp(pki_card_this, pki_card, 8)==0 )
			{
				sndLen = rcvLen-6-20-1;
				memcpy(sndData, rcvData+6+20, sndLen);
			}			
			else
			{
				wrLog("��ԭ���׹����߼����Ų�һ��pki_card [%s]", pki_card);
				*party = FROMCTSI_N;
				*rspcode = PKICHGNOTFINISH;
				return 4;// 4��PKI�������������δ֪
			}			
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	
	wrLog("��2: commuWithYCTIC��");
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
				return 0;// 0���ɹ�
			}
			else if( rcvData[6+5]==0xA1 || rcvData[6+5]==0xFF )
			{
				*party = FROMCTSI;
				*rspcode = FAIL_LAST;
				closesocket(sd);
				return 5;// 5��ʧ��
			}
			else
			{
				*party = FROMYCT_N;
				*rspcode = rcvData[6+5];
				closesocket(sd);
				return 7;// 7�����ͨ���صĴ��� ���δ֪
			}
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
}

void CYCTYPTrade::setInPar(CGlobal inGbl) //���ô������
{
	gbl = inGbl;
	getComPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, m_com, m_comType);
}
int CYCTYPTrade::getComPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int &com, int &comType)
{
	int flag = 0; //��ʼ��
	//���ô���
	if(inType==5 || inType==8)
	{
		comType = inType;
		com  = 0x03;
		flag = 1;
	}
	//�󴮿�
	else if(leftType==5 || leftType==8)
	{
		comType = leftType;
		com  = 0x01;
		flag = 1;
	}
	//�Ҵ���
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
//ȷ�������
// ��Σ�yct_data:���ͨ������Ϣ
// ���Σ���
// ����ֵ��
// 2��ȷ�ϳɹ�
// 3��ȷ��ʧ��
// 5�������ɹ�
// 6������ʧ��
int CYCTYPTrade::AckRevTrade(CGlobal gbl, YCT_DATA yct_data)
{
	int iRet=0;
	if(yct_data.status==1)//�ɹ�
	{
		memset(gbl.fee_type, 0, sizeof(gbl.fee_type));
		memcpy(gbl.fee_type, "0083", 4);
		CTrade trade;
		trade.set_in_par(gbl);
		trade.cardInq_yctyp();
		iRet=AckTrade(gbl, yct_data);//ȷ��
		wrLog("���ͨ����״̬��%d��",iRet);
	}
	else if (yct_data.status==5)
	{
		CTrade trade;
		trade.set_in_par(gbl);
		iRet=AckTrade(gbl, yct_data);//ȷ��
		wrLog("���ͨ����״̬��%d��",iRet);
	}

	else if(yct_data.status==4 || yct_data.status==9) //�˷ѽ���
	{
		iRet=RevTrade(gbl, yct_data);//����
	}
	else
	{
		iRet = yct_data.status;
	}
	return iRet;
}


//������
//payType:1:����Ʊ�Ȳ�ѯ 2:����Ʊֱ�ӳ�ֵ 3:����90�α�����Ч 4:����90��������Ч 5:�������� 6:�����޴� 7:�ֽ��ֵ 8:����ѯ
// ����ֵ��
// 0���ɹ���ȷ��
// 1���ɹ�δȷ��
// 2��ʧ���ѳ���
// 3��ʧ��δ����
// 4��δ֪���
// 5���۷�ǰʧ��
int CYCTYPTrade::TradeHdl(CGlobal gbl, char *title, int payType, char *pki_card, char *signin_serial)
{
	int iRet=0, iSelect=0;
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	char m_title[1024];
	char priCont[2048];
	bbzero(m_title);
	bbzero(priCont);
	
	strcpy(m_title, title);
	
	CDB::update_epos_term(gbl.pos_no, "8");//���ն�״̬��Ϊ"�����۷���"
	
	iRet = TS_OutPutMsg(gbl.dPosInfo, "��ָ���ͻ�����������ȷ�����ڶ������ϣ������ƶ�����\n�����������ͨ/������ͨ��ֵ�������ȹػ����ٷ����ڶ������ϣ��������Ų����\n����ȷ�ϡ����� �������ء�ȡ��", 0, iSelect);
	if( iRet==0 && iSelect==0 )
	{
		NULL;
	}
	else
	{
		return 5;// 5���۷�ǰʧ��
	}
	
	if( payType==1 || payType==2 || payType==7 || payType==8)
	{
		strcpy(gbl.fee_type, "0056");
		iRet = YCT_NormalTrade(&gbl, payType, &m_party, &m_rspcode, priCont);
		if (payType==7)
		{
			strcpy(m_title,"���ͨ�ֽ��ֵ");
		}
		else
		{
			strcpy(m_title, "���ͨ��ͨǮ����ֵ");
		}
	}
	else if( payType==3 || payType==4 || payType==5 || payType==6 )
	{
		strcpy(gbl.fee_type, "0247");//��Ʊ
		iRet = MonthTrade(&gbl, payType, pki_card, signin_serial, &m_party, &m_rspcode, priCont);
	}
	else
	{
		m_party = FROMCTSI;
		m_rspcode = FATALERR;
		iRet = 5;                                                                                                                                                                                                                                                                                                                                                                                                                            
		wrLog("payType error");
	}

	CDB::update_epos_term(gbl.pos_no, "1");//���ն�״̬��Ϊ"����"

	if (payType==8 &&(m_party==FROMSUCC && (m_rspcode==SUCC_MSG || m_rspcode==SUCC_NOMSG)))
	{
		return 0;
	}
	showErrMsg(gbl, m_party, m_rspcode);
	if( m_party==FROMSUCC && (m_rspcode==SUCC_MSG || m_rspcode==SUCC_NOMSG) )  //�ɹ���ӡƾ��
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
		strcpy(menuList.acTitle, "��ѡ����Ҫ��ֵ�Ľ��");
		strcpy(menuList.menuItem[0], "50Ԫ");
		strcpy(menuList.menuItem[1], "100Ԫ");
		strcpy(menuList.menuItem[2], "150Ԫ");
		strcpy(menuList.menuItem[3], "200Ԫ");


	}
	else
	{
		menuList.iTotalNum = 5;
		strcpy(menuList.acTitle, "��ѡ����Ҫ��ֵ�Ľ��");
		strcpy(menuList.menuItem[0], "50Ԫ");
		strcpy(menuList.menuItem[1], "100Ԫ");
		strcpy(menuList.menuItem[2], "150Ԫ");
		strcpy(menuList.menuItem[3], "200Ԫ");
		//	strcpy(menuList.menuItem[4],"��Ѻ��(10Ԫ)"); //Ѻ��
		strcpy(menuList.menuItem[4], "1��Ǯ");

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
			//		strcpy(pgbl->amount,"000000001000"); //��Ѻ��10Ԫ��������ҪE����ֵ
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
					//		strcpy(pgbl->amount,"000000001000"); //��Ѻ��10Ԫ��������ҪE����ֵ
					//		return 0;
				case 5:
					strcpy(pgbl->amount,"000000000001");
					return 0;
				default: 
					return CANCLE_KEY;
		}
				

		}

	
	//�ж�����Ƿ����
	if(CDB::getDayAmo(pgbl->dPosInfo.pos_no, &nowAmo)==0)
	//20120105�޸�-yyx
	//if (CDB::pep_getDayAmo(pgbl->pos_no, &nowAmo) == 0)
	{
		if( pgbl->dayAmo!=0 && nowAmo+atoi(pgbl->amount)>pgbl->dayAmo )
		{
			TS_OutPutMsg(pgbl->dPosInfo, "�ѳ��������������ѽ�����ȡ��", 1, iSelect);
			return -1;
		}
	}
	else
	{
		TS_OutPutMsg(pgbl->dPosInfo, "���ݿ������������ȡ��\n���Ժ�����!\n ������׽������ȷ���� ���һ�ʽ��ײ�ѯ", 1, iSelect);
		return -1;
	}
}

// ��ͨǮ����������
// ��Σ���
// ���Σ���
// ����ֵ��
// 0���ɹ���ȷ��
// 1���ɹ�δȷ��
// 2��ʧ���ѳ���
// 3��ʧ��δ����
// 4��δ֪���
// 5���۷�ǰʧ��
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
		return 5;// 5���۷�ǰʧ��
	}
	
	if((pgbl->posInfo).rightType == 0x08)	//���������
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
	//��ʱ������������������
	else
	{
		TS_OutPutMsg(pgbl->dPosInfo, "��ЧӲ�����޷��������ף����������ã�������������ϵ�����׿ͷ�", 0, iSelect);
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 5;
	}


	if(payType==1 || payType==7 || payType==8)//�Ȳ�ѯ
	{
		//�ж϶���������
		//iRet = ST_initReader(pgbl->dPosInfo, com, comType);
		if((pgbl->posInfo).rightType == 0x08)
		{
			if(YCT_VER==3)
			iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			else
			iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			//���н��ײ�ѯ
		}
		else
		{
			iRet = NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
		}

		if (m_rspcode == BLACKLIST)
		{
			*party = m_party;
			*rspcode = m_rspcode;
			TS_OutPutMsg(pgbl->dPosInfo,"�˿�Ϊ�������������ײ�����!",1,iSelect);
			return 0;
		}
		
		if( iRet==0 )
		{
			if (M1_CPU == 1)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//��ʾ���
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
//    			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				Display_credit = atof(mny)/100;
				sprintf(show_money,"%.2f",Display_credit);
			}
			if (M1_CPU == 2)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//��ʾ���
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
				//			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				wrLog("�������:%.2fԪ",atof(des_lim));
				Display_credit = atof(mny)/100 - atof(des_lim);
//				wrLog("Ǯ����ʾ�����:%.2f",Display_credit);
				sprintf(show_money,"%.2f",Display_credit);
//				wrLog("Ǯ����ʾ���:%.2f",atof(show_money));
			}

			wrMonitorLog(12, 0, 0, *pgbl, pack);	//�����־
			bbzero( temp_buff );
			if (payType == 8)
			{
				sprintf(temp_buff, "�������Ϊ��%sԪ\n����Esc������", show_money);
			}
			else
			{
				sprintf(temp_buff, "�������Ϊ��%sԪ\n����ȷ�ϡ���ֵ\n�������ء�ȡ��", show_money);
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
				return 5;// 5���۷�ǰʧ��
			}
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5���۷�ǰʧ��
		}
	}
	else
	{
		NULL;
	}

	if (payType == 8)
	{
		//ֻ��ѯ
		return 0;
	}
	//ѡ���
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
		return 5;// 5���۷�ǰʧ��
	}
	
	bbzero( temp_buff );
	if(payType == 1 || payType==7)
		sprintf(temp_buff, "����ǰ��%sԪ\n��ֵ��%.2fԪ\n���׺��%.2fԪ\n����ȷ�ϡ���ֵ\n�������ء�ȡ��", 
		show_money, atof(pgbl->amount)/100, atof(pgbl->amount)/100 + atof(show_money));
	else if(payType == 2)
		sprintf(temp_buff, "��ֵ��%.2fԪ\n����ȷ�ϡ���ֵ\n�������ء�ȡ��", 
		atof(pgbl->amount)/100);

/*
	if (atof(pgbl->amount)/100 + money - 1000.00 >0.001)
	{
		TS_OutPutMsg(pgbl->dPosInfo," \n����ֵ�󳬹�������ޡ��밴��ȷ�ϡ�����",0,iSelect);
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
		return 5;// 5���۷�ǰʧ��
	}
	

	//�۷ѵ�ʱ���ȡһ�ο���Ϣ��������ֶ��������ͨ���źͽ��׵Ŀ��Ų�һ�£���ȡ�����ף�������ʾ�û��������������ξ����ɹ�����ȡ���ôν���
	//��ȡ��Ƭ��������Ϣ,���ؿ�Ƭ��������Ϣ
	/***************************begin*********************************************/
	for (int i = 0; i<3; i++)
	{
		bbzero(card_physics_info);
		query_iRet=R_PUB_QRY_PHYSICS_INFO(pgbl->dPosInfo,com,comType,(BYTE *)card_physics_info);
		if (query_iRet == 0)
		{
			char SW1SW2[4+1];
			bbzero(SW1SW2);
			CTools::byte_to_hex((BYTE *)card_physics_info+8,(BYTE *)ticket_no,8);//ȡ���߼�����
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
				sprintf(outPut_Msg,"��źÿ�Ƭ�����¶���,������Ӧ��״̬�����%s",errMsg);
				TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
				
			}
			else
			{
				if (strcmp(yct_data.ticket_no,ticket_no) !=0 ) //�жϿ�Ƭ�Ƿ��������֮ǰ��ѯ�Ŀ�Ƭһ��
				{
					TS_OutPutMsg(pgbl->dPosInfo,"��Ƭ������Ŀ�Ƭ��Ϣ��һ�£�������",1,iSelect);
				}
				
				else
				{
					
					break;
					
				}
				
			}
			
			
			
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��Ƭ��Ϣ���������·ſ�",1,iSelect);
			
		}
	}
	
	if (query_iRet!=0 )
	{
		TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��Ƭ��Ϣʧ�ܣ�����ʧ��",1,iSelect);
		*party = FROMCTSI;
		*rspcode = USERCANCEL;
		return 5;
	}
	else
	{
		if (strcmp(yct_data.ticket_no,ticket_no) !=0)
		{
			TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��Ƭ��Ϣʧ�ܣ�����ʧ��",1,iSelect);
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
			return 5;
			
		}
		
	}
	
	
	/***************end*of*readcardinfo*******************************************************/
	

	/***********************end*of*readcardinfo***********************************************/


	if(atoi(pgbl->amount)==1000) //��Ѻ��pe�����п۷�
		
	{
		TS_OutPutMsg(pgbl->dPosInfo,"\n��ҵ����������ͨѺ�𿨣���ͨ����Ѻ����г�ֵ������ˢ��\n ����ȷ�ϡ����룬����ESC���˳�",0,iSelect);
		//��¼���ͨ�������� 
		
		CTools::getDate(yct_data.txn_date);
		CTools::getTime(yct_data.txn_time);
		//�������ͨ��ֵ�ն˺�
		CTools::getYctStan(pgbl->stan_no); //��ȡ���ͨϵͳ�ο���
		CTools::getYct_posSerial(pgbl->pos_serial);
		
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
		strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
		strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
		strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
		strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
		
		yct_data.status = 6; //Ĭ��Ϊ��ֵʧ��.����ʧ��
		CDB::setYctYPData(yct_data); 
	}
    else
	{
		//ˢ��������PE�۷�

		iRet = PayPe(pgbl, yct_data, &m_party, &m_rspcode);
		
		if( iRet==0 )//�ɹ�
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
			//�ڻ�ȡ��ֵ��Կ֮ǰ��Ĭ�ϳ�ֵʧ��
			yct_data.status = 4;
			//��¼���ͨ��������
			CDB::setYctYPData(yct_data);
			
			//���½����ܶ�
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
			//��¼���ͨ��������
			yct_data.status = 4;
			CDB::setYctYPData(yct_data);
			
			//PEȷ�������
			trade.set_in_par(*pgbl);
			yct_data.status = trade.YCT_AckRevTrade(yct_data,yct_data.status); //�۷�ǰʧ��
			//���ͳ�ֵȷ��
			if (yct_data.status==5)
			{
				iRet=trade.YCT_AckRevTrade(yct_data,yct_data.status); //�����ɹ��ŷ���ȷ��
			}
			//�������ݿ�
			CDB::updateYctYPData(yct_data);
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 4;// 5���۷�ǰʧ��
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 5;// 5���۷�ǰʧ��
		}

	}
	
	try
	{
		//��ֵ
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
		TS_OutPutMsg(pgbl->dPosInfo, " \n\n\n���ͨ��ֵ�����쳣�����ٴβ�ѯ���˶Գ�ֵ�Ƿ�ɹ������������밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ", 1,iSelect);
	}
	wrYctDataLog("NormalPay", yct_data);
	
	if( iRet==0 )
	{
		if(atoi(pgbl->amount)==1000)
		{
			
			yct_data.status = 2; //��Ѻ��ɹ�������Ҫpe ����ȷ�ϣ����ͨ�ն˽���״ֱ̬���޸�Ϊ2
			CDB::updateYctYPData(yct_data);
			
		}
		else
		{
			yct_data.status = 1;
		}
	

	}
	else if( iRet==1 || iRet==2 || iRet==3 || iRet==4 || iRet==8 )
	{
		if(atoi(pgbl->amount)==1000) //�������Ѻ��
		{
			
			yct_data.status=6;
		}
		
		else
		{
			yct_data.status = 4;
		}
	}
	else if(iRet==10)//д��ʱ��ʱ������ȷ���Ƿ��ֵ�ɹ�
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
						if(iRet==0)//�ɹ�
						{

							if(atoi(pgbl->amount)==1000)

								{
								//�����Ѻ���ֵ����ֵ�ɹ�����pe ȷ�ϣ�ֱ���޸Ľ���״̬
								//	CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
								yct_data.status = 2;
								CDB::updateYctYPData(yct_data);
								}
							
							else
							{
							//���½����ܶ�
							CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
							yct_data.status = 1;//��ֵ�ɹ�δȷ��
							}
						}
						else//δ֪
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//�������ݿ�
							CDB::updateYctYPData(yct_data);
							return 4;// 4��δ֪���
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
						if(iRet==5)//ʧ��
						{
							if (atoi(pgbl->amount)==1000)
							{
								yct_data.status = 6;//��ֵʧ�ܳ���ʧ��
							}
							else
							{
								
								yct_data.status = 4;//��ֵʧ��δ����
								
							}
							
						}
						else//δ֪
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//�������ݿ�
							CDB::updateYctYPData(yct_data);
							return 4;// 4��δ֪���
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
	else//��Ҫ������������
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
				yct_data.status = 7;//�޷���֪��ֵ���
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
		if(iRet==0)//�ɹ�
		{
	
			if(atoi(pgbl->amount)==1000)
			{
				//�����Ѻ���ֵ������Ҫ����peȷ�ϣ�ֱ���޸����Ľ���״̬
				//CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 2;
				CDB::updateYctYPData(yct_data);
			}
			else
			{
				//���½����ܶ�
				CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 1;//��ֵ�ɹ�δȷ��	
				
			}		
		}
		else if(iRet==5)//ʧ��
		{
			if (atoi(pgbl->amount)==1000)
			{
				yct_data.status = 6;//��ֵʧ�ܳ���ʧ��
			}
			else
			{
				yct_data.status = 4;//��ֵʧ��δ����
			}
		}
		else//δ֪
		{
			yct_data.status = 0;
			*party = m_party;
			*rspcode = m_rspcode;
			//�������ݿ�
			CDB::updateYctYPData(yct_data);
			return 4;// 4��δ֪���
		}
	}
	CDB::updateYctYPData(yct_data);
	//PEȷ�������
	yct_data.status = AckRevTrade(*pgbl, yct_data);
	if (yct_data.status == 3)
	{
		*party = FROMPE;
		*rspcode = 99;
		return 1;
	}
	//�������ݿ�
	CDB::updateYctYPData(yct_data);
	CDB::update_epos_term(pgbl->pos_no, "1");//���ն�״̬��Ϊ"����"

	if (payType == 7)
	{
		set_yctthird_priCont(0,&yct_data,priCont);
	}
	else
	{
		strcat(priCont, "���ͨ���ţ�");
		strcat(priCont, yct_data.ticket_no);
		strcat(priCont, "\n���ͨ��ˮ�ţ�");
		strcat(priCont, yct_data.yct_serial+2);
		sprintf(priCont+strlen(priCont), "\nԭ���: %.2fԪ", atof(yct_data.ticket_amo_bef)/100);
		sprintf(priCont+strlen(priCont), "\n��ֵ���: %.2fԪ", atof(yct_data.amount)/100);
		sprintf(priCont+strlen(priCont), "\n�����: %.2fԪ", atof(yct_data.ticket_amo_bef)/100 + atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n��ֵ���: %.2fԪ", atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n�����: %.2fԪ", atof(yct_data.ticket_amo_aft)/100);
	}
	strcpy(pgbl->yct_disp,priCont);
	
	*party = m_party;
	*rspcode = m_rspcode;
	return 0;
}


//���캯��
// ��ͨǮ����������
// ��Σ���
// ���Σ���
// ����ֵ��
// 0���ɹ���ȷ��
// 1���ɹ�δȷ��
// 2��ʧ���ѳ���
// 3��ʧ��δ����
// 4��δ֪���
// 5���۷�ǰʧ��
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
			return 5;// 5���۷�ǰʧ��
	}

	
	if(pgbl->posInfo.rightType == 0x08)	//���������
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
	//��ʱ������������������
	else
	{
		TS_OutPutMsg(pgbl->dPosInfo, "��ЧӲ�����޷��������ף����������ã�������������ϵ�����׿ͷ�", 0, iSelect);
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 5;
	}


	if(payType==1 || payType==7 || payType==8)//�Ȳ�ѯ
	{
		//�ж϶���������
		//iRet = ST_initReader(pgbl->dPosInfo, com, comType);
		if(pgbl->posInfo.rightType == 0x08)
		{
			if(YCT_VER==3)
			iRet = ST_NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			else
			iRet = ST_NormalEnqPay2(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
			//���н��ײ�ѯ
		}
		else
		{
			iRet = NormalEnqPay(pgbl->dPosInfo, com, comType, NORMALENQ, &yct_data, "000000000000", &m_party, &m_rspcode);
		}

		if (m_rspcode == BLACKLIST)
		{
			*party = m_party;
			*rspcode = m_rspcode;
			TS_OutPutMsg(pgbl->dPosInfo,"�˿�Ϊ�������������ײ�����!",1,iSelect);
			return 0;
		}
		
		if( iRet==0 )
		{
			if (M1_CPU == 1)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//��ʾ���
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
//    			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				Display_credit = atof(mny)/100;
				sprintf(show_money,"%.2f",Display_credit);
			}
			if (M1_CPU == 2)
			{
				wrYctDataLog("NormalEnq", yct_data);
				//��ʾ���
				bbzero(mny);
				sprintf(mny,"%s",yct_data.ticket_amo_bef);
				//			CTools::chgToMny(yct_data.ticket_amo_bef, mny);
				wrLog("�������:%.2fԪ",atof(des_lim));
				Display_credit = atof(mny)/100 - atof(des_lim);
//				wrLog("Ǯ����ʾ�����:%.2f",Display_credit);
				sprintf(show_money,"%.2f",Display_credit);
//				wrLog("Ǯ����ʾ���:%.2f",atof(show_money));
			}

			wrMonitorLog(12, 0, 0, *pgbl, pack);	//�����־
			bbzero( temp_buff );
			if (payType == 8)
			{
				sprintf(temp_buff, "�������Ϊ��%sԪ\n����Esc������", show_money);
			}
			else
			{
				sprintf(temp_buff, "�������Ϊ��%sԪ\n����ȷ�ϡ���ֵ\n�������ء�ȡ��", show_money);
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
				return 5;// 5���۷�ǰʧ��
			}
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5���۷�ǰʧ��
		}
	}
	else
	{
		NULL;
	}

	if (payType == 8)
	{
		//ֻ��ѯ
		return 0;
	}
	//ѡ���
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
		return 5;// 5���۷�ǰʧ��
	}
	
	
	bbzero( temp_buff );
	if(payType == 1 || payType==7)
		sprintf(temp_buff, "����ǰ��%sԪ\n��ֵ��%.2fԪ\n���׺��%.2fԪ\n����ȷ�ϡ���ֵ\n�������ء�ȡ��", 
		show_money, atof(pgbl->amount)/100, atof(pgbl->amount)/100 + atof(show_money));
	else if(payType == 2)
		sprintf(temp_buff, "��ֵ��%.2fԪ\n����ȷ�ϡ���ֵ\n�������ء�ȡ��", 
		atof(pgbl->amount)/100);

/*
	if (atof(pgbl->amount)/100 + money - 1000.00 >0.001)
	{
		TS_OutPutMsg(pgbl->dPosInfo," \n����ֵ�󳬹�������ޡ��밴��ȷ�ϡ�����",0,iSelect);
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
		return 5;// 5���۷�ǰʧ��
	}
	
	//�۷ѵ�ʱ���ȡһ�ο���Ϣ��������ֶ��������ͨ���źͽ��׵Ŀ��Ų�һ�£���ȡ�����ף�������ʾ�û��������������ξ����ɹ�����ȡ���ôν���
	//��ȡ��Ƭ��������Ϣ,���ؿ�Ƭ��������Ϣ
	/***************************begin*********************************************/
	for (int i = 0; i<3; i++)
	{
		bbzero(card_physics_info);
		query_iRet=R_PUB_QRY_PHYSICS_INFO(pgbl->dPosInfo,com,comType,(BYTE *)card_physics_info);
		if (query_iRet == 0)
		{
			char SW1SW2[4+1];
			bbzero(SW1SW2);
			CTools::byte_to_hex((BYTE *)card_physics_info+8,(BYTE *)ticket_no,8);//ȡ���߼�����
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
				sprintf(outPut_Msg,"��źÿ�Ƭ�����¶���,������Ӧ��״̬�����%s",errMsg);
				TS_OutPutMsg(pgbl->dPosInfo,outPut_Msg,1,iSelect);
				
			}
			else
			{
				if (strcmp(yct_data.ticket_no,ticket_no) !=0 ) //�жϿ�Ƭ�Ƿ��������֮ǰ��ѯ�Ŀ�Ƭһ��
				{
					TS_OutPutMsg(pgbl->dPosInfo,"��Ƭ������Ŀ�Ƭ��Ϣ��һ�£�������",1,iSelect);
				}
				
				else
				{
					
					break;
					
				}
				
			}
			
			
			
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��Ƭ��Ϣ���������·ſ�",1,iSelect);

		}
	}
	
	if (query_iRet!=0 )
	{
		TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��Ƭ��Ϣʧ�ܣ�����ʧ��",1,iSelect);
		*party = FROMCTSI;
		*rspcode = USERCANCEL;
		return 5;
	}
	else
	{
		if (strcmp(yct_data.ticket_no,ticket_no) !=0)
		{
			TS_OutPutMsg(pgbl->dPosInfo,"��ȡ��Ƭ��Ϣʧ�ܣ�����ʧ��",1,iSelect);
			*party = FROMCTSI;
			*rspcode = USERCANCEL;
			return 5;

		}

	}


	/***************end*of*readcardinfo*******************************************************/


	if(atoi(pgbl->amount)==1000) //��Ѻ��pe�����п۷�
		
	{
		TS_OutPutMsg(pgbl->dPosInfo,"\n��ҵ����������ͨѺ�𿨣���ͨ����Ѻ����г�ֵ������ˢ��\n ����ȷ�ϡ����룬����ESC���˳�",0,iSelect);
		//��¼���ͨ�������� 
		
		CTools::getDate(yct_data.txn_date);
		CTools::getTime(yct_data.txn_time);
		//�������ͨ��ֵ�ն˺�
		CTools::getYctStan(pgbl->stan_no); //��ȡ���ͨϵͳ�ο���
		CTools::getYct_posSerial(pgbl->pos_serial);
		
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.stan, pgbl->stan_no, sizeof(yct_data.stan)-1);
		strncpy(yct_data.biz_type, pgbl->fee_type, sizeof(yct_data.biz_type)-1);
		strncpy(yct_data.term_id, pgbl->pos_no, sizeof(yct_data.term_id)-1);
		strncpy(yct_data.term_serial, pgbl->pos_serial, sizeof(yct_data.term_serial)-1);
		strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
		strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
		strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
		
		yct_data.status = 6; //Ĭ��Ϊ��ֵʧ��.����ʧ��
		CDB::setYctYPData(yct_data); 
	}
    else
	{
		//ˢ��������PE�۷�
		char pos_serial[7+1];
		bbzero(pos_serial);
		trade.set_in_par(*pgbl);
		iRet = trade.YCT_PayPe(yct_data, &m_party, &m_rspcode,pgbl->stan_no,pos_serial);
	
		if( iRet==0 )//�ɹ�
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
			strncpy(yct_data.term_serial, pos_serial, sizeof(yct_data.term_serial)-1); //�˴�Ҫ����۷�ʱ����ն���ˮ�ű���һ��
			bbzero(yct_data.bank);
			strncpy(yct_data.bank, pgbl->bank, sizeof(yct_data.bank)-1);
			bbzero(yct_data.card_no);
			strncpy(yct_data.card_no, pgbl->card_no, sizeof(yct_data.card_no)-1);
			bbzero(yct_data.amount);
			strncpy(yct_data.amount, pgbl->amount, sizeof(yct_data.amount)-1);
			bbzero(yct_data.yct_serial);
			bbzero(yct_data.result_message);
			bbzero(yct_data.ticket_amo_aft);
			//�ڻ�ȡ��ֵ��Կ֮ǰ��Ĭ�ϳ�ֵʧ��
			yct_data.status = 4;
			//��¼���ͨ��������
			CDB::setYctYPData(yct_data);
			
			//���½����ܶ�
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
			//��¼���ͨ��������
			yct_data.status = 4;
			CDB::setYctYPData(yct_data);
			//PEȷ�������
			yct_data.status = trade.YCT_AckRevTrade(yct_data,yct_data.status); //���ͨ����
			if (yct_data.status==5)
			{
				yct_data.status=trade.YCT_AckRevTrade(yct_data,yct_data.status);
			}
			//�������ݿ�
			CDB::updateYctYPData(yct_data);
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 4;// 5���۷�ǰʧ��
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			
			return 5;// 5���۷�ǰʧ��
		}

	}
	
	try
	{
		//��ֵ
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
		TS_OutPutMsg(pgbl->dPosInfo, " \n\n\n���ͨ��ֵ�����쳣�����ٴβ�ѯ���˶Գ�ֵ�Ƿ�ɹ������������밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ", 1,iSelect);
	}
	wrYctDataLog("NormalPay", yct_data);
	
	if( iRet==0  || iRet==-1) //-1 �ǳ�ʱ���
	{
		if(atoi(pgbl->amount)==1000)
		{
			
			yct_data.status = 2; //��Ѻ��ɹ�������Ҫpe ����ȷ�ϣ����ͨ�ն˽���״ֱ̬���޸�Ϊ2
			CDB::updateYctYPData(yct_data);
			
		}
		else
		{
			yct_data.status = 1;
		}
	

	}
	else if( iRet==1 || iRet==2 || iRet==3 || iRet==4 || iRet==8 )
	{
		if(atoi(pgbl->amount)==1000) //�������Ѻ��
		{
			
			yct_data.status=6;
		}
		
		else
		{
			yct_data.status = 4;
		}
	}
	else if(iRet==10)//д��ʱ��ʱ������ȷ���Ƿ��ֵ�ɹ�
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
						if(iRet==0)//�ɹ�
						{

							if(atoi(pgbl->amount)==1000)

								{
								//�����Ѻ���ֵ����ֵ�ɹ�����pe ȷ�ϣ�ֱ���޸Ľ���״̬
								//	CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
								yct_data.status = 2;
								CDB::updateYctYPData(yct_data);
								}
							
							else
							{
							//���½����ܶ�
							CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
							yct_data.status = 1;//��ֵ�ɹ�δȷ��
							}
						}
						else//δ֪
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//�������ݿ�
							CDB::updateYctYPData(yct_data);
							return 4;// 4��δ֪���
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
						if(iRet==5)//ʧ��
						{
							if (atoi(pgbl->amount)==1000)
							{
								yct_data.status = 6;//��ֵʧ�ܳ���ʧ��
							}
							else
							{
								
								yct_data.status = 4;//��ֵʧ��δ����
								
							}
							
						}
						else//δ֪
						{
							yct_data.status = 0;
							*party = m_party;
							*rspcode = m_rspcode;
							//�������ݿ�
							CDB::updateYctYPData(yct_data);
							return 4;// 4��δ֪���
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
	else//��Ҫ������������
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
				yct_data.status = 7;//�޷���֪��ֵ���
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
		if(iRet==0)//�ɹ�
		{
	
			if(atoi(pgbl->amount)==1000)
			{
				//�����Ѻ���ֵ������Ҫ����peȷ�ϣ�ֱ���޸����Ľ���״̬
				//CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 2;
				CDB::updateYctYPData(yct_data);
			}
			else
			{
				//���½����ܶ�
				CDB::setSumDayAmo(atoi(pgbl->amount), pgbl->pos_no);
				yct_data.status = 1;//��ֵ�ɹ�δȷ��	
				
			}		
		}
		else if(iRet==5)//ʧ��
		{
			if (atoi(pgbl->amount)==1000)
			{
				yct_data.status = 6;//��ֵʧ�ܳ���ʧ��
			}
			else
			{
				yct_data.status = 4;//��ֵʧ��δ����
			}
		}
		else//δ֪
		{
			yct_data.status = 0;
			*party = m_party;
			*rspcode = m_rspcode;
			//�������ݿ�
			CDB::updateYctYPData(yct_data);
			return 4;// 4��δ֪���
		}
	}
	CDB::updateYctYPData(yct_data);
	//PEȷ�������
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
	//�������ݿ�
	CDB::updateYctYPData(yct_data);
	CDB::update_epos_term(pgbl->pos_no, "1");//���ն�״̬��Ϊ"����"

	if (payType == 7)
	{
		set_yctthird_priCont(0,&yct_data,priCont);
	}
	else
	{
		strcat(priCont, "���ͨ���ţ�");
		strcat(priCont, yct_data.ticket_no);
		strcat(priCont, "\n���ͨ��ˮ�ţ�");
		strcat(priCont, yct_data.yct_serial+2);
		sprintf(priCont+strlen(priCont), "\nԭ���: %.2fԪ", atof(yct_data.ticket_amo_bef)/100);
		sprintf(priCont+strlen(priCont), "\n��ֵ���: %.2fԪ", atof(yct_data.amount)/100);
		sprintf(priCont+strlen(priCont), "\n�����: %.2fԪ", atof(yct_data.ticket_amo_bef)/100 + atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n��ֵ���: %.2fԪ", atof(yct_data.amount)/100);
//		sprintf(priCont+strlen(priCont), "\n�����: %.2fԪ", atof(yct_data.ticket_amo_aft)/100);
	}
	strcpy(pgbl->yct_disp,priCont);
	
	*party = m_party;
	*rspcode = m_rspcode;
	return 0;
}


// ����У����
// ��Σ�data:����; dataLen:���ݳ���
// ���Σ���
// ����ֵ
// 0��һ��
// 1������
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

// ����������ͺͽ�������
// ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������;
// ���Σ���
// ����ֵ
// 0���ɹ�
// 1��ʧ��
int CYCTYPTrade::SndRcvIcData(const TSDLLPosInfo dPosInfo, const int com, const int comType, const char *title, const BYTE *downData, const int downLen, BYTE *upData, int *upLen, int baudRate)
{
	int ret, iSelect;
	TSIcReaderCommand icCmd;
	
	memset(&icCmd, 0, sizeof(icCmd));
	strcpy(icCmd.acTitle, title);
	icCmd.com = com;
	icCmd.comType = comType;
	icCmd.uDownLen = downLen;	//�·����ݵĳ���
	
	memcpy(icCmd.uDownData, downData, downLen);	//�·�������, ����ʾ�����ܳ��Ȳ��ó��� 240 �ֽ�
	wrHex("�·�����������", icCmd.uDownData, icCmd.uDownLen);
	ret = TS_IcReaderCommand(dPosInfo, &icCmd, baudRate);  //�·�����
	if( ret || icCmd.uUpLen==0 ) //���󣬷��س�����Ϣ
	{
		*upLen = 0;
		wrLog("���󷵻�ֵ: [%d]", ret);
		wrHex("��������������", icCmd.uUpData, icCmd.uUpLen);
		if(ret == 7)
			TS_OutPutMsg(dPosInfo, " \n ���������������!\n�����Ӻö���������!", 0, iSelect);
		else
			TS_OutPutMsg(dPosInfo, " \n  ���ն˽�������!\n�����Ӻö���������!", 0, iSelect);
		return 1;
	}
	
	wrHex("��������������", icCmd.uUpData, icCmd.uUpLen);
	
	memcpy(upData, icCmd.uUpData, icCmd.uUpLen);	//���͵�����
	*upLen = icCmd.uUpLen;	//�������ݵĳ���
	
	if( isXor(upData, *upLen) ) //У��
	{
		wrLog("У�������");
		return 1;
	}	
	return 0;
}


// �����ݽ��д��
// ��Σ�tag:���ı�ʶ; ver:�汾; num:����˳���; len:���ĳ���(cmd,stat,data�ĳ���); cmd:�����ʶ; stat:״̬��ʶ; data:����;
// ���Σ�sndData:�����������
// ����ֵ��������
int CYCTYPTrade::packData(int tag, int ver, int num, int len, int cmd, int stat, const BYTE *data, BYTE *sndData)
{
	int sndLen;
	BYTE c;  //У����
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

// ���Ӳ���
// ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������;
// ���Σ���
// ����ֵ
// 0���ɹ�
// 1��ʧ��
int CYCTYPTrade::connTest(const TSDLLPosInfo dPosInfo, const int com, const int comType)
{
	char sndData[256], rcvData[256], data[2];
	int sndLen, rcvLen;
	int len, num, cmd;
	
	wrLog("�����Ӳ��ԡ�");
	num = 0;
	cmd = 0x10;
	len = 2;
	
	bbzero(sndData);
	bbzero(data);
	sndLen = packData(TAG, VER, num, len, cmd, STAT, (BYTE *)data, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if( SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen) )
	{
		wrHex("��������������", (BYTE *)rcvData, rcvLen);
		return 1;
	}
	
	return 0;
}

// ��12λ��Ϊ4λhex��ʽ
// ��Σ�amo:12λ�Ľ��
// ���Σ�hexAmo:4λhex��ʽ�Ľ��
// ����ֵ����
void CYCTYPTrade::getHexAmo(const char *amo, BYTE *hexAmo)
{
	unsigned long lamo = atoi(amo);
	hexAmo[0] = (BYTE)(lamo / 0x1000000);
	hexAmo[1] = (BYTE)(lamo % 0x1000000 /0x10000);
	hexAmo[2] = (BYTE)(lamo % 0x10000 /0x100);
	hexAmo[3] = (BYTE)(lamo % 0x100);
}

// numλhex��ʽ��Ϊ��12λ���
// ��Σ�hexAmo:4λhex��ʽ�Ľ�� num:λ��
// ���Σ�amo:12λ�Ľ��
// ����ֵ����
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



// numλhex��ʽ��Ϊ��12λ���
// ��Σ�hexAmo:4λhex��ʽ�Ľ�� num:λ��
// ���Σ�amo:12λ�Ľ��
// ����ֵ����
//���з������ݵ�λ����

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

// �����ͨ����
// ��Σ�sd:���Ӷ˿�; sendData:��������; sendLen:�������ݳ���
// ���Σ�recvData:��������; recvLen:�������ݳ���
// ����ֵ
// 0���ɹ�
// 1������ʧ��
// 2������ʧ��
// 3������ʧ��
int CYCTYPTrade::SndRcvYctData(const SOCKET sd, char *sendData,int sendLen, char *recvData, int *recvLen, int recvMaxLen)
{
    int iRet=0;
	CConnect con;//������

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

// �ϴ����ݰ������ͨ�����������ջ�Ӧ
// ��Σ�sdType:0:������;1:������; sd:�˿ں�; num:��num������; ndData:�������ͨ������; sndLen:�������ͨ�����ݳ���
// ���Σ�rcvData:�Ӷ��������յ�����; rcvLen:�Ӷ��������յ����ݵĳ���
// ����ֵ
// 0���ɹ�
// 1�������ͨ����ʧ��
// 2�������������ʧ��
int CYCTYPTrade::commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen)
{
	int iRet=0, len=0, icLen=0, sndICLen=0;
	BYTE icData[256], sndICData[256];
	
	wrHex("�����ͨ��������", (BYTE*)sndData, sndLen);
	bbzero(icData);
	iRet = SndRcvYctData(sd, (char *)sndData, sndLen, (char *)icData+20, &icLen, 256);//���ͨ�������ݰ������135�ֽڣ�
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
	wrHex("�����ͨ��������", (BYTE*)icData+20, icLen);
	icLen = icLen + 20;
	len = icLen + 2;
	
	wrLog("��%d: commuWithIC��", num);
	
	bbzero(sndICData);	
	sndICLen = packData(TAG, VER, num*2-1, len, cmd, STAT, icData, sndICData);
	*rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndICData, sndICLen, rcvData, rcvLen);
	if( iRet==0 )
	{
		return 0;// 0���ɹ�
	}
	else
	{
		return 2;// 2�������������ʧ��
	}
}

// ��ͨǮ���Ĳ�ѯ��ֵ
// ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������; enq:0:Ҫ��ֵ; 1:ֻ��ѯ
// ���Σ�pyct_data:���ͨ������Ϣ; errType:��������0:�ɹ�1:���������Ĵ���; errCode:�������0:�ɹ�
// ����ֵ
// 0���ɹ�(��ѯ���ֵ)
// 1�������������ʧ��
// 2�����������صĴ���
// 3�������ͨ����ʧ��
// 4�����ͨ���صĴ���
// 5�������ͨ����ʧ�ܣ���Ҫ������������
// 6�������������ʧ�ܣ���Ҫ������������
// 7��û��ȷ�ϱ�ʶ����Ҫ������������
// 8��ȷ�ϳ�ֵʧ��
// 9�����������صĴ�����Ҫ������������
int CYCTYPTrade::NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256];
	SOCKET sd=0;
	CConnect con;//������
	
	wrLog("��NormalEnqPay��");
	
	iRet = connTest(dPosInfo, com, comType);//���Ӳ��ԣ�ͬʱҲ�Ͽ��ϱʽ��ף���ʼ��������
	if( iRet==0 )//�ɹ�
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 1;// 1�������������ʧ��
	}
	
	wrLog("��1: commuWithIC��");
	memcpy(pyct_data->amount, amount, 12);
	bbzero(icData);
	getHexAmo(amount, icData);	
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, NORMAL_PAY_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE*)pyct_data->pki_card, 4);//�����߼�����
			wrLog("�����߼�����pki_card [%s]", pyct_data->pki_card);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ��
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	wrLog("��2: commuWithYCTIC��");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			strcpy(pyct_data->yct_serial, "00");
			CTools::byte_to_hex((BYTE *)rcvData+6, (BYTE *)pyct_data->yct_serial+2, 4);//���ͨ������ˮ��
			wrLog("���ͨ������ˮ��yct_serial [%s]", pyct_data->yct_serial);
			CTools::byte_to_hex((BYTE *)rcvData+6+4, (BYTE *)pyct_data->ticket_no, 8);//���ͨƱ������
			wrLog("���ͨƱ������ticket_no: [%s]", pyct_data->ticket_no);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);			
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
	
	wrLog("��3: commuWithYCTIC��");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 3, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			getAsiAmo(rcvData+6, 3, pyct_data->ticket_amo_bef); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
			wrLog("���ͨƱ�����ticket_amo:[%s]", pyct_data->ticket_amo_bef);//���ͨƱ�����
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			wrLog("��FAIL:4: commuWithYCTIC��");
			rcvLen = 0;
			bbzero(rcvData);
			iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 4, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
			if( iRet==0  )
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				closesocket(sd);			
				return 2;// 2�����������صĴ���
			}
			else if( iRet==1  )
			{
				*party = FROMCTSI;
				*rspcode = YCTTIMEOUT;
				closesocket(sd);
				return 3;// 3�������ͨ����ʧ��
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				closesocket(sd);
				return 1;// 1�������������ʧ��
			}
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);			
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
	
	if(enq==NORMALENQ)//ֻ��ѯ
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
	
	wrLog("��4: commuWithYCTIC��");
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
				return 2;// 2�����������صĴ���
			}
			else
			{
				*party = FROMIC_N;
				*rspcode = rcvData[5];
				closesocket(sd);
				return 9;// 9�����������صĴ�����Ҫ������������
			}
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 5;// 5�������ͨ����ʧ�ܣ���Ҫ������������
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 6�������������ʧ�ܣ���Ҫ������������
	}
	
	wrLog("��5: commuWithYCTIC��");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 5, NORMAL_PAY_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[12]==0x00 )
		{
			getAsiAmo(rcvData+6+10, 4, pyct_data->ticket_amo_aft); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
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
			return 7;// 7��û��ȷ�ϱ�ʶ����Ҫ������������
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICSAYFAIL;
			closesocket(sd);
			return 8;// 8��ȷ�ϳ�ֵʧ��
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 5;// 5�������ͨ����ʧ�ܣ���Ҫ������������
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 6�������������ʧ�ܣ���Ҫ������������
	}
}

//��������:������������հ��ͽ��
// ��Σ�flag:2:���Ѱ�;4:ȷ�ϰ�;5:������
// ���Σ���
// ����ֵ
// 0���ɹ�
// 1�����ʧ�ܣ��۷�ʧ��
// 2����ʱ�����δ֪
// 3�����ʧ�ܣ����δ֪
int CYCTYPTrade::packHdl(CGlobal gbl, CPack *ppack, int flag)
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

// ȷ�Ͻ���
// ��Σ�yct_data:���ͨ������Ϣ
// ���Σ���
// ����ֵ��
// 2����ȷ��
// 3��δȷ��
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
		wrLog("��*******PEȷ��*******��");
		//дȷ����
		pack.wrField(4, gbl);
		/************����48���63��*********/
		memcpy(pack.FIELD48,gbl.fee_type,4);	//4λ��Ϣ��
		bbzero(pack.FIELD48);
		b48_len = 4;
		memcpy(pack.FIELD48+b48_len,biz_status,2);
		b48_len+=2;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff,"%02d", 25);	//���ų���
		memcpy(pack.FIELD48+b48_len, lenBuff, 2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);//���ͨ�߼�����
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);//���PKI������
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
		//ȷ�ϰ����
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
			;//ȷ�ϲ��ɹ�����ȷ�ϣ�ֱ������ȷ�ϴ�����
		}
	}

	if (yct_data.status==1) //�ɹ���ȷ��
	{
		return 3;
	}
	else
		return 5;//ʧ�ܵ�ȷ��
	
	
}


// ��������
// ��Σ�yct_data:���ͨ������Ϣ
// ���Σ���
// ����ֵ��
// 5���ѳ���
// 6��δ����
int CYCTYPTrade::RevTrade(CGlobal gbl, YCT_DATA yct_data)
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
		memcpy(pack.FIELD48,"0056",4);	//4λ��Ϣ��
		b48_len = 4;
		memcpy(pack.FIELD48+b48_len, gbl.pos_serial, 6);	//����ʱ6λԭ�ն���ˮ��
		b48_len += 6;
		memset(lenBuff,0,sizeof(lenBuff));
		sprintf(lenBuff,"%02d", 25);	//���ų���
		memcpy(pack.FIELD48+b48_len,lenBuff,2);	
		b48_len += 2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);//���ͨ�߼�����
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);//���PKI������
		b48_len += 8;
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
		else if (iRet == 2)
		{
			//��ʱ
			return 6;
		}
	}
	return 6;// 6��δ����
}

// ��������
// ��Σ�payType:��ֵ����
// ���Σ�pyct_detail:��ֵ��ϸ
// ����ֵ��
// 0���ɹ�
// 1���޴˳�ֵ����
// 2�����ն˽���ʧ��
// 0x36 CANCLE_KEY���û�ȡ��
int CYCTYPTrade::MonthSelectWay(const TSDLLPosInfo dPosInfo, int payType, PYCT_DETAIL pyct_detail)
{
	int iRet=0, iSelect=0;
	TSDLLPosInfo m_dPosInfo;
	TSMENULIST menuList;
	TSTradeInfoSend tradeInfoSend;
	TSTradeInfoRecv tradeInfoRecv;
	if( payType==3 )//����90�α�����Ч
	{
		pyct_detail->ticket_type = 0x01;
		pyct_detail->birth_date = 0x01;
		pyct_detail->bus_type = 0x01;
		pyct_detail->bus_num = 90;
		return 0;
	}
	else if( payType==4 )//����90��������Ч
	{
		pyct_detail->ticket_type = 0x01;
		pyct_detail->birth_date = 0x02;
		pyct_detail->bus_type = 0x01;
		pyct_detail->bus_num = 90;
		return 0;
	}
	else if( payType==5 )//��������
	{
		//������
		memset(&tradeInfoSend, 0, sizeof(tradeInfoSend));
		memset(&tradeInfoRecv, 0, sizeof(tradeInfoRecv));
		
		strcpy(tradeInfoSend.acMainTitle, "��������");
		tradeInfoSend.uHasTradeID = 1;
		tradeInfoSend.uTotalContents = 2;
		
		strcpy(tradeInfoSend.Contents[0].acTitle, "�������1��������·");
		tradeInfoSend.Contents[0].uCommand = TS_INPUT_ALL_CHAR;//������·
		tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME;//��ʱ30��
		tradeInfoSend.Contents[0].uCount = 0;//����1��
		tradeInfoSend.Contents[0].uMinLen = 1;
		tradeInfoSend.Contents[0].uMaxLen = 5;
		
		strcpy(tradeInfoSend.Contents[1].acTitle, "�������2��������·");
		tradeInfoSend.Contents[1].uCommand = TS_INPUT_ALL_CHAR;//������·
		tradeInfoSend.Contents[1].uTimeOut = EPOS_OVER_TIME;//��ʱ30��
		tradeInfoSend.Contents[1].uCount = 0;//����1��
		tradeInfoSend.Contents[1].uMinLen = 1;
		tradeInfoSend.Contents[1].uMaxLen = 5;
		
		memcpy(&m_dPosInfo, &dPosInfo, sizeof(m_dPosInfo));
		iRet = TS_TradeControl(&m_dPosInfo, &tradeInfoSend, &tradeInfoRecv);		
		if( iRet!=0 || tradeInfoRecv.Contents[0].uMsgLen>5 || tradeInfoRecv.Contents[1].uMsgLen>5 )
		{			
			return 2;// 2�����ն˽���ʧ��
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
		strcpy(menuList.acTitle, "��������");
		
		menuList.iTotalNum = 2;
		strcpy(menuList.menuItem[0], "������Ч");
		strcpy(menuList.menuItem[1], "������Ч");		
		iRet = TS_Menu(dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return 2;// 2�����ն˽���ʧ��
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
			return CANCLE_KEY;// 0x36 �û�ȡ��
		}
		return 0;
	}
	else if( payType==6 )//�����޴�
	{
		pyct_detail->ticket_type = 0x02;
		pyct_detail->railway_type = 0x01;
		
		memset(&menuList,0,sizeof(menuList));
		strcpy(menuList.acTitle, "�����޴�");
		
		menuList.iTotalNum = 3;
		strcpy(menuList.menuItem[0], "20��");
		strcpy(menuList.menuItem[1], "35��");
		strcpy(menuList.menuItem[2], "50��");
		iRet = TS_Menu(dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return 2;// 2�����ն˽���ʧ��
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
			return CANCLE_KEY;// 0x36 �û�ȡ��
		}
		
		memset(&menuList,0,sizeof(menuList));
		strcpy(menuList.acTitle, "�����޴�");
		
		menuList.iTotalNum = 2;
		strcpy(menuList.menuItem[0], "������Ч");
		strcpy(menuList.menuItem[1], "������Ч");		
		iRet = TS_Menu(dPosInfo, &menuList, iSelect, MENU_TIME_OUT);
		if(iRet==HOTKEY)
		{
			return CANCLE_KEY;
		}
		else if(iRet!=0)
		{
			return 2;// 2�����ն˽���ʧ��
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
			return CANCLE_KEY;// 0x36 �û�ȡ��
		}
		return 0;
	}
	else
	{
		return 1;
	}	
}

// ��ƱǮ����������
// ��Σ���
// ���Σ���
// ����ֵ��
// 0���ɹ���ȷ��
// 1���ɹ�δȷ��
// 2��ʧ���ѳ���
// 3��ʧ��δ����
// 4��δ֪���
// 5���۷�ǰʧ��
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
		return 5;// 5���۷�ǰʧ��
	}
	
	memset(&yct_data, 0, sizeof(yct_data));
	memset(&yct_detail, 0, sizeof(yct_detail));
	
	//ѡ�ײ�
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
		return 5;// 5���۷�ǰʧ��
	}
	
	//��ѯ��Ʊ����ǰ��Ϣ	
	iRet = MonthEnq(pgbl->dPosInfo, com, comType, yct_detail.ticket_type, yct_data.yct_serial, pgbl->amount, pki_card, signin_serial, &m_party, &m_rspcode);
	if( iRet==0 )//��ѯ�ɹ�
	{
		NULL;
	}
	else
	{
		*party = m_party;
		*rspcode = m_rspcode;
		return 5;// 5���۷�ǰʧ��
	}
	
	//ȷ��·��
	iRet = MonthPay1_2(pgbl->dPosInfo, com, comType, yct_detail, yct_data.yct_serial, pgbl->amount, pki_card, signin_serial, &m_party, &m_rspcode, m_priCont);
	if( iRet==0 )//ȷ�ϳ�ֵ
	{
		//ˢ��������PE�۷�
		iRet = PayPe(pgbl, yct_data, &m_party, &m_rspcode);
		if( iRet==0 )//�ɹ�
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
			//��¼���ͨ��������
			CDB::setYctYPData(yct_data);
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5���۷�ǰʧ��
		}
		
		//��ֵ
		iRet = MonthPay3_4(pgbl->dPosInfo, com, comType, &yct_data, &m_party, &m_rspcode);
		wrYctDataLog("after MonthPay3_4", yct_data);
		
		strcat(priCont, "���ͨ���ţ�");
		strcat(priCont, yct_data.ticket_no);
		strcat(priCont, "\n");
		strcat(priCont, m_priCont);
		
		if( iRet==0 )
		{
			*party = FROMSUCC;
			*rspcode = SUCC_MSG;
			yct_data.status = 1;//��ֵ�ɹ�δȷ��
		}
		else if( iRet==1 || iRet==2 || iRet==3 || iRet==7 )
		{
			*party = m_party;
			*rspcode = m_rspcode;
			yct_data.status = 4;//��ֵʧ��δ����
		}
		else//��Ҫ������������		
		{
			iRet = MonthNotFinish(pgbl->dPosInfo, com, comType, signin_serial, yct_data.yct_serial, yct_data.pki_card, yct_data.ticket_amo_aft, &m_party, &m_rspcode);
			wrYctDataLog("after MonthNotFinish", yct_data);
			if(iRet==0)//�ɹ�
			{
				*party = FROMSUCC;
				*rspcode = SUCC_MSG;
				yct_data.status = 1;//��ֵ�ɹ�δȷ��
				
			}
			else if(iRet==5)//ʧ��
			{
				*party = m_party;
				*rspcode = m_rspcode;
				yct_data.status = 4;//��ֵʧ��δ����
			}
			else//δ֪
			{
				yct_data.status = 0;//�ѿ۷ѽ��δ֪
				*party = FROMCTSI_N;
				*rspcode = PAYBUTNOTFINISH;
				//�������ݿ�
				CDB::updateYctYPData(yct_data);	
				return 4;// 4��δ֪���
			}
		}
		//PEȷ�������
		yct_data.status = AckRevTrade(*pgbl, yct_data);
		//�������ݿ�
		CDB::updateYctYPData(yct_data);		
		return 0;
	}
	else
	{
		*party = m_party;
		*rspcode = m_rspcode;
		return 5;// 5���۷�ǰʧ��
	}
}

// ��ƱǮ����ǩ��
// ��Σ���
// ���Σ�pki_card(8):PKI�����ţ�signin_serial(16):������ˮ��
// ����ֵ
// 0���ɹ�
// 1�������������ʧ�ܣ�ǩ��ʧ��
// 2�����������صĴ���ǩ��ʧ��
// 3�������ͨ����ʧ�ܣ�ǩ��ʧ��
// 4������ǩ��ʧ��
int CYCTYPTrade::MonthSignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	SOCKET sd=0;
	CConnect con;//������
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1], signin_serial_this[16+1];
	
	wrLog("��MonthSignIn��");
	
	iRet = connTest(dPosInfo, com, comType);//���Ӳ��ԣ�ͬʱҲ�Ͽ��ϱʽ��ף���ʼ��������
	if( iRet==0 )//�ɹ�
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = CONNCOMFAIL;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	wrLog("��1: commuWithIC��");
	bbzero(icData);
	icLen = 0;
	len = icLen + 2;
	
	bbzero(sndData);	
	sndLen = packData(TAG, VER, 1, len, MONTH_SIGNIN_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			bbzero(pki_card_this);
			CTools::byte_to_hex(rcvData+6, (BYTE*)pki_card_this, 4);//�����߼�����
			wrLog("�¹����߼�����pki_card_this [%s] �ɹ����߼�����pki_card [%s]", pki_card_this, pki_card);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	
	wrLog("��2: commuWithYCTIC��");
	rcvLen = 0;
	bbzero(rcvData);
	iRet = commuWithYCTIC(dPosInfo, com, comType, sd, 2, MONTH_SIGNIN_CMD, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			bbzero(signin_serial_this);
			CTools::byte_to_hex(rcvData+6, signin_serial_this, 8);
			wrLog("��������ˮ [%s] ��������ˮpki_card [%s]", signin_serial_this, signin_serial);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	wrLog("��3: commuWithYCTIC��");
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
				return 0;// 0���ɹ�
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = SIGNINFAIL;
				closesocket(sd);
				return 4;// 4������ǩ��ʧ��
			}
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ��󣬽��δ֪
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ�ܣ����δ֪
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
}

// ��Ʊ��ѯ
// ��Σ�pki_card:PKI������; signin_serial(16):������ˮ��
// ���Σ�pki_card:PKI������; signin_serial(16):������ˮ��, 
// ����ֵ
// 0���ɹ�
// 1�������������ʧ��
// 2�����������صĴ���
// 3�������ͨ����ʧ��
// 4�����ͨ���صĴ���
// 5��ǩ��ʧ��
// 6���û�ȡ��
int CYCTYPTrade::MonthEnq(const TSDLLPosInfo dPosInfo, const int com, const int comType, char ticket_type, char *yct_serial, char *amount, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, i=0, iSelect=0, msgLen=0;
	SOCKET sd=0;
	CConnect con;//������
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	char msg[1024];
	bbzero(msg);
	
	wrLog("��MonthEnq��");
	
	if( memcmp(signin_serial, "0000000000000000", 16)==0 )
	{
		iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
		if( iRet==0 )//�ɹ�
		{
			NULL;
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5��ǩ��ʧ��
		}
	}
	else
	{
		NULL;
	}
	
	for(i=0; i<2; i++)//ѭ��2��
	{
		iRet = connTest(dPosInfo, com, comType);//���Ӳ��ԣ�ͬʱҲ�Ͽ��ϱʽ��ף���ʼ��������
		if( iRet==0 )//�ɹ�
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 1;// 1�������������ʧ�ܣ����δ֪
		}
		
		wrLog("��1: commuWithIC��");
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
		iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
		if( iRet==0 )
		{
			if( rcvData[4]==0x00 && rcvData[5]==0x00 )
			{
				CTools::byte_to_hex(rcvData+6+8, (BYTE*)pki_card_this, 4);//�����߼�����
				wrLog("�����߼�����pki_card_this [%s]", pki_card_this);
				if( memcmp(pki_card_this, pki_card, 8)==0 )
				{
					sndLen = rcvLen-6-20-1;
					memcpy(sndData, rcvData+6+20, sndLen);
					break;
				}
				else
				{
					iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
					if( iRet==0 )//�ɹ�
					{
						continue;
					}
					else
					{
						*party = m_party;
						*rspcode = m_rspcode;
						return 5;// 5��ǩ��ʧ��
					}
				}			
			}
			else if( rcvData[4]==0xA1 && rcvData[5]==0x36 )
			{
				iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
				if( iRet==0 )//�ɹ�
				{
					continue;
				}
				else
				{
					*party = m_party;
					*rspcode = m_rspcode;
					return 5;// 5��ǩ��ʧ��
				}
			}
			else
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				return 2;// 2�����������صĴ���
			}
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			return 1;// 1�������������ʧ��
		}
	}
	if( i>=2 )
	{
		*party = FROMCTSI;
		*rspcode = FATALERR;
		return 1;// 1�������������ʧ��
	}
	else
	{
		NULL;
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	wrLog("��2: commuWithYCTIC��");
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
				strcat(msg, "��Ʊ��ǰ��Ϣ�����˿�\n���ţ�");
			}
			else
			{
				strcat(msg, "��Ʊ��ǰ��Ϣ��ѧ����\n���ţ�");
			}
			CTools::byte_to_hex(rcvData+6+12, (BYTE *)msg+strlen(msg), 8);
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;//���ͨ���صĴ���
			*rspcode = rcvData[6];
			closesocket(sd);
			return 4;// 4�����ͨ���صĴ���
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 7;// 7���������ͨ������ʧ��
	}
	
	wrLog("��3: commuWithYCTIC��");
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
			strcat(msg, "��ȷ�ϡ�����\n�����ء��˳�");
			iRet = TS_OutPutMsg(dPosInfo, msg, 0, iSelect);
			if( iRet==0 && iSelect==0 )
			{
				*party = FROMSUCC;
				*rspcode = SUCC_NOMSG;
				closesocket(sd);
				return 0;// 0���ɹ�
			}
			else if( iRet==0 )
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				closesocket(sd);
				return 6;// 6���û�ȡ��
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				closesocket(sd);
				return 1;// 1�������������ʧ��
			}
		}
		else if( rcvData[4]==0xA1 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;//���ͨ���صĴ���
			*rspcode = rcvData[6];
			closesocket(sd);
			return 4;// 4�����ͨ���صĴ���
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
}

// ��Ʊ��ֵ��ǰ1��2��
// ��Σ�pki_card:PKI������; signin_serial(16):������ˮ��
// ���Σ�pyct_data:���ͨ������Ϣ; pki_card:PKI������; signin_serial(16):������ˮ��
// ����ֵ
// 0���ɹ�
// 1�������������ʧ��
// 2�����������صĴ���
// 3�������ͨ����ʧ��
// 4�����ͨ���صĴ���
// 5��ǩ��ʧ��
// 6���û�ȡ��
int CYCTYPTrade::MonthPay1_2(const TSDLLPosInfo dPosInfo, const int com, const int comType, YCT_DETAIL yct_detail, char *yct_serial, char *amount, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, i=0, iSelect=0;
	SOCKET sd=0;
	CConnect con;//������
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1];
	char date6[6+1], date10[10+1], numBuf[8+1];
	char msg[1024];
	bbzero(msg);
	
	wrLog("��MonthPay1_2��");
	
	if( memcmp(signin_serial, "0000000000000000", 16)==0 )
	{
		iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
		if( iRet==0 )//�ɹ�
		{
			NULL;
		}
		else
		{
			*party = m_party;
			*rspcode = m_rspcode;
			return 5;// 5��ǩ��ʧ��
		}
	}
	else
	{
		NULL;
	}
	
	for(i=0; i<2; i++)//ѭ��2��
	{
		iRet = connTest(dPosInfo, com, comType);//���Ӳ��ԣ�ͬʱҲ�Ͽ��ϱʽ��ף���ʼ��������
		if( iRet==0 )//�ɹ�
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = CONNCOMFAIL;
			return 1;// 1�������������ʧ�ܣ����δ֪
		}
		
		wrLog("��1: commuWithIC��");
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
		iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
		if( iRet==0 )
		{
			if( rcvData[4]==0x00 && rcvData[5]==0x00 )
			{
				CTools::byte_to_hex(rcvData+6+8, (BYTE*)pki_card_this, 4);//�����߼�����
			//	wrLog("�����߼�����pki_card_this [%s]", pki_card_this);
				if( memcmp(pki_card_this, pki_card, 8)==0 )
				{
					sndLen = rcvLen-6-20-1;
					memcpy(sndData, rcvData+6+20, sndLen);
					break;
				}
				else
				{
					iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
					if( iRet==0 )//�ɹ�
					{
						continue;
					}
					else
					{
						*party = m_party;
						*rspcode = m_rspcode;
						return 5;// 5��ǩ��ʧ��
					}
				}			
			}
			else if( rcvData[4]==0xA1 && rcvData[5]==0x36 )
			{
				iRet = MonthSignIn(dPosInfo, com, comType, pki_card, signin_serial, &m_party, &m_rspcode);
				if( iRet==0 )//�ɹ�
				{
					continue;
				}
				else
				{
					*party = m_party;
					*rspcode = m_rspcode;
					return 5;// 5��ǩ��ʧ��
				}
			}
			else
			{
				*party = FROMIC;
				*rspcode = rcvData[5];
				return 2;// 2�����������صĴ���
			}
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			return 1;// 1�������������ʧ��
		}
	}
	if( i>=2 )
	{
		*party = FROMCTSI;
		*rspcode = FATALERR;
		return 1;// 1�������������ʧ��
	}
	else
	{
		NULL;
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	wrLog("��2: commuWithYCTIC��");
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
				strcat(msg, "���γ�ֵ��Ϣ�����˿�");
				strcat(priCont, "�����ͳ��˿�");
			}
			else
			{
				strcat(msg, "���γ�ֵ��Ϣ��ѧ����");
				strcat(priCont, "������ѧ����");
			}
			strcat(msg, "\n��ʼ����: ");
			strcat(priCont, " ��ʼ����:");
			
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
			sprintf(msg+strlen(msg), "\n��ֵ���: %dԪ", atoi(amount)/100);
			sprintf(priCont+strlen(priCont), "\n��ֵ���: %dԪ", atoi(amount)/100);
			
			bbzero(numBuf);
			CTools::byte_to_hex(rcvData+23, (BYTE *)numBuf, 4);
			sprintf(msg+strlen(msg), "\n����: %d��", atoi(numBuf));
			sprintf(priCont+strlen(priCont), " ����: %d��", atoi(numBuf));
			
			strcat(msg, "\n·������1: ");
			strcat(priCont, "\n·������1: ");
			strcat(msg, (char *)rcvData+6+21);
			strcat(priCont, (char *)rcvData+6+21);
			strcat(msg, "\n·������2: ");
			strcat(priCont, "\n·������2: ");
			strcat(msg, (char *)rcvData+6+41);
			strcat(priCont, (char *)rcvData+6+41);
			
			strcat(msg, "\n\n��ȷ�ϡ���ֵ\n�����ء��˳�");
			
			iRet = TS_OutPutMsg(dPosInfo, msg, 0, iSelect);
			if( iRet==0 && iSelect==0 )
			{
				*party = FROMSUCC;
				*rspcode = SUCC_NOMSG;
				closesocket(sd);
				return 0;// 0���ɹ�
			}
			else if( iRet==0 )
			{
				*party = FROMCTSI;
				*rspcode = USERCANCEL;
				closesocket(sd);
				
				wrLog("��3: cancel commuWithIC��");
				bbzero(icData);
				icData[0] = 0x02;
				icLen = 20;
				len = icLen + 2;
				
				bbzero(sndData);
				sndLen = packData(TAG, VER, 3*2-1, len, MONTH_PAY_CMD, STAT, icData, sndData);
				rcvLen = 0;
				bbzero(rcvData);
				iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
				
				return 6;// 6���û�ȡ��
			}
			else
			{
				*party = FROMCTSI;
				*rspcode = ICTIMEOUT;
				closesocket(sd);
				return 1;// 1�������������ʧ��
			}
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;//���ͨ���صĴ���
			*rspcode = rcvData[6];
			closesocket(sd);
			return 4;// 4�����ͨ���صĴ���
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
}

// ��Ʊ��ֵ��ǰ3��4��
// ��Σ���
// ���Σ�pyct_data:���ͨ������Ϣ
// ����ֵ
// 0���ɹ�
// 1�������������ʧ��
// 2�����������صĴ���
// 3�����ͨ���صĴ���
// 4�������ͨ����ʧ�ܣ���Ҫ���ָ�
// 5�������������ʧ�ܣ���Ҫ���ָ�
// 6�����������صĴ�����Ҫ���ָ�
// 7���������ͨ������ʧ��
int CYCTYPTrade::MonthPay3_4(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, i=0, iSelect=0;
	SOCKET sd=0;
	CConnect con;//������
	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;
	BYTE icData[256], sndData[256], rcvData[256];
	
	wrLog("��MonthPay3_4��");
	
	wrLog("��3: commuWithIC��");
	bbzero(icData);
	icData[0] = 0x01;
	icLen = 20;
	len = icLen + 2;
	
	bbzero(sndData);
	sndLen = packData(TAG, VER, 3*2-1, len, MONTH_PAY_CMD, STAT, icData, sndData);
	rcvLen = 0;
	bbzero(rcvData);
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0 )
	{
		if( rcvData[4]==0x00 && rcvData[5]==0x00 )
		{
			CTools::byte_to_hex(rcvData+6, (BYTE*)pyct_data->ticket_no, 8);//���ͨƱ������
			getAsiAmo(rcvData+6+8, 4, pyct_data->ticket_amo_bef);
			sndLen = rcvLen-6-20-1;
			memcpy(sndData, rcvData+6+20, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[5];
			return 2;//2�����������صĴ���
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ��
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCTYP_IP, YCTYP_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 7;// 7���������ͨ������ʧ��
	}
	
	wrLog("��4: commuWithYCTIC��");
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
			return 0;// 0���ɹ�
		}
		else if( rcvData[4]==0x00 && rcvData[5]==0x01 )
		{
			*party = FROMYCT;
			*rspcode = rcvData[6];
			closesocket(sd);
			return 3;// 3�����ͨ���صĴ���
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[5];
			closesocket(sd);
			return 6;// 6�����������صĴ�����Ҫ���ָ�
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 4;// 4�������ͨ����ʧ�ܣ���Ҫ���ָ�
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 5;// 5�������������ʧ�ܣ���Ҫ���ָ�
	}
}

// ˢ����������PE���۷Ѱ�
// ��Σ�biz_type(4):ҵ����룻amount(12):���
// ���Σ���
// ����ֵ
// 0���۷ѳɹ�
// 1���۷�ʧ��
// 2����ʱ����
int CYCTYPTrade::PayPe(CGlobal *pgbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode)
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
		trade.get_gbl(*pgbl);	//ȡ����trade���л�õ���Ϣ
		pack.wrField(2, *pgbl);	//д������
		/***********�޸�һЩ����Ϣ*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,pgbl->fee_type,4);	//4λ��Ϣ��
		b48_len = 4;
		memset(len_buff,0,sizeof(len_buff));
		sprintf(len_buff,"%02d", 25);	//���ų���
		memcpy(pack.FIELD48+b48_len,len_buff,2);	
		b48_len +=2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);	//���ͨ�߼�����
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);	//���ͨ������
		b48_len += 8;
		pack.Packet_Table[48].Real_Length=b48_len;
		/**********************************/
		
		//���Ѱ�����
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
			if(trade.reGetPwd( "���ͨ��ֵ" ) == 0) //�������������������
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
			TS_OutPutMsg(pgbl->dPosInfo,"\n     ��֧�ֵĿ�\n   �뻻������ˢ��", 0, iSelect);
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


//���캯��
// ˢ����������PE���۷Ѱ�
// ��Σ�biz_type(4):ҵ����룻amount(12):���
// ���Σ���
// ����ֵ
// 0���۷ѳɹ�
// 1���۷�ʧ��
// 2����ʱ����
int CYCTYPTrade::PayPe(CGlobal gbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode)
{
	int  iRet, flag,mngCard_count=1,iSelect;
	int b48_len = 0;
	char len_buff[10];
	CPack pack;
	CTrade trade;
	
	wrLog("��*******PE�۷�*******��");	
	
	//������Ϣ,ȡ��ˢ������
	flag = 0;	//���������ʶ
	/********************����ˢ��****************/
	mngCard_count=1;
	trade.set_in_par(gbl);
	for(int maxTry=0; maxTry<50; maxTry++)	//������������
	{
		if(!flag)
		{
			iRet = trade.mngCard_Hdl("���ͨ��ֵ");
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

		pack.wrField(2, gbl);	//д������
		/***********�޸�һЩ����Ϣ*********/
		bbzero(pack.FIELD48);
		memcpy(pack.FIELD48,gbl.fee_type,4);	//4λ��Ϣ��
		b48_len = 4;
		memset(len_buff,0,sizeof(len_buff));
		sprintf(len_buff,"%02d", 25);	//���ų���
		memcpy(pack.FIELD48+b48_len,len_buff,2);	
		b48_len +=2;
		memcpy(pack.FIELD48+b48_len, yct_data.ticket_no, 16);	//���ͨ�߼�����
		b48_len += 16;
		memcpy(pack.FIELD48+b48_len, "|", 1);
		b48_len += 1;
		memcpy(pack.FIELD48+b48_len, yct_data.pki_card, 8);	//���ͨ������
		b48_len += 8;
		pack.Packet_Table[48].Real_Length=b48_len;
		/**********************************/
		
		//���Ѱ�����
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
			if(trade.reGetPwd( "���ͨ��ֵ" ) == 0) //�������������������
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
			TS_OutPutMsg(gbl.dPosInfo,"\n     ��֧�ֵĿ�\n   �뻻������ˢ��", 0, iSelect);
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


//����pin
int CYCTYPTrade::yctSetting(char *pinValue)
{
	char sndData[256], rcvData[256];
	int iSelect;
	int sndLen, rcvLen;
	int stat, len, num, cmd;
	
	wrLog("������pin��");
	num = 0;
	cmd = 0x11;
	stat = 0x00;
	len = 33;
	bbzero(sndData);
	sndLen = packData(TAG, VER, num, len, cmd, stat, (BYTE *)pinValue, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	SndRcvIcData(gbl.dPosInfo,m_com,m_comType,"", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen);
	if (rcvData[4] == 0x00 && rcvData[5] == 0x00)
	{
		TS_OutPutMsg(gbl.dPosInfo, " \n\n     ���óɹ�!", 1, iSelect);
	}
	else
	{
		TS_OutPutMsg(gbl.dPosInfo, " \n\n     ���ó���!", 1, iSelect);
		return -1;
	}	
	return 0;
}

void CYCTYPTrade::wrYctDataLog(char *msg, YCT_DATA yct_data)
{
	char ticketno[16+1];
	bbzero(ticketno);
	CTools::shildWords(yct_data.ticket_no,7,5,ticketno);

	wrLog("��YctData����%s��", msg);
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
			TS_OutPutMsg(gbl.dPosInfo, "���ͨ��ֵ�ɹ�\n�밴��ȷ�ϡ���ӡƾ֤", 0, iSelect);
			break;
		case SUCC_NOMSG:
			break;
		case SUCC_MSG_LAST:
			TS_OutPutMsg(gbl.dPosInfo, "��һ�����ͨ��ֵ�ɹ�", 0, iSelect);
			break;
		default:
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888!\n", party, rspcode);
			TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
			break;
		}
	}
	else if( party==FROMCTSI || party==FROMCTSI_N )
	{
		switch(rspcode)
		{
		case 0x01:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳδ�ܷ����ж����������ã�����δ��ͨ����ҵ�񣬿�ֱ������[861]���뿪ͨ��\n");
			break;
		case 0x02:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "���ͨ��ֵ��ȡ��\n");
			return 0;
			break;
		case 0x03:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�۷ѳ�ʱ\n");
			break;
		case 0x04:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����������ͽ������ݳ�ʱ\n");
			break;
		case 0x05:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ�����ж�������������ȷ���������Ƿ���ȷ���ӣ�Ȼ��ѡ��[����]�����²�����\n");
			break;
		case 0x06:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ͨ���ͽ������ݳ�ʱ\n");
			break;
		case 0x07:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����������ȷ������״̬\n");
			break;
		case 0x08:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "������ָ������ʧ��\n");
			break;
		case 0x09:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ���������\n");
			break;
		case 0x0A:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ���������\n");
			break;
		case 0x0B:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��֧�ֵĿ�\n");
			break;
		case 0x0C:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "PKI���Ѹ������޷���ȡ�ϱʽ��׽��\n");
			break;
		case 0x0D:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "������ȡ�ϱʽ��׽����ȷ�ϳ�ֵʧ��\n");
			break;
		case 0x0E:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ѿ۷ѣ�����ֵ���δ֪\n");
			break;
		case 0x0F:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ǩ������\n");
			break;
		default:
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMCTSI )
		{
			strcat(msg, "���ͨ��ֵʧ��\n");
		}
		else
		{
			strcat(msg, "���ͨ��ֵ���δ֪\n");
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
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����������д��\n");
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
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
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
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��źÿ�\n");
			break;
		case 0x71:
		case 0x72:
		case 0x74:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ȷ�Ŀ�\n");
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
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ܾ����ף��򻻿�\n");
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
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��źÿ�\n");
			break;
		case 0x8B:
		case 0x8C:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����������ٴβ���\n");
			break;
		case 0x93:
		case 0x94:
		case 0x95:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0xC1:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ͨƱ��\n");
			break;
		case 0xC2:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "Ʊ������\n");
			break;
		case 0xC4:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "Ʊ��״̬�쳣���ܳ�ֵ\n");
			break;
		case 0xC5:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ�����������\n");
			break;
		case 0xC6:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ����Ʊ�����\n");
			break;
		case 0xC7:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ���Υ��ҵ�����\n");
			break;
		case 0xC8:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������������\n");
			break;
		case 0xC9:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "û�ж�Ӧ�ĳ�ֵ��¼\n");
			break;
		case 0xCA:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ��ˮ����ϵͳ��¼��һ��\n");
			break;
		case 0xCB:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ȷ�ϳɹ��Ľ��׼�¼�������޸�Ϊʧ��\n");
			break;
		case 0xCC:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ȷ��ʧ�ܵĽ��׼�¼�������޸�Ϊ�ɹ�\n");
			break;
		case 0xCD:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ȷ�ϳɹ�\n");
			break;
		case 0xCE:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ȷ��ʧ��\n");
			break;
		case 0xCF:
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "���ͨ���صĽ�������Ϲ淶\n");
			break;
		default:
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMIC )
		{
			strcat(msg, "���ͨ��ֵʧ��\n");
		}
		else
		{
			strcat(msg, "���ͨ��ֵ���δ֪\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}
	else if( party==FROMYCT || party==FROMYCT_N )
	{
		switch(rspcode)
		{
		case 0x00:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ�ɹ�\n");
			break;
		case 0xFF:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ����Ҫ������\n");
			break;
		case 0x01:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "������\n");
			break;
		case 0x02:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ˮ�Ŵ�\n");
			break;
		case 0x03:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�׶δ���\n");
			break;
		case 0x11:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��Ʊ���Ͳ���\n");
			break;
		case 0x12:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����Ͳ���\n");
			break;
		case 0x13:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����������\n");
			break;
		case 0x14:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ն˽��û�ISAM_IDΪ��\n");
			break;
		case 0x15:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����Ų�����\n");
			break;
		case 0x16:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x17:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "Ʊ��������Ч��\n");
			break;
		case 0x20:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ն����һ�ʼ�¼\n");
			break;
		case 0x21:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "û�и�����������Ʊ�ײ�\n");
			break;
		case 0x22:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��·�Ŵ���\n");
			break;
		case 0x2F:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x30:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����ͷ��\n");
			break;
		case 0x31:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��Ƭ��һ��\n");
			break;
		case 0x32:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ˮ�Ų�һ��\n");
			break;
		case 0x33:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "MAC��\n");
			break;
		case 0x3F:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x41:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "���ױ���û�н�����ѯ����\n");
			break;
		case 0x42:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��γ��ִ���\n");
			break;
		case 0x43:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ЧƱ��\n");
			break;
		case 0x44:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����ԭ��������Ϣ������\n");
			break;
		case 0x45:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ظ���ֵ\n");
			break;
		case 0x46:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ������������\n");
			break;
		case 0x47:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ظ���ֵ\n");
			break;
		case 0x48:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "���ڷ���ʱ��\n");
			break;
		case 0x49:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x50:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ʺű�����\n");
			break;
		case 0x51:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ն�ÿ���޶�\n");
			break;
		case 0x52:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ʺ�����\n");
			break;
		case 0x53:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵǰ�������Ч\n");
			break;
		case 0x54:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "Ʊ�����ں�Ҫ�������ڲ���Ӧ\n");
			break;
		case 0x55:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ն����һ�ʼ�¼\n");
			break;
		case 0x5F:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0xA1:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��Ʊ��ֵ������ϵͳ����Ҫ������\n");
			break;
		case 0x99:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ����\n");
			break;
		default:			
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMYCT )
		{
			strcat(msg, "���ͨ��ֵʧ��\n");
		}
		else
		{
			strcat(msg, "���ͨ��ֵ���δ֪\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}	
	else
	{
		sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}
	return 0;
}

// ����Ƽ��¶������汾
// ��ͨǮ���Ĳ�ѯ��ֵ
// ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������; enq:0:Ҫ��ֵ; 1:ֻ��ѯ
// ���Σ�pyct_data:���ͨ������Ϣ; errType:��������0:�ɹ�1:���������Ĵ���; errCode:�������0:�ɹ�
// ����ֵ
// 0���ɹ�(��ѯ���ֵ)
// 1�������������ʧ��
// 2�����������صĴ���
// 3�������ͨ����ʧ��
// 4�����ͨ���صĴ���
// 5�������ͨ����ʧ�ܣ���Ҫ������������
// 6�������������ʧ�ܣ���Ҫ������������
// 7��û��ȷ�ϱ�ʶ����Ҫ������������
// 8��ȷ�ϳ�ֵʧ��
// 9�����������صĴ�����Ҫ������������

int CYCTYPTrade::ST_NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	int iSelect;
	BYTE term_id[7+1];
	char term_id_tmp[14+1];
	BYTE term_time[7];
	BYTE SK[32+1]; //�ػ���Կ
	
	BYTE pysical_card[8+1]; //������
	BYTE logical_card[8+1]; //�߼�����
	char term_time_tmp[14];
	BYTE card_pycicalInfo1[32+1];//��Ƭ������Ϣ
	BYTE SW1SW2[4+1];
	int status=0;
	BYTE SAK[1+1];
	BYTE card_physics_info[51];
	SOCKET sd=0;
	CConnect con;//������
	int baudRate = 57600;
	//�����еĲ�����
	BYTE type [2] ; //��Ϣ������
	BYTE ver [2]; //���İ汾��
	BYTE RFU [34];
	BYTE rsp_code[8+1];
	int trade_type = 0;
	BYTE rsp_message[48];
	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE R_CPU_LOAD_STATUS[1+1];
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //�������
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
	bbzero(RFU); //RFU ����
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
	bbzero(query_info);
	
	//BYTE payType='0x00';
	//��ȡ��Ƭ��������Ϣ,���ؿ�Ƭ��������Ϣ
	iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
	

	if (iRet == 1)
	{
		TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		for (int i = 0; i<2; i++)
		{
			iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
			if (iRet == 0)
			{
				break;
			}
			TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		}

	}
//	if (iRet == -1)
//	{
//		*party = FROMCTSI;
//		*rspcode = CARDNOTFOUND;
//		closesocket(sd);
//		return 1;// 1�������������ʧ��
//	}
	if(iRet==0)
	{
		CTools::byte_to_hex((BYTE *)card_physics_info,(BYTE *)pyct_data->ticket_physicalNo,8);//��ȡ������
		CTools::byte_to_hex((BYTE *)card_physics_info+8,(BYTE *)pyct_data->ticket_no,8);//ȡ���߼�����
		memcpy(SAK,card_physics_info+16,1) ;//SAK
		memcpy(card_pycicalInfo1,card_physics_info+17,32); //��Ƭ������Ϣ1
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
			sprintf(outPut_Msg,"������Ӧ��״̬�����%s",errMsg);
			TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1�������������ʧ��
		}
		
		wrLog("������:%s",pyct_data->ticket_physicalNo);
	//	wrLog("�߼�����:%s",pyct_data->ticket_no);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32);
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);	
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = CARDNOTFOUND;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}

	
	
	//���ý��ײ�ѯP_TRAN_GRY ��ƽ̨��������
		wrLog("��P_TRAN_QRY��");
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
						wrLog("M1Ǯ��(��ѯ��Ȧ��ͳ���)");
						
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
						wrLog("M1�û��˻���ֵ");
						break;
					}
				case (0x20):
					{
						wrLog("CPUǮ����ֵ����ѯ��Ȧ��ͳ�����");			
						iRet = CPU_EnqPay(dPosInfo,com,comType,enq,pyct_data,amount,0x00,query_info,party,rspcode);
						M1_CPU = 2;
						break;
					}
				case(0x21):
					{
						wrLog("OC3.CPUȦ��(M1���ת��)");
						change = 1;
//					    iRet =	CPU_EnqPay(dPosInfo,com,comType,enq,pyct_data,"000000000000",0x10,query_info,party,rspcode);
//						R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //��Ӧ��ͬ��
						iRet =	CPU_EnqPay(dPosInfo,com,comType,0,pyct_data,"000000000000",0x10,query_info,party,rspcode);
						bbzero(pyct_data->ticket_amo_bef);
						strcpy(pyct_data->ticket_amo_bef,pyct_data->ticket_amo_aft);
						bbzero(pyct_data->ticket_amo_aft);
						wrLog("��Ƭͬ�������ת���Ѿ����,�������ֵ");
						TS_OutPutMsg(dPosInfo,"ͬ����ɣ���enter������ֵ",1,iSelect);
						M1_CPU = 2;
						break;
					}
				case (0x22):
					{
						wrLog("oc3.cpuȦ�棨cpu�û��˻���ֵ��");
						break;
					}
				case (0x30):
					{
						wrLog("ִ����չӦ�ò���");
						break;
					}
				case (0x31):
					{
						wrLog("ִ����չӦ�ò�ѯ");
						break;
					}
				case (0x40):
					{
						wrLog("ִ�к���������");					
						iRet = EXCUTE_BLACKLIST(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
						break;
					}
				case (0x60):				
					{
						wrLog("ִ��CPUͬ��");
						change = 1;
						iRet = EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
						M1_CPU = 2;
						
						if (iRet == 1 && *rspcode == R_APP_CARD_UPGRADE_EXEFAIL)
						{
							wrLog("�Ѿ���������ִ�г�ֵ���Ͳ�ѯ��");
							TS_OutPutMsg(dPosInfo,"�����Ŀ��ŵ���������!",1,iSelect);
							//����ִ�г�ֵ���Ͳ�ѯ
							M1_CPU = 1;
							goto HereTRANQRY; 
						}
						if (iRet == 3)
						{
							wrLog("M1Ǯ��(��ѯ��Ȧ��ͳ���)");
							iRet = excute_M1_LOAD(dPosInfo,com,comType,pyct_data,enq,party,rspcode,amount,0x00);
						}
						break;		
					}
				default:
					wrLog("�޷�ʶ��Ľ�������");
					*party = FROMYCT_O;
					*rspcode = UNRECOGNIZED_TRADETYPE;
					iRet = 1;
					break;
				}
	}
	if(iRet != 0)
	{
		if (iRet==-1) //��ʱ
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


// ����Ƽ��¶������汾
// �ڶ������ͨ��ͨǮ���Ĳ�ѯ��ֵ
// ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������; enq:0:Ҫ��ֵ; 1:ֻ��ѯ
// ���Σ�pyct_data:���ͨ������Ϣ; errType:��������0:�ɹ�1:���������Ĵ���; errCode:�������0:�ɹ�
// ����ֵ
// 0���ɹ�(��ѯ���ֵ)
// 1�������������ʧ��
// 2�����������صĴ���
// 3�������ͨ����ʧ��
// 4�����ͨ���صĴ���
// 5�������ͨ����ʧ�ܣ���Ҫ������������
// 6�������������ʧ�ܣ���Ҫ������������
// 7��û��ȷ�ϱ�ʶ����Ҫ������������
// 8��ȷ�ϳ�ֵʧ��
// 9�����������صĴ�����Ҫ������������
int CYCTYPTrade::ST_NormalEnqPay2(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256];
	BYTE ontsStatus1;
	SOCKET sd=0;
	CConnect con;//������
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
	wrLog("��ST_NormalEnqPay��");

	//������ֵѰ��
	wrLog("��1: commuWithIC:ONTS REQUEST��");
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	icData[0] = 0x02;
	len += 1;
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_REQUEST, (BYTE *)icData, (BYTE *)sndData);

	wrHex("�·�����", (BYTE *)sndData, sndLen);
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
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ��
	}

	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	//����������ֵ���ݣ�challenge��
	wrLog("��2: commuWithYCTIC:ONTS CHALLENGE��");		//--------------�����ͨ���������ͽ׶�1���ģ����ؽ׶�2����
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 2, ST_ONTS_CHALLENGE, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)
		{
		//	CTools::byte_to_hex(rcvData+4, (BYTE*)pyct_data->pki_card, 4);//�����߼�����
		//	wrLog("�����߼�����pki_card [%s]", pyct_data->pki_card);
		//	strcpy(pyct_data->yct_serial, "00");
		//	CTools::byte_to_hex((BYTE *)rcvData+8, (BYTE *)pyct_data->yct_serial+2, 4);//���ͨ������ˮ��
		//	wrLog("���ͨ������ˮ��yct_serial [%s]", pyct_data->yct_serial);
		//  Nothing to do...
		}
		else if(rcvData[3]==MOC_ERROR_ONTS_REFUSED)
		{
			//------------���ͨ�쳣����-------------
			*party = FROMYCT_O;
			*rspcode = rcvData[13];
			closesocket(sd);			
			return 4;// 2�����������صĴ���
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}

	//����������ֵ��֤����
	wrLog("��3: commuWithIC:ONTS AUTH PREPARE��");		//--------------���������������֤����������ؽ׶�3����
	len = 2;
	bbzero(sndData);
	bbzero(icData);
	
	rcvLen = 0;
	bbzero(rcvData);

	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_AUTH_PREPARE, (BYTE *)icData, (BYTE *)sndData);
	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
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
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}

	//����������ֵ��֤�������
	wrLog("��4: commuWithYCTIC:ONTS AUTH EXECUTE��");		//-------------�����ͨ���������ͽ׶�3���ģ����ؽ׶�4����
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 4, ST_ONTS_AUTH_EXECUTE, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL || rcvData[3]==MOC_ERROR_ONTS_AUTH)
		{
			CTools::byte_to_hex(rcvData+4, (BYTE*)pyct_data->pki_card, 4);//�����߼�����
			//wrLog("�����߼�����pki_card [%s]", pyct_data->pki_card);
			strcpy(pyct_data->yct_serial, "00");
			CTools::byte_to_hex((BYTE *)rcvData+8, (BYTE *)pyct_data->yct_serial+2, 4);//���ͨ������ˮ��
			wrLog("���ͨ������ˮ��yct_serial [%s]", pyct_data->yct_serial);
			ontsStatus1 = rcvData[13];
			//-------------------��Ҫ����ONTS STATUS2,���Է�ȷ��------------------------
 			if(rcvData[12] == 0x00 && rcvData[13] == 0x00 && rcvData[14] == 0x00 && rcvData[15] == 0x00)
 			{
 				CTools::byte_to_hex((BYTE *)rcvData+16+4, (BYTE *)pyct_data->ticket_no, 8);//���ͨƱ������
 				wrLog("���ͨƱ������ticket_no: [%s]", pyct_data->ticket_no);
 				getAsiAmo(rcvData+16+25, 4, pyct_data->ticket_amo_bef); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
 				wrLog("���ͨƱ�����ticket_amo:[%s]", pyct_data->ticket_amo_bef);//���ͨƱ�����
 			}
			else if(rcvData[12] == 0x00 && rcvData[13] == 0x43)
			{
				//-----������һ���������������-------
				NULL;
			}
 			else
			{
				*party = FROMYCT;
				*rspcode = rcvData[13];			//----------------??
				closesocket(sd);
				return 2;// 2�����������صĴ���
			}
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}

	if(enq==NORMALENQ && ontsStatus1 != 0x43)//ֻ��ѯ
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

	//���ɳ�ֵ���ײ�������
	wrLog("��5: commuWithIC:ONTS CHARGE PREPARE��");		//---------------��������������ɳ�ֵ���ײ�������������ؽ׶�5����
	memcpy(pyct_data->amount, amount, 12);
	bbzero(icData);
	getHexAmo(amount, icData);	
	wrHex("��ֵ���",icData,4);
	icLen = 4;
	icData[4] = 0x14;				//------------�������ʹ��룿�̶���0x14
	icLen += 1;
	len = icLen + 2;
	bbzero(sndData);
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_CHARGE_PREPARE, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL || (rcvData[3]==MOC_ERROR_CARD_NAMELIST || rcvData[3]==MOC_ERROR_CARD_READ))
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
// 		else if(rcvData[3]==MOC_ERROR_CARD_NAMELIST || rcvData[3]==MOC_ERROR_CARD_READ)
// 		{
// 			//------------������������ʹ���쳣���Ľ��е�5�׶εĴ���-----------
// 			NULL;
// 		}
// 		else if( rcvData[3]==MOC_ERROR_CARD_READ)
// 		{
// 			//------------��ֵ�׶�5���쳣���ģ��������Ϊ��һ�����ĵ�ONTS STATUS2-------
// 			*party = FROMYCT_O;
// 			*rspcode = ontsStatus1;
// 			closesocket(sd);
// 			return 4;// 2�����������صĴ���
// 		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		wrLog("test.ONTS CHARGE PREPARE timeout");
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}

	//==============================================���²����쳣��Ҫ����������===================================================

	pyct_data->status = 0;
	CDB::updateYctYPData(*pyct_data);
	
	//ִ��������ֵ����
	wrLog("��6: commuWithYCTIC:ONTS CHARGE EXECUTE��");		//--�����ͨ���������ͽ׶�5���ģ����ؽ׶�6���ģ�������������������ؽ׶�7���Ĳ���
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 6, ST_ONTS_CHARGE_EXECUTE, sndData, sndLen, rcvData, &rcvLen);
	//=========================================================================================
	//���ѷ����󣬵��Ƕ������޷��ص������δ�����
	//=========================================================================================
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL || rcvData[3]==MOC_ERROR_TS_FALL )
		{
			CTools::byte_to_hex(rcvData+4, (BYTE*)pyct_data->result_message, 16);//���׽�����ݱ���
			wrLog("���ؽ������result_message [%s]", pyct_data->result_message);

			getAsiAmo(rcvData+4+12, 4, pyct_data->ticket_amo_aft); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
			wrLog("���ͨƱ����ֵ�����ticket_amo:[%s]", pyct_data->ticket_amo_aft);//���ͨƱ�����

			CDB::updateYctYPData(*pyct_data);

			if(ontsStatus1 == 0x43)
			{
				*party = FROMYCT;
				*rspcode = 0x43;
				closesocket(sd);
				return 4;// 2�����������صĴ���
			}
		}
		else if( rcvData[3]==MOC_ERROR_ONTS_REFUSED )//--------------------��ֵ�׶�6���쳣���ģ�������ж�006F��--------
		{
			//------------���ͨ�쳣����-------------
			*party = FROMYCT_O;
			*rspcode = rcvData[13];
			closesocket(sd);
			return 4;// 2�����������صĴ���
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 9;// 9�����������صĴ�����Ҫδ��������
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 11;// 11����ȡ��Կ��ʱ����Ҫ������������������
	}
	else
	{
		wrLog("test.ONTS CHARGE EXECUTE timeout");
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 10;// 10�������������ʧ�ܣ��޷���ȷ�Ƿ��ֵ�ɹ�����Ҫ��ѯ���
	}

	if(enq==NORMALENQ)//ֻ��ѯ
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

	wrLog("��7: commuWithIC:ONTS COMMIT��");		//---------------�����������ONTS_COMMIT������ؽ׶�7����
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_COMMIT, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)//------------�ύ��ֵ���ʧ����δ�����
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else if( rcvData[3]==MOC_ERROR_PKI_DES)//
		{
			*party = FROMIC_N;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 9;// 9�����������صĴ�����Ҫδ��������
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 1�������������ʧ�ܣ���Ҫδ��������
	}

	//������ȷ�Ͻ��
	wrLog("��8: commuWithYCTIC:ONTS CONFIRM��");	
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
			return 8;// 8��ȷ�ϳ�ֵʧ��
		}
		else
		{
			*party = FROMIC_N;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 9;// 9�����������صĴ�����Ҫδ��������
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI_N;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 5;// 5�������ͨ����ʧ�ܣ���Ҫδ��������
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 6;// 6�������������ʧ�ܣ���Ҫδ��������
	}
	
	return 0;
}

// �����ݽ��д��
// ��Σ�tag:���ı�ʶ; ver:�汾; num:����˳���; len:���ĳ���(cmd,stat,data�ĳ���); cmd:�����ʶ; stat:״̬��ʶ; data:����;
// ���Σ�sndData:�����������
// ����ֵ��������
int CYCTYPTrade::ST_packData(int head, int len, int cmd, const BYTE *data, BYTE *sndData)
{
	int sndLen;
	BYTE c;  //У����
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
	
	wrHex("�����ͨ��������", (BYTE*)sndData, sndLen);
	bbzero(icData);
	iRet = SndRcvYctData(sd, (char *)sndData, sndLen, (char *)icData, &icLen, 256);//���ͨ�������ݰ������135�ֽڣ�
	if( iRet!=0)
	{
		return 1;
	}
	else
	{
		NULL;
	}
	
	wrHex("�����ͨ��������", (BYTE*)icData, icLen);
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
	
	
	wrLog("��%d: commuWithIC��", num);
	
	bbzero(sndICData);	
	sndICLen = ST_packData(ST_HEAD, len, cmd, icData, sndICData);
	*rcvLen = 0;
	bbzero(rcvData);
	
	iRet = SndRcvIcData(dPosInfo, com, comType, "���Ժ�...", sndICData, sndICLen, rcvData, rcvLen, 57600);
	if( iRet==0 )
	{
		return 0;// 0���ɹ�
	}
	else
	{
		return 2;// 2�������������ʧ��
	}
}


int CYCTYPTrade::ST_initReader(const TSDLLPosInfo dPosInfo, const int com, const int comType)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, cmd, iSelect=0;
	BYTE icData[256], sndData[256], rcvData[256], pin[7];
	int baudRate = 57600;
	
	wrLog("����ʼ����������");
	//��ȡPIN��
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	cmd = 0x61;
	icData[0] = 0x00;
	icData[1] = 0x51;
	len += 2;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==0x00)
		{
			//��ʼ������
			wrLog("������Ƽ���������");
			NULL;
		}
		else
		{
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		return 1;// 1�����������صĴ���
	}
	
	
	bbzero(pin);
	
	iRet = CDB::getYctPIN2(dPosInfo.pos_no, pin);
	if(iRet == 1)
	{
		wrLog("������״̬��");
		TS_OutPutMsg(dPosInfo, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ", 0, iSelect);
	}
	else if(iRet)
	{
		wrLog("����ȡPINʧ�ܡ�");
		TS_OutPutMsg(dPosInfo, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ", 0, iSelect);
		return 3;	//��ȡPINʧ��
	}
	
	wrLog("getYctPIN2: %s", pin);
	
	wrLog("��У��PIN��");
	//��ȡpin�ĺ���
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
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==0x00)
		{
			//��ʼ������
			wrLog("��PINУ��ɹ���");
			NULL;
		}
		else if( rcvData[3]==MOC_ERROR_SAM_PIN )
		{
			wrLog("��SAM���������");
			CDB::setYCTPinErr(dPosInfo.pos_no);
			TS_OutPutMsg(dPosInfo, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ", 0, iSelect);
			return 2;// 2�����������صĴ���
		}
		else
		{
			wrLog("��PINУ��ʧ�ܣ�%x��", rcvData[3]);
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		return 1;// 1�����������صĴ���
	}
	
	return 0;
}

int CYCTYPTrade::ST_NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE icData[256], sndData[256], rcvData[256], pki_card_this[8+1], bTemp[17];
	SOCKET sd=0;
	CConnect con;//������
	int baudRate = 57600;
	
	wrLog("��ST_NormalNotFinish��");
	
	
	//����δ�������״�����������
	wrLog("��1: commuWithIC:ONTS UREQUEST��");
	bbzero(sndData);
	bbzero(icData);
	len = 2;
	icData[0] = 0x02;
	len += 1;
	sndLen = ST_packData(ST_HEAD, len, ST_ONTS_UREQUEST, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)	//��ȡ�׶�A�ı���
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	//����δ�������״�����ս���ݣ�challenge��
	wrLog("��2: commuWithYCTIC:ONTS UCHALLENGE��");		//--------------�����ͨ���������ͽ׶�A���ģ����ؽ׶�B����
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
			return 4;// 2�����������صĴ���
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
	
	
	//�ύδ�������׵�״̬����
	wrLog("��3: commuWithIC:ONTS UCOMMIT��");
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
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)	//��ȡ�׶�C�ı���
		{
			sndLen = rcvLen-4-1;
			memcpy(sndData, rcvData+4, sndLen);
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);
			return 2;// 2�����������صĴ���
		}
	}
	else
	{
		*party = FROMCTSI_N;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ�ܣ����δ֪
	}
	
	//����δ�������״���ȷ�Ͻ��
	wrLog("��4: commuWithYCTIC:ONTS UCONFIRM��");		//--------------�����ͨ���������ͽ׶�C���ģ����ؽ׶�D����
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC(dPosInfo, com, comType, sd, 4, ST_ONTS_UCONFIRM, sndData, sndLen, rcvData, &rcvLen);
	if( iRet==0  )
	{
		if( rcvData[3]==MOC_ERROR_NORMAL)	//ȷ�Ͻ��
		{
		/*
		//��ȡ��ֵ���,��λ�ȡ?
		if(rcvData[16+4+4+4] == 0x00 && rcvData[16+4+4+4+1] == 0x00)	//�Ƿ��н׶�D���ģ���
		{
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
		closesocket(sd);
		return 0;// 0���ɹ�
		}
		else
		{
		*party = FROMCTSI;
		*rspcode = FAIL_LAST;
		closesocket(sd);
		return 5;// 5��ʧ��
		}
			*/
			CTools::byte_to_hex(rcvData+4, (BYTE*)pki_card_this, 4);//�����߼�����
			//wrLog("�����߼�����pki_card_this [%s]", pki_card_this);
			if( memcmp(pki_card_this, pki_card, 8)==0 )
			{
				if(rcvData[16] == 0x00 && rcvData[17] == 0x00)
				{
					*party = FROMSUCC;
					*rspcode = SUCC_MSG;
					closesocket(sd);
					return 0;// 0���ɹ�
				}
				else if(rcvData[16] == 0xff && rcvData[17] == 0xff)
				{
					*party = FROMCTSI;
					*rspcode = FAIL_LAST;
					closesocket(sd);
					return 5;// 5��ʧ��
				}
				else
				{
					*party = FROMCTSI_N;
					*rspcode = PKICHGNOTFINISH;
					closesocket(sd);
					return 4;// 4��PKI�������������δ֪
				}
			}
			else
			{
				wrLog("��ԭ���׹����߼����Ų�һ��pki_card [%s]", pki_card);
				*party = FROMCTSI_N;
				*rspcode = PKICHGNOTFINISH;
				closesocket(sd);
				return 4;// 4��PKI�������������δ֪
			}	
		}
		else if( rcvData[3]==MOC_ERROR_ONTS_COMMIT)
		{
			*party = FROMYCT_O;
			*rspcode = rcvData[17];
			closesocket(sd);
			//��ˮ�Ų�һ�£��˽��������ͨ������
			if(rcvData[17] == 0xD1)
			{
				wrLog("����ˮ��ϵͳ��ˮ��һ�£���Ϊʧ��");
				return 5;// 5��ʧ��
			}
			else
				return 4;// 2�����������صĴ���
		}
		else
		{
			*party = FROMIC;
			*rspcode = rcvData[3];
			closesocket(sd);			
			return 2;// 2�����������صĴ���
		}
	}
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
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
				sprintf(msg, "���ͨ��Ѻ��ɹ�\n%s\n�밴��ȷ�ϡ���ӡƾ֤", gbl.yct_disp);
			}
			
			else
			{
				sprintf(msg, "���ͨ��ֵ�ɹ�\n%s\n�밴��ȷ�ϡ���ӡƾ֤", gbl.yct_disp);
				
			}
			
			TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);

			break;
		case SUCC_NOMSG:
			break;
		case SUCC_MSG_LAST:
			TS_OutPutMsg(gbl.dPosInfo, "��һ�����ͨ��ֵ�ɹ�", 0, iSelect);
			break;
		default:
			if(atoi(gbl.amount)==1000)
			{
				
				TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ʧ�ܣ�����ϵ���ͨ�ͷ�", 0, iSelect);
			}
			else
			{
				sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888!\n", party, rspcode);
				TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
			}

			break;
		}
	}
	else if( party==FROMCTSI || party==FROMCTSI_N )

	{
		if(atoi(gbl.amount)==1000)
		{
			TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ʧ�ܣ�����ϵ���ͨ�ͷ�", 0, iSelect);

		}

		else
		{

			switch(rspcode)
			{
			case 0x01:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "ϵͳδ�ܷ����ж����������ã�����δ��ͨ����ҵ�񣬿�ֱ������[861]���뿪ͨ��\n");
				break;
			case 0x02:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "���ͨ��ֵ��ȡ��\n");
				return 0;
				break;
			case 0x03:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "�۷ѳ�ʱ\n");
				break;
			case 0x04:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "����������ͽ������ݳ�ʱ\n");
				break;
			case 0x05:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "ϵͳ�����ж�������������ȷ���������Ƿ���ȷ���ӣ�Ȼ��ѡ��[����]�����²�����\n");
				break;
			case 0x06:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "�����ͨ���ͽ������ݳ�ʱ\n");
				break;
			case 0x07:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "����������ȷ������״̬\n");
				break;
			case 0x08:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "������ָ������ʧ��\n");
				break;
			case 0x09:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "ϵͳ���������\n");
				break;
			case 0x0A:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "ϵͳ���������\n");
				break;
			case 0x0B:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "��֧�ֵĿ�\n");
				break;
			case 0x0C:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "PKI���Ѹ������޷���ȡ�ϱʽ��׽��\n");
				break;
			case 0x0D:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "������ȡ�ϱʽ��׽����ȷ�ϳ�ֵʧ��\n");
				break;
			case 0x0E:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "�ѿ۷ѣ�����ֵ���δ֪\n");
				break;
			case 0x0F:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "ǩ������\n");
				break;
			case 0x10:
				sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
				strcat(msg, "�ǳ���Ǹ���޷���֪��ֵ��������ٴη����ѯ���ף�ͨ��Ʊ�����˶Խ����Ƿ�ɹ������������������ʣ�����ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n");
				break;
			case 0x11:
				sprintf(msg, "�������޷������� [%02X%02X] \n", party, rspcode);
				strcat(msg, "��ָ���ͻ�����������ȷ�����ڶ������ϣ������ƶ�����\n");
				break;
			case 0x15:
				sprintf(msg, "��Ϊ�������� [%02X%02X] \n", party, rspcode);
				strcat(msg, "����ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n");
				break;
			default:
				sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
				break;
		}

		}
	
		if( party==FROMCTSI )
		{
			strcat(msg, "���ͨ��ֵʧ��\n");
		}
		else
		{
			strcat(msg, "���ͨ��ֵ���δ֪\n");
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
			TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ʧ�ܣ�����ϵ���ͨ�ͷ�", 0, iSelect);

		}
		else
		{
			sprintf(msg, "���������صĴ����룺[%02X%02X] \n", party, rspcode);
			switch(rspcode)
			{
			case 0x60:
				strcat(msg, "������û�а�װSAM��������ͷ�������ϵ\n");
				break;
			case 0x62:
				strcat(msg, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ\n");
				break;
			case 0x80:
				strcat(msg, "�������Ҳ���������ѿ��ŵ���������\n");
				break;
			case 0x90:
				strcat(msg, "�����ͨ��\n");
				break;
			case 0x91:
				strcat(msg, "��Ƭ�Ѿ�����\n");
				break;
			case 0x93:
				strcat(msg, "��Ƭ�Ƿ�\n");
				break;
			case 0x94:
				strcat(msg, "��ƬΪ��������\n");
				break;
			case 0x96:
				strcat(msg, "Ǯ������ֵ�󳬳�����\n");
				break;
		case 0x13:
			sprintf(msg, "���ת��ʧ�� [%02X%02X] \n", party, rspcode);
			strcat(msg, "�뷵�����˵���������ָ���ͻ�����������ȷ�����ڶ������ϣ������ƶ�����\n");
			break;
		case 0x01:
			strcat(msg, "���������ش�������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n");
			break;
		default:
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			break;
		}
		if( party==FROMIC )
		{
			strcat(msg, "���ͨ��ֵʧ��\n");
		}
		else
		{
			strcat(msg, "���ͨ��ֵ���δ֪\n");
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
				TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ɹ�\n", 0, iSelect);
			}
			else
			{
				TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ʧ�ܣ�����ϵ���ͨ�ͷ�", 0, iSelect);
				
			}
			
		}

		else
		{
			
				switch(rspcode)
		{
		case 0x00:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ�ɹ�\n");
			break;
		case 0xFF:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ����Ҫ������\n");
			break;
		case 0x01:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "������\n");
			break;
		case 0x02:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ˮ�Ŵ�\n");
			break;
		case 0x03:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�׶δ���\n");
			break;
		case 0x11:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��Ʊ���Ͳ���\n");
			break;
		case 0x12:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����Ͳ���\n");
			break;
		case 0x13:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����������\n");
			break;
		case 0x14:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ն˽��û�ISAM_IDΪ��\n");
			break;
		case 0x15:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����Ų�����\n");
			break;
		case 0x16:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x17:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "Ʊ��������Ч��\n");
			break;
		case 0x20:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ն����һ�ʼ�¼\n");
			break;
		case 0x21:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "û�и�����������Ʊ�ײ�\n");
			break;
		case 0x22:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��·�Ŵ���\n");
			break;
		case 0x2F:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x30:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����ͷ��\n");
			break;
		case 0x31:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��Ƭ��һ��\n");
			break;
		case 0x32:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ˮ�Ų�һ��\n");
			break;
		case 0x33:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "MAC��\n");
			break;
		case 0x3F:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x41:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "���ױ���û�н�����ѯ����\n");
			break;
		case 0x42:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��γ��ִ���\n");
			break;
		case 0x43:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x44:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "����ԭ��������Ϣ������\n");
			break;
		case 0x45:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ظ���ֵ\n");
			break;
		case 0x46:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵ������������\n");
			break;
		case 0x47:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ظ���ֵ\n");
			break;
		case 0x48:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "���ڷ���ʱ��\n");
			break;
		case 0x49:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0x50:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ʺű�����\n");
			break;
		case 0x51:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ն�ÿ���޶�\n");
			break;
		case 0x52:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�ʺ�����\n");
			break;
		case 0x53:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��ֵǰ�������Ч\n");
			break;
		case 0x54:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "Ʊ�����ں�Ҫ�������ڲ���Ӧ\n");
			break;
		case 0x55:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "�����ն����һ�ʼ�¼\n");
			break;
		case 0x5F:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��������\n");
			break;
		case 0xA1:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "��Ʊ��ֵ������ϵͳ����Ҫ������\n");
			break;
		case 0x99:
			sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
			strcat(msg, "ϵͳ����\n");
			break;
		default:			
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			break;
		}
		
		}
		if( party==FROMYCT )
		{
			strcat(msg, "���ͨ��ֵʧ��\n");
		}
		else
		{
			strcat(msg, "���ͨ��ֵ���δ֪\n");
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
	}	
	else if( party==FROMYCT_O )

	{

		if(atoi(gbl.amount)==1000)
		{
			if(rspcode==0x01||rspcode==0x02||rspcode==0x03)
			{
				TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ɹ�", 0, iSelect);
			}
			
			else
			{
				TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ʧ�ܣ�����ϵ���ͨ�ͷ�", 0, iSelect);
			}
			
		}
		else
		{
					sprintf(msg, "���ͨ���صĴ����룺[%02X%02X] \n", party, rspcode);
		switch(rspcode)
		{
		case 0x21:
			strcat(msg, "�����߼�������Ч\n");
			break;
		case 0x22:
			strcat(msg, "�����汾����Ч\n");
			break;
		case 0x23:
			strcat(msg, "����������Ч��\n");
			break;
		case 0x24:
			strcat(msg, "������ͣ��\n");
			break;
		case 0x25:
			strcat(msg, "������Ȳ���\n");
			break;
		case 0x26:
			strcat(msg, "�������ڷ���ʱ��\n");
			break;
		case 0x41:
			strcat(msg, "�����ͨϵͳƱ��\n");
			break;
		case 0x42:
			strcat(msg, "Ʊ��������Ч��\n");
			break;
		case 0x43:
			strcat(msg, "��������\n");
			break;
		case 0x51:
			strcat(msg, "Ѱ������\n");
			break;
		case 0x52:
			strcat(msg, "������Կ����\n");
			break;
		case 0x53:
			strcat(msg, "��Ʊ��������Ϣ������\n");
			break;
		case 0x54:
			strcat(msg, "��Ʊ��Ǯ��������\n");
			break;
		case 0x5A:
			strcat(msg, "Ʊ���Ѿ�Ϊ��������\n");
			break;
		case 0x5B:
			strcat(msg, "д��������ʶ�ɹ�\n");
			break;
		case 0x5C:
			strcat(msg, "д��������ʶʧ��\n");
			break;
			//-----------------��Ҫȷ��-------------------
		case 0x01:
			strcat(msg, "���׳ɹ�\n");
			break;
		case 0x02:
			strcat(msg, "���׳ɹ�\n");
			break;
		case 0x03:
			strcat(msg, "���׳ɹ�\n");
			break;
		case 0xF1:
			strcat(msg, "����ʧ��\n");
			break;
		case 0xF2:
			strcat(msg, "����ʧ��\n");
			break;
		case 0xF3:
			strcat(msg, "����ʧ��\n");
			break;
		case 0xF4:
			strcat(msg, "����ʧ��\n");
			break;
		case 0xF5:
			strcat(msg, "����ʧ��\n");
			break;
			//---------------------------------------------
		case 0x61:
			strcat(msg, "Ʊ��״̬�쳣�����ܳ�ֵ\n");
			break;
		case 0x62:
			strcat(msg, "��ֵ�����������\n");
			break;
		case 0x63:
			strcat(msg, "��ֵ����Ʊ���޶�\n");
			break;
		case 0x64:
			strcat(msg, "��ֵ���Υ��ҹ��涨\n");
			break;
		case 0xFF://-------ȷ�ϳ�ֵʧ�ܣ����ܲ��������ﴦ��----------
			strcat(msg, "��ֵʧ��\n");
			break;
		case 0xD1:
			strcat(msg, "ϵͳ��ˮ����ϵͳ��¼��һ�£���ֵʧ��\n");
			break;
		case 0xD2:
			strcat(msg, "��ȷ�ϳ������ײ������޸�Ϊʧ��\n");
			break;
		case 0xD3:
			strcat(msg, "��ȷ��ʧ�ܽ��ײ������޸�Ϊ�ɹ�\n");
			break;
		case 0x31:
			strcat(msg, "��ֵ���Υ��ҵ�����������ѡ������г�ֵ\n");
			break;
		case 0x30:
			strcat(msg, "��ֵ�������ޣ�������ѡ������г�ֵ\n");
			break;
		case 0x32:
			strcat(msg, "���ͨ��̨���ش����޷�ʶ��Ľ�������\n");
			break;
		default:			
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			break;
		}
		TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
		
		}

	}
	else
	{
		if (atoi(gbl.amount)==1000)
		{
			TS_OutPutMsg(gbl.dPosInfo, "���ͨ��Ѻ��ʧ�ܣ�����ϵ���ͨ�ͷ�", 0, iSelect);
		}
		
		else
		{
			sprintf(msg, "���ͨ��ֵδ֪����[%02X%02X]������ϵ���߿ͷ����ġ�Ctrl+H��  400-80-11888\n", party, rspcode);
			TS_OutPutMsg(gbl.dPosInfo, msg, 0, iSelect);
			
		}
	}

	return 0;
}

/*************************���������ͨ��ֵ�������� ��ʼ*****************************************/

//��д�������ʼ��������������ֻ����һ�Ρ�
//�����������ʼ������

int CYCTYPTrade::ST_initReader3(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data)
{
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0, cmd, iSelect=0;
	BYTE icData[256], sndData[256], rcvData[256], pin[7];
	int baudRate = 57600;
	BYTE INT_KEY[8];
	
	wrLog("��ST_initReader3��");
	bbzero(pin);
	iRet = CDB::getYctPIN2(dPosInfo.pos_no, pin);
	if(iRet == 1)
	{
		wrLog("������״̬��");
		TS_OutPutMsg(dPosInfo, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ", 0, iSelect);
	}
	else if(iRet)
	{
		wrLog("����ȡPINʧ�ܡ�");
		TS_OutPutMsg(dPosInfo, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ", 0, iSelect);
		return 3;	//��ȡPINʧ��
	}
	
	
	wrLog("getYctPIN2: %s", pin);
	//	return 3;
	
	wrLog("��У��PIN��");
	//��ȡpin�ĺ���
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
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			wrHex("GPK_ID", (BYTE *)rcvData, 9);
			//��ʼ������
			wrLog("����ʼ���ɹ���");
			CTools::byte_to_hex(rcvData+4,(BYTE *) pyct_data->pki_card, 4);//�����߼�����
			//wrLog("�����߼�����pki_card [%s]", pyct_data->pki_card);
			NULL;
			
		}
		else if( rcvData[3]==MOC_ERROR_SAM_PIN )
		{
			
			wrLog("��SAM���������");
			CDB::setYCTPinErr(dPosInfo.pos_no);
			TS_OutPutMsg(dPosInfo, "���ͨSAM������������ʱ�޷�ʹ�ã�����ͷ�������ϵ", 0, iSelect);
			return 2;// 2�����������صĴ���
		}
		else
		{
			wrLog("��PINУ��ʧ�ܣ�%x��", rcvData[3]);
			return 2;// 2�����������صĴ���
		}
	}
	
	else
	{
		return 1;// 1�����������صĴ���
	}
	
	return 0;
	
}

//�����ͨ��������������
int CYCTYPTrade::ST_commuWithYCTIC3(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data, const int com, const int comType, const SOCKET sd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen)
{
	int iRet=0;
	
	wrHex("�����ͨ��������", (BYTE*)sndData, sndLen);
	iRet = SndRcvYctData(sd, (char *)sndData, sndLen, (char *)rcvData,rcvLen, 500);//���ͨ�������ݰ�������ֽ�500��
	//��ر��ķ��ͺ�ֱ�ӷ��أ�����ȴ��������ݰ�
	if (sndData[0] == 0xfd)
	{
		//���ͨ����+1��ͬʱ����yctyp_signin����ı�������ֶ�
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
	
	
	wrHex("�����ͨ��������", (BYTE*)rcvData, *rcvLen);
	//���ͨ����+1��ͬʱ����yctyp_signin����ı�������ֶ�
	pyct_data->yct_sequence_NO++;
	wrLog("��ǰ�������: %d",pyct_data->yct_sequence_NO);
	CDB::updateYct3SequenceNo((char *)dPosInfo.pos_no,pyct_data->yct_sequence_NO);
	
	if(*rcvLen == 0)
		return 1;
	
	return 0;
	
}

//�ն�ǩ��
int CYCTYPTrade::yct_SignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * party ,BYTE* rspcode)

{
	
	int iRet=0, len=0, icLen=0, sndLen=0, rcvLen,cmd=0;
	SOCKET sd=0;
	int baudRate=57600;
	CConnect con;//������
	BYTE icData[256], sndData[256], rcvData[256];
	BYTE term_id[6]; //�ն˺�
	BYTE DE1[136+1];
	BYTE DE2[136+1];
	BYTE DE3[136+1];
	BYTE term_time[12+1];
	BYTE SK[32+1];
	BYTE SERVER_TIME[6+1]; //������ʱ��
	BYTE SKEY[32+1];
	bbzero(term_id);
	bbzero(SK);
	bbzero(SKEY);
	bbzero(icData);
	bbzero(sndData);
	bbzero(DE1);
	bbzero(DE2);
	bbzero(DE3);
	
	wrLog("��ǩ������1:R_AC_LOGIN_1��"); 	//�ն�ǩ��
	CTools::get_termTime((char *)term_time); //��ȡ�ն�ʱ��
	
	len = 2;
	cmd = 0xC8; //ǩ������
	
	CTools::hex_to_byte((BYTE *)pyct_data->pki_card,(BYTE *)icData,8); //�ն˵Ĺ�����
	
	memcpy(icData+4,term_id,6);  //�ն˱��
	CTools::hex_to_byte((BYTE*)term_time,icData+10,14);//�ն�ʱ��
	len += 17;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			memcpy(DE1,rcvData+4,136);//ȡ����
			wrHex("����������ǩ��DE1:%s",rcvData,rcvLen);
			NULL;
		}
		
		else if(rcvData[3]==0x61)
		{
			
			wrLog("SAM����ʼ��ʧ��"); 
		}
		
	}
	
	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	wrLog("����DE1 ������������");		//--------------�����ͨ���������ͽ׶�1���ģ����ؽ׶�2����
	
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, DE1, 136, rcvData, &rcvLen);
	
	if( iRet==0  )
	{
		
		memcpy(DE2,rcvData,136);
		BYTE de2_tmp[272];
		CTools::byte_to_hex(rcvData,de2_tmp,136);
		wrLog("����������ǩ������DE2:%s",de2_tmp);
		
	}
	
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
	
	wrLog("�ն˽�DE2�·���������");
	
	bbzero(icData);
	bbzero(sndData);
	
	len = 2;
	cmd = 0xC9; //ǩ��2����
	
	memcpy(icData, DE2, 136); //�ն˵Ĺ�����
	len += 136;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			
			CTools::byte_to_hex(rcvData+4,pyct_data->SHID,8); //������ˮ��
			memcpy(SK,rcvData+12,32); //skȡǰ16���ֽڣ���ΪAES��Կ
			memcpy(pyct_data->SKEY,SK,16);
			CTools::byte_to_hex(pyct_data->SKEY,SKEY,16);
			//wrLog("DE3:%s",DE3);
			CTools::byte_to_hex(rcvData+44,SERVER_TIME,6);  //ȡ������ʱ��
			bbzero(pyct_data->SignIn_ServerTime);
			strcpy(pyct_data->SignIn_ServerTime,"20");
			strcat(pyct_data->SignIn_ServerTime,(char *)SERVER_TIME);
			memcpy(DE3,rcvData+50,136);//����ǩ������3����
			
			wrHex("����������ǩ������DE3",DE3,136);
			wrLog("SHID:%s",pyct_data->SHID);
			wrLog("SKEYΪ��%s",SKEY);
			wrLog("ǩ��������ʱ��:server_time:%s",SERVER_TIME);
			
			NULL;
		}
		else if(rcvData[3]==0xFF)
		{
			
			wrLog("MOC_AUTH_FAIL ʧ��");
			return 3;// �����������ʧ��
			
		}
		
	}
	
	wrLog("��3: commuWithYCTIC��");		//--------------�����ͨ���������ͽ׶�2���ģ����ؽ׶�3����
	rcvLen = 0;
	bbzero(rcvData);
	iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data,com, comType, sd, DE3, 136, rcvData, &rcvLen);
	if( iRet==0  )
	{
		
		if(rcvData[8]==0x00)
		{
			wrLog("��֤�ɹ�");
			pyct_data->yct_sequence_NO=0; //ǩ��֮�����������Ϊ0
			CDB::updateYct3SequenceNo((char *)dPosInfo.pos_no,pyct_data->yct_sequence_NO);		
		}
		else if(rcvData[8]==0xFF)
		{
			wrLog("��֤ʧ��");
			return 4;  //��֤ʧ��
		}
	}
	
	else if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
	
	return 0;
}


//��ȡ��Ƭ��������Ϣ,���������ţ��߼����ţ�SAK,��������Ϣ1����SW1SW2
int CYCTYPTrade::R_PUB_QRY_PHYSICS_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE*physicsInfo)

{
	int  len=0, icLen=0, sndLen=0, rcvLen=0,cmd=0,iRet=0;//,iSelect;
	BYTE icData[256], sndData[256], rcvData[256];
	
	bbzero(icData);
	bbzero(sndData);
	
	int baudRate = 57600;
	//��ѯ����������Ϣ 
	wrLog("����ѯ����������Ϣ��");
	
	len = 2; //����
	cmd = 0x93; 
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
	{
		if( rcvData[3]==0x00)
		{
			wrHex("����������Ϣ��",rcvData,rcvLen);
			memcpy(physicsInfo,rcvData+4,51);
			NULL;
		}
		else if (rcvData[3]==0x80)
		{
			return 1;
		}
		
		//else �������������б�
		else
		{
			iRet = -1;
			return iRet;
			
		}
		
	}
	return 0;
	
}

//����CPU��������ѯ��Ȧ��

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
	BYTE outside_info[16];//�ⲿϵͳ������Ϣ
	BYTE tuozhan_stan[8];
	char yct_sequence_NO[4+1];
	BYTE len_tmp[2];
	CConnect con;
	int load_status;
	BYTE followup_oper[2+1];
	BYTE charge_authcode[8+1];
	int iSelect;
	int load_suc=0; //Ȧ��ɹ���־
	
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
	wrLog("��CPU_EnqPay��");
	CTools::getDate(pyct_data->txn_date);
    CTools::getTime(pyct_data->txn_time);
	//1.P_PUB_SET_READCARDINFO(���ö�����Ϣ)
	R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,query_info,SW1SW2);
	
	//4.�ն�����������Ͷ���ָ��R_CPU_GET_CARDINFO CPUȡ����Ϣ
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
			return 1;// 1�������������ʧ��
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
		//ֻ��ѯ
		return 0;
	}

	if (payType == 0x20)
	{
		sprintf(temp,"����:%s\n\n��ֵǰ�����:%.2fԪ\n\n����Enter����ֵ  ����Esc������",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100 - atof(des_lim));
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
     	//	sprintf(bef,"\n\n%sԪ",pyct_data->ticket_amo_bef);
		sprintf(temp,"\n\n��ֵǰ�����:%.2fԪ\n\n����Enter����ֵ  ����Esc������",atof(pyct_data->ticket_amo_bef)/100 - atof(des_lim));
		strcat(auto_show,temp);
		TS_OutPutMsg(dPosInfo,auto_show,0,iSelect);
		if (iSelect)
		{
			*party=FROMCTSI_N;
			*rspcode=USERCANCEL;
			return 5;
		}
	}
	
	//�ն����ֵ����������Ȧ���ѯ�����ģ�P_CPU_LOAD_QRY��
	iRet = P_CPU_LOAD_QRY(dPosInfo,com, comType,count_info,pyct_data,amount,payType,trade_message1,party,rspcode);
    if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		return 3;// 3�������ͨ����ʧ��
	}
	else if (iRet != 0)
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ��
	}
	
	//6.�ն������������Ȧ���ʼ��ָ��R_CPU_LOAD_INIT
    R_CPU_LOAD_INIT(dPosInfo,com, comType,pyct_data,trade_message1,trade_sequence,yct_term_status,card_info);
	
	
	//7.Ȧ���ʼ���ɹ�֮�����ֵ����������Ȧ��(P_CPU_LOAD)
	iRet = P_CPU_LOAD(dPosInfo,com, comType,pyct_data,card_info,yct_term_status,trade_message2,charge_authcode);
	if( iRet==1  )
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;	
		return 3;// 3�������ͨ����ʧ��
	}
	else if (iRet != 0)
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		return 1;// 1�������������ʧ��
	}
	
	put_card = 0;
	//����������Ȧ��ָ�� R_CPU_LOAD(cpuǮ��Ȧ��) ��cpu������cpuǮ����ֵ ��������ն˽������󣬲�ѯ��Ƭ���Ҳʧ�ܵ�ʱ��������⣡��
	iRet = R_CPU_LOAD(dPosInfo,com, comType,pyct_data,trade_message2,TAC,load_status,count_info);
	if (iRet==0)
	{
			load_suc=1; //��ʾ��ȷȦ��ɹ�
	}

	if (change == 1 && iRet == -1) //δ֪���
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
			TS_OutPutMsg(dPosInfo, "\nͬ������\n   ����ȷ�ϡ�����", 0,iSelect);
			return 1;
		}
	}

	else if (iRet ==-1) 
	{
		TS_OutPutMsg(dPosInfo, "\n���ͨ��ֵ�쳣�����г�����\n   ����ȷ�ϡ�����", 0,iSelect);
		R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,flag1);
		if (cpurspStatus == 128 || cpurspStatus ==147)
		{
			int k=0;
			for (k;k<2;k++)
			{
				R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,1);
				//			TS_OutPutMsg(dPosInfo, "\n��ѿ��ŵ���������\n   ����ȷ�ϡ�����", 0,iSelect);
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

//		TS_OutPutMsg(dPosInfo,"���ͨ��ֵʧ��",1,iSelect);
		if (iRet != 0 && i>3)
		{
			//���´ο���������Ȧ���Ȧ�泷��ʱ���ٴ��Զ��ύ��
			//pyct_data->status = 0;
		//	*party=FROMIC;
		//	*rspcode=ICSAYFAIL;
			pyct_data->yct_ack=0; //��Ҫ���������ύ
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

	if (iRet == 1) //������ֶ�������������
	{
		//*party = FROMCTSI;
		//*rspcode = ICTIMEOUT;
		moni=1;
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
		return -1;// -101�������������ʧ��
	}
	
	//���¶��� ���Ȧ��ɹ� ������ȷȦ��ɹ��ġ����Բ����ж�����
	
	flag1 = 1;

	for (int k=0;k<3;k++)
	{
		R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,flag1);
		Sleep(1000);
		//			TS_OutPutMsg(dPosInfo, "\n��ѿ��ŵ���������\n   ����ȷ�ϡ�����", 0,iSelect);
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
//	wrLog("��ֵǰ: %s",amo_bef);
//	wrLog("��ֵ���: %s",amount);
//	wrLog("��ֵ��: %s",amo_aft);
//	if (atuo_charge == 1)
//	{
//		CTools::HexToDec((char *)amount,amount);
//		wrLog("----------------:%s",amount);
//	}
	wrLog("��ֵǰ:%d",atoi(amo_bef));
	wrLog("��ֵ���:%d",atoi(amount));
	wrLog("��ֵ��:%d",atoi(amo_aft));
	if ( cpurspStatus == 0)
	{
		if (load_suc==1  || (atoi(amo_aft) == atoi(amo_bef) + atoi(amount) )) //���Ȧ��ɹ����߶�����ǰ���ֵ����ȵġ�
		{

			if (load_suc==1 ) //�����1����� //Ȧ����ȷ�ɹ�
			{
				bbzero(pyct_data->ticket_amo_aft);
				long int a = atoi(pyct_data->amount) + atoi(pyct_data->ticket_amo_bef);
				sprintf(pyct_data->ticket_amo_aft,"%12d",a);

			}
			wrLog("Ȧ��ɹ�");
			//��ӡȦ��ɹ�����
		}
		else
		{
			wrLog("Ȧ���д�����ǰ����ȷ�����г���");
			TS_OutPutMsg(dPosInfo,"Ȧ��ʧ�ܣ����г���",1,iSelect);
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
				//���´ο���������Ȧ���Ȧ�泷��ʱ���ٴ��Զ��ύ��
				//pyct_data->status = 0;
			//	*party=FROMIC;
			//	*rspcode=CONNCOMFAIL;
			//	return 10;
				pyct_data->yct_ack=0; 
			}			
			*party=FROMIC;
			*rspcode=CONNCOMFAIL;
			return 1;// 1�������������ʧ��
		}
	}

	wrLog("cpurspStatus:%d",cpurspStatus);
		if (cpurspStatus != 0)
		{
			moni = 1;
		}
		else
			moni = 0;
		
		//9.�ն������������Ȧ��״̬�ύ���ģ�P_cpu_load_submitȦ���ύ��
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
			//���´ο���������Ȧ���Ȧ�泷��ʱ���ٴ��Զ��ύ��
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
	
	
	//���ÿ���Ϣ
	int  CYCTYPTrade::R_PUB_SET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,char* logical_card,BYTE * query_info,char *SW1SW2)
	{
		
		int len=0,cmd=0,sndLen = 0,iRet = 0,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int bandRate=57600;
		bbzero(icData);
		bbzero(sndData);
		bbzero(rcvData);

		wrLog("��R_PUB_SET_READCARDINFO��");

		len = 2;
		cmd = 0x95; 
		CTools::hex_to_byte((BYTE *)logical_card,(BYTE *)icData,16);
		len+=8;
		memcpy(icData + 8,query_info, 48); //��ѯ��Ϣ
		len += 48;
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
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
				//CTools::byte_to_hex((BYTE *)rcvData+4, (BYTE * )SW1SW2, 2);//��״̬��
			}
			//else ��������������
			
			else
				
			{
				return -1;
				
			}
			
		}
		
		return 0;
		
	}
	
	//��ȡcpu���Ŀ���Ϣ ��ȡǮ������Ƭ״̬
	//flag:  0 - ��ֵǰ��ѯ���, 1 - ��ֵ���ѯ���
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
		
		wrLog("��R_CPU_GET_READCARDINFO��");
		
		len = 2;
		cmd = 0x79; 
		
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			cpurspStatus = rcvData[3];
			wrLog("cpurspStatus:%d",cpurspStatus);
			if( rcvData[3]==0x00)
			{
				BYTE mny[4+1];	
				CTools::byte_to_hex((BYTE *)rcvData+4,(BYTE *)pyct_data->pki_card,4);//pki������
				CTools::byte_to_hex((BYTE *)rcvData+8,(BYTE *)pyct_data->ticket_physicalNo,8);//������
				CTools::byte_to_hex((BYTE *)rcvData+16,(BYTE *)pyct_data->ticket_no,8); //�߼�����
				CTools::byte_to_hex((BYTE *)rcvData+24,(BYTE *)mny,4); //Ǯ�����
				
				if (flag == 0)
				{
					//Ȧ��ǰ
					//	getAsiAmo3(rcvData+24, 4, ); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
					CTools::HexToDec((char *)mny, pyct_data->ticket_amo_bef);
				}
				else if (flag ==1)
				{
					//Ȧ���
					//CTools::byte_to_hex((BYTE *)rcvData+24,(BYTE *)pyct_data->ticket_amo_aft,4); //Ǯ�����
					//	getAsiAmo3(rcvData+24, 4, pyct_data->ticket_amo_aft); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
					CTools::HexToDec((char *)mny, pyct_data->ticket_amo_aft);
				}
				bbzero(pyct_data->downLimit);
				bbzero(pyct_data->upperLimit);
				CTools::byte_to_hex((BYTE *)rcvData+28,(BYTE *)down_limit,1); //Ǯ������
				//memcpy(pyct_data->upperLimit,down_limit,1);
				memcpy(pyct_data->downLimit,rcvData+28,1);
				//CTools::byte_to_hex((BYTE *)rcvData+29,(BYTE *)up_limit,3);//���ͨ�������
				//memcpy(pyct_data->upperLimit,up_limit,3);
				memcpy(pyct_data->upperLimit,rcvData+29,3);
				//CTools::byte_to_hex((BYTE *)rcvData+32,(BYTE *)pyct_data->card_status,1);// ��Ƭ״̬
				pyct_data->card_status=rcvData[32];
				//CTools::byte_to_hex((BYTE *)rcvData+33,(BYTE *)count_info,16); //������Ϣ
				memcpy(count_info,rcvData+33,16);//������Ϣ
				CTools::byte_to_hex((BYTE *)rcvData+49,(BYTE *)pyct_data->SW1SW2,2); //��Ƭ����״̬��
				CTools::HexToDec((char *)down_limit,des_lim);
				
				switch (pyct_data->card_status) //���ؿ�Ƭ��״̬
				{
				case 0x00:
					wrLog("M1��");
					break;
				case 0x01:
					wrLog("cpu����M1����δ�ر�");
					break;
				case 0x02:
					wrLog("cpu����M1�Ѿ��ر�");
					break;
				case 0x03:
					wrLog("cpu����M1Ӧ���Ѿ�����");
					break;
					
				}
				
				wrLog("��ȡcpu������Ϣ���£�");
				wrLog("������:%s",pyct_data->pki_card);
			//	wrLog("�߼�����:%s",pyct_data->ticket_no);
				
				wrLog("Ǯ�����: %s",mny);
				if (flag == 0)
				{
					wrLog("Ȧ��ǰ��Ƭ���Ϊ(�Է�Ϊ��λ��ʮ���ƽ��): %s",pyct_data->ticket_amo_bef);//���ͨ���
				}
				else if (flag == 1)
				{
					wrLog("Ȧ���Ƭ���Ϊ(�Է�Ϊ��λ��ʮ���ƽ��): %s",pyct_data->ticket_amo_aft);//���ͨ���
				}	
				wrHex("��Ƭ����: ",(BYTE *)pyct_data->upperLimit,3);		//���ͨcpu������
				wrHex("��Ƭ����Ϊ��",(BYTE *)pyct_data->downLimit,1); //���ͨcpu������
				wrHex("������Ϣ��",count_info,16);
				wrLog("���ͨ����״̬:%s",pyct_data->SW1SW2); //���ͨ����״̬��
				
				NULL;
			}
			if (rcvData[3]==0x80 || rcvData[3]==147)
			{
					TS_OutPutMsg(dPosInfo, "\n��ѿ��ŵ���������\n   ����ȷ�ϡ�����", 0,iSelect);
			}
		}
			//else ��������������
			else
			{
				wrLog("��ֵʧ��"); 		
			}
			
		
	}
	
	
	//�������������� ������������ʼ�� R_SET_BLACKLIST_INIT
	
	int  CYCTYPTrade::R_SET_BLACKLIST_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data )
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		BYTE SW1SW2[2];
		
		wrLog("��R_SET_BLACKLIST_INIT��");
		bbzero(icData);
		bbzero(sndData);
		
		len = 2;
		cmd = 0x21; 
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)		
		{
			if( rcvData[3]==0x00)
			{
				
				bbzero(pyct_data->ticket_physicalNo); //������
				bbzero(pyct_data->ticket_no);  //�߼�����
				
				CTools::byte_to_hex((BYTE *) rcvData+4,(BYTE *)pyct_data->ticket_physicalNo,8);
				CTools::byte_to_hex((BYTE *) rcvData+12,(BYTE *)pyct_data->ticket_no,8);
				CTools::byte_to_hex((BYTE *) rcvData+20,(BYTE *)pyct_data->BLACKLIST_CARDINFO,16); //������
				CTools::byte_to_hex((BYTE *) rcvData+36,(BYTE *)pyct_data->SW1SW2,2); 
				memcpy(SW1SW2,(BYTE *) rcvData+36,2);
				
				wrLog("����������������:%s",pyct_data->ticket_physicalNo);
				wrLog("���������߼�����:%s",pyct_data->ticket_no);
				wrLog("��������ʼ����״̬:%s",pyct_data->SW1SW2);
				
				NULL;
			}
			//else ��������������
			
			else
			{
				return -1;
				
			}
			
		}
		
		return 0;
		
	}
	
	
	//P_SET_BLACKLIST() // ������ ����trade_info3
	
	int CYCTYPTrade::P_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,\
		BYTE * trade_info3, BYTE * rsp,BYTE * party,BYTE* rspcode)		
	{		
		BYTE sndData[255]; 
		BYTE rcvData[255];


		int len;
		int iRet=0;
		BYTE pack_data[255];

		int sndLen=0;//���ͱ��ĳ���
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[20];

		int out=0;
		char yct_sequence_NO[8+1];
		BYTE rsp_code[4+1];

		wrLog("��P_SET_BLACKLIST��");
		bbzero(sndData);
		bbzero(rcvData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(yct_sequence_NO);
		bbzero(rsp_code);
		bbzero(pack_data);
		
		len=0;//�����峤��

		CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //������
		len+=4;
		CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //������
		len+=8;
		CTools::hex_to_byte((BYTE* )pyct_data->ticket_no,pack_data+len,16); //�߼�����
		len+=8;
		CTools::hex_to_byte((BYTE*)pyct_data->BLACKLIST_CARDINFO,pack_data+len,32); //����������Ϣ
		wrHex("����������Ϣ: ",pack_data+len,16);
		len+=16;
		memcpy(pack_data+len,RFU,20);
		len+=20;

		iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB05" , (BYTE*)"0001" , 0x02 , 0x80 , \
			pack_data , len , rcvData , &rcvLen);
		if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			return 3;// 3�������ͨ����ʧ��
		}
		
		len = 8;
		wrHex("������ˮ��: ",rcvData + len,8);
		len+=8;
		//memcpy(pyct_data->yct_stan,rcvData + len,8);
		CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)pyct_data->yct_stan,8);//ϵͳ��ˮ��
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
			char Output_Msg[1024];
			int iSelect = 0;
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

		bbzero(trade_info3);
		memcpy(trade_info3,rcvData+len,32);
		wrHex("������Ϣ3: ",trade_info3,32);
		len += 32;
		

		return 0;
		
	}
	
	
	//��������������  ���ú�������R_SET_BLACKList
	
	int  CYCTYPTrade::R_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,BYTE *SW1SW2,BYTE *status)
	{
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;

		wrLog("��R_SET_BLACKLIST��");
		bbzero(icData);
		bbzero(sndData);
		bbzero(SW1SW2);
		bbzero(icData);
		
		len = 2;
		cmd = 0x22; 
		memcpy(icData,trade_info,32);
		len+=32;
		
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			status[0]=rcvData[3]; //���ض�����״̬
	
			if( rcvData[3]==0x00)
			{			
				memcpy(SW1SW2,(BYTE *) rcvData+4,2);
				bbzero(pyct_data->SW1SW2);
				memcpy(pyct_data->SW1SW2,(BYTE *) rcvData+4,2);
			}
			//else ��������������
			else
			{
				return -1;		
			}	
		}
		
		return 0;		
	}
	
	
	//P_BLACKLIST_SUBMIT // �������ύ
	
	int CYCTYPTrade::P_BLACKLIST_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,\
		BYTE *status, BYTE * SW1SW2 ,BYTE * party,BYTE* rspcode)
	{
		BYTE sndData [255];
		BYTE rcvData[255];


		int len;
		int iRet=0;
		BYTE pack_data[255];

		int sndLen=0;//���ͱ��ĳ���
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[20];

		int out=0;
		BYTE rsp_code[4+1];

		wrLog("��P_BLACKLIST_SUBMIT��");
		bbzero(sndData);
		bbzero(EN_SHID);
		bbzero(RFU);
		bbzero(rcvData);
		bbzero(rsp_code);
		
		len=0;//�����峤��

		bbzero(pack_data);		
		CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //������
		len+=4;
		CTools::hex_to_byte((BYTE *) pyct_data->yct_stan,pack_data+len ,16); //��ӦAB06�����е�ϵͳ��ˮ��
		wrHex("ϵͳ��ˮ��: ",pack_data+len,8);
		len+=8;
		CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //������
		len+=8;
		CTools::hex_to_byte((BYTE* )pyct_data->ticket_no,pack_data+len,16); //�߼�����
		len+=8;
		memcpy(pack_data+len,(char *)SW1SW2,2); //������ָ���ú�������R_SET_BLACKLIST���ص�SW1SW2
		len+=2;
	//	memcpy(pack_data+len,(char *)status,1); //���������ط��ص�ָ��status 
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
			return 3;// 3�������ͨ����ʧ��
		}
		
		len = 8;
		wrHex("������ˮ��: ",rcvData + len,8);
		len+=8;
		memcpy(pyct_data->yct_stan,rcvData + len,8);
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
		bbzero(pyct_data->note);		
		memcpy(pyct_data->note,rcvData + len,32);
		wrHex("��ע: ",(BYTE *)pyct_data->note,32);
		len+=32;	
		return 0;
	}
	
	//ִ�к���������
	int CYCTYPTrade:: EXCUTE_BLACKLIST(const TSDLLPosInfo dPosInfo,const int com,const int comType, PYCT_DATA pyct_data,\
		BYTE * query_info,BYTE *party, BYTE *rspcode)
	{
		BYTE rsp_message [48]; //��ѯ��Ϣ
		BYTE trade_info3[32];
		BYTE SW1SW2 [2];
		BYTE status[1+1];
		char *rsp = {0};
		int iRet=0;
		
		bbzero(trade_info3);
		bbzero(rsp_message);
		bbzero(status);

		R_PUB_SET_READCARDINFO(dPosInfo,com,comType,pyct_data->ticket_no,query_info,pyct_data->SW1SW2);
		iRet = R_SET_BLACKLIST_INIT(dPosInfo,com,comType,pyct_data); //�Ժ���������ʼ��
		if (iRet != 0)
		{
			char Msg[512];
			get_cardReader_errMsg((BYTE*)pyct_data->status,Msg);
			wrLog("����������: %s",Msg);
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

		wrLog("���������������");
			
		R_SET_BLACKLIST(dPosInfo,com,comType,pyct_data,trade_info3,SW1SW2,status); 
			
		//Ĭ���ύ�����ظ��ύ�ķ�ʽ��������ζ����ɹ������´ο�����Ȧ����߳�����ʱ���ٴ��ύ
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
			//���´ο���������Ȧ���Ȧ�泷��ʱ���ٴ��Զ��ύ��
			pyct_data->status = 0;
		}
// 		else if(strcmp(rsp,"AA010001")==0)
// 		{
// 			
// 			wrLog("�Ѿ�ִ�гɹ������ü�������������");
// 			return 0;
// 			
// 		}	
		*party = FROMCTSI;
		*rspcode = BLACKLIST;
		return 0;	
	}
	
	
	
	//�ն�����������Ϳ�Ƭͬ����ѯ�Ĳ���
	//rsp_message �Ƕ��������ص�����
	
	int  CYCTYPTrade::R_CARD_QRY_UPGRADE(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data, const int com, const int comType,BYTE *random_num,BYTE * rsp_message)
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		bbzero(icData);
		bbzero(sndData);
		
		wrLog("��R_CARD_QRY_UPGRADE��");
		len = 2;
		cmd = 0x25; 
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			cpurspStatus = rcvData[3];
			if( rcvData[3]==0x00)
			{
				len = 4;
				wrHex("PKI�����ţ�",rcvData+len,4);
				len += 4;
				wrHex("�����ţ�",rcvData+len,8);
				CTools::byte_to_hex((BYTE*)rcvData+len,(BYTE*)pyct_data->ticket_physicalNo,8);
				len += 8;
				//wrHex("�߼����ţ�",rcvData+len,8);
				len += 8;
				wrHex("�������",rcvData+len,8);
				CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)random_num,8);
				len += 8;
				CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)rsp_message,200);
				wrHex("����Ϣ��",rcvData+len,200);
			}
			//else ��������������		
			else
			{
				return -1;
				
			}
			
		}
		
		return 0;
		
	}
	
	//ʵ��cpu��ͬ��
	int CYCTYPTrade::P_M1_TO_CPU(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, \
		BYTE * upgrade_info, int  &next_status,BYTE *syn_order,BYTE * party,BYTE* rspcode)		
	{
		
		BYTE sndData [1024];
		BYTE rcvData[1024];
		int len;
		int iRet=0;
		BYTE pack_data[255];
		BYTE rsp[8];
		int sndLen=0;//���ͱ��ĳ���
		int rcvLen = 0;
		BYTE EN_SHID[16];
		BYTE RFU[36];
		BYTE temp_len[10];
		int out=0;
		SOCKET sd=0;
		CConnect con;
		char  yct_sequence_NO[8+1]; //�������
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
		
		wrLog("��P_M1_TO_CPU��");
		len=0;//�����峤��
		
		bbzero(pack_data);

		CTools::hex_to_byte((BYTE * )pyct_data->pki_card,pack_data+len,8);        //PKI������
		len+=4;
		CTools::hex_to_byte((BYTE * )pyct_data->ticket_physicalNo,pack_data+len,16); //������
		len+=8;
		CTools::hex_to_byte((BYTE * )pyct_data->ticket_no,pack_data+len,16);      //�߼�����
		len+=8;
		CTools::hex_to_byte((BYTE * )pyct_data->random_num,pack_data+len,16);  //�����
		len+=8;
		//memcpy(pack_data,upgrade_info,208);
		CTools::hex_to_byte((BYTE * )upgrade_info,pack_data+len,400);  //����Ϣ
		len+=200;
		memcpy(pack_data+len,RFU,36);
		len+=36;
		

		iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AB01" , (BYTE*)"0001" , 0x02 , 0x80 , \
			pack_data , len , rcvData , &rcvLen);
		if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			return 3;// 3�������ͨ����ʧ��
		}

		len = 8;
		wrHex("������ˮ��: ",rcvData + len,8);
		len+=8;
		memcpy(pyct_data->yct_stan,rcvData + len,8);
		wrHex("����ϵͳ��ˮ��: ",rcvData + len,8);
		len+=8;
		CTools::byte_to_hex((BYTE *) rcvData+len,rsp,4);
		wrLog("�����룺%s",rsp);
		len+=4;

		if(strcmp((char *)rsp,"00000000")==0)
		{		
			next_status=rcvData[len];
			len += 1;
			memcpy(syn_order,rcvData+len,64);//ͬ��ָ��
			wrHex("ͬ��ָ�",syn_order,64);
			len+=64;
			wrHex("��ע��",rcvData+len,80);
			len += 80;				
		}
/*		
		else if(strcmp((char *)rsp,"AA01AB01")==0)
		{
					
			wrLog("��δ�������г���¼��ͬ��������ֹ");
			TS_OutPutMsg(dPosInfo,"��δ�������г���¼��ͬ��������ֹ!",1,iSelect);
			return -1;
		}				
		else if(strcmp((char *)rsp,"AA01AB02"))		
		{
			wrLog("��δ��������¼��ͬ��������ֹ");
			TS_OutPutMsg(dPosInfo,"��δ��������¼��ͬ��������ֹ!",1,iSelect);
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
			CTools::byte_to_hex((BYTE *)rcvData+len,(BYTE *)error_info,50);//������Ϣ����
			CTools::tobyte(error_info,byte_errinfo);
			CTools::trim(byte_errinfo);
			wrLog("������Ϣ����: %s",byte_errinfo);	
			bbzero(outmsg);
			sprintf(outmsg,"��ֵ��̨���ش���\n\n������Ϣ������%s",byte_errinfo);
			TS_OutPutMsg(dPosInfo,outmsg,1,iSelect);
			return 1;
		}

	return 0;		
}
	
	
	//��ͬ��Ӧ��
	int  CYCTYPTrade::R_APP_CARD_UPGRADE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE* upgrade_info)
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		BYTE SW1SW2[2];
		
		bbzero(icData);
		bbzero(sndData);
		bbzero(SW1SW2);
		wrLog("��R_APP_CARD_UPGRADE��");
		len = 2;
		cmd = 0x26; 
		memcpy(icData,upgrade_info,64);
		len+=64;
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			pyct_data->status = rcvData[3];
			if( rcvData[3]==0x00)
			{
				bbzero(pyct_data->SW1SW2);
				CTools::byte_to_hex((BYTE *)rcvData+4, (BYTE * )pyct_data->SW1SW2, 2);//��״̬��
				
				NULL;
			}
			//else ��������������
			
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
        
		wrLog("��EXCUTE_M1_TO_CPU��");

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
			TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������!",1,iSelect);
			for (int i=0; i<2; i++)
			{
				iRet=R_CARD_QRY_UPGRADE(dPosInfo,pyct_data,com,comType,random_num,rsp_message);
				if (cpurspStatus == 0)
				{
					break;
				}
				TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������!",1,iSelect);
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
		//wrHex("����������",(BYTE*)next_status,1);
		if(iRet==0)
		{
			switch (next_status)
			{		
				
			case 0x00:				
				wrLog("��Ƭͬ�������ת���Ѿ����,�������ֵ");
				TS_OutPutMsg(dPosInfo,"ͬ����ɣ���enter������ֵ",1,iSelect);
				break;
				
			case 0x01:				
				wrLog("���к�����Ƭͬ������");
				M1_CPU = 1;
				//��ִ��ͬ��ָ��
				iRet = R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //��Ӧ��ͬ��
				if (iRet != 0)
				{
					//ִ��ͬ��ָ��ʧ����ִ�г�ֵ���Ͳ�ѯ
					TS_OutPutMsg(dPosInfo,"��ѿ����ڶ�������!",1,iSelect);
					char errMsg[1024];
					bbzero(errMsg);
					get_cardReader_errMsg((BYTE *)pyct_data->status,errMsg);
					wrLog("���������ش�����Ϣ��%s",errMsg);
					*party = FROMIC;
					*rspcode = R_APP_CARD_UPGRADE_EXEFAIL;
//					return 1;
				}
				iRet = EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,query_info,party,rspcode);
				break;
				
			case 0x02:	
				wrLog("��Ƭͬ���Ѿ���ɣ����ն˷���M1���ת��");//
//				R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //��Ӧ��ͬ��
				iRet = R_APP_CARD_UPGRADE(dPosInfo,com,comType,pyct_data,syn_order); //��Ӧ��ͬ��
				if (iRet != 0)
				{
					//ִ��ͬ��ָ��ʧ����ִ�г�ֵ���Ͳ�ѯ
					TS_OutPutMsg(dPosInfo,"��ѿ����ڶ�������!",1,iSelect);
					char errMsg[1024];
					bbzero(errMsg);
					get_cardReader_errMsg((BYTE *)pyct_data->status,errMsg);
					wrLog("���������ش�����Ϣ��%s",errMsg);
					*party = FROMIC;
					*rspcode = R_APP_CARD_UPGRADE_EXEFAIL;
//					return 1;
				}

				iRet =	CPU_EnqPay(dPosInfo,com,comType,0,pyct_data,"000000000000",0x10,query_info,party,rspcode);
				bbzero(pyct_data->ticket_amo_bef);
				strcpy(pyct_data->ticket_amo_bef,pyct_data->ticket_amo_aft);
				bbzero(pyct_data->ticket_amo_aft);
				wrLog("��Ƭͬ�������ת���Ѿ����,�������ֵ");
				TS_OutPutMsg(dPosInfo,"ͬ����ɣ���enter������ֵ",1,iSelect);
				break;
				
			case 0x03:			
				wrLog("��Ƭͬ��δ��ɣ��쳣�˳�");
				TS_OutPutMsg(dPosInfo,"��Ƭͬ��δ��ɣ��쳣�˳�!",1,iSelect);
				*party = FROMCTSI;
				*rspcode = TRANSFER_ERROR;
				return 1;			
			default:			
				wrLog("δ֪ԭ���쳣�˳� %d",next_status);
				*party = FROMCTSI;
				*rspcode = TRANSFER_ERROR;
				return 1;			
			}
			
			return 0;			
		}
		else
		{
			wrLog("��Ƭͬ��δ��ɣ��쳣�˳���");
			*party = FROMCTSI;
			*rspcode = TRANSFER_ERROR;
			return 1;
		}	
	}
	
	
	
	//����M1 Ȧ���ѯ
	//appType:0x00Ϊ�̻�Ȧ�棬0x01 Ϊ����Ȧ��.0x20:��ֵ���˻���ֵ;0x30Ϊ�������ת��;0x40 Ϊ�쳣��Ϣ�ύ;0x50 :��չӦ��
	
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
		int sndLen=0;//���ͱ��ĳ���
		int rcvLen = 0; //���ձ���
		BYTE EN_SHID[16];
		BYTE RFU[23];
		BYTE MAC[4+1];
		int out=0;
		SOCKET sd;
		CConnect con;
		BYTE yct_sequence_NO[8];
		BYTE outside_info[16];
		BYTE  expand_serial[8];
		BYTE charge_password[16]; //��ֵ����
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
		
		len=0;//�����峤��
		sndLen=0;
		
		CTools::hex_to_byte((BYTE *)"AA61",type,4);
		CTools::hex_to_byte((BYTE *)"0001",ver,4);
		
		memcpy(sndData,type,2); //��Ϣ������
		memcpy(sndData+2,ver,2); //���İ汾��
		sndData[4]=0x02; //����������㷨
		sndData[5]=0x80; //��ֵ�����ֽ�˳��
		sndLen+=6;
		
		bbzero(pack_data);
		
		CTools::hex_to_byte((BYTE *) pyct_data->SHID,pack_data,16);  //SHID
		len+=8;
		sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
		CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //�������//���ն˽��й���ÿ���ϴ�������������+1
		len+=4;
		CTools::hex_to_byte((BYTE *)pyct_data->pki_card,pack_data+len,8);//PKI ������
		len+=4;
		CTools::hex_to_byte((BYTE *) pyct_data->ticket_physicalNo,pack_data+len,16); //������
		len+=8;
		CTools::hex_to_byte((BYTE *) pyct_data->ticket_no,pack_data+len,16); //�߼�����
		len+=8;
		memcpy(pack_data+len,pysical_info1,32);
		len+=32;
		
		pack_data[len]=appType; //Ӧ������ 
		len+=1;
	//	CTools::hex_to_byte((BYTE *)trade_money,pack_data+len,8);
	    getHexAmo((const char *)trade_money,hex_amoney);
		memcpy(pack_data+len,hex_amoney,4);
		
		//CTools::hex_to_byte((BYTE *)trade_money,(BYTE *)pack_data+len,8);//���׽�� �����Գ�ֵ1��Ǯ.���ڽ���30 ����40 ����ĳ�ֵ�������Ч�ġ�
		len+=4;

		if (appType != 0x01)
		{
			CTools::hex_to_byte((BYTE *)"000000000000000000000000000000000000000000000000",(BYTE *) pack_data+len,32);
		}
		else
		{
			CTools::hex_to_byte((BYTE *)pyct_data->card_no,(BYTE *) pack_data+len,32); //����Ȧ��ʱ��������ۿ�������˺�
		}

		len+=16;
		CTools::hex_to_byte((BYTE * ) pyct_data->stan,outside_info,16);//���ϵͳ��ˮ�ţ��������
		memcpy(pack_data+len,outside_info,16);//�ⲿϵͳ������Ϣ�ⲿ��Ϣ�����12λϵͳ�ο���
		len+=16;
		
		wrHex("outside_info",outside_info,16);
		
		//������չ��ˮ.Ӧ������Ϊ0x50 ����0x30��ʱ��������չӦ�û��߿�תֵ����ˮ�ţ���0x50��0x30ȫ������0x00
		if (appType==0x50)
		{
			//������չӦ��
			memcpy(pack_data+len,pyct_data->extension_stan,8); //��չ��ˮ��
		}		
		else if (appType==0x30)
		{
			//���뿨ת����ˮ��
			memcpy(pack_data+len,pyct_data->transfer_stan,8); //��תֵ��ˮ��
		}
		else
		{
			memcpy(pack_data+len,expand_serial,8); //��չ��ˮ��
		}

		len+=8;
		
		if ((bank_signed == 1) && (atoi(ornotpassword)!=0))
		{
			strcpy(tradeInfoSend.Contents[0].acTitle, "����������ǩԼ�����룺");	
			tradeInfoSend.Contents[0].uCommand = TS_INPUT_PWD;	//����
			tradeInfoSend.Contents[0].uCardType = CARD_TYPE;//������
			tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME;//��ʱ30��
			tradeInfoSend.Contents[0].uMinLen = 6;
			tradeInfoSend.Contents[0].uMaxLen = 6;
			iRet = TS_TradeControl(&dPosInfo, &tradeInfoSend, &tradeInfoRecv);
			//		char charge_sig[16];
			if(iRet != 0)
			{
				wrLog("TS_TradeControl���ش���[%d]", iRet);
				TS_OutPutMsg(dPosInfo, "TS_TradeControl���ش���\n   ����ȷ�ϡ�����", 0,iSelect);
				//		wrMonitorLog(1, 1, iRet, gbl, pack);	//�����־
				return iRet;
			}
			
			md5_csum((BYTE *)tradeInfoRecv.Contents[0].uMsg,tradeInfoRecv.Contents[0].uMsgLen,charge_password);
			wrHex("charge_password ���ܺ� :",charge_password,16);
			
		}

//		bank_signed = 0;
		memcpy(pack_data+len,charge_password,16); //��ֵ����
		len+=16;
		memcpy(pack_data+len,RFU,23); //RFU
		len+=23;
		wrHex("RFU",RFU,23);
		
		//����mac
		bbzero(md5_digest);
		md5_csum(pack_data,len,md5_digest);
		memcpy(MAC,md5_digest,4); //У����
		memcpy(pack_data+len,MAC,4);
		len+=4;
		wrLog("len:%d",len);
		wrHex("aa61 MAC",MAC,4);
		wrHex("aa61 pack_data",pack_data,len);
		
		sprintf((char *)len_tmp,"%04X",len);
		CTools::hex_to_byte((BYTE *)len_tmp,sndData+sndLen,4);//�����峤��
		sndLen+=2;
		CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //ʹ��3des��SHID
		CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16); 
		sndLen+=8;
		
		BYTE *enrpt_packdata=AESEncryptEx(pack_data+8,144,pyct_data->SKEY,16,&out); //ʹ��skͨ��AES����
		wrHex("aa61��Ҫ���ܵ�����:",pack_data+8,144);
		
		wrHex("aa61:",enrpt_packdata,out);
		
		memcpy(sndData+sndLen,enrpt_packdata,out);
		sndLen+=out;
		
		//���ͨ��������
		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3�������ͨ����ʧ��
		}
		
		bbzero(rcvData);
		iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //�����ݷ��͸����ͨ
		
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
			
			//����SHID
			CTools::byte_to_hex((BYTE *)rcvData+8,(BYTE *)EN_SHID,8);
			
			CDes::TripleDesDec((char *)CKEY,(char* )EN_SHID, (char *)pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_Data,16);
			packLen+=8;
			
			BYTE * aes_rcvData=AESDecryptEx((BYTE *) rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out); //����������ˮ��֮��ı���������
			wrHex("aa62 aes_rcvData",aes_rcvData,out);
			memcpy(decrpt_Data+packLen,aes_rcvData,out-4);
			packLen+=out-4;
			memcpy(mac_this,aes_rcvData+(out-4),4); //��ȡmac_this
			
			//����macУ��
			bbzero(md5_digest);
			md5_csum(decrpt_Data,packLen,md5_digest);
			memcpy(MAC,md5_digest,4); //У����
			
			if(memcmp((char*)mac_this,(char *)MAC,4)==0)
			{
				
				wrLog("MACУ��ɹ�");
				bbzero(pyct_data->yct_stan);
				bbzero(rspcode);
				
				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)pyct_data->yct_stan,8); //����ϵͳ��ˮ��
				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)pyct_data->yct_serial,8); //����ϵͳ��ˮ��
				wrLog("���ͨϵͳ��ˮ��:%s",pyct_data->yct_stan);
				CTools::byte_to_hex((BYTE *) aes_rcvData+8,rsp,4); //��������벻��0x00000000 �������Ǵ�����Ϣ����
				if (strcmp((char *)rsp,"00000000")==0)
				{
					CTools::byte_to_hex((BYTE *) aes_rcvData+12,back_time,7); //��̨ʱ��
					memcpy(query_info,aes_rcvData+19,48); // ��ѯ��Ϣ
					
					wrHex("query_info",query_info,48);
					wrLog("��ֵ��̨ʱ��Ϊ��%s",back_time);
					
				}
				else
				{
					if ((strcmp((char *)rsp,"AA01E202")==0) && (bank_signed == 1))
					{
						TS_OutPutMsg(dPosInfo, "�˻����������ȷ�Ϻ�����!\n\n\n", 0,iSelect);
					}
					CTools::byte_to_hex((BYTE *)aes_rcvData+12,(BYTE *)error_info,50);//������Ϣ����
					CTools::tobyte(error_info,byte_errinfo);
					CTools::trim(byte_errinfo);
					TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				    wrLog("������Ϣ����: %s",byte_errinfo);	
					return 3;
				}
				
			}
			
		}
		else if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			closesocket(sd);
			return 3;// 3�������ͨ����ʧ��
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			closesocket(sd);
			return 1;// 1�������������ʧ��
		}
		
		return 0;
		
	}
	
	
	//M1ȡ����Ϣ
	
	int  CYCTYPTrade::R_M1_GET_CARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,int &rspStatus,int flag)
		
	{
		
		int len=0,cmd=0,sndLen,iRet,rcvLen=0;
		BYTE icData[255],sndData[255],rcvData[255];
		int baudRate=57600;
		BYTE trade_count[4];
		BYTE mny[8];
		int iSelect = 0;
		
		wrLog("��R_M1_GET_CARDINFO��");
		bbzero(icData);
		bbzero(sndData);
		bbzero(trade_count);
		
		len = 2;
		cmd = 0xB9; 
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
			
		{
			rspStatus=rcvData[3];
			wrLog("rspStatus:%d",rspStatus);
			if( rcvData[3]==0x00)
			{
				bbzero(pyct_data->pki_card);
				bbzero(pyct_data->ticket_no);
				CTools::byte_to_hex((BYTE *)rcvData+4,(BYTE *)pyct_data->pki_card,4); 
				CTools::byte_to_hex((BYTE *) rcvData+8,(BYTE *)pyct_data->ticket_physicalNo,8);// ������
				CTools::byte_to_hex((BYTE *) rcvData+16,(BYTE *)pyct_data->ticket_no,8); // �߼�����
				memcpy(trade_info,(char *)rcvData+24,88); //������Ϣ
				
				wrHex("trade_info:",trade_info,88);
				CTools::byte_to_hex((BYTE *)rcvData+112,(BYTE *)mny,4);// ��Ƭ���pyct_data->ticket_amo_bef
				if (flag == 0)
				{
					bbzero(pyct_data->ticket_amo_bef);
                 	getAsiAmo3(rcvData+112, 4, pyct_data->ticket_amo_bef); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
					wrLog("���ͨƱ�����ticket_amo:[%s]", pyct_data->ticket_amo_bef);//���ͨƱ�����
				}
			    else if (flag == 1)
			    {
					bbzero(pyct_data->ticket_amo_aft);
                    getAsiAmo3(rcvData+112, 4, pyct_data->ticket_amo_aft); //ȡ���Է�Ϊ��λ��ʮ���ƽ��
					wrLog("���ͨƱ�����ticket_amo:[%s]", pyct_data->ticket_amo_aft);//���ͨƱ�����
			    }
				
				CTools::byte_to_hex((BYTE *)rcvData+116,trade_count,2);
				wrLog("PKI������Ϊ:%s",pyct_data->pki_card);
				wrLog("������:%s",pyct_data->ticket_physicalNo);
				//wrLog("�߼�����:%s",pyct_data->ticket_no);
				
				
				wrLog("M1��Ƭ�����%s",mny); 
				return 0;
				
			}
			//else ��������������
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
	
	
	//M1������Ȧ��
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
		int sndLen=0;//���ͱ��ĳ���
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
		
		len=0;//�����峤��
		
		CTools::hex_to_byte((BYTE *)"AA63",type,4);
		CTools::hex_to_byte((BYTE *)"0001",ver,4);
		
		memcpy(sndData,type,2); //��Ϣ������
		memcpy(sndData+2,ver,2); //���İ汾��
		sndData[4]=0x02; //����������㷨
		sndData[5]=0x80; //��ֵ�����ֽ�˳��
		sndLen+=6;
		
		bbzero(pack_data);
		
		CTools::hex_to_byte((BYTE *) pyct_data->SHID,pack_data,16);  //SHID
		len+=8;
		sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
		CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //�������//���ն˽��й���ÿ���ϴ�������������+1
		len+=4;
		CTools::hex_to_byte((BYTE *)pyct_data->pki_card,pack_data+len,8);//PKI ������
		len+=4;
		CTools::hex_to_byte((BYTE *) pyct_data->ticket_physicalNo,pack_data+len,16); //������
		wrLog("�����ţ�%s",pyct_data->ticket_physicalNo);
		len+=8;
		CTools::hex_to_byte((BYTE *) pyct_data->yct_stan,pack_data+len,16) ; //ϵͳ��ˮ��
		wrLog("���ͨϵͳ��ˮ��:%s",pyct_data->yct_stan);
		len+=8;
		memcpy(pack_data+len,trade_info,88); //������Ϣ
		wrHex("������Ϣtrade_info",trade_info,88);
		len+=88;
		memcpy(pack_data+len,RFU,28);
		len+=28;
		//����mac
		bbzero(md5_digest);
		md5_csum(pack_data,len,md5_digest);
		memcpy(MAC,md5_digest,4); //У����
		
		memcpy(pack_data+len,MAC,4);
		len+=4;
		wrHex("AA63 ���ݰ�����֮ǰ:",pack_data+8,len-8);
		
		sprintf((char *)len_tmp,"%04X",len);
		wrLog("len_tmp:%s",len_tmp);
		CTools::hex_to_byte((BYTE *)len_tmp,sndData+sndLen,4);//�����峤��
		sndLen+=2;
		CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //ʹ��3des��SHID
		CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16); 
		sndLen+=8;
		BYTE *enrpt_packdata=AESEncryptEx(pack_data+8,144,pyct_data->SKEY,16,&out); //ʹ��skͨ��AES����
		
		memcpy(sndData+sndLen,enrpt_packdata,out);
		sndLen+=out;
		wrHex("aa63 ���ݰ�����֮��",sndData,sndLen);
		
		//���ͨ��������
		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3�������ͨ����ʧ��
		}
		
		iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //�����ݷ��͸����ͨ
		
		if( iRet==0  )
		{
			
			BYTE mac_this[4];
			//BYTE md5_digest[16];
			BYTE decrpt_Data[255];
			bbzero(MAC);
			bbzero(EN_SHID);
			int packLen=0;
			BYTE trade_time[14]; //��ֵʱ��
			
			bbzero(trade_time);
			bbzero(decrpt_Data);
			bbzero(md5_digest);
			bbzero(MAC);
			bbzero(mac_this);
			
			//CTools::byte_to_hex((BYTE *) rcvData+6,(BYTE *)packLen,2); //�����峤��
			
			//����SHID
			CTools::byte_to_hex((BYTE *)rcvData+8,(BYTE *)EN_SHID,8);
			CDes::TripleDesDec((char *)CKEY,(char* )EN_SHID, (char *)pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_Data,16);
			packLen+=8;
			
			BYTE * aes_rcvData=AESDecryptEx((BYTE *) rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out); //����������ˮ��֮��ı���������
			wrHex("aes_rcvData",aes_rcvData,out);
			memcpy(decrpt_Data+packLen,aes_rcvData,out-4);
			packLen+=out-4;
			
			memcpy(mac_this,aes_rcvData+(out-4),4);
			wrHex("aa64",mac_this,4);
			
			//����macУ��
			bbzero(md5_digest);
			md5_csum(decrpt_Data,packLen,md5_digest);
			memcpy(MAC,md5_digest,4); //У����
			wrHex("aa64 mac",MAC,4);

			if(memcmp((char*)mac_this,(char *)MAC,4)==0)
			{
				
				wrLog("MACУ��ɹ�");
				bbzero(pyct_data->yct_stan);
				bbzero(pyct_data->tradeTime);
				bbzero(pyct_data->TAC);
				bbzero(pyct_data->yct_termid);
				bbzero(pyct_data->trader_id);
				bbzero(pyct_data->trader_name);
				bbzero(pyct_data->out_logicalcard);
				bbzero(pyct_data->note);
				bbzero(rspcode);
				
				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)pyct_data->yct_stan,8); //����ϵͳ��ˮ��
				wrLog("ϵͳ�ο���:%s",pyct_data->yct_stan);
				CTools::byte_to_hex((BYTE *) aes_rcvData+8,rsp,4); //��������벻��0x00000000 �������Ǵ�����Ϣ����
				wrLog("rsp:%s",rsp);
				if (strcmp((char *)rsp,"00000000")==0)
				{
					
					memcpy(charge_info,aes_rcvData+12,24); //��ֵ��Ϣ
					CTools::byte_to_hex(aes_rcvData+36,(BYTE *)pyct_data->tradeTime,7);//��������ʱ��
					CTools::byte_to_hex(aes_rcvData+43,(BYTE *)pyct_data->charge_authcode,4); //��ֵƾ֤��
					CTools::byte_to_hex(aes_rcvData+47,(BYTE *)pyct_data->yct_termid,6); // ���ͨ�ն˱��
					CTools::byte_to_hex(aes_rcvData+53,(BYTE *)pyct_data->trader_id,8) ; //�̻����
					CTools::byte_to_hex(aes_rcvData+61,(BYTE *)pyct_data->trader_name,20); //�̻����
					CTools::byte_to_hex(aes_rcvData+81,(BYTE *)pyct_data->out_logicalcard,8) ;//ת�����߼����� //0x30 �����ת�Ƶ�ʱ����Ч
					CTools::byte_to_hex(aes_rcvData+89,(BYTE *)pyct_data->note,64); //��ע
					
					wrHex("aa64 ���س�ֵ��Ϣ",charge_info,24);
					wrLog("aa64����ʱ��:%s",pyct_data->tradeTime);
					wrLog("aa64 ��ֵƾ֤��:%s",pyct_data->charge_authcode);
					wrLog("aa64 ���ͨ�ն˱��:%s",pyct_data->yct_termid);
					wrLog("aa64 ���ͨ�̻����:%s",pyct_data->trader_id);
					wrLog("aa64 ���ͨ�̻���ƣ�%s",pyct_data->trader_name);
					//wrLog("aa64 ת�����߼����ţ�%s",pyct_data->out_logicalcard);
					wrLog("aa64 ��ע:%s",pyct_data->note);
					
					next_action=aes_rcvData[153]; //��������
					
					if(next_action==0x00)
					{
						wrLog("�����Ѿ����");
						return 0;
					}
					
					else
					{
						wrLog("������صĲ�������Ҫִ����չӦ�õĲ���");
						//ExtendApp(gbl,)
					}
					
				}
				else
				{
					char error_info[128];
					char byte_errinfo[128];
					wrLog("���׷�����:%s",rsp);
					if (memcmp(rsp,"AA01F001",8) == 0)
					{
						//��ֵ���Υ��ҵ�����
						*party = FROMYCT_O;
						*rspcode = AMOUNTRULE_VIALATION;	
					}
					else if (memcmp(rsp,"AA01F002",8) == 0)
					{
						//��ֵ��������
						*party = FROMYCT_O;
						*rspcode = AMOUNTRULE_VIALATION;	
					}
					else if (strcmp((char *)rsp,"AA01FD22")==0)
					{
						TS_OutPutMsg(dPosInfo," \n �˻�����!\n�밴��ȷ�ϡ�����",0,iSelect);
					}
					bbzero(error_info);
					bbzero(byte_errinfo);
					CTools::byte_to_hex((BYTE *)aes_rcvData+12,(BYTE *)error_info,50);//������Ϣ����
					CTools::tobyte(error_info,byte_errinfo);
					CTools::trim(byte_errinfo);
					wrLog("������Ϣ����: %s",byte_errinfo);
					
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
			return 3;// 3�������ͨ����ʧ��
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			closesocket(sd);
			return 1;// 1�������������ʧ��
		}
		
		return 0;
		
	}
	
	
	//�Կ�����R_M1_LOAD
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
		CTools::hex_to_byte((BYTE *) pyct_data->tradeTime,icData,14); //����ʱ��
		len+=7;
		memcpy(icData+7,charge_info,24); //��ֵ��Ϣ
		len+=24;
		sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
		wrHex("�·�����", (BYTE *)sndData, sndLen);
		if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
		
		{

			status=rcvData[3]; //���ض�����״̬
			if( rcvData[3]==0x00)	
			{
				
				BYTE mny[4+1];
				int  tradeType;
				char trade_count[2+1];
				bbzero(mny);
				
				bbzero(pyct_data->ticket_amo_aft);
				wrLog("M1��ֵ�ɹ�");
				memcpy(mny,rcvData+4,4); //���
				getAsiAmo3(rcvData+4, 4, pyct_data->ticket_amo_aft); //ȡ���Է�Ϊ��λ��ʮ���ƽ��,��λ��ǰ
				tradeType=rcvData[8];
				
				wrLog("tradeType:%d",tradeType); //��������
				memcpy(trade_count,rcvData+9,2); //���ͨƱ������
				pyct_data->trade_count=atoi(trade_count);
				
				wrLog("���ͨƱ������:%d",pyct_data->trade_count);
				wrLog("��ֵ֮������Ϊ:%s",pyct_data->ticket_amo_aft);	
				wrHex("��ֵ֮֮���Ʊ�����Ϊ",mny,4); 
				
				NULL;
			}
			//else ��������������
			
			else
			{
				return -1;
				
			}
			
		}
		else
			return -1; //������û�з�������
		
		return 0;
	}
	
	
	//M1������
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
		int sndLen=0;//���ͱ��ĳ���
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
		len=0;//�����峤��
		
		CTools::hex_to_byte((BYTE *)"AA65",type,4);
		CTools::hex_to_byte((BYTE *)"0001",ver,4);
		
		memcpy(sndData,type,2); //��Ϣ������
		memcpy(sndData+2,ver,2); //���İ汾��
		sndData[4]=0x02; //����������㷨
		sndData[5]=0x80; //��ֵ�����ֽ�˳��
		sndLen+=6;
		
		bbzero(pack_data);
		
		CTools::hex_to_byte((BYTE *) pyct_data->SHID,pack_data,16);  //SHID
		len+=8;
		sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
		CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //�������//���ն˽��й���ÿ���ϴ�������������+1
		len+=4;
		CTools::hex_to_byte((BYTE *) aa62_stan,pack_data+len,16); //��ӦAA62�����е�ϵͳ��ˮ��
		len+=8;
		CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //������
		len+=4;
		CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //������
		len+=8;
		CTools::hex_to_byte((BYTE* )pyct_data->ticket_no,pack_data+len,16); //�߼�����
		len+=8;
		CTools::hex_to_byte((BYTE *)reason,pack_data+len,2); //����ԭ�� ���� 01����ֵд��ʧ�ܣ�02��δ�յ�AA62���Ļ���δ������������ֵָ��
		len+=1;
		sprintf(hex_status,"%02X",status);
		CTools::hex_to_byte((BYTE *)hex_status,pack_data+len,2);//������ָ��R_M1_LOAD���ص�status���
		len+=1;
		getHexAmo((const char*)money,hex_amount);
		memcpy((char*)pack_data+len,hex_amount,4);
		//CTools::hex_to_byte((BYTE *)money,pack_data+len,4);
		len+=4;
		CTools::hex_to_byte((BYTE *)pyct_data->charge_authcode,pack_data+len,8);//��ֵƾ֤��
		//memcpy(pack_data+len,TAC,4); 
		len+=4;
		memcpy(pack_data+len,trade_info,88);
		len+=88;
		memcpy(pack_data+len,RFU,26);
		len+=26;
		//����mac
		md5_csum(pack_data,len,md5_digest);
		memcpy(MAC,md5_digest,4); //У����
		memcpy(pack_data+len,MAC,4); //ȡУ����ǰ��λ
		len+=4;
		
		sprintf((char *)temlen,"%04X",len);
		CTools::hex_to_byte((BYTE *)temlen,sndData+6,4);//�����峤��
		sndLen+=2;
		strcpy(temp_buffer,(char*)sndData);
		CTools::byte_to_hex((BYTE*)sndData,(BYTE *)temp_buffer,8);
		CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //ʹ��3des��SHID
		CTools::hex_to_byte((BYTE *)EN_SHID,sndData+8,16); 
		sndLen+=8;
		strcat(temp_buffer,(char *)pyct_data->SHID);
		//strcat(temp_buffer,(char*)pack_data+8);
		CTools::byte_to_hex((BYTE*)pack_data+8,(BYTE *)temp_buffer + 32,len-8);
		CDB::updateYctRollback(pyct_data,temp_buffer);

		BYTE *enrpt_packdata=AESEncryptEx(pack_data+8,160,pyct_data->SKEY,16,&out); //ʹ��SKEYͨ��AES ����
		wrHex("aa65��Ҫ���ܵ�����:",pack_data+8,160);
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
			return 3;// 3�������ͨ����ʧ��
		}
		
		iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data,com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //�����ݷ��͸����ͨ
		
		if( iRet==0  )
		{
			BYTE mac_this[10];
			BYTE decrpt_Data[255];
		    int packLen=0;

			bbzero(MAC);
			bbzero(EN_SHID);
			bbzero(decrpt_Data);
			
			//���Ľ��ܼ�����MACУ��
			
			//����SHID
			CTools::byte_to_hex((BYTE *)rcvData+8,EN_SHID,16);
			
			CDes::TripleDesDec((char *)CKEY,(char *)EN_SHID,(char *)pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_Data,16);
			packLen+=8;
			
			BYTE *des_rcvData=AESDecryptEx((BYTE *) rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out);; //���ܱ����������
			wrHex("des_rcvData",des_rcvData,out);
			memcpy(decrpt_Data+packLen,des_rcvData,out-4);
			packLen+=out-4;
			memcpy(MAC,des_rcvData+(out-4),4);//У����
			//CTools::byte_to_hex((BYTE*)des_rcvData+152,(BYTE *)MAC,4);
			//����macУ��
			bbzero(md5_digest);
			md5_csum(decrpt_Data,packLen,md5_digest);
			memcpy(mac_this,md5_digest,4); //У����
			wrHex("mac_this",mac_this,4);
			wrHex("aa66 mac",MAC,4);
			if(memcmp((char*)mac_this,(char *)MAC,4)==0)
			{
				
				wrLog("MACУ��ɹ�");
//				bbzero(pyct_data->yct_stan);
//				CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE *)pyct_data->yct_stan,8); //����ϵͳ��ˮ��
				wrLog("--------:%s",pyct_data->yct_stan);
				CTools::byte_to_hex((BYTE *) des_rcvData+8,(BYTE *)pyct_data->SW1SW2,4);//������ 
				if (memcmp(pyct_data->SW1SW2,"00000000",8) != 0)
				{
                    memcpy(error_message,des_rcvData+12,50);
					wrLog("���صĴ�����Ϣ��%s",error_message);
					CDB::updateYctuntxn(pyct_data,aa62_stan);
				}
				else
				{
//					TS_OutPutMsg(dPosInfo,"�����ɹ�",0,iSelect);
					wrLog("�����ɹ�");
					pyct_data->status=5;
					CDB::updateYctuntxn(pyct_data,aa62_stan);
				}
				CTools::byte_to_hex((BYTE *) des_rcvData,(BYTE *)pyct_data->note,48);//��ע��Ϣ
				next_action=des_rcvData[60];
				
			}
			
			else
				
			{
				
				wrLog("MAC У��ʧ��");
			}
			
		}

		else if( iRet==1  )
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;	
			closesocket(sd);
			return 3;// 3�������ͨ����ʧ��
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = ICTIMEOUT;
			closesocket(sd);
			return 1;// 1�������������ʧ��
		}
		
		
		return 0;
	}
	
	
	//ִ��M1Ǯ����Ȧ��
	int CYCTYPTrade::excute_M1_LOAD(const TSDLLPosInfo dPosInfo,const int com,const int comType,PYCT_DATA pyct_data,\
		int enq,BYTE *party,BYTE *rspcode,char *tradeamount,int payType)
	{
		int iRet=0,iDb=0;
		int readCard_iRet=0; //��ȡ��Ƭ�ķ�����Ϣ
		BYTE pysical_info [51]; //��Ƭ������Ϣ
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
		//4.�ն�����������·��Ͳ�ѯ��������Ϣ��ָ��
		iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType, pysical_info);
		if (iRet == 1)
		{
			TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",0,iSelect);
			for (int i = 0; i<2; i++)
			{
				iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType, pysical_info);
				if (iRet == 0)
				{
					break;
				}
				TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",0,iSelect);
			}
		}
		if(iRet != 0)
		{
			*party=FROMIC;
			*rspcode=ICTIMEOUT;
			TS_OutPutMsg(dPosInfo,"��ȡ��Ƭ��������Ϣ��������",0,iSelect);
			return 1;
		}
		
		if(iRet==0)
		{
			
			bbzero(SW1SW2);
			CTools::byte_to_hex((BYTE *)pysical_info,(BYTE *)pyct_data->ticket_physicalNo,8);//��ȡ������
			
			CTools::byte_to_hex((BYTE *)pysical_info+8,(BYTE *)pyct_data->ticket_no,8);//ȡ���߼�����
			memcpy(SAK,pysical_info+16,1); //sak
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
				TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
				*party = FROMIC;
				*rspcode = ERROR_FROMIC;
				return 10;// 1�������������ʧ��
			}

			wrLog("������:%s",pyct_data->ticket_physicalNo);
		//	wrLog("�߼�����:%s",pyct_data->ticket_no);
			CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
			wrHex("SAK",SAK,1);
			wrHex("card_pycicalInfo1",card_pycicalInfo1,32);
			
		}

		
		//pe
		
		//5.�ն����ֵ����������M1��ֵ��ѯ������
		iRet=P_M1_LOAD_QRY(dPosInfo,com,comType,pyct_data,(BYTE *)tradeamount,card_pycicalInfo1,payType,query_info,party,rspcode);//P_M1_LOAD_QRY Ȧ���ѯ(�̻�Ȧ��)
		
		if(iRet==0)
		{
			flag=0;
			bbzero(SW1SW2);
			//������ķ����ǳɹ��������������ָ��R_PUB_SET_READCARDINFO
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
			iRet=R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);//6.�ն������������Mȡ����Ϣָ��]
			if (rspStatus == 128 || rspStatus == 132)
			{
				TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
				for (int i = 0;i<2; i++)
				{
					iRet=R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);
					
					if (rspStatus != 128 && rspStatus != 132)
					{
						break;
					}
					TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
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
				sprintf(temp,"����:%s\n\n��ֵǰ�����:%.2fԪ\n\n����Enter����ֵ  ����Esc������",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100);
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
			//	sprintf(bef,"\n\n%sԪ",pyct_data->ticket_amo_bef);
				sprintf(temp,"\n\n��ֵǰ�����:%.2fԪ\n\n����Enter����ֵ  ����Esc������",atof(pyct_data->ticket_amo_bef)/100);
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
			//PE�۷�
			CTools::getDate(pyct_data->txn_date);
		    CTools::getTime(pyct_data->txn_time);
			pyct_data->status=0;
			strcpy(pyct_data->biz_type,"0056");
			//strcpy(pyct_data->bank,"000000");
			//strcpy(pyct_data->term_serial,txn_serial);
			strcpy(pyct_data->yct_termid,dPosInfo.pos_no);
			strcpy(pyct_data->amount,tradeamount);
		//	iDb=CDB::insetYctYPtxnDATA(pyct_data,aa62_stan,0);
			//����M1��Ȧ��
			iRet=P_M1_LOAD(dPosInfo,com,comType,pyct_data,trade_info,next_action,charge_info,party,rspcode); //7.����������M1Ȧ��
			if (iRet != 0) 
			{
				//�����ͨ���������ͻع�����
				strcpy(reason,"02");
				for (int i=0;i<3;i++)
				{
					iRet=P_M1_ROLLBACK(dPosInfo,com,comType,status,pyct_data,reason,aa62_stan,(BYTE *)tradeamount,(BYTE *)pyct_data->TAC,trade_info,next_action,party,rspcode);
					if (iRet == 0)
					{
						break; 
					}
				}

				if (iRet !=0 || i>=3) //������ͨ����ʧ��  
				{
					pyct_data->yct_ack=0; //��Ҫ�����ͨ����������ȷ�ϱ���
					*party=FROMIC;
					*rspcode=ICSAYFAIL;
					return 1;
				}

				return 1;

			}

		   	iRet=R_M1_LOAD(dPosInfo,com,status,comType,pyct_data,charge_info); //8.�ն����ֵ���������صĳɹ����ĺ������������Ȧ��ָ�
			if (iRet==0 && status==0) //Ȧ��ɹ�
			{
				bbzero(bef_balance);
				bbzero(aft_balance);
				strcpy(bef_balance,pyct_data->ticket_amo_bef) ;
				strcpy(aft_balance,pyct_data->ticket_amo_aft);
			}

			//��ֵ֮�����ζ��� ͨ����������ֵ��ȷ����ֵ�Ƿ�ɹ���
			for (int i = 0;i<3; i++)
				
			{
					readCard_iRet=R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,1);
					
					if (readCard_iRet==0 && rspStatus == 0 )  //��������ɹ�
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
							TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
						}

						continue;

					}
				
			}
			

			if (iRet ==0 || readCard_iRet == 0 ) //���������Ȧ���з���
			{

				if (atoi(aft_balance)== atoi(bef_balance) + atoi(tradeamount))
				{
					pyct_data->status=2;
					//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
					wrLog("��ֵ�ɹ�");
					*party = FROMSUCC;
					*rspcode = SUCC_MSG;
				}

				else 
				{
					strcpy(reason,"01");
					//���½��г�������;
					
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
//					TS_OutPutMsg(dPosInfo,"��ֵʧ��",1,iSelect);
					*party=FROMIC;
					*rspcode=CONNCOMFAIL;
					return 5; 
				}

			}
			/*
			else if (status!=0 ) //���Ȧ����ȷʧ�� ����ʧ��
			{

				strcpy(reason,"01");
				//���½��г�������;
				
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
				//					TS_OutPutMsg(dPosInfo,"��ֵʧ��",1,iSelect);
				*party=FROMIC;
				*rspcode=CONNCOMFAIL;
					return 5; 

			}

			*/
			else  //Ĭ�ϳɹ�
			{
				moni = 1;
				pyct_data->status=2;
				//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
				wrLog("��ֵ�ɹ�");
				*party = FROMSUCC;
				*rspcode = SUCC_MSG;
			}

			//8.ִ����R_M1_LOADָ����Ҫ���¶�ȡ��Ƭ����ִ��R_M1_GET_CARDINFOָ��

/*			if(rspStatus ==0x00)
			{
				flag=1;
				R_M1_GET_CARDINFO(dPosInfo,com,comType,pyct_data,trade_info,rspStatus,flag);
				wrLog("rspStatus %d",rspStatus);
				if(rspStatus ==0x00)
				{
					flag=0;
					//���¶�ȡ��ֵ���˻�
					strcpy(aft_balance,pyct_data->ticket_amo_aft);
					if (atoi(aft_balance)== atoi(bef_balance) + atoi(tradeamount))
					{
                        pyct_data->status=2;
						//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
						wrLog("��ֵ�ɹ�");
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
					//���¶�ȡ��ֵ���˻�
					strcpy(aft_balance,pyct_data->ticket_amo_aft);
					if (atoi(aft_balance)== atoi(bef_balance) + atoi(tradeamount))
					{
                        pyct_data->status=2;
						//iDb=CDB::updateYctuntxn(pyct_data,aa62_stan);
						wrLog("��ֵ�ɹ�");
						*party = FROMSUCC;
						*rspcode = SUCC_MSG;
					    break;
					}
			        else
					{
                        wrLog("Ȧ���д�����ǰ����ȷ�����г���");
                        pyct_data->status=4;
						//CDB::updateYctuntxn(pyct_data,aa62_stan);
						strcpy(reason,"01");
						//���½��г�������;
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
					wrLog("�����·źÿ�");	
				}		
				else 
				{
		//			wrLog("Ȧ��ʧ�ܣ����г���");
		//			pyct_data->status=4;
		//			CDB::updateYctuntxn(pyct_data);
		//			strcpy(reason,"02");
		//			//���½��г�������;
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
					return 5;                //���Ƴɹ�		
				}
			}
*/
/*			if (flag != 0)
			{
				wrLog("M1�����ζ���ʧ��");
				//�����ͨ��̨�������
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
				return 5;                //��ֵʧ��	
			}
*/
		}
		else
		{
			TS_OutPutMsg(dPosInfo,"��ֵʧ��",1,iSelect);
			return 1;
			
		}		
		return 0;	
	}
	
	/**********************************************************************/
	/*                             CPU                                    */


//CPUȦ���ʼ��
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
	
	wrLog("��R_CPU_LOAD_INIT��");
	
	len = 2;
	cmd = 0x7A; 
	//CTools::byte_to_hex((BYTE *)trade_message1,icData,128);
	memcpy(icData,trade_message1,64);
	len+=64;
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		CTools::byte_to_hex((BYTE *)rcvData+3,yct_term_status,1); //���ͨ�ն�״̬
		wrHex("������� :",rcvData+3,1);
		if( rcvData[3]==0x00)
		{
			bbzero(SW1SW2);
			CTools::byte_to_hex(rcvData+4,trade_sequence,2); //�����������
			
			memcpy(card_info,rcvData+6,80);//����Ϣ
			CTools::byte_to_hex((BYTE *)rcvData+86,(BYTE *)pyct_data->SW1SW2,2);//��״̬��Ϣ
			
			wrLog("�����������:%s",trade_sequence);
			wrHex("����Ϣ: ",card_info,80);
			wrLog("CPUȦ�淵�ؿ�״̬Ϊ:%s",pyct_data->SW1SW2);
			
			NULL;
		}
		//else ��������������		
		else
		{
			wrLog("���������󣬴������ [%s], ������ĵ��д����壡",yct_term_status);
			// 			*party = FROMCTSI;
			// 			*rspcode = ICTIMEOUT;
			// 			closesocket(sd);
			// 			return 1;// 1�������������ʧ��
		}
	}
		return 0;
}//R_CPU_LOAD_INIT()

	
	//CPUȦ���ѯ
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
		BYTE type [2]; //��Ϣ������
		BYTE ver [2]; //���İ汾��
		char MAC[4];
		BYTE mny[12+1];
		BYTE RFU[22+1];
		BYTE rcvData[512];
		BYTE EN_SHID[16+1];
		int out=0;	
		int baudRate=57600;
		BYTE outside_info[16];//�ⲿϵͳ������Ϣ
		BYTE tuozhan_stan[8];
		char yct_sequence_NO[4+1];
		BYTE len_tmp[2];
		CConnect con;
	    BYTE hexAmo[10];
		BYTE *enrpt_packdata;
		BYTE md5_digest[16] = { 0 };
		int iSelect;

		wrLog("��P_CPU_LOAD_QRY��");
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
	
	sndLen=0;//���ͱ��ĳ���
	rcvLen = 0;
	
	len=0;//�����峤��
	
	CTools::hex_to_byte((BYTE *)"AA81",type,4);
	CTools::hex_to_byte((BYTE *)"0001",ver,4);
	memcpy(sndData,type,2); //��Ϣ������
	memcpy(sndData+2,ver,2); //���İ汾��
	
	sndData[4]=0x02; //����������㷨
	sndData[5]=0x80; //��ֵ�����ֽ�˳��
	sndLen+=6;
	
	bbzero(pack_data);
	bbzero(MAC);
	bbzero(trade_message1);
	
	//memcpy(pack_data,pyct_data->SHID,8); //������ˮ��
	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //������ˮ��
	len+=8;
	//sprintf(yct_sequence_NO,"%04x",pyct_data->yct_sequence_NO); //�������
	sprintf((char *)yct_sequence_NO,"%08X",pyct_data->yct_sequence_NO);
	CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,8); //�������//���ն˽��й���ÿ���ϴ�������������+1
	//CTools::hex_to_byte((BYTE * )yct_sequence_NO,pack_data+len,4);
	//memcpy(pack_data+len,yct_sequence_NO,4);
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card,pack_data+len,8); //pki������
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //������
	len+=8;
	
	CTools::hex_to_byte((BYTE*) pyct_data->ticket_no,pack_data+len,16);//�߼�����
	len+=8;
	pack_data[len]=pyct_data->card_status; //��Ƭ״̬
	len+=1;
	wrHex("count_info :",count_info,16);
	memcpy(pack_data+len,count_info,32); //ȡ����ָ��cpuȡ����Ϣ���ص�����
	len+=16;
	
    wrLog("appType : %d",(BYTE *)appType,1);
	pack_data[len]=appType; //�̻�Ȧ��
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
    memcpy(pack_data+len,hexAmo,4);//Ȧ����	
  
	len+=4;
	
	if (appType != 0x01)
	{
		CTools::hex_to_byte((BYTE *)"000000000000000000000000000000000000000000000000",(BYTE *) pack_data+len,32);
	}
    else
	{
		CTools::hex_to_byte((BYTE *)pyct_data->card_no,(BYTE *) pack_data+len,32); //����Ȧ��ʱ��������ۿ�������˺�
	}
	
	len+=16;
	//memcpy(outside_info,pyct_data->stan,16); 
	CTools::hex_to_byte((BYTE * ) pyct_data->stan,outside_info,16);//���ϵͳ��ˮ�ţ��������
	wrHex("outside_info :",outside_info,16);
	memcpy(pack_data+len,outside_info,16); //�ⲿϵͳ������Ϣ�������̻�����¼�����Ա��ż������̻��Ľ�����ˮ�ŵ���Ϣ�����ݱ����ڶ����ļ���
	len+=16;
	

	//������չ��ˮ.Ӧ������Ϊ0x50 ����0x30��ʱ��������չӦ�û��߿�תֵ����ˮ�ţ���0x50��0x30ȫ������0x00
	if (appType==0x50)
	{
		//������չӦ��
		wrHex("pyct_data->extension_stan :",(BYTE *)pyct_data->extension_stan,8);
		memcpy(pack_data+len,pyct_data->extension_stan,8); //��չ��ˮ��
	}		
	else if (appType==0x30)
	{
		//���뿨ת����ˮ��
		wrHex("pyct_data->transfer_stan :",(BYTE *)pyct_data->transfer_stan,8);
		memcpy(pack_data+len,pyct_data->transfer_stan,8); //��תֵ��ˮ��
	}
	else
	{
		CTools::hex_to_byte((BYTE *)"0000000000000000",(BYTE *) pack_data+len,16);//��0X50 ��0x30����Ӧ��ȫ��0x00
	}
	len+=8;
//	wrLog("---------------------------%d",atoi(ornotpassword));
	if ((bank_signed == 1) && (atoi(ornotpassword)!=0))
	{
		strcpy(tradeInfoSend.Contents[0].acTitle, "����������ǩԼ�����룺");	
		tradeInfoSend.Contents[0].uCommand = TS_INPUT_PWD;	//����
		tradeInfoSend.Contents[0].uCardType = CARD_TYPE;//������
		tradeInfoSend.Contents[0].uTimeOut = EPOS_OVER_TIME;//��ʱ30��
		tradeInfoSend.Contents[0].uMinLen = 6;
		tradeInfoSend.Contents[0].uMaxLen = 6;
		iRet = TS_TradeControl(&dPosInfo, &tradeInfoSend, &tradeInfoRecv);
//		char charge_sig[16];
		if(iRet != 0)
		{
			wrLog("TS_TradeControl���ش���[%d]", iRet);
			TS_OutPutMsg(dPosInfo, "TS_TradeControl���ش���\n   ����ȷ�ϡ�����", 0,iSelect);
			//		wrMonitorLog(1, 1, iRet, gbl, pack);	//�����־
			return iRet;
		}
		bbzero(charge_password);
	
//		char temp[16];
//		strcpy(temp,(char *)tradeInfoRecv.Contents[0].uMsg);
//		strupr(temp);
		//תΪʮ������
//		CTools::DecToHex(temp, charge_sig,16);
		md5_csum((BYTE *)tradeInfoRecv.Contents[0].uMsg,tradeInfoRecv.Contents[0].uMsgLen,charge_password);
		wrHex("charge_password ���ܺ� :",charge_password,16);

	}

//	bank_signed = 0;

//	memcpy(charge_password,(char *)"888888",16);
	memcpy(pack_data+len,charge_password,16);// ��ֵ����
	len+=16;
	wrHex("RFU :",RFU,22);
	memcpy(pack_data+len,RFU,22); //Ĭ��Ϊ0x00
	len+=22;

	//����mac
	//get_md5(pack_data,md5_digest);
	wrHex("�����壨û��MAC��:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4); //У����
	memcpy(pack_data+len,MAC,4); //ȡУ����ǰ��λ
	len+=4;
	wrHex("mac",(BYTE *)MAC,4);
	wrHex("pack_data:",(BYTE *)pack_data,len);
	
	//SHID ʹ��3DES����;
	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID);
	wrLog("����ǰ��SHID: %s",pyct_data->SHID);
	wrLog("���ܺ��SHID: %s",EN_SHID);

	enrpt_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,pyct_data->SKEY,16,&out); //��������ˮ�����ⱨ��ʹ��skͨ��AES����

	wrHex("AA81��Ҫ���ܵ����� :",pack_data+8,len-8);
	wrHex("AA81���ܺ������   :",enrpt_packdata,out);
	
	wrLog("�������ܳ���: %d",len);
	sprintf((char *)len_tmp,"%04x",len);
	//memcpy(sndData,len_tmp,2); //�����峤��
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //�����峤��
	sndLen+=2;
	CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16);
	sndLen+=8;
	memcpy(sndData+sndLen,enrpt_packdata,out);//��������
	sndLen+=out;

//	md5_csum(decrpt_data,sndLen,md5_digest); //����md5����

	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}

	iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data,com, comType, sd, sndData,sndLen, rcvData, &rcvLen); //�����ݷ��͸����ͨ
	
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
		CDes::TripleDesDec((char * )CKEY,(char *)EN_SHID,(char *)pyct_data->SHID);//������ˮ��
		wrLog("������ˮ����:%s",pyct_data->SHID);
		CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
		packLen+=8;
		
		BYTE *des_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,(BYTE *)pyct_data->SKEY,16,&out); //������ʹ��SKEY���н���
		memcpy(decrpt_data+packLen,des_rcvData,out-4); //
		packLen+=out-4;
		
		wrHex("���ܱ����壨��������ˮ�ţ�",des_rcvData,out);
		memcpy(mac_this,des_rcvData+(out-4),4); //��ȡMAC
		
		//��Ҫ����md5�����ݣ�
		wrHex("decrpt_data",decrpt_data,packLen);
		//����macУ��
		bbzero(md5_digest);
		md5_csum(decrpt_data,packLen,md5_digest); //����md5����
		memcpy(MAC,md5_digest,4); //У����
		wrHex("MAC",(BYTE *)MAC,4);
		wrHex("mac_this",mac_this,4);
		
		if(memcmp(MAC,mac_this,4)==0)
		{
			char rsp_code[8+1];
			BYTE sys_time[14+1];
			BYTE NOTE [96+1];
			char error_info[128];
			char byte_errinfo[128];
			
			wrLog("MACУ��ɹ�");

			CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE*)pyct_data->yct_stan,8); //���ͨ��ֵ��ˮ��
			CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE*)pyct_data->yct_serial,8); //���ͨ��ֵ��ˮ��
			CTools::byte_to_hex((BYTE *)des_rcvData+8,(BYTE *)rsp_code,4);//������
			//��������벻��0x00000000 �������Ǵ�����Ϣ����
			if (strcmp((char *)rsp_code,"00000000")!=0)
			{
			    wrLog("������:%s",rsp_code);
				bbzero(error_info);
				bbzero(byte_errinfo);
				CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)error_info,50);//������Ϣ����
				CTools::tobyte(error_info,byte_errinfo);
				CTools::trim(byte_errinfo);
				//TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				wrLog("������Ϣ����: %s",byte_errinfo);	
				if ((strcmp((char *)rsp_code,"AA01E202")==0) && (bank_signed == 1))
				{
					TS_OutPutMsg(dPosInfo, "�˻����������ȷ�Ϻ�����!\n\n\n", 0,iSelect);
				}
				return 1;
			}
			else
			{
				//CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)trade_message1,64);//���ؽ�����Ϣ1
				memcpy(trade_message1,(BYTE *)des_rcvData+12,64);
				CTools::byte_to_hex((BYTE *)des_rcvData+76,(BYTE *)sys_time,7);//����ϵͳʱ��			
				CTools::byte_to_hex((BYTE *)des_rcvData+83,(BYTE *)NOTE,48);//Ȧ���ѯ���ص�˵����Ϣ		
				wrLog("������ˮ��Ϊ:%s",pyct_data->SHID);
				wrLog("ϵͳ��ˮ��:%s",pyct_data->yct_stan);
				wrLog("������:%s",rsp_code);
				wrHex("������Ϣ1��",trade_message1,64);
				wrLog("Ȧ��˵����Ϣ:%s",NOTE);		
				wrLog("ϵͳʱ��Ϊ��%s",sys_time);
				//wrLog("���ͨ����Ȧ��ϵͳ�ο���Ϊ��%s",pyct_data->yct_stan);
			}
		}		
		else
		{
			wrLog("���ݰ�MACУ�����");	
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
	
	wrLog("��R_CPU_LOAD��");
	
	bbzero(icData);
	bbzero(sndData);
	
	len = 2;
	cmd = 0x7B; 
	wrHex("trade_message2 :",trade_message2,32);
	//CTools::hex_to_byte(trade_message2,icData,64);
	memcpy(icData,trade_message2,32); //������Ϣ2
	wrHex("icData :",icData,32);
	len +=32;
	
	sndLen = ST_packData(ST_HEAD, len, cmd, (BYTE *)icData, (BYTE *)sndData);	
	wrHex("�·�����", (BYTE *)sndData, sndLen);
	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)rcvData, &rcvLen, baudRate)) == 0)
	{
		
		load_status = rcvData[3];
		//memcpy(load_status,rcvData[3],1);
		//CTools::byte_to_hex((BYTE *)rcvData+3,yct_term_status,1); //���ͨ�ն�״̬
        wrHex("������� :",rcvData+3,1);
		if( rcvData[3]==0x00) //�����ֵ״̬Ϊ00����Ȧ��ɹ�
		{
			//load_status=0;
			//bbzero(SW1SW2);
			bbzero(pyct_data->SW1SW2);
			CTools::byte_to_hex(rcvData+4,TAC,4); //������֤��
			CTools::byte_to_hex(rcvData+8,(BYTE *)pyct_data->SW1SW2,2);//��Ƭ����״̬��
			//memcpy(SW1SW2,rcvData+8,2);
			wrLog("������֤��Ϊ:%s",TAC);
			wrLog("��Ƭ����״̬��:%s",pyct_data->SW1SW2);
			
			
			//�����Ƭ״̬��������ox9000 �Ա�д��ǰ�������ֵ��ȷ����ʾȦ��ɹ�
			if(strcmp(pyct_data->SW1SW2, "9000") == 0)
			{
				
				return 0;
			}
			
			return 0;
		}
		//else ��������������		
		else
		{
//			load_status;
			wrLog("Ȧ��ʧ��");
			return -1;
			//R_CPU_GET_READCARDINFO(dPosInfo, com, comType,pyct_data,count_info,1);
			//��Ҫ���¶������󣬽�������ط��������ն˲��ܴ�ӡȦ�浥�ݣ���ʾȦ��ʧ��	
			
		}
		//���������ִ��R_cpu_loadָ������������Ӧ����Ҫ���¶�ȡ����������ɹ������Ҽ�ֵ��ȷ����ʾȦ��ɹ���
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
	BYTE type [2]; //��Ϣ������
	BYTE ver [2]; //���İ汾��
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


  	wrLog("��P_CPU_LOAD��");

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(txn_serial);
	sndLen=0;//���ͱ��ĳ���
	rcvLen = 0;
	
	len=0;//�����峤��
	
	CTools::getYct_posSerial(txn_serial);
	//strcpy(pyct_data->term_serial,txn_serial);
	CTools::hex_to_byte((BYTE *)"AA83",type,4);
	CTools::hex_to_byte((BYTE *)"0001",ver,4);
	memcpy(sndData,type,2); //��Ϣ������
	memcpy(sndData+2,ver,2); //���İ汾��
	sndData[4]=0x02; //����������㷨
	sndData[5]=0x80; //��ֵ�����ֽ�˳��
	sndLen+=6;
	
	bbzero(pack_data);
	bbzero(MAC);
	bbzero(enrpt_packdata);

	//memcpy(pack_data,pyct_data->SHID,8); //������ˮ��
	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //������ˮ��
	len+=8;
	//CTools::hex_to_byte((BYTE * )pyct_data->yct_sequence_NO,pack_data,4); //�������//���ն˽��й���ÿ���ϴ�������������+1
	sprintf((char *)yct_sequence_NO,"%08X",(unsigned int )pyct_data->yct_sequence_NO);  //���ͨ������ţ�����Ϊhex��
	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8); 
	len+=4;
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,pack_data+len,8); //������
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo,pack_data+len,16); //������
	len+=8;
	CTools::hex_to_byte((BYTE *) pyct_data->ticket_no,pack_data+len,16); //�߼�����
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->yct_stan,pack_data+len,16); //ϵͳ��ˮ��
	len+=8;
	wrHex("card_info: ",card_info,80);
	memcpy(pack_data+len,card_info,80); //ȡ����ָ��cpuȡ����Ϣ���ص�����
	len+=80;
	CTools::hex_to_byte((BYTE *)pyct_data->SW1SW2,pack_data+len,4); //��Ƭ������sw1sw2
	len+=2;
	memcpy(pack_data+len,yct_term_status,1) ;//��Ӧ����������0xff
	len+=1;
	memcpy(pack_data+len,RFU,25);//Ĭ��Ϊ0x00
	len+=25;
	
	//����mac
	//	CMd5::md5_finish(pack_data,md5_digest); 
	wrHex("�����壨û��MAC��:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4); //У����
	memcpy(pack_data+len,MAC,4); //ȡУ����ǰ��λ
	len+=4;
	wrHex("mac",(BYTE *)MAC,4);
	wrHex("pack_data:",(BYTE *)pack_data,len);

	//memcpy(MAC,md5_digest,4); //У����
	//memcpy(pack_data,MAC,4); //ȡУ����ǰ��λ
	//len+=4;

	//����
	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)EN_SHID); //ʹ��3des��SHID
	wrLog("����ǰ��SHID: %s",pyct_data->SHID);
	wrLog("���ܺ��SHID: %s",EN_SHID);
	
	//enrpt_packdata=AES.AESEncryptEx(pack_data,len,pyct_data->SKEY,16,&out); //ʹ��SKEYͨ��AES ����
    BYTE *enc_packdata;
	enc_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,pyct_data->SKEY,16,&out); //��������ˮ�����ⱨ��ʹ��skͨ��AES����
	
	wrHex("AA83��Ҫ���ܵ����� :",pack_data+8,len-8);
	wrHex("AA83���ܺ������   :",enc_packdata,out);
	
// 	CTools::hex_to_byte((BYTE *)len,sndData+6,2);//�����峤��
// 	CTools::hex_to_byte(EN_SHID,sndData+8,16);
// 	CTools::hex_to_byte(enrpt_packdata,sndData+24,out);
// 	sndLen+=out;

	wrLog("�������ܳ���: %d",len);
	sprintf((char *)len_tmp,"%04x",len);
	//memcpy(sndData,len_tmp,2); //�����峤��
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //�����峤��
	sndLen+=2;
	CTools::hex_to_byte((BYTE *)EN_SHID,sndData+sndLen,16);
	sndLen+=8;
	memcpy(sndData+sndLen,enc_packdata,out);//��������
	sndLen+=out;

	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		return 3;// 3�������ͨ����ʧ��
	}
	
	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //�����ݷ��͸����ͨ
	
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
		
		//���Ľ��ܼ�����MACУ��
		
		//����SHID
		//CTools::byte_to_hex(rcvData+8,EN_SHID,16);
		CTools::byte_to_hex((BYTE *)rcvData+8,EN_SHID,8);
		bbzero(pyct_data->SHID);
		CDes::TripleDesDec((char * )CKEY,(char *)EN_SHID,(char *)pyct_data->SHID);//������ˮ��
		wrLog("������ˮ����:%s",pyct_data->SHID);

// 		//		CDes::TripleDesDec(CKEY,EN_SHID,pyct_data->SHID);
// 		
// 		//		des_rcvData=CAES::aes_decrypt(rcvData+16,len-16,pyct_data->SKEY,16,&out); //���ܱ����������
// 		CTools::byte_to_hex((BYTE*)des_rcvData+204,(BYTE *)MAC,4);//У����
// 		//����macУ��
// 		//		CMd5::md5_finish(rcvData,md5_this);
// 		memcpy((CHAR *)mac_this,md5_this,4);


		CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
		packLen+=8;
		
		des_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,pyct_data->SKEY,16,&out); //������ʹ��SKEY���н���
		memcpy(decrpt_data+packLen,des_rcvData,out-4); //
		packLen+=out-4;
		
		wrHex("���ܱ����壨��������ˮ�ţ�",des_rcvData,out);
		memcpy(mac_this,des_rcvData+(out-4),4); //��ȡMAC
		
		//��Ҫ����md5�����ݣ�
		wrHex("decrpt_data",decrpt_data,packLen);
		//����macУ��
		bbzero(md5_digest);
		md5_csum(decrpt_data,packLen,md5_digest); //����md5����
		memcpy(MAC,md5_digest,4); //У����
		wrHex("MAC",(BYTE *)MAC,4);
		wrHex("mac_this",mac_this,4);
		
		if(memcmp(MAC,mac_this,4)==0)
		{
			
			wrLog("MACУ��ɹ�");
			bbzero(pyct_data->yct_stan);
			
			CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE *)pyct_data->yct_stan,8); //����ϵͳ��ˮ��
			wrLog("ϵͳ��ˮ��: %s",pyct_data->yct_stan);
			CTools::byte_to_hex((BYTE *)des_rcvData+8,(BYTE *)rsp_code,4);//������

			if (strcmp((char *)rsp_code,"00000000")!=0)
			{
				char error_info[128];
				char byte_errinfo[128];
				wrLog("���׷�����:%s",rsp_code);
				if (strcmp((char *)rsp_code,"AA01FD22")==0)
				{
					TS_OutPutMsg(dPosInfo," \n �˻�����!\n�밴��ȷ�ϡ�����",0,iSelect);
				}
				bbzero(error_info);
				bbzero(byte_errinfo);
				CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)error_info,50);//������Ϣ����
				CTools::tobyte(error_info,byte_errinfo);
				CTools::trim(byte_errinfo);
				wrLog("������Ϣ����: %s",byte_errinfo);
				//TS_OutPutMsg(dPosInfo,byte_errinfo,0,iSelect);
				return 1;
			}
			else
			{
				bbzero(trade_message2);
				//CTools::byte_to_hex((BYTE *)des_rcvData+12,(BYTE *)trade_message2,64);//���ؽ�����Ϣ2
				memcpy((BYTE *)trade_message2,des_rcvData+12,32);//���ؽ�����Ϣ2

				bbzero(pyct_data->tradeTime);
				CTools::byte_to_hex((BYTE *)des_rcvData+44,(BYTE *)pyct_data->tradeTime,7);//���ؽ���ʱ��
				
				bbzero(pyct_data->amount);
				CTools::getAsiAmo((BYTE *)des_rcvData+51,4,pyct_data->amount);
				//CTools::byte_to_hex((BYTE *)des_rcvData+51,(BYTE *)pyct_data->amount,4);//���ؽ��׽��
				//memcpy((BYTE *)des_rcvData+51,(BYTE *)trade_mny,4);
				bbzero(charge_authcode);
				CTools::byte_to_hex((BYTE *)des_rcvData+55,(BYTE *)charge_authcode,4); //���ؽ���ƾ֤��
				bbzero(pyct_data->charge_authcode);
				memcpy(pyct_data->charge_authcode,charge_authcode,8);
				
				bbzero(pyct_data->yct_termid);
				memcpy((BYTE *)pyct_data->yct_termid,des_rcvData+59,6);
				//CTools::byte_to_hex((BYTE *)des_rcvData+59,(BYTE *)pyct_data->yct_termid,6);//�ն˱��
				
				bbzero(pyct_data->trader_id);
				memcpy((BYTE *)pyct_data->trader_id,des_rcvData+65,8);
				//CTools::byte_to_hex((BYTE *)des_rcvData+65,(BYTE *)pyct_data->trader_id,8);//�̻����
				
				bbzero(pyct_data->trader_name);
				memcpy((BYTE *)pyct_data->trader_name,des_rcvData+73,20);
				//CTools::byte_to_hex((BYTE *)des_rcvData+73,(BYTE *)pyct_data->trader_name,20);//�̻����

				bbzero(out_tickect_no);
				CTools::byte_to_hex((BYTE *)des_rcvData+93,(BYTE *)out_tickect_no,8);//P_CPU_LOAD_QRY�н׶�1Ӧ��������0x30�������ת��ʱ��Ч
				CTools::byte_to_hex((BYTE *)des_rcvData+101,(BYTE *)pyct_data->note,64);//������д��ֵ�ɹ��ı�ע
				CTools::byte_to_hex((BYTE *)des_rcvData+165,(BYTE *)RFU,39);//RFU
				
				
				wrLog("���׷�����Ϊ��%s",rsp_code);
				wrHex("des_rcvData+12 :",des_rcvData+12,32);
				wrHex("���ؽ�����Ϣ2 :",trade_message2,32);
				wrLog("����ʱ�䣺%s",pyct_data->tradeTime);
				wrLog("���׽��: %s",pyct_data->amount);
				wrLog("��ֵƾ֤��:%s",charge_authcode);
				wrHex("�ն˱��: ",(BYTE *)pyct_data->yct_termid,6);
				wrHex("�̻����: ",(BYTE *)pyct_data->trader_id,8);
				wrHex("�̻����: ",(BYTE *)pyct_data->trader_name,20);
				//wrLog("ת�����߼�����:%s",out_tickect_no);
				wrLog("��ע:%s",pyct_data->note);
				wrLog("RFU:%s",RFU);
			}
		}
		else
		{
			
			wrLog("���ݰ�MACУ��ʧ��");
		}
		
		return iRet;
	} 
	
	else if( iRet==1  )
	{
		closesocket(sd);
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		closesocket(sd);
		return 1;// 1�������������ʧ��
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
	BYTE type [2]; //��Ϣ������
	BYTE ver [2]; //���İ汾��
	BYTE trade_authcode[2+1];//����ƾ֤��
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

	wrLog("��P_CPU_LOAD_SUBMIT��");

	bbzero(sndData);
	bbzero(rcvData);
	bbzero(RFU);
	bbzero(trade_authcode);
	bbzero(yct_sequence_NO);
	bbzero(decrpt_data);



	sndLen=0;//���ͱ��ĳ���
	rcvLen = 0;
	
	len=0;//�����峤��
	
	CTools::hex_to_byte((BYTE *)"AA85",type,4);
	CTools::hex_to_byte((BYTE *)"0001",ver,4);
	memcpy(sndData,type,2); //��Ϣ������
	sndLen+=2;
	memcpy(sndData+2,ver,2); //���İ汾��
	sndLen+=2;
	
	sndData[4]=0x02; //����������㷨
	sndData[5]=0x80; //��ֵ�����ֽ�˳��
	sndLen+=2;
	
	bbzero(pack_data);
// 
// 	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //������ˮ��
// 	len+=8;	
// 	sprintf(yct_sequence_NO,"%08X",(unsigned int )pyct_data->yct_sequence_NO);  //���ͨ������ţ�����Ϊhex��
// 	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8); 
// 	//CTools::hex_to_byte((BYTE *)pyct_data->yct_sequence_NO,pack_data+len,8);//�������
// 	len+=4;


	CTools::hex_to_byte((BYTE*)pyct_data->pki_card,pack_data+len,8); //pki������
	len+=4;
//	memcpy(pack_data+len,pyct_data->yct_stan,8); //ϵͳ��ˮ�� ��Ӧ��AA82��ϵͳ��ˮ��
	wrLog("pyct_data->yct_stan :%s",pyct_data->yct_stan);
	CTools::hex_to_byte((BYTE *)pyct_data->yct_stan,pack_data+len,16); //ϵͳ��ˮ��
	len+=8;
	CTools::hex_to_byte((BYTE *) pyct_data->ticket_physicalNo,pack_data+len,16);//������
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,pack_data+len,16); //�߼�����
	len+=8;

	sprintf((char *)loadstatus,"%02X",0); 
	CTools::hex_to_byte((BYTE *)loadstatus,pack_data+len,2); 
	wrLog("loadstatus:  %s",(char *)loadstatus);
	//memcpy(pack_data+len,(BYTE*)load_status,1);//hex ,00�����ύ��AA84���Ļ���δ������������ֵָ��
	len+=1;
	CTools::hex_to_byte((BYTE*)TAC,pack_data+len,8); //������֤�����TAC�룬�޷�����TACʱ��0x00;
	wrHex("������֤��: ",pack_data+len,4);
	len+=4;
	CTools::hex_to_byte((BYTE*)pyct_data->SW1SW2,pack_data+len,4); //������ָ��R_CPU_LOAD���ص�SW1SW2
	len+=2;
	//memcpy(pack_data+len,(BYTE*)yct_term_status,1);//������ָ��R_CPU_LOAD����status ���������Ӧʱoxff
	sprintf((char *)loadstatus,"%02X",load_status); 
	CTools::hex_to_byte((BYTE*)loadstatus,pack_data+len,2);//������ָ��R_CPU_LOAD����status ���������Ӧʱoxff
	wrHex("������������: ",pack_data+len,1);
	len+=1;
	if (memcmp((BYTE *)loadstatus,"01",2) == 0)
	{
        //memcpy(trade_authcode,0x00,1);
		CTools::hex_to_byte((BYTE*)"0x00",pack_data+len,8);//Ȧ��״̬Ϊ01ʱ�򣬿���д0x00
	}
	else
	{
		CTools::hex_to_byte((BYTE*)charge_authcode,pack_data+len,8);
		//memcpy(pack_data+len,,4);
	}
	wrHex("��ֵƾ֤��: ",pack_data+len,4);
	len+=4;
	
	//���µ���R_CPU_GET_CARDINFO ���Ȧ��״̬Ϊ0x01Ҳ��0x00���޷����¶���ʱ��0x00
	//���ú���get_cpu_cardInfo();����������Ҫ���¶�������
	
	
	getHexAmo(pyct_data->amount,amo);
	wrHex("amo :",amo,4);
	memcpy(pack_data+len,amo,4);//���׽��
	//CTools::hex_to_byte((BYTE *)amo,(BYTE *)pack_data+len,8);
	wrHex("���׽��: ",pack_data+len,4);
	len+=4;

	getHexAmo(pyct_data->ticket_amo_aft,amo);
	wrHex("amo :",amo,4);
	memcpy(pack_data+len,amo,4);//��Ȧ���Ǯ�����
	//memcpy(pack_data,pyct_data->ticket_amo_aft,4);
//	CTools::hex_to_byte((BYTE *)amo,(BYTE *)pack_data+len,8);
	wrHex("Ǯ�����: ",pack_data+len,4);
	len+=4;
	//memcpy(down_limit,pyct_data->downLimit,1);
	//CTools::hex_to_byte((BYTE *)pyct_data->downLimit,pack_data+len,2); //Ǯ������
	
	memcpy(pack_data+len,pyct_data->downLimit,1); //�������
	wrHex("Ǯ������: ",pack_data+len,1);
	len+=1;
	//memcpy(up_limit,pyct_data->downLimit,3);
	//CTools::hex_to_byte((BYTE *)pyct_data->downLimit,pack_data+len,6); //���ͨ�������
	
	memcpy(pack_data+len,pyct_data->upperLimit,3);//�������
	wrHex("Ǯ������: ",pack_data+len,3);
	len+=3;
	wrHex("count_info: ",count_info,16);
	memcpy(pack_data+len,count_info,16);//������Ϣ
	len+=16;
	//bbzero(RFU);
	memcpy(pack_data+len,RFU,36);//RFU
	len+=36;
	
	  //����mac
// 	  CMd5::md5_finish(sndData+6,md5_digest);
// 	  memcpy(MAC,md5_digest,4); //У����
// 	  memcpy(pack_data,MAC,4); //ȡУ����ǰ��λ
// 	  len+=4;
    wrLog("len: %d",len);
	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA85" , (BYTE*)"0001" , 0x02 , 0x80 , \
		 pack_data , len , rcvData , &rcvLen);

	if (iRet != 0)
	{
		wrLog("���͵����ͨ������ʧ��");				
		return 1;
	}

	BYTE des_rcvData[512];
	BYTE rsp_code[4+1];
	bbzero(rsp_code);
	bbzero(des_rcvData);
	memcpy(des_rcvData,rcvData+8+8,rcvLen-8-8);
	//bbzero(pyct_data->yct_stan);
	
	//CTools::byte_to_hex((BYTE *)des_rcvData,(BYTE *)pyct_data->yct_stan,8); //����ϵͳ��ˮ��
	//wrLog("����ϵͳ��ˮ��: %s",pyct_data->yct_stan);
	wrHex("����ϵͳ��ˮ��: ",des_rcvData,8);
	CTools::byte_to_hex((BYTE *)des_rcvData+8,(BYTE *)rsp_code,4);//������
				
	if (strcmp((char *)rsp_code,"00000000")!=0)
	{
		char error_info[128];
		char byte_errinfo[128];
		wrLog("���׷�����:%s",rsp_code);
		bbzero(error_info);
		bbzero(byte_errinfo);
		CTools::byte_to_hex((BYTE *)des_rcvData+13,(BYTE *)error_info,50);//������Ϣ����
		CTools::tobyte(error_info,byte_errinfo);
		CTools::trim(byte_errinfo);
		wrLog("������Ϣ����: %s",byte_errinfo);				
		return 1;
	}
	else
	{	
		CTools::byte_to_hex(des_rcvData+28,followup_oper,1);//hex ox00��ʾ�����Ѿ���� 0x01ʱ�򣬻���Ҫ������չӦ�õĲ���
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
	BYTE SK[32+1]; //�ػ���Կ
	
	BYTE pysical_card[8+1]; //������
	BYTE logical_card[8+1]; //�߼�����
	char term_time_tmp[14];
	//BYTE card_pycicalInfo1[32+1];//��Ƭ������Ϣ
	BYTE SW1SW2[4+1];
	int status=0;
	BYTE SAK[1+1];
	SOCKET sd=0;
	CConnect con;//������
	int baudRate = 57600;
	//�����еĲ�����
	BYTE type [2]; //��Ϣ������
	BYTE ver [2]; //���İ汾��
	BYTE RFU [34];
	BYTE  MAC [4];
	BYTE rsp_code[8+1];
	BYTE rsp_message[48];

	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE R_CPU_LOAD_STATUS[1+1];
	BYTE enrpt_SHID[30]; 
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //�������
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
	bbzero(RFU); //RFU ����
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
     len=0;//�����峤�� 
	sndLen=0;//���ͱ��ĳ���
	rcvLen = 0;

	bbzero(sndData);
	bbzero(rcvData);

	bbzero(pack_data);

	CTools::hex_to_byte((BYTE *)"AA51",sndData,4); //��Ϣ������
	sndLen+=2;
	CTools::hex_to_byte((BYTE *)"0003",sndData+sndLen,4); //���İ汾��
	sndLen+=2;

	sndData[4]=0x02; //����������㷨
	sndData[5]=0x80; //��ֵ�����ֽ�˳��
	sndLen+=2; 
	CTools::hex_to_byte((BYTE *)pyct_data->SHID,pack_data,16); //��ˮ��
	len+=8;
	sprintf(yct_sequence_NO,"%08X",(unsigned int )pyct_data->yct_sequence_NO);  //���ͨ������ţ�����Ϊhex��
	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8); 
	len+=4;
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)pack_data+len,8); //pki����
	len+=4;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,(BYTE *)pack_data+len,16); //������
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,(BYTE *)pack_data+len,16);// //�߼�����
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->SAK,SAK,2);
	memcpy(pack_data+len,SAK,1); 
	len+=1;
	memcpy(pack_data+len,card_pycicalInfo1,32);
	len+=32;
	pack_data[len]=oper_code; //������ ִ������ѯ
	len+=1;
	memcpy(pack_data+len,RFU,34);
	len+=34;
	wrHex("RTU",RFU,34);

	//����mac 
	wrHex("�����壨û��MAC��:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4);
	memcpy(pack_data+len,MAC,4);
	len+=4;
	wrHex("mac",MAC,4);

	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)enrpt_SHID);
	
	wrLog("����֮ǰ��SHID:%s",pyct_data->SHID);
	wrLog("����֮���SHID:%s",enrpt_SHID);

	//�������г���SHID ���౨����ʹ�õ���AES���ܣ�SHID ʹ��3DES;
	wrHex("packData:",(BYTE *)pack_data,len);

	enrpt_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,(BYTE *)pyct_data->SKEY,16,&out); //ʹ��skͨ��AES����

	wrHex("aa51��Ҫ���ܵ����ݣ�",pack_data+8,len-8);
	wrHex("aa51 encpt_packdata:",enrpt_packdata,out);
	
	sprintf((char *)len_tmp,"%04X",8+out); //�����峤��
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //�����峤��
	sndLen+=2;
	
	CTools::hex_to_byte((BYTE *)enrpt_SHID,sndData+sndLen,16);  //���ܵ�SHID 
	sndLen+=8;
	memcpy(sndData+sndLen,enrpt_packdata,out);
	sndLen+=out;
                                                                                                                                                                                                                                                                                                                                         
	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}


	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //�����ݷ��͸����ͨ	
	if( iRet==0  )
	{
		char  tag[4]; //��Ϣ������
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
			wrLog("FE04���ģ���Ҫ����ǩ��");
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
			CDes::TripleDesDec((char * )CKEY,(char *)enrpt_SHID,(char *)pyct_data->SHID);//��ˮ��
			wrLog("������ˮ����:%s",pyct_data->SHID);
			CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
			packLen+=8;

			BYTE *aes_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,(BYTE *)pyct_data->SKEY,16,&out); //������ʹ��SKEY���н���
			memcpy(decrpt_data+packLen,aes_rcvData,out-4); //
			packLen+=out-4;

			wrHex("���ܱ����壨��������ˮ�ţ�",aes_rcvData,out);
			memcpy(mac_this,aes_rcvData+(out-4),4); //��ȡMAC
			
			//��Ҫ����md5�����ݣ�
			wrHex("decrpt_data",decrpt_data,packLen);
			//����macУ��
			bbzero(md5_digest);
			md5_csum(decrpt_data,packLen,md5_digest); //����md5����
			memcpy(MAC,md5_digest,4); //У����
			wrHex("MAC",MAC,4);
			wrHex("mac_this",mac_this,4);

			if(memcmp((char *)MAC,(char *)mac_this,4)==0) //����MACУ��
			{	
				wrLog("macУ��ɹ�");

				CTools::byte_to_hex((BYTE *)aes_rcvData,(BYTE *)rsp_code,4); //������
				trade_type=aes_rcvData[4]; //���ؽ�������
				wrLog("aes_rcvData[4] : %d",aes_rcvData[4]);
				memcpy(rsp_message,aes_rcvData+5,48); ////���ز�ѯ��Ϣ
				bbzero(query_info);
				memcpy(query_info,aes_rcvData+5,48);
				wrLog("������Ϊ��%s",rsp_code);
				wrLog("���ؽ�������Ϊ��%d",trade_type);
				wrHex("���ز�ѯ��Ϣ",query_info,48);
			}

			else
			{
				wrLog("MACУ�����");
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
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}
}

/**************************���������ͨ��ֵ�������� ����****************************************/
int CYCTYPTrade::P_ACCT_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, int &payType, char *czamount, char *money_limit,char *char_fre,char *ornotpassword,char *statusflag, BYTE *sign_status, BYTE *party, BYTE *rspcode)
{
    int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	BYTE  sndData[256], rcvData[256];
	BYTE SK[32+1]; //�ػ���Կ
	
	BYTE pysical_card[8+1]; //������
	BYTE logical_card[8+1]; //�߼�����
	BYTE card_pycicalInfo1[32+1];//��Ƭ������Ϣ
	BYTE SW1SW2[4+1];
	int status=0;
	BYTE SAK[1+1];
	SOCKET sd=0;
	CConnect con;//������
	int baudRate = 57600;
	//�����еĲ�����
	BYTE type [2]; //��Ϣ������
	BYTE ver [2]; //���İ汾��
	BYTE RFU [36];
	BYTE  MAC [4];
	BYTE rsp_code[8+1];
	int out=0,out1=0;
	BYTE SERVER_TIME [14+1];
	BYTE R_CPU_LOAD_STATUS[1+1];
	BYTE enrpt_SHID[30]; 
	BYTE SHID[16+1];
	char  yct_sequence_NO[8+1]; //�������
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
	bbzero(RFU); //RFU ����
	bbzero(yct_sequence_NO);
	bbzero(md5_digest);
	bbzero(char_fre);
	bbzero(hex_amount);
	bbzero(charge_limit);

	wrLog("��P_ACCT_QRY��");

	CTools::hex_to_byte((BYTE*)"AB31",sndData,4);                 //��Ϣ������
	sndLen +=2;
	CTools::hex_to_byte((BYTE*)"0001",sndData+sndLen,4);          //���İ汾��
	sndLen +=2;
	sndData[4]=0x02;
	sndData[5]=0x80;
	sndLen +=2;
	CTools::hex_to_byte((BYTE*)pyct_data->SHID,pack_data,16);     //������ˮ��
	len +=8;
	sprintf(yct_sequence_NO,"%08x",(unsigned int )pyct_data->yct_sequence_NO);     //�������
	CTools::hex_to_byte((BYTE *)yct_sequence_NO,pack_data+len,8);
	len +=4;
	CTools::hex_to_byte((BYTE *) pyct_data->pki_card,(BYTE *)pack_data+len,8);     //pki����
	len+=4;
	CTools::hex_to_byte((BYTE *)"02",(BYTE *)pack_data+len,2);                     //�ն�����
	len +=1;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_no,(BYTE *)pack_data+len,16); //������
	len+=8;
	CTools::hex_to_byte((BYTE *)pyct_data->ticket_physicalNo,(BYTE *)pack_data+len,16);    //�߼�����
	len+=8;
	memcpy(pack_data+len,RFU,35);
	len+=35;
	wrHex("RTU",RFU,35);



	//����mac 
	wrHex("�����壨û��MAC��:",pack_data,len);
	md5_csum(pack_data,len,md5_digest);
	memcpy(MAC,md5_digest,4);
	memcpy(pack_data+len,MAC,4);
	len+=4;
	wrHex("mac",MAC,4);
	
	CDes::TripleDesEncrypt((char *)CKEY,(char *)pyct_data->SHID,(char *)enrpt_SHID);
	
	wrLog("����֮ǰ��SHID:%s",pyct_data->SHID);
	wrLog("����֮���SHID:%s",enrpt_SHID);
	
	//�������г���SHID ���౨����ʹ�õ���AES���ܣ�SHID ʹ��3DES;
	wrHex("packData:",(BYTE *)pack_data,len);
	
	enrpt_packdata=AESEncryptEx((BYTE *)pack_data+8,len-8,(BYTE *)pyct_data->SKEY,16,&out); //ʹ��skͨ��AES����
	
	wrHex("AB31��Ҫ���ܵ����ݣ�",pack_data+8,len-8);
	wrHex("AB31 encpt_packdata:",enrpt_packdata,out);
	wrLog("out: %d",out);
	sprintf((char *)len_tmp,"%04X",8+out); //�����峤��
	CTools::hex_to_byte(len_tmp,sndData+sndLen,4); //�����峤��
	sndLen+=2;
	
	CTools::hex_to_byte((BYTE *)enrpt_SHID,sndData+sndLen,16);  //���ܵ�SHID 
	sndLen+=8;
	memcpy(sndData+sndLen,enrpt_packdata,out);
	sndLen+=out;
	
	//���ͨ��������
	iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
	if( iRet==0 )
	{
		NULL;
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = YCTTIMEOUT;
		return 3;// 3�������ͨ����ʧ��
	}
	
	
	iRet = ST_commuWithYCTIC3(dPosInfo,pyct_data, com, comType, sd, sndData,sndLen , rcvData, &rcvLen); //�����ݷ��͸����ͨ
    if (iRet ==0)
    {
		
		CTools::byte_to_hex((BYTE *)rcvData+8,enrpt_SHID,8);
		bbzero(pyct_data->SHID);
		CDes::TripleDesDec((char * )CKEY,(char *)enrpt_SHID,(char *)pyct_data->SHID);//������ˮ��
		wrLog("������ˮ����:%s",pyct_data->SHID);
		CTools::hex_to_byte((BYTE *)pyct_data->SHID,decrpt_data,16);
		packLen+=8;
		
		BYTE *aes_rcvData=AESDecryptEx((BYTE  *)rcvData+16,rcvLen-16,(BYTE *)pyct_data->SKEY,16,&out); //������ʹ��SKEY���н���
		memcpy(decrpt_data+packLen,aes_rcvData,out-4); //
		packLen+=out-4;
		
		wrHex("���ܱ����壨��������ˮ�ţ�",aes_rcvData,out);
		memcpy(mac_this,aes_rcvData+(out-4),4); //��ȡMAC
		
		//��Ҫ����md5�����ݣ�
		wrHex("decrpt_data",decrpt_data,packLen);
		//����macУ��
		bbzero(md5_digest);
		md5_csum(decrpt_data,packLen,md5_digest); //����md5����
		memcpy(MAC,md5_digest,4); //У����
		wrHex("MAC",MAC,4);
		wrHex("mac_this",mac_this,4);

		if (memcmp((char *)MAC,(char *)mac_this,4)==0)
		{
			wrLog("macУ��ɹ�");
			len2 +=12;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)rsp_code,4); //������
			wrLog("������ %s",rsp_code);
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
			   wrLog("������Ϣ���� %s",error_message);
			   
			   *party = FROMYCT;
			   *rsp_code = FROMYCT_N;
			   return 1;
			}
			len2 +=4;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)hex_amount,4);
			CTools::HexToDec(hex_amount,czamount);              //��ֵ���˻����
			//getAsiAmo3(aes_rcvData+len2,4,czamount); 
			wrLog("��ֵ���˻���� %.2f Ԫ",atof(czamount)/100);
			len2 +=4;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)ornotpassword,1);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)statusflag,1);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)sign_status,1); //ʵʱ�ۿ�ǩԼ״̬
			wrLog("������ %s",sign_status);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)char_fre,1);      //��ֵƵ��
			wrLog("��ֵƵ�� %s",char_fre);
			len2 +=1;
			CTools::byte_to_hex((BYTE *)aes_rcvData+len2,(BYTE *)money_limit,4); //��ֵ���
			wrLog("��ֵ��� %s",money_limit);

		//�����ֱ�ʾ�˻��Ƿ���Ҫ��ֵ����
			if (strcmp(ornotpassword,"00")==0)
				strcpy(ornotpassword,"��");
			else
				strcpy(ornotpassword,"��");
		
			//�����ֱ�ʾ�˻�״̬��־


			if (strcmp(statusflag,"00")==0)
			{
				strcpy(statusflag,"����");
			}
			else 
				if (strcmp(statusflag,"01")==0)
			{
					strcpy(statusflag," ��ͣ��");
			}
			else 
				if (strcmp(statusflag,"02")==0)
			{
					strcpy(statusflag,"��ʾ����");
			}
			else 
				if (strcmp(statusflag,"03")==0)
			{
					strcpy(statusflag,"��ʾ�ϲ�����");
			}
				
			//��������ʾ��ֵƵ��
				if (strcmp(char_fre,"00")==0)
			{
					strcpy(char_fre,"������");
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
		return 3;// 3�������ͨ����ʧ��
	}
	else
	{
		*party = FROMCTSI;
		*rspcode = ICTIMEOUT;
		closesocket(sd);
		return 1;// 1�������������ʧ��
	}

    return 0;
}

int CYCTYPTrade::Yct_AcctHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE *party, BYTE *rspcode)
{
    int iRet=0, len=0, icLen=0, sndLen=0, rcvLen=0;
	int iAom;
	
	BYTE pysical_card[8+1]; //������
	BYTE logical_card[8+1]; //�߼�����
	BYTE card_pycicalInfo1[32+1];//��Ƭ������Ϣ
	BYTE rsp_message[1204];
	char SW1SW2[4+1];
	BYTE SAK[1+1];
	BYTE card_physics_info[51];
	SOCKET sd=0;
	CConnect con;//������
	int baudRate = 57600;
	//�����еĲ�����
	BYTE type [2]; //��Ϣ������
	BYTE ver [2]; //���İ汾��
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
	bbzero(RFU); //RFU ����
	bbzero(md5_digest);
	bbzero(acctamount);
	bbzero(charge_money);
	bbzero(char_fre);
	bbzero(statusflag);
	bbzero(ornotpassword);
	
	//��ȡ��Ƭ��������Ϣ,���ؿ�Ƭ��������Ϣ
	iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
	if (iRet == 1)
	{
		TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		for (int i = 0; i<2; i++)
		{
			iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
			if (iRet == 0)
			{
				break;
			}
			TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		}

	}
	
	if (iRet ==0)
	{
		CTools::byte_to_hex((BYTE*) card_physics_info,(BYTE*)pyct_data->ticket_physicalNo,8);//��ȡ������
		CTools::byte_to_hex((BYTE*) card_physics_info+8,(BYTE*)pyct_data->ticket_no,8);      //��ȡ�߼�����
		memcpy(SAK,card_physics_info+16,1);                                                  //SAK
		memcpy(card_pycicalInfo1,card_physics_info+17,32);                                   //��Ƭ������Ϣ1
		CTools::byte_to_hex(card_physics_info+49,(BYTE*)SW1SW2,2);                                  //SW1SW2
		
		wrLog("SW1SW2:%s",SW1SW2);//��Ƭ״̬

		if (memcmp(SW1SW2,"9000",4) != 0)
		{
			*party=FROMIC;
			*rspcode=ICTIMEOUT;
			TS_OutPutMsg(dPosInfo,"",0,iSelect);
			return 1;
		}
		wrLog("������:%s",pyct_data->ticket_physicalNo);
		//wrLog("�߼�����:%s",pyct_data->ticket_no);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32); 
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
	}
	else
	{
		*party=FROMIC;
		*rspcode=ICTIMEOUT;
		TS_OutPutMsg(dPosInfo,"��ȡ��Ƭ��������Ϣ��������",0,iSelect);
		return 1;
	}
	//P_ACCT_QRY��ֵ���ѯ
	iRet=P_ACCT_QRY(dPosInfo,com,comType,pyct_data,payType,czamount,money_limit,char_fre,ornotpassword,statusflag,sign_status,party,rspcode);
	if (iRet ==3)
	{
		return 3; // 3�������ͨ����ʧ��
	}
	else if(iRet ==1)
	{
     	return 1; // 1�������������ʧ��
	}	
	else
	{
		wrLog("Ӧ������ %d",payType);

		sprintf(temp,"��ֵ���˻����: %.2fԪ\nʹ�ó�ֵ���˻�����ֵ������Enter��ѡ���ֵ��� ����Esc������",atof(czamount)/100);
		if(atol((const char *)czamount) > 0)
		{
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);
			if(iSelect)
			{
				//�û�ȡ��
				*party = FROMCTSI_N;
				*rspcode = USERCANCEL;	
				return 5;
			}
		}
		else
		{
			if (memcmp(sign_status,"02",2) == 0)
			{
				TS_OutPutMsg(dPosInfo,"���ĳ�ֵ���˻����Ϊ0���ɽ���ǩԼ�˻���ֵ\n����Enter��������ֵ   ����ESC������",0,iSelect);
				if(iSelect)
				{
					//�û�ȡ��
					*party = FROMCTSI_N;
					*rspcode = USERCANCEL;	
					return 5;
				}
							//��������ʾʵʱ�ۿ�ǩԼ״̬
				if (strcmp((char *)sign_status,"00")==0)
				{
					strcpy((char *)sign_status,"δǩԼ");
				}
				else 
					if (strcmp((char *)sign_status,"01")==0)
				{
						strcpy((char *)sign_status," δ����");
				}
				else 
					if (strcmp((char *)sign_status,"02")==0)
				{
						strcpy((char *)sign_status,"�Ѽ���");
				}
				else 
					if (strcmp((char *)sign_status,"03")==0)
				{
					strcpy((char *)sign_status,"��ͣ");
				}
				else 
					if (strcmp((char *)sign_status,"04")==0)
				{
					strcpy((char *)sign_status,"��Լ");
				}
					CTools::HexToDec((char *)money_limit,money_limit);
					sprintf(auto_show,"\t\t����:%s\n\n��ֵ���˻����:%.2fԪ\t�˻��Ƿ���Ҫ��ֵ����:%s\n\n�˻�״̬:%s\t\tʵʱ�ۿ�ǩԼ״̬:%s\n\n��ֵƵ��:%s��/һ��\t��ֵ���:%.2fԪ",pyct_data->ticket_no,atof(czamount)/100,ornotpassword,statusflag,sign_status,char_fre,atof(money_limit)/100);
//					TS_OutPutMsg(dPosInfo,auto_show,0,iSelect);
			}
			else
			{
				TS_OutPutMsg(dPosInfo,"����δ��ͨ��ֵ���˻���ǩԼ�˻�����ѡ��ʹ�á��ֽ��ֵ����ʽ���г�ֵ\n����ESC������",0,iSelect);
				*party = FROMCTSI_N;
				*rspcode = USERCANCEL;	
				return 5;
			}
		}
	}
    //��ȡ��Ƭ��������Ϣ,���ؿ�Ƭ��������Ϣ
	iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
	if (iRet == 1)
	{
		TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		for (int i = 0; i<2; i++)
		{
			iRet=R_PUB_QRY_PHYSICS_INFO(dPosInfo,com,comType,card_physics_info);
			if (iRet == 0)
			{
				break;
			}
			TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		}

	}
	
	
	if (iRet ==0)
	{
		CTools::byte_to_hex((BYTE*) card_physics_info,(BYTE*)pyct_data->ticket_physicalNo,8);//��ȡ������
		CTools::byte_to_hex((BYTE*) card_physics_info+8,(BYTE*)pyct_data->ticket_no,8);      //��ȡ�߼�����
		memcpy(SAK,card_physics_info+16,1);                                                  //SAK
		memcpy(card_pycicalInfo1,card_physics_info+17,32);                                   //��Ƭ������Ϣ1
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
			sprintf(outPut_Msg,"������Ӧ��״̬�����%s",errMsg);
			TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
			*party = FROMIC;
			*rspcode = ERROR_FROMIC;
			return 1;// 1�������������ʧ��
		}

		wrLog("������:%s",pyct_data->ticket_physicalNo);
		//wrLog("�߼�����:%s",pyct_data->ticket_no);
		wrHex("card_pycicalInfo1",card_pycicalInfo1,32); 
		CTools::byte_to_hex(SAK,(BYTE *)pyct_data->SAK,1);
		wrHex("SAK",SAK,1);
	}
	else
	{
		*party=FROMIC;
		*rspcode=ICTIMEOUT;
		TS_OutPutMsg(dPosInfo,"��ȡ��Ƭ��������Ϣ��������",0,iSelect);
		return 1; // 1�������������ʧ��
	}
	//���ý��ײ�ѯP_TRAN_QRY ��ƽ̨��������
TRANQRY: iRet=P_TRAN_QRY(dPosInfo,com,comType,pyct_data,card_pycicalInfo1,0x04,trade_type,rsp_message,party,rspcode);
	if (iRet == 0)
	{    bank_signed = 0;
		switch (trade_type)
		{
			
		case(0x00):
			{
				wrLog("M1Ǯ��(��ѯ��Ȧ��ͳ���)");
				R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,rsp_message,SW1SW2);
				//�жϳ�ֵ���˻����/ǩԼ�˻���ֵ��ѡ���ֵ���
				if (payType == 0x20)
				{
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return 3; // 3�������ͨ����ʧ��
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
					sprintf(temp,"����ǩԼ�˻��Ķ����: %.2fԪ",atof(money_limit)/100);
//					TS_OutPutMsg(dPosInfo,temp,0,iSelect);
					strcpy(tradeamount,money_limit);
				}
				strcpy(pyct_data->amount,tradeamount);
				iRet=excute_M1_LOAD(dPosInfo,com,comType,pyct_data,2,party,rspcode,tradeamount,payType);
				
				break;
			}
		case (0x02):
			{
				wrLog("M1�û��˻���ֵ");
				R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,rsp_message,SW1SW2);
				//�жϳ�ֵ���˻����/ǩԼ�˻���ֵ��ѡ���ֵ���
				if (payType == 0x20)
				{
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return 3; // 3�������ͨ����ʧ��
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
					sprintf(temp,"����ǩԼ�˻��Ķ����: %.2fԪ",atof(money_limit)/100);
//					TS_OutPutMsg(dPosInfo,temp,0,iSelect);
					strcpy(tradeamount,money_limit);
				}
				strcpy(pyct_data->amount,tradeamount);
				iRet=excute_M1_LOAD(dPosInfo,com,comType,pyct_data,2,party,rspcode,tradeamount,payType);
				break;
			}
		case (0x20):
			{
				wrLog("CPUǮ����ֵ����ѯ��Ȧ��ͳ�����");
				//�жϳ�ֵ���˻����/ǩԼ�˻���ֵ��ѡ���ֵ���
				if (payType == 0x20)
				{                                                
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return 3;  // 3�������ͨ����ʧ��
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
//					sprintf(temp,"����ǩԼ�˻������: %.2fԪ",atof(charge_money)/100);
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
				wrLog("OC3.CPUȦ��(M1���ת��)");
				
				break;
			}
		case (0x22):
			{
				wrLog("oc3.cpuȦ�棨cpu�û��˻���ֵ��");
				wrLog("CPUǮ����ֵ����ѯ��Ȧ��ͳ�����");
				//�жϳ�ֵ���˻����/ǩԼ�˻���ֵ��ѡ���ֵ���
				if (payType == 0x20)
				{                                                
					iAom=AutoSelectAmo(dPosInfo,czamount,tradeamount);
					if (iAom <0 || iAom == 0x36)
					{
						return CANCLE_KEY;  // 3�������ͨ����ʧ��
					}
				}
				else
				{
					bank_signed = 1;
					memset(temp,0,sizeof(temp));
					CTools::HexToDec(money_limit,charge_money); 
					sprintf(temp,"����ǩԼ�˻������: %.2fԪ",atof(charge_money)/100);
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
				wrLog("ִ����չӦ�ò���");
				break;
			}
		case (0x31):
			{
				wrLog("ִ����չӦ�ò�ѯ");
				break;
			}
		case (0x40):
			{
				wrLog("ִ�к���������");				
				EXCUTE_BLACKLIST(dPosInfo,com,comType,pyct_data,rsp_message,party,rspcode);
				break;
			}
		case (0x60):
			
			{
				wrLog("ִ��CPUͬ��");
				change = 1;
				R_PUB_SET_READCARDINFO(dPosInfo, com, comType,pyct_data->ticket_no,rsp_message,SW1SW2);
				iRet=EXCUTE_M1_TO_CPU(dPosInfo,com,comType,pyct_data,rsp_message,party,rspcode);
				if (*rspcode == R_APP_CARD_UPGRADE_EXEFAIL)
				{
					wrLog("�Ѿ���������ִ�г�ֵ���Ͳ�ѯ��");
					TS_OutPutMsg(dPosInfo,"��ѿ��ŵ���������",1,iSelect);
					//����ִ�г�ֵ���Ͳ�ѯ
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
		return 3; // 3�������ͨ����ʧ��
	}
	if(iRet ==1)
	{
		pyct_data->status = 5;
		bbzero(pyct_data->term_serial);
		bbzero(pyct_data->ticket_amo_aft);
		CTools::getYct_posSerial(pyct_data->term_serial);
		strcpy(pyct_data->yct_serial,pyct_data->yct_stan);
		CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);
		return 1; // 1�������������ʧ��
	}
	if (iRet == 11)
	{
		pyct_data->status = 6;
		bbzero(pyct_data->term_serial);
		bbzero(pyct_data->ticket_amo_aft);
		CTools::getYct_posSerial(pyct_data->term_serial);
		strcpy(pyct_data->yct_serial,pyct_data->yct_stan);
		CDB::insetYctYPtxnDATA(pyct_data,pyct_data->yct_stan,1);
		return 1; // 1�������������ʧ��
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

	//P_ACCT_QRY��ֵ���ѯ
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
			sprintf(temp,"\n\n����:%s\n\n����ֵ�����:%.2fԪ\n\n����Enter����ֵ  ����Esc������",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100);
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);

		}
		if (M1_CPU == 1 &&payType == 0x20)
		{
			sprintf(temp,"���ţ�%s\n\n��ֵ���˻����: %.2fԪ\n\n��ֵ�����:%0.2fԪ\n\n\n\n\n����Enter����ֵ  ����Esc������",pyct_data->ticket_no,atof(czamount)/100,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100);
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);
		}
		if(M1_CPU == 2 && payType == 0x22)
		{
			sprintf(temp,"\n\n����:%s\n\n����ֵ�����:%.2fԪ\n\n����Enter����ֵ  ����Esc������",pyct_data->ticket_no,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100 - atof(des_lim));
			TS_OutPutMsg(dPosInfo,temp,0,iSelect);

		}
		if (M1_CPU == 2 && payType == 0x20)
		{
			sprintf(temp,"���ţ�%s\n\n��ֵ���˻����: %.2fԪ\n\n��ֵ�����:%0.2fԪ\n\n\n\n\n����Enter����ֵ  ����Esc������",pyct_data->ticket_no,atof(czamount)/100,atof(pyct_data->ticket_amo_bef)/100 + atof(tradeamount)/100 - atof(des_lim));
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
	wrLog("��%d",nowAmo);
	if (nowAmo >0 && nowAmo <5000)
	{
		menulist.iTotalNum = 1;
		strcpy(menulist.acTitle,"��ѡ�����ֵ�Ľ��");
		strcpy(menulist.menuItem[0],"һ�ʳ���");
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
		strcpy(menulist.acTitle,"��ѡ�����ֵ�Ľ��");
		strcpy(menulist.menuItem[0],"50Ԫ");
		strcpy(menulist.menuItem[1],"һ�ʳ���");
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
		strcpy(menulist.acTitle,"��ѡ�����ֵ�Ľ��");
		strcpy(menulist.menuItem[0],"50Ԫ");
		strcpy(menulist.menuItem[1],"100Ԫ");
		strcpy(menulist.menuItem[2],"һ�ʳ���");
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
		strcpy(menulist.acTitle,"��ѡ�����ֵ�Ľ��");
		strcpy(menulist.menuItem[0],"50Ԫ");
		strcpy(menulist.menuItem[1],"100Ԫ");
		strcpy(menulist.menuItem[2],"200Ԫ");
		strcpy(menulist.menuItem[3],"һ�ʳ���");
//		strcpy(menulist.menuItem[4],"0.1Ԫ");
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
		strcpy(menulist.acTitle,"��ѡ�����ֵ�Ľ��");
		strcpy(menulist.menuItem[0],"50Ԫ");
		strcpy(menulist.menuItem[1],"100Ԫ");
		strcpy(menulist.menuItem[2],"200Ԫ");
//		strcpy(menulist.menuItem[3],"0.1Ԫ");
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

	//���г�ֵ���˻�
	strcpy(yct_data.bank,pgbl.bank);
	strcpy(yct_data.card_no,pgbl.card_no);
	strcpy(yct_data.biz_type,pgbl.fee_type);
	strcpy(yct_data.term_id,pgbl.dPosInfo.pos_no);
	iRet=Yct_AcctHdl(pgbl.dPosInfo,com,comType,NORMALPAY,&yct_data,"000000000000",&m_party,&m_rspcode);
	if (iRet ==1)
	{
		// 1�������������ʧ��
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
		CTools::HexToDec(yct_data.yct_stan,pgbl.stan_no,12); //�Զ���ֵʱƱ�ݵ�ϵͳ�ο��������ͨ��ˮ��
		itype=0;
		set_yctthird_priCont(itype,&yct_data,priCont);
		CPrint ptr;
		ptr.set_in_par(pgbl);
		ptr.print_yctthird_detail("���ͨ�Զ���ֵ",priCont);
	}

	
	return 0;
}

//��ѯ��Ӧ����Ϣ
//out: card_appinfo - ��Ӧ����Ϣ
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
	
	wrLog("��R_PUB_QRY_APP_INFO��");
	
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
		//��ʾת��ʧ��
		get_cardReader_errMsg(ReaderStatus,errMsg);
		sprintf(outPut_Msg,"���������ش�����Ϣ��%s",errMsg);
		TS_OutPutMsg(dPosInfo,outPut_Msg,1,iSelect);
		return 2;
	}
	
    tmp_len+=4;
	
	bbzero(pyct_data->ticket_physicalNo);
	wrHex("������: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_physicalNo,outBuf+tmp_len,8);
	tmp_len+=8;
	
//	wrHex("�߼�����: ",outBuf+tmp_len,8);
	memcpy(pyct_data->ticket_no,outBuf+tmp_len,8);
	tmp_len+=8;
	
	wrHex("SAK: ",outBuf+tmp_len,1);
	tmp_len+=1;
	
	bbzero(card_appinfo);
	memcpy(card_appinfo,outBuf+tmp_len,40);
	wrHex("��Ӧ����Ϣ: ",card_appinfo,40);
	tmp_len+=40;
	
    bbzero(mny);
	CTools::byte_to_hex((BYTE *)outBuf+tmp_len,(BYTE *)mny,4); //Ǯ�����	
	if (flag == 0)
	{
		bbzero(pyct_data->ticket_amo_bef);
		CTools::HexToDec((char *)mny, pyct_data->ticket_amo_bef);
		wrHex("�۷�ǰǮ�����: ",outBuf+tmp_len,4);
	}
	else if (flag == 1)
	{
		bbzero(pyct_data->ticket_amo_aft);
		CTools::HexToDec((char *)mny, pyct_data->ticket_amo_aft);
		wrHex("�۷Ѻ�Ǯ�����: ",outBuf+tmp_len,4);
	}
	tmp_len+=4;
	
	bbzero(pyct_data->downLimit);
	memcpy(pyct_data->downLimit,outBuf+tmp_len,1);
	wrHex("�������: ",outBuf+tmp_len,1);
	tmp_len+=1;
	
	bbzero(pyct_data->upperLimit);
	memcpy(pyct_data->upperLimit,outBuf+tmp_len,3);
	wrHex("�������: ",outBuf+tmp_len,3);
	tmp_len+=3;
	
	wrHex("SW1SW2: ",outBuf+tmp_len,2);
	memcpy(pyct_data->SW1SW2,outBuf+tmp_len,2);
	tmp_len+=2;
	return 0;
}

//�������ͨ��ֵ��������ӡ��ϸ����
//in : type - ��������  0:��ֵ�ɹ�����   1:��ֵ���Ƴɹ����� 2:��ֵ�������� 
//						3:ת����Ȧ�ᵥ�� 4:ת�뿨Ȧ�浥��   5:ת�뿨Ȧ�����Ƴɹ�����
//out: priCont - ��ӡ��ϸ����
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
		strcpy(pyct_data->trade_type,"��ֵ");
	}
	else if (type == 2)
	{
		strcpy(pyct_data->trade_type,"��ֵ����");
	}
    else if (type == 3 )
    {
		strcpy(pyct_data->trade_type,"���ת��");
    }
	else if (type == 4 || type == 5)
	{
		strcpy(pyct_data->trade_type,"���ת��");
	}

		strcat(priCont, "���׿���: ");
		strcat(priCont, pyct_data->ticket_no);
// 		strcat(priCont, "\n��������: ");
// 		strcat(priCont, pyct_data->trade_type);
		strcat(priCont, "\n�� �� ��: ");
		strcat(priCont, pyct_data->pki_card);
		
// 		if (type == 2)
// 		{
// 			strcat(priCont, "\n����ʱ��: ");
// 		}
// 		else if (type == 3 || type ==4 || type == 5)
// 		{
// 			strcat(priCont, "\nת��ʱ��: ");
// 		}
// 		else
// 		{
// 			strcat(priCont, "\n����ʱ��: ");
// 		}
// 		//ת��ʱ��
// 
// 		bbzero(fmt_time);
// 		bbzero(fmt_date);
// 		CTools::formatDate(pyct_data->txn_date,fmt_date,3);
// 		CTools::formatTime(pyct_data->txn_time,fmt_time);
// 		sprintf(priCont+strlen(priCont),"%s %s",fmt_date,fmt_time);
// 		
// 		
// 		strcat(priCont, "\n\n");//������
		if (type == 0 || type == 1)
		{
			strcat(priCont, "\n���ͨ��ˮ��: ");		
			bbzero(yct_stan_temp);
			CTools::HexToDec(pyct_data->yct_stan,yct_stan_temp,8);
			strcat(priCont, yct_stan_temp);
			strcat(priCont, "\nƾ ֤ ��: ");
			bbzero(auth_temp);
			CTools::HexToDec(pyct_data->charge_authcode,auth_temp,8);
			strcat(priCont, auth_temp);
		}
		else if (type == 2)
		{
			strcat(priCont, "\nԭϵͳ�ο���: ");
			strcat(priCont, pyct_data->stan);

			strcat(priCont, "\nԭ���ͨ��ֵ��ˮ��: ");
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
//			wrLog("Ǯ����ʾ�����:%.2f",Display_credit);
		}

		if (atuo_charge == 1 && M1_CPU == 1)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ��ԭ���: RMB %.2fԪ", atof(pyct_data->ticket_amo_bef)/100);
		}
		if (atuo_charge == 1 && M1_CPU == 2)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ��ԭ���: RMB %.2fԪ", atof(pyct_data->ticket_amo_bef)/100 - atof(des_lim));
		}
		if(atuo_charge != 1 && (M1_CPU != 1 || M1_CPU != 2))
			sprintf(priCont+strlen(priCont), "\n���ͨ��ԭ���: RMB %.2fԪ", Display_credit );

		if (type == 2)
		{
			sprintf(priCont+strlen(priCont), "\n�������: RMB %.2fԪ", atof(pyct_data->amount)/100);	
		}
//		if (type == 1)
		if(moni ==1)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ�������: RMB %.2f(*)Ԫ", Display_credit+atof(pyct_data->amount)/100);
		}
		if (type == 2)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ�������: RMB %.2fԪ", Display_credit-atof(pyct_data->amount)/100);	
		}
		if (type != 2 && atuo_charge == 1 && M1_CPU == 1 && moni != 1)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ�������: RMB %.2fԪ", atof(pyct_data->ticket_amo_aft)/100);
		}
		if (type != 2 && atuo_charge == 1 && M1_CPU == 2 && moni !=1)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ�������: RMB %.2fԪ", atof(pyct_data->ticket_amo_aft)/100 - atof(des_lim) );
		}
		if(type != 2 && moni != 1 && atuo_charge != 1)
		{
			sprintf(priCont+strlen(priCont), "\n���ͨ�������: RMB %.2fԪ", Display_credit+atof(pyct_data->amount)/100);
		}

//		if (type == 1)
		if(moni ==1)
		{
			sprintf(priCont+strlen(priCont), "\n���׽��: RMB %.2f(*)Ԫ", atof(pyct_data->amount)/100);
		}
		else if (type != 2)
		{
			sprintf(priCont+strlen(priCont), "\n���׽��: RMB %.2fԪ", atof(pyct_data->amount)/100);	
		}
		CTools::hex_to_byte((BYTE*)pyct_data->note,(BYTE*)note,strlen(pyct_data->note));
		strcat(priCont, "\n��    ע: ");
		strcat(priCont, note);

		strcat(priCont, "\nע:�����ݽ����ο�, ������������");
		strcat(priCont,"\n��Ʊ��������ƾ֤");
		strcat(priCont,"\n�ͷ��绰400-80-11888");
		strcat(priCont,"\n\n\n\n\n\n");

	return 0;
}

//��ȡ����Ӧ״̬�뺬��
int CYCTYPTrade::get_cardStatus_errMsg(BYTE *SW1SW2,char *errMsg)
{
	char temp[10+1];
	char outstr[32];
	int  temp_int = 0;

	bbzero(temp);
	bbzero(errMsg);

	if (SW1SW2[0] == 0x61)
	{
		strcpy(errMsg,"��������");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"����Ϣ�ṩ");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"���͵����ݿ����д�");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"�ļ�����<Le");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x83)
	{
		strcpy(errMsg,"ѡ����ļ���Ч");
	}
	else if (SW1SW2[0] == 0x62 && SW1SW2[1] == 0x84)
	{
		strcpy(errMsg,"FC1��ʽ��P2ָ���Ĳ���");
	}
	else if (SW1SW2[0] == 0x63 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"��֤ʧ��");
	}
	else if (SW1SW2[0] == 0x63)
	{
		CTools::byte_to_hex((BYTE *)SW1SW2+1,(BYTE *)temp,2);
		if (memcmp(temp,"C",1) == 0)
		{
			bbzero(outstr);
			CTools::HexToDec(temp+1,outstr);//ȡ��SW2�ڶ�λ��תΪʮ����
			temp_int = atoi(outstr);
			sprintf(errMsg,"��֤ʧ�ܣ���ʣ��%d�γ��Ի���",temp_int);
		}
	}
	else if (SW1SW2[0] == 0x64 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"״̬��ʶλδ��");
	}
	else if (SW1SW2[0] == 0x65 && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"�ڴ����");
	}
	else if (SW1SW2[0] == 0x67 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"���ȴ���");
	}
	else if (SW1SW2[0] == 0x68 && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"��֧�ְ�ȫ����");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"���ܴ���");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x01)
	{
		strcpy(errMsg,"������ܣ���Ч״̬��");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"�������ļ��ṹ������");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"�����㰲ȫ״̬");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x83)
	{
		strcpy(errMsg,"��֤��������");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x84)
	{
		strcpy(errMsg,"����������Ч");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x85)
	{
		strcpy(errMsg,"ʹ������������");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x86)
	{
		strcpy(errMsg,"����������ִ�е��������ǵ�ǰEF��");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x87)
	{
		strcpy(errMsg,"��ȫ���������ʧ");
	}
	else if (SW1SW2[0] == 0x69 && SW1SW2[1] == 0x88)
	{
		strcpy(errMsg,"��ȫ��Ϣ���ݶ�����ȷ");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x80)
	{
		strcpy(errMsg,"�������������ȷ");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x81)
	{
		strcpy(errMsg,"���ܲ�֧��");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x82)
	{
		strcpy(errMsg,"δ�ҵ��ļ�");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x83)
	{
		strcpy(errMsg,"δ�ҵ���¼");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x84)
	{
		strcpy(errMsg,"�ļ��д洢�ռ䲻��");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x86)
	{
		strcpy(errMsg,"P1��P2��������ȷ");
	}
	else if (SW1SW2[0] == 0x6A && SW1SW2[1] == 0x88)
	{
		strcpy(errMsg,"���������Ҳ���");
	}
	else if (SW1SW2[0] == 0x6B && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"��������ƫ�Ƶ�ַ������EF��");
	}
	else if (SW1SW2[0] == 0x6C)
	{
		CTools::byte_to_hex((BYTE *)SW1SW2+1,(BYTE *)temp,1);
		bbzero(outstr);
		CTools::HexToDec(temp,outstr);//ȡ��SW2��תΪʮ����
		temp_int = atoi(outstr);
		sprintf(errMsg,"���ȴ���ʵ�ʳ���Ϊ��%d",temp_int);
	}
	else if (SW1SW2[0] == 0x6F && SW1SW2[1] == 0x00)
	{
		strcpy(errMsg,"������Ч");
	}

	wrLog("����Ӧ״̬�뺬�壺%s",errMsg);
	return 0;
}

//��ȡ������������Ϣ
int CYCTYPTrade::get_cardReader_errMsg(BYTE *status,char *errMsg)
{
	int iStatus;
	bbzero(errMsg);

	iStatus = int(status);
	switch(iStatus)
	{
		case 0x60:
			strcpy(errMsg,"û�а�װSAM��");
			break;
		case 0x61:
			strcpy(errMsg,"SAM����ʼ�������δ��ʼ��");
			break;
		case 0x62:
			strcpy(errMsg,"SAM������PIN����");
			break;
		case 0x63:
			strcpy(errMsg,"SAM�������뽻�����Ͳ�ƥ��");
			break;
		case 0x64:
			strcpy(errMsg,"SAM��ѡ���ļ�����");
			break;
		case 0x65:
			strcpy(errMsg,"SAM������");
			break;
		case 0x66:
			strcpy(errMsg,"SAM��д����");
			break;
		case 0x67:
			strcpy(errMsg,"SAM����֤����");
			break;
		case 0x68:
			strcpy(errMsg,"SAM�����������");
			break;
		case 0x69:
			strcpy(errMsg,"SAM��DES�������");
			break;
		case 0x6A:
			strcpy(errMsg,"SAM������Ǯ����Կ����");
			break;
		case 0x71:
			strcpy(errMsg,"PKI��RSA�������");
			break;
		case 0x72:
			strcpy(errMsg,"PKI��DES�������");
			break;
		case 0x7E:
			strcpy(errMsg,"SAM��ִ��APDU�������");
			break;
		case 0x7F:
			strcpy(errMsg,"SAM��������ʱ");
			break;
		case 0x80:
			strcpy(errMsg,"û�п�");
			break;
		case 0x81:
			strcpy(errMsg,"ѡ��Ƭ����");
			break;
		case 0x82:
			strcpy(errMsg,"ͣ�ÿ�Ƭ����");
			break;
		case 0x83:
			strcpy(errMsg,"��֤��Ƭ����");
			break;
		case 0x84:
			strcpy(errMsg,"��Ƭ����������");
			break;
		case 0x85:
			strcpy(errMsg,"��Ƭд��������");
			break;
		case 0x86:
			strcpy(errMsg,"��Ƭд������;�ж�");
			break;
		case 0x87:
			strcpy(errMsg,"��ֵ��Ƭ����Ӧ");
			break;
		case 0x90:
			strcpy(errMsg,"���Ǳ�ϵͳ��׼�Ŀ�Ƭ");
			break;
		case 0x91:
			strcpy(errMsg,"��Ƭ������Ч��");
			break;
		case 0x92:
			strcpy(errMsg,"���д����Ӧ�ô������");
			break;
		case 0x93:
			strcpy(errMsg,"�Ƿ���");
			break;
		case 0x94:
			strcpy(errMsg,"��������");
			break;
		case 0x95:
			strcpy(errMsg,"Ǯ������");
			break;
		case 0x96:
			strcpy(errMsg,"Ǯ����������");
			break;
		case 0x97:
			strcpy(errMsg,"Ǯ��δ����");
			break;
		case 0x98:
		case 0x9A:
			strcpy(errMsg,"Ǯ����ͣ��");
			break;
		case 0x99:
			strcpy(errMsg,"Ǯ���������ƻ�");
			break;
		case 0x9F:
			strcpy(errMsg,"������Ϣ�����ƻ�");
			break;
		case 0xAF:
			strcpy(errMsg,"��Ƭ������ʱ");
			break;
		case 0xB0:
			strcpy(errMsg,"���ײ�����;�ж�");
			break;
		case 0xB1:
			strcpy(errMsg,"�����ж�");
			break;
		case 0xB2:
			strcpy(errMsg,"ǰһ��ָ��δִ�л�ִ��ʧ��");
			break;
		case 0xC1:
			strcpy(errMsg,"������ֵ���󱻾ܾ�");
			break;
		case 0xC2:
			strcpy(errMsg,"������ֵ��֤ʧ��");
			break;
		case 0xC3:
			strcpy(errMsg,"���׽���ύ����");
			break;
		case 0xCE:
			strcpy(errMsg,"������ֵЭ�����");
			break;
		case 0xCF:
			strcpy(errMsg,"���ײ�����ʱ");
			break;
		case 0xD0:
			strcpy(errMsg,"Զ�̶�д��ִ�д�");
			break;
		case 0xE0:
			strcpy(errMsg,"MIFAREӲ����ʼ������");
			break;
		case 0xE1:
			strcpy(errMsg,"SAMӲ����ʼ������");
			break;
		case 0xE2:
			strcpy(errMsg,"�������");
			break;
		case 0xE3:
			strcpy(errMsg,"��������");
			break;
		case 0xE4:
			strcpy(errMsg,"����ʹ���");
			break;
		case 0xE5:
			strcpy(errMsg,"��·ͨѶ��ʱ");
			break;
		case 0xE6:
			strcpy(errMsg,"�ڲ�FLASHд����");
			break;
		case 0x30:
			strcpy(errMsg,"����ͷ��");
			break;
		case 0x31:
			strcpy(errMsg,"��Ƭ��һ��");
			break;	
		case 0x32:
			strcpy(errMsg,"��ˮ�Ų�һ��");
			break;
		case 0x33:
			strcpy(errMsg,"MAC��");
			break;
		case 0x3F:
			strcpy(errMsg,"��֧�ֵ�����");
			break;
		default:
			strcpy(errMsg,"����");
			break;
	}

	wrLog("���������ش�����Ϣ��%s",errMsg);
	return 0;
}


int CYCTYPTrade::P_TRAN_QRY3(const TSDLLPosInfo& dPosInfo , PYCT_DATA pyct_data , unsigned char SAK , unsigned char* cardInfo , unsigned char exeFlag , unsigned char* outBuf , int* len )
{
	unsigned char inBuf[512] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = -1,iSelect;
    wrLog("��P_TRAN_QRY3��");
	//PKI
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//������
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//�߼�����
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//SAK
	inBuf[bufLen ++] = SAK ;
	//��������Ϣ1
	memcpy(inBuf + bufLen , cardInfo , 32);
	bufLen += 32;
	//ִ�в���
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
		TS_OutPutMsg(dPosInfo , "����ʧ�ܣ�" , 0 , iSelect );

		return 1;
	}
	return 0;
}

/************************************************************************/
/* Author: Sun Chengcheng                                               */
/* Data  : 2012-06-06 15:03                                             */
/* �˺���Ϊ�������������������                                         */
/* ������� dPosInfo���ն���Ϣ��com��comType�����ںš���������          */
/* commandCode������ģ�inBuf��inLength�������������򳤶�           */
/* outBuf��outLengthΪ������������==ȫ����==                          */
/* ע�⣬�˴������ֻ�� �ĵ��е� ������ ��                              */
/* ���ĵ� 8.4.2.3��R_CPU_LOAD��P117�У�commandCode Ϊ0x7B����inBuf����  */
/* ������Ϣ2��inLenghtӦ��Ϊ32                                          */
/* ���������outBuf��ͳһ�ο��ĵ�P101ҳ�������������ݣ�                 */
/* Header(1�ֽ�) + Len(1�ֽ�) + Command(1�ֽ�) + Status(1�ֽ�) +        */
/* Data(�˴���Ϊ������֤��4�ֽ�+SW1SW2���ֽ�) + CheckSum(1�ֽ�)         */
/* =11 �ֽ� �� �� *outLength = 11                                       */
/* ������Ҫע�⣬���Ϊ���ڴ�RAW�ֽ�����Ҫʹ�������е���              */
/* byte_to_hex��ת��Ϊ�ɼ�����                                          */
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
/* �˺���Ϊ���ֵ��̨������������                                       */
/* ������� dPosInfo���ն���Ϣ��PYCT_DATA �� ���ͨ�ṹ��Ϣ��           */
/* ʹ�õ���shid��yct_sequence_NO���ʲ���Ϊ��                            */
/* infoType��ver��Ϊ��Ϣ�����롢���İ汾�ţ�                            */
/* ��ASCII�뼴�ɣ��� (BYTE*)"AA74"                                      */
/* encryptMethod��endian������������㷨����ֵ�����ֽ���              */
/* ʮ���������δ��룬Ŀǰ�����㷨ֻ֧��0x02                             */
/* inBuf , inLen ��Ϊ�������г��ˡ�������ˮ�š�������ţ�У����֮�⡱�� */
/* ����ȫ�����ݼ��䳤�ȣ���R_COMMAND�е�inBuf��inLenght����             */
/* outBuf��outLenΪ������������==ȫ����== ����R_COMMAND���ƣ�         */
/* ������ܺ��ȫ���ģ���������ͷ�������壬��ϸ��                       */
/* ����ʹ�ÿɲο�����P_TRAN_QRY3��P_CPU_CANCLE�ȷ���ʹ�ú���            */
/* ������Ҫע�⣬                                                       */
/* 1�����Ϊ���ڴ�RAW�ֽ�����Ҫʹ�������е���byte_to_hex��ת��        */
/* Ϊ�ɼ����� 2�������ڲ��Զ�ʵ��AES���ܡ�MD5У�飬                     */
/* ����Ϊ�����Ķ��ڷ��������λ�ò�һ��û�з�װ�������жϣ�             */
/* ������ʹ��ErrReturn                                                  */
/************************************************************************/
int CYCTYPTrade::P_COMMAND(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data ,const unsigned char* infoType,  \
						   const unsigned char* ver, int encryptMethod , int endian , unsigned char* inBuf , int inLen , \
							unsigned char* outBuf, int* outLen)
{
	int sndLen = 0;
	int Len    = 0;
	int iRet   = 0;
	unsigned char headerData[256] = { 0 };					//����ͷ
	unsigned char bodyData[256] ={0 };						//������
	unsigned char sndData[512] = { 0 };						//�����ͱ���
	unsigned char md5_dig[16] = { 0 };
	unsigned char byteTmp[512] = { 0 };
	unsigned char debugInfo[256]= { 0 };
	char undecripdata[512] = { 0 };
	char SHID[16+1];

	CTools::hex_to_byte( infoType , headerData , 4);		//��Ϣ������
	sndLen += 2;
	CTools::hex_to_byte( ver      ,  headerData+sndLen , 4);//���İ汾��
	sndLen += 2;
	headerData[4] = encryptMethod;							//����������㷨
	headerData[5] = endian;									//��ֵ�����ֽ���
	sndLen += 2;
	sprintf((char*)byteTmp , "%04X" , 16 + inLen);
	CTools::hex_to_byte( byteTmp , headerData + sndLen , 4);
	sndLen += 2;
	memcpy(sndData , headerData , sndLen);					//����ͷ���

	bbzero(SHID);
	strcpy(SHID,(char *)pyct_data->SHID);
	CTools::hex_to_byte((BYTE *)SHID , bodyData , 16);	//������ˮ��
	Len += 8;
	char seq_NO[10] = { 0 };
	sprintf(seq_NO , "%08X" , pyct_data->yct_sequence_NO);	
	CTools::hex_to_byte( (BYTE*)seq_NO ,  bodyData+Len , 8);//�������
	Len += 4;
	memcpy(bodyData + Len , inBuf , inLen);					//������Ϣ�򣬴����ֱ��copy��ʹ�ô˺�����Ҫע��˴�
	Len += inLen;

	md5_csum( bodyData , Len ,  md5_dig );					//MD5SUMУ������
	memcpy(bodyData+Len , md5_dig , 4);
	Len += 4;
	
	memcpy(debugInfo , headerData , sndLen);
	memcpy(debugInfo + sndLen , bodyData , Len);
	wrHex("�����ͱ���-δ����:" , debugInfo , sndLen + Len);
	CTools::byte_to_hex((BYTE *)debugInfo,(BYTE *)undecripdata,sndLen + Len);
	wrLog("undecripdata: %s",undecripdata);

	//������ˮ�ż���
	CDes::TripleDesEncrypt((char*)CKEY, (char*)pyct_data->SHID , (char*)byteTmp );
	CTools::hex_to_byte(byteTmp , sndData + 8 , 16);
	sndLen += 8;
	//�������������ּ���
	int encrLen = 0;
	unsigned char* encr_packdata =  AESEncryptEx( bodyData+8 , Len - 8 , pyct_data->SKEY , 16, &encrLen );
	memcpy(sndData + 16 , encr_packdata , encrLen);
	free(encr_packdata);
	encr_packdata = NULL;
	sndLen = 8 + 8 + encrLen ;								//��ͷ+������ˮ��+�Ѽ��ܰ��岿��
	//ͨѶ���ͨ������
	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(YCT_IP , YCT_PORT , &sd);
	if (iRet != 0 )
	{
		return 1;
	}
	unsigned char rcvData[256] = { 0 };
	int  rcvLen		  = 0 ;

	//���汨��ԭ�ĵ�result_message_third,���ý���״̬Ϊ0 �������������AA73 M1��ֵ�����ύ��AA95 cpu��ֵ�����ύ��AA85cpu��ֵ�ύ
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
		return 2;									//FE04���ģ���Ҫ����ǩ��
	}
	if (rcvData[4] != 0x02)
	{
		return 1;									//�ݲ�֧�ֳ�0x02 AES-128����ļ��ܷ�ʽ
	}

	unsigned char bodyDecrypted[256] = { 0 };		//���ܺ�ı�����
	char tmp_shid[16+2] = { 0 };	
	int pack_len = 0;

	CTools::byte_to_hex( rcvData + 8 , byteTmp , 8);		//������ˮ��
	CDes::TripleDesDec((char*)CKEY , (char*)byteTmp , tmp_shid);		//����������ˮ��
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
		wrLog("MACУ�����");
		return 1;									//����������MACУ�����
	}

	memcpy(outBuf , rcvData , 8);					//����ͷ
	memcpy(outBuf + 8 , bodyDecrypted , pack_len);	//�����岻��У����
	memcpy(outBuf + 8 + pack_len , MAC2 , 4);		//У����
	*outLen = 8 + pack_len + 4 ;					//���س���
	wrHex("�յ�����-�ѽ���:" , outBuf , *outLen);
	free(des_data);
	des_data = NULL;
	return 0;
}

//��ֵ���� flow:ԭϵͳ��ˮ�ţ�pz����ֵƾ֤�ţ�amount�����׽��ѷ�Ϊ��λ
//����ǰ����ʧ�ܣ����翨Ƭ���������Ҳ���ƾ֤�ŵȵȣ�����1
//�ɹ�����0
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
	strcpy(pyct_data.org_yctstan,flow); //���ͨ������ˮ��
	com = pgbl->com;
	comType = pgbl->comType;
// 	if (init_sign(pgbl , com , comType , yct_data) != 0)
// 	{
// 		return 1;
// 	}
	
	char cur_signTime[30+1];
	CDB::getYct3SignInfo(pgbl->dPosInfo.pos_no,&pyct_data,cur_signTime);

	//����������Ϣ
	do 
	{		
		iRet = R_PUB_QRY_PHYSICS_INFO2(pgbl->dPosInfo , com , comType , (BYTE*)outBuf , &outLen);
		if (iRet == 0)
		{
			CTools::byte_to_hex((BYTE*)outBuf + 4 , (BYTE*)pyct_data.ticket_physicalNo , 8);
			CTools::byte_to_hex((BYTE*)outBuf + 12, (BYTE*)pyct_data.ticket_no , 8);
			wrLog("������:%s",pyct_data.ticket_physicalNo);
			//wrLog("�߼�����:%s",pyct_data.ticket_no);
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo , "�����������û�ſ�����ѿ��ŵ��������ϣ�\n����Enter�����ԣ�����ESC�����ء�" ,0 , iSelect );
			if (iSelect != 0)
			{
				return iRet ;
			}
		}
	} while (iRet != 0); //ȡ�����߶�����ȷ
	
	//���ý������Ͳ�ѯ
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
		//ִ�н������Ͳ�ѯ
		goto here;
	}
	
	if (iRet==-1) //����ʧ��
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
		//�ֽ��ֵ��PE����
		wrYctDataLog("after charge cancel", pyct_data);
		//PEȷ�������
		
		pyct_data.status = trade.YCT_AckRevTrade( pyct_data,pyct_data.status);
		if (pyct_data.status==5) //��������ɹ�����Ҫ����ȷ�ϰ�
		{
			pyct_data.status=trade.YCT_AckRevTrade(pyct_data,pyct_data.status);
		}

		if (pyct_data.status == 5)
		{		
		
		//	CDB::updateYctTxnStatus((char *)flow,"8"); //���µ��������ͨ���� edit by liuxiao
			pyct_data.status=8;
			CDB::updateYctYPData(pyct_data);
			//���ô�ӡ��Ϣ
			TS_OutPutMsg(pgbl->dPosInfo,"���ͨ��ֵ�����ɹ�\n����ȷ�ϡ���ӡƾ֤",1,iSelect);
			set_yctthird_priCont(2,&pyct_data,priCont);
			CPrint prt;
			CPack pack;
			char m_title[64];
			strcpy(m_title,"���ͨ��ֵ����");
			prt.set_in_par(*pgbl);
			prt.print_yctthird_detail(m_title,priCont);
		}
		else
		{
			//CDB::updateYctTxnStatus((char *)flow,"9");
			pyct_data.status=9; //����ʧ��
			CDB::updateYctYPData(pyct_data);
			TS_OutPutMsg(pgbl->dPosInfo , "E������ʧ�ܣ����ͨ��ֵ����ʧ�ܣ�����ϵ���߿ͷ���Ctrl+H��" ,0 , iSelect );
		}
	}
	else
	{		
	
		//CDB::updateYctTxnStatus((char *)flow,"8"); //edit by liuxiao
		pyct_data.status=8;
		CDB::updateYctYPData(pyct_data);

		//���ô�ӡ��Ϣ
		set_yctthird_priCont(2,&pyct_data,priCont);
		TS_OutPutMsg(pgbl->dPosInfo,"���ͨ��ֵ�����ɹ�\n����ȷ�ϡ���ӡƾ֤",1,iSelect);
		CPrint prt;
		CPack pack;
		char m_title[64];
		strcpy(m_title,"���ͨ��ֵ����");
		prt.set_in_par(*pgbl);
		prt.print_yctthird_detail(m_title,priCont);
	}

	return 0;
}

//CPU������
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

	//����׼������
	iRet = R_PUB_SET_READCARDINFO(pgbl->dPosInfo, com , comType  , pyct_data->ticket_no , query_info,  (char*)SW1SW2);
	if (iRet != 0)
	{
		return -1;
	}
	
	//��CPU����Ϣ
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
// 		//����YCT_DATA����ʾ����Ϣ
// 		CTools::byte_to_hex((BYTE *)outBuf+4,(BYTE *)pyct_data->pki_card,4);//pki������
// 		CTools::byte_to_hex((BYTE *)outBuf+8,(BYTE *)pyct_data->ticket_physicalNo,8);//������
// 		CTools::byte_to_hex((BYTE *)outBuf+16,(BYTE *)pyct_data->ticket_no,8); //�߼�����
// 		CTools::byte_to_hex((BYTE *)outBuf+24,(BYTE *)pyct_data->ticket_amo_bef,4); //Ǯ�����
// 		CTools::byte_to_hex((BYTE *)outBuf+28,(BYTE *)pyct_data->downLimit,1); //Ǯ������
// 		CTools::byte_to_hex((BYTE *)outBuf+29,(BYTE *)pyct_data->upperLimit,3);//���ͨ�������
// 		CTools::byte_to_hex((BYTE *)outBuf+49,(BYTE *)pyct_data->SW1SW2,2); //��Ƭ����״̬��
// 		pyct_data->card_status=outBuf[32];
// 		memcpy(countInfo,outBuf+33,16);//������Ϣ
// 		
// 		wrLog("====������Ϣ====");
// 		wrLog("������:%s",pyct_data->pki_card);
// 		wrLog("�߼�����:%s",pyct_data->ticket_no);
// 		wrLog("��Ƭ���Ϊ:%s",pyct_data->ticket_amo_bef);//���ͨ���
// 		wrLog("��Ƭ����:%s",pyct_data->upperLimit);		//���ͨcpu������
// 		wrLog("��Ƭ����Ϊ��%s",pyct_data->downLimit); //���ͨcpu������
// 		wrLog("���ͨ����״̬:%s",pyct_data->SW1SW2); //���ͨ����״̬��
// 		wrLog("====������Ϣ====");
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
	unsigned char tjjyxh[2+2] = { 0 };		//�ѻ��������
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
	
	//�ն����¶�������ȡ��Ƭ������Ϣ
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
	//wrHex("������Ǯ����" , (BYTE*)cardInfo + 24 , 4);

	//�ȶ�ǰ������ֵ
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
		wrLog("�����ɹ�");
		pyct_data->status='4';
	}
	else
	{
		wrLog("����ʧ��");
		TS_OutPutMsg(pgbl->dPosInfo,"����ʧ��!",1,iSelect);
		return -1; //����ʧ�ܣ�������Ҫ���ش���

	}
	
	for (int i=0 ; i != 3; ++i)
	{
		//Ĭ�������ύ���ɹ����˳�
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

	if (followerup_oper[0] != 0x00)
	{
		return 1;
	}
  
	return 0;
}

//M1������
int CYCTYPTrade::Charge_Cancle_M1(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow , const char* pz , double amount,BYTE *query_info)
{
	int iRet = 1 ,  outLen = 0 , rspCode = 0 , iSelect = 0;
	char outBuf[1024] = { 0 };
	char SW1SW2[64] = { 0 };
	char trade_info[88] = { 0 };
	BYTE cancel_info[64]={0}; //������Ϣ
	char cancle_status; //�������������������
	char cancle_stan[8]={0}; //����ϵͳ��ˮ��
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
		TS_OutPutMsg(pgbl->dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		for (int i = 0;i<2; i++)
		{
			iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,0);
			
			if (rspCode != 128 && rspCode != 132)
			{
				break;
			}
			TS_OutPutMsg(pgbl->dPosInfo,"��ѿ��ŵ���������",1,iSelect);
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
		//TS_OutPutMsg(pgbl->dPosInfo,"����ʧ�ܣ�",1,iSelect);
		return -1;
	}

	memcpy(cancle_stan,outBuf+16,8); //����ϵͳ��ˮ��
	unsigned char bizData[16] = { 0 };//R_M1_CANCLE_INIT��R_M1_CANCLE��Ҫ�Ľ������ݣ�����һ�����
	memcpy(bizData , outBuf + 28 , 16);

	iRet = R_M1_CANCLE_INIT(pgbl->dPosInfo , com , comType , (char*)bizData , (BYTE*)outBuf , &outLen);//��Ҫ��ȡ�������ݵĳ�����ʼ������
	
	if (iRet != 0)
	{
		return -1;
	}
	
	memcpy(cancel_info,outBuf+4,64); //������Ϣ

	bbzero(outBuf);
	outLen=0;
	iRet = R_M1_CANCLE(pgbl->dPosInfo , com , comType , (char*)bizData , (BYTE*)outBuf , &outLen);
	cancle_status=outBuf[3]; //�������
	wrHex("��������������",(BYTE *)outBuf,outLen);

	if (iRet != 0)
	{
		return -1; //���������ִ�д���
	}

	iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,1);
	if (rspCode == 128 || rspCode == 132)
	{
		TS_OutPutMsg(pgbl->dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		for (int i = 0;i<2; i++)
		{
			iRet = R_M1_GET_CARDINFO(pgbl->dPosInfo , com , comType , pyct_data , (BYTE*)trade_info , rspCode,1);
			
			if (rspCode != 128 && rspCode != 132)
			{
				break;
			}
			TS_OutPutMsg(pgbl->dPosInfo,"��ѿ��ŵ���������",1,iSelect);
		}	
	}
	
	if (iRet != 0)
	{
		return -1;
	}
	
	for (int i=0 ; i != 3; ++i)
	{
		//Ĭ�������ύ���ɹ����˳�
		bbzero(outBuf);
		outLen=0;
		iRet = P_M1_CANCLE_SUBMIT(pgbl->dPosInfo,pyct_data,cancle_stan,cancel_info,(BYTE *)trade_info,cancle_status,(BYTE *)outBuf,&outLen);
		
		if (iRet == 0 || i<3)
		{
			break;
		}

	}
	//���µ��������������ݣ�

	if (iRet!=0)
	{
		pyct_data->yct_ack=0; //������Ҫ�ط�commit��

	}

	bbzero(temp);
	len=28;

	memcpy(temp,outBuf + len,7);
	wrHex("��������ʱ��: ",(BYTE *)outBuf + len,7);
	CTools::byte_to_hex((BYTE *)temp,(BYTE *)pyct_data->tradeTime,14);
    len+=7;
	
	bbzero(pyct_data->yct_termid);
	memcpy((BYTE *)pyct_data->yct_termid,outBuf + len,6);
	wrHex("�ն˱��: ",(BYTE *)pyct_data->yct_termid,6);
	len+=6;
	
	bbzero(pyct_data->trader_id);
	memcpy((BYTE *)pyct_data->trader_id,outBuf + len,8);
	wrHex("�̻����: ",(BYTE *)pyct_data->trader_id,8);
	len+=8;
	
	bbzero(pyct_data->trader_name);
	memcpy((BYTE *)pyct_data->trader_name,outBuf + len,20);
	wrHex("�̻����: ",(BYTE *)pyct_data->trader_name,20);
	len+=20;
	
	bbzero(pyct_data->note);
	memcpy(pyct_data->note,outBuf + len,64);
	wrHex("��ע: ",(BYTE *)pyct_data->note,64);
	len+=64;
	
	bbzero(followup_oper);
	memcpy(followup_oper,outBuf + len,1);
	wrHex("��������: ",followup_oper,1);

	//�ж��Ƿ����ɹ� ����������ǰ���ֵ
	wrLog("����ǰ���:%.2f",atof(pyct_data->ticket_amo_bef));
	wrLog("���������:%.2f",atof(pyct_data->ticket_amo_aft));
	wrLog("���׽��:%.2f",amount);
	wrLog("����״̬:%s",cancle_status);

	
	if (cancle_status=='\x00' && atof(pyct_data->ticket_amo_aft)==atof(pyct_data->ticket_amo_bef)-amount)
	{
		//�����ɹ�
		pyct_data->status=4;
	}

	else
	{	
		TS_OutPutMsg(pgbl->dPosInfo,"����ʧ��!",1,iSelect);
		return CANCLE_KEY; //����ʧ�ܣ�������Ҫ���ش���
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
	wrLog("��P_CPU_CANCLE_QRY��");
	BYTE hexamo[8+1] = {0};
	unsigned char inBuf[1024] = { 0 } ;
	unsigned char bytetmp[256]= { 0 } ;
	int bufLen = 0 , iRet = 0;
	//PKI������
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf , bytetmp , 4);
	bufLen += 4;
	//������
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//�߼�����
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//Ǯ�����
	CTools::getHexAmo(pyct_data->ticket_amo_bef,(BYTE *)hexamo);
	wrHex("hexamo: ",hexamo,4);
	//CTools::hex_to_byte((BYTE*)hexamo , bytetmp , 8);
	//wrHex("bytetmp: ",bytetmp,4);
	memcpy(inBuf + bufLen , hexamo , 4);
	bufLen += 4;
	//�������
	CTools::hex_to_byte((BYTE*)down_limit , bytetmp , 2);
	memcpy(inBuf + bufLen , bytetmp , 1);
	bufLen += 1;
	//������Ϣ
	memcpy(inBuf + bufLen , countInfo , 16);
	bufLen += 16;
	//���׽��
	char amount_tmp[10] = {0 };
	sprintf(amount_tmp , "%08X" , amount);
	CTools::hex_to_byte((BYTE*)amount_tmp , bytetmp , 8);
	wrHex("���׽�" , bytetmp , 4);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//ϵͳ��ˮ��
	CTools::hex_to_byte((BYTE*)flow , bytetmp , 16);
	wrHex("ϵͳ��ˮ�ţ�" , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//��ֵƾ֤��
	CTools::hex_to_byte((BYTE*)pz , bytetmp , 8);
	wrHex("��ֵƾ֤�ţ�" , bytetmp , 4);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//RFU
	bufLen += 31;	//������Ϊ��

	iRet = P_COMMAND(dPosInfo , pyct_data , (BYTE*)"AA91" , (BYTE*)"0001" , 0x02 , 0x80 , \
		 inBuf , bufLen , outBuf , len);
	if (iRet != 0)
	{
		return iRet;	//���� or ���ճ���
	}
	if (ErrReturn(dPosInfo , outBuf + 24) != 0)
	{
		return 1;		//���������
	}
	return 0;
}

int CYCTYPTrade::R_CPU_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, char* bizInfo3 , unsigned char* outBuf , int* outLen)
{
	wrLog("��R_CPU_CANCLE_INIT��");
	return R_COMMAND(dPosInfo , com , comType , 0x75 , bizInfo3, 32, (char*)outBuf , outLen);
}

int CYCTYPTrade::P_CPU_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , const unsigned char* cardinfo1 , const unsigned char* retCode , int readerCode , unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = 0;
	wrLog("��P_CPU_CANCLE��");

	//ϵͳ��ˮ��
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//PKI
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//����Ϣ1
	memcpy(inBuf + bufLen , cardinfo1 , 128);
	bufLen += 128;
	//��Ƭ������
	memcpy(inBuf + bufLen , retCode , 2);
	bufLen += 2;
	//������������
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
	wrLog("��R_CPU_CANCLE��");
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
	char termVer[16+2] = { 0 };//�ն�����汾��
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
	iRet = con.conToHost(YCT_MONITOR_IP , YCT_MONITOR_PORT , &sd); //���������ͨ��ط�����
	if (iRet != 0 )
	{
		return 1;
	}
	unsigned char rcvData[256] = { 0 };

	//��ʼ��com �� comtype����Ϣ���ҽ���ǩ������ȡYCT_DATA�ṹ
//  	if (init_sign(pgbl , com , comType , yct_data) != 0)
//  	{
//  		return 1;
//  	}

	char cur_signTime[30+1];
	CDB::getYct3SignInfo(pgbl->pos_no,pyct_data,cur_signTime);

    com = pgbl->com;
	comType = pgbl->comType;

	//׼��P_MONITOR��
	//1��R_PUB_GETVERSION
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
	//2��R_PUB_GET_PKISTATE
	iRet = R_PUB_GET_PKISTATE(pgbl->dPosInfo , com , comType , outBuf , &outLen);
	if (iRet != 0)
	{
		return 1;
	}
	MonitorStat = outBuf[4];
	//3�����ݿ��ȡ���һ�εļ�¼
	CDB::get_Last_Biz(lastBizStat , lastBizStan);
	
	//׼��P_MONITOR��
	//��ͷ
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
	//����
	CTools::hex_to_byte((BYTE*)pyct_data->SHID , byteTmp , 16);
	memcpy(monitor + bufLen , byteTmp , 8);
	bufLen += 8;
	bufLen += 1;//�����汾��
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , byteTmp , 8);
	wrHex("PKI�����ţ�" , byteTmp , 4);
	memcpy(monitor + bufLen , byteTmp, 4);
	bufLen += 4;
	time_t t = time(0);
	strftime((char*)byteTmp , sizeof(byteTmp) , "%Y%m%d%H%M%%S" , localtime(&t));
	CTools::hex_to_byte(byteTmp , monitor + bufLen , 14);
	bufLen += 7; //ʱ���
	monitor[bufLen++] = lastBizStat ; 
	memcpy(monitor + bufLen , lastBizStan , 8);
	bufLen += 8;
	memcpy(monitor + bufLen , ReaderVer , 60);
	bufLen += 60;
	memcpy(monitor + bufLen , termVer , 16);
	bufLen += 16;
	monitor[bufLen++] = MonitorStat ;
	//����׼�����
	wrHex("��ʱ��ر��ģ�" , (BYTE*)monitor , bufLen);

	memset(rcvData , 0 , sizeof(rcvData));
	iRet = ST_commuWithYCTIC3(pgbl->dPosInfo, pyct_data ,  com , comType,sd , monitor , bufLen ,  rcvData , &rcvLen);
	if (iRet != 0)
	{
		wrLog("��ر��ķ���ʧ��!");
	}
	else
	{
		wrLog("��ر��ķ��ͳɹ�!");
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

	//��ʼ�����ںţ���������,ǩ����ȡYCT_DATA�ṹ
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
			//���ÿ�Ƭ������Ϣ
			CTools::byte_to_hex((BYTE*)outBuf + 4 , (BYTE*)pyct_data->ticket_physicalNo , 8);
			CTools::byte_to_hex((BYTE*)outBuf + 12, (BYTE*)pyct_data->ticket_no , 8);
		}
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo , "������Ϣ����\n����Enter�����Զ���������ESC��ȡ�������ء�" , 0 , iSelect );
			if (iSelect != 0)
			{
				wrLog("iSelect : %d" , iSelect);
				return iRet ;
			}
		}
	} while ( iRet != 0);

	//���µ�0x03����ִ����չӦ�ò���
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

	//������ݱ��ķ�����ʾ�˵��Թ��û�ѡ����չӦ��
	int bizCount = outBuf[20];
	char bizName[5] = { 0 };
	char bizDesc[31]= { 0 };
	TSMENULIST menuList;

	memset(&menuList,0,sizeof(menuList));
	menuList.iTotalNum = bizCount;
	strcpy(menuList.acTitle, "��ѡ����չҵ������\n");

	for (int i=0; i!= bizCount ; ++i)
	{
		//Сѧ�ӷ�����ʽ���Լ���ҵ����������ƫ��
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
	
	memcpy(bizDesc , outBuf + 21 + 34*(iSelect - 1) , 4);	//ҵ������
	wrHex("ҵ�����ͣ�" , (BYTE*)bizDesc , 4);
	
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

		//�����Ϣ����-��չ��Ϣ����
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
		memcpy(code , outBuf + 31 , code_length);	//������ָ��ȺͲ���ָ��ȡ��
		memcpy(flow , outBuf + 16 , 8);				//ϵͳ��ˮ��ȡ��

		if (outBuf[28] == 0x01)
		{
			//�����ݷ��͸����������˳�
			iRet = R_EXTEND(pgbl->dPosInfo , com , comType , code_length , code , (BYTE*)outBuf , &outLen);
			if (iRet != 0)
			{
				return 1;
			}
			wBreak = false;
		}
		else if (outBuf[28] == 0x02)
		{
			//�޺���ָ��
			wBreak = false;
		}
		else if (outBuf[28] == 0x03)
		{
			//����ע��ʾ�û��Ƿ����ִ��ָ��
			char infMessage[128] = { 0 };
			strcpy(infMessage , "��������Ҫ����ִ���������ݣ�����Enter����������ESC��ȡ����\n" );
			memcpy(infMessage + strlen(infMessage) , outBuf + 31 + code_length , 64);
			TS_OutPutMsg(pgbl->dPosInfo , infMessage , 0 , iSelect);
			if (iSelect != 0)
			{
				return 1;
			}
		}
		else if (outBuf[28] == 0x04)
		{
			//ֱ��ִ�к���ָ��
			cur_code = 0x01 ;
			wBreak = true;
		}
		else if (outBuf[28] == 0x05)
		{
			//����ֵ //TODO
			wBreak = false;
		}
		else if (outBuf[28] == 0x06)
		{
			//������ //TODO
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
	memcpy(inBuf , bizInfo , 16);//��������
	time_t t = time(0);
	strftime((char*)byteTmp , sizeof(byteTmp) , "%Y%m%d%H%M%%S" , localtime(&t));
	CTools::hex_to_byte(byteTmp , (BYTE*)inBuf + 16 , 14);
	return R_COMMAND(dPosInfo , com , comType , 0xBB , inBuf , 16 + 7 ,(char*) outBuf , len);
}

int CYCTYPTrade::R_M1_CANCLE(const TSDLLPosInfo& dPosInfo , int com , int comType , char* bizInfo , unsigned char* outBuf , int* len)
{
	char inBuf[64] = { 0 };
	unsigned char byteTmp[64] = { 0 };
	memcpy(inBuf , bizInfo , 16);//��������
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
	
	//ϵͳ��ˮ��
	//CTools::hex_to_byte(flow , bytetmp , 16);
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//PKI
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf + bufLen , bytetmp , 4);
	bufLen += 4;
	//ҵ������
	memcpy(inBuf + bufLen , bizType , 4);
	bufLen += 4;
	//ǰ�β�������
	inBuf[bufLen++] = firstType;
	//ǰ�β�����ˮ��
	memcpy(inBuf + bufLen , firstFlow , 8);
	bufLen += 8;
	//��չ��Ϣ����
	sprintf((char*)bytetmp , "%04X" , infLen);
	CTools::hex_to_byte(bytetmp , inBuf + bufLen , 4);
	bufLen += 2;
	//��չ��Ϣ
	memcpy(inBuf + bufLen , cardInfo , infLen);
	bufLen += infLen;
	//RFU ��֤��������ˮ������16�ı���
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
	long amo = amount;//�Է�Ϊ��λ
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
flow:ϵͳ��ˮ�ţ���Ӧ���ǳ����׶�2��ϵͳ��ˮ��
cancleInfo:������Ϣ������Ϣ�Ƕ�����ָ��R_M1_CANCLE_INIT���ؽ��
bizInfo��������Ϣ�����µ��ö�����ָ��R_M1_GET_CARDINFO���ؽ������Ϣ��������Ч
readerCode���������������ݡ�������ָ��R_M1_CANCLE���ص�status���
outBuf:���ر����峤��
*/
int CYCTYPTrade::P_M1_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, const char* flow , unsigned char* cancleInfo , unsigned char* bizInfo , unsigned char readerCode, unsigned char* outBuf , int* len)
{
	unsigned char inBuf[256] = { 0 };
	unsigned char bytetmp[50]= { 0 };
	int bufLen = 0 , iRet = 0;

	//PKI������
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf+ bufLen , bytetmp , 4);
	bufLen += 4;
	//ϵͳ��ˮ��
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//������
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//�߼�����
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//������Ϣ
	memcpy(inBuf + bufLen , cancleInfo , 64);
	bufLen += 64;
	//������Ϣ
	memcpy(inBuf + bufLen , bizInfo , 88);
	bufLen += 88;
	//������������
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

	wrLog("��P_CPU_CANCLE_SUBMIT��");
	
	//PKI������
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , bytetmp , 8);
	memcpy(inBuf+ bufLen , bytetmp , 4);
	bufLen += 4;
	//ϵͳ��ˮ��
	memcpy(inBuf + bufLen , flow , 8);
	bufLen += 8;
	//������
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_physicalNo , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//�߼�����
	CTools::hex_to_byte((BYTE*)pyct_data->ticket_no , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//TAC
	memcpy(inBuf + bufLen , TAC , 4);
	bufLen += 4;
	//MAC2
	memcpy(inBuf + bufLen , MAC2 , 4);
	bufLen += 4;
	//��Ƭ������
	memcpy(inBuf + bufLen , cardCode , 2);
	bufLen += 2;
	//������������
	inBuf[bufLen] = readerCode ; 
//	memcpy(inBuf + bufLen,(BYTE *)readerCode,1);
	bufLen += 1;
	//Ǯ�����
	memcpy(inBuf + bufLen , bef , 4);
	bufLen += 4;
	//�������
	inBuf[bufLen] = down_bef ;
	//memcpy(inBuf + bufLen,(BYTE *)down_bef,1);
	bufLen += 1;
	//�������
	memcpy(inBuf + bufLen , up_bef , 3);
	bufLen += 3;
	//������Ϣ
	memcpy(inBuf + bufLen , countInfo , 16);
	bufLen += 16;
	//RFU
	memcpy(inBuf + bufLen , RFU , 25);
	bufLen += 25;
	//������Ҫ�ύ������
	
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

//��ʼ������������ȡȫ�ֵ� ���ںš��������͵���Ϣ��ǩ����ȡyct_data�ṹ�����ô�ֵ��
int CYCTYPTrade::init_sign(CGlobal *pgbl , int& com , int& comType , YCT_DATA& yct_data)
{
	int iRet = 0, iSign = 0, iSelect = 0, firstSign = 0;
	//char sysTime[30];
	long lInterval = 0;
	char cur_signInTime[30+1];
	char fmtDate[10+1];
	char fmtTime[10+1];

	bbzero(cur_signInTime);

    //���ͨ���ݽṹ��ʼ��
	init_yct_data(&yct_data);

	iRet = CTrade::getComPar(pgbl->posInfo.leftType, pgbl->posInfo.rightType, pgbl->posInfo.inType, &com, &comType);
	if( iRet != 0 )
	{
		return 1;
	}

// 	if((pgbl->posInfo).rightType == 0x08)	//���������
// 	{
// 		comType = 0x05;
// 		iRet = ST_initReader3(pgbl->dPosInfo,com,comType,&yct_data); //������������������ͨ��ֵ��������ʼ��
// 		if(iRet)
// 		{
// 			return 1;
// 		}
// 	}
// 	else //��ʱ������������������
// 	{
// 		TS_OutPutMsg(pgbl->dPosInfo, "��ЧӲ�����޷��������ף����������ã�������������ϵ�����׿ͷ�", 0, iSelect);
// 		return 1;
// 	}

	//��ȡ�ն�ǩ��ʱ��
	iRet = CDB::getYct3SignInfo(pgbl->dPosInfo.pos_no,&yct_data,cur_signInTime);
	if (iRet == -3)
	{
		firstSign = 1; //�ն˵�һ��ǩ��
		wrLog("���ͨ��ֵ�ն˵�һ��ǩ��");
	}

	//�����ǵ�һ��ǩ����Ƚϵ�ǰǩ��ʱ����ϵͳʱ��
// 	if(firstSign != 1)
// 	{
// 		CTools::getCurDatetime(sysTime);
// 		
// 		lInterval = CDB::getTimeInterval(cur_signInTime,sysTime);
// 		
// 		if( lInterval <= SKEY_VALIDATE )
// 		{
// 			wrLog("���ͨ��ֵ�ն�һ���������ٴ�ǩ��");
// 			return 0;
// 		}
// 		wrLog("���ͨ��ֵ�ն���Ҫ����ǩ��");
// 	}


	BYTE m_party=FROMSUCC, m_rspcode=SUCC_MSG;

	if((pgbl->posInfo).rightType == 0x08)	//���������
	{
		comType = 0x05;
		if(YCT_VER==3)
		{
			iRet = ST_initReader3(pgbl->dPosInfo,com,comType,&yct_data); //������������������ͨ��ֵ��������ʼ��
		}
		
		else 
		{
			iRet = ST_initReader(pgbl->dPosInfo, com, comType); //����������ڶ������ͨ��ֵ��������ʼ��
		}
		if(iRet)
		{
	 		return 1;
	 	}
	}
	else //��ʱ������������������
	{
		TS_OutPutMsg(pgbl->dPosInfo, "��ЧӲ�����޷��������ף����������ã�������������ϵ�����׿ͷ�", 0, iSelect);
		return 1;
	}

	iRet = yct_SignIn(pgbl->dPosInfo,com,comType,&yct_data,&m_party,&m_rspcode);
	if (iRet == 0)
	{ 
		wrLog("ǩ���ɹ�");
	}
	else
	{		
		wrLog("ǩ��ʧ��");
		TS_OutPutMsg(pgbl->dPosInfo, "���ͨ�ն�ǩ��ʧ�ܣ������´�iPos�������½���ǩ��", 0, iSelect);
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
	wrLog("��R_PUB_SET_READCARDINFO��");
	//�߼�����
	CTools::hex_to_byte(logic_No , bytetmp , 16);
	memcpy(inBuf + bufLen , bytetmp , 8);
	bufLen += 8;
	//��ѯ��Ϣ
	memcpy(inBuf + bufLen , qryInfo , 48);
	bufLen += 48;

	return R_COMMAND(dPosInfo , com , comType , 0x95 , (char*)inBuf , bufLen , (char*)outBuf , len);
}

//ԭ������������ϰ�ߣ���װ�ṹ�����ã���д֮
int CYCTYPTrade::R_CPU_GET_CARDINFO(const TSDLLPosInfo& dPosInfo , int com , int comType , unsigned char* outBuf , int*len)
{
	wrLog("��R_CPU_GET_CARDINFO��");
	return R_COMMAND(dPosInfo , com , comType , 0x79 , NULL , 0 , (char*)outBuf , len);
}

//���ڷ������жϣ�offset Ϊ����������λ�õ�ƫ��
int CYCTYPTrade::ErrReturn(const TSDLLPosInfo& dPosInfo , const unsigned char* offset)
{
	char zero[4] = { 0 };
	int iSelect = 0;
	if (memcmp(offset , zero , 4) == 0)
	{
		return 0;
	}
//	TS_OutPutMsg(dPosInfo , (char*)offset + 4 , 0 , iSelect );
	TS_OutPutMsg(dPosInfo , "��������������û�ѿ��ŵ���������" , 0 , iSelect );

	return 1;
}

int CYCTYPTrade::R_PUB_QRY_PHYSICS_INFO2(const TSDLLPosInfo& dPosInfo , int com ,int comType , unsigned char* outBuf, int*len)
{
	wrLog("��R_PUB_QRY_PHYSICS_INFO��");
	return R_COMMAND(dPosInfo , com , comType , 0x93 , NULL , 0 , (char*)outBuf , len);
}


// ��������Ƽ� ���ͨ��ֵ�������汾
// ���������״���
// ��Σ�yct_serial(8):������ˮ��;pki_card(8):PKI������
// ���Σ�ticket_amo_aft(12):Ʊ����ֵ�����; errCode:�������
// ����ֵ
// 0���ɹ�
// 1�������������ʧ�ܣ����δ֪
// 2�����������صĴ��󣬽��δ֪
// 3�������ͨ����ʧ�ܣ����δ֪
// 4��PKI�������������δ֪
// 5��ʧ�� 
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
	char type[4+1];//��Ϣ������
	char ver[4+1];//���İ汾��
	BYTE encAlgo[2];//��������������㷨
	BYTE endian[2];//��ֵ�����ֽ�˳��

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
			wrLog("ǩ���ɹ�");
		}
		
		if(iRet) //ǩ��ʧ��
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
		
		wrHex("����������㷨: ",encAlgo,1);
		wrHex("��ֵ�����ֽ�˳��: ",endian,1);
		
		memcpy(strbodylen,result_message + msg_len,4);
		wrLog("strbodylen: %s",strbodylen);
		CTools::HexToDec(strbodylen,templen);
		body_len = atoi(templen);
		msg_len+=4;
		
		CTools::hex_to_byte((BYTE *)result_message + msg_len + 16 + 8,(BYTE *)inBuf,body_len-(16+8+8)/2);
		msg_len+=body_len * 2 -16-8-8;
		
		wrHex("inBuf: ",inBuf,body_len-(16+8+8)/2);
		
		//���ͨ��������
		iRet = con.conToHost(YCT_IP, YCT_PORT, &sd);
		if( iRet==0 )
		{
			NULL;
		}
		else
		{
			*party = FROMCTSI;
			*rspcode = YCTTIMEOUT;
			return 3;// 3�������ͨ����ʧ��
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

		pyct_data->yct_ack = 1; //����״̬Ϊ1��ȷ��״̬
		*party = FROMSUCC;
		*rspcode = SUCC_MSG;
		closesocket(sd);

	return 0;
}


//�������
/*
int CYCTYPTrade::P_SET_MONITOR(CGlobal *pgbl,const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data)
{
	unsigned char monitor[114]    = { 0 };
	unsigned char setmonitor1[87] = { 0 };
	unsigned char setmonitor2[256]= { 0 };
	unsigned char byteTmp[256]    = { 0 };
	char termVer[16+2] = { 0 };//�ն�����汾��
	int setFlag = 0;
	int bufLen  = 0;
	int iRet	= 0;
	int rcvLen	= 0;
	int upfreq  = 0;
	int iSelect = 0;
	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(YCT_MONITOR_IP , YCT_MONITOR_PORT , &sd); //���������ͨ��ط�����
	if (iRet != 0 )
	{
		return 1;
	}

	//SET_MONITOR�׶�1����ͷ
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
	//����
	CTools::hex_to_byte((BYTE*)pyct_data->SHID , byteTmp , 16);
	memcpy(setmonitor1 + bufLen , byteTmp , 8);
	bufLen += 8;
	bufLen += 1;//�����汾��ȡ0
	CTools::hex_to_byte((BYTE*)pyct_data->pki_card , byteTmp , 8);
	memcpy(setmonitor1 + bufLen , byteTmp , 4);
	bufLen += 4;
	bufLen += 2;//�ն�����ȡ0000
	bufLen += 64;//RFU����
	
	wrHex("������÷��ͱ��ģ�" , setmonitor1 , bufLen);

	unsigned char rcvData[256] = { 0 };
	iRet = ST_commuWithYCTIC3(pgbl->dPosInfo, pyct_data , com , comType ,sd , setmonitor1 , bufLen ,  setmonitor2 , &rcvLen);
	if (iRet != 0)
	{
		return 1;
	}
	char zeroQ[4] = { 0 };
	if (memcmp(setmonitor2 + 20 , zeroQ , 4) != 0)
	{
		return 1;	//���������
	}
	wrHex("������÷��ر��ģ�" , setmonitor2 , rcvLen);
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
	 
	wrYct3MonitorLog((char *)dPosInfo.pos_no,"�ն˼�����÷�����Ϣ");
 	wrYct3MonitorLog((char *)dPosInfo.pos_no,"���Ƶ�ʣ�%s",pgbl->monitor_frequency);
 	wrYct3MonitorLog((char *)dPosInfo.pos_no,"�´θ���ʱ�䣺%s",pgbl->next_monitortime);
	char strtemp[1024];
 	bbzero(strtemp);
	CTools::tobyte(pgbl->announce,strtemp);
 	wrYct3MonitorLog((char *)dPosInfo.pos_no,"������Ϣ��%s",strtemp);
	wrYct3MonitorLog((char *)dPosInfo.pos_no,"�ն�����汾�ţ�%s",pgbl->term_ver);

	return 0;
}
*/

/**************************������ ���ͨscc��ֵ�������� ����****************************************/

//��ʼ�����ͨ�ṹ��
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
	pyct_data->yct_ack=1; //Ĭ��ֵΪ0
	/**************************���������ͨ��������*********************/

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

	/*****************���ͨ�������ת��*******************/
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