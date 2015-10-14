// TranProDlg.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <string>
#include <exception>
#include "ssmApp.h"
#include "TranProDlg.h"
#include "Winuser.h"
#include "Des.h"
#include "md5.h"

//#include <vector>
//#include "DataBase.h"
//#include <windows.h>
//#include <atmi.h>
//#include <fml.h>
//#include "eb_fml_tbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* TITLE_1				= "���������ն˽���ƾ��";
const char* TITLE_2				= "\n";

const char* ERR_MSG_FILTER		= ",";		//������Ϣ�ļ���ʽ����Ϣ����ͺ�����Ϣ��ķָ���
const char* STR_FILTER			= "|";		//�ֶηָ���


//bool CopyConfigFile(CString csSrcFile, CString csDesFile)
//{
//	CStdioFile f1(csSrcFile,CFile::modeRead);
//	CFile cFile;
//	//g_ConfigFile_bak
//
//	f1.
//
//	if(PathFileExists(g_ConfigFile_bak))
//	{
//		//if(!cFile.Open(acFileName,CFile::modeWrite|CFile::shareExclusive))
//		{
//			return false;
//		}
//	}
//	else
//	{
//		//if(!cFile.Open(acFileName,CFile::modeWrite|CFile::shareExclusive|CFile::modeCreate))
//		{
//			return false;
//		}
//	}	
//	cFile.SeekToEnd();
//	//cFile.Write(acLog,iLen);
//	cFile.Close();
//	return true;	
//
//
// return false;
//}


void CTranProDlg::TRSHOW(char* szOutBuf,...)
{
	va_list vl;
	char bufTmp[4096]={0};
	va_start(vl,szOutBuf);
	wvsprintf(bufTmp,szOutBuf,vl);
	va_end(vl);
	g_QueueToFlash.AddTail("ToFlash_TranRet",_MakeString("%s|%s",GetFlashSerial(),bufTmp));
}
void CTranProDlg::TRSHOW2(CString cs)
{
	cs.Insert(0,_MakeString("%s|",GetFlashSerial()));
	g_QueueToFlash.AddTail("ToFlash_TranRet",cs);
}
inline void CLOSEAPP(char* szOutBuf){g_QueueToFlash.AddTail("CloseApp",szOutBuf);}

/////////////////////////////////////////////////////////////////////////////

//m_tranCode������Ķ���
/*
600001	Ͷ��Ա��¼
600002	Ͷ��
600003	��ʱ������
600004	ȡ��
600005	����
600006	��ѯ��ʱ��
600007	��ѯ������Ϣ
600008	�ն��豸״̬�ϱ�
600009	��ҵ��״̬�ϱ�
600010	��֤��У��
600011	Ͷ��Աע��
600012	Ͷ��Ա����
600013	�ն˻�ע��
600014	ȡ���������֤
600015  Ͷ��Ա�ֻ���¼
600016  �ֻ���¼��̬����֤
600017  Ͷ��Ա��ѯ������Ϣ
*/

// CTranProDlg dialog

CTranProDlg::CTranProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTranProDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTranProDlg)
	//}}AFX_DATA_INIT

	m_pDB = NULL;
	m_IsLoadConfigFailed=false;
	m_bIsInited=false;
	m_iPort=0;
	m_IP.Empty();
	m_bIsRunningKeyboard=false;
	m_bIsRunningReadIDCard=false;
	m_bIsRunningReadICCard=false;
	m_bIsCancelReadIDCard=false;
	m_bIsCancelBarCode=false;
	m_bIsBarcodeOpening = false;
	m_bIsClosingDoor = false;
	m_bIsCheckingOpenDoor = false;
	m_iFlashSerial=0;
	m_iSerial=1;
	m_lDVR_UserID = -1;
	m_iIsUpdateingBoxStatus=0;
	m_iIsUpdateingStatusLocker=0;	
	m_iWorkMode=0;
	m_iIsPostDelaySend=0;
	m_iIsTakeoutDelaySend=0;
	m_iIsPostFailOpenDoor=0;
	m_iIsLoginInOffLine=0;
	m_iDownloadDay=-1;
	m_bIsKeyBoardPause=false;
	m_bRecvFlashHeartbeat=true;
	m_bSendTrans=false; 
	m_bNetWorkFail = false;
	m_bSendBoxStatus=false;
	m_bReboot = false;
	ZERO(&m_stDVR_StateInfo)

	m_iState=STATE_BUSY;
	m_iBarCodeCount = 0;
	m_iIsSending = 0;
	m_iNeedRegister = 0;
	m_iVerifyType = 2; //Ĭ���ֻ���+����У�鷽ʽ

	if(!LoadConfig())
	{
		m_IsLoadConfigFailed = true;
		//ExitSsmApp();
	}
}
CTranProDlg::~CTranProDlg()
{
	DisconnectDB();

	if(m_lDVR_UserID>0)
		NET_DVR_Logout(m_lDVR_UserID);
	NET_DVR_Cleanup();	
}


void CTranProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTranProDlg)
	
//	DDX_Control(pDX, IDC_STMDEVICECTRL1, m_device);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTranProDlg, CDialog)
	//{{AFX_MSG_MAP(CTranProDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_COPYDATA()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTranProDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTranProDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTranProDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTranProDlg)
	//ON_EVENT(CTranProDlg, IDC_ACTZT598CTRL1, 1 /* PressKey */, OnPressKeyActzt598ctrl1, VTS_I2)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTranProDlg message handlers

BOOL CTranProDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();	

	MoveWindow(-1,-1,0,0);
	ShowWindow(SW_HIDE);
	_InitWSAEnviorment();
	AfxBeginThread(__MessageFromFlash,this);

	return TRUE;  // return TRUE unless you set the focus to a controladminstrator
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTranProDlg::OnTimer(UINT nIDEvent) 
{	
	CDialog::OnTimer(nIDEvent);
}

void CTranProDlg::OnClose() 
{
	CDialog::OnClose();
}

UINT CTranProDlg::__DeviceSelfChecking(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->DeviceSelfChecking();
}
UINT CTranProDlg::DeviceSelfChecking()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"�߳�����:�豸�Լ�");
	DWORD start=GetTickCount()-(m_iSelfCheckInterval*1000)+30*1000;//do it after 30s
	DWORD now;
	while(true)
	{
		Sleep(1000);
		if(STATE_IDLE==m_iState)
		{
			now=GetTickCount();
			if((now-start)>=(DWORD)(m_iSelfCheckInterval*1000))
			{
				start=now;
				SelfTest();
			}
		}
	}

	g_Log.WriteLogEx(LOG_JOURNAL,"�߳���ֹ:�豸�Լ�");
	return 0;	
}

CString CTranProDlg::GetFlashSerial()
{
	if(99999999==m_iFlashSerial)
		m_iFlashSerial=1;
	else
		++m_iFlashSerial;
	return _MakeString("%08d",m_iFlashSerial);
}
UINT CTranProDlg::__NetworkMonitor(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->NetworkMonitor();
}

UINT CTranProDlg::NetworkMonitor()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"�߳�����:������");

	bool bNetWorkStatus = false,bSendNetWorkStatus=false;
	int iSendNetWorkFail=0,iRecvFailCount=0;
	int iDBconnetFail = 0;
	CString recv;
	int iReturnCode;
	CString msg;
	CTime now=CTime::GetCurrentTime();
	CTime startBoxStatus=now-(CTimeSpan)m_iBoxMonitorInterval;
	CTime startDeviceStatus=now-(CTimeSpan)m_iMonitorInterval;
	CTime startTransaction=now-(CTimeSpan)m_iTransactionQueueInterval;		
	CTime startFlashHeartbeat=now-(CTimeSpan)m_iFlashHeartbeatInterval;	
	CTimeSpan Inteval;

	while(true)
	{
		if(STATE_IDLE==m_iState) 
		{//flash�����������⴦��
			now=CTime::GetCurrentTime();
			Inteval=now-startFlashHeartbeat;
			bNetWorkStatus = CheckNetWork();  //�������
			if(Inteval.GetTotalSeconds()>=m_iFlashHeartbeatInterval || bNetWorkStatus!=bSendNetWorkStatus)//flash����ʱ�䵽 �� ��������ϴη������������״̬��ͬ
			{
				bSendNetWorkStatus=bNetWorkStatus;
				iRecvFailCount=0;
				if(iRecvFailCount>=5)
				{
					g_Log.WriteLogEx(LOG_JOURNAL,"**FlashͨѶ��������������");
					ExitWindows(EWX_REBOOT, EWX_FORCE);//��������						
				}

				m_bSendTrans=false;
				m_bRecvFlashHeartbeat=false;
				startFlashHeartbeat=now;
				if(!bNetWorkStatus)
				{
					if(!iSendNetWorkFail)
					{
						iSendNetWorkFail = 1;
						g_Log.WriteLogEx(LOG_JOURNAL,"**�������");					
					}
					TRSHOW("ExecFunc_NetworkOnline|0"); 							
				}
				if(bNetWorkStatus)
				{
					if(iSendNetWorkFail)
					{
						iSendNetWorkFail = 0;
						g_Log.WriteLogEx(LOG_JOURNAL,"����ָ�����");					
					}
					TRSHOW("ExecFunc_NetworkOnline|1"); 							
				}			
			}
			Sleep(1000);
		}
		if(STATE_IDLE==m_iState && !m_iNeedRegister)//��δ�����ն�ע�ᣬ���ϴ���ʱ״̬����
		{//��ҵ��״̬����
			now=CTime::GetCurrentTime();
			Inteval=now-startBoxStatus;
			if(Inteval.GetTotalSeconds()>=m_iBoxMonitorInterval || m_bSendBoxStatus)
			{
				m_bSendBoxStatus=false;
				startBoxStatus=now;
				m_tranCode = "600009";
				CString send=Pack();
				Transaction(send,recv,iReturnCode,msg);  

				//m_tranCode = "600018";
				//CString send=Pack();
				//Transaction(send,recv,iReturnCode,msg);  

				Sleep(1000);
			}
		}
		if(STATE_IDLE==m_iState && !m_iNeedRegister) 
		{//�ն�Ӳ��״̬����
			now=CTime::GetCurrentTime();
			Inteval=now-startDeviceStatus;
			if(Inteval.GetTotalSeconds()>=m_iMonitorInterval)
			{
				startDeviceStatus=now;
				m_tranCode = "600008";
				CString send=Pack();
				int iRet = Transaction(send,recv,iReturnCode,msg);  

				Sleep(1000);
			}
		}
		if(STATE_IDLE==m_iState)
		{//���׶���
			now=CTime::GetCurrentTime();
			Inteval=now-startTransaction;
			if(Inteval.GetTotalSeconds()>=m_iTransactionQueueInterval /*&& !iDBconnetFail*/)
			{
				startTransaction=now;
				TransactionQueue();
				Sleep(1000);
			}
		}
		if(STATE_IDLE==m_iState && !m_iNeedRegister)
		{//��֤�˺�����ĺϷ��� �� ��ʱ��������
			if(m_iIsLoginInOffLine)
			{
				now=CTime::GetCurrentTime();
				int a=now.GetHour();
				if(m_iDownloadDay!=now.GetDay()&&m_iDownloadOffLineData>=now.GetHour() && !iDBconnetFail)
				{
					m_iDownloadDay=now.GetDay();
					g_Log.WriteLogEx(LOG_JOURNAL,"׼�������ѻ�����");
					TRSHOW("ExecFunc_Maintenance"); 
					g_Log.WriteLogEx(LOG_JOURNAL,"�ȴ���ͣ����");
					int iCount = 0;
					while(iCount<300)//���ȴ�3��					
					{
						if(STATE_PAUSE==m_iState)
							break;
						++iCount;
						Sleep(10);
					}
					if(STATE_PAUSE==m_iState)
					{
						g_Log.WriteLogEx(LOG_JOURNAL,"��ʼ�����ѻ�����");
						CString strSQL="SELECT * FROM postman;";
						if (m_pDB->ExeSQLQuery(strSQL))
						{
							if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
							{
								MYSQL_RES * pRes=m_pDB->GetResultSet();
								int rowcount=(int)pRes->row_count;
								CString *result=new CString[rowcount*2];
								for(int i=0;i<rowcount;++i)
								{
									MYSQL_ROW row = NULL;
									mysql_data_seek(pRes,i);
									row = mysql_fetch_row(pRes);
									*(result+2*i)=row[0];
									*(result+2*i+1)=row[1];
								}
								m_pDB->FreeResultSet();
							
								for(int i=0;i<rowcount;++i)
								{							
									m_PostLoginID=*(result+2*i);
									m_PostLoginPW=*(result+2*i+1);

									m_cbox.SetBoxHostTimeoutAvailable();//�����ʱ��Ϣ
									m_tranCode = "600001";
									CString send=Pack();
									CString recv;
									CString csReturnMsg;
									if(Transaction(send,recv,iReturnCode,csReturnMsg)>0)//��ȷ������
									{//�˺�������֤ʧ��
										g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��Ա[%s]�˺�����ʧЧ",m_PostLoginID);
										DeletePostman(m_PostLoginID);									
									}
									else
									{
										m_tranCode = "600006";
										send=Pack();
										if(Transaction(send,recv,iReturnCode,csReturnMsg))
										{
											g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯ��ʱ����[%d][%s]",iReturnCode,csReturnMsg);
										}
										else
											SaveTimeoutPackageToDB();  //��¼�ɹ������泬ʱ����
									}
								}
								delete[] result;
							}						
						}

						m_PostLoginID.Empty();
						m_PostLoginPW.Empty();
						TRSHOW("ExecFunc_Service"); 
						g_Log.WriteLogEx(LOG_JOURNAL,"ͬ���ѻ��������");					
					}//if(STATE_PAUSE==m_iState)
				}//�����ѻ�������������				
			}//if(m_iIsLoginInOffLine)
		}			
		if(STATE_IDLE==m_iState)
		{
			if(m_bReboot)
			{
				now=CTime::GetCurrentTime();
				int iHour=now.GetHour();
				if(m_iRebootTime==iHour)			
				{
					g_Log.WriteLogEx(LOG_JOURNAL,"**���ݿ��쳣����������");
					ExitWindows(EWX_REBOOT, EWX_FORCE);//��������								
				}
			}		
		}
		if(!WaitForSingleObject(g_SystemCmdDown,0))
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"�յ���������:����Ӧ��");
			g_Log.WriteLogEx(LOG_JOURNAL,"׼������Ӧ��");
			while(true)
			{
				if(STATE_IDLE==m_iState)
					break;
				Sleep(200);
			}
			g_Log.WriteLogEx(LOG_JOURNAL,"��ʼ����Ӧ��");
			ExitSsmApp();
		}

		Sleep(1000);
	}

	g_Log.WriteLogEx(LOG_JOURNAL,"�߳���ֹ:������");
	return 0;
}

UINT CTranProDlg::__MessageFromFlash(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->MessageFromFlash();
}
UINT CTranProDlg::MessageFromFlash()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"�߳�����:���ս�����Ϣ");
	while(true)
	{
		if(!g_QueueFromFlash.IsEmpty())
		{
			CString csCommand,csParameter;
			int iUseless;
			g_QueueFromFlash.GetHead(csCommand,csParameter,iUseless);
			OnBeginTran(csCommand,csParameter);
			Sleep(100);
			continue;
		}
		Sleep(100);
	}
	g_Log.WriteLogEx(LOG_JOURNAL,"�߳���ֹ:���ս�����Ϣ");
	return 0;
}
UINT CTranProDlg::__ReadBarCode(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->ReadBarCode();
}
UINT CTranProDlg::ReadBarCode()
{
	if(m_bIsBarcodeOpening)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**�������߳�������");
		return 0;
	}

	g_Log.WriteLogEx(LOG_JOURNAL,"�������߳̿�ʼ");
	m_bIsBarcodeOpening = 1;

	CString barCode,barCodeBefore;
	CString cstmp;
	if(ERR_OK!=m_CodeBarScaner.SetWorkMode(4))	
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**������ת��ģʽ[4]����");
		TRSHOW("ExecFunc_BarCode|00|�����ǹ���[1]");	
	}
	else
	{
		while(true)
		{
			if(m_bIsCancelBarCode || STATE_IDLE==m_iState)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ��������");
				break;
			}
			barCode.Empty();
			Sleep(200);
			try
			{
				barCode=m_CodeBarScaner.GetBarCode();			
			}
			catch(...)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"**��ȡ�����쳣");
				TRSHOW("ExecFunc_BarCode|00|�����ǹ���[2]");
				break;
			}
			if(!barCode.IsEmpty())  //���ɹ������ַ�������
			{
				BYTE szFlag = 163;
				for(int i=0;i<barCode.GetLength();i++)
				{
					if(barCode.GetAt(i)<'0'|| barCode.GetAt(i)>'z')  
					{
						if(szFlag==(BYTE)barCode.GetAt(i)) //�ж��Ƿ�Ϊȫ���ַ�
						{
							if(i+1<barCode.GetLength())
							{
								barCode.SetAt(i,barCode.GetAt(i+1)-128); //ת�ɰ���ַ�
								barCode.Delete(i+1);
							}
							else
							{
								barCode.Delete(i);			
								i-=1;
							}
						}
						else   //��Ч�ַ�
						{
							barCode.Delete(i);			
							i-=1;
						}
					}
					else 
					{
						if( (barCode.GetAt(i)>'9'&& barCode.GetAt(i)<'A') || (barCode.GetAt(i)>'Z'&& barCode.GetAt(i)<'a') )
						{
							barCode.Delete(i);	 //ɾ�������ֺͷ���ĸ		
							i-=1;
						}			
					}					
				}
				barCode.AppendChar('\0');
				g_Log.WriteLogEx(LOG_JOURNAL,"������ɹ�[%d:%s]",barCode.GetLength(),barCode);
				if(barCode.GetLength()>7 && barCode.Compare(barCodeBefore))//ɨ��������һ�β�ͬʱ������Ӧ
				{
					barCodeBefore = barCode;
					TRSHOW("ExecFunc_BarCode|01|%s",barCode);						
				}
			}
		}
	}

	m_CodeBarScaner.SetWorkMode(6);
	m_bIsBarcodeOpening=0;//��ȡ�رգ�
	g_Log.WriteLogEx(LOG_JOURNAL,"�������߳���ֹ");
	return 0;
}
UINT CTranProDlg::__ReadIDCard(LPVOID lpParam)//��ȡ��������̣߳�
{
	return ((CTranProDlg*)lpParam)->ReadIDCard();
}
UINT CTranProDlg::ReadIDCard()
{
	if(m_bIsRunningReadIDCard)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"�����֤�̲߳����ظ�����!");
	}
	else
	{	
		m_bIsRunningReadIDCard=true;
		g_Log.WriteLogEx(LOG_JOURNAL,"�����֤�߳̿�ʼ");
		CIDCardData cIDCardData;
		CString last;
		while(true)
		{
			if(m_bIsCancelReadIDCard || STATE_IDLE==m_iState)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ�������֤");
				break;
			}
			Sleep(200);
		
			if(ERR_OK==m_oIDCardReader.Authenticate()&&ERR_OK==m_oIDCardReader.ReadContent(cIDCardData))
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�����֤�ɹ�");
				if(m_iAllowRepeatIDNO)
					TRSHOW("ExecFunc_ReadIDCard|01|%s|%s",cIDCardData.name,cIDCardData.idcode);
				else
				{
					if(last!=cIDCardData.idcode)
					{
						last=cIDCardData.idcode;
						m_cIDCardData.name = cIDCardData.name;  //ˢ������������ʱҪ�ϴ�
						m_cIDCardData.idcode = cIDCardData.idcode;
						m_cIDCardData.sex=cIDCardData.sex;
						m_cIDCardData.department=cIDCardData.department;
						m_cIDCardData.start=cIDCardData.start;
						TRSHOW("ExecFunc_ReadIDCard|01|%s|%s",cIDCardData.name,cIDCardData.idcode);
					}
				}
				cIDCardData.ClearData();
				Sleep(300);
			}
		}

		m_bIsRunningReadIDCard=false;
		g_Log.WriteLogEx(LOG_JOURNAL,"�����֤�߳���ֹ");
	}	
	return 0;	
}
UINT CTranProDlg::__ReadICCard(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->ReadICCard();	
}
UINT CTranProDlg::ReadICCard()
{	
	if(m_bIsRunningReadICCard)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"��IC���̲߳����ظ�����!");
	}
	else
	{
		m_bIsRunningReadICCard=true;
		g_Log.WriteLogEx(LOG_JOURNAL,"��IC���߳̿�ʼ");
		CString lastsn;

#ifndef _DEBUG
		while(true)
		{
			if(m_bIsCancelReadICCard /*|| STATE_IDLE==m_iState*/)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ����IC��");
				break;
			}
			MySleep(200);
			CString sn=m_oIDCardReader.ICReadCardSN();
			if(!sn.IsEmpty())
			{
				CString snReverse = sn.Mid(6,2) + sn.Mid(4,2) + sn.Mid(2,2) + sn.Mid(0,2);
				unsigned int iSn = 0;
				sscanf(snReverse, "%x", &iSn);//��ʮ������תΪʮ����
				sn.Format("%u", iSn);//����޷������Σ�
				if(m_iAllowRepeatIDNO)
					TRSHOW("ExecFunc_ReadICCard|01|%s",sn);
				else
				{
					if(lastsn!=sn)
					{
						lastsn=sn;
						TRSHOW("ExecFunc_ReadICCard|01|%s",sn);
					}
				}
				MySleep(1000);
			}
		}
#else
		//Sleep(5000);
		//TRSHOW("ExecFunc_ReadICCard|01|12345678");
#endif
		m_bIsRunningReadICCard=false;
		g_Log.WriteLogEx(LOG_JOURNAL,"��IC���߳���ֹ");
	}
	
	return 0;
}
UINT CTranProDlg::__ReadKeyBoard(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->ReadKeyBoard();
}
UINT CTranProDlg::ReadKeyBoard()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"�����߳̿�ʼ");
	m_bIsRunningKeyboard = true;
	while(true)
	{
		Sleep(100);
		if(m_bIsKeyBoardPause)
			continue;
		BYTE ch=0;
		DWORD len=1;
		m_pincom.ReadCom(&ch,len,1000);
		if(ch && STATE_IDLE!=m_iState)
		{
			TRSHOW("ExecFunc_KeyEvent|%d",ch);
		}		
	}
	m_bIsRunningKeyboard = false;
	g_Log.WriteLogEx(LOG_JOURNAL,"�����߳���ֹ");
	return 0;
}
UINT CTranProDlg::CloseDoor()
{
	if(m_bIsClosingDoor)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"�����̲߳����ظ�����!");
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"�����߳̿�ʼ");
		m_bIsClosingDoor = 1;
		int iHavePackage=0;
		int iSendMessage=0;
		int iOpenDoorCount=0;
	
		while(true)
		{
			if(m_bIsCancelCloseDoor || STATE_IDLE==m_iState)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ������");
				break;
			}

			if(CloseLogicalBox(m_iLogicalCurrentBox,iHavePackage))
			{
				m_iCurrentOpenBox = -1;
				//Sleep(300);//����̫����ERR_COM
				if(WORKMODE_POST==m_iWorkMode)
				{
					//if(!IsEmptyLogicalBox(m_iLogicalCurrentBox))	//iStatus[1]: 1���л���	  0���޻��� 
					if(!iHavePackage)
					{
						g_Log.WriteLogEx(LOG_JOURNAL,"**Ͷ�ݷ�����ź��ⲻ������");
						TRSHOW("ExecFunc_CloseDoor|00|����û����!�����²���.");					
					}
					else
					{
						m_cbox.SetNotEmptyFail(m_iLogicalCurrentBox,0); //
						TRSHOW("ExecFunc_CloseDoor|01");
					}
					break;
				}
				else if(WORKMODE_POSTMAN_TAKE==m_iWorkMode||WORKMODE_TAKE==m_iWorkMode||WORKMODE_MANAGER_TAKE==m_iWorkMode||WORKMODE_POST_CANCEL==m_iWorkMode)
				{
					//if(IsEmptyLogicalBox(m_iLogicalCurrentBox))
					if(iHavePackage)
						g_Log.WriteLogEx(LOG_JOURNAL,"**ȡ��������ɺ�,�߼���[%d]�ڷǿ�",m_iLogicalCurrentBox);
					TRSHOW("ExecFunc_CloseDoor|01");
					break;
				}
				else
				{
					if(STATE_IDLE!=m_iState)
						TRSHOW("ExecFunc_CloseDoor|01");
					break;
				}
			}
			else
			{
				if(!iSendMessage && WORKMODE_POST==m_iWorkMode)
				{
					//Sleep(300);
					//if(IsEmptyLogicalBox(m_iLogicalCurrentBox))//δ���ż�⵽������������ʾ��������ʾ�ı䡣ֻ��һ��
					if(iHavePackage)
					{
						iSendMessage = 1;		
						TRSHOW("ExecFunc_CheckInputPackage|01");
					}				
				}
			}

			Sleep(500);
		}

		m_bIsClosingDoor = 0;
		g_Log.WriteLogEx(LOG_JOURNAL,"�����߳���ֹ");
	}

	return 0;
}
UINT CTranProDlg::__CloseDoor(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->CloseDoor();	
}
UINT CTranProDlg::__CheckOpenDoor(LPVOID lpParam)
{
	return ((CTranProDlg*)lpParam)->CheckOpenDoor();	
}
UINT CTranProDlg::CheckOpenDoor()
{
	if(m_bIsCheckingOpenDoor)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"����ſ��̲߳����ظ�����!");
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"����ſ��߳̿�ʼ");
		m_bIsCheckingOpenDoor = true;
		int iOpenCount=0,iLastCount=-1;
		int iAllBoxCount=0,iLogicalBox = 0;
		int iCabinetCount = m_cbox.GetCabinetCount();
		int iStatus[64]={-1};
			
		while(true)
		{
			if(m_bIsCancelCheckOpenDoor || STATE_IDLE==m_iState || STATE_PAUSE==m_iState)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ������ſ�״̬");
				break;
			}

			iOpenCount = 0;
			CString csMessage="��";
			for(int i=0;i<iCabinetCount;i++)
			{
				if(i>0)
					iAllBoxCount += m_cbox.GetBoxCount(i);

				Sleep(10);
				if(!m_lockerDll.Status(m_cbox.GetCabinetID(i+1),0,iStatus)) 
				{
					int iBoxCount = m_cbox.GetBoxCount(i+1);
					for(int j=0;j<iBoxCount;j++)
					{
						iLogicalBox = iAllBoxCount+j+1;//��ǰ�߼����
						if(!iStatus[j] && m_cbox.GetClassifyBoxSpec(iLogicalBox))//�ſ� 
						{
							++iOpenCount;
							m_iCurrentOpenBox = iLogicalBox;
							csMessage += _MakeString("%s,",m_cbox.GetBoxName(iLogicalBox));
						}
					}				
				}
			}

			csMessage.TrimRight(',');
			csMessage+="�����Ŵ�,��ر����ź��ٽ���Ͷ��";

			if(m_bIsCancelCheckOpenDoor || STATE_IDLE==m_iState || STATE_PAUSE==m_iState)//����Ϣʱ�ȼ���Ƿ��������ת
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ������ſ�״̬");
				break;
			}

			if(iOpenCount!=iLastCount)
			{//���̫ⷢ����Ϣ��flash
				TRSHOW("ExecFunc_CheckOpenedDoor|%d|%s",iOpenCount,csMessage);	
				iLastCount=iOpenCount;
			}

			if(!iOpenCount) //�ſ�����0��ʱ��ֱ����ת��Ͷ������ 
			{
				m_iCurrentOpenBox = -1;
				break;		
			}

			Sleep(500);
		}

		m_bIsCheckingOpenDoor = false;
		g_Log.WriteLogEx(LOG_JOURNAL,"����ſ��߳���ֹ");
	}

	return 0;
}

