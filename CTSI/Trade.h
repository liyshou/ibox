// Trade.h: interface for the CTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADE_H__C7E243D1_16DD_4096_B889_C55C2B319F08__INCLUDED_)
#define AFX_TRADE_H__C7E243D1_16DD_4096_B889_C55C2B319F08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "Global.h"
#include "Pack.h"
#include "xmlParser.h"


class CTrade  
{

private:
	CGlobal gbl;
	CPack pack;
	//char mac_random_num[8+1];	//��mac�õ������
	
public:
	CTrade();
	virtual ~CTrade();
	
	int updateConInfo();

	int signWorkID(char *workid);

	int BankHK(const char *type, int inputTel=0);//���л���
	int BankHK_telInqHdl(const char *type, int inputTel, int &first3mon);//��ѯ����Ƿ��
	int BankHK_telInqMsg(const char *type, int &first3mon);//�·���ѯ��Ϣ
	int BankHK_telPayHdl(const char *type, int bizIdx=0, int first3mon=0);//��Ƿ��
	int BankHK_prepay(const char *type);//��Ƿ��ʱԤ��
	int BankHK_nameconfirm(const char *type);//�û���ƥ��
	int BankHK_HK(int type);//�Ǽǻ���

	int Bankc_zz(const char *title); //���п�ת��

	int UnionTransfer_out(const char *title);//����ת��(ת��)
	int	UnionTransfer_in(const char *title);//����ת��(ת��)
	int UnionbanInqMoney(const char *title);	//���п�����ѯ(����)
	int UnionbanInqLast();
	int UnionbanInqHistory(int inqType);
	int UnionbanInqMenu(int type=0);

	int NH_Transfer(const char *title); //ũ��ת����ʾ�汾

	int banTransfer(const char *title);	//���л���
	int banTransLocal(const char *title, PTSTradeInfoRecv pTradeInfoRecv);	//���л��ʱ��ػ�
	int banInqTrade(int inqType);
	int banInqMoneyTrade(const char *title, char *amount_out=NULL);	//���п�����ѯ(����)
	int banInqLast();
	int banInqHistory(int inqType);
	int banReturnFail(char *response_code);
	int banInqMenu(int type=0);
		
	int getBankMny(const char *title);	//���п�����ѯ(���к�����)

	void set_in_par(CGlobal in_gbl); //���ô������
	void get_gbl(CGlobal &out_gbl);
	void get_xlt_info(char *card, int &track2_len, char *track2=NULL);
	
	int logonHdl();			//ǩ�����ɼ�����ϵ��
	int logonHdl(int type);	//ǩ�����¼�����ϵ��
	int mngCard(const char *type, int card_flag = 0,  int pay_flag = MNG_ONE, const char *title="", int minLen=12, int maxLen=16) ;	//ˢ����Ĭ��Ϊ��ͨ�� �������̣�Ĭ����ÿ�ʽ��Ѷ�ˢ��
	int getCardInfo(const char *type, char *card_info, int card_flag = 0, int pay_flag = MNG_ONE, const char *title="", int minLen=12, int maxLen=16);
	int getCardNum(char *card_info, int card_flag = 0 );
	int bindcard(char *cardno_in, char *cardno_out, int *cardno_out_len);

	int	telTrade(const char *type, int inputTel = 0 );	//���Ž��� type:����; oldTel:0-������绰���� 1-ʹ���ϴβ�ѯ���׵ĵ绰����
	int mblTrade();	//�ƶ�����
	int uniTrade();	//��ͨ����
	int	merTrade();	//�̻�����
	int	taxTrade(char *type, int input_flag=0);	//����˰
	int QQTrade(const char *type, int select=0,int input_flag=0);    //��ѶQQ KF:2006.10.24
	int RaffleTrade(const char *type, int select=0);    //��ʳ�ֵ
	int xltTrade(const char *type);	//С��ͨ�����׿�
	int cardChgPwd();	//�޸Ŀ����루e����С��ͨ�����׿���
	int cardInqHdl(char *amount_out=NULL);	//����ѯ��e����С��ͨ�����׿���
	int ecardInqHdl();//e����ѯ edit by liuxiao 20130923
	int yj_InqHdl(); //��ѯ���
	int cardInq_yctyp();
	int sellCardTrade(const char *type, int select=0,int input_flag=0);	//�ۿ�����
	int sellZWgameCard(const char *type, int select=0,int input_flag=0); //������Ϸ�����Ѹ�Ϊ����ͨ����
	int CATVTrade(const char *type);	//���ߵ���,2006-11-20 by lihk
	int rePrintHdl(int flag); //����Ʊ
	int newrePrtTrade(int flag);//��ݸ��stan�źͽ��Ѻ��벹��Ʊ
	int safeOutPutMsg(char *msg, int flag);

