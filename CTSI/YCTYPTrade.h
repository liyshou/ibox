// YCTYPTrade.h: interface for the CYCTYPTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(YCTTRADE_TRAH__BA427F3F_89A8_429A_BBBD_C794CFDB3EE3__INCLUDED_)
#define YCTTRADE_TRAH__BA427F3F_89A8_429A_BBBD_C794CFDB3EE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <winsock2.h>
#include "Global.h"
#include "Pack.h"
#include "Trade.h"

//读卡器写死的标志
#define TAG 7
#define VER 1
#define STAT 0x00
//程序上普通钱包的查询交费标志
#define NORMALPAY 0
#define NORMALENQ 1
#define AUTOCHARGE 2
//读卡器的功能命令
#define NORMAL_PAY_CMD 0x85
#define NORMAL_NOTFINISH_CMD 0x86
#define MONTH_SIGNIN_CMD 0x87
#define MONTH_PAY_CMD 0x88
#define MONTH_NOTFINISH_CMD 0x89
#define MONTH_ENQ_CMD 0x8A
//羊城通返回码的机构
#define FROMSUCC 0x00//成功

#define FROMIC 0x01//读卡器返回的错误，充值失败
#define FROMYCT 0x02//羊城通返回的错误，充值失败
#define FROMPE 0x03//PE返回的错误(含超时)
#define FROMCTSI 0x04//CTSI返回的错误，充值失败


#define FROMIC_N 0x05//读卡器返回的错误，充值结果未知
#define FROMYCT_N 0x06//羊城通返回的错误，充值结果未知
#define FROMCTSI_N 0x07//CTSI返回的错误，充值结果未知

#define FROMYCT_O 0x08//羊城通直接返回的错误

//FROMSUCC
#define SUCC_MSG 0x01//成功，提示羊城通充值成功
#define SUCC_NOMSG 0x02//成功，不提示
#define SUCC_MSG_LAST 0x03//成功，提示上笔羊城通充值成功
#define SUCC_NEEDOTHEROPR 0x04//成功，还有相关操作，需要执行交易类型查询
#define SUCC_NEEDEXAPPOPR 0x05 //扣费成功，还有相关操作，还需要执行扩展应用操作
#define SUCC_PURCHASE 0x06//扣费成功
#define SUCC_TRANSFER 0x07//转移成功


//FROMIC
#define ERROR_FROMIC 0x01 //读卡器返回的错误
#define R_APP_CARD_UPGRADE_EXEFAIL 0x02//执行同步指令失败


//FROMCTSI FROMCTSI_N
#define FATALERR 0xFF//致命错误，务必检查
#define COMNOTFOUND 0x01//没有找到读卡器
#define USERCANCEL 0x02//用户取消
#define PAYTIMEOUT 0x03//扣费超时
#define ICTIMEOUT 0x04//接收或发送IC数据超时
#define CONNCOMFAIL 0x05//连接读卡器失败
#define YCTTIMEOUT 0x06//接收或发送YCT数据超时
#define ICNOTSURE 0x07//读卡器不能确定交易状态
#define ICSAYFAIL 0x08//读卡器指明交易失败
#define PACKFAIL 0x09//打包失败
#define UNPACKFAIL 0x0A//解包失败
#define CARDNOTFIX 0x0B//不支持的卡
#define PKICHGNOTFINISH 0x0C//PKI卡已更换，无法读取上笔交易结果
#define FAIL_LAST 0x0D//经过读取上笔交易结果后，确认充值失败
#define PAYBUTNOTFINISH 0x0E//已扣费，但充值结果未知
#define SIGNINFAIL 0x0F//签到失败
#define NORESULT 0x10//签到失败
#define CARDNOTFOUND 0x11//没有找到卡，读卡错误
#define LASTTRANSFER_NOTDONE 0x12//该卡上次转移的充值没成功，转到充值的流程
#define TRANSFER_ERROR 0x13//转移失败
#define PURCHASE_FAIL 0x14//扣费失败
#define BLACKLIST 0x15//卡为黑名单

// FROMYCT_O
#define AMOUNTRULE_VIALATION 0x30 //充值金额违反业务规则
#define BALANCE_EXCEED 0x96 //充值后余额超上限
#define UNRECOGNIZED_TRADETYPE 0x32 //无法识别的交易类型

