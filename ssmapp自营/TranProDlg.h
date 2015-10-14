//{{AFX_INCLUDES()
//#include "stmdevice.h"
//#include "BankForm.h"

//}}AFX_INCLUDES
#if !defined(AFX_TRANPRODLG_H__1E402F5B_864F_4F4F_9443_76F8FF8544A7__INCLUDED_)
#define AFX_TRANPRODLG_H__1E402F5B_864F_4F4F_9443_76F8FF8544A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranProDlg.h : header file
//

#include "DataBase.h"
#include "OIDCardReader.h"
#include "OIDCardReaderDef.h"
#include "CoHandWrite.h"
#include "COMDll.h"
#include "pubVar.h"
#include "DataType.h"
#include "DecodeCardSdk.h"
#include "HCNetSDK.h"
#include "plaympeg4.h"

/////////////////////////////////////////////////////////////////////////////
// CTranProDlg dialog

class CTranProDlg : public CDialog
{
// Construction
public:
	CTranProDlg(CWnd* pParent = NULL);   // standard constructor
	~CTranProDlg();

// Dialog Data
	//{{AFX_DATA(CTranProDlg)
	enum { IDD = IDD_TRANPRO_DIALOG };
//	CSTMDevice	m_device;
	OIDCardReader m_oIDCardReader;
	LockerDll m_lockerDll;
	CodeBarScanerDll m_CodeBarScaner;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranProDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTranProDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDestroy();		
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//�������
	CCOMDll m_pincom;

	static UINT __ReadIDCard(LPVOID lpParam);
	UINT ReadIDCard();
	static UINT __ReadICCard(LPVOID lpParam);
	UINT ReadICCard();
	static UINT __CloseDoor(LPVOID lpParam);
	UINT CloseDoor();
	static UINT __ReadBarCode(LPVOID lpParam);
	UINT ReadBarCode();
	static UINT __MessageFromFlash(LPVOID lpParam);
	UINT MessageFromFlash();	
	static UINT __DeviceSelfChecking(LPVOID lpParam);
	UINT DeviceSelfChecking();
	static UINT __NetworkMonitor(LPVOID lpParam);
	UINT NetworkMonitor();
	static UINT __ReadKeyBoard(LPVOID lpParam);
	UINT ReadKeyBoard();
	static UINT __CheckOpenDoor(LPVOID lpParam);
	UINT CheckOpenDoor();
	//static UINT __CheckInputPackage(LPVOID lpParam);
	//UINT CheckInputPackage();	

	void TRSHOW(char* szOutBuf,...);
	void TRSHOW2(CString cs);
	int m_iFlashSerial;
	CString GetFlashSerial();
	int LoadConfig();
	int VerifyConfig();
	int Init();
	void OnBeginTran(CString csCommand,CString csParameter);
	void ExecFunc(CString csCommand,CString csParameter);
	void ExitSsmApp(void);

	//xml current class tool set
	CString GetDataXMLConfig(CString tree);
	int GetDataIntXMLConfig(CString tree);
	void SetDataXMLConfig(CString tree,CString value);	
	void DeleteDataXMLConfig(CString tree);
	//xml current class tool set

	//TOOL
	CString DesEncryption(CString InData,BYTE* key);
	CString DesDecryption(CString InData,BYTE* key);
	CString XDEncryption(CString InData,BYTE* key);
	CString ANSItoUTF8(CString InData);
	CString UTF8toANSI(CString InData);

	//TOOL end

	//transaction
	CString CreatePW();
	CString CreatePickupPW();
	CString TrancodeChinese(CString trancode);
	int SendDataToService(char *pSendData,int iSendLen,char *pRecvData,int iRecvLen,int nNetTimeout,int iRecvFlag = 1);
	int Transaction(CString send,CString &recv,int &iReturnCode,CString &msg,bool bInsertQueueWhileFail=false);

