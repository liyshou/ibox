/****************************************************************
//��ѶCTSIЭ�鶯̬��
//��Ѷ���ſƼ����޹�˾  http://www.payeasy.com.cn/
//��Ȩ���� (C) 2008-2015
//�޸���־: 
//2008-04-25 Ҷ�ƾ� ����  
****************************************************************/

/////////////////////////////////////////////////////////////////
//	TSCTSI.h
#ifndef __TSCTSI_H_KF20080101_

#define __TSCTSI_H_KF20080101_

#define SUCC 0
#define CANCLE_KEY 0x36		//ȡ��
#define HOTKEY 0x37
#define HANG_UP_KEY -101	//�һ�

#define MAX_DATA			1024		//���յ�fsk�����Ч����
#define MENU_ITEM_MAXCOUNT	25			//ÿ�η��Ͳ˵���������Ŀ
#define MENU_ITEM_MAXLEN	30			//�˵������Ƶ���󳤶�
#define TITLE_MAXLEN		30			//�˵�������󳤶�
#define MAX_CONT			30			//���׿��ƺ��������������Ŀ
#define MAX_LOCAL_MENU		256			//���²˵����������Ĳ˵�����
#define MAX_COLS			100			//���ն˲�ѯ��ϸ���������
#define MAX_COLS_NAME_BUF	1024		//���ն˲�ѯ��ϸ���������ռ�
#define MAX_INTERM_BUF		81920		//���ն˲�ѯ��ϸ���������ռ�
#define MAX_TXN_LOG			1000		//���ն˲�ѯ��ϸ������¼����

#define OVER_TIME			75			//��ʱʱ��
#define MSG_RETIME			5			//�·���ʾ��Ϣ�Զ�ˢ��ʱ��, ��λ���룩

//����ģʽ
#define TS_MAG_CARD			0x01		//ˢ��
#define TS_INPUT_CARD		0x02		//���俨��
#define TS_MAG_INPUT_CARD   0x03		//ˢ��������
#define TS_INPUT_PIN        0x04		//��������(��ʾ*)
#define TS_INPUT_MATH       0x05		//���� (��ʵ��������ʾ, 0~9)
#define TS_INPUT_LET		0x06		//��ĸ
#define TS_INPUT_CH			0x07		//����
#define TS_INPUT_ALL_CHAR   0x08		//ȫ�ַ�
#define TS_INPUT_MONEY      0x09		//������(����0.00��ʽ��ʾ)
#define TS_INPUT_DATE       0x0A		//����
#define TS_INPUT_YYYYMM     0x0B		//����
#define TS_INPUT_PWD		0x0C		//��������
//����ģʽ�Ӳ���
#define MAX_EXT_ENC			10			//��չ�ӽ����㷨�����
#define EXT_ENC_1			1			//��չ�����㷨1
#define EXT_DEC_2			2			//��չ�����㷨2
//�ӽ���ģʽ
#define TS_NO_ENC			0						//�����ܽ���(Ĭ��)
//�ɼ���
#define TS_UNION			1*MAX_EXT_ENC			//��������
#define TS_DES_1			2*MAX_EXT_ENC			//1��DES
#define TS_DES_2			3*MAX_EXT_ENC			//���������2��DES
#define TS_DES_2_DOWN		4*MAX_EXT_ENC			//�·������2��DES
//�¼���
#define TS_DES_3			1*MAX_EXT_ENC			//3��DES����, ���ܸ���
#define TS_DES_3_TEK_DEC	2*MAX_EXT_ENC			//ʹ��TEK��3��DES����
#define TS_DES_3_TEK_ENC	3*MAX_EXT_ENC			//ʹ��TEK��3��DES����
#define TS_EXT_ENC_1		4*MAX_EXT_ENC+EXT_ENC_1	//ʹ����չ�����㷨1
#define TS_EXT_DEC_2		4*MAX_EXT_ENC+EXT_DEC_2	//ʹ����չ�����㷨2
//�¼�����ϵ�ܳ׺�
#define TS_TMK_KEY			1			//TMK_KEY
#define TS_TPK_KEY			2			//TPK_KEY
#define TS_TAK_KEY			3			//TAK_KEY
#define TS_TEK_KEY			4			//TEK_KEY

typedef unsigned int	SOCKET;			//SOCKET����
typedef	unsigned char	BYTE;			//BYTE����

//fsk������Ϣ��ָ���
typedef struct _tagTSFskContent
{
	char title[TITLE_MAXLEN+1];			//������
	BYTE date[4+1];						//������
	BYTE tim[3+1];						//������

	BYTE posNum[8+1];					//�ն˺�
	BYTE serial[3+1];					//������ˮ
	int macCardType;					//��mac������
	int macKeyIndex;					//mac�ܳ׺�
	BYTE macRandom[8+1];				//mac�����
	BYTE mac[8+1];						//mac����

	int msgType;						//��Ϣ����
	int codeLen;						//�����볤��
	int codeNum;						//�����뼯����
	BYTE operCode[200];					//������ָ�
	int dataLen;						//��Ч����
	BYTE data[MAX_DATA];				//��Ч���ݼ�
	BYTE enyFlag;						//���ܸ���, �¾ɼ�����ϵ��־
}TSFskContent, *PTSFskContent;

