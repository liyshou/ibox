/****************************************************************
//天讯CTSI协议动态库
//天讯电信科技有限公司  http://www.payeasy.com.cn/
//版权所有 (C) 2008-2015
//修改日志: 
//2008-04-25 叶科军 创建  
****************************************************************/

/////////////////////////////////////////////////////////////////
//	TSCTSI.h
#ifndef __TSCTSI_H_KF20080101_

#define __TSCTSI_H_KF20080101_

#define SUCC 0
#define CANCLE_KEY 0x36		//取消
#define HOTKEY 0x37
#define HANG_UP_KEY -101	//挂机

#define MAX_DATA			1024		//接收的fsk最大有效数据
#define MENU_ITEM_MAXCOUNT	25			//每次发送菜单项的最大数目
#define MENU_ITEM_MAXLEN	30			//菜单项名称的最大长度
#define TITLE_MAXLEN		30			//菜单标题最大长度
#define MAX_CONT			30			//交易控制函数的输入最大项目
#define MAX_LOCAL_MENU		256			//更新菜单函数中最大的菜单项数
#define MAX_COLS			100			//软终端查询明细的最大列数
#define MAX_COLS_NAME_BUF	1024		//软终端查询明细的列名最大空间
#define MAX_INTERM_BUF		81920		//软终端查询明细的内容最大空间
#define MAX_TXN_LOG			1000		//软终端查询明细的最大记录条数

#define OVER_TIME			75			//超时时间
#define MSG_RETIME			5			//下发提示信息自动刷新时间, 单位（秒）

//输入模式
#define TS_MAG_CARD			0x01		//刷卡
#define TS_INPUT_CARD		0x02		//手输卡号
#define TS_MAG_INPUT_CARD   0x03		//刷卡并手输
#define TS_INPUT_PIN        0x04		//输入密码(显示*)
#define TS_INPUT_MATH       0x05		//数字 (按实际内容显示, 0~9)
#define TS_INPUT_LET		0x06		//字母
#define TS_INPUT_CH			0x07		//汉字
#define TS_INPUT_ALL_CHAR   0x08		//全字符
#define TS_INPUT_MONEY      0x09		//输入金额(按￥0.00格式显示)
#define TS_INPUT_DATE       0x0A		//日期
#define TS_INPUT_YYYYMM     0x0B		//年月
#define TS_INPUT_PWD		0x0C		//明文密码
//加密模式子参数
#define MAX_EXT_ENC			10			//扩展加解密算法最大数
#define EXT_ENC_1			1			//扩展加密算法1
#define EXT_DEC_2			2			//扩展解密算法2
//加解密模式
#define TS_NO_ENC			0						//不加密解密(默认)
//旧加密
#define TS_UNION			1*MAX_EXT_ENC			//银联流程
#define TS_DES_1			2*MAX_EXT_ENC			//1次DES
#define TS_DES_2			3*MAX_EXT_ENC			//上送随机数2次DES
#define TS_DES_2_DOWN		4*MAX_EXT_ENC			//下发随机数2次DES
//新加密
#define TS_DES_3			1*MAX_EXT_ENC			//3次DES加密, 加密改造
#define TS_DES_3_TEK_DEC	2*MAX_EXT_ENC			//使用TEK做3次DES解密
#define TS_DES_3_TEK_ENC	3*MAX_EXT_ENC			//使用TEK做3次DES加密
#define TS_EXT_ENC_1		4*MAX_EXT_ENC+EXT_ENC_1	//使用扩展加密算法1
#define TS_EXT_DEC_2		4*MAX_EXT_ENC+EXT_DEC_2	//使用扩展解密算法2
//新加密体系密匙号
#define TS_TMK_KEY			1			//TMK_KEY
#define TS_TPK_KEY			2			//TPK_KEY
#define TS_TAK_KEY			3			//TAK_KEY
#define TS_TEK_KEY			4			//TEK_KEY

typedef unsigned int	SOCKET;			//SOCKET类型
typedef	unsigned char	BYTE;			//BYTE类型

