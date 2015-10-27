// YCTYPTrade.h: interface for the CYCTYPTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(YCTTRADE_TRAH__BA427F3F_89A8_429A_BBBD_C794CFDB3EE3__INCLUDED_)
#define YCTTRADE_TRAH__BA427F3F_89A8_429A_BBBD_C794CFDB3EE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <winsock2.h>
#include "Global.h"
#include "Pack.h"
#include "Trade.h"

//������д���ı�־
#define TAG 7
#define VER 1
#define STAT 0x00
//��������ͨǮ���Ĳ�ѯ���ѱ�־
#define NORMALPAY 0
#define NORMALENQ 1
#define AUTOCHARGE 2
//�������Ĺ�������
#define NORMAL_PAY_CMD 0x85
#define NORMAL_NOTFINISH_CMD 0x86
#define MONTH_SIGNIN_CMD 0x87
#define MONTH_PAY_CMD 0x88
#define MONTH_NOTFINISH_CMD 0x89
#define MONTH_ENQ_CMD 0x8A
//���ͨ������Ļ���
#define FROMSUCC 0x00//�ɹ�

#define FROMIC 0x01//���������صĴ��󣬳�ֵʧ��
#define FROMYCT 0x02//���ͨ���صĴ��󣬳�ֵʧ��
#define FROMPE 0x03//PE���صĴ���(����ʱ)
#define FROMCTSI 0x04//CTSI���صĴ��󣬳�ֵʧ��


#define FROMIC_N 0x05//���������صĴ��󣬳�ֵ���δ֪
#define FROMYCT_N 0x06//���ͨ���صĴ��󣬳�ֵ���δ֪
#define FROMCTSI_N 0x07//CTSI���صĴ��󣬳�ֵ���δ֪

#define FROMYCT_O 0x08//���ֱͨ�ӷ��صĴ���

//FROMSUCC
#define SUCC_MSG 0x01//�ɹ�����ʾ���ͨ��ֵ�ɹ�
#define SUCC_NOMSG 0x02//�ɹ�������ʾ
#define SUCC_MSG_LAST 0x03//�ɹ�����ʾ�ϱ����ͨ��ֵ�ɹ�
#define SUCC_NEEDOTHEROPR 0x04//�ɹ���������ز�������Ҫִ�н������Ͳ�ѯ
#define SUCC_NEEDEXAPPOPR 0x05 //�۷ѳɹ���������ز���������Ҫִ����չӦ�ò���
#define SUCC_PURCHASE 0x06//�۷ѳɹ�
#define SUCC_TRANSFER 0x07//ת�Ƴɹ�


//FROMIC
#define ERROR_FROMIC 0x01 //���������صĴ���
#define R_APP_CARD_UPGRADE_EXEFAIL 0x02//ִ��ͬ��ָ��ʧ��


//FROMCTSI FROMCTSI_N
#define FATALERR 0xFF//����������ؼ��
#define COMNOTFOUND 0x01//û���ҵ�������
#define USERCANCEL 0x02//�û�ȡ��
#define PAYTIMEOUT 0x03//�۷ѳ�ʱ
#define ICTIMEOUT 0x04//���ջ���IC���ݳ�ʱ
#define CONNCOMFAIL 0x05//���Ӷ�����ʧ��
#define YCTTIMEOUT 0x06//���ջ���YCT���ݳ�ʱ
#define ICNOTSURE 0x07//����������ȷ������״̬
#define ICSAYFAIL 0x08//������ָ������ʧ��
#define PACKFAIL 0x09//���ʧ��
#define UNPACKFAIL 0x0A//���ʧ��
#define CARDNOTFIX 0x0B//��֧�ֵĿ�
#define PKICHGNOTFINISH 0x0C//PKI���Ѹ������޷���ȡ�ϱʽ��׽��
#define FAIL_LAST 0x0D//������ȡ�ϱʽ��׽����ȷ�ϳ�ֵʧ��
#define PAYBUTNOTFINISH 0x0E//�ѿ۷ѣ�����ֵ���δ֪
#define SIGNINFAIL 0x0F//ǩ��ʧ��
#define NORESULT 0x10//ǩ��ʧ��
#define CARDNOTFOUND 0x11//û���ҵ�������������
#define LASTTRANSFER_NOTDONE 0x12//�ÿ��ϴ�ת�Ƶĳ�ֵû�ɹ���ת����ֵ������
#define TRANSFER_ERROR 0x13//ת��ʧ��
#define PURCHASE_FAIL 0x14//�۷�ʧ��
#define BLACKLIST 0x15//��Ϊ������

// FROMYCT_O
#define AMOUNTRULE_VIALATION 0x30 //��ֵ���Υ��ҵ�����
#define BALANCE_EXCEED 0x96 //��ֵ��������
#define UNRECOGNIZED_TRADETYPE 0x32 //�޷�ʶ��Ľ�������

#define ST_HEAD 0xBA	//���������ͨѶ��ͷ
//�����������������
#define ST_ONTS_REQUEST	0x81	//Ѱ������
#define ST_ONTS_CHALLENGE 0x82	//��ս
#define ST_ONTS_AUTH_PREPARE 0x83 //���ɳ�ֵ��֤����
#define ST_ONTS_AUTH_EXECUTE 0x84 //�����ֵ��֤�������
#define ST_ONTS_CHARGE_PREPARE 0x85 //���ɳ�ֵ���ײ�������
#define ST_ONTS_CHARGE_EXECUTE 0x86 //ִ�г�ֵ����
#define ST_ONTS_COMMIT 0x87		//�ύ����״̬����
#define ST_ONTS_CONFIRM 0x88	//������ȷ�Ͻ��

#define ST_ONTS_UREQUEST 0x8A	//����δ�������״�����������
#define ST_ONTS_UCHALLENGE 0x8B	//����δ�������״�����ս����
#define ST_ONTS_UCOMMIT 0x8C	//�ύδ�������׵�״̬����
#define ST_ONTS_UCONFIRM 0x8D	//����δ��������ȷ�Ͻ��

//�����������Ӧ״̬��
#define MOC_ERROR_NORMAL		0x00
#define MOC_ERROR_SAM_PIN		0x62
#define MOC_ERROR_NOTAG         0x80
#define MOC_ERROR_CARD_READ		0x84
#define MOC_ERROR_CARD_NAMELIST	0x94
#define MOC_ERROR_TS_FALL		0xB1
#define MOC_ERROR_ONTS_REFUSED	0xC1
#define MOC_ERROR_ONTS_AUTH		0xC2
#define MOC_ERROR_ONTS_COMMIT	0xC3
#define MOC_ERROR_PKI_DES		0x72

//�������ת����
#define INTO_QRY 0x01 //ת�뿨��ѯ
#define OUT_QRY  0x02//ת������ѯ

#define SKEY_VALIDATE 12 //SKEY��Ч�ڣ���λСʱ ��

#define CKEY "11223344556677888877665544332211"

class CYCTYPTrade  
{
public:
	CYCTYPTrade();
	virtual ~CYCTYPTrade();

	// ����Ƿ���δ֪����
	// ��Σ�term_id:�ն˺ţ�
	// ���Σ�pyct_data:���ͨ������Ϣ
	// ����ֵ��
	// 0:���ͨ��
	// 1:����"δ֪�������"����
	// 2:ǩ����pki��������ԭ���ײ�һ��
	int static NotFinishChk(CGlobal gbl, char *term_id, char *pki_card, char *signin_serial);

	int static YCT_NormalTrade(CGlobal *pgbl, int payType, BYTE* party, BYTE* rspcode, char *priCont);
	//������
	//payType:0:��Ʊ 1:����Ʊ�Ȳ�ѯ 2:����Ʊֱ�ӳ�ֵ
	// ����ֵ��
	// 0���ɹ���ȷ��
	// 1���ɹ�δȷ��
	// 2��ʧ���ѳ���
	// 3��ʧ��δ����
	// 4��δ֪���
	// 5���۷�ǰʧ��
	int static TradeHdl(CGlobal gbl, char *title, int month, char *pki_card=NULL, char *signin_serial=NULL);