//�˵��ṹ��
typedef struct _tagTSMenuList
{
	char acTitle[TITLE_MAXLEN+1];		//�˵�����
	int  iTotalNum; 					//�˵�������	
	char menuItem[MENU_ITEM_MAXCOUNT][MENU_ITEM_MAXLEN+1];//���˵�������

}TSMENULIST, *PTSMENULIST;


//������Ϣ�ṹ��->������Ϣ�ṹ��
typedef struct _tagSendContent
{
	char acTitle[80+1];					//��ʾ�ִ� (С��80�ַ�)		
	unsigned char uCommand;				//��������, ��ȡ��������
	unsigned char uCount;				//�ɼ�����, Ĭ��Ϊ0, 1�βɼ�
	
	unsigned char uDesMode;				//�����㷨, 1��������, 2һ��DES, 3�������������DES����, 4�·����������DES����
	unsigned char uCardType;			//���ܿ�����
	unsigned char uKeyIndex;			//��Կ���	
	unsigned char uKeyRand[8+1];		//��Կ�����
	
	unsigned char uTimeOut;				//�ȴ��û����볬ʱʱ��, ��λ:��
	unsigned char uMinLen;				//������С����
	unsigned char uMaxLen;				//������󳤶�
}SendContent;

//������Ϣ�ṹ��
typedef struct _tagTSTradeInfoSend
{	
	short sendType;						//0��ʾϵͳ�·��Ĳ����룬Ϊ1��ʾ�ն����͵Ľṹ��
	char acMainTitle[80+1];				//����ʾ�ִ� (С��80�ַ�)
	unsigned char uTotalContents;		//����������
	unsigned char uHasTradeID;			//������ˮ�ű�־
	unsigned char uMacCardType;			//MAC������
	unsigned char uMacKeyIndex;			//MAC����ʹ�õ���Կ���	
	unsigned char uMacKeyRand[8+1];		//MAC����ʹ�õ������
	SendContent Contents[MAX_CONT];		//����������Ϣ
	
}TSTradeInfoSend, *PTSTradeInfoSend;

//������Ϣ�ṹ��
typedef struct _tagTSTradeInfoRecv
{	
	struct RecvContent
	{
		unsigned char uMsgLen;			//�������ݵĳ���
		unsigned char uMsg[200+1];		//��������
		unsigned char uKeyRand[8+1];	//��Կ�����
	};
	unsigned char uTradeID[6+1];		//������ˮ��
	unsigned char uTotalContents;		//����������
	RecvContent Contents[MAX_CONT];		//����������Ϣ
	
}TSTradeInfoRecv, *PTSTradeInfoRecv;


//����
typedef struct _tagTSSendShortMsg
{
	char sendTime[14+1];				//����ʱ�� [20071001153020]
	char sendNum[20+1];					//���ͷ�����
	char content[140+1];				//����Ϣ����

}TSSendShortMsg, *PTSSendShortMsg;

//֪ͨ
typedef struct _tagTSSendInform
{
	char sendTime[16+1];				//����ʱ�� [20071001153020]
	char title[20+1];					//����
	char content[140+1];				//֪ͨ����

}TSSendInform, *PTSSendInform;


//�ʵ����׽����Ϣ
typedef struct _tagTSBillResult
{	
	char sendTime[14+1];				//����ʱ�� [20071001153020]
	unsigned char billNo[16+1];			//�ʵ���
	unsigned char result;				//�����ʵ�״̬�Ķ�Ӧ�� 0x00��δ֧����0x01����֧��(�ɹ�)��0x03�����ڣ�0x04ɾ���ʵ�

}TSBillResult, *PTSBillResult;

//���׽����Ϣ
typedef struct _tagTSTradeResult
{	
	char sendTime[14+1];				//����ʱ�� [20071001153020]
	char title[20+1];					//����
	char content[140+1];				//���׽����Ϣ

}TSTradeResult, *PTSTradeResult;


//δ֧���˵�
typedef struct _tagTSUnpayedBill
{
	unsigned char dispFlag;				//��ʾ���Ʊ�־
	unsigned char billNo[16+1];			//�ʵ���
	char sendTime[14+1];				//�ʵ�����ʱ��
	char endTime[12+1];					//�ʵ���ֹʱ�� [200708021111]
	char title[20+1];					//�ʵ�����
	char amount[12+1];					//�ʵ����
	char merInfo[20+1];					//�ʵ��̼���Ϣ
	char billDetail[80+1];				//�ʵ���ϸ��Ϣ

}TSUnpayedBill, *PTSUnpayedBill;

