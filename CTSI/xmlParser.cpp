// xmlParser.cpp: implementation of the CxmlParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "xmlParser.h"
#include "TSCTSI.h"
#include <winsock2.h>
#include "Global.h"
#include "tinystr.h" //add by liuxiao
#include "tinyxml.h" //add by liuxiao
#include "cert.h"
#include "Tools.h"
#include "base64.h" 

#include <IOSTREAM>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//请求(地市查询(水电煤))
const field_cfg acReq_PublicTransQuery[] = 
{
    0,  "ORGANNO",      "", "MERID",        "VerifyParameter",    "", //机构号
    0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值80
    0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
    1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
    0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
    0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
    0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
    0,  "TRANSDT",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
    0,  "TERMINALNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
    0,  "PCITYCODE",    "",             "ACCEPTAREACODE",   "PARAMETERS", "PayPlatRequestParameter", //受理区域编码 
    0,  "TRANSDT",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //受理日期 yyyyMMddhhmmss
    1,  "REMARK1",      "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //预留域1
    1,  "REMARK2",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //预留域2
    0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};

//请求(账单查询(水电煤))
const field_cfg acReq_PublicBillQuery[] = 
{
    0,  "ORGANNO",      "", "MERID",        "VerifyParameter",    "", //机构号
    0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值80
    0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
    1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
    0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
    0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
    0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
    0,  "TRANSDT",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
    
    0,  "LINENO",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //流水号 
    0,  "TERMINALNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
    0,  "SELECTTYPE",   "",             "SELECTTYPE",       "PARAMETERS", "PayPlatRequestParameter", //查询类型：001:用户号，002:条形码
    0,  "TRIORDERNO",   "",             "SELECTVALUE",      "PARAMETERS", "PayPlatRequestParameter", //用户号或条形码,根据查询类型填写相应的值
    0,  "TRANSDT",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //受理日期 yyyyMMddhhmmss
    0,  "PCITYCODE",    "",             "ACCEPTAREACODE",   "PARAMETERS", "PayPlatRequestParameter", //受理区域编码 
    0,  "PAYMENTCODE",  "",             "ADDITEM1",         "PARAMETERS", "PayPlatRequestParameter", //缴费省代码（6位）+缴费城市代码（6位）+缴费项目编号（4位）
    0,  "QUERYAMOUNT",  "",             "ADDITEM2",         "PARAMETERS", "PayPlatRequestParameter", //上海水和煤气要求必须输入 
    1,  "TICKETTITLE",  "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //预留域1,北京供暖作为发票台头使用，而且必须输入。
    1,  "REMARK2",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //预留域2
    1,  "REMARK3",      "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //预留域3
    1,  "REMARK4",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //预留域4
    0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};