	void FreshSerialNO();
	CString GetSerialNOString();
	void PostmanPhoneLogin();
	void PostmanLoginState();
	void PhoneLoginVerificationCodeState();
	void PostLoginState();
	void CheckPackageInfoState();
	//void ModifyPackageInfoState();
	void DeliverGetPackageInfoState();
	void ConfirmDepositState();
	void VerificationCodeState();
	void OpenDoorState(int iMode,CString Value);
	bool OpenDoorByIDState(int iID);
	bool OpenDoorBySpecState(int iFlashSpec);
	void EmptyTransMsg();//��ս��ױ�����Ϣ ������һ��
	CString BoxSpecChinese(int spec);
	bool CheckNetWork();
		
	void Loop();
	void MySleep(DWORD dwMilliseconds);	
	void SleepDebug(DWORD);
	void SleepRelease(DWORD);

	void SelfTest();
	void InitAllDevices();//��ʼ�������豸 ֱ�����йؼ��豸�ɹ�
	void ResetAllDevices();//��ʼ�������豸һ��;
	int InitDevice(int deivce);
	int InitLocker();
	int InitIDCardReader();
	int InitBarcodeReader();
	int InitPIN();
	int InitDVR();
	void UpdateStatusDevice(int deivce);
	void UpdateStatusLocker();
	void UpdateStatusIDCardReader();
	void UpdateStatusBarcodeReader();
	void UpdateStatusPIN();
	void UpdateStatusDVR();
	void UpdateBoxStatus();
	bool OpenLogicalBox(int iLogicalBoxID);//����
	bool CloseLogicalBox(int iLogicalBoxID,int& iIsPutPackage);//������
	bool IsEmptyLogicalBox(int iLogicalBoxID);//������

	CString TransformBoxNoToBoxID(int iLogicalBoxID);//ת�����
	int TransformBoxIDToBoxNo(CString csBoxID);

	//���ݿ����
	CDataBase* m_pDB;
	bool InitDB();  //��ʼ��ʱ����
	void DisconnectDB();  //�����˳�ʱ����
	bool ExeSQLQuery(const char* sqlString);
	bool CheckMailNo(CString csMailNo);	//��������Ƿ��Ѵ��ڣ�ɨ�������������ظ���
	bool CheckBoxNo(int iBox);	//������ݿ����Ƿ����и���ŵ�Ͷ������
	bool SavePackageToDB(int iBox,CString csPW,int iBarCodeCount,CString csBarCode[],CString csPostLoginID,CString csName,CString csAddress,CString csMobile,int iDeadline,int iVerifyType=2,CString csExtraData="");  //ȷ��Ͷ�ݺ󱣴������Ϣ
	bool SaveTimeoutPackageToDB();	//�����̨���صĳ�ʱ�����������治����
	void LoadTimeoutBoxFromPostman();//��postman����ʱ������Ϣ
	void LoadAvailableBoxFromPostman();
	bool UpdateDataToDB(int iBoxNo=0); //�������ݿ����ݣ�����ȡ��������ʹ��
	bool LoadPackageByBoxIDFromDB(int iBox,CString& csPW,int& iBarCodeCount,CString csBarCode[],CString &csPostmanID,CString &csExtraData); //ȡ������ʱ��ͨ����Ż�ȡ�������ݽ���У��
	bool LoadPackageByPWFromDB(int &iBox,CString csPW,int& iBarCodeCount,CString csBarCode[],CString &csPostmanID,int &iVerifyType,CString &csMobile,CString &csExtraData); //ȡ������ʱ��ͨ�������ȡ�������ݽ���У��
	bool LoadPackageFromDB(std::vector<PackageInfo>& vstPackageInfo);
	bool FindPickupPWFromDB(CString PW); //Ͷ������ȡ������ʱ�����������ظ���
	void UpdateLocalPackageStatus();  //���±��صĴ������
	bool UpdateTimeoutBoxToPostman(int iBoxNo);
	CString GetPackageInfoFromDB();	//��ȡ���ذ�����Ϣ  ����Աȡ��ʱ����
	CString GetPackageInfoFromDB(CString csPostmanID);	//��ȡ���ذ�����Ϣ  ����Աȡ��ʱ����
	CString CheckPackagePhoneInfoFromDB();//��ѯ���ذ�����Ϣ   �ͻ�ȡ��ʱ����
	bool CheckPackageInfoFromDB(int iBoxNo,CString MailNo);  //��ѯ���ݿ����Ƿ��ж�Ӧ������
	bool TransactionInsertQueueToDB(CString csTranCode,CString csSerialNO,CString csTranData,int iMaxRetryTime=RETRY_ENDLESS); //���׶���  �������ò���
	void TransactionQueue();
	bool SaveResponse(CString csResponse);  //�����̨��������
	//������֤����ר��
	bool SavePostman(CString id,CString pw);//�������Ա
	bool DeletePostman(CString id);
	bool VerifyPostman(CString id,CString pw);

private:
	//�߳�ͬ������
	bool m_bIsCancelReadICCard;	
	bool m_bIsCancelReadIDCard;
	bool m_bIsRunningReadIDCard;
	bool m_bIsRunningReadICCard;
	bool m_bIsCancelCloseDoor;
	bool m_bIsCancelCheckOpenDoor;
	bool m_bIsRunningKeyboard;
	
