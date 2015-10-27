// Tools.cpp: implementation of the CTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <STDIO.H>
#include "TissonCTSI.h"
#include "Tools.h"
#include "Def.h"
#include <winsock2.h>
#include <IOSTREAM> //add by liuxiao 
#include <STRING> //add by liuxiao
#include <IOSTREAM>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define  CRC_PRESET             0x0000
#define  CRC_POLYNOM            0x1021
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTools::CTools()
{

}

CTools::~CTools()
{

}

//a=0 encrypt,�ƶ�����
//a=1 decrypt,�ۿ�����
void CTools::Encrypt(char *inPwd,char *outPwd, int a)
{
	int i, len;
	char temp_buff[20+1],temp;
	
	len = strlen(inPwd);
	memset(temp_buff,0,sizeof(temp_buff));
	memcpy(temp_buff,inPwd,len);
	
	for (i=0; i<len; i++)
	{
		temp= temp_buff[i];
		if (i%2)
			temp = temp ^ 0x7;
		else
			temp = temp ^ 0x5;
		if( a == 0)
			outPwd[ len-i-1 ] = temp;
		else
			outPwd[i] = temp;
	}
	outPwd[len] = 0;
}

//�䳤�ͽ���Ϊ��λ��Ϊxx.xx��ʽ
//e.g. 5000 -> "50.00"
void CTools::longToMny(long longMny, char *mny)
{
	sprintf(mny, "%.2lf", ((double)longMny)/100);
}


//�Ѽ۸��ַ�����������С���㣩��Ϊ12λ��ʽ��ȥС���㣬ǰ��0��
//ע��С�����Ϊ2λ
void CTools::chg12asc(const char *inAmo,char *outAmo)
{
	int amoLen ,len, i;
	char temp[13];
	amoLen = strlen(inAmo);
	for(i=0;i<amoLen;i++)	//�ж�inAmo�Ǹ�������������
		if(inAmo[i]=='.')
			break;
	memset(temp,0,sizeof(temp));
	if(i==amoLen)	//��������
	{		
		strcpy(temp,inAmo);
		strncat(temp,"00",2);
		len = strlen(temp);
	}
	else	//������
	{
		for(i=0;i<amoLen;i++)
			if(inAmo[i]!='.')
				strncat(temp,inAmo+i,1);
		len = strlen(temp);
	}
	for(i = 0;i<12-len;i++)		//ǰ��0������12λ
		outAmo[i] = '0';
	strcat(outAmo,temp);
	outAmo[13] = '\0';
}

//ת��12λ�ַ���Ϊ��xx.xx���Ľ���ʽ,flag=1������С�Ԫ��(Ĭ��)��flag=0,û�С�Ԫ����Ĭ��flagΪ1
void CTools::chgToMny(const char *str,char *mny,int flag)	
{
	int i = 0 ;
	double m = 0;
	double n=0.0;

	for(i=0;i<12;i++){	//�ж��Ƿ��Ǹ���
		if(str[i]=='-')
			break;
	}

	if(i==12)	//����
	{
		m= atof(str);
		n = 100;
		sprintf(mny , "%.2f" ,(double)m/n);
	}

	else	//����
	{
		m = (long)atof(str+i+1);
		n = 100;
		sprintf(mny , "-%.2lf" ,(double)m/n);
	}
	if(flag==1)
		strcat(mny,"Ԫ");
}

void CTools::chgToPreMny(const char *str,char *mny)	//��Ԥ����ת��������
{
	int i;
	long m;
	double n;
	for(i=0;i<12;i++){	//�ж��Ƿ��Ǹ���
		if(str[i]=='-')
			break;
	}
	
	if(i==12)	//����
	{
		m= atol(str);
		n = 100;
		sprintf(mny , "%.2lf" ,(double)m/n);
	}
	
	else	//����
	{
		m = (long)atof(str+i+1);
		n = 100;
		sprintf(mny , "%.2lf" ,(double)m/n);
	}
	
}



//����������    buff="\x00\x00\x01\x12",if length=4 then return is 4
//���룺	buff,length
//�����	num 
//bcd���Ϊlength���ȵ�����
int  CTools::BCD_To_INT_10(BYTE *buff,BYTE length)
{
	int num;
	BYTE high,low;
	BYTE buf[20];
	int len;

	memcpy(buf,buff,length);
	buf[length] = 0;

	num = 0;
	len = length;
	while(len)
		{
		num *= 100;
		low=buf[0]&0x0F;
		high=((buf[0]>>4)&0x0F)*10;
		num=num+high+low;
		memcpy(buf,&buf[1],len+1);
		len--;
		}
	return num;
}

//������:       LongToBcd
//����������    ����ֵת����BCD���ʽ(number = 123,  length=6, then buff="\x00\x01\x23")
//���룺	number -��ֵ length:Ҫת���ĳ���
//�����	buff -BCD��
//���أ�	��
//����ֵ���Ȳ�����ǰ��0��ʹ����Ϊlength��Ȼ���ٱ�Ϊbcd��
//ע�⣺Ӧ��֤length��С��number�ĳ���
void CTools::LongToBcd(WORD number,BYTE *buff,BYTE length)
{
	char tmpStr[100],numStr[100],str[100];
	int i,j;

//	IntToAsc(number, numStr);
	memset(numStr,0,sizeof(numStr));	//������ת��Ϊ�ַ���
	if(number == 0)
		strcpy(numStr,"0");
	else
	{
		memset(str,0,sizeof(str));
		for(i=0; number>0; i++)
		{
			str[i] = number%10+0x30;
			number/=10;
		}
		str[i]=0;
		for(j=strlen(str),i=0;j>0;j--,i++)
			numStr[i] = str[j-1];
	}

	i=strlen(numStr);	
	if(i>length)	//��lengthС����ֵ���ȣ���buffΪ�գ�����
	{
		strcpy((char *)buff,"");
		return;
	}

	memset( tmpStr, '0', length);
	for( i=strlen(numStr)-1,j=length-1; i>=0; i--,j--)
		tmpStr[j] = numStr[i];
	tmpStr[length] = 0;
//	hex_to_byte(buff, (BYTE *)tmpStr, length ,0);	//ǰ��0
	hex_to_byte((BYTE *)tmpStr, buff, length/2 ,0);	//ǰ��0
}



