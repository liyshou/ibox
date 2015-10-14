#if !defined(AFX_TRANPROTHREAD_H__514FE5AE_9BE0_4299_8EDA_DDD43AC1654A__INCLUDED_)
#define AFX_TRANPROTHREAD_H__514FE5AE_9BE0_4299_8EDA_DDD43AC1654A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranProThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CTranProThread thread

class CTranProThread : public CWinThread
{
	DECLARE_DYNCREATE(CTranProThread)
protected:
	CTranProThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranProThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTranProThread();

	// Generated message map functions
	//{{AFX_MSG(CTranProThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANPROTHREAD_H__514FE5AE_9BE0_4299_8EDA_DDD43AC1654A__INCLUDED_)
