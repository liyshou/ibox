// TissonCTSI.h : main header file for the TissonCTSI application
//

#if !defined(AFX_TissonCTSI_H__BFAAFF3F_CD37_4ED6_97E6_A719397E035B__INCLUDED_)
#define AFX_TissonCTSI_H__BFAAFF3F_CD37_4ED6_97E6_A719397E035B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTissonCTSIApp:
// See TissonCTSI.cpp for the implementation of this class
//-

class CTissonCTSIApp : public CWinApp
{
public:
	CTissonCTSIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTissonCTSIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTissonCTSIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TissonCTSI_H__BFAAFF3F_CD37_4ED6_97E6_A719397E035B__INCLUDED_)