//��12.34���͵��ַ�����Ϊ��д��ʽ	
//����capMny�ĳ���
int CTools::chgToCap(const char *mny,char *capMny)	
{
	char m_mny[30], *p=NULL, *q=NULL;
	strcpy(m_mny, mny);
	for(p=m_mny; *p!=0; p++)
	{
		if(*p=='.')
		{
			q=p;			
			break;
		}
	}
	for(p++; q!=NULL && *p!=0; p++,q++)
	{
		*q=*p;
	}
	if(q!=NULL)
	{
		*q=0;
	}
	else
	{
		strcat(m_mny, "00");
	}
	if(atof(m_mny)>=0){
		ConvertMoneyUpper((DWORD)atol(m_mny),capMny);
	}
	else{
		strcpy(capMny,"��");
		ConvertMoneyUpper((DWORD)atol(m_mny),capMny);
	}
	return strlen(capMny);
}

void CTools::UpperMoney(char c, char *out)
{
	switch( c )
	{
	case 1:
		strcpy( out, "Ҽ");
		break;
	case 2:
		strcpy( out, "��");
		break;
	case 3:
		strcpy( out, "��");
		break;
	case 4:
		strcpy( out, "��");
		break;
	case 5:
		strcpy( out, "��");
		break;
	case 6:
		strcpy( out, "½");
		break;
	case 7:
		strcpy( out, "��");
		break;
	case 8:
		strcpy( out, "��");
		break;
	case 9:
		strcpy( out, "��");
		break;
	case 0:
		strcpy( out, "��");
		break;
	}
}

void CTools::ConvertMoneyUpper(DWORD amount, char *out)
{
	char dbuffer1[100];
	DWORD yuan,cent;
	BYTE status=0;

	yuan = amount / 100;
	cent = amount % 100;
	strcpy( out, ""); /*set upper monry buffer empty*/
	if( yuan / 100000 != 0 )
	{
		UpperMoney( (char)(yuan/100000), dbuffer1);
		strcat( out, dbuffer1);
		strcat( out, "ʰ");
		yuan = yuan % 100000;
		status = 1;
	}
	if( yuan / 10000 != 0)
	{
		UpperMoney( (char)(yuan/10000), dbuffer1 );
		strcat( out, dbuffer1 );
		strcat( out, "��");
		yuan = yuan % 10000;
/*add by Jacky 990717*/
		status = 2;
/*end of add*/
	}
	else
	{
		if( status == 1 )
			strcat( out, "��");
	}
	if( yuan / 1000 != 0 )
	{
		UpperMoney( (char)(yuan/1000), dbuffer1 );
		strcat( out, dbuffer1 );
		strcat( out, "Ǫ");
		yuan = yuan % 1000;
/*change by Jacky 990717*/
/*		status = 1;*/
		status = 3;
/*end of change*/
	}
	if( yuan / 100 != 0 )
	{
/*add by Jacky 990717*/
		if(status != 0 && status != 3)
		{
			UpperMoney( (char)0, dbuffer1 );
			strcat( out, dbuffer1 );
		}
/*end of add*/
		UpperMoney( (char)(yuan/100), dbuffer1 );
		strcat( out, dbuffer1 );
		strcat( out, "��");
		yuan = yuan % 100;
/*change by Jacky 990717*/
/*		status = 1;*/
		status = 4;
/*end of change*/
	}
	if( yuan / 10 != 0 )
	{
/*add by Jacky 990717*/
		if ( status != 0 && status != 4 )
		{
			UpperMoney( (char)0, dbuffer1 );
			strcat( out, dbuffer1 );
		}
/*end of add*/
		UpperMoney( (char)(yuan/10), dbuffer1 );
		strcat( out, dbuffer1 );
		strcat( out, "ʰ");
		yuan = yuan % 10;
/*change by Jacky 990717*/
/*		status = 1;*/
		status = 5;
/*end of change*/
	}
	if( yuan != 0 )
	{
/*add by Jacky 990717*/
		if ( status != 0 && status != 5 )
		{
			UpperMoney( (char)0, dbuffer1 );
			strcat( out, dbuffer1 );
		}
/*end of add*/
		UpperMoney( (char)yuan, dbuffer1 );
		strcat( out, dbuffer1 );
	}
	else
	{
		if( status == 0 )
		{
			UpperMoney( (char)yuan, dbuffer1 );
			strcpy( out, dbuffer1 );
		}
	}
	strcat( out, "Բ");

	UpperMoney( (char)(cent/10), dbuffer1 );
	strcat( out, dbuffer1 );
	strcat( out, "��");
	cent = cent % 10;
	UpperMoney( (char)cent, dbuffer1 );
	strcat( out, dbuffer1 );
	strcat( out, "��");
}


