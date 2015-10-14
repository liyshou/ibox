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
	//密码键盘
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
	void EmptyTransMsg();//清空交易变量信息 避免上一个
	CString BoxSpecChinese(int spec);
	bool CheckNetWork();
		
	void Loop();
	void MySleep(DWORD dwMilliseconds);	
	void SleepDebug(DWORD);
	void SleepRelease(DWORD);

	void SelfTest();
	void InitAllDevices();//初始化所有设备 直至所有关键设备成功
	void ResetAllDevices();//初始化所有设备一次;
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
	bool OpenLogicalBox(int iLogicalBoxID);//开门
	bool CloseLogicalBox(int iLogicalBoxID,int& iIsPutPackage);//检测关门
	bool IsEmptyLogicalBox(int iLogicalBoxID);//检测货物

	CString TransformBoxNoToBoxID(int iLogicalBoxID);//转换箱号
	int TransformBoxIDToBoxNo(CString csBoxID);

	//数据库操作
	CDataBase* m_pDB;
	bool InitDB();  //初始化时调用
	void DisconnectDB();  //程序退出时调用
	bool ExeSQLQuery(const char* sqlString);
	bool CheckMailNo(CString csMailNo);	//检查条码是否已存在，扫描完条码后检验重复性
	bool CheckBoxNo(int iBox);	//检查数据库中是否已有该箱号的投递数据
	bool SavePackageToDB(int iBox,CString csPW,int iBarCodeCount,CString csBarCode[],CString csPostLoginID,CString csName,CString csAddress,CString csMobile,int iDeadline,int iVerifyType=2,CString csExtraData="");  //确认投递后保存包裹信息
	bool SaveTimeoutPackageToDB();	//保存后台返回的超时包裹，单机版不适用
	void LoadTimeoutBoxFromPostman();//从postman读超时包裹信息
	void LoadAvailableBoxFromPostman();
	bool UpdateDataToDB(int iBoxNo=0); //更新数据库数据，开门取件后等情况使用
	bool LoadPackageByBoxIDFromDB(int iBox,CString& csPW,int& iBarCodeCount,CString csBarCode[],CString &csPostmanID,CString &csExtraData); //取件交易时，通过箱号获取包裹数据进行校验
	bool LoadPackageByPWFromDB(int &iBox,CString csPW,int& iBarCodeCount,CString csBarCode[],CString &csPostmanID,int &iVerifyType,CString &csMobile,CString &csExtraData); //取件交易时，通过密码获取包裹数据进行校验
	bool LoadPackageFromDB(std::vector<PackageInfo>& vstPackageInfo);
	bool FindPickupPWFromDB(CString PW); //投递生成取件密码时，检查密码的重复性
	void UpdateLocalPackageStatus();  //更新本地的存件数据
	bool UpdateTimeoutBoxToPostman(int iBoxNo);
	CString GetPackageInfoFromDB();	//获取本地包裹信息  管理员取件时调用
	CString GetPackageInfoFromDB(CString csPostmanID);	//获取本地包裹信息  管理员取件时调用
	CString CheckPackagePhoneInfoFromDB();//查询本地包裹信息   客户取件时调用
	bool CheckPackageInfoFromDB(int iBoxNo,CString MailNo);  //查询数据库中是否有对应的数据
	bool TransactionInsertQueueToDB(CString csTranCode,CString csSerialNO,CString csTranData,int iMaxRetryTime=RETRY_ENDLESS); //交易队列  单机版用不上
	void TransactionQueue();
	bool SaveResponse(CString csResponse);  //保存后台返回数据
	//本地验证密码专用
	bool SavePostman(CString id,CString pw);//保存管理员
	bool DeletePostman(CString id);
	bool VerifyPostman(CString id,CString pw);