#define ST_HEAD 0xBA	//索天读卡器通讯包头
//索天读卡器功能命令
#define ST_ONTS_REQUEST	0x81	//寻卡命令
#define ST_ONTS_CHALLENGE 0x82	//挑战
#define ST_ONTS_AUTH_PREPARE 0x83 //生成充值认证数据
#define ST_ONTS_AUTH_EXECUTE 0x84 //处理充值认证结果数据
#define ST_ONTS_CHARGE_PREPARE 0x85 //生成充值交易参数数据
#define ST_ONTS_CHARGE_EXECUTE 0x86 //执行充值交易
#define ST_ONTS_COMMIT 0x87		//提交交易状态数据
#define ST_ONTS_CONFIRM 0x88	//处理交易确认结果

#define ST_ONTS_UREQUEST 0x8A	//生成未完整交易处理请求数据
#define ST_ONTS_UCHALLENGE 0x8B	//处理未完整交易处理挑战数据
#define ST_ONTS_UCOMMIT 0x8C	//提交未完整交易的状态数据
#define ST_ONTS_UCONFIRM 0x8D	//处理未完整交易确认结果

//索天读卡器响应状态码
#define MOC_ERROR_NORMAL		0x00
#define MOC_ERROR_SAM_PIN		0x62
#define MOC_ERROR_NOTAG         0x80
#define MOC_ERROR_CARD_READ		0x84
#define MOC_ERROR_CARD_NAMELIST	0x94
#define MOC_ERROR_TS_FALL		0xB1
#define MOC_ERROR_ONTS_REFUSED	0xC1
#define MOC_ERROR_ONTS_AUTH		0xC2
#define MOC_ERROR_ONTS_COMMIT	0xC3
#define MOC_ERROR_PKI_DES		0x72

//卡间余额转移用
#define INTO_QRY 0x01 //转入卡查询
#define OUT_QRY  0x02//转出卡查询

#define SKEY_VALIDATE 12 //SKEY有效期，单位小时 ，

#define CKEY "11223344556677888877665544332211"

class CYCTYPTrade  
{
public:
	CYCTYPTrade();
	virtual ~CYCTYPTrade();

	// 检查是否有未知交易
	// 入参：term_id:终端号；
	// 出参：pyct_data:羊城通交易信息
	// 返回值：
	// 0:检查通过
	// 1:处理"未知结果交易"出错
	// 2:签到的pki卡卡号与原交易不一致
	int static NotFinishChk(CGlobal gbl, char *term_id, char *pki_card, char *signin_serial);

	int static YCT_NormalTrade(CGlobal *pgbl, int payType, BYTE* party, BYTE* rspcode, char *priCont);
	//主函数
	//payType:0:月票 1:非月票先查询 2:非月票直接充值
	// 返回值：
	// 0：成功已确认
	// 1：成功未确认
	// 2：失败已冲正
	// 3：失败未冲正
	// 4：未知结果
	// 5：扣费前失败
	int static TradeHdl(CGlobal gbl, char *title, int month, char *pki_card=NULL, char *signin_serial=NULL);

	int yctSetting(char *pinValue);
	void setInPar(CGlobal inGbl); //设置传入参数
	static int getComPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int &com, int &comType);//获取打印参数 com: 使用串口 comType:串口类型
	int static showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);
	// 月票钱包的主函数
	// 入参：无
	// 出参：无
	// 返回值：
	// 0：成功已确认
	// 1：成功未确认
	// 2：失败已冲正
	// 3：失败未冲正
	// 4：未知结果
	// 5：扣费前失败
	int static MonthTrade(CGlobal *pgbl, int payType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont);

