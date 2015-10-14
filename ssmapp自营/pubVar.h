#ifndef _PUBVAR_H
#define _PUBVAR_H
#include "nb30.h"
#include "Box.h"

///////////////////////////////////////////////////////////////////////

const long	EXITAPP				  = 1;	//退出应用程序
const long	RESTART				  = 2;	//重新启动系统
const long	HALTSYS				  = 3;	//关闭计算机

typedef struct {
	CString csName;
	int iLevel;//重要级别：1,关键设备，初始化失败不能服务; 2,可选设备，初始化失败可进服务
	int iExist;//是否装备	
	int iType;//类型
	int iPort;//端口
	int iCount;
	CString csIP;
	CString csUserName;
	CString csPW;
	int iStatus;//
} STHardWare;

typedef struct {
	ULONG i[2];                          /* number of _bits_ handled mod 2^64 */
	ULONG buf[4];                                           /* scratch buffer */
	unsigned char in[71];                                     /* input buffer */
	unsigned char digest[16];            /* actual digest after MD5Final call */
}MD5_CTX;

typedef struct {
	char szTerminalID[20];
	char szBankID[20];
	char szLocalIP[16];
	char szLocation[256];
	char szAreaCode[64];
	char szTerminalName[128];
	int iPort;
	int iRegister;
} STTerminalInfo;

typedef struct 
{
	CString BoxNo;
	CString PW;
	CString MailNo;
	CString InBoxTime;
	CString Deadline;
	CString PostmanID;
	CString CustomerMobile;
	CString CustomerName;
	CString CustomerAddress;
	CString PostmanPhone;
	CString CompShortName;
	CString ExtraData;
	CString MailStatus;
	void empty()
	{
		BoxNo.Empty();
		PW.Empty();
		MailNo.Empty();
		InBoxTime.Empty();
		Deadline.Empty();
		PostmanID.Empty();
		CustomerMobile.Empty();
		CustomerName.Empty();
		CustomerAddress.Empty();
		ExtraData.Empty();
		PostmanPhone.Empty();
		CompShortName.Empty();
		MailStatus.Empty();
	}
} PackageInfo;

typedef struct 
{
	CString PostmanID;
	CString PW;
	CString IDCard;
	CString Name;
	CString Phone;
	CString Begintime;
	CString Endtime;
	void empty()
	{
		PostmanID.Empty();
		PW.Empty();
		IDCard.Empty();
		Name.Empty();
		Phone.Empty();
		Begintime.Empty();
		Endtime.Empty();		
	}
} PostmanInfo;

#define	STATE_IDLE			1	//空闲广告
#define	STATE_MAINMENU		2	//主界面
#define	STATE_BUSY			3	//交易中
#define STATE_MAINTENANCE	4	//维护界面
#define STATE_PAUSE			5	//暂停服务

#define DEVICE_START 0
#define DEVICE_LOCKER 0
#define DEVICE_IDCR 1//idcard
#define DEVICE_BCR 2//barcode
#define DEVICE_PIN 3//pinboard
#define DEVICE_DVR 4
#define DEVICE_END 4

#define LEVEL_KEY		1
#define LEVEL_OPTION	2

#endif