//͸������ָ��
typedef struct _tagTSIcReaderCommand
{
	int com;							//ʹ�õĴ���
	int comType;						//��������	

	char acTitle[80+1];					//��ʾ�ִ� (С��80�ַ�)
	unsigned char uDownLen;				//�·����ݵĳ���
	unsigned char uDownData[240];		//�·�������, ����ʾ�����ܳ��Ȳ����� 240 �ֽ�

	unsigned char uUpLen;				//�������ݵĳ���
	unsigned char uUpData[240];			//���͵�����

	unsigned char uTradeID[6+1];		//������ˮ��

}TSIcReaderCommand, *PTSIcReaderCommand;


//HID�豸͸���ṹ��
typedef struct _tagTSHID_IcReaderCommand
{

	char acTitle[80+1];					//��ʾ�ִ� (С��80�ַ�)
	unsigned char uDownLen;				//�·����ݵĳ���
	unsigned char uDownData[240];		//�·�������, ����ʾ�����ܳ��Ȳ����� 240 �ֽ�

	unsigned char uUpLen;				//�������ݵĳ���
	unsigned char uUpData[240];			//���͵�����

	unsigned char uTradeID[6+1];		//������ˮ��

}TSHID_IcReaderCommand, *PTSHID_IcReaderCommand; 



//�ն˲���
typedef struct _tagTSPosPar
{
	unsigned char num;					//��������
	struct ParCont
	{
		unsigned char parId;			//������
		unsigned char parLen;			//��������
		BYTE parCont[64];				//��������
	};
	ParCont parCont[MAX_CONT];

}TSPosPar, *PTSPosPar;


//������Ϣ
typedef struct _tagTSOperInfo
{
	unsigned char num;					//ģ����
	struct temp							//ģ��
	{
		unsigned char showType;			//��ʾ����
		unsigned char len;				//���ݳ���
		char content[85];				//��ʾ����
	};
	temp tempInfo[MAX_CONT];
}TSOperInfo;

//��ʾ��Ϣ
typedef struct _tagTSCueInfo
{
	unsigned char num;					//��Ϣ��¼��
	struct Info
	{
		unsigned char index;			//��ʾ��Ϣ����
		unsigned char flag;				//��Ϣ������־, '0'��ʾ������, '1'��ʾ����
		TSOperInfo operInfo;			//��Ϣ����ģ��
	};
	Info info[MAX_CONT];

}TSCueInfo, *PTSCueInfo;

//��ȫ�·���Ϣ
typedef struct _tagTSOutPutMsg_S
{
	unsigned char uCardType;			//������
	unsigned char uKeyIndex;			//�ܳ׺�
	unsigned char uDesMode;				//�����㷨
	unsigned char uMsgDataLen;			//��Ϣ�ĳ���
	unsigned char uMsgData[240+1];		//��Ϣ����
	
}TSOutPutMsg_S, *PTSOutPutMsg_S;

//��ȫ��ӡ
typedef struct _tagTSB4Print_S
{
	unsigned char uCardType;			//������
	unsigned char uKeyIndex;			//�ܳ׺�
	unsigned char uDesMode;				//���ܱ�־
	int	iDataLen;						//��ӡ��Ϣ�ĳ���
	unsigned char uPrintData[1000+1];	//��ӡ��Ϣ
	
}TSB4Print_S, *PTSB4Print_S;

#define MAX_MSG 345
//�·�������Ϣ(��Ϣ�ܳ�����С��MAX_MSG)
typedef struct _tagMulMsg
{
	int num;	//��Ϣ����
	char msg[8][121];

}MulMsg, *PMulMsg;

//�ն���Ϣ ��ȡ�ն���Ϣ
typedef struct _tagTSPosInfo
{
	char posNum[8+1];					//�ն˺�
	char prmVer[4+1];					//����汾
	char funVer[8+1];					//���ܰ汾
	char telNum[48+1];					//�绰����
	char psamNum[8+1];					//psam����
	unsigned char askType;				//��������
	unsigned char inFlag;				//������ʾ��־
	unsigned char leftType;				//�󴮿��豸����
	unsigned char rightType;			//�Ҵ����豸����
	unsigned char inType;				//�����豸����
	unsigned char enyFlag;				//���ܸ���, ����Ƿ�Ϊ�¼�����ϵ, 20080303
	//unsigned char HID_TYPE;				//HID�豸 by liuxiao 20130407

}TSPosInfo, *PTSPosInfo;

//DLL������ն���Ϣ
typedef struct _tagTSDLLPosInfo
{
	SOCKET sd;							//�˿ں�
	char pos_no[8+1];					//�ն˺�
	unsigned char enyFlag;				//������ϵ��ʶ:0�ɼ�����ϵ, 1�¼�����ϵ
	int macCardType;					//��Э����MAC�Ŀ�����
	char macKey[16+1];					//��Э����MAC����Կ
	char psamNum[8+1];					//��Э���PSAM������
	char prmVer[4+1];					//����汾
	char pan[6+1];						//�ʺ�PAN
	char tskey[4][16+1];				//�����ն��õ�4��KEY
}TSDLLPosInfo, *PTSDLLPosInfo;

