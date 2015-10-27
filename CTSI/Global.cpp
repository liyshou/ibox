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
	bbzero(card_no);		//卡号
	bbzero(amount);	//交费金额
//	bbzero(yymmdd);		//8位年月日
//	bbzero(hhmmss);		//6位时分秒
	bbzero(stan_no);		//系统参考号
	bbzero(pos_no);		//终端号
//	bbzero(merchant_code);	//商户号
	bbzero(fee_type);     //业务代码
	bbzero(order_no);	//商户订单号
	bbzero(phone_no);	//电话号码
	
	bbzero(pay_no);		//付费帐号
	bbzero(pos_serial);	//终端流水
	bbzero(user_name);	//用户名
	bbzero(password);		//密码
	bbzero(new_password);	//修改密码的新密码
	bbzero(ago_fee_buffer);	//以往欠费
	bbzero(pre_fee_buffer);	//预付费	
	bbzero(real_fee_buffer);	//实时话费
	bbzero(sub_code);		//子业务代码
	bbzero(track2);		//2磁道信息
	bbzero(track3);		//3磁道信息
	
	bbzero(xlt_type);	//小灵通按卡余额交还是按金额交代码
	bbzero(xlt_card);	//小灵通交费卡信息
	
	bbzero(random_num);	//2次des产生加密的随机数
//	bbzero(mac_random_num);	//算mac用的随机数
//	bbzero(rcv_mac);	//pos上送的16位mac
//	bbzero(cal_mac);	//所算的16位mac
	
	bbzero(tax_serial); //2006-10-19，地税流水
	bbzero(car_no);      //车牌号
	bbzero(car_type);    //车类型
	bbzero(pay_tax);	//应交税额
	bbzero(late_tax);	//滞纳金
	bbzero(mul_tax);	//罚金
	
	bbzero(QQ_amount); //Q币个数或QQ会员包月的次数
	bbzero(QQrate);    //Q币比值(2位，若为Q币充值"85"表示：Q币:人民币=1:0.85 或 若为QQ会员包月 "10"表示月租为10元)
	
	bbzero(sell_card_type);		//售卡类型
	bbzero(card_num);	//售卡，卡号
	bbzero(card_pwd);	//售卡，密码
	bbzero(card_serial);	//售卡，卡序列号
	bbzero(card_amount);	//售卡，售卡面值
	bbzero(mer_ecard);
	bbzero(product_name);
	bbzero(product_num);
	bbzero(order_time);
	product_flag = 0;
	
	//有线电视，2006-11-20 by lihk
	bbzero(CATV_id_card); //身份证号
	bbzero(CATV_manual_num); //用户手册号
	CATV_choice = 0;

	bbzero(uni_flag);		//联通预付费标志
	bbzero(tmp_pre_amount);	//移动有欠费而交预付费时，打印预付费用，2006－10－25
	bbzero(call_tel); //来电号码

	bbzero(id_num); //证件号

	bbzero(xmcard_type);
	bbzero(xmcard_name);

	bbzero(overdraft_amount);

	bbzero(update_messag);
	bbzero(ICCID_no);

	mycoolpayeasy_return = 0;

	timeout=0;
	
	num = 0;		//数量	
	phone_len = 0;		//电话号码长度
	card_no_len = 0;	//卡号长
	pay_no_len = 0;	//付费帐号长度
	track2_len = 0;	//2磁道长度
	track3_len = 0;	//3磁道长度
	user_name_len = 0;	//用户名长
	
	phs_ticket = 0;	//商户标志，用于区分是否打印付费易卡收据
	get_card = MNG_ONE;	//刷卡交费流程标志，MNG_ONE:一次刷卡输密码连续交费 MNG_MUL:每次交费都刷卡 MNG_ONE_BIND:一次刷卡输密码连续交费（可用绑定） MNG_MUL_BIND:每次交费都刷卡（可用绑定）
	key_num = 0;	//加密密匙号
	
	mbl_flag = 0;	//标志移动是否是在有欠费的情况下交预付费

	menu_group = 1;	//菜单组号
	tgroup = 1;		//终端组号
	group_type = 1;
	askType = 0;
	dayAmo = 0;
	DianXin_Turn = 0;
	Reversal_count = 0;
	term_level = 0;
	bbzero(welcome);

	memset(&detail, 0, sizeof(detail));

	memset(&posInfo, 0, sizeof(posInfo));	//终端信息
	
	bbzero(ip);
	bbzero(city_no);
	bbzero(acc_city_no);

	input_times = 1;	//直充类业务(联通、移动)输入号码次数

	print_paper = 0;	//默认短发票

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
	bbzero(ReturnAmt); //写入新卡金额
	bbzero(ReturnDeposit) ; //退用户押金
	bbzero(MonthFeeFlag) ;; //月费收取标志
	bbzero(FeeAmtFlag); //手续费收取标志
	bbzero(MonthFeeAmt); //卡成本或月费
	bbzero(FeeAmt) ; //手续费
	bbzero(bus_showId); //公交卡号卡面号
	random_value=0; //东莞公交随机数

}

CGlobal::~CGlobal()
{

}



//注意不要放在业务内，因有初始化子业务代码
void CGlobal::init_global()
{
	bbzero(amount);
	bbzero(order_no);
	bbzero(phone_no);
	bbzero(pay_no);
	bbzero(pos_serial);
	bbzero(ago_fee_buffer);	//以往欠费
	bbzero(pre_fee_buffer);	//预付费	
	bbzero(real_fee_buffer);	//实时话费
	bbzero(sub_code);
	bbzero(stan_no);
	bbzero(ICCID_no);
	bbzero(user_name);
//	bbzero(yymmdd);
//	bbzero(hhmmss);
	bbzero(xlt_type);
	bbzero(xlt_card);
	bbzero(tax_serial); //add by lihk 2006-10-19，地税流水
	bbzero(car_no);      //车牌号
	bbzero(car_type);    //车类型
	bbzero(pay_tax);	//应交税额
	bbzero(late_tax);	//滞纳金
	bbzero(mul_tax);	//罚金
	billNum=0; //账单数

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

	bbzero(CATV_id_card); //身份证号
	bbzero(CATV_manual_num); //用户手册号
	CATV_choice = 0;

	mycoolpayeasy_return = 0;

	phone_len = 0;
	pay_no_len = 0;
	user_name_len = 0;
	mbl_flag = 0;
	timeout=0;

	memset(&detail,0,sizeof(detail) );	
	memset(&Manual_Detail,0,sizeof(Manual_Detail) );	//有线手册号查询返回明细
	memset(&dgtSignInfo,0,sizeof(dgtSignInfo)); //东莞通签到信息。
	memset(&dgtCard,0,sizeof(dgtCard));

	bbzero(pki_card);
	bbzero(signin_serial);

	bill_list_len = 0;
	pep_type = 0;
	memset(&bill_item_list, 0, sizeof(bill_item_list));
	memset(&bill_list, 0, sizeof(bill_list));
	//水电煤全局变量
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