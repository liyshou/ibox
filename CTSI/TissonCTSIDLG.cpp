// TissonCTSIDlg.cpp : implementation file

/*****************2006-10-30*******************/
//����������л���
//���ݿ�������
/**********************************************/

#include "stdafx.h"
#include "TissonCTSI.h"
#include "TissonCTSIDlg.h"
#include "DB.h"
#include <winsock2.h>
#include "packet.h"
#include "Trade.h"
#include "Global.h"
#include "MenuHdl.h"
#include "WrLog.h"
#include "ADOConn.h"
#include "Def.h"

#include "Connect.h"
#include "DGBusTrade.h"
#include "Des.h"
#include "YCTYPTrade.h"
#include "MyCrypto.h"

int flash_flag = 1;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***********************************************/





/***********************************************/


/****************���п���*****************/
//e����1��key
char *CARD_TABLE_ECARD[ECARD_TABLE_MAX]
={
	"1088"//e��
  };

//���У���4��key
char *CARD_TABLE_CMB[CMB_TABLE_MAX]
={
    "690755",                         
    "429331",
    "69545",
    "545619",
    "54562",
    "54594",
    "42933",
    "95555",
    "62258",
    "468203",
    "410062",
    "512425",
    "524011"
  };
//����: ��7��key
char *CARD_TABLE_CCB[CCB_TABLE_MAX]	
={
    "5544033",    
    "5491031",
    "553242",
    "5453242",                               
    "436742",
    "437642",
    "453242",
    "53242",
    "54532",
    "54411",
    "49103"

};
//ũ��: ��3��key
char *CARD_TABLE_ABC[ABC_TABLE_MAX]   
={
    "63",  
    "434401",
    "95599",
    "53591",
    "49102",
    "622848"
 };

//����: ��17��key
char *CARD_TABLE_CIT[CIT_TABLE_MAX]	
={
     "40551",
     "60142",
	 "53783",
	 "49104",
	 "66537",
	 "62225"
  };

//������ҵ����: ��6��key
char *CARD_TABLE_GCB[GCB_TABLE_MAX]	
={
     "622467",
     "940016",
	 "603445",
  };

//���У���2��key
/*char *CARD_TABLE_ICBC[ICBC_TABLE_MAX] 
={
    "62",        
    "662521",
	"95588",
	"53098",
	"4270",
	"5309",
	"5482",
	"55836",
	"5187",
	"4381",
	"4580",
	"4518"
  };	


//�ʴ�: ��5��key
char *CARD_TABLE_PSD[PSD_TABLE_MAX] 
={
    "60",
    "12345",
    "9551"
  };

//����: ��11��key
char *CARD_TABLE_BOC[BOC_TABLE_MAX] 
={
     "04",
     "5183",
     "5184",
     "847",
     "5588",
     "5477",
     "4241",
     "4563"
};

//�㷢:��9��key
char *CARD_TABLE_GDB[GDB_TABLE_MAX]	
={
     "68",
     "9111"
  }; 

//ũ��:��13��key
char *CARD_TABLE_NXB[NXB_TABLE_MAX]	
={
     "01909",
	  "94",
	  "909810",
	  "940035"
  };

//����: ��19��key
char *CARD_TABLE_MSB[MSB_TABLE_MAX] 
={
     "4155",     
  };

//����: ��15��key
char *CARD_TABLE_ZXB[ZXB_TABLE_MAX]	
={
     "4336",
     "9688",
     "62269"
  }; */
/*****************************************/

//extern CGlobal gbl;



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTissonCTSIDlg dialog