//���±��ز˵�
typedef struct _tagTSLocalMenu
{
	unsigned char num;					//���²˵���

	struct MenuContent
	{
		unsigned char index;			//�˵���Ϣ����
		unsigned char flag;				//��Ϣ������־, '0'��ʾ������, '1'��ʾ����
		unsigned char askType;			//��������
		unsigned char contLen;			//�˵����ݳ���
		char content[80+1];				//�˵�����
	};
	MenuContent menuCont[10];

}TSLocalMenu, *PTSLocalMenu;

//�����ܳ�
typedef struct _tagTSPSAMKey
{
	unsigned char num;					//�����ܳ���
	struct Content
	{
		unsigned char keyIndex;			//�ܳ�����
		char keyInfo[24+1];				//�ܳ���Ϣ
		char macData[4+1];
	};
	Content keyCont[4];					//���4��key

}TSPSAMKey, *PTSPSAMKey;

//�˵�
typedef struct _tagTSMenus
{
	unsigned char num;					//�˵�����
	struct Menus
	{
		char isused;					//�˵�������־:'0'��ʾ�����ã�'1'��ʾ����
		int level1;						//һ���˵��е�λ�ô�1��ʼ�����Ϊ8
		int level2;						//�����˵��е�λ�ô�1��ʼ�����Ϊ8��0��ʾ�޸ü��˵�
		int level3;						//�����˵��е�λ�ô�1��ʼ�����Ϊ6��0��ʾ�޸ü��˵�
		char askType;					//�������ͣ�Ĭ��Ϊ0x00
		char sysnum;					//���ĺ�����ţ�Ĭ��Ϊ'1'
		int tcodelen;				//�˵���������
		char tcode[70+1];			//�˵�����
		int menulen;					//�˵����Ƴ���
		char menu[30+1];				//�˵�����
		int menuid;						//�˵�ID
		char info[200+1];				//�˵���Ϣ
	};
	Menus menus[MAX_LOCAL_MENU];	
}TSMenus, *PTSMenus;

//FTP�ļ���Ϣ
typedef struct _tagTSFtpFiles
{
	unsigned char updateNum;			//�ļ�����
	struct FtpFiles
	{
		char ip[64+1];					//IP
		char port[5+1];					//�˿�
		char usr[30+1];					//�û���
		char pwd[30+1];					//����
		char srcFile[250+1];			//�����ļ�·��
		char destFile[250+1];			//�����ļ�·��
		char md5[16+1];					//MD5������Ϣ
	};
	FtpFiles ftpFiles[MAX_CONT];		//FTP��Ϣ
	FtpFiles ftpFilesBak[MAX_CONT];		//����FTP��Ϣ
	int excFilesNum;					//ִ�е��ļ���
	char excFilesName[MAX_CONT][250];	//ִ�е��ļ���
	int shutDownFlag;					//�Ƿ�رճ���0���ر�1�ر�
}TSFtpFiles, *PTSFtpFiles;

//������ϸ��Ϣ
typedef struct _tagTSTxnLogs
{
	int colsnum;						//����
	int colsWidth[MAX_COLS];			//�п�
	int colsNameLen;
	char colsName[MAX_COLS_NAME_BUF];	//���ն˲�ѯ��ϸ���������ռ�
	int itemsLen;						//���ݳ���
	char items[MAX_INTERM_BUF];			//���ն˲�ѯ��ϸ���������ռ�
	char top[MAX_COLS_NAME_BUF];		//��ʾ��TOP
	char hint[MAX_COLS_NAME_BUF];		//��ʾ��
	char bottom[MAX_COLS_NAME_BUF];		//��ʾ��BOTTOM
}TSTxnLogs, *PTSTxnLogs;

