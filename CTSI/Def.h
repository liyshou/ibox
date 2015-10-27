#ifndef __DEF_H_
#define __DEF_H_

#pragma   warning(disable:4786)

#include "afxmt.h"
#include "WrLog.h"
#include "Tools.h"
#include "Connect.h"
#include "ADOConn.h"
#include "TSCTSI.h"

#include <vector>
using namespace std;

typedef		unsigned long           DWORD;
typedef		unsigned short          WORD;
typedef		unsigned char           BYTE;

//creat by lihk 2006-6-23
#define		NUMERIC			0x01
#define		BITS			0x02
#define		ALPHANUM		0x03
#define 	ALPHANUMS		0x04
#define		TRACK			0x05

#define		FIXED		0x01
#define		LLVAR		0x02
#define		LLLVAR		0x03


#define		MTI			0xff
#define 	Bit0	    0
#define		Bit1		1
#define 	Bit2		2
#define		Bit3		3
#define		Bit4		4
#define		Bit5		5
#define		Bit6		6
#define 	Bit7		7
#define		Bit8		8
#define		Bit9		9
#define		Bit10		10
#define		Bit11		11
#define		Bit12		12
#define		Bit13		13
#define		Bit14		14
#define		Bit15		15
#define		Bit16		16
#define		Bit17		17
#define		Bit18		18
#define		Bit19		19
#define		Bit20		20
#define		Bit21		21
#define		Bit22		22
#define		Bit23		23
#define		Bit24		24
#define		Bit25		25
#define 	Bit26		26
#define		Bit27		27
#define		Bit28		28
#define		Bit29		29
#define		Bit30		30
#define		Bit31		31
#define		Bit32		32
#define		Bit33		33
#define		Bit34		34
#define		Bit35		35
#define		Bit36		36
#define		Bit37		37
#define		Bit38		38
#define		Bit39		39
#define		Bit40		40
#define		Bit41		41
#define 	Bit42		42
#define		Bit43		43
#define		Bit44		44
#define		Bit45		45
#define		Bit46		46
#define 	Bit47		47
#define		Bit48		48
#define		Bit49		49
#define		Bit50		50
#define		Bit51	    51
#define		Bit52		52
#define		Bit53		53
#define		Bit54		54
#define		Bit55		55
#define		Bit56		56
#define		Bit57		57
#define		Bit58		58
#define		Bit59		59
#define		Bit60	    60
#define		Bit61		61
#define		Bit62		62
#define		Bit63		63
#define		Bit64		64

#define ORDER_LEN 16
#define bbzero(m)        memset((m),0,sizeof(m))	//2006-10-24

#define ERROR_CODE -1	
#define RE_MNG_CARD 0x35	//重新刷卡标志
#define PE_OVER_TIME 90		//等待pe返回超时时间
#define EPOS_OVER_TIME 30	//epos超时时间
#define MENU_TIME_OUT 60000 //菜单超时时间(单位：毫秒)
#define MAXAMOLEN 15		//最大金额长度
#define MAXCOUNT 20
#define CON_MAX 65536
#define DB_MAX 1000			//数据库数组最大数1000
#define MAXTRY 50			//不断循环的最大循环次数

#define MNG_ONE 0			//一次刷卡输密码连续交费
#define MNG_MUL 1			//每次交费都刷卡
#define MNG_ONE_BIND 2		//一次刷卡输密码连续交费(可用绑定e卡或刷银行卡)
#define MNG_MUL_BIND 3		//每次交费都刷卡(可用绑定e卡或刷银行卡)
#define MNG_ONE_BIND_E 4	//一次刷卡输密码连续交费(用绑定e卡)
#define MNG_MUL_BIND_E 5	//每次交费都刷卡(用绑定e卡)

#define ONLY_MNG 0			//只刷卡
#define ONLY_INPUT 1		//只输入卡号
#define MNG_OR_INPUT 2		//刷卡或输入


#define HOTKEY1 0x05		//快捷键20071124
#define HOTKEY2 0x06
#define HOTKEY3 0x07
#define HOTKEY4 0x08
#define PAYTONETRADE 0x09


//E卡转帐与充值的业务代码(有新的业务代码须添上)
#define ECARDTRADE "[0301][0302][0075][0074][0083][0221][0109][0268][0339][0340]"

extern char logon_random1[8+1];	//logon返回前8位
extern char logon_random2[8+1];	//logon返回后8位,银联签到key
extern int logon_flag;
extern int yct3_initsign_flag;//羊城通第三代签到标识

extern int syn_flag;  //同步标志
extern CCriticalSection cs_g_connStat;	//数据库连接资源锁
extern CCriticalSection cs_ONLINE_NUM;	//在线连接数锁
extern CCriticalSection cs_FILE;		//日志文件锁

extern CConnect con;//连接类
extern CWrLog log;

//包结构体
typedef struct
{
	unsigned char	Bit_Name;
	unsigned char	Type;		//字符类型
	unsigned char	Format;		//长度类型
	WORD	Max_Length;
	WORD	Real_Length;
	unsigned char	*Rev_Data;
	unsigned char	*Send_Data;
}Packet_Struct;



