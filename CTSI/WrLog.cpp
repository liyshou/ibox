// wrLog.cpp: implementation of the CwrLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "WrLog.h"
#include "Global.h"
#include "Pack.h"
#include "Def.h"
#include "DB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern CGlobal gbl;

#define MAX 8192
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWrLog::CWrLog()
{

}

CWrLog::~CWrLog()
{

}
char CWrLog::g_pos_no[8+1] = "";
int  CWrLog::log_sd = 0;

//hex_len��hex�ĳ���
void wrHex(char *name,BYTE *hex,int hex_len)		//bcdת���ַ�����д��log�ļ�
{
//	BYTE *str = new BYTE[8192];
	BYTE str[81920];
	bbzero(str);
	CTools::byte_to_hex(hex, str, hex_len);

	wrLog("%s[%d]��[%s]",name,hex_len*2,str);
//	delete[] str;
}


//дlog
void wrLog(const char *format,...)
{
	va_list ap;
	int n;
	char buff[MAX];
	char strTime[15+1];
	char strDay[8+1];
	char temp[MAX+30];
	
	memset(buff,0,sizeof(buff));
	va_start (ap,format);
	vsprintf(buff,format,ap);
	va_end(ap);
	
	n = strlen(buff);
	memset( strDay, 0, sizeof(strDay) );
	get_date8(strDay);
	memset( strTime, 0, sizeof(strTime) );
	bbzero(temp);
	CTools::getTime(temp, 1);
	memset( strTime, 0, sizeof(strTime) );
	memcpy(strTime, temp, 2);
	memcpy(strTime+2, ":", 1);
	memcpy(strTime+3, temp+2, 2);
	memcpy(strTime+5, ":", 1);
	memcpy(strTime+6, temp+4, 2);
	memcpy(strTime+8, ".", 1);
	memcpy(strTime+9, temp+6, 3);
	
	memset( temp, 0, sizeof(temp) );
	sprintf( temp, "log\\%s",strDay);
	if( CreateDir(temp) )
	{
		FILE *fp;
		strcat(temp, "\\");
		if( strcmp(log.g_pos_no, "") )
			strcat(temp, log.g_pos_no);	//ȡePOS�ն˺���Ϊ�ļ��������������
		else
			strcat(temp, "system");
		strcat(temp, ".log");
		fp = fopen( temp, "a");
		
		if( fp == NULL)
		{
			fp = fopen( temp, "w+");
			if(fp == NULL)
				return;
		}
		memset( temp, 0, sizeof(temp) );
		sprintf(temp, "[%s %s %d] %s\n", strDay, strTime, log.log_sd, buff);
		fputs(temp, fp);
		fclose(fp);
	}
	else
		return;
}

void get_date8 (char *cDate)	//ȡ8λ���ڣ�e.g 20060809
{
	static char buff[8+1];
	struct tm     *tm_now;
	time_t        time_now;
	
	time_now = time (NULL);
	tm_now = localtime (&time_now);
	sprintf (buff, "%04d%02d%02d", tm_now->tm_year + 1900,
		tm_now->tm_mon + 1, tm_now->tm_mday);
	strcpy(cDate,buff);
}

void get_time8(char *cTime)		//ȡ8λʱ�䣬e.g 15:56:24
{
	static char buff[8+1];
	time_t t;
	struct tm tm;
	time(&t);
	memcpy(&tm, localtime(&t), sizeof(tm));
	sprintf(buff, "%.2d:%.2d:%.2d",tm.tm_hour,tm.tm_min,tm.tm_sec);
	strcpy(cTime, buff);
}