	int piccTrade(); //��ǿ��
	int foshanTrade(const char *title, const char *city_no);  //ȫʡ�ۺ��շ� 

	int merInqTrade(int inqFlag); //�̻����ײ�ѯ

	//С��ͨ2��ת��
	int xltAccTrans(const char *title);  
	//С��ͨ2�ڳ�ֵ
	int xltMnyIn(const char *title);
	//С��ͨ2�ڲ�ѯ
	int xltInqTrade(const char *title);

	//���г�ֵ flag:1.�ƶ�2.���ݵ���3.ȫʡ����
	int mblAirPay(const char *title, int biz_flag, int select=0, int input_flag=0,int mbl_input_flag=0);
	//�㶫�ƶ�ʡ�ڳ�ֵ
	int mblAirPayForGDYD(const char *title, int select=0, int input_flag=0,int mbl_input_flag=0);

	//�������ѯ����
	int nameInqHdl(char *user_name, int *user_name_len,char *amount);

	//����ѯ�ĵ���ֱ�� flag:1 ��һ����  2 ADSL  3 ����ͬ��
	int newAirPay(const char *title, int type_flag);

	//���Ӵ��� ����ֱ�� flag:1 ��һ����  2 ADSL  3 ����ͬ��
	int DZDBAirPay(const char *title, int type_flag, int functype);
	int phone_pay_trade(const char *title);
	//�ƶ�ȫ��ͨ��ֵ
	int mblTrade_pre(const char *title);	

	//e����ֵ
	int ecardMnyIn_Hdl(const char *title, int area_flag=0);
	int ecardMnyIn(const char *title, int area_flag=0);

	//e����ֵ����
	int ecardAppli_Hdl(const char *title);
	int ecardAppli(const char *title);

	//���µ�Ӱ�ۿ�Ʊ
	int filmPay(const char *title);

	int sendMsg();
	int print_fail(const char *title);//�ش�ӡʧ�ܷ�Ʊ
	int print_last(const char *title,int mbl_flag=0);//�ش�ӡ���һ�ŷ�Ʊ
	int getBit46(char *bit46);

	//���ŷ�Ʊ����
	int rePrintTel(const char *title);

	//e����ֵ��ʾ
	int getEcardMsg(const char *title, int flag=0);//e����ֵ��ʾ

	int testIcReaderCommand(const char *title);
	int testFSKPack(const char *title);

	//��ͨ����
	int trafficTrade(const char *title, int flag=0);
	int carnum_trafficTrade(const char *title, int flag=0);
	int traffic_pay(const char *title, char *car_type, char *paper_num);

    //ȫʡ����
	int provinceTrafficTrade(const char *title);
	int provinceTrafficInqHdl(const char *title);
	int provinceTrafficPayHdl(const char *title);

	//��������ƾ֤
	int rePrintTra(const char *title);

	//�������ͨPIN
	int SetPIN(int flag);

	
	int ysqCatv();
	//������ǩԼ
	int ysqCatv_new();
	//�̻�ǩԼ
	int customer_sign();

	//С��ͨԤ����100��300
	int YH_xlt(const char *type);

	//////////////////*���Թ��ܺ���*/////////////////////////
	
	int Test_TS_GetPosInfo(int flag);//�����ն���Ϣ�Ļ�ȡ flag: 1.�ն˵绰���룻2.�ն˺ţ�3.�ն��󴮿��豸��4.�ն��Ҵ����豸��5.�ն������豸
	int Test_TS_OutPutMsg(int flag);//������ʾ��Ϣ flag: 1.�·���ʾ��Ϣ��2.�·���ȫ��ʾ��Ϣ��3.�·�������ʾ��Ϣ
	int Test_TS_B4Print(int flag);//���Դ�ӡ���� flag: 1.��ӡ��2.��ȫ��ӡ
	int Test_TS_SendMsg(int flag);//���Դ�����Ϣ���� flag: 1.֪ͨ��2.���׽����3.�˵����׽����4.�˵���5.�汾�ţ�6.�ն����к�
	int Test_TS_PSAM(int flag);//����PSAM�� flag: 1.PSAM���ţ�2.PSAM�������
	int Test_TS_UpdateMsg(int flag);//���Ը��¹��� flag: 1.������ʾ��Ϣ��2.�����ն˲�����3.������ҳ��Ϣ��4.���±��ز˵���5.����PSAM����Կ
	int Test_TS_TradeControl(int flag);//���Խ��׿��ƺ��� flag: 1.�������俨�� 2.����ˢ������ȡ������Ϣ 3.���������� 4.������������ 5.��������ȫ�ַ� 6.�������� 7.�������� 8.������������ 9.����������ĸ 10.�������뺺��
	