//东莞公交包结构体
typedef struct
{
	 char	Type;		//字符类型
	 char	Format;		//长度类型 固定长度还是边长
 	 int	Max_Length; //最大长度
	 int	Real_Length; //实际长度
	 char	*Rev_Data; //接收数据
	 char	*Send_Data; //发送数据
	 char    name[100]; //变量名称
	 char   optionFlag;//是否可选
	

}Pack_dgt_Struct;




//打印明细结构体
typedef struct _tagPrnDetail
{
	int nMonCnt;	//交费的月份数
	struct MonDetail
	{
		int nCnt;			//明细数目
		char tmFrom[15];	//交费起始日
		char tmTo[15];		//交费结束日
		char month[7];
		struct Content
		{
			char type[40];		//条目
			char amo[MAXAMOLEN];		//金额	
		};
		char detail98Amo[13];	//电信发票修改20080129	
		char detail98Name[11];
		Content cnt[MAXCOUNT];	//明细，每月内容
	};
	MonDetail monDetail[6];		//各个交费时段的明细
}PrnDetail,*pPrnDetail;


//有线手册号查询返回结果,2006-11-21 by lihk
typedef struct _tagCATV_Struct1
{
	int nCnt;  //手册明细数
	int add_len;	//地址长度
	char address[40+1];	//地址

	struct Content
	{
		int user_name_len;		//用户名长
		char user_name[60+1];	//用户名
		char amount[12+1];		//应收金额
		char remain_amount[12+1];//剩余预存
		int card1_len;			//卡号1长(交费用)
		char card1[20+1];		//卡号1
		int card2_len;			//卡号2长(显示用)
		char card2[20+1];		//卡号2
		/*-------珠江宽频劲速套餐--------*/
		char stat[3+1];			//用户状态
		char remark[99+1];		//备注
	};
	Content cnt[MAXCOUNT];	//手册明细
}CATV_Struct1;


//有线身份证查询返回结果,2006-11-21 by lihk
typedef struct _tagCATV_Struct2
{
	int nCnt;	//手册号个数
	struct Content
	{
		int manual_num_len;   //手册号长度
		char manual_num[11+1];	//手册号
		int add_len;	//地址长度
		char address[40+1];	//地址
	};
	Content cnt[MAXCOUNT];	//手册号信息
}CATV_Struct2;

//车船税 账单查询结构体 

typedef struct _tagCarTaxBills_Struct  
{
	int nCnt;	//账单数
	struct CarTaxDATAS
	{

	char CUSTOMERNAME[64+1];//车主姓名
	char BILLBARCODE[200+1]; //车船税流水号
	char BILLNO[200+1]; //总税额
	char BILLMONTH[64+1]; //年份
	char BILLBATCH[12+1]; //抄次
	char CONTRACTNO[32+1]; //应征金额
	char BILLDELAY[12+1]; //滞纳金额
	char BALANCE[12+1]; //罚款金额
	int  LASTPAYDATE;   //是否补缴

	};
	CarTaxDATAS cardTaxDatas[MAXCOUNT];
	
}CarTaxBills_Struct; //车船税账单



//提示参数
typedef struct _tagCueInfo
{
	int  flag1;
	char info1[450+1];
	int  flag2;
	char info2[450+1];

}CueInfo, *PCueInfo;


//打印模板
typedef struct _tagPrintTemp
{
	char date[11];		//日期
	char tim[9];		//时间
	char count[15];		//计数
	char name[300];		//用户名
	char phone[50];		//号码
	char card[30];		//卡号
	char serial[20];	//流水号
	char stan[30];		//参考号
	char month[31];		//交费月
	char type[60];		//业务类型 2012-01-31 长度修改为60
	char total[30];		//合计
	char amo[30];		//实扣
	char detail98Amo[13];	//电信发票修改20080129	
	char detail98Name[11];
	char capsTotal[50];	//合计（大写）
	char pos[9];		//终端号
	int  n;				//明细中"\n"的个数
	char content[1024];	//打印明细

}PrintTemp, *PPrintTemp;


//交行查询返回明细
typedef struct _tagBanDetail
{
	char card[25];		//卡号
	char traDate[25];	//日期
	char traTime[25];	//时间
	char amount[13];	//金额
	char serial[7];		//流水
	char peStan[13];	//pe系统参考号
	char uniStan[13];	//银联系统参考号

}BanDetail, *PBanDetail;

//监控明细20071224
typedef struct _tagMonitorDetail
{
	char posNum[8+1];	//终端号
	char serial[6+1];	//终端流水
	char callTel[32+1];	//来电号码
	char traType[2+1];	//交易类型
	char stat[2+1];		//交易状态
	char rspCode[2+1];	//返回码
	char corrCode[9+1];	//bit62错误码
	char feeType[4+1];	//业务代码
	char amount[12+1];	//交易金额
	char bank[6+1];		//银行代码
	char card[30];		//交易卡号
	char traNum[30];	//交易号码
	char stan[13];		//系统参考号

}MonitorDetail, *PMonitorDetail;