//请求(缴费(水电煤))
const field_cfg acReq_PublicPay[] = 
{
    0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
    0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值80
    0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
    1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
    0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
    0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
    0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
    0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
    0,  "ORDERSEQ",       "",            "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //流水号 
    0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
    0,  "ACCEPTDATE",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //受理日期 yyyyMMddhhmmss
    0,  "SYSTEMNO",  "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //系统参考号
    0,  "TXNAMOUNT",       "",             "TXNAMOUNT",        "PARAMETERS", "PayPlatRequestParameter", //金额
    0,  "CASHTYPE",     "",             "CASHTYPE",         "PARAMETERS", "PayPlatRequestParameter", //销账单号类型,0-条码  1-缴费单号 
    0,  "CASHORDER",   "",             "CASHORDER",        "PARAMETERS", "PayPlatRequestParameter", //账期
	0,  "CASHNUMBER",   "",             "CASHNUMBER",        "PARAMETERS", "PayPlatRequestParameter", //销账单号
	0,  "PAYTYPE",      "",             "PAYTYPE",          "PARAMETERS", "PayPlatRequestParameter", //支付方式, 送"1"
    0,  "ECARDNO",   "",             "ECARDNO",        "PARAMETERS", "PayPlatRequestParameter", //E卡信息
	0,  "PASSFLAG",   "",             "PASSFLAG",        "PARAMETERS", "PayPlatRequestParameter", //密码类型   
    0,  "PSAMCARDNO",        "",             "PSAMCARDNO",       "PARAMETERS", "PayPlatRequestParameter", //PSAM卡号
	0,  "OPERUSER",        "",             "OPERUSER",       "PARAMETERS", "PayPlatRequestParameter", //企业账户操作元
	0,  "OPERPASSWORD",        "",             "OPERPASSWORD",       "PARAMETERS", "PayPlatRequestParameter", //企业账户操作员密码
    1,  "REMARK1",      "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //预留域1
    1,  "REMARK2",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //预留域2
    0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


// 车船税 账单查询报文
const field_cfg acReq_carBillQuery[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //交易发起方流水号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		1,  "PLATENO",    "",             "PLATENO",           "PARAMETERS", "PayPlatRequestParameter", //车牌号
		0,  "ENGINENO",      "",             "ENGINENO",       "PARAMETERS", "PayPlatRequestParameter", //发动机号
		0,  "ACCEPTDATE",      "",            "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //商户受理时间
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//车船税——账单应答报文
const field_cfg acRsp_carBillQuery[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述            
		0,  "SYSTEMNO",       "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //系统参考号
		0,  "BILLNUM",        "",                 "BILLNUM",           "RESULTDATESET",    "PayPlatResponseParameter", //账单记录数
		0,  "TMNNUMNO",          "",                 "TMNNUMNO",       "RESULTDATESET",    "PayPlatResponseParameter", //受理时间
		0,  "DATAS",            "",                 "DATAS",            "RESULTDATESET",    "PayPlatResponseParameter", //客户终端号
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};

//车船税账单缴费请求报文

const field_cfg acReq_carTaxPay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值80
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //流水号 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E卡
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //默认为2
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //psam卡号
		0,  "ACCEPTDATE",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //受理日期 yyyyMMddhhmmss
		0,  "SYSTEMNO",  "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //系统参考号
		0,  "TXNAMOUNT",       "",             "TXNAMOUNT",        "PARAMETERS", "PayPlatRequestParameter", //金额
		1,  "PLATENO",     "",             "PLATENO",         "PARAMETERS", "PayPlatRequestParameter", //车牌号8位 
		0,  "PAYTYPE",      "",             "PAYTYPE",          "PARAMETERS", "PayPlatRequestParameter", //支付方式, 送"3"    
		0,  "OPERUSER",        "",             "OPERUSER",       "PARAMETERS", "PayPlatRequestParameter", //企业账户操作员
		0,  "OPERPASSWORD",       "",             "OPERPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //企业账户操作元密码
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//银行卡信息查询
const field_cfg acReq_bankInfoQry[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值80
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "CUSTCODE",       "",             "CUSTCODE",         "PARAMETERS", "PayPlatRequestParameter",  
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", 
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//查询银行卡应发报文
const field_cfg acRsp_bankInfoQry[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "RESULTDATASET",       "",					"RESULTDATASET",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//车船税缴费应答报文
const field_cfg acRsp_carTaxPay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "SYSTEMNO",      "",					 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //还款金额
		1,  "PLATENO",         "",                 "PLATENO",         "RESULTDATESET",    "PayPlatResponseParameter", //销账单号类型,0-条码  1-缴费单号
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//3G流量卡充值请求报文
const field_cfg acReq_g3Pay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERNO",       "",             "ORDERNO",         "PARAMETERS", "PayPlatRequestParameter", //订单号 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E卡信息
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //密码类型
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAM卡
		0,  "STAFFCODE",      "",             "STAFFCODE",       "PARAMETERS", "PayPlatRequestParameter", //用户名
		0,  "PAYPASSWORD",  "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //交易密码
		0,  "RECHARGETYPE",       "",             "RECHARGETYPE",        "PARAMETERS", "PayPlatRequestParameter", //充值类型
		0,  "VERIFY",       "",             "VERIFY",        "PARAMETERS", "PayPlatRequestParameter", //是否需要验证手机号 
		0,  "PHONE",     "",             "PHONE",         "PARAMETERS", "PayPlatRequestParameter",		//充入手机号码		
		0,  "RECHARGEFLOW",      "",             "RECHARGEFLOW",          "PARAMETERS", "PayPlatRequestParameter", //充入流量   
		0,  "ACCEPTAREACODE",        "",             "ACCEPTAREACODE",       "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "TXNAMOUNT",        "",             "TXNAMOUNT",       "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "TRADETIME",       "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //交易受理时间
		0,  "SYSTEMNO",       "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//全国话费充值请求报文

const field_cfg acReq_phonePay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERNO",       "",             "ORDERNO",         "PARAMETERS", "PayPlatRequestParameter", //订单号 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E卡信息
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //密码类型
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAM卡
		0,  "STAFFCODE",      "",             "STAFFCODE",       "PARAMETERS", "PayPlatRequestParameter", //用户名
		0,  "PAYPASSWORD",  "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //交易密码
		0,  "RECHARGETYPE",       "",             "RECHARGETYPE",        "PARAMETERS", "PayPlatRequestParameter", //充值类型 03010008：全国电信直充 05010005全国移动直充1601000 全国联通直充
		0,  "PHONE",     "",             "PHONE",         "PARAMETERS", "PayPlatRequestParameter",		//充入手机号码		
		0,  "RECHARGEAMOUNT",      "",             "RECHARGEAMOUNT",          "PARAMETERS", "PayPlatRequestParameter", //充入金额
		0,  "ACCEPTAREACODE",        "",             "ACCEPTAREACODE",       "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "TXNAMOUNT",        "",             "TXNAMOUNT",       "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "TRADETIME",       "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //交易受理时间
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};

//移动（广东省内）手机话费充值请求报文
const field_cfg acReq_phonePayGDYD[] = 
{
	0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //订单号 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E卡信息
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //密码类型
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAM卡
		0,  "STAFFCODE",      "",             "STAFFCODE",       "PARAMETERS", "PayPlatRequestParameter", //用户名
		0,  "PAYPASSWORD",  "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //交易密码
		0,  "PHONE",     "",             "PHONE",         "PARAMETERS", "PayPlatRequestParameter",		//充入手机号码
		0,  "RECHARGETYPE",     "",             "RECHARGETYPE",         "PARAMETERS", "PayPlatRequestParameter",		//充值类型
		0,  "TXAMOUNT",        "",             "TXAMOUNT",       "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "TRADETIME",       "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //交易受理时间
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};

//账户验证查询接口 
const field_cfg acReq_g3PhoneCheck[] = 
{
	0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "VERIFY",       "",             "VERIFY",        "PARAMETERS", "PayPlatRequestParameter", //0:个人账户	1：3G流量充值不填写默认为个人账户验证
		0,  "ACCTCODE",     "",             "ACCTCODE",         "PARAMETERS", "PayPlatRequestParameter", //账户号码
		0,  "ACCEPTAREACODE",     "",             "ACCEPTAREACODE",         "PARAMETERS", "PayPlatRequestParameter",
		0,  "ACCEPTTIME",     "",             "ACCEPTTIME",         "PARAMETERS", "PayPlatRequestParameter",		
		0,  "ACCEPTDATE",       "",             "ACCEPTDATE",         "PARAMETERS", "PayPlatRequestParameter", //交易受理时间
		0,  "REAMOUNT",       "",             "REAMOUNT",         "PARAMETERS", "PayPlatRequestParameter", 
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//东莞公交订单确认接口
const field_cfg acReq_dgt_orderVerify[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //商户平台生成的订单号
		0,  "TRANSSEQ",       "",             "TRANSSEQ",        "PARAMETERS", "PayPlatRequestParameter", //交易流水号
		0,  "TMNNUMNO",     "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //客户终端号
		0,  "BUSCODE",     "",             "BUSCODE",         "PARAMETERS", "PayPlatRequestParameter", //业务操作代码
		1,  "BUSREASON",     "",             "BUSREASON",         "PARAMETERS", "PayPlatRequestParameter",	//业务操作描述	
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//东莞公交通知订单生成接口 请求

const field_cfg acReq_dgt_orderProduce[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
	
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //订单号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "STAFFCODE",       "",             "STAFFCODE",        "PARAMETERS", "PayPlatRequestParameter", //用户名
		0,  "TRADETIME",     "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //受理时间
		0,  "EDCARDID",     "",             "EDCARDID",         "PARAMETERS", "PayPlatRequestParameter", //M1卡号  
		0,  "EACOUNT",     "",             "EACOUNT",         "PARAMETERS", "PayPlatRequestParameter",	//M1金额	
		0,  "RETURNACOUNT",       "",             "RETURNACOUNT",         "PARAMETERS", "PayPlatRequestParameter", //写入新卡余额  
		0,  "RETURNDEPOSIT",       "",             "RETURNDEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //退用户押金
		0,  "MONTHFEFLAG",       "",             "MONTHFEFLAG",         "PARAMETERS", "PayPlatRequestParameter", //月费收取标志 
		0,  "FEEACOUNTFLAG",       "",             "FEEACOUNTFLAG",         "PARAMETERS", "PayPlatRequestParameter", //手续费收取标志
		0,  "MONTHFEEACOUNT",       "",             "MONTHFEEACOUNT",         "PARAMETERS", "PayPlatRequestParameter", //卡成本或月费 
		0,  "FEEACOUNT",       "",             "FEEACOUNT",         "PARAMETERS", "PayPlatRequestParameter", //手续费 
		0,  "BATCHNO",       "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //批次号 
		0,  "POSID",       "",             "POSID",         "PARAMETERS", "PayPlatRequestParameter", //清算设备代码 
		0,  "CARDID",       "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //CPU卡号 
		0,  "CARDSELLDATE",       "",             "CARDSELLDATE",         "PARAMETERS", "PayPlatRequestParameter", //售卡日期 
		0,  "TRANSCMESSAGE",       "",             "TRANSCMESSAGE",         "PARAMETERS", "PayPlatRequestParameter", //交易类型
		0,  "BALANCEBEFORE",       "",             "BALANCEBEFORE",         "PARAMETERS", "PayPlatRequestParameter", //CPU卡余额
		0,  "TXNDATE",       "",             "TXNDATE",         "PARAMETERS", "PayPlatRequestParameter", //交易日期
		0,  "TXNTIME",       "",             "TXNTIME",         "PARAMETERS", "PayPlatRequestParameter", //交易时间
		0,  "DATEOFSETTLEMENT",       "",             "DATEOFSETTLEMENT",         "PARAMETERS", "PayPlatRequestParameter", //结算日期
		0,  "DEPOSIT",       "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //押金
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};


//东莞公交查询接口。用于卡操作时出现超时情况
const field_cfg acReq_dgt_keepQry[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //订单号
		0,  "KEEPCODE",   "",             "KEEPCODE",         "PARAMETERS", "PayPlatRequestParameter", //keepcode
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};

//广璐qq业务请求

const field_cfg acReq_QQ_guanglu_pay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //订单号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //TMNNUMNO
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //STAFFCODE
		0,  "PAYPASSWORD",   "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //PAYPASSWORD
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //PASSFLAG
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //ECARDNO
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAMCARDNO
		
		0,  "PRODUCTCODE",   "",             "PRODUCTCODE",         "PARAMETERS", "PayPlatRequestParameter", //PRODUCTCODE
		0,  "REQIP",   "",             "REQIP",         "PARAMETERS", "PayPlatRequestParameter", //REQIP
		0,  "ACCTCODE",   "",             "ACCTCODE",         "PARAMETERS", "PayPlatRequestParameter", //ACCTCODE
		0,  "RECHNUM",   "",             "RECHNUM",         "PARAMETERS", "PayPlatRequestParameter", //ACCTCODE
		0,  "RECHAMOUNT",   "",             "RECHAMOUNT",         "PARAMETERS", "PayPlatRequestParameter", //RECHAMOUNT
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //TRADETIME
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
};

//广璐qq业务返回
const field_cfg acRsp_QQ_guanglu_pay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TRADETIME",       "",					"TRADETIME",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TXNAMOUNT",         "",                 "TXNAMOUNT",         "RESULTDATESET",    "PayPlatResponseParameter", //交易金额 
		0,  "SYSTEMNO",         "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //交易金额 
		0,  "PAYAMOUNT",         "",                 "PAYAMOUNT",         "RESULTDATESET",    "PayPlatResponseParameter", //实际交易金额
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};
 

//东莞公交查询返回报文

const field_cfg acRsp_dgt_keepQry[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "SERIALNUM",       "",					"SERIALNUM",         "RESULTDATESET",    "PayPlatResponseParameter", //SERIALNUM
		0,  "SYSTEMNO",           "",                "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "POSID",         "",                 "POSID",         "RESULTDATESET",    "PayPlatResponseParameter", //交易金额 
		0,  "SAMID",         "",                 "SAMID",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "STAFFCODE",         "",                 "STAFFCODE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "EDCARDID",         "",                 "EDCARDID",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDOPRTYPE",         "",                 "CARDOPRTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CITYCODE",         "",                 "CITYCODE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDID",         "",                 "CARDID",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDMKND",         "",                 "CARDMKND",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDSKND",         "",                 "CARDSKND",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDMODEL",         "",                 "CARDMODEL",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "TRANSTYPE",         "",                 "TRANSTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "AUTHSEQ",         "",                 "AUTHSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "DICMAC",         "",                 "DICMAC",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "MAC2",         "",                 "MAC2",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "LIMITEAUTHSEQL",         "",                 "LIMITEAUTHSEQL",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "BATCHNO",         "",                 "BATCHNO",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "SETTDATE",         "",                 "SETTDATE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "ORDERSTAT",         "",                 "ORDERSTAT",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "PAYSTAT",         "",                 "PAYSTAT",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "BUSSTAT",         "",                 "BUSSTAT",         "RESULTDATESET",    "PayPlatResponseParameter",
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};

//东莞公交异步订单确认应答报文

const field_cfg acRsp_dgt_orderVerify[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TXNAMOUNT",         "",                 "TXNAMOUNT",         "RESULTDATESET",    "PayPlatResponseParameter", //交易金额 
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//东莞公交异步订单生成应答报文
const field_cfg acRsp_dgt_orderProduce[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "EDCARDID",         "",                 "EDCARDID",         "RESULTDATESET",    "PayPlatResponseParameter", //M1卡号 
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};



//账户验证查询接口应答报文
const field_cfg acRsp_g3PhoneCheck[] = 
{
	0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "SYSTEMNO",         "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//3G流量卡请求应答报文
const field_cfg acRsp_G3Pay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //订单金额
		0,  "PHONE",         "",                 "PHONE",         "RESULTDATESET",    "PayPlatResponseParameter", //手机号码
		0,  "RECHARGEFLOW",       "",					 "RECHARGEFLOW",        "RESULTDATESET",    "PayPlatResponseParameter", //充入流量
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//全国话费充值请求应答报文
const field_cfg acRsp_phonePay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //订单金额
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};

//移动（广东省内）手机话费充值应答报文
const field_cfg acRsp_phonePayGDYD[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TRADETIME",           "",               "TRADETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //订单金额
		0,  "SYSTEMNO",           "",               "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //系统参考号
		0,  "TXAMOUNT",           "",               "TXAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //充入金额
		0,  "PAYAMOUNT",           "",               "PAYAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //实际交易金额
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};

//应答(地市查询(水电煤))
const field_cfg acRsp_PublicTransQuery[] = 
{
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
    0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
    0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
    0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
    0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
    0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
    1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述            
    0,  "TERMINALNO",       "",                 "TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
    0,  "PCITYCODE",        "",                 "ACCEPTAREACODE",   "RESULTDATESET",    "PayPlatResponseParameter", //受理区域编码
    0,  "TRANSDT",          "",                 "ACCEPTDATE",       "RESULTDATESET",    "PayPlatResponseParameter", //受理时间
    0,  "DATAS",            "",                 "DATAS",            "RESULTDATESET",    "PayPlatResponseParameter", //
    1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
    1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
    0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//水电煤账单查询 应答报文
const field_cfg acRsp_PublicBillQuery[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "SYSTEMNO",      "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //系统参考号
		0,  "BILLNUM",          "",                 "BILLNUM",          "RESULTDATESET",    "PayPlatResponseParameter", //账单记录条数		
		0,  "DATAS",            "",                 "DATAS",            "RESULTDATESET",    "PayPlatResponseParameter", //	
		0,  "TERMINALNO",       "",                 "TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "BILLSTAT",       "",					 "BILLSTAT",         "RESULTDATESET",    "PayPlatResponseParameter", //账单状态
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//水电煤缴费应答报文
const field_cfg acRsp_PublicPay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //流水号 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "SYSTEMNO",      "",					 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //还款金额
		0,  "CASHTYPE",         "",                 "CASHTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", //销账单号类型,0-条码  1-缴费单号
		0,  "CASHORDER",       "",					 "CASHORDER",        "RESULTDATESET",    "PayPlatResponseParameter", //销账单号
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};



//东莞公交签到信息请求报文 获取KEYSET 

const field_cfg acReq_DGTSignIn[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //发起方交易流水号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //操作员
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //受理时间
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）

};


//东莞公交签到应答报文
const field_cfg acRsp_DGTSignIn[] = 
{

		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //终端流水号
		0,  "POSID",         "",					 "POSID",			"RESULTDATESET",    "PayPlatResponseParameter", //清算设备代码
		1,  "KEYSET",      "",						"KEYSET",         "RESULTDATESET",    "PayPlatResponseParameter", //密钥信息
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM卡号
		0,  "SAMAUTHINFO",         "",                 "SAMAUTHINFO",         "RESULTDATESET",    "PayPlatResponseParameter", //SAM卡授权信息
		0,  "EDAUTHINFO",       "",					 "EDAUTHINFO",        "RESULTDATESET",    "PayPlatResponseParameter", //ED卡授权信息
		0,  "SETTDATE",       "",					 "SETTDATE",        "RESULTDATESET",    "PayPlatResponseParameter", //结算日期
		0,  "BATCHNO",       "",					 "BATCHNO",        "RESULTDATESET",    "PayPlatResponseParameter", //批次号
		0,  "SYSDATETIME",       "",					 "SYSDATETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //批次号
		0,  "AUTHCODE",       "",					 "AUTHCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //授权认证码
		0,  "PARAMBIT",       "",					 "PARAMBIT",        "RESULTDATESET",    "PayPlatResponseParameter", //参数更新标志位
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//东莞公交签退请求报文 

const field_cfg acReq_DGTSignOut[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //发起方交易流水号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //操作员
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //受理时间
		0,  "SETTDATE",   "",             "SETTDATE",         "PARAMETERS", "PayPlatRequestParameter", //结算日期
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //批次号
		0,  "TOTALSVNUM",   "",             "TOTALSVNUM",         "PARAMETERS", "PayPlatRequestParameter", //本批次累计售卡充值交易总笔数
		0,  "TOTALSVAMT",   "",             "TOTALSVAMT",         "PARAMETERS", "PayPlatRequestParameter", //本次累计充值交易总金额
		0,  "TOTALSALEDEP",   "",             "TOTALSALEDEP",         "PARAMETERS", "PayPlatRequestParameter", //本次累计售卡交易押金总金额
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
		
};


//东莞公交签退应答报文
const field_cfg acRsp_DGTSignOut[] = 
{
	
	0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //终端流水号
		0,  "POSID",         "",					 "POSID",			"RESULTDATESET",    "PayPlatResponseParameter", //清算设备代码
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM卡号
		0,  "EDCARDID",           "",               "EDCARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //操作主卡卡号
		0,  "SETTDATE",       "",					 "SETTDATE",        "RESULTDATESET",    "PayPlatResponseParameter", //结算日期
		0,  "BATCHNO",       "",					 "BATCHNO",        "RESULTDATESET",    "PayPlatResponseParameter", //批次号
		0,  "TOTALSVNUM",       "",					 "TOTALSVNUM",        "RESULTDATESET",    "PayPlatResponseParameter", //本批次累计售卡充资交易总笔数
		0,  "TOTALSVAMT",       "",					 "TOTALSVAMT",        "RESULTDATESET",    "PayPlatResponseParameter", //本次累计充值交易总金额（分）
		0,  "TOTALSALEDEP",       "",					 "TOTALSALEDEP",        "RESULTDATESET",    "PayPlatResponseParameter", //本次累计售卡交易押金总金额（分）
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//东莞公交卡操作请求报文
const field_cfg acReq_DGTCardOperate[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //发起方交易流水号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E卡号
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAMCARDNO	PSAM卡号
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //密码类型
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //用户名(工号)
		0,  "PAYPASSWORD",   "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //密码
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //受理时间
		0,  "CARDOPRTYPE",   "",             "CARDOPRTYPE",         "PARAMETERS", "PayPlatRequestParameter", //卡操作类型
		0,  "CITYCODE",   "",             "CITYCODE",         "PARAMETERS", "PayPlatRequestParameter", //城市代码
		0,  "CARDID",   "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //卡内号
		0,  "CARDMKND",   "",             "CARDMKND",         "PARAMETERS", "PayPlatRequestParameter", //卡主类型
		0,  "CARDSKND",   "",             "CARDSKND",         "PARAMETERS", "PayPlatRequestParameter", //子卡类型
		0,  "CARDMODEL",   "",             "CARDMODEL",         "PARAMETERS", "PayPlatRequestParameter", //卡型
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //业务类型
		0,  "DEPOSIT",   "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //押金
		0,  "ORIGAMT",   "",             "ORIGAMT",         "PARAMETERS", "PayPlatRequestParameter", //应收金额
		0,  "CARDVALDATE",   "",             "CARDVALDATE",         "PARAMETERS", "PayPlatRequestParameter", //卡有效期
		0,  "SRCBAL",   "",             "SRCBAL",         "PARAMETERS", "PayPlatRequestParameter", //卡有效期
		0,  "CARDSEQ",   "",             "CARDSEQ",         "PARAMETERS", "PayPlatRequestParameter", //卡片交易流水
		0,  "KEYVER",   "",             "KEYVER",         "PARAMETERS", "PayPlatRequestParameter", //卡片密钥版本
		0,  "ALGIND",   "",             "ALGIND",         "PARAMETERS", "PayPlatRequestParameter", //卡片算法标志
		0,  "CARDRAND",   "",             "CARDRAND",         "PARAMETERS", "PayPlatRequestParameter", //卡片随机数
		0,  "MAC1",   "",             "MAC1",         "PARAMETERS", "PayPlatRequestParameter", //充资返回信息MAC1
		0,  "DIVDATA",   "",             "DIVDATA",         "PARAMETERS", "PayPlatRequestParameter", //分散因子
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //批次号
		0,  "KEYSET",   "",             "KEYSET",         "PARAMETERS", "PayPlatRequestParameter", //密钥
		0,  "COMMAND",   "",             "COMMAND",         "PARAMETERS", "PayPlatRequestParameter", //APDU指令
		0,  "COMMANDLEN",   "",             "COMMANDLEN",         "PARAMETERS", "PayPlatRequestParameter", //APDU指令
		0,  "POSID",   "",             "POSID",         "PARAMETERS", "PayPlatRequestParameter", //POSID
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
		
};


//东莞公交卡操作应答报文

const field_cfg acRsp_DGTCardOperate[] = 
{
	
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //终端流水号
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM卡号
		0,  "STAFFCODE",           "",               "STAFFCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //操作员
		0,  "EDCARDID",       "",					 "EDCARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //操作主卡卡号
		0,  "CITYCODE",       "",					 "CITYCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //城市代码
		0,  "CARDID",       "",					 "CARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //卡内号
		0,  "CARDMKND",       "",					 "CARDMKND",        "RESULTDATESET",    "PayPlatResponseParameter", //主卡类型
		0,  "CARDSKND",       "",					 "CARDSKND",        "RESULTDATESET",    "PayPlatResponseParameter", //子卡类型
		0,  "CARDMODEL",       "",					 "CARDMODEL",        "RESULTDATESET",    "PayPlatResponseParameter", //卡型
		0,  "SYSTEMNO",       "",					 "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //交易流水号
		0,  "AUTHSEQ",       "",					 "AUTHSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //密钥授权流水号
		0,  "TRANSSEQ",       "",					 "TRANSSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //订单号
		0,  "LIMITEAUTHSEQL",       "",					 "LIMITEAUTHSEQL",        "RESULTDATESET",    "PayPlatResponseParameter", //额度授权流水号
		0,  "DICMAC",       "",					 "DICMAC",        "RESULTDATESET",    "PayPlatResponseParameter", //DICMAC
		0,  "MAC2",       "",					 "MAC2",        "RESULTDATESET",    "PayPlatResponseParameter", //DICMAC
		0,  "SYSDATETIME",       "",					 "SYSDATETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //中心时间
		0,  "MESSAGEDATETIME",       "",					 "MESSAGEDATETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //中心时间
		0,  "BATCHNO",       "",					 "BATCHNO",        "RESULTDATESET",    "PayPlatResponseParameter", //结算时间
		0,  "SETTDATE",       "",					 "SETTDATE",        "RESULTDATESET",    "PayPlatResponseParameter", //批次号
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};



//东莞公交报文及开卡冲正请求报文

const field_cfg acReq_DGTCardSale[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //发起方交易流水号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "OPERATIONTYPE",   "",             "OPERATIONTYPE",         "PARAMETERS", "PayPlatRequestParameter",  //操作类型	开卡：ATO 开卡冲正：ATR	不送默认为开卡ATO
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //操作员
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //业务类型
		0,  "TAC",   "",					"TAC",				 "PARAMETERS", "PayPlatRequestParameter", //交易押码
		0,  "SYSTEMNO",   "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //交易流水号
		0,  "APTRANSSEQ",   "",             "APTRANSSEQ",         "PARAMETERS", "PayPlatRequestParameter", //原交易流水号
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //业务类型
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //受理时间
		0,  "EDCARDID",   "",             "EDCARDID",         "PARAMETERS", "PayPlatRequestParameter", //操作主卡卡号
		0,  "CARDID",   "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //卡内号
		0,  "TXNDATE",   "",             "TXNDATE",         "PARAMETERS", "PayPlatRequestParameter", //交易日期
		0,  "TXNTIME",   "",             "TXNTIME",         "PARAMETERS", "PayPlatRequestParameter", //交易时间
		0,  "CARDCNT",   "",             "CARDCNT",         "PARAMETERS", "PayPlatRequestParameter", //卡计数器
		0,  "CARDMKND",   "",             "CARDMKND",         "PARAMETERS", "PayPlatRequestParameter", //卡主类型
		0,  "CARDSKND",   "",             "CARDSKND",         "PARAMETERS", "PayPlatRequestParameter", //卡子类型
		0,  "CARDMODEL",   "",             "CARDMODEL",         "PARAMETERS", "PayPlatRequestParameter", //卡型
		0,  "SALEMODE",   "",             "SALEMODE",         "PARAMETERS", "PayPlatRequestParameter", //售卡方式
		0,  "DEPOSIT",   "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //押金
		0,  "BEFBALANCE",   "",             "BEFBALANCE",         "PARAMETERS", "PayPlatRequestParameter", //交易前余额
		0,  "TXNAMT",   "",             "TXNAMT",         "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "ORIGAMT",   "",             "ORIGAMT",         "PARAMETERS", "PayPlatRequestParameter", //交易金额
		0,  "CARDVALDATE",   "",             "CARDVALDATE",         "PARAMETERS", "PayPlatRequestParameter", //卡有效期
		0,  "CITYCODE",   "",             "CITYCODE",         "PARAMETERS", "PayPlatRequestParameter", //卡内城市代码
		0,  "CARDVERNO",   "",             "CARDVERNO ",         "PARAMETERS", "PayPlatRequestParameter", //卡内版本号
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //批次号
		0,  "AUTHSEQ",   "",             "AUTHSEQ",         "PARAMETERS", "PayPlatRequestParameter", //授权流水号
		0,  "LIMITEDAUTHSEQL",   "",             "LIMITEDAUTHSEQL",         "PARAMETERS", "PayPlatRequestParameter", //额度授权流水号
		0,  "KEYSET",   "",             "KEYSET",         "PARAMETERS", "PayPlatRequestParameter", //密钥
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
		
};

//东莞公交开卡及冲正 应答
const field_cfg acRsp_DGTCardSale[] = 
{
	
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "STAFFCODE",       "",					"STAFFCODE",         "RESULTDATESET",    "PayPlatResponseParameter", //操作员
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //终端流水号
		0,  "POSID",         "",					 "POSID",			"RESULTDATESET",    "PayPlatResponseParameter", //清算设备代码
		0,  "TRANSTYPE",         "",					 "TRANSTYPE",			"RESULTDATESET",    "PayPlatResponseParameter", //业务类型
		1,  "KEYSET",      "",						"KEYSET",         "RESULTDATESET",    "PayPlatResponseParameter", //密钥信息
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM卡号
		0,  "EDCARDID",         "",                 "EDCARDID",         "RESULTDATESET",    "PayPlatResponseParameter", //操作主卡卡号
		0,  "CITYCODE",       "",					 "CITYCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //城市代码
		0,  "CARDID",       "",					 "CARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //卡内号
		0,  "CARDMKND",       "",					 "CARDMKND",        "RESULTDATESET",    "PayPlatResponseParameter", //主卡类型
		0,  "CARDSKND",       "",					 "CARDSKND",        "RESULTDATESET",    "PayPlatResponseParameter", //子卡类型
		0,  "SYSTEMNO",       "",					 "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //子卡类型
		0,  "APTRANSSEQ",       "",					 "APTRANSSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //子卡类型
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//东莞公交充值及冲正报文
const field_cfg acReq_DGTCardCharge[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //机构号
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //渠道号，标识渠道类型，定值60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //受理终端号，和机构号一一对应
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务名称
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //服务编码
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //格式：省份-应用系统名称-版本号-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //终端号+yyyyMMddhhmmss+4位流水号
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //发起方交易流水号
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //终端号
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //操作员
		0,  "OPERATIONTYPE",   "",             "OPERATIONTYPE",         "PARAMETERS", "PayPlatRequestParameter", //操作类型
		0,  "SYSTEMNO",   "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //操作类型
		0,  "APTRANSSEQ",   "",             "APTRANSSEQ",         "PARAMETERS", "PayPlatRequestParameter", //原交易流水号
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //原交易流水号
		0,  "TAC",   "",					 "TAC",				"PARAMETERS", "PayPlatRequestParameter", //交易押码
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //受理时间
		0,  "EDCARDID",   "",             "EDCARDID",         "PARAMETERS", "PayPlatRequestParameter", //操作主卡卡号
		0,  "CARDID",   "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //卡内号
		0,  "TXNDATE",   "",             "TXNDATE",         "PARAMETERS", "PayPlatRequestParameter", //交易日期
		0,  "TXNTIME",   "",             "TXNTIME",         "PARAMETERS", "PayPlatRequestParameter", //交易时间
		0,  "CARDCNT",   "",             "CARDCNT",         "PARAMETERS", "PayPlatRequestParameter", //卡计数器
		0,  "CARDMKND",   "",             "CARDMKND",         "PARAMETERS", "PayPlatRequestParameter", //卡主类型
		0,  "CARDSKND",   "",             "CARDSKND",         "PARAMETERS", "PayPlatRequestParameter", //卡子类型
		0,  "CARDMODEL",   "",             "CARDMODEL",         "PARAMETERS", "PayPlatRequestParameter", //卡型 1  CPU卡3  M1卡 
		0,  "BEFBALANCE",   "",             "BEFBALANCE",         "PARAMETERS", "PayPlatRequestParameter", //交易前余额
		0,  "ORIGAMT",   "",             "ORIGAMT",         "PARAMETERS", "PayPlatRequestParameter", //应收金额
		0,  "TXNAMT",   "",             "TXNAMT",         "PARAMETERS", "PayPlatRequestParameter", //实收金额
		0,  "HANDINGCHARGE",   "",             "HANDINGCHARGE",         "PARAMETERS", "PayPlatRequestParameter", //手续费 
		0,  "DEPOSIT",   "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //押金
		0,  "CARDVALDATE",   "",             "CARDVALDATE",         "PARAMETERS", "PayPlatRequestParameter", //卡有效期
		0,  "CITYCODE",   "",             "CITYCODE",         "PARAMETERS", "PayPlatRequestParameter", //卡内城市代码
		0,  "CARDVERNO",   "",             "CARDVERNO",         "PARAMETERS", "PayPlatRequestParameter", //卡内版本号
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //批次号
		0,  "AUTHSEQ",   "",             "AUTHSEQ",         "PARAMETERS", "PayPlatRequestParameter", //授权流水号
		0,  "LIMITEDAUTHSEQL",   "",             "LIMITEDAUTHSEQL",         "PARAMETERS", "PayPlatRequestParameter", //额度授权流水号
		0,  "LASTPOSSVSEQ",   "",             "LASTPOSSVSEQ",         "PARAMETERS", "PayPlatRequestParameter", //POS终端上笔充值流水号 ？！
		0,  "KEYSET",   "",             "KEYSET",         "PARAMETERS", "PayPlatRequestParameter", //密钥
		0,  "",             "",             "",                 "", "", //数组结束标志（这个和报文无关）
		
};


//东莞公交充值及冲正 应答报文
const field_cfg acRsp_DGTCardCharge[] = 
{
	
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //签名
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //公钥
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //查询类响应为20，其余都为10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //取自对应请求KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //如果成功”SUCESS”，如果失败”FAIL”    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //响应码 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //响应码描述        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //终端号
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //终端流水号
		0,  "STAFFCODE",           "",                "STAFFCODE",         "RESULTDATESET",    "PayPlatResponseParameter", //操作员
		0,  "TRANSTYPE",           "",                "TRANSTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", //业务类型
		0,  "POSID",           "",                "POSID",         "RESULTDATESET",    "PayPlatResponseParameter", //清算设备代码
		0,  "CARDID",           "",               "CARDID",        "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "EDCARDID",       "",					 "EDCARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //操作主卡卡号
		0,  "SAMID",       "",					 "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", ////SAM卡号
		0,  "CITYCODE",       "",					 "CITYCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //城市代码
		0,  "CARDMKND",       "",					 "CARDMKND",        "RESULTDATESET",    "PayPlatResponseParameter", //主卡类型
		0,  "CARDSKND",       "",					 "CARDSKND",        "RESULTDATESET",    "PayPlatResponseParameter", //CARDSKND	子卡类型
		0,  "SYSTEMNO",       "",					 "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //CARDSKND	子卡类型
		0,  "APTRANSSEQ",       "",					 "APTRANSSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //CARDSKND	子卡类型		
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //预留域2
		0,  "",                 "",                 "",                 "",                 "", //数组结束标志（这个和报文无关）
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxmlParser::CxmlParser()
{

}

CxmlParser::~CxmlParser()
{
	

}


int CxmlParser:: packAreaQryxml(CGlobal gbl,char *OUTTMNNUMNO,char * areaCode,char *ACCEPTDATE, string &strReq0,string &strReq1,char * WEBSVRNAME,char *WEBSVRCODE)
{	

	//strReq0="INF06002|440100-APP001-001-132.97.117.170|APP001|132.97.117.170|"; 
	string data_sign=""; //数字签名信息
	string data_sign_base64=""; //64编码字符串
	string CER_Str=""; //公钥信息
	string platString=""; //平台数据
	char keep_str[32+1];
	int iRet=0;
	bbzero(keep_str);
	//CTools::get_keep(keep_str);
	CTools::get_keep(gbl.pos_no,keep_str);
	char strReqTmp[100];
	char APPFROMTmp[50];
	bbzero(APPFROMTmp);
	bbzero(strReqTmp);
	sprintf(strReqTmp,"INF06002|440100-APP001-001-%s|APP001|%s|",gbl.server_ip,gbl.server_ip);
	sprintf(APPFROMTmp,"440100-APP001-001-%s",gbl.server_ip);
	strReq0.assign(strReqTmp,strlen(strReqTmp));
	
	//PayPlatRequestParameter需要用商户私钥进行加密
	TiXmlDocument *PayPlatDoc=new TiXmlDocument;
	TiXmlElement *PayPlatRequestParameter=new TiXmlElement("PayPlatRequestParameter");
	PayPlatDoc->LinkEndChild(PayPlatRequestParameter);
	TiXmlElement *CTRL_INFO =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO->SetAttribute("WEBSVRNAME",WEBSVRNAME); //服务名称
	CTRL_INFO->SetAttribute("WEBSVRCODE",WEBSVRCODE); //服务器编码
	CTRL_INFO->SetAttribute("APPFROM",APPFROMTmp); //请求本服务的应用标识码

	CTRL_INFO->SetAttribute("KEEP",keep_str); //本次服务的标识流水，参考规则:机构号+终端号+YYMMDDHHMMSS+随机数
	CTRL_INFO->SetAttribute("REQUESTTIME",ACCEPTDATE); //请求时间
	
	PayPlatRequestParameter->LinkEndChild(CTRL_INFO);
	
	TiXmlElement *PARAMETERS =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter->LinkEndChild(PARAMETERS);
	
	//水电煤地区查询接口平台参数PARAMETERS
	TiXmlElement *OUTTMNNUMNOEle=new TiXmlElement("TMNNUMNO");
	TiXmlElement * ACCEPTAREACODEEle=new TiXmlElement("ACCEPTAREACODE");
	TiXmlElement *ACCEPTDATEEle=new TiXmlElement("ACCEPTDATE");
	PARAMETERS->LinkEndChild(OUTTMNNUMNOEle);
	PARAMETERS->LinkEndChild(ACCEPTAREACODEEle);
	PARAMETERS->LinkEndChild(ACCEPTDATEEle);

	TiXmlText *TMNNUMNOTxt=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle->LinkEndChild(TMNNUMNOTxt);

	TiXmlText *ACCEPTAREACODETXT=new TiXmlText(areaCode);
	ACCEPTAREACODEEle->LinkEndChild(ACCEPTAREACODETXT);
	TiXmlText *ACCEPTDATETXT=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle->LinkEndChild(ACCEPTDATETXT);
	TiXmlPrinter platPrinter;
	platPrinter.SetLineBreak(NULL);
	platPrinter.SetIndent(0);
	PayPlatDoc->Accept(&platPrinter);
	platString=platPrinter.CStr();
    wrLog("platString:%s",platString.c_str());
	//平台数据转utf-8
	ANSItoUTF8(platString);
	//数字签名
	Ccert::data_sign("privateKey.pem",platString,data_sign);
	data_sign_base64=base64_encode(data_sign);
	wrLog("data_sign_64:%s",data_sign_base64.c_str());
	//读取公钥信息
	Ccert::cert_key("cert.pem",CER_Str);
	wrLog("公钥信息:%s",CER_Str.c_str());
	Ccert::cert_verify_file("cert.pem",platString.c_str(),data_sign);

	TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlDeclaration  *pDelaration=new TiXmlDeclaration("1.0","UTF-8",""); //UTF-8
	pDoc->LinkEndChild(pDelaration); //添加根节点
	TiXmlElement *Request=new TiXmlElement("Request"); 
	pDoc->LinkEndChild(Request); //添加根节点
	TiXmlElement *VerifyParameter=new TiXmlElement("VerifyParameter");
	Request->LinkEndChild(VerifyParameter);
	
	TiXmlElement *MERIEle=new TiXmlElement("MERID");
	TiXmlText *MERIDContent=new TiXmlText(MERID); //机构编码
	MERIEle->LinkEndChild(MERIDContent);

	VerifyParameter->LinkEndChild(MERIEle);
	TiXmlElement * SIGN=new TiXmlElement("SIGN");
	VerifyParameter->LinkEndChild(SIGN);
	TiXmlText * SIGN_Content=new TiXmlText(data_sign_base64.c_str());
	wrLog("data_sign_base64:%s",data_sign_base64.c_str());
	SIGN->LinkEndChild(SIGN_Content);

	TiXmlElement *TMNNUMNOEle=new TiXmlElement("TMNNUM");
	VerifyParameter->LinkEndChild(TMNNUMNOEle);
	TiXmlText *TMNNUM_CONTENT=new TiXmlText(TMNNUM); //机构对应的接入终端号
	TMNNUMNOEle->LinkEndChild(TMNNUM_CONTENT);

	TiXmlElement * CHANNELCODE =new TiXmlElement("CHANNELCODE"); //渠道号码
	VerifyParameter->LinkEndChild(CHANNELCODE);
	TiXmlText *CHANNEL_CODE_CONTENT=new TiXmlText("60");
	CHANNELCODE->LinkEndChild(CHANNEL_CODE_CONTENT);

	TiXmlElement *CER=new TiXmlElement("CER");	//公钥信息
	VerifyParameter->LinkEndChild(CER);
	TiXmlText *CER_CONTENT=new TiXmlText(CER_Str.c_str());
	CER->LinkEndChild(CER_CONTENT);
	
	//PayPlatRequestParameter需要用商户私钥进行加密
	
	TiXmlElement *PayPlatRequestParameter1=new TiXmlElement("PayPlatRequestParameter");
	Request->LinkEndChild(PayPlatRequestParameter1);
	TiXmlElement *CTRL_INFO1 =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO1->SetAttribute("WEBSVRNAME",WEBSVRNAME); //服务名称
	CTRL_INFO1->SetAttribute("WEBSVRCODE",WEBSVRCODE); //服务器编码
	CTRL_INFO1->SetAttribute("APPFROM",APPFROMTmp); //请求本服务的应用标识码
	CTRL_INFO1->SetAttribute("KEEP",keep_str); //本次服务的标识流水，参考规则:终端号+YYMMDDHHMMSS+随机数
	CTRL_INFO1->SetAttribute("REQUESTTIME",ACCEPTDATE); //请求时间
	
	PayPlatRequestParameter1->LinkEndChild(CTRL_INFO1);
	
	TiXmlElement *PARAMETERS1 =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter1->LinkEndChild(PARAMETERS1);
	
	//水电煤地区查询接口平台参数PARAMETERS
	TiXmlElement *OUTTMNNUMNOEle1=new TiXmlElement("TMNNUMNO");
	TiXmlElement * ACCEPTAREACODEEle1=new TiXmlElement("ACCEPTAREACODE");
	TiXmlElement *ACCEPTDATEEle1=new TiXmlElement("ACCEPTDATE");
	PARAMETERS1->LinkEndChild(OUTTMNNUMNOEle1);
	PARAMETERS1->LinkEndChild(ACCEPTAREACODEEle1);
	PARAMETERS1->LinkEndChild(ACCEPTDATEEle1);
	TiXmlText *TMNNUMNOTxt1=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle1->LinkEndChild(TMNNUMNOTxt1);
	TiXmlText *ACCEPTAREACODETXT1=new TiXmlText(areaCode);
	ACCEPTAREACODEEle1->LinkEndChild(ACCEPTAREACODETXT1);
	TiXmlText *ACCEPTDATETXT1=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle1->LinkEndChild(ACCEPTDATETXT1);
	
	
	//水电煤地区查询接口平台参数PARAMETERS结束

	//输出到字符串
	TiXmlPrinter printer;
	printer.SetLineBreak(NULL);
	printer.SetIndent(0);
	pDoc->Accept(&printer);
	strReq1=printer.CStr();
	wrLog("strReq1:%s",strReq1.c_str());
	wrLog("strReq0:%s",strReq0.c_str());	
	ANSItoUTF8(strReq0);
	ANSItoUTF8(strReq1);
	return 0;
} 


/************************************************************************/
/* //水电煤账单查询接口
ACCEPTAREACODE:受理地区城市编码
ADDITEM1：缴费省代码（6位）+缴费城市代码（6位）+缴费项目编号（4位）
ADDITEM2：上海水和煤气要求必须输入
SELECTVALUE:用户号码或者条形码
ADDITEM2:查询金额。上海水和煤气交费必须填
                                                                
/************************************************************************/
int CxmlParser:: packSDMQryxml(CGlobal gbl,char * WEBSVRNAME,char *WEBSVRCODE,char *OUTTMNNUMNO,char *SELECTTYPE,char *SELECTVALUE,char *ACCEPTDATE,char *ACCEPTAREACODE,char *ADDITEM1,char *ADDITEM2,string &strReq0,string &strReq1 ) //上海水费和煤气使用
{
	string data_sign=""; //签名
	string CER_Str=""; 
	char keep_str[32+1];
	string platString="";
	char orderNo[32+1];
	bbzero(orderNo);
	string data_sign_base64="";
	bbzero(keep_str);
	bbzero(orderNo);
	bbzero(keep_str);
	//CTools::get_keep(keep_str);
	CTools::get_keep(gbl.pos_no,keep_str);

	CTools::get_orderNo(orderNo);
	char strReqTmp[100];
	char APPFROMTmp[50];
	bbzero(APPFROMTmp);
	bbzero(strReqTmp);
	sprintf(strReqTmp,"%s|440100-APP001-001-%s|APP001|%s|",WEBSVRCODE,gbl.server_ip,gbl.server_ip);
	sprintf(APPFROMTmp,"440100-APP001-001-%s",gbl.server_ip);
	strReq0.assign(strReqTmp,strlen(strReqTmp));

	//ADDITEM1 缴费省代码（6位）+缴费城市代码（6位）+缴费项目编号（4位）

	//对平台数据进行数字签名;
	 TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlElement *PayPlatRequestParameter=new TiXmlElement("PayPlatRequestParameter");
	pDoc->LinkEndChild(PayPlatRequestParameter);
	TiXmlElement *CTRL_INFO =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO->SetAttribute("WEBSVRNAME",WEBSVRNAME); //服务名称
	CTRL_INFO->SetAttribute("WEBSVRCODE",WEBSVRCODE); //服务器编码
	CTRL_INFO->SetAttribute("APPFROM",APPFROMTmp); //请求本服务的应用标识码
	CTRL_INFO->SetAttribute("KEEP",keep_str); //本次服务的标识流水，参考规则:终端号+YYMMDDHHMMSS+随机数
	CTRL_INFO->SetAttribute("REQUESTTIME",ACCEPTDATE); //请求时间
	
	PayPlatRequestParameter->LinkEndChild(CTRL_INFO);
	
	TiXmlElement *PARAMETERS =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter->LinkEndChild(PARAMETERS);
	
	TiXmlElement *ORDERSEQEle=new TiXmlElement("ORDERSEQ");
	TiXmlElement * OUTTMNNUMNOEle=new TiXmlElement("TMNNUMNO");
	TiXmlElement *SELECTTYPEEle=new TiXmlElement("SELECTTYPE");
	TiXmlElement *SELECTVALUEEle=new TiXmlElement("SELECTVALUE");
	TiXmlElement *ACCEPTDATEEle=new TiXmlElement("ACCEPTDATE");
	TiXmlElement *ACCEPTAREACODEEle=new TiXmlElement("ACCEPTAREACODE");//受理地区
	TiXmlElement *ADDITEM1Ele=new TiXmlElement("ADDITEM1");//查询的地区
	
	PARAMETERS->LinkEndChild(ORDERSEQEle);
	PARAMETERS->LinkEndChild(OUTTMNNUMNOEle);
	PARAMETERS->LinkEndChild(SELECTTYPEEle);
	PARAMETERS->LinkEndChild(SELECTVALUEEle);
	PARAMETERS->LinkEndChild(ACCEPTDATEEle);
	PARAMETERS->LinkEndChild(ACCEPTAREACODEEle);
	PARAMETERS->LinkEndChild(ADDITEM1Ele);
	
	//赋值
	TiXmlText *ORDERSEQTxt=new TiXmlText(orderNo);
	ORDERSEQEle->LinkEndChild(ORDERSEQTxt);
	TiXmlText *OUTTMNNUMNOTxt=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle->LinkEndChild(OUTTMNNUMNOTxt);
	TiXmlText *SELECTTYPETxt=new TiXmlText(SELECTTYPE);
	SELECTTYPEEle->LinkEndChild(SELECTTYPETxt);
	TiXmlText *SELECTVALUETxt=new TiXmlText(SELECTVALUE);
	SELECTVALUEEle->LinkEndChild(SELECTVALUETxt);
	TiXmlText *ACCEPTDATETxt=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle->LinkEndChild(ACCEPTDATETxt);
	TiXmlText *ACCEPTAREACODETxt=new TiXmlText(ACCEPTAREACODE);
	ACCEPTAREACODEEle->LinkEndChild(ACCEPTAREACODETxt);
	TiXmlText *ADDITEM1Txt=new TiXmlText(ADDITEM1);
	ADDITEM1Ele->LinkEndChild(ADDITEM1Txt);
	
	TiXmlElement *ADDITEM2Ele=new TiXmlElement("ADDITEM2");//查询金额
	PARAMETERS->LinkEndChild(ADDITEM2Ele);
	TiXmlText *ADDITEM2Txt=new TiXmlText(ADDITEM2);
	ADDITEM2Ele->LinkEndChild(ADDITEM2Txt);

	TiXmlPrinter platPrinter;
	platPrinter.SetIndent(NULL);
	platPrinter.SetLineBreak(NULL);
	pDoc->Accept(&platPrinter);
	platString=platPrinter.CStr();
    wrLog("platString:%s",platString.c_str());
	//平台签名信息转utf-8
	ANSItoUTF8(platString);
	//数字签名

	Ccert::data_sign("privateKey.pem",platString,data_sign);
	data_sign_base64=base64_encode(data_sign);
	//读取公钥信息
	Ccert::cert_key("cert.pem",CER_Str);
	//平台数据数字签名结束

	TiXmlDocument *pDoc1=new TiXmlDocument;
	TiXmlDeclaration  *pDelaration=new TiXmlDeclaration("1.0","UTF-8",""); //UTF-8
	pDoc1->LinkEndChild(pDelaration); //添加根节点
	TiXmlElement *Request=new TiXmlElement("Request"); 
	pDoc1->LinkEndChild(Request); //添加根节点
	TiXmlElement *VerifyParameter=new TiXmlElement("VerifyParameter");
	Request->LinkEndChild(VerifyParameter);
	
	TiXmlElement *MERIDEle=new TiXmlElement("MERID");
	TiXmlText *MERIDContent=new TiXmlText(MERID); //机构编码
	MERIDEle->LinkEndChild(MERIDContent);
	
	VerifyParameter->LinkEndChild(MERIDEle);
	TiXmlElement * SIGNEle=new TiXmlElement("SIGN");
	VerifyParameter->LinkEndChild(SIGNEle);
	TiXmlText * SIGN_Content=new TiXmlText(data_sign_base64.c_str());
	wrLog("data_sign_base64:%s",data_sign_base64.c_str());
	SIGNEle->LinkEndChild(SIGN_Content);
	
	TiXmlElement *TMNNUMNOELe=new TiXmlElement("TMNNUM");
	VerifyParameter->LinkEndChild(TMNNUMNOELe);
	TiXmlText *TMNNUM_CONTENT=new TiXmlText(TMNNUM); //机构对应的接入终端号
	TMNNUMNOELe->LinkEndChild(TMNNUM_CONTENT);
	
	TiXmlElement * CHANNELCODEEle =new TiXmlElement("CHANNELCODE"); //渠道号码
	VerifyParameter->LinkEndChild(CHANNELCODEEle);
	TiXmlText *CHANNEL_CODE_CONTENT=new TiXmlText(CHANNELCODE);
	CHANNELCODEEle->LinkEndChild(CHANNEL_CODE_CONTENT);
	
	TiXmlElement *CER=new TiXmlElement("CER");	//公钥信息
	VerifyParameter->LinkEndChild(CER);
	TiXmlText *CER_CONTENT=new TiXmlText(CER_Str.c_str());
	CER->LinkEndChild(CER_CONTENT);


	TiXmlElement *PayPlatRequestParameter1=new TiXmlElement("PayPlatRequestParameter");
	Request->LinkEndChild(PayPlatRequestParameter1);
	TiXmlElement *CTRL_INFO1 =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO1->SetAttribute("WEBSVRNAME",WEBSVRNAME); //服务名称
	CTRL_INFO1->SetAttribute("WEBSVRCODE",WEBSVRCODE); //服务器编码
	CTRL_INFO1->SetAttribute("APPFROM",APPFROMTmp); //请求本服务的应用标识码
	CTRL_INFO1->SetAttribute("KEEP",keep_str); //本次服务的标识流水，参考规则:终端号+YYMMDDHHMMSS+随机数
	CTRL_INFO1->SetAttribute("REQUESTTIME",ACCEPTDATE); //请求时间
	
	PayPlatRequestParameter1->LinkEndChild(CTRL_INFO1);
	
	TiXmlElement *PARAMETERS1 =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter1->LinkEndChild(PARAMETERS1);

	TiXmlElement *ORDERSEQEle1=new TiXmlElement("ORDERSEQ");
	TiXmlElement *OUTTMNNUMNOEle1=new TiXmlElement("TMNNUMNO");
	TiXmlElement *SELECTTYPEEle1=new TiXmlElement("SELECTTYPE");
	TiXmlElement *SELECTVALUEEle1=new TiXmlElement("SELECTVALUE");
	TiXmlElement *ACCEPTDATEEle1=new TiXmlElement("ACCEPTDATE");
	TiXmlElement *ACCEPTAREACODEEle1=new TiXmlElement("ACCEPTAREACODE");//受理地区
	TiXmlElement *ADDITEM1Ele1=new TiXmlElement("ADDITEM1");//查询的地区

	PARAMETERS1->LinkEndChild(ORDERSEQEle1);
	PARAMETERS1->LinkEndChild(OUTTMNNUMNOEle1);
	PARAMETERS1->LinkEndChild(SELECTTYPEEle1);
	PARAMETERS1->LinkEndChild(SELECTVALUEEle1);
	PARAMETERS1->LinkEndChild(ACCEPTDATEEle1);
	PARAMETERS1->LinkEndChild(ACCEPTAREACODEEle1);
	PARAMETERS1->LinkEndChild(ADDITEM1Ele1);

	//赋值
	TiXmlText *ORDERSEQTxt1=new TiXmlText(orderNo);
	ORDERSEQEle1->LinkEndChild(ORDERSEQTxt1);
	TiXmlText *OUTTMNNUMNOTxt1=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle1->LinkEndChild(OUTTMNNUMNOTxt1);
	TiXmlText *SELECTTYPETxt1=new TiXmlText(SELECTTYPE);
	SELECTTYPEEle1->LinkEndChild(SELECTTYPETxt1);
	TiXmlText *SELECTVALUETxt1=new TiXmlText(SELECTVALUE);
	SELECTVALUEEle1->LinkEndChild(SELECTVALUETxt1);
	TiXmlText *ACCEPTDATETxt1=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle1->LinkEndChild(ACCEPTDATETxt1);
	TiXmlText *ACCEPTAREACODETxt1=new TiXmlText(ACCEPTAREACODE);
	ACCEPTAREACODEEle1->LinkEndChild(ACCEPTAREACODETxt1);
	TiXmlText *ADDITEM1Txt1=new TiXmlText(ADDITEM1);
	ADDITEM1Ele1->LinkEndChild(ADDITEM1Txt1);

	TiXmlElement *ADDITEM2Ele1=new TiXmlElement("ADDITEM2");//查询金额
	PARAMETERS1->LinkEndChild(ADDITEM2Ele1);
	TiXmlText *ADDITEM2Txt1=new TiXmlText(ADDITEM2);
	ADDITEM2Ele1->LinkEndChild(ADDITEM2Txt1);

	//平台数据结束

	//输出到字符串
	TiXmlPrinter printer;
	printer.SetLineBreak(0);
	printer.SetIndent(0);
	pDoc1->Accept(&printer);
	strReq1=printer.CStr();
	wrLog("strReq1:%s",strReq1.c_str());
	wrLog("strReq0:%s",strReq0.c_str());
	ANSItoUTF8(strReq1);
	ANSItoUTF8(strReq0);
	return 0;

}




//ANSI转UTF8  
void CxmlParser:: ANSItoUTF8(string &strAnsi)  
{  
    //获取转换为宽字节后需要的缓冲区大小，创建宽字节缓冲区，936为简体中文GB2312代码页  
    UINT nLen = MultiByteToWideChar(936,NULL,strAnsi.c_str(),-1,NULL,NULL);  
    WCHAR *wszBuffer = new WCHAR[nLen+1];  
    nLen = MultiByteToWideChar(936,NULL,strAnsi.c_str(),-1,wszBuffer,nLen);  
    wszBuffer[nLen] = 0;  
    //获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区  
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
    CHAR *szBuffer = new CHAR[nLen+1];  
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
    szBuffer[nLen] = 0;  
	
    strAnsi = szBuffer;  
    //内存清理  
    delete []wszBuffer;  
    delete []szBuffer;  
}  


//UTF8转ANSI  
void CxmlParser:: UTF8toANSI(string &strUTF8)  
{  
    //获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区  
    UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8.c_str(),-1,NULL,NULL);  
    WCHAR *wszBuffer = new WCHAR[nLen+1];  
    nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8.c_str(),-1,wszBuffer,nLen);  
    wszBuffer[nLen] = 0;  
	
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
    CHAR *szBuffer = new CHAR[nLen+1];  
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
    szBuffer[nLen] = 0;  
	
    strUTF8 = szBuffer;  
    //清理内存  
    delete []szBuffer;  
    delete []wszBuffer;  
}  


int CxmlParser::unpack(const string &_strSrc, string m_strTransCode, map<string, string> &_mapData, string &_strErr)
{
    //先获取待验签的数据
	
    string::size_type uiStart = 0, uiEnd = 0;
    string strStart="";
	string strEnd="";
	string RESPONSECODE_str="";
    string strUnsign="";
	string sign_decode64=""; 
	string strCert="";
	string showMsg="";

	showMsg=_strSrc;
	UTF8toANSI(showMsg);
	wrLog("解包数据:%s",showMsg.c_str());

    strStart = "<PayPlatResponseParameter>";
    strEnd = "</PayPlatResponseParameter>";        
    uiStart = _strSrc.find(strStart);
    uiEnd = _strSrc.find(strEnd);
	
    if (uiStart == string::npos) 
    {        
        uiStart = 0;
    }  
    /*   
    else
    {
        uiStart = uiStart + strStart.size();
    } 
    */          
    if (uiEnd == string::npos) 
    {        
        uiEnd = _strSrc.size();
    }          
    else
    {
        uiEnd = uiEnd + strEnd.size();
    } 
    
    strUnsign = _strSrc.substr(uiStart, uiEnd-uiStart);

    //解包            
    int i=0, iFind=0, iRet=0;
    string strGbkValue, strTmp;
    const field_cfg * pFieldCfg = NULL;
	map<string, string>::iterator iter;
	TiXmlDocument	docXml;
    TiXmlElement *peleRoot = NULL;
    TiXmlElement *peleGrant = NULL;
    TiXmlElement *peleParent = NULL;
    TiXmlElement *peleSelf = NULL;
    
    docXml.Parse(_strSrc.c_str(), 0, TIXML_ENCODING_UTF8);
    //docXml.Parse(_strSrc.c_str()); //后2个参数用默认值会出现段错误吗???
    //查找根节点
    if ( (peleRoot=docXml.RootElement()) == NULL ) //Request
	{
	    _strErr = "根节点不存在";
		return -1;
	}        
    
	CxmlParser xmlPaser;
	pFieldCfg=xmlPaser.get_cfg(2,m_strTransCode);
    if( pFieldCfg == NULL )
    {
        _strErr = "获取配置数据失败";
        return -1;
    }        

    for(; strcmp(pFieldCfg[i].m_strKey,"")!=0; ++i)
    {      
        iFind = 0;
        if (strcmp(pFieldCfg[i].m_strGrant,"")!=0)//祖父节点名存在则查找所属的祖父节点名
        {
            for (peleGrant=peleRoot->FirstChildElement(); peleGrant!=NULL; peleGrant=peleGrant->NextSiblingElement())    
            {    
                if (strcmp(peleGrant->Value(), pFieldCfg[i].m_strGrant)==0) 
                { 
					/*
					if (TRADE_FLAG==0)
					{
						wrLog("m_strGrant:%s",pFieldCfg[i].m_strGrant);
						wrLog("peleGrant->Value():%s",peleGrant->Value());
					}
					*/

                    iFind = 1;
                    break;  
                }  
				
	
            }             
        }            
        if (iFind == 0)
        {
            peleGrant = peleRoot;                
        }   
              
        //查找所属的父节点名    
        iFind = 0;    
        for (peleParent=peleGrant->FirstChildElement(); peleParent!=NULL; peleParent=peleParent->NextSiblingElement())    
        {   
            if (strcmp(peleParent->Value(),pFieldCfg[i].m_strParent)==0) 
            {  
                iFind = 1;
                break;  
            }  
	
        }             
        if (iFind == 0)
        {
            //_strErr = "配置数据错误, 父节点不存在(" + pFieldCfg[i].m_strKey + ")";
            //return -1;
            continue;
        }                       
        //查找节点本身是否存在
        iFind = 0;
        for (peleSelf=peleParent->FirstChildElement(); peleSelf!=NULL; peleSelf=peleSelf->NextSiblingElement())    
        {    
            if (strcmp(peleSelf->Value(),pFieldCfg[i].m_strSelf)==0) 
            {  

                iFind = 1;
                break;  
            }  


        }         
        if (iFind == 0)
        {
            //_strErr = "配置数据错误, 自身节点不存在(" + pFieldCfg[i].m_strKey + ")";
            //return -1;
            continue;
        }                       
        //if (peleSelf->NoChildren())//这个貌似是指是否有属性
        if (peleSelf->FirstChildElement() == NULL)
        {            
            if (strcmp(pFieldCfg[i].m_strAttr,"")==0) //非属性
            {
                if (peleSelf->GetText() != NULL)
                {  
                    strTmp = string(peleSelf->GetText());

					if (pFieldCfg[i].m_iIsCN==1)
					{

						CxmlParser::UTF8toANSI(strTmp);
					}

					  strGbkValue = strTmp;
                     _mapData[pFieldCfg[i].m_strKey] = strGbkValue.c_str();
                }   
			
            }
            else
            {
                if (peleSelf->Attribute(pFieldCfg[i].m_strAttr) != NULL) //如果是属性值
                {
                 
					strTmp = peleSelf->Attribute(pFieldCfg[i].m_strAttr);
					
                     if (pFieldCfg[i].m_iIsCN==1)
                     {
						 CxmlParser::UTF8toANSI(strTmp);
                     }
					
					  strGbkValue = strTmp;
	                _mapData[pFieldCfg[i].m_strKey] = strGbkValue.c_str();
                }                                                         
            } 
        }
        else
        {       
            //cout << "do specfield" << endl;
        	TiXmlPrinter *ppriXml = new TiXmlPrinter(); 
            peleSelf->Accept(ppriXml); 
        	//cout << string(ppriXml->CStr()) << endl;
            strTmp = string(ppriXml->CStr()); 
			if (pFieldCfg[i].m_iIsCN==1)
			{
				CxmlParser::UTF8toANSI(strTmp);
			}

			 strGbkValue = strTmp;
			 _mapData[pFieldCfg[i].m_strKey] = strGbkValue.c_str();
            //do_specfield(peleSelf, _mapData);
        }                                  
    }  
	
    string strSign="";
	strCert="";
    if ( (iter=_mapData.find("SIGN")) == _mapData.end() )
    {
        _strErr = "验签失败,没有签名信息";
        return -1;
    }   

    strSign = iter->second; 
	wrLog("验签数据:%s",strSign.c_str());

    //cout << strSign << endl;    
    if ( (iter=_mapData.find("CER")) == _mapData.end() )
    {
        _strErr = "验签失败,没有公钥信息";
        return -1;
    }

    strCert = iter->second;  
    //cout << strCert << endl;
	sign_decode64=base64_decode(strSign);

    iRet=Ccert::cert_verify(strCert,strUnsign, sign_decode64);        
    if ( iRet != 0)
    {
 
        _strErr = "验签失败";
        return -1;
    } 
	
	//检查返回码信息，返回错误信息
	if ( (iter=_mapData.find("RSPCODE")) == _mapData.end() )
    {
        _strErr = "无返回码";
        return -2;
    }   

    RESPONSECODE_str = iter->second; 
	if (strcmp(RESPONSECODE_str.c_str(),"000000")!=0) //如果返回码不是
	{
	
		if ((iter=_mapData.find("RSPMSG"))==_mapData.end())
		{
			_strErr="无错误信息";
			
			return -3;

		}

		_strErr=iter->second;
		wrLog("_strErr:%s",_strErr.c_str());
	
	}

    return 0;
}   


const field_cfg * CxmlParser::get_cfg(int _iReqOrRsp,string m_strTransCode)
{
    if (_iReqOrRsp == 1)
    {
            
       if(m_strTransCode == "INF06002")//地市查询(水电煤)
        {
            return &acReq_PublicTransQuery[0];
        }            
        else if(m_strTransCode == "INF06003")//账单查询(水电煤)
        {
            return &acReq_PublicBillQuery[0];
        }            
        else if(m_strTransCode == "INF06102")//缴费(水电煤)
        {
            return &acReq_PublicPay[0];
        }  
		else if(m_strTransCode == "INF06006")//车船税账单查询应答报文
        {
            return &acReq_carBillQuery[0];
        }  

		else if(m_strTransCode == "INF06104")//车船税缴费查询应答报文
        {
            return &acReq_carTaxPay[0];
        }  
		else if(m_strTransCode == "INF02021")//3g流量卡查询应答报文
        {
            return &acReq_g3Pay[0];
        }  
		else if (m_strTransCode=="INF06004")
		{
			return &acReq_g3PhoneCheck[0];
		}
		else if (m_strTransCode=="INF02011") //全国话费充值
		{
			return  &acReq_phonePay[0];
		}
		else if(m_strTransCode == "INF02038")//移动（广东省内）手机话费充值请求报文
        {
            return &acReq_phonePayGDYD[0];
        } 
		else if (m_strTransCode=="INF06201") //[东莞公交]签到
		{
			return &acReq_DGTSignIn[0];
		}
		else if (m_strTransCode=="INF06202") //[东莞公交]签退
		{
			return &acReq_DGTSignOut[0];
		}
		else if (m_strTransCode=="INF06203") //[东莞公交]卡操作
		{
			return &acReq_DGTCardOperate[0];
		}
		else if (m_strTransCode=="INF06204") //[东莞公交]开卡及冲正
		{
			return &acReq_DGTCardSale[0];
		}
		else if (m_strTransCode=="INF06205") //INF06205	[东莞公交]充值及冲正
		{
			return &acReq_DGTCardCharge[0];
		}
		else if (m_strTransCode=="INF01022") //INF01022 订单生成
		{
			return &acReq_dgt_orderProduce[0];
		}
		else if (m_strTransCode=="INF01023") //INF01023 订单确认
		{
			return &acReq_dgt_orderVerify[0];
		}
		else if (m_strTransCode=="INF06208") //东莞通通过keep查询
		{
			return &acReq_dgt_keepQry [0];
		}
		else if (m_strTransCode=="INF02037") //广禄QQ
		{
			return &acReq_QQ_guanglu_pay [0];
		}
		else if (m_strTransCode=="INF02039") //彩易qq
		{
			return &acReq_QQ_guanglu_pay [0];
		}
		else if (m_strTransCode=="INF02002") //银行卡查询
		{
			return &acReq_bankInfoQry [0];
		}

    }
    else
    {
        
       if(m_strTransCode == "INF06002")//地市查询(水电煤)
        {
            return &acRsp_PublicTransQuery[0];
        }            
        else if(m_strTransCode == "INF06003")//账单查询(水电煤)
        {
            return &acRsp_PublicBillQuery[0];
        }            
        else if(m_strTransCode == "INF06102")//缴费(水电煤)
        {
            return &acRsp_PublicPay[0];
        } 
		else if(m_strTransCode == "INF06006")//车船税账单查询应答报文
        {
            return &acRsp_carBillQuery[0];
        } 
		else if(m_strTransCode == "INF06104")//车船税缴费应答报文
        {
            return &acRsp_carTaxPay[0];
        } 
		else if(m_strTransCode == "INF02021")//流量卡
        {
            return &acRsp_G3Pay[0];
        } 
		else if (m_strTransCode=="INF06004")
		{
			return &acRsp_g3PhoneCheck[0];
		}
		else if (m_strTransCode=="INF02011") //全国话费充值
		{
		
			return &acRsp_G3Pay[0];

		}
		else if(m_strTransCode == "INF02038")//移动（广东省内）手机话费充值应答报文
        {
            return &acRsp_phonePayGDYD[0];
        } 
		else if (m_strTransCode=="INF06201") //[东莞公交]签到
		{
			return &acRsp_DGTSignIn[0];
		}
		else if (m_strTransCode=="INF06202") //[东莞公交]签退
		{
			return &acRsp_DGTSignOut[0];
		}
		else if (m_strTransCode=="INF06203") //[东莞公交]卡操作
		{
			return &acRsp_DGTCardOperate[0];
		}
		else if (m_strTransCode=="INF06204") //[东莞公交]开卡及冲正
		{
			return &acRsp_DGTCardSale[0];
		}
		else if (m_strTransCode=="INF06205") //INF06205	[东莞公交]充值及冲正
		{
			return &acRsp_DGTCardCharge[0];
		}
		else if (m_strTransCode=="INF01022") //INF01022	
		{
			return &acRsp_dgt_orderProduce[0];
		}

		else if (m_strTransCode=="INF01023") //INF01023
		{
			return &acRsp_dgt_orderVerify[0];
		}
		else if (m_strTransCode=="INF06208") //INF06208
		{
			return &acRsp_dgt_keepQry[0];
		}
		else if (m_strTransCode=="INF02037") //广禄qq
		{
			return &acRsp_QQ_guanglu_pay[0];
		}
		else if (m_strTransCode=="INF02039") //彩易qq
		{
			return &acRsp_QQ_guanglu_pay[0];
		}

		else if (m_strTransCode=="INF02002") //银行卡查询
		{
			return &acRsp_bankInfoQry[0];
		}
			 
    }                        
    
    return NULL;                
}


int CxmlParser::getBills( string datas,CGlobal gbl) //获取用户账单信息
{

	int totalNum=0;
	string dataXMLBuffer="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	dataXMLBuffer.append(datas);
	wrLog("dataXml:%s",dataXMLBuffer.c_str());

	//解析datas xml获取area_datas
	TiXmlDocument	docXml;
    docXml.Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml.RootElement();
	TiXmlElement *entity=rootElement->FirstChildElement();
	int i =0;

	while (entity)
	{

		TiXmlAttribute *indexAttribute=entity->FirstAttribute();
		while(indexAttribute)
		{
		
			const char *attributeName=indexAttribute->Name();
			if (strcmp(attributeName,"CUSTOMERNAME")==0) //客户姓名
			{
			
				
				strcpy(gbl.sdmBills.BillDetails[i].CUSTOMERNAME,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLNO")==0) // 缴费单号
			{
				strcpy(gbl.sdmBills.BillDetails[i].BILLNO,indexAttribute->Value());

			}
			else if (strcmp(attributeName,"BILLMONTH")) //账期
			{
				strcpy(gbl.sdmBills.BillDetails[i].BILLMONTH,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLBATCH"))
			{

				strcpy(gbl.sdmBills.BillDetails[i].BILLBATCH,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"CONTRACTNO"))
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].CONTRACTNO,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"BILLAMOUNT")) // 缴费金额
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLAMOUNT,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLDELAY"))
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLDELAY,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLDELAY"))
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BALANCE,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLDATE")) //开户日期
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLDATE,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"LASTPAYDATE")) //开户日期
			{
				strcpy(gbl.sdmBills.BillDetails[i].LASTPAYDATE,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"LASTPAYDATE")) //最后缴费日期
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].LASTPAYDATE,indexAttribute->Value());
			}
			
			else  if (strcmp(attributeName,"BILLSTATUS")) //订单状态
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLSTATUS,indexAttribute->Value());
			}
			
			else  if (strcmp(attributeName,"PASSWORD")) //账号密码
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].PASSWORD,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"THIRDCODE")) //账号密码
			{
				strcpy(gbl.sdmBills.BillDetails[i].THIRDCODE,indexAttribute->Value());
			}

			indexAttribute=indexAttribute->Next();
			i ++;
		
		}

		
		entity=entity->NextSiblingElement;
	}

	return 0;

}