CTissonCTSIDlg::CTissonCTSIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTissonCTSIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTissonCTSIDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTissonCTSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTissonCTSIDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTissonCTSIDlg, CDialog)
	//{{AFX_MSG_MAP(CTissonCTSIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(FRESH_BUTTON, OnFlashDB)
	ON_BN_CLICKED(SENDMSG_BUTTON, sendMsg)
	ON_BN_CLICKED(SENDINFO_BUTTON, sendInfo)
	ON_BN_CLICKED(SENDBILL_BUTTON, sendBill)
	ON_BN_CLICKED(SENDRESULT_BUTTON, sendResult)
	ON_BN_CLICKED(CHG_STAT, OnChangeStat)
	ON_BN_CLICKED(UPDATE_PAR, OnUpdatePar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTissonCTSIDlg message handlers

BOOL CTissonCTSIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
	OnOK();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTissonCTSIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTissonCTSIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTissonCTSIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************************/
////////////////////////////////////////////////////////////////////////////////////
void CTissonCTSIDlg::My_OnFlashDB()
{
	int ret = 0, syn_flag_is_zero=0, i=0;
	if(syn_flag == 0)
	{
		syn_flag = 1;
		syn_flag_is_zero = 1;
	}
	else
	{
		syn_flag_is_zero = 0;
	}
	CDB::waitForidlesse();
	
	if(logon_flag == 0)  //�����ʼ��ʱ�������ݿ�
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ���С�");
		ret = CDB::getTable();
		while( ret && (i++<20))  //ֱ�����³ɹ�Ϊֹ
			ret = CDB::getTable();
		Sleep(500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ�����ݿ�");
	}
	else  //�����������ֶ��������ݿ�
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ���С�");
		ret = CDB::getTable();
		Sleep(500);
		if( ret == 0 )
			GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ�³ɹ���");
		else
			GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ��ʧ�ܣ�");
		Sleep(1500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ�����ݿ�");
	}
	if(syn_flag_is_zero)
	{
		syn_flag = 0;
	}
	else
	{
		NULL;
	}
	
	if(ret)		//ʧ��
		flash_flag = 1;
	else
		flash_flag = 0;
}
void CTissonCTSIDlg::OnFlashDB()	//ˢ�����ݿ�
{
	int ret = 0, syn_flag_is_zero=0, i=0;
	if(syn_flag == 0)
	{
		syn_flag = 1;
		syn_flag_is_zero = 1;
	}
	else
	{
		syn_flag_is_zero = 0;
	}
	CDB::waitForidlesse();
	
	if(logon_flag == 0)  //�����ʼ��ʱ�������ݿ�
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ���С�");
		ret = CDB::getTable();
		while( ret && (i++<20))  //ֱ�����³ɹ�Ϊֹ
			ret = CDB::getTable();
		Sleep(500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ�����ݿ�");
	}
	else  //�����������ֶ��������ݿ�
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ���С�");
		ret = CDB::getTable();
		Sleep(500);
		if( ret == 0 )
			GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ�³ɹ���");
		else
			GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ��ʧ�ܣ�");
		Sleep(1500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("ˢ�����ݿ�");
	}
	if(syn_flag_is_zero)
	{
		syn_flag = 0;
	}
	else
	{
		NULL;
	}

	if(ret)		//ʧ��
		flash_flag = 1;
	else
		flash_flag = 0;
}

void CTissonCTSIDlg::OnOK() 
{
	wrLog(" ");
	wrLog("==================");
	wrLog("��������...");
	/*------------------------------------------------*/
	logon_flag = 0;
	syn_flag = 0;
	memset(logon_random2,0,sizeof(logon_random2));
	//��ȡ�����ļ�
	if( getEposPar() )  
	{
		MessageBox("��ȡ�����ļ�ctsi.iniʧ�ܣ����������ļ��Ƿ��ڳ���Ŀ¼�£�");
		return;
	}

	//=========��ֹ�����������========
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, PRM_TITLE);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox("����������");
		::ReleaseMutex(hMutex);
		::CloseHandle(hMutex);
		exit(0);
	}
	//=================================

	//�������ݿ�����
	int i;
	for(i=0; i<MAX_CONNECT_NUM; i++)
	{
		if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //��������ʧ��
		{
			wrLog("�����ݿ�����%d����ʧ�ܡ�", i);
			g_adoConnInfo[i].connStat = 2;
		}
		else
		{
			g_adoConnInfo[i].connStat = 0;
		}
		bbzero(g_adoConnInfo[i].termId);
	}
	//ȡ���ݿ���Ϣ
	flash_flag = 1;
	OnFlashDB();
	if(flash_flag)
		return;

	/*------------------------------------------------*/

	if (DG_BUS_FLAG == 1)
	{
		/*----------------������ݸ��������----------------*/
		HANDLE hHandle_bus= CreateThread(NULL, 1000,
			(LPTHREAD_START_ROUTINE)OpenBusServer,(LPVOID)(this), 0, NULL);
		CloseHandle(hHandle_bus);
		/*------------------------------------------------*/
//		AfxBeginThread(OpenBusServer, (LPVOID)(this));
	}


	GetDlgItem(IDOK)->EnableWindow(FALSE);

	CConnect con;
	SOCKET sd;
	while( con.srvListen(sd, LISTEN_PORT)  ){
		wrLog("����%d�˿�ʧ������10��", LISTEN_PORT);
		Sleep(1000*10);
	}

	HANDLE hHandle = CreateThread(NULL, 1000, (LPTHREAD_START_ROUTINE)OpenServer,(LPVOID)sd, 0, NULL);
	CloseHandle(hHandle);
//	AfxBeginThread(OpenServer, (LPVOID)sd);

	hHandle= CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)chkAdoConn, NULL, 0, NULL);
	CloseHandle(hHandle);
//	AfxBeginThread(chkAdoConn, NULL);

	hHandle= CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)OnlineThread, NULL, 0, NULL);
	CloseHandle(hHandle);
//	AfxBeginThread(OnlineThread, NULL);

	hHandle= CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)ReFreshData, (LPVOID)(this), 0, NULL);
	CloseHandle(hHandle);
//	AfxBeginThread(ReFreshData, (LPVOID)(this));

	//�����ն˼�������߳�
//  hHandle= CreateThread(NULL, 1000, (LPTHREAD_START_ROUTINE)YCT3Monitor_MainHdl, NULL, 0, NULL);
//  CloseHandle(hHandle);
	//��ȡ����״̬������ʱ�� 
}

UINT OpenBusServer(LPVOID lpv)
{
	CDGBusTrade bus_con;
	while (1)
	{
		bus_con.BusHostSendRecvData();
		Sleep(1000);
	}
	return 0;
}

void CTissonCTSIDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	wrLog("�����˳�...");
	if( MessageBox("ȷ���˳�����", NULL, MB_YESNO) != IDYES)
		return;
	wrLog("�ֶ��˳�����");
	CDialog::OnCancel();
}