//�ж�Ŀ¼�Ƿ����
bool DirExist(const char *pszDirName)
{
	WIN32_FIND_DATA fileinfo;
    char	_szDir[_MAX_PATH];
    strcpy(_szDir, pszDirName);
    int nLen = strlen(_szDir);
    if( (_szDir[nLen-1] == '\\') || (_szDir[nLen-1] == '/') )
    {
    	_szDir[nLen-1] = '\0';
    }
	HANDLE	hFind = ::FindFirstFile(_szDir, &fileinfo);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
    if( fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
    {
    	::FindClose(hFind);
        return true;
    }
    ::FindClose(hFind);
	return false;
}



// ����Ŀ¼��������Ŀ¼�����Դ����༶��Ŀ¼
bool CreateDir(const char *pszDirName)
{
	BOOL bRet = FALSE;
    char  _szDir[_MAX_PATH];
    char  _szTmp[_MAX_DIR];
    int  nLen = 0;
    int  idx ;
	if( (DirExist(pszDirName)) == true )
    	return true;
    strcpy(_szDir, pszDirName);
    nLen = strlen(_szDir);
    if( _szDir[nLen-1] == '\\' || _szDir[nLen-1] == '/' )
    {
    	_szDir[nLen-1] = '\0';
    }
    nLen = strlen(_szDir);
    memset(_szTmp, 0, _MAX_DIR);
    char  _str[2];
    for(idx = 0; idx < nLen; idx++)
    {
    	if(_szDir[idx] != '\\' )
        {
        	_str[0] = _szDir[idx];
            _str[1] = 0;
			strcat(_szTmp, _str);
        }
        else
        {
            bRet = ::CreateDirectory(_szTmp, NULL);
            if( bRet )
            {
            	::SetFileAttributes(_szTmp, FILE_ATTRIBUTE_NORMAL);
            }
        	_str[0] = _szDir[idx];
            _str[1] = 0;
			strcat(_szTmp, _str);
        }
        if( idx == nLen-1 )
        {
            bRet = ::CreateDirectory(_szTmp, NULL);
            if( bRet )
            {
            	::SetFileAttributes(_szTmp, FILE_ATTRIBUTE_NORMAL);
            }
        }
    }
	if( DirExist(_szTmp) )
    	return true;
	return false;
}


//д������ˮ��־
//flag: 0, pos����; 1, pe�·�
//trFlag: 1,��ѯ; 2,����; 3,ǩ��
void wrPosLog(int flag, int trFlag, CGlobal gbl, CPack pack)
{
	char strTime[8+1];
	char strDay[8+1];
	char temp[100];
	char trType[3];
	char correctCode[10];

	if( trFlag == 1 )
		strcpy(trType, "01");
	if( trFlag == 2 )
		strcpy(trType, "02");
	if( trFlag == 3 )
		strcpy(trType, "05");

	memset( strDay, 0, sizeof(strDay) );
	get_date8(strDay);
	memset( strTime, 0, sizeof(strTime) );
	get_time8(strTime);
	
	memset( temp, 0, sizeof(temp) );
	sprintf( temp, "log\\%s",strDay);
	if( CreateDir(temp) )
	{
		FILE *fp;
		strcat(temp, "\\");
		if( !flag )  //�ն�����
			strcat(temp, "pos_up");
		else  //�����·�
			strcat(temp, "pe_down");
		strcat(temp, ".log");

		fp = fopen( temp, "a");
		if( fp == NULL)
		{
			fp = fopen( temp, "w+");
			if(fp == NULL)
				return;
		}
		memset( temp, 0, sizeof(temp) );
		if( !flag )
		{
			sprintf(temp, "%s; %s; %s; %s; %s; %s\n", strDay, strTime, gbl.pos_serial, trType, gbl.pos_no, gbl.fee_type);
		}
		else
		{
			bbzero(correctCode);
			if( atoi((char *)pack.FIELD39) == 80)
			{
				memcpy(correctCode, pack.FIELD62, 9);
			}
			sprintf(temp, "%s; %s; %s; %s; %s; %s; %s; %s\n", strDay, strTime, pack.FIELD37, trType, pack.FIELD39, gbl.pos_no, gbl.fee_type, correctCode);
		}
		fputs(temp, fp);
		fclose(fp);
	}
}


//д����ͳ����־
//tim: ͳ��ʱ��
//num: ��ǰ������
void wrOnlineLog(int num)
{
	char strTime[8+1];
	char strDay[8+1];
	char temp[100];

	memset( strDay, 0, sizeof(strDay) );
	get_date8(strDay);
	memset( strTime, 0, sizeof(strTime) );
	get_time8(strTime);
	
	memset( temp, 0, sizeof(temp) );
	sprintf( temp, "log\\%s",strDay);
	if( CreateDir(temp) )
	{
		strcat(temp, "\\����ͳ��");
		strcat(temp, strDay);
		strcat(temp, ".log");

		FILE *fp;
		fp = fopen( temp, "a");
		if( fp == NULL)
		{
			fp = fopen( temp, "w+");
			if(fp == NULL)
				return;
		}
		memset( temp, 0, sizeof(temp) );
		sprintf(temp, "[%s %s]��ǰ������: %d\n", strDay, strTime, num);
		fputs(temp, fp);
		fclose(fp);
	}
}



//д�����־
//type: ��������
//stat: ����״̬
//rspCode: ������
void wrMonitorLog(int type, int stat, int rspCode, CGlobal gbl, CPack pack)
{
	char strTime[8+1];
	char strDay[8+1];
	char temp[300], path[100];
	MonitorDetail detail;

	wrLog("��wrMonitorLog��type[%d] stat[%d] rspCode[%d]", type, stat, rspCode);

	/*----------------��д��־����------------------*/
	memset(&detail, 0, sizeof(detail));
	strncpy(detail.posNum, gbl.pos_no, sizeof(detail.posNum)-1);		//�ն˺�
	strncpy(detail.callTel, gbl.call_tel, sizeof(detail.callTel)-1);	//�������
	sprintf(detail.traType, "%02d", (unsigned int)type);	//��������
	sprintf(detail.stat, "%02d", (unsigned int)stat);		//״̬��
	sprintf(detail.rspCode, "%02d", (unsigned int)rspCode);//������
	if(atoi(detail.rspCode) == 80)			//bit62������, 20080127
		memcpy(detail.corrCode, pack.FIELD62, 9);
	strncpy(detail.feeType, gbl.fee_type, sizeof(detail.feeType)-1);	//ҵ�����
	strncpy(detail.serial, gbl.pos_serial, sizeof(detail.serial)-1);	//�ն���ˮ
	strncpy(detail.traNum, gbl.phone_no, sizeof(detail.traNum)-1);	//���׺���
	if(type == 2 || type == 13 || type==15)	//���ѻ����ͨ����ʱ
	{
		strncpy(detail.amount, gbl.amount, sizeof(detail.amount)-1);	//���
		strncpy(detail.card, gbl.card_no, sizeof(detail.card)-1);		//����
		strncpy(detail.bank, gbl.bank, sizeof(detail.bank)-1);			//���д���
	}
	if(stat == 0 || stat == 3)	//pe���سɹ���ʧ��ʱ
		if(type == 1 || type == 2 || type == 3 || type == 13 || type==15) 
			strncpy(detail.stan, (char *)pack.FIELD37, sizeof(detail.stan)-1);	//ϵͳ�ο���

	/*-----------------д��־-----------------*/
	bbzero(strDay);
	get_date8(strDay);
	bbzero(strTime);
	get_time8(strTime);
	
	bbzero(path);
	strcpy(path, "MonitorLog");

	if( CreateDir(path) )
	{
		strcat(path, "\\monitor");
		strcat(path, strDay);
		strcat(path, ".log");

		bbzero(temp);
		sprintf(temp, "%s %s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;\n", 
		strDay, strTime, detail.posNum, detail.serial, detail.callTel, detail.traType, detail.stat, detail.rspCode, detail.feeType, detail.amount, detail.bank, detail.card, detail.traNum, detail.stan, detail.corrCode);

		cs_FILE.Lock();	//ͬ������
		FILE *fp;
		fp = fopen( path, "a");
		if( fp == NULL)
		{
			fp = fopen( path, "w+");
			if(fp == NULL)
			{
				wrLog("��wrMonitorLog��end: file open err");
				return;
			}
		}
		fputs(temp, fp);
		fclose(fp);
		cs_FILE.Unlock();
	}
	wrLog("��wrMonitorLog��end");
}

//дlog
void wrDGBLog(const char *format,...)
{
	va_list ap;
	int n;
	char buff[MAX];
	char strTime[12+1];
	char strDay[8+1];
	char temp[64];
	CTools tool;
	memset(buff,0,sizeof(buff));
	va_start (ap,format);
	vsprintf(buff,format,ap);
	va_end(ap);
	
	n = strlen(buff);
	memset( strDay, 0, sizeof(strDay) );
	tool.get_date8(strDay);
	memset(temp, 0, sizeof(temp));
	tool.get_time(temp, 1);
	memset( strTime, 0, sizeof(strTime) );
	memcpy(strTime, temp, 2);
	memcpy(strTime+2, ":", 1);
	memcpy(strTime+3, temp+2, 2);
	memcpy(strTime+5, ":", 1);
	memcpy(strTime+6, temp+4, 2);
	memcpy(strTime+8, ".", 1);
	memcpy(strTime+9, temp+6, 3);
	
	memset( temp, 0, sizeof(temp) );
	sprintf( temp, "log\\%s",strDay);
	if( CreateDir(temp) )
	{
		FILE *fp;
		strcat(temp, "\\");
		strcat(temp, "DGBusTrade");
		strcat(temp, ".log");
		fp = fopen( temp, "a");
		if( fp == NULL)
		{
			fp = fopen( temp, "w+");
			if(fp == NULL)
				return;
		}
		
		memset( temp, 0, sizeof(temp) );
		sprintf(temp, "[%s %s] ", strDay, strTime);
		if(temp == NULL)
		{
			fclose(fp);
			return;
		}
		fputs(temp, fp);
		
		if(buff == NULL)
		{
			fclose(fp);
			return;
		}
		fputs(buff, fp);
		fputs("\n", fp);
		fclose(fp);
	}
}

void wrDGBusHex(char *name,BYTE *hex,int len)		//bcdת���ַ�����д��log�ļ�
{
	BYTE str[4096];
	memset(str,0,sizeof(str));
	bcd_to_asc(str, hex, len*2, 1);
	
	wrDGBLog("%s[%d]��[%s]",name,len*2,str);
}

void wrYct3MonitorLog(char *pos_no,const char *format,...) //д���ͨ��ֵ�������ն˼����־
{
	va_list ap;
	int n;
	char buff[MAX];
	char strTime[12+1];
	char strDay[8+1];
	char temp[64];
	CTools tool;
	memset(buff,0,sizeof(buff));
	va_start (ap,format);
	vsprintf(buff,format,ap);
	va_end(ap);
	
	n = strlen(buff);
	memset( strDay, 0, sizeof(strDay) );
	tool.get_date8(strDay);
	memset(temp, 0, sizeof(temp));
	tool.get_time(temp, 1);
	memset( strTime, 0, sizeof(strTime) );
	memcpy(strTime, temp, 2);
	memcpy(strTime+2, ":", 1);
	memcpy(strTime+3, temp+2, 2);
	memcpy(strTime+5, ":", 1);
	memcpy(strTime+6, temp+4, 2);
	memcpy(strTime+8, ".", 1);
	memcpy(strTime+9, temp+6, 3);
	
	memset( temp, 0, sizeof(temp) );
	sprintf( temp, "YCT3MonitorLog\\%s",strDay);
	if( CreateDir(temp) )
	{
		FILE *fp;
		strcat(temp, "\\TermMonitor_");
		strcat(temp, pos_no);
		strcat(temp, ".log");
		fp = fopen( temp, "a");
		if( fp == NULL)
		{
			fp = fopen( temp, "w+");
			if(fp == NULL)
				return;
		}
		
		memset( temp, 0, sizeof(temp) );
		sprintf(temp, "[%s %s] ", strDay, strTime);
		if(temp == NULL)
		{
			fclose(fp);
			return;
		}
		fputs(temp, fp);
		
		if(buff == NULL)
		{
			fclose(fp);
			return;
		}
		fputs(buff, fp);
		fputs("\n", fp);
		fclose(fp);
	}
}