//����Ҫ��ӡ��Ŀ��ϸ�ĸ���ֵ���
//mny��2ά���飬��Ž��
//nCnt:mny����Ŀ
//total����ͺ��ֵ
int CTools::getTalAmo(const char mny[][MAXAMOLEN],char *total,int nCnt)		
{
	int i,j;
	float temp,tal = 0;
	for(i=0;i<nCnt;i++)
	{
		if(!strncmp(mny[i],"-",1))	//��Ϊ����
		{
			temp = (float)atof(mny[i]+1);
			temp = 0 - temp;
		}
		else
			temp = (float)atof(mny[i]);
		tal +=temp;
	}
	sprintf(total,"%.2f",tal);
	j = strlen(total);
	return j;
}

//��ȡ���������
void CTools::getYesterday(char *date)
{
	CTime   cYesterday = CTime::GetCurrentTime() - CTimeSpan(1,0,0,0);   
	CString   sYesterday = cYesterday.Format("%Y%m%d"); 
	if( date != NULL)
		strcpy(date, (LPCTSTR)sYesterday);
}

//��ȡ���µ��·�
void CTools::getLastMonth(char *date)
{
	CTime   cYesterday = CTime::GetCurrentTime();
	if( date != NULL)
	{
		if(cYesterday.GetMonth()!=1)
		{
			sprintf(date,"%04d%02d",cYesterday.GetYear(),cYesterday.GetMonth()-1);
		}else
		{
			sprintf(date,"%04d12",cYesterday.GetYear()-1);
		}
	}
}
//��ȡ���µ��·�
void CTools::getTheMonth(char *date)
{
	CTime   cYesterday = CTime::GetCurrentTime();
	if( date != NULL)
	{
		sprintf(date,"%04d%02d",cYesterday.GetYear(),cYesterday.GetMonth());
	}
}
//��������������������Ϊbcd��ʽ
//���룺	instr-�ַ���, inLen-instr����, type-��inLenΪ����ʱ��0��ʽ,0Ϊǰ��0(Ĭ��),����Ϊ��0
//�����	outstr-BCD�ַ���
//���أ�	����ַ�����
int CTools::hex_to_byte (const BYTE *instr, BYTE *outstr, int inLen, int type)
{
  register int           i, j;
  register char          ch;
  BYTE tmp[4096];

  memset(tmp, 0, sizeof(tmp));
  if(inLen%2)	//����
  {
	  if(type)	//��0
	  {
		  memcpy(tmp, instr, inLen);
		  tmp[inLen] = '0';
	  }
	  else		//ǰ��0
	  {
		  tmp[0] = '0';
		  memcpy(tmp+1, instr, inLen);
	  }
	  inLen++;
  }
  else
	  memcpy(tmp, instr, inLen);

  for (i = j = 0; i<inLen; i++)  
  {
    ch = tmp[i];
    if (ch >= '0' && ch <= '9')
      ch -= '0';
    else
	{
      if (ch >= 'a' && ch <= 'f')
        ch = ch - 'a' + 10;
      else
	  {
        if (ch >= 'A' && ch <= 'F')
          ch = ch - 'A' + 10;
        else
          ch = 0;
	  }
	}
    if (i & 1)
      outstr[j++] += ch;
    else
      outstr[j] = ch << 4;
  }
  outstr[j] = 0;

  return (j);
}

// convert binary value in input buffer to hex string
// (e.g 0x137A8C (3 bytes) to "137A8C" (6 bytes),
// i.e. the reverse function of "hex_to_byte"
void CTools::byte_to_hex (const BYTE *in_buff, BYTE *out_buff, int in_len)
{
  int   i, j;
  BYTE  ch;

  for (i = j = 0; i < in_len; i++)  
  {
	  ch = (in_buff[i] >> 4) & 0x0f;
	  out_buff[j++] = ch > 9 ? ch + 'A' - 10 : ch + '0';
	  ch = in_buff[i] & 0x0f;
	  out_buff[j++] = ch > 9 ? ch + 'A' - 10 : ch + '0';
  }
  out_buff[j] = 0;
}


//type: 0-��8λ����Ϊ2007/11/12��ʽ��Ĭ�ϣ�
//		1-��6λ����Ϊ07/11/12��ʽ
//		2-��4λ����Ϊ11/12��ʽ
//      3-��8λ����Ϊ2007-11-12��ʽ
void CTools::formatDate(char *yymmdd, char *cDate, int type)
{
	if(yymmdd == NULL || cDate == NULL)
		return;
	if(type == 0)
	{
		memcpy(cDate,yymmdd,4);
		strcat(cDate,"/");
		memcpy(cDate+5,yymmdd+4,2);
		strcat(cDate,"/");
		memcpy(cDate+8,yymmdd+6,2);
		
	}
	else if(type == 1)
	{
		memcpy(cDate, yymmdd, 2);
		strcat(cDate, "/");
		memcpy(cDate+3, yymmdd+2, 2);
		strcat(cDate,"/");
		memcpy(cDate+6, yymmdd+4, 2);
	}
	else if(type == 2)
	{
		memcpy(cDate, yymmdd, 2);
		strcat(cDate,"/");
		memcpy(cDate+3, yymmdd+2, 2);
	}
	if(type == 3)
	{
		memcpy(cDate,yymmdd,4);
		strcat(cDate,"-");
		memcpy(cDate+5,yymmdd+4,2);
		strcat(cDate,"-");
		memcpy(cDate+8,yymmdd+6,2);
		
	}
}