	int yctSetting(char *pinValue);
	void setInPar(CGlobal inGbl); //���ô������
	static int getComPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int &com, int &comType);//��ȡ��ӡ���� com: ʹ�ô��� comType:��������
	int static showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);
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
	int static MonthTrade(CGlobal *pgbl, int payType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont);

private:
	static int AutoSelectAmo(const TSDLLPosInfo dposInfo,char* czamo,char * tradeamount);
	static int Yct_AcctHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode);
	static int  P_ACCT_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, int &payType, char *czamount, char *money_limit,char *char_fre,char *ornotpassword,char *statusflag, BYTE *sign_status,BYTE *party,BYTE *rspcode);
	// ��������Ƽ��汾
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
	int static ST_NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode);

	//��������Ƽ��汾
	// ��ͨǮ���Ĳ�ѯ��ֵ
	// // ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������; enq:0:Ҫ��ֵ; 1:ֻ��ѯ; amount:��ֵ���
	// ���Σ�pyct_data:���ͨ������Ϣ; errCode:�������
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
	int static ST_NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode);  //���������ͨ��ֵ
	int static ST_NormalEnqPay2(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode); //�ڶ������ͨ��ֵ

	// ��������Ƽ��汾
	// �����ݽ��д��
	// ��Σ�tag:���ı�ʶ; ver:�汾; num:����˳���; len:���ĳ���(cmd,stat,data�ĳ���); cmd:�����ʶ; stat:״̬��ʶ; data:����;
	// ���Σ�sndData:�����������
	// ����ֵ��������
	int static ST_packData(int head, int len, int cmd, const BYTE *data, BYTE *sndData);

	//��������Ƽ��汾
	//�ϴ����ݰ������ͨ�����������ջ�Ӧ
	//��Σ�sdType:0:������;1:������; sd:�˿ں�; num, ��num������
	//		sndData, �������ͨ������
	//		sndLen, �������ͨ�����ݳ���
	//���Σ�rcvData, �Ӷ��������յ�����
	//		rcvLen, �Ӷ��������յ����ݵĳ���
	//����ֵ��
	//0���ɹ�
	//1�������ͨ����ʧ��
	//2�������������ʧ��
	int static ST_commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen);

	// ��������Ƽ��汾
	// ��ʼ��������
	// ����ֵ
	// 0���ɹ�
	// 1��ʧ��
	int static ST_initReader(const TSDLLPosInfo dPosInfo, const int com, const int comType);

	// ��������Ƽ��汾
	// ��ʾ������Ϣ
	int  static ST_showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);

	//==========================================================================================================

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
	int static NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode);
	
	// ��ƱǮ���Ĳ���������
	// ��Σ�signin_serial(16):������ˮ��;yct_serial(8):������ˮ��;pki_card(8):PKI������
	// ���Σ�ticket_amo_aft(12):Ʊ�����; errCode:�������
	// ����ֵ
	// 0���ɹ�
	// 1�������������ʧ�ܣ����δ֪
	// 2�����������صĴ��󣬽��δ֪
	// 3�������ͨ����ʧ�ܣ����δ֪
	// 4��PKI�������������δ֪
	// 5��ʧ��

	int static MonthNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *signin_serial, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode);
	
	//ȷ�������
	// ��Σ�yct_data:���ͨ������Ϣ
	// ���Σ���
	// ����ֵ��
	// 2��ȷ�ϳɹ�
	// 3��ȷ��ʧ��
	// 5�������ɹ�
	// 6������ʧ��
	int static AckRevTrade(CGlobal gbl, YCT_DATA yct_data);

	// ��ͨǮ��ѡ���ֵ���
	// ��Σ���
	// ���Σ�amount:���
	// ����ֵ��
	// 0��ȡ���ɹ�
	// ��0��ȡ���ʧ��
	int static NormalSelectAmo(CGlobal* gbl);

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
	int static NormalTrade(CGlobal *pgbl, int payType, BYTE* party, BYTE* rspcode, char *priCont);
	
	


	// ��ͨǮ���Ĳ�ѯ��ֵ
	// // ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������; enq:0:Ҫ��ֵ; 1:ֻ��ѯ; amount:��ֵ���
	// ���Σ�pyct_data:���ͨ������Ϣ; errCode:�������
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
	int static NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode);

	//��������:������������հ��ͽ��
	// ��Σ�flag:2:���Ѱ�;4:ȷ�ϰ�;5:������
	// ���Σ���
	// ����ֵ
	// 0���ɹ�
	// ��0��ʧ��
	int static packHdl(CGlobal gbl, CPack *ppack, int flag);

	// ȷ�Ͻ���
	// ��Σ�yct_data:���ͨ������Ϣ
	// ���Σ���
	// ����ֵ��
	// 0����ȷ��
	// 1��δȷ��
	int static AckTrade(CGlobal gbl, YCT_DATA yct_data);

	// ��������
	// ��Σ�yct_data:���ͨ������Ϣ
	// ���Σ���
	// ����ֵ��
	// 0���ѳ���
	// 1��δ����
	int static RevTrade(CGlobal gbl, YCT_DATA yct_data);

	int static MonthSelectWay(const TSDLLPosInfo dPosInfo, int payType, PYCT_DETAIL pyct_detail);



	// ��ƱǮ����ǩ��
	// ��Σ���
	// ���Σ�pki_card(8):PKI�����ţ�signin_serial(16):������ˮ��
	// ����ֵ
	// 0���ɹ�
	// 1�������������ʧ�ܣ�ǩ��ʧ��
	// 2�����������صĴ���ǩ��ʧ��
	// 3�������ͨ����ʧ�ܣ�ǩ��ʧ��
	// 4������ǩ��ʧ��
	int static MonthSignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode);

	// ��Ʊ��ѯ
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
	int static MonthEnq(const TSDLLPosInfo dPosInfo, const int com, const int comType, char ticket_type, char *yct_serial, char *amount, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode);

	// ��Ʊ��ֵ��ǰ1��2��
	// ��Σ�signin_serial(16):������ˮ��
	// ���Σ�pyct_data:���ͨ������Ϣ
	// ����ֵ
	// 0���ɹ�(��ѯ���ֵ)
	// 1�������������ʧ��
	// 2�����������صĴ���
	// 3�������ͨ����ʧ��
	// 4�����ͨ���صĴ���
	// 5��PKI������һ�£���Ҫ����ǩ��
	int static MonthPay1_2(const TSDLLPosInfo dPosInfo, const int com, const int comType, YCT_DETAIL yct_detail, char *yct_serial, char *pki_card, char *amount, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont);

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
	int static MonthPay3_4(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE* party, BYTE* rspcode);

	// ˢ����������PE���۷Ѱ�
	// ��Σ�biz_type(4):ҵ����룻amount(12):���
	// ���Σ���
	// ����ֵ
	// 0���۷ѳɹ�
	// 1���۷�ʧ��
	// 2����ʱ����
	int static PayPe(CGlobal *pgbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode);
	//���캯�� by liuxiao
	int static PayPe(CGlobal gbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode);
	// ���Ӳ���
	// ��Σ���
	// ���Σ���
	// ����ֵ
	// 0���ɹ�
	// 1��ʧ��
	int static connTest(const TSDLLPosInfo dPosInfo, const int com, const int comType);

	// �����ݽ��д��
	// ��Σ�tag:���ı�ʶ; ver:�汾; num:����˳���; len:���ĳ���(cmd,stat,data�ĳ���); cmd:�����ʶ; stat:״̬��ʶ; data:����;
	// ���Σ�sndData:�����������
	// ����ֵ��������
	int static packData(int tag, int ver, int num, int len, int cmd, int stat, const BYTE *data, BYTE *sndData);

	// ����������ͺͽ�������
	// ��Σ�dPosInfo:DLL������ն���Ϣ; com:�������; comType:��������;
	// ���Σ���
	// ����ֵ
	// 0���ɹ�
	// 1��ʧ��
	int static SndRcvIcData(const TSDLLPosInfo dPosInfo, const int com, const int comType, const char *title, const BYTE *downData, const int downLen, BYTE *upData, int *upLen, int baudRate=9600);

	// ��12λ��Ϊ4λhex��ʽ
	// ��Σ�amo:12λ�Ľ��
	// ���Σ�hexAmo:4λhex��ʽ�Ľ��
	// ����ֵ����
	void static getHexAmo(const char *amo, BYTE *hexAmo);

	// numλhex��ʽ��Ϊ��12λ���
	// ��Σ�hexAmo:4λhex��ʽ�Ľ�� num:λ��
	// ���Σ�amo:12λ�Ľ��
	// ����ֵ����
	void static getAsiAmo(const BYTE *hexAmo, int num, char *amo);

	// ����У����
	// ��Σ�data:����; dataLen:���ݳ���
	// ���Σ���
	// ����ֵ
	// 0��һ��
	// 1������
	int static isXor(const BYTE *data, int dataLen);

	/****************************************************/
	//�ϴ����ݰ������ͨ�����������ջ�Ӧ
	//��Σ�sdType:0:������;1:������; sd:�˿ں�; num, ��num������
	//		sndData, �������ͨ������
	//		sndLen, �������ͨ�����ݳ���
	//���Σ�rcvData, �Ӷ��������յ�����
	//		rcvLen, �Ӷ��������յ����ݵĳ���
	//����ֵ��
	//0���ɹ�
	//1�������ͨ����ʧ��
	//2�������������ʧ��
	/****************************************************/
	int static commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen);

	//---------------------------------------------------------------------------
	//������:   SndRcvYctData
	//���������������ͨ�������ͽ������󣬲�����Ӧ����Ϣ
	//���룺	sendData�����ͱ������ݣ�sendLen�����ͱ��ĳ��ȣ�recvLen�����ձ��ĳ���
	//�����	recvData�����ձ���
	//���أ�	>=0�����ճ��ȣ�<0������
	//---------------------------------------------------------------------------
	int static SndRcvYctData(const SOCKET sd, char *sendData,int sendLen, char *recvData, int *recvLen, int recvMaxLen);

	void static wrYctDataLog(char *msg, YCT_DATA yct_data);

	int static MW_showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);

	//int static showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);
	/***************************���������ͨ��ֵ����������ʼ*********************************************/
	
	//��������ʼ��
	int static ST_initReader3(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data);
	
	//��������������ݽ���
	int static ST_commuWithYCTIC3(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data, const int com, const int comType, const SOCKET sd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen);

	//�������������ǩ������
	int  static yct_SignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * party ,BYTE* rspcode);
	
	//��ȡ��������Ϣ
	int static R_PUB_QRY_PHYSICS_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE*physicsInfo);

	//���ÿ���Ϣ
	int static R_PUB_SET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,char* logical_card,BYTE * query_info,char *SW1SW2);
	
	//��ȡCPU��Ƭ��Ϣ
	//flag:  0 - ��ֵǰ��ѯ���, 1 - ��ֵ���ѯ���
	void static R_CPU_GET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE *count_info,int flag = 0);

	//ִ��CPUȦ��
	int static CPU_EnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, \
		PYCT_DATA pyct_data, char *amount, BYTE payType,BYTE *query_info,BYTE* party, BYTE* rspcode);
	
	//�������������� ������������ʼ�� R_SET_BLACKLIST_INIT
	int  static R_SET_BLACKLIST_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data );

	//���������

	int static P_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_info3, BYTE *rsp,BYTE * party,BYTE* rspcode);

	//�������ύ
	int static P_BLACKLIST_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,BYTE *status, BYTE * SW1SW2 ,BYTE * party,BYTE* rspcode);

	//���ú�����
	int  static R_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,BYTE *SW1SW2,BYTE *status);

	//ִ�к���������
	int static  EXCUTE_BLACKLIST(const TSDLLPosInfo dPosInfo,const int com,const int comType, PYCT_DATA pyct_data,\
		BYTE * query_info,BYTE *party, BYTE *rspcode);
	
	//CPUͬ����ѯ
	int  static R_CARD_QRY_UPGRADE(const TSDLLPosInfo dPosInfo, PYCT_DATA pyct_data, const int com, const int comType,BYTE *random_num,BYTE * rsp_message);

	//��Ƭͬ��Ӧ��
	int static R_APP_CARD_UPGRADE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE* upgrade_info);
	
	//cpu ͬ��
	int static P_M1_TO_CPU(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, \
		BYTE * upgrade_info, int  &next_status,BYTE *syn_order,BYTE * party,BYTE* rspcode);

	//ִ��cpu ͬ��

	int static	EXCUTE_M1_TO_CPU(const TSDLLPosInfo dPosInfo,const int com,const int comType,PYCT_DATA pyct_data,BYTE *query_info,BYTE * party, BYTE *rspcode);

	//	m1Ǯ����ѯ
	int static P_M1_LOAD_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_money,BYTE *pysical_info1,int appType,BYTE * query_info,BYTE * party,BYTE* rspcode);

	//M1��ȡ����Ϣ
	int  static R_M1_GET_CARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,int& rspStatus,int flag);
	
	int  static P_M1_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_info,int next_action,BYTE * charge_info,BYTE * party,BYTE* rspcode);

	//����M1��Ȧ��
	int  static R_M1_LOAD(const TSDLLPosInfo dPosInfo, const int com, int &status,const int comType,PYCT_DATA pyct_data,BYTE * charge_info);

	//M1����
	int static P_M1_ROLLBACK(const TSDLLPosInfo dPosInfo, const int com, const int comType, int status,PYCT_DATA pyct_data,char* reason,char *aa65_stan,BYTE *money,BYTE * TAC,BYTE * trade_info,int next_action,BYTE *party,BYTE *rspcode);

	//��M1���в�ѯ
	int static excute_M1_LOAD(const TSDLLPosInfo dPosInfo,const int com,const int comType,PYCT_DATA pyct_data,\
		int enq,BYTE *party,BYTE *rspcode,char *tradeamount,int payType);
	
	//�Խ�����ת�� M1ȡ����ѯΪ��λ��ǰ
	void static getAsiAmo3(const BYTE *hexAmo, int num, char *amo);

	//�������Ͳ�ѯ
	int static P_TRAN_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, \
		PYCT_DATA pyct_data,BYTE *card_pycicalInfo1,BYTE oper_code,int &trade_type,BYTE *query_info,BYTE *party, BYTE *rspcode);


	//��ֵ��������
	int static Charge_Cancle_CPU(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow ,  const char* pz , double amount,BYTE *query_info);
	int static Charge_Cancle_M1(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow ,  const char* pz , double amount,BYTE *query_info);
	//P_TRAN_QRY
	int static P_TRAN_QRY3(const TSDLLPosInfo& dPosInfo ,  PYCT_DATA pyct_data , \
		unsigned char SAK , unsigned char* cardInfo , unsigned char exeFlag , unsigned char* outBuf , int* len );
	
	//R_CPU_CANCLE_INIT
	int static R_CPU_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, char* bizInfo3 , unsigned char* outBuf , int* outLen);
	int static R_M1_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com , const int comType, char* bizInfo , unsigned char* outBuf , int* len);
	int static R_M1_CANCLE(const TSDLLPosInfo& dPosInfo , int com , int comType , char* bizInfo , unsigned char* outBuf , int* len);
	
	//�������������������
	int static R_COMMAND(const TSDLLPosInfo dPosInfo, const int com, const int comType,		\
		int commandCode , char* inBuf , int inLength , char* outBuf, int* outLength);
	//���ն˷�����������������
	int static P_COMMAND(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data ,const unsigned char* infoType,  \
		const unsigned char* ver, int encryptMethod , int endian , unsigned char* inBuf , int inLen , \
		unsigned char* outBuf, int* outLen);
	//P_CPU_CANCLE_QRY
	int static P_CPU_CANCLE_QRY(const TSDLLPosInfo dPosInfo,  PYCT_DATA pyct_data , unsigned char* countInfo , \
		const char* flow, const char* pz , 	int amount, unsigned char* outBuf , int* len);
	int static P_CPU_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , \
		const unsigned char* cardinfo1 , const unsigned char* retCode , int readerCode , unsigned char* outBuf , int* len);
	int static R_CPU_CANCLE(const TSDLLPosInfo dPosInfo , int com , int comtype , double amount , const unsigned char* bizInfo4 , \
		unsigned char* outBuf , int* len);
	//int static P_SET_MONITOR(const TSDLLPosInfo dPosInfo , int com , int comType);
	int static R_PUB_GET_PKISTATE(const TSDLLPosInfo dPosInfo , int com , int comType , unsigned char* outBuf , int* len);
	int static R_PUB_GETVERSION(const TSDLLPosInfo dPosInfo , int com , int comType , unsigned char* outbuf , int* len);
	int static R_PUB_SET_READCARDINFO2(const TSDLLPosInfo& dPosInfo , int com , int comType , \
		unsigned char* logic_No, unsigned char* qryInfo , unsigned char* outBuf , int* len );
	int static P_CPU_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, \
		unsigned char* flow, unsigned char* TAC , unsigned char* MAC2 , unsigned char* cardCode , unsigned char readerCode,	\
		unsigned char* bef , unsigned char* up_bef, unsigned char down_bef , unsigned char* countInfo, \
		unsigned char* outBuf , int* len);
	int static P_M1_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , double amount , \
		unsigned char* pz , unsigned char* bizInfo,  unsigned char* outBuf , int* len);
	int static P_M1_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, \
		const char* flow , unsigned char* cancleInfo , unsigned char* bizInfo , unsigned char readerCode,	\
		unsigned char* outBuf , int* len);
	
	int static P_EXTEND_QRY(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, unsigned char* outBuf , int* len);
	int static P_EXTEND(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data,  \
		unsigned char* flow , unsigned char* bizType , int firstType , unsigned char* firstFlow , \
		int infLen , unsigned char* cardInfo ,	unsigned char* outBuf , int* outLen);
	int static R_EXTEND(const TSDLLPosInfo dPosInfo , int com , int comtype ,\
		int codeLength , unsigned char* code , unsigned char* outBuf , int* len);

	int static R_CPU_GET_CARDINFO(const TSDLLPosInfo& dPosInfo , int com , int comType , unsigned char* outBuf , int*len);
	int static ErrReturn(const TSDLLPosInfo& dPosInfo , const unsigned char* offset);
	//��ȡ��Ƭ������Ϣ�����·�װ --scc
	int static R_PUB_QRY_PHYSICS_INFO2(const TSDLLPosInfo& dPosInfo , int com ,int comType , unsigned char* outBuf, int*len);
