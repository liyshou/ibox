// Tools.h: interface for the CTools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_H__045D6E06_D393_49AB_B98D_3EBBCB42DAE5__INCLUDED_)
#define AFX_TOOLS_H__045D6E06_D393_49AB_B98D_3EBBCB42DAE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAXAMOLEN 15

#include <string>
using namespace std;

class CTools  
{
public:

	CTools();
	virtual ~CTools();
	static void getYct_posSerial(char * term_serial);
	void static chg12asc(const char *inAmo,char *outAmo);
	void static chgToMny(const char *str,char *mny,int flag=1);
	int  static BCD_To_INT_10(BYTE *buff,BYTE length);
	void static LongToBcd(WORD number,BYTE *buff,BYTE length);
	int  static chgToCap(const char *mny,char *capMny);	//把12.34类型的字符串变为大写形式
	int  static getTalAmo(const char mny[][MAXAMOLEN],char *total,int nCnt);
	void static longToMny(long longMny, char *mny);
	void static Encrypt(char *inPwd,char *outPwd, int a);

	void static getYesterday(char *date);
	void static getLastMonth(char *date);
	void static getTheMonth(char *date);

	int  static hex_to_byte (const BYTE *instr, BYTE *outstr, int inLen, int type=0);
	void static byte_to_hex (const BYTE *in_buff, BYTE *out_buff, int in_len);

	void static formatDate(char *yymmdd, char *cDate, int type=0);
	void static formatTime(char *hhmmss, char *cTime);

	void static UpperMoney( char c, char *out);
	void static ConvertMoneyUpper(DWORD amount, char *out);
	void static lowerToUpper(char *ch);

	void static getTime(char *cTime, int flag=0);
	void static get_time(char *cTime,int flag=0);		//取时间，e.g 155624123
	void static getDate(char *cDate);
	void static get_termTime(char * cTime); //获取终端时间，时间格式为20110318152930
	void static getYctStan(char *yct_stan) ;//获取羊城通系统参考号
	void static itemAppend(const char *instr, int len, char* outstr);
	void static decIBcont(int count, char* src, char result[][2][50]);//解IB返回

	long static getFileSize(char *filename);
	
	void static crc_cu_16( unsigned char *SerBuffer, unsigned int length);
	void static sixten_toten(char*input,char* output);
	void static bus_money_change(char *input,char*output);
	void static get_date8 (char *cDate);
	int static IsDigit(char*cData);

	int static chkDate(int nYear, int nMonth, int nDay);

	void static trim(char* str, int type = 0);//去字符串两边空格，type: 0 两边空格，1 左边空格，2 右边空格

	int static findChar(char* src, char ch, int start); //查找字符
	void static getCurrentTime(char *cTime); //获取当前时间
	void static chgToPreMny(const char *str,char *mny); //将预付金转换成正数
	int static isAllNum(char *cData); //判断是否全部是数字

	int static isAllNumLetter(char *cData); //判断是否含有字母和数字以外的字符
	void static generate_random_num(char *str,int len);//产生指定为的随机数
	int static get_dgtOrderNo(char *orderN0);//东莞通订单号及流水号(时间加三个随机数)
	int static changeM1CardId(const char * instr, char *outstr);   
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
    
	int static tobyte (char *instr, char *outstr);
	
	void static get_msectime(char *time);
	
	//8位十六进制数转为N位十进制数，前面补零，默认转为12位十进制数
	//instr: 十六进制数 
	//outstr: 十进制数  e.g. : instr = "00004E20" then outstr = "000000020000"
	int static HexToDec(const char * instr, char *outstr,int n=12);

	//十进制转换为十六进制
	int static DecToHex(const char * strDec, char *strHex,int n=16);

	//获取当前日期时间
	//type : 0 - 2012/7/9 14:30:09
	int static getCurDatetime(char *curDatetime,int type = 0);
	int static  get_keep(char *keep );
	int static get_keep(char *term_id,char *keep );
	int  static replace (string &oldString,string &oldStr,string &newStr);
	int  static get_orderNo(char *orderNo);
	int static getDGTLocalSeq(char *localSequence); //暂时用时间+随机数。以后会改造
	int static getDGTM1CardId(char *cardId,char * cardShowId);
	int static  addZero(char *oldStr,int askLen,int flag,char *newStr);
	int static addSpace(char *oldStr,int askLen,int flag,char *newStr);
	void static generate_random_num(int random_value,char *str,int len);
	int static get_keep(char *term_id,int *random_value,char *keep );
	int static transYYYMMDD(char *oldStr,char *newStr);
	void static  CSreplace(std::string& s1,std::string& s2,std::string& s3,int count);
	int static  shildWords(char *origWord,int indexStart,int totalLen,char *newWord);

};

#endif // !defined(AFX_TOOLS_H__045D6E06_D393_49AB_B98D_3EBBCB42DAE5__INCLUDED_)