/*==========================*/

//数据库表bank_card结构
typedef struct
{
	char card[10];  //卡号前缀
	int card_len;   //卡长
	char bank[6+1];  //银行代码
	int track_no;  //磁道
	int offset;    //偏移量
}Bank_card;
//数据库表bank_info结构
typedef struct
{
	char bank[6+1]; //银行代码
	int key; //密匙组数
}Bank_info;

//数据库表bank_busi结构
typedef struct
{
	char bank[6+1];  //银行代码
	char biz_type[4+1];  //业务代码
	char acc_area[4+1];//终端归属地 0000:全部 其它:各地市
	int pos_type;    //终端类型: 0:全部; 1:EPOS; 2:IPOS
}Bank_busi;

//数据库表epos_group结构
typedef struct
{
	int tgroup;  //组号
	int group_type; //菜单组别
	char welcome[500+1];//商户广告语
	char update_info[300+1];//终端更新提示语
	int eCard;  //是否支持e卡
	int phs_ticket; //付费易卡是否打印发票
	int get_card;  //刷卡标志
	int DianXin_Turn;       //电信直充与售卡开关。0：直充；1：网厅
}Epos_group;

typedef struct
{   
	char area[4+1];
	char biz_type[4+1];
	char name[30+11];
	char info[300+1];
	int amount;
}Favorable_air_pay;

//数据库表epos_group结构
typedef struct
{
	int tgroup;		//终端组号
	int asktype;	//请求类型
	char bank[6+1];	//银行机构号
	int menu_group;	//菜单组号
}Epos_menu_get;

//数据库表epos_group结构
typedef struct
{
	int menu_group;	//菜单组号
	char menu_ver[8+1];
	int mbl_flag;
}Epos_menu_group;

//数据库表epos_menu结构
typedef struct
{
	int menu_id; //菜单编号
	char name[30]; //菜单名
	char info[200+1];//菜单信息
}Epos_menu;

//数据库表epos_menu_conf结构
typedef struct
{
	int menu_group; //菜单组号
	int parent;	    //父菜单编号
	int currentm;	//当前菜单编号
	int child;		//子菜单编号
}Epos_menu_conf;

//数据库表epos_pcard_conf结构
// typedef struct 
// {
// 	char card_type[12+1]; //游戏卡类型编号
// 	char card_name[30]; //游戏卡名称
// 	char card_price[12+1];	//游戏卡价钱
// }Epos_pcard_conf;

//车辆违章查询返回结构
typedef struct
{
	char paper_type[4+1]; //文书类别
	char paper_num[16+1];//文书号
	char data[14+1];//违法日期
	char behavior[32+4];//违法行为
	char area[32+4];//违法地点
//	int tag;//0：未交罚 1：已交罚
	
}car_paper_info;


typedef struct  
{
	int fee_group; //广告语组号
	char fee_type[4+1]; //相关联的业务编号
	char advertisement[1024];//广告语
}Print_adv;

typedef struct  
{
	SOCKET sd;//端口
	u_char s_b1;
	u_char s_b2;
	u_char s_b3;
	u_char s_b4;
}Coninfo;

typedef struct  
{
	char phone_num[11+1];
	char phone_price[12+1];
	char yx_date[10+1];
	char phone_message[200+1];
	int flag;//0未用1已用
}CDMA_INFO;

typedef struct  
{
	int flag;//0:未使用1:已占用2:处理中3:处理完毕
	char stan[12+1];//流水号
	char data[1024];//数据
	time_t inTime;//写入内存的时间
	int data_len;
}Bus_order;

typedef struct  
{
	char bus_charge_key[8+1];//充值密码
	char bus_discharge_key[16+1];//解密密钥
}Bus_key;

typedef struct  
{
	char pos_flag[1+1];//0:适合所有；1:适合EPOS；2:适合IPOS
	int menu_id;
	char tips[300+1];
	char return_flag[1+1];//0:不返回主菜单；1:返回主菜单
}Epos_tips;

typedef struct  
{
	char biz_type[4+1];		//业务代码
	char card_type[4+1];	//子业务代码
	long amount;			//电信充值付费卡面值
}Sell_Card_Amo;

typedef struct  
{
	char mobile_field[3+1];//手机号码段
	int mobile_owner;//1：电信；2：移动：3：联通
}Epos_mobile;


typedef struct  
{
	char AREACODE[6+1];//水电煤业务开通地区
	char DX_CODE[4+1]; //电信地区号码
	char AREA_NAME[60]; //地区名称
	char PARENT_CODE[6+1]; //父亲地区代码
	char PARENT_NAME[60+1];//父亲地区名称
	int  AREA_FLAG; //地区代码标志 1为一级，2为2级
	char SHORTNAME[60];//地区简称

}SDM_AREA; //煤开通地水电区


