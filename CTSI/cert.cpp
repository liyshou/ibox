#include "StdAfx.h"
#include <stdio.h>
#include "cert.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "WrLog.h"
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>


#include "Tools.h"
#include "cert.h"

#pragma comment (lib,"libeay32.lib")
#pragma comment (lib,"ssleay32.lib")


Ccert::Ccert()
{

}

Ccert::~Ccert()
{

	
}


/*
* ����		--	ǩ��
* �������	--privateKEYFile:˽Կ�ļ�����λ��
*			--unsighedData:��ǩ��������
			--signedData:ǩ������
* return	--	0Ϊ�ɹ�������Ϊʧ��
*/
int Ccert::data_sign(const string &privateKEYFile, const string &data,string &signedData)
{
	
	EVP_MD_CTX md_ctx;
	EVP_PKEY *pkey;
	int err=0;
	unsigned int sign_len=0;

	//��ʼ��
	SSLeay_add_all_algorithms();
	ERR_load_crypto_strings();
	BIO *bio=BIO_new(BIO_s_file());
	int i =BIO_read_filename(bio,privateKEYFile.c_str());
	if(i<=0)
	{
		puts("read key file err");
		return -1 ;
		
		
	}
	
	pkey=PEM_read_bio_PrivateKey(bio,NULL,NULL,NULL);
	
	if (pkey==NULL)
	{
		ERR_print_errors_fp(stderr);
		return -1;
		
	}
	
	//��������ǩ��
	sign_len=256;
	char *pcString=NULL;
	pcString=new char[sign_len];
	memset(pcString,0,sign_len);

	EVP_SignInit(&md_ctx,EVP_sha1());
	EVP_SignUpdate(&md_ctx,data.c_str(),data.size());
	err=EVP_SignFinal(&md_ctx,(unsigned char *)pcString,&sign_len,pkey);
	if(err!=1)
	{
		
		ERR_print_errors_fp(stderr);
		return -1;
		
	}
	
	signedData.assign(pcString,sign_len);
	delete [] pcString;
	EVP_MD_CTX_cleanup(&md_ctx);
	BIO_free(bio);
	return 0;
	
}

/*
* ����		--��ǩ
* �������	--certFile������֤��·��
*			--strUnSign:����ǩ������
*           --strsign ����ǩ������

* return	--0Ϊ�ɹ�������Ϊʧ��
*/
int Ccert::cert_verify(const string &cerString,const string &data,const string &signStr)
{

	X509 *x509;
	EVP_MD_CTX md_ctx;
	EVP_PKEY *pkey;
	int err;
	string certStr="";
    ERR_load_crypto_strings();
	//��ȡ��Ϣ
	BIO *bio=NULL;
	//����Կ��Ϣ������ļ���ʽ
	getCertString(cerString,certStr);
	if ((bio=BIO_new_mem_buf((char *) certStr.c_str(),certStr.size()))==NULL)
	{
		return -1;
	}
	
	
	x509=PEM_read_bio_X509(bio,NULL,0,NULL);

	if (x509==NULL)
	{
		ERR_print_errors_fp(stderr);
		return -1;
	}

	//��ȡ��Կ
	pkey=X509_get_pubkey(x509);
	if (pkey==NULL)
	{
		ERR_print_errors_fp(stderr);
		if (x509!=NULL)
		{
			X509_free(x509);
		}
		return -1;
	}

	//��ǩ
	EVP_VerifyInit(&md_ctx,EVP_sha1());
	EVP_VerifyUpdate(&md_ctx,data.c_str(),data.size());
	unsigned char aucSign[512]={0};
	memcpy(aucSign,signStr.c_str(),signStr.size());
	wrLog("ǩ�ֳ���:%d",signStr.size());
	err=EVP_VerifyFinal(&md_ctx,aucSign,signStr.size(), pkey);
	if (err!=1)
	{
		wrLog("��ǩʧ��");
		//ERR_print_errors_fp(stderr);
		if (pkey!=NULL)
		{
			EVP_PKEY_free(pkey);
		}
		if (x509==NULL)
		{
			X509_free(x509);
		}
	
		return -1;
	}

	wrLog("��ǩ�ɹ�");
	if (pkey!=NULL)
	{
		EVP_PKEY_free(pkey);
	}

	if (x509 !=NULL)
	{
		X509_free(x509);

	}

	BIO_free(bio);


	return 0;

}