//��6λʱ��Ϊ10:25:35��ʽ
void CTools::formatTime(char *hhmmss, char *cTime)
{
	if(hhmmss == NULL || cTime == NULL)
		return;
	memcpy(cTime,hhmmss,2);
	strcat(cTime,":");
	memcpy(cTime+3,hhmmss+2,2);
	strcat(cTime,":");
	memcpy(cTime+6,hhmmss+4,2);
}

//ȡʱ�䣬e.g 155624123
//flag: Ĭ��Ϊ0����ʾȡ6λʱ��(��ȷ����)����0ʱ��ȡ9λʱ��(��ȷ������)
void CTools::getTime(char *cTime, int flag)		
{
	char buff[10];
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);  

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "%.2d%.2d%.2d%.3d",systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	if(flag)
		strcpy(cTime, buff);
	else
		memcpy(cTime, buff, 6);
}

//�������ַ����е�Сд��ĸװ��Ϊ��д��ĸ
void CTools::lowerToUpper(char *ch){
	unsigned int i=0;
	for(i=0;i<strlen(ch);i++)
	{
		if(ch[i]>='a'&&ch[i]<='z')
		{
			ch[i]=ch[i]-32;
		} 
		else ch[i]=ch[i];
    }
}


//ȡʱ�䣬e.g 155624123
//flag: Ĭ��Ϊ0����ʾȡ6λʱ��(��ȷ����)����0ʱ��ȡ9λʱ��(��ȷ������)
void CTools::get_time(char *cTime, int flag)		
{
	char buff[10];
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);  
	
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "%.2d%.2d%.2d%.3d",systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	if(flag)
		strcpy(cTime, buff);
	else
		memcpy(cTime, buff, 6);
}

//ȡʱ����Ϊ091026113445��Ϊϵͳ�ο���

void CTools::getYctStan(char *yct_stan) //��ȡ���ͨϵͳ�ο���
{	
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);
	sprintf(yct_stan,"%02d%02d%02d%02d%02d%02d",systime.wYear%100,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
	wrLog("yct_stan:%s",yct_stan);
	
}


//ȡʱ��113445��Ϊ�ն���ˮ��
void CTools::getYct_posSerial(char * term_serial)//��ȡ���ͨ�ն���ˮ��
{
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);
	sprintf(term_serial,"%02d%02d%02d",systime.wHour,systime.wMinute,systime.wSecond);
	
}

//ȡʱ�� , e.g 12:03
void CTools::getCurrentTime(char *cTime){
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf(cTime,"%02d:%02d",sysTime.wHour,sysTime.wMinute);//��ȡ��ǰʱ��
	
}

//ȡ���ڣ�e.g 20081101
void CTools::getDate(char *cTime)		
{
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);
	sprintf(cTime, "%04d%02d%02d",systime.wYear, systime.wMonth, systime.wDay);
}

//��ȡ�ն�ʱ��
void CTools::get_termTime (char *cTime)	//ȡ���ڣ�e.g 20060809153025
{
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);
	sprintf(cTime, "%04d%02d%02d%02d%02d%02d",systime.wYear, systime.wMonth, systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);	

}

//�������ַ����е�Сд��ĸװ��Ϊ��д��ĸ
void CTools::itemAppend(const char *instr, int len, char* outstr){
	int reallen=strlen(instr);
	char lenbuf[20+1];
	bbzero(lenbuf);	
	sprintf(lenbuf, "%020d", reallen);
	strcpy(outstr,lenbuf+20-len);
	strcat(outstr,instr);
}

//��IB����
void CTools::decIBcont(int count, char* src, char result[][2][50]){
	char lenbuf[4+1];
	char *idx=src;
	int len=0;
	bbzero(lenbuf);
	for(int i=0; i<count; i++){
		memcpy(result[i][0], idx, 4);
		idx+=4;
		memcpy(lenbuf, idx, 4);
		len = atoi(lenbuf);
		idx += 4;
		memcpy(result[i][1], idx, 4);
		idx += len;
	}
}

long CTools::getFileSize(char *filename)
{
	long siz = 0;
	FILE *fp = fopen(filename, "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		siz = ftell(fp);
		fclose(fp);
	}
	return siz;
}

//��ݸ��������CRC16
void CTools::crc_cu_16( unsigned char *SerBuffer, unsigned int length)   
{
	
	unsigned int i,j;
	unsigned int Crc;
	Crc = CRC_PRESET;
	
	for (i = 0; i < length; i++)
	{
		
		Crc = Crc ^((unsigned int)SerBuffer[i] << 8);
		
		for (j = 0; j < 8; j++)
		{
			if (Crc & 0x8000)
			{
				Crc = (Crc << 1) ^ CRC_POLYNOM;
			}
			else
			{
				Crc = (Crc << 1);
			}
		}
	}
	SerBuffer[length]     = (unsigned char)(Crc >> 8);
	SerBuffer[length + 1] = (unsigned char) Crc;
	SerBuffer[length + 2] =0;
}


//��ݸ��������ΪHEX 4byte ��λ���ң���λ���� ��ȷ����
void CTools::bus_money_change(char *input,char*output)
{
	char temp[4+1];
	memset(temp,0,sizeof(temp));
	hex_to_byte((const BYTE *)input,(BYTE *)temp,8);
	for (int i=0;i<4;i++)
	{
		output[i] = temp[4-i-1];
	}
}


