// Global.cpp: implementation of the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "Global.h"
#include "Def.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CGlobal::CGlobal()
{
	bbzero(bank);
	bbzero(card_no);		//����
	bbzero(amount);	//���ѽ��
//	bbzero(yymmdd);		//8λ������
//	bbzero(hhmmss);		//6λʱ����
	bbzero(stan_no);		//ϵͳ�ο���
	bbzero(pos_no);		//�ն˺�
//	bbzero(merchant_code);	//�̻���
	bbzero(fee_type);     //ҵ�����
	bbzero(order_no);	//�̻�������
	bbzero(phone_no);	//�绰����
	
	bbzero(pay_no);		//�����ʺ�
	bbzero(pos_serial);	//�ն���ˮ
	bbzero(user_name);	//�û���
	bbzero(password);		//����
	bbzero(new_password);	//�޸������������
	bbzero(ago_fee_buffer);	//����Ƿ��
	bbzero(pre_fee_buffer);	//Ԥ����	
	bbzero(real_fee_buffer);	//ʵʱ����
	bbzero(sub_code);		//��ҵ�����
	bbzero(track2);		//2�ŵ���Ϣ
	bbzero(track3);		//3�ŵ���Ϣ
	
	bbzero(xlt_type);	//С��ͨ���������ǰ�������
	bbzero(xlt_card);	//С��ͨ���ѿ���Ϣ
	
	bbzero(random_num);	//2��des�������ܵ������
//	bbzero(mac_random_num);	//��mac�õ������
//	bbzero(rcv_mac);	//pos���͵�16λmac
//	bbzero(cal_mac);	//�����16λmac
	
	bbzero(tax_serial); //2006-10-19����˰��ˮ
	bbzero(car_no);      //���ƺ�
	bbzero(car_type);    //������
	bbzero(pay_tax);	//Ӧ��˰��
	bbzero(late_tax);	//���ɽ�
	bbzero(mul_tax);	//����
	
	bbzero(QQ_amount); //Q�Ҹ�����QQ��Ա���µĴ���
	bbzero(QQrate);    //Q�ұ�ֵ(2λ����ΪQ�ҳ�ֵ"85"��ʾ��Q��:�����=1:0.85 �� ��ΪQQ��Ա���� "10"��ʾ����Ϊ10Ԫ)
	
	bbzero(sell_card_type);		//�ۿ�����
	bbzero(card_num);	//�ۿ�������
	bbzero(card_pwd);	//�ۿ�������
	bbzero(card_serial);	//�ۿ��������к�
	bbzero(card_amount);	//�ۿ����ۿ���ֵ
	bbzero(mer_ecard);
	bbzero(product_name);
	bbzero(product_num);
	bbzero(order_time);
	product_flag = 0;
	
	//���ߵ��ӣ�2006-11-20 by lihk
	bbzero(CATV_id_card); //���֤��
	bbzero(CATV_manual_num); //�û��ֲ��
	CATV_choice = 0;

	bbzero(uni_flag);		//��ͨԤ���ѱ�־
	bbzero(tmp_pre_amount);	//�ƶ���Ƿ�Ѷ���Ԥ����ʱ����ӡԤ�����ã�2006��10��25
	bbzero(call_tel); //�������

	bbzero(id_num); //֤����

	bbzero(xmcard_type);
	bbzero(xmcard_name);

	bbzero(overdraft_amount);

	bbzero(update_messag);
	bbzero(ICCID_no);

	mycoolpayeasy_return = 0;

	timeout=0;
	
	num = 0;		//����	
	phone_len = 0;		//�绰���볤��
	card_no_len = 0;	//���ų�
	pay_no_len = 0;	//�����ʺų���
	track2_len = 0;	//2�ŵ�����
	track3_len = 0;	//3�ŵ�����
	user_name_len = 0;	//�û�����
	
	phs_ticket = 0;	//�̻���־�����������Ƿ��ӡ�����׿��վ�
	get_card = MNG_ONE;	//ˢ���������̱�־��MNG_ONE:һ��ˢ���������������� MNG_MUL:ÿ�ν��Ѷ�ˢ�� MNG_ONE_BIND:һ��ˢ���������������ѣ����ð󶨣� MNG_MUL_BIND:ÿ�ν��Ѷ�ˢ�������ð󶨣�
	key_num = 0;	//�����ܳ׺�
	
	mbl_flag = 0;	//��־�ƶ��Ƿ�������Ƿ�ѵ�����½�Ԥ����

	menu_group = 1;	//�˵����
	tgroup = 1;		//�ն����
	group_type = 1;
	askType = 0;
	dayAmo = 0;
	DianXin_Turn = 0;
	Reversal_count = 0;
	term_level = 0;
	bbzero(welcome);

	memset(&detail, 0, sizeof(detail));

	memset(&posInfo, 0, sizeof(posInfo));	//�ն���Ϣ
	
	bbzero(ip);
	bbzero(city_no);
	bbzero(acc_city_no);

	input_times = 1;	//ֱ����ҵ��(��ͨ���ƶ�)����������

	print_paper = 0;	//Ĭ�϶̷�Ʊ

	commission_free = 0;

	bill_list_len = 0;
	pep_type = 0;
	memset(&bill_item_list, 0, sizeof(bill_item_list));
	memset(&bill_list, 0, sizeof(bill_list));
	memset(&yct_disp, 0, sizeof(yct_disp));
	bbzero(DEPOSIT);
	memset(&dgtSignInfo,0,sizeof(dgtSignInfo));
	memset(&dgtCard,0,sizeof(dgtCard));
	memset(&glQQPayInfo,0,sizeof(glQQPayInfo));

	bbzero(Centerseqno);
	bbzero(ReturnAmt); //д���¿����
	bbzero(ReturnDeposit) ; //���û�Ѻ��
	bbzero(MonthFeeFlag) ;; //�·���ȡ��־
	bbzero(FeeAmtFlag); //��������ȡ��־
	bbzero(MonthFeeAmt); //���ɱ����·�
	bbzero(FeeAmt) ; //������
	bbzero(bus_showId); //�������ſ����
	random_value=0; //��ݸ���������

}

