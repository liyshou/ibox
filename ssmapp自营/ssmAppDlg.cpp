// ssmAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "ssmApp.h"
#include "ssmAppDlg.h"
#include "TranProDlg.h"
#include "TranProThread.h"
#include "TranProDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CTranProThread *m_pTranProThread;

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
// CSsmAppDlg dialog

CSsmAppDlg::CSsmAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSsmAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSsmAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSsmAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSsmAppDlg)
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_flash);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSsmAppDlg, CDialog)
	//{{AFX_MSG_MAP(CSsmAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSsmAppDlg, CDialog)
	//{{AFX_EVENTSINK_MAP(CSsmAppDlg)
	ON_EVENT(CSsmAppDlg,IDC_SHOCKWAVEFLASH1,150 /* FSCommandShockwave */,OnFSCommandShockwaveflash1,VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSsmAppDlg message handlers

BOOL CSsmAppDlg::OnInitDialog()
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
	CRect rect;
	GetDesktopWindow()->GetWindowRect(&rect);
	MoveWindow(&rect);

	m_flash.MoveWindow(&rect);
	m_flash.SetMovie(g_MainSwf);
	m_flash.ShowWindow(TRUE);
	m_flash.SetFocus();
	
	SetForegroundWindow();
	SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	CRuntimeClass *pThreadClass=RUNTIME_CLASS(CTranProThread);
	m_pTranProThread=(CTranProThread *)AfxBeginThread(pThreadClass);
	DWORD ver=m_flash.FlashVersion();
	g_FlashVersion.Format("%d.%d.%d.%d",0xFF&(ver>>24),0xFF&(ver>>16),0xFF&(ver>>8),0xFF&(ver));
	
	//隐藏任务栏
#ifndef _DEBUG
	g_hTrayWnd=::FindWindow("Shell_TrayWnd",NULL);
	if(g_hTrayWnd)
		::ShowWindow(g_hTrayWnd,SW_HIDE);
#endif

	AfxBeginThread(__MessageToFlash,this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSsmAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSsmAppDlg::OnPaint() 
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
HCURSOR CSsmAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSsmAppDlg::CloseApp(int iFlag) 
{	
	if(iFlag==HALTSYS) 
	{
		::ShowCursor(TRUE);
		::ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE | EWX_POWEROFF,0);
	}	
	else if(iFlag==RESTART) 
	{
		::ShowCursor(TRUE);
		::ExitWindowsEx(EWX_REBOOT,1);
	}
	
	CDialog::OnOK();
}

BOOL CSsmAppDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_RBUTTONDOWN||pMsg->message==WM_RBUTTONUP)
		return FALSE;
	if(pMsg->message==WM_KEYDOWN && pMsg->hwnd==m_flash.m_hWnd)
	 	return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CSsmAppDlg::OnFSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args)
{
	// TODO: Add your control notification handler code here重发
	if(strcmp(command,"ExecFunc_Log"))
		g_Log.WriteLogEx(LOG_FLASH,"收到指令[%s,%s]",command,args);

	if (!strcmp(command,"fullscreen")||
		!strcmp(command,"showmenu")||
		!strcmp(command,"allowscale")||
		!strcmp(command,"exec")||
		!strcmp(command,"quit")||
		!strcmp(command,""))
		return;

	if(!strcmp(command,"ResponseOK"))
	{
		if(!g_FlashSerialDone.IsEmpty())
		{//除非重发次数满了还收不到响应 否则不应该流水号非空，又收到新的确认响应。
			g_Log.WriteLogEx(LOG_FLASH,"警告,Flash已处理流水号非空[%s]!",g_FlashSerialDone);
		}
		g_FlashSerialDone=args;
	}
	else
		g_QueueFromFlash.AddTail(command,args);
	//
}
UINT CSsmAppDlg::__MessageToFlash(LPVOID lpParam)
{
	return ((CSsmAppDlg*)lpParam)->MessageToFlash();
}
UINT CSsmAppDlg::MessageToFlash()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"线程启动:发送界面消息");
	while(true)
	{
		Sleep(0);
		if(!g_QueueToFlash.IsEmpty())
		{
			Sleep(50);
			CString csCommand,csParameter;
			int iData3;
			g_QueueToFlash.GetHead(csCommand,csParameter,iData3);
			OnCompleteTran(csCommand,csParameter);
		}
	}
	g_Log.WriteLogEx(LOG_JOURNAL,"线程终止:发送界面消息");
	return 0;
}
void CSsmAppDlg::OnCompleteTran(CString csCommand,CString csParameter)
{
	g_Log.WriteLogEx(LOG_FLASH,"发出指令[%s,%s]",csCommand,csParameter);
	if(!csCommand.Compare("CloseApp"))
	{//此类型命令本地消化 不出口至flash
		if(!csParameter.Compare("EXITAPP"))
 			CloseApp(EXITAPP);
		else if(!csParameter.Compare("HALTSYS"))
			CloseApp(HALTSYS);
		else if(!csParameter.Compare("RESTART"))
 			CloseApp(RESTART);
		return;
	}
 	//m_flash.SetVariable((LPCTSTR)(csCommand),(LPCTSTR)csParameter);
	CString serial=_GetField(csParameter,1,'|');
	CString ToFlashCommand;
	ToFlashCommand.Format("<invoke name=\"ToFlash_TranRet\" returntype=\"xml\"><arguments><string>%s</string></arguments></invoke>",csParameter);
	for(int i=0;i<10;++i)
	{
		CString FlashAnswer;
		try
		{
			FlashAnswer=m_flash.CallFunction(ToFlashCommand.GetBuffer());//这里会卡住，而且没有超时，永远不返回！
		}
		catch(...)
		{
			g_Log.WriteLogEx(LOG_TRACE,"ToFlashCommand Exception:%s,%s.",FlashAnswer,ToFlashCommand);
		}
		//g_Log.WriteLogEx(LOG_FLASH,"send ok.",i);
		int isOK=false;
		for(int j=0;j<10;++j)
		{
			MySleep(50);
			if(serial==g_FlashSerialDone)
			{
				g_FlashSerialDone.Empty();
				isOK=true;
				break;//收到响应 结束
			}
		}
		if(isOK)
			break;
		else
		{
			g_Log.WriteLogEx(LOG_FLASH,"重发%d次[%s}",i+1,serial);
			continue;
		}
	}
}
void CSsmAppDlg::MySleep(DWORD dwMilliseconds)
{
	MSG msg;
	int count=dwMilliseconds/10;
	for(int i=0;i<count;i++)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{

			if (msg.message != WM_QUIT)
			{
				if(NULL!=msg.hwnd)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				PostMessage(msg.message,msg.wParam,msg.lParam);
				break;
			}
		}
		Sleep(10);
	}
}
