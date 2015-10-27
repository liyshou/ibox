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
	//char mac_random_num[8+1];	//算mac用的随机数
	
public:
	CTrade();
	virtual ~CTrade();
	
	int updateConInfo();

	int signWorkID(char *workid);

	int BankHK(const char *type, int inputTel=0);//银行划扣
	int BankHK_telInqHdl(const char *type, int inputTel, int &first3mon);//查询有无欠费
	int BankHK_telInqMsg(const char *type, int &first3mon);//下发查询信息
	int BankHK_telPayHdl(const char *type, int bizIdx=0, int first3mon=0);//交欠费
	int BankHK_prepay(const char *type);//无欠费时预存
	int BankHK_nameconfirm(const char *type);//用户名匹配
	int BankHK_HK(int type);//登记划扣

	int Bankc_zz(const char *title); //银行卡转账

	int UnionTransfer_out(const char *title);//银联转账(转出)
	int	UnionTransfer_in(const char *title);//银联转账(转入)
	int UnionbanInqMoney(const char *title);	//银行卡余额查询(银联)
	int UnionbanInqLast();
	int UnionbanInqHistory(int inqType);
	int UnionbanInqMenu(int type=0);

	int NH_Transfer(const char *title); //农行转账演示版本

	int banTransfer(const char *title);	//银行划帐
	int banTransLocal(const char *title, PTSTradeInfoRecv pTradeInfoRecv);	//银行划帐本地化
	int banInqTrade(int inqType);
	int banInqMoneyTrade(const char *title, char *amount_out=NULL);	//银行卡余额查询(交行)
	int banInqLast();
	int banInqHistory(int inqType);
	int banReturnFail(char *response_code);
	int banInqMenu(int type=0);
		
	int getBankMny(const char *title);	//银行卡余额查询(建行和招行)

	void set_in_par(CGlobal in_gbl); //设置传入参数
	void get_gbl(CGlobal &out_gbl);
	void get_xlt_info(char *card, int &track2_len, char *track2=NULL);
	
	int logonHdl();			//签到（旧加密体系）
	int logonHdl(int type);	//签到（新加密体系）
	int mngCard(const char *type, int card_flag = 0,  int pay_flag = MNG_ONE, const char *title="", int minLen=12, int maxLen=16) ;	//刷卡，默认为普通卡 交费流程：默认是每笔交费都刷卡
	int getCardInfo(const char *type, char *card_info, int card_flag = 0, int pay_flag = MNG_ONE, const char *title="", int minLen=12, int maxLen=16);
	int getCardNum(char *card_info, int card_flag = 0 );
	int bindcard(char *cardno_in, char *cardno_out, int *cardno_out_len);

	int	telTrade(const char *type, int inputTel = 0 );	//电信交易 type:标题; oldTel:0-需输入电话号码 1-使用上次查询交易的电话号码
	int mblTrade();	//移动交易
	int uniTrade();	//联通交易
	int	merTrade();	//商户交易
	int	taxTrade(char *type, int input_flag=0);	//车船税
	int QQTrade(const char *type, int select=0,int input_flag=0);    //腾讯QQ KF:2006.10.24
	int RaffleTrade(const char *type, int select=0);    //体彩充值
	int xltTrade(const char *type);	//小灵通付费易卡
	int cardChgPwd();	//修改卡密码（e卡和小灵通付费易卡）
	int cardInqHdl(char *amount_out=NULL);	//卡查询（e卡和小灵通付费易卡）
	int ecardInqHdl();//e卡查询 edit by liuxiao 20130923
	int yj_InqHdl(); //查询酬金
	int cardInq_yctyp();
	int sellCardTrade(const char *type, int select=0,int input_flag=0);	//售卡交易
	int sellZWgameCard(const char *type, int select=0,int input_flag=0); //众网游戏卡（已改为天下通卡）
	int CATVTrade(const char *type);	//有线电视,2006-11-20 by lihk
	int rePrintHdl(int flag); //补打发票
	int newrePrtTrade(int flag);//东莞按stan号和交费号码补打发票
	int safeOutPutMsg(char *msg, int flag);

	int piccTrade(); //车强险
	int foshanTrade(const char *title, const char *city_no);  //全省综合收费 

	int merInqTrade(int inqFlag); //商户交易查询

	//小灵通2期转帐
	int xltAccTrans(const char *title);  
	//小灵通2期充值
	int xltMnyIn(const char *title);
	//小灵通2期查询
	int xltInqTrade(const char *title);

	//空中充值 flag:1.移动2.广州电信3.全省电信
	int mblAirPay(const char *title, int biz_flag, int select=0, int input_flag=0,int mbl_input_flag=0);
	//广东移动省内充值
	int mblAirPayForGDYD(const char *title, int select=0, int input_flag=0,int mbl_input_flag=0);

	//按号码查询姓名
	int nameInqHdl(char *user_name, int *user_name_len,char *amount);

	//含查询的电信直充 flag:1 单一交费  2 ADSL  3 按合同号
	int newAirPay(const char *title, int type_flag);

	//电子代办 电信直充 flag:1 单一交费  2 ADSL  3 按合同号
	int DZDBAirPay(const char *title, int type_flag, int functype);
	int phone_pay_trade(const char *title);
	//移动全球通充值
	int mblTrade_pre(const char *title);	

	//e卡充值
	int ecardMnyIn_Hdl(const char *title, int area_flag=0);
	int ecardMnyIn(const char *title, int area_flag=0);

	//e卡充值申请
	int ecardAppli_Hdl(const char *title);
	int ecardAppli(const char *title);

	//最新电影折扣票
	int filmPay(const char *title);

	int sendMsg();
	int print_fail(const char *title);//重打印失败发票
	int print_last(const char *title,int mbl_flag=0);//重打印最后一张发票
	int getBit46(char *bit46);

	//电信发票补打
	int rePrintTel(const char *title);

	//e卡充值提示
	int getEcardMsg(const char *title, int flag=0);//e卡充值提示

	int testIcReaderCommand(const char *title);
	int testFSKPack(const char *title);

	//交通罚款
	int trafficTrade(const char *title, int flag=0);
	int carnum_trafficTrade(const char *title, int flag=0);
	int traffic_pay(const char *title, char *car_type, char *paper_num);

    //全省交罚
	int provinceTrafficTrade(const char *title);
	int provinceTrafficInqHdl(const char *title);
	int provinceTrafficPayHdl(const char *title);

	//交罚补打凭证
	int rePrintTra(const char *title);

	//设置羊城通PIN
	int SetPIN(int flag);

	
	int ysqCatv();
	//交费易签约
	int ysqCatv_new();
	//商户签约
	int customer_sign();

	//小灵通预付费100送300
	int YH_xlt(const char *type);

	//////////////////*测试功能函数*/////////////////////////
	
	int Test_TS_GetPosInfo(int flag);//测试终端信息的获取 flag: 1.终端电话号码；2.终端号；3.终端左串口设备；4.终端右串口设备；5.终端内置设备
	int Test_TS_OutPutMsg(int flag);//测试提示信息 flag: 1.下发提示信息；2.下发安全提示信息；3.下发多条提示信息
	int Test_TS_B4Print(int flag);//测试打印功能 flag: 1.打印；2.安全打印
	int Test_TS_SendMsg(int flag);//测试传输信息功能 flag: 1.通知；2.交易结果；3.账单交易结果；4.账单；5.版本号；6.终端序列号
	int Test_TS_PSAM(int flag);//测试PSAM卡 flag: 1.PSAM卡号；2.PSAM卡随机数
	int Test_TS_UpdateMsg(int flag);//测试更新功能 flag: 1.更新提示信息；2.更新终端参数；3.更新首页信息；4.更新本地菜单；5.更新PSAM卡密钥
	int Test_TS_TradeControl(int flag);//测试交易控制函数 flag: 1.测试手输卡号 2.测试刷卡并获取卡的信息 3.测试输入金额 4.测试输入数字 5.测试输入全字符 6.测试日期 7.测试年月 8.测试明文密码 9.测试输入字母 10.测试输入汉字
	
	/////////////////////////////////////////////////////////


	int selectxlt();//小灵通选号
	//	int telbell();//七彩铃音
	//	int telmail();//语音信箱
	int sx_ysl(int flag);//  实现预受理：flag：1.银行划扣受理；2.已有固话新装ADSL；3.申请e8套餐；4.e8包年升级套餐；5.申请e6套餐
	int telIBSS(const char *title, char *ctrl3, char *biztid5);
	int selectcdma();//c网选号



	//业务受理
	int add_ADSL(const char *title);

	//星空网盟点卡销售
	int sell_xmcard(const char *title, int cflag=0);

	//终端销售 
	int sell_pos(int sflag);
	int payonce(int subtype, const char *title);

	int UpdateMenus(int menu_group, char sysnum);

	//肇庆 电信预受理
	int dxysl_trad(const char *title);
	//清远 电信预受理
	int qy_dxysl_trad(const char *title, int t_select);

	//汽车年票
	int nianpiaoTrade(const char *title, int flag);
	int simplePackHdl(char* send_data,char* recv_data);//年票demo版包处理

	//汽车年票交纳
	int car_np_Trade(const char *title, int input_flag=0);

	//软终端交易明细查询
	int iposTxnEnq(const char *title, int inout, int mode,int mbl_flag=0, int query_flag=0);//int inout:0交费 1充值 mode:0老板模式 1员工模式 mbl_flag:2非移动 1移动 0无限制 query_flag:0全部1最近24小时
    //单日交易明细查询-门户调用
    int online_ser(const std::string& bizType, int b_boss);
    int drcx_online(int b_boss);//当日查询
    int yjycx_online(int b_boss);//月交易查询
    int mxcx_online(int b_boss);//明细查询
    int cxcz_online(int b_boss);//查询冲正
	int selfService_online(int b_boss);//登陆子服务门户首页
	int last_mnth_online(int b_boss);//上月交易查询
	int zjdzcx_online(int b_boss);//资金到账查询
	int cashTransfer_online(int b_boss);
    //密码改造2012-09-05 -- BEGIN --
    int iPos_Login(char* amount_out);
    int tenk_encrypt(const std::string& pwd , char* outBuf , size_t outMaxLen);
    int bosspwd_chg();//老板密码修改
    int transpwd_chg();//转账密码修改
    int paypwd_chg();//交易，支付密码修改
    //从终端获取一行输入count次数，type，输入形式，类同TS_INPUT_XXX, title标题显示，minlen，maxlen，长度限制
    std::string get_line_from_term(int count , int type, int minlen , int maxlen , const std::string& title);
    //终端信息提示
    int info(const std::string& msg);
    //flag : 1 支付密码校验，2转账密码校验，0老板密码校验
    int pwd_chk(int flag);
    //密码改造2012-09-05 -- END --

	//修改老板密码(不经PE直连接口)
	int chgPEPwd(const char *title);
    //修改转账密码(不经PE直连接口)
	int chgTransferPwd(const char *title);
	//更新终端参数
	int updatePosPar(const char *title, int t_flag=0);
	//校验老板密码或转账密码
	int checkPEPwd(char *pwd,int type=0);//type=0为校验老板密码；type=1为校验转账密码
	//设置转账密码
    int setTransferPwd(char *newPwd,char *oldPwd);
	//校验转账密码是否为空
	int checkPwdNotNull();
	int checkPEPwd_Hdl(const char *title);
	int checkTransferPwd_Hdl(const char *title);
	//修改IPOS密码输入方式
	int chgEPwdType(const char *title);
	//修改日交易限额
	int chgDayAmo(const char *title);
	//CDMA选号
	int tel_card_select(const char *title, int tel_card_type); //tel_card_type 电信卡类型:1：153畅听卡  2.153大众卡

	//交费易钱包充值
	int PayEasy_MoneyIn(const char *title);
	//商户结算
	int chk_money(const char *title);

	//东莞公交
	int busTrade(const char *type,int chargeType);//东莞公交
	int busNotEndHdl(const char *type);//东莞公交未结束交易
	BYTE xor(const BYTE *data, int dataLen);
	int  busSndRcvIcData(const char *title, const BYTE *downData, const int downLen, BYTE *upData, int &upLen, char * pos_serial);
	//东莞公交HID读卡器设备发送及接收数据
	int    busSndRcvHID_IcData(const char *title, const BYTE *downData, const int downLen, BYTE *upData, int &upLen, char * pos_serial);
	void   returnBusFail(char stat);  //东莞公交读卡器错误返回处理
	void   returnHIDBusFail(char stat);  //东莞HID公交读卡器错误返回处理
	int   static  getComPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int *com, int *comType);
	int  dgtResubmit(TSDLLPosInfo dPosInfo,char *reSubmitField,char * tradeStatus,char * orderStatus);
	int dgtResubmitCheck(CGlobal *pgbl);
	int dgtCardsQuery();

	//信用卡还款
	int Credit_Card_Repayments(const char *title);
	int Credit_Card_Repayments_Hdl(const char *title);

	//E卡卡内转账
	int eCard_Trans(const char *title,int tag=0);
	int eCard_Trans_Hdl(const char *title,int tag=0);
	

	//超爽交费易
	int MyCoolPayEasy(const char *title);
	int MyCoolPayEasy_Hdl(const char *title, int input_flag=0);
	int MyCoolPayEasy_CarNum_traffic_inq(const char *title, char *incarnum, car_paper_info *car_paperinfo);
	int MyCoolPayEasy_CarTax_inq(const char *title, char *carnum, char *mny);

	//商户酬金结转
	int Customer_Trade();
	
	int revAppli(const char *title);

	//界面标题设置
	int SetTitle();

	//系统最新更新
	int GetNewMessage(int flag=0);

	//东莞宽带套餐续存
	int DG_ADSL_xc(const char *title, int type_flag);
	int new_DG_ADSL_XC(const char*title);

	//电信直充双选择模式
	int DX_AirPay_D(const char *title, int type_flag, int input_flag=0, int select_flag=0);

	//深圳电信
	int sz_dx_trade(const char *title);
	
	int mngCard_Hdl(const char *type, const char *title="", int card_flag=0,  int pay_flag=-1, int minLen=12, int maxLen=16);
	int reGetPwd(const char *type);
	int returnFail(char *response_code);	//失败返回
	int returnFail2(char *response_code);

	//大客户版本
	int Submission(const char*type);
	int Checkmission(const char*type);
	int Paymission(const char*type);

	//客服中心
	int OpenKF(int flag=0);

	//逢万送十活动 Q币领取
	int Present_handsel(const char*type);
	int Present_handsel_Hdl(const char*type);


	//东莞手机放号
	int Sell_blank_phonecard(const char*title, int type); // 0 东莞电信 1 联通
	int Confirm_blank_phonecard(const char*title, int type);	// 0 东莞电信 1 联通
	int Confirm_blank_phonecard_hdl(const char*title, int type); // 0 东莞电信 1 联通
	int Sell_phoneno(const char*title);
	int Sell_phoneno_hdl(const char*title);

	//电信业务预受理
	int telaffair_preaccept(const char* title);

	//电信直充
	int pep_query_hdl(char* areacode, char* num, char* month, char* feetype, int type_flag);
	int pep_query(char* areacode, char* num, char* month, char* feetype, char* returnstring);
	int pep_show_bill(char* totalfee, char* area_code, int flag);
	int pep_distribute();

	//游戏售卡
	int sell_txtgame_card(const char* type);

	//游戏直充
	int game_recharge_showGames(const char* title, int type); //0: 全部  1:关键字
	int game_recharge_showAmount(const char* title);
	int game_recharge_do1(const char* title);
	int game_recharge_do2(const char* title);
	int qqGame_recharge_do(const char* title);//腾讯QQ直充，天下通接口，可充全国。


	//联通直充
	int unicom_direct_recharge(const char* title);

	//联通空中放号
	int Unicom_Sell_Phoneno(const char* title);
	int Unicom_Sell_Phoneno_Hdl1(const char* title);
	int Unicom_Sell_Phoneno_Hdl2(const char* title);

	//检验是否广州号码
	int is_gd_phone(char* phone);

	//长途汽车票
	int coachTicketTradeGetInfo();
	int coachTicketTradeQuery();
	int coachTicketTradePay();

	//设置打印新电信发票（稍长发票纸）
	int setPrintPaper();

	//e卡查询
	int ecard_query(const char* type);

	//推荐业务
	int getRecommandMenu(int type);
	int getYCTRecommandMenu();

	int YctSuccTxnEnq(const char *title);
	int printYctBill(const char *title);

	//羊城通白卡/套餐购买业务
	int Yct_WCardPack_purchaseHdl(const char *title);
	//羊城通白卡采购业务
	int Yct_Whitecard_purchaseHdl(const char *title);
	//羊城通套餐采购业务
	int Yct_Package_purchaseHdl(const char *title);
	int Yct_ICReader_purchaseHdl(const char *title);

	//翼支付账户充值
	int bestPay_rechargeHdl(const char *title,int iSelect);
    
	/*------------薪加薪理财-------------*/
	//薪加薪审核权限设置
	int xjx_checkauthorcfg_hdl(const char *title);
	//薪加薪二级审核设置
	int xjx_LLcheckcfg_hdl(const char *title);
     //薪加薪三级审核设置
	int xjx_LLLcheckcfg_hdl(const char *title);
	//设置审核金额
	int xjx_setcheckamount_hdl(int ilevel, const char *title);
	//设置员工(包括录入员，审核员，二级审核员)
	int xjx_setstaff_hdl(int iSelect,int ilevel);
	//设置划拨账号
	//flag: 0 - 设置划出账号；1 - 设置汇入账号 
	int xjx_accountcfg_hdl(int flag,const char *title);
	//审核设置查询
	//iType: 0 - 二级审核查询；1 - 三级审核查询；
	int xjx_checkconfig_InqHdl(int iType,char *title);
	//划拨账号设置查询
	//iType: 0 - 划出账号查询；1 - 汇入账号查询；
	int xjx_accountconfig_InqHdl(int iType,char *title);
	//修改密码
	//iuserType: 0 - 管理员；1 - 录入员；2 - 审核员；3 - 二级审核员
	int xjx_updatepwd_Hdl(int iuserType,char *title);

	//薪加薪资金划拨
	int xjx_funds_transferhdl(const char *title);
	//交易查询
	//flag: 0 - 成功交易；1 - 所有交易
	int xjx_txn_InqHdl(int flag,const char *title);
	//审核薪加薪资金划拨
	int xjx_txn_checkHdl(const char *title);
	//薪加薪理财资金到账查询
	int xjx_pymreceived_InqHdl(const char *title);
	//获取交易状态说明
	//输入参数：char *status - 交易状态
	//输出参数：char *status_info - 交易状态说明
	int xjx_get_statusinfo(char *status,char *status_info);
	/*-----------------------------------*/

	/*羊城通充值撤销*/
	int Yct_charge_cancel(const char *title);
	/*羊城通卡扩展应用*/
	int Yct_Card_Extend(const char* title);
	int YctradeHdl(CGlobal gbl, char *title, int payType, char *pki_card, char *signin_serial);
	int YCT_AckTrade(CGlobal gbl, YCT_DATA yct_data,int status );
	int YCT_RevTrade(CGlobal gbl, YCT_DATA yct_data);
	int  YCT_AckRevTrade( YCT_DATA yct_data,int status);
	int  YCT_PayPe(YCT_DATA yct_data, BYTE* party, BYTE* rspcode,char *stan,char *pos_serial);
	//全国水电煤业务
	int sdm_showAreas(const char* title, int type,CGlobal gbl); //显示充值地区
	int showSMD_cityList(char *title); //按照省份编号进行查询缴费地区
	int sdm_district_service(const char *title ); //水电煤地市查询
	int sdm_service(const char * title);
	int sdm_trade(char *title); //水电煤查询接口
	int area_query_service(CGlobal gbl,string &datas,char *WEBSVRNAME,char *WEBSVRCODE,string &strRsp,string &errStr);
	int sdm_query_service(char *WEBSVRNAME,char *WEBSVRCODE,char *SELECTTYPE,char *sdm_SYSTEMNO,int *billNum,int *BILLSTAT,string &strerr);
	int sdm_pay_service(char *WEBSVRNAME,char *WEBSVRCODE,char *sdm_SYSTEMNO,char *sdm_txnamount,int CASHTYPE,char *feeCircle,int PAYTYPE,char *OPERUSER,char *OPERPASSWORD,string &errString,string &errcode);
	//int  show_areaDatas(vector<BUSDATAS> &datasVector,CGlobal gbl);
	int  show_sdmBills(CGlobal gbl); //显示水电煤账单信息
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
	//东莞通；
	int dgtCardSale(char *type);
	int dgtBusTrade( char *type,int chargeType);
	int dgtSignIn(); //签到交易
	int dgtSignOut(); //签退交易
	int dgtCardInqHdl(PDGTCard pdgtCard,int flag) ; //读取卡片余额等信息
	int showDGTChargeTrade();
	int showDGTALLTrade();
	int dgtCardChargeRollBackService(char *WEBSVRNAME,char *WEBSVRCODE,char *TRANSSEQ, string &strerr);
	int dgtCardTransfer(char *type);
	int dgtM1CardInqHdl(PDGTM1Card pdgtM1Card,int type); //读取卡片余额等信息
	int dgtOrderVerifyService(char *WEBSVRNAME,char *WEBSVRCODE,int type,char *BUSCODE ,CHAR *BUS_CONTERNT,string &strerr);
	int dgtOrderProduceService(char *WEBSVRNAME,char *WEBSVRCODE,char *TRANSSEQ,string &strerr);
	int checkdgtUnsubmit(int tradeFlag);
	int qq_guanglu_pay_service(char *WEBSVRNAME,char *WEBSVRCODE,char *operUser,char *password,char *SYSTEMNO,string &rspcode,string &strerr);
	int GLQQTrade(const char *type, int select=0,int input_flag=0);
	int bankInfoQry();
	int getBankInfoService(char * WEBSVRNAME,char *WEBSVRCODE,char *staff_code,char *CUSTCODE);
	int getCardStatusResult(); //东莞公交激活状态查询
private:
	//小灵通2期查询处理
	int xltInqHdl(const char *title);
	int xltInqMsg(const char *title);

	//小灵通预付费100送300
	int YH_xlttelInqHdl(const char *type);
	int YH_xlttelInqMsg(const char *type);
	int YH_xlttelPayHdl(const char *type);	
	
	//电信交易处理
	int telInqHdl(const char *type, int inputTel, int &first3mon);	//电信交易 type:标题; oldTel:0-需输入电话号码 1-使用上次查询交易的电话号码; first3mon:0-非三个月以上欠费的前3个月的交费 1-三个月以上欠费的前3个月的交费
	int telInqMsg(const char *type, int &first3mon);// first3mon:0-非三个月以上欠费的前3个月的交费 1-三个月以上欠费的前3个月的交费
	int telPayHdl(const char *type, int bizIdx=0, int first3mon=0);//  first3mon:0-非三个月以上欠费的前3个月的交费 1-三个月以上欠费的前3个月的交费
	int telPayHdls(const char *type, int first3mon);	//电信多业务交费(预缴费)  first3mon:0-非三个月以上欠费的前3个月的交费 1-三个月以上欠费的前3个月的交费

	//移动交易处理
	int mblInqHdl();
	int mblInqMsg();
	int mblPayHdl();

	//联通交易处理
	int uniInqHdl();
	int uniInqMsg();
	int uniPayHdl();

	//全省综合收费处理
	int foshanInqHdl(const char *title, const char *city_no);
	int foshanInqMsg();
	int foshanPayHdl(const char *title);
	int foshanRdField63();
	void foshanCkCode(int code, char *type);
	
	//电子代办打印明细获取
	int DZDBRdField63();
	//商户交易处理
	int merInqHdl();
	int merInqMsg();
	int merPayHdl();

	//车船税处理
	int taxInqHdl(char *type, int input_flag=0);
	int taxInqMsg(char *type);
	int taxPayHdl(char *type);
	
	//3g流量卡充值
	int AutoSelectG3TC(const TSDLLPosInfo dposInfo,char *money,char *flow);

	//腾讯QQ处理
	int QQInqHdl(const char *type, int input_flag=0);
	int QQInqMsg();
	int QQPayHdl(const char *type, int select=0,int input_flag=0);

	//体彩账户充值
	int RaffleInqHdl(const char *type);
	int RafflePayHdl(const char *type, int select=0);

	//有线电视处理
	int CATVInqHdl(const char *type, int flag = 0);
	int CATVInqMsg1();
	int CATVInqMsg2(const char *type);
	int CATVPayHdl(const char *type);
	int CATVMenu(int flag);

	//交罚业务处理
	int trafficInqHdl(const char *type, char *enqFIELD48, char *enqFIELD63, char *enqStan,int flag=0);
	int trafficInqMsg(const char *type, char *enqFIELD39, char *enqFIELD63, char *gbl_amount);
	int trafficPayHdl(const char *type, char *enqFIELD48, char *enqFIELD63, char *enqStan);

	//商户交易查询
	int merInqTrade_inq(int inqFlag);
	int merInqTrade_msg(int inqFlag);

	int customer_sign_Hdl();

	int posReturnFail(int iRet,char *acErrMsgBuf);	//POS失败返回

	int xltMulCard(const char *type, int flag = 1);//小灵通多卡处理
	int packHdl(int flag);	//包处理函数
	int selectAmount(int flag = 0, int select=0);//交易过程中需要选择的金额相关的菜单
	int selectDGAmount(int flag = 0, int select=0);//交易过程中需要选择的金额相关的菜单
	int selectSubcode();//交易过程中需要选择的子业务代码相关的菜单
	int telPayExtend(const char *type);//电信交费后，附加询问是否做预付金和办理授权
	int ysqHdl();/*预授权*/
    int ysqconfirm();
	int ysqInqHdl(int type);

	int banInqMoneyHdl(const char *title, char *amount_out=NULL);	//银行卡余额查询(交行)
	
	int get_key_num(char *bank_code);	//取卡表
	
	int getCardPwd(const char *type);
	int getCard_1(const char *type, int card_flag = 1, const char *title="", int minLen=12, int maxLen=12);	//一次采集卡号和密码（不加密）

	int cardno_fee(char bank[6+1], char *fee_type, char *acc_city_no, int pos_type);//卡与业务的支持关系
	int get_area_biz(int biz_flag, char *area, char *accarea, char *biz_type);
	int get_favorable_menu(char *area,char *biz_type,int menu[]);
	
	int isTrack2(char *card_info, int &pos);
	int isTrack3(char *card_info, int &pos);

	void getFeeType(const char *feeType, char *name);
	int  getRealPwd(char *card, char *inPwd, char *outPwd);

	//东莞莞市通公交充值
	int busChargeZero(const char *feeType,const char *type); //0元充值,仅仅用于东莞公交清华同方读卡器设备 
	int dgtBusInqHdl(const char *type,int chargeTye);

	int dgtEffectiveCheck(int type,char * errMsg); //卡片有效性检查
	int dgtSignInService(char *WEBSVRNAME,char *WEBSVRCODE,PDGTSignInfo pdgtSignInfo,string &strerr);
	int dgtSignOutService(char *WEBSVRNAME,char *WEBSVRCODE,string &strerr);
	int cardOperate_service(char *WEBSVRNAME,char *WEBSVRCODE,char *PASSFLAG, char *CARDOPRTYPE,char *TRANSTYPE,char *APDU,char *TRANSSEQ,string &rsp_code,string &strerr);
	int cardSaleService(char *WEBSVRNAME,char *WEBSVRCODE,char *OPERATIONTYPE,char * TRANSTYPE,char  *TRANSSEQ,char *SALEMODE,string &strerr);
	int dgtCardChargeService(char *WEBSVRNAME,char *WEBSVRCODE,char *OPERATIONTYPE,char *TRANSTYPE,char *TRANSSEQ, string &strerr);
	int dgtCardCharge(char *title);
	int cardSale(CGlobal gbl,char *tranMessage,char *tranType);
	int dgtRollBack();
	int getGgtResultByKeep(char *WEBSVRNAME,char *WEBSVRCODE,char *KEEPCODE,string& strPaystat,string &strerr);//通过keep值获取详情


	//东莞公交(汇通)
	int busInqHdl(const char *type,int chargeType);	//查询
	int busPayHdl(const char *type);	//交费
	int busRevHdl(const char *type);	//冲正
	int busConHdl(const char *type, int flag=0);	//确认,flag:0成功；1失败
	int busbusConHdl(const char *type, int flag=0);	//确认,flag:0成功；1失败
	int  read_IcCardInfo(const char   *type, char * term_serial,int deviceType,char *cardMny); //读取IC卡信息
	int cardPayTelOne(const char *type, char *amount, char *fee_type, char *sell_card_type, char *cardpwd);
	//dest_list: 0:固话 1:小灵通 2:手机 3:ADSL;latn_id_list:区号 广州020;charge_num_list:电话号码
	int cardPayTel(const char *type, char *latn_id_list, char *dest_list, char *charge_num_list, char *amount, char *fee_type);
	int payModeSelect(const char *type, char *biz_type, char *amount, char *city_no, char *phone_no);
	int MyCoolPayEasy_cardPayTel(const char *type, char *latn_id_list, char *dest_list, char *charge_num_list, char *amount, char *fee_type);
	
	void getHeadPage(int tgroup, int term_level, vector<Epos_new_message> &pNew_message, int title_flag, char *pos_no, char *mny, char *yj_buff, char *headPaye);

	int m_com;	//使用串口
	int m_comType;	//串口类型

};

#endif // !defined(AFX_TRADE_H__C7E243D1_16DD_4096_B889_C55C2B319F08__INCLUDED_)