typedef struct _tagsdmBills_Struct  
{
	int nCnt;	//账单数
	char payment_name[100]; //收费单位

	struct billDatas
	{
		char CUSTOMERNAME[64+1]; //客户姓名
		char BILLBARCODE[200+1]; //条形码
		char BILLNO[200+1]; //交费单号
		char BILLMONTH[10+1]; //账期
		char BILLBATCH[12+1]; //抄次
		char CONTRACTNO[32+1];//合同号
		char BILLAMOUNT[12+1]; //缴费金额
		char BILLDELAY[12+1]; //滞纳金
		char BALANCE[12+1]; //账户余额
		char BILLDATE[14+1];//开帐日期
		char LASTPAYDATE[14+1]; //最后缴费日期
		char BILLSTATUS[3+1];//账单状态
		char PASSWORD[64+1]; //账单密码
		char THIRDCODE[8+1]; // 第三方代码
		
	};

	billDatas BillDetails[MAXCOUNT];
	
}sdmBills_Struct,*SDM_BILLS_DATA; 


typedef struct  
{
	char PAYMENTCODE[30+1];//水电煤交费单位编码 至少18位
	char BUSCODE[3+1]; //业务代码 001:水；002：电；003：煤 004：暖气
	char PAYMENTNAME[128+1]; //交费单位名称 如xx水务公司
	char  BUSNAME[64+1]; //水费或者电费
	
}BUSDATAS  ; //水电煤交费单位信息




typedef struct  
{
	char title_message[30+1];//标题提示栏
	char sys_new_message[1024];//系统更新信息
	int my_group;//所属组别
}Epos_new_message;


typedef struct
{
	char stan[16+1];			//系统参考号
	char txn_date[8+1];			//交易日期
	char txn_time[6+1];			//交易时间
	char biz_type[4+1];			//业务代码
	int  status;					//交易状态
	char term_id[8+1];			//终端号
	char term_serial[6+1];		//流水号
	char bank[8+1];				//银行代码
	char card_no[20+1];			//银行卡号
	char amount[12+1];			//交易金额
	char yct_serial[16+1];		//羊城通交易流水号
	char pki_card[8+1];			//管理卡逻辑卡号
	char ticket_no[16+1];		//羊城通票卡卡号
	char ticket_amo_bef[12+1];	//羊城通票卡充值前余额
	char ticket_amo_aft[12+1];	//羊城通票卡充值后余额
	//char pki_pin[7];			//读卡器pin码
	char result_message[1024];	//索天科技版本读卡器所需数据

	/**************************第三代羊城通新增变量*********************/
	
	char ticket_physicalNo[16+1];//羊城通票卡物理卡号        
	BYTE SKEY[32];                 //加密密钥skey 使用AES 加密 
	BYTE SHID[16+1];			  //羊城通握手流水号8个字节
	int  yct_sequence_NO;			//羊城通报文序号	
	char downLimit [1+1];		//钱包下限
	char upperLimit[3+1];		//钱包上限
	char yct_stan[16+1]	;		//羊城通充值第三代系统流水号
	char org_yctstan[12+1];     //原充值系统流水号（充值撤销用）
	
	BYTE card_status ;			//卡状态aa64交易时间
	char SW1SW2[4+1];			//卡返回状态码
	
	BYTE BLACKLIST_CARDINFO[32+1];		//黑名单卡信息

	char trader_id[16+1]; //商户编号
	char trader_name[40+1];//商户简称
	char out_logicalcard[16+1] ;// 转出逻辑卡号
	char note [128+1]; //成功备注
	char tradeTime[14+1]; //充值时间
	char TAC[8+1]; //充值认证号
	char yct_termid[12]; //羊城通终端号
	int  trade_count; //羊城通票卡交易计数
	char trade_type[32+1];//交易类型
	char charge_authcode[8+1];//充值凭证号
	char result_message_third[1024];	//提交报文
	int com;
	int comType;
	char SignIn_ServerTime[30+1];//签到成功时的后台时间
	char SAK[1+1];//SAK
    char random_num[16+1];//随机数
	int yct_ack; //是否提交commit 如果没有执行commit 则需要重新发送commit
	/*****************羊城通卡间余额转移*******************/
	/*  added by Wudp 20120607                            */
	/******************************************************/
	char extension_stan[16+1];		//羊城通充值第三代扩展应用流水号
	char transfer_stan[16+1];       //羊城通充值第三代卡转值流水号
	//转出卡
	char out_logicardno[16+1];		//转出卡逻辑卡号
	char out_balance[12+1];	        //转出卡余额
	char out_balance_bef[12+1];	    //转出卡原余额
	char out_balance_aft[12+1];	    //转移后转出卡余额
	char out_downLimit [1+1];		//转出卡钱包下限
	char out_upperLimit[3+1];		//转出卡钱包上限
	char out_cardappinfo[40+1];     //转出卡应用信息

	//转入卡
	char in_logicardno[16+1];		//转入卡逻辑卡号
	char in_balance[12+1];	        //转入卡余额
	char in_balance_bef[12+1];	    //转入卡原余额
	char in_balance_aft[12+1];	    //转移后转入卡余额
	char in_downLimit [1+1];		//转入卡钱包下限
	char in_upperLimit[3+1];		//转入卡钱包上限
	char in_cardappinfo[40+1];		//转入卡应用信息
	/*****************羊城通卡间余额转移******************/
    
    


	/**************************第三代羊城通新增变量*********************/

}YCT_DATA, *PYCT_DATA;