//获取用户的账单信息
int  CxmlParser::getAreaDatas( string datas,CGlobal gbl)
{

	int iRet;
	TSTxnLogs txnLogs;
	char content[500];
	int content_len=0;
	char index[2+1];
	int datasLen=0;
	bbzero(index);
	BUSDATAS  areasDataBuffer;
	//ANSItoUTF8(datas);
	datasLen=datas.size();
	string  dataXMLBuffer="<?xml version=\".0\" encoding=\"UTF-8\" ?>\n";
	memset(&areasDataBuffer, 0, sizeof(areasDataBuffer));
	bbzero(content);// PAYMENTCODE(16)+BUSCODE(3)+strlen(PAYMENTNAME)+PAYMENTNAME(128)+strlen(BUSNAME)+BUSNAME(64+1)
	memset(&txnLogs, 0, sizeof(txnLogs));
	dataXMLBuffer.append(datas);
	wrLog("datas长度:%d",datas.size());
	
	wrLog("dataXml:%s",dataXMLBuffer.c_str());

	
	if(!sdm_datasVector.empty())
	{
		sdm_datasVector.clear();
	}

  
	//解析datas xml获取area_datas
	
	TiXmlDocument *docXml=new TiXmlDocument;
    docXml->Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml->RootElement();
	TiXmlElement *entity=NULL;

	for  (entity=rootElement->FirstChildElement();entity!=NULL;entity=entity->NextSiblingElement())
	{
		memset(&areasDataBuffer, 0, sizeof(areasDataBuffer));

		TiXmlAttribute *indexAttribute=new TiXmlAttribute();
		
		for(indexAttribute=entity->FirstAttribute();indexAttribute!=NULL;indexAttribute=indexAttribute->Next())
		{
			string attributeName="";
			attributeName.assign(indexAttribute->Name(),strlen(indexAttribute->Name()));

			//strcpy(attributeName.c_str(),indexAttribute->Name());
			
			if (strcmp(attributeName.c_str(),"PAYMENTCODE")==0)
			{
				bbzero(areasDataBuffer.PAYMENTCODE);
				strcpy(areasDataBuffer.PAYMENTCODE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName.c_str(),"BUSCODE")==0)
			{
	
				bbzero(areasDataBuffer.BUSCODE);
				strcpy(areasDataBuffer.BUSCODE,indexAttribute->Value());

			}
			else if (strcmp(attributeName.c_str(),"PAYMENTNAME")==0)
			{
				bbzero(areasDataBuffer.PAYMENTNAME);
				string PAYMENTNAME="";
				PAYMENTNAME=indexAttribute->Value();
				UTF8toANSI(PAYMENTNAME);
				strcpy(areasDataBuffer.PAYMENTNAME,PAYMENTNAME.c_str());

			}

			else  if (strcmp(attributeName.c_str(),"BUSNAME")==0)
			{

				bbzero(areasDataBuffer.BUSNAME);
				string BUSNAME="";
				BUSNAME=indexAttribute->Value();
				//strcpy(BUSNAME.c_str(),indexAttribute->Value());
 				UTF8toANSI(BUSNAME);
				strcpy(areasDataBuffer.BUSNAME,BUSNAME.c_str());
			
			}
		
		}
		
		sdm_datasVector.push_back(areasDataBuffer);

	}
	
	
	//显示
	txnLogs.colsnum = 5;
	txnLogs.colsWidth[0] = 50;
	txnLogs.colsWidth[1] = 100;
	txnLogs.colsWidth[2] = 250;
	txnLogs.colsWidth[3] = 0;
	txnLogs.colsWidth[4] = 0;
	txnLogs.colsNameLen = 60;
	memcpy(txnLogs.colsName, "序号\x00业务名称\x00收费单位\x00菜单ID\x00菜单内容", txnLogs.colsNameLen);
	sprintf(txnLogs.top, "请选择水电煤业务缴费项目");

	for(int i=0; i<sdm_datasVector.size();i++)
	{
		char bus_name[64+1];
		char PAYMENTNAME[128+1];
		bbzero(bus_name);
		bbzero(PAYMENTNAME);
		strcpy(bus_name,sdm_datasVector[i].BUSNAME); 
		strcpy(PAYMENTNAME,sdm_datasVector[i].PAYMENTNAME);
		sprintf(index,"%02d",(i+1));
		if( txnLogs.itemsLen+strlen(index) < MAX_INTERM_BUF ) //序号
		{
			strcpy(txnLogs.items+txnLogs.itemsLen, index); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}
	
		if( txnLogs.itemsLen+strlen(bus_name) < MAX_INTERM_BUF ) //业务名称
		{
			strcpy(txnLogs.items+txnLogs.itemsLen, bus_name); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;

		}

		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(PAYMENTNAME) < MAX_INTERM_BUF ) //收费单位
		{
			strcpy(txnLogs.items+txnLogs.itemsLen, PAYMENTNAME); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
		}
		
		else
		{
			break;
		}

		
		//下发菜单
		if( txnLogs.itemsLen+strlen("787") < MAX_INTERM_BUF )  
		{
			
			strcpy(txnLogs.items+txnLogs.itemsLen, "787"); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen("787") + 1;
		}
		
		else
		{
			break;
		}

  
		//下发内容
		char len_buffer[2+1];
		bbzero(content);
		content_len=0;
		bbzero(len_buffer);
		sprintf(len_buffer,"%02d",strlen(sdm_datasVector[i].PAYMENTCODE));
		memcpy(content+content_len,len_buffer,2);
		content_len+=2;
		memcpy(content+content_len,sdm_datasVector[i].PAYMENTCODE,strlen(sdm_datasVector[i].PAYMENTCODE));
		content_len+=strlen(sdm_datasVector[i].PAYMENTCODE);

		memcpy(content+content_len,sdm_datasVector[i].BUSCODE,3);
		content_len+=3;

		bbzero(len_buffer);
		sprintf(len_buffer,"%02d",strlen(PAYMENTNAME));
		memcpy(content+content_len,len_buffer,2);
		content_len+=2;
		memcpy(content+content_len,sdm_datasVector[i].PAYMENTNAME,atoi(len_buffer));
		content_len+=strlen(sdm_datasVector[i].PAYMENTNAME);
		bbzero(len_buffer);
		sprintf(len_buffer,"%02d",strlen(sdm_datasVector[i].BUSNAME));
		memcpy(content+content_len,len_buffer,2);
		content_len+=2;
		memcpy(content+content_len,sdm_datasVector[i].BUSNAME,atoi(len_buffer));
		content_len+=strlen(sdm_datasVector[i].BUSNAME);

		if((txnLogs.itemsLen+strlen(content)) < MAX_INTERM_BUF )  //需要账单查询的值 strlen(paymentcode)+PAYMENTCODE+BUSCODE(3)+strlen(PAYMENTNAME)+PAYMENTNAME(128)+strlen(BUSNAME)+BUSNAME(64+1)
		{

			strcpy(txnLogs.items+txnLogs.itemsLen, content); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
		}
		
		else
		{
			break;
		}

	}
		sprintf(txnLogs.hint, "共%d个缴费项目", i);
		iRet=TS_ShowTxnLogs(gbl.dPosInfo,&txnLogs);
		if (iRet!=0)
		{
			wrLog("TS_ShowTxnLogs返回错误[%d]", iRet); 
			return -1;
		}

		return 0 ;
	
}

