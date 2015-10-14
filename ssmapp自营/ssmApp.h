// ssmApp.h : main header file for the SSMAPP application
//

#if !defined(AFX_SSMAPP_H__C5E66D91_C4DA_4AB4_9E74_A49F0E9069C5__INCLUDED_)
#define AFX_SSMAPP_H__C5E66D91_C4DA_4AB4_9E74_A49F0E9069C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxmt.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSsmAppApp:
// See ssmApp.cpp for the implementation of this class
//

class CSsmAppApp : public CWinApp
{
public:
	CSsmAppApp();
	CString m_csAppPath;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSsmAppApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSsmAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSMAPP_H__C5E66D91_C4DA_4AB4_9E74_A49F0E9069C5__INCLUDED_)
