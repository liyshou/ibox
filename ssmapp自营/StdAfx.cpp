// stdafx.cpp : source file that includes just the standard includes
//	ssmApp.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

HWND				g_hTrayWnd;
CLog				g_Log;
HANDLE				g_SystemCmdDown;
CQueue	  			g_QueueFromFlash;
CQueue				g_QueueToFlash;
CQueue	  			g_QueueLog;

CString				g_AppPathName;
CString				g_AppPath;
CString				g_MainSwf;
CString				g_ConfigFile;
CString				g_ConfigFile_bak;
CString				g_DogConfigFilel;
CMarkup				g_XMLConfig;
CString				g_AppVersion="1.0.7.6";
CString				g_RequirementVersion="20141112";
CString				g_FlashSerialDone;
CString				g_FlashVersion;
CString				g_FlashPlayerVersion;
int					g_PlainTraceLog;