//获取用户的账单信息

int  CxmlParser::getBillDatas(CGlobal *gbl, string datas)
{

	string dataXMLBuffer="";
	int content_len=0;
	dataXMLBuffer="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	dataXMLBuffer.append(datas);

	wrLog("dataXml:%s",datas.c_str());
	wrLog("数据长度:%d",datas.size());
  
	//解析datas xml获取账单详细情况
	memset(&gbl->sdmBills,0,sizeof(gbl->sdmBills));
	TiXmlDocument *docXml=new TiXmlDocument;
    docXml->Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml->RootElement();
	TiXmlElement *entity=NULL;
	int i=0;

	for  (entity=rootElement->FirstChildElement();entity!=NULL;entity=entity->NextSiblingElement())
	{

		TiXmlAttribute *indexAttribute=new TiXmlAttribute();
		
		for(indexAttribute=entity->FirstAttribute();indexAttribute!=NULL;indexAttribute=indexAttribute->Next())
		{
			char attributeName[50];
			bbzero(attributeName);
			strcpy(attributeName,indexAttribute->Name());
			
			if (strcmp(attributeName,"CUSTOMERNAME")==0)
			{
				
				string CUSTOMERNAME="";
				CUSTOMERNAME=indexAttribute->Value();
				UTF8toANSI(CUSTOMERNAME);
				bbzero(gbl->sdmBills.BillDetails[i].CUSTOMERNAME);
				strcpy(gbl->sdmBills.BillDetails[i].CUSTOMERNAME,CUSTOMERNAME.c_str());
				
			}
			else if (strcmp(attributeName,"BILLBARCODE")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].BILLBARCODE);
				strcpy(gbl->sdmBills.BillDetails[i].BILLBARCODE,indexAttribute->Value());

			}
			else if (strcmp(attributeName,"BILLNO")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLNO);
				strcpy(gbl->sdmBills.BillDetails[i].BILLNO,indexAttribute->Value());

			}
			else  if (strcmp(attributeName,"BILLMONTH")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].BILLMONTH);
				strcpy(gbl->sdmBills.BillDetails[i].BILLMONTH,indexAttribute->Value());
			
			}
			else if (strcmp(attributeName,"BILLBATCH")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].BILLBATCH);
				strcpy(gbl->sdmBills.BillDetails[i].BILLBATCH,indexAttribute->Value());

			}

			else if (strcmp(attributeName,"CONTRACTNO")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].CONTRACTNO);
				strcpy(gbl->sdmBills.BillDetails[i].CONTRACTNO,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLAMOUNT")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLAMOUNT);
				strcpy(gbl->sdmBills.BillDetails[i].BILLAMOUNT,indexAttribute->Value());
				wrLog("sdm_bill_detail.BILLAMOUNT:%s",gbl->sdmBills.BillDetails[i].BILLAMOUNT);
				
			}

			else if (strcmp(attributeName,"BILLDELAY")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLDELAY);
				strcpy(gbl->sdmBills.BillDetails[i].BILLDELAY,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BALANCE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BALANCE);
				strcpy(gbl->sdmBills.BillDetails[i].BALANCE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLDATE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLDATE);
				strcpy(gbl->sdmBills.BillDetails[i].BILLDATE,indexAttribute->Value());
				
			}

			else if (strcmp(attributeName,"LASTPAYDATE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].LASTPAYDATE);
				strcpy(gbl->sdmBills.BillDetails[i].LASTPAYDATE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLSTATUS")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLSTATUS);
				strcpy(gbl->sdmBills.BillDetails[i].BILLSTATUS,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"PASSWORD")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].PASSWORD);
				strcpy(gbl->sdmBills.BillDetails[i].PASSWORD,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"THIRDCODE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].THIRDCODE);
				strcpy(gbl->sdmBills.BillDetails[i].THIRDCODE,indexAttribute->Value());
				
			}
			
		
		}

		i ++;

	}

		gbl->sdmBills.nCnt=i;
		wrLog("账期:%d",gbl->sdmBills.nCnt);

		return 0 ;
}