	/////////////////////////////////////////////////////////


	int selectxlt();//С��ͨѡ��
	//	int telbell();//�߲�����
	//	int telmail();//��������
	int sx_ysl(int flag);//  ʵ��Ԥ����flag��1.���л�������2.���й̻���װADSL��3.����e8�ײͣ�4.e8���������ײͣ�5.����e6�ײ�
	int telIBSS(const char *title, char *ctrl3, char *biztid5);
	int selectcdma();//c��ѡ��



	//ҵ������
	int add_ADSL(const char *title);

	//�ǿ����˵㿨����
	int sell_xmcard(const char *title, int cflag=0);

	//�ն����� 
	int sell_pos(int sflag);
	int payonce(int subtype, const char *title);

	int UpdateMenus(int menu_group, char sysnum);

	//���� ����Ԥ����
	int dxysl_trad(const char *title);
	//��Զ ����Ԥ����
	int qy_dxysl_trad(const char *title, int t_select);

	//������Ʊ
	int nianpiaoTrade(const char *title, int flag);
	int simplePackHdl(char* send_data,char* recv_data);//��Ʊdemo�������

	//������Ʊ����
	int car_np_Trade(const char *title, int input_flag=0);

	//���ն˽�����ϸ��ѯ
	int iposTxnEnq(const char *title, int inout, int mode,int mbl_flag=0, int query_flag=0);//int inout:0���� 1��ֵ mode:0�ϰ�ģʽ 1Ա��ģʽ mbl_flag:2���ƶ� 1�ƶ� 0������ query_flag:0ȫ��1���24Сʱ
    //���ս�����ϸ��ѯ-�Ż�����
    int online_ser(const std::string& bizType, int b_boss);
    int drcx_online(int b_boss);//���ղ�ѯ
    int yjycx_online(int b_boss);//�½��ײ�ѯ
    int mxcx_online(int b_boss);//��ϸ��ѯ
    int cxcz_online(int b_boss);//��ѯ����
	int selfService_online(int b_boss);//��½�ӷ����Ż���ҳ
	int last_mnth_online(int b_boss);//���½��ײ�ѯ
	int zjdzcx_online(int b_boss);//�ʽ��˲�ѯ
	int cashTransfer_online(int b_boss);
    //�������2012-09-05 -- BEGIN --
    int iPos_Login(char* amount_out);
    int tenk_encrypt(const std::string& pwd , char* outBuf , size_t outMaxLen);
    int bosspwd_chg();//�ϰ������޸�
    int transpwd_chg();//ת�������޸�
    int paypwd_chg();//���ף�֧�������޸�
    //���ն˻�ȡһ������count������type��������ʽ����ͬTS_INPUT_XXX, title������ʾ��minlen��maxlen����������
    std::string get_line_from_term(int count , int type, int minlen , int maxlen , const std::string& title);
    //�ն���Ϣ��ʾ
    int info(const std::string& msg);
    //flag : 1 ֧������У�飬2ת������У�飬0�ϰ�����У��
    int pwd_chk(int flag);
    //�������2012-09-05 -- END --

	//�޸��ϰ�����(����PEֱ���ӿ�)
	int chgPEPwd(const char *title);
    //�޸�ת������(����PEֱ���ӿ�)
	int chgTransferPwd(const char *title);
	//�����ն˲���
	int updatePosPar(const char *title, int t_flag=0);
	//У���ϰ������ת������
	int checkPEPwd(char *pwd,int type=0);//type=0ΪУ���ϰ����룻type=1ΪУ��ת������
	//����ת������
    int setTransferPwd(char *newPwd,char *oldPwd);
	//У��ת�������Ƿ�Ϊ��
	int checkPwdNotNull();
	int checkPEPwd_Hdl(const char *title);
	int checkTransferPwd_Hdl(const char *title);
	//�޸�IPOS�������뷽ʽ
	int chgEPwdType(const char *title);
	//�޸��ս����޶�
	int chgDayAmo(const char *title);
	//CDMAѡ��
	int tel_card_select(const char *title, int tel_card_type); //tel_card_type ���ſ�����:1��153������  2.153���ڿ�

	//������Ǯ����ֵ
	int PayEasy_MoneyIn(const char *title);
	//�̻�����
	int chk_money(const char *title);

	//��ݸ����
	int busTrade(const char *type,int chargeType);//��ݸ����
	int busNotEndHdl(const char *type);//��ݸ����δ��������
	BYTE xor(const BYTE *data, int dataLen);
	int  busSndRcvIcData(const char *title, const BYTE *downData, const int downLen, BYTE *upData, int &upLen, char * pos_serial);
	//��ݸ����HID�������豸���ͼ���������
	int    busSndRcvHID_IcData(const char *title, const BYTE *downData, const int downLen, BYTE *upData, int &upLen, char * pos_serial);
	void   returnBusFail(char stat);  //��ݸ�������������󷵻ش���
	void   returnHIDBusFail(char stat);  //��ݸHID�������������󷵻ش���
	int   static  getComPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int *com, int *comType);
	int  dgtResubmit(TSDLLPosInfo dPosInfo,char *reSubmitField,char * tradeStatus,char * orderStatus);
	int dgtResubmitCheck(CGlobal *pgbl);
	int dgtCardsQuery();

	//���ÿ�����
	int Credit_Card_Repayments(const char *title);
	int Credit_Card_Repayments_Hdl(const char *title);

	//E������ת��
	int eCard_Trans(const char *title,int tag=0);
	int eCard_Trans_Hdl(const char *title,int tag=0);
	

	//��ˬ������
	int MyCoolPayEasy(const char *title);
	int MyCoolPayEasy_Hdl(const char *title, int input_flag=0);
	int MyCoolPayEasy_CarNum_traffic_inq(const char *title, char *incarnum, car_paper_info *car_paperinfo);
	int MyCoolPayEasy_CarTax_inq(const char *title, char *carnum, char *mny);

	//�̻�����ת
	int Customer_Trade();
	
	int revAppli(const char *title);

	//�����������
	int SetTitle();

	//ϵͳ���¸���
	int GetNewMessage(int flag=0);

	//��ݸ����ײ�����
	int DG_ADSL_xc(const char *title, int type_flag);
	int new_DG_ADSL_XC(const char*title);

	//����ֱ��˫ѡ��ģʽ
	int DX_AirPay_D(const char *title, int type_flag, int input_flag=0, int select_flag=0);

	//���ڵ���
	int sz_dx_trade(const char *title);
	
	int mngCard_Hdl(const char *type, const char *title="", int card_flag=0,  int pay_flag=-1, int minLen=12, int maxLen=16);
	int reGetPwd(const char *type);
	int returnFail(char *response_code);	//ʧ�ܷ���
	int returnFail2(char *response_code);

	//��ͻ��汾
	int Submission(const char*type);
	int Checkmission(const char*type);
	int Paymission(const char*type);

	//�ͷ�����
	int OpenKF(int flag=0);

	//������ʮ� Q����ȡ
	int Present_handsel(const char*type);
	int Present_handsel_Hdl(const char*type);


	//��ݸ�ֻ��ź�
	int Sell_blank_phonecard(const char*title, int type); // 0 ��ݸ���� 1 ��ͨ
	int Confirm_blank_phonecard(const char*title, int type);	// 0 ��ݸ���� 1 ��ͨ
	int Confirm_blank_phonecard_hdl(const char*title, int type); // 0 ��ݸ���� 1 ��ͨ
	int Sell_phoneno(const char*title);
	int Sell_phoneno_hdl(const char*title);

	//����ҵ��Ԥ����
	int telaffair_preaccept(const char* title);

	//����ֱ��
	int pep_query_hdl(char* areacode, char* num, char* month, char* feetype, int type_flag);
	int pep_query(char* areacode, char* num, char* month, char* feetype, char* returnstring);
	int pep_show_bill(char* totalfee, char* area_code, int flag);
	int pep_distribute();

	//��Ϸ�ۿ�
	int sell_txtgame_card(const char* type);

	//��Ϸֱ��
	int game_recharge_showGames(const char* title, int type); //0: ȫ��  1:�ؼ���
	int game_recharge_showAmount(const char* title);
	int game_recharge_do1(const char* title);
	int game_recharge_do2(const char* title);
	int qqGame_recharge_do(const char* title);//��ѶQQֱ�䣬����ͨ�ӿڣ��ɳ�ȫ����


	//��ֱͨ��
	int unicom_direct_recharge(const char* title);

	//��ͨ���зź�
	int Unicom_Sell_Phoneno(const char* title);
	int Unicom_Sell_Phoneno_Hdl1(const char* title);
	int Unicom_Sell_Phoneno_Hdl2(const char* title);

	//�����Ƿ���ݺ���
	int is_gd_phone(char* phone);

	//��;����Ʊ
	int coachTicketTradeGetInfo();
	int coachTicketTradeQuery();
	int coachTicketTradePay();

	//���ô�ӡ�µ��ŷ�Ʊ���Գ���Ʊֽ��
	int setPrintPaper();

	//e����ѯ
	int ecard_query(const char* type);

	//�Ƽ�ҵ��
	int getRecommandMenu(int type);
	int getYCTRecommandMenu();

	int YctSuccTxnEnq(const char *title);
	int printYctBill(const char *title);

	//���ͨ�׿�/�ײ͹���ҵ��
	int Yct_WCardPack_purchaseHdl(const char *title);
	//���ͨ�׿��ɹ�ҵ��
	int Yct_Whitecard_purchaseHdl(const char *title);
	//���ͨ�ײͲɹ�ҵ��
	int Yct_Package_purchaseHdl(const char *title);
	int Yct_ICReader_purchaseHdl(const char *title);

	//��֧���˻���ֵ
	int bestPay_rechargeHdl(const char *title,int iSelect);
    
	/*------------н��н���-------------*/
	//н��н���Ȩ������
	int xjx_checkauthorcfg_hdl(const char *title);
	//н��н�����������
	int xjx_LLcheckcfg_hdl(const char *title);
     //н��н�����������
	int xjx_LLLcheckcfg_hdl(const char *title);
	//������˽��
	int xjx_setcheckamount_hdl(int ilevel, const char *title);
	//����Ա��(����¼��Ա�����Ա���������Ա)
	int xjx_setstaff_hdl(int iSelect,int ilevel);
	//���û����˺�
	//flag: 0 - ���û����˺ţ�1 - ���û����˺� 
	int xjx_accountcfg_hdl(int flag,const char *title);
	//������ò�ѯ
	//iType: 0 - ������˲�ѯ��1 - ������˲�ѯ��
	int xjx_checkconfig_InqHdl(int iType,char *title);
	//�����˺����ò�ѯ
	//iType: 0 - �����˺Ų�ѯ��1 - �����˺Ų�ѯ��
	int xjx_accountconfig_InqHdl(int iType,char *title);
	//�޸�����
	//iuserType: 0 - ����Ա��1 - ¼��Ա��2 - ���Ա��3 - �������Ա
	int xjx_updatepwd_Hdl(int iuserType,char *title);

	//н��н�ʽ𻮲�
	int xjx_funds_transferhdl(const char *title);
	//���ײ�ѯ
	//flag: 0 - �ɹ����ף�1 - ���н���
	int xjx_txn_InqHdl(int flag,const char *title);
	//���н��н�ʽ𻮲�
	int xjx_txn_checkHdl(const char *title);
	//н��н����ʽ��˲�ѯ
	int xjx_pymreceived_InqHdl(const char *title);
	//��ȡ����״̬˵��
	//���������char *status - ����״̬
	//���������char *status_info - ����״̬˵��
	int xjx_get_statusinfo(char *status,char *status_info);
	/*-----------------------------------*/

	/*���ͨ��ֵ����*/
	int Yct_charge_cancel(const char *title);
	/*���ͨ����չӦ��*/
	int Yct_Card_Extend(const char* title);
	int YctradeHdl(CGlobal gbl, char *title, int payType, char *pki_card, char *signin_serial);
	int YCT_AckTrade(CGlobal gbl, YCT_DATA yct_data,int status );
	int YCT_RevTrade(CGlobal gbl, YCT_DATA yct_data);
	int  YCT_AckRevTrade( YCT_DATA yct_data,int status);
	int  YCT_PayPe(YCT_DATA yct_data, BYTE* party, BYTE* rspcode,char *stan,char *pos_serial);
	//ȫ��ˮ��úҵ��
	int sdm_showAreas(const char* title, int type,CGlobal gbl); //��ʾ��ֵ����
	int showSMD_cityList(char *title); //����ʡ�ݱ�Ž��в�ѯ�ɷѵ���
	int sdm_district_service(const char *title ); //ˮ��ú���в�ѯ
	int sdm_service(const char * title);
	int sdm_trade(char *title); //ˮ��ú��ѯ�ӿ�
	int area_query_service(CGlobal gbl,string &datas,char *WEBSVRNAME,char *WEBSVRCODE,string &strRsp,string &errStr);
	int sdm_query_service(char *WEBSVRNAME,char *WEBSVRCODE,char *SELECTTYPE,char *sdm_SYSTEMNO,int *billNum,int *BILLSTAT,string &strerr);
	int sdm_pay_service(char *WEBSVRNAME,char *WEBSVRCODE,char *sdm_SYSTEMNO,char *sdm_txnamount,int CASHTYPE,char *feeCircle,int PAYTYPE,char *OPERUSER,char *OPERPASSWORD,string &errString,string &errcode);
	//int  show_areaDatas(vector<BUSDATAS> &datasVector,CGlobal gbl);
	int  show_sdmBills(CGlobal gbl); //��ʾˮ��ú�˵���Ϣ
	int  sdm_pay(char *title,char *sdm_txnAmount,char *BILLMONTH,int BILLSTAT,string &errString);
	int  carTax_query_service(CGlobal gbl,char *WEBSVRNAME,char *WEBSVRCODE,int *billNum,char *sdm_SYSTEMNO,string &strerr);
	int  carTax_pay_service(char *WEBSVRNAME,char *WEBSVRCODE,char *SELECTTYPE,char *operUser,char *password,char *sdm_SYSTEMNO,string &strerr);
	int  showSDM_Bills(const char *type,int BILLSTAT);
	int  sdm_pay_handle(char *type);
	int sdm_query_handle(char *type);
	int sdm_handle(char *type);
    int sdm_qury_input(char * title);
	int g3DisChargeCard_pay_service(CGlobal gbl,char *WEBSVRNAME,char *WEBSVRCODE,char *RECHARGETYPE,char *isVerify,char *operUser,char *password,char *SYSTEMNO,string &rspcode,string &strerr);
	int phone_pay_service(char *WEBSVRNAME,char *WEBSVRCODE,char *RECHARGETYPE,char *operUser,char *password,char *SYSTEMNO,string &rspcode,string &strerr);
	int phone_pay_serviceGDYD(char *WEBSVRNAME,char *WEBSVRCODE,char *RECHARGETYPE,char *operUser,char *password,char *SYSTEMNO,CGlobal& gbl,string &rspcode,string &strerr);
	int g3_trade(char *title);
	int pay(const char *title,char *errMsg);
	int accountCheck_service(char *WEBSVRNAME,char *WEBSVRCODE,char *Verify,string &strerr);
	int g3_PhoneCheck(char *errMsg);
	//��ݸͨ��
	int dgtCardSale(char *type);
	int dgtBusTrade( char *type,int chargeType);
	int dgtSignIn(); //ǩ������
	int dgtSignOut(); //ǩ�˽���
	int dgtCardInqHdl(PDGTCard pdgtCard,int flag) ; //��ȡ��Ƭ������Ϣ
	int showDGTChargeTrade();
	int showDGTALLTrade();
	int dgtCardChargeRollBackService(char *WEBSVRNAME,char *WEBSVRCODE,char *TRANSSEQ, string &strerr);
	int dgtCardTransfer(char *type);
	int dgtM1CardInqHdl(PDGTM1Card pdgtM1Card,int type); //��ȡ��Ƭ������Ϣ
	int dgtOrderVerifyService(char *WEBSVRNAME,char *WEBSVRCODE,int type,char *BUSCODE ,CHAR *BUS_CONTERNT,string &strerr);
	int dgtOrderProduceService(char *WEBSVRNAME,char *WEBSVRCODE,char *TRANSSEQ,string &strerr);
	int checkdgtUnsubmit(int tradeFlag);
	int qq_guanglu_pay_service(char *WEBSVRNAME,char *WEBSVRCODE,char *operUser,char *password,char *SYSTEMNO,string &rspcode,string &strerr);
	int GLQQTrade(const char *type, int select=0,int input_flag=0);
	int bankInfoQry();
	int getBankInfoService(char * WEBSVRNAME,char *WEBSVRCODE,char *staff_code,char *CUSTCODE);
	int getCardStatusResult(); //��ݸ��������״̬��ѯ
