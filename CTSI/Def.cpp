#include "Def.h"
#include "afxmt.h"
#include "WrLog.h"
#include "Tools.h"
#include "Connect.h"
#include "ADOConn.h"

#pragma comment(lib, "TSCTSI.lib")
#pragma comment(lib, "wsock32.lib")

char logon_random1[8+1];	//logon����ǰ8λ
char logon_random2[8+1];	//logon���غ�8λ������ǩ��key

int logon_flag = 0;		//ǩ����־����ʼΪ0��һ����Ч
int yct3_initsign_flag = 0;		//���ͨ��ֵ������ǩ����־����ʼΪ0��һ����Ч

Ado_Conn_Info g_adoConnInfo[DB_MAX];//���ݿ�����
Coninfo allConinfo[CON_MAX];//�ն�������Ϣ�洢��

int syn_flag;  //ͬ����־
CCriticalSection cs_g_connStat;	//���ݿ�������Դ��
CCriticalSection cs_ONLINE_NUM;	//������������
CCriticalSection cs_FILE;		//��־�ļ���

CWrLog log;



// Epos_menu_conf menu_conf[MAX_MENU][MAX_MENU_ITEM];   //�˵����ñ�Epos_menu_conf
// int menu_cnt[MAX_MENU];						 //�˵���Ĳ˵���Ŀ��
Print_adv print_adv[MAX_PRINT_ADV][MAX_PRINT_ADV_ITEM]; //���ݿ��Print_adv
int print_adv_cnt[MAX_PRINT_ADV];             //����print_adv�ĸ���
Bus_order bus_order[MAX_BUS_ORDER];                  //�빫�������Ĵ洢�ڴ�ռ�
Bus_key bus_key;                                     //�洢����ǩ����ĳ�ֵ�����Լ�������Կ


//���п���Bank_card
vector<Bank_card> bank_card;	

//������Ϣ��Bank_info
vector<Bank_info> bank_info;	

//����ҵ���Ӧ��Bank_busi
vector<Bank_busi> bank_busi; 

//���ݿ��Epos_group
vector<Epos_group> epos_group;

//���ݿ��Favorable_air_pay
vector<Favorable_air_pay> favorable_air_pay;

//�ն������˵��Ķ�Ӧ��epos_menu_get
vector<Epos_menu_get> epos_menu_get;	

//�˵�����epos_menu_group
vector<Epos_menu_group> epos_menu_group;

//�ǿ�������Ϸ����Epos_pcard_conf
//vector<Epos_pcard_conf> pcard_conf;

//�˵���ű�Epos_menu
vector<Epos_menu> epos_menu;	

//�˵���Ĳ˵���Ŀ��
vector<int> menu_cnt;

//�˵����ñ�Epos_menu_conf
vector< vector<Epos_menu_conf> > menu_conf;   


//�洢epos_tips������
vector<Epos_tips> epos_tips;

//�洢sell_card_amo(��俨���)������
vector<Sell_Card_Amo> sell_card_amo;	

//�洢���ݿ��epos_mobile������
vector<Epos_mobile> epos_mobile;

//�洢���ݿ��epos_new_message
vector<Epos_new_message> new_message;
			
//�洢���ݿ��epos_area_busi
vector<Epos_area_biz> area_biz;

//�㶫��������
int field_group[200];
vector<Mobile_Field_Area> gd_field;
int	epos_mobile_totalNum=0; //��������

//���淵�����Ӧ������Ϣ
vector<Fail_Message> fail_message;

int sdm_group[200]; //ˮ��úҵ��
vector <SDM_AREA> sdm_city;
vector <BUSDATAS> sdm_datasVector; 



/*--------------------------��������---------------------------*/
char DB_CON_STR[500] = "";	//���ݿ������ַ���
char DB_CON_STR_EN[500]=""; //���ݿ����Ӵ�����
char MERID[16+1]=""			; //����
char TMNNUM[12+1]="";		;//���������ն�
char CHANNELCODE[2+1];		 //��������
int TRADE_FLAG  = 0;		//�������ͣ�0: ���ԣ�1: ������2:����
int CARD_TYPE = 0;			//���׿�����, 0x00���󿨣�0x01��1��С��������أ���0x02��2��С��
int MAX_CONNECT_NUM = 10;	//���ݿ����������
int DEBUG_LEVEL = 1;		//���Լ���Խ�������־Խ��
int FSK_DEBUG = 1;
CueInfo cueInfo;			//�˵�ǰ��ʾ��Ϣ
int TRADE_STAT = 0;			//����״̬��0��������1����ͣ
char CONNECT_SENTENCE[100]; //�������
char PE_IP[20];				//PE��������ַ
int  PE_PORT;				//PE�������˿�
int LISTEN_PORT;			//�����˿�
char DGBUS_IP[20];           //��ݸ�����������˿�
int DGBUS_PORT;					//��ݸ�����������˿�

char DGTBUS_IP[20];				//��ݸͨ����IP
int DGTBUS_PORT;                 //��ݸͨ�����˿�
char DGT_Unitid[9];				//	��ݸͨӪ�˵�λ

int YCT_VER;				//���ͨ��ֵ�������汾��
char YCT_IP[20];			//���ͨ��������ַ
int  YCT_PORT;				//���ͨ�������˿�
char YCT_MONITOR_IP[20];    //���ͨ��ط�������ַ
int YCT_MONITOR_PORT;       //���ͨ��ض˿�
char YCTYP_IP[20];			//���ͨ��Ʊ��������ַ
int  YCTYP_PORT;			//���ͨ��Ʊ�������˿�
char AIRPAY[4+1];			//���г�ֵҵ�����
char pepwd_ip[16+1];		//�ϰ������������ַ
char ecard_ip[16+1];        //E����ѯ�����������ѯ��������ַ
char pepwd_port[5+1];		//�ϰ������������ַ
char ecard_ser_port[5+1];	//e����ѯ�������˿�
char prmVer[(4+1)*10];		//��ǰ���ն˳���汾��
char prmMD5[(32+1)*10];		//��ǰ���ն˳����MD5
char excMAC[(16+1)*10];		//�������õĻ�����
TSFtpFiles ftpFiles;		//���ն�Զ�̸�����Ϣ
int DG_BUS_FLAG;			//��ݸ������־.0���޶�ݸ������1:�ж�ݸ����
int YiDong_Turn = 0;        //�ƶ�ֱ�����ۿ����ء�0��ֱ�䣻1���ۿ�
int LianTong_Turn = 0;      //��ֱͨ�����ۿ����ء�0��ֱ�䣻1���ۿ�
char PRM_TITLE[100];		//�������
char CT10000_TERM[1000+1];	//��������CT10000��ʹ�ó�ֵ���ѿ����ն�,00000000��ʾ��������
char FORBID_BIZ[100];		//�������
int FORBID_TIME=0;			//�������ظ���ֵ��ʱ��(����)
int ONLINE_NUM = 0;			//��ǰ������
int MAX_ONLINE_NUM = 100;	//�������������
int REFRESH_PORT;
char REFRESH_KEY[33];
char REFRESH_IP[16*100];
int service_stop; //�սῪʼʱ��
int service_start; //�ս����ʱ��
char COACH_TICKET_IP[20];	//��;����Ʊ��������ַ
int  COACH_TICKET_PORT;		//��;����Ʊ�������˿�

char DIY_DCN_URL[30];		//����DIY
char CS_DCN_URL[30];		//�����ͷ�
char DIY_GBL_URL[30];		//����DIY
char CS_GBL_URL[30];		//�����ͷ