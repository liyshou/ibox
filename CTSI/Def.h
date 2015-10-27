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
#define RE_MNG_CARD 0x35	//����ˢ����־
#define PE_OVER_TIME 90		//�ȴ�pe���س�ʱʱ��
#define EPOS_OVER_TIME 30	//epos��ʱʱ��
#define MENU_TIME_OUT 60000 //�˵���ʱʱ��(��λ������)
#define MAXAMOLEN 15		//������
#define MAXCOUNT 20
#define CON_MAX 65536
#define DB_MAX 1000			//���ݿ����������1000
#define MAXTRY 50			//����ѭ�������ѭ������

#define MNG_ONE 0			//һ��ˢ����������������
#define MNG_MUL 1			//ÿ�ν��Ѷ�ˢ��
#define MNG_ONE_BIND 2		//һ��ˢ����������������(���ð�e����ˢ���п�)
#define MNG_MUL_BIND 3		//ÿ�ν��Ѷ�ˢ��(���ð�e����ˢ���п�)
#define MNG_ONE_BIND_E 4	//һ��ˢ����������������(�ð�e��)
#define MNG_MUL_BIND_E 5	//ÿ�ν��Ѷ�ˢ��(�ð�e��)

#define ONLY_MNG 0			//ֻˢ��
#define ONLY_INPUT 1		//ֻ���뿨��
#define MNG_OR_INPUT 2		//ˢ��������


#define HOTKEY1 0x05		//��ݼ�20071124
#define HOTKEY2 0x06
#define HOTKEY3 0x07
#define HOTKEY4 0x08
#define PAYTONETRADE 0x09


//E��ת�����ֵ��ҵ�����(���µ�ҵ�����������)
#define ECARDTRADE "[0301][0302][0075][0074][0083][0221][0109][0268][0339][0340]"

extern char logon_random1[8+1];	//logon����ǰ8λ
extern char logon_random2[8+1];	//logon���غ�8λ,����ǩ��key
extern int logon_flag;
extern int yct3_initsign_flag;//���ͨ������ǩ����ʶ

extern int syn_flag;  //ͬ����־
extern CCriticalSection cs_g_connStat;	//���ݿ�������Դ��
extern CCriticalSection cs_ONLINE_NUM;	//������������
extern CCriticalSection cs_FILE;		//��־�ļ���

extern CConnect con;//������
extern CWrLog log;

//���ṹ��
typedef struct
{
	unsigned char	Bit_Name;
	unsigned char	Type;		//�ַ�����
	unsigned char	Format;		//��������
	WORD	Max_Length;
	WORD	Real_Length;
	unsigned char	*Rev_Data;
	unsigned char	*Send_Data;
}Packet_Struct;



//��ݸ�������ṹ��
typedef struct
{
	 char	Type;		//�ַ�����
	 char	Format;		//�������� �̶����Ȼ��Ǳ߳�
 	 int	Max_Length; //��󳤶�
	 int	Real_Length; //ʵ�ʳ���
	 char	*Rev_Data; //��������
	 char	*Send_Data; //��������
	 char    name[100]; //��������
	 char   optionFlag;//�Ƿ��ѡ
	

}Pack_dgt_Struct;




//��ӡ��ϸ�ṹ��
typedef struct _tagPrnDetail
{
	int nMonCnt;	//���ѵ��·���
	struct MonDetail
	{
		int nCnt;			//��ϸ��Ŀ
		char tmFrom[15];	//������ʼ��
		char tmTo[15];		//���ѽ�����
		char month[7];
		struct Content
		{
			char type[40];		//��Ŀ
			char amo[MAXAMOLEN];		//���	
		};
		char detail98Amo[13];	//���ŷ�Ʊ�޸�20080129	
		char detail98Name[11];
		Content cnt[MAXCOUNT];	//��ϸ��ÿ������
	};
	MonDetail monDetail[6];		//��������ʱ�ε���ϸ
}PrnDetail,*pPrnDetail;


//�����ֲ�Ų�ѯ���ؽ��,2006-11-21 by lihk
typedef struct _tagCATV_Struct1
{
	int nCnt;  //�ֲ���ϸ��
	int add_len;	//��ַ����
	char address[40+1];	//��ַ

	struct Content
	{
		int user_name_len;		//�û�����
		char user_name[60+1];	//�û���
		char amount[12+1];		//Ӧ�ս��
		char remain_amount[12+1];//ʣ��Ԥ��
		int card1_len;			//����1��(������)
		char card1[20+1];		//����1
		int card2_len;			//����2��(��ʾ��)
		char card2[20+1];		//����2
		/*-------�齭��Ƶ�����ײ�--------*/
		char stat[3+1];			//�û�״̬
		char remark[99+1];		//��ע
	};
	Content cnt[MAXCOUNT];	//�ֲ���ϸ
}CATV_Struct1;


