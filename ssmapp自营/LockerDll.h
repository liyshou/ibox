#include "stdafx.h"

#pragma once

#ifdef LOCKERDLL_EXPORTS
#define LOCKERDLL_API __declspec(dllexport)
#else
#define LOCKERDLL_API __declspec(dllimport)
#endif
// LockerDll

class LockerDll : public CWnd
{
	DECLARE_DYNAMIC(LockerDll)

public:
	LOCKERDLL_API LockerDll();
	LOCKERDLL_API virtual ~LockerDll();

protected:
	DECLARE_MESSAGE_MAP()

public:
	LOCKERDLL_API CString GetDLLVersion();
	LOCKERDLL_API int GetDeviceLastError();
	//Dev
	LOCKERDLL_API int InitComm(int port);
	LOCKERDLL_API int CloseComm();
	LOCKERDLL_API int Reset(int &cabinetCount,int* pCCabinetID);//20个柜 32个门 32个包裹传感器
	LOCKERDLL_API int OpenDoor(int iCabinet,int iCase,int *iStatus);//iCase=0开所有门 iStatus是64字节；iCase!=0开一个门 iStatus是2字节
	LOCKERDLL_API int Status(int iCabinet,int iCase,int *iStatus);
private:
	void LRC(BYTE lrc[2],BYTE *str,int len);
	void Convert(BYTE b[4],int i[32]);//低到高
	int iDeviceError;
	int m_iSerial;
	bool m_bInOperate;
	//HANDLE m_hComm;
	//bool SetRWTimeout(int msRITO, int msRTTOM, int msRTTOC, int msWTTOM, int msWTTOC);
	//int ReadCom(BYTE* recv,DWORD &len,DWORD iTimeOut);
	//int WriteCom(BYTE* send,DWORD len,DWORD iTimeOut);
	//int Communication(BYTE* send,DWORD sLen,BYTE* recv,DWORD &rLen,DWORD iTimeOut);
};

#define ERR_UNKNOWN					-1
#define ERR_OK						0
#define ERR_UNKNOWN_DEVICE			1
#define ERR_INVALID_PORT			2
#define ERR_ALREADY_SET_TYPE		3	//重复设置设备类型
#define ERR_EXECUTE					4	//执行失败
#define ERR_COM						5	//串口错
#define ERR_ABORT					6	//中断
#define ERR_COMMUNICATION			7	//通讯失败
#define ERR_PARAMETER				8	//参数错误
#define ERR_UNSUPPORT				9	//命令不能执行
#define ERR_CARDTYPE				10	//错误的卡
#define ERR_DLL						11	//加载动态库失败
#define ERR_DLLDIRECTORY			12	//设置动态库目录失败
#define ERR_INOPERATE				13	//执行上一个操作中
#define ERR_TIMEOUT					14	//超时
#define ERR_BUFFER_SMALL			15	//缓冲区太小
#define ERR_VERIFY					16	//数据校验失败
#define ERR_LRC						17	//校验LRC失败
#define ERR_INOPERATION				18	//设备忙 上一操作中
//设备返回码
#define ERR_DEVICE_OK				0x30  //操作成功
#define ERR_DEVICE_INVALID_ID		0x33  //ID非法
#define ERR_DEVICE_TIME_LIMIT		0x34  //时间设置错误或非法
#define ERR_DEVICE_UNKNOWN			0x36  //其他错误，电控锁坏或感应器坏
#define ERR_DEVICE_COMMAND_SN		0x41  //错误命令序列
#define ERR_DEVICE_COMMAND_VARIFY	0x42  //命令序列检验错误
#define ERR_DEVICE_COMMAND_LENGTH	0x43  //命令序列长度错误
#define ERR_DEVICE_COMMINICATION	0x44  //通信超时
#define ERR_DEVICE_SENSOR			0x5b  //感应器错误
