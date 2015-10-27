// WrLog.h: interface for the CWrLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRLOG_H__EF6864A0_8E88_4116_A187_B59072B3390D__INCLUDED_)
#define AFX_WRLOG_H__EF6864A0_8E88_4116_A187_B59072B3390D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlobal;
class CPack;

void wrLog(const char *format,...);
void wrHex(char *name,BYTE *hex,int hex_len);

void wrPosLog(int flag, int trFlag, CGlobal gbl, CPack pack);
void wrOnlineLog(int num);


void get_date8 (char *cDate);	//ȡ8λ���ڣ�e.g 20060809
void get_time8(char *cTime)	;	//ȡ8λʱ�䣬e.g 15:56:24

bool DirExist(const char *pszDirName);	//�ж�Ŀ¼�Ƿ����
bool CreateDir(const char *pszDirName);	// ����Ŀ¼��������Ŀ¼�����Դ����༶��Ŀ¼

void wrDGBLog(const char *format,...);  
void wrDGBusHex(char *name, unsigned char *hex,int lenstr);

void wrYct3MonitorLog(char *pos_no,const char *format,...); //д���ͨ��ֵ�������ն˼����־

class CWrLog  
{
public:
	CWrLog();
	virtual ~CWrLog();
	__declspec(thread) static char g_pos_no[8+1];
	__declspec(thread) static int  log_sd;

};

#endif // !defined(AFX_WRLOG_H__EF6864A0_8E88_4116_A187_B59072B3390D__INCLUDED_)