private:
	//С��ͨ2�ڲ�ѯ����
	int xltInqHdl(const char *title);
	int xltInqMsg(const char *title);

	//С��ͨԤ����100��300
	int YH_xlttelInqHdl(const char *type);
	int YH_xlttelInqMsg(const char *type);
	int YH_xlttelPayHdl(const char *type);	
	
	//���Ž��״���
	int telInqHdl(const char *type, int inputTel, int &first3mon);	//���Ž��� type:����; oldTel:0-������绰���� 1-ʹ���ϴβ�ѯ���׵ĵ绰����; first3mon:0-������������Ƿ�ѵ�ǰ3���µĽ��� 1-����������Ƿ�ѵ�ǰ3���µĽ���
	int telInqMsg(const char *type, int &first3mon);// first3mon:0-������������Ƿ�ѵ�ǰ3���µĽ��� 1-����������Ƿ�ѵ�ǰ3���µĽ���
	int telPayHdl(const char *type, int bizIdx=0, int first3mon=0);//  first3mon:0-������������Ƿ�ѵ�ǰ3���µĽ��� 1-����������Ƿ�ѵ�ǰ3���µĽ���
	int telPayHdls(const char *type, int first3mon);	//���Ŷ�ҵ�񽻷�(Ԥ�ɷ�)  first3mon:0-������������Ƿ�ѵ�ǰ3���µĽ��� 1-����������Ƿ�ѵ�ǰ3���µĽ���

	//�ƶ����״���
	int mblInqHdl();
	int mblInqMsg();
	int mblPayHdl();

	//��ͨ���״���
	int uniInqHdl();
	int uniInqMsg();
	int uniPayHdl();

	//ȫʡ�ۺ��շѴ���
	int foshanInqHdl(const char *title, const char *city_no);
	int foshanInqMsg();
	int foshanPayHdl(const char *title);
	int foshanRdField63();
	void foshanCkCode(int code, char *type);
	
	//���Ӵ����ӡ��ϸ��ȡ
	int DZDBRdField63();
	//�̻����״���
	int merInqHdl();
	int merInqMsg();
	int merPayHdl();

	//����˰����
	int taxInqHdl(char *type, int input_flag=0);
	int taxInqMsg(char *type);
	int taxPayHdl(char *type);
	
	//3g��������ֵ
	int AutoSelectG3TC(const TSDLLPosInfo dposInfo,char *money,char *flow);

	//��ѶQQ����
	int QQInqHdl(const char *type, int input_flag=0);
	int QQInqMsg();
	int QQPayHdl(const char *type, int select=0,int input_flag=0);

	//����˻���ֵ
	int RaffleInqHdl(const char *type);
	int RafflePayHdl(const char *type, int select=0);

	//���ߵ��Ӵ���
	int CATVInqHdl(const char *type, int flag = 0);
	int CATVInqMsg1();
	int CATVInqMsg2(const char *type);
	int CATVPayHdl(const char *type);
	int CATVMenu(int flag);

	//����ҵ����
	int trafficInqHdl(const char *type, char *enqFIELD48, char *enqFIELD63, char *enqStan,int flag=0);
	int trafficInqMsg(const char *type, char *enqFIELD39, char *enqFIELD63, char *gbl_amount);
	int trafficPayHdl(const char *type, char *enqFIELD48, char *enqFIELD63, char *enqStan);

	//�̻����ײ�ѯ
	int merInqTrade_inq(int inqFlag);
	int merInqTrade_msg(int inqFlag);

	int customer_sign_Hdl();

	int posReturnFail(int iRet,char *acErrMsgBuf);	//POSʧ�ܷ���

	int xltMulCard(const char *type, int flag = 1);//С��ͨ�࿨����
	int packHdl(int flag);	//��������
	int selectAmount(int flag = 0, int select=0);//���׹�������Ҫѡ��Ľ����صĲ˵�
	int selectDGAmount(int flag = 0, int select=0);//���׹�������Ҫѡ��Ľ����صĲ˵�
	int selectSubcode();//���׹�������Ҫѡ�����ҵ�������صĲ˵�
	int telPayExtend(const char *type);//���Ž��Ѻ󣬸���ѯ���Ƿ���Ԥ����Ͱ�����Ȩ
	int ysqHdl();/*Ԥ��Ȩ*/
    int ysqconfirm();
	int ysqInqHdl(int type);

	int banInqMoneyHdl(const char *title, char *amount_out=NULL);	//���п�����ѯ(����)
	
	int get_key_num(char *bank_code);	//ȡ����
	
	int getCardPwd(const char *type);
	int getCard_1(const char *type, int card_flag = 1, const char *title="", int minLen=12, int maxLen=12);	//һ�βɼ����ź����루�����ܣ�

	int cardno_fee(char bank[6+1], char *fee_type, char *acc_city_no, int pos_type);//����ҵ���֧�ֹ�ϵ
	int get_area_biz(int biz_flag, char *area, char *accarea, char *biz_type);
	int get_favorable_menu(char *area,char *biz_type,int menu[]);
	
	int isTrack2(char *card_info, int &pos);
	int isTrack3(char *card_info, int &pos);

	void getFeeType(const char *feeType, char *name);
	int  getRealPwd(char *card, char *inPwd, char *outPwd);

	//��ݸݸ��ͨ������ֵ
	int busChargeZero(const char *feeType,const char *type); //0Ԫ��ֵ,�������ڶ�ݸ�����廪ͬ���������豸 
	int dgtBusInqHdl(const char *type,int chargeTye);

	int dgtEffectiveCheck(int type,char * errMsg); //��Ƭ��Ч�Լ��
	int dgtSignInService(char *WEBSVRNAME,char *WEBSVRCODE,PDGTSignInfo pdgtSignInfo,string &strerr);
	int dgtSignOutService(char *WEBSVRNAME,char *WEBSVRCODE,string &strerr);
	int cardOperate_service(char *WEBSVRNAME,char *WEBSVRCODE,char *PASSFLAG, char *CARDOPRTYPE,char *TRANSTYPE,char *APDU,char *TRANSSEQ,string &rsp_code,string &strerr);
	int cardSaleService(char *WEBSVRNAME,char *WEBSVRCODE,char *OPERATIONTYPE,char * TRANSTYPE,char  *TRANSSEQ,char *SALEMODE,string &strerr);
	int dgtCardChargeService(char *WEBSVRNAME,char *WEBSVRCODE,char *OPERATIONTYPE,char *TRANSTYPE,char *TRANSSEQ, string &strerr);
	int dgtCardCharge(char *title);
	int cardSale(CGlobal gbl,char *tranMessage,char *tranType);
	int dgtRollBack();
	int getGgtResultByKeep(char *WEBSVRNAME,char *WEBSVRCODE,char *KEEPCODE,string& strPaystat,string &strerr);//ͨ��keepֵ��ȡ����


	//��ݸ����(��ͨ)
	int busInqHdl(const char *type,int chargeType);	//��ѯ
	int busPayHdl(const char *type);	//����
	int busRevHdl(const char *type);	//����
	int busConHdl(const char *type, int flag=0);	//ȷ��,flag:0�ɹ���1ʧ��
	int busbusConHdl(const char *type, int flag=0);	//ȷ��,flag:0�ɹ���1ʧ��
	int  read_IcCardInfo(const char   *type, char * term_serial,int deviceType,char *cardMny); //��ȡIC����Ϣ
	int cardPayTelOne(const char *type, char *amount, char *fee_type, char *sell_card_type, char *cardpwd);
	//dest_list: 0:�̻� 1:С��ͨ 2:�ֻ� 3:ADSL;latn_id_list:���� ����020;charge_num_list:�绰����
	int cardPayTel(const char *type, char *latn_id_list, char *dest_list, char *charge_num_list, char *amount, char *fee_type);
	int payModeSelect(const char *type, char *biz_type, char *amount, char *city_no, char *phone_no);
	int MyCoolPayEasy_cardPayTel(const char *type, char *latn_id_list, char *dest_list, char *charge_num_list, char *amount, char *fee_type);
	
	void getHeadPage(int tgroup, int term_level, vector<Epos_new_message> &pNew_message, int title_flag, char *pos_no, char *mny, char *yj_buff, char *headPaye);

	int m_com;	//ʹ�ô���
	int m_comType;	//��������

};

#endif // !defined(AFX_TRADE_H__C7E243D1_16DD_4096_B889_C55C2B319F08__INCLUDED_)
