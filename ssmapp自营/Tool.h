// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TOOL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TOOL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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
//字符串
#define STRING(n) #n
#define ZERO(a) memset((a),0,sizeof(a));
#define ATODW(A) (*(DWORD *)A)

TOOL_API CString _MakeString(char* format,...);//格式同sprintf
/*_FormatString()：填充字符串 一般用于填充空格，调整打印位置
format里"?"和"*"是通配符。"?"表示source里的一个字节。"*"表示source的所有字节。
autoEnd：source已尽，是否把剩余的format附后。
例子1：source="123456789"，format="?abc?def*gh"，autoEnd=0，返回值=1abc2def3456789gh
例子2：source="123456789"，format="?abc?def*gh"，autoEnd=1，返回值=1abc2def3456789
*/
TOOL_API CString _FormatString(CString source,CString format,int autoEnd=1);
/*_AmountUpperCase(CString):人民币小写转大写
例子1：lowerCase="123.45"，返回值=壹佰贰拾叁元四角伍分
例子2：lowerCase="123.4"，返回值=壹佰贰拾叁元四角整
*/
TOOL_API CString _AmountUpperCase(CString lowerCase);
/*_OneBitNumber2ChineseChar(char):数字转汉字
例子1：num='1'，返回值=壹
例子2：num='0'，返回值=零
*/
TOOL_API CString _OneBitNumber2ChineseChar(char num);
TOOL_API CString _EditAmt(CString in);
TOOL_API int _GetCharCount(CString in,char character);
TOOL_API CString _GetField(CString in,int n,char cFilter);
TOOL_API int _GetFieldCount(CString in,char cFilter);

//文件
TOOL_API bool _DeleteFile(CString from);
TOOL_API bool _CopyFile(CString from,CString to);//pTo是路径，不含文件名，不存在的目录被自动新建
TOOL_API bool _CopyFileBinary(CString from,CString to);//to是路径含文件名，因此可以复制顺带改名。
TOOL_API bool _MoveFile(CString from,CString to);
TOOL_API bool _RenameFile(CString from,CString to);
TOOL_API bool _CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
TOOL_API bool _RenameFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath);
TOOL_API bool _FolderExist(CString strPath);
TOOL_API ULONGLONG _GetDirSize(CString strDirPath);

//XML
TOOL_API bool _GoToXML(CMarkup &markup,CString tree,int isMustExist=0);//以当前位置为起点，定位到指定位置。isMustExist=0，不存在则新建，因此一定成功。isMustExist=1，不存在则失败。
TOOL_API bool _GoIntoXML(CMarkup &markup,CString tree,int isMustExist=0);//以当前位置为起点，进入到指定位置。isMustExist=0，不存在则新建，因此一定成功。isMustExist=1，不存在则失败。
TOOL_API bool _FindElemByAttribXML(CMarkup &markup,CString tagName,CString attribName,CString attribValue);//从当前位置下查找指定标签名和属性值的元素。
TOOL_API CString _GetDataXML(CString tree,CString path);//tree是xml内容的层次，以\\作分隔符，类似文件系统目录树。path是xml文件路径。
TOOL_API bool _SetDataXML(CString tree,CString value,CString path);
TOOL_API bool _DeleteDataXML(CString tree,CString path);
TOOL_API CString _GetDataXMLCM(CString tree,CMarkup cm);
TOOL_API void _SetDataXMLCM(CString tree,CString value,CMarkup &cm);
TOOL_API void _DeleteDataXMLCM(CString tree,CMarkup &cm);
TOOL_API CString _GetDataXMLString(CString tree,CString data);//tree是xml内容的层次，以\\作分隔符，类似文件系统目录树。data是xml内容。
TOOL_API void _SetDataXMLString(CString tree,CString value,CString &data);
TOOL_API void _DeleteDataXMLString(CString tree,CString &data);
/*
	_SelectDataAndPackXML()：从XML当前位置下 在多条数据中通过指定标签和属性选取特定数据组成一条记录。
	markup：正确加载xml文件的对象，并已移动到数据所在路径下。
	tagName：保存数据的标签名
	attribName：用于判断是否选取的属性名
	attribValue：符合条件的属性值。多个属性值用分隔符隔开。如"1,3,5,6"，表示选取1、3、5、6域的数据组成一条记录。
	attribValueSeparator：多个属性值之间的分隔符。
	fieldSeparator：多个数据之间的分隔符。
	isAddNO：决定是否在每域前加编号 0不加 1加序号 2加属性值
	NOSeparator：序号和数据之间的分隔符。
*/
TOOL_API CString _SelectDataAndPackXML(CMarkup &markup,CString attribValue,int isAddNO=0,char fieldSeparator='^',CString tagName="Field",CString attribName="NO",char attribValueSeparator=',',char NOSeparator='^');

//硬件
TOOL_API char _GetFirstRemovable();//获取第一个移动设备盘符。可用于U盘升级。
TOOL_API CString _GetLocalIP();
TOOL_API CString _GetLocalMAC();

//时间
TOOL_API CString _GetDate();
TOOL_API CString _GetTime();
TOOL_API CString _GetYear();
TOOL_API CString _GetMonth();
TOOL_API CString _GetDay();

//系统环境
TOOL_API int _InitWSAEnviorment();//初始化wosa环境，一般用于tcp/ip协议。
TOOL_API int _RegisterOcx(CString csPathName);
TOOL_API int _UnregisterOcx(CString csPathName);
//编码
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
//注册表
TOOL_API int _RegGetValueInt(CString tree,int &out);
TOOL_API int _RegGetValueString(CString tree,CString &out);
TOOL_API int _RegSetValueInt(CString tree,int in);
TOOL_API int _RegSetValueString(CString tree,CString in);
TOOL_API int _RegDeleteValue(CString tree);