//�������֤��ѯ���ؽ��,2006-11-21 by lihk
typedef struct _tagCATV_Struct2
{
	int nCnt;	//�ֲ�Ÿ���
	struct Content
	{
		int manual_num_len;   //�ֲ�ų���
		char manual_num[11+1];	//�ֲ��
		int add_len;	//��ַ����
		char address[40+1];	//��ַ
	};
	Content cnt[MAXCOUNT];	//�ֲ����Ϣ
}CATV_Struct2;

//����˰ �˵���ѯ�ṹ�� 

typedef struct _tagCarTaxBills_Struct  
{
	int nCnt;	//�˵���
	struct CarTaxDATAS
	{

	char CUSTOMERNAME[64+1];//��������
	char BILLBARCODE[200+1]; //����˰��ˮ��
	char BILLNO[200+1]; //��˰��
	char BILLMONTH[64+1]; //���
	char BILLBATCH[12+1]; //����
	char CONTRACTNO[32+1]; //Ӧ�����
	char BILLDELAY[12+1]; //���ɽ��
	char BALANCE[12+1]; //������
	int  LASTPAYDATE;   //�Ƿ񲹽�

	};
	CarTaxDATAS cardTaxDatas[MAXCOUNT];
	
}CarTaxBills_Struct; //����˰�˵�



//��ʾ����
typedef struct _tagCueInfo
{
	int  flag1;
	char info1[450+1];
	int  flag2;
	char info2[450+1];

}CueInfo, *PCueInfo;


//��ӡģ��
typedef struct _tagPrintTemp
{
	char date[11];		//����
	char tim[9];		//ʱ��
	char count[15];		//����
	char name[300];		//�û���
	char phone[50];		//����
	char card[30];		//����
	char serial[20];	//��ˮ��
	char stan[30];		//�ο���
	char month[31];		//������
	char type[60];		//ҵ������ 2012-01-31 �����޸�Ϊ60
	char total[30];		//�ϼ�
	char amo[30];		//ʵ��
	char detail98Amo[13];	//���ŷ�Ʊ�޸�20080129	
	char detail98Name[11];
	char capsTotal[50];	//�ϼƣ���д��
	char pos[9];		//�ն˺�
	int  n;				//��ϸ��"\n"�ĸ���
	char content[1024];	//��ӡ��ϸ

}PrintTemp, *PPrintTemp;


//���в�ѯ������ϸ
typedef struct _tagBanDetail
{
	char card[25];		//����
	char traDate[25];	//����
	char traTime[25];	//ʱ��
	char amount[13];	//���
	char serial[7];		//��ˮ
	char peStan[13];	//peϵͳ�ο���
	char uniStan[13];	//����ϵͳ�ο���

}BanDetail, *PBanDetail;

//�����ϸ20071224
typedef struct _tagMonitorDetail
{
	char posNum[8+1];	//�ն˺�
	char serial[6+1];	//�ն���ˮ
	char callTel[32+1];	//�������
	char traType[2+1];	//��������
	char stat[2+1];		//����״̬
	char rspCode[2+1];	//������
	char corrCode[9+1];	//bit62������
	char feeType[4+1];	//ҵ�����
	char amount[12+1];	//���׽��
	char bank[6+1];		//���д���
	char card[30];		//���׿���
	char traNum[30];	//���׺���
	char stan[13];		//ϵͳ�ο���

}MonitorDetail, *PMonitorDetail;


/*==========================*/

//���ݿ��bank_card�ṹ
typedef struct
{
	char card[10];  //����ǰ׺
	int card_len;   //����
	char bank[6+1];  //���д���
	int track_no;  //�ŵ�
	int offset;    //ƫ����
}Bank_card;
//���ݿ��bank_info�ṹ
typedef struct
{
	char bank[6+1]; //���д���
	int key; //�ܳ�����
}Bank_info;

//���ݿ��bank_busi�ṹ
typedef struct
{
	char bank[6+1];  //���д���
	char biz_type[4+1];  //ҵ�����
	char acc_area[4+1];//�ն˹����� 0000:ȫ�� ����:������
	int pos_type;    //�ն�����: 0:ȫ��; 1:EPOS; 2:IPOS
}Bank_busi;

//���ݿ��epos_group�ṹ
typedef struct
{
	int tgroup;  //���
	int group_type; //�˵����
	char welcome[500+1];//�̻������
	char update_info[300+1];//�ն˸�����ʾ��
	int eCard;  //�Ƿ�֧��e��
	int phs_ticket; //�����׿��Ƿ��ӡ��Ʊ
	int get_card;  //ˢ����־
	int DianXin_Turn;       //����ֱ�����ۿ����ء�0��ֱ�䣻1������
}Epos_group;

typedef struct
{   
	char area[4+1];
	char biz_type[4+1];
	char name[30+11];
	char info[300+1];
	int amount;
}Favorable_air_pay;