typedef struct
{
	char ticket_type;			//月票类型 01公交 02地铁 03公交地铁
	char birth_date;			//套餐生效日期 01当月 02下月
	char railway_type;			//地铁套餐类型 01限次
	char railway_num;			//地铁限次套餐的次数 HEX
	char bus_type;				//公交套餐类型 01限次 02限线
	char bus_num;				//公交限次套餐的次数 HEX
	char bus_way1[5+1];			//公交限线路1
	char bus_way2[5+1];			//公交限线路2
	char bus_way3[5+1];			//公交限线路3
}YCT_DETAIL, *PYCT_DETAIL;

typedef struct  
{
	int biz_flag;		//业务类型：1:电信空充（定额无查询）;2:移动空充;3:联通空充（联欣）;4:联通空充（全省）;5:电信空充（带查询输金额）;6:售卡;7:Q币充值;8:交通罚款 等等不断扩充中
	char area[4+1];		//物理地区的区号
	char accarea[4+1];	//帐户地区的区号（若物理地区的区号不为0000，则帐户地区的区号也不能为0000）
	char biz_type[4+1];	//业务代码
	int status;
}Epos_area_biz;

typedef struct  
{
	CADOConn adoConn;	//一个数据库连接
	int connStat;		//该连接状态：0：未用，1：已用，2：失效
	DWORD threadID;		//占用该连接的线程ID
	char termId[8+1];	//占用该连接的终端号
	char connDate[8+1];	//占用该连接时的日期
	char connTime[6+1];	//占用该连接时的时间
}Ado_Conn_Info;

typedef struct  
{
	char batch_value[20+1];
	char pay_money[12+1];
	int pay_count;
}Batch_Value_Info;


typedef struct  
{
	char mobile_field[8+1];
	char area[20+1]; //城市
	char province[20+1]; //省份
	int field_len;

}Mobile_Field_Area;

typedef struct
{
	char phone_head_no[11+1];
}PHONE_HEAD_NO;

//电信直充信息
typedef struct
{
	char username[40];
	char mainphone[16];
	char phone[16];
	char month[9];
	int item_num;
	int item_index[20];
	int print_flag;
}Bill;


//东莞通卡片基础信息
typedef struct
{
	char cardType[2+1];//卡片类型
	char cardInfo[255]; //卡片信息
	char otherData[255]; //其他信息(A卡返回的)
	char CItyCode[2+1]; //城市编码
	char compay_code[2+1]; //发卡方单位
	char busi_code[2+1]; //行业代码
	char deposit[2+1]; //押金
	char appSerial[8+1];//应用序号
	char appFlag[1+1]; //应用标志 
	char cardParent_type[1+1]; //卡主类型
	char cardChild_type[1+1]; //卡子类型
	char openDate[4+1]; //发卡日期
	char effectiveDate[4+1]; //卡有效日期
	char deviceInfo[6+1]; //发行设备信息
	char appVer[2+1];//应用版本信息
	char cardMny[12+1]; //卡片余额
	char CARDSEQ[6+1]; //卡片交易流水
	char KEYVER[2+1]; //卡片密钥版本
	char ALGIND[2+1]; //卡片算法标志
	char CARDRAND[8+1]; //卡片随机数
	char MAC1[8+1]; //充资返回信息MAC1
	char MAC2[8+1]; //钱包加值需要的 MAC2
	char cardShowId [16+1];  //十进制表示的卡号

	char CHARGE_DIVDATA[18+1]; //分散因子(充值的时候的分散因子)
	char CARDSALE_DIVDATA[18+1]; //售卡的时候的分散因子
	char CARDID[8+1]; //卡内号
	char AUTHSEQ[10+1]; //密钥授权流水号
	char LIMITEAUTHSEQL[10+1]; //额度授权流水号
	char DICMAC[8+1];
	char CARDCNT[6+1]; //卡内计数器
	char CARDVERNO[2+1]; //卡内版本号
	char EDCARDID[16+1]; //无用
	char connectCity[2+1];
	char tradeMny[8+1];//交易余额
	char tradeAfterMny[8+1]; //交易后余额
	char tradeBeForeMny[8+1]; //交易前余额
	char tradeTime[6+1]; //交易时间123012
	char tradeDate[8+1]; //交易日期 20121010
	char TAC[8+1]; //TAC
	char LoadSEQ[4+1]; //充资交易流水号
	char lastPosSeq[9+1]; //上笔交易pos流水号
	char posSeq[9+1]; //交易终端流水号
	char sysTime[14+1]; //20140123102412 中心时间
	char MESSAGEDATETIME[14+1]; //报文发送时间
	char APDU[70]; //卡操作请求的时候会使用到
	char cardOperType[2+1]; //卡片操作类型
	char sale_type[2+1]; //卡销售方式
	char BATCHNO[6+1]; //批次号
	char KEYSET[64+1]; //密钥
	char SETDATE[8+1]; //结算日期
	char KEEP[32+1]; //卡操作请求的时候的请求流水号
	char resubmitField[1024]; //东莞提交数据
	char openTime[6+1]; //开卡时间

}DGTCard,*PDGTCard;