void CTools::sixten_toten(char*input,char* output)
{	
	int i;int j;
	int len,a[8+1],sum=0;
	len=strlen(input);
	for(i=0;i<len;i++)
	{
		switch(input[i])
		{
		case '0':a[i]=0;break;
		case '1':a[i]=1;break;
		case '2':a[i]=2;break;
		case '3':a[i]=3;break;
		case '4':a[i]=4;break;
		case '5':a[i]=5;break;
		case '6':a[i]=6;break;
		case '7':a[i]=7;break;
		case '8':a[i]=8;break;
		case '9':a[i]=9;break;
		case 'A':a[i]=10;break;
		case 'B':a[i]=11;break;
		case 'C':a[i]=12;break;
		case 'D':a[i]=13;break;
		case 'E':a[i]=14;break;
		case 'F':a[i]=15;break;
		}
		
	}
	for(i=0;i<len;i++)
	{
		for(j=i;j<len-1;j++)
			a[i]*=16;
		sum+=a[i];
	}
	sprintf(output,"%d",sum);
}

void CTools::get_date8 (char *cDate)	//ȡ8λ���ڣ�e.g 20060809
{
	char buff[8+1];
	struct tm     *tm_now;
	time_t        time_now;
	
	time_now = time (NULL);
	tm_now = localtime (&time_now);
	memset(buff, 0, sizeof(buff));
	sprintf (buff, "%04d%02d%02d", tm_now->tm_year + 1900,
		tm_now->tm_mon + 1, tm_now->tm_mday);
	strcpy(cDate,buff);
}

int CTools::IsDigit(char*cData) 	//�ж��ַ��Ƿ�Ϊ����
{
	unsigned int i,j,flag=0;
	char num[1+1];
	for (j=0;j<strlen(cData);j++)
	{
		flag = 0;
		for (i=0;i<10;i++)
		{
			bbzero(num);
			sprintf(num,"%d",i);
			if (memcmp(cData+j,num,1)==0)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
			return -1;
		}
	}
	return 0;
}

int CTools::chkDate(int nYear, int nMonth, int nDay)
{
	if(nDay >= 1 && nDay <= 31)
	{
		NULL;
	}
	else
	{
		wrLog("chkDate nDay[%d]", nDay);
		return 1;
	}
	if(nMonth >= 1 && nMonth <= 12)
	{
		NULL;
	}
	else
	{
		wrLog("chkDate nMonth[%d]", nMonth);
		return 2;
	}
	if(nYear >= 1900)
	{
		NULL;
	}
	else
	{
		wrLog("chkDate nYear[%d]", nYear);
		return 3;
	}
	return 0;
}

void CTools::trim(char * str, int type)
{
	int start,end,i;  
    if (str)  
    {  
		start = 0;
		end = strlen(str) - 1;
		if(type == 0 || type == 1)
		{
			for(; isspace(str[start]); start++);  
        }
		if(type == 0 || type == 2)
		{
			for(; isspace(str[end]); end--);  
        }

		for(i=start; i<=end; i++)  
		{
            str[i-start]=str[i]; 
		}

        str[end-start+1]='\0';  
    }  
}

int CTools::findChar(char* src, char ch, int start)
{
	int i, len, at = -1;
	
	len = strlen(src);
	for(i = start; i < len; i++)
	{
		if(src[i] == ch)
		{
			at = i;
			break;
		}
	}
	
	return at;
}

//�ж��Ƿ�ȫ��������
int CTools::isAllNum(char *cData) //�ж�ȫ��������
{
	int flag=0;
	int len=0;
	len=strlen(cData);
	
	for(int i=0;i<len;i++)
	{
		
		if((cData[i]<'0'|| cData[i]>'9'))
		{
			flag=1;
			break;
		}
	
	}
	
	return flag;
	
}

//�ж��Ƿ�ȫ�������ֻ�ȫ������ĸ������ĸ�����ֵ����
//���أ�0 - �ǣ� 1 - �ַ����������ֺ���ĸ������ַ�
int CTools::isAllNumLetter(char *cData) 
{
	int flag=0;
	int len=0;
	len=strlen(cData);
	
	for(int i=0;i<len;i++)
	{
		
		if((cData[i] >= '0' && cData[i] <= '9') || (cData[i] >= 'a' && cData[i] <= 'z') || (cData[i] >= 'A' && cData[i] <= 'Z'))
		{
			continue;
		}
		else
		{
			flag=1;
			break;
		}
		
	}
	
	return flag;
}

//����ָ�����ȵ������
//input:  len - ������ĳ���
//output: str - ��������ַ����ͣ�
//eg: len = 8 , then str = "12345678"
void CTools::generate_random_num(char *str,int len)
{
	int random_num;
	char tmp_num[2];

    memset(str,0,sizeof(str));
	memset(tmp_num,0,sizeof(tmp_num));

	srand( (unsigned)time( NULL ) );

	for(int i=0;i<len;i++)
	{ 
		random_num = rand() % 10+ 1;
		sprintf(tmp_num,"%d",random_num);
		if (i ==0 )
		{
			strncpy(str,tmp_num,1);
		}
		else
		{
			strncat(str,tmp_num,1);
		}
	}
}