public:
	//��ֵ��������
	int static Charge_Cancle(CGlobal *pgbl , const char* flow ,  const char* pz , const char* amount,YCT_DATA pyct_data);
	//�ն˼��
	int static termMonitor(LPVOID pgbl,PYCT_DATA pyct_data);
	//��չӦ��,execFlag ǰ�β�������
	int static ExtendApp(CGlobal *pgbl , int execFlag );
	//CPUȦ���ʼ��
	/***************************���������ͨ��ֵ������������*********************************************/
	int  static Yct_AutoTradeHdl(CGlobal gbl,char* title);
	/**************************** CPU ****************************************/
// 	//CPUȦ���ʼ��
// 	int  static R_CPU_LOAD_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info_1);
// 	
// 	//CPUȦ���ѯ
// 	int static P_CPU_LOAD_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE *count_info,\
// 		PYCT_DATA pyct_data,  const char * amount,BYTE appType,\
// 		BYTE * trade_message1,BYTE *party,BYTE* rspcode);
	

	//CPUȦ���ʼ��
	//in: TSDLLPosInfo dPosInfo
	//     int com,int comType
	//     PYCT_DATA pyct_data
	//out: BYTE *trade_message1
	//     BYTE *trade_sequence
    //	   BYTE *yct_term_status
	//     BYTE *card_info
	int  static R_CPU_LOAD_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *trade_message1,BYTE *trade_sequence,BYTE *yct_term_status,BYTE *card_info);
	
	//CPUȦ���ѯ
	int static P_CPU_LOAD_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE *count_info,\
		PYCT_DATA pyct_data,  const char * amount,BYTE appType,\
		BYTE * trade_message1,BYTE *party,BYTE* rspcode);
		
    //R_CPU_LOAD CPUǮ��Ȧ��
	//in  : trade_message2 - ������Ϣ2
	//out : TAC - ������֤��, 
	//      load_status - Ȧ��״̬ 0 �����ύ��1 ������ϣ�δ�յ�AA84���Ļ�δ������������ֵָ�� 
	int static R_CPU_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *trade_message2,BYTE *TAC,int &load_status,BYTE *count_info);
	
	int static P_CPU_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_info,BYTE *yct_term_status,BYTE *trade_message2,BYTE *charge_authcode);
	
	int static P_CPU_LOAD_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		int &load_status,BYTE *TAC,BYTE *yct_term_status,BYTE *count_info,BYTE *charge_authcode,BYTE *followup_oper);
	/**************************** CPU ****************************************/

	//��ѯ��Ӧ����Ϣ
	//in: flag - 0 �۷�ǰ��ѯ��1 �۷Ѻ��ѯ(Ĭ��Ϊ0)
	//out: card_appinfo - ��Ӧ����Ϣ
	int static R_PUB_QRY_APP_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_appinfo,BYTE *ReaderStatus,int flag = 0);

	//�������ͨ��ֵ��������ӡ��ϸ����
	//in : type - ��������  0:��ֵ�ɹ�����   1:��ֵ���Ƴɹ����� 2:��ֵ�������� 
	//						3:ת����Ȧ�ᵥ�� 4:ת�뿨Ȧ�浥��   5:ת�뿨Ȧ�����Ƴɹ�����
	//out: priCont - ��ӡ��ϸ����
	int static set_yctthird_priCont(int type,PYCT_DATA pyct_data,char *priCont);
	
	//��ȡ����Ӧ״̬�뺬��
    int static get_cardStatus_errMsg(BYTE *SW1SW2,char *errMsg);

	//��ȡ������������Ϣ
    int static get_cardReader_errMsg(BYTE *status,char *errMsg);


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
	int static ST_YctThird_NotFinishHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		char *yct_serial,char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode);

	//�������
	int static P_SET_MONITOR(CGlobal *pgbl,const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data);

	int static init_sign(CGlobal *pgbl , int& com , int& comType , YCT_DATA& yct_data);

	//��ʼ�����ͨ�ṹ��
	int static init_yct_data(PYCT_DATA pyct_data);
	/***************************���������ͨ��ֵ������������*********************************************/

	CGlobal gbl;
	CPack pack;
	CTrade trade;
	SOCKET sd2;  //���ͨ������ͨ��socket
	char m_PKLogCard[8+1]; //��ֵ����(PK��)�߼�����
	char m_YCTLogCard[16+1]; //Ʊ��(���ͨ��)�߼�����
	char m_stan[8+1]; //ϵͳ��ˮ��
	char m_YCTAmo[10]; //���ͨ���
	char m_stat[4+1];	//����״̬��ʶ
	
	char YCTHOSTNAME[20];
	int  YCTPORT;
	
	int m_com;	//ʹ�ô���
	int m_comType;	//��������

};

#endif // !defined(AFX_YCTYPTRADE_H__BA427F3F_89A8_429A_BBBD_C794CFDB3EE3__INCLUDED_)