int CTranProDlg::Init()
{
//////////////////////////////
	//char var[64]={0};
	//		m_csMobile = "123456789*1";
	//		__int64 a = _atoi64(m_csMobile);
	//		int b = atoi(m_csMobile);
	//		int c = atol(m_csMobile);
	//		CString csMobile = _MakeString("%d",_atoi64(m_csMobile));
	//		CString csMobile2 = _i64toa(_atoi64(m_csMobile),var,10);
	//			int  d = 1;
		//m_csTerminalInitKey = "98989898";
		//m_tranCode="600018";
		//CString send=Pack();
		//CString recv,csReturnMsg;
		//int iReturnCode;

		//if(Transaction(send,recv,iReturnCode,csReturnMsg))
		//{//fail
		//	g_Log.WriteLogEx(LOG_JOURNAL,"**�ն˳�ʼ��ʧ��");
		//}
		//else
		//{
		//	g_Log.WriteLogEx(LOG_JOURNAL,"�ն˳�ʼ���ɹ�");
		//}
		//TRSHOW("ExecFunc_InitMsg|01|�˳��鿴");
		//while(true)
		//{
		//	Sleep(2000);
		//}
///////////////////////////

	if(m_IsLoadConfigFailed)
	{
		TRSHOW("ExecFunc_InitMsg|01|����������Ϣʧ��");
		Loop();	
	}

	if(!InitDB())
	{		
		TRSHOW("ExecFunc_InitMsg|01|��ʼ�����ݿ�ʧ��");
		Loop();		
	}

	if(m_bIsInited)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"����:�ظ���ʼ��!!");
		return -1;
	}

	m_bIsInited=true;
	m_csProfile.Format("������:%s\r\n�ն˺�:%s\r\n������:%s\r\nFlash:%s\r\n",m_BankID,m_TermID,g_AppVersion,g_FlashVersion);
	TRSHOW("ExecFunc_InitMsg|01|�����Ű��ն�ϵͳ��������...\r\n%s",m_csProfile);

	g_Log.WriteLogEx(LOG_JOURNAL,"������[%s]",m_BankID);
	g_Log.WriteLogEx(LOG_JOURNAL,"�ն˺�[%s]",m_TermID);
	g_Log.WriteLogEx(LOG_JOURNAL,"����IP[%s] MAC[%s]",m_localIP,m_localMAC);
	g_Log.WriteLogEx(LOG_JOURNAL,"��̨��ַ[%s:%d]",m_IP,m_iPort);
	g_Log.WriteLogEx(LOG_JOURNAL,"������[%s]",g_AppVersion);
	g_Log.WriteLogEx(LOG_JOURNAL,"����汾��[%s]",g_RequirementVersion);
	g_Log.WriteLogEx(LOG_JOURNAL,"Flash�汾��[%s]",g_FlashVersion);
	g_Log.WriteLogEx(LOG_JOURNAL,"���ú�����汾��[%s]",_GetVersion());
	g_Log.WriteLogEx(LOG_JOURNAL,"�������ذ汾��[%s].",m_lockerDll.GetDLLVersion());
	g_Log.WriteLogEx(LOG_JOURNAL,"�߼����֤�������汾��[%s]",m_oIDCardReader.GetDLLVersion());
	g_Log.WriteLogEx(LOG_JOURNAL,"����ɨ���ǰ汾��[%s]",m_CodeBarScaner.GetDLLVersion());

	Sleep(1000);
	SleepDebug(2000);
	g_Log.WriteLogEx(LOG_JOURNAL,"��ʼ���豸");

	mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,100,100,NULL,NULL); 

	InitAllDevices();
	UpdateBoxStatus();
	AfxBeginThread(__DeviceSelfChecking,this);
	AfxBeginThread(__NetworkMonitor,this); //������

	Sleep(1000);
	if(m_iNeedRegister)//�Զ���ʼ��
	{
		int currentOption=GetDataIntXMLConfig("KingPeace\\Box\\CurrentOption");
		CString csBoxName = GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxName",currentOption));
		if(csBoxName.IsEmpty())
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**���÷�������,δ���ø������");	
		}
		else
		{
			m_csTerminalInitKey = GetDataXMLConfig("KingPeace\\Terminal\\HostKey");
			m_tranCode="600018";
			CString send=Pack();
			CString recv,csReturnMsg;
			int iReturnCode;

			if(Transaction(send,recv,iReturnCode,csReturnMsg))
			{//fail
				g_Log.WriteLogEx(LOG_JOURNAL,"**�ն˳�ʼ��ʧ��[%d][%s]",iReturnCode,csReturnMsg);
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�ն˳�ʼ���ɹ�");
				SetDataXMLConfig("KingPeace\\Terminal\\NeedRegister","0");
				m_iNeedRegister = 0;
			}
		}	
	}
	

//////////////////////////////////
	//HandWriteStop();
	//HandWriteStartUp(this->GetSafeHwnd());
	//while(true)
	//{
	//	Sleep(10);
	//}

	//int iVar = 300;
	//TRSHOW("ExecFunc_InitMsg|01|���ڽ������ذ�ѹ�����ԣ�����رճ��򼰵��ԡ�����Ƶ��[%d]����",iVar);
	//CTime start=CTime::GetCurrentTime();
	//g_Log.WriteLogEx(LOG_JOURNAL,"ѹ�����Կ�ʼʱ��: %d��%d�ա�%d:%d:%d",\
	//	start.GetMonth(),start.GetDay(),start.GetHour(),start.GetMinute(),start.GetSecond());

	//int iCount[18]={0};
	//int iAllCount = 0;
	//int iFailCount = 0,iTestCount=0;
	//int iStatus[64]={-1};
	//int iRet=0;
	//while(true)
	//{
	//	if(iTestCount>2000)
	//		iTestCount;

	//	for(int i=0;i<18;i++)
	//	{
	//		for(int j=0;j<100;j++)
	//		{
	//			Sleep(iVar);
	//			++iTestCount;
	//			iRet=m_lockerDll.Status(1,i+1,iStatus);
	//			if(ERR_OK!=iRet)
	//			{	
	//				++iCount[i];
	//				++iFailCount;

	//			}			
	//			else
	//				iFailCount=0;
	//		}
	//		Sleep(iVar);
	//		++iTestCount;
	//		iRet=m_lockerDll.Status(1,0,iStatus);
	//		if(ERR_OK!=iRet)
	//		{	
	//			++iAllCount;
	//			++iFailCount;
	//		}			
	//		else
	//			iFailCount=0;

	//		if(iFailCount>=99)
	//			break;	
	//	}   

	//	if(iTestCount>=3000)
	//	{
	//		for(int i=0;i<18;i++)
	//		{
	//			if(iCount[i])
	//				g_Log.WriteLogEx(LOG_JOURNAL,"%d������ϴ�����[%d]",i+1,iCount[i]);					
	//		}
	//		if(iAllCount)
	//			g_Log.WriteLogEx(LOG_JOURNAL,"������ϴ�����[%d]",iAllCount);	
	//	}

	//	if(iFailCount>=99)
	//		break;	
	//}
	//CTime end=CTime::GetCurrentTime();
	//for(int i=0;i<18;i++)
	//	g_Log.WriteLogEx(LOG_JOURNAL,"%d������ϴ�����[%d]",i+1,iCount[i]);

	//g_Log.WriteLogEx(LOG_JOURNAL,"ѹ�����Խ�������ʼʱ��: %d��%d�ա�%d:%d:%d������ʱ��: %d��%d�ա�%d:%d:%d",\
	//	start.GetMonth(),start.GetDay(),start.GetHour(),start.GetMinute(),start.GetSecond(),\
	//	end.GetMonth(),end.GetDay(),end.GetHour(),end.GetMinute(),end.GetSecond());

	//TRSHOW("ExecFunc_InitMsg|01|ѹ�����Խ�������ʼʱ��: %d��%d�ա�%d:%d:%d������ʱ��: %d��%d�ա�%d:%d:%d",\
	//	start.GetMonth(),start.GetDay(),start.GetHour(),start.GetMinute(),start.GetSecond(),\
	//	end.GetMonth(),end.GetDay(),end.GetHour(),end.GetMinute(),end.GetSecond());

	//ExitWindows(EWX_SHUTDOWN, EWX_FORCE);//	
//////////////////////

	if(m_iNeedRegister)
	{
		TRSHOW("ExecFunc_InitMsg|01|�ն˳�ʼ��ʧ��");
		Loop();		
	}
	TRSHOW("ExecFunc_Init|01");
	return true;//flash turn to out of service if return false
}

void CTranProDlg::Loop()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"������ѭ��");
	while(true)
		Sleep(1000);
}

int CTranProDlg::LoadConfig()
{
	if(!g_XMLConfig.Load(g_ConfigFile))
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**����������Ϣʧ��");
		return 0;
	}

	bool bFail=false;
	m_pDB = new CDataBase();
	m_csDBIP = GetDataXMLConfig("KingPeace\\DataBase\\IP");
	m_csDBUser = GetDataXMLConfig("KingPeace\\DataBase\\User");
	m_csDBPassword = GetDataXMLConfig("KingPeace\\DataBase\\PW");
	m_csDBName = GetDataXMLConfig("KingPeace\\DataBase\\DBName");
	m_iDBPort = GetDataIntXMLConfig("KingPeace\\DataBase\\Port");
	if (m_iDBPort <= 0)
		m_iDBPort = 3306;	
	g_Log.WriteLogEx(LOG_JOURNAL,"ʹ�����ݿ�%s",m_csDBName);

	m_stHardWare[DEVICE_LOCKER].csName=GetDataXMLConfig("KingPeace\\HardWare\\Locker\\Name");
	m_stHardWare[DEVICE_LOCKER].iExist=GetDataIntXMLConfig("KingPeace\\HardWare\\Locker\\Exist");	
	m_stHardWare[DEVICE_LOCKER].iLevel=GetDataIntXMLConfig("KingPeace\\HardWare\\Locker\\Level");
	m_stHardWare[DEVICE_LOCKER].iType=GetDataIntXMLConfig("KingPeace\\HardWare\\Locker\\Type");
	m_stHardWare[DEVICE_LOCKER].iPort=GetDataIntXMLConfig("KingPeace\\HardWare\\Locker\\Port");
	m_stHardWare[DEVICE_LOCKER].iStatus=0;

	m_stHardWare[DEVICE_IDCR].csName=GetDataXMLConfig("KingPeace\\HardWare\\IDCardReader\\Name");
	m_stHardWare[DEVICE_IDCR].iExist=GetDataIntXMLConfig("KingPeace\\HardWare\\IDCardReader\\Exist");
	m_stHardWare[DEVICE_IDCR].iLevel=GetDataIntXMLConfig("KingPeace\\HardWare\\IDCardReader\\Level");
	m_stHardWare[DEVICE_IDCR].iType=GetDataIntXMLConfig("KingPeace\\HardWare\\IDCardReader\\Type");
	m_stHardWare[DEVICE_IDCR].iPort=GetDataIntXMLConfig("KingPeace\\HardWare\\IDCardReader\\Port");
	m_stHardWare[DEVICE_IDCR].iStatus=0;

	m_stHardWare[DEVICE_BCR].csName=GetDataXMLConfig("KingPeace\\HardWare\\BarCodeReader\\Name");
	m_stHardWare[DEVICE_BCR].iExist=GetDataIntXMLConfig("KingPeace\\HardWare\\BarCodeReader\\Exist");
	m_stHardWare[DEVICE_BCR].iLevel=GetDataIntXMLConfig("KingPeace\\HardWare\\BarCodeReader\\Level");
	m_stHardWare[DEVICE_BCR].iType=GetDataIntXMLConfig("KingPeace\\HardWare\\BarCodeReader\\Type");
	m_stHardWare[DEVICE_BCR].iPort=GetDataIntXMLConfig("KingPeace\\HardWare\\BarCodeReader\\Port");
	m_stHardWare[DEVICE_BCR].iStatus=0;

	m_stHardWare[DEVICE_PIN].csName=GetDataXMLConfig("KingPeace\\HardWare\\PIN\\Name");
	m_stHardWare[DEVICE_PIN].iExist=GetDataIntXMLConfig("KingPeace\\HardWare\\PIN\\Exist");
	m_stHardWare[DEVICE_PIN].iLevel=GetDataIntXMLConfig("KingPeace\\HardWare\\PIN\\Level");
	m_stHardWare[DEVICE_PIN].iType=GetDataIntXMLConfig("KingPeace\\HardWare\\PIN\\Type");
	m_stHardWare[DEVICE_PIN].iPort=GetDataIntXMLConfig("KingPeace\\HardWare\\PIN\\Port");
	m_stHardWare[DEVICE_PIN].iStatus=0;

	m_stHardWare[DEVICE_DVR].csName=GetDataXMLConfig("KingPeace\\HardWare\\DVR\\Name");
	m_stHardWare[DEVICE_DVR].iExist=GetDataIntXMLConfig("KingPeace\\HardWare\\DVR\\Exist");
	m_stHardWare[DEVICE_DVR].iLevel=GetDataIntXMLConfig("KingPeace\\HardWare\\DVR\\Level");
	m_stHardWare[DEVICE_DVR].iPort=GetDataIntXMLConfig("KingPeace\\HardWare\\DVR\\Port");
	m_stHardWare[DEVICE_DVR].csIP=GetDataXMLConfig("KingPeace\\HardWare\\DVR\\IP");
	m_stHardWare[DEVICE_DVR].csUserName=GetDataXMLConfig("KingPeace\\HardWare\\DVR\\User");
	m_stHardWare[DEVICE_DVR].csPW=GetDataXMLConfig("KingPeace\\HardWare\\DVR\\PW");
	m_stHardWare[DEVICE_DVR].iCount=min(GetDataIntXMLConfig("KingPeace\\HardWare\\DVR\\Count"),3);//���3������ͷ
	m_stHardWare[DEVICE_DVR].iStatus=0;

	int currentOption=GetDataIntXMLConfig("KingPeace\\Box\\CurrentOption");
	_GoToXML(g_XMLConfig,_MakeString("KingPeace\\Box\\Option%d",currentOption),1);
	g_Log.WriteLogEx(LOG_JOURNAL,"������÷���%d:%s",currentOption,g_XMLConfig.GetAttrib("disc"));
	m_cbox.Init(_GetFieldCount(GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxCount",currentOption)),','),_GetFieldCount(GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxType",currentOption)),','),GetDataIntXMLConfig("KingPeace\\Box\\BoxSizeCount"));
	for(int i=0;i<m_cbox.GetBoxSizeCount();++i)
		m_cbox.InitBoxSize(i+1,GetDataIntXMLConfig(_MakeString("KingPeace\\Box\\BoxSize%d\\Length",i+1)),GetDataIntXMLConfig(_MakeString("KingPeace\\Box\\BoxSize%d\\Width",i+1)),GetDataIntXMLConfig(_MakeString("KingPeace\\Box\\BoxSize%d\\Height",i+1)));
	if(!(m_cbox.InitBoxCountBatch(GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxCount",currentOption)))
		&& m_cbox.InitBoxTypeBatch(GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxType",currentOption)))
		&& m_cbox.InitBoxHighBatch(GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxHigh",currentOption)))
		/*&& m_cbox.InitBoxNameBatch(GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxName",currentOption)))*/ ))
		bFail=true;  //���÷�����ֹͣ����
	CString csBoxName = GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxName",currentOption));
	if(csBoxName.IsEmpty())
	{//��û�����ø�����ƣ���Ĭ��Ϊ����
		for(int i=0;i<m_cbox.GetBoxCountTotal();i++)	
			csBoxName+=_MakeString("%d,",i+1);
		csBoxName.TrimRight(',');
	}
	m_cbox.InitBoxNameBatch(csBoxName);

	m_iAllowRepeatIDNO=GetDataIntXMLConfig("KingPeace\\Terminal\\AllowRepeatIDNO");
	g_Log.WriteLogEx(LOG_JOURNAL,"�����ȡ�ظ����֤��IC��:%s",m_iAllowRepeatIDNO?"��":"��");
	m_iIsPostDelaySend=GetDataIntXMLConfig("KingPeace\\Terminal\\PostDelaySend");
	g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��ȷ��%sʱ����",m_iIsPostDelaySend?"��":"ʵ");
	m_iIsPostFailOpenDoor=GetDataIntXMLConfig("KingPeace\\Terminal\\PostFailOpenDoor");
	if(!m_iIsPostDelaySend)
	{//ʵʱ����
		g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��ʵʱ����ʧ��%s����",m_iIsPostFailOpenDoor?"":"��");
	}
	m_iIsLoginInOffLine=GetDataIntXMLConfig("KingPeace\\Terminal\\LoginInOffLine");
	g_Log.WriteLogEx(LOG_JOURNAL,"%s����½",m_iIsLoginInOffLine?"��":"��");
	m_iDownloadOffLineData=GetDataIntXMLConfig("KingPeace\\Terminal\\DownloadOffLineData")%24;
	if(m_iIsLoginInOffLine)
		g_Log.WriteLogEx(LOG_JOURNAL,"ÿ�������ѻ�����ʱ��:%dʱ",m_iDownloadOffLineData);
	m_iLoginInOnLineWhileOffLineFail=GetDataIntXMLConfig("KingPeace\\Terminal\\LoginInOnLineWhileOffLineFail");
	if(m_iIsLoginInOffLine)
		g_Log.WriteLogEx(LOG_JOURNAL,"�ѻ���½ʧ�ܺ�%s������½",m_iLoginInOnLineWhileOffLineFail?"":"��");
	m_iIsTakeoutDelaySend=GetDataIntXMLConfig("KingPeace\\Terminal\\TakeoutDelaySend");
	g_Log.WriteLogEx(LOG_JOURNAL,"ȡ������%sʱ����",m_iIsTakeoutDelaySend?"��":"ʵ");
	m_iPlainMessage=GetDataIntXMLConfig("KingPeace\\Terminal\\PlainMessage");	
	g_Log.WriteLogEx(LOG_JOURNAL,"����������̨���ı���:%s",m_iPlainMessage?"��":"��");
	m_iUpdateTime=GetDataIntXMLConfig("KingPeace\\Terminal\\UpdateTime");
	g_Log.WriteLogEx(LOG_JOURNAL,"ͬ������ʱ��:%s",m_iUpdateTime?"��":"��");
	g_PlainTraceLog=GetDataIntXMLConfig("KingPeace\\Terminal\\PlainTraceLog");
	if(g_PlainTraceLog)
		g_Log.WriteLogEx(LOG_JOURNAL,"���ĵ�����־");

	m_iNetTimeout=max(10,GetDataIntXMLConfig("KingPeace\\Terminal\\NetTimeout"));
	g_Log.WriteLogEx(LOG_JOURNAL,"ͨѶ��ʱʱ��[%d]��",m_iNetTimeout);

	m_iTransactionQueueInterval=GetDataIntXMLConfig("KingPeace\\Terminal\\TransactionQueueInterval");
		m_iTransactionQueueInterval=max(m_iTransactionQueueInterval,30);//�����ڰ����
	m_iMonitorInterval=GetDataIntXMLConfig("KingPeace\\Terminal\\MonitorInterval");
		//m_iMonitorInterval=max(m_iMonitorInterval,300);//������5����
	m_iBoxMonitorInterval=GetDataIntXMLConfig("KingPeace\\Terminal\\BoxMonitorInterval");
		m_iBoxMonitorInterval=max(m_iBoxMonitorInterval,14400);//������4Сʱ
	m_iSelfCheckInterval=GetDataIntXMLConfig("KingPeace\\Terminal\\SelfCheckInterval");
		//m_iSelfCheckInterval=max(m_iSelfCheckInterval,600);//������10����
	m_iFlashHeartbeatInterval=GetDataIntXMLConfig("KingPeace\\Terminal\\FlashHeartbeatInterval");
		m_iFlashHeartbeatInterval=max(m_iFlashHeartbeatInterval,180);//������3����
	int iOpenFailMaxTimes=GetDataIntXMLConfig("KingPeace\\Terminal\\OpenFailMaxTime");
	int iEmptyFailMaxTimes=GetDataIntXMLConfig("KingPeace\\Terminal\\EmptyFailMaxTime");
	m_cbox.SetAllowErrorTimes(iOpenFailMaxTimes,iEmptyFailMaxTimes);
	CString csRebootTime = GetDataXMLConfig("KingPeace\\Terminal\\RebootTime");
	csRebootTime.IsEmpty()? m_iRebootTime=3 : m_iRebootTime=atoi(csRebootTime);

	g_Log.WriteLogEx(LOG_JOURNAL,"���׶��д�����[%d]��",m_iTransactionQueueInterval);
	g_Log.WriteLogEx(LOG_JOURNAL,"���״̬�������[%d]��",m_iBoxMonitorInterval);
	g_Log.WriteLogEx(LOG_JOURNAL,"�豸״̬�������[%d]��",m_iMonitorInterval);
	g_Log.WriteLogEx(LOG_JOURNAL,"�豸�Լ���[%d]��",m_iSelfCheckInterval);
	
	m_TermID=GetDataXMLConfig("KingPeace\\Terminal\\TerminalID");	
	m_BankID=GetDataXMLConfig("KingPeace\\Terminal\\BankID");
	m_iPort=GetDataIntXMLConfig("KingPeace\\Service\\SMS\\Port");
	m_IP=GetDataXMLConfig("KingPeace\\Service\\SMS\\IP");
	m_localIP=_GetLocalIP();
	m_localMAC=_GetLocalMAC().TrimRight(";");

	m_csExitPW=GetDataXMLConfig("KingPeace\\Management\\ExitPW");
	m_csManagePW=GetDataXMLConfig("KingPeace\\Management\\ManagePW");
	m_csTerminalKey=GetDataXMLConfig("KingPeace\\Terminal\\HostKey");
	memcpy(m_auHostKey,m_csTerminalKey,8);//�������� ���ַ���������Կ
	memcpy(m_auLocalKey,GetDataXMLConfig("KingPeace\\Terminal\\LocalKey"),8);
	//m_iSerial=GetDataIntXMLConfig("KingPeace\\Terminal\\SerialNumber");
	m_iNeedRegister=GetDataIntXMLConfig("KingPeace\\Terminal\\NeedRegister");
	CString csRequirementVersion = GetDataXMLConfig("KingPeace\\Terminal\\RequirementVersion");	
	if(!csRequirementVersion.IsEmpty())
		g_RequirementVersion = csRequirementVersion;
	g_XMLConfig.ResetPos();
	_GoIntoXML(g_XMLConfig,"KingPeace\\Management\\ManagePAN");
	while(g_XMLConfig.FindElem("PAN"))
	{		
		g_Log.WriteLogEx(LOG_TRACE,"����Ա����[%s]",g_XMLConfig.GetData());
	}

	if(bFail)
		return 0;
	else
		return VerifyConfig();
}
int CTranProDlg::VerifyConfig()
{
	if(m_TermID.IsEmpty())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ȡ[Terminal][TerminalID]����.");
		return 0;
	}
	if(m_BankID.IsEmpty())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ȡ[Terminal][BankID]����.");
		return 0;
	}
	if(m_IP.IsEmpty())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ȡ[Terminal][IP]����.");
		return 0;
	}

	return 1;
}
void CTranProDlg::OnBeginTran(CString csCommand,CString csParameter)
{
	//m_tranCode=csCommand;
	if(!csCommand.Left(8).Compare("ExecFunc"))
	{
		ExecFunc(csCommand,csParameter);		
	}
	else
	{
	}
}