//���ݿ��epos_group�ṹ
typedef struct
{
	int tgroup;		//�ն����
	int asktype;	//��������
	char bank[6+1];	//���л�����
	int menu_group;	//�˵����
}Epos_menu_get;

//���ݿ��epos_group�ṹ
typedef struct
{
	int menu_group;	//�˵����
	char menu_ver[8+1];
	int mbl_flag;
}Epos_menu_group;

//���ݿ��epos_menu�ṹ
typedef struct
{
	int menu_id; //�˵����
	char name[30]; //�˵���
	char info[200+1];//�˵���Ϣ
}Epos_menu;

//���ݿ��epos_menu_conf�ṹ
typedef struct
{
	int menu_group; //�˵����
	int parent;	    //���˵����
	int currentm;	//��ǰ�˵����
	int child;		//�Ӳ˵����
}Epos_menu_conf;

//���ݿ��epos_pcard_conf�ṹ
// typedef struct 
// {
// 	char card_type[12+1]; //��Ϸ�����ͱ��
// 	char card_name[30]; //��Ϸ������
// 	char card_price[12+1];	//��Ϸ����Ǯ
// }Epos_pcard_conf;

//����Υ�²�ѯ���ؽṹ
typedef struct
{
	char paper_type[4+1]; //�������
	char paper_num[16+1];//�����
	char data[14+1];//Υ������
	char behavior[32+4];//Υ����Ϊ
	char area[32+4];//Υ���ص�
//	int tag;//0��δ���� 1���ѽ���
	
}car_paper_info;


typedef struct  
{
	int fee_group; //��������
	char fee_type[4+1]; //�������ҵ����
	char advertisement[1024];//�����
}Print_adv;

typedef struct  
{
	SOCKET sd;//�˿�
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
	int flag;//0δ��1����
}CDMA_INFO;

typedef struct  
{
	int flag;//0:δʹ��1:��ռ��2:������3:�������
	char stan[12+1];//��ˮ��
	char data[1024];//����
	time_t inTime;//д���ڴ��ʱ��
	int data_len;
}Bus_order;

typedef struct  
{
	char bus_charge_key[8+1];//��ֵ����
	char bus_discharge_key[16+1];//������Կ
}Bus_key;

typedef struct  
{
	char pos_flag[1+1];//0:�ʺ����У�1:�ʺ�EPOS��2:�ʺ�IPOS
	int menu_id;
	char tips[300+1];
	char return_flag[1+1];//0:���������˵���1:�������˵�
}Epos_tips;

typedef struct  
{
	char biz_type[4+1];		//ҵ�����
	char card_type[4+1];	//��ҵ�����
	long amount;			//���ų�ֵ���ѿ���ֵ
}Sell_Card_Amo;

typedef struct  
{
	char mobile_field[3+1];//�ֻ������
	int mobile_owner;//1�����ţ�2���ƶ���3����ͨ
}Epos_mobile;


typedef struct  
{
	char AREACODE[6+1];//ˮ��úҵ��ͨ����
	char DX_CODE[4+1]; //���ŵ�������
	char AREA_NAME[60]; //��������
	char PARENT_CODE[6+1]; //���׵�������
	char PARENT_NAME[60+1];//���׵�������
	int  AREA_FLAG; //���������־ 1Ϊһ����2Ϊ2��
	char SHORTNAME[60];//�������

}SDM_AREA; //ú��ͨ��ˮ����


typedef struct _tagsdmBills_Struct  
{
	int nCnt;	//�˵���
	char payment_name[100]; //�շѵ�λ

	struct billDatas
	{
		char CUSTOMERNAME[64+1]; //�ͻ�����
		char BILLBARCODE[200+1]; //������
		char BILLNO[200+1]; //���ѵ���
		char BILLMONTH[10+1]; //����
		char BILLBATCH[12+1]; //����
		char CONTRACTNO[32+1];//��ͬ��
		char BILLAMOUNT[12+1]; //�ɷѽ��
		char BILLDELAY[12+1]; //���ɽ�
		char BALANCE[12+1]; //�˻����
		char BILLDATE[14+1];//��������
		char LASTPAYDATE[14+1]; //���ɷ�����
		char BILLSTATUS[3+1];//�˵�״̬
		char PASSWORD[64+1]; //�˵�����
		char THIRDCODE[8+1]; // ����������
		
	};

	billDatas BillDetails[MAXCOUNT];
	
}sdmBills_Struct,*SDM_BILLS_DATA; 


typedef struct  
{
	char PAYMENTCODE[30+1];//ˮ��ú���ѵ�λ���� ����18λ
	char BUSCODE[3+1]; //ҵ����� 001:ˮ��002���磻003��ú 004��ů��
	char PAYMENTNAME[128+1]; //���ѵ�λ���� ��xxˮ��˾
	char  BUSNAME[64+1]; //ˮ�ѻ��ߵ��
	
}BUSDATAS  ; //ˮ��ú���ѵ�λ��Ϣ




