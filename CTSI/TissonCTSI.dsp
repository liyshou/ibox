# Microsoft Developer Studio Project File - Name="TissonCTSI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TissonCTSI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TissonCTSI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TissonCTSI.mak" CFG="TissonCTSI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TissonCTSI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TissonCTSI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TissonCTSI - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Z7 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TissonCTSI - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nafxcwd.lib libcmtd.lib wsock32.lib /nologo /stack:0x2dc6c0 /subsystem:windows /debug /machine:I386 /nodefaultlib:"nafxcwd.lib  libcmtd.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TissonCTSI - Win32 Release"
# Name "TissonCTSI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DES.cpp
# End Source File
# Begin Source File

SOURCE=.\ADOConn.cpp
# End Source File
# Begin Source File

SOURCE=.\AES.cpp
# End Source File
# Begin Source File

SOURCE=.\base64.cpp
# End Source File
# Begin Source File

SOURCE=.\cert.cpp
# End Source File
# Begin Source File

SOURCE=.\Connect.cpp
# End Source File
# Begin Source File

SOURCE=.\ctsipass.cpp
# End Source File
# Begin Source File

SOURCE=.\DB.cpp
# End Source File
# Begin Source File

SOURCE=.\Def.cpp
# End Source File
# Begin Source File

SOURCE=.\Des.cpp
# End Source File
# Begin Source File

SOURCE=.\DGBusTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\DGTBusTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\ks_soap.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuHdl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCrypto.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDes.cpp
# End Source File
# Begin Source File

SOURCE=.\Pack.cpp
# End Source File
# Begin Source File

SOURCE=.\Print.cpp
# End Source File
# Begin Source File

SOURCE=.\soapC.cpp
# End Source File
# Begin Source File

SOURCE=.\soapClient.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\stdsoap2.cpp
# End Source File
# Begin Source File

SOURCE=.\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\TissonCTSI.cpp
# End Source File
# Begin Source File

SOURCE=.\TissonCTSI.rc
# End Source File
# Begin Source File

SOURCE=.\TissonCTSIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\Trade.cpp
# End Source File
# Begin Source File

SOURCE=.\WrLog.cpp
# End Source File
# Begin Source File

SOURCE=.\xmlParser.cpp
# End Source File
# Begin Source File

SOURCE=.\YCT3_BalanceTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\YCT3_Purchase.cpp
# End Source File
# Begin Source File

SOURCE=.\YCT3Trade.cpp
# End Source File
# Begin Source File

SOURCE=.\YCTYPTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\ZHBusTrade.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DES.h
# End Source File
# Begin Source File

SOURCE=.\ADOConn.h
# End Source File
# Begin Source File

SOURCE=.\AES.h
# End Source File
# Begin Source File

SOURCE="D:\Microsoft Visual Studio\VC98\MFC\Include\AFXTLS_.H"
# End Source File
# Begin Source File

SOURCE="D:\Microsoft Visual Studio\VC98\MFC\Include\AFXV_W32.H"
# End Source File
# Begin Source File

SOURCE=.\base64.h
# End Source File
# Begin Source File

SOURCE=.\bestpay.h
# End Source File
# Begin Source File

SOURCE=.\cert.h
# End Source File
# Begin Source File

SOURCE=.\Connect.h
# End Source File
# Begin Source File

SOURCE=.\ctsipass.h
# End Source File
# Begin Source File

SOURCE=.\DB.h
# End Source File
# Begin Source File

SOURCE=.\Def.h
# End Source File
# Begin Source File

SOURCE=.\Des.h
# End Source File
# Begin Source File

SOURCE=.\DGBusTrade.h
# End Source File
# Begin Source File

SOURCE=.\DGTBUSPack.h
# End Source File
# Begin Source File

SOURCE=.\DGTBusTrade.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\ks_soap.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\MenuHdl.h
# End Source File
# Begin Source File

SOURCE=.\MyCrypto.h
# End Source File
# Begin Source File

SOURCE=.\MyDes.h
# End Source File
# Begin Source File

SOURCE=.\Pack.h
# End Source File
# Begin Source File

SOURCE=.\Print.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE="..\..\openssl-0.9.8\include\openssl\rsa.h"
# End Source File
# Begin Source File

SOURCE="F:\openssl\openssl-0.9.8\include\openssl\rsa.h"
# End Source File
# Begin Source File

SOURCE=.\soapH.h
# End Source File
# Begin Source File

SOURCE=.\soapStub.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stdsoap2.h
# End Source File
# Begin Source File

SOURCE=.\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\TissonCTSI.h
# End Source File
# Begin Source File

SOURCE=.\TissonCTSIDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\Trade.h
# End Source File
# Begin Source File

SOURCE=.\TSCTSI.h
# End Source File
# Begin Source File

SOURCE="..\..\dll_¶«Ý¸¹«½»\TSCTSI.h"
# End Source File
# Begin Source File

SOURCE=.\WrLog.h
# End Source File
# Begin Source File

SOURCE=.\xmlParser.h
# End Source File
# Begin Source File

SOURCE=.\YCT3_BalanceTransfer.h
# End Source File
# Begin Source File

SOURCE=.\YCT3_Purchase.h
# End Source File
# Begin Source File

SOURCE=.\YCT3Trade.h
# End Source File
# Begin Source File

SOURCE=.\YCTYPTrade.h
# End Source File
# Begin Source File

SOURCE=.\ZHBusTrade.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\centerm_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\centerm_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TissonCTSI.ico
# End Source File
# Begin Source File

SOURCE=.\res\TissonCTSI.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\bestpay.wsdl
# End Source File
# Begin Source File

SOURCE=.\cert.pem
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.dispatchCommand.req.xml
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.dispatchCommand.res.xml
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.dispatchCommandEXT.req.xml
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.dispatchCommandEXT.res.xml
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.dispatchCommandIPOS.req.xml
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.dispatchCommandIPOS.res.xml
# End Source File
# Begin Source File

SOURCE=.\DealProcessorSoapBinding.nsmap
# End Source File
# Begin Source File

SOURCE=.\libeay32.dll
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\privateKey.pem
# End Source File
# Begin Source File

SOURCE=.\readme
# End Source File
# Begin Source File

SOURCE=.\ssleay32.dll
# End Source File
# Begin Source File

SOURCE=..\..\483\ctsi\TSCTSI.dll
# End Source File
# Begin Source File

SOURCE=.\ssleay32.lib
# End Source File
# Begin Source File

SOURCE=.\libeay32.lib
# End Source File
# Begin Source File

SOURCE=.\TSCTSI.lib
# End Source File
# End Target
# End Project
