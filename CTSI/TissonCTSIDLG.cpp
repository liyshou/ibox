// TissonCTSIDlg.cpp : implementation file

/*****************2006-10-30*******************/
//生产与测试切换：
//数据库中设置
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


/****************银行卡表*****************/
//e卡第1把key
char *CARD_TABLE_ECARD[ECARD_TABLE_MAX]
={
	"1088"//e卡
  };

//招行：第4把key
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
//建行: 第7把key
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
//农行: 第3把key
char *CARD_TABLE_ABC[ABC_TABLE_MAX]   
={
    "63",  
    "434401",
    "95599",
    "53591",
    "49102",
    "622848"
 };

//交行: 第17把key
char *CARD_TABLE_CIT[CIT_TABLE_MAX]	
={
     "40551",
     "60142",
	 "53783",
	 "49104",
	 "66537",
	 "62225"
  };

//广州商业银行: 第6把key
char *CARD_TABLE_GCB[GCB_TABLE_MAX]	
={
     "622467",
     "940016",
	 "603445",
  };

//工行：第2把key
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


//邮储: 第5把key
char *CARD_TABLE_PSD[PSD_TABLE_MAX] 
={
    "60",
    "12345",
    "9551"
  };

//中行: 第11把key
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

//广发:第9把key
char *CARD_TABLE_GDB[GDB_TABLE_MAX]	
={
     "68",
     "9111"
  }; 

//农信:第13把key
char *CARD_TABLE_NXB[NXB_TABLE_MAX]	
={
     "01909",
	  "94",
	  "909810",
	  "940035"
  };

//民生: 第19把key
char *CARD_TABLE_MSB[MSB_TABLE_MAX] 
={
     "4155",     
  };

//中信: 第15把key
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
	
	if(logon_flag == 0)  //程序初始化时更新数据库
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新中…");
		ret = CDB::getTable();
		while( ret && (i++<20))  //直到更新成功为止
			ret = CDB::getTable();
		Sleep(500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新数据库");
	}
	else  //程序运行中手动更新数据库
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新中…");
		ret = CDB::getTable();
		Sleep(500);
		if( ret == 0 )
			GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新成功！");
		else
			GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新失败！");
		Sleep(1500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新数据库");
	}
	if(syn_flag_is_zero)
	{
		syn_flag = 0;
	}
	else
	{
		NULL;
	}
	
	if(ret)		//失败
		flash_flag = 1;
	else
		flash_flag = 0;
}
void CTissonCTSIDlg::OnFlashDB()	//刷新数据库
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
	
	if(logon_flag == 0)  //程序初始化时更新数据库
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新中…");
		ret = CDB::getTable();
		while( ret && (i++<20))  //直到更新成功为止
			ret = CDB::getTable();
		Sleep(500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新数据库");
	}
	else  //程序运行中手动更新数据库
	{
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新中…");
		ret = CDB::getTable();
		Sleep(500);
		if( ret == 0 )
			GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新成功！");
		else
			GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新失败！");
		Sleep(1500);
		GetDlgItem(FRESH_BUTTON)->SetWindowText("刷新数据库");
	}
	if(syn_flag_is_zero)
	{
		syn_flag = 0;
	}
	else
	{
		NULL;
	}

	if(ret)		//失败
		flash_flag = 1;
	else
		flash_flag = 0;
}

