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
	LOCKERDLL_API int Reset(int &cabinetCount,int* pCCabinetID);//20���� 32���� 32������������
	LOCKERDLL_API int OpenDoor(int iCabinet,int iCase,int *iStatus);//iCase=0�������� iStatus��64�ֽڣ�iCase!=0��һ���� iStatus��2�ֽ�
	LOCKERDLL_API int Status(int iCabinet,int iCase,int *iStatus);
private:
	void LRC(BYTE lrc[2],BYTE *str,int len);
	void Convert(BYTE b[4],int i[32]);//�͵���
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
#define ERR_ALREADY_SET_TYPE		3	//�ظ������豸����
#define ERR_EXECUTE					4	//ִ��ʧ��
#define ERR_COM						5	//���ڴ�
#define ERR_ABORT					6	//�ж�
#define ERR_COMMUNICATION			7	//ͨѶʧ��
#define ERR_PARAMETER				8	//��������
#define ERR_UNSUPPORT				9	//�����ִ��
#define ERR_CARDTYPE				10	//����Ŀ�
#define ERR_DLL						11	//���ض�̬��ʧ��
#define ERR_DLLDIRECTORY			12	//���ö�̬��Ŀ¼ʧ��
#define ERR_INOPERATE				13	//ִ����һ��������
#define ERR_TIMEOUT					14	//��ʱ
#define ERR_BUFFER_SMALL			15	//������̫С
#define ERR_VERIFY					16	//����У��ʧ��
#define ERR_LRC						17	//У��LRCʧ��
#define ERR_INOPERATION				18	//�豸æ ��һ������
//�豸������
#define ERR_DEVICE_OK				0x30  //�����ɹ�
#define ERR_DEVICE_INVALID_ID		0x33  //ID�Ƿ�
#define ERR_DEVICE_TIME_LIMIT		0x34  //ʱ�����ô����Ƿ�
#define ERR_DEVICE_UNKNOWN			0x36  //�������󣬵���������Ӧ����
#define ERR_DEVICE_COMMAND_SN		0x41  //������������
#define ERR_DEVICE_COMMAND_VARIFY	0x42  //�������м������
#define ERR_DEVICE_COMMAND_LENGTH	0x43  //�������г��ȴ���
#define ERR_DEVICE_COMMINICATION	0x44  //ͨ�ų�ʱ
#define ERR_DEVICE_SENSOR			0x5b  //��Ӧ������
