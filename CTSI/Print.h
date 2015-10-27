// Print.h: interface for the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINT_H__A552D9AF_7618_43B1_9BA1_96BCC2977A51__INCLUDED_)
#define AFX_PRINT_H__A552D9AF_7618_43B1_9BA1_96BCC2977A51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "Global.h"
#include "Pack.h"

class CPrint  
{
private:
	int rdField63_2();
	int rdField63_2(CPack pack);
	int rdField63_3(char *card_num, char *card_pwd, char *card_serial, char *amount, char *eff_yymmdd ,char *xm_serial);
	int rdField63_3(CPack pack,char *card_num, char *card_pwd, char *card_serial, char *amount, char *eff_yymmdd ,char *xm_serial);
	void ckCode(const char *code, char *type);
	int set_content(PPrintTemp prt, char *cont, int type=2, int flag=0);
	int set_yctthird_content(PPrintTemp prt, char *cont);
	int print_pre_detail(const char *type);	 //打印预付费明细
	int print_pym_detail(const char *type);   //打印欠费明细
	int print_card_detail(const char *type);	 //打印售卡明细	
	int print_mer_detail(const char *type);	 //打印商户明细（目前为天天）	
	int print_CATV_detail(const char *type);	 //打印有线欠费明细
	int print_CATVDiscount_detail(const char *type);	         //打印珠江宽频优惠
	int print_PICC_detail(const char *type);   //打印picc明细
	int print_film_detail(const char *type);   //打印电影票明细
	int print_ysq_detail(const char *type);    //打印预授权明细
	int print_ADD_ADSL(const char *type);      //打印固话加装ADSL
	int print_traffic_detail(const char *type, char *field48="", char *field63="",int b63Idx=0);//打印交通罚款明细
	int print_SELL_POS(const char *type,char *field63="");
	int print_SELL_POS(CPack pack,const char *type,char *field63);
	int print_chk_amo(const char *type, char *content);
	int reprint_bus_detail(const char *type);//公交开卡补打印
	void tranBusCard(char *bus_card_4, char *bus_card_no_10);
	int print_yct_detail(const char *type, char *priCont);//羊城通充值打印
	int print_bestpay_detail(const char *type);
	int print_traffic_detail(CPack pack,const char *type, char *field48, char *field63,int b63Idx);//交通罚款
public:


	CPrint();
	virtual ~CPrint();

	int print_np(char*car_owner,char*car_no,char*tel_phone);
	int print_customsign_detail(char*id_card,char*custom_no,char*tel_phone);
	int print_NHBank_detail(char*card_num,char*recv_name,char*kh_bank,char*tra_amo_buff);
	int print_sx_ysl(int flag);
	int print_ibss(char *telno,char *id_card,char *yyt_name);
	int print_selectcdma(int flag,char *yyt_name);
	int print_payonce_detail(const char *type);

	int print_ban_detail(int type=0);
	int print_Unionban_detail(int type=0);
	void set_in_par(CGlobal in_gbl, CPack in_pack); //设置传入参数
	void set_in_par(CGlobal in_gbl); //设置传入参数

	int print_detail(int flag, const char *type, char *field48="", char *field63="", char *priCont="");   //打印明细
	int print_fail(int iseny, char pMsg[2048], int cpyReprint);

	int Add_advertisement(const char *type_fee,char *advertisement,int *adv_num);
	
	void print_heibiao(int flag=0);
	int rdField63(CGlobal &out_gbl, int flag = 1);
	int rdField63(CPack pack,CGlobal &out_gbl, int flag = 1);
	static int getPrintPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int *prtCom, int *prtType);
    int print_bus_detail(char *bus_card_info, int flag);  //打印东莞公交充值

	int print_func(char *pMsg,int msg_len, int page=1, int cpyReprint=1,int acount=1,int current_count=0);
	int print_func_safe(char *pMsg, int msg_len, int cpyReprint=1,int acount=1,int current_count=0);	
	int print_dg_adsl_xc(const char *title);
	int print_new_dg_adsl_xc(const char *title);
	int MyCoolPayEasy_print_card_detail(const char *type);	 //打印售卡明细
	int print_SZDXTrade_detail(char*qf_amount,char*ss_amount);//深圳电信明细
	int print_credit_payment(char*credit_no);
	int print_province_traffic_detail(const char *type);
	int print_sell_phone(char *title);
	int print_telaffair_preaccept(int tradeType, const char* title);	//打印电信业务预受理明细
	int print_pep_detail(long print_fee, int print_type);					//	打印电信直充明细
	int print_huizhou_pep_detail (long print_fee, int print_type);					//	打印电信直充明细
	int print_game_recharge(const char* title, const char* message);
	int print_coach_ticket_detail(const char* title, const char* message);
	void printYctBill(char *date, char *normal_amount, char *plus_amount);
	int print_new_pep_detail(long print_fee, int print_type);//打印新版发票（开始是清远市开始使用这种发票）
	int print_xjx_detail(const char *type);//打印薪加薪资金划拨发票
	int print_yctthird_detail(const char *type, char *priCont);//羊城通充值第三代打印
	int print_sdm_detail(const char * type); //水电煤缴费
	int print_pre_detail(CPack pack,const char *type);	 //打印预付费明细 //for test
	int print_card_detail(CPack pack,const char *type);
	int print_pym_detail(CPack pack,const char *type);	
	int print_mer_detail(CPack pack,const char *type);	
	int print_CATV_detail(CPack pack,const char *type);
	int print_PICC_detail(CPack pack,const char *type);
	int print_film_detail(CPack pack,const char *type);	
	int print_ysq_detail(CPack pack,const char *type);

	int print_CATVDiscount_detail(CPack pack,const char *type);		
	int print_ADD_ADSL(CPack pack,const char *type);

	int print_chk_amo(CPack pack,const char *type, char *content);
	int reprint_bus_detail(CPack pack,const char *type)	;	//公交开卡补打印
	int print_province_traffic_detail(CPack pack,const char *type)	;	
	int print_bestpay_detail(CPack pack,const char *type);	//翼支付打印
	int print_yct_sellDetail(const char *type);	//羊城通读卡器销售
	int print_xjx_detail(CPack pack,const char *type);//打印薪加薪资金划拨发票
	int print_g3chargeCard_detail(const char *type); //打印3G流量卡充值
	int print_phonePay_detail(const char *type);//打印全国充值票据
	int print_phonePay_detailForGDYD(const char *type);//打印广东充值票据
	int print_carTax_detail(char *type); //打印车船税
	int print_dgtBus_detail(const char *type);
	int print_glQQ_detail(const char *type);
private:
	CGlobal gbl;
	CPack pack;
	int m_prtCom;
	int m_prtType;	
};

#endif // !defined(AFX_PRINT_H__A552D9AF_7618_43B1_9BA1_96BCC2977A51__INCLUDED_)
