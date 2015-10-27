// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__635C685F_4310_48D9_9A01_8ECD8498001B__INCLUDED_)
#define AFX_GLOBAL_H__635C685F_4310_48D9_9A01_8ECD8498001B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TSCTSI.h"
#include "Def.h"

class CGlobal  
{
public:
	CGlobal();
	virtual ~CGlobal();
	void init_global();
	
public:

	char card_no[31];	//����
	char amount[12+1];	//���ѽ�� ���Ƿ��
	char origAmount[12+1]; //Ӧ�ս��
	char overdraft_amount[12+1]; //Ƿ�ѽ��
	char stan_no[13];	//ϵͳ�ο���
	char TRANSSEQ[20+1];	//������
	char APTRANSSEQ[15+1]; // ����������
	char pos_no[9];		//�ն˺�
	char fee_type[5];   //ҵ�����
	char fee_type2[5];   //ҵ�����2
	char order_no[ORDER_LEN+1];	//�̻�������
	char order_time[14+1]; //��������ʱ��
	char phone_no[50];	//�绰����
	char phone_city[50]; //����ι�����
	char phone_province[50];//����ι���ʡ��
	int	 phone_owner; //������Ӫ�̣�1�����ţ�2 �ƶ� 3 ��ͨ
	
	char curworkid[8+1]; //��ǰ����
	char telphone[11+1]; //���л���ʱ������ֻ�����

	char custom_no[8+1]; //�̻�ǩԼ���̻��ն˺�
	char custom_pwd[6+1]; //�̻�ǩԼ���̻�����
	char custom_id_card[20+1]; //�̻�ǩԼ���̻����֤��

	char pay_no[30];	//�����ʺ�
	char pos_serial[7];	//�ն���ˮ
	char user_name[61];	//�û���
	char password[9];		//����
	char new_password[8+1];	//�޸������������
	char ago_fee_buffer[13];	//����Ƿ��
	char pre_fee_buffer[13];	//Ԥ����	
	char real_fee_buffer[13];	//ʵʱ����
	char sub_code[3];		//��ҵ�����
	char track2[50];		//2�ŵ���Ϣ
	char track3[110];		//3�ŵ���Ϣ
	
	char xlt_type[2+1];	//С��ͨ���������ǰ�������
	char xlt_card[512];	//С��ͨ���ѿ���Ϣ
	
	char random_num[8+1];	//2��des�������ܵ������
	
	char tax_serial[11+1]; //2006-10-19����˰��ˮ
	char car_no[8+1];      //���ƺ�
	char car_type[2+1];    //������
	char pay_tax[12+1];	//Ӧ��˰��
	char late_tax[12+1];	//���ɽ�
	char mul_tax[12+1];	//����
	int billNum; //�˵���
	CarTaxBills_Struct carTaxBills;
	
	char QQ_amount[4+1]; //Q�Ҹ�����QQ��Ա���µĴ���
	char QQrate[3+1];    //Q�ұ�ֵ(2λ����ΪQ�ҳ�ֵ"85"��ʾ��Q��:�����=1:0.85 �� ��ΪQQ��Ա���� "10"��ʾ����Ϊ10Ԫ)
	
	int card_flag;//��ݸ�����ͱ�־
	char sell_card_type[4+1];		//�ۿ�����
	char card_num[20+1];	//�ۿ�������
	char card_pwd[30+1];	//�ۿ�������
	char card_serial[20+1];	//�ۿ��������к�
	char card_amount[12+1];	//�ۿ����ۿ���ֵ
	
	//���ߵ��ӣ�2006-11-20 by lihk
	char CATV_id_card[18+1]; //���֤��
	char CATV_manual_num[11+1]; //�û��ֲ��
	char CATV_IC_NUM[20+1];//IC��
	char CATV_MAC_NUM[12+1];//MAC��ַ
	int  CATV_choice;
	CATV_Struct1 Manual_Detail;	//�ֲ�Ų�ѯ������ϸ
	CATV_Struct2 Id_Detail;	//���֤��ѯ������ϸ
	char catv_paypassword[6+1];//֧������
	char uni_flag[2];		//��ͨԤ���ѱ�־
	char tmp_pre_amount[13];	//�ƶ���Ƿ�Ѷ���Ԥ����ʱ����ӡԤ�����ã�2006��10��25
	char call_tel[16+1]; //�������
	char mer_ecard[20]; //�̻��󶨵�e��

	char id_num[30];  //֤������, 20070807 by lihk
	int  num;  //����, 20070807 by lihk
	
	int phone_len;		//�绰���볤��
	int card_no_len;	//���ų�
	int pay_no_len;	//�����ʺų���
	int track2_len;	//2�ŵ�����
	int track3_len;	//3�ŵ�����
	int user_name_len;	//�û�����
	
	int phs_ticket;	//�̻���־�����������Ƿ��ӡ�����׿��վ�
	int get_card;	//ˢ���������̱�־��0:һ��ˢ���������������� 1:ÿ�ν��Ѷ�ˢ�� By:KF 06.11.09
	int key_num;	//�����ܳ׺�
	char bank[6+1];	//���д���
	