//
//����ָ�����ȵ������
//input:  len - ������ĳ���
//output: str - ��������ַ����ͣ�
//eg: len = 8 , then str = "12345678" �޸ġ�
void CTools::generate_random_num(int random_value,char *str,int len) //changed by liuxiao
{
	int random_num;
	char tmp_num[2];
	
    memset(str,0,sizeof(str));
	memset(tmp_num,0,sizeof(tmp_num));
	
	//srand( (unsigned)time( NULL ) ); //changed by liuxiao

	srand( (unsigned)(time( NULL ) +random_value) );

	for(int i=0;i<len;i++)
	{ 
		random_num = rand() % 10+ 1;
		sprintf(tmp_num,"%d",random_num);
		if (i ==0 )
		{
			strncpy(str,tmp_num,1);
		}
		else
		{
			strncat(str,tmp_num,1);
		}
	}
}



// ��12λ��Ϊ4λhex��ʽ
// ��Σ�amo:12λ�Ľ��
// ���Σ�hexAmo:4λhex��ʽ�Ľ��
// ����ֵ����
void CTools::getHexAmo(const char *amo, BYTE *hexAmo)
{
	unsigned long lamo = atoi(amo);
	hexAmo[0] = (BYTE)(lamo / 0x1000000);
	hexAmo[1] = (BYTE)(lamo % 0x1000000 /0x10000);
	hexAmo[2] = (BYTE)(lamo % 0x10000 /0x100);
	hexAmo[3] = (BYTE)(lamo % 0x100);
}


// numλhex��ʽ��Ϊ��12λ���
// ��Σ�hexAmo:4λhex��ʽ�Ľ�� num:λ��
// ���Σ�amo:12λ�Ľ��
// ����ֵ����
void CTools::getAsiAmo(const BYTE *hexAmo, int num, char *amo)
{
	unsigned long lamo = 0; 
	for(int i=0; i<num; i++)
	{
		lamo = lamo * 256;
		lamo = lamo + hexAmo[i];
	}
	sprintf(amo, "%012ld", lamo);
}

int CTools::tobyte (char *instr, char *outstr)
{
	register int           i, j;
	register char          ch;
	
	for (i = j = 0; instr[i]; i++)  {
		ch = instr[i];
		if (ch >= '0' && ch <= '9')
			ch -= '0';
		else
			if (ch >= 'a' && ch <= 'f')
				ch = ch - 'a' + 10;
			else
				if (ch >= 'A' && ch <= 'F')
					ch = ch - 'A' + 10;
				else
					ch = 0;
				
				if (i & 1)
					outstr[j++] += ch;
				else
					outstr[j] = ch << 4;
	}
	outstr[j] = 0;
	
	return (j);
}

//8λʮ��������תΪNλʮ��������ǰ�油�㣬Ĭ��תΪ12λʮ������
//instr: ʮ�������� 
//outstr: ʮ������  e.g. : instr = "00004E20" then outstr = "000000020000"
int CTools::HexToDec(const char * instr, char *outstr,int n)    
{ 
	int   tmp = 0;
	long  result=0; 
	int   basic=1; 
	for (int  li = strlen(instr)-1; li>=0; li--) 
	{ 
		if (instr[li]>= '0'   &&   instr[li] <= '9') 
		{ 
			tmp   =   instr[li]   -   48; 
		}   else   if   (instr[li]>= 'a'   &&   instr[li] <= 'z ') 
		{ 
			tmp   =   instr[li]   -   'a'   +   10; 
		}   else   if   (instr[li]>= 'A'   &&   instr[li] <= 'Z ') 
		{ 
			tmp   =   instr[li]   -   'A'   +   10; 
		} 
		result   +=   tmp   *   basic; 
		basic   *=   16; 
	} 
	
	switch(n)
	{
	
	case 6:
		sprintf(outstr, "%06ld", result);
		break;
	case 8:
		sprintf(outstr, "%08ld", result);
		break;
	case 10:
		sprintf(outstr,"%10ld",result);
		 break;
	case 12:
		sprintf(outstr, "%012ld", result);
		break;
	case 16:
		sprintf(outstr, "%016ld", result);
		break;
	default:
		sprintf(outstr, "%012ld", result);
		break;
	}
	
	return   0; 
}



//ת��M1card ת��10����
int CTools::changeM1CardId(const char * instr, char *outstr)    
{ 
	double    tmp = 0;
	double   result=0; 
	double   basic=1; 
	char outstr_tmp[10+1];
	bbzero(outstr_tmp);

	for (int li = strlen(instr)-1; li>=0; li--) 
	{ 
		if (instr[li]>= '0'   &&   instr[li] <= '9') 
		{ 
			tmp   =   instr[li]   -   48; 
		}   else   if   (instr[li]>= 'a'   &&   instr[li] <= 'z ') 
		{ 
			tmp   =   instr[li]   -   'a'   +   10; 
		}   else   if   (instr[li]>= 'A'   &&   instr[li] <= 'Z ') 
		{ 
			tmp   =   instr[li]   -   'A'   +   10; 
		} 
		result   +=   tmp   *   basic; 
		basic   *=   16; 
	} 
	
		sprintf(outstr_tmp,"%0.0lf",result);
		bbzero(outstr);
		sprintf(outstr,"%010s",outstr_tmp);
	
	return   0; 
}



