// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8B2BDF19_11F0_47E4_967A_43D2B23528C4__INCLUDED_)
#define AFX_STDAFX_H__8B2BDF19_11F0_47E4_967A_43D2B23528C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _DEBUG
#define _AFXDLL
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Shlwapi.h"
#include <atlimage.h>
#include "Markup.h"
#include "Queue.h"
#include "Log.h"
#include "tool.h"
#include "LockerDll.h"
#include "CodeBarScanerDll.h"
#include "mysql.h"
#include <vector>
#include <map>

//#include "DataBase.h"

//#pragma comment(lib, "libmysql.lib")


#define  MAX_MESSAGE_SIZE 4096
#define  MAX_QUEUE_SIZE   1000
//
#define  LOG_JOURNAL	0
#define  LOG_KTVIEW		1

#ifdef _DEBUG
#define  LOG_TRACE		2
#else
#define  LOG_TRACE		2|128
#endif

#define  LOG_FLASH		3
#define  LOG_COMMUNICATION		4
#define  LOG_DATABASE		5

#define  LOG_ENCRYPT	128

#define  DEVICE_LEVEL_KEY		1//�ؼ��豸
#define  DEVICE_LEVEL_NORMAL	2//һ���豸

#define  WORKMODE_POST			1//Ͷ��
#define  WORKMODE_POSTMAN_TAKE	2//Ͷ��Ա����
#define  WORKMODE_TAKE			3//�û�ȡ��
#define  WORKMODE_POST_CANCEL	4//Ͷ��ȡ��
#define  WORKMODE_MANAGER_TAKE	5//����Աȡ��
//#define  WORKMODE_TIME_TAKE		6//ȡ��ʱ�� ��ȡ��ʱ�������մ���
#define  WORKMODE_OPENDOOR_TEST		7//����Ա�������

#define	RETRY_ENDLESS		0//�����ط������� ������

#define OPENDOOR_MODE_BOXID		1//ָ����ſ���
#define OPENDOOR_MODE_BOXTYPE	2//ָ�������Ϳ���
#define OPENDOOR_MODE_ONCE		3//һ���Կ���
//

extern	HWND				g_hTrayWnd;
extern  CLog				g_Log;
extern	HANDLE				g_SystemCmdDown;
extern  CQueue	  			g_QueueFromFlash;
extern  CQueue	  			g_QueueToFlash;
extern  CQueue	  			g_QueueLog;
extern  CString				g_ConfigFile;//���������ļ�
extern  CString				g_ConfigFile_bak;//���������ļ�
extern	CMarkup				g_XMLConfig;
extern	CString				g_AppPath;
extern	CString				g_MainSwf;
extern	CString				g_AppPathName;
extern	CString				g_AppVersion;//Ӧ�ð汾��
extern	CString				g_RequirementVersion;//����汾��
extern  CString				g_FlashSerialDone;
extern	CString				g_FlashVersion;
extern	CString				g_FlashPlayerVersion;
extern	int					g_PlainTraceLog;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__8B2BDF19_11F0_47E4_967A_43D2B23528C4__INCLUDED_)