//fsk返回信息（指令集）
typedef struct _tagTSFskContent
{
	char title[TITLE_MAXLEN+1];			//发送用
	BYTE date[4+1];						//接收用
	BYTE tim[3+1];						//接收用

	BYTE posNum[8+1];					//终端号
	BYTE serial[3+1];					//交易流水
	int macCardType;					//算mac卡类型
	int macKeyIndex;					//mac密匙号
	BYTE macRandom[8+1];				//mac随机数
	BYTE mac[8+1];						//mac数据

	int msgType;						//消息类型
	int codeLen;						//操作码长度
	int codeNum;						//操作码集个数
	BYTE operCode[200];					//操作码指令集
	int dataLen;						//有效数据
	BYTE data[MAX_DATA];				//有效数据集
	BYTE enyFlag;						//加密改造, 新旧加密体系标志
}TSFskContent, *PTSFskContent;

//菜单结构体
typedef struct _tagTSMenuList
{
	char acTitle[TITLE_MAXLEN+1];		//菜单标题
	int  iTotalNum; 					//菜单总项数	
	char menuItem[MENU_ITEM_MAXCOUNT][MENU_ITEM_MAXLEN+1];//各菜单项名称

}TSMENULIST, *PTSMENULIST;


//发送信息结构体->单个信息结构体
typedef struct _tagSendContent
{
	char acTitle[80+1];					//提示字串 (小于80字符)		
	unsigned char uCommand;				//控制命令, 读取数据类型
	unsigned char uCount;				//采集次数, 默认为0, 1次采集
	
	unsigned char uDesMode;				//加密算法, 1银联流程, 2一次DES, 3上送随机数二次DES加密, 4下发随机数二次DES解密
	unsigned char uCardType;			//加密卡座号
	unsigned char uKeyIndex;			//密钥组号	
	unsigned char uKeyRand[8+1];		//密钥随机数
	
	unsigned char uTimeOut;				//等待用户输入超时时间, 单位:秒
	unsigned char uMinLen;				//输入最小长度
	unsigned char uMaxLen;				//输入最大长度
}SendContent;

//发送信息结构体
typedef struct _tagTSTradeInfoSend
{	
	short sendType;						//0表示系统下发的操作码，为1表示终端上送的结构体
	char acMainTitle[80+1];				//总提示字串 (小于80字符)
	unsigned char uTotalContents;		//输入总项数
	unsigned char uHasTradeID;			//交易流水号标志
	unsigned char uMacCardType;			//MAC卡类型
	unsigned char uMacKeyIndex;			//MAC计算使用的密钥组号	
	unsigned char uMacKeyRand[8+1];		//MAC计算使用的随机数
	SendContent Contents[MAX_CONT];		//各输入项信息
	
}TSTradeInfoSend, *PTSTradeInfoSend;

//返回信息结构体
typedef struct _tagTSTradeInfoRecv
{	
	struct RecvContent
	{
		unsigned char uMsgLen;			//返回数据的长度
		unsigned char uMsg[200+1];		//返回数据
		unsigned char uKeyRand[8+1];	//密钥随机数
	};
	unsigned char uTradeID[6+1];		//交易流水号
	unsigned char uTotalContents;		//返回总项数
	RecvContent Contents[MAX_CONT];		//各返回项信息
	
}TSTradeInfoRecv, *PTSTradeInfoRecv;


//短信
typedef struct _tagTSSendShortMsg
{
	char sendTime[14+1];				//发送时间 [20071001153020]
	char sendNum[20+1];					//发送方号码
	char content[140+1];				//短信息内容

}TSSendShortMsg, *PTSSendShortMsg;

//通知
typedef struct _tagTSSendInform
{
	char sendTime[16+1];				//发送时间 [20071001153020]
	char title[20+1];					//标题
	char content[140+1];				//通知内容

}TSSendInform, *PTSSendInform;


//帐单交易结果信息
typedef struct _tagTSBillResult
{	
	char sendTime[14+1];				//发送时间 [20071001153020]
	unsigned char billNo[16+1];			//帐单号
	unsigned char result;				//更新帐单状态的对应码 0x00：未支付；0x01：已支付(成功)；0x03：过期；0x04删除帐单

}TSBillResult, *PTSBillResult;

//交易结果信息
typedef struct _tagTSTradeResult
{	
	char sendTime[14+1];				//发送时间 [20071001153020]
	char title[20+1];					//标题
	char content[140+1];				//交易结果信息

}TSTradeResult, *PTSTradeResult;


//未支付账单
typedef struct _tagTSUnpayedBill
{
	unsigned char dispFlag;				//显示控制标志
	unsigned char billNo[16+1];			//帐单号
	char sendTime[14+1];				//帐单发送时间
	char endTime[12+1];					//帐单截止时间 [200708021111]
	char title[20+1];					//帐单标题
	char amount[12+1];					//帐单金额
	char merInfo[20+1];					//帐单商家信息
	char billDetail[80+1];				//帐单详细信息

}TSUnpayedBill, *PTSUnpayedBill;

//透传操作指令
typedef struct _tagTSIcReaderCommand
{
	int com;							//使用的串口
	int comType;						//串口类型	

	char acTitle[80+1];					//提示字串 (小于80字符)
	unsigned char uDownLen;				//下发数据的长度
	unsigned char uDownData[240];		//下发的数据, 与提示串的总长度不超过 240 字节

	unsigned char uUpLen;				//上送数据的长度
	unsigned char uUpData[240];			//上送的数据

	unsigned char uTradeID[6+1];		//交易流水号

}TSIcReaderCommand, *PTSIcReaderCommand;


//HID设备透传结构体
typedef struct _tagTSHID_IcReaderCommand
{

	char acTitle[80+1];					//提示字串 (小于80字符)
	unsigned char uDownLen;				//下发数据的长度
	unsigned char uDownData[240];		//下发的数据, 与提示串的总长度不超过 240 字节

	unsigned char uUpLen;				//上送数据的长度
	unsigned char uUpData[240];			//上送的数据

	unsigned char uTradeID[6+1];		//交易流水号

}TSHID_IcReaderCommand, *PTSHID_IcReaderCommand; 



//终端参数
typedef struct _tagTSPosPar
{
	unsigned char num;					//参数个数
	struct ParCont
	{
		unsigned char parId;			//参数号
		unsigned char parLen;			//参数长度
		BYTE parCont[64];				//参数内容
	};
	ParCont parCont[MAX_CONT];

}TSPosPar, *PTSPosPar;


//操作信息
typedef struct _tagTSOperInfo
{
	unsigned char num;					//模板数
	struct temp							//模板
	{
		unsigned char showType;			//显示类型
		unsigned char len;				//内容长度
		char content[85];				//显示内容
	};
	temp tempInfo[MAX_CONT];
}TSOperInfo;

//提示信息
typedef struct _tagTSCueInfo
{
	unsigned char num;					//信息记录数
	struct Info
	{
		unsigned char index;			//提示信息索引
		unsigned char flag;				//信息操作标志, '0'表示不可用, '1'表示可用
		TSOperInfo operInfo;			//信息内容模板
	};
	Info info[MAX_CONT];

}TSCueInfo, *PTSCueInfo;

//安全下发信息
typedef struct _tagTSOutPutMsg_S
{
	unsigned char uCardType;			//卡类型
	unsigned char uKeyIndex;			//密匙号
	unsigned char uDesMode;				//加密算法
	unsigned char uMsgDataLen;			//信息的长度
	unsigned char uMsgData[240+1];		//信息内容
	
}TSOutPutMsg_S, *PTSOutPutMsg_S;

//安全打印
typedef struct _tagTSB4Print_S
{
	unsigned char uCardType;			//卡类型
	unsigned char uKeyIndex;			//密匙号
	unsigned char uDesMode;				//加密标志
	int	iDataLen;						//打印信息的长度
	unsigned char uPrintData[1000+1];	//打印信息
	
}TSB4Print_S, *PTSB4Print_S;

#define MAX_MSG 345
//下发多条信息(信息总长度需小于MAX_MSG)
typedef struct _tagMulMsg
{
	int num;	//信息条数
	char msg[8][121];

}MulMsg, *PMulMsg;

//终端信息 读取终端信息
typedef struct _tagTSPosInfo
{
	char posNum[8+1];					//终端号
	char prmVer[4+1];					//程序版本
	char funVer[8+1];					//功能版本
	char telNum[48+1];					//电话号码
	char psamNum[8+1];					//psam卡号
	unsigned char askType;				//请求类型
	unsigned char inFlag;				//来电显示标志
	unsigned char leftType;				//左串口设备类型
	unsigned char rightType;			//右串口设备类型
	unsigned char inType;				//内置设备类型
	unsigned char enyFlag;				//加密改造, 辨别是否为新加密体系, 20080303
	//unsigned char HID_TYPE;				//HID设备 by liuxiao 20130407

}TSPosInfo, *PTSPosInfo;

