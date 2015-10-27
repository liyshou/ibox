#include "YCTYPTrade.h"
#include "YCT3_Purchase.h"
#include "DB.h"
#include "Print.h"

class CYCTBlnTransfer:public CYCTYPTrade
{
public:

	/**************************** Balance Transfer ****************************************/
	//�������ת��
	int static BalanceTransfer_TradeHdl(CGlobal gbl, YCT_DATA yct_data,char *title);
	int static Balance_Transfer_QRY(CGlobal *pgbl, PYCT_DATA pyct_data,char *trans_amo,\
		BYTE *transqry_type,BYTE *wallet_type,BYTE* party, BYTE* rspcode, char *priCont);
	
	//ת�ƿ���ѯ
	//in: dPosInfo,com,comType,pyct_data,
	//	card_pycicalInfo1 - ��������Ϣ1,transqry_type - ת�����ͣ�01ת�뿨��ѯ��02��ʾת������ѯ��
	//out: query_info,party,rspcode
	int static P_TRANSFOR_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_pycicalInfo1,BYTE *transqry_type,BYTE *query_info,BYTE *last_transtan,BYTE *wallet_type,BYTE *party, BYTE *rspcode);

	
	//ת��ȷ�ϲ�ѯ
    //in:  trans_amo     - ת�ƽ��
	//out: out_deductamo - ת�����ۼ����
	//     in_chargeamo  - ת�뿨��ֵ���
	int static P_TRANSFOR_COMFIRM_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		char *trans_amo,BYTE *out_deductamo,BYTE *in_chargeamo);
	
	//��ʾת����Ϣ
	int static show_yctthird_transforinfo(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data,char *transfer_amo);


	//type - �������� 0 - ת����Ȧ�ᵥ�ݣ�1 - ת�뿨Ȧ�浥�ݣ�2 - ת�뿨Ȧ�����Ƴɹ�����
	int static set_blntransfer_priCont(int type,PYCT_DATA pyct_data,char *priCont);
	/**************************** Balance Transfer ****************************************/

protected:
private:
};