/***********************************************************
��������: ��ȡ�ն���Ϣ
�������:
	SOCKET sd ��FSK������ͨ�ŵ�SOCKET
	int macCardType ��MAC���ӽ�����ʹ�õĿ�����
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ20��
�������:
	PTSPosInfo pPosInfo �ն���Ϣ
	PTSDLLPosInfo pDllPosInfo DLL������ն���Ϣ
	int &askType ��������
����ֵ: 
	SUCC �ɹ�;iSelectΪ�û�ѡ��Ĳ˵���
	HOTKEY ��ݼ�;iSelectΪ��ݼ�ֵ
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPosInfo(SOCKET sd, PTSPosInfo pPosInfo, PTSDLLPosInfo pDllPosInfo, int &askType, int &menu_id, int macCardType, int iTimeOut = 20);

/***********************************************************
��������: �·��˵�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSMENULIST menuList �˵��ṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int &iSelect ���ݷ���ֵȷ��
����ֵ: 
	SUCC �ɹ�;iSelectΪ�û�ѡ��Ĳ˵���
	HOTKEY ��ݼ�;iSelectΪ��ݼ�ֵ
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_Menu(const TSDLLPosInfo dPosInfo, PTSMENULIST menuList, int &iSelect, int timeOut = OVER_TIME);

/***********************************************************
��������: �·���ʾ��Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	char *msg ��ʾ��Ϣ����
	int flag 0-���û���[ȷ��]������;1-���û���[ȷ��]�����ػ����5����ն˻��Զ�����;2-��ָ��ʱ��(reTime)�Զ�ˢ��
	int reTime ��ָ��ʱ��(reTime)�Զ�ˢ��ʱ��ʱ�� ��λ:��###
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int &iSelect ���ݷ���ֵȷ��
����ֵ: 
	SUCC �ɹ�;iSelectΪ�û�ѡ��Ĳ˵���
	HOTKEY ��ݼ�;iSelectΪ��ݼ�ֵ
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_OutPutMsg(const TSDLLPosInfo dPosInfo, char *msg, int flag, int &iSelect, int reTime=MSG_RETIME, int timeOut=OVER_TIME);

/***********************************************************
��������: �·���ȫ��ʾ��Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSOutPutMsg_S pOutPutMsg_S ��ȫ�·���Ϣ
	int flag 0-���û���[ȷ��]������;1-���û���[ȷ��]�����ػ����5����ն˻��Զ�����;2-��ָ��ʱ��(reTime)�Զ�ˢ��
	int reTime ��ָ��ʱ��(reTime)�Զ�ˢ��ʱ��ʱ�� ��λ:��###
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int &iSelect ���ݷ���ֵȷ��
����ֵ: 
	SUCC �ɹ�;iSelectΪ�û�ѡ��Ĳ˵���
	HOTKEY ��ݼ�;iSelectΪ��ݼ�ֵ
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_OutPutMsg_S(const TSDLLPosInfo dPosInfo, PTSOutPutMsg_S pOutPutMsg_S, int iFlag, int &iSelect, int reTime=MSG_RETIME, int timeOut=OVER_TIME);

/***********************************************************
��������: �·�������ʾ��Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PMulMsg mulMsg ������Ϣ
	int flag 0-���û���[ȷ��]������;1-���û���[ȷ��]�����ػ����5����ն˻��Զ�����;2-��ָ��ʱ��(reTime)�Զ�ˢ��
	int reTime ��ָ��ʱ��(reTime)�Զ�ˢ��ʱ��ʱ�� ��λ:��###
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int &iSelect ���ݷ���ֵȷ��
����ֵ: 
	SUCC �ɹ�;iSelectΪ�û�ѡ��Ĳ˵���
	HOTKEY ��ݼ�;iSelectΪ��ݼ�ֵ
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_OutPutMsg_M(const TSDLLPosInfo dPosInfo, PMulMsg mulMsg, int iFlag, int &iSelect, int reTime=MSG_RETIME, int timeOut=OVER_TIME);

/***********************************************************
��������: �һ�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_HangUp(const TSDLLPosInfo dPosInfo, int timeOut=OVER_TIME);

/***********************************************************
��������: ���׿��ƺ��� 
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSTradeInfoSend pTradeInfoSend ������Ϣ���ͽṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	PTSTradeInfoRecv pTradeInfoRecv, ������Ϣ���սṹ��
����ֵ: SUCC �ɹ�;
***********************************************************/
extern "C" int __declspec(dllexport)TS_TradeControl(PTSDLLPosInfo pdPosInfo, PTSTradeInfoSend pTradeInfoSend, PTSTradeInfoRecv pTradeInfoRecv, int timeOut=OVER_TIME);