typedef struct  
{
	char title_message[30+1];//������ʾ��
	char sys_new_message[1024];//ϵͳ������Ϣ
	int my_group;//�������
}Epos_new_message;


typedef struct
{
	char stan[16+1];			//ϵͳ�ο���
	char txn_date[8+1];			//��������
	char txn_time[6+1];			//����ʱ��
	char biz_type[4+1];			//ҵ�����
	int  status;					//����״̬
	char term_id[8+1];			//�ն˺�
	char term_serial[6+1];		//��ˮ��
	char bank[8+1];				//���д���
	char card_no[20+1];			//���п���
	char amount[12+1];			//���׽��
	char yct_serial[16+1];		//���ͨ������ˮ��
	char pki_card[8+1];			//�����߼�����
	char ticket_no[16+1];		//���ͨƱ������
	char ticket_amo_bef[12+1];	//���ͨƱ����ֵǰ���
	char ticket_amo_aft[12+1];	//���ͨƱ����ֵ�����
	//char pki_pin[7];			//������pin��
	char result_message[1024];	//����Ƽ��汾��������������

	/**************************���������ͨ��������*********************/
	
	char ticket_physicalNo[16+1];//���ͨƱ��������        
	BYTE SKEY[32];                 //������Կskey ʹ��AES ���� 
	BYTE SHID[16+1];			  //���ͨ������ˮ��8���ֽ�
	int  yct_sequence_NO;			//���ͨ�������	
	char downLimit [1+1];		//Ǯ������
	char upperLimit[3+1];		//Ǯ������
	char yct_stan[16+1]	;		//���ͨ��ֵ������ϵͳ��ˮ��
	char org_yctstan[12+1];     //ԭ��ֵϵͳ��ˮ�ţ���ֵ�����ã�
	
	BYTE card_status ;			//��״̬aa64����ʱ��
	char SW1SW2[4+1];			//������״̬��
	
	BYTE BLACKLIST_CARDINFO[32+1];		//����������Ϣ

	char trader_id[16+1]; //�̻����
	char trader_name[40+1];//�̻����
	char out_logicalcard[16+1] ;// ת���߼�����
	char note [128+1]; //�ɹ���ע
	char tradeTime[14+1]; //��ֵʱ��
	char TAC[8+1]; //��ֵ��֤��
	char yct_termid[12]; //���ͨ�ն˺�
	int  trade_count; //���ͨƱ�����׼���
	char trade_type[32+1];//��������
	char charge_authcode[8+1];//��ֵƾ֤��
	char result_message_third[1024];	//�ύ����
	int com;
	int comType;
	char SignIn_ServerTime[30+1];//ǩ���ɹ�ʱ�ĺ�̨ʱ��
	char SAK[1+1];//SAK
    char random_num[16+1];//�����
	int yct_ack; //�Ƿ��ύcommit ���û��ִ��commit ����Ҫ���·���commit
	/*****************���ͨ�������ת��*******************/
	/*  added by Wudp 20120607                            */
	/******************************************************/
	char extension_stan[16+1];		//���ͨ��ֵ��������չӦ����ˮ��
	char transfer_stan[16+1];       //���ͨ��ֵ��������תֵ��ˮ��
	//ת����
	char out_logicardno[16+1];		//ת�����߼�����
	char out_balance[12+1];	        //ת�������
	char out_balance_bef[12+1];	    //ת����ԭ���
	char out_balance_aft[12+1];	    //ת�ƺ�ת�������
	char out_downLimit [1+1];		//ת����Ǯ������
	char out_upperLimit[3+1];		//ת����Ǯ������
	char out_cardappinfo[40+1];     //ת����Ӧ����Ϣ

	//ת�뿨
	char in_logicardno[16+1];		//ת�뿨�߼�����
	char in_balance[12+1];	        //ת�뿨���
	char in_balance_bef[12+1];	    //ת�뿨ԭ���
	char in_balance_aft[12+1];	    //ת�ƺ�ת�뿨���
	char in_downLimit [1+1];		//ת�뿨Ǯ������
	char in_upperLimit[3+1];		//ת�뿨Ǯ������
	char in_cardappinfo[40+1];		//ת�뿨Ӧ����Ϣ
	/*****************���ͨ�������ת��******************/
    
    


	/**************************���������ͨ��������*********************/

}YCT_DATA, *PYCT_DATA;


typedef struct
{
	char ticket_type;			//��Ʊ���� 01���� 02���� 03��������
	char birth_date;			//�ײ���Ч���� 01���� 02����
	char railway_type;			//�����ײ����� 01�޴�
	char railway_num;			//�����޴��ײ͵Ĵ��� HEX
	char bus_type;				//�����ײ����� 01�޴� 02����
	char bus_num;				//�����޴��ײ͵Ĵ��� HEX
	char bus_way1[5+1];			//��������·1
	char bus_way2[5+1];			//��������·2
	char bus_way3[5+1];			//��������·3
}YCT_DETAIL, *PYCT_DETAIL;

