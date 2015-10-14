// TranProThread.cpp : implementation file
//

#include "stdafx.h"
#include "ssmApp.h"
#include "TranProThread.h"
#include "TranProDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTranProThread

IMPLEMENT_DYNCREATE(CTranProThread, CWinThread)

CTranProThread::CTranProThread()
{
}

CTranProThread::~CTranProThread()
{
}

BOOL CTranProThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CTranProDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	return FALSE;
}

int CTranProThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTranProThread, CWinThread)
	//{{AFX_MSG_MAP(CTranProThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTranProThread message handlers
