#include "YCTYPTrade.h"
#include "YCT3_Purchase.h"
#include "DB.h"
#include "Print.h"

class CYCTBlnTransfer:public CYCTYPTrade
{
public:

	/**************************** Balance Transfer ****************************************/
	//卡间余额转移
	int static BalanceTransfer_TradeHdl(CGlobal gbl, YCT_DATA yct_data,char *title);
	int static Balance_Transfer_QRY(CGlobal *pgbl, PYCT_DATA pyct_data,char *trans_amo,\
		BYTE *transqry_type,BYTE *wallet_type,BYTE* party, BYTE* rspcode, char *priCont);
	
	//转移卡查询
	//in: dPosInfo,com,comType,pyct_data,
	//	card_pycicalInfo1 - 卡物理信息1,transqry_type - 转移类型（01转入卡查询，02表示转出卡查询）
	//out: query_info,party,rspcode
	int static P_TRANSFOR_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_pycicalInfo1,BYTE *transqry_type,BYTE *query_info,BYTE *last_transtan,BYTE *wallet_type,BYTE *party, BYTE *rspcode);

	
	//转移确认查询
    //in:  trans_amo     - 转移金额
	//out: out_deductamo - 转出卡扣减金额
	//     in_chargeamo  - 转入卡充值金额
	int static P_TRANSFOR_COMFIRM_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		char *trans_amo,BYTE *out_deductamo,BYTE *in_chargeamo);
	
	//显示转移信息
	int static show_yctthird_transforinfo(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data,char *transfer_amo);


	//type - 单据类型 0 - 转出卡圈提单据，1 - 转入卡圈存单据，2 - 转入卡圈存疑似成功单据
	int static set_blntransfer_priCont(int type,PYCT_DATA pyct_data,char *priCont);
	/**************************** Balance Transfer ****************************************/

protected:
private:
};