/*/////////////////////////////

�������ͱ���:
ExecFunc_OpenDoor��ȡ������ʱ���� 
ExecFunc_ConfirmDeposit��Ͷ��

/////////////////////////////*/
void CTranProDlg::ExecFunc(CString csCommand,CString csParameter)
{
	if(!strcmp(csCommand,"ExecFunc_NetworkOnline"))  //flash����
	{
		m_bRecvFlashHeartbeat=true;
	}
	else if(!strcmp(csCommand,"ExecFunc_Init"))
	{
		Init();
	}
	else if(!strcmp(csCommand,"ExecFunc_PrintNote"))
	{//��ӡ
		TRSHOW("ExecFunc_PrintNote|00|��ӡ��ϣ���ȡƾ��");
	}
	else if(!strcmp(csCommand,"ExecFunc_Exit"))
	{//�˳�Ӧ��,�ڲ�ʹ��
		if(DesEncryption(_GetField(csParameter,1,'|'),m_auLocalKey)==m_csExitPW)
		{
			ExitSsmApp();		
		}

		else
			TRSHOW("ExecFunc_Exit|00|�˳��������");
	}
	else if(!strcmp(csCommand,"ExecFunc_Shutdown"))
	{//�رռ����
		g_Log.WriteLogEx(LOG_JOURNAL,"�ػ�");
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
		if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid))
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"��ȡ�ػ���Ȩʧ��[%d]",GetLastError());
			TRSHOW("ExecFunc_Shutdown|01|��ȡ�ػ���Ȩʧ��");
		}
		else
		{
			tkp.PrivilegeCount = 1;  // one privilege to set    
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
			AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
			if(!ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0))
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�ػ�ʧ��[%d]",GetLastError());
				TRSHOW("ExecFunc_Shutdown|01|�ػ�ʧ��");
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�ػ��ɹ�");
				TRSHOW("ExecFunc_Shutdown|00|���ڹػ�");
			}
		}		
	}
	else if(!strcmp(csCommand,"ExecFunc_Restart"))
	{//���������,�ڲ�ʹ��
		g_Log.WriteLogEx(LOG_JOURNAL,"����");
		HANDLE hToken; 
        TOKEN_PRIVILEGES tkp; 
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
		if(!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid))
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"��ȡ������Ȩʧ��[%d]",GetLastError());
			TRSHOW("ExecFunc_Restart|01|��ȡ������Ȩʧ��");
		}
		else
		{
			tkp.PrivilegeCount = 1;  // one privilege to set    
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
			if(!ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0))
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"����ʧ��[%d]",GetLastError());
				TRSHOW("ExecFunc_Restart|01|����ʧ��");
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�����ɹ�");
				TRSHOW("ExecFunc_Restart|00|��������");
			}
		}		
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckUpgradePackage"))
	{
		TRSHOW("ExecFunc_CheckUpgradePackage|00|�˹�����δʵ��");
	}
	else if(!strcmp(csCommand,"ExecFunc_CopyLogToRemovableDrive"))
	{
		//TRSHOW("ExecFunc_CopyLogToRemovableDrive|00|"); //������Ϣ
		TRSHOW("ExecFunc_CopyLogToRemovableDrive|01|�˹�����δʵ��");
	}
	else if(!strcmp(csCommand,"ExecFunc_MachineCondition"))
	{
		TRSHOW("ExecFunc_MachineCondition|00|%s",m_csProfile);
	}
	else if(!strcmp(csCommand,"ExecFunc_ManageCommands")) //��̨ά����¼
	{
		CString csPAN = _GetField(csParameter,1,'|');//�˺�
		bool bKeyCorrect=(DesEncryption(_GetField(csParameter,2,'|'),m_auLocalKey)==m_csManagePW);	
		bool bIsManagerPAN=false,bIsVenderPAN=false;
		
		g_XMLConfig.ResetPos();
		_GoIntoXML(g_XMLConfig,"KingPeace\\Management\\ManagePAN");
		while(_FindElemByAttribXML(g_XMLConfig,"PAN","",""))
		{
			if(g_XMLConfig.GetData()==csPAN) //�˺���ͬ
			{
				bIsManagerPAN=true;
				break;
			}
			g_XMLConfig.FindNode();
		}

		if(!bIsManagerPAN)
		{
			g_XMLConfig.ResetPos();
			_GoIntoXML(g_XMLConfig,"KingPeace\\Management\\VenderPAN");
			while(_FindElemByAttribXML(g_XMLConfig,"PAN","",""))
			{
				if(g_XMLConfig.GetData()==csPAN) //�˺���ͬ
				{
					bIsVenderPAN=true;
					break;
				}
				g_XMLConfig.FindNode();
			}
		}

		if(!bIsManagerPAN && !bIsVenderPAN)
		{
			TRSHOW("ExecFunc_ManageCommands|00|����Ա�˺Ų�����");
		}
		else if(!bKeyCorrect)
		{
			TRSHOW("ExecFunc_ManageCommands|00|����Ա�������");
		}
		else
		{
			m_PostLoginID=csPAN;
			if(bIsManagerPAN)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,">>>>����Ա[%s]��½<<<<",csPAN);
				TRSHOW("ExecFunc_ManageCommands|01|0|��½�ɹ�");			
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,">>>>ά����Ա[%s]��½<<<<",csPAN);
				TRSHOW("ExecFunc_ManageCommands|01|1|��½�ɹ�");				
			}
		}
	}
	else if(!strcmp(csCommand,"ExecFunc_AddManagerID"))
	{
		CString csPAN=csParameter;
		
		bool bPANExist=false;
		
		g_XMLConfig.ResetPos();
		_GoIntoXML(g_XMLConfig,"KingPeace\\Management\\ManagePAN");
		while(_FindElemByAttribXML(g_XMLConfig,"PAN","",""))
		{
			if(g_XMLConfig.GetData()==csPAN) //�˺���ͬ
			{
				bPANExist=true;
				break;
			}
			g_XMLConfig.FindNode();
		}
		if(bPANExist)
		{
			TRSHOW("ExecFunc_AddManagerID|00|�˹���Ա�Ѵ���");
		}
		else
		{
			g_XMLConfig.AddElem("PAN",csPAN);
			g_XMLConfig.Save(g_ConfigFile);
			TRSHOW("ExecFunc_AddManagerID|01|��ӹ���Ա�ɹ�");
		}
	}
	else if(!strcmp(csCommand ,"ExecFunc_PhoneLogin"))//�ֻ���¼������֤
	{
		m_csMobile =_GetField(csParameter ,1,'|');
		m_PostLoginPW=DesEncryption(_GetField(csParameter,2,'|'),m_auHostKey);//����
		PostmanPhoneLogin();		
	}
	else if(!strcmp(csCommand,"ExecFunc_PhoneLoginVerificationCode"))
	{	
		m_csVerifyCode=csParameter;
		//m_csVerifyCode=_GetField(csParameter ,1,'|');//���ģ�
		PhoneLoginVerificationCodeState();
	}
	else if(!strcmp(csCommand,"ExecFunc_GetVersion"))
	{
		TRSHOW("ExecFunc_GetVersion|01|%s_%s|%s",g_AppVersion,g_RequirementVersion,m_TermID);
	}
	else if(!strcmp(csCommand,"ExecFunc_ModifyTerminalParameter"))
	{
		m_TermID=_GetField(csParameter,2,'|');
		m_BankID=_GetField(csParameter,1,'|');
		SetDataXMLConfig("KingPeace\\Terminal\\TerminalID",m_TermID);
		SetDataXMLConfig("KingPeace\\Terminal\\BankID",m_BankID);
		
		TRSHOW("ExecFunc_ModifyTerminalParameter|�޸Ĳ����ɹ�");
	}
	else if(!strcmp(csCommand,"ExecFunc_ReadIDCard"))
	{
		m_bIsCancelReadIDCard=0;
		AfxBeginThread(__ReadIDCard,(LPVOID)this);
	}	
	else if(!strcmp(csCommand,"ExecFunc_CancelReadIDCard"))
	{
		m_bIsCancelReadIDCard=1;
	}
	else if(!strcmp(csCommand,"ExecFunc_Timeout"))
	{
		m_bIsCancelReadIDCard=1;
		m_bIsCancelReadICCard=1;
		m_bIsCancelBarCode=1;
	}
	else if(!strcmp(csCommand,"ExecFunc_Autorun"))
	{
		TRSHOW("%s|01|%s",csCommand,_RegSetValueString("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\ssmApp",g_AppPathName)?"���óɹ�":"����ʧ��");
	}
	else if(!strcmp(csCommand,"ExecFunc_CancelAutorun"))
	{
		TRSHOW("%s|01|%s",csCommand,_RegDeleteValue("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\ssmApp")?"ȡ���ɹ�":"ȡ��ʧ��");
	}
	else if(!strcmp(csCommand,"ExecFunc_ResetDevice"))
	{
		ResetAllDevices();
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckIDCardReader"))
	{
		if(!InitIDCardReader())
			TRSHOW("%s|02",csCommand);
		else
			TRSHOW("%s|01",csCommand);
	}
	else if(!strcmp(csCommand,"ExecFunc_PostmanLogIn"))
	{
		m_PostLoginID.Empty();
		m_PostLoginPW.Empty();
		m_PostLoginID=_GetField(csParameter,1,'|');
		m_PostLoginPW=DesEncryption(_GetField(csParameter,2,'|'),m_auHostKey);
		PostmanLoginState();
	}
	else if(!strcmp(csCommand,"ExecFunc_ReadICCard"))
	{
		m_bIsCancelReadICCard=0;
		//m_iState=STATE_BUSY;
		AfxBeginThread(__ReadICCard,(LPVOID)this);
	}
	else if(!strcmp(csCommand,"ExecFunc_CancelReadICCard"))
	{
		m_bIsCancelReadICCard=1;
	}
	else if(!strcmp(csCommand,"ExecFunc_GetCabinetStatus"))//
	{
		UpdateBoxStatus();
		CString tmp=_MakeString("ExecFunc_GetCabinetStatus|01|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d||||||||||",
			m_cbox.GetClassifyBoxCount(1,5),//��ʱ5����
			//m_cbox.GetClassifyBoxCount(1,4,3),//��ʱ�ϲ�4����
			//m_cbox.GetClassifyBoxCount(1,4,1),//��ʱ�²�4����
			m_cbox.GetClassifyBoxCount(1,4),//��ʱ4����
			m_cbox.GetClassifyBoxCount(1,3),//��ʱ3����
			m_cbox.GetClassifyBoxCount(1,2),//��ʱ2����
			m_cbox.GetClassifyBoxCount(1,1),//��ʱ1����
			m_cbox.GetClassifyBoxCount(0,5),//��Ͷ��5����
			//m_cbox.GetClassifyBoxCount(0,4,3),//��Ͷ���ϲ�4����
			//m_cbox.GetClassifyBoxCount(0,4,1),//��Ͷ���²�4����
			m_cbox.GetClassifyBoxCount(0,4),//��Ͷ���ϲ�4����
			m_cbox.GetClassifyBoxCount(0,3),//��Ͷ��3����
			m_cbox.GetClassifyBoxCount(0,2),//��Ͷ��2����
			m_cbox.GetClassifyBoxCount(0,1));//��Ͷ��1����
		
		for(int i=0;i<m_cbox.GetBoxCountTotal();++i)
			tmp.Append(_MakeString("|%d",m_cbox.GetStatus(i+1)));
		TRSHOW(tmp.GetBuffer());
	}
	else if(!strcmp(csCommand,"ExecFunc_OpenDoor"))
	{
		m_iWorkMode=atoi(_GetField(csParameter,1,'|'));
		OpenDoorState(atoi(_GetField(csParameter,2,'|')),_GetField(csParameter,3,'|'));
	}
	else if(!strcmp(csCommand,"ExecFunc_BarCode"))
	{	
		//�������ʾ�°�����ʼ ��ս�����Ϣ�����߳���ֹ �Ժ��п��ܸ��ĵط�����
		m_bIsCancelCloseDoor=1;
		m_iLogicalCurrentBox=-1;
		m_csCurrentBoxID.Empty(); 
		m_postmanID.Empty();
		m_pw.Empty();
		m_iBarCodeCount=0;
		m_barCode[0].Empty();	
		m_cIDCardData.ClearData();
		m_csMobile.Empty();

		m_bIsCancelBarCode=0;
		AfxBeginThread(__ReadBarCode,(LPVOID)this);
	}
	else if(!strcmp(csCommand,"ExecFunc_BarCodeCompleted"))
	{
		m_iBarCodeCount=1;
		CString csBarCode=_GetField(csParameter,1,'|');
		m_iDeadline = atoi(_GetField(csParameter,2,'|'));
		bool bFind = false;

		if(CheckMailNo(csBarCode))
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**����[%s]�Ѵ���",csBarCode);	
			TRSHOW("ExecFunc_BarCodeCompleted|00|������Ϣ�Ѵ��ڣ��޷��ظ�Ͷ��");		
		}
		else
		{
			csParameter.TrimRight('|');
			for(int i=0;i<m_iBarCodeCount;i++)
				m_barCode[i]=_GetField(csParameter,i+1,'|');			
			TRSHOW("ExecFunc_BarCodeCompleted|01");
		}
	}
	else if(!strcmp(csCommand,"ExecFunc_CancelBarCode"))
	{
		m_bIsCancelBarCode=1;
	}
	else if(!strcmp(csCommand,"ExecFunc_VerifyPackageMessage"))
	{	
		if(2==m_iVerifyType)
		{
			CString tmp=_GetField(csParameter,4,'|');
			if((4==tmp.GetLength()&&tmp==m_csMobile.Right(4))||
				(11==tmp.GetLength()&&tmp==m_csMobile))
			{
				TRSHOW("ExecFunc_VerifyPackageMessage|01|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
			}
			else
			{
				TRSHOW("ExecFunc_VerifyPackageMessage|00|�ֻ��Ų���ȷ,������");
			}
		}
		else if(4==m_iVerifyType)
		{//У�����֤����ĺϷ��� _GetField(csParameter,1,'|') �����ҹ�ʽ
			TRSHOW("ExecFunc_VerifyPackageMessage|01|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		}
		else
		{
			TRSHOW("ExecFunc_VerifyPackageMessage|01|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		}
	}
	else if(!strcmp(csCommand,"ExecFunc_CloseDoor"))
	{
		m_bIsCancelCloseDoor=0;
		AfxBeginThread(__CloseDoor,(LPVOID)this);
	}
	else if(!strcmp(csCommand,"ExecFunc_CancelCloseDoor"))
	{
		m_bIsCancelCloseDoor=1;
	}
	else if(!strcmp(csCommand,"ExecFunc_ConfirmDeposit")) 
	{//00ʧ�ܿ��� 01�ɹ� 02ʧ�ܲ�����
		ConfirmDepositState();		
	}
	else if(!strcmp(csCommand,"ExecFunc_GetPackageMessage")) //1.Ͷ��Ա��ѯ����������Աȡ��ʱ��ѯ����
	{
		DeliverGetPackageInfoState();
	}
	else if(!strcmp(csCommand,"ExecFunc_GetPackageMessageByAdmin")) //����Աȡ��ʱ��ѯ����
	{
		CString csPackageInfo;
		csPackageInfo = GetPackageInfoFromDB();

		if(!csPackageInfo.IsEmpty())
			csPackageInfo.Insert(0,"ExecFunc_GetPackageMessageByAdmin|01|");
		else
			csPackageInfo = "ExecFunc_GetPackageMessageByAdmin|00|�ް�����Ϣ";	

		TRSHOW2(csPackageInfo);	
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckPhoneMessageInfo"))//2.�ͻ���ѯ����
	{	
		m_csMobile = _GetField(csParameter,1,'|');
		CString csPackageInfo = CheckPackagePhoneInfoFromDB();
		if(csPackageInfo.IsEmpty())
			csPackageInfo = "ExecFunc_CheckPhoneMessageInfo|01|�ް�����Ϣ";
		else
			csPackageInfo.Insert(0,"ExecFunc_CheckPhoneMessageInfo|01");

		TRSHOW2(csPackageInfo);
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckStatus"))//�������ʱ������
	{
		int iStatus[64]={-1};
		int iRet,iCurrentCabinet=0;
		bool bFind;
		int iEmptyCount=0,iNotEmptyCount=0;
		CString data,csNotEmpty;
		std::vector<PackageInfo> vstPackageInfo;
		std::vector<PackageInfo>::iterator iter;

		LoadPackageFromDB(vstPackageInfo);//һ���Ի�ȡ����Ͷ�����ݣ����ⷴ����ѯ���µȴ�ʱ�䳤

		for(int i=0;i<m_cbox.GetBoxCountTotal();i++)
		{	
			bFind = false;
			for(iter=vstPackageInfo.begin();iter!=vstPackageInfo.end();iter++)
			{
				if(i+1 == atoi(iter->BoxNo))
				{
					bFind = true;
					break;
				}		
			}
			if(!bFind)//���ݿ���û�и����Ͷ������
			{	
				++iEmptyCount;
				data+=_MakeString("%s^",m_cbox.GetBoxName(i+1));

				int iPhysicalCabinet=m_cbox.GetPhysicalCabinet(i+1);
				int iPhysicalBox=m_cbox.GetPhysicalBox(i+1);
				if(iCurrentCabinet!=iPhysicalCabinet) //����ÿ���䶼ȥ�����ڣ����µȴ�ʱ�䳤
				{
					iRet=m_lockerDll.Status(iPhysicalCabinet,0,iStatus);	
					iCurrentCabinet=iPhysicalCabinet;
				}
				if(iStatus[iPhysicalBox+31])
				{
					++iNotEmptyCount;
					csNotEmpty+=_MakeString("%s^",m_cbox.GetBoxName(i+1));				
				}
			}		
		}	
		data.TrimRight('^');
		csNotEmpty.TrimRight('^');
		TRSHOW("%s|01|%d|%d|%s|%s",csCommand,iEmptyCount,iNotEmptyCount,data,csNotEmpty);	
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckPackageInfo"))//3.Ͷ��ʱ��ѯ������Ϣ����ȡ�ͻ��ֻ��ţ�
	{
		CheckPackageInfoState();
	}
	else if(!strcmp(csCommand,"ExecFunc_ModifyPackageInfo"))
	{
		m_csMobile=csParameter;
		TRSHOW("ExecFunc_ModifyPackageInfo|01|�޸ĳɹ�");
		/*ֱ�Ӽ�¼���� Ͷ�ݽ����﷢��ȥ*/
	}
	else if(!strcmp(csCommand,"ExecFunc_WorkMode"))
	{
		m_iWorkMode=atoi(csParameter);
	}
	else if(!strcmp(csCommand,"ExecFunc_Idle"))
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"���н���");
		EmptyTransMsg();		
		m_cbox.SetBoxHide();//�������ģʽ���ص���
		m_iWorkMode=0;
		m_iState=STATE_IDLE;
	}
	else if(!strcmp(csCommand,"ExecFunc_StartHandWrite"))
	{
		HandWriteStartUp(this->GetSafeHwnd());
	}
	else if(!strcmp(csCommand,"ExecFunc_StopHandWrite")) 
	{
		HandWriteStop();
	}
	else if(!strcmp(csCommand,"ExecFunc_MainMenu"))
	{
		m_iState=STATE_MAINMENU;
	}
	else if(!strcmp(csCommand,"ExecFunc_Busy"))
	{
		m_iState=STATE_BUSY;
	}
	else if(!strcmp(csCommand,"ExecFunc_Maintenance"))
	{
		m_iState=STATE_MAINTENANCE;
	}
	else if(!strcmp(csCommand,"ExecFunc_Pause"))
	{
		m_iState=STATE_PAUSE;
	}
	else if(!strcmp(csCommand,"ExecFunc_VerificationCode"))
	{
		m_csMobile=csParameter;
		VerificationCodeState();
	}
	//else if(!strcmp(csCommand,"ExecFunc_StartKeyboard"))
	//{
	//	m_bIsKeyBoardPause=false;
	//}
	//else if(!strcmp(csCommand,"ExecFunc_StopKeyboard"))
	//{
	//	m_bIsKeyBoardPause=true;
	//}
	else if(!strcmp(csCommand,"ExecFunc_CheckPackagePW"))
	{
	   //1�����룬��֤ȡ��
	   //2���ֻ�����+���룬��֤ȡ��
	   //3������+���룬��֤ȡ��
	   //4�����֤����+���룬��֤ȡ��
	   //5������+���ζ�����֤�룬��֤ȡ��

		m_pw=DesEncryption(csParameter,m_auHostKey);
		if(LoadPackageByPWFromDB(m_iLogicalCurrentBox,m_pw,m_iBarCodeCount,m_barCode,m_postmanID,m_iVerifyType,m_csMobile,m_csExtraData))
		{
			m_csCurrentBoxID=m_cbox.GetBoxName(m_iLogicalCurrentBox); 
			TRSHOW("%s|01|%d",csCommand,m_iVerifyType);  
		}
		else
			TRSHOW("%s|00|�������",csCommand);
	}
	else if(!strcmp(csCommand,"ExecFunc_InitTerminal"))
	{
		int currentOption=GetDataIntXMLConfig("KingPeace\\Box\\CurrentOption");
		CString csBoxName = GetDataXMLConfig(_MakeString("KingPeace\\Box\\Option%d\\BoxName",currentOption));
		if(csBoxName.IsEmpty())
		{
			TRSHOW("ExecFunc_InitTerminal|00|���÷�������,δ���ø������");		
		}
		else
		{
			m_csTerminalInitKey = _GetField(csParameter,1,'|');
			m_tranCode="600018";
			CString send=Pack();
			CString recv,csReturnMsg;
			int iReturnCode;

			if(Transaction(send,recv,iReturnCode,csReturnMsg))
			{//fail
				g_Log.WriteLogEx(LOG_JOURNAL,"**�ն˳�ʼ��ʧ��[%d][%s]",iReturnCode,csReturnMsg);
				TRSHOW("ExecFunc_InitTerminal|00|�ն˳�ʼ��ʧ��");
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�ն˳�ʼ���ɹ�");
				SetDataXMLConfig("KingPeace\\Terminal\\NeedRegister","0");
				TRSHOW("ExecFunc_InitTerminal|01|�ն˳�ʼ���ɹ�");
			}
		}
	}
	else if(!strcmp(csCommand,"ExecFunc_GetBreakdownBoxInfo"))
	{
		int iStatus[64]={-1};
		int iRet,iCurrentCabinet=0;
		bool bOpenFail,bEmptyFail,bHavePackage;
		bool bFind;
		CString data;
		std::vector<PackageInfo> vstPackageInfo;
		std::vector<PackageInfo>::iterator iter;

		LoadPackageFromDB(vstPackageInfo);//һ���Ի�ȡ����Ͷ�����ݣ����ⷴ����ѯ���µȴ�ʱ�䳤

		for(int i=0;i<m_cbox.GetBoxCountTotal();i++)
		{	
			bFind = false;
			for(iter=vstPackageInfo.begin();iter!=vstPackageInfo.end();iter++)
			{
				if(i+1 == atoi(iter->BoxNo))
				{
					bFind = true;
					break;
				}		
			}
			if(!bFind)//���ݿ���û�и����Ͷ������
			{					
				int iPhysicalCabinet=m_cbox.GetPhysicalCabinet(i+1);
				int iPhysicalBox=m_cbox.GetPhysicalBox(i+1);
				if(iCurrentCabinet!=iPhysicalCabinet) //����ÿ���䶼ȥ�����ڣ����µȴ�ʱ�䳤
				{
					iRet=m_lockerDll.Status(iPhysicalCabinet,0,iStatus);	
					iCurrentCabinet=iPhysicalCabinet;
				}
				iStatus[iPhysicalBox+31]? bHavePackage=true : bHavePackage=false;
				m_cbox.GetAllowOpenFailTimes()<=m_cbox.GetOpenFail(i+1) ? bOpenFail=true : bOpenFail=false;
				m_cbox.GetAllowEmptyFailTimes()<=m_cbox.GetNotEmptyFail(i+1) ? bEmptyFail=true : bEmptyFail=false;
				//IsEmptyLogicalBox(i+1) ? bHavePackage=true : bHavePackage=false;
				if(bOpenFail || bEmptyFail || bHavePackage)
				{
					data+=_MakeString("|%s",m_cbox.GetBoxName(i+1));
					bOpenFail ? data+="^   ����" : data+="^   ����";
					bEmptyFail ? data+="^   ����" : data+="^   ����";
					bHavePackage ? data+="^        ��" : data+="^        ��";
				}	
			}		
		}		

		if(data.IsEmpty())
			TRSHOW("%s|00|�޹��ϸ����Ϣ",csCommand);
		else
			TRSHOW("%s|01%s",csCommand,data);
	}
	else if(!strcmp(csCommand,"ExecFunc_FixBox"))
	{
		m_cbox.SetNotEmptyFail();
		m_cbox.SetOpenFail();
		TRSHOW("ExecFunc_FixBox|01|���������״̬�ɹ�");
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckPhoneMessageInfo"))
	{
		TRSHOW("%s|00|������δʵ��",csCommand);
	}
	else if(!strcmp(csCommand,"ExecFunc_CheckOpenedDoor"))
	{
		m_bIsCancelCheckOpenDoor = false;
		AfxBeginThread(__CheckOpenDoor,(LPVOID)this);	
	}
	else if(!strcmp(csCommand,"ExecFunc_CancelCheckOpenedDoor"))
	{
		m_bIsCancelCheckOpenDoor = true;	
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"�޶�Ӧ����[%s]",csCommand);
		TRSHOW("%s|01|�޶�Ӧ����",csCommand);
	}
}

BOOL CTranProDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	int cInks = pCopyDataStruct->dwData;
	
	if(cInks>0)
	{
		char pszLastHanZi[32]={0};		
		for(int i=0;i<cInks;i++)
		{
			pszLastHanZi[i*3]=((WORD*)(pCopyDataStruct->lpData))[i]&0x00FF;
			pszLastHanZi[i*3+1]=((WORD*)(pCopyDataStruct->lpData))[i]>>8&0x00FF;
			pszLastHanZi[i*3+2]='|';
		}
		TRSHOW(_MakeString("HandWrite|%s",pszLastHanZi).GetBuffer());		
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CTranProDlg::ExitSsmApp(void)
{
	CLOSEAPP("EXITAPP");
}

void CTranProDlg::OnBnClickedOk()
{
	;
}
void CTranProDlg::OnBnClickedCancel()
{
	;
}
void CTranProDlg::OnDestroy()
{
	CDialog::OnDestroy();
}
void CTranProDlg::EmptyTransMsg()
{
	m_pw.Empty();//ȡ������ ����
	m_PostLoginID.Empty();		//Ͷ��Ա��¼ID
	m_PostLoginPW.Empty();		//Ͷ��Ա��½����
	m_compShortName.Empty();//������˾��
	m_deliveryName.Empty();//Ͷ��Ա����
	m_mailStatus.Empty();	//�ʼ�״̬��
	//m_tranCode.Empty();	
	m_csVerifyCode.Empty();
	m_postmanID.Empty();//Ͷ����Ա�ı��
	//m_barCode[0].Empty();
	m_csMobile.Empty();
	m_cIDCardData.ClearData();
	m_iBarCodeCount=0;
	m_iLogicalCurrentBox = -1;//��ǰ���ڲ��������ŵ��߼���� ��1��ʼ
	m_iCurrentOpenBox = -1;
	m_iVerifyType = 2;
	m_csExtraData.Empty();  
	m_vPackageInfo.clear();
}
CString CTranProDlg::BoxSpecChinese(int spec)
{
	if(1==spec)
		return "������";
	if(2==spec)
		return "С��";
	if(3==spec)
		return "����";
	if(4==spec)
		return "�²����";
	if(5==spec)
		return "�ϲ����";
	if(6==spec)
		return "������";
	
	return "δ֪������";
}
bool CTranProDlg::CheckNetWork()
{
	bool	bRet = false;
	int		iClientSocketId = 0;
	struct	sockaddr_in		Remote;

	Remote.sin_family = AF_INET;
 	Remote.sin_port   = htons((unsigned short)m_iPort);
 	Remote.sin_addr.s_addr = inet_addr(m_IP);
	
	iClientSocketId = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		
	if(iClientSocketId > 0)
	{		
		int iReturn=connect(iClientSocketId,(struct sockaddr *)&Remote,sizeof(Remote));		
		if(iReturn != SOCKET_ERROR)
			bRet = true;
	}

	closesocket(iClientSocketId);
	
	return bRet;
}
CString CTranProDlg::XDEncryption(CString InData,BYTE* key)
{
	char szPW[17]={0};
	BYTE in[8]={0};
	BYTE out[8]={0};
	memcpy(in,InData,min(InData.GetLength(),8));
	Des3(out,in,8,key,8,DES_ENCRYPT);
	BCD2ASCII(szPW,out,8);

	g_Log.WriteLogEx(LOG_TRACE,"����[%s]->[%s]",InData,szPW);
	return szPW;
}
CString CTranProDlg::DesEncryption(CString InData,BYTE* key)
{
	char szPW[17]={0};
	BYTE InKey[8]={0};
	BYTE OutKey[8]={0};
	
	memcpy(InKey,InData.GetBuffer(),min(InData.GetLength(),8));
	if(InData.GetLength()<8)
		memset(InKey+InData.GetLength(),8-InData.GetLength(),sizeof(BYTE)*(8-InData.GetLength()));  //��̨java���õ�DES����ģʽ��ECB/PKCS5Padding
	Des3(OutKey,InKey,8,key,8,DES_ENCRYPT);
	BCD2ASCII(szPW,OutKey,8);

	g_Log.WriteLogEx(LOG_TRACE,"����[%s]->[%s]",InData,szPW);
	return szPW;
}
CString CTranProDlg::DesDecryption(CString InData,BYTE* key)
{
	char szPW[17]={0};
	BYTE InKey[8]={0};
	BYTE OutKey[8]={0};

	if(InData.GetLength()<sizeof(szPW))
	{
		strcpy(szPW,InData);
		ASCII2BCD(InKey,szPW,16);
		Des3(OutKey,InKey,8,key,8,DES_DECRYPT);
		OutKey[6]=0;
	}
	
	g_Log.WriteLogEx(LOG_TRACE,"����[%s]->[%s]",InData,OutKey);
	return OutKey;
}
CString CTranProDlg::GetSerialNOString()
{
	return _MakeString("%06d",m_iSerial);
}
void CTranProDlg::FreshSerialNO()
{
	if(m_iSerial<999999)
	{
		//_SetDataXML("KingPeace\\Terminal\\SerialNumber",_MakeString("%06d",++m_iSerial),g_ConfigFile);
		++m_iSerial;
	}
	else
	{
		m_iSerial=1;
		//_SetDataXML("KingPeace\\Terminal\\SerialNumber","000001",g_ConfigFile);
	}
}
int CTranProDlg::GetDataIntXMLConfig(CString tree)
{
	return atoi(GetDataXMLConfig(tree));
}
CString CTranProDlg::GetDataXMLConfig(CString tree)
{
	return _GetDataXMLCM(tree,g_XMLConfig);	
}
void CTranProDlg::SetDataXMLConfig(CString tree,CString value)
{
	_SetDataXMLCM(tree,value,g_XMLConfig);
	g_XMLConfig.Save(g_ConfigFile);
}
void CTranProDlg::DeleteDataXMLConfig(CString tree)
{
	_DeleteDataXMLCM(tree,g_XMLConfig);
	g_XMLConfig.Save(g_ConfigFile);
}
BOOL CTranProDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CDialog::PreTranslateMessage(pMsg);
}
CString CTranProDlg::CreatePW()
{
	CString pw;	
	srand(GetTickCount());
	int k=rand()*12345+67890;
	pw.Format("%06d",k);	
	pw.Delete(0,pw.GetLength()-6);
	return pw;
}
CString CTranProDlg::CreatePickupPW()
{
	return DesEncryption(CreatePW(),m_auHostKey);
}
void CTranProDlg::SleepDebug(DWORD dwMilliseconds)
{
#ifdef _DEBUG
	Sleep(dwMilliseconds);
#endif
} 
void CTranProDlg::SleepRelease(DWORD dwMilliseconds)
{
#ifndef _DEBUG
	Sleep(dwMilliseconds);
#endif
}
void CTranProDlg::InitAllDevices()
{
	for(int i=DEVICE_START;i<=DEVICE_END;i++)
	{
		int a=m_stHardWare[i].iExist; 
		g_Log.WriteLogEx(LOG_JOURNAL,"װ��[%d],װ������[%d],װ������[%d],װ������[%s]",i,m_stHardWare[i].iExist,m_stHardWare[i].iLevel,m_stHardWare[i].csName);
		if(!m_stHardWare[i].iExist)
		{
			if(LEVEL_OPTION==m_stHardWare[i].iLevel)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"��װ��%s[%d]",m_stHardWare[i].csName,m_stHardWare[i].iExist);
				TRSHOW("ExecFunc_InitMsg|01|��װ��%s",m_stHardWare[i].csName);
				SleepRelease(1000);
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�ؼ��豸��װ��%s",m_stHardWare[i].csName);
				TRSHOW("ExecFunc_InitMsg|01|�ؼ��豸�쳣\r\n��װ��%s",m_stHardWare[i].csName);
				Loop();
			}
			continue;
		}
		while(true)
		{
			TRSHOW("ExecFunc_InitMsg|01|��ʼ��%s",m_stHardWare[i].csName);
			if(InitDevice(i))
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"%s����",m_stHardWare[i].csName);
				break;
			}
			else
			{
				if(LEVEL_OPTION==m_stHardWare[i].iLevel)
				{
					TRSHOW("ExecFunc_InitMsg|01|%s����",m_stHardWare[i].csName);
					SleepRelease(1000);
					break;
				}
				else
				{
					for(int j=m_iSelfCheckInterval;j>0;j--)
					{
						TRSHOW("ExecFunc_InitMsg|01|�ؼ��豸�쳣\r\n%s����\r\n%d�������",m_stHardWare[i].csName,j);
						Sleep(1000);
					}
				}
			}
		}
	}
}
void CTranProDlg::ResetAllDevices()
{
	int nonexistent=0;
	int normal=0;
	int abnormal=0;
	CString csNormal;
	CString csNoEquiped;
	CString csError;
	for(int i=DEVICE_START;i<=DEVICE_END;i++)
	{
		if(!m_stHardWare[i].iExist)
		{
			nonexistent++;
			//g_Log.WriteLogEx(LOG_JOURNAL,"��װ��%s",m_stHardWare[i].csName);
			//TRSHOW("ExecFunc_ResetDevice|00|��װ��%s",m_stHardWare[i].csName);
			csNoEquiped += _MakeString("%s��װ����\n",m_stHardWare[i].csName); 
			SleepRelease(1000);
			continue;
		}
		else
		{
			//TRSHOW("ExecFunc_ResetDevice|00|��ʼ��%s",m_stHardWare[i].csName);
			if(InitDevice(i))
			{
				normal++;
				//g_Log.WriteLogEx(LOG_JOURNAL,"%s����",m_stHardWare[i].csName);
				//TRSHOW("ExecFunc_ResetDevice|00|%s����",m_stHardWare[i].csName);
				csNormal += _MakeString("%s������\n",m_stHardWare[i].csName); 
				SleepRelease(1000);
			}
			else
			{
				abnormal++;
				//g_Log.WriteLogEx(LOG_JOURNAL,"%s����",m_stHardWare[i].csName);
				//TRSHOW("ExecFunc_ResetDevice|00|%s����",m_stHardWare[i].csName);
				csError += _MakeString("%s���ϡ�\n",m_stHardWare[i].csName); 
				SleepRelease(1000);
			}
		}
	}	
	g_Log.WriteLogEx(LOG_JOURNAL,"%s%s%s",csNoEquiped,csError,csNormal);
	TRSHOW("ExecFunc_ResetDevice|01|��λ��ϡ�\r\n%s%s%s",csNoEquiped,csError,csNormal);
	//g_Log.WriteLogEx(LOG_JOURNAL,"%d���豸%d���豸��װ��,%d���豸������%d���豸�쳣��",nonexistent,normal,abnormal);
	//TRSHOW("ExecFunc_ResetDevice|01|��λ��ϡ�\r\n%d���豸%d���豸��װ��,%d���豸������%d���豸�쳣��",nonexistent,normal,abnormal);
}
int CTranProDlg::InitDevice(int deivce)
{
	if(DEVICE_LOCKER==deivce)
		return InitLocker();
	if(DEVICE_IDCR==deivce)
		return InitIDCardReader();
	if(DEVICE_BCR==deivce)
		return InitBarcodeReader();
	if(DEVICE_PIN==deivce)
		return InitPIN();
	if(DEVICE_DVR==deivce)
		return InitDVR();
	return 0;
}
int CTranProDlg::InitLocker()
{
	m_lockerDll.InitComm(m_stHardWare[DEVICE_LOCKER].iPort);

	int iCabinetCount=0;
	int iCabinetID[20]={0};
	m_stHardWare[DEVICE_LOCKER].iStatus=m_lockerDll.Reset(iCabinetCount,iCabinetID);

	if(!iCabinetCount)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**���ư��ȡ������ʧ��");
		return 0;
	}
	if(iCabinetCount>m_cbox.GetCabinetCount())//add
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��װ��[%d]��,����[%d]��,��һ��",iCabinetCount,m_cbox.GetCabinetCount());
		return 0;	
	}
	if(iCabinetCount<=0||iCabinetCount>20)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��װ��[%d]��,��������.",m_cbox.GetCabinetCount());
		return 0;		
	}
	if(iCabinetCount!=m_cbox.GetCabinetCount())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"���ư巵��[%d]���������ļ�[%d]����.���߲�һ�¡�",iCabinetCount,m_cbox.GetCabinetCount());
		//m_cbox.SetCabinetCount(iCabinetCount);
	}
	for(int i=0;i<iCabinetCount;++i)
	{
		if(iCabinetID[i]<=m_cbox.GetCabinetCount())//�����źϷ�ʱ����
			m_cbox.SetCabinetID(iCabinetID[i],iCabinetID[i]);	
	}

	if(m_stHardWare[DEVICE_LOCKER].iStatus)
	{		
		g_Log.WriteLogEx(LOG_JOURNAL,"���������ع���[%d].",m_stHardWare[DEVICE_LOCKER].iStatus);
		return 0;
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"װ������[%d]��",m_cbox.GetCabinetCount());		
		UpdateBoxStatus();
	}
	for(int i=0;i<m_cbox.GetBoxCountTotal();i++)  //��ʼ����Ӳ��״̬
	{
		m_vBoxDoorStatus[i+1]=0;
	}
	return 1;
}
int CTranProDlg::InitIDCardReader()
{
	CString csType;
	1==m_stHardWare[DEVICE_IDCR].iType ? csType="GTICR100":(2==m_stHardWare[DEVICE_IDCR].iType?csType="iDR210":csType="δ֪����");
	g_Log.WriteLogEx(LOG_JOURNAL,"%s����[%s]",m_stHardWare[DEVICE_IDCR].csName,csType);

	m_oIDCardReader.SetDeviceType(m_stHardWare[DEVICE_IDCR].iType);
	m_oIDCardReader.SetPort(m_stHardWare[DEVICE_IDCR].iPort);		
	m_stHardWare[DEVICE_IDCR].iStatus=m_oIDCardReader.InitComm();
	if(m_stHardWare[DEVICE_IDCR].iStatus)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"���֤����������[%d].",m_stHardWare[DEVICE_IDCR].iStatus);
		return 0;
	}
	return 1;
}
int CTranProDlg::InitBarcodeReader()
{
	CString csType;
	1==m_stHardWare[DEVICE_BCR].iType ? csType="LV1000R":(2==m_stHardWare[DEVICE_BCR].iType?csType="LV420R":csType="δ֪����");
	g_Log.WriteLogEx(LOG_JOURNAL,"%s����[%s]",m_stHardWare[DEVICE_BCR].csName,csType);

	int iRet = m_CodeBarScaner.SetDeviceType(m_stHardWare[DEVICE_BCR].iType);
	iRet = m_CodeBarScaner.InitCom(m_stHardWare[DEVICE_BCR].iPort);
	iRet = m_stHardWare[DEVICE_BCR].iStatus=m_CodeBarScaner.SetWorkMode(6);
	if(m_stHardWare[DEVICE_BCR].iStatus)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"����ɨ���ǹ���[%d].",m_stHardWare[DEVICE_BCR].iStatus);
		return 0;
	}
	return 1;
}
int CTranProDlg::InitPIN()
{
	m_pincom.InitComm(m_stHardWare[DEVICE_PIN].iPort,CBR_9600,8,NOPARITY,ONESTOPBIT);
	m_stHardWare[DEVICE_PIN].iStatus=!m_pincom.IsInit();
	if(m_stHardWare[DEVICE_PIN].iStatus)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"%s����[%d].",m_stHardWare[DEVICE_PIN].csName,m_stHardWare[DEVICE_PIN].iStatus);
		return 0;
	}
	if(!m_bIsRunningKeyboard)	
		AfxBeginThread(__ReadKeyBoard,(LPVOID)this);	

	return 1;
}
int CTranProDlg::InitDVR()
{
	m_stHardWare[DEVICE_DVR].iStatus = 2;
	if(NET_DVR_Init()) 
	{
		LPNET_DVR_DEVICEINFO_V30 pstDVR_DevInfo = new NET_DVR_DEVICEINFO_V30;
		ZERO(pstDVR_DevInfo)
		//m_lDVR_UserID = NET_DVR_Login_V30(m_stHardWare[DEVICE_DVR].csIP.GetBuffer(),m_stHardWare[DEVICE_DVR].iPort,_T(m_stHardWare[DEVICE_DVR].csName.GetBuffer()),_T(m_stHardWare[DEVICE_DVR].csPW.GetBuffer()),pstDVR_DevInfo);
		m_lDVR_UserID = NET_DVR_Login_V30(m_stHardWare[DEVICE_DVR].csIP.GetBuffer(),m_stHardWare[DEVICE_DVR].iPort,_T("admin"),_T("12345"),pstDVR_DevInfo);

		if(m_lDVR_UserID!=-1)
		{
			if(NET_DVR_GetDVRWorkState_V30(m_lDVR_UserID,&m_stDVR_StateInfo))//��ȡ�豸״̬
				m_stHardWare[DEVICE_DVR].iStatus = m_stDVR_StateInfo.dwDeviceStatic;
		}
		delete pstDVR_DevInfo;
	}

	if(m_stHardWare[DEVICE_DVR].iStatus)
	{
		long lErrID = NET_DVR_GetLastError();
		g_Log.WriteLogEx(LOG_JOURNAL,"%s����[%ld][%s].",m_stHardWare[DEVICE_DVR].csName,lErrID,NET_DVR_GetErrorMsg(&lErrID));	
		return 0;
	}

	return 1;
}
void CTranProDlg::UpdateStatusDevice(int deivce)
{
	if(DEVICE_LOCKER==deivce)
		UpdateStatusLocker();
	if(DEVICE_IDCR==deivce)
		UpdateStatusIDCardReader();
	if(DEVICE_BCR==deivce)
		UpdateStatusBarcodeReader();
	if(DEVICE_PIN==deivce)
		UpdateStatusPIN();
	if(DEVICE_DVR==deivce)
		UpdateStatusDVR();
}
void CTranProDlg::UpdateBoxStatus()
{
	if(m_iIsUpdateingBoxStatus)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"���ڸ�����״̬");		
	}
	else
	{
		m_iIsUpdateingBoxStatus=true;
		UpdateStatusLocker();
		UpdateLocalPackageStatus();
		m_cbox.UpdateStatus();
		m_cbox.PrintAllStatus();
		m_iIsUpdateingBoxStatus=false;
	}
}

