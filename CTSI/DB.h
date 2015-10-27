// DB.h: interface for the CDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_H__E8F9BF40_A9EB_4BC4_994D_043F428A1CA5__INCLUDED_)
#define AFX_DB_H__E8F9BF40_A9EB_4BC4_994D_043F428A1CA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Def.h"
#include "ADOConn.h"
#include "TSCTSI.h"

class CPack;
class CGlobal;

class CDB  
{
public:
	CDB();
	virtual ~CDB();

	int static getTable();
	int static getEposTerm(char *pos_no, char *call_tel, int *tgroup, int *group_type, int *phs_ticket, int *get_card, char *mer_ecard, int *logonFlag, char *welcome ,char *workid, PTSDLLPosInfo pDllPosInfo, char *usbid, int *dayAmo, char *city_no, char *acc_city_no, int *title_flag, char *pki_card, char *signin_serial, int *DianXin_Turn, int *Reversal_count, int *term_level,char*update_message, int* input_times, int* print_paper, int* commission_free,int * hid_type,int *busFlag);

	int static getMenu(int tgroup, int askType, char *bank, int *menu_group, int *main_group, char * main_ver, char * menu_ver);
	int static setEposTxn(int status, CGlobal gbl, CPack pack, char *seq);
	int static insertEposTxn( CGlobal gbl,char *tradeStatus);
	int static setEposLogonStatus(int status, char *term_id);
	int static setSumDayAmo(int amount, char *term_id);
	int static getEcard(char *pos_no, char *card);  //lihk,2007-6-6
	int static getEcard(char *pos_no, char *card,char * businame);  //lihk,2007-6-6
	int static getPosno(char *card,char *pos_no);
	int static getCardPrint(char *stan, char *field63, char *fee_type, char *sell_card_type, char *card_amount, char *card_no);
	int static getRePrint(char *stan,char *term_id,char *card_no,int &iseny,char pMsg[10][2048], char *prmVer, int *num, int mbl_flag=0);//kf,20080418
	int static updateRePrint(char *stan,char *term_id,char *card_no,int &iseny);//kf,20080514
	int static isLock();
	int static waitForidlesse();

	int static setYctTxn(CGlobal gbl, CPack pack, char *process, char *pk_card, char *YCT_card, char *seq);  //lihk,2007-3-16
	int static setYctStat(int stat, char *seq);  //lihk,2007-3-16
	int static updateYct(char *PKLogCard, char *process, char *YCTLogCard, char *seq);  //kf,20080718
	int static setRePrint(char *stan,char *term_id,char *card_no,int iseny,char *pMsg,int tgroup);//kf,20080418
	int static getEcardTxn(char *pe_card_no, int flag, char *last_event_seq, char *event_seq, char *txn_type, char *txn_time, char *chg_amount);
	int static updateEcardTxn(char *event_seq);

	//�����ݿ��ж�ȡ���ͨ����������Ϣ
	int static getYctRev(char *seq, char *pos_no, char *pos_serial, char *card, char *amo, char *YCT_Card);
	int static getYctAck(char *stan, char *pos_no, char *pos_serial, char *intAmo, char *hexAmo, char *YCT_Card, char *stat, char *seq);
	int static setYctAck(char *seq, char *stat, char *hexAmo);  //����δȷ����Ϣ
	int static getYctStat(char *pos_no, char *yctStan, char *seq, char *stan);
	int static updateYctAck(char *seq, char *stat);  //����δȷ����Ϣ
	int static get_yctptxn(CGlobal &gbl,PYCT_DATA yctData,char * charge_amo);// �����Ƿ�������ͨ�ɳ�������
	int static getYctPIN(char *eposid,char *pin);
	int static getDBtime(char *dBTime);//��ȡ���ݿ�ʱ��
	//��ȡ�ն˶�Ӧ��ISAM������
	int static getYctPIN2(const char *eposid, unsigned char *pin);
	//������������1,��ʱͣ��
	int static setYCTPinErr(const char *eposid);

	/*int static get_pcardnum();*/
	//������ǩ��
	int static checkworkID(CGlobal gbl,char *workid);
	int static setworkID(CGlobal gbl,char *workid);

	//��ȡĳ��˵�
	int static getGroupMenus(int menu_group, Epos_menu_conf mymenus[], int *total);

	//�����ݿ���ȡ��������ϸ��Ϣ����ṹ��TSTxnLogs��
	int static getTxnLogs(char *term_id, char *date, int status, int inout, int mode, PTSTxnLogs ptxnLogs, int *itemNum, int reversal_count,int mbl_flag=0);
	
	//E���ʽ��ʲ�ѯ
	int static getEcardTrade(char* ecard_no,PTSTxnLogs ptxnLogs, int *itemNum);
	//���Զ���Э���-20130114
    int static getEcardTxnLogs(char* ecard_no, char* term_no,PTSTxnLogs ptxnLogs, int *itemNum);

	//�����ݿ���ȡ��ĳ�ն˵ĵ��ճɹ������ܶ�(�Է�Ϊ��λ)
	int static getDayAmo(char *term_id, int *dayAmo);
	//20130105����-yyx
	//int static pep_getDayAmo(char *term_id, int *dayAmo);

	int static updateDayAmo(char *term_id, int dayAmo);
	int static getEncryptType(int *encrypt_type,char *term_id);
	int static setEncryptType(int encrypt_type,int type,char *term_id);
	int static countSaleTxn(char *term_id, char *product_id,int *cnt);

	//��ѯ�Ƿ�ͬ��
	int static setAgreeFlag(int agreeFlag,char *term_id);
	int static getAgreeFlag(int *agreeFlag,char *term_id);

	//CDMAѡ��
	int static select_cdma_sj(CDMA_INFO* cdma_info,int t_flag);//t_flag:1.153���Ŀ� 2.153���ڿ�
	int static select_cdma_mh(CDMA_INFO* cdma_info,char *phone_num,int t_flag);//t_flag:1.153���Ŀ� 2.153���ڿ�

	//��ѯ׼�����������
	int static showchktxn(char *term_id, char *detail);
	//����
	int static chktxn(char *term_id, char *ecardamo, char *bankamo, char *chktermseq, char *chksysseq);

	//��ݸ����

	int static get_DGBus_stan(char *seq);
	int static update_HID_type(char *term_id,int HID_TYPE);
	int static set_DGBus_Txn (CGlobal gbl);
	int static update_DGBus_Txn(CGlobal gbl, char *status);
	int static get_DGBus_Txn_NotEnd(CGlobal &gbl, char *status);
	int static update_epos_term(char *term_id, char *status);
	int static revAppliChk(char *stan,char *input_amount, char *msg,CGlobal* gbl);
	int static revAppli(char *stan, char *msg,CGlobal gbl);
	int static isSonCard(char *cardno);
	//20121229-����ǰ�ýӿڻ�ȡ�Ƿ�Ϊ���ն˱�ʶ
	int static pep_isSonCard(char *termmid);

	int static isTransferParentTerm(char *term_id);
	int static isTransferSonCard(char *p_cardno,char *s_cardno);
    //�ʽ𻮲�
    int static pep_getSonCard(char* ecard_no , char* p_cardno , char * busiName,PTSTxnLogs ptxnlogs);
	int static getSonCard(char *p_cardno, PTSTxnLogs ptxnLogs);
	//�����ս����޶�
	int static set_dayAmt(char* ipos_no, char* dayamt);
	//��ȡϵͳ�ս����޶�̻������ս����޶�̻��ȼ�
	int static get_dayAmt(char* ipos_no, char* self_buf, int selfbuflen, char* sys_buf, int sysbuflen, int* shlevel);

	//����IPOS����
	int static update_title_flag(CGlobal gbl);
	int static getYctNotFinish(char *term_id, PYCT_DATA pyct_data);
	int static updateYctYPData(YCT_DATA yct_data);
	int static setYctYPData(YCT_DATA yct_data);
	int static setYctYPSignIn(const char *term_id, char *pki_card, char *signin_serial);
	//���ú�����10�������Ƿ��׹�
	int static chKChinaUnicom(char *bizs, char *phone_no, int amo);
	int static getConnStat();
	//��ͻ�
	int static getBatch_value(char *term_id,Batch_Value_Info *batch_info);
	int static getBatchList(char *batch_value,char *moneycount,PTSTxnLogs ptxnLogs,int itemcount);
	int static checkBatch(char *term_id);
	int static deleteBatch(char *batch_value);
	//������ʮ
	int static getIposPromo(char*term_id,char*start_date,char*end_date,char*donate_count,char*remain_count,char*all_amount,char*pro_id);
	int static setIposPromo(char*term_id,char*pro_id,int remain_count);

	//���зź�
	int static getUimLog(char*term_id,PTSTxnLogs ptxnLogs, int type);
	int static getUimLog_detail(char*stan,char*acount,char*total_fee,char*name,char*telphone,char*address);

	int static getTEL_STOCKS(int select_type,PTSTxnLogs ptxnLogs,char*city_no,char*phone_head=NULL,char*phone_part=NULL);
	int static getTEL_STOCKS_detail(char*phone,char*fee,char*type);
	
	int static getTel_part(PHONE_HEAD_NO*tel_part,char*city_no);

	int static getTermUim(PTSTxnLogs ptxnLogs, char* phone_no, char* term_id, int type);
	
	//��Ϸֱ��
	int static getGameList(PTSTxnLogs ptxnLogs, const char* sql);
	int static getServerList(PTSTxnLogs ptxnLogs, const char* game_name, const char* game_id, const char* autogame_id, const char* game_value, const char* amount);
	int static getMoneyList(PTSTxnLogs ptxnLogs, const char* sql);
	//ȫ�����Ż�
	int static getTrafficCode(PTSTxnLogs ptxnLogs);
	int static getAreaName(int code, char* area_name);

	//��ȡ��վ��
	int static getBusStop(PTSTxnLogs ptxnLogs, char* area, char* scont);

	//���³�ֵ��ҵ������������
	int static update_input_times(char *term_id, int input_times);

	int static update_commission_free(char *term_id, int commission_free);
	//���·�Ʊֽ����
	int static update_print_paper(char *term_id, int print_paper);
	//��ȡת�������ʵ�˺ż�����
	int static getRealNum(char *orig_num, char *real_num,char *username);
	int static getYctTxn(char *term_id, char *yct_date, PTSTxnLogs ptxnLogs);
	int static setYctBill(char *date, char *normal_amount, char *plus_amount);
    
	/************************************************************************/
	//�㷢н��н
	int static setXjxTxn(CGlobal gbl);
    //н��н���ײ�ѯ
	//flag: 0 - �ɹ����ף�1 - ���н��ף�2 - ʧ�ܽ���
	int static getXjxTxn(int flag,char *from_date,char *to_date, PTSTxnLogs ptxnLogs);
    int static getUnreviewedXjxTxn(PTSTxnLogs ptxnLogs);
	int static updateXjxTxnStatus(int iStatus,char* XjxSeq);
	int static getXjxUnreviewedRecordSet(int &conn_index,_RecordsetPtr &recordset);
    //�ý����������ȡ���׼�¼
	int static xjx_gettxnfromSeq(CGlobal *gbl);
	int static getXjxInaccount(char* XjxSeq,char *inaccount);
	int static getXjxTermsserial(char* XjxSeq,char *term_serial);
	int static updXjxStanRspcode(char* XjxSeq,char* rspcode,char *stan);
    int static closeXjxDBConn(int &index);
	
	//��ȡ����
	int static xjx_get_password(int iusertype, char *term_id, char *password);
	//�޸�����
	int static xjx_upd_password(int iusertype, char *new_pwd);
	//������˽��
	int static xjx_set_checkamount(int ilevel,char *amount);
	//����Ա������
	int static xjx_update_staff(int ilevel,char *staff_id);
	//����˺��Ƿ����
	int static xjx_check_account(char *account,char *account_name,char *account_bank);
	//¼���˺�
	int static xjx_add_account(char *account,char *acc_name,char *acc_bank);
	//���û����˺�
	//flag: 0 - �����˺ţ�1 - �����˺�
	int static xjx_set_account(int flag,char *account);
	//������ò�ѯ
	//flag: 0 - ������ˣ�1 - �������
	int static xjx_get_checkconfig(int flag, char *checker_id,char *amount,char *term_num);
	//�����˺����ò�ѯ
	//flag: 0 - �����˺ţ�1 - �����˺�
	int static xjx_get_accountconfig(int flag, char *acc_num,char *acc_name, char *acc_bank);
	//�����ն˺Ż�ȡ��˵ȼ�
	int static xjx_get_CheckLevel(char *term_id,char *level);
	//������˵ȼ���ȡ��Ҫ��˵Ľ��׼�¼
	int static xjx_get_UncheckTxnLogs(int level,PTSTxnLogs ptxnLogs);
	//���½���״̬�����Ա�ն˺ţ���ͨ������
	int static xjx_update_checkstatus(char *xjx_seq,char *level,char *status,char *term_id,char *reason);
    /************************************************************************/

	/*******���ͨ����*******/
	int static insert_YCTDistri_Record();

	//��ݸͨ���ݱ�
	//����ǩ������
	int static insert_dgtSignData( DGTSignInfo dgtSignInfo,char *term_id);
	int static update_dgtSignData( DGTSignInfo dgtSignInfo,char *term_id);
	int static checkSignStatus(char *term_id,PDGTSignInfo pdgtSignInfo);
	int static  update_signOutInfo(char *term_id,PDGTSignInfo pdgtSignInfo);
	int static insert_dgtTxnLog(CGlobal gbl,int trade_status);
	int static update_dgtSignInfo(char * term_id,DGTSignInfo dgtSignInfo);
	int static getLastPosSEQ(char  * term_id,char * lastPosSEQ);
	INT static  getDGTDeviceInfo(char *posid,char *posKey,char *posId,char *busi_id);
	int static getDGTcodeMessge(char * rsp_code,char *rsp_message);
	int static IsGetM1CardInDgtTransferLog(CGlobal *pgbl);
	int static updateM1CardStatus(char *M1CardId,char *cardStatus);
	int static checkM1CardStatus(char *m1CardId); 
	int static getdgtUnsubmit( CGlobal *pgbl,char * biz_type,int tradeFlag,int *tradeStatus);
	int static update_dgtTxnLog(char *sysyno,int status);
	int static update_dgtTxnLog_byKeep(CGlobal gbl);
	int static update_dgtTxnLogByStan(char *sysyno,int status);
	int static update_dgtTxnStatusByKeep(char *keep,int status);
	int static updateLastSigninStatus(char *term_id);
	int static updateTranOrderStatus(char * Centerseqno,char *orderStatus) ;

	/*************************���ͨ��ֵ������*******************************/
	//�ն˼�أ�ȡ�����һ����¼��״̬�������ڲ���stan��status��
	int static get_Last_Biz(unsigned char& status , unsigned char* stan);
	int static setYctThirdStatus(int stat, char *seq);
	int static updateYctThirdData(YCT_DATA yct_data);
	/*****************************************************************/
	int static updateYctRollback(YCT_DATA *yct_data,char* message);
	int static updateYctResultMsg(YCT_DATA *yct_data,char* message);
	void static getYct_posSerial(char * term_serial);
	int static updateYctuntxn(YCT_DATA *yct_data,char* stan);
	int static insetYctYPtxnDATA(YCT_DATA *yct_data,char *stan,int charge_type);
	int static getChargeType(char *stan,int charge_type,char *pe_stan,char *pos_serial);
	int static updateYctTxnStatus(char *stan,char *status);
	/*****************************************************************/
	int static getYct3SignInfo(char *term_id,PYCT_DATA pyct_data,char *cur_signTime);
	int static insertYct3SignInfo(char *term_id,PYCT_DATA pyct_data,char *cur_signTime);
	int static updateYct3SignInfo(char *term_id,PYCT_DATA pyct_data,char *cur_signTime);	
	int static checkYct3SignTimeInterval(char *term_id,char *interval);
	int static updateYct3SequenceNo(char *term_id,int sequence_no);
	long static getTimeInterval(const char *startTime,const char *endTime);
	//��ȡˮ��ú��ͨҵ�����
	int static getSDM_Area();
	int static isOpenSDMService(char city,SDM_AREA sdm_city);
	int static getSDMList(PTSTxnLogs ptxnLogs, const char* sql,int type,CGlobal gbl ); //��ȡˮ��ú�б�
	/************************************************************************/
	int static getBizName(const char *feeType,char * bizName); //��ȡҵ������
	//��ȡ���������
	int static phone_district_qry(char *phone_no,char *phone_province,char *phone_city,int *mobile_owner);
	int static is_gd_district(char *city_no,char *city_name); //��ѯ�Ƿ��ǹ㶫�����������0�����س�������
	int static insert_dgtTransferLog(CGlobal gbl,char *status);
	int static getCardTransferProcess(CGlobal *pgbl,char *tradeStatus,char *processInfo);
	int  static updateTranChargeProcess(CGlobal *pgbl,char *tradeStatus);
	int static getUnsubmitTransfer(CGlobal *pgbl,char *tradeStatus,char * orderStatus,char *unSubmitField);
	int static setdgtResubmit(char *centerseqno,char *unSubmitField);
	int static isCardInIgoreList(char *busCard);
	int static update_cardIgoreStatus (char * bus_card,char * status);

private:
	int static getBankCard();
	int static getBankInfo();
	int static getBankBusi();
	int static getEposGroup();
	int static getEposMenuGet();
	int static getEposMenuGroup();
	int static getEposMenu();
	int static getMenuConf();
//	int static getPcardConf();
    int static get_print_adv();
	int static getEposTips();
	int static getSellCardAmo();
	int static getEposMobile();
	int static getEposTurn();
	int static getEposNewMessage();
	//�����ݿ���ȡĳ����ĳҵ���ҵ�����
	int static getAreaBiz();
    int static getFavorableAirPay();
	//��ȡ�ֻ������������Ӧ��ϵ
	int static getMobileArea();


	//��ȡ������Ϣ
	int static getFailMessage();
	//��ȡ������Ϣ
	int static getPosAttr();

};

#endif // !defined(AFX_DB_H__E8F9BF40_A9EB_4BC4_994D_043F428A1CA5__INCLUDED_)
