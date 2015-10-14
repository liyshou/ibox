// ssmAppDlg.h : header file
//
//{{AFX_INCLUDES()
#include "shockwaveflash.h"
#include "TranProDlg.h"
//}}AFX_INCLUDES

#if !defined(AFX_SSMAPPDLG_H__31732144_6399_41AD_BA6D_A90FF96DE5DE__INCLUDED_)
#define AFX_SSMAPPDLG_H__31732144_6399_41AD_BA6D_A90FF96DE5DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSsmAppDlg dialog

class CSsmAppDlg : public CDialog
{
// Construction
public:
	CSsmAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSsmAppDlg)
	enum { IDD = IDD_SSMAPP_DIALOG };
	CShockwaveFlash	m_flash;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSsmAppDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSsmAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args);
	
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//void OnCompleteTran(WPARAM wParam,LPARAM lParam);
	static UINT __MessageToFlash(LPVOID lpParam);
	UINT MessageToFlash();
	void MySleep(DWORD dwMilliseconds);
	void OnCompleteTran(CString csCommand,CString csParameter);	
	void CloseApp(int iFlag);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSMAPPDLG_H__31732144_6399_41AD_BA6D_A90FF96DE5DE__INCLUDED_)