private:
	static int AutoSelectAmo(const TSDLLPosInfo dposInfo,char* czamo,char * tradeamount);
	static int Yct_AcctHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode);
	static int  P_ACCT_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, int &payType, char *czamount, char *money_limit,char *char_fre,char *ornotpassword,char *statusflag, BYTE *sign_status,BYTE *party,BYTE *rspcode);
	// 福州索天科技版本
	// 普通钱包的不完整交易
	// 入参：yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
	// 出参：ticket_amo_aft(12):票卡充值后余额; errCode:出错代码
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败，结果未知
	// 2：读卡器返回的错误，结果未知
	// 3：与羊城通交互失败，结果未知
	// 4：PKI卡被更换，结果未知
	// 5：失败
	int static ST_NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode);

	//福州索天科技版本
	// 普通钱包的查询充值
	// // 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型; enq:0:要充值; 1:只查询; amount:充值金额
	// 出参：pyct_data:羊城通交易信息; errCode:出错代码
	// 返回值
	// 0：成功(查询或充值)
	// 1：与读卡器交互失败
	// 2：读卡器返回的错误
	// 3：与羊城通交互失败
	// 4：羊城通返回的错误
	// 5：与羊城通交互失败，需要做不完整交易
	// 6：与读卡器交互失败，需要做不完整交易
	// 7：没有确认标识，需要做不完整交易
	// 8：确认充值失败
	// 9：读卡器返回的错误，需要做不完整交易
	int static ST_NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode);  //第三代羊城通充值
	int static ST_NormalEnqPay2(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode); //第二代羊城通充值

	// 福州索天科技版本
	// 对数据进行打包
	// 入参：tag:报文标识; ver:版本; num:报文顺序号; len:报文长度(cmd,stat,data的长度); cmd:命令标识; stat:状态标识; data:数据;
	// 出参：sndData:换包后的数据
	// 返回值：包长度
	int static ST_packData(int head, int len, int cmd, const BYTE *data, BYTE *sndData);

	//福州索天科技版本
	//上传数据包给羊城通服务器并接收回应
	//入参：sdType:0:短连接;1:长连接; sd:端口号; num, 第num步操作
	//		sndData, 发向羊城通的数据
	//		sndLen, 发向羊城通的数据长度
	//出参：rcvData, 从读卡器接收的数据
	//		rcvLen, 从读卡器接收的数据的长度
	//返回值：
	//0：成功
	//1：与羊城通交互失败
	//2：与读卡器交互失败
	int static ST_commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen);

	// 福州索天科技版本
	// 初始化读卡器
	// 返回值
	// 0：成功
	// 1：失败
	int static ST_initReader(const TSDLLPosInfo dPosInfo, const int com, const int comType);

	// 福州索天科技版本
	// 显示错误信息
	int  static ST_showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);

	//==========================================================================================================

	// 普通钱包的不完整交易
	// 入参：yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
	// 出参：ticket_amo_aft(12):票卡充值后余额; errCode:出错代码
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败，结果未知
	// 2：读卡器返回的错误，结果未知
	// 3：与羊城通交互失败，结果未知
	// 4：PKI卡被更换，结果未知
	// 5：失败
	int static NormalNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode);
	
	// 月票钱包的不完整交易
	// 入参：signin_serial(16):握手流水号;yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
	// 出参：ticket_amo_aft(12):票卡余额; errCode:出错代码
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败，结果未知
	// 2：读卡器返回的错误，结果未知
	// 3：与羊城通交互失败，结果未知
	// 4：PKI卡被更换，结果未知
	// 5：失败

	int static MonthNotFinish(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *signin_serial, char *yct_serial, char *pki_card, char *ticket_amo_aft, BYTE* party, BYTE* rspcode);
	
	//确认与冲正
	// 入参：yct_data:羊城通交易信息
	// 出参：无
	// 返回值：
	// 2：确认成功
	// 3：确认失败
	// 5：冲正成功
	// 6：冲正失败
	int static AckRevTrade(CGlobal gbl, YCT_DATA yct_data);

	// 普通钱包选择充值金额
	// 入参：无
	// 出参：amount:金额
	// 返回值：
	// 0：取金额成功
	// 非0：取金额失败
	int static NormalSelectAmo(CGlobal* gbl);

	// 普通钱包的主函数
	// 入参：无
	// 出参：无
	// 返回值：
	// 0：成功已确认
	// 1：成功未确认
	// 2：失败已冲正
	// 3：失败未冲正
	// 4：未知结果
	// 5：扣费前失败
	int static NormalTrade(CGlobal *pgbl, int payType, BYTE* party, BYTE* rspcode, char *priCont);
	
	


	// 普通钱包的查询充值
	// // 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型; enq:0:要充值; 1:只查询; amount:充值金额
	// 出参：pyct_data:羊城通交易信息; errCode:出错代码
	// 返回值
	// 0：成功(查询或充值)
	// 1：与读卡器交互失败
	// 2：读卡器返回的错误
	// 3：与羊城通交互失败
	// 4：羊城通返回的错误
	// 5：与羊城通交互失败，需要做不完整交易
	// 6：与读卡器交互失败，需要做不完整交易
	// 7：没有确认标识，需要做不完整交易
	// 8：确认充值失败
	// 9：读卡器返回的错误，需要做不完整交易
	int static NormalEnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, PYCT_DATA pyct_data, char *amount, BYTE* party, BYTE* rspcode);

	//包处理函数:打包、发包、收包和解包
	// 入参：flag:2:交费包;4:确认包;5:冲正包
	// 出参：无
	// 返回值
	// 0：成功
	// 非0：失败
	int static packHdl(CGlobal gbl, CPack *ppack, int flag);

	// 确认交易
	// 入参：yct_data:羊城通交易信息
	// 出参：无
	// 返回值：
	// 0：已确认
	// 1：未确认
	int static AckTrade(CGlobal gbl, YCT_DATA yct_data);

	// 冲正交易
	// 入参：yct_data:羊城通交易信息
	// 出参：无
	// 返回值：
	// 0：已冲正
	// 1：未冲正
	int static RevTrade(CGlobal gbl, YCT_DATA yct_data);

	int static MonthSelectWay(const TSDLLPosInfo dPosInfo, int payType, PYCT_DETAIL pyct_detail);



	// 月票钱包的签到
	// 入参：无
	// 出参：pki_card(8):PKI卡卡号；signin_serial(16):握手流水号
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败，签到失败
	// 2：读卡器返回的错误，签到失败
	// 3：与羊城通交互失败，签到失败
	// 4：最终签到失败
	int static MonthSignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode);

	// 月票查询
	// 入参：pki_card:PKI卡卡号; signin_serial(16):握手流水号
	// 出参：pyct_data:羊城通交易信息; pki_card:PKI卡卡号; signin_serial(16):握手流水号
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败
	// 2：读卡器返回的错误
	// 3：与羊城通交互失败
	// 4：羊城通返回的错误
	// 5：签到失败
	// 6：用户取消
	int static MonthEnq(const TSDLLPosInfo dPosInfo, const int com, const int comType, char ticket_type, char *yct_serial, char *amount, char *pki_card, char *signin_serial, BYTE* party, BYTE* rspcode);

	// 月票充值的前1，2步
	// 入参：signin_serial(16):握手流水号
	// 出参：pyct_data:羊城通交易信息
	// 返回值
	// 0：成功(查询或充值)
	// 1：与读卡器交互失败
	// 2：读卡器返回的错误
	// 3：与羊城通交互失败
	// 4：羊城通返回的错误
	// 5：PKI卡卡不一致，需要重新签到
	int static MonthPay1_2(const TSDLLPosInfo dPosInfo, const int com, const int comType, YCT_DETAIL yct_detail, char *yct_serial, char *pki_card, char *amount, char *signin_serial, BYTE* party, BYTE* rspcode, char *priCont);

	// 月票充值的前3，4步
	// 入参：无
	// 出参：pyct_data:羊城通交易信息
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败
	// 2：读卡器返回的错误
	// 3：羊城通返回的错误
	// 4：与羊城通交互失败，需要做恢复
	// 5：与读卡器交互失败，需要做恢复
	// 6：读卡器返回的错误，需要做恢复
	int static MonthPay3_4(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE* party, BYTE* rspcode);

	// 刷卡输密码向PE发扣费包
	// 入参：biz_type(4):业务代码；amount(12):金额
	// 出参：无
	// 返回值
	// 0：扣费成功
	// 1：扣费失败
	// 2：超时返回
	int static PayPe(CGlobal *pgbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode);
	//改造函数 by liuxiao
	int static PayPe(CGlobal gbl, YCT_DATA yct_data, BYTE* party, BYTE* rspcode);
	// 连接测试
	// 入参：无
	// 出参：无
	// 返回值
	// 0：成功
	// 1：失败
	int static connTest(const TSDLLPosInfo dPosInfo, const int com, const int comType);

	// 对数据进行打包
	// 入参：tag:报文标识; ver:版本; num:报文顺序号; len:报文长度(cmd,stat,data的长度); cmd:命令标识; stat:状态标识; data:数据;
	// 出参：sndData:换包后的数据
	// 返回值：包长度
	int static packData(int tag, int ver, int num, int len, int cmd, int stat, const BYTE *data, BYTE *sndData);

	// 向读卡器发送和接收数据
	// 入参：dPosInfo:DLL所需的终端信息; com:串口序号; comType:串口类型;
	// 出参：无
	// 返回值
	// 0：成功
	// 1：失败
	int static SndRcvIcData(const TSDLLPosInfo dPosInfo, const int com, const int comType, const char *title, const BYTE *downData, const int downLen, BYTE *upData, int *upLen, int baudRate=9600);

	// 把12位金额化为4位hex形式
	// 入参：amo:12位的金额
	// 出参：hexAmo:4位hex形式的金额
	// 返回值：无
	void static getHexAmo(const char *amo, BYTE *hexAmo);

	// num位hex形式化为把12位金额
	// 入参：hexAmo:4位hex形式的金额 num:位数
	// 出参：amo:12位的金额
	// 返回值：无
	void static getAsiAmo(const BYTE *hexAmo, int num, char *amo);

	// 计算校验码
	// 入参：data:数据; dataLen:数据长度
	// 出参：无
	// 返回值
	// 0：一致
	// 1：不符
	int static isXor(const BYTE *data, int dataLen);

	/****************************************************/
	//上传数据包给羊城通服务器并接收回应
	//入参：sdType:0:短连接;1:长连接; sd:端口号; num, 第num步操作
	//		sndData, 发向羊城通的数据
	//		sndLen, 发向羊城通的数据长度
	//出参：rcvData, 从读卡器接收的数据
	//		rcvLen, 从读卡器接收的数据的长度
	//返回值：
	//0：成功
	//1：与羊城通交互失败
	//2：与读卡器交互失败
	/****************************************************/
	int static commuWithYCTIC(const TSDLLPosInfo dPosInfo, const int com, const int comType, const SOCKET sd, int num, int cmd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen);

	//---------------------------------------------------------------------------
	//函数名:   SndRcvYctData
	//功能描述：向羊城通主机发送交易请求，并接受应答信息
	//输入：	sendData：发送报文内容，sendLen：发送报文长度，recvLen：接收报文长度
	//输出：	recvData：接收报文
	//返回：	>=0：接收长度，<0：错误
	//---------------------------------------------------------------------------
	int static SndRcvYctData(const SOCKET sd, char *sendData,int sendLen, char *recvData, int *recvLen, int recvMaxLen);

	void static wrYctDataLog(char *msg, YCT_DATA yct_data);

	int static MW_showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);

	//int static showErrMsg(CGlobal gbl, BYTE party, BYTE rspcode);
	/***************************第三代羊城通充值新增函数开始*********************************************/
	
	//读卡器初始化
	int static ST_initReader3(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data);
	
	//与服务器进行数据交互
	int static ST_commuWithYCTIC3(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data, const int com, const int comType, const SOCKET sd, const BYTE *sndData, int sndLen, BYTE *rcvData, int *rcvLen);

	//福州索天读卡器签到报文
	int  static yct_SignIn(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * party ,BYTE* rspcode);
	
	//读取卡物理信息
	int static R_PUB_QRY_PHYSICS_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE*physicsInfo);

	//设置卡信息
	int static R_PUB_SET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,char* logical_card,BYTE * query_info,char *SW1SW2);
	
	//读取CPU卡片信息
	//flag:  0 - 充值前查询余额, 1 - 充值后查询余额
	void static R_CPU_GET_READCARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE *count_info,int flag = 0);

	//执行CPU圈存
	int static CPU_EnqPay(const TSDLLPosInfo dPosInfo, const int com, const int comType, int enq, \
		PYCT_DATA pyct_data, char *amount, BYTE payType,BYTE *query_info,BYTE* party, BYTE* rspcode);
	
	//黑名单卡处理函数 至黑名单卡初始化 R_SET_BLACKLIST_INIT
	int  static R_SET_BLACKLIST_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data );

	//捕获黑名单

	int static P_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_info3, BYTE *rsp,BYTE * party,BYTE* rspcode);

	//黑名单提交
	int static P_BLACKLIST_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data,BYTE *status, BYTE * SW1SW2 ,BYTE * party,BYTE* rspcode);

	//设置黑名单
	int  static R_SET_BLACKLIST(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,BYTE *SW1SW2,BYTE *status);

	//执行黑名单处理
	int static  EXCUTE_BLACKLIST(const TSDLLPosInfo dPosInfo,const int com,const int comType, PYCT_DATA pyct_data,\
		BYTE * query_info,BYTE *party, BYTE *rspcode);
	
	//CPU同步查询
	int  static R_CARD_QRY_UPGRADE(const TSDLLPosInfo dPosInfo, PYCT_DATA pyct_data, const int com, const int comType,BYTE *random_num,BYTE * rsp_message);

	//卡片同步应用
	int static R_APP_CARD_UPGRADE(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE* upgrade_info);
	
	//cpu 同步
	int static P_M1_TO_CPU(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, \
		BYTE * upgrade_info, int  &next_status,BYTE *syn_order,BYTE * party,BYTE* rspcode);

	//执行cpu 同步

	int static	EXCUTE_M1_TO_CPU(const TSDLLPosInfo dPosInfo,const int com,const int comType,PYCT_DATA pyct_data,BYTE *query_info,BYTE * party, BYTE *rspcode);

	//	m1钱包查询
	int static P_M1_LOAD_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_money,BYTE *pysical_info1,int appType,BYTE * query_info,BYTE * party,BYTE* rspcode);

	//M1卡取卡信息
	int  static R_M1_GET_CARDINFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info,int& rspStatus,int flag);
	
	int  static P_M1_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType, PYCT_DATA pyct_data, BYTE * trade_info,int next_action,BYTE * charge_info,BYTE * party,BYTE* rspcode);

	//进行M1的圈存
	int  static R_M1_LOAD(const TSDLLPosInfo dPosInfo, const int com, int &status,const int comType,PYCT_DATA pyct_data,BYTE * charge_info);

	//M1冲正
	int static P_M1_ROLLBACK(const TSDLLPosInfo dPosInfo, const int com, const int comType, int status,PYCT_DATA pyct_data,char* reason,char *aa65_stan,BYTE *money,BYTE * TAC,BYTE * trade_info,int next_action,BYTE *party,BYTE *rspcode);

	//对M1进行查询
	int static excute_M1_LOAD(const TSDLLPosInfo dPosInfo,const int com,const int comType,PYCT_DATA pyct_data,\
		int enq,BYTE *party,BYTE *rspcode,char *tradeamount,int payType);
	
	//对金额进行转换 M1取卡查询为低位在前
	void static getAsiAmo3(const BYTE *hexAmo, int num, char *amo);

	//交易类型查询
	int static P_TRAN_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType, \
		PYCT_DATA pyct_data,BYTE *card_pycicalInfo1,BYTE oper_code,int &trade_type,BYTE *query_info,BYTE *party, BYTE *rspcode);


	//充值撤销函数
	int static Charge_Cancle_CPU(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow ,  const char* pz , double amount,BYTE *query_info);
	int static Charge_Cancle_M1(CGlobal *pgbl , PYCT_DATA pyct_data , int com , int comType , const char* flow ,  const char* pz , double amount,BYTE *query_info);
	//P_TRAN_QRY
	int static P_TRAN_QRY3(const TSDLLPosInfo& dPosInfo ,  PYCT_DATA pyct_data , \
		unsigned char SAK , unsigned char* cardInfo , unsigned char exeFlag , unsigned char* outBuf , int* len );
	
	//R_CPU_CANCLE_INIT
	int static R_CPU_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType, char* bizInfo3 , unsigned char* outBuf , int* outLen);
	int static R_M1_CANCLE_INIT(const TSDLLPosInfo dPosInfo, const int com , const int comType, char* bizInfo , unsigned char* outBuf , int* len);
	int static R_M1_CANCLE(const TSDLLPosInfo& dPosInfo , int com , int comType , char* bizInfo , unsigned char* outBuf , int* len);
	
	//与读卡器交互基础函数
	int static R_COMMAND(const TSDLLPosInfo dPosInfo, const int com, const int comType,		\
		int commandCode , char* inBuf , int inLength , char* outBuf, int* outLength);
	//与终端服务器交互基础函数
	int static P_COMMAND(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data ,const unsigned char* infoType,  \
		const unsigned char* ver, int encryptMethod , int endian , unsigned char* inBuf , int inLen , \
		unsigned char* outBuf, int* outLen);
	//P_CPU_CANCLE_QRY
	int static P_CPU_CANCLE_QRY(const TSDLLPosInfo dPosInfo,  PYCT_DATA pyct_data , unsigned char* countInfo , \
		const char* flow, const char* pz , 	int amount, unsigned char* outBuf , int* len);
	int static P_CPU_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , \
		const unsigned char* cardinfo1 , const unsigned char* retCode , int readerCode , unsigned char* outBuf , int* len);
	int static R_CPU_CANCLE(const TSDLLPosInfo dPosInfo , int com , int comtype , double amount , const unsigned char* bizInfo4 , \
		unsigned char* outBuf , int* len);
	//int static P_SET_MONITOR(const TSDLLPosInfo dPosInfo , int com , int comType);
	int static R_PUB_GET_PKISTATE(const TSDLLPosInfo dPosInfo , int com , int comType , unsigned char* outBuf , int* len);
	int static R_PUB_GETVERSION(const TSDLLPosInfo dPosInfo , int com , int comType , unsigned char* outbuf , int* len);
	int static R_PUB_SET_READCARDINFO2(const TSDLLPosInfo& dPosInfo , int com , int comType , \
		unsigned char* logic_No, unsigned char* qryInfo , unsigned char* outBuf , int* len );
	int static P_CPU_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, \
		unsigned char* flow, unsigned char* TAC , unsigned char* MAC2 , unsigned char* cardCode , unsigned char readerCode,	\
		unsigned char* bef , unsigned char* up_bef, unsigned char down_bef , unsigned char* countInfo, \
		unsigned char* outBuf , int* len);
	int static P_M1_CANCLE(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data, const char* flow , double amount , \
		unsigned char* pz , unsigned char* bizInfo,  unsigned char* outBuf , int* len);
	int static P_M1_CANCLE_SUBMIT(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, \
		const char* flow , unsigned char* cancleInfo , unsigned char* bizInfo , unsigned char readerCode,	\
		unsigned char* outBuf , int* len);
	
	int static P_EXTEND_QRY(const TSDLLPosInfo dPosInfo , PYCT_DATA pyct_data, unsigned char* outBuf , int* len);
	int static P_EXTEND(const TSDLLPosInfo dposinfo , PYCT_DATA pyct_data,  \
		unsigned char* flow , unsigned char* bizType , int firstType , unsigned char* firstFlow , \
		int infLen , unsigned char* cardInfo ,	unsigned char* outBuf , int* outLen);
	int static R_EXTEND(const TSDLLPosInfo dPosInfo , int com , int comtype ,\
		int codeLength , unsigned char* code , unsigned char* outBuf , int* len);

	int static R_CPU_GET_CARDINFO(const TSDLLPosInfo& dPosInfo , int com , int comType , unsigned char* outBuf , int*len);
	int static ErrReturn(const TSDLLPosInfo& dPosInfo , const unsigned char* offset);
	//获取卡片物理信息，重新封装 --scc
	int static R_PUB_QRY_PHYSICS_INFO2(const TSDLLPosInfo& dPosInfo , int com ,int comType , unsigned char* outBuf, int*len);