//东莞通确认补发需要的数据目前主要用于东莞通开卡 add by liuxiao 

typedef struct
{
	char TMNNUMNO[12+1]; //客户终端号
	char SYSTEMNO[20]; //系统参考号
	char TRANSTYPE[4]; //业务类型
	char TAC[2+1];//交易押码
	char APTRANSSEQ[16]; //原交易流水号
	char TRADETIME[15]; //受理时间
	char EDCARDID[17]; //操作主卡卡号
	char CARDID[17]; //卡内号
	char CARDCNT[7]; //卡计数器
	char CARDMKND[3]; //卡主类型
	char CARDSKND[3]; // 卡子类型
	char CARDMODEL[2]; //卡型
	char SALEMODE[3]; // 售卡方式
	char DEPOSIT[9]; //押金
	char BEFBALANCE[9]; // 交易前余额
	char TXNAMT[9]; //交易金额
	char CARDVALDATE[9]; //	卡有效期
	char CITYCODE[5]; //  卡内城市代码
	char CARDVERNO[3]; // 	卡内版本号
	char BATCHNO[7]; //批次号
	char AUTHSEQ[19]; //授权流水号
	char LIMITEDAUTHSEQL[11]; //额度授权流水号
	char KEYSET[65]; //密钥

}DGTResubmitInfo,*PDGTResubmitInfo;



//东莞通M1卡卡片基础信息

typedef struct
{
	char bus_card[8+1]; //公交卡号
	char bus_amt[8+1];
	char bus_card_type[2+1]; //应用子卡类型
	char check_date[6+1];//年审日期
	char psam[16+1]; //psam卡号
	char recogise_code[8+1]; //认证码
	char dkq_code[4+1];
	char card_parent_type[2+1];
	char card_child_type[2+1];
	char M1CardCSN[8+1];
	char bus_showCard[16+1];
	char city_code[4+1];
	char cardVadate[8+1]; //有效期
	char appSerial[6+1]; //应用卡编号
	char openDate[8+1]; //开卡日期
	char openTime[4+1]; //开卡时间
	char deposit[6+1]; //押金
	char cardStatus[2+1]; //卡片状态


}DGTM1Card,*PDGTM1Card;


//东莞公交充值记录 最近4笔交易
typedef struct _tagdgtChargeTrades 
{
	int nCnt;	//充值记录数
	
	struct cardChargeDetails
	{
		char cardSEQ[4+1];
		char cardID[8+1];
		char tradeAfterMny[6+1];
		char tradeMny[8+1];
		char  tradeType[2+1];
		char  POS_ID[12+1];
		char trade_date[8+1];
		char trade_time[6+1];
		char TAC[8+1];	
	};
	
	cardChargeDetails cardChargeDetail[10];
	
}dgtChargeTrades,*pdgtChargeTrades; 


//东莞所以充值及消费公交充值记录最近10笔交易
typedef struct _tagdgtALLTrades
{
	int nCnt;	//充值记录数
	
	struct cardALLTradeDetails
	{
		char cardSEQ[4+1];
		char cardId[8+1]; //卡号
		char limitMny[6+1];// 透支限额
		char tradeMny[8+1]; //交易金额
		char tradeType[2+1]; //交易类型
		char POS_ID[12+1]; //清算终端
		char trade_date[8+1]; //交易日期
		char trade_time[6+1]; //交易时间
	};
	
	cardALLTradeDetails cardAllTradeDetail[10];
	
}dgtALLTrades,*pdgtALLTrades; 


//东莞公交签到信息
typedef struct  
{
	char POSID[12+1]; //清算设备代码
	char KEYSET[32+1]; //密钥信息
	char SAMID[16+1]; //SAM卡号
	char SAMAUTHINFO[16+1]; //SAM卡授权信息
	char EDAUTHINFO[16+1]; //ED卡授权信息
	char SETTDATE[8+1]; //结算日期
	char BATCHNO[7+1]; //批次号
	char SYSDATETIME[14+1]; //中心时间
	char AUTHCODE[24+1]; //授权认证码（M1）
	int  sign_status;

	char TOTALSVNUM[10+1]; //本批次累计售卡充资交易总笔数
	char TOTALSVAMT[10+1]; //本次累计充值交易总金额（分）
	char TOTALSALEDEP[10+1]; //本次累计售卡交易押金总金额（分）
	char LOG_SEQ[10+1]; //签到流水

 
}DGTSignInfo,*PDGTSignInfo; 