//��ȡ M1����
int CTools::getDGTM1CardId(char *cardId,char * cardShowId)
{

	char card_10[10+1];
	BYTE card_5_byte[5+1];
	BYTE card_tranfer_5[5+1];
	char card_temp[10+1];

	changeM1CardId(cardId,card_10);
	bbzero(card_temp);
	bbzero(card_tranfer_5);
	hex_to_byte((BYTE *) card_10, (BYTE *)card_5_byte,10);
	for (int i=0;i<5;i++)
	{
		card_tranfer_5[i]=card_5_byte[4-i];

	}

	//����ת��
	byte_to_hex((BYTE * ) card_tranfer_5,(BYTE *)card_temp,5);

	for(int j=0;j<10;j++)
	{
		char buffer[1+1];
		bbzero(buffer);
		sprintf(buffer,"%c",card_temp[j]);

		if (atoi(buffer)>=5)
		{
			int temp;
			char temp_str[1+1];
			bbzero(temp_str);
			temp=atoi(buffer)-5;
			sprintf(temp_str,"%d",temp);
			cardShowId[j]=temp_str[0];
		}
		else
		{
			int temp;
			char temp_str[1+1];
			bbzero(temp_str);
			temp=5+atoi(buffer);
			sprintf(temp_str,"%d",temp);
			cardShowId[j]=temp_str[0];
		}

	}
		
   return 0;

}

//��ȡ��ǰ����ʱ��
//type : 0 - 2012/7/9 14:30:09
int CTools::getCurDatetime(char *curDatetime,int type)
{
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);
	bbzero(curDatetime);
	sprintf(curDatetime,"%02d/%02d/%02d %02d:%02d:%02d",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond);
   
	return 0;
}


//ʮ����ת��Ϊʮ������
int CTools::DecToHex(const char * strDec, char *strHex,int n)
{
	long lDec = 0;

	lDec = atol(strDec);

	bbzero(strHex);
	switch(n)
	{
	case 4:
		sprintf(strHex,"%04X",lDec);
		break;
	case 6:
		sprintf(strHex,"%06X",lDec);
		break;
	case 8:
		sprintf(strHex,"%08X",lDec);
		break;
	case 16:
		sprintf(strHex,"%016X",lDec);
		break;
	default:
		sprintf(strHex,"%016X",lDec);
		break;
	}

	return 0;
}


//��ȡϵͳ��keep �ն˺�+yyyyMMddhhmmss+4λ��ˮ�� 32 
int CTools::get_keep(char *keep )
{
	char current_time[15+1];
	char TMN[12+1];
	char rand_num[4+1];
	int num;

	bbzero(rand_num);
	bbzero(TMN);
	bbzero(current_time);
	get_termTime(current_time);
	strcpy(TMN,TMNNUM);
	
	srand((unsigned int )time(0));
	num=1000+rand()%(9000);
	sprintf(rand_num,"%d",num);
	strcpy(keep,TMN);
	strcat(keep,current_time);
	strcat(keep,rand_num);
	wrLog("get_keep:%s",keep);

	return 0;
}

//��ȡʱ�䣬��ȷ������1403241720345

