#include "YCTYPTrade.h"


class CYCTPurchase:public CYCTYPTrade
{
public:
	/**************************** Card Purchase ****************************************/
	//卡扣费流程
	//in: BYTE *app_type - 应用类型, char *purchase_amo - 扣费金额, BYTE *extension_stan - 扩展应用流水
	int static Purchase_Handle(CGlobal *pgbl,PYCT_DATA pyct_data,BYTE *app_type,char *purchase_amo,BYTE *extension_stan,BYTE* party, BYTE* rspcode);
	
	//in: BYTE *app_type - 应用类型, char *purchase_amo - 扣费金额, BYTE *extension_stan - 扩展应用流水
	//out: BYTE *consume_info - 消费交易信息
	int static P_PURCHASE_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *app_type,char *purchase_amo,BYTE *card_pycicalInfo1,BYTE *extension_stan,BYTE *consume_tradeinfo);
	
	//扣费初始化
	//in: consume_tradeinfo - 消费交易信息
	//out: consume_info - 消费信息, status - 读卡器返回码
	int static R_PURCHASE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *consume_tradeinfo,BYTE *consume_info,BYTE *status);
	//扣费
	//in: consume_info - 消费信息, status - 读卡器返回码.
	//out: BYTE *consume_cmd - 消费指令
	int static P_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *consume_info,BYTE *status,BYTE *consume_cmd);
	
	//扣费
	//in: BYTE *consume_cmd - 消费指令
	//out: BYTE *TAC - 交易认证码,BYTE *MAC2 - MAC2 
	int static R_PURCHASE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *consume_cmd,BYTE *wallet_type,BYTE *MAC2);
	
	int static P_PURCHASE_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *wallet_type,BYTE *MAC2,BYTE *status,BYTE *card_appInfo,BYTE *followerup_oper);
	
	//比较扣费前后余额
    //in: PYCT_DATA pyct_data
	//out: 0 - 扣费前后余额减值正确，1 - 扣费前后余额减值错误
	int static balance_compare(PYCT_DATA pyct_data,char *trade_amount);
	/**************************** Card Purchase ****************************************/

};