public:
	//充值撤销函数
	int static Charge_Cancle(CGlobal *pgbl , const char* flow ,  const char* pz , const char* amount,YCT_DATA pyct_data);
	//终端监控
	int static termMonitor(LPVOID pgbl,PYCT_DATA pyct_data);
	//扩展应用,execFlag 前次操作类型
	int static ExtendApp(CGlobal *pgbl , int execFlag );
	//CPU圈存初始化
	/***************************第三代羊城通充值新增函数结束*********************************************/
	int  static Yct_AutoTradeHdl(CGlobal gbl,char* title);
	/**************************** CPU ****************************************/
// 	//CPU圈存初始化
// 	int  static R_CPU_LOAD_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,BYTE * trade_info_1);
// 	
// 	//CPU圈存查询
// 	int static P_CPU_LOAD_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE *count_info,\
// 		PYCT_DATA pyct_data,  const char * amount,BYTE appType,\
// 		BYTE * trade_message1,BYTE *party,BYTE* rspcode);
	

	//CPU圈存初始化
	//in: TSDLLPosInfo dPosInfo
	//     int com,int comType
	//     PYCT_DATA pyct_data
	//out: BYTE *trade_message1
	//     BYTE *trade_sequence
    //	   BYTE *yct_term_status
	//     BYTE *card_info
	int  static R_CPU_LOAD_INIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *trade_message1,BYTE *trade_sequence,BYTE *yct_term_status,BYTE *card_info);
	
	//CPU圈存查询
	int static P_CPU_LOAD_QRY(const TSDLLPosInfo dPosInfo, const int com, const int comType,BYTE *count_info,\
		PYCT_DATA pyct_data,  const char * amount,BYTE appType,\
		BYTE * trade_message1,BYTE *party,BYTE* rspcode);
		
    //R_CPU_LOAD CPU钱包圈存
	//in  : trade_message2 - 交易信息2
	//out : TAC - 交易认证码, 
	//      load_status - 圈存状态 0 正常提交，1 网络故障，未收到AA84报文或未向读卡器发起充值指令 
	int static R_CPU_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *trade_message2,BYTE *TAC,int &load_status,BYTE *count_info);
	
	int static P_CPU_LOAD(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_info,BYTE *yct_term_status,BYTE *trade_message2,BYTE *charge_authcode);
	
	int static P_CPU_LOAD_SUBMIT(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		int &load_status,BYTE *TAC,BYTE *yct_term_status,BYTE *count_info,BYTE *charge_authcode,BYTE *followup_oper);
	/**************************** CPU ****************************************/

	//查询卡应用信息
	//in: flag - 0 扣费前查询，1 扣费后查询(默认为0)
	//out: card_appinfo - 卡应用信息
	int static R_PUB_QRY_APP_INFO(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		BYTE *card_appinfo,BYTE *ReaderStatus,int flag = 0);

	//设置羊城通充值第三代打印明细内容
	//in : type - 单据类型  0:充值成功单据   1:充值疑似成功单据 2:充值撤销单据 
	//						3:转出卡圈提单据 4:转入卡圈存单据   5:转入卡圈存疑似成功单据
	//out: priCont - 打印明细内容
	int static set_yctthird_priCont(int type,PYCT_DATA pyct_data,char *priCont);
	
	//获取卡响应状态码含义
    int static get_cardStatus_errMsg(BYTE *SW1SW2,char *errMsg);

	//获取读卡器错误信息
    int static get_cardReader_errMsg(BYTE *status,char *errMsg);


	// 福州索天科技 羊城通充值第三代版本
	// 不完整交易处理
	// 入参：yct_serial(8):交易流水号;pki_card(8):PKI卡卡号
	// 出参：ticket_amo_aft(12):票卡充值后余额; errCode:出错代码
	// 返回值
	// 0：成功
	// 1：与读卡器交互失败，结果未知
	// 2：读卡器返回的错误，结果未知
	// 3：与羊城通交互失败，结果未知
	// 4：PKI卡被更换，结果未知
	// 5：失败
	int static ST_YctThird_NotFinishHdl(const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data,\
		char *yct_serial,char *pki_card, char *ticket_amo_aft, char* result_message, BYTE* party, BYTE* rspcode);

	//监控配置
	int static P_SET_MONITOR(CGlobal *pgbl,const TSDLLPosInfo dPosInfo, const int com, const int comType,PYCT_DATA pyct_data);

	int static init_sign(CGlobal *pgbl , int& com , int& comType , YCT_DATA& yct_data);

	//初始化羊城通结构体
	int static init_yct_data(PYCT_DATA pyct_data);
	/***************************第三代羊城通充值新增函数结束*********************************************/

	CGlobal gbl;
	CPack pack;
	CTrade trade;
	SOCKET sd2;  //羊城通服务器通信socket
	char m_PKLogCard[8+1]; //充值管理卡(PK卡)逻辑卡号
	char m_YCTLogCard[16+1]; //票卡(羊城通卡)逻辑卡号
	char m_stan[8+1]; //系统流水号
	char m_YCTAmo[10]; //羊城通余额
	char m_stat[4+1];	//交易状态标识
	
	char YCTHOSTNAME[20];
	int  YCTPORT;
	
	int m_com;	//使用串口
	int m_comType;	//串口类型

};

#endif // !defined(AFX_YCTYPTRADE_H__BA427F3F_89A8_429A_BBBD_C794CFDB3EE3__INCLUDED_)