UINT OpenServer(LPVOID lpv)
{
	SOCKET sd_cl;
	SOCKET sd=(SOCKET)lpv;	
    SOCKADDR_IN cli_addr;
	Coninfo *pconinfo;
    int nSize=0, conSize=0; 
    HANDLE hHandle;
	conSize = sizeof(cli_addr);
	while(1)
	{
		nSize = conSize;		
		sd_cl = accept(sd, (struct sockaddr *)&cli_addr, &nSize);
		if (sd_cl == INVALID_SOCKET)
			continue;
		
		if( ONLINE_NUM>MAX_ONLINE_NUM )
		{
			closesocket(sd_cl);
		}
		else
		{
			if(sd_cl>=CON_MAX)
			{
				pconinfo = (Coninfo *)malloc(sizeof(Coninfo));		
			}
			else
			{
				pconinfo = &(allConinfo[sd_cl]);
			}
			memset(pconinfo, 0, sizeof(Coninfo));
			pconinfo->sd = sd_cl;
			pconinfo->s_b1 = cli_addr.sin_addr.S_un.S_un_b.s_b1;
			pconinfo->s_b2 = cli_addr.sin_addr.S_un.S_un_b.s_b2;
			pconinfo->s_b3 = cli_addr.sin_addr.S_un.S_un_b.s_b3;
			pconinfo->s_b4 = cli_addr.sin_addr.S_un.S_un_b.s_b4;

			//��ȡһ��Ч����������һ�����߳�
			hHandle= CreateThread(NULL, 1000, (LPTHREAD_START_ROUTINE)SessionThread, (LPVOID)(pconinfo), 0, NULL);
			CloseHandle(hHandle);

	//		AfxBeginThread(SessionThread, (LPVOID)(pconinfo));
		}		
    }
	
	return 0;	
}


UINT SessionThread(LPVOID lpv)
{
	Coninfo coninfo;
	memcpy(&coninfo, (void *)lpv, sizeof(Coninfo));
	if(coninfo.sd>=CON_MAX)
	{
		free((Coninfo*)lpv);
	}
	else
	{
		NULL;
	}

	cs_ONLINE_NUM.Lock();
	if( ONLINE_NUM>MAX_ONLINE_NUM )
	{
		cs_ONLINE_NUM.Unlock();
		return 0;
	}
	else
	{
		ONLINE_NUM++;
		cs_ONLINE_NUM.Unlock();
	}

	CMenuHdl menuHdl;
	try
	{
		menuHdl.Menu_main_hdl( coninfo );
	}
	catch(...)
	{
		wrLog("��====SessionThread====��");
		menuHdl.HangUp( coninfo.sd );
		closesocket(coninfo.sd);
	}
	DWORD threadID = ::GetCurrentThreadId();
	for(int i=0; i<MAX_CONNECT_NUM; i++)
	{
		if( g_adoConnInfo[i].connStat==1 && g_adoConnInfo[i].threadID==threadID )
		{
			wrLog("g_connInfo[%d]: connStat[%d], threadID[%ld], termId[%s], connDate[%s], connTime[%s]", i, g_adoConnInfo[i].connStat, g_adoConnInfo[i].threadID, g_adoConnInfo[i].termId, g_adoConnInfo[i].connDate, g_adoConnInfo[i].connTime);
			g_adoConnInfo[i].connStat = 0;
		}
		else
		{
			NULL;
		}
	}

	cs_ONLINE_NUM.Lock();
	ONLINE_NUM--;
	cs_ONLINE_NUM.Unlock();
	return 0;
}


//״̬�л������л�����ʱʹ��20071111
void CTissonCTSIDlg::OnChangeStat() 
{

	if(TRADE_STAT)
	{
		TRADE_STAT = 0;
		GetDlgItem(IDC_STATIC2)->SetWindowText("����״̬������");
	}
	else
	{
		TRADE_STAT = 1;
		GetDlgItem(IDC_STATIC2)->SetWindowText("����״̬����ͣ");
	}		
}