typedef struct  
{
	int biz_flag;		//ҵ�����ͣ�1:���ſճ䣨�����޲�ѯ��;2:�ƶ��ճ�;3:��ͨ�ճ䣨������;4:��ͨ�ճ䣨ȫʡ��;5:���ſճ䣨����ѯ���;6:�ۿ�;7:Q�ҳ�ֵ;8:��ͨ���� �ȵȲ���������
	char area[4+1];		//�������������
	char accarea[4+1];	//�ʻ����������ţ���������������Ų�Ϊ0000�����ʻ�����������Ҳ����Ϊ0000��
	char biz_type[4+1];	//ҵ�����
	int status;
}Epos_area_biz;

typedef struct  
{
	CADOConn adoConn;	//һ�����ݿ�����
	int connStat;		//������״̬��0��δ�ã�1�����ã�2��ʧЧ
	DWORD threadID;		//ռ�ø����ӵ��߳�ID
	char termId[8+1];	//ռ�ø����ӵ��ն˺�
	char connDate[8+1];	//ռ�ø�����ʱ������
	char connTime[6+1];	//ռ�ø�����ʱ��ʱ��
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
	char area[20+1]; //����
	char province[20+1]; //ʡ��
	int field_len;

}Mobile_Field_Area;

typedef struct
{
	char phone_head_no[11+1];
}PHONE_HEAD_NO;

//����ֱ����Ϣ
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


//��ݸͨ��Ƭ������Ϣ
typedef struct
{
	char cardType[2+1];//��Ƭ����
	char cardInfo[255]; //��Ƭ��Ϣ
	char otherData[255]; //������Ϣ(A�����ص�)
	char CItyCode[2+1]; //���б���
	char compay_code[2+1]; //��������λ
	char busi_code[2+1]; //��ҵ����
	char deposit[2+1]; //Ѻ��
	char appSerial[8+1];//Ӧ�����
	char appFlag[1+1]; //Ӧ�ñ�־ 
	char cardParent_type[1+1]; //��������
	char cardChild_type[1+1]; //��������
	char openDate[4+1]; //��������
	char effectiveDate[4+1]; //����Ч����
	char deviceInfo[6+1]; //�����豸��Ϣ
	char appVer[2+1];//Ӧ�ð汾��Ϣ
	char cardMny[12+1]; //��Ƭ���
	char CARDSEQ[6+1]; //��Ƭ������ˮ
	char KEYVER[2+1]; //��Ƭ��Կ�汾
	char ALGIND[2+1]; //��Ƭ�㷨��־
	char CARDRAND[8+1]; //��Ƭ�����
	char MAC1[8+1]; //���ʷ�����ϢMAC1
	char MAC2[8+1]; //Ǯ����ֵ��Ҫ�� MAC2
	char cardShowId [16+1];  //ʮ���Ʊ�ʾ�Ŀ���

	char CHARGE_DIVDATA[18+1]; //��ɢ����(��ֵ��ʱ��ķ�ɢ����)
	char CARDSALE_DIVDATA[18+1]; //�ۿ���ʱ��ķ�ɢ����
	char CARDID[8+1]; //���ں�
	char AUTHSEQ[10+1]; //��Կ��Ȩ��ˮ��
	char LIMITEAUTHSEQL[10+1]; //�����Ȩ��ˮ��
	char DICMAC[8+1];
	char CARDCNT[6+1]; //���ڼ�����
	char CARDVERNO[2+1]; //���ڰ汾��
	char EDCARDID[16+1]; //����
	char connectCity[2+1];
	char tradeMny[8+1];//�������
	char tradeAfterMny[8+1]; //���׺����
	char tradeBeForeMny[8+1]; //����ǰ���
	char tradeTime[6+1]; //����ʱ��123012
	char tradeDate[8+1]; //�������� 20121010
	char TAC[8+1]; //TAC
	char LoadSEQ[4+1]; //���ʽ�����ˮ��
	char lastPosSeq[9+1]; //�ϱʽ���pos��ˮ��
	char posSeq[9+1]; //�����ն���ˮ��
	char sysTime[14+1]; //20140123102412 ����ʱ��
	char MESSAGEDATETIME[14+1]; //���ķ���ʱ��
	char APDU[70]; //�����������ʱ���ʹ�õ�
	char cardOperType[2+1]; //��Ƭ��������
	char sale_type[2+1]; //�����۷�ʽ
	char BATCHNO[6+1]; //���κ�
	char KEYSET[64+1]; //��Կ
	char SETDATE[8+1]; //��������
	char KEEP[32+1]; //�����������ʱ���������ˮ��
	char resubmitField[1024]; //��ݸ�ύ����
	char openTime[6+1]; //����ʱ��

}DGTCard,*PDGTCard;