bool CTranProDlg::OpenLogicalBox(int iLogicalBoxID)
{
	if(!iLogicalBoxID)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**���[%d]�Ƿ�",iLogicalBoxID);
		return false;
	}
	
	int iPhysicalCabinet=m_cbox.GetPhysicalCabinet(iLogicalBoxID);
	int iPhysicalBox=m_cbox.GetPhysicalBox(iLogicalBoxID);
	int iStatus[64];
	g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]׼������,�����[%d],������[%d]",iLogicalBoxID,iPhysicalCabinet,iPhysicalBox);
	int iRet=m_lockerDll.OpenDoor(iPhysicalCabinet,iPhysicalBox,iStatus);
	if(ERR_OK!=iRet)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**����ʧ��");
		return false;
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"����ɹ�");
		return true;
	}
}
bool CTranProDlg::CloseLogicalBox(int iLogicalBoxID,int& iIsPutPackage)
{
	if(!iLogicalBoxID)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"��ŷǷ���");
		return false;
	}
	
	int iPhysicalCabinet=m_cbox.GetPhysicalCabinet(iLogicalBoxID);
	int iPhysicalBox=m_cbox.GetPhysicalBox(iLogicalBoxID);
	int iStatus[64]={-1};
	g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]������,�����[%d],������[%d]",iLogicalBoxID,iPhysicalCabinet,iPhysicalBox);
	int iRet=m_lockerDll.Status(iPhysicalCabinet,iPhysicalBox,iStatus);
	if(ERR_OK!=iRet)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"ȡ��״̬ʧ��,�����ſ�1");
	}
	else
	{
		if(iStatus[1])
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]�л�",iLogicalBoxID);
			iIsPutPackage=1;
		} 
		else
			iIsPutPackage=0;

		if(iStatus[0])
		{//һ�μ���Ź� ��ʱ�ټ�� ��ֹ�ŵ���
			Sleep(500);
			iRet=m_lockerDll.Status(iPhysicalCabinet,iPhysicalBox,iStatus);
			if(ERR_OK!=iRet)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"ȡ��״̬ʧ��,�����ſ�2");
			}
			else
			{
				if(iStatus[0])
				{//�����Ź�
					g_Log.WriteLogEx(LOG_JOURNAL,"���ųɹ�");
					return true;
				}
				else
				{
					//g_Log.WriteLogEx(LOG_JOURNAL,"�ſ�");
				}
			}
		}
		else
		{
			//g_Log.WriteLogEx(LOG_JOURNAL,"�ſ�");
		}
	}
	return false;
}
bool CTranProDlg::IsEmptyLogicalBox(int iLogicalBoxID)
{
	if(!iLogicalBoxID)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"�߼����[%d]�Ƿ���",iLogicalBoxID);
		return false;
	}
	
	int iPhysicalCabinet=m_cbox.GetPhysicalCabinet(iLogicalBoxID);
	int iPhysicalBox=m_cbox.GetPhysicalBox(iLogicalBoxID);
	int iStatus[64]={-1};
	g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]������,�����[%d],������[%d]",iLogicalBoxID,iPhysicalCabinet,iPhysicalBox);
	int iRet=m_lockerDll.Status(iPhysicalCabinet,iPhysicalBox,iStatus);
	if(ERR_OK!=iRet)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"ȡ��״̬ʧ��,�����л�");
		return true;
	}
	else
	{
		if(iStatus[1])
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]�л�",iLogicalBoxID);
			return true;
		} 
	}
	g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]��",iLogicalBoxID);
	return false;
}
CString CTranProDlg::TransformBoxNoToBoxID(int iBoxNo)
{
	CString csBoxID;
	int iPhysicalCabinet=m_cbox.GetPhysicalCabinet(iBoxNo);
	int iPhysicalBox=m_cbox.GetPhysicalBox(iBoxNo);	

	if(0<iPhysicalCabinet && 27>iPhysicalCabinet)//26����ĸ,�������26����
	{
		csBoxID=_MakeString("%s%d",iPhysicalCabinet+64,iPhysicalBox);	
	}
	return csBoxID;
}
int CTranProDlg::TransformBoxIDToBoxNo(CString csBoxID)
{
	int iBoxNo;
	char szBoxID[10]={0};
	memcpy(szBoxID,csBoxID,min(csBoxID.GetLength(),10));
	int iPhysicalCabinet = szBoxID[0]-64;
	int iPhysicalBox = atoi(szBoxID);	

	if(iPhysicalCabinet>1)
		iBoxNo = m_cbox.GetBoxCount(iPhysicalCabinet-1) + iPhysicalBox;
	else
		iBoxNo = iPhysicalBox;

	return iBoxNo;
}
void CTranProDlg::UpdateStatusLocker()
{
	if(m_iIsUpdateingStatusLocker)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"���ڸ����䴫����״̬");		
	}
	else
	{
		m_iIsUpdateingStatusLocker=true;
		if(m_stHardWare[DEVICE_LOCKER].iExist)
		{
			m_stHardWare[DEVICE_LOCKER].iStatus = 0; //Ĭ��Ϊ�豸����

			int iStatus[64]={-1};
			int iRet;
			for(int i=0;i<m_cbox.GetCabinetCount();i++)
			{
				iRet=m_lockerDll.Status(m_cbox.GetCabinetID(i+1),0,iStatus);
				if(ERR_OK!=iRet)
				{//abnormal
					m_stHardWare[DEVICE_LOCKER].iStatus=1;
					break;
				}
				else
				{
					for(int j=0;j<m_cbox.GetBoxCount(i+1);++j)
					{
						m_cbox.SetBoxHardwareClose(m_cbox.GetLogicalBoxByPhysicalCabinetNO(i,j+1),iStatus[j]);
						m_cbox.SetBoxHardwareNotEmpty(m_cbox.GetLogicalBoxByPhysicalCabinetNO(i,j+1),iStatus[j+32]);
					}
				}
			}
		}
		m_iIsUpdateingStatusLocker=false;
	}
}
void CTranProDlg::UpdateStatusIDCardReader()
{
	if(m_stHardWare[DEVICE_IDCR].iExist)
	{
		m_stHardWare[DEVICE_IDCR].iStatus=0;
	}
}
void CTranProDlg::UpdateStatusBarcodeReader()
{
	if(m_stHardWare[DEVICE_BCR].iExist)
	{
		m_stHardWare[DEVICE_BCR].iStatus=m_CodeBarScaner.SetWorkMode(6);
	}
}
void CTranProDlg::UpdateStatusPIN()
{
	if(m_stHardWare[DEVICE_PIN].iExist)
	{
		m_stHardWare[DEVICE_PIN].iStatus=!m_pincom.IsInit();
	}
}
void CTranProDlg::UpdateStatusDVR()
{
	if(m_stHardWare[DEVICE_DVR].iExist)
	{
		if(m_lDVR_UserID!=-1)
		{
			if(NET_DVR_GetDVRWorkState_V30(m_lDVR_UserID,&m_stDVR_StateInfo))//��ȡ�豸״̬
				m_stHardWare[DEVICE_DVR].iStatus = m_stDVR_StateInfo.dwDeviceStatic;
		}
	}
}
void CTranProDlg::SelfTest()
{
	g_Log.WriteLogEx(LOG_JOURNAL,"���Ӳ��״̬");
	for(int i=DEVICE_START;i<=DEVICE_END;i++)
	{
		UpdateStatusDevice(i);
	}
}


int CTranProDlg::SendDataToService(char *pSendData,int iSendLen,char *pRecvData,int iRecvLen,int iNetTimeout,int iRecvFlag)
{
	int		iReturn = 0;
	iNetTimeout*=1000;
	int		iClientSocketId = 0;
	struct	sockaddr_in		Remote;

	Remote.sin_family = AF_INET;
 	Remote.sin_port   = htons((unsigned short)m_iPort);
 	Remote.sin_addr.s_addr = inet_addr(m_IP);

	CString data = CW2A(CT2W(pSendData),CP_UTF8);	  //ת����utf8����

	m_iIsSending = 1;		
	iClientSocketId = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		
	if(iClientSocketId > 0)
	{		
		iReturn=connect(iClientSocketId,(struct sockaddr *)&Remote,sizeof(Remote));
			
		if(iReturn != SOCKET_ERROR)
		{
			setsockopt(iClientSocketId,SOL_SOCKET,SO_SNDTIMEO,(char *)&iNetTimeout,sizeof(int));  //���÷��ͳ�ʱ����
			iReturn = send(iClientSocketId, data, data.GetLength(), 0);
			g_Log.WriteLogEx(LOG_COMMUNICATION,"������[%s:%d],���[%d],�׽���[%d]",m_IP,m_iPort,iReturn,iClientSocketId);
			g_Log.WriteLog(LOG_COMMUNICATION,data.GetBuffer(),data.GetLength());
			if(iReturn<=0)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"**���ͱ���ʧ��[%d]",iReturn);
				iReturn=-3;
			}
			else
			{
				if(iRecvFlag)
				{
					setsockopt(iClientSocketId,SOL_SOCKET,SO_RCVTIMEO,(char *)&iNetTimeout,sizeof(int));  //���ý��ճ�ʱ����
					iReturn = recv( iClientSocketId, pRecvData, iRecvLen, 0);						
					g_Log.WriteLogEx(LOG_COMMUNICATION,"����,���[%d]���׽���[%d]",iReturn,iClientSocketId);
					if(iReturn<=0)
					{
						g_Log.WriteLogEx(LOG_JOURNAL,"**���ձ���ʧ��[%d]",iReturn);
						iReturn=-4;
					}
					else
						g_Log.WriteLog(LOG_COMMUNICATION,_UTF8ToANSI(pRecvData).GetBuffer(),strlen(pRecvData));
				}
			}
		}
		else
		{
			g_Log.WriteLogEx(LOG_COMMUNICATION,"**TCP/IP����[%s:%d]ʧ��,�׽���[%d]",m_IP,m_iPort,iClientSocketId);
			iReturn = -2;
		}
	}
	else
	{
		g_Log.WriteLogEx(LOG_COMMUNICATION,"TCP/IP�׽��ִ���");
		iReturn = -1;
	}	
	closesocket(iClientSocketId);
	
	m_iIsSending = 0;
	return iReturn;
}