private:
	//线程同步变量
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
	bool m_bIsKeyBoardPause;//键盘暂停
	
	bool m_bIsInited;
	bool m_IsLoadConfigFailed;
	int m_iIsUpdateingBoxStatus;
	int m_iIsUpdateingStatusLocker;	
	int m_iIsSending;	
	

	//终端参数
	CString m_Version;
	CString m_TermID;
	CString m_BankID;
	CString m_csProfile;//机器信息	
	CString m_localIP;
	CString m_IP;	
	int m_iPort;
	CString m_localMAC;	
	STHardWare m_stHardWare[DEVICE_END+1];
	CBox m_cbox;
	BYTE m_auHostKey[8];//广州邮政后台通信密钥 用于加密交易信息：取件密码。
	BYTE m_auLocalKey[8];//用于加密本地密码：管理员登陆密码、退出密码。	
	CString m_csManagePW;//管理密码 密文
	CString m_csExitPW;//退出密码 密文
	CString m_csTerminalKey;//终端通讯密码
	CString m_csDBIP;
	CString m_csDBUser;
	CString m_csDBPassword;
	CString m_csDBName;
	int m_iDBPort;
	long m_lDVR_UserID;
	NET_DVR_WORKSTATE_V30 m_stDVR_StateInfo;

	//条件参数
	int m_iPlainMessage;
	int m_iUpdateTime;
	int m_iRebootTime;
	int m_iMonitorInterval;//设备状态报告间隔
	int m_iBoxMonitorInterval;//格口状态报告间隔
	int m_iFlashHeartbeatInterval;
	int m_iTransactionQueueInterval;
	int m_iSelfCheckInterval;//自检间隔	
	int m_iIsPostDelaySend;//投递交易延时处理
	int m_iIsPostFailOpenDoor;//投递交易实时处理失败开箱
	int m_iIsTakeoutDelaySend;//取件交易延时处理
	int m_iIsLoginInOffLine;//脱机登陆
	int m_iLoginInOnLineWhileOffLineFail;//脱机登陆失败后联机登陆
	int m_iAllowRepeatIDNO;
	int m_iNetTimeout;
	int m_iDownloadOffLineData;
	int m_iDownloadDay;//下载脱机数据的天数
	int m_iWorkMode;
	int m_iState;
	int m_iNeedRegister;
	bool m_bSendTrans;
	bool m_bNetWorkFail;
	bool m_bRecvFlashHeartbeat;
	bool m_bSendBoxStatus;
	bool m_bReboot; //数据库异常时重启主机（插入包裹表或队列表失败）
	CCriticalSection critical_FlashHeartbeat;
		

	//交易参数
	CString m_csTerminalInitKey;//终端初始化密码
	CString m_pw;//取件密码 密文
	CString m_PostLoginID;		//投递员登录ID
	CString m_PostLoginPW;		//投递员登陆密码
	CString m_compShortName;//物流公司名
	CString m_deliveryName;//投递员名字
	CString m_mailStatus;	//邮件状态
	CString m_smsStatus;	//短信状态
	CString m_tranCode;	
	CString m_csVerifyCode;
	CString m_postmanID;//投递人员的编号
	CString m_barCode[10];
	CString m_csMobile;
	int m_iBarCodeCount;
	int m_iDeadline;  //包裹超时时间
	int m_iSerial;
	CIDCardData m_cIDCardData;
	int m_iLogicalCurrentBox;//当前正在操作开关门的逻辑箱号 从1开始
	int m_iCurrentOpenBox;	 //当前已打开的箱
	CString m_csCurrentBoxID;//当前正在操作开关门的箱名称
	int m_iVerifyType;
	CString m_csExtraData;  
	std::vector<PackageInfo> m_vPackageInfo;
	std::map<int,int> m_vBoxDoorStatus; //格式：箱号，门状态（0—正常  1—投递员回收开箱失败	2—客户取件开箱失败) 

	
	//////////报文处理部分///////////////
	CString Pack();
	int Unpack(const char* szInData,CString& csMsg);
	CString GZYZPackTitle();
	CString GZYZPackInitTerminal();					//1.终端初始化     
	CString GZYZPackPostLogin();					//2.投递员登录
	CString GZYZPackPostPhoneLogin();				//投递员手机登录
	CString GZYZPackCheckTimeout();					//3.查询超时包裹
	CString GZYZPackTakeTimeout();					//4.取走超时包裹
	CString GZYZPackGetPackageInfo();				//5.查询包裹信息
	CString GZYZPackGetDeliveryPackageInfo();			//6查询包裹邮件信息
	//CString GZYZPackModifyPackageInfo();				//6.修改包裹信息
	CString GZYZPackConfirmDelivery();				//7.确认投递
	//int GZYZPackConfirmDeposit(CString BoxNo,char *ErrMsg,int MsgbuffLen);			//8.确认寄存  	  无用
	//int GZYZPackPostTakeout(CString BoxNo,char *ErrMsg,int MsgbuffLen);			//9.取走寄存包裹  无用
	CString GZYZPackCustomerTakeout();				//10.取走包裹
	CString GZYZPackTerminalStatus();				//11.终端状态报告
	CString GZYZPackBoxStatus();						//12.箱业务状态报告
	CString GZYZPackVerification();						//动态码验证
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
