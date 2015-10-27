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

	char card_no[31];	//卡号
	char amount[12+1];	//交费金额 （是否金额）
	char origAmount[12+1]; //应收金额
	char overdraft_amount[12+1]; //欠费金额
	char stan_no[13];	//系统参考号
	char TRANSSEQ[20+1];	//订单号
	char APTRANSSEQ[15+1]; // 冲正订单号
	char pos_no[9];		//终端号
	char fee_type[5];   //业务代码
	char fee_type2[5];   //业务代码2
	char order_no[ORDER_LEN+1];	//商户订单号
	char order_time[14+1]; //订单发起时间
	char phone_no[50];	//电话号码
	char phone_city[50]; //号码段归属地
	char phone_province[50];//号码段归属省份
	int	 phone_owner; //号码运营商，1，电信，2 移动 3 联通
	
	char curworkid[8+1]; //当前工号
	char telphone[11+1]; //银行划扣时输入的手机号码

	char custom_no[8+1]; //商户签约中商户终端号
	char custom_pwd[6+1]; //商户签约中商户密码
	char custom_id_card[20+1]; //商户签约中商户身份证号

	char pay_no[30];	//付费帐号
	char pos_serial[7];	//终端流水
	char user_name[61];	//用户名
	char password[9];		//密码
	char new_password[8+1];	//修改密码的新密码
	char ago_fee_buffer[13];	//以往欠费
	char pre_fee_buffer[13];	//预付费	
	char real_fee_buffer[13];	//实时话费
	char sub_code[3];		//子业务代码
	char track2[50];		//2磁道信息
	char track3[110];		//3磁道信息
	
	char xlt_type[2+1];	//小灵通按卡余额交还是按金额交代码
	char xlt_card[512];	//小灵通交费卡信息
	
	char random_num[8+1];	//2次des产生加密的随机数
	
	char tax_serial[11+1]; //2006-10-19，地税流水
	char car_no[8+1];      //车牌号
	char car_type[2+1];    //车类型
	char pay_tax[12+1];	//应交税额
	char late_tax[12+1];	//滞纳金
	char mul_tax[12+1];	//罚金
	int billNum; //账单数
	CarTaxBills_Struct carTaxBills;
	
	char QQ_amount[4+1]; //Q币个数或QQ会员包月的次数
	char QQrate[3+1];    //Q币比值(2位，若为Q币充值"85"表示：Q币:人民币=1:0.85 或 若为QQ会员包月 "10"表示月租为10元)
	
	int card_flag;//东莞卡类型标志
	char sell_card_type[4+1];		//售卡类型
	char card_num[20+1];	//售卡，卡号
	char card_pwd[30+1];	//售卡，密码
	char card_serial[20+1];	//售卡，卡序列号
	char card_amount[12+1];	//售卡，售卡面值
	
	//有线电视，2006-11-20 by lihk
	char CATV_id_card[18+1]; //身份证号
	char CATV_manual_num[11+1]; //用户手册号
	char CATV_IC_NUM[20+1];//IC号
	char CATV_MAC_NUM[12+1];//MAC地址
	int  CATV_choice;
	CATV_Struct1 Manual_Detail;	//手册号查询返回明细
	CATV_Struct2 Id_Detail;	//身份证查询返回明细
	char catv_paypassword[6+1];//支付密码
	char uni_flag[2];		//联通预付费标志
	char tmp_pre_amount[13];	//移动有欠费而交预付费时，打印预付费用，2006－10－25
	char call_tel[16+1]; //来电号码
	char mer_ecard[20]; //商户绑定的e卡

	char id_num[30];  //证件号码, 20070807 by lihk
	int  num;  //数量, 20070807 by lihk
	
	int phone_len;		//电话号码长度
	int card_no_len;	//卡号长
	int pay_no_len;	//付费帐号长度
	int track2_len;	//2磁道长度
	int track3_len;	//3磁道长度
	int user_name_len;	//用户名长
	
	int phs_ticket;	//商户标志，用于区分是否打印付费易卡收据
	int get_card;	//刷卡交费流程标志，0:一次刷卡输密码连续交费 1:每次交费都刷卡 By:KF 06.11.09
	int key_num;	//加密密匙号
	char bank[6+1];	//银行代码
	
	int mbl_flag;	//标志移动是否是在有欠费的情况下交预付费
	int menu_group;	//菜单组别
	int tgroup;		//终端组别
	int group_type; //终端组别类型:0:商户版 1:非商户版
	int askType;	//请求类型
	char welcome[500+1]; //欢迎语
	char update_messag[300+1];//更新提示语句

	char total_fee_type[10][5+1];	//综合收费业务代码
	char total_amount[10][12+1];	//综合收费金额

	char adsl_test_type[30];
	char adsl_custom_type[30];
	char adsl_by_type[30];
	char adsl_modem_type[30];

	char xmcard_type[12+1];  //星空网盟点卡类型
	char xmcard_name[30];    //星空网盟点卡名称

	char product_name[30];   //终端销售产品名称
	char product_num[4+1];   //终端销售产品数量
	char product_perprice[12+1];//终端销售单价
	int product_flag;       //1:POS机2:打印纸3.288礼包4.CDMA选号
	int cdma_num;

	int dayAmo;

	char pay_easy_zh[80+1];

	char city_no[4+1];		//终端所在地区的固话区号
	char acc_city_no[4+1];	//终端所属地区的固话区号

	PrnDetail detail;
	TSPosInfo posInfo;		//终端信息
	TSDLLPosInfo dPosInfo;//DLL所需要的终端信息

	char ip[16+1];
	char server_ip[16+1];
	//东莞公交变量
	char bus_card[8+1];//公交卡卡唯一号(BCD)
	char bus_showId[10+1]; //公交卡面号
	char bus_selltype[1+1];//公交卡售卡类型
	char bus_serial[6+1];//交费流水标识
	char bus_card_serial[1+1];//交易流水标识
	char bus_enq_stan[12+1];//黑名单的交易流水
	char bus_conf[4+1];//公交卡卡认证码
	char bus_old_amo[8+1];//公交卡充值前余额
	char bus_pay_amo[8+1];//公交卡充值金额
	char bus_new_amo[8+1];//公交卡充值后金额
	char bus_card_info[48+1];//交易数据上传信息
	char bus_date[8+1];//黑名单的日期
	char bus_time[6+1];//黑名单的时间
	char dkq_code[4+1];//读卡器机具编号
	char dkq_PSAM[16+1];//读卡器中PSAM卡号
	char bus_yj_amo[8+1];//押金
	char bus_card_type[2+1];//应用子类型
	char bus_failcode[2+1];//错误代码
	int HID_TYPE;		//东莞清华同方读卡器设置 默认为0，关闭。
	int bus_trade_flag; //东莞扣费标记，1为扣费中，0为交易完成。
	//东莞通签到信息
	DGTSignInfo dgtSignInfo;
	DGTCard dgtCard;
	DGTM1Card dgtM1Card; //东莞通旧卡信息
	GLQQPayInfo glQQPayInfo;//广禄qq交易信息
	//DGTDeviceInfo dgtDeviceInfo; //设备信息
	char busi_id[12+1]; //企业账户id
	char posKey[32+1]; //终端私钥
	char posId[12+1]; //清算设备代码
	char dgt_LocalSequence[11+1]; //东莞通交易流水号
	char dgt_posSerial[9+1];//东莞通终端流水号
	char Centerseqno[18+1]; //东莞通中心流水
	int  random_value; //东莞公交随机数

	//下面参数以旧换新中使用
	
	char ReturnAmt[10+1]; //写入新卡金额
	char ReturnDeposit[8+1]; //退用户押金
	char MonthFeeFlag[1+1]; //月费收取标志
	char FeeAmtFlag[1+1]; //手续费收取标志
	char MonthFeeAmt[8+1]; //卡成本或月费
	char FeeAmt[8+1]; //手续费


	char DEPOSIT[8+1]; //押金
	char SYSTEMNO[15+1]; //系统参考号

	char phone_amount[12+1];   //直充后返回的手机余额

	int mycoolpayeasy_return;
	int title_flag;   //IPOS标题设置
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
	int DianXin_Turn;		//电信直充与售卡开关。0：直充；1：网厅
	int Reversal_count;		//商户冲正次数限额
	int term_level;//商户级别
	int timeout;

	int menu_group_mbl_flag;//0：无限制 1：非移动版 2：移动版

	char car_id[6+1];//车架号后6位(改为6位 2012-01-16)
	char engine_no[6+1];//发动机号后6位(改为6位 2012-01-16)
	char traffic_code[250+1];//违章代码
	char traffic_area[2+1];//违章地区
	char transfer_flag[1+1];//投递标志
	char paper_no[2+1];//文书号
	char address[60+1];//收件人地址
	char traffic_mny[20];//罚款
	char transfer_mny[20];//邮递费
	char service_mny[20];//服务费
	char total_mny[20];//总金额
	char traffic_area_name[20];//违章地区名
	char traffic_place[80+1]; //违章地点
	char traffic_date[80];//违章的时间


	char ICCID_no[20+1];

	int input_times;//直充号码输入次数

	int print_paper;//套打发票纸

	int bill_list_len;		//电信直充信息
	int pep_type;
	Bill bill_list[MAX_BILL_NUMBER];
	Bill_Item bill_item_list[MAX_BILL_ITEM_NUMBER];

	//char DIY_URL[30];	//DIY网站地址
	char CS_URL[30];	//客服地址

	char yct_disp[1024+1];   //羊城通充值信息

	int commission_free;
	char asdl_user [60+1]; //中山ASDL 用户名
	int asdl_user_len ; //中山ASDL 长度

	char bestpayAccount[11+1]; //翼支付账户
	int	bestpayAccount_len; //翼支付账户长度
	char bestpayPass [100];//翼支付账号密码
	int bestpayPass_len; //翼支付账户密码长度
	char bestpayTotalMny[13+1];//翼支付账户余额
	int agreeFlag;//判断用户是否已经确认

	/*----------------------薪加薪理财------------------------*/
	char xjx_stan[12+1];//系统参考号
	char xjx_seq[10+1];//薪加薪交易记录序号
    char in_account[30+1];	//转入账号
	char out_account[30+1];   //转出账号
	char abstract[50+1];    //摘要
	char payer_info[50+1]; //付款方信息
	char payee_info[50+1];  //收款方信息
	char xjx_phone_no[50+1]; //电话号码
	char term_serial[6+1];  //终端流水号
	char xjxbank[6+1];//银行代码
	char status_info[30+1];//交易状态描述
	char txn_datetime[20+1];//到账日期/时间
	char xjx_rspcode[2+1];//返回码
	/*--------------------------------------------------------*/

	/*****************羊城通充值第三代新增全局变量***************************/
	int com;
	int comType;
	//终端监控
	char monitor_frequency[4+1];//监控频率
	char next_monitortime[14+1];//下次发起监控配置的时间
	char announce[128+1];//公告信息
	char term_ver[32+1];//终端软件版本号
	/************************************************************************/

	/*********************水电煤业务全局变量*********************************/
	char AREACODE[6+1];//水电煤业务业务受理地区
	char DX_CODE[4+1]; //电信地区号码
	char AREA_NAME[60]; //地区名称
	char PARENT_CODE[6+1]; //父亲地区代码
	char PARENT_NAME[60+1];//父亲地区名称
	int  AREA_FLAG; //地区代码标志 1为一级，2为2级
	char SHORTNAME[60];//地区简称

	char sdm_PAYMENTCODE[30+1]; //收费单位编号 至少18位
	char sdm_PAYMENTNAME[128+1]; //收费单位名称
	char sdm_BUSCODE[3+1]; //业务代码
	char sdm_BUSNAME[64+1]; //水电煤业务名称
	char sdm_CUSTOMERNAME[64+1]; //水电煤客户姓名
	char sdm_totalFee[12+1]; //水电煤总缴费金额
	char sdm_SELECTVALUE[64+1]; //用户号或者条形码
	char query_amy[12+1]; //查询金额
	char sdm_SYSTEMNO[15+1]; //系统参考号
	int	 SDM_BILLNUM;        //账单数
	char SDM_BILLNO[200]; //水电煤账单号
	char SDM_BILLMONTH[100+1]; //账期显示 
	char sdm_TRANSSEQ [15+1] ; //交易流水号
	char sdm_ORDERSEQ[32+1]; //订单号
	char deleyAMount[12+1]; //滞纳金
	char sdm_billAmount[12+1]; //账单金额
	char sdm_TXNAMOUNT[14+1] ; //交易金额14位。分为单位
	int  BILLSTAT; //水电煤账单类型
	sdmBills_Struct sdmBills; //水电煤账单信息

	/*******************************3G流量卡**************************************/
	char RECHARGEFLOW[12+1]; //充入流量
	char g3Phone[11+1];//充值手机号

	

};

#endif // !defined(AFX_GLOBAL_H__635C685F_4310_48D9_9A01_8ECD8498001B__INCLUDED_)
