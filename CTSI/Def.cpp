#include "Def.h"
#include "afxmt.h"
#include "WrLog.h"
#include "Tools.h"
#include "Connect.h"
#include "ADOConn.h"

#pragma comment(lib, "TSCTSI.lib")
#pragma comment(lib, "wsock32.lib")

char logon_random1[8+1];	//logon返回前8位
char logon_random2[8+1];	//logon返回后8位，银联签到key

int logon_flag = 0;		//签到标志，初始为0，一次有效
int yct3_initsign_flag = 0;		//羊城通充值第三代签到标志，初始为0，一次有效

Ado_Conn_Info g_adoConnInfo[DB_MAX];//数据库连接
Coninfo allConinfo[CON_MAX];//终端连接信息存储器

int syn_flag;  //同步标志
CCriticalSection cs_g_connStat;	//数据库连接资源锁
CCriticalSection cs_ONLINE_NUM;	//在线连接数锁
CCriticalSection cs_FILE;		//日志文件锁

CWrLog log;



// Epos_menu_conf menu_conf[MAX_MENU][MAX_MENU_ITEM];   //菜单配置表Epos_menu_conf
// int menu_cnt[MAX_MENU];						 //菜单组的菜单条目数
Print_adv print_adv[MAX_PRINT_ADV][MAX_PRINT_ADV_ITEM]; //数据库表Print_adv
int print_adv_cnt[MAX_PRINT_ADV];             //数组print_adv的个数
Bus_order bus_order[MAX_BUS_ORDER];                  //与公交交互的存储内存空间
Bus_key bus_key;                                     //存储公交签到后的充值密码以及解密密钥


//银行卡表Bank_card
vector<Bank_card> bank_card;	

//银行信息表Bank_info
vector<Bank_info> bank_info;	

//银行业务对应表Bank_busi
vector<Bank_busi> bank_busi; 

//数据库表Epos_group
vector<Epos_group> epos_group;

//数据库表Favorable_air_pay
vector<Favorable_air_pay> favorable_air_pay;

//终端组别与菜单的对应表epos_menu_get
vector<Epos_menu_get> epos_menu_get;	

//菜单组别表epos_menu_group
vector<Epos_menu_group> epos_menu_group;

//星空网盟游戏卡表Epos_pcard_conf
//vector<Epos_pcard_conf> pcard_conf;

//菜单编号表Epos_menu
vector<Epos_menu> epos_menu;	

//菜单组的菜单条目数
vector<int> menu_cnt;

//菜单配置表Epos_menu_conf
vector< vector<Epos_menu_conf> > menu_conf;   


//存储epos_tips的内容
vector<Epos_tips> epos_tips;

//存储sell_card_amo(电充卡金额)的内容
vector<Sell_Card_Amo> sell_card_amo;	

//存储数据库表epos_mobile的内容
vector<Epos_mobile> epos_mobile;

//存储数据库表epos_new_message
vector<Epos_new_message> new_message;
			
//存储数据库表epos_area_busi
vector<Epos_area_biz> area_biz;

//广东地区号码
int field_group[200];
vector<Mobile_Field_Area> gd_field;
int	epos_mobile_totalNum=0; //号码总数

//保存返回码对应错误信息
vector<Fail_Message> fail_message;

int sdm_group[200]; //水电煤业务
vector <SDM_AREA> sdm_city;
vector <BUSDATAS> sdm_datasVector; 



/*--------------------------程序配置---------------------------*/
char DB_CON_STR[500] = "";	//数据库连接字符串
char DB_CON_STR_EN[500]=""; //数据库连接串明文
char MERID[16+1]=""			; //机构
char TMNNUM[12+1]="";		;//机构接入终端
char CHANNELCODE[2+1];		 //机构代码
int TRADE_FLAG  = 0;		//交易类型，0: 测试，1: 生产，2:备份
int CARD_TYPE = 0;			//交易卡类型, 0x00：大卡，0x01：1号小卡（靠电池），0x02：2号小卡
int MAX_CONNECT_NUM = 10;	//数据库最大连接数
int DEBUG_LEVEL = 1;		//调试级别，越大输出日志越少
int FSK_DEBUG = 1;
CueInfo cueInfo;			//菜单前提示信息
int TRADE_STAT = 0;			//交易状态，0，正常，1，暂停
char CONNECT_SENTENCE[100]; //连接语句
char PE_IP[20];				//PE服务器地址
int  PE_PORT;				//PE服务器端口
int LISTEN_PORT;			//监听端口
char DGBUS_IP[20];           //东莞公交服务器端口
int DGBUS_PORT;					//东莞公交服务器端口

char DGTBUS_IP[20];				//东莞通公交IP
int DGTBUS_PORT;                 //东莞通公交端口
char DGT_Unitid[9];				//	东莞通营运单位

int YCT_VER;				//羊城通充值服务器版本号
char YCT_IP[20];			//羊城通服务器地址
int  YCT_PORT;				//羊城通服务器端口
char YCT_MONITOR_IP[20];    //羊城通监控服务器地址
int YCT_MONITOR_PORT;       //羊城通监控端口
char YCTYP_IP[20];			//羊城通月票服务器地址
int  YCTYP_PORT;			//羊城通月票服务器端口
char AIRPAY[4+1];			//空中充值业务代码
char pepwd_ip[16+1];		//老板密码服务器地址
char ecard_ip[16+1];        //E卡查询，话费详情查询服务器地址
char pepwd_port[5+1];		//老板密码服务器地址
char ecard_ser_port[5+1];	//e卡查询服务器端口
char prmVer[(4+1)*10];		//当前软终端程序版本号
char prmMD5[(32+1)*10];		//当前软终端程序版MD5
char excMAC[(16+1)*10];		//不作绑定用的机器码
TSFtpFiles ftpFiles;		//软终端远程更新信息
int DG_BUS_FLAG;			//东莞公交标志.0：无东莞公交，1:有东莞公交
int YiDong_Turn = 0;        //移动直充与售卡开关。0：直充；1：售卡
int LianTong_Turn = 0;      //联通直充与售卡开关。0：直充；1：售卡
char PRM_TITLE[100];		//程序标题
char CT10000_TERM[1000+1];	//允许开放用CT10000来使用充值付费卡的终端,00000000表示开放所有
char FORBID_BIZ[100];		//程序标题
int FORBID_TIME=0;			//不允许重复充值的时间(分钟)
int ONLINE_NUM = 0;			//当前连接数
int MAX_ONLINE_NUM = 100;	//最大连接限制数
int REFRESH_PORT;
char REFRESH_KEY[33];
char REFRESH_IP[16*100];
int service_stop; //日结开始时间
int service_start; //日结结束时间
char COACH_TICKET_IP[20];	//长途汽车票服务器地址
int  COACH_TICKET_PORT;		//长途汽车票服务器端口

char DIY_DCN_URL[30];		//内网DIY
char CS_DCN_URL[30];		//内网客服
char DIY_GBL_URL[30];		//公网DIY
char CS_GBL_URL[30];		//公网客服