void CTissonCTSIDlg::My_OnUpdatePar()
{
	int i;
	int maxConNum, traFlag;
	static char my_db_con_str[500]="";
	
	//��ȡ��ǰ������Ϣ
	traFlag = TRADE_FLAG;
	maxConNum = MAX_CONNECT_NUM;	
	//���²���
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("�����С�");
	syn_flag = 1;
	CDB::waitForidlesse();
	
	if(getEposPar())
	{
		MessageBox("��ȡ�����ļ�ctsi.iniʧ�ܣ����������ļ��Ƿ��ڳ���Ŀ¼�£�");
		GetDlgItem(UPDATE_PAR)->SetWindowText("����ʧ�ܣ�");
		Sleep(500);
		GetDlgItem(UPDATE_PAR)->SetWindowText("���²���");
		return;
	}
	//�������ݿ�����
	if(strcmp(my_db_con_str, DB_CON_STR)==0)
	{
		strcpy(my_db_con_str, DB_CON_STR);
		/*----------------�������ݿ�����----------------*/
		for(i=0; i<MAX_CONNECT_NUM; i++)
		{
			g_adoConnInfo[i].adoConn.ExitConnect();  //�ͷ�����
			if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //��������
			{
				wrLog("�����ݿ�����%d����ʧ�ܡ�", i);
				g_adoConnInfo[i].connStat = 2;
			}
			wrLog("��������%d", i);
		}
		OnFlashDB();
	}
	else
	{
		if(maxConNum > MAX_CONNECT_NUM)		//����������
		{
			for(i=maxConNum-1; i!=MAX_CONNECT_NUM-1; i--)
			{
				g_adoConnInfo[i].adoConn.ExitConnect();  //�ͷ�����
				wrLog("�ͷ�����%d", i);
			}
		}
		else if(maxConNum < MAX_CONNECT_NUM)//����������
		{
			for(i=maxConNum; i<MAX_CONNECT_NUM; i++)
			{
				if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //��������
				{
					wrLog("�����ݿ�����%d����ʧ�ܡ�", i);
					g_adoConnInfo[i].connStat = 2;
				}
				wrLog("��������%d", i);
			}
		}
	}
	syn_flag = 0;
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("���³ɹ���");
	Sleep(1000);
	GetDlgItem(UPDATE_PAR)->SetWindowText("���²���");
}
//���²�������20071130
void CTissonCTSIDlg::OnUpdatePar() 
{
	int i;
	int maxConNum, traFlag;
	static char my_db_con_str[500]="";
	
	//��ȡ��ǰ������Ϣ
	traFlag = TRADE_FLAG;
	maxConNum = MAX_CONNECT_NUM;	
	//���²���
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("�����С�");
	syn_flag = 1;
	CDB::waitForidlesse();
	
	if(getEposPar())
	{
		MessageBox("��ȡ�����ļ�ctsi.iniʧ�ܣ����������ļ��Ƿ��ڳ���Ŀ¼�£�");
		GetDlgItem(UPDATE_PAR)->SetWindowText("����ʧ�ܣ�");
		Sleep(500);
		GetDlgItem(UPDATE_PAR)->SetWindowText("���²���");
		return;
	}
	//�������ݿ�����
	if(strcmp(my_db_con_str, DB_CON_STR)==0)
	{
		strcpy(my_db_con_str, DB_CON_STR);
		/*----------------�������ݿ�����----------------*/
		for(i=0; i<MAX_CONNECT_NUM; i++)
		{
			g_adoConnInfo[i].adoConn.ExitConnect();  //�ͷ�����
			if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //��������
			{
				wrLog("�����ݿ�����%d����ʧ�ܡ�", i);
				g_adoConnInfo[i].connStat = 2;
			}
			wrLog("��������%d", i);
		}
		OnFlashDB();
	}
	else
	{
		if(maxConNum > MAX_CONNECT_NUM)		//����������
		{
			for(i=maxConNum-1; i!=MAX_CONNECT_NUM-1; i--)
			{
				g_adoConnInfo[i].adoConn.ExitConnect();  //�ͷ�����
				wrLog("�ͷ�����%d", i);
			}
		}
		else if(maxConNum < MAX_CONNECT_NUM)//����������
		{
			for(i=maxConNum; i<MAX_CONNECT_NUM; i++)
			{
				if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //��������
				{
					wrLog("�����ݿ�����%d����ʧ�ܡ�", i);
					g_adoConnInfo[i].connStat = 2;
				}
				wrLog("��������%d", i);
			}
		}
	}
	syn_flag = 0;
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("���³ɹ���");
	Sleep(1000);
	GetDlgItem(UPDATE_PAR)->SetWindowText("���²���");
	
}


