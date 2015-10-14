#ifndef _PUBVAR_H
#define _PUBVAR_H
#include "nb30.h"
#include "Box.h"

///////////////////////////////////////////////////////////////////////

const long	EXITAPP				  = 1;	//�˳�Ӧ�ó���
const long	RESTART				  = 2;	//��������ϵͳ
const long	HALTSYS				  = 3;	//�رռ����

typedef struct {
	CString csName;
	int iLevel;//��Ҫ����1,�ؼ��豸����ʼ��ʧ�ܲ��ܷ���; 2,��ѡ�豸����ʼ��ʧ�ܿɽ�����
	int iExist;//�Ƿ�װ��	
	int iType;//����
	int iPort;//�˿�
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

#define	STATE_IDLE			1	//���й��
#define	STATE_MAINMENU		2	//������
#define	STATE_BUSY			3	//������
#define STATE_MAINTENANCE	4	//ά������
#define STATE_PAUSE			5	//��ͣ����

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