	bool m_bIsCancelBarCode;
	bool m_bIsBarcodeOpening;
	bool m_bIsClosingDoor;
	bool m_bIsCheckingOpenDoor;
	bool m_bIsKeyBoardPause;//������ͣ
	
	bool m_bIsInited;
	bool m_IsLoadConfigFailed;
	int m_iIsUpdateingBoxStatus;
	int m_iIsUpdateingStatusLocker;	
	int m_iIsSending;	
	

	//�ն˲���
	CString m_Version;
	CString m_TermID;
	CString m_BankID;
	CString m_csProfile;//������Ϣ	
	CString m_localIP;
	CString m_IP;	
	int m_iPort;
	CString m_localMAC;	
	STHardWare m_stHardWare[DEVICE_END+1];
	CBox m_cbox;
	BYTE m_auHostKey[8];//����������̨ͨ����Կ ���ڼ��ܽ�����Ϣ��ȡ�����롣
	BYTE m_auLocalKey[8];//���ڼ��ܱ������룺����Ա��½���롢�˳����롣	
	CString m_csManagePW;//�������� ����
	CString m_csExitPW;//�˳����� ����
	CString m_csTerminalKey;//�ն�ͨѶ����
	CString m_csDBIP;
	CString m_csDBUser;
	CString m_csDBPassword;
	CString m_csDBName;
	int m_iDBPort;
	long m_lDVR_UserID;
	NET_DVR_WORKSTATE_V30 m_stDVR_StateInfo;

	//��������
	int m_iPlainMessage;
	int m_iUpdateTime;
	int m_iRebootTime;
	int m_iMonitorInterval;//�豸״̬������
	int m_iBoxMonitorInterval;//���״̬������
	int m_iFlashHeartbeatInterval;
	int m_iTransactionQueueInterval;
	int m_iSelfCheckInterval;//�Լ���	
	int m_iIsPostDelaySend;//Ͷ�ݽ�����ʱ����
	int m_iIsPostFailOpenDoor;//Ͷ�ݽ���ʵʱ����ʧ�ܿ���
	int m_iIsTakeoutDelaySend;//ȡ��������ʱ����
	int m_iIsLoginInOffLine;//�ѻ���½
	int m_iLoginInOnLineWhileOffLineFail;//�ѻ���½ʧ�ܺ�������½
	int m_iAllowRepeatIDNO;
	int m_iNetTimeout;
	int m_iDownloadOffLineData;
	int m_iDownloadDay;//�����ѻ����ݵ�����
	int m_iWorkMode;
	int m_iState;
	int m_iNeedRegister;
	bool m_bSendTrans;
	bool m_bNetWorkFail;
	bool m_bRecvFlashHeartbeat;
	bool m_bSendBoxStatus;
	bool m_bReboot; //���ݿ��쳣ʱ����������������������б�ʧ�ܣ�
	CCriticalSection critical_FlashHeartbeat;
		

