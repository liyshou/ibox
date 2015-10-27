// cert.h: interface for the cert.h class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CERT_H__D2B97D30_FAD4_40A4_8540_F845D73E2B18__INCLUDED_)
#define AFX_CERT_H__D2B97D30_FAD4_40A4_8540_F845D73E2B18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <string>
using namespace std;

class Ccert  

{
	
public:

	Ccert();
	virtual ~Ccert();
	//int sign(const string &_strPass, const string &_strUnSing, string &_strSign, string &_strCert);
	int static cert_verify(const string &certStr,const string &strUnSign,const string &strsign);
	int static cert_key(char *certFile,string &publickey);
	int static data_sign(const string &privateKEYFile,const string &data,string &signedData);
	int  static replace (string &oldString,string &oldStr,string &newStr);
	int  static getCertString(string str,string &cerString); //´¦Àí¹«Ô¿×Ö·û´®
	int static cert_verify_file(const string &cerFileName,const string &data,const string &signStr);

};

#endif // !defined(AFX_CERT_H__D2B97D30_FAD4_40A4_8540_F845D73E2B18__INCLUDED_)