//东莞公交终端信息
typedef struct  
{
	char POSID[12+1]; //清算设备代码
	char BUSI_ID[12+1]; //对应企业账户终端
	char POSKEY[32+1]; //终端私钥
	
}DGTDeviceInfo,*PDGTDeviceInfo; 


//广禄qq
typedef struct  
{
	char PRODUCTCODE[10]; //产品编码
	char ACCTCODE[100]; //账户号码
	char POSKEY[32+1]; //终端私钥
	char RECHNUM[10]; // 充值数量 产品编码为0时单位为：个，其余单位为：月
	char RECHAMOUNT[12+1]; //充值金额
}GLQQPayInfo,*PGLQQPayInfo; 


typedef struct  
{
	char phone[16];
	char month[9];
	char item_name[30];
	char fee[10];
	int flag;
}Bill_Item;

//水电煤业务
typedef struct
{
	char username[40];
	char mainphone[16];
	char phone[16];
	char month[9];
	int item_num;
	int item_index[20];
	int print_flag;
}SDMBill;

//水电煤业务
typedef struct  
{
	char phone[16];
	char month[9];
	char item_name[30];
	char fee[10];
	int flag;
}SDM_Bill_Item;

//返回码信息
typedef struct 
{
	char rsp_id[12];
	char party_code[7];
	char bit39_rsp[3];
	char bit62_rsp[3];
	char busi_desc[300];
}Fail_Message;

/*----------------数据库宏----------------*/

#define MAX_PRINT_ADV 50 //最大广告组数
#define MAX_PRINT_ADV_ITEM 300 //最大广告项数
#define MAX_MENU  100        //最大菜单组数
#define MAX_MENU_ITEM  1000  //最大菜单项数
#define MAX_HOT_KEY  6		//最大快捷键数20071124

#define MAX_CDMA_INFO 100  //最大CDMA手机数
#define MAX_BUS_ORDER 800  //与公交交互内存空间最大数


#define MAX_BATCH_VALUE_INFO 25 //最大Batch_Value_Info个数

#define MAX_BILL_NUMBER 30	//电信直充话费查询账单数量
#define MAX_BILL_ITEM_NUMBER 50  

//薪加薪交易状态
#define XJX_TRANS_SUCC             "0"  //交易成功
#define XJX_TRANS_FAIL             "1"  //交易失败
#define XJX_NOT_CHECKED            "2"  //交易未审核
#define XJX_LLCHECK_PASSED         "3"  //交易已通过二级审核
#define XJX_LLCHECK_NOTPASS        "4"  //交易二级审核不通过
#define XJX_LLLCHECK_PASSED        "5"  //交易已通过三级审核
#define XJX_NOT_PASS               "6"  //交易三级审核不通过
#define XJX_TRANS_REFUSE           "7"  //交易拒绝（交易金额超过三级审核金额）

/*---------------羊城通销售---------------*/
#define YCT_CARD_UNITPRICE         25   //羊城通卡单价（元/张）
#define YCT_CARDDELIVERY_COSTS     8    //羊城通配送快递费（单位元，仅在购买数量小于10张时使用）
#define YCT_PACKAGE_UNITPRICE      1388 //羊城通套餐单价


/*----------------------------------------*/
//extern Epos_menu_conf menu_conf[MAX_MENU][MAX_MENU_ITEM];   //菜单配置表Epos_menu_conf
//extern int menu_cnt[MAX_MENU];								//菜单组的菜单条目数
extern Print_adv print_adv[MAX_PRINT_ADV][MAX_PRINT_ADV_ITEM]; //数据库表Print_adv
extern int print_adv_cnt[MAX_PRINT_ADV];                    //数组print_adv的个数
extern Bus_order bus_order[MAX_BUS_ORDER];                  //与公交交互的存储内存空间
extern Bus_key bus_key;                                     //存储公交签到后的充值密码以及解密密钥

//数据库表Bank_card
extern vector<Bank_card> bank_card;

//数据库表Bank_info
extern vector<Bank_info> bank_info;

//数据库表Bank_busi
extern vector<Bank_busi> bank_busi;

//数据库表Epos_group
extern vector<Epos_group> epos_group;

//数据库表Favorable_air_pay
extern vector<Favorable_air_pay> favorable_air_pay;

//终端组别与菜单的对应表epos_menu_get
extern vector<Epos_menu_get> epos_menu_get;	

//菜单组别表epos_menu_group
extern vector<Epos_menu_group> epos_menu_group;		

//星空网盟游戏卡配置表Epos_pcard_conf
//extern vector<Epos_pcard_conf> pcard_conf;  

//菜单编号表Epos_menu
extern vector<Epos_menu> epos_menu;		

//菜单组的菜单条目数
extern vector<int> menu_cnt;

//菜单配置表Epos_menu_conf
extern vector<vector<Epos_menu_conf> > menu_conf;   
							