/***********************************************************
��������: ��ӡ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int printCom ��ӡ���ں�
	int printType ��ӡ������
	int iPage ��ӡҳ��
	char* acTitle ��ӡʱ��Ļ��ʾ������
	unsigned char* uszPrintMsg ��ӡ����
	int iPrintLen ��ӡ���ݳ���
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_B4Print(const TSDLLPosInfo dPosInfo, int printCom, int printType, int iPage, char* acTitle, unsigned char* uszPrintMsg, int iPrintLen, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ��ȫ��ӡ
�������:
const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
int printCom ��ӡ���ں�
int printType ��ӡ������
int iPage ��ӡҳ��
char* acTitle ��ӡʱ��Ļ��ʾ������
int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
SUCC �ɹ�;
<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_B4Print_S(const TSDLLPosInfo dPosInfo, int printCom, int printType, int iPage, char* acTitle, PTSB4Print_S pB4Print_S, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ����͸��
�������:
const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
PTSIcReaderCommand pIcReaderCommand ͸�������ṹ��
int baudRate ����Ӳ��������
int overTim �ն˵ȴ����ڷ��صĳ�ʱʱ��, ��λ:��, Ĭ��Ϊ10��
int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
PTSIcReaderCommand pIcReaderCommand ͸�������ṹ��	
����ֵ: 
SUCC �ɹ�;
<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_IcReaderCommand(const TSDLLPosInfo dPosInfo, PTSIcReaderCommand pIcReaderCommand, int baudRate=9600, int overTim=65, int iTimeOut=OVER_TIME); //by liuxiao 65 changed by 25

extern "C" int __declspec(dllexport)TS_HID_IcReaderCommand(const TSDLLPosInfo dPosInfo, PTSHID_IcReaderCommand pHID_IcReaderCommand, int overTim=65, int iTimeOut=OVER_TIME);
/***********************************************************
��������: ���ն˷��Ͷ���
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSSendShortMsg pSendShortMsg ���Žṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendShortMsg(const TSDLLPosInfo dPosInfo, PTSSendShortMsg pSendShortMsg, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն���ȡ����
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	PTSSendShortMsg shortMsg ���Žṹ��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetShortMsg(const TSDLLPosInfo dPosInfo, PTSSendShortMsg shortMsg, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն˷���֪ͨ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSSendInform pSendInform ֪ͨ�ṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendInform(const TSDLLPosInfo dPosInfo, PTSSendInform pSendInform, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն˷��ͽ��׽����Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSTradeResult pTradeResult ���׽���ṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendTradeResult(const TSDLLPosInfo dPosInfo, PTSTradeResult pTradeResult, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն˷��� �ʵ����׽����Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSBillResult pBillResult �ʵ����׽����Ϣ�ṹ��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendBillResult(const TSDLLPosInfo dPosInfo, PTSBillResult pBillResult, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն��·� �ʵ�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSUnpayedBill pUnpayedBill �ʵ���Ϣ�ṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_SendBill(const TSDLLPosInfo dPosInfo, PTSUnpayedBill pUnpayedBill, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն˶�ȡ �ʵ�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *billNo �ʵ���
	char *amount ���
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetBillInfo(const TSDLLPosInfo dPosInfo, char *billNo, char *amount, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն��·� ����
�������:
	const PTSDLLPosInfo pdPosInfo DLL������ն���Ϣ
	char *cardNum ����
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
	char comType �����豸����
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_SetCardNum(PTSDLLPosInfo pdPosInfo, char *cardNum, char comType=0, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ն˶�ȡ ����
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *cardNum ����
	char *posTime BCD��ʽ, ��ʽyyyymmddhh24:mi:ss
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetCardNum(const TSDLLPosInfo dPosInfo, char *cardNum, char *posTime, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ������ʾ��Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int type 1-������ʾ��Ϣ, 2-������ʾ��Ϣ
	PTSCueInfo pCueInfo ��ʾ��Ϣ�ṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateCueInfo(const TSDLLPosInfo dPosInfo, int type, PTSCueInfo pCueInfo, int iTimeOut=OVER_TIME);

/***********************************************************
��������: �����ն˲���
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSPosPar pPosPar �ն˲����ṹ��
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdatePosPar(const TSDLLPosInfo dPosInfo, PTSPosPar pPosPar, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ������ҳ��Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	char *title ��ҳ��Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateHeadPage(const TSDLLPosInfo dPosInfo, char *title, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���������ն�
�������:
	char *posTelNum �ն˵�ź���
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	SOCKET &sd �˿ں�	
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_CallPos(SOCKET &sd, char *posTelNum, int rcvTimeOut=60, int sndTimeOut=30);

/***********************************************************
��������: ��ȡ�汾��
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ	
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *version �汾��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetVersion(const TSDLLPosInfo dPosInfo, char *version, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ��ȡ�ն˺�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *posNum �ն˺�
����ֵ:
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPosNum(const TSDLLPosInfo dPosInfo, char *posNum, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ��ȡ�ն����к�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *posNum �ն����к�
����ֵ:
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPosSerial(const TSDLLPosInfo dPosInfo, char *posSerial, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ����PSAM���ܳ�
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSPSAMKey pPSAMKey �����ܳ׽ṹ��
	int cardType ������
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ:
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdatePSAMKey(const TSDLLPosInfo dPosInfo, PTSPSAMKey pPSAMKey, int cardType, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ��ȡPSAM���ż������(�ڸ�����Կʱ��)
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int cardType ������
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *PSAMNum PSAM���� 8�ֽ�
	char *random ����� 4�ֽ�
����ֵ:
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetPSAMNum(const TSDLLPosInfo dPosInfo, int cardType, char *PSAMNum, char *random, int iTimeOut = OVER_TIME);

/***********************************************************
��������: ��ȡ���ز˵�����
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	PTSTradeInfoSend pTradeInfoSend, ������Ϣ���ͽṹ��
	int iTimeOut, SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	PTSTradeInfoRecv pTradeInfoRecv, ������Ϣ���սṹ��
����ֵ:
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ReadLocalCont(const TSDLLPosInfo dPosInfo, PTSTradeInfoSend pTradeInfoSend, PTSTradeInfoRecv pTradeInfoRecv, int rcvTimeOut=60, int sndTimeOut=30);

/***********************************************************
��������: ���ַ���ת����16���Ƶ�BCD��
�������:	 
	unsigned char *ascii_buf, ��Ҫת�����ַ���
	int conv_len, ��Ҫת�����ַ�������
	unsigned char type������
�������:
	unsigned char *bcd_buf, ת�����BCD��
asc_to_bcd(bcd, (UCHAR*)"123", 3, 0); //��� Ϊ "0x12 0x30"
asc_to_bcd(bcd, (UCHAR*)"123", 3, 1); //��� Ϊ "0x01 0x23"
***********************************************************/
extern "C" int __declspec(dllexport)asc_to_bcd(unsigned char *bcd_buf, unsigned char *ascii_buf, int conv_len, unsigned char type);