void CTools:: get_msectime(char *time)
{
	SYSTEMTIME   systime;   
	::GetLocalTime(&systime);  
	
	memset(time, 0, sizeof(time));
	sprintf(time, "%02d%02d%02d%02d%02d%02d%03d",systime.wYear%100,systime.wMonth,systime.wDay,systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	wrLog("time:%s",time);
	
}


//��ȡϵͳ��keep �ն˺�+yyyyMMddhhmmss+8λ��ˮ�� 32 changed by liuxiao
int CTools::get_keep(char *term_id,char *keep )
{
	char current_time[15+1];
	char TMN[8+1];
	char  random_num[8+1];

	bbzero(TMN);
	bbzero(current_time);
	get_msectime(current_time);
	strcpy(TMN,term_id);
	generate_random_num(random_num,8);

	bbzero(keep);
	strcpy(keep,TMN); 
	strcat(keep,current_time); 
	strcat(keep,random_num);
	wrLog("get_keep:%s",keep);
	
	return 0;
}

//

//��ȡϵͳ��keep �ն˺�+yyyyMMddhhmmss+8λ��ˮ�� 32 changed by liuxiao
int CTools::get_keep(char *term_id,int *random_value,char *keep )
{
	char current_time[15+1];
	char TMN[8+1];
	char  random_num[8+1];

	*random_value=*random_value+1;
	
	bbzero(TMN);
	bbzero(current_time);
	get_msectime(current_time);
	strcpy(TMN,term_id);
	bbzero(random_num);
	generate_random_num(*random_value,random_num,8);
	wrLog("random:%s",random_num);
	
	bbzero(keep);
	strcpy(keep,TMN); 
	strcat(keep,current_time); 
	strcat(keep,random_num);
	wrLog("get_keep:%s",keep);
	
	return 0;
}

//��ʽ��ʽ 15+5λ�����
//13 09 26 17 17 89 234 XXXXX
int CTools::get_orderNo(char *orderNo)
{
	SYSTEMTIME   systime;   
	char random[5+1];
	char buff[20+1];
	bbzero(buff);
	bbzero(random);
	::GetLocalTime(&systime);  
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "%.2d%.2d%.2d%.2d%.2d%.3d",systime.wYear%1000,systime.wDay,systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	CTools::generate_random_num(random,5);
	sprintf(orderNo,"%s%s",buff,random);
	wrLog("���ɶ�����Ϊ:%s",orderNo);
	return 0;
}


//YYMMDD ת���ɳ�YYYYMMDD
int CTools::transYYYMMDD(char *oldStr,char *newStr)
{
	char currentDate[8+1];
	char  currentDate_buffer[6+1];
	char  oldStr_y[2+1];
	char current_2[2+1];

	bbzero(currentDate_buffer);
	bbzero(currentDate);
	bbzero(oldStr_y);
	bbzero(current_2);

	getDate(currentDate);
	memcpy(currentDate_buffer,currentDate+2,6);
	memcpy(current_2,currentDate,2);

	if (atoi(oldStr) <=atoi(currentDate_buffer))
	{
		bbzero(newStr);
		sprintf(newStr,"%d",atoi(current_2));
		strcat(newStr,oldStr);

	}
	else
	{
	
		bbzero(newStr);
		sprintf(newStr,"%d",(atoi(current_2)-1));
		strcat(newStr,oldStr);
	}
	
	return 0;

}


//��ݸ�������ͷ���ˮ�� 10λ�� �� + �� + 3λ����� +��λ�����
int CTools::getDGTLocalSeq(char *localSequence)
{

	SYSTEMTIME   systime;  
	char buff[6+1];
	char random1[4+1];
	char random2[4+1];
	int random_num=0;

	bbzero(buff);
	::GetLocalTime(&systime);
	sprintf(buff,"%02d%02d",systime.wMonth,systime.wDay);
	
	bbzero(random1);
	bbzero(random2);
	random_num+=1;
	CTools::generate_random_num(random_num,random1,3);
	random_num+=1;
	CTools::generate_random_num(random_num,random2,3);

	sprintf(localSequence,"%s%s%s",buff,random1,random2);
	wrLog("localSeq:%s",localSequence);

	return 0;

}


//
//flag=0,��0.flag=1ǰ��0
int CTools::addZero(char *oldStr,int askLen,int flag,char *newStr)
{
	
	char old_str_tmp[100];
	bbzero(old_str_tmp);
	int count=askLen-strlen(oldStr);
	
	if (count<=0)  //�������ַ������ڹ涨���ȣ���ȡ�ʵ�����
	{
		bbzero(newStr);
		memcpy(newStr,oldStr,askLen);
		return 0;
	}
	
	
	if (flag==0) //�����Ҫ���0
		
	{
		strcpy(old_str_tmp,oldStr);
		for(int j=0;j<count;j++)
		{
			
			strcat(old_str_tmp,"0");
			
		}
		
		bbzero(newStr);
		strcpy(newStr,old_str_tmp);
	}
	
	else //ǰ��0 
	{
		
		for(int j=0;j<count;j++)
		{
			
			strcat(old_str_tmp,"0");
		}
		
		strcat(old_str_tmp,oldStr);
		strcpy(newStr,old_str_tmp);
		
	}	
	
	return 0;
	
}


//�󲹿ո� Ĭ����flag=0 �󲹿ո�
int CTools::addSpace(char *oldStr,int askLen,int flag,char *newStr)
{
	char old_str_tmp[100];
	bbzero(old_str_tmp);
	int count=askLen-strlen(oldStr);
	
	if (count<=0)  //�������ַ������ڹ涨���ȣ���ȡ�ʵ�����
	{
		bbzero(newStr);
		memcpy(newStr,oldStr,askLen);
		return 0;
	}
	
	
	if (flag==0) //Ĭ���Ǻ�
		
	{
		strcpy(old_str_tmp,oldStr);
		for(int j=0;j<count;j++)
		{
			
			strcat(old_str_tmp," ");
			
		}
		
		bbzero(newStr);
		strcpy(newStr,old_str_tmp);
	}
	
	else 
	{
		
		for(int j=0;j<count;j++)
		{
			
			strcat(old_str_tmp," ");
		}
		
		strcat(old_str_tmp,oldStr);
		strcpy(newStr,old_str_tmp);
		
	}	
	
	return 0;

}

//��ݸ����������.���ն���ˮ�Ÿ��ã�9λ ʱ��+��������� 
int CTools::get_dgtOrderNo(char *orderN0)
{
	SYSTEMTIME   systime;  
	char buff[6+1];
	char random[3+1];
	bbzero(buff);
	bbzero(random);

	::GetLocalTime(&systime);
	sprintf(buff,"%02d%02d%02d",systime.wHour,systime.wMinute,systime.wSecond);

	bbzero(random);
	CTools::generate_random_num(random,3);
	bbzero(orderN0);
	memcpy(orderN0,buff,6);
	strcat(orderN0,random);
	return 0;
}


int  CTools::replace (string &oldString,string &oldStr,string &newStr)

{
	int pos=oldString.find(oldStr);
	while(pos!=-1)
	{
		oldString.replace(pos,oldStr.length(),newStr); 
		pos=oldString.find(oldStr);
		
	}
	return 0;
}



//�滻�ַ����ķ���

void CTools::CSreplace(std::string& s1,std::string& s2,std::string& s3,int count)
{
	std::string::size_type pos=0;
	std::string::size_type a=s2.size();
	std::string::size_type b=s3.size();
	
	if(count == -1)
	{
		while((pos=s1.find(s2,pos))!=std::string::npos)
		{
			s1.replace(pos,a,s3);
			pos+=b;
		}
	}
	else{
		int c = 0;
		while((pos=s1.find(s2,pos))!=std::string::npos)
		{
			s1.replace(pos,a,s3);
			pos+=b;
			if(++c == count)
				return;
		}
	}
}


//���ǺŽ�������
int CTools::shildWords(char *origWord,int indexStart,int totalLen,char *newWord)
{

	bbzero(newWord);
	memcpy(newWord, origWord,strlen(origWord));

	for(int i=0;i<totalLen;i++)	//����ʾ�������п���
	{
		newWord[i+indexStart] = '*';
	}

	return 0;

}