/*
* ����		--��ǩ
* �������	--certFile������֤��·��
*			--strUnSign:����ǩ������
*           --strsign ����ǩ������

  * return	--0Ϊ�ɹ�������Ϊʧ��
*/
int Ccert::cert_verify_file(const string &cerFileName,const string &data,const string &signStr)
{
	
	X509 *x509;
	EVP_MD_CTX md_ctx;
	EVP_PKEY *pkey;
	int err;

    ERR_load_crypto_strings();
	//��ȡ��Ϣ
	BIO *bio=BIO_new(BIO_s_file());
	int i =BIO_read_filename(bio,cerFileName.c_str());
	if(i<=0)
	{
		puts("read key file err");
		return -1 ;
		
		
	}
	
	x509=PEM_read_bio_X509(bio,NULL,NULL,NULL);
	
	if (x509==NULL)
	{
		ERR_print_errors_fp(stderr);
		return -1;
	}
	
	//��ȡ��Կ
	pkey=X509_get_pubkey(x509);
	if (pkey==NULL)
	{
		ERR_print_errors_fp(stderr);
		if (x509!=NULL)
		{
			X509_free(x509);
		}
		return -1;
	}
	
	//��ǩ
	EVP_VerifyInit(&md_ctx,EVP_sha1());
	EVP_VerifyUpdate(&md_ctx,data.c_str(), data.size());
	unsigned char aucSign[512]={0};
	memcpy(aucSign,signStr.c_str(),signStr.size());
	err=EVP_VerifyFinal(&md_ctx,aucSign,signStr.size(), pkey);
	if (err!=1)
	{
		ERR_print_errors_fp(stderr);
		if (pkey!=NULL)
		{
			EVP_PKEY_free(pkey);
		}
		if (x509==NULL)
		{
			X509_free(x509);
		}
		return -1;
	}
	
	BIO_free(bio);
	if (pkey!=NULL)
	{
		EVP_PKEY_free(pkey);
	}
	
	if (x509 !=NULL)
	{
		X509_free(x509);
		
	}

	return 0;
	
}


//��ȡ��Կ��Ϣ
//��֤����ǩ��
int Ccert::cert_key(char *certFile, string &cert)
{
	
	FILE *fp=NULL;
	X509 *x509;
	EVP_PKEY *pkey;
	string cert_string;
	string oldStr="\n";
	string changeStr="";
	string strStart="-----BEGIN CERTIFICATE-----";
	string strEnd="-----END CERTIFICATE-----";

    ERR_load_crypto_strings();
	//��ȡ��Կ

	BIO *bio=BIO_new(BIO_s_file());
	int i =BIO_read_filename(bio,certFile);
	if(i<=0)
	{
		puts("read key file err");
		return -1 ;
		
		
	}

	x509=PEM_read_bio_X509(bio,NULL,0,NULL);
	if (x509==NULL)
	{
		ERR_print_errors_fp(stderr);
		return -1;
	}
	
	//��ȡ��Կ
	pkey=X509_get_pubkey(x509);
	if (pkey==NULL)
	{
		ERR_print_errors_fp(stderr);
		return -1;
	}

	BUF_MEM *bptr=NULL;
	BIO * bio_cert=BIO_new((BIO_s_mem()));
	BIO_set_close(bio_cert,BIO_CLOSE);
	PEM_write_bio_X509(bio_cert,x509);
	BIO_get_mem_ptr(bio_cert,&bptr);
	cert.assign(bptr->data,bptr->length);
	BIO_free_all(bio_cert);
	BIO_free_all(bio);
	
	//ɾ�����з�����β��֤������
	CTools::replace(cert,oldStr,changeStr);
	CTools::replace(cert,strStart,changeStr);
	CTools::replace(cert,strEnd,changeStr);

	return 0;
	
}


int  Ccert::replace (string &oldString,string &oldStr,string &newStr)

{

	int pos=oldString.find(oldStr);
	while(pos!=-1)
	{
		oldString.replace(pos,oldStr.length(),newStr);
		pos=oldString.find(oldStr);
		
	}
	return 0;
}

int Ccert::getCertString(string str,string &cerString) //����Կ�ַ���

{
	char stringBuffer[2048];
	char cerStringBuffer[2048];
	char subString[65+1];
	char str_tmp[500];
	memset(cerStringBuffer,0,sizeof(cerStringBuffer));
	memset(str_tmp,0,sizeof(str_tmp));
	memset(stringBuffer,0,sizeof(stringBuffer));
	int len=0;
	int count=0;
	len=str.size();
	count=len/64;
	int header=0;
	
	memcpy(stringBuffer,str.c_str(),str.size());
	sprintf(cerStringBuffer,"-----BEGIN CERTIFICATE-----\n"); //֤���ļ���ͷ
	header=strlen(cerStringBuffer);

	for (int i=0;i<count;i++)
	{

		memset(subString,0,sizeof(subString));
		memcpy(subString,stringBuffer+i*64,64);
		strcat(subString,"\n");
		strcat(cerStringBuffer,subString);

	}

	if (len%64 !=0)
	{
		memset(subString,0,sizeof(subString));
		memcpy(subString,stringBuffer+count*64,(len%64));
		strcat(cerStringBuffer,subString);
		strcat(cerStringBuffer,"\n");
	}

	//�ļ���ͷ�Ӻ�ĩβ׷��
	strcat(cerStringBuffer,"-----END CERTIFICATE-----");

	//wrLog("��Կ�ַ���Ϊ:%s",cerStringBuffer);

	cerString.assign(cerStringBuffer,strlen(cerStringBuffer));

	//wrLog("cerString:%s",cerString.c_str());
	return 0;
}