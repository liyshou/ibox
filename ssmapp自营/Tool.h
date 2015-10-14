// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TOOL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TOOL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once

#ifdef TOOL_EXPORTS
#define TOOL_API __declspec(dllexport)
#else
#define TOOL_API __declspec(dllimport)
#endif



#include "stdafx.h"
#include "markup.h"
#include "nb30.h"

typedef struct _ASTAT_ 
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff[30];
}ASTAT,*PASTAT; 

TOOL_API CString _GetVersion();
//�ַ���
#define STRING(n) #n
#define ZERO(a) memset((a),0,sizeof(a));
#define ATODW(A) (*(DWORD *)A)

TOOL_API CString _MakeString(char* format,...);//��ʽͬsprintf
/*_FormatString()������ַ��� һ���������ո񣬵�����ӡλ��
format��"?"��"*"��ͨ�����"?"��ʾsource���һ���ֽڡ�"*"��ʾsource�������ֽڡ�
autoEnd��source�Ѿ����Ƿ��ʣ���format����
����1��source="123456789"��format="?abc?def*gh"��autoEnd=0������ֵ=1abc2def3456789gh
����2��source="123456789"��format="?abc?def*gh"��autoEnd=1������ֵ=1abc2def3456789
*/
TOOL_API CString _FormatString(CString source,CString format,int autoEnd=1);
/*_AmountUpperCase(CString):�����Сдת��д
����1��lowerCase="123.45"������ֵ=Ҽ�۷�ʰ��Ԫ�Ľ����
����2��lowerCase="123.4"������ֵ=Ҽ�۷�ʰ��Ԫ�Ľ���
*/
TOOL_API CString _AmountUpperCase(CString lowerCase);
/*_OneBitNumber2ChineseChar(char):����ת����
����1��num='1'������ֵ=Ҽ
����2��num='0'������ֵ=��
*/
TOOL_API CString _OneBitNumber2ChineseChar(char num);
TOOL_API CString _EditAmt(CString in);
TOOL_API int _GetCharCount(CString in,char character);
TOOL_API CString _GetField(CString in,int n,char cFilter);
TOOL_API int _GetFieldCount(CString in,char cFilter);

//�ļ�
TOOL_API bool _DeleteFile(CString from);
TOOL_API bool _CopyFile(CString from,CString to);//pTo��·���������ļ����������ڵ�Ŀ¼���Զ��½�
TOOL_API bool _CopyFileBinary(CString from,CString to);//to��·�����ļ�������˿��Ը���˳��������
TOOL_API bool _MoveFile(CString from,CString to);
TOOL_API bool _RenameFile(CString from,CString to);
TOOL_API bool _CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
TOOL_API bool _RenameFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
TOOL_API bool _FolderExist(CString strPath);
TOOL_API ULONGLONG _GetDirSize(CString strDirPath);

//XML
TOOL_API bool _GoToXML(CMarkup &markup,CString tree,int isMustExist=0);//�Ե�ǰλ��Ϊ��㣬��λ��ָ��λ�á�isMustExist=0�����������½������һ���ɹ���isMustExist=1����������ʧ�ܡ�
TOOL_API bool _GoIntoXML(CMarkup &markup,CString tree,int isMustExist=0);//�Ե�ǰλ��Ϊ��㣬���뵽ָ��λ�á�isMustExist=0�����������½������һ���ɹ���isMustExist=1����������ʧ�ܡ�
TOOL_API bool _FindElemByAttribXML(CMarkup &markup,CString tagName,CString attribName,CString attribValue);//�ӵ�ǰλ���²���ָ����ǩ��������ֵ��Ԫ�ء�
TOOL_API CString _GetDataXML(CString tree,CString path);//tree��xml���ݵĲ�Σ���\\���ָ����������ļ�ϵͳĿ¼����path��xml�ļ�·����
TOOL_API bool _SetDataXML(CString tree,CString value,CString path);
TOOL_API bool _DeleteDataXML(CString tree,CString path);
TOOL_API CString _GetDataXMLCM(CString tree,CMarkup cm);
TOOL_API void _SetDataXMLCM(CString tree,CString value,CMarkup &cm);
TOOL_API void _DeleteDataXMLCM(CString tree,CMarkup &cm);
TOOL_API CString _GetDataXMLString(CString tree,CString data);//tree��xml���ݵĲ�Σ���\\���ָ����������ļ�ϵͳĿ¼����data��xml���ݡ�
TOOL_API void _SetDataXMLString(CString tree,CString value,CString &data);
TOOL_API void _DeleteDataXMLString(CString tree,CString &data);
/*
	_SelectDataAndPackXML()����XML��ǰλ���� �ڶ���������ͨ��ָ����ǩ������ѡȡ�ض��������һ����¼��
	markup����ȷ����xml�ļ��Ķ��󣬲����ƶ�����������·���¡�
	tagName���������ݵı�ǩ��
	attribName�������ж��Ƿ�ѡȡ��������
	attribValue����������������ֵ���������ֵ�÷ָ�����������"1,3,5,6"����ʾѡȡ1��3��5��6����������һ����¼��
	attribValueSeparator���������ֵ֮��ķָ�����
	fieldSeparator���������֮��ķָ�����
	isAddNO�������Ƿ���ÿ��ǰ�ӱ�� 0���� 1����� 2������ֵ
	NOSeparator����ź�����֮��ķָ�����
*/
TOOL_API CString _SelectDataAndPackXML(CMarkup &markup,CString attribValue,int isAddNO=0,char fieldSeparator='^',CString tagName="Field",CString attribName="NO",char attribValueSeparator=',',char NOSeparator='^');

//Ӳ��
TOOL_API char _GetFirstRemovable();//��ȡ��һ���ƶ��豸�̷���������U��������
TOOL_API CString _GetLocalIP();
TOOL_API CString _GetLocalMAC();

//ʱ��
TOOL_API CString _GetDate();
TOOL_API CString _GetTime();
TOOL_API CString _GetYear();
TOOL_API CString _GetMonth();
TOOL_API CString _GetDay();

//ϵͳ����
TOOL_API int _InitWSAEnviorment();//��ʼ��wosa������һ������tcp/ipЭ�顣
TOOL_API int _RegisterOcx(CString csPathName);
TOOL_API int _UnregisterOcx(CString csPathName);
//����
TOOL_API CString _XML2HTTP(CString csXML,CString tranCode,CString IP,int port);
TOOL_API void _ConvertGBKToUtf8(CString& strGBK);
TOOL_API void _ConvertUtf8ToGBK(CString& strUtf8);
TOOL_API void _ConvertGBKToURL(CString& strGBK);
TOOL_API CString _EncodeMD5(char *pData);
TOOL_API CString _Base64Decode(char *pBase64Src,int iSrcLen);
TOOL_API CString _Bass64Encode(const char *pInData,int iDataLen);
TOOL_API CString _ANSI2UTF8(char *strANSI,int iLen);
TOOL_API CString _UTF8ToANSI(const char* utf8);
TOOL_API CString _Hex2ASC(const BYTE *Hex, int Len);
//ע���
TOOL_API int _RegGetValueInt(CString tree,int &out);
TOOL_API int _RegGetValueString(CString tree,CString &out);
TOOL_API int _RegSetValueInt(CString tree,int in);
TOOL_API int _RegSetValueString(CString tree,CString in);
TOOL_API int _RegDeleteValue(CString tree);