	//���ײ���
	CString m_csTerminalInitKey;//�ն˳�ʼ������
	CString m_pw;//ȡ������ ����
	CString m_PostLoginID;		//Ͷ��Ա��¼ID
	CString m_PostLoginPW;		//Ͷ��Ա��½����
	CString m_compShortName;//������˾��
	CString m_deliveryName;//Ͷ��Ա����
	CString m_mailStatus;	//�ʼ�״̬
	CString m_smsStatus;	//����״̬
	CString m_tranCode;	
	CString m_csVerifyCode;
	CString m_postmanID;//Ͷ����Ա�ı��
	CString m_barCode[10];
	CString m_csMobile;
	int m_iBarCodeCount;
	int m_iDeadline;  //������ʱʱ��
	int m_iSerial;
	CIDCardData m_cIDCardData;
	int m_iLogicalCurrentBox;//��ǰ���ڲ��������ŵ��߼���� ��1��ʼ
	int m_iCurrentOpenBox;	 //��ǰ�Ѵ򿪵���
	CString m_csCurrentBoxID;//��ǰ���ڲ��������ŵ�������
	int m_iVerifyType;
	CString m_csExtraData;  
	std::vector<PackageInfo> m_vPackageInfo;
	std::map<int,int> m_vBoxDoorStatus; //��ʽ����ţ���״̬��0������  1��Ͷ��Ա���տ���ʧ��	2���ͻ�ȡ������ʧ��) 

	
	//////////���Ĵ�����///////////////
	CString Pack();
	int Unpack(const char* szInData,CString& csMsg);
	CString GZYZPackTitle();
	CString GZYZPackInitTerminal();					//1.�ն˳�ʼ��     
	CString GZYZPackPostLogin();					//2.Ͷ��Ա��¼
	CString GZYZPackPostPhoneLogin();				//Ͷ��Ա�ֻ���¼
	CString GZYZPackCheckTimeout();					//3.��ѯ��ʱ����
	CString GZYZPackTakeTimeout();					//4.ȡ�߳�ʱ����
	CString GZYZPackGetPackageInfo();				//5.��ѯ������Ϣ
	CString GZYZPackGetDeliveryPackageInfo();			//6��ѯ�����ʼ���Ϣ
	//CString GZYZPackModifyPackageInfo();				//6.�޸İ�����Ϣ
	CString GZYZPackConfirmDelivery();				//7.ȷ��Ͷ��
	//int GZYZPackConfirmDeposit(CString BoxNo,char *ErrMsg,int MsgbuffLen);			//8.ȷ�ϼĴ�  	  ����
	//int GZYZPackPostTakeout(CString BoxNo,char *ErrMsg,int MsgbuffLen);			//9.ȡ�߼Ĵ����  ����
	CString GZYZPackCustomerTakeout();				//10.ȡ�߰���
	CString GZYZPackTerminalStatus();				//11.�ն�״̬����
	CString GZYZPackBoxStatus();						//12.��ҵ��״̬����
	CString GZYZPackVerification();						//��̬����֤
	CString GetTimestamp();
	CString GetTimestampZone(CString ts);
	CString GZYZDEncodeBass64(CString in,int iPlainMessage);
	CString GZYZDecodeBase64(CString in,int iPlainMessage);
	bool GZYZUnpackHttp(CString &soap,CString http);
	bool GZYZUnpackSoap(CString &response,CString soap);

	bool Response(CString plainResponse);
	bool GZYZTerminalStatusResponse(CString plainResponse);
	bool GZYZGetPackageResponse(CString plainResponse);
	bool GZYZGetExpiredItemsResponse(CString plainResponse);
	bool GZYZPostPhoneLoginResponse(CString plainResponse);
	bool GZYZPackGetDeliveryPackageResponse(CString plainResponse);
	bool GZYZPackLoginResponse(CString plainResponse);
	bool GZYZPackInitTerminalResponse(CString plainResponse);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANPRODLG_H__1E402F5B_864F_4F4F_9443_76F8FF8544A7__INCLUDED_)