//�������ļ���ȡ����������20071130
int getEposPar() 
{
	wrLog("��getEposPar����ȡ����");
	int iRet=0;
	char numBuf[20+1];
	CString  FilePath = ".\\ctsi.ini";	
	CString strValue = _T("");

	//��������
	CString strSection = "PE";
	CString strSectionKey = "TRADE_FLAG";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	TRADE_FLAG=atoi(numBuf);
	//PE������IP
	strSection = "PE";
	strSectionKey = "PE_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, PE_IP, sizeof(PE_IP), FilePath);
	//PE�������˿�
	strSection = "PE";
	strSectionKey = "PE_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	PE_PORT=atoi(numBuf);
	//��������˿�
	strSection = "PE";
	strSectionKey = "LISTEN_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	LISTEN_PORT=atoi(numBuf);
	//�������
	strSection = "PE";
	strSectionKey = "PRM_TITLE";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, PRM_TITLE, sizeof(PRM_TITLE), FilePath);
	AfxGetMainWnd()->SetWindowText(PRM_TITLE);
	//��������˿�
	strSection = "DGBUS";
	strSectionKey = "DG_BUS_FLAG";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	DG_BUS_FLAG=atoi(numBuf);
	if (DG_BUS_FLAG==1) //����ж�ݸ���� ��ȡ������IP�Ͷ˿�
	{
		strSection = "DGBUS";
		strSectionKey = "DGBUS_IP";	
		GetPrivateProfileString (strSection,strSectionKey, NULL, DGBUS_IP, sizeof(DGBUS_IP), FilePath); 
		//��ݸ�����������˿�
		strSection = "DGBUS";
		strSectionKey = "DGBUS_PORT";	
		GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
		DGBUS_PORT=atoi(numBuf);
	}


	//��ȡ��ݸͨ�˿ں�IP
	strSection = "DGBUS";
	strSectionKey = "DGTBUS_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, DGTBUS_IP, sizeof(DGTBUS_IP), FilePath); 
	//��ݸ�����������˿�
	strSection = "DGBUS";
	strSectionKey = "DGTBUS_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	DGTBUS_PORT=atoi(numBuf);

	//��ݸͨӪ���豸����
	strSection = "DGBUS";
	strSectionKey = "DGT_Unitid";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, DGT_Unitid, sizeof(DGT_Unitid), FilePath); 

	//���������ͨ��ֵ�������汾��
	strSection = "YCT";
	strSectionKey = "YCT_VER";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	YCT_VER=atoi(numBuf);

	//YCT������IP
	strSection = "YCT";
	strSectionKey = "YCT_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, YCT_IP, sizeof(YCT_IP), FilePath); 
	//YCT�������˿�
	strSection = "YCT";
	strSectionKey = "YCT_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	YCT_PORT=atoi(numBuf);

	//���������ͨ��ط�����
	
	/*strSection = "YCT";
	strSectionKey = "YCT_MONITOR_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, YCT_MONITOR_IP, sizeof(YCT_IP), FilePath);
	//���������ͨ��ض˿�
	strSection = "YCT";
	strSectionKey = "YCT_MONITOR_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	YCT_MONITOR_PORT=atoi(numBuf);
	*/
	//�������ݿ�����
	strSection = "DB";
	strSectionKey = "DB_CON_STR_EN";
	GetPrivateProfileString (strSection,strSectionKey, NULL, DB_CON_STR_EN, sizeof(DB_CON_STR_EN), FilePath);
	//�������ݿ������ַ��� (����)
	wrLog("DB_CON_STR_EN:%s",DB_CON_STR_EN);
	convert_plain_DBstr(DB_CON_STR_EN,DB_CON_STR);
	//���ݿ�������
	strSection = "DB";
	strSectionKey = "MAX_CONNECT_NUM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	MAX_CONNECT_NUM=atoi(numBuf);
	//���Լ���
	strSection = "DEBUG";
	strSectionKey = "DEBUG_LEVEL";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	DEBUG_LEVEL=atoi(numBuf);
	//FSK���Լ���
	strSection = "DEBUG";
	strSectionKey = "FSK_DEBUG";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	FSK_DEBUG=atoi(numBuf);
	//�������
	strSection = "USER";
	strSectionKey = "CONNECT_SENTENCE";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, CONNECT_SENTENCE, sizeof(CONNECT_SENTENCE), FilePath);	
	//�ճ�ҵ�����
	strSection = "BIZ";
	strSectionKey = "AIRPAY";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, AIRPAY, sizeof(AIRPAY), FilePath);
	//��������CT10000��ʹ�ó�ֵ���ѿ����ն�,00000000��ʾ��������
	strSection = "PE";
	strSectionKey = "CT10000_TERM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, CT10000_TERM, sizeof(CT10000_TERM), FilePath);
	//�������
	strSection = "PE";
	strSectionKey = "FORBID_BIZ";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, FORBID_BIZ, sizeof(FORBID_BIZ), FilePath);
	//�������ظ���ֵ��ʱ����(����)
	strSection = "PE";
	strSectionKey = "FORBID_TIME";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	FORBID_TIME=atoi(numBuf);

	//���������
	strSection = "PE";
	strSectionKey = "MAX_ONLINE_NUM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	MAX_ONLINE_NUM=atoi(numBuf);
	//�սῪʼʱ��
	strSection = "PE";
	strSectionKey = "service_stop";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	service_stop=atoi(numBuf);
	//�ս����ʱ��
	strSection = "PE";
	strSectionKey = "service_start";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	service_start=atoi(numBuf);

	//��;����Ʊ������IP
	strSection = "COACH_TICKET";
	strSectionKey = "COACH_TICKET_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, COACH_TICKET_IP, sizeof(COACH_TICKET_IP), FilePath);
	//��;����Ʊ�������˿�
	strSection = "COACH_TICKET";
	strSectionKey = "COACH_TICKET_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	COACH_TICKET_PORT=atoi(numBuf);
	//������
	strSection = "PE";
	strSectionKey = "MERID";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, MERID, sizeof(MERID), FilePath);
	//������Ӧ�Ľ����ն�
	strSection = "PE";
	strSectionKey = "TMNNUM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, TMNNUM, sizeof(TMNNUM), FilePath);
	//��������
	strSection = "PE";
	strSectionKey = "CHANNELCODE";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, CHANNELCODE, sizeof(CHANNELCODE), FilePath);

	//��ǰ���ն˰汾��
	strSection = "IPOSUPDATE";
	strSectionKey = "PRMVER";	
	iRet = GetPrivateProfileString (strSection,strSectionKey, NULL, prmVer, sizeof(prmVer), FilePath);
	if(iRet<4)
	{
		strcpy(prmVer, "9001");
	}

	if(TRADE_FLAG)  //����
		CARD_TYPE = 1;
	else			//����
		CARD_TYPE = 0;

	wrLog("TRADE_FLAG = %d", TRADE_FLAG);
	if(TRADE_FLAG==0)
	{
		wrLog("DB_CON_STR = [%s]", DB_CON_STR);
	
	}

	wrLog("MAX_CONNECT_NUM = %d", MAX_CONNECT_NUM);
	wrLog("DEBUG_LEVEL = %d", DEBUG_LEVEL);
	wrLog("FSK_DEBUG = %d", FSK_DEBUG);
	wrLog("CONNECT_SENTENCE = [%s]", CONNECT_SENTENCE);
	wrLog("PE_IP&PORT = [%s][%d]", PE_IP, PE_PORT);
	wrLog("YCT_IP&PORT = [%s][%d]", YCT_IP, YCT_PORT);
	wrLog("YCT_VER:%d",YCT_VER);
	//wrLog("YCT_MONITOR_IP&YCT_MONITOR_PORT = [%s][%d]", YCT_MONITOR_IP, YCT_MONITOR_PORT);
	wrLog("COACH_TICKET_IP&PORT = [%s][%d]", COACH_TICKET_IP, COACH_TICKET_PORT);
	wrLog("AIRPAY = [%s]", AIRPAY);
	wrLog("����:%s",MERID);
	wrLog("���������ն�:%s",TMNNUM);
	wrLog("�������룺%s",CHANNELCODE);
	wrLog("��ݸ����������IP:%s",DGBUS_IP);
	wrLog("��ݸ�����˿�:%d",DGBUS_PORT);

	memset(&ftpFiles, 0, sizeof(ftpFiles));
	ftpFiles.updateNum = 1;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","IP", NULL, ftpFiles.ftpFiles[0].ip, sizeof(ftpFiles.ftpFiles[0].ip), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","PORT", NULL, ftpFiles.ftpFiles[0].port, sizeof(ftpFiles.ftpFiles[0].port), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","USR", NULL, ftpFiles.ftpFiles[0].usr, sizeof(ftpFiles.ftpFiles[0].usr), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;	
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","PWD", NULL, ftpFiles.ftpFiles[0].pwd, sizeof(ftpFiles.ftpFiles[0].pwd), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","SRCFILE", NULL, ftpFiles.ftpFiles[0].srcFile, sizeof(ftpFiles.ftpFiles[0].srcFile), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","DESTFILE", NULL, ftpFiles.ftpFiles[0].destFile, sizeof(ftpFiles.ftpFiles[0].destFile), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","IP_BAK", NULL, ftpFiles.ftpFilesBak[0].ip, sizeof(ftpFiles.ftpFilesBak[0].ip), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","PORT_BAK", NULL, ftpFiles.ftpFilesBak[0].port, sizeof(ftpFiles.ftpFilesBak[0].port), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","USR_BAK", NULL, ftpFiles.ftpFilesBak[0].usr, sizeof(ftpFiles.ftpFilesBak[0].usr), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;	
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","PWD_BAK", NULL, ftpFiles.ftpFilesBak[0].pwd, sizeof(ftpFiles.ftpFilesBak[0].pwd), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","SRCFILE_BAK", NULL, ftpFiles.ftpFilesBak[0].srcFile, sizeof(ftpFiles.ftpFilesBak[0].srcFile), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","DESTFILE_BAK", NULL, ftpFiles.ftpFilesBak[0].destFile, sizeof(ftpFiles.ftpFilesBak[0].destFile), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	ftpFiles.excFilesNum = 1;
	
	iRet = GetPrivateProfileString ("IPOSUPDATE","EXECFILE", NULL, ftpFiles.excFilesName[0], sizeof(ftpFiles.excFilesName[0]), ".\\ctsi.ini");
	if(iRet<=0)
		ftpFiles.updateNum = 0;
	
	ftpFiles.shutDownFlag = 1;

	iRet = GetPrivateProfileString ("PE","PEPWD_IP", NULL, pepwd_ip, sizeof(pepwd_ip), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;
	iRet = GetPrivateProfileString ("PE","ECARD_IP", NULL, ecard_ip, sizeof(ecard_ip), ".\\ctsi.ini");
    if(iRet<=0)
		return -1;

	iRet = GetPrivateProfileString ("PE","PEPWD_PORT", NULL, pepwd_port, sizeof(pepwd_port), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;
		
	iRet = GetPrivateProfileString ("PE","ECARD_SER_PORT", NULL, ecard_ser_port, sizeof(ecard_ser_port), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;

	iRet = GetPrivateProfileString ("PE","PRMMD5", NULL, prmMD5, sizeof(prmMD5), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;
	
	iRet = GetPrivateProfileString ("PE","EXCMAC", NULL, excMAC, sizeof(excMAC), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;

	iRet = GetPrivateProfileString ("REFRESH","REFRESH_PORT", NULL, numBuf, sizeof(numBuf), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;
	REFRESH_PORT = atoi(numBuf);

	iRet = GetPrivateProfileString ("REFRESH","REFRESH_KEY", NULL, REFRESH_KEY, sizeof(REFRESH_KEY), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;

	iRet = GetPrivateProfileString ("REFRESH","REFRESH_IP", NULL, REFRESH_IP, sizeof(REFRESH_IP), ".\\ctsi.ini");
	if(iRet<=0)
		return -1;
	
	return 0;
}

//����ͳ��
UINT OnlineThread(LPVOID lpv)
{
	while(1)
	{
		wrOnlineLog(ONLINE_NUM);
		Sleep(60000);
	}
	return 0;
}


//���ݿ����Ӿ���Ա
UINT chkAdoConn(LPVOID lpv) 
{
	CTime cCurTime;
	CTime cTxnTime;
	int nYear=0, nMonth=0, nDay=0, nHour=0, nMin=0, nSec=0;
	char cDateBuf[4+1];
	while(1)
	{
		for(int i=0; i<MAX_CONNECT_NUM; i++)
		{
			wrLog("g_connInfo[%d]: connStat[%d], threadID[%ld], termId[%s], connDate[%s], connTime[%s]", i, g_adoConnInfo[i].connStat, g_adoConnInfo[i].threadID, g_adoConnInfo[i].termId, g_adoConnInfo[i].connDate, g_adoConnInfo[i].connTime);
			if( g_adoConnInfo[i].connStat==1 )
			{
				bbzero(cDateBuf);
				strncpy(cDateBuf, g_adoConnInfo[i].connDate, 4);
				nYear = atoi(cDateBuf);
				bbzero(cDateBuf);
				strncpy(cDateBuf, g_adoConnInfo[i].connDate+4, 2);
				nMonth = atoi(cDateBuf);
				bbzero(cDateBuf);
				strncpy(cDateBuf, g_adoConnInfo[i].connDate+6, 2);
				nDay = atoi(cDateBuf);
				bbzero(cDateBuf);
				strncpy(cDateBuf, g_adoConnInfo[i].connTime, 2);
				nHour = atoi(cDateBuf);
				bbzero(cDateBuf);
				strncpy(cDateBuf, g_adoConnInfo[i].connTime+2, 2);
				nMin = atoi(cDateBuf);
				bbzero(cDateBuf);
				strncpy(cDateBuf, g_adoConnInfo[i].connTime+4, 2);
				nSec = atoi(cDateBuf);
				if(CTools::chkDate(nYear, nMonth, nDay)==0)
				{
					cCurTime = CTime::GetCurrentTime();
					cTxnTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
					if( cTxnTime <  cCurTime - CTimeSpan(0,1,0,0) ) //�����1��Сʱǰ������
					{
						wrLog("chkAdoConn time out");
						g_adoConnInfo[i].adoConn.ExitConnect();
						if( !g_adoConnInfo[i].adoConn.OnInitADOConn() )
						{
							g_adoConnInfo[i].connStat = 0;
						}
						else
						{
							g_adoConnInfo[i].connStat = 2;
						}
					}
					else
					{
						NULL;
					}
				}
				else
				{
					wrLog("chkAdoConn date err");
				}
			}
			else
			{
				NULL;
			}
		}
		Sleep(1000*60*60);//һ��Сʱ��һ��
	}
	return 0;
}

//���Ͷ���
void CTissonCTSIDlg::sendMsg() 
{
	
}

//����֪ͨ
void CTissonCTSIDlg::sendInfo() 
{
	
}

//����δ֧���ʵ�
void CTissonCTSIDlg::sendBill() 
{

}

void CTissonCTSIDlg::sendResult() 
{

}

//����ˢ��ר���߳�
UINT ReFreshData(LPVOID lpv)
{
	CTissonCTSIDlg* dlg=(CTissonCTSIDlg*)lpv;
	int iRet;
	CConnect con;
	SOCKET sd;
	SOCKET sd_cl;
	SOCKADDR_IN cli_addr;
	int nSize=0, conSize=0; 
	char recv_Data[1024],in_ip[16];
	char fresh_date[8+1],fresh_time[6+1],fresh_action[2+1],buff[1024],len[3+1],recv_buf[1024],key_buff[32+1];
	while( con.srvListen(sd, REFRESH_PORT)  ){
		wrLog("��ˢ�¡�����%d�˿�ʧ������10��", REFRESH_PORT);
		Sleep(1000*10);
	}
	wrLog("��ˢ�¡�����ˢ�½ӿڣ������˿�%d", REFRESH_PORT);
	conSize = sizeof(cli_addr);
	while(true)
	{
		nSize = conSize;
		sd_cl = accept(sd, (struct sockaddr *)&cli_addr, &nSize);
		if (sd_cl == INVALID_SOCKET)
			continue;		
		
		bbzero(recv_Data);
		bbzero(in_ip);
		sprintf(in_ip,"%d.%d.%d.%d",cli_addr.sin_addr.S_un.S_un_b.s_b1,cli_addr.sin_addr.S_un.S_un_b.s_b2,cli_addr.sin_addr.S_un.S_un_b.s_b3,cli_addr.sin_addr.S_un.S_un_b.s_b4);
		if (strstr(REFRESH_IP,in_ip)==NULL)
		{
			closesocket(sd_cl);
			continue;
		}else{
			iRet = recv( sd_cl, recv_Data, 1024, 0 );
			if( iRet==0 )
			{
				closesocket(sd_cl);
				continue;
			}else{
				bbzero(buff);
				bbzero(recv_buf);
				bbzero(len);
				bbzero(key_buff);
				memcpy(len,recv_Data,3);
				memcpy(recv_buf,recv_Data+3,atoi(len));
//				CTools::byte_to_hex((BYTE*)buff,(BYTE*)recv_buf,atoi(len));
				CTools::byte_to_hex((BYTE*)REFRESH_KEY,(BYTE*)key_buff,16);
				bbzero(buff);
				iRet = CDes::TripleDesDec(key_buff,recv_buf,buff);
				if (iRet == 0)
				{
					bbzero(fresh_date);
					bbzero(fresh_time);
					bbzero(fresh_action);
					memcpy(fresh_date,buff,8);
					memcpy(fresh_time,buff+8,6);
					memcpy(fresh_action,buff+14,2);

					if (memcmp(fresh_action,"01",2)==0)
					{
						wrLog("��ˢ�����ݿ�-��ʼ��IP:%s ʱ�䣺%s %s",in_ip,fresh_date,fresh_time);
						dlg->My_OnFlashDB();
						wrLog("��ˢ�����ݿ�-������IP:%s ʱ�䣺%s %s",in_ip,fresh_date,fresh_time);
						send(sd_cl, "00", 2, 0);
						closesocket(sd_cl);
					}else if (memcmp(fresh_action,"02",2)==0)
					{
						wrLog("��ˢ�������ļ�-��ʼ��IP:%s ʱ�䣺%s %s",in_ip,fresh_date,fresh_time);
						dlg->My_OnUpdatePar();
						wrLog("��ˢ�������ļ�-������IP:%s ʱ�䣺%s %s",in_ip,fresh_date,fresh_time);
						send(sd_cl, "00", 2, 0);
						closesocket(sd_cl);
					}else{ 
						send(sd_cl, "01", 2, 0);
						closesocket(sd_cl);
						continue;
					}
				}else{
					send(sd_cl, "02", 2, 0);
					closesocket(sd_cl);
					continue;
				}
				
			}
		}
		
	}

	//closesocket(sd);
	return 0;
}

/*
DWORD WINAPI YCT3Monitor_MainHdl(LPVOID lpv)
{
	int iRet = 0;
	int main_group = 1;			//���˵�������
	CGlobal gbl;   
	while(1)
	{	

// 		bbzero(main_ver);
// 		memset(&gbl.posInfo, 0, sizeof(gbl.posInfo));
// 		memset(&gbl.dPosInfo, 0, sizeof(gbl.dPosInfo));
// 		
// 		iRet = TS_GetPosInfo(sd, &gbl.posInfo, &gbl.dPosInfo, gbl.askType, menu_id, CARD_TYPE);	//��ȡ�ն���Ϣ
// 		
		if (strcmp(log.g_pos_no, gbl.dPosInfo.pos_no) == 0)
		{
			Sleep(5000);//˯��5��
			continue;
		}
		else
		{
			//��һ�λ�ȡ���ն������������ն˼���߳�
			HANDLE hHandle= CreateThread(NULL, 1000, (LPTHREAD_START_ROUTINE)YCT3TermMonitor, (LPVOID)(&gbl), 0, NULL);
			CloseHandle(hHandle);
		}
	}
	
	return 0;
}

DWORD WINAPI YCT3TermMonitor(LPVOID lpv)
{
	CGlobal gbl;
	YCT_DATA yct_data;
	memcpy(&gbl, (void *)lpv, sizeof(CGlobal));

	strcpy( log.g_pos_no, gbl.dPosInfo.pos_no);	//�����̵߳��ն˺ű���(���̶߳�����������־ʶ��)
	//log.log_sd = sd;
	
	wrLog("�ն˼�ؿ�������");
	wrYct3MonitorLog(gbl.dPosInfo.pos_no,"===========================");
	wrYct3MonitorLog(gbl.dPosInfo.pos_no,"�����ն˼��");

	//��ʼ�����ںţ���������,ǩ����ȡYCT_DATA�ṹ
	if (CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data) != 0)
	{
		return 1;
	}

	
	CYCTYPTrade::P_SET_MONITOR(&gbl,gbl.dPosInfo, gbl.com, gbl.comType,&yct_data);
	
	int iMonFrq = atoi(gbl.monitor_frequency);//���Ƶ��
	
	while(1)
	{
		//CYCTYPTrade::termMonitor(&gbl);
		Sleep(iMonFrq * 60 * 1000);
	}
	
	wrYct3MonitorLog(gbl.dPosInfo.pos_no,"�ر��ն˼��");
}
*/


//�������ݿ���ַ���
int  getDB_Prority(char *str,char *delimiter,PDB_PRORITY db_prority) //�ָ��ַ�����������Ҫ���ַ�����Ӧ��ֵ
{
	
	char *p;
	
	char *strContent=str;
	p=strtok(strContent,delimiter);
	
	int i=0;
	while(p)
	{
		
		if ((memcmp(p,"Provider",strlen("Provider")))==0)
		{
			
			strcpy(db_prority->Provider,p);
			
		}
		else if ((memcmp(p,"Data",strlen("Data")))==0)
		{
			strcpy(db_prority->Data,p);
			
			
		}
		else if ((memcmp(p,"Password",strlen("Password")))==0)
		{
			
			strcpy(db_prority->Password,p);
			
		}
		
		else if ((memcmp(p,"User",strlen("User")))==0)
		{
			
			strcpy(db_prority->User,p);
			
		}
		
		else if ((memcmp(p,"Persist",strlen("Persist")))==0)
		{
			strcpy(db_prority->Persist,p);
			
		}
		
		
		p=strtok(NULL,delimiter);
	}
	
	return 0;
	
}


int convert_plain_DBstr(char *str,char *plainStr)
{

	int iRet=0;
	DB_PRORITY db_prority; //���ݽṹ��

	string conString=string(str);
	char un_pass[200];
	char un_userId[200];
	char userString [100];
	char passString [100];
	char user_id_str[100];
	char pass_str[100];
	string un_userId_str;
	string un_pass_str;

	bbzero(passString);
	bbzero(userString);
	bbzero(un_pass);
	bbzero(un_userId);
	bbzero(user_id_str);
	bbzero(pass_str);
	bbzero(passString);
	
	iRet=getDB_Prority((char *)conString.c_str(),";",&db_prority);
	spit(db_prority.User,"=","User",userString);
	spit(db_prority.Password,"=","Password",passString);
	
	bbzero(db_prority.User);
	bbzero(db_prority.Password);
	strcpy(db_prority.User,userString);
	strcpy(db_prority.Password,passString);

	//���н���
	memcpy(user_id_str,db_prority.User,strlen(db_prority.User));
	memcpy(pass_str,db_prority.Password,strlen(db_prority.Password));

	un_userId_str=decrypt(user_id_str,DB_KEY);
	un_pass_str=decrypt(pass_str,DB_KEY);

	bbzero(db_prority.User);
	bbzero(db_prority.Password);
	sprintf(db_prority.User,"User ID=%s",un_userId_str.c_str());
	bbzero(db_prority.Password);
	sprintf(db_prority.Password,"Password=%s",un_pass_str.c_str());
	sprintf(plainStr,"%s;%s;%s;%s;%s",db_prority.Provider,db_prority.Data,db_prority.Password,db_prority.User,db_prority.Persist);
	
	return 0;
}


//�������ݿ���ַ���
int spit(char*str,char *delimiter,char * keyName,char * keyValue) //�ָ��ַ�����������Ҫ���ַ�����Ӧ��ֵ
{
	
	char *p;
	
	p=strtok(str,delimiter);
	
	int i=0;
	while(p)
	{
		
		if ((memcmp(p,keyName,strlen(keyName)))!=0)
		{
			
			strcpy(keyValue,p);
			
		}
	
		
		p=strtok(NULL,delimiter);
	}
	
	return 0;
	
}