/***********************************************************
��������: ��BCD��ת�����ַ���
�������:	 
	unsigned char *bcd_buf, ��Ҫת����BCD��
	int conv_len, ת�����ַ����ĳ���
	unsigned char type������
�������:
	unsigned char *ascii_buf, ת������ַ���
bcd_to_asc(asc, (UCHAR*)"\x12\x34", 3, 0); //��� Ϊ "123"
bcd_to_asc(asc, (UCHAR*)"\x12\x34", 3, 1); //��� Ϊ "234"
***********************************************************/
extern "C" int __declspec(dllexport)bcd_to_asc(unsigned char *ascii_buf, unsigned char *bcd_buf, int conv_len, unsigned char type);

/***********************************************************
��������: ��Դ�ַ���ȥ�����߿ո�Ľ�� ��ֵ��Ŀ���ַ���
�������:	 
	unsigned char *buf, �������ַ���	
	len,Ŀ���ַ��������ռ�
�������:
	unsigned char *out_buf, Ŀ���ַ���
����ֵ:
	�����Ŀ���ַ����ĳ���
trim(" 1 2 3 ", 7); //��� Ϊ "123"
trim(" 1 2 3 ", 2); //��� Ϊ "1"
***********************************************************/
extern "C" int __declspec(dllexport)trim(char* str,int len);