/*
 * 功    能	--	xml类型报文组包
 * 输入参数	-- _mapData	待组包的域
 * 输出参数	-- _strDest	组包后的报文
 *          -- _strErr	错误信息
 * return	--	0:成功  -1:失败
 */
int CxmlParser::pack(string m_strTransCode,string &strReq1, map<string, string> &_mapData, string &_strErr)
{   
    int i=0, iFind=0, iRet=0;
    string strXml, strUtfValue;
    const field_cfg * pFieldCfg = NULL;
    map<string, string>::iterator iter;
	TiXmlDocument	docXml;
    TiXmlElement *peleRoot = NULL;
    TiXmlElement *peleGrant = NULL;
    TiXmlElement *peleParent = NULL;
    TiXmlElement *peleSelf = NULL;
	string data_sign_base64="";
	string show_strReq1="";

    if (strcmp(strReq1.c_str(),"")==0)
    {        
        strReq1 = "<?xml version=\"1.0\" encoding=\"utf-8\"?><Request><VerifyParameter></VerifyParameter><PayPlatRequestParameter></PayPlatRequestParameter></Request>"; //初始化
    }        
    docXml.Parse(strReq1.c_str(), 0, TIXML_ENCODING_UTF8);
    //docXml.Parse(_strDest.c_str()); //后2个参数用默认值会出现段错误吗???
    /*
    if (docXml.Parse(strXml.c_str()) == NULL)
    {
        _strErr = "生成初始XML失败";
        return -1;
    }   
    */     
    //查找根节点
    if ( (peleRoot=docXml.RootElement()) == NULL ) //Request
	{
	    _strErr = "根节点不存在";
		return -1;
	}        
    
	CxmlParser xmlPaser;
	pFieldCfg=xmlPaser.get_cfg(1,m_strTransCode);

    if( pFieldCfg== NULL )
    {
        _strErr = "获取配置数据失败";
        return -1;
    }        

	
    for(; strcmp(pFieldCfg[i].m_strKey,"")!=0; ++i)
    {

		//wrLog(" pFieldCfg[%d].m_strKey:%s" ,i,pFieldCfg[i].m_strKey);


        if ( (iter=_mapData.find(pFieldCfg[i].m_strKey)) == _mapData.end() )
        {
			wrLog("not find");
            continue;
        }   
		

        iFind = 0;
        if (strcmp(pFieldCfg[i].m_strGrant,"")!=0)//祖父节点名存在则查找所属的祖父节点名
        {

            for (peleGrant=peleRoot->FirstChildElement(); peleGrant!=NULL; peleGrant=peleGrant->NextSiblingElement())    
            {    
                if (strcmp(peleGrant->Value(),pFieldCfg[i].m_strGrant)==0) 
                {  
                    iFind = 1;
                    break;  
                }  
            }             
        }            
        if (iFind == 0)
        {
            peleGrant = peleRoot;                
        }   
        iFind = 0;    
        if (strcmp(pFieldCfg[i].m_strParent,"")==0)
        {
			char errBuffer[100];
			bbzero(errBuffer);
			sprintf(errBuffer,"配置数据错误:%s",pFieldCfg[i].m_strKey);
			_strErr.assign(errBuffer,strlen(errBuffer));
            return -1;
        }            
        //查找所属的父节点名                 
        for (peleParent=peleGrant->FirstChildElement(); peleParent!=NULL; peleParent=peleParent->NextSiblingElement())    
        {    
            if (strcmp(peleParent->Value(), pFieldCfg[i].m_strParent)==0) 
            {  
                iFind = 1;
                break;  
            }  
        }             
        if (iFind == 0)
        {
            //插入父节点
            if ( (peleParent=(TiXmlElement *)peleGrant->InsertEndChild(TiXmlElement(pFieldCfg[i].m_strParent))) == NULL)
            {
				char errBuffer[100];
				bbzero(errBuffer);
				sprintf(errBuffer,"插入父节点失败:%s",pFieldCfg[i].m_strKey );
                _strErr.assign(errBuffer,strlen(errBuffer));
            	return -1;
            } 
        }                       
        
        if (pFieldCfg[i].m_iIsCN == 1)
        {
           
			ANSItoUTF8(iter->second);
			strUtfValue=iter->second;
			//如果是中文，需要转为UTF-8进行编码
            //strUtfValue = iter->second;
  
        }
        else
        {
            strUtfValue = iter->second;
        }                                
        
        //插入自身节点
        if (strcmp(pFieldCfg[i].m_strAttr,"")==0)
        {
            if ( (peleSelf=(TiXmlElement *)peleParent->InsertEndChild(TiXmlElement(pFieldCfg[i].m_strSelf))) == NULL)
            {
				char errBuffer[100];
				bbzero(errBuffer);
				sprintf(errBuffer,"插入自身节点失败:%s",pFieldCfg[i].m_strKey);
            	return -1;
            } 
            peleSelf->Clear();
            TiXmlText *ptxtSelf = new TiXmlText(strUtfValue.c_str());
            peleSelf->LinkEndChild(ptxtSelf);                       
        }
        else
        {
            //查找节点本身是否存在
            iFind = 0;
            for (peleSelf=peleParent->FirstChildElement(); peleSelf!=NULL; peleSelf=peleSelf->NextSiblingElement())    
            {    
                if (strcmp(peleSelf->Value(),pFieldCfg[i].m_strSelf)==0) 
                {  
                    iFind = 1;
                    break;  
                }  
            } 
            if (iFind == 1)
            {
                peleSelf->SetAttribute(pFieldCfg[i].m_strAttr, strUtfValue.c_str());
            }
            else
            {
	            TiXmlElement eleAdd("");
	            eleAdd.SetAttribute(pFieldCfg[i].m_strAttr, strUtfValue.c_str());
	            eleAdd.SetValue(pFieldCfg[i].m_strSelf);
            	if (peleParent->InsertEndChild(eleAdd) == NULL)
            	{
					char errBuffer[100];
					bbzero(errBuffer);
					sprintf(errBuffer, "插入自身节点失败[%s]",pFieldCfg[i].m_strKey);
					_strErr.assign(errBuffer,strlen(errBuffer));

            		return -1;
            	}                
            }                                                        
        }                                                 
    }            
    
	TiXmlPrinter *ppriXml = new TiXmlPrinter(); 
	ppriXml->SetIndent(0); //去掉缩进
	ppriXml->SetLineBreak(NULL);//去掉换行
    docXml.Accept(ppriXml); 
	strReq1.assign(ppriXml->CStr());

    //签名
    string::size_type uiStart = 0, uiEnd = 0;
    string strStart, strEnd;
    string strUnsign, strSign, strCert;
    strStart = "<PayPlatRequestParameter>";
    strEnd = "</PayPlatRequestParameter>";        
    uiStart = strReq1.find(strStart);
    uiEnd = strReq1.find(strEnd);
    //cout << uiStart << endl;
    //cout << uiEnd << endl;
    if (uiStart == string::npos) 
    {        
        uiStart = 0;
    }  
    /*   
    else
    {
        uiStart = uiStart + strStart.size();
    } 
    */          
    if (uiEnd == string::npos) 
    {        
        uiEnd = strReq1.size();
    }          
    else
    {
        uiEnd = uiEnd + strEnd.size();
    } 
           
    //cout << _strDest << endl;
    strUnsign = strReq1.substr(uiStart, uiEnd-uiStart);
    //cout << strUnsign << endl;
	
	Ccert::data_sign("privateKey.pem",strUnsign,strSign);
	Ccert::cert_key("cert.pem",strCert);
	data_sign_base64=base64_encode(strSign);
	wrLog("sizeof(strCer:%d)",strCert.size());
	wrLog("data_sign_base64:%s",data_sign_base64.c_str());
	wrLog("sizeof(data_sign_base64):%d",data_sign_base64.size());

	char strTmp1[500];
	char strTmp2[2049];
	char sign_tmp[50];
	char cer_tmp[50];
	bbzero(cer_tmp);
	bbzero(sign_tmp);
	bbzero(strTmp1);
	bbzero(strTmp2);

	sprintf(strTmp1,"<SIGN>%s</SIGN>",data_sign_base64.c_str());
	sprintf(strTmp2,"<CER>%s</CER>",strCert.c_str());
	strcpy(sign_tmp,"<SIGN>SIGN</SIGN>");
	strcpy(cer_tmp,"<CER>CER</CER>");
	string string_tmp1="";
	string_tmp1.assign(strTmp1,strlen(strTmp1));
	string string_tmp2="";
	string_tmp2.assign(strTmp2,strlen(strTmp2));
	string sign_stringBuffer="";
	string cer_stringBuffer="";
	sign_stringBuffer.assign(sign_tmp,strlen(sign_tmp));
	cer_stringBuffer.assign(cer_tmp,strlen(cer_tmp));
    CTools::replace(strReq1, sign_stringBuffer,string_tmp1);     
    CTools::replace(strReq1, cer_stringBuffer, string_tmp2);
	show_strReq1=strReq1;
	UTF8toANSI(show_strReq1);
	wrLog("发送数据包:%s",show_strReq1.c_str());

    return 0;

} 