//DLL所需的终端信息
typedef struct _tagTSDLLPosInfo
{
	SOCKET sd;							//端口号
	char pos_no[8+1];					//终端号
	unsigned char enyFlag;				//加密体系标识:0旧加密体系, 1新加密体系
	int macCardType;					//旧协议算MAC的卡座号
	char macKey[16+1];					//旧协议算MAC的密钥
	char psamNum[8+1];					//新协议的PSAM卡卡号
	char prmVer[4+1];					//程序版本
	char pan[6+1];						//帐号PAN
	char tskey[4][16+1];				//虚拟终端用的4把KEY
}TSDLLPosInfo, *PTSDLLPosInfo;

//更新本地菜单
typedef struct _tagTSLocalMenu
{
	unsigned char num;					//更新菜单数

	struct MenuContent
	{
		unsigned char index;			//菜单信息索引
		unsigned char flag;				//信息操作标志, '0'表示不可用, '1'表示可用
		unsigned char askType;			//请求类型
		unsigned char contLen;			//菜单内容长度
		char content[80+1];				//菜单内容
	};
	MenuContent menuCont[10];

}TSLocalMenu, *PTSLocalMenu;

//更新密匙
typedef struct _tagTSPSAMKey
{
	unsigned char num;					//更新密匙数
	struct Content
	{
		unsigned char keyIndex;			//密匙索引
		char keyInfo[24+1];				//密匙信息
		char macData[4+1];
	};
	Content keyCont[4];					//最多4把key

}TSPSAMKey, *PTSPSAMKey;

//菜单
typedef struct _tagTSMenus
{
	unsigned char num;					//菜单个数
	struct Menus
	{
		char isused;					//菜单操作标志:'0'表示不可用，'1'表示可用
		int level1;						//一级菜单中的位置从1开始，最大为8
		int level2;						//二级菜单中的位置从1开始，最大为8，0表示无该级菜单
		int level3;						//三级菜单中的位置从1开始，最大为6，0表示无该级菜单
		char askType;					//请求类型，默认为0x00
		char sysnum;					//中心号码序号，默认为'1'
		int tcodelen;				//菜单操作长度
		char tcode[70+1];			//菜单操作
		int menulen;					//菜单名称长度
		char menu[30+1];				//菜单名称
		int menuid;						//菜单ID
		char info[200+1];				//菜单信息
	};
	Menus menus[MAX_LOCAL_MENU];	
}TSMenus, *PTSMenus;

//FTP文件信息
typedef struct _tagTSFtpFiles
{
	unsigned char updateNum;			//文件个数
	struct FtpFiles
	{
		char ip[64+1];					//IP
		char port[5+1];					//端口
		char usr[30+1];					//用户名
		char pwd[30+1];					//密码
		char srcFile[250+1];			//下载文件路径
		char destFile[250+1];			//保存文件路径
		char md5[16+1];					//MD5较验信息
	};
	FtpFiles ftpFiles[MAX_CONT];		//FTP信息
	FtpFiles ftpFilesBak[MAX_CONT];		//备份FTP信息
	int excFilesNum;					//执行的文件数
	char excFilesName[MAX_CONT][250];	//执行的文件名
	int shutDownFlag;					//是否关闭程序0不关闭1关闭
}TSFtpFiles, *PTSFtpFiles;

//交易明细信息
typedef struct _tagTSTxnLogs
{
	int colsnum;						//列数
	int colsWidth[MAX_COLS];			//列宽
	int colsNameLen;
	char colsName[MAX_COLS_NAME_BUF];	//软终端查询明细的列名最大空间
	int itemsLen;						//内容长度
	char items[MAX_INTERM_BUF];			//软终端查询明细的内容最大空间
	char top[MAX_COLS_NAME_BUF];		//提示语TOP
	char hint[MAX_COLS_NAME_BUF];		//提示语
	char bottom[MAX_COLS_NAME_BUF];		//提示语BOTTOM
}TSTxnLogs, *PTSTxnLogs;