/***********************************************************
��������: ����ն˱��ز˵�
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	int iTimeOut SOCKET ͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ClearMenus(const TSDLLPosInfo dPosInfo, int iTimeOut=OVER_TIME);

/***********************************************************
��������: �����ն˱��ز˵�
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	TSMenus menus �˵��ṹ��
	int iTimeOut SOCKET ͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateMenus(const TSDLLPosInfo dPosInfo, TSMenus menus, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ���ɽ��ڲ˵�ָ��
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	PTSTradeInfoSend pTradeInfoSend ������Ϣ���ͽṹ��
	int iTimeOut SOCKET ͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *tcode ָ������
	int *tcodelen ָ���
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_TradeEncode(const TSDLLPosInfo dPosInfo, PTSTradeInfoSend pTradeInfoSend, char *tcode, int *tcodelen, int timeOut=OVER_TIME);

/***********************************************************
��������: ���ɽ��ڲ˵�ָ��
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	PTSTradeInfoSend pTradeInfoSend ������Ϣ���ͽṹ��
	int iTimeOut SOCKET ͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *tcode ָ������
	int *tcodelen ָ���
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_GetMacKey(const TSDLLPosInfo dPosInfo, char *mac16_out, int timeOut=OVER_TIME);

/***********************************************************
��������: ������TEK���ܵ�����
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	content �����ܵ�����
	contLen �����ܵ����ݳ���
	psamCardNo16 16λ��PSAM������ 
�������:
	enyData ���ܺ������
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TEKEny(const TSDLLPosInfo dPosInfo, char *content, int contLen, char *psamCardNo16, char *enyData);

/***********************************************************
��������: ��FTP�����ļ�����ִ����Ӧ����
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	TSFtpFiles FTP�ļ���Ϣ
�������:
	fileNo ��ǰ�Ѹ��µ��ļ���
	pageNo ��ǰ�Ѹ��µ��ļ��Ķκ�
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_UpdateFiles(const TSDLLPosInfo dPosInfo, TSFtpFiles ftpFiles, int iTimeOut=OVER_TIME);

/***********************************************************
��������: �·�������ϸ
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	PTSTxnLogs ptxnLogs ������ϸ��Ϣ
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ShowTxnLogs(const TSDLLPosInfo dPosInfo, PTSTxnLogs ptxnLogs, int iTimeOut=OVER_TIME);

/***********************************************************
��������: �·���ǰ��ѡ�������(2012-03-28 ����)
�������:
	const TSDLLPosInfo dPosInfo DLL ������ն���Ϣ
	int   icolumn   ��Ҫ��ȡ������������
����ֵ: 
	char *selItemCont ��ǰ��ѡ�������
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
//extern "C" int __declspec(dllexport)TS_GetSelItemCont(const TSDLLPosInfo dPosInfo, char *selItemCont,int icolumn,int iTimeOut=OVER_TIME);

/***********************************************************
��������: �����������
�������:
	char * pwd ���� 6�ֽ�ASII
	char * random ����� 16�ֽ�ASII
�������:
	char * enc_result ���ܽ�� 16�ֽ�ASII
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)myEncryptForBankcardPwd(char * pwd, char * random,char * enc_result);

/***********************************************************
��������: ��ͨ�������
�������:
	char * pwd ���� 6�ֽ�ASII
�������:
	char * enc_result ���ܽ�� 16�ֽ�ASII
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)myEncryptForPayEasyMemberPwd(char * pwd, char * enc_result);

/***********************************************************
��������: ��Ǯ��
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
	SUCC �ɹ�;
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_OpenMnyBox(const TSDLLPosInfo dPosInfo, char *instruct, int instructLen, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ע�������ĸ��ѿ���ֵҳ��
�������:
const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
char *httpcon ��������
char *request ������ҳ����Ե�ַ
char *formData ������
char *getData ���ؽ��(�������)
int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
SUCC �ɹ�;
<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_FillCt10000(const TSDLLPosInfo dPosInfo, char *httpcon, char *request, char *formData, char *getData, int iTimeOut=OVER_TIME);

/***********************************************************
��������: ��IE�ϴ���ַΪURL����ҳ
�������:
const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
char *url ��ַ
int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
����ֵ: 
SUCC �ɹ�;
<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_OpenIE(const TSDLLPosInfo dPosInfo, char *url, int iTimeOut=OVER_TIME);

/***********************************************************
��������: �麣����ע��
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int result ��������
����ֵ: 
	SUCC �ɹ�
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusPosLogin(const TSDLLPosInfo dPosInfo, char* errMsg, int timeOut=OVER_TIME);

/***********************************************************
��������: �麣����ǩ��
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	char *operUniqueId ����ԱID
	char *passwd ����Ա����
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int result ��������
����ֵ: 
	SUCC �ɹ�
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusSignIn(const TSDLLPosInfo dPosInfo, char *operUniqueId, char *passwd, char* errMsg, int timeOut=OVER_TIME);

/***********************************************************
��������: �麣��������ѯ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int result ��������
	long &out_balance���صĿ�������"��"��ʾ
	long &out_asn���صĿ���
	long &out_indate���صĿ���Ч�ڣ���ʽ�磺20201231
	long &out_cardType���صĿ�����
����ֵ: 
	SUCC �ɹ�
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusQuery(const TSDLLPosInfo dPosInfo,  long &out_balance, long &out_asn, char *out_indate, char *out_cardType, char* errMsg, int timeOut=OVER_TIME);

/***********************************************************
��������: �麣������ֵ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	nAmount��ֵ����"��"��ʾ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int result ��������
	long &out_balance���صĳ�ֵ��������"��"��ʾ
	long &out_tradeId���صĽ�����ˮ��
	long &out_asn���صĿ���
	long &oldBalance���صĳ�ֵǰ��������"��"��ʾ
	long &out_operUniqueId���صĲ���Ա��
	char *out_cardType���صĿ�����
	char *out_devUniqueId���ص��豸��
	long &out_Time���صĽ���ʱ��
����ֵ: 
	SUCC �ɹ�
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusTopUp(const TSDLLPosInfo dPosInfo, long nAmount, long &out_balance, long &out_tradeId, long &out_asn, long &oldBalance, long &out_operUniqueId, char *out_cardType, char *out_devUniqueId, long &out_Time, char* errMsg, int timeOut=200);

/***********************************************************
��������: �麣������ȡ������Ϣ
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	int result ��������
	char *out_mesg���صĴ�����Ϣ
����ֵ: 
	SUCC �ɹ�
	<0 ʧ��
***********************************************************/
extern "C" int __declspec(dllexport)TS_ZHBusGetErrMsg(const TSDLLPosInfo dPosInfo, char *out_mesg, int timeOut=OVER_TIME);


/***********************************************************
��������: �·�����
�������:
	const TSDLLPosInfo dPosInfo DLL������ն���Ϣ
	char *msg ��������
	int iTimeOut SOCKETͨ�ų�ʱʱ��, ��λ:��, Ĭ��Ϊ60��
�������:
	char *msg ��������
����ֵ: 
	SUCC �ɹ�
	<0 ʧ��
***********************************************************/
//extern "C" int __declspec(dllexport)TS_SendCommand(const TSDLLPosInfo dPosInfo, char *msg, int timeOut=OVER_TIME);

#endif 