void CTissonCTSIDlg::OnOK() 
{
	wrLog(" ");
	wrLog("==================");
	wrLog("启动服务...");
	/*------------------------------------------------*/
	logon_flag = 0;
	syn_flag = 0;
	memset(logon_random2,0,sizeof(logon_random2));
	//读取配置文件
	if( getEposPar() )  
	{
		MessageBox("读取配置文件ctsi.ini失败，请检查配置文件是否在程序目录下！");
		return;
	}

	//=========防止多次启动代码========
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, PRM_TITLE);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox("程序已启动");
		::ReleaseMutex(hMutex);
		::CloseHandle(hMutex);
		exit(0);
	}
	//=================================

	//创建数据库连接
	int i;
	for(i=0; i<MAX_CONNECT_NUM; i++)
	{
		if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //建立连接失败
		{
			wrLog("【数据库连接%d建立失败】", i);
			g_adoConnInfo[i].connStat = 2;
		}
		else
		{
			g_adoConnInfo[i].connStat = 0;
		}
		bbzero(g_adoConnInfo[i].termId);
	}
	//取数据库信息
	flash_flag = 1;
	OnFlashDB();
	if(flash_flag)
		return;

	/*------------------------------------------------*/

	if (DG_BUS_FLAG == 1)
	{
		/*----------------创建东莞公交连接----------------*/
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
		wrLog("监听%d端口失败休眠10秒", LISTEN_PORT);
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

	//开启终端监控主控线程
//  hHandle= CreateThread(NULL, 1000, (LPTHREAD_START_ROUTINE)YCT3Monitor_MainHdl, NULL, 0, NULL);
//  CloseHandle(hHandle);
	//获取服务状态及工作时间 
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
//	wrLog("程序退出...");
	if( MessageBox("确认退出程序？", NULL, MB_YESNO) != IDYES)
		return;
	wrLog("手动退出程序");
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

			//获取一有效连接则启动一服务线程
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
		wrLog("【====SessionThread====】");
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


//状态切换，供切换程序时使用20071111
void CTissonCTSIDlg::OnChangeStat() 
{

	if(TRADE_STAT)
	{
		TRADE_STAT = 0;
		GetDlgItem(IDC_STATIC2)->SetWindowText("交易状态：正常");
	}
	else
	{
		TRADE_STAT = 1;
		GetDlgItem(IDC_STATIC2)->SetWindowText("交易状态：暂停");
	}		
}


void CTissonCTSIDlg::My_OnUpdatePar()
{
	int i;
	int maxConNum, traFlag;
	static char my_db_con_str[500]="";
	
	//获取当前参数信息
	traFlag = TRADE_FLAG;
	maxConNum = MAX_CONNECT_NUM;	
	//更新参数
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("更新中…");
	syn_flag = 1;
	CDB::waitForidlesse();
	
	if(getEposPar())
	{
		MessageBox("读取配置文件ctsi.ini失败，请检查配置文件是否在程序目录下！");
		GetDlgItem(UPDATE_PAR)->SetWindowText("更新失败！");
		Sleep(500);
		GetDlgItem(UPDATE_PAR)->SetWindowText("更新参数");
		return;
	}
	//更新数据库连接
	if(strcmp(my_db_con_str, DB_CON_STR)==0)
	{
		strcpy(my_db_con_str, DB_CON_STR);
		/*----------------创建数据库连接----------------*/
		for(i=0; i<MAX_CONNECT_NUM; i++)
		{
			g_adoConnInfo[i].adoConn.ExitConnect();  //释放连接
			if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //建立连接
			{
				wrLog("【数据库连接%d建立失败】", i);
				g_adoConnInfo[i].connStat = 2;
			}
			wrLog("创建连接%d", i);
		}
		OnFlashDB();
	}
	else
	{
		if(maxConNum > MAX_CONNECT_NUM)		//减少连接数
		{
			for(i=maxConNum-1; i!=MAX_CONNECT_NUM-1; i--)
			{
				g_adoConnInfo[i].adoConn.ExitConnect();  //释放连接
				wrLog("释放连接%d", i);
			}
		}
		else if(maxConNum < MAX_CONNECT_NUM)//增加连接数
		{
			for(i=maxConNum; i<MAX_CONNECT_NUM; i++)
			{
				if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //建立连接
				{
					wrLog("【数据库连接%d建立失败】", i);
					g_adoConnInfo[i].connStat = 2;
				}
				wrLog("创建连接%d", i);
			}
		}
	}
	syn_flag = 0;
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("更新成功！");
	Sleep(1000);
	GetDlgItem(UPDATE_PAR)->SetWindowText("更新参数");
}
//更新参数配置20071130
void CTissonCTSIDlg::OnUpdatePar() 
{
	int i;
	int maxConNum, traFlag;
	static char my_db_con_str[500]="";
	
	//获取当前参数信息
	traFlag = TRADE_FLAG;
	maxConNum = MAX_CONNECT_NUM;	
	//更新参数
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("更新中…");
	syn_flag = 1;
	CDB::waitForidlesse();
	
	if(getEposPar())
	{
		MessageBox("读取配置文件ctsi.ini失败，请检查配置文件是否在程序目录下！");
		GetDlgItem(UPDATE_PAR)->SetWindowText("更新失败！");
		Sleep(500);
		GetDlgItem(UPDATE_PAR)->SetWindowText("更新参数");
		return;
	}
	//更新数据库连接
	if(strcmp(my_db_con_str, DB_CON_STR)==0)
	{
		strcpy(my_db_con_str, DB_CON_STR);
		/*----------------创建数据库连接----------------*/
		for(i=0; i<MAX_CONNECT_NUM; i++)
		{
			g_adoConnInfo[i].adoConn.ExitConnect();  //释放连接
			if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //建立连接
			{
				wrLog("【数据库连接%d建立失败】", i);
				g_adoConnInfo[i].connStat = 2;
			}
			wrLog("创建连接%d", i);
		}
		OnFlashDB();
	}
	else
	{
		if(maxConNum > MAX_CONNECT_NUM)		//减少连接数
		{
			for(i=maxConNum-1; i!=MAX_CONNECT_NUM-1; i--)
			{
				g_adoConnInfo[i].adoConn.ExitConnect();  //释放连接
				wrLog("释放连接%d", i);
			}
		}
		else if(maxConNum < MAX_CONNECT_NUM)//增加连接数
		{
			for(i=maxConNum; i<MAX_CONNECT_NUM; i++)
			{
				if( g_adoConnInfo[i].adoConn.OnInitADOConn() )  //建立连接
				{
					wrLog("【数据库连接%d建立失败】", i);
					g_adoConnInfo[i].connStat = 2;
				}
				wrLog("创建连接%d", i);
			}
		}
	}
	syn_flag = 0;
	OnChangeStat();
	GetDlgItem(UPDATE_PAR)->SetWindowText("更新成功！");
	Sleep(1000);
	GetDlgItem(UPDATE_PAR)->SetWindowText("更新参数");
	
}


//从配置文件中取出参数配置20071130
int getEposPar() 
{
	wrLog("【getEposPar】获取参数");
	int iRet=0;
	char numBuf[20+1];
	CString  FilePath = ".\\ctsi.ini";	
	CString strValue = _T("");

	//交易类型
	CString strSection = "PE";
	CString strSectionKey = "TRADE_FLAG";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	TRADE_FLAG=atoi(numBuf);
	//PE服务器IP
	strSection = "PE";
	strSectionKey = "PE_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, PE_IP, sizeof(PE_IP), FilePath);
	//PE服务器端口
	strSection = "PE";
	strSectionKey = "PE_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	PE_PORT=atoi(numBuf);
	//程序监听端口
	strSection = "PE";
	strSectionKey = "LISTEN_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	LISTEN_PORT=atoi(numBuf);
	//程序标题
	strSection = "PE";
	strSectionKey = "PRM_TITLE";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, PRM_TITLE, sizeof(PRM_TITLE), FilePath);
	AfxGetMainWnd()->SetWindowText(PRM_TITLE);
	//程序监听端口
	strSection = "DGBUS";
	strSectionKey = "DG_BUS_FLAG";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	DG_BUS_FLAG=atoi(numBuf);
	if (DG_BUS_FLAG==1) //如果有东莞公交 读取服务器IP和端口
	{
		strSection = "DGBUS";
		strSectionKey = "DGBUS_IP";	
		GetPrivateProfileString (strSection,strSectionKey, NULL, DGBUS_IP, sizeof(DGBUS_IP), FilePath); 
		//东莞公交服务器端口
		strSection = "DGBUS";
		strSectionKey = "DGBUS_PORT";	
		GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
		DGBUS_PORT=atoi(numBuf);
	}


	//获取东莞通端口和IP
	strSection = "DGBUS";
	strSectionKey = "DGTBUS_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, DGTBUS_IP, sizeof(DGTBUS_IP), FilePath); 
	//东莞公交服务器端口
	strSection = "DGBUS";
	strSectionKey = "DGTBUS_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	DGTBUS_PORT=atoi(numBuf);

	//东莞通营运设备代码
	strSection = "DGBUS";
	strSectionKey = "DGT_Unitid";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, DGT_Unitid, sizeof(DGT_Unitid), FilePath); 

	//第三代羊城通充值服务器版本号
	strSection = "YCT";
	strSectionKey = "YCT_VER";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	YCT_VER=atoi(numBuf);

	//YCT服务器IP
	strSection = "YCT";
	strSectionKey = "YCT_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, YCT_IP, sizeof(YCT_IP), FilePath); 
	//YCT服务器端口
	strSection = "YCT";
	strSectionKey = "YCT_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	YCT_PORT=atoi(numBuf);

	//第三代羊城通监控服务器
	
	/*strSection = "YCT";
	strSectionKey = "YCT_MONITOR_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, YCT_MONITOR_IP, sizeof(YCT_IP), FilePath);
	//第三代羊城通监控端口
	strSection = "YCT";
	strSectionKey = "YCT_MONITOR_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	YCT_MONITOR_PORT=atoi(numBuf);
	*/
	//连接数据库类型
	strSection = "DB";
	strSectionKey = "DB_CON_STR_EN";
	GetPrivateProfileString (strSection,strSectionKey, NULL, DB_CON_STR_EN, sizeof(DB_CON_STR_EN), FilePath);
	//解析数据库连接字符串 (明文)
	wrLog("DB_CON_STR_EN:%s",DB_CON_STR_EN);
	convert_plain_DBstr(DB_CON_STR_EN,DB_CON_STR);
	//数据库连接数
	strSection = "DB";
	strSectionKey = "MAX_CONNECT_NUM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	MAX_CONNECT_NUM=atoi(numBuf);
	//调试级别
	strSection = "DEBUG";
	strSectionKey = "DEBUG_LEVEL";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	DEBUG_LEVEL=atoi(numBuf);
	//FSK调试级别
	strSection = "DEBUG";
	strSectionKey = "FSK_DEBUG";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	FSK_DEBUG=atoi(numBuf);
	//连接语句
	strSection = "USER";
	strSectionKey = "CONNECT_SENTENCE";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, CONNECT_SENTENCE, sizeof(CONNECT_SENTENCE), FilePath);	
	//空充业务代码
	strSection = "BIZ";
	strSectionKey = "AIRPAY";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, AIRPAY, sizeof(AIRPAY), FilePath);
	//允许开放用CT10000来使用充值付费卡的终端,00000000表示开放所有
	strSection = "PE";
	strSectionKey = "CT10000_TERM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, CT10000_TERM, sizeof(CT10000_TERM), FilePath);
	//程序标题
	strSection = "PE";
	strSectionKey = "FORBID_BIZ";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, FORBID_BIZ, sizeof(FORBID_BIZ), FilePath);
	//不允许重复充值的时间间隔(分钟)
	strSection = "PE";
	strSectionKey = "FORBID_TIME";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	FORBID_TIME=atoi(numBuf);

	//最大连接数
	strSection = "PE";
	strSectionKey = "MAX_ONLINE_NUM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	MAX_ONLINE_NUM=atoi(numBuf);
	//日结开始时间
	strSection = "PE";
	strSectionKey = "service_stop";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	service_stop=atoi(numBuf);
	//日结结束时间
	strSection = "PE";
	strSectionKey = "service_start";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath);
	service_start=atoi(numBuf);

	//长途汽车票服务器IP
	strSection = "COACH_TICKET";
	strSectionKey = "COACH_TICKET_IP";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, COACH_TICKET_IP, sizeof(COACH_TICKET_IP), FilePath);
	//长途汽车票服务器端口
	strSection = "COACH_TICKET";
	strSectionKey = "COACH_TICKET_PORT";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, numBuf, sizeof(numBuf), FilePath); 
	COACH_TICKET_PORT=atoi(numBuf);
	//机构号
	strSection = "PE";
	strSectionKey = "MERID";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, MERID, sizeof(MERID), FilePath);
	//机构对应的接入终端
	strSection = "PE";
	strSectionKey = "TMNNUM";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, TMNNUM, sizeof(TMNNUM), FilePath);
	//机构代码
	strSection = "PE";
	strSectionKey = "CHANNELCODE";	
	GetPrivateProfileString (strSection,strSectionKey, NULL, CHANNELCODE, sizeof(CHANNELCODE), FilePath);

	//当前软终端版本号
	strSection = "IPOSUPDATE";
	strSectionKey = "PRMVER";	
	iRet = GetPrivateProfileString (strSection,strSectionKey, NULL, prmVer, sizeof(prmVer), FilePath);
	if(iRet<4)
	{
		strcpy(prmVer, "9001");
	}

	if(TRADE_FLAG)  //生产
		CARD_TYPE = 1;
	else			//测试
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
	wrLog("机构:%s",MERID);
	wrLog("机构接入终端:%s",TMNNUM);
	wrLog("机构代码：%s",CHANNELCODE);
	wrLog("东莞公交服务器IP:%s",DGBUS_IP);
	wrLog("东莞公交端口:%d",DGBUS_PORT);

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