//��ݸͨȷ�ϲ�����Ҫ������Ŀǰ��Ҫ���ڶ�ݸͨ���� add by liuxiao 

typedef struct
{
	char TMNNUMNO[12+1]; //�ͻ��ն˺�
	char SYSTEMNO[20]; //ϵͳ�ο���
	char TRANSTYPE[4]; //ҵ������
	char TAC[2+1];//����Ѻ��
	char APTRANSSEQ[16]; //ԭ������ˮ��
	char TRADETIME[15]; //����ʱ��
	char EDCARDID[17]; //������������
	char CARDID[17]; //���ں�
	char CARDCNT[7]; //��������
	char CARDMKND[3]; //��������
	char CARDSKND[3]; // ��������
	char CARDMODEL[2]; //����
	char SALEMODE[3]; // �ۿ���ʽ
	char DEPOSIT[9]; //Ѻ��
	char BEFBALANCE[9]; // ����ǰ���
	char TXNAMT[9]; //���׽��
	char CARDVALDATE[9]; //	����Ч��
	char CITYCODE[5]; //  ���ڳ��д���
	char CARDVERNO[3]; // 	���ڰ汾��
	char BATCHNO[7]; //���κ�
	char AUTHSEQ[19]; //��Ȩ��ˮ��
	char LIMITEDAUTHSEQL[11]; //�����Ȩ��ˮ��
	char KEYSET[65]; //��Կ

}DGTResubmitInfo,*PDGTResubmitInfo;



//��ݸͨM1����Ƭ������Ϣ

typedef struct
{
	char bus_card[8+1]; //��������
	char bus_amt[8+1];
	char bus_card_type[2+1]; //Ӧ���ӿ�����
	char check_date[6+1];//��������
	char psam[16+1]; //psam����
	char recogise_code[8+1]; //��֤��
	char dkq_code[4+1];
	char card_parent_type[2+1];
	char card_child_type[2+1];
	char M1CardCSN[8+1];
	char bus_showCard[16+1];
	char city_code[4+1];
	char cardVadate[8+1]; //��Ч��
	char appSerial[6+1]; //Ӧ�ÿ����
	char openDate[8+1]; //��������
	char openTime[4+1]; //����ʱ��
	char deposit[6+1]; //Ѻ��
	char cardStatus[2+1]; //��Ƭ״̬


}DGTM1Card,*PDGTM1Card;