//获取用户车船税账单信息

int  CxmlParser::getCardTaxBills( string datas,CGlobal gbl)
{

	string dataXMLBuffer="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	int content_len=0;
	int i=0;
	dataXMLBuffer.append(datas);
	wrLog("dataXml:%s",dataXMLBuffer);
	//解析datas xml获取账单详细情况
	
	TiXmlDocument *docXml=new TiXmlDocument;
    docXml->Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml->RootElement();
	TiXmlElement *entity=NULL;
	
	memset(&gbl.carTaxBills,0,sizeof(gbl.carTaxBills));

	for  (entity=rootElement->FirstChildElement();entity!=NULL;entity=entity->NextSiblingElement())
	{

		TiXmlAttribute *indexAttribute=new TiXmlAttribute();
		
		for(indexAttribute=entity->FirstAttribute();indexAttribute!=NULL;indexAttribute=indexAttribute->Next())
		{
			

			char attributeName[50];
			bbzero(attributeName);
			strcpy(attributeName,indexAttribute->Name());
			
			if (strcmp(attributeName,"CUSTOMERNAME")==0) //车主姓名
			{
				
				string CUSTOMERNAME="";
				CUSTOMERNAME=indexAttribute->Value();
				UTF8toANSI(CUSTOMERNAME);
				strcpy(gbl.carTaxBills.cardTaxDatas[i].CUSTOMERNAME,CUSTOMERNAME.c_str());
				
			}
			else if (strcmp(attributeName,"BILLBARCODE")==0) //车船缴费流水号
			{
				
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLBARCODE,indexAttribute->Value());

			}
			else if (strcmp(attributeName,"BILLNO")==0) //总税额
			{
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLNO,indexAttribute->Value());

			}
			else  if (strcmp(attributeName,"BILLMONTH")==0) //年份
			{
	
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLMONTH,indexAttribute->Value());
			
			}
			else if (strcmp(attributeName,"BILLBATCH")==0) //抄次
			{
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLBATCH,indexAttribute->Value());

			}

			else if (strcmp(attributeName,"CONTRACTNO")==0) //应征金额
			{
				
				strcpy(gbl.carTaxBills.cardTaxDatas[i].CONTRACTNO,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLDELAY")==0) //滞纳金额
			{
	
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLDELAY,indexAttribute->Value());

				
			}

			else if (strcmp(attributeName,"BALANCE")==0) //罚款金额
			{
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BALANCE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"LASTPAYDATE")==0) //是否补缴
			{
				gbl.carTaxBills.cardTaxDatas[i].LASTPAYDATE=atoi(indexAttribute->Value());
				
			}
			
		}
		

		i++;

	}

		gbl.carTaxBills.nCnt=i;

		return 0 ;
	
}

