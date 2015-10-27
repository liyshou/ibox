#include "YCTYPTrade.h"


class CYCTPurchase:public CYCTYPTrade
{
public:
	/**************************** Card Purchase ****************************************/
	//���۷�����
	//in: BYTE *app_type - Ӧ������, char *purchase_amo - �۷ѽ��, BYTE *extension_stan - ��չӦ����ˮ
	int static Purchase_Handle(CGlobal *pgbl,PYCT_DATA pyct_data,BYTE *app_type,char *purchase_amo,BYTE *extension_stan,BYTE* party, BYTE* rspcode);
	
	//in: BYTE *app_type - Ӧ������, char *purchase_amo - �۷ѽ��, BYTE *extension_stan - ��չӦ����ˮ
	//out: BYTE *consume_info - ���ѽ�����Ϣ
	int static P_PURCHASE_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *app_type,char *purchase_amo,BYTE *card_pycicalInfo1,BYTE *extension_stan,BYTE *consume_tradeinfo);
	
	//�۷ѳ�ʼ��
	//in: consume_tradeinfo - ���ѽ�����Ϣ
	//out: consume_info - ������Ϣ, status - ������������
	int static R_PURCHASE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *consume_tradeinfo,BYTE *consume_info,BYTE *status);
	//�۷�
	//in: consume_info - ������Ϣ, status - ������������.
	//out: BYTE *consume_cmd - ����ָ��
	int static P_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *consume_info,BYTE *status,BYTE *consume_cmd);
	
	//�۷�
	//in: BYTE *consume_cmd - ����ָ��
	//out: BYTE *TAC - ������֤��,BYTE *MAC2 - MAC2 
	int static R_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *consume_cmd,BYTE *wallet_type,BYTE *MAC2);
	
	int static P_PURCHASE_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *wallet_type,BYTE *MAC2,BYTE *status,BYTE *card_appInfo,BYTE *followerup_oper);
	
	//�ȽϿ۷�ǰ�����
    //in: PYCT_DATA pyct_data
	//out: 0 - �۷�ǰ������ֵ��ȷ��1 - �۷�ǰ������ֵ����
	int static balance_compare(PYCT_DATA pyct_data,char *trade_amount);
	/**************************** Card Purchase ****************************************/

};