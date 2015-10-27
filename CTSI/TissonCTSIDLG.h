// TissonCTSIDlg.h : header file
//

#if !defined(AFX_TissonCTSIDLG_H__DA401D23_201D_4BC7_8FA5_5D3AE337918D__INCLUDED_)
#define AFX_TissonCTSIDLG_H__DA401D23_201D_4BC7_8FA5_5D3AE337918D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DB_KEY "12345678"

typedef struct
{
	char Provider[100]; //Provider
	char Data[1024]; //Data Source
	char Password[100]; //Password
	char User[100]; //User ID
	char Persist[100]; //Persist Security Info
	
}
DB_PRORITY, *PDB_PRORITY;

/////////////////////////////////////////////////////////////////////////////
// CTissonCTSIDlg dialog

UINT OpenServer(LPVOID lpv);
UINT SessionThread(LPVOID lpv);
int YCTRevThread();
int YCTAckThread();
int getEposPar();
UINT OnlineThread(LPVOID lpv);
UINT chkAdoConn(LPVOID lpv);
UINT ReFreshData(LPVOID lpv);
UINT OpenBusServer(LPVOID lpv);
int  getDB_Prority(char * str,char *delimiter,PDB_PRORITY db_prority);
int spit(char *str,char *delimiter,char * keyName,char * keyValue) ; //¡¤???¡Á?¡¤?¡ä?2¡é2¨¦?¨°D¨¨¨°a¦Ì?¡Á?¡¤?¡ä???¨®|¦Ì??¦Ì
int convert_plain_DBstr(char *str,char *plainStr);
//DWORD WINAPI YCT3Monitor_MainHdl(LPVOID lpv);
DWORD WINAPI YCT3TermMonitor(LPVOID lpv);
DWORD WINAPI test();

#include <winsock2.h>
#include "Def.h"

class CTissonCTSIDlg : public CDialog
{
// Construction
public:
	CTissonCTSIDlg(CWnd* pParent = NULL);	// standard constructor
	
	void My_OnUpdatePar();
	void My_OnFlashDB();
//	void Menu_01(int i, PCTMENULIST pMenuList);
//	int Menu_01_hdl(int iMenuLevel,int iSelect,int loop,SOCKET sd);

	
// Dialog Data
	//{{AFX_DATA(CTissonCTSIDlg)
	enum { IDD = IDD_TissonCTSI_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTissonCTSIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTissonCTSIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFlashDB();
	afx_msg void sendMsg();
	afx_msg void sendInfo();
	afx_msg void sendBill();
	afx_msg void sendResult();
	afx_msg void OnChangeStat();
	afx_msg void OnUpdatePar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TissonCTSIDLG_H__DA401D23_201D_4BC7_8FA5_5D3AE337918D__INCLUDED_)