CGlobal::~CGlobal()
{

}



//ע�ⲻҪ����ҵ���ڣ����г�ʼ����ҵ�����
void CGlobal::init_global()
{
	bbzero(amount);
	bbzero(order_no);
	bbzero(phone_no);
	bbzero(pay_no);
	bbzero(pos_serial);
	bbzero(ago_fee_buffer);	//����Ƿ��
	bbzero(pre_fee_buffer);	//Ԥ����	
	bbzero(real_fee_buffer);	//ʵʱ����
	bbzero(sub_code);
	bbzero(stan_no);
	bbzero(ICCID_no);
	bbzero(user_name);
//	bbzero(yymmdd);
//	bbzero(hhmmss);
	bbzero(xlt_type);
	bbzero(xlt_card);
	bbzero(tax_serial); //add by lihk 2006-10-19����˰��ˮ
	bbzero(car_no);      //���ƺ�
	bbzero(car_type);    //������
	bbzero(pay_tax);	//Ӧ��˰��
	bbzero(late_tax);	//���ɽ�
	bbzero(mul_tax);	//����
	billNum=0; //�˵���

	bbzero(new_password);
	bbzero(uni_flag);
	bbzero(tmp_pre_amount);
//	bbzero(rcv_mac);
//	bbzero(cal_mac);
//	bbzero(mac_random_num);
	bbzero(sell_card_type);
	bbzero(QQ_amount);
	bbzero(QQrate);
	bbzero(card_num);
	bbzero(card_pwd);
	bbzero(card_serial);
	bbzero(card_amount);
	bbzero(xmcard_type);
	bbzero(xmcard_name);
	bbzero(product_name);
	bbzero(product_num);
	bbzero(overdraft_amount);
	bbzero(update_messag);
	product_flag = 0;

	bbzero(CATV_id_card); //���֤��
	bbzero(CATV_manual_num); //�û��ֲ��
	CATV_choice = 0;

	mycoolpayeasy_return = 0;

	phone_len = 0;
	pay_no_len = 0;
	user_name_len = 0;
	mbl_flag = 0;
	timeout=0;

	memset(&detail,0,sizeof(detail) );	
	memset(&Manual_Detail,0,sizeof(Manual_Detail) );	//�����ֲ�Ų�ѯ������ϸ
	memset(&dgtSignInfo,0,sizeof(dgtSignInfo)); //��ݸͨǩ����Ϣ��
	memset(&dgtCard,0,sizeof(dgtCard));

	bbzero(pki_card);
	bbzero(signin_serial);

	bill_list_len = 0;
	pep_type = 0;
	memset(&bill_item_list, 0, sizeof(bill_item_list));
	memset(&bill_list, 0, sizeof(bill_list));
	//ˮ��úȫ�ֱ���
	bbzero(AREACODE); 
	bbzero(DX_CODE);
	bbzero(AREA_NAME);
	bbzero(PARENT_CODE);
	bbzero(PARENT_NAME);
	AREA_FLAG=1;
	bbzero(SHORTNAME);
	bbzero(sdm_PAYMENTCODE);
	bbzero(sdm_PAYMENTNAME);
	bbzero(sdm_BUSCODE);
	bbzero(sdm_BUSNAME);
	bbzero(sdm_CUSTOMERNAME);
	bbzero(sdm_totalFee);
	bbzero(sdm_SELECTVALUE);
	bbzero(query_amy);
	bbzero(sdm_SYSTEMNO);
	SDM_BILLNUM=0;
	bbzero(SDM_BILLNO);
	bbzero(SDM_BILLMONTH);
	bbzero(sdm_TRANSSEQ ); 
	bbzero(sdm_ORDERSEQ);
	bbzero(deleyAMount);
	bbzero(sdm_TXNAMOUNT);
	memset(&carTaxBills,0,sizeof(carTaxBills));
	memset(&sdmBills,0,sizeof(sdmBills));
	bbzero(TRANSSEQ);


}