//存储epos_tips的内容
extern vector<Epos_tips> epos_tips;
		
//存储sell_card_amo(电充卡金额)的内容
extern vector<Sell_Card_Amo> sell_card_amo;

//存储数据库表epos_mobile的内容
extern vector<Epos_mobile> epos_mobile;

//存储数据库表epos_new_message
extern vector<Epos_new_message> new_message; 
	
//存储数据库表epos_area_busi
extern vector<Epos_area_biz> area_biz;

//广东地区号码
extern int field_group[200];
extern vector<Mobile_Field_Area> gd_field;
extern int epos_mobile_totalNum;
//保存返回码对应错误信息
extern vector<Fail_Message> fail_message;

//水电煤业务
extern int sdm_group[200];
extern vector <SDM_AREA> sdm_area;
extern vector <BUSDATAS> sdm_datasVector; //水电煤业务信息


extern void wrMonitorLog(int type, int stat, int rspCode, CGlobal gbl, CPack pack);

/*--------------------------程序配置---------------------------*/
extern int TRADE_FLAG;		//交易类型，0: 测试，1: 生产
extern int MAX_CONNECT_NUM; //数据库最大连接数
extern int DEBUG_LEVEL;		//调试级别，越大输出日志越少
extern int FSK_DEBUG;
extern char DB_CON_STR[500];//数据库连接字符串
extern char DB_CON_STR_EN[500];//数据库连接字符串（用户名和密码为明文
extern char MERID[16+1];	//机构
extern char TMNNUM[12+1];	//机构接入终端号
extern char CHANNELCODE[2+1]; //机构代码
extern int CARD_TYPE;		//交易卡类型, 0x00：大卡，0x01：1号小卡（靠电池），0x02：2号小卡
extern CueInfo cueInfo;		//菜单前提示信息
extern int TRADE_STAT;		//交易状态，0，正常，1，暂停
extern char CONNECT_SENTENCE[100]; //连接语句
extern char PE_IP[20];				//PE服务器地址
extern int  PE_PORT;				//PE服务器端口
extern int LISTEN_PORT;				//监听端口
extern char DGBUS_IP[20];			//	东莞公交服务器IP
extern int DGBUS_PORT;				//东莞公交服务器端口

extern char DGTBUS_IP[20];
extern int DGTBUS_PORT;
extern char DGT_Unitid[9];			//东莞通运营

extern int  YCT_VER;				//羊城通服务器版本
extern char YCT_IP[20];				//羊城通服务器地址
extern int  YCT_PORT;				//羊城通服务器端口
extern char YCT_MONITOR_IP[20];				//羊城通监控服务器地址
extern int  YCT_MONITOR_PORT;				//羊城通监控服务器端口
extern char YCTYP_IP[20];			//羊城通月票服务器地址
extern int  YCTYP_PORT;				//羊城通月票服务器端口
extern char AIRPAY[4+1];			//空中充值业务代码
extern char pepwd_ip[16+1];			//老板密码服务器地址
extern char pepwd_port[5+1];		//老板密码服务器地址
extern char ecard_ip[16+1];         //E卡查询，话费查询地址
extern char ecard_ser_port[5+1];	//e卡查询服务器端口
extern char prmVer[(4+1)*10];		//当前软终端程序版本号
extern char prmMD5[(32+1)*10];		//当前软终端程序版MD5
extern char excMAC[(16+1)*10];		//不作绑定用的机器码
extern TSFtpFiles ftpFiles;			//软终端远程更新信息
extern int DG_BUS_FLAG;				//东莞公交标志.0：无东莞公交，1:有东莞公交
extern int YiDong_Turn;             //移动直充与售卡开关。0：直充；1：售卡
extern int LianTong_Turn;           //联通直充与售卡开关。0：直充；1：售卡
extern char PRM_TITLE[100];			//程序标题
extern char CT10000_TERM[1000+1];	//允许开放用CT10000来使用充值付费卡的终端,00000000表示开放所有
extern char FORBID_BIZ[100];		//程序标题
extern int FORBID_TIME;				//不允许重复充值的时间
extern int ONLINE_NUM;				//当前连接数
extern int MAX_ONLINE_NUM;			//最大连接限制数
extern int REFRESH_PORT;
extern char REFRESH_KEY[33];
extern char REFRESH_IP[16*100];
extern char COACH_TICKET_IP[20];	//长途汽车票服务器地址
extern int  COACH_TICKET_PORT;		//长途汽车票服务器端口
extern int service_stop; //日结开始时间
extern int service_start;//日结结束时间
extern Ado_Conn_Info g_adoConnInfo[DB_MAX];//数据库连接
extern Coninfo allConinfo[CON_MAX];//终端连接信息存储器

//extern char DIY_DCN_URL[30];	//内网DIY
extern char CS_DCN_URL[30];		//内网客服
//extern char DIY_GBL_URL[30];	//公网DIY
extern char CS_GBL_URL[30];		//公网客服

#endif