//��ݸ������ֵ��¼ ���4�ʽ���
typedef struct _tagdgtChargeTrades 
{
	int nCnt;	//��ֵ��¼��
	
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


//��ݸ���Գ�ֵ�����ѹ�����ֵ��¼���10�ʽ���
typedef struct _tagdgtALLTrades
{
	int nCnt;	//��ֵ��¼��
	
	struct cardALLTradeDetails
	{
		char cardSEQ[4+1];
		char cardId[8+1]; //����
		char limitMny[6+1];// ͸֧�޶�
		char tradeMny[8+1]; //���׽��
		char tradeType[2+1]; //��������
		char POS_ID[12+1]; //�����ն�
		char trade_date[8+1]; //��������
		char trade_time[6+1]; //����ʱ��
	};
	
	cardALLTradeDetails cardAllTradeDetail[10];
	
}dgtALLTrades,*pdgtALLTrades; 


//��ݸ����ǩ����Ϣ
typedef struct  
{
	char POSID[12+1]; //�����豸����
	char KEYSET[32+1]; //��Կ��Ϣ
	char SAMID[16+1]; //SAM����
	char SAMAUTHINFO[16+1]; //SAM����Ȩ��Ϣ
	char EDAUTHINFO[16+1]; //ED����Ȩ��Ϣ
	char SETTDATE[8+1]; //��������
	char BATCHNO[7+1]; //���κ�
	char SYSDATETIME[14+1]; //����ʱ��
	char AUTHCODE[24+1]; //��Ȩ��֤�루M1��
	int  sign_status;

	char TOTALSVNUM[10+1]; //�������ۼ��ۿ����ʽ����ܱ���
	char TOTALSVAMT[10+1]; //�����ۼƳ�ֵ�����ܽ��֣�
	char TOTALSALEDEP[10+1]; //�����ۼ��ۿ�����Ѻ���ܽ��֣�
	char LOG_SEQ[10+1]; //ǩ����ˮ

 
}DGTSignInfo,*PDGTSignInfo; 


//��ݸ�����ն���Ϣ
typedef struct  
{
	char POSID[12+1]; //�����豸����
	char BUSI_ID[12+1]; //��Ӧ��ҵ�˻��ն�
	char POSKEY[32+1]; //�ն�˽Կ
	
}DGTDeviceInfo,*PDGTDeviceInfo; 


//��»qq
typedef struct  
{
	char PRODUCTCODE[10]; //��Ʒ����
	char ACCTCODE[100]; //�˻�����
	char POSKEY[32+1]; //�ն�˽Կ
	char RECHNUM[10]; // ��ֵ���� ��Ʒ����Ϊ0ʱ��λΪ���������൥λΪ����
	char RECHAMOUNT[12+1]; //��ֵ���
}GLQQPayInfo,*PGLQQPayInfo; 


typedef struct  
{
	char phone[16];
	char month[9];
	char item_name[30];
	char fee[10];
	int flag;
}Bill_Item;

//ˮ��úҵ��
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

//ˮ��úҵ��
typedef struct  
{
	char phone[16];
	char month[9];
	char item_name[30];
	char fee[10];
	int flag;
}SDM_Bill_Item;

//��������Ϣ
typedef struct 
{
	char rsp_id[12];
	char party_code[7];
	char bit39_rsp[3];
	char bit62_rsp[3];
	char busi_desc[300];
}Fail_Message;

/*----------------���ݿ��----------------*/

#define MAX_PRINT_ADV 50 //���������
#define MAX_PRINT_ADV_ITEM 300 //���������
#define MAX_MENU  100        //���˵�����
#define MAX_MENU_ITEM  1000  //���˵�����
#define MAX_HOT_KEY  6		//����ݼ���20071124

#define MAX_CDMA_INFO 100  //���CDMA�ֻ���
#define MAX_BUS_ORDER 800  //�빫�������ڴ�ռ������


#define MAX_BATCH_VALUE_INFO 25 //���Batch_Value_Info����

#define MAX_BILL_NUMBER 30	//����ֱ�仰�Ѳ�ѯ�˵�����
#define MAX_BILL_ITEM_NUMBER 50  

//н��н����״̬
#define XJX_TRANS_SUCC             "0"  //���׳ɹ�
#define XJX_TRANS_FAIL             "1"  //����ʧ��
#define XJX_NOT_CHECKED            "2"  //����δ���
#define XJX_LLCHECK_PASSED         "3"  //������ͨ���������
#define XJX_LLCHECK_NOTPASS        "4"  //���׶�����˲�ͨ��
#define XJX_LLLCHECK_PASSED        "5"  //������ͨ���������
#define XJX_NOT_PASS               "6"  //����������˲�ͨ��
#define XJX_TRANS_REFUSE           "7"  //���׾ܾ������׽���������˽�

/*---------------���ͨ����---------------*/
#define YCT_CARD_UNITPRICE         25   //���ͨ�����ۣ�Ԫ/�ţ�
#define YCT_CARDDELIVERY_COSTS     8    //���ͨ���Ϳ�ݷѣ���λԪ�����ڹ�������С��10��ʱʹ�ã�
#define YCT_PACKAGE_UNITPRICE      1388 //���ͨ�ײ͵���


/*----------------------------------------*/
//extern Epos_menu_conf menu_conf[MAX_MENU][MAX_MENU_ITEM];   //�˵����ñ�Epos_menu_conf
//extern int menu_cnt[MAX_MENU];								//�˵���Ĳ˵���Ŀ��
extern Print_adv print_adv[MAX_PRINT_ADV][MAX_PRINT_ADV_ITEM]; //���ݿ��Print_adv
extern int print_adv_cnt[MAX_PRINT_ADV];                    //����print_adv�ĸ���
extern Bus_order bus_order[MAX_BUS_ORDER];                  //�빫�������Ĵ洢�ڴ�ռ�
extern Bus_key bus_key;                                     //�洢����ǩ����ĳ�ֵ�����Լ�������Կ

//���ݿ��Bank_card
extern vector<Bank_card> bank_card;

//���ݿ��Bank_info
extern vector<Bank_info> bank_info;

//���ݿ��Bank_busi
extern vector<Bank_busi> bank_busi;

//���ݿ��Epos_group
extern vector<Epos_group> epos_group;

//���ݿ��Favorable_air_pay
extern vector<Favorable_air_pay> favorable_air_pay;

//�ն������˵��Ķ�Ӧ��epos_menu_get
extern vector<Epos_menu_get> epos_menu_get;	

//�˵�����epos_menu_group
extern vector<Epos_menu_group> epos_menu_group;		

//�ǿ�������Ϸ�����ñ�Epos_pcard_conf
//extern vector<Epos_pcard_conf> pcard_conf;  

//�˵���ű�Epos_menu
extern vector<Epos_menu> epos_menu;		

//�˵���Ĳ˵���Ŀ��
extern vector<int> menu_cnt;

//�˵����ñ�Epos_menu_conf
extern vector<vector<Epos_menu_conf> > menu_conf;   
							

//�洢epos_tips������
extern vector<Epos_tips> epos_tips;
		
//�洢sell_card_amo(��俨���)������
extern vector<Sell_Card_Amo> sell_card_amo;

//�洢���ݿ��epos_mobile������
extern vector<Epos_mobile> epos_mobile;

//�洢���ݿ��epos_new_message
extern vector<Epos_new_message> new_message; 
	
//�洢���ݿ��epos_area_busi
extern vector<Epos_area_biz> area_biz;

//�㶫��������
extern int field_group[200];
extern vector<Mobile_Field_Area> gd_field;
extern int epos_mobile_totalNum;
//���淵�����Ӧ������Ϣ
extern vector<Fail_Message> fail_message;

//ˮ��úҵ��
extern int sdm_group[200];
extern vector <SDM_AREA> sdm_area;
extern vector <BUSDATAS> sdm_datasVector; //ˮ��úҵ����Ϣ


extern void wrMonitorLog(int type, int stat, int rspCode, CGlobal gbl, CPack pack);

/*--------------------------��������---------------------------*/
extern int TRADE_FLAG;		//�������ͣ�0: ���ԣ�1: ����
extern int MAX_CONNECT_NUM; //���ݿ����������
extern int DEBUG_LEVEL;		//���Լ���Խ�������־Խ��
extern int FSK_DEBUG;
extern char DB_CON_STR[500];//���ݿ������ַ���
extern char DB_CON_STR_EN[500];//���ݿ������ַ������û���������Ϊ����
extern char MERID[16+1];	//����
extern char TMNNUM[12+1];	//���������ն˺�
extern char CHANNELCODE[2+1]; //��������
extern int CARD_TYPE;		//���׿�����, 0x00���󿨣�0x01��1��С��������أ���0x02��2��С��
extern CueInfo cueInfo;		//�˵�ǰ��ʾ��Ϣ
extern int TRADE_STAT;		//����״̬��0��������1����ͣ
extern char CONNECT_SENTENCE[100]; //�������
extern char PE_IP[20];				//PE��������ַ
extern int  PE_PORT;				//PE�������˿�
extern int LISTEN_PORT;				//�����˿�
extern char DGBUS_IP[20];			//	��ݸ����������IP
extern int DGBUS_PORT;				//��ݸ�����������˿�

extern char DGTBUS_IP[20];
extern int DGTBUS_PORT;
extern char DGT_Unitid[9];			//��ݸͨ��Ӫ

extern int  YCT_VER;				//���ͨ�������汾
extern char YCT_IP[20];				//���ͨ��������ַ
extern int  YCT_PORT;				//���ͨ�������˿�
extern char YCT_MONITOR_IP[20];				//���ͨ��ط�������ַ
extern int  YCT_MONITOR_PORT;				//���ͨ��ط������˿�
extern char YCTYP_IP[20];			//���ͨ��Ʊ��������ַ
extern int  YCTYP_PORT;				//���ͨ��Ʊ�������˿�
extern char AIRPAY[4+1];			//���г�ֵҵ�����
extern char pepwd_ip[16+1];			//�ϰ������������ַ
extern char pepwd_port[5+1];		//�ϰ������������ַ
extern char ecard_ip[16+1];         //E����ѯ�����Ѳ�ѯ��ַ
extern char ecard_ser_port[5+1];	//e����ѯ�������˿�
extern char prmVer[(4+1)*10];		//��ǰ���ն˳���汾��
extern char prmMD5[(32+1)*10];		//��ǰ���ն˳����MD5
extern char excMAC[(16+1)*10];		//�������õĻ�����
extern TSFtpFiles ftpFiles;			//���ն�Զ�̸�����Ϣ
extern int DG_BUS_FLAG;				//��ݸ������־.0���޶�ݸ������1:�ж�ݸ����
extern int YiDong_Turn;             //�ƶ�ֱ�����ۿ����ء�0��ֱ�䣻1���ۿ�
extern int LianTong_Turn;           //��ֱͨ�����ۿ����ء�0��ֱ�䣻1���ۿ�
extern char PRM_TITLE[100];			//�������
extern char CT10000_TERM[1000+1];	//��������CT10000��ʹ�ó�ֵ���ѿ����ն�,00000000��ʾ��������
extern char FORBID_BIZ[100];		//�������
extern int FORBID_TIME;				//�������ظ���ֵ��ʱ��
extern int ONLINE_NUM;				//��ǰ������
extern int MAX_ONLINE_NUM;			//�������������
extern int REFRESH_PORT;
extern char REFRESH_KEY[33];
extern char REFRESH_IP[16*100];
extern char COACH_TICKET_IP[20];	//��;����Ʊ��������ַ
extern int  COACH_TICKET_PORT;		//��;����Ʊ�������˿�
extern int service_stop; //�սῪʼʱ��
extern int service_start;//�ս����ʱ��
extern Ado_Conn_Info g_adoConnInfo[DB_MAX];//���ݿ�����
extern Coninfo allConinfo[CON_MAX];//�ն�������Ϣ�洢��

//extern char DIY_DCN_URL[30];	//����DIY
extern char CS_DCN_URL[30];		//�����ͷ�
//extern char DIY_GBL_URL[30];	//����DIY
extern char CS_GBL_URL[30];		//�����ͷ�

#endif