//在线统计
UINT OnlineThread(LPVOID lpv)
{
	while(1)
	{
		wrOnlineLog(ONLINE_NUM);
		Sleep(60000);
	}
	return 0;
}


//数据库连接纠察员
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
					if( cTxnTime <  cCurTime - CTimeSpan(0,1,0,0) ) //如果是1个小时前的连接
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
		Sleep(1000*60*60);//一个小时查一次
	}
	return 0;
}

//发送短信
void CTissonCTSIDlg::sendMsg() 
{
	
}

//发送通知
void CTissonCTSIDlg::sendInfo() 
{
	
}

//发送未支付帐单
void CTissonCTSIDlg::sendBill() 
{

}

void CTissonCTSIDlg::sendResult() 
{

}

//接受刷新专用线程
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
		wrLog("【刷新】监听%d端口失败休眠10秒", REFRESH_PORT);
		Sleep(1000*10);
	}
	wrLog("【刷新】启动刷新接口，监听端口%d", REFRESH_PORT);
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
						wrLog("【刷新数据库-开始】IP:%s 时间：%s %s",in_ip,fresh_date,fresh_time);
						dlg->My_OnFlashDB();
						wrLog("【刷新数据库-结束】IP:%s 时间：%s %s",in_ip,fresh_date,fresh_time);
						send(sd_cl, "00", 2, 0);
						closesocket(sd_cl);
					}else if (memcmp(fresh_action,"02",2)==0)
					{
						wrLog("【刷新配置文件-开始】IP:%s 时间：%s %s",in_ip,fresh_date,fresh_time);
						dlg->My_OnUpdatePar();
						wrLog("【刷新配置文件-结束】IP:%s 时间：%s %s",in_ip,fresh_date,fresh_time);
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
	int main_group = 1;			//主菜单的组别号
	CGlobal gbl;   
	while(1)
	{	

// 		bbzero(main_ver);
// 		memset(&gbl.posInfo, 0, sizeof(gbl.posInfo));
// 		memset(&gbl.dPosInfo, 0, sizeof(gbl.dPosInfo));
// 		
// 		iRet = TS_GetPosInfo(sd, &gbl.posInfo, &gbl.dPosInfo, gbl.askType, menu_id, CARD_TYPE);	//获取终端信息
// 		
		if (strcmp(log.g_pos_no, gbl.dPosInfo.pos_no) == 0)
		{
			Sleep(5000);//睡眠5秒
			continue;
		}
		else
		{
			//第一次获取到终端连接则启动终端监控线程
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

	strcpy( log.g_pos_no, gbl.dPosInfo.pos_no);	//设置线程的终端号变量(各线程独立，用于日志识别)
	//log.log_sd = sd;
	
	wrLog("终端监控开启……");
	wrYct3MonitorLog(gbl.dPosInfo.pos_no,"===========================");
	wrYct3MonitorLog(gbl.dPosInfo.pos_no,"开启终端监控");

	//初始化串口号，串口类型,签到获取YCT_DATA结构
	if (CYCTYPTrade::init_sign(&gbl , gbl.com , gbl.comType , yct_data) != 0)
	{
		return 1;
	}

	
	CYCTYPTrade::P_SET_MONITOR(&gbl,gbl.dPosInfo, gbl.com, gbl.comType,&yct_data);
	
	int iMonFrq = atoi(gbl.monitor_frequency);//监控频率
	
	while(1)
	{
		//CYCTYPTrade::termMonitor(&gbl);
		Sleep(iMonFrq * 60 * 1000);
	}
	
	wrYct3MonitorLog(gbl.dPosInfo.pos_no,"关闭终端监控");
}
*/


//解析数据库库字符串
int  getDB_Prority(char *str,char *delimiter,PDB_PRORITY db_prority) //分割字符串并查找需要的字符串对应的值
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
	DB_PRORITY db_prority; //数据结构体

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

	//进行解密
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


//解析数据库库字符串
int spit(char*str,char *delimiter,char * keyName,char * keyValue) //分割字符串并查找需要的字符串对应的值
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