	int mbl_flag;	//��־�ƶ��Ƿ�������Ƿ�ѵ�����½�Ԥ����
	int menu_group;	//�˵����
	int tgroup;		//�ն����
	int group_type; //�ն��������:0:�̻��� 1:���̻���
	int askType;	//��������
	char welcome[500+1]; //��ӭ��
	char update_messag[300+1];//������ʾ���

	char total_fee_type[10][5+1];	//�ۺ��շ�ҵ�����
	char total_amount[10][12+1];	//�ۺ��շѽ��

	char adsl_test_type[30];
	char adsl_custom_type[30];
	char adsl_by_type[30];
	char adsl_modem_type[30];

	char xmcard_type[12+1];  //�ǿ����˵㿨����
	char xmcard_name[30];    //�ǿ����˵㿨����

	char product_name[30];   //�ն����۲�Ʒ����
	char product_num[4+1];   //�ն����۲�Ʒ����
	char product_perprice[12+1];//�ն����۵���
	int product_flag;       //1:POS��2:��ӡֽ3.288���4.CDMAѡ��
	int cdma_num;

	int dayAmo;

	char pay_easy_zh[80+1];

	char city_no[4+1];		//�ն����ڵ����Ĺ̻�����
	char acc_city_no[4+1];	//�ն����������Ĺ̻�����

	PrnDetail detail;
	TSPosInfo posInfo;		//�ն���Ϣ
	TSDLLPosInfo dPosInfo;//DLL����Ҫ���ն���Ϣ

	char ip[16+1];
	char server_ip[16+1];
	//��ݸ��������
	char bus_card[8+1];//��������Ψһ��(BCD)
	char bus_showId[10+1]; //���������
	char bus_selltype[1+1];//�������ۿ�����
	char bus_serial[6+1];//������ˮ��ʶ
	char bus_card_serial[1+1];//������ˮ��ʶ
	char bus_enq_stan[12+1];//�������Ľ�����ˮ
	char bus_conf[4+1];//����������֤��
	char bus_old_amo[8+1];//��������ֵǰ���
	char bus_pay_amo[8+1];//��������ֵ���
	char bus_new_amo[8+1];//��������ֵ����
	char bus_card_info[48+1];//���������ϴ���Ϣ
	char bus_date[8+1];//������������
	char bus_time[6+1];//��������ʱ��
	char dkq_code[4+1];//���������߱��
	char dkq_PSAM[16+1];//��������PSAM����
	char bus_yj_amo[8+1];//Ѻ��
	char bus_card_type[2+1];//Ӧ��������
	char bus_failcode[2+1];//�������
	int HID_TYPE;		//��ݸ�廪ͬ������������ Ĭ��Ϊ0���رա�
	int bus_trade_flag; //��ݸ�۷ѱ�ǣ�1Ϊ�۷��У�0Ϊ������ɡ�
	//��ݸͨǩ����Ϣ
	DGTSignInfo dgtSignInfo;
	DGTCard dgtCard;
	DGTM1Card dgtM1Card; //��ݸͨ�ɿ���Ϣ
	GLQQPayInfo glQQPayInfo;//��»qq������Ϣ
	//DGTDeviceInfo dgtDeviceInfo; //�豸��Ϣ
	char busi_id[12+1]; //��ҵ�˻�id
	char posKey[32+1]; //�ն�˽Կ
	char posId[12+1]; //�����豸����
	char dgt_LocalSequence[11+1]; //��ݸͨ������ˮ��
	char dgt_posSerial[9+1];//��ݸͨ�ն���ˮ��
	char Centerseqno[18+1]; //��ݸͨ������ˮ
	int  random_value; //��ݸ���������

	//��������Ծɻ�����ʹ��
	
	char ReturnAmt[10+1]; //д���¿����
	char ReturnDeposit[8+1]; //���û�Ѻ��
	char MonthFeeFlag[1+1]; //�·���ȡ��־
	char FeeAmtFlag[1+1]; //��������ȡ��־
	char MonthFeeAmt[8+1]; //���ɱ����·�
	char FeeAmt[8+1]; //������


	char DEPOSIT[8+1]; //Ѻ��
	char SYSTEMNO[15+1]; //ϵͳ�ο���

	char phone_amount[12+1];   //ֱ��󷵻ص��ֻ����

	int mycoolpayeasy_return;
	int title_flag;   //IPOS��������
	char adsl_xc_type[5+1];
	char adsl_xc_code[20+1];

	char adsl_number[99+1];
	char adsl_code[99+1];
	char adsl_name[99+1];
	char adsl_speed[99+1];
	char adsl_date_long[99+1];
	char adsl_money[99+1];
	char adsl_date_end[99+1];

	char pki_card[8+1];
	char signin_serial[16+1];
	int DianXin_Turn;		//����ֱ�����ۿ����ء�0��ֱ�䣻1������
	int Reversal_count;		//�̻����������޶�
	int term_level;//�̻�����
	int timeout;

	int menu_group_mbl_flag;//0�������� 1�����ƶ��� 2���ƶ���

	char car_id[6+1];//���ܺź�6λ(��Ϊ6λ 2012-01-16)
	char engine_no[6+1];//�������ź�6λ(��Ϊ6λ 2012-01-16)
	char traffic_code[250+1];//Υ�´���
	char traffic_area[2+1];//Υ�µ���
	char transfer_flag[1+1];//Ͷ�ݱ�־
	char paper_no[2+1];//�����
	char address[60+1];//�ռ��˵�ַ
	char traffic_mny[20];//����
	char transfer_mny[20];//�ʵݷ�
	char service_mny[20];//�����
	char total_mny[20];//�ܽ��
	char traffic_area_name[20];//Υ�µ�����
	char traffic_place[80+1]; //Υ�µص�
	char traffic_date[80];//Υ�µ�ʱ��


	char ICCID_no[20+1];

	int input_times;//ֱ������������

	int print_paper;//�״�Ʊֽ

	int bill_list_len;		//����ֱ����Ϣ
	int pep_type;
	Bill bill_list[MAX_BILL_NUMBER];
	Bill_Item bill_item_list[MAX_BILL_ITEM_NUMBER];

	//char DIY_URL[30];	//DIY��վ��ַ
	char CS_URL[30];	//�ͷ���ַ

	char yct_disp[1024+1];   //���ͨ��ֵ��Ϣ

	int commission_free;
	char asdl_user [60+1]; //��ɽASDL �û���
	int asdl_user_len ; //��ɽASDL ����

	char bestpayAccount[11+1]; //��֧���˻�
	int	bestpayAccount_len; //��֧���˻�����
	char bestpayPass [100];//��֧���˺�����
	int bestpayPass_len; //��֧���˻����볤��
	char bestpayTotalMny[13+1];//��֧���˻����
	int agreeFlag;//�ж��û��Ƿ��Ѿ�ȷ��

	/*----------------------н��н���------------------------*/
	char xjx_stan[12+1];//ϵͳ�ο���
	char xjx_seq[10+1];//н��н���׼�¼���
    char in_account[30+1];	//ת���˺�
	char out_account[30+1];   //ת���˺�
	char abstract[50+1];    //ժҪ
	char payer_info[50+1]; //�����Ϣ
	char payee_info[50+1];  //�տ��Ϣ
	char xjx_phone_no[50+1]; //�绰����
	char term_serial[6+1];  //�ն���ˮ��
	char xjxbank[6+1];//���д���
	char status_info[30+1];//����״̬����
	char txn_datetime[20+1];//��������/ʱ��
	char xjx_rspcode[2+1];//������
	/*--------------------------------------------------------*/

	/*****************���ͨ��ֵ����������ȫ�ֱ���***************************/
	int com;
	int comType;
	//�ն˼��
	char monitor_frequency[4+1];//���Ƶ��
	char next_monitortime[14+1];//�´η��������õ�ʱ��
	char announce[128+1];//������Ϣ
	char term_ver[32+1];//�ն�����汾��
	/************************************************************************/

	/*********************ˮ��úҵ��ȫ�ֱ���*********************************/
	char AREACODE[6+1];//ˮ��úҵ��ҵ���������
	char DX_CODE[4+1]; //���ŵ�������
	char AREA_NAME[60]; //��������
	char PARENT_CODE[6+1]; //���׵�������
	char PARENT_NAME[60+1];//���׵�������
	int  AREA_FLAG; //���������־ 1Ϊһ����2Ϊ2��
	char SHORTNAME[60];//�������

	char sdm_PAYMENTCODE[30+1]; //�շѵ�λ��� ����18λ
	char sdm_PAYMENTNAME[128+1]; //�շѵ�λ����
	char sdm_BUSCODE[3+1]; //ҵ�����
	char sdm_BUSNAME[64+1]; //ˮ��úҵ������
	char sdm_CUSTOMERNAME[64+1]; //ˮ��ú�ͻ�����
	char sdm_totalFee[12+1]; //ˮ��ú�ܽɷѽ��
	char sdm_SELECTVALUE[64+1]; //�û��Ż���������
	char query_amy[12+1]; //��ѯ���
	char sdm_SYSTEMNO[15+1]; //ϵͳ�ο���
	int	 SDM_BILLNUM;        //�˵���
	char SDM_BILLNO[200]; //ˮ��ú�˵���
	char SDM_BILLMONTH[100+1]; //������ʾ 
	char sdm_TRANSSEQ [15+1] ; //������ˮ��
	char sdm_ORDERSEQ[32+1]; //������
	char deleyAMount[12+1]; //���ɽ�
	char sdm_billAmount[12+1]; //�˵����
	char sdm_TXNAMOUNT[14+1] ; //���׽��14λ����Ϊ��λ
	int  BILLSTAT; //ˮ��ú�˵�����
	sdmBills_Struct sdmBills; //ˮ��ú�˵���Ϣ

	/*******************************3G������**************************************/
	char RECHARGEFLOW[12+1]; //��������
	char g3Phone[11+1];//��ֵ�ֻ���

	

};

#endif // !defined(AFX_GLOBAL_H__635C685F_4310_48D9_9A01_8ECD8498001B__INCLUDED_)