/***********************************************************
功能描述: 获取终端信息
输入参数:
	SOCKET sd 与FSK服务器通信的SOCKET
	int macCardType 算MAC及加解密所使用的卡座号
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为20秒
输出参数:
	PTSPosInfo pPosInfo 终端信息
	PTSDLLPosInfo pDllPosInfo DLL所需的终端信息
	int &askType 请求类型
返回值: 
	SUCC 成功;iSelect为用户选择的菜单项
	HOTKEY 快捷键;iSelect为快捷键值
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPosInfo(SOCKET sd, PTSPosInfo pPosInfo, PTSDLLPosInfo pDllPosInfo, int &askType, int &menu_id, int macCardType, int iTimeOut = 20);

/***********************************************************
功能描述: 下发菜单
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSMENULIST menuList 菜单结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int &iSelect 根据返回值确定
返回值: 
	SUCC 成功;iSelect为用户选择的菜单项
	HOTKEY 快捷键;iSelect为快捷键值
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_Menu(const TSDLLPosInfo dPosInfo, PTSMENULIST menuList, int &iSelect, int timeOut = OVER_TIME);

/***********************************************************
功能描述: 下发提示信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	char *msg 提示信息内容
	int flag 0-需用户按[确认]键返回;1-需用户按[确认]键返回或持续5秒后终端机自动返回;2-按指定时间(reTime)自动刷新
	int reTime 按指定时间(reTime)自动刷新时的时间 单位:秒###
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int &iSelect 根据返回值确定
返回值: 
	SUCC 成功;iSelect为用户选择的菜单项
	HOTKEY 快捷键;iSelect为快捷键值
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_OutPutMsg(const TSDLLPosInfo dPosInfo, char *msg, int flag, int &iSelect, int reTime=MSG_RETIME, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 下发安全提示信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSOutPutMsg_S pOutPutMsg_S 安全下发信息
	int flag 0-需用户按[确认]键返回;1-需用户按[确认]键返回或持续5秒后终端机自动返回;2-按指定时间(reTime)自动刷新
	int reTime 按指定时间(reTime)自动刷新时的时间 单位:秒###
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int &iSelect 根据返回值确定
返回值: 
	SUCC 成功;iSelect为用户选择的菜单项
	HOTKEY 快捷键;iSelect为快捷键值
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_OutPutMsg_S(const TSDLLPosInfo dPosInfo, PTSOutPutMsg_S pOutPutMsg_S, int iFlag, int &iSelect, int reTime=MSG_RETIME, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 下发多条提示信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PMulMsg mulMsg 多条信息
	int flag 0-需用户按[确认]键返回;1-需用户按[确认]键返回或持续5秒后终端机自动返回;2-按指定时间(reTime)自动刷新
	int reTime 按指定时间(reTime)自动刷新时的时间 单位:秒###
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int &iSelect 根据返回值确定
返回值: 
	SUCC 成功;iSelect为用户选择的菜单项
	HOTKEY 快捷键;iSelect为快捷键值
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_OutPutMsg_M(const TSDLLPosInfo dPosInfo, PMulMsg mulMsg, int iFlag, int &iSelect, int reTime=MSG_RETIME, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 挂机
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_HangUp(const TSDLLPosInfo dPosInfo, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 交易控制函数 
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSTradeInfoSend pTradeInfoSend 交易信息发送结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	PTSTradeInfoRecv pTradeInfoRecv, 交易信息接收结构体
返回值: SUCC 成功;
***********************************************************/
extern "C" int __declspec(dllexport)TS_TradeControl(PTSDLLPosInfo pdPosInfo, PTSTradeInfoSend pTradeInfoSend, PTSTradeInfoRecv pTradeInfoRecv, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 打印
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int printCom 打印串口号
	int printType 打印机类型
	int iPage 打印页数
	char* acTitle 打印时屏幕显示的文字
	unsigned char* uszPrintMsg 打印内容
	int iPrintLen 打印内容长度
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_B4Print(const TSDLLPosInfo dPosInfo, int printCom, int printType, int iPage, char* acTitle, unsigned char* uszPrintMsg, int iPrintLen, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 安全打印
输入参数:
const TSDLLPosInfo dPosInfo DLL所需的终端信息
int printCom 打印串口号
int printType 打印机类型
int iPage 打印页数
char* acTitle 打印时屏幕显示的文字
int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
SUCC 成功;
<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_B4Print_S(const TSDLLPosInfo dPosInfo, int printCom, int printType, int iPage, char* acTitle, PTSB4Print_S pB4Print_S, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 串口透传
输入参数:
const TSDLLPosInfo dPosInfo DLL所需的终端信息
PTSIcReaderCommand pIcReaderCommand 透传操作结构体
int baudRate 串口硬件波特率
int overTim 终端等待串口返回的超时时间, 单位:秒, 默认为10秒
int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
PTSIcReaderCommand pIcReaderCommand 透传操作结构体	
返回值: 
SUCC 成功;
<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_IcReaderCommand(const TSDLLPosInfo dPosInfo, PTSIcReaderCommand pIcReaderCommand, int baudRate=9600, int overTim=65, int iTimeOut=OVER_TIME); //by liuxiao 65 changed by 25

extern "C" int __declspec(dllexport)TS_HID_IcReaderCommand(const TSDLLPosInfo dPosInfo, PTSHID_IcReaderCommand pHID_IcReaderCommand, int overTim=65, int iTimeOut=OVER_TIME);
/***********************************************************
功能描述: 向终端发送短信
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSSendShortMsg pSendShortMsg 短信结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendShortMsg(const TSDLLPosInfo dPosInfo, PTSSendShortMsg pSendShortMsg, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 从终端提取短信
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	PTSSendShortMsg shortMsg 短信结构体
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetShortMsg(const TSDLLPosInfo dPosInfo, PTSSendShortMsg shortMsg, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 向终端发送通知
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSSendInform pSendInform 通知结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendInform(const TSDLLPosInfo dPosInfo, PTSSendInform pSendInform, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 向终端发送交易结果信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSTradeResult pTradeResult 交易结果结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendTradeResult(const TSDLLPosInfo dPosInfo, PTSTradeResult pTradeResult, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 向终端发送 帐单交易结果信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSBillResult pBillResult 帐单交易结果信息结构体
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendBillResult(const TSDLLPosInfo dPosInfo, PTSBillResult pBillResult, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 向终端下发 帐单
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSUnpayedBill pUnpayedBill 帐单信息结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendBill(const TSDLLPosInfo dPosInfo, PTSUnpayedBill pUnpayedBill, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 从终端读取 帐单
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *billNo 帐单号
	char *amount 金额
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetBillInfo(const TSDLLPosInfo dPosInfo, char *billNo, char *amount, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 向终端下发 卡号
输入参数:
	const PTSDLLPosInfo pdPosInfo DLL所需的终端信息
	char *cardNum 卡号
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
	char comType 串口设备类型
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_SetCardNum(PTSDLLPosInfo pdPosInfo, char *cardNum, char comType=0, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 从终端读取 卡号
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *cardNum 卡号
	char *posTime BCD形式, 格式yyyymmddhh24:mi:ss
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetCardNum(const TSDLLPosInfo dPosInfo, char *cardNum, char *posTime, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 更新提示信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int type 1-错误提示信息, 2-操作提示信息
	PTSCueInfo pCueInfo 提示信息结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateCueInfo(const TSDLLPosInfo dPosInfo, int type, PTSCueInfo pCueInfo, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 更新终端参数
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSPosPar pPosPar 终端参数结构体
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdatePosPar(const TSDLLPosInfo dPosInfo, PTSPosPar pPosPar, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 更新首页信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	char *title 首页信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateHeadPage(const TSDLLPosInfo dPosInfo, char *title, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 主动呼叫终端
输入参数:
	char *posTelNum 终端电放号码
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	SOCKET &sd 端口号	
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_CallPos(SOCKET &sd, char *posTelNum, int rcvTimeOut=60, int sndTimeOut=30);

/***********************************************************
功能描述: 读取版本号
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息	
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输入参数:
	char *version 版本号
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetVersion(const TSDLLPosInfo dPosInfo, char *version, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 读取终端号
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *posNum 终端号
返回值:
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPosNum(const TSDLLPosInfo dPosInfo, char *posNum, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 读取终端序列号
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *posNum 终端序列号
返回值:
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPosSerial(const TSDLLPosInfo dPosInfo, char *posSerial, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 更新PSAM卡密匙
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSPSAMKey pPSAMKey 更新密匙结构体
	int cardType 卡座号
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值:
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdatePSAMKey(const TSDLLPosInfo dPosInfo, PTSPSAMKey pPSAMKey, int cardType, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 读取PSAM卡号及随机数(在更新密钥时用)
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int cardType 卡座号
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *PSAMNum PSAM卡号 8字节
	char *random 随机数 4字节
返回值:
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPSAMNum(const TSDLLPosInfo dPosInfo, int cardType, char *PSAMNum, char *random, int iTimeOut = OVER_TIME);

/***********************************************************
功能描述: 读取本地菜单内容
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	PTSTradeInfoSend pTradeInfoSend, 交易信息发送结构体
	int iTimeOut, SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	PTSTradeInfoRecv pTradeInfoRecv, 交易信息接收结构体
返回值:
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ReadLocalCont(const TSDLLPosInfo dPosInfo, PTSTradeInfoSend pTradeInfoSend, PTSTradeInfoRecv pTradeInfoRecv, int rcvTimeOut=60, int sndTimeOut=30);

/***********************************************************
功能描述: 将字符串转换成16进制的BCD码
输入参数:	 
	unsigned char *ascii_buf, 需要转换的字符串
	int conv_len, 需要转换的字符串长度
	unsigned char type，类型
输出参数:
	unsigned char *bcd_buf, 转换后的BCD码
asc_to_bcd(bcd, (UCHAR*)"123", 3, 0); //结果 为 "0x12 0x30"
asc_to_bcd(bcd, (UCHAR*)"123", 3, 1); //结果 为 "0x01 0x23"
***********************************************************/
extern "C" int __declspec(dllexport)asc_to_bcd(unsigned char *bcd_buf, unsigned char *ascii_buf, int conv_len, unsigned char type);

/***********************************************************
功能描述: 将BCD码转换成字符串
输入参数:	 
	unsigned char *bcd_buf, 需要转换的BCD码
	int conv_len, 转换后，字符串的长度
	unsigned char type，类型
输出参数:
	unsigned char *ascii_buf, 转换后的字符串
bcd_to_asc(asc, (UCHAR*)"\x12\x34", 3, 0); //结果 为 "123"
bcd_to_asc(asc, (UCHAR*)"\x12\x34", 3, 1); //结果 为 "234"
***********************************************************/
extern "C" int __declspec(dllexport)bcd_to_asc(unsigned char *ascii_buf, unsigned char *bcd_buf, int conv_len, unsigned char type);

/***********************************************************
功能描述: 将源字符串去除两边空格的结果 赋值给目标字符串
输入参数:	 
	unsigned char *buf, 待处理字符串	
	len,目标字符串的最大空间
输出参数:
	unsigned char *out_buf, 目标字符串
返回值:
	处理后目标字符串的长度
trim(" 1 2 3 ", 7); //结果 为 "123"
trim(" 1 2 3 ", 2); //结果 为 "1"
***********************************************************/
extern "C" int __declspec(dllexport)trim(char* str,int len);

/***********************************************************
功能描述: 清空终端本地菜单
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	int iTimeOut SOCKET 通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ClearMenus(const TSDLLPosInfo dPosInfo, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 更新终端本地菜单
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	TSMenus menus 菜单结构体
	int iTimeOut SOCKET 通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateMenus(const TSDLLPosInfo dPosInfo, TSMenus menus, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 生成金融菜单指令
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	PTSTradeInfoSend pTradeInfoSend 交易信息发送结构体
	int iTimeOut SOCKET 通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *tcode 指令内容
	int *tcodelen 指令长度
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_TradeEncode(const TSDLLPosInfo dPosInfo, PTSTradeInfoSend pTradeInfoSend, char *tcode, int *tcodelen, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 生成金融菜单指令
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	PTSTradeInfoSend pTradeInfoSend 交易信息发送结构体
	int iTimeOut SOCKET 通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *tcode 指令内容
	int *tcodelen 指令长度
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetMacKey(const TSDLLPosInfo dPosInfo, char *mac16_out, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 生成用TEK加密的数据
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	content 被加密的数据
	contLen 被加密的数据长度
	psamCardNo16 16位的PSAM卡卡号 
输出参数:
	enyData 加密后的数据
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TEKEny(const TSDLLPosInfo dPosInfo, char *content, int contLen, char *psamCardNo16, char *enyData);

/***********************************************************
功能描述: 从FTP下载文件，并执行相应操作
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	TSFtpFiles FTP文件信息
输出参数:
	fileNo 当前已更新的文件号
	pageNo 当前已更新的文件的段号
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateFiles(const TSDLLPosInfo dPosInfo, TSFtpFiles ftpFiles, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 下发交易明细
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	PTSTxnLogs ptxnLogs 交易明细信息
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ShowTxnLogs(const TSDLLPosInfo dPosInfo, PTSTxnLogs ptxnLogs, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 下发当前所选项的内容(2012-03-28 新增)
输入参数:
	const TSDLLPosInfo dPosInfo DLL 所需的终端信息
	int   icolumn   需要获取的内容所在列
返回值: 
	char *selItemCont 当前所选项的内容
	SUCC 成功;
	<0 失败
***********************************************************/
//extern "C" int __declspec(dllexport)TS_GetSelItemCont(const TSDLLPosInfo dPosInfo, char *selItemCont,int icolumn,int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 金融密码加密
输入参数:
	char * pwd 密码 6字节ASII
	char * random 随机数 16字节ASII
输出参数:
	char * enc_result 加密结果 16字节ASII
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)myEncryptForBankcardPwd(char * pwd, char * random,char * enc_result);

/***********************************************************
功能描述: 普通密码加密
输入参数:
	char * pwd 密码 6字节ASII
输出参数:
	char * enc_result 加密结果 16字节ASII
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)myEncryptForPayEasyMemberPwd(char * pwd, char * enc_result);

/***********************************************************
功能描述: 打开钱箱
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
	SUCC 成功;
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_OpenMnyBox(const TSDLLPosInfo dPosInfo, char *instruct, int instructLen, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 注入网厅的付费卡充值页面
输入参数:
const TSDLLPosInfo dPosInfo DLL所需的终端信息
char *httpcon 网厅域名
char *request 网厅的页面相对地址
char *formData 表单内容
char *getData 返回结果(针对网厅)
int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
SUCC 成功;
<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_FillCt10000(const TSDLLPosInfo dPosInfo, char *httpcon, char *request, char *formData, char *getData, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 在IE上打开网址为URL的网页
输入参数:
const TSDLLPosInfo dPosInfo DLL所需的终端信息
char *url 网址
int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
返回值: 
SUCC 成功;
<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_OpenIE(const TSDLLPosInfo dPosInfo, char *url, int iTimeOut=OVER_TIME);

/***********************************************************
功能描述: 珠海公交注册
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int result 返回内容
返回值: 
	SUCC 成功
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusPosLogin(const TSDLLPosInfo dPosInfo, char* errMsg, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 珠海公交签到
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	char *operUniqueId 操作员ID
	char *passwd 操作员密码
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int result 返回内容
返回值: 
	SUCC 成功
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusSignIn(const TSDLLPosInfo dPosInfo, char *operUniqueId, char *passwd, char* errMsg, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 珠海公交余额查询
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int result 返回内容
	long &out_balance返回的卡内余额，以"分"表示
	long &out_asn返回的卡号
	long &out_indate返回的卡有效期，格式如：20201231
	long &out_cardType返回的卡类型
返回值: 
	SUCC 成功
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusQuery(const TSDLLPosInfo dPosInfo,  long &out_balance, long &out_asn, char *out_indate, char *out_cardType, char* errMsg, int timeOut=OVER_TIME);

/***********************************************************
功能描述: 珠海公交充值
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	nAmount充值金额，以"分"表示
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int result 返回内容
	long &out_balance返回的充值后卡内余额，以"分"表示
	long &out_tradeId返回的交易流水号
	long &out_asn返回的卡号
	long &oldBalance返回的充值前卡内余额，以"分"表示
	long &out_operUniqueId返回的操作员号
	char *out_cardType返回的卡类型
	char *out_devUniqueId返回的设备号
	long &out_Time返回的交易时间
返回值: 
	SUCC 成功
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusTopUp(const TSDLLPosInfo dPosInfo, long nAmount, long &out_balance, long &out_tradeId, long &out_asn, long &oldBalance, long &out_operUniqueId, char *out_cardType, char *out_devUniqueId, long &out_Time, char* errMsg, int timeOut=200);

/***********************************************************
功能描述: 珠海公交获取错误信息
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	int result 返回内容
	char *out_mesg返回的错误信息
返回值: 
	SUCC 成功
	<0 失败
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusGetErrMsg(const TSDLLPosInfo dPosInfo, char *out_mesg, int timeOut=OVER_TIME);


/***********************************************************
功能描述: 下发命令
输入参数:
	const TSDLLPosInfo dPosInfo DLL所需的终端信息
	char *msg 命令内容
	int iTimeOut SOCKET通信超时时间, 单位:秒, 默认为60秒
输出参数:
	char *msg 返回内容
返回值: 
	SUCC 成功
	<0 失败
***********************************************************/
//extern "C" int __declspec(dllexport)TS_SendCommand(const TSDLLPosInfo dPosInfo, char *msg, int timeOut=OVER_TIME);

#endif 