void CTranProDlg::ConfirmDepositState()
{//00ʧ�ܿ��� 01�ɹ� 02ʧ�ܲ�����
	if(WORKMODE_POST!=m_iWorkMode)
	{
		TRSHOW("ExecFunc_ConfirmDeposit|00|��Ͷ��ģʽ");
		g_Log.WriteLogEx(LOG_JOURNAL,"**����ģʽ����[%d]",m_iWorkMode);
	}
	else
	{	
		if(!IsEmptyLogicalBox(m_iLogicalCurrentBox))	//iStatus[1]: 1���л���	  0���޻��� 
			m_cbox.SetNotEmptyFail(m_iLogicalCurrentBox,m_cbox.GetNotEmptyFail(m_iLogicalCurrentBox)+1); //���ӷ���ʧ�ܼ���		

		if(LoadPackageByBoxIDFromDB(m_iLogicalCurrentBox,m_pw,m_iBarCodeCount,m_barCode,m_postmanID,m_csExtraData))//������Ϣ�Ѵ���
		{//�����ܵ�����ɣ���
			g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]�ǿ�,����Ͷ��",m_iLogicalCurrentBox);
			TRSHOW("ExecFunc_ConfirmDeposit|00|��ڱ�ռ�ã���ȡ�ذ���������");
		}
		else 
		{
			while(true)//����ȡ������
			{
				m_pw=CreatePickupPW();

				if(!FindPickupPWFromDB(m_pw))
					break;
				Sleep(0);
			}
			g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]����Ͷ�ݰ�����Ϣ",m_iLogicalCurrentBox);
			bool bSavePackage = SavePackageToDB(m_iLogicalCurrentBox,m_pw,m_iBarCodeCount,m_barCode,m_PostLoginID,m_cIDCardData.name,m_cIDCardData.address,m_csMobile,m_iDeadline,m_iVerifyType);
			if(!bSavePackage)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"**��������ʧ�ܣ�����ȡ�ذ���");
				TRSHOW("ExecFunc_ConfirmDeposit|00|Ͷ��ʧ�ܣ���ȡ�ذ���������");				
			}
			else
			{
				m_cbox.SetBoxLocalPackage(m_iLogicalCurrentBox,1);
				m_cbox.SetBoxHostTimeoutAvailable(m_iLogicalCurrentBox,0);
				m_cbox.SetBoxHostPackageAvailable(m_iLogicalCurrentBox,1);

				m_tranCode="600002";
				CString send=Pack();
				CString recv;
				int iReturnCode,iSend=1;
				CString csReturnMsg;

				if(m_iIsPostDelaySend)
				{
					if(TransactionInsertQueueToDB(m_tranCode,GetSerialNOString(),send))
					{
						iSend=0;
						TRSHOW("ExecFunc_ConfirmDeposit|01|Ͷ�ݳɹ�");				
					}
				}

				if(iSend)//ʵʱ����
				{
					if(m_iIsPostFailOpenDoor)
					{
						if(Transaction(send,recv,iReturnCode,csReturnMsg))
						{//ȷ��Ͷ�ݽ���ʧ�� ���ִ������� ȫ������ط� ���Ҳ����ʧ�ܶ�һ��
							g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ�ݽ���ʧ��,����[%d][%s]",iReturnCode,csReturnMsg);
							TRSHOW("ExecFunc_ConfirmDeposit|00|Ͷ�ݽ���ʧ��,����");
						}
						else
						{
							TRSHOW("ExecFunc_ConfirmDeposit|01|Ͷ�ݳɹ�");
						}
					}
					else
					{
						if(Transaction(send,recv,iReturnCode,csReturnMsg,true))
						{//ȷ��Ͷ�ݽ���ʧ�� ���ִ������� ȫ������ط� ���Ҳ����ʧ�ܶ�һ��
							g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ�ݽ���ʧ��,������[%d][%s]",iReturnCode,csReturnMsg);
						}
						TRSHOW("ExecFunc_ConfirmDeposit|01|Ͷ�ݳɹ�");
					}
				}//ʵʱ����
			}//��������ʧ��
		}//�жϱ����Ƿ���ڸð���
	}//�жϹ���ģʽ
}
/*
void CTranProDlg::ModifyPackageInfoState()
{
	if(HOST_XD==m_iCodeForm)
	{
		TRSHOW("ExecFunc_ModifyPackageInfo|00|�����޸�");
	}
	else if(HOST_GZYZ==m_iCodeForm)
	{
		m_tranCode="updatePackage";
		CString send=Pack();
		CString recv;
		int iReturnCode;
		CString csReturnMsg;
		if(Transaction(send,recv,iReturnCode,csReturnMsg))
		{
			TRSHOW("ExecFunc_ModifyPackageInfo|00|�޸�ʧ��");
		}
		else
		{
//			m_csMobile=m_csMobileModify;//m_csMobileModify������ɾ��
			TRSHOW("ExecFunc_ModifyPackageInfo|01|�޸ĳɹ�");
		}
	}
}
*/
void CTranProDlg::CheckPackageInfoState()
{
	m_tranCode="600007";
	CString send=Pack();
	CString recv;
	int iReturnCode;
	CString csReturnMsg;
	if(Transaction(send,recv,iReturnCode,csReturnMsg))
	{//fail
		g_Log.WriteLogEx(LOG_JOURNAL,"ȡ������Ϣ[%d][%s]",iReturnCode,csReturnMsg);
		m_iVerifyType=2;
		TRSHOW("ExecFunc_CheckPackageInfo|01|0|%s|||�ֻ���4λ+����",m_barCode[0]);
	}
	else
	{//�Ƿ��ɹ� ��û���ݣ���
		if(1==atoi(m_mailStatus) || 2==atoi(m_mailStatus))
		{
			int iCount = 0;
			while(m_bIsBarcodeOpening)
			{
				Sleep(500);
				iCount++;
				if(iCount>=10) //���ȴ�5��
					break;		
			}
			TRSHOW("ExecFunc_CheckPackageInfo|00|���ʼ��������ظ�Ͷ��");		
		}
		else
		{
			char szTmp[64]={0};
			CString csMobile = _i64toa(_atoi64(m_csMobile),szTmp,10);
			CString csVerifyType;;
			switch(m_iVerifyType)
			{
			case 1:csVerifyType="����ȡ��";break;
			case 2:csVerifyType="�ֻ���4λ+����";break;
			case 3:csVerifyType="����+����";break;
			case 4:csVerifyType="���֤��+����";break;
			case 5:csVerifyType="����+���ζ�����֤��";break;
			default:m_iVerifyType=2;csVerifyType="�ֻ���4λ+����";break;//��̨�޷���ȡ����ʽ����Ĭ���ֻ���+����ķ�ʽ
			}
			if(m_csMobile.GetLength()!=11 
				|| /*_MakeString("%d",atoi(m_csMobile)).GetLength()!=11*/csMobile.GetLength()!=11
				|| m_csMobile.GetAt(0)!='1')
				TRSHOW("ExecFunc_CheckPackageInfo|01|0|%s|||�ֻ���4λ+����",m_barCode[0]);
			else
				TRSHOW("ExecFunc_CheckPackageInfo|01|1|%s|%s|%s|%s",m_barCode[0],m_csMobile,m_cIDCardData.name,csVerifyType);
		}
	}
}
void CTranProDlg::DeliverGetPackageInfoState()
{
	m_tranCode="600017";
	CString send=Pack();
	CString recv,csReturnMsg;
	int iReturnCode;
	CString csFlashData;

	int iRet = Transaction(send,recv,iReturnCode,csReturnMsg);
	g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��Ա��ѯ���������Ϣ[%d]",iRet);
	csFlashData = GetPackageInfoFromDB(m_PostLoginID);
	if(csFlashData.IsEmpty())
		csFlashData = "ExecFunc_GetPackageMessage|01|0";
	else
		csFlashData.Insert(0,"ExecFunc_GetPackageMessage|01|");

	TRSHOW2(csFlashData);
}
void CTranProDlg::PostmanLoginState()
{
	int iReturnCode;
	CString csReturnMsg;
	
	bool isLoginInSuccess=false;
	int isLoginInOnLine=false;
	if(m_iIsLoginInOffLine)
	{//������֤
		if(VerifyPostman(m_PostLoginID,m_PostLoginPW))
		{//������֤�ɹ� ����������֤
			isLoginInSuccess=true;
			if(m_pDB->ExeSQLQuery(_MakeString("SELECT Name,Others FROM postman WHERE PostmanID='%s'",m_PostLoginID))) 
			{
				if(m_pDB->StoreResult()&&m_pDB->GetRowNumOfSelect()>0) 
				{
					MYSQL_ROW row = m_pDB->GetRow();
					m_deliveryName=row[0];
					m_compShortName=row[1];
				}
				m_pDB->FreeResultSet();
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��Ա[%s]��������˾��Ϣ",m_PostLoginID);
			}
		}
		else
		{//������֤ʧ�� ����û�˺� �� �������ɾ�������˺����ϣ����ù���ɾ���Ľ��
			if(m_iLoginInOnLineWhileOffLineFail)//����������½ ɾ��������
				DeletePostman(m_PostLoginID);
		}
	}

	if(!m_iIsLoginInOffLine||(!isLoginInSuccess&&m_iLoginInOnLineWhileOffLineFail))//���ѻ� ���� �ѻ�+�ѻ�ʧ����������
	{//������֤
		isLoginInOnLine=true;

		m_tranCode = "600001";
		CString send=Pack();
		CString recv;

		if(Transaction(send,recv,iReturnCode,csReturnMsg))
			g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��Ա��½[%d][%s]",iReturnCode,csReturnMsg);
		else 
			isLoginInSuccess=true;
	}

	if(isLoginInSuccess)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,">>>>Ͷ��Ա[%s]��½<<<<",m_PostLoginID);
		iReturnCode = 1;
		csReturnMsg.Format("��ӭ %s %s ʹ�����������Ű���",m_compShortName,m_deliveryName);
		if(isLoginInOnLine)
		{
			m_tranCode = "600006";
			CString send=Pack();
			CString recv;
			int iReturnCode2;
			CString csReturnMsg2;
			if(Transaction(send,recv,iReturnCode2,csReturnMsg2))
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯ��ʱ����[%d][%s]",iReturnCode2,csReturnMsg2);
			}
			SaveTimeoutPackageToDB();  //��¼�ɹ������泬ʱ����
		}

		m_cbox.SetBoxHostTimeoutAvailable();//�����ʱ��Ϣ				
		LoadTimeoutBoxFromPostman();
		m_cbox.SetBoxHide(0,1);//����������
		LoadAvailableBoxFromPostman();//�����ݿ�ȡ������
	}

	isLoginInSuccess? TRSHOW("ExecFunc_PostmanLogIn|01|%s",csReturnMsg) : TRSHOW("ExecFunc_PostmanLogIn|00|%s",csReturnMsg);
}
void CTranProDlg::PhoneLoginVerificationCodeState()
{
	m_tranCode = "600016";
	CString send=Pack();
	CString recv;
	int iReturnCode;
	CString csReturnMsg;
	if(Transaction(send,recv,iReturnCode,csReturnMsg))//�ֻ���½ʧ��
	{
		TRSHOW("ExecFunc_PhoneLoginVerificationCode|00|%s",csReturnMsg);
	}
	else
	{
		m_tranCode = "600006";//get timeout packages infomation
		CString send=Pack();
		CString recv;
		int iReturnCode2;
		CString csReturnMsg2;
		if(Transaction(send,recv,iReturnCode2,csReturnMsg2))
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯ��ʱ����[%d][%s]",iReturnCode2,csReturnMsg2);
		}
		SaveTimeoutPackageToDB();  //��¼�ɹ������泬ʱ����
		m_cbox.SetBoxHide(0,1);//����������
		LoadAvailableBoxFromPostman();//�����ݿ�ȡ������

		TRSHOW("ExecFunc_PhoneLoginVerificationCode|01|��ӭ %s %s ʹ�����������Ű���",m_compShortName,m_deliveryName);
	}
}
void CTranProDlg::PostmanPhoneLogin()
{	
	m_tranCode = "600015";
	CString send=Pack();
	CString recv;
	int iReturnCode;
	CString csReturnMsg;
	if(Transaction(send,recv,iReturnCode,csReturnMsg))
	{//fail
		TRSHOW("ExecFunc_PhoneLogin|00|%s",csReturnMsg);
	}
	else
	{
		TRSHOW("ExecFunc_PhoneLogin|01");
	}
}
void CTranProDlg::OpenDoorState(int iMode,CString Value)
{
	if(m_bIsClosingDoor)//�����߳���������
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**����ʱ�����߳���������");
		m_bIsCancelCloseDoor=1;
		for(int i=0;i<10;i++)//���ͣ��5��
		{
			Sleep(500);
			if(!m_bIsClosingDoor)
				break;
		}
		if(m_bIsClosingDoor)
		{//�����߳�δֹͣ
			if(WORKMODE_POST_CANCEL!=m_iWorkMode)
			{
				//TRSHOW("ExecFunc_OpenDoor|02|����ʧ��|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));

				int iBoxID;
				CString csBoxName;

				if(OPENDOOR_MODE_BOXTYPE == iMode)//�������Ϳ���
				{
					int iBoxStatus , iBoxType;
					iBoxType = atoi(Value);
					if(WORKMODE_POST == m_iWorkMode)
						iBoxStatus = 0;//������
					else if(WORKMODE_POSTMAN_TAKE == m_iWorkMode)
						iBoxStatus = 1;//����ʱ������

					if(m_iCurrentOpenBox>0 && iBoxType==m_cbox.GetClassifyBoxSpec(m_iCurrentOpenBox) && m_cbox.GetStatus(m_iCurrentOpenBox)==iBoxStatus)
						iBoxID = m_iCurrentOpenBox;								
					else
						iBoxID = m_cbox.GetClassifyBoxID(iBoxStatus,iBoxType);	

					csBoxName = m_cbox.GetBoxName(iBoxID);							
				}
				else
				{
					iBoxID = m_cbox.GetLogicalBoxByBoxName(Value);
					csBoxName = Value;
				}

				TRSHOW("ExecFunc_OpenDoor|02|����ʧ��|%d|%s",iBoxID,csBoxName); 
			}

			g_Log.WriteLogEx(LOG_JOURNAL,"**�����߳��޷���ֹ");
			return;
		}
	}

	int iValue = -1;
	if(WORKMODE_OPENDOOR_TEST==m_iWorkMode)
	{//�����а������ܿ�����ԣ�����02���̲����ã���ǰ�ڴ˴���
		iValue = m_cbox.GetLogicalBoxByBoxName(Value);
		if(iValue<=0 || iValue>m_cbox.GetBoxCountTotal())
		{
			TRSHOW("ExecFunc_OpenDoor|00|����������");
			return;
		}
		else if(m_cbox.GetBoxLocalPackage(iValue))
		{
			TRSHOW("ExecFunc_OpenDoor|00|%s�������а������ݣ����ù���Աȡ�����ܿ��䡣",m_cbox.GetBoxName(iValue));
			return;
		}

		if(OpenDoorByIDState(iValue))
			TRSHOW("ExecFunc_OpenDoor|01|����ɹ�|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		else
			TRSHOW("ExecFunc_OpenDoor|02|����ʧ��|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		return;
	}
	else
		iValue=atoi(Value);

	if(OPENDOOR_MODE_BOXID==iMode)  
	{//����ſ���
		if(OpenDoorByIDState(iValue))
		{
			if(WORKMODE_MANAGER_TAKE==m_iWorkMode)
				TRSHOW("ExecFunc_OpenDoor|01|����Աȡ���󣬸��ʼ������������Ű���Ͷ����ȡ��������������ʽ����Ͷ��|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
			else
				TRSHOW("ExecFunc_OpenDoor|01|����ɹ�|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		}
		else
		{
			TRSHOW("ExecFunc_OpenDoor|02|����ʧ��|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		}
	}
	else if(OPENDOOR_MODE_BOXTYPE==iMode)
	{//�������Ϳ���
		if(OpenDoorBySpecState(iValue))
			TRSHOW("ExecFunc_OpenDoor|01|����ɹ�|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		else
			TRSHOW("ExecFunc_OpenDoor|02|����ʧ��|%d|%s",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));	
			//WORKMODE_POST==m_iWorkMode ? TRSHOW("ExecFunc_OpenDoor|00|����ʧ��") : TRSHOW("ExecFunc_OpenDoor|02|����ʧ��|%d",m_iLogicalCurrentBox);			
	}
	else if(OPENDOOR_MODE_ONCE==iMode)
	{//�������Կ���	
		if(WORKMODE_POSTMAN_TAKE==m_iWorkMode)
			m_cbox.SetBoxHostTimeoutAvailable(m_iLogicalCurrentBox,0); //Ͷ��Աȡ���ڼ�ʧ�ܣ����θó�ʱ��
		OpenDoorByIDState(iValue);
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**����ģʽ����[%d]",iMode);
		TRSHOW("ExecFunc_OpenDoor|02|����ģʽ����[%d]",iMode);
	}		
}
bool CTranProDlg::OpenDoorByIDState(int iID)
{
	bool bRes=false;
	bool iCanOpendoor=false;//У�鿪�ŵĺϷ���
	if(!iID)
		g_Log.WriteLogEx(LOG_JOURNAL,"���Ų����Ƿ�");
	else if(-1==iID)
		g_Log.WriteLogEx(LOG_JOURNAL,"�޿�����");
	else
	{
		if(WORKMODE_TAKE==m_iWorkMode||WORKMODE_MANAGER_TAKE==m_iWorkMode||WORKMODE_POSTMAN_TAKE==m_iWorkMode)
		{//�ͻ�ȡ���͹���Ա��̨ȡ����Ͷ��Աȡ��ʱ�� ����һ��Ҫ�а�����Ϣ
			CString csPW;
			if(LoadPackageByBoxIDFromDB(iID,csPW,m_iBarCodeCount,m_barCode,m_postmanID,m_csExtraData))
			{
				//if(WORKMODE_TAKE!=m_iWorkMode||csPW==m_pw)//�ͻ�ȡ��ҪУ������ ����Աȡ������Ҫ
				//{
				//	m_pw=csPW;  //add by yjx Э���̨����Աȡ����Ҫ����
				//	iCanOpendoor=true;				
				//}
				//else
				//	g_Log.WriteLogEx(LOG_JOURNAL,"�������[%s]",m_pw);
				iCanOpendoor=true;	
			}
			else
				g_Log.WriteLogEx(LOG_JOURNAL,"�ް�����Ϣ");
		}
		else
			iCanOpendoor=true;
	}

	if(iCanOpendoor) //������������
	{
		m_iLogicalCurrentBox=iID;
		m_csCurrentBoxID=m_cbox.GetBoxName(m_iLogicalCurrentBox); 
		m_cbox.PrintWarning(m_iLogicalCurrentBox);
		bRes=OpenLogicalBox(m_iLogicalCurrentBox);
		if(bRes)
		{//����ɹ�
			m_cbox.SetOpenFail(m_iLogicalCurrentBox,0);//��տ���ʧ�ܼ���
			m_vBoxDoorStatus[m_iLogicalCurrentBox-1]=0;
			m_cbox.SetBoxLocalPackage(m_iLogicalCurrentBox,0);
			m_cbox.SetBoxHostTimeoutAvailable(m_iLogicalCurrentBox,0);//���Ŀ���ʱ�� ��������Ϊ��̨��֪��ȡ�� �������ʧ�� �����´�Ͷ��Ա��½ʱ��ȡ��̨��ȷ״̬
			m_cbox.SetBoxHostPackageAvailable(m_iLogicalCurrentBox,0);
			UpdateTimeoutBoxToPostman(m_iLogicalCurrentBox); //����postman���Ӧ�ĳ�ʱ������

			if(!UpdateDataToDB(m_iLogicalCurrentBox)) //�������ݿ�����
				m_cbox.SetBoxHide(m_iLogicalCurrentBox,1);      //��������ʧ�ܣ����θ���

			m_tranCode.Empty();
			if(WORKMODE_POSTMAN_TAKE==m_iWorkMode)
			{//���ųɹ� ɾ�����ذ�����Ϣ ���ײ������
				m_tranCode = "600003";
			}
			else if(WORKMODE_TAKE==m_iWorkMode)
			{
				m_tranCode = "600004";
			}
			else if(WORKMODE_MANAGER_TAKE==m_iWorkMode)
			{
				m_tranCode = "600004";
				m_pw.Empty();//����Աȡ����������				
				m_cIDCardData.idcode=m_PostLoginID;
				m_cIDCardData.name="Postman";
			}
			if(!m_tranCode.IsEmpty())
			{
				int iSend=1;
				CString send=Pack();
				if(m_iIsTakeoutDelaySend)
				{
					if(TransactionInsertQueueToDB(m_tranCode,GetSerialNOString(),send))  //ֱ�������
						iSend=0;
				}

				if(iSend)
				{
					CString recv;
					int iReturnCode;
					CString csReturnMsg;
					if(Transaction(send,recv,iReturnCode,csReturnMsg,true)) 
					{//����ȡ������ʧ�� ���ִ������� ȫ������ط� ���Ҳ����ʧ�ܶ�һ��
						g_Log.WriteLogEx(LOG_JOURNAL,"����ȡ��[%d][%s]",iReturnCode,csReturnMsg);
					}
				}
			}//������
		}
		else
		{
			m_bSendBoxStatus = true;

			m_cbox.SetOpenFail(m_iLogicalCurrentBox,m_cbox.GetOpenFail(m_iLogicalCurrentBox)+1); //��¼����ʧ�ܴ���
			if(WORKMODE_POSTMAN_TAKE==m_iWorkMode)
				m_vBoxDoorStatus[m_iLogicalCurrentBox-1]=1;
			else if(WORKMODE_TAKE==m_iWorkMode || WORKMODE_MANAGER_TAKE==m_iWorkMode)
				m_vBoxDoorStatus[m_iLogicalCurrentBox-1]=2;
			else
				m_bSendBoxStatus=false;
		}
	}
	return bRes;
}
bool CTranProDlg::OpenDoorBySpecState(int iFlashSpec)
{//iFlashSpec  ����������� 6����5�ϴ�4�´�3��2С1��С
	//iSpec �߼������� 5����4��3��2С1��С
	g_Log.WriteLogEx(LOG_JOURNAL,"���ܿ�������[%d.%s]",iFlashSpec,BoxSpecChinese(iFlashSpec));
	bool bRes=false;
	int iStatus=-1;
	int iSpec;
	int iHigh=-1;
			
	if(WORKMODE_POST==m_iWorkMode)
		iStatus=0;//������
	else if(WORKMODE_POSTMAN_TAKE==m_iWorkMode)
		iStatus=1;//����ʱ������
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"��ǰģʽ[%d]�����ܿ��䲻����",m_iWorkMode);
		return false;
	}			

	iSpec=iFlashSpec;

	int iOpenDoorCount = 0;
	int iBox;
	while(true)
	{
		if(m_iCurrentOpenBox>0 && iSpec==m_cbox.GetClassifyBoxSpec(m_iCurrentOpenBox) && m_cbox.GetStatus(m_iCurrentOpenBox)==iStatus)
			iBox = m_iCurrentOpenBox;
		else
			iBox=m_cbox.GetClassifyBoxID(iStatus,iSpec,iHigh);

		if(iBox<=0)
		{//�����͸߶��޿�����
			break;
		}

		m_iLogicalCurrentBox=iBox;
		g_Log.WriteLogEx(LOG_JOURNAL,"���ܿ���ѡ���߼���[%d][%s]",m_iLogicalCurrentBox,m_cbox.GetBoxName(m_iLogicalCurrentBox));
		
		if(OpenDoorByIDState(m_iLogicalCurrentBox))
		{//���ųɹ�
			bRes=true;
			//if(m_iCurrentOpenBox>0)
			//	m_iCurrentOpenBox = -1;
			break;
		}
		else
		{
			if(iOpenDoorCount<3)
			{//����ʧ�ܳ������¿�����3�Σ����ٷ�������������
				iOpenDoorCount++;	
				m_iCurrentOpenBox = m_iLogicalCurrentBox;
			}
			else
			{
				if(m_iCurrentOpenBox>0)
					m_iCurrentOpenBox = -1;
				iOpenDoorCount = 0;
				m_cbox.SetBoxHide(m_iLogicalCurrentBox,1);//����ģʽ�����ش��� ���ⷴ������һ����
			}
		}
	}

	return bRes;
}
void CTranProDlg::MySleep(DWORD dwMilliseconds)
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
void CTranProDlg::VerificationCodeState()
{
	m_tranCode = "600010";

	int iReturnCode;
	CString csReturnMsg;
	CString send=Pack();
	CString recv;

	if(Transaction(send,recv,iReturnCode,csReturnMsg))
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"��ȡ��֤��[%d][%s]",iReturnCode,csReturnMsg);
	}
	else
	{
		iReturnCode=1;
	}

	TRSHOW("ExecFunc_VerificationCode|%02d|%s",iReturnCode,m_csExtraData);
}

//���أ�
//0  �ɹ�
//-1 �׽��ִ�
//-2 ����ʧ��
//-3 ���ͱ���ʧ��
//-4 ���ձ���ʧ��
//-5 ���ʧ��
//
int CTranProDlg::Transaction(CString send,CString &recv,int &iReturnCode,CString &csReturnMsg,bool bInsertQueueWhileFail)
{//0������
	if(!send.GetLength())//�����ĵز����Ϳս��� ��������̨�����ڵĽ��׿����ڽ��������Pack�ӿ��з��ؿ��ַ��� ����Ҫ���ⲿ�жϺ�̨����
		return 0;
	m_bSendTrans = true;
	g_Log.WriteLogEx(LOG_JOURNAL,"���׷���:%s,�߼�������[%s]",TrancodeChinese(m_tranCode),m_tranCode);
	g_Log.WriteLogEx(LOG_COMMUNICATION,"���׷���:%s,�߼�������[%s]",TrancodeChinese(m_tranCode),m_tranCode);
	//g_Log.WriteLogEx(LOG_TRACE,"���׷���:%s,�߼�������[%s]",TrancodeChinese(m_tranCode),m_tranCode);

	recv.Empty();
	iReturnCode=0;
	csReturnMsg.Empty();

	//char szRecvData[8192]={0};
	char szRecvData[1024*500]={0};//Ͷ��Ա��ѯ�������ʱ��Ϣ�����ܻ�ܴ󣬹�����������500K
	int iRecvLen = sizeof(szRecvData);
	int iRes=SendDataToService(send.GetBuffer(),send.GetLength(),szRecvData,iRecvLen,m_iNetTimeout);
	if(iRes<=0) //ͨѶ����
	{
		 if(!iRes)
			iRes=-4;//�յ�����Ӧ���ɽ���ʧ��
		iReturnCode=2;
		if(-3==iRes)
			csReturnMsg="�����쳣1";//���ͱ���ʧ��
		else if(-4==iRes)
			csReturnMsg="�����쳣2";//���ձ���ʧ��
		else
			csReturnMsg="�����쳣3";//ͨѶ����ʧ��

		m_bNetWorkFail = true;
	}
	else
	{
		//g_Log.WriteLogEx(LOG_COMMUNICATION,"���Ľ���:%s",UTF8toANSI(szRecvData));
		if(!Unpack(szRecvData,csReturnMsg))//����ɹ�
		{
			iRes=0;
			iReturnCode=0;				
		}
		else
		{
			iRes=-5;//�������
			iReturnCode=2;	
		}

		m_bNetWorkFail = false;
	}

	if(!iRes)//�ɹ� ������Ӧ��
	{
		SaveResponse(m_csExtraData);
	}	
	else
	{//ʧ�� �ط�
		if(bInsertQueueWhileFail)
			TransactionInsertQueueToDB(m_tranCode,GetSerialNOString(),send);
	}
	return iRes;
}
CString CTranProDlg::Pack()
{
	if(m_tranCode.IsEmpty())//�ú�̨��֧�ֵĽ�������
		return "";	

	FreshSerialNO();

	g_Log.WriteLogEx(LOG_JOURNAL,"�������:%s,��ˮ[%s],�߼�������[%s]",TrancodeChinese(m_tranCode),GetSerialNOString(),m_tranCode);
	g_Log.WriteLogEx(LOG_COMMUNICATION,"�������:%s,��ˮ[%s],�߼�������[%s]",TrancodeChinese(m_tranCode),GetSerialNOString(),m_tranCode);
	//g_Log.WriteLogEx(LOG_TRACE,"�������:%s,��ˮ[%s],�߼�������[%s]",TrancodeChinese(m_tranCode),GetSerialNOString(),m_tranCode);

	CString data;
	if(!m_tranCode.Compare("600001"))	//��¼����	
		data = GZYZPackPostLogin();
	else if(!m_tranCode.Compare("600006"))	//��ѯ��ʱ����
		data = GZYZPackCheckTimeout();
	else if(!m_tranCode.Compare("600003"))	//ȡ��ʱ����
		data = GZYZPackTakeTimeout();
	else if(!m_tranCode.Compare("600007"))	//��ѯ������Ϣ
		data = GZYZPackGetPackageInfo();
	/*else if("updatePackage"==m_tranCode)	//�޸İ�����Ϣ
		return GZYZPackModifyPackageInfo();*/
	else if(!m_tranCode.Compare("600002"))	//Ͷ�ݽ���
		data = GZYZPackConfirmDelivery();
	/*else if("DeliverDeposit"==m_tranCode)	//ȷ�ϼĴ潻��
		return GZYZPackConfirmDeposit();
	else if("deliverPickupItem"==m_tranCode)	//ȡ�߼Ĵ����
		return GZYZPackPostTakeout();*/
	else if(!m_tranCode.Compare("600004"))	//ȡ������
		data = GZYZPackCustomerTakeout();
	else if(!m_tranCode.Compare("600008"))	//�ն��豸״̬
		data = GZYZPackTerminalStatus();
	else if(!m_tranCode.Compare("600009"))	//�ն���ҵ��״̬
		data = GZYZPackBoxStatus();
	else if(!m_tranCode.Compare("600015"))	//phone login in
		data = GZYZPackPostPhoneLogin();
	else if(!m_tranCode.Compare("600016"))	//phone login in
		data = GZYZPackVerification();
	else if(!m_tranCode.Compare("600017"))	//phone login in
		data = GZYZPackGetDeliveryPackageInfo();
	else if(!m_tranCode.Compare("600018"))	//�ն˳�ʼ��
		data = GZYZPackInitTerminal();
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**δ֪�߼�������[%s]",m_tranCode);

	//CString csSendData = CW2A(CT2W(data),CP_UTF8);	  //ת����utf8����
	return data;
}
int CTranProDlg::Unpack(const char* szInData,CString& csMsg)
{
	int iRet=-2;
	int flag=0;
	CString soap;
	CString response;
	CString plainResponse;
	CString http = szInData;
	if(GZYZUnpackHttp(soap,http))
	{
		if(GZYZUnpackSoap(response,soap))
		{
			plainResponse=GZYZDecodeBase64(response,m_iPlainMessage);

			g_Log.WriteLogEx(LOG_COMMUNICATION,"��̨��Ӧ[%d]:",plainResponse.GetLength());
			g_Log.WriteLog(LOG_COMMUNICATION,plainResponse.GetBuffer(),plainResponse.GetLength());

			CString csFlag=_GetDataXMLString("flag",plainResponse);
			if(!csFlag.IsEmpty())
			{
				flag=atoi(csFlag);
				if(1!=flag)
				{
					int pos1=plainResponse.Find("Message=\"");
					int pos2=plainResponse.Find("\"",pos1+9);
					if(pos1>0&&pos2>pos1)
						csMsg=plainResponse.Mid(pos1+9,pos2-pos1-9);
					if(csMsg.IsEmpty())
						csMsg="����ʧ��";
					iRet = -1;
				}
				else
				{
					csMsg="���׳ɹ�";
					iRet = 0;
				}
			}
			else if(!strcmp("600006",m_tranCode) || !strcmp("600007",m_tranCode))
			{
				csMsg="���׳ɹ�";
				iRet = 0;//����������û��flag��־ 		
			}
			else
			{
				csMsg="����ʧ��";
				iRet = -1;		
			}	

			if(!iRet)
			{
				if(!Response(plainResponse))
				{
					csMsg="����ʧ��";//���ʧ��
					iRet = -2;
				}
			}
		}
	}

	return iRet;
}
CString CTranProDlg::TrancodeChinese(CString trancode)
{
	if("600001"==trancode)
		return "Ͷ��Ա��¼";
	if("600002"==trancode)
		return "Ͷ��";
	if("600003"==trancode)
		return "��ʱ������";
	if("600004"==trancode)
		return "ȡ��";
	if("600005"==trancode)
		return "����";
	if("600006"==trancode)
		return "��ѯ��ʱ��";
	if("600007"==trancode)
		return "��ѯ������Ϣ";
	if("600008"==trancode)
		return "�ն��豸״̬�ϱ�";
	if("600009"==trancode)
		return "��ҵ��״̬�ϱ�";
	if("600010"==trancode)
		return "��֤��У��";
	if("600011"==trancode)
		return "Ͷ��Աע��";
	if("600012"==trancode)
		return "Ͷ��Ա����";
	if("600013"==trancode)
		return "�ն˻�ע��";
	if("600014"==trancode)
		return "ȡ���������֤";
	if("600015"==trancode)
		return "Ͷ��Ա�ֻ���¼";
	if("600016"==trancode)
		return "Ͷ��Ա��¼��̬����֤";
	if("600017"==trancode)
		return "Ͷ��Ա��ѯ������Ϣ";
	if("600018"==trancode)
		return "�ն˳�ʼ��";
	return "δ֪����";
}

bool CTranProDlg::InitDB()
{
	// �������ݶ���
	if (!m_pDB->InitDB())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ʼ�����ݿ�ʧ��[%d][%s]",m_pDB->GetErrorNo(),m_pDB->GetErrorString());
		return false;
	}	
	
	if (!m_pDB->ConnectDB(m_csDBIP, m_csDBUser, m_csDBPassword, m_csDBName, m_iDBPort))
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**�������ݿ�ʧ��[%d][%s]",m_pDB->GetErrorNo(),m_pDB->GetErrorString());
		return false;
	}
	
	if (!m_pDB->ExeSQLQuery("SET NAMES GB2312"))
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**�������ݿ����ʧ��");
		return false;
	}

	if(!m_pDB->ExeSQLQuery(_MakeString("use %s",m_csDBName)))
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**ѡ�����ݿ�ʧ��");
		return false;
	}

	return true;
}
void CTranProDlg::DisconnectDB()
{
	m_pDB->DisconnectDB();
}
bool CTranProDlg::ExeSQLQuery(const char* sqlString)
{
	if(m_pDB->ExeSQLQuery(sqlString))
		return true;

	int err=m_pDB->GetErrorNo();
	g_Log.WriteLogEx(LOG_JOURNAL,"ִ�����ݿ����ʧ��[%d:%s]",err,m_pDB->GetErrorString());
	if(2006==err||2013==err)
	{//2006 MySQL server has gone away,2013 Lost connection to MySQL server during query
		g_Log.WriteLogEx(LOG_JOURNAL,"���ݿ�Ͽ�,������������");
		if(InitDB())
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"�����������ݿ�ɹ�");
			if(m_pDB->ExeSQLQuery(sqlString))
				return true;
		}
	}
	g_Log.WriteLogEx(LOG_JOURNAL,"ִ�����ݿ������Ȼʧ��[%d:%s]",m_pDB->GetErrorNo(),m_pDB->GetErrorString());
	return false;
}
bool CTranProDlg::DeletePostman(CString id)
{
	bool ret=false;
		
	if(m_pDB->ExeSQLQuery(_MakeString("DELETE FROM postman WHERE PostmanID='%s'",id))&&m_pDB->GetRowNumOfAffected()>0)
	{
		m_pDB->Commit();
		g_Log.WriteLogEx(LOG_JOURNAL,"ɾ������Ͷ��Ա[%s]",id);
		ret=true;
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"ɾ������Ͷ��Ա[%s],ʧ��[%d][%s]",id,m_pDB->GetErrorNo(),m_pDB->GetErrorString());
		ret=false;
	}

	return ret;
}
bool CTranProDlg::VerifyPostman(CString id,CString pw)
{
	bool ret=false;

	if (m_pDB->ExeSQLQuery(_MakeString("SELECT PW FROM postman WHERE PostmanID='%s'",id))) 
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect() > 0) 
		{
			MYSQL_ROW row = m_pDB->GetRow();
			
			if(row[0]==pw)
			{
				ret=true;
				g_Log.WriteLogEx(LOG_JOURNAL,"����Ͷ��Ա[%s],�˺�����ƥ��",id);
			}
			else
			{
				ret=false;
				g_Log.WriteLogEx(LOG_JOURNAL,"����Ͷ��Ա[%s],�˺����벻ƥ��",id);
			}
		}
		m_pDB->FreeResultSet();
	}

	return ret;
}
bool CTranProDlg::SavePostman(CString id,CString pw)
{
	bool ret=false;
		
	if (m_pDB->ExeSQLQuery(_MakeString("INSERT INTO Postman VALUES('%s', '%s', '', '', '', '', '', '', '', '')",id, pw)) && m_pDB->GetRowNumOfAffected() > 0) 
	{
		m_pDB->Commit();   //����������޸����ݿ����ݺ�ȫ����commitһ��
		g_Log.WriteLogEx(LOG_JOURNAL,"���ӱ���Ͷ��Ա[%s]",id);
		ret=true;
	}
	else
	{
		//g_Log.WriteLogEx(LOG_JOURNAL,"���ӱ���Ͷ��Ա[%s],ʧ��[%s]",id,m_pDB->GetErrorString());
		ret=false;
	}

	return ret;
}
bool CTranProDlg::CheckMailNo(CString csMailNo)
{
	CString strSQL;
	strSQL.Format("SELECT * FROM Package WHERE MailNo='%s'", csMailNo);
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult()&&m_pDB->GetRowNumOfSelect()>0) 
		{
			m_pDB->FreeResultSet();
			return true;
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s]",m_pDB->GetErrorString());

	return false;
}
bool CTranProDlg::CheckBoxNo(int iBox)
{
	CString strSQL;
	strSQL.Format("SELECT * FROM Package WHERE BoxNo='%d'", iBox);
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult()&&m_pDB->GetRowNumOfSelect()>0) 
		{
			m_pDB->FreeResultSet();
			return true;
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s]",m_pDB->GetErrorString());
	
	return false;
}
bool CTranProDlg::UpdateTimeoutBoxToPostman(int iBoxNo)
{
	bool bRet = true;
	std::map<CString,CString> mHostTimeout;
	std::map<CString,CString>::iterator itera;

	if (m_pDB->ExeSQLQuery(_MakeString("SELECT * FROM postman")))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
				mHostTimeout[row[0]] = _MakeString("%s,",row[7]);
		}
		else
			g_Log.WriteLogEx(LOG_JOURNAL,"**��Ͷ��Ա��Ϣ");

		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPostman���ݱ�ʧ��[%d][%s]",m_pDB->GetErrorNo(),m_pDB->GetErrorString());

	for(itera=mHostTimeout.begin();itera!=mHostTimeout.end();itera++)
	{
		if(0<=itera->second.Find(_MakeString("%d,",iBoxNo)))
		{
			itera->second.Replace(_MakeString("%d,",iBoxNo),NULL);
			itera->second.TrimRight(",");
			m_pDB->ExeSQLQuery(_MakeString("UPDATE postman SET HostTimeoutBox='%s' WHERE PostmanID='%s'",itera->second,itera->first));
			if(m_pDB->GetRowNumOfAffected()>0)
				g_Log.WriteLogEx(LOG_JOURNAL,"����Ͷ��Ա��ʱ�����ݳɹ�,Ͷ��ԱID[%s],���º�ʱ��[%s]",itera->first,itera->second);
			else
			{
				bRet=false;
				g_Log.WriteLogEx(LOG_JOURNAL,"**����Ͷ��Ա��ʱ������ʧ��,Ͷ��ԱID[%s]",itera->first,itera->second);
			}
		}
	}

	return bRet;
}
//void CTranProDlg::PostmanCheckTimeOutBox(int iDBMode,CString csPostmanID)
//{
//	m_cbox.SetBoxHostTimeoutAvailable(); //��ճ�ʱ������
//
//	CString strSQL;
//	strSQL.Format("SELECT * FROM Package WHERE PostmanID='%s' AND Flag='2'", csPostmanID);
//	if (m_pDB->ExeSQLQuery(strSQL))
//	{
//		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
//		{
//			int iBox=0;
//			CString csBoxNo;
//			MYSQL_ROW row = NULL;
//			while(row = m_pDB->GetRow()) 
//			{
//				csBoxNo = row[0];
//				iBox = atoi(csBoxNo);
//				if(iBox>0&&iBox<=m_cbox.GetBoxCountTotal())
//				{//�������Ч��Χ��
//					m_cbox.SetBoxHostTimeoutAvailable(iBox,1);
//					//m_cbox.SetBoxHostPackageAvailable(iBox,1);				
//				}
//			}
//		}
//		else
//			g_Log.WriteLogEx(LOG_JOURNAL,"�޳�ʱ����");
//
//		m_pDB->FreeResultSet();
//	}
//	else
//		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s]",m_pDB->GetErrorString());
//}
bool CTranProDlg::SavePackageToDB(int iBox,CString csPW,int iBarCodeCount,CString csBarCode[],CString csPostLoginID,CString csName,CString csAddress,CString csMobile,int iDeadline,int iVerifyType,CString csExtraData)
{
	CString csTraceLog=_MakeString("�߼���[%d]�������,����[%s],����[%d]��:",iBox,csPW,iBarCodeCount);
	CString csLog=_MakeString("�߼���[%d]�������,����[%d]��:",iBox,iBarCodeCount);
	for(int i=0;i<iBarCodeCount;++i)
	{
		csLog.Append(_MakeString("%s,",csBarCode[i]));
		csTraceLog.Append(_MakeString("%s,",csBarCode[i]));
	}
	csLog.TrimRight(',');
	csTraceLog.TrimRight(',');
	g_Log.WriteLogEx(LOG_JOURNAL,csLog);
	g_Log.WriteLogEx(LOG_TRACE,csTraceLog);

	if(!iBox||iBox>m_cbox.GetBoxCountTotal())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**���[%d]�Ƿ�",iBox);
		return false;
	}

	bool bSave = true;

	CString strSQL;	
	CString strDateTime;
	CTime curTime = CTime::GetCurrentTime();
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());	
	iDeadline>0 ? iDeadline : iDeadline=1;

	for(int i=0;i<iBarCodeCount;i++)
	{
		strSQL.Format("INSERT INTO Package VALUES('%d', '%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s', '0', '%d', '1', '%s')",
			iBox, csPW, csBarCode[i], strDateTime, iDeadline, csPostLoginID, csMobile, csName, csAddress,iVerifyType,csExtraData);		

		if (!(m_pDB->ExeSQLQuery(strSQL) && m_pDB->GetRowNumOfAffected()>0)) 
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**����Package���ݱ�ʧ��[%d][%s]",m_pDB->GetErrorNo(),m_pDB->GetErrorString());
			strSQL.Format("��������:'%d', '%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s', '0', '%d', '1', '%s'",iBox, csPW, csBarCode[i], strDateTime, iDeadline, csPostLoginID, csMobile, csName, csAddress,iVerifyType,csExtraData);
			g_Log.WriteLog(LOG_JOURNAL,strSQL.GetBuffer(),strSQL.GetLength());		

			strSQL.Format("INSERT INTO Package VALUES('%d', '%s', '%s', '%s', '%d', '%s', '%s', '', '', '0', '%d', '1', '%s')",
				iBox, csPW, csBarCode[i], strDateTime, iDeadline, csPostLoginID, csMobile, iVerifyType, csExtraData);	
			if(!(m_pDB->ExeSQLQuery(strSQL) && m_pDB->GetRowNumOfAffected()>0))//�������ģ��ٲ���һ��
			{
				bSave = false;
				m_bReboot = true;
				g_Log.WriteLogEx(LOG_JOURNAL,"**�ٴβ���Package���ݱ�ʧ��[%d][%s]",m_pDB->GetErrorNo(),m_pDB->GetErrorString());	
				break;
			}
			else
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�ٴβ���Package���ݱ�ɹ�");
				m_pDB->Commit();
			}
		}
		else
			m_pDB->Commit();   //����������޸����ݿ����ݺ�ȫ����commitһ��
	}

	return bSave;
}
void CTranProDlg::LoadAvailableBoxFromPostman()
{
	CString strSQL;
	strSQL.Format("SELECT HostAvailableBox FROM Postman WHERE PostmanID='%s';",m_PostLoginID);  //��̨���صĿ��ÿ���
	if(m_pDB->ExeSQLQuery(strSQL))
	{
		if(m_pDB->StoreResult()&&m_pDB->GetRowNumOfSelect()>0)
		{
			MYSQL_ROW row = m_pDB->GetRow();//ֻȡһ�� PostmanID������ �����ظ�
			if(row)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��Ա[%s]�ɲ������[%s]",m_PostLoginID,row[0]);
				int count=_GetFieldCount(row[0],',');
				for(int i=0;i<count;++i)
				{
					//int box=atoi(_GetField(row[0],i+1,','));
					int box = m_cbox.GetLogicalBoxByBoxName(_GetField(row[0],i+1,','));//��̨���صĸ��ID�Ǵ���ĸ+���ֵ���ʽ����ת������
					if(m_cbox.GetPhysicalCabinet(box)>0)
						m_cbox.SetBoxHide(box,0);//���ݿ����и��� ���Ÿ���
				}
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"ִ��SQLʧ��[%s][%s][%d]",strSQL,m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	strSQL.Format("SELECT HostTimeoutBox FROM Postman WHERE PostmanID='%s';",m_PostLoginID); //��̨���صĳ�ʱ��
	if(m_pDB->ExeSQLQuery(strSQL))
	{
		if(m_pDB->StoreResult()&&m_pDB->GetRowNumOfSelect()>0)
		{
			MYSQL_ROW row = m_pDB->GetRow();//ֻȡһ�� PostmanID������ �����ظ�
			if(row)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"Ͷ��Ա[%s]�ɲ������ڸ��[%s]",m_PostLoginID,row[0]);
				int count=_GetFieldCount(row[0],',');
				for(int i=0;i<count;++i)
				{
					int box=atoi(_GetField(row[0],i+1,','));
						m_cbox.SetBoxHide(box,0);//���ݿ����и��� ���Ÿ���
				}
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"ִ��SQLʧ��[%s][%s][%d]",strSQL,m_pDB->GetErrorString(),m_pDB->GetErrorNo());
}
void CTranProDlg::LoadTimeoutBoxFromPostman()
{
	CString strSQL;
	strSQL.Format("SELECT HostTimeoutBox FROM Postman WHERE PostmanID='%s';",m_PostLoginID);
	if(m_pDB->ExeSQLQuery(strSQL))
	{
		if(m_pDB->StoreResult()&&m_pDB->GetRowNumOfSelect()>0)
		{
			MYSQL_ROW row = m_pDB->GetRow();//ֻȡһ�� PostmanID������ �����ظ�
			if(row)
			{
				int count=_GetFieldCount(row[0],',');
				for(int i=0;i<count;++i)
				{
					int box=atoi(_GetField(row[0],i+1,','));
					//int box = m_cbox.GetLogicalBoxByBoxName(_GetField(row[0],i+1,','));//��̨���صĸ��ID�Ǵ���ĸ+���ֵ���ʽ����ת������
					m_cbox.SetBoxHostTimeoutAvailable(box,1);//��̨��ʱ���ܱȱ������ݶ� ����Ӱ���͸�flash������׼ȷ��
				}
			}
		}
		m_pDB->FreeResultSet();
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"ִ��SQLʧ��[%s][%s][%d]",strSQL,m_pDB->GetErrorString(),m_pDB->GetErrorNo());
	}
}
bool CTranProDlg::SaveTimeoutPackageToDB()
{
	CString strSQL;
	int iPackageExist=0;
	int iBoxCount = 0;
	iBoxCount = m_cbox.GetBoxCountTotal();

	for(int i=0;i<iBoxCount;i++)
	{
		if(m_cbox.GetBoxHostTimeoutAvailable(i+1)&&m_cbox.GetBoxLocalPackage(i+1))  //��̨���س�ʱ �� ��������
		{		
			iPackageExist=0;

			strSQL.Format("SELECT * FROM TransQueue WHERE BoxNo='%d'", i+1); 
			if (m_pDB->ExeSQLQuery(strSQL))
			{
				if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
				{
					m_pDB->FreeResultSet();
					m_cbox.SetBoxHostTimeoutAvailable(i+1,0);  //���׶��б����������ݣ�����ʾ��ʱ��
					continue;// 
				}
				m_pDB->FreeResultSet();
			}
			else
				g_Log.WriteLogEx(LOG_JOURNAL,"**��ʱ��������ѯTransQueue���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());
			
			strSQL.Format("UPDATE package SET Flag='2' WHERE BoxNo='%d'",i+1);
			m_pDB->ExeSQLQuery(strSQL);
			if(m_pDB->GetRowNumOfAffected()>0)
			{
				g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]����״̬�ı�->[��ʱ]",i+1);
			}
		}	
	}

	return true;
}
bool CTranProDlg::UpdateDataToDB(int iBoxNo)
{
	bool bRet = false;
	bool bAllSucceed = true;	
	m_pDB->AutoCommit(FALSE);
	CString strSQL;	
	CString strDateTime;
	CTime curTime = CTime::GetCurrentTime();
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());	
	std::vector<PackageInfo> vPackageInfo;	//һ�������������Ԥ��
	std::vector<PackageInfo>::iterator pPackageInfo;
	PackageInfo stPackageInfo;

	if(WORKMODE_POSTMAN_TAKE==m_iWorkMode) //���հ���
	{
		//strSQL.Format("SELECT * FROM Package WHERE BoxNo='%d' AND Flag='2';",iBoxNo);
		strSQL.Format("SELECT * FROM Package WHERE BoxNo='%d';",iBoxNo);
		if (m_pDB->ExeSQLQuery(strSQL))
		{
			if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
			{
				MYSQL_ROW row = NULL;
				while(row = m_pDB->GetRow()) 
				{
					stPackageInfo.empty();
					stPackageInfo.BoxNo = row[0];
					stPackageInfo.PW = row[1];
					stPackageInfo.MailNo = row[2];
					stPackageInfo.InBoxTime = row[3];
					stPackageInfo.Deadline = row[4];
					stPackageInfo.PostmanID = row[5];
					stPackageInfo.CustomerMobile = row[6];
					stPackageInfo.CustomerName = row[7];
					stPackageInfo.CustomerAddress = row[8];
					vPackageInfo.push_back(stPackageInfo);
				}
			}
			else
			{
				m_pDB->FreeResultSet();
				g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯ��ƥ�����ݣ�BoxNo[%d]",iBoxNo);
				m_pDB->AutoCommit(TRUE);
				return false;
			}
			m_pDB->FreeResultSet();
		}
		else
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());
			m_pDB->AutoCommit(TRUE);
			return false;
		}

		for(pPackageInfo=vPackageInfo.begin();pPackageInfo!=vPackageInfo.end();pPackageInfo++)
		{
			strSQL.Format("INSERT INTO Recycle VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '0', null)",
				(*pPackageInfo).BoxNo, (*pPackageInfo).PW, (*pPackageInfo).MailNo, (*pPackageInfo).InBoxTime, (*pPackageInfo).Deadline, 
				(*pPackageInfo).PostmanID, (*pPackageInfo).CustomerMobile, (*pPackageInfo).CustomerName, (*pPackageInfo).CustomerAddress, strDateTime, m_PostLoginID);				
	
			if (m_pDB->ExeSQLQuery(strSQL) && m_pDB->GetRowNumOfAffected()>0)
			{
				//strSQL.Format("DELETE FROM Package WHERE MailNo='%s' AND Flag='2';", (*pPackageInfo).MailNo);
				strSQL.Format("DELETE FROM Package WHERE MailNo='%s';", (*pPackageInfo).MailNo);
				if (m_pDB->ExeSQLQuery(strSQL)) 
					m_pDB->Commit();				
				else
				{
					bAllSucceed = false;		
					m_pDB->Rollback();	
					g_Log.WriteLogEx(LOG_JOURNAL,"**ɾ��Package���ݱ����ʼ�[%s]ʧ��:[%s][%d]",(*pPackageInfo).MailNo,m_pDB->GetErrorString(),m_pDB->GetErrorNo());	
				}
			}
			else
			{
				bAllSucceed = false;	
				g_Log.WriteLogEx(LOG_JOURNAL,"**�����ʼ�[%s]��Recycle���ݱ���ʧ��:[%s][%d]",(*pPackageInfo).MailNo,m_pDB->GetErrorString(),m_pDB->GetErrorNo());
			}				
		}
	}

	else if(WORKMODE_TAKE==m_iWorkMode || WORKMODE_MANAGER_TAKE==m_iWorkMode)
	{
		int iPickupTimeout = 0;
		if(WORKMODE_MANAGER_TAKE==m_iWorkMode)
		{
			m_cIDCardData.idcode=m_PostLoginID;
			m_cIDCardData.name="Postman";			
		}

		strSQL.Format("SELECT * FROM Package WHERE BoxNo='%d';",iBoxNo);
		if(m_pDB->ExeSQLQuery(strSQL))
		{
			if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
			{
				MYSQL_ROW row = NULL;
				while(row = m_pDB->GetRow()) 
				{
					stPackageInfo.empty();
					stPackageInfo.BoxNo = row[0];
					stPackageInfo.PW = row[1];
					stPackageInfo.MailNo = row[2];
					stPackageInfo.InBoxTime = row[3];
					stPackageInfo.Deadline = row[4];
					stPackageInfo.PostmanID = row[5];
					stPackageInfo.CustomerMobile = row[6];
					stPackageInfo.CustomerName = row[7];
					stPackageInfo.CustomerAddress = row[8];
					vPackageInfo.push_back(stPackageInfo);
				}
			}
			else
			{				
				g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯ��ƥ�����ݣ�BoxNo[%d]",iBoxNo);
				m_pDB->AutoCommit(TRUE);
				return false;
			}
			m_pDB->FreeResultSet();
		}
		else
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());
			m_pDB->AutoCommit(TRUE);
			return false;
		}

		for(pPackageInfo=vPackageInfo.begin();pPackageInfo!=vPackageInfo.end();pPackageInfo++)
		{
			strSQL.Format("INSERT INTO pickuphistory VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '0', null)",
				(*pPackageInfo).BoxNo, (*pPackageInfo).PW, (*pPackageInfo).MailNo, (*pPackageInfo).InBoxTime, (*pPackageInfo).Deadline, 
				(*pPackageInfo).PostmanID, (*pPackageInfo).CustomerMobile, (*pPackageInfo).CustomerName, (*pPackageInfo).CustomerAddress,
				strDateTime, m_cIDCardData.idcode, m_cIDCardData.name);				
	
			if (m_pDB->ExeSQLQuery(strSQL) && m_pDB->GetRowNumOfAffected()>0)
			{
				strSQL.Format("DELETE FROM Package WHERE MailNo='%s';", (*pPackageInfo).MailNo);

				if (m_pDB->ExeSQLQuery(strSQL)) 
					m_pDB->Commit();				
				else
				{
					bAllSucceed = false;	
					m_pDB->Rollback();	
					g_Log.WriteLogEx(LOG_JOURNAL,"**ɾ�����ݱ����ʼ�[%s]ʧ��:[%s][%d]",(*pPackageInfo).MailNo,m_pDB->GetErrorString(),m_pDB->GetErrorNo());	
				}
			}
			else
			{
				bAllSucceed = false;	
				g_Log.WriteLogEx(LOG_JOURNAL,"**�����ʼ�[%s]��pickuphistory���ݱ���ʧ��:[%s][%d]",(*pPackageInfo).MailNo,m_pDB->GetErrorString(),m_pDB->GetErrorNo());
			}
		}
	}
	else
		bAllSucceed = true;

	m_pDB->AutoCommit(TRUE);
	bRet = bAllSucceed;

	return bRet;
}
bool CTranProDlg::LoadPackageByPWFromDB(int &iBox,CString csPW,int& iBarCodeCount,CString csBarCode[],CString& csPostmanID,int &iVerifyType,CString &csMobile,CString &csExtraData)
{
	g_Log.WriteLogEx(LOG_JOURNAL,"����[%s]��ȡ������Ϣ",csPW);

	CString strSQL;
	strSQL.Format("SELECT * FROM Package WHERE PW='%s'", csPW);
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			m_iBarCodeCount=(int)m_pDB->GetRowNumOfSelect();
			MYSQL_ROW row = NULL;
			row = m_pDB->GetRow();
			iBox = atoi(row[0]);
			csBarCode[0] = row[2];
			csPostmanID = row[5];
			csMobile=row[6];
			iVerifyType=atoi(row[10]);
			csExtraData=row[12];
			m_pDB->FreeResultSet();
			return true;
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**ȡ��,��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	g_Log.WriteLogEx(LOG_JOURNAL,"����[%s]�ް�����Ϣ",csPW);
	return false;
}
bool CTranProDlg::LoadPackageFromDB(std::vector<PackageInfo>& vstPackageInfo)
{
	g_Log.WriteLogEx(LOG_JOURNAL,"��ȡ���������Ϣ");
	bool bRet=false;
	CString strSQL;
	strSQL.Format("SELECT * FROM Package");
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			bRet = true;
			PackageInfo stPackageInfo;
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				stPackageInfo.BoxNo = row[0];
				stPackageInfo.PW = row[1];
				stPackageInfo.MailNo = row[2];
				stPackageInfo.InBoxTime = row[3];
				stPackageInfo.Deadline = row[4];
				stPackageInfo.PostmanID = row[5];
				stPackageInfo.CustomerMobile = row[6];
				stPackageInfo.CustomerName = row[7];
				stPackageInfo.CustomerAddress = row[8];
				stPackageInfo.ExtraData = row[12];
				vstPackageInfo.push_back(stPackageInfo);
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯ���������Ϣʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	return bRet;
}
bool CTranProDlg::FindPickupPWFromDB(CString PW)
{
	bool bFind = false;
	
	CString strSQL;
	strSQL.Format("SELECT * FROM Package WHERE PW='%s'", PW);
	if (m_pDB->ExeSQLQuery(strSQL) && m_pDB->StoreResult())
	{
		if (m_pDB->GetRowNumOfSelect()>0)
			bFind = true;
		m_pDB->FreeResultSet();
	}

	return bFind;
}
bool CTranProDlg::LoadPackageByBoxIDFromDB(int iBox,CString& csPW,int& iBarCodeCount,CString csBarCode[],CString& csPostmanID,CString& csExtraData)
{
	g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]��ȡ������Ϣ",iBox);
	if(!iBox)
		return false;
		
	CString strSQL;
	strSQL.Format("SELECT * FROM Package WHERE BoxNo='%d'", iBox);
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			row = m_pDB->GetRow();
			csPW = row[1];			
			iBarCodeCount=1;
			csBarCode[0] = row[2];
			csPostmanID = row[5];
			csExtraData = row[12];
			m_pDB->FreeResultSet();
			return true;
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());	

	g_Log.WriteLogEx(LOG_JOURNAL,"�߼���[%d]�ް�����Ϣ",iBox);
	return false;
}
void CTranProDlg::UpdateLocalPackageStatus()
{
	m_cbox.SetBoxLocalPackage();
	int iBoxID;
	int iCheckFail = 0;
	CString strSQL;
	//strSQL.Format("SELECT * FROM Package WHERE Flag='1'");
	strSQL.Format("SELECT * FROM Package");
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				iBoxID = atoi(row[0]);
				if(iBoxID>0&&iBoxID<=m_cbox.GetBoxCountTotal())
					m_cbox.SetBoxLocalPackage(iBoxID,1);					
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��������״̬,��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	strSQL.Format("SELECT * FROM Package WHERE Flag='2'");
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				iBoxID = atoi(row[0]);
				if(iBoxID>0&&iBoxID<=m_cbox.GetBoxCountTotal())
					m_cbox.SetBoxLocalPackage(iBoxID,1);
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��������״̬,��ѯpackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());		
}
CString  CTranProDlg::CheckPackagePhoneInfoFromDB()
{
	CString data;
	PackageInfo stPackageInfo;
	std::vector<PackageInfo> vPackageInfo;	
	std::vector<PackageInfo>::iterator pPackageInfo;

	m_pDB->ExeSQLQuery(_MakeString("SELECT * FROM Package where CustomerMobile='%s'",m_csMobile));
	if (m_pDB->StoreResult()) 
	{
		if(m_pDB->GetRowNumOfSelect()>0)
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				stPackageInfo.BoxNo = m_cbox.GetBoxName(atoi(row[0])); //���
				stPackageInfo.MailNo = row[2];//������
				stPackageInfo.InBoxTime = row[3];//����
				stPackageInfo.InBoxTime.Replace("-","/");
				stPackageInfo.PostmanID = row[5];//postmanid
				vPackageInfo.push_back(stPackageInfo);
			}				
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	data.Empty();
	for(pPackageInfo=vPackageInfo.begin();pPackageInfo!=vPackageInfo.end();pPackageInfo++)
	{
		m_pDB->ExeSQLQuery(_MakeString("SELECT * FROM postman where PostmanID='%s'",pPackageInfo->PostmanID));
		if (m_pDB->StoreResult()) 
		{
			if(m_pDB->GetRowNumOfSelect()>0)
			{
				MYSQL_ROW row = m_pDB->GetRow();
				pPackageInfo->PostmanPhone = row[2];
				pPackageInfo->CompShortName = row[9];			
			
			}
			m_pDB->FreeResultSet();
		}
		else
			g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯpostman���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());	

		//pPackageInfo->PostmanPhone.IsEmpty()?pPackageInfo->PostmanPhone="           ":pPackageInfo->PostmanPhone;
		data+=_MakeString("|%s^%s^%s^%s^%s",pPackageInfo->BoxNo,pPackageInfo->MailNo,pPackageInfo->PostmanPhone,pPackageInfo->InBoxTime,pPackageInfo->CompShortName);	
	}

	return data;	
}
bool CTranProDlg::CheckPackageInfoFromDB(int iBoxNo,CString csMailNo)
{
	bool bRet = false;
	CString strSQL;
	strSQL.Format("SELECT * FROM Package where BoxNo='%d' and MailNo='%s'",iBoxNo,csMailNo);
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
			bRet=true;
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	return bRet;
}
CString CTranProDlg::GetPackageInfoFromDB()
{
	int iCount=0;
	CString data;
	CString csPackageInfo;

	PackageInfo stPackageInfo;
	std::vector<PackageInfo> vPackageInfo;
	std::vector<PackageInfo>::iterator itera;

	PostmanInfo stPostmanInfo;
	std::vector<PostmanInfo> vPostmanInfo;
	std::vector<PostmanInfo>::iterator iteraForPostman;

	CString strSQL;
	if(STATE_MAINTENANCE!=m_iState)//state_maintengance//����ά��
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**������ģʽ[%d]",m_iState);
		return "";
	}		

	strSQL.Format("SELECT * FROM Package order by InBoxTime;");//����Ա ѡ�����а���
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				stPackageInfo.BoxNo=row[0];
				stPackageInfo.MailNo=row[2];
				stPackageInfo.InBoxTime=row[3];
				stPackageInfo.PostmanID=row[5];
				stPackageInfo.CustomerMobile=row[6];
				stPackageInfo.MailStatus=row[9];
				vPackageInfo.push_back(stPackageInfo);
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	strSQL.Format("SELECT * FROM postman;");//��ѯͶ��Ա�����ֻ���
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				stPostmanInfo.PostmanID=row[0];
				stPostmanInfo.Name=row[3];
				stPostmanInfo.Phone=row[4];
				vPostmanInfo.push_back(stPostmanInfo);
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	for(itera=vPackageInfo.begin();itera!=vPackageInfo.end();itera++)
	{
		iCount++;
		strSQL.Format("SELECT * FROM transqueue where MailNo='%s'",itera->MailNo);//����Ա ѡ�����а���
		if (m_pDB->ExeSQLQuery(strSQL))
		{
			if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
				itera->MailStatus="2";
			m_pDB->FreeResultSet();
		}
		else
			g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

		data.Append("|");
		data.Append(itera->BoxNo);  //�������
		data.Append("^");
		data.Append(itera->MailNo);//������
		data.Append("^");
		data.Append(itera->CustomerMobile);  //�ֻ���	
		data.Append("^");
		data.Append(itera->InBoxTime);//����
		data.Append("^");
		switch(atoi(itera->MailStatus))//����״̬
		{
		case 0: data.Append("Ͷ��ʧ��");break;
		case 1: data.Append("��ȡ    ");break;
		case 2: data.Append("���ڴ���");break;
		default:break;
		}
		data.Append("^");
		data.Append(m_cbox.GetBoxName(atoi(itera->BoxNo)));//��ʾ���

		for(iteraForPostman=vPostmanInfo.begin();iteraForPostman!=vPostmanInfo.end();iteraForPostman++)
		{
			if(itera->PostmanID==iteraForPostman->PostmanID)
			{
				data.Append("^");
				data.Append(iteraForPostman->Phone);//Ͷ��Ա����
				data.Append("^");
				data.Append(iteraForPostman->Name);//Ͷ��Ա����
				break;
			}				
		}
		if(iteraForPostman==vPostmanInfo.end())
		{
			data.Append("^^");//ƥ�䲻����Ϊ��
		}
	}
	
	if(0<vPackageInfo.size())
	{
		csPackageInfo.Format("��%d������",vPackageInfo.size());	
		csPackageInfo += data;
	}

	return csPackageInfo;
}
CString CTranProDlg::GetPackageInfoFromDB(CString csPostmanID)
{
	int iCount = 0;
	CString data,csFlashData,csMailStatus;

	PackageInfo stPackageInfo;
	std::vector<PackageInfo> vPackageInfo;
	std::vector<PackageInfo>::iterator iteraForDB,iteraForTrans;

	CString strSQL;
	strSQL.Format("SELECT * FROM Package WHERE PostmanID='%s' order by InBoxTime",csPostmanID);//����Ա ѡ�����а���
	if (m_pDB->ExeSQLQuery(strSQL))
	{
		if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
		{
			MYSQL_ROW row = NULL;
			while(row = m_pDB->GetRow()) 
			{
				stPackageInfo.BoxNo=row[0];
				stPackageInfo.MailNo=row[2];
				stPackageInfo.InBoxTime=row[3];
				stPackageInfo.CustomerMobile=row[6];
				stPackageInfo.MailStatus=row[9];
				vPackageInfo.push_back(stPackageInfo);
			}
		}
		m_pDB->FreeResultSet();
	}
	else
		g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯPackage���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());

	for(iteraForDB=vPackageInfo.begin();iteraForDB!=vPackageInfo.end();iteraForDB++)
	{
		iCount++;
		csMailStatus = "���ڴ���";
		data.Append("|");
		data.Append(iteraForDB->BoxNo);  //�������
		data.Append("^");
		data.Append(iteraForDB->MailNo);//������
		data.Append("^");
		data.Append(iteraForDB->CustomerMobile);  //�ֻ���	
		data.Append("^");
		data.Append(iteraForDB->InBoxTime);//����
		data.Append("^");
		for(iteraForTrans=m_vPackageInfo.begin();iteraForTrans!=m_vPackageInfo.end();iteraForTrans++)
		{
			//if(iteraForDB->BoxNo==iteraForTrans->BoxNo && iteraForDB->MailNo==iteraForTrans->MailNo)
			if(m_cbox.GetBoxName(atoi(iteraForDB->BoxNo))==iteraForTrans->BoxNo && iteraForDB->MailNo==iteraForTrans->MailNo)
			{
				csMailStatus=iteraForTrans->MailStatus;		
				break;
			}
		}
		data+=csMailStatus;
		data.Append("^");
		data.Append(m_cbox.GetBoxName(atoi(iteraForDB->BoxNo)));//��ʾ���
	}

	if(0<vPackageInfo.size())
		csFlashData.Format("��ӭ  %s  ��%d������%s",m_deliveryName,vPackageInfo.size(),data);	

	return csFlashData;
}
bool CTranProDlg::TransactionInsertQueueToDB(CString csTranCode,CString csSerialNO,CString csTranData,int iMaxRetryTime) //���׶���  �������ò���
{
	if(iMaxRetryTime<0)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"������Ӵ���,������Դ����Ƿ�[%d]",iMaxRetryTime);
		return false;
	}
	g_Log.WriteLogEx(LOG_JOURNAL,"�������:%s,��ˮ[%s],�߼�������[%s],���ݳ���[%d],������Դ���[%s]",TrancodeChinese(csTranCode),csSerialNO,csTranCode,csTranData.GetLength(),(RETRY_ENDLESS==iMaxRetryTime)?"����":_MakeString("%d",iMaxRetryTime));
	

	CString strDateTime;
	CTime curTime = CTime::GetCurrentTime();
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d",curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());	

	CString strSQL;
	strSQL.Format("INSERT INTO TransQueue VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%d', '0')",
	m_csCurrentBoxID/*m_iLogicalCurrentBox*/,m_barCode[0],csTranCode,csSerialNO,strDateTime,csTranData,iMaxRetryTime);	

	if (m_pDB->ExeSQLQuery(strSQL) && m_pDB->GetRowNumOfAffected()>0)
	{
		m_pDB->Commit();   //����������޸����ݿ����ݺ�ȫ����commitһ��
		return true;
	}
	else
	{
		m_bReboot = true;
		strSQL.Format("��������:[%s][%s][%s][%s][%s][%s][%d]",m_csCurrentBoxID,m_barCode[0],csTranCode,csSerialNO,strDateTime,csTranData,iMaxRetryTime);
		g_Log.WriteLogEx(LOG_JOURNAL,"**���뽻�׶���TransQueue���ݱ�ʧ��[%s][%d],SerialNO[%s]",m_pDB->GetErrorString(),m_pDB->GetErrorNo(),csSerialNO);	
		g_Log.WriteLog(LOG_JOURNAL,strSQL.GetBuffer(),strSQL.GetLength());			
	}
	
	return false;
}
void CTranProDlg::TransactionQueue()
{
	bool bExecuted = false; //add by yjx 
	int maxRetryTimes,currentRetryTimes;
	CString data;
	CString recv;
	CString serial;
	CString msg;
	CString time;
	int iReturnCode;
	int iCommRet;

	CString strSQL;
	while(STATE_IDLE==m_iState)
	{
		strSQL.Format("SELECT * FROM TransQueue ORDER BY SerialNo");
		if (m_pDB->ExeSQLQuery(strSQL))
		{
			if (m_pDB->StoreResult() && m_pDB->GetRowNumOfSelect()>0) 
			{
				MYSQL_ROW row = NULL;
				//while(row = m_pDB->GetRow()) 
				if(row = m_pDB->GetRow())   //ÿ��ֻ��һ��
				{
					m_barCode[0] = row[1];
					m_tranCode = row[2];
					serial = row[3];
					data = row[5];
					maxRetryTimes = atoi(row[6]);
					currentRetryTimes = atoi(row[7])+1;
				}
			}
			else
			{
				m_pDB->FreeResultSet();
				return;
			}
			m_pDB->FreeResultSet();
		}
		else
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**��ѯTransQueue���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());	
			return;
		}

		g_Log.WriteLogEx(LOG_JOURNAL,"���׶���ѡ��:%s,��ˮ[%s],�߼�������[%s],������Դ���[%s],��ǰ���Դ���[%d]",TrancodeChinese(m_tranCode),serial,m_tranCode,(RETRY_ENDLESS==maxRetryTimes)?"����":_MakeString("%d",maxRetryTimes),currentRetryTimes);
		g_Log.WriteLogEx(LOG_COMMUNICATION,"���׶���ѡ��:%s,��ˮ[%s],�߼�������[%s],������Դ���[%s],��ǰ���Դ���[%d]",TrancodeChinese(m_tranCode),serial,m_tranCode,(RETRY_ENDLESS==maxRetryTimes)?"����":_MakeString("%d",maxRetryTimes),currentRetryTimes);
		//g_Log.WriteLogEx(LOG_TRACE,"���׶���ѡ��:%s,��ˮ[%s],�߼�������[%s],������Դ���[%s],��ǰ���Դ���[%d]",TrancodeChinese(m_tranCode),serial,m_tranCode,(RETRY_ENDLESS==maxRetryTimes)?"����":_MakeString("%d",maxRetryTimes),currentRetryTimes);
			
		iCommRet=Transaction(data,recv,iReturnCode,msg);
		if(-1==iCommRet || -2==iCommRet || -3==iCommRet || -4==iCommRet)
		{//socket�� ���ʹ� 
			g_Log.WriteLogEx(LOG_JOURNAL,"**�ӷ�����ʧ��:%s,��ˮ[%s],�߼�������[%s]",TrancodeChinese(m_tranCode),serial,m_tranCode);
			//g_Log.WriteLogEx(LOG_TRACE,"�ӷ�����ʧ��:%s,��ˮ[%s],�߼�������[%s]",TrancodeChinese(m_tranCode),serial,m_tranCode);
			if(currentRetryTimes==maxRetryTimes)
			{//���Դ����� �����ط�
				g_Log.WriteLogEx(LOG_JOURNAL,"�ӷ������Ѵﵽ������Դ���,�������:%s,��ˮ[%s],�߼�������[%s],������Դ���[%d],��ǰ���Դ���[%d]",TrancodeChinese(m_tranCode),serial,m_tranCode,maxRetryTimes,currentRetryTimes);
				strSQL.Format("DELETE FROM TransQueue WHERE SerialNo='%s';",serial);  //���׳��У����ٷ��͡�
				if (!m_pDB->ExeSQLQuery(strSQL)) 
					g_Log.WriteLogEx(LOG_JOURNAL,"**ɾ��TransQueue���ݱ���ʧ��:[%s][%d],��ˮ��[%s]",m_pDB->GetErrorString(),m_pDB->GetErrorNo(),serial);	
				else
					m_pDB->Commit();
			}
			else
			{
				strSQL.Format("UPDATE TransQueue SET CurrentTryNum='%d' WHERE SerialNo='%s';",currentRetryTimes,serial);
				if (!m_pDB->ExeSQLQuery(strSQL)) 
					g_Log.WriteLogEx(LOG_JOURNAL,"�������Դ���ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());
			}
			break;//����ͨ�Ŵ� �����Ҳ�������� ��֤���׶���FIFO		
		}
		else//��ȷ�����롢���׳ɹ�
		{
/*			if(-4==iCommRet)
				g_Log.WriteLogEx(LOG_JOURNAL,"**����ʧ��,�����ɹ�");
			else */if(-5==iCommRet)
				g_Log.WriteLogEx(LOG_JOURNAL,"**��̨���ؽ���ʧ��,�����ط�");

			strSQL.Format("DELETE FROM TransQueue WHERE SerialNo='%s';",serial);  //���׳��У����ٷ��͡�
			if (!m_pDB->ExeSQLQuery(strSQL)) 
				g_Log.WriteLogEx(LOG_JOURNAL,"**ɾ��TransQueue���ݱ���ʧ��:[%s][%d],��ˮ��[%s]",m_pDB->GetErrorString(),m_pDB->GetErrorNo(),serial);	
			else
				m_pDB->Commit();
		}	
	}//IDLE
}
bool CTranProDlg::SaveResponse(CString csResponse)  //�����̨��������
{
	bool bRet = true;

	//if(csResponse.IsEmpty() && "600002"!=m_tranCode)
	//	return false;

	if("600001"==m_tranCode||"600016"==m_tranCode)   
	{
		if(csResponse.IsEmpty())
			return false;

		CString strSQL;
		csResponse.TrimRight(',');

		m_deliveryName=_GetField(csResponse,1,'|');//deliveryName
		m_compShortName=_GetField(csResponse,2,'|');//compShortName
		CString tmp=csResponse.Right(csResponse.GetLength()-csResponse.Find('|',csResponse.Find('|')+1)-1);//��ȡ������

		if("600001"==m_tranCode)
		{
			SavePostman(m_PostLoginID,m_PostLoginPW);
			strSQL.Format("UPDATE postman SET Name='%s', Others='%s' WHERE PostmanID='%s'",m_deliveryName,m_compShortName,m_PostLoginID);			
			if(!m_pDB->ExeSQLQuery(strSQL))
			{
				bRet = false;
				g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��postman���ݱ�ʧ��[%s]",strSQL);			
			}
		}	
		if("600016"==m_tranCode)
		{
			SavePostman(m_PostLoginID,m_PostLoginPW);
			strSQL.Format("UPDATE postman SET Name='%s', Others='%s',Phone='%s' WHERE PostmanID='%s'",m_deliveryName,m_compShortName,m_csMobile,m_PostLoginID);			
			if(!m_pDB->ExeSQLQuery(strSQL))
			{
				bRet = false;
				g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��postman���ݱ�ʧ��[%s]",strSQL);			
			}
		}
		
		strSQL.Format("UPDATE postman SET HostAvailableBox='%s' WHERE PostmanID='%s'",tmp,m_PostLoginID);
			
		if(!m_pDB->ExeSQLQuery(strSQL))
		{
			bRet = false;
			g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��postman���ݱ�ʧ��[%s]",strSQL);		
		}
	}
	else if("600002"==m_tranCode)  //Ͷ��
	{
		CString strSQL;
		strSQL.Format("UPDATE package SET HaveSendMessage='1' WHERE MailNo='%s'",m_barCode[0]);   //��̨��Ӧ�ɹ���־
		
		if(!m_pDB->ExeSQLQuery(strSQL))
		{
			bRet = false;
			g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��package���ݱ�ʧ��[%s][%d]",m_pDB->GetErrorString(),m_pDB->GetErrorNo());		
		}
	}
	else if("600006"==m_tranCode)  
	{//��ʱ����
		csResponse.TrimRight(',');
		CString strSQL;
		strSQL.Format("UPDATE postman SET HostTimeoutBox='%s' WHERE PostmanID='%s'",csResponse,m_PostLoginID);
		if(!m_pDB->ExeSQLQuery(strSQL))
		{
			bRet = false;
			g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��postman���ݱ�ʧ��[%s]",csResponse);				
		}
	}
	else if("600015"==m_tranCode)
	{
		if(csResponse.IsEmpty())
			return false;

		m_PostLoginID=csResponse;
		SavePostman(m_PostLoginID,m_PostLoginPW);//����Ѵ��ھͻ�ʧ�� ���Ḳ��ԭ��Ϣ
	}
	//else if("600016"==m_tranCode)
	//{
	//	csResponse.TrimRight(',');
	//	m_deliveryName=_GetField(csResponse,1,'|');//deliveryName
	//	m_compShortName=_GetField(csResponse,2,'|');//compShortName
	//	CString tmp=csResponse.Right(csResponse.GetLength()-csResponse.Find('|',csResponse.Find('|')+1)-1);
	//	CString strSQL;
	//	strSQL.Format("UPDATE postman  SET HostAvailableBox='%s' WHERE PostmanID='%s'",tmp,m_PostLoginID);
	//		
	//	if(m_pDB->ExeSQLQuery(strSQL))
	//		bRet = true;
	//	else
	//		g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��postman���ݱ�ʧ��[%s]",tmp);
	//	
	//}
	else if("600007"==m_tranCode)
	{
		if(csResponse.IsEmpty())
			return false;

		CString strSQL;
		csResponse.TrimRight(',');
		m_mailStatus=_GetField(csResponse,1,'|');//mailStatus
		strSQL.Format("UPDATE package SET VerifyType='%s' WHERE MailNo='%s'", m_iVerifyType,m_barCode[0]);			
		if(!m_pDB->ExeSQLQuery(strSQL))
		{
			bRet = false;
			g_Log.WriteLogEx(LOG_JOURNAL,"**�����̨������Ϣ��package���ݱ�ʧ��[%s]",strSQL);		
		}
	}
	else if("600017"==m_tranCode)
	{
		//csResponse.TrimRight(',');
		//m_deliveryName=_GetField(csResponse,1,'|');//deliveryName
		//m_compShortName=_GetField(csResponse,2,'|');//compShortName
		//CString tmp=csResponse.Right(csResponse.GetLength()-csResponse.Find('|',csResponse.Find('|')+1)-1);//��ȡ������
	}
	return bRet;
}


////////////���Ĵ�����////////////////
CString CTranProDlg::GZYZPackTitle()
{
	CString csHead,cstmp;
	csHead.Format("POST /iboxWebService/ws/iboxService?wsdl HTTP/1.1\r\nContent-Type: text/xml; charset=UTF-8\r\n");
	cstmp.Format("Accept: */*\r\nSOAPAction: \"\"\r\nUser-Agent: Apache CXF 2.4.2\r\nCache-Control: no-cache\r\nPragma: no-cache\r\nHost: %s:%d\r\nConnection: keep-alive\r\n",m_localIP,m_iPort);
	csHead+=cstmp;
	return csHead;
}
CString CTranProDlg::GZYZPackInitTerminal()   //1.�ն˳�ʼ��
{
	CString csBrand = GetDataXMLConfig("KingPeace\\Terminal\\Brand");  //Ʒ��
	CString csModel = GetDataXMLConfig("KingPeace\\Terminal\\Model");  //�ͺ�
	CString csLocation = GetDataXMLConfig("KingPeace\\Terminal\\Location");//��װ��ַ
	CString csSize = GetDataXMLConfig("KingPeace\\Box\\CabinetSize");	   //ռ�سߴ�
	if(csSize.IsEmpty())
		csSize = "1";

	char szBoxType[5]={0};
	int iStatus = 0;
	CMarkup cm;
	cm.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	cm.ResetPos();
	_GoIntoXML(cm,"terminal");
	cm.AddElem("terminalNo",m_TermID);//�豸���
	cm.AddElem("initPass",m_csTerminalInitKey);//��ʼ������
	cm.AddElem("brand",csBrand);//Ʒ��
	cm.AddElem("model",csModel);//�ͺ�
	cm.AddElem("location",csLocation);//��װ��ַ
	cm.AddElem("size",csSize);//ռ�سߴ�
	cm.AddElem("cabinetQuantity",m_cbox.GetCabinetCount());//��չ������
	cm.AddElem("quantity",m_cbox.GetBoxCountTotal());//���������
	cm.AddElem("boxs");		

	for(int i=0;i<m_cbox.GetBoxCountTotal();i++)
	{
		cm.AddChildElem("box");
		cm.IntoElem();
		cm.AddChildElem("boxNo",m_cbox.GetBoxName(i+1));
		cm.AddChildElem("boxStatus",0);//״̬��0-���� 1-ռ�� 2-����
		switch(m_cbox.GetClassifyBoxSpec(i+1))
		{
		case 1:sprintf(szBoxType,"��С");break;
		case 2:sprintf(szBoxType,"С");break;
		case 3:sprintf(szBoxType,"��");break;
		case 4:sprintf(szBoxType,"��");break;	
		case 5:sprintf(szBoxType,"����");break;
		//default:sprintf(szBoxType,"N");break;	
		default:sprintf(szBoxType,"��");break;	
		}
		cm.AddChildElem("kind",szBoxType);//�������
		cm.AddChildElem("size",1);//��ڳߴ�
		cm.OutOfElem();
	}
	cm.OutOfElem();

	CString csXML = cm.GetDoc();
	CString csMD5Code = _EncodeMD5((csXML+m_csTerminalKey).GetBuffer());
	csXML = CW2A(CT2W(cm.GetDoc()),CP_UTF8);
	CString csBass64Encode=GZYZDEncodeBass64(csXML,m_iPlainMessage);
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	//_MakeString��1024�ֽ�����
	CString csBody="<soap:Body><ns2:InitTerminalBox xmlns:ns2=\"http://ws.gzpost.com\"><terminalBoxs>"+csBass64Encode+"</terminalBoxs></ns2:InitTerminalBox></soap:Body></soap:Envelope>";
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	
	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackPostLogin()	//2.Ͷ��ԱIC����¼
{
	CString csTmp;
	csTmp += m_TermID;//�����ն�id
	csTmp += m_PostLoginID;//Ͷ��Աid
	csTmp += m_PostLoginPW;//Ͷ��Ա����
	csTmp += m_csTerminalKey;//�����ն��ܳ� 
	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:VerifyOperatorV2 xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><ICCardNo>%s</ICCardNo><password>%s</password></ns2:VerifyOperatorV2></soap:Body></soap:Envelope>",m_TermID,m_PostLoginID,m_PostLoginPW);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	//terminalNo �ն���
	return GZYZPackTitle()+csLength+csHead+csBody;	//���غ��Ҫ���GZYZPackTitle()
}
CString CTranProDlg::GZYZPackPostPhoneLogin()
{
	CString csTmp;
	csTmp += m_TermID;//�����ն�id
	csTmp += m_csMobile;//Ͷ��Ա�ֻ���
	csTmp += m_PostLoginPW;//Ͷ��Ա�ֻ�����
	csTmp += m_csTerminalKey;//�����ն��ܳ�
	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:VerifyOperatorPh1 xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><mobileNo>%s</mobileNo><password>%s</password></ns2:VerifyOperatorPh1></soap:Body></soap:Envelope>",m_TermID,m_csMobile,m_PostLoginPW);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	//terminalNo �ն���
	return GZYZPackTitle()+csLength+csHead+csBody;	//���غ��Ҫ���GZYZPackTitle()
}
CString CTranProDlg::GZYZPackVerification()
{
	CString csTmp;
	csTmp += m_TermID;//�����ն�id
	csTmp += m_PostLoginID;//Ͷ��Ա�˺�
	csTmp += m_csVerifyCode;
	csTmp += m_csTerminalKey;//�����ն��ܳ�
	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:VerifyOperatorPh2 xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><loginCode>%s</loginCode><tempPassword>%s</tempPassword></ns2:VerifyOperatorPh2></soap:Body></soap:Envelope>",m_TermID,m_PostLoginID,m_csVerifyCode);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());

	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackCheckTimeout()		//3.��ѯ��ʱ����
{
	CString csTmp;
	csTmp += m_TermID;
	csTmp += m_PostLoginID;
	csTmp += m_csTerminalKey;
	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:GetExpiredItems xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><loginCode>%s</loginCode></ns2:GetExpiredItems></soap:Body></soap:Envelope>",m_TermID,m_PostLoginID);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());	

	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackTakeTimeout()	//4.ȡ�߳�ʱ����  
{
	CString csTime=GetTimestamp();
	CString csTmp = m_TermID;
	//csTmp += _MakeString("%d",m_iLogicalCurrentBox);
	csTmp += m_csCurrentBoxID;
	csTmp += m_barCode[0];
	csTmp += m_PostLoginID;
	csTmp += csTime.Mid(0,19);
	csTmp += m_csTerminalKey;

	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),csTime,m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:WithdrawExpiredItem xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><boxNo>%s</boxNo><mailNo>%s</mailNo><loginCode>%s</loginCode><outTime>%s</outTime></ns2:WithdrawExpiredItem></soap:Body></soap:Envelope>",m_TermID,m_csCurrentBoxID,m_barCode[0],m_PostLoginID,GetTimestampZone(csTime));
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());	
	
	return GZYZPackTitle()+csLength+csHead+csBody;
}

CString CTranProDlg::GZYZPackGetDeliveryPackageInfo()	//5.��ѯ�����ʼ���Ϣ
{
	CString csTmp = m_TermID;
	csTmp += m_PostLoginID;
	csTmp += m_csTerminalKey;

	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:getDeliveryPackage xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><loginCode>%s</loginCode></ns2:getDeliveryPackage></soap:Body></soap:Envelope>",m_TermID,m_PostLoginID);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	
	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackGetPackageInfo()	//5.��ѯ������Ϣ
{
	CString csTmp = m_TermID;
	csTmp +=m_barCode[0];
	csTmp += m_PostLoginID;
	csTmp += m_csTerminalKey;

	CString csMD5Code=_EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:getPackage xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><mailNo>%s</mailNo><loginCode>%s</loginCode></ns2:getPackage></soap:Body></soap:Envelope>",m_TermID,m_barCode[0],m_PostLoginID);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	
	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackConfirmDelivery()	//7.ȷ��Ͷ��
{
	CString csTime=GetTimestamp();
	CString csTmp = m_TermID;
	//csTmp += _MakeString("%d",m_iLogicalCurrentBox);
	csTmp += m_csCurrentBoxID;
	csTmp += m_barCode[0];
	csTmp += m_PostLoginID;
	csTmp += m_pw;
	csTmp += csTime.Mid(0,19);
	csTmp+=m_cIDCardData.name;
	csTmp+=m_csMobile;
	csTmp+=_MakeString("%d",m_iVerifyType);
	csTmp += m_csTerminalKey;

	CString csMD5Code = _EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),csTime,m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:ConfirmDeliveryV2 xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><boxNo>%s</boxNo><mailNo>%s</mailNo><loginCode>%s</loginCode><openBoxKey>%s</openBoxKey><putTime>%s</putTime><customerName>%s</customerName><customerMobile>%s</customerMobile><mailType>%d</mailType></ns2:ConfirmDeliveryV2></soap:Body></soap:Envelope>",m_TermID,m_csCurrentBoxID,m_barCode[0],m_PostLoginID,m_pw,GetTimestampZone(csTime),m_cIDCardData.name,m_csMobile,m_iVerifyType);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	
	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackCustomerTakeout()	//10.ȡ�߰���
{
	CString csTime = GetTimestamp();
	CString csTmp = m_TermID;
	//csTmp += _MakeString("%d",m_iLogicalCurrentBox);;
	csTmp += m_csCurrentBoxID;
	csTmp += m_barCode[0];
	csTmp += m_cIDCardData.idcode;
	csTmp += csTime.Mid(0,19);
	csTmp += m_cIDCardData.name;
	csTmp += m_csTerminalKey;
	CString csMD5Code = _EncodeMD5(csTmp.GetBuffer());
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),csTime,m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:PickupItem xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><boxNo>%s</boxNo><mailNo>%s</mailNo><idCard>%s</idCard><localTime>%s</localTime><custName>%s</custName></ns2:PickupItem></soap:Body></soap:Envelope>",m_TermID,m_csCurrentBoxID,m_barCode[0],m_cIDCardData.idcode,GetTimestampZone(csTime),m_cIDCardData.name);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	
	return GZYZPackTitle()+csLength+csHead+csBody;
}
CString CTranProDlg::GZYZPackTerminalStatus()	//11.�ն�״̬����
{
	CString csTime = GetTimestamp();
//����xml����
	CMarkup cm;
	cm.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	cm.ResetPos();
	_GoIntoXML(cm,"terminal");
	cm.AddElem("terminalNo",m_TermID);//�豸���
	cm.AddElem("powerStatus",1);		//��Դ
	if(m_stHardWare[DEVICE_LOCKER].iStatus)//����
		cm.AddElem("terminalStatus",2); 
	else
		cm.AddElem("terminalStatus",1);	

	//cm.AddElem("Camera1Status",1);		//����ͷ1
	//cm.AddElem("Camera2Status",1);		//����ͷ2
	//cm.AddElem("Camera3Status",1);		//����ͷ3

	if(!m_stHardWare[DEVICE_DVR].iExist)
	{
		cm.AddElem("camera1Status",0);		//����ͷ1
		cm.AddElem("camera2Status",0);		//����ͷ2
		cm.AddElem("camera3Status",0);		//����ͷ3	
	}
	else if(!m_stHardWare[DEVICE_DVR].iStatus)
	{
		for(int i=0;i<3;i++)
		{
			if(i<m_stHardWare[DEVICE_DVR].iCount)
			{
				if(!m_stDVR_StateInfo.struChanStatic[i].bySignalStatic)
					cm.AddElem(_MakeString("camera%dStatus",i+1),1);	
				else
					cm.AddElem(_MakeString("camera%dStatus",i+1),2);
			}
			else
				cm.AddElem(_MakeString("camera%dStatus",i+1),0);
		}	
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			if(i<m_stHardWare[DEVICE_DVR].iCount)
			{
				cm.AddElem(_MakeString("camera%dStatus",i+1),2);		
			}
			else
				cm.AddElem(_MakeString("camera%dStatus",i+1),0);	
		}	
	}
 
	if(!m_stHardWare[DEVICE_PIN].iExist) //�������
		cm.AddElem("passkeyboardStatus",0);	
	else if(!m_stHardWare[DEVICE_PIN].iStatus)
		cm.AddElem("passkeyboardStatus",1);		
	else
		cm.AddElem("passkeyboardStatus",2); 

	if(!m_stHardWare[DEVICE_IDCR].iExist) //IC��IDˢ����
	{
		cm.AddElem("cardreaderStatus",0);	//������
		cm.AddElem("icStatus",0); 	
		cm.AddElem("idcardStatus",0); 		
	}
	else if(!m_stHardWare[DEVICE_IDCR].iStatus)
	{
		cm.AddElem("cardreaderStatus",1);	
		cm.AddElem("icStatus",1); 	
		cm.AddElem("idcardStatus",1); 		
	}
	else
	{
		cm.AddElem("cardreaderStatus",2);	
		cm.AddElem("icStatus",2); 	
		cm.AddElem("idcardStatus",2); 
	}
	if(!m_stHardWare[DEVICE_BCR].iExist) //������ɨ����
		cm.AddElem("barcodeStatus",0);
	else if(!m_stHardWare[DEVICE_BCR].iStatus)
		cm.AddElem("barcodeStatus",1);
	else
		cm.AddElem("barcodeStatus",2);
	cm.AddElem("softwareVersion",g_AppVersion+"_"+g_RequirementVersion); //����汾��
	cm.AddElem("localTime",GetTimestampZone(GetTimestamp()));		//��ǰʱ��

	CString csXML = CW2A(CT2W(cm.GetDoc()),CP_UTF8);
	CString csMD5Code = _EncodeMD5((csXML+m_csTerminalKey).GetBuffer());
	CString csBass64Encode = GZYZDEncodeBass64(csXML,m_iPlainMessage);  //base64����
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),csTime,m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:ReportTerminalStatus xmlns:ns2=\"http://ws.gzpost.com\"><terminalBoxs>%s</terminalBoxs></ns2:ReportTerminalStatus></soap:Body></soap:Envelope>",csBass64Encode);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());	
	return GZYZPackTitle()+csLength+csHead+csBody;	
}
CString CTranProDlg::GZYZPackBoxStatus()		//12.��ҵ��״̬����
{
	//UpdateBoxStatus();
	//����xml����
	CString csPW,csBarCode[10],csPostmanID,csExtraData;
	int iBarCodeCount;
	int iPhysicalCabinet,iPhysicalBox,iCurrentCabinet=0;
	int iDoorStatus[64]={-1};
	int iBoxStatus=0,iGateLockState=0;
	bool bFind = false;

	std::vector<PackageInfo> vstPackageInfo;
	std::vector<PackageInfo>::iterator iter;
	LoadPackageFromDB(vstPackageInfo);//һ���Ի�ȡ����Ͷ�����ݣ����ⷴ����ѯ���µȴ�ʱ�䳤

	CMarkup cm;
	cm.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	cm.ResetPos();
	_GoIntoXML(cm,"terminal");
	cm.AddElem("terminalNo",m_TermID);//�豸���
	cm.AddElem("boxs");		//��ǰʱ��

	for(int i=0;i<m_cbox.GetBoxCountTotal();i++)
	{
		cm.AddChildElem("box");
		cm.IntoElem();
		//cm.AddChildElem("boxNo",i+1);
		cm.AddChildElem("boxNo",m_cbox.GetBoxName(i+1)); //�������  

		iPhysicalCabinet=m_cbox.GetPhysicalCabinet(i+1);
		iPhysicalBox=m_cbox.GetPhysicalBox(i+1);
		if(iCurrentCabinet!=iPhysicalCabinet) //����ÿ���䶼ȥ�����ڣ����µȴ�ʱ�䳤
		{
			if(m_lockerDll.Status(iPhysicalCabinet,0,iDoorStatus))
				memset(iDoorStatus,1,sizeof(iDoorStatus)); //�����ȡ״̬ʧ�ܣ���Ĭ��ȫ���Ź�	
			iCurrentCabinet=iPhysicalCabinet;
		}

		if(LoadPackageByBoxIDFromDB(i+1,csPW,iBarCodeCount,csBarCode,csPostmanID,csExtraData))
			iBoxStatus=1;//
		else
		{	
			iDoorStatus[iPhysicalBox+31]? iBoxStatus=2 : iBoxStatus=0;
			if(m_cbox.GetAllowOpenFailTimes()<=m_cbox.GetOpenFail(i+1) || m_cbox.GetAllowEmptyFailTimes()<=m_cbox.GetNotEmptyFail(i+1))
				iBoxStatus = 2; //�Ź��ϣ�����3�ο���ʧ�ܻ�����3��Ͷ�ݼ������
		}

		iDoorStatus[iPhysicalBox-1]? iGateLockState=0 : iGateLockState=1;//0:�Ź�  1:�ſ�
		if(m_cbox.GetAllowOpenFailTimes()<=m_cbox.GetOpenFail(i+1))
			iGateLockState = 2; //�Ź��ϣ�����3�ο���ʧ��

		cm.AddChildElem("boxStatus",iBoxStatus);//���ҵ��״̬  0-����  1-ռ��  2-����
		cm.AddChildElem("gateLockStatus",iGateLockState);//��״̬  0-�Ź�  1-�ſ�  2-����

		cm.OutOfElem();
	}
	cm.OutOfElem();

	CString csXML = CW2A(CT2W(cm.GetDoc()),CP_UTF8);
	CString csMD5Code = _EncodeMD5((csXML+m_csTerminalKey).GetBuffer());
	CString csBass64Encode=GZYZDEncodeBass64(csXML,m_iPlainMessage);
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>1.0.0</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	//_MakeString��1024�ֽ�����
	CString csBody="<soap:Body><ns2:ReportTerminalBoxStatus xmlns:ns2=\"http://ws.gzpost.com\"><terminalBoxs>"+csBass64Encode+"</terminalBoxs></ns2:ReportTerminalBoxStatus></soap:Body></soap:Envelope>";
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());
	
	return GZYZPackTitle()+csLength+csHead+csBody;

}
bool CTranProDlg::GZYZUnpackHttp(CString &soap,CString http)
{
	int iPos1,iPos2;
	iPos1 = http.Find("<soap:Envelope");
	iPos2 = http.Find("</soap:Envelope>");
	if(iPos2>=0&&iPos2>iPos1)
	{
		soap=http.Mid(iPos1,iPos2-iPos1+16);
		return true;
	}
	return false;
}
bool CTranProDlg::GZYZUnpackSoap(CString &response,CString soap)
{
	int iPos1,iPos2;
	iPos1 = soap.Find("<return>");
	iPos2 = soap.Find("</return>");
	if(iPos2>=0&&iPos2>iPos1)
	{
		response=soap.Mid(iPos1+8,iPos2-iPos1-8);
		return true;
	}
	return false;
}

CString CTranProDlg::GetTimestamp()
{
	SYSTEMTIME   st={0}; 
	GetLocalTime(&st);
	
	return _MakeString("%04d-%02d-%02d %02d:%02d:%02d.%03d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
}
CString CTranProDlg::GetTimestampZone(CString ts)
{
	ts.Replace(' ','T');
	ts.Append("+08:00");
	return ts;
}
CString CTranProDlg::GZYZDEncodeBass64(CString in,int iPlainMessage)
{
	g_Log.WriteLogEx(LOG_COMMUNICATION,"�ն�����[%d]:",in.GetLength());
	g_Log.WriteLog(LOG_COMMUNICATION,in.GetBuffer(),in.GetLength());
	if(iPlainMessage)
		return in;
	else
		return _Bass64Encode(in.GetBuffer(),in.GetLength());
}
CString CTranProDlg::ANSItoUTF8(CString InData)
{
	int len = InData.GetLength();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, InData, -1, NULL, 0 );   
	wchar_t *pUnicode = new  wchar_t[unicodeLen+1];  
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
	::MultiByteToWideChar(CP_ACP, 0, InData, -1, (LPWSTR)pUnicode, unicodeLen );  //ansiתunicode

	int iTextLen = WideCharToMultiByte( CP_UTF8, 0, pUnicode, -1, NULL, 0, NULL, NULL );
	char* pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8, 0, pUnicode, -1, pElementText, iTextLen, NULL, NULL );//unicodeתutf8 

	CString strText = pElementText;
	delete[] pElementText;
	delete  pUnicode; 
 	return strText;
}
CString CTranProDlg::UTF8toANSI(CString strUTF8)  
{  
    //��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����  
    UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,NULL,NULL);  
    WCHAR *wszBuffer = new WCHAR[nLen+1];  
    nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,wszBuffer,nLen);  
    wszBuffer[nLen] = 0;  
  
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
    CHAR *szBuffer = new CHAR[nLen+1];  
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
    szBuffer[nLen] = 0;  
      
    CString csRet = szBuffer;  
    //�����ڴ�  
    delete []szBuffer;  
    delete []wszBuffer;  
	return csRet;
} 
CString CTranProDlg::GZYZDecodeBase64(CString in,int iPlainMessage)
{
	if(iPlainMessage)
		return in;
	else
		return _UTF8ToANSI(_Base64Decode(in.GetBuffer(),in.GetLength()));
}
bool CTranProDlg::Response(CString plainResponse)
{
	if(!strcmp(m_tranCode,"600001")||!strcmp(m_tranCode,"600016"))	//Ͷ��Ա��¼
	{
		return GZYZPackLoginResponse(plainResponse);	
	}
	else if(!strcmp(m_tranCode,"600008"))	//����
	{
		return GZYZTerminalStatusResponse(plainResponse);
	}
	else if(!strcmp(m_tranCode,"600007"))	//��ѯ������Ϣ
	{
		return GZYZGetPackageResponse(plainResponse);
	}
	else if(!strcmp(m_tranCode,"600006"))	//��ѯ��ʱ����
	{
		return GZYZGetExpiredItemsResponse(plainResponse);
	}
	else if(!strcmp(m_tranCode,"600015"))//Ͷ��Ա�ֻ���¼1
	{
		return GZYZPostPhoneLoginResponse(plainResponse);
	}
	else if(!strcmp(m_tranCode,"600017"))//
	{
		return GZYZPackGetDeliveryPackageResponse(plainResponse);
	}
	else if(!strcmp(m_tranCode,"600018"))//�ն˳�ʼ��
	{
		return GZYZPackInitTerminalResponse(plainResponse);
	}

	return true;
}
bool CTranProDlg::GZYZPackInitTerminalResponse(CString plainResponse)
{
	CString csTerminalKey=_GetDataXMLString("terminalPass",plainResponse);	
	if(!csTerminalKey.IsEmpty())
	{
		if(8<csTerminalKey.GetLength())
			g_Log.WriteLogEx(LOG_JOURNAL,"**�ն����볬��8λ[%d]",csTerminalKey.GetLength());
		m_csTerminalKey = csTerminalKey.Mid(0,8);
		memcpy(m_auHostKey,m_csTerminalKey,8);
		_SetDataXML("KingPeace\\Terminal\\HostKey",m_csTerminalKey,g_ConfigFile);
	}
	return true;
}
bool CTranProDlg::GZYZTerminalStatusResponse(CString plainResponse)
{
	if(m_iUpdateTime)
	{//һ�������ڼ�ֻ��ʱһ��
		m_iUpdateTime=0;
		CString serverTime=_GetDataXMLString("serverTime",plainResponse);				
		if(!serverTime.IsEmpty())
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"��ʱ[%s]",serverTime);
			SYSTEMTIME curr_st;
			ZERO(&curr_st)
			curr_st.wYear = atoi(serverTime.Mid(0,4));
			curr_st.wMonth = atoi(serverTime.Mid(5,2));
			curr_st.wDay = atoi(serverTime.Mid(8,2));
			curr_st.wHour = atoi(serverTime.Mid(11,2));
			curr_st.wMinute = atoi(serverTime.Mid(14,2));
			curr_st.wSecond = atoi(serverTime.Mid(17,2));
			curr_st.wMilliseconds=0;
			if(curr_st.wYear>0 && curr_st.wMonth>0 && curr_st.wDay>0)
				SetLocalTime(&curr_st);
		}
	}

	return true;
}
bool CTranProDlg::GZYZGetPackageResponse(CString plainResponse)
{
	bool bRes=false;
	CString csBarCode = m_barCode[0];  //�������������
	m_csExtraData.Empty();
	CMarkup cm;
	cm.SetDoc(plainResponse);
	cm.ResetPos();	
	if(csBarCode.IsEmpty())
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**�������벻��Ϊ��");
	}
	else if(_GetDataXMLString("Mail\\mailNo",plainResponse)!=csBarCode)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**��̨���ذ������벻��[%s][%s]",csBarCode,_GetDataXMLString("Mail\\mailNo",plainResponse));		
	}
	else
	{
		bRes=true;
		CString tmp;
		m_cIDCardData.name.Empty();
		m_csMobile.Empty();
		m_cIDCardData.address.Empty();
		m_mailStatus.Empty();

		m_cIDCardData.name=_GetDataXMLString("Mail\\customerName",plainResponse);
		m_cIDCardData.address=_GetDataXMLString("Mail\\customerAddress",plainResponse);
		m_csMobile=_GetDataXMLString("Mail\\customerMobile",plainResponse);		
		m_iVerifyType=atoi(_GetDataXMLString("Mail\\virefyType",plainResponse));
		m_mailStatus=_GetDataXMLString("Mail\\mailStatus",plainResponse);
	}		

	return bRes;
}
bool CTranProDlg::GZYZGetExpiredItemsResponse(CString plainResponse)
{
	m_cbox.SetBoxHostTimeoutAvailable();
	m_csExtraData.Empty();
	int iBox;
	CString csBoxNo,csMailNo;
	CMarkup cm;
	cm.SetDoc(plainResponse);
	cm.ResetPos();
	if(_GoIntoXML(cm,"Boxes",1))
	{
		while(cm.FindElem("box"))
		{
			cm.ResetChildPos();
			if(cm.FindChildElem("boxNo"))
				csBoxNo=cm.GetChildData();
			//iBox=atoi(csBoxNo);
			iBox=m_cbox.GetLogicalBoxByBoxName(csBoxNo);

			cm.ResetChildPos();
			if(cm.FindChildElem("mailNo"))
				csMailNo=cm.GetChildData();

			bool bRet = CheckPackageInfoFromDB(iBox,csMailNo);
			
			if(iBox>0 && iBox<=m_cbox.GetBoxCountTotal())
			{//�������Ч��Χ��
				if(bRet)
				{
					m_cbox.SetBoxHostTimeoutAvailable(iBox,1);
					m_cbox.SetBoxHostPackageAvailable(iBox,1);
					m_csExtraData+=_MakeString("%d,",iBox);				
				}
				else
				{
					//m_cbox.SetBoxHide(iBox,1);
					g_Log.WriteLogEx(LOG_JOURNAL,"**����:ǰ��̨���ݲ�һ��! ���[%d],�ʼ���[%s]",iBox,csMailNo);
				}
			}			
		}
	}
	
	return true;
}
bool CTranProDlg::GZYZPostPhoneLoginResponse(CString plainResponse)
{
	m_csExtraData.Empty();
	CMarkup cm;
	cm.SetDoc(plainResponse);
	cm.ResetPos();
	if(cm.FindElem("loginCode"))
	{
		m_csExtraData = cm.GetData();
	}
	return true;
}
bool CTranProDlg::GZYZPackLoginResponse(CString plainResponse)
{
	m_csExtraData.Empty();
	CMarkup cm;
	cm.SetDoc(plainResponse);
	cm.ResetPos();//
	if(cm.FindElem("deliveryName"))
	{
		m_csExtraData = cm.GetData();
	}
	cm.ResetMainPos();//
	if(cm.FindElem("compShortName"))
	{
		m_csExtraData += "|";
		m_csExtraData += cm.GetData();
		m_csExtraData += "|";
	}
	cm.ResetMainPos();
	if(_GoIntoXML(cm,"boxNoes",1))
	{
		while(cm.FindElem("boxNo"))
		{
			//int iBox=atoi(cm.GetData());
			//if(iBox>0&&iBox<=m_cbox.GetBoxCountTotal())
			//{//�������Ч��Χ��
			//	m_csExtraData += _MakeString("%d,",iBox);
			//}		
			m_csExtraData += _MakeString("%s,",cm.GetData());

		}
	}

	return true;
}

bool CTranProDlg::GZYZPackGetDeliveryPackageResponse(CString plainResponse)
{
	CString csTmp;
	PackageInfo stPackageInfo;
	CMarkup cm;

	m_vPackageInfo.clear();
	cm.SetDoc(plainResponse);
	cm.ResetPos();

	if(_GoIntoXML(cm,"mails",1))
	{
		while(cm.FindElem("mail"))
		{
			cm.ResetChildPos();
			cm.FindChildElem("boxNo");
			stPackageInfo.BoxNo=cm.GetChildData();
			cm.ResetChildPos();
			cm.FindChildElem("mailNo");
			stPackageInfo.MailNo=cm.GetChildData();
			cm.ResetChildPos();
			cm.FindChildElem("customerMobile");
			stPackageInfo.CustomerMobile=cm.GetChildData();
			cm.ResetChildPos();
			cm.FindChildElem("putTime");
			csTmp=cm.GetChildData();
			stPackageInfo.InBoxTime=csTmp.Mid(0,10)+" "+csTmp.Mid(11,5);
			cm.ResetChildPos();
			cm.FindChildElem("smsStatus");
			csTmp=cm.GetChildData();

			switch(atoi(csTmp))
			{
			case 0:stPackageInfo.MailStatus="����δ����";break;
			case 1:stPackageInfo.MailStatus="�����ͳ��޻�ִ";break;
			case 2:stPackageInfo.MailStatus="���ŷ���ʧ��";break;
			case 3:stPackageInfo.MailStatus="�����ѳ���";break;
			case 4:stPackageInfo.MailStatus="���ŷ��ͳɹ�";break;	
			default:break;
			}
			m_vPackageInfo.push_back(stPackageInfo);		
		}
	}

	return true;
}

/*
CString CTranProDlg::GZYZTransactionModifyPackageInfo() //6.�޸İ�����Ϣ
{
	CString csMD5Code=_EncodeMD5((m_TermID+m_PostLoginID+m_barCode[0]+m_csMobileModify+m_csTermialKey).GetBuffer());//ֻ�޸��ֻ��� �����ű�����
	CString csHead=_MakeString("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>%s</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",m_Version,GetSerialNOString(),GetTimestamp(),m_TermID,csMD5Code);
	CString csBody=_MakeString("<soap:Body><ns2:updatePackage xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><loginCode>%s</loginCode><mailNo>%s</mailNo><customerMobile>%s</customerMobile></ns2:updatePackage></soap:Body></soap:Envelope>",m_TermID,m_PostLoginID,m_barCode[0],m_csMobileModify);
	CString csLength=_MakeString("Content-Length: %d\r\n\r\n",csHead.GetLength()+csBody.GetLength());

	return GZYZPackTitle()+csLength+csHead+csBody;
}
*/
/*int CTranProDlg::GZYZTransactionConfirmDeposit(CString BoxNo,char *ErrMsg,int MsgbuffLen)	//8.ȷ�ϼĴ� 
{
	g_Log.WriteLogEx(LOG_JOURNAL,"������ˮ[%s],ȷ�ϼĴ�",GetSerialNOString());
	g_Log.WriteLogEx(LOG_COMMUNICATION,"������ˮ[%s],DeliverDeposit",GetSerialNOString());
	g_Log.WriteLogEx(LOG_TRACE,"������ˮ[%s],DeliverDeposit",GetSerialNOString());

	char szTime[64]={0};
	CString csTime = GetTimestamp();
	CString cstmp = m_TermID+BoxNo+m_PostLoginID+csTime.Mid(0,19)+m_csTermialKey;  
	CString csMD5Code = _EncodeMD5(cstmp.GetBuffer());

	CString csHead1 = GZYZPackTitle(); 
	CString csHead2;
	csHead2.Format("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>%s</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",m_Version,GetSerialNOString(),csTime,m_TermID,csMD5Code);
	FreshSerialNO();

	CString csBody;
	csBody.Format("<soap:Body><ns2:DeliverDeposit xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><boxNo>%s</boxNo><loginCode>%s</loginCode><putTime>%s</putTime></ns2:DeliverDeposit></soap:Body></soap:Envelope>",m_TermID,BoxNo,m_PostLoginID,szTime);
	cstmp.Format("Content-Length: %d\r\n\r\n",csHead2.GetLength()+csBody.GetLength());	//<putTime>%s</putTime>  csTime.Mid(0,19)
	CString csSendData = csHead1+cstmp+csHead2+csBody;

	char szRecvData[8192]={0};
	int iRet = SendDataToService(csSendData.GetBuffer(),csSendData.GetLength(),szRecvData,sizeof(szRecvData),1);
	if(iRet>0)
	{
		char szRecv[8192]={0};
		iRet = Unpack(szRecvData, szRecv,sizeof(szRecv));
		if(0>=iRet)	
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**���ʧ��");
			memcpy(ErrMsg,szRecv,min(strlen(szRecv),MsgbuffLen));
			return 0;
		}

		CString csRecv(szRecv);
		if(csRecv.Find("</flag>")<0 || strcmp("1",csRecv.Mid(csRecv.Find("</flag>")-1,1))) 
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**���ұ�ʶλ<flag>ʧ��");
			strcpy(ErrMsg,"����ʧ��");
			return -1;
		}
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**ͨѶ����");
		strcpy(ErrMsg,"ͨѶ����");
		return -1;
	}

	return iRet;
}
int CTranProDlg::GZYZTransactionPostTakeout(CString BoxNo,char *ErrMsg,int MsgbuffLen)	//9.ȡ�߼Ĵ����
{
	g_Log.WriteLogEx(LOG_JOURNAL,"������ˮ[%s],Ͷ��Աȡ�߼Ĵ����",GetSerialNOString());
	g_Log.WriteLogEx(LOG_COMMUNICATION,"������ˮ[%s],deliverPickupItem",GetSerialNOString());
	g_Log.WriteLogEx(LOG_TRACE,"������ˮ[%s],deliverPickupItem",GetSerialNOString());

	CString csTime = GetTimestamp();
	CString cstmp = m_TermID+BoxNo+m_PostLoginID+csTime.Mid(0,19)+m_csTermialKey;  
	CString csMD5Code = _EncodeMD5(cstmp.GetBuffer());

	CString csHead1 = GZYZPackTitle(); 
	CString csHead2;
	csHead2.Format("<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Header><RequestSOAPHeader><version>%s</version><messageId>%s</messageId><timestamp>%s</timestamp><userId>%s</userId><sign>%s</sign></RequestSOAPHeader></soap:Header>",m_Version,GetSerialNOString(),csTime,m_TermID,csMD5Code);
	FreshSerialNO();

	CString csBody;
	csBody.Format("<soap:Body><ns2:deliverPickupItem xmlns:ns2=\"http://ws.gzpost.com\"><terminalNo>%s</terminalNo><boxNo>%s</boxNo><loginCode>%s</loginCode><localTime>%s</localTime></ns2:deliverPickupItem></soap:Body></soap:Envelope>",m_TermID,BoxNo,m_PostLoginID,GetTimestampZone());
	cstmp.Format("Content-Length: %d\r\n\r\n",csHead2.GetLength()+csBody.GetLength());	//<putTime>%s</putTime>  csTime.Mid(0,19)
	CString csSendData = csHead1+cstmp+csHead2+csBody;

	char szRecvData[8192]={0};
	int iRet = SendDataToService(csSendData.GetBuffer(),csSendData.GetLength(),szRecvData,sizeof(szRecvData),1);
	if(iRet>0)
	{
		char szRecv[8192]={0};
		iRet = Unpack(szRecvData, szRecv,sizeof(szRecv));
		if(0>=iRet)	
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**���ʧ��");
			memcpy(ErrMsg,szRecv,min(strlen(szRecv),MsgbuffLen));
			return 0;
		}

		CString csRecv(szRecv);
		if(csRecv.Find("</flag>")<0 || strcmp("1",csRecv.Mid(csRecv.Find("</flag>")-1,1))) 
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"**���ұ�ʶλ<flag>ʧ��");
			strcpy(ErrMsg,"����ʧ��");
			return -1;
		}
	}
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**ͨѶ����");
		strcpy(ErrMsg,"ͨѶ����");
		return -1;
	}

	return iRet;
}*/
