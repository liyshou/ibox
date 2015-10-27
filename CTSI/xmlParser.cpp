// xmlParser.cpp: implementation of the CxmlParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "xmlParser.h"
#include "TSCTSI.h"
#include <winsock2.h>
#include "Global.h"
#include "tinystr.h" //add by liuxiao
#include "tinyxml.h" //add by liuxiao
#include "cert.h"
#include "Tools.h"
#include "base64.h" 

#include <IOSTREAM>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//����(���в�ѯ(ˮ��ú))
const field_cfg acReq_PublicTransQuery[] = 
{
    0,  "ORGANNO",      "", "MERID",        "VerifyParameter",    "", //������
    0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ80
    0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
    1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
    0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
    0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
    0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
    0,  "TRANSDT",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
    0,  "TERMINALNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
    0,  "PCITYCODE",    "",             "ACCEPTAREACODE",   "PARAMETERS", "PayPlatRequestParameter", //����������� 
    0,  "TRANSDT",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //�������� yyyyMMddhhmmss
    1,  "REMARK1",      "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����1
    1,  "REMARK2",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����2
    0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};

//����(�˵���ѯ(ˮ��ú))
const field_cfg acReq_PublicBillQuery[] = 
{
    0,  "ORGANNO",      "", "MERID",        "VerifyParameter",    "", //������
    0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ80
    0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
    1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
    0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
    0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
    0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
    0,  "TRANSDT",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
    
    0,  "LINENO",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //��ˮ�� 
    0,  "TERMINALNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
    0,  "SELECTTYPE",   "",             "SELECTTYPE",       "PARAMETERS", "PayPlatRequestParameter", //��ѯ���ͣ�001:�û��ţ�002:������
    0,  "TRIORDERNO",   "",             "SELECTVALUE",      "PARAMETERS", "PayPlatRequestParameter", //�û��Ż�������,���ݲ�ѯ������д��Ӧ��ֵ
    0,  "TRANSDT",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //�������� yyyyMMddhhmmss
    0,  "PCITYCODE",    "",             "ACCEPTAREACODE",   "PARAMETERS", "PayPlatRequestParameter", //����������� 
    0,  "PAYMENTCODE",  "",             "ADDITEM1",         "PARAMETERS", "PayPlatRequestParameter", //�ɷ�ʡ���루6λ��+�ɷѳ��д��루6λ��+�ɷ���Ŀ��ţ�4λ��
    0,  "QUERYAMOUNT",  "",             "ADDITEM2",         "PARAMETERS", "PayPlatRequestParameter", //�Ϻ�ˮ��ú��Ҫ��������� 
    1,  "TICKETTITLE",  "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����1,������ů��Ϊ��Ʊ̨ͷʹ�ã����ұ������롣
    1,  "REMARK2",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����2
    1,  "REMARK3",      "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����3
    1,  "REMARK4",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����4
    0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};

//����(�ɷ�(ˮ��ú))
const field_cfg acReq_PublicPay[] = 
{
    0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
    0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ80
    0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
    1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
    0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
    0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
    0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
    0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
    0,  "ORDERSEQ",       "",            "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //��ˮ�� 
    0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
    0,  "ACCEPTDATE",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //�������� yyyyMMddhhmmss
    0,  "SYSTEMNO",  "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //ϵͳ�ο���
    0,  "TXNAMOUNT",       "",             "TXNAMOUNT",        "PARAMETERS", "PayPlatRequestParameter", //���
    0,  "CASHTYPE",     "",             "CASHTYPE",         "PARAMETERS", "PayPlatRequestParameter", //���˵�������,0-����  1-�ɷѵ��� 
    0,  "CASHORDER",   "",             "CASHORDER",        "PARAMETERS", "PayPlatRequestParameter", //����
	0,  "CASHNUMBER",   "",             "CASHNUMBER",        "PARAMETERS", "PayPlatRequestParameter", //���˵���
	0,  "PAYTYPE",      "",             "PAYTYPE",          "PARAMETERS", "PayPlatRequestParameter", //֧����ʽ, ��"1"
    0,  "ECARDNO",   "",             "ECARDNO",        "PARAMETERS", "PayPlatRequestParameter", //E����Ϣ
	0,  "PASSFLAG",   "",             "PASSFLAG",        "PARAMETERS", "PayPlatRequestParameter", //��������   
    0,  "PSAMCARDNO",        "",             "PSAMCARDNO",       "PARAMETERS", "PayPlatRequestParameter", //PSAM����
	0,  "OPERUSER",        "",             "OPERUSER",       "PARAMETERS", "PayPlatRequestParameter", //��ҵ�˻�����Ԫ
	0,  "OPERPASSWORD",        "",             "OPERPASSWORD",       "PARAMETERS", "PayPlatRequestParameter", //��ҵ�˻�����Ա����
    1,  "REMARK1",      "",             "REMARK1",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����1
    1,  "REMARK2",      "",             "REMARK2",          "PARAMETERS", "PayPlatRequestParameter", //Ԥ����2
    0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


// ����˰ �˵���ѯ����
const field_cfg acReq_carBillQuery[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //���׷�����ˮ��
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		1,  "PLATENO",    "",             "PLATENO",           "PARAMETERS", "PayPlatRequestParameter", //���ƺ�
		0,  "ENGINENO",      "",             "ENGINENO",       "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "ACCEPTDATE",      "",            "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //�̻�����ʱ��
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//����˰�����˵�Ӧ����
const field_cfg acRsp_carBillQuery[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������            
		0,  "SYSTEMNO",       "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //ϵͳ�ο���
		0,  "BILLNUM",        "",                 "BILLNUM",           "RESULTDATESET",    "PayPlatResponseParameter", //�˵���¼��
		0,  "TMNNUMNO",          "",                 "TMNNUMNO",       "RESULTDATESET",    "PayPlatResponseParameter", //����ʱ��
		0,  "DATAS",            "",                 "DATAS",            "RESULTDATESET",    "PayPlatResponseParameter", //�ͻ��ն˺�
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};

//����˰�˵��ɷ�������

const field_cfg acReq_carTaxPay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ80
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //��ˮ�� 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E��
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //Ĭ��Ϊ2
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //psam����
		0,  "ACCEPTDATE",      "",             "ACCEPTDATE",       "PARAMETERS", "PayPlatRequestParameter", //�������� yyyyMMddhhmmss
		0,  "SYSTEMNO",  "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //ϵͳ�ο���
		0,  "TXNAMOUNT",       "",             "TXNAMOUNT",        "PARAMETERS", "PayPlatRequestParameter", //���
		1,  "PLATENO",     "",             "PLATENO",         "PARAMETERS", "PayPlatRequestParameter", //���ƺ�8λ 
		0,  "PAYTYPE",      "",             "PAYTYPE",          "PARAMETERS", "PayPlatRequestParameter", //֧����ʽ, ��"3"    
		0,  "OPERUSER",        "",             "OPERUSER",       "PARAMETERS", "PayPlatRequestParameter", //��ҵ�˻�����Ա
		0,  "OPERPASSWORD",       "",             "OPERPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //��ҵ�˻�����Ԫ����
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//���п���Ϣ��ѯ
const field_cfg acReq_bankInfoQry[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ80
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "CUSTCODE",       "",             "CUSTCODE",         "PARAMETERS", "PayPlatRequestParameter",  
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", 
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//��ѯ���п�Ӧ������
const field_cfg acRsp_bankInfoQry[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "RESULTDATASET",       "",					"RESULTDATASET",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//����˰�ɷ�Ӧ����
const field_cfg acRsp_carTaxPay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "SYSTEMNO",      "",					 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //������
		1,  "PLATENO",         "",                 "PLATENO",         "RESULTDATESET",    "PayPlatResponseParameter", //���˵�������,0-����  1-�ɷѵ���
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//3G��������ֵ������
const field_cfg acReq_g3Pay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERNO",       "",             "ORDERNO",         "PARAMETERS", "PayPlatRequestParameter", //������ 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E����Ϣ
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAM��
		0,  "STAFFCODE",      "",             "STAFFCODE",       "PARAMETERS", "PayPlatRequestParameter", //�û���
		0,  "PAYPASSWORD",  "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "RECHARGETYPE",       "",             "RECHARGETYPE",        "PARAMETERS", "PayPlatRequestParameter", //��ֵ����
		0,  "VERIFY",       "",             "VERIFY",        "PARAMETERS", "PayPlatRequestParameter", //�Ƿ���Ҫ��֤�ֻ��� 
		0,  "PHONE",     "",             "PHONE",         "PARAMETERS", "PayPlatRequestParameter",		//�����ֻ�����		
		0,  "RECHARGEFLOW",      "",             "RECHARGEFLOW",          "PARAMETERS", "PayPlatRequestParameter", //��������   
		0,  "ACCEPTAREACODE",        "",             "ACCEPTAREACODE",       "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "TXNAMOUNT",        "",             "TXNAMOUNT",       "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "TRADETIME",       "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //��������ʱ��
		0,  "SYSTEMNO",       "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//ȫ�����ѳ�ֵ������

const field_cfg acReq_phonePay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERNO",       "",             "ORDERNO",         "PARAMETERS", "PayPlatRequestParameter", //������ 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E����Ϣ
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAM��
		0,  "STAFFCODE",      "",             "STAFFCODE",       "PARAMETERS", "PayPlatRequestParameter", //�û���
		0,  "PAYPASSWORD",  "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "RECHARGETYPE",       "",             "RECHARGETYPE",        "PARAMETERS", "PayPlatRequestParameter", //��ֵ���� 03010008��ȫ������ֱ�� 05010005ȫ���ƶ�ֱ��1601000 ȫ����ֱͨ��
		0,  "PHONE",     "",             "PHONE",         "PARAMETERS", "PayPlatRequestParameter",		//�����ֻ�����		
		0,  "RECHARGEAMOUNT",      "",             "RECHARGEAMOUNT",          "PARAMETERS", "PayPlatRequestParameter", //������
		0,  "ACCEPTAREACODE",        "",             "ACCEPTAREACODE",       "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "TXNAMOUNT",        "",             "TXNAMOUNT",       "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "TRADETIME",       "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //��������ʱ��
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};

//�ƶ����㶫ʡ�ڣ��ֻ����ѳ�ֵ������
const field_cfg acReq_phonePayGDYD[] = 
{
	0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //������ 
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E����Ϣ
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAM��
		0,  "STAFFCODE",      "",             "STAFFCODE",       "PARAMETERS", "PayPlatRequestParameter", //�û���
		0,  "PAYPASSWORD",  "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "PHONE",     "",             "PHONE",         "PARAMETERS", "PayPlatRequestParameter",		//�����ֻ�����
		0,  "RECHARGETYPE",     "",             "RECHARGETYPE",         "PARAMETERS", "PayPlatRequestParameter",		//��ֵ����
		0,  "TXAMOUNT",        "",             "TXAMOUNT",       "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "TRADETIME",       "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //��������ʱ��
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};

//�˻���֤��ѯ�ӿ� 
const field_cfg acReq_g3PhoneCheck[] = 
{
	0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "VERIFY",       "",             "VERIFY",        "PARAMETERS", "PayPlatRequestParameter", //0:�����˻�	1��3G������ֵ����дĬ��Ϊ�����˻���֤
		0,  "ACCTCODE",     "",             "ACCTCODE",         "PARAMETERS", "PayPlatRequestParameter", //�˻�����
		0,  "ACCEPTAREACODE",     "",             "ACCEPTAREACODE",         "PARAMETERS", "PayPlatRequestParameter",
		0,  "ACCEPTTIME",     "",             "ACCEPTTIME",         "PARAMETERS", "PayPlatRequestParameter",		
		0,  "ACCEPTDATE",       "",             "ACCEPTDATE",         "PARAMETERS", "PayPlatRequestParameter", //��������ʱ��
		0,  "REAMOUNT",       "",             "REAMOUNT",         "PARAMETERS", "PayPlatRequestParameter", 
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//��ݸ��������ȷ�Ͻӿ�
const field_cfg acReq_dgt_orderVerify[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //�̻�ƽ̨���ɵĶ�����
		0,  "TRANSSEQ",       "",             "TRANSSEQ",        "PARAMETERS", "PayPlatRequestParameter", //������ˮ��
		0,  "TMNNUMNO",     "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ͻ��ն˺�
		0,  "BUSCODE",     "",             "BUSCODE",         "PARAMETERS", "PayPlatRequestParameter", //ҵ���������
		1,  "BUSREASON",     "",             "BUSREASON",         "PARAMETERS", "PayPlatRequestParameter",	//ҵ���������	
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//��ݸ����֪ͨ�������ɽӿ� ����

const field_cfg acReq_dgt_orderProduce[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
	
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //������
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "STAFFCODE",       "",             "STAFFCODE",        "PARAMETERS", "PayPlatRequestParameter", //�û���
		0,  "TRADETIME",     "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "EDCARDID",     "",             "EDCARDID",         "PARAMETERS", "PayPlatRequestParameter", //M1����  
		0,  "EACOUNT",     "",             "EACOUNT",         "PARAMETERS", "PayPlatRequestParameter",	//M1���	
		0,  "RETURNACOUNT",       "",             "RETURNACOUNT",         "PARAMETERS", "PayPlatRequestParameter", //д���¿����  
		0,  "RETURNDEPOSIT",       "",             "RETURNDEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //���û�Ѻ��
		0,  "MONTHFEFLAG",       "",             "MONTHFEFLAG",         "PARAMETERS", "PayPlatRequestParameter", //�·���ȡ��־ 
		0,  "FEEACOUNTFLAG",       "",             "FEEACOUNTFLAG",         "PARAMETERS", "PayPlatRequestParameter", //��������ȡ��־
		0,  "MONTHFEEACOUNT",       "",             "MONTHFEEACOUNT",         "PARAMETERS", "PayPlatRequestParameter", //���ɱ����·� 
		0,  "FEEACOUNT",       "",             "FEEACOUNT",         "PARAMETERS", "PayPlatRequestParameter", //������ 
		0,  "BATCHNO",       "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //���κ� 
		0,  "POSID",       "",             "POSID",         "PARAMETERS", "PayPlatRequestParameter", //�����豸���� 
		0,  "CARDID",       "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //CPU���� 
		0,  "CARDSELLDATE",       "",             "CARDSELLDATE",         "PARAMETERS", "PayPlatRequestParameter", //�ۿ����� 
		0,  "TRANSCMESSAGE",       "",             "TRANSCMESSAGE",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "BALANCEBEFORE",       "",             "BALANCEBEFORE",         "PARAMETERS", "PayPlatRequestParameter", //CPU�����
		0,  "TXNDATE",       "",             "TXNDATE",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "TXNTIME",       "",             "TXNTIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "DATEOFSETTLEMENT",       "",             "DATEOFSETTLEMENT",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "DEPOSIT",       "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //Ѻ��
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};


//��ݸ������ѯ�ӿڡ����ڿ�����ʱ���ֳ�ʱ���
const field_cfg acReq_dgt_keepQry[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //������
		0,  "KEEPCODE",   "",             "KEEPCODE",         "PARAMETERS", "PayPlatRequestParameter", //keepcode
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};

//���qqҵ������

const field_cfg acReq_QQ_guanglu_pay[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		
		0,  "ORDERSEQ",   "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //������
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //TMNNUMNO
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //STAFFCODE
		0,  "PAYPASSWORD",   "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //PAYPASSWORD
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //PASSFLAG
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //ECARDNO
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAMCARDNO
		
		0,  "PRODUCTCODE",   "",             "PRODUCTCODE",         "PARAMETERS", "PayPlatRequestParameter", //PRODUCTCODE
		0,  "REQIP",   "",             "REQIP",         "PARAMETERS", "PayPlatRequestParameter", //REQIP
		0,  "ACCTCODE",   "",             "ACCTCODE",         "PARAMETERS", "PayPlatRequestParameter", //ACCTCODE
		0,  "RECHNUM",   "",             "RECHNUM",         "PARAMETERS", "PayPlatRequestParameter", //ACCTCODE
		0,  "RECHAMOUNT",   "",             "RECHAMOUNT",         "PARAMETERS", "PayPlatRequestParameter", //RECHAMOUNT
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //TRADETIME
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
};

//���qqҵ�񷵻�
const field_cfg acRsp_QQ_guanglu_pay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TRADETIME",       "",					"TRADETIME",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TXNAMOUNT",         "",                 "TXNAMOUNT",         "RESULTDATESET",    "PayPlatResponseParameter", //���׽�� 
		0,  "SYSTEMNO",         "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //���׽�� 
		0,  "PAYAMOUNT",         "",                 "PAYAMOUNT",         "RESULTDATESET",    "PayPlatResponseParameter", //ʵ�ʽ��׽��
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};
 

//��ݸ������ѯ���ر���

const field_cfg acRsp_dgt_keepQry[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "SERIALNUM",       "",					"SERIALNUM",         "RESULTDATESET",    "PayPlatResponseParameter", //SERIALNUM
		0,  "SYSTEMNO",           "",                "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "POSID",         "",                 "POSID",         "RESULTDATESET",    "PayPlatResponseParameter", //���׽�� 
		0,  "SAMID",         "",                 "SAMID",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "STAFFCODE",         "",                 "STAFFCODE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "EDCARDID",         "",                 "EDCARDID",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDOPRTYPE",         "",                 "CARDOPRTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CITYCODE",         "",                 "CITYCODE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDID",         "",                 "CARDID",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDMKND",         "",                 "CARDMKND",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDSKND",         "",                 "CARDSKND",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "CARDMODEL",         "",                 "CARDMODEL",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "TRANSTYPE",         "",                 "TRANSTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "AUTHSEQ",         "",                 "AUTHSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "DICMAC",         "",                 "DICMAC",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "MAC2",         "",                 "MAC2",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "LIMITEAUTHSEQL",         "",                 "LIMITEAUTHSEQL",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "BATCHNO",         "",                 "BATCHNO",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "SETTDATE",         "",                 "SETTDATE",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "ORDERSTAT",         "",                 "ORDERSTAT",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "PAYSTAT",         "",                 "PAYSTAT",         "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "BUSSTAT",         "",                 "BUSSTAT",         "RESULTDATESET",    "PayPlatResponseParameter",
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};

//��ݸ�����첽����ȷ��Ӧ����

const field_cfg acRsp_dgt_orderVerify[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TXNAMOUNT",         "",                 "TXNAMOUNT",         "RESULTDATESET",    "PayPlatResponseParameter", //���׽�� 
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//��ݸ�����첽��������Ӧ����
const field_cfg acRsp_dgt_orderProduce[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "EDCARDID",         "",                 "EDCARDID",         "RESULTDATESET",    "PayPlatResponseParameter", //M1���� 
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};



//�˻���֤��ѯ�ӿ�Ӧ����
const field_cfg acRsp_g3PhoneCheck[] = 
{
	0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "SYSTEMNO",         "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//3G����������Ӧ����
const field_cfg acRsp_G3Pay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //�������
		0,  "PHONE",         "",                 "PHONE",         "RESULTDATESET",    "PayPlatResponseParameter", //�ֻ�����
		0,  "RECHARGEFLOW",       "",					 "RECHARGEFLOW",        "RESULTDATESET",    "PayPlatResponseParameter", //��������
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//ȫ�����ѳ�ֵ����Ӧ����
const field_cfg acRsp_phonePay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //�������
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};

//�ƶ����㶫ʡ�ڣ��ֻ����ѳ�ֵӦ����
const field_cfg acRsp_phonePayGDYD[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TRADETIME",           "",               "TRADETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //�������
		0,  "SYSTEMNO",           "",               "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //ϵͳ�ο���
		0,  "TXAMOUNT",           "",               "TXAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //������
		0,  "PAYAMOUNT",           "",               "PAYAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //ʵ�ʽ��׽��
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};

//Ӧ��(���в�ѯ(ˮ��ú))
const field_cfg acRsp_PublicTransQuery[] = 
{
    0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
    0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
    0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
    0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
    0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
    0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
    0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
    1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������            
    0,  "TERMINALNO",       "",                 "TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
    0,  "PCITYCODE",        "",                 "ACCEPTAREACODE",   "RESULTDATESET",    "PayPlatResponseParameter", //�����������
    0,  "TRANSDT",          "",                 "ACCEPTDATE",       "RESULTDATESET",    "PayPlatResponseParameter", //����ʱ��
    0,  "DATAS",            "",                 "DATAS",            "RESULTDATESET",    "PayPlatResponseParameter", //
    1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
    1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
    0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//ˮ��ú�˵���ѯ Ӧ����
const field_cfg acRsp_PublicBillQuery[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "SYSTEMNO",      "",                 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //ϵͳ�ο���
		0,  "BILLNUM",          "",                 "BILLNUM",          "RESULTDATESET",    "PayPlatResponseParameter", //�˵���¼����		
		0,  "DATAS",            "",                 "DATAS",            "RESULTDATESET",    "PayPlatResponseParameter", //	
		0,  "TERMINALNO",       "",                 "TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "BILLSTAT",       "",					 "BILLSTAT",         "RESULTDATESET",    "PayPlatResponseParameter", //�˵�״̬
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//ˮ��ú�ɷ�Ӧ����
const field_cfg acRsp_PublicPay[] = 
{
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //��ˮ�� 
		0,  "TRANSSEQ",         "",                 "TRANSSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "SYSTEMNO",      "",					 "SYSTEMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "TXNAMOUNT",           "",               "TXNAMOUNT",        "RESULTDATESET",    "PayPlatResponseParameter", //������
		0,  "CASHTYPE",         "",                 "CASHTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", //���˵�������,0-����  1-�ɷѵ���
		0,  "CASHORDER",       "",					 "CASHORDER",        "RESULTDATESET",    "PayPlatResponseParameter", //���˵���
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};



//��ݸ����ǩ����Ϣ������ ��ȡKEYSET 

const field_cfg acReq_DGTSignIn[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //���𷽽�����ˮ��
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //����Ա
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�

};


//��ݸ����ǩ��Ӧ����
const field_cfg acRsp_DGTSignIn[] = 
{

		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն���ˮ��
		0,  "POSID",         "",					 "POSID",			"RESULTDATESET",    "PayPlatResponseParameter", //�����豸����
		1,  "KEYSET",      "",						"KEYSET",         "RESULTDATESET",    "PayPlatResponseParameter", //��Կ��Ϣ
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM����
		0,  "SAMAUTHINFO",         "",                 "SAMAUTHINFO",         "RESULTDATESET",    "PayPlatResponseParameter", //SAM����Ȩ��Ϣ
		0,  "EDAUTHINFO",       "",					 "EDAUTHINFO",        "RESULTDATESET",    "PayPlatResponseParameter", //ED����Ȩ��Ϣ
		0,  "SETTDATE",       "",					 "SETTDATE",        "RESULTDATESET",    "PayPlatResponseParameter", //��������
		0,  "BATCHNO",       "",					 "BATCHNO",        "RESULTDATESET",    "PayPlatResponseParameter", //���κ�
		0,  "SYSDATETIME",       "",					 "SYSDATETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //���κ�
		0,  "AUTHCODE",       "",					 "AUTHCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //��Ȩ��֤��
		0,  "PARAMBIT",       "",					 "PARAMBIT",        "RESULTDATESET",    "PayPlatResponseParameter", //�������±�־λ
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//��ݸ����ǩ�������� 

const field_cfg acReq_DGTSignOut[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //���𷽽�����ˮ��
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //����Ա
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "SETTDATE",   "",             "SETTDATE",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //���κ�
		0,  "TOTALSVNUM",   "",             "TOTALSVNUM",         "PARAMETERS", "PayPlatRequestParameter", //�������ۼ��ۿ���ֵ�����ܱ���
		0,  "TOTALSVAMT",   "",             "TOTALSVAMT",         "PARAMETERS", "PayPlatRequestParameter", //�����ۼƳ�ֵ�����ܽ��
		0,  "TOTALSALEDEP",   "",             "TOTALSALEDEP",         "PARAMETERS", "PayPlatRequestParameter", //�����ۼ��ۿ�����Ѻ���ܽ��
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
		
};


//��ݸ����ǩ��Ӧ����
const field_cfg acRsp_DGTSignOut[] = 
{
	
	0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն���ˮ��
		0,  "POSID",         "",					 "POSID",			"RESULTDATESET",    "PayPlatResponseParameter", //�����豸����
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM����
		0,  "EDCARDID",           "",               "EDCARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //������������
		0,  "SETTDATE",       "",					 "SETTDATE",        "RESULTDATESET",    "PayPlatResponseParameter", //��������
		0,  "BATCHNO",       "",					 "BATCHNO",        "RESULTDATESET",    "PayPlatResponseParameter", //���κ�
		0,  "TOTALSVNUM",       "",					 "TOTALSVNUM",        "RESULTDATESET",    "PayPlatResponseParameter", //�������ۼ��ۿ����ʽ����ܱ���
		0,  "TOTALSVAMT",       "",					 "TOTALSVAMT",        "RESULTDATESET",    "PayPlatResponseParameter", //�����ۼƳ�ֵ�����ܽ��֣�
		0,  "TOTALSALEDEP",       "",					 "TOTALSALEDEP",        "RESULTDATESET",    "PayPlatResponseParameter", //�����ۼ��ۿ�����Ѻ���ܽ��֣�
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//��ݸ����������������
const field_cfg acReq_DGTCardOperate[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //���𷽽�����ˮ��
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "ECARDNO",   "",             "ECARDNO",         "PARAMETERS", "PayPlatRequestParameter", //E����
		0,  "PSAMCARDNO",   "",             "PSAMCARDNO",         "PARAMETERS", "PayPlatRequestParameter", //PSAMCARDNO	PSAM����
		0,  "PASSFLAG",   "",             "PASSFLAG",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //�û���(����)
		0,  "PAYPASSWORD",   "",             "PAYPASSWORD",         "PARAMETERS", "PayPlatRequestParameter", //����
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "CARDOPRTYPE",   "",             "CARDOPRTYPE",         "PARAMETERS", "PayPlatRequestParameter", //����������
		0,  "CITYCODE",   "",             "CITYCODE",         "PARAMETERS", "PayPlatRequestParameter", //���д���
		0,  "CARDID",   "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //���ں�
		0,  "CARDMKND",   "",             "CARDMKND",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDSKND",   "",             "CARDSKND",         "PARAMETERS", "PayPlatRequestParameter", //�ӿ�����
		0,  "CARDMODEL",   "",             "CARDMODEL",         "PARAMETERS", "PayPlatRequestParameter", //����
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //ҵ������
		0,  "DEPOSIT",   "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //Ѻ��
		0,  "ORIGAMT",   "",             "ORIGAMT",         "PARAMETERS", "PayPlatRequestParameter", //Ӧ�ս��
		0,  "CARDVALDATE",   "",             "CARDVALDATE",         "PARAMETERS", "PayPlatRequestParameter", //����Ч��
		0,  "SRCBAL",   "",             "SRCBAL",         "PARAMETERS", "PayPlatRequestParameter", //����Ч��
		0,  "CARDSEQ",   "",             "CARDSEQ",         "PARAMETERS", "PayPlatRequestParameter", //��Ƭ������ˮ
		0,  "KEYVER",   "",             "KEYVER",         "PARAMETERS", "PayPlatRequestParameter", //��Ƭ��Կ�汾
		0,  "ALGIND",   "",             "ALGIND",         "PARAMETERS", "PayPlatRequestParameter", //��Ƭ�㷨��־
		0,  "CARDRAND",   "",             "CARDRAND",         "PARAMETERS", "PayPlatRequestParameter", //��Ƭ�����
		0,  "MAC1",   "",             "MAC1",         "PARAMETERS", "PayPlatRequestParameter", //���ʷ�����ϢMAC1
		0,  "DIVDATA",   "",             "DIVDATA",         "PARAMETERS", "PayPlatRequestParameter", //��ɢ����
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //���κ�
		0,  "KEYSET",   "",             "KEYSET",         "PARAMETERS", "PayPlatRequestParameter", //��Կ
		0,  "COMMAND",   "",             "COMMAND",         "PARAMETERS", "PayPlatRequestParameter", //APDUָ��
		0,  "COMMANDLEN",   "",             "COMMANDLEN",         "PARAMETERS", "PayPlatRequestParameter", //APDUָ��
		0,  "POSID",   "",             "POSID",         "PARAMETERS", "PayPlatRequestParameter", //POSID
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
		
};


//��ݸ����������Ӧ����

const field_cfg acRsp_DGTCardOperate[] = 
{
	
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն���ˮ��
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM����
		0,  "STAFFCODE",           "",               "STAFFCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //����Ա
		0,  "EDCARDID",       "",					 "EDCARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //������������
		0,  "CITYCODE",       "",					 "CITYCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //���д���
		0,  "CARDID",       "",					 "CARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //���ں�
		0,  "CARDMKND",       "",					 "CARDMKND",        "RESULTDATESET",    "PayPlatResponseParameter", //��������
		0,  "CARDSKND",       "",					 "CARDSKND",        "RESULTDATESET",    "PayPlatResponseParameter", //�ӿ�����
		0,  "CARDMODEL",       "",					 "CARDMODEL",        "RESULTDATESET",    "PayPlatResponseParameter", //����
		0,  "SYSTEMNO",       "",					 "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //������ˮ��
		0,  "AUTHSEQ",       "",					 "AUTHSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //��Կ��Ȩ��ˮ��
		0,  "TRANSSEQ",       "",					 "TRANSSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //������
		0,  "LIMITEAUTHSEQL",       "",					 "LIMITEAUTHSEQL",        "RESULTDATESET",    "PayPlatResponseParameter", //�����Ȩ��ˮ��
		0,  "DICMAC",       "",					 "DICMAC",        "RESULTDATESET",    "PayPlatResponseParameter", //DICMAC
		0,  "MAC2",       "",					 "MAC2",        "RESULTDATESET",    "PayPlatResponseParameter", //DICMAC
		0,  "SYSDATETIME",       "",					 "SYSDATETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //����ʱ��
		0,  "MESSAGEDATETIME",       "",					 "MESSAGEDATETIME",        "RESULTDATESET",    "PayPlatResponseParameter", //����ʱ��
		0,  "BATCHNO",       "",					 "BATCHNO",        "RESULTDATESET",    "PayPlatResponseParameter", //����ʱ��
		0,  "SETTDATE",       "",					 "SETTDATE",        "RESULTDATESET",    "PayPlatResponseParameter", //���κ�
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};



//��ݸ�������ļ���������������

const field_cfg acReq_DGTCardSale[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //���𷽽�����ˮ��
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "OPERATIONTYPE",   "",             "OPERATIONTYPE",         "PARAMETERS", "PayPlatRequestParameter",  //��������	������ATO ����������ATR	����Ĭ��Ϊ����ATO
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //����Ա
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //ҵ������
		0,  "TAC",   "",					"TAC",				 "PARAMETERS", "PayPlatRequestParameter", //����Ѻ��
		0,  "SYSTEMNO",   "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //������ˮ��
		0,  "APTRANSSEQ",   "",             "APTRANSSEQ",         "PARAMETERS", "PayPlatRequestParameter", //ԭ������ˮ��
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //ҵ������
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "EDCARDID",   "",             "EDCARDID",         "PARAMETERS", "PayPlatRequestParameter", //������������
		0,  "CARDID",   "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //���ں�
		0,  "TXNDATE",   "",             "TXNDATE",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "TXNTIME",   "",             "TXNTIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "CARDCNT",   "",             "CARDCNT",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDMKND",   "",             "CARDMKND",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDSKND",   "",             "CARDSKND",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDMODEL",   "",             "CARDMODEL",         "PARAMETERS", "PayPlatRequestParameter", //����
		0,  "SALEMODE",   "",             "SALEMODE",         "PARAMETERS", "PayPlatRequestParameter", //�ۿ���ʽ
		0,  "DEPOSIT",   "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //Ѻ��
		0,  "BEFBALANCE",   "",             "BEFBALANCE",         "PARAMETERS", "PayPlatRequestParameter", //����ǰ���
		0,  "TXNAMT",   "",             "TXNAMT",         "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "ORIGAMT",   "",             "ORIGAMT",         "PARAMETERS", "PayPlatRequestParameter", //���׽��
		0,  "CARDVALDATE",   "",             "CARDVALDATE",         "PARAMETERS", "PayPlatRequestParameter", //����Ч��
		0,  "CITYCODE",   "",             "CITYCODE",         "PARAMETERS", "PayPlatRequestParameter", //���ڳ��д���
		0,  "CARDVERNO",   "",             "CARDVERNO ",         "PARAMETERS", "PayPlatRequestParameter", //���ڰ汾��
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //���κ�
		0,  "AUTHSEQ",   "",             "AUTHSEQ",         "PARAMETERS", "PayPlatRequestParameter", //��Ȩ��ˮ��
		0,  "LIMITEDAUTHSEQL",   "",             "LIMITEDAUTHSEQL",         "PARAMETERS", "PayPlatRequestParameter", //�����Ȩ��ˮ��
		0,  "KEYSET",   "",             "KEYSET",         "PARAMETERS", "PayPlatRequestParameter", //��Կ
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
		
};

//��ݸ�������������� Ӧ��
const field_cfg acRsp_DGTCardSale[] = 
{
	
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "STAFFCODE",       "",					"STAFFCODE",         "RESULTDATESET",    "PayPlatResponseParameter", //����Ա
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն���ˮ��
		0,  "POSID",         "",					 "POSID",			"RESULTDATESET",    "PayPlatResponseParameter", //�����豸����
		0,  "TRANSTYPE",         "",					 "TRANSTYPE",			"RESULTDATESET",    "PayPlatResponseParameter", //ҵ������
		1,  "KEYSET",      "",						"KEYSET",         "RESULTDATESET",    "PayPlatResponseParameter", //��Կ��Ϣ
		0,  "SAMID",           "",               "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", //SAM����
		0,  "EDCARDID",         "",                 "EDCARDID",         "RESULTDATESET",    "PayPlatResponseParameter", //������������
		0,  "CITYCODE",       "",					 "CITYCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //���д���
		0,  "CARDID",       "",					 "CARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //���ں�
		0,  "CARDMKND",       "",					 "CARDMKND",        "RESULTDATESET",    "PayPlatResponseParameter", //��������
		0,  "CARDSKND",       "",					 "CARDSKND",        "RESULTDATESET",    "PayPlatResponseParameter", //�ӿ�����
		0,  "SYSTEMNO",       "",					 "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //�ӿ�����
		0,  "APTRANSSEQ",       "",					 "APTRANSSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //�ӿ�����
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//��ݸ������ֵ����������
const field_cfg acReq_DGTCardCharge[] = 
{
		0,  "MERID",      "", "MERID",        "VerifyParameter",    "", //������
		0,  "CHANNELCODE",  "", "CHANNELCODE",  "VerifyParameter",  "", //�����ţ���ʶ�������ͣ���ֵ60
		0,  "TMNNUM",       "", "TMNNUM",       "VerifyParameter", "", //�����ն˺ţ��ͻ�����һһ��Ӧ
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		1,  "WEBSVRNAME",   "WEBSVRNAME",   "CTRL-INFO",        "PayPlatRequestParameter", "", //��������
		0,  "WEBSVRCODE",   "WEBSVRCODE",   "CTRL-INFO",        "PayPlatRequestParameter", "", //�������
		0,  "APPFROM",      "APPFROM",      "CTRL-INFO",        "PayPlatRequestParameter", "", //��ʽ��ʡ��-Ӧ��ϵͳ����-�汾��-IP
		0,  "KEEP",         "KEEP",         "CTRL-INFO",        "PayPlatRequestParameter", "", //�ն˺�+yyyyMMddhhmmss+4λ��ˮ��
		0,  "REQUESTTIME",      "REQUESTTIME",  "CTRL-INFO",        "PayPlatRequestParameter", "", //yyyyMMddhhmmss
		
		0,  "ORDERSEQ",       "",             "ORDERSEQ",         "PARAMETERS", "PayPlatRequestParameter", //���𷽽�����ˮ��
		0,  "TMNNUMNO",   "",             "TMNNUMNO",         "PARAMETERS", "PayPlatRequestParameter", //�ն˺�
		0,  "STAFFCODE",   "",             "STAFFCODE",         "PARAMETERS", "PayPlatRequestParameter", //����Ա
		0,  "OPERATIONTYPE",   "",             "OPERATIONTYPE",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "SYSTEMNO",   "",             "SYSTEMNO",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "APTRANSSEQ",   "",             "APTRANSSEQ",         "PARAMETERS", "PayPlatRequestParameter", //ԭ������ˮ��
		0,  "TRANSTYPE",   "",             "TRANSTYPE",         "PARAMETERS", "PayPlatRequestParameter", //ԭ������ˮ��
		0,  "TAC",   "",					 "TAC",				"PARAMETERS", "PayPlatRequestParameter", //����Ѻ��
		0,  "TRADETIME",   "",             "TRADETIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "EDCARDID",   "",             "EDCARDID",         "PARAMETERS", "PayPlatRequestParameter", //������������
		0,  "CARDID",   "",             "CARDID",         "PARAMETERS", "PayPlatRequestParameter", //���ں�
		0,  "TXNDATE",   "",             "TXNDATE",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "TXNTIME",   "",             "TXNTIME",         "PARAMETERS", "PayPlatRequestParameter", //����ʱ��
		0,  "CARDCNT",   "",             "CARDCNT",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDMKND",   "",             "CARDMKND",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDSKND",   "",             "CARDSKND",         "PARAMETERS", "PayPlatRequestParameter", //��������
		0,  "CARDMODEL",   "",             "CARDMODEL",         "PARAMETERS", "PayPlatRequestParameter", //���� 1  CPU��3  M1�� 
		0,  "BEFBALANCE",   "",             "BEFBALANCE",         "PARAMETERS", "PayPlatRequestParameter", //����ǰ���
		0,  "ORIGAMT",   "",             "ORIGAMT",         "PARAMETERS", "PayPlatRequestParameter", //Ӧ�ս��
		0,  "TXNAMT",   "",             "TXNAMT",         "PARAMETERS", "PayPlatRequestParameter", //ʵ�ս��
		0,  "HANDINGCHARGE",   "",             "HANDINGCHARGE",         "PARAMETERS", "PayPlatRequestParameter", //������ 
		0,  "DEPOSIT",   "",             "DEPOSIT",         "PARAMETERS", "PayPlatRequestParameter", //Ѻ��
		0,  "CARDVALDATE",   "",             "CARDVALDATE",         "PARAMETERS", "PayPlatRequestParameter", //����Ч��
		0,  "CITYCODE",   "",             "CITYCODE",         "PARAMETERS", "PayPlatRequestParameter", //���ڳ��д���
		0,  "CARDVERNO",   "",             "CARDVERNO",         "PARAMETERS", "PayPlatRequestParameter", //���ڰ汾��
		0,  "BATCHNO",   "",             "BATCHNO",         "PARAMETERS", "PayPlatRequestParameter", //���κ�
		0,  "AUTHSEQ",   "",             "AUTHSEQ",         "PARAMETERS", "PayPlatRequestParameter", //��Ȩ��ˮ��
		0,  "LIMITEDAUTHSEQL",   "",             "LIMITEDAUTHSEQL",         "PARAMETERS", "PayPlatRequestParameter", //�����Ȩ��ˮ��
		0,  "LASTPOSSVSEQ",   "",             "LASTPOSSVSEQ",         "PARAMETERS", "PayPlatRequestParameter", //POS�ն��ϱʳ�ֵ��ˮ�� ����
		0,  "KEYSET",   "",             "KEYSET",         "PARAMETERS", "PayPlatRequestParameter", //��Կ
		0,  "",             "",             "",                 "", "", //���������־������ͱ����޹أ�
		
};


//��ݸ������ֵ������ Ӧ����
const field_cfg acRsp_DGTCardCharge[] = 
{
	
		0,  "SIGN",         "", "SIGN",         "VerifyParameter",  "", //ǩ��
		0,  "CER",          "", "CER",          "VerifyParameter",  "", //��Կ
		0,  "REQWEBSVRCODE",    "REQWEBSVRCODE",    "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����WEBSVRCODE
		0,  "RESPONSETYPE",     "RESPONSETYPE",     "RESPONSE-INFO",    "PayPlatResponseParameter", "", //��ѯ����ӦΪ20�����඼Ϊ10
		0,  "KEEP",             "KEEP",             "RESPONSE-INFO",    "PayPlatResponseParameter", "", //ȡ�Զ�Ӧ����KEEP
		0,  "RESULT",           "RESULT",           "RESPONSE-INFO",    "PayPlatResponseParameter", "", //����ɹ���SUCESS�������ʧ�ܡ�FAIL��    
		0,  "RSPCODE",          "",                 "RESPONSECODE",     "PayPlatResponseParameter", "", //��Ӧ�� 
		1,  "RSPMSG",           "",                 "RESPONSECONTENT",  "PayPlatResponseParameter", "", //��Ӧ������        
		
		0,  "TMNNUMNO",       "",					"TMNNUMNO",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն˺�
		0,  "ORDERSEQ",           "",                "ORDERSEQ",         "RESULTDATESET",    "PayPlatResponseParameter", //�ն���ˮ��
		0,  "STAFFCODE",           "",                "STAFFCODE",         "RESULTDATESET",    "PayPlatResponseParameter", //����Ա
		0,  "TRANSTYPE",           "",                "TRANSTYPE",         "RESULTDATESET",    "PayPlatResponseParameter", //ҵ������
		0,  "POSID",           "",                "POSID",         "RESULTDATESET",    "PayPlatResponseParameter", //�����豸����
		0,  "CARDID",           "",               "CARDID",        "RESULTDATESET",    "PayPlatResponseParameter", 
		0,  "EDCARDID",       "",					 "EDCARDID",        "RESULTDATESET",    "PayPlatResponseParameter", //������������
		0,  "SAMID",       "",					 "SAMID",        "RESULTDATESET",    "PayPlatResponseParameter", ////SAM����
		0,  "CITYCODE",       "",					 "CITYCODE",        "RESULTDATESET",    "PayPlatResponseParameter", //���д���
		0,  "CARDMKND",       "",					 "CARDMKND",        "RESULTDATESET",    "PayPlatResponseParameter", //��������
		0,  "CARDSKND",       "",					 "CARDSKND",        "RESULTDATESET",    "PayPlatResponseParameter", //CARDSKND	�ӿ�����
		0,  "SYSTEMNO",       "",					 "SYSTEMNO",        "RESULTDATESET",    "PayPlatResponseParameter", //CARDSKND	�ӿ�����
		0,  "APTRANSSEQ",       "",					 "APTRANSSEQ",        "RESULTDATESET",    "PayPlatResponseParameter", //CARDSKND	�ӿ�����		
		1,  "REMARK1",          "",                 "REMARK1",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����1
		1,  "REMARK2",          "",                 "REMARK2",          "RESULTDATESET",    "PayPlatResponseParameter", //Ԥ����2
		0,  "",                 "",                 "",                 "",                 "", //���������־������ͱ����޹أ�
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxmlParser::CxmlParser()
{

}

CxmlParser::~CxmlParser()
{
	

}


int CxmlParser:: packAreaQryxml(CGlobal gbl,char *OUTTMNNUMNO,char * areaCode,char *ACCEPTDATE, string &strReq0,string &strReq1,char * WEBSVRNAME,char *WEBSVRCODE)
{	

	//strReq0="INF06002|440100-APP001-001-132.97.117.170|APP001|132.97.117.170|"; 
	string data_sign=""; //����ǩ����Ϣ
	string data_sign_base64=""; //64�����ַ���
	string CER_Str=""; //��Կ��Ϣ
	string platString=""; //ƽ̨����
	char keep_str[32+1];
	int iRet=0;
	bbzero(keep_str);
	//CTools::get_keep(keep_str);
	CTools::get_keep(gbl.pos_no,keep_str);
	char strReqTmp[100];
	char APPFROMTmp[50];
	bbzero(APPFROMTmp);
	bbzero(strReqTmp);
	sprintf(strReqTmp,"INF06002|440100-APP001-001-%s|APP001|%s|",gbl.server_ip,gbl.server_ip);
	sprintf(APPFROMTmp,"440100-APP001-001-%s",gbl.server_ip);
	strReq0.assign(strReqTmp,strlen(strReqTmp));
	
	//PayPlatRequestParameter��Ҫ���̻�˽Կ���м���
	TiXmlDocument *PayPlatDoc=new TiXmlDocument;
	TiXmlElement *PayPlatRequestParameter=new TiXmlElement("PayPlatRequestParameter");
	PayPlatDoc->LinkEndChild(PayPlatRequestParameter);
	TiXmlElement *CTRL_INFO =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO->SetAttribute("WEBSVRNAME",WEBSVRNAME); //��������
	CTRL_INFO->SetAttribute("WEBSVRCODE",WEBSVRCODE); //����������
	CTRL_INFO->SetAttribute("APPFROM",APPFROMTmp); //���󱾷����Ӧ�ñ�ʶ��

	CTRL_INFO->SetAttribute("KEEP",keep_str); //���η���ı�ʶ��ˮ���ο�����:������+�ն˺�+YYMMDDHHMMSS+�����
	CTRL_INFO->SetAttribute("REQUESTTIME",ACCEPTDATE); //����ʱ��
	
	PayPlatRequestParameter->LinkEndChild(CTRL_INFO);
	
	TiXmlElement *PARAMETERS =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter->LinkEndChild(PARAMETERS);
	
	//ˮ��ú������ѯ�ӿ�ƽ̨����PARAMETERS
	TiXmlElement *OUTTMNNUMNOEle=new TiXmlElement("TMNNUMNO");
	TiXmlElement * ACCEPTAREACODEEle=new TiXmlElement("ACCEPTAREACODE");
	TiXmlElement *ACCEPTDATEEle=new TiXmlElement("ACCEPTDATE");
	PARAMETERS->LinkEndChild(OUTTMNNUMNOEle);
	PARAMETERS->LinkEndChild(ACCEPTAREACODEEle);
	PARAMETERS->LinkEndChild(ACCEPTDATEEle);

	TiXmlText *TMNNUMNOTxt=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle->LinkEndChild(TMNNUMNOTxt);

	TiXmlText *ACCEPTAREACODETXT=new TiXmlText(areaCode);
	ACCEPTAREACODEEle->LinkEndChild(ACCEPTAREACODETXT);
	TiXmlText *ACCEPTDATETXT=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle->LinkEndChild(ACCEPTDATETXT);
	TiXmlPrinter platPrinter;
	platPrinter.SetLineBreak(NULL);
	platPrinter.SetIndent(0);
	PayPlatDoc->Accept(&platPrinter);
	platString=platPrinter.CStr();
    wrLog("platString:%s",platString.c_str());
	//ƽ̨����תutf-8
	ANSItoUTF8(platString);
	//����ǩ��
	Ccert::data_sign("privateKey.pem",platString,data_sign);
	data_sign_base64=base64_encode(data_sign);
	wrLog("data_sign_64:%s",data_sign_base64.c_str());
	//��ȡ��Կ��Ϣ
	Ccert::cert_key("cert.pem",CER_Str);
	wrLog("��Կ��Ϣ:%s",CER_Str.c_str());
	Ccert::cert_verify_file("cert.pem",platString.c_str(),data_sign);

	TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlDeclaration  *pDelaration=new TiXmlDeclaration("1.0","UTF-8",""); //UTF-8
	pDoc->LinkEndChild(pDelaration); //��Ӹ��ڵ�
	TiXmlElement *Request=new TiXmlElement("Request"); 
	pDoc->LinkEndChild(Request); //��Ӹ��ڵ�
	TiXmlElement *VerifyParameter=new TiXmlElement("VerifyParameter");
	Request->LinkEndChild(VerifyParameter);
	
	TiXmlElement *MERIEle=new TiXmlElement("MERID");
	TiXmlText *MERIDContent=new TiXmlText(MERID); //��������
	MERIEle->LinkEndChild(MERIDContent);

	VerifyParameter->LinkEndChild(MERIEle);
	TiXmlElement * SIGN=new TiXmlElement("SIGN");
	VerifyParameter->LinkEndChild(SIGN);
	TiXmlText * SIGN_Content=new TiXmlText(data_sign_base64.c_str());
	wrLog("data_sign_base64:%s",data_sign_base64.c_str());
	SIGN->LinkEndChild(SIGN_Content);

	TiXmlElement *TMNNUMNOEle=new TiXmlElement("TMNNUM");
	VerifyParameter->LinkEndChild(TMNNUMNOEle);
	TiXmlText *TMNNUM_CONTENT=new TiXmlText(TMNNUM); //������Ӧ�Ľ����ն˺�
	TMNNUMNOEle->LinkEndChild(TMNNUM_CONTENT);

	TiXmlElement * CHANNELCODE =new TiXmlElement("CHANNELCODE"); //��������
	VerifyParameter->LinkEndChild(CHANNELCODE);
	TiXmlText *CHANNEL_CODE_CONTENT=new TiXmlText("60");
	CHANNELCODE->LinkEndChild(CHANNEL_CODE_CONTENT);

	TiXmlElement *CER=new TiXmlElement("CER");	//��Կ��Ϣ
	VerifyParameter->LinkEndChild(CER);
	TiXmlText *CER_CONTENT=new TiXmlText(CER_Str.c_str());
	CER->LinkEndChild(CER_CONTENT);
	
	//PayPlatRequestParameter��Ҫ���̻�˽Կ���м���
	
	TiXmlElement *PayPlatRequestParameter1=new TiXmlElement("PayPlatRequestParameter");
	Request->LinkEndChild(PayPlatRequestParameter1);
	TiXmlElement *CTRL_INFO1 =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO1->SetAttribute("WEBSVRNAME",WEBSVRNAME); //��������
	CTRL_INFO1->SetAttribute("WEBSVRCODE",WEBSVRCODE); //����������
	CTRL_INFO1->SetAttribute("APPFROM",APPFROMTmp); //���󱾷����Ӧ�ñ�ʶ��
	CTRL_INFO1->SetAttribute("KEEP",keep_str); //���η���ı�ʶ��ˮ���ο�����:�ն˺�+YYMMDDHHMMSS+�����
	CTRL_INFO1->SetAttribute("REQUESTTIME",ACCEPTDATE); //����ʱ��
	
	PayPlatRequestParameter1->LinkEndChild(CTRL_INFO1);
	
	TiXmlElement *PARAMETERS1 =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter1->LinkEndChild(PARAMETERS1);
	
	//ˮ��ú������ѯ�ӿ�ƽ̨����PARAMETERS
	TiXmlElement *OUTTMNNUMNOEle1=new TiXmlElement("TMNNUMNO");
	TiXmlElement * ACCEPTAREACODEEle1=new TiXmlElement("ACCEPTAREACODE");
	TiXmlElement *ACCEPTDATEEle1=new TiXmlElement("ACCEPTDATE");
	PARAMETERS1->LinkEndChild(OUTTMNNUMNOEle1);
	PARAMETERS1->LinkEndChild(ACCEPTAREACODEEle1);
	PARAMETERS1->LinkEndChild(ACCEPTDATEEle1);
	TiXmlText *TMNNUMNOTxt1=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle1->LinkEndChild(TMNNUMNOTxt1);
	TiXmlText *ACCEPTAREACODETXT1=new TiXmlText(areaCode);
	ACCEPTAREACODEEle1->LinkEndChild(ACCEPTAREACODETXT1);
	TiXmlText *ACCEPTDATETXT1=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle1->LinkEndChild(ACCEPTDATETXT1);
	
	
	//ˮ��ú������ѯ�ӿ�ƽ̨����PARAMETERS����

	//������ַ���
	TiXmlPrinter printer;
	printer.SetLineBreak(NULL);
	printer.SetIndent(0);
	pDoc->Accept(&printer);
	strReq1=printer.CStr();
	wrLog("strReq1:%s",strReq1.c_str());
	wrLog("strReq0:%s",strReq0.c_str());	
	ANSItoUTF8(strReq0);
	ANSItoUTF8(strReq1);
	return 0;
} 


/************************************************************************/
/* //ˮ��ú�˵���ѯ�ӿ�
ACCEPTAREACODE:����������б���
ADDITEM1���ɷ�ʡ���루6λ��+�ɷѳ��д��루6λ��+�ɷ���Ŀ��ţ�4λ��
ADDITEM2���Ϻ�ˮ��ú��Ҫ���������
SELECTVALUE:�û��������������
ADDITEM2:��ѯ���Ϻ�ˮ��ú�����ѱ�����
                                                                
/************************************************************************/
int CxmlParser:: packSDMQryxml(CGlobal gbl,char * WEBSVRNAME,char *WEBSVRCODE,char *OUTTMNNUMNO,char *SELECTTYPE,char *SELECTVALUE,char *ACCEPTDATE,char *ACCEPTAREACODE,char *ADDITEM1,char *ADDITEM2,string &strReq0,string &strReq1 ) //�Ϻ�ˮ�Ѻ�ú��ʹ��
{
	string data_sign=""; //ǩ��
	string CER_Str=""; 
	char keep_str[32+1];
	string platString="";
	char orderNo[32+1];
	bbzero(orderNo);
	string data_sign_base64="";
	bbzero(keep_str);
	bbzero(orderNo);
	bbzero(keep_str);
	//CTools::get_keep(keep_str);
	CTools::get_keep(gbl.pos_no,keep_str);

	CTools::get_orderNo(orderNo);
	char strReqTmp[100];
	char APPFROMTmp[50];
	bbzero(APPFROMTmp);
	bbzero(strReqTmp);
	sprintf(strReqTmp,"%s|440100-APP001-001-%s|APP001|%s|",WEBSVRCODE,gbl.server_ip,gbl.server_ip);
	sprintf(APPFROMTmp,"440100-APP001-001-%s",gbl.server_ip);
	strReq0.assign(strReqTmp,strlen(strReqTmp));

	//ADDITEM1 �ɷ�ʡ���루6λ��+�ɷѳ��д��루6λ��+�ɷ���Ŀ��ţ�4λ��

	//��ƽ̨���ݽ�������ǩ��;
	 TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlElement *PayPlatRequestParameter=new TiXmlElement("PayPlatRequestParameter");
	pDoc->LinkEndChild(PayPlatRequestParameter);
	TiXmlElement *CTRL_INFO =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO->SetAttribute("WEBSVRNAME",WEBSVRNAME); //��������
	CTRL_INFO->SetAttribute("WEBSVRCODE",WEBSVRCODE); //����������
	CTRL_INFO->SetAttribute("APPFROM",APPFROMTmp); //���󱾷����Ӧ�ñ�ʶ��
	CTRL_INFO->SetAttribute("KEEP",keep_str); //���η���ı�ʶ��ˮ���ο�����:�ն˺�+YYMMDDHHMMSS+�����
	CTRL_INFO->SetAttribute("REQUESTTIME",ACCEPTDATE); //����ʱ��
	
	PayPlatRequestParameter->LinkEndChild(CTRL_INFO);
	
	TiXmlElement *PARAMETERS =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter->LinkEndChild(PARAMETERS);
	
	TiXmlElement *ORDERSEQEle=new TiXmlElement("ORDERSEQ");
	TiXmlElement * OUTTMNNUMNOEle=new TiXmlElement("TMNNUMNO");
	TiXmlElement *SELECTTYPEEle=new TiXmlElement("SELECTTYPE");
	TiXmlElement *SELECTVALUEEle=new TiXmlElement("SELECTVALUE");
	TiXmlElement *ACCEPTDATEEle=new TiXmlElement("ACCEPTDATE");
	TiXmlElement *ACCEPTAREACODEEle=new TiXmlElement("ACCEPTAREACODE");//�������
	TiXmlElement *ADDITEM1Ele=new TiXmlElement("ADDITEM1");//��ѯ�ĵ���
	
	PARAMETERS->LinkEndChild(ORDERSEQEle);
	PARAMETERS->LinkEndChild(OUTTMNNUMNOEle);
	PARAMETERS->LinkEndChild(SELECTTYPEEle);
	PARAMETERS->LinkEndChild(SELECTVALUEEle);
	PARAMETERS->LinkEndChild(ACCEPTDATEEle);
	PARAMETERS->LinkEndChild(ACCEPTAREACODEEle);
	PARAMETERS->LinkEndChild(ADDITEM1Ele);
	
	//��ֵ
	TiXmlText *ORDERSEQTxt=new TiXmlText(orderNo);
	ORDERSEQEle->LinkEndChild(ORDERSEQTxt);
	TiXmlText *OUTTMNNUMNOTxt=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle->LinkEndChild(OUTTMNNUMNOTxt);
	TiXmlText *SELECTTYPETxt=new TiXmlText(SELECTTYPE);
	SELECTTYPEEle->LinkEndChild(SELECTTYPETxt);
	TiXmlText *SELECTVALUETxt=new TiXmlText(SELECTVALUE);
	SELECTVALUEEle->LinkEndChild(SELECTVALUETxt);
	TiXmlText *ACCEPTDATETxt=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle->LinkEndChild(ACCEPTDATETxt);
	TiXmlText *ACCEPTAREACODETxt=new TiXmlText(ACCEPTAREACODE);
	ACCEPTAREACODEEle->LinkEndChild(ACCEPTAREACODETxt);
	TiXmlText *ADDITEM1Txt=new TiXmlText(ADDITEM1);
	ADDITEM1Ele->LinkEndChild(ADDITEM1Txt);
	
	TiXmlElement *ADDITEM2Ele=new TiXmlElement("ADDITEM2");//��ѯ���
	PARAMETERS->LinkEndChild(ADDITEM2Ele);
	TiXmlText *ADDITEM2Txt=new TiXmlText(ADDITEM2);
	ADDITEM2Ele->LinkEndChild(ADDITEM2Txt);

	TiXmlPrinter platPrinter;
	platPrinter.SetIndent(NULL);
	platPrinter.SetLineBreak(NULL);
	pDoc->Accept(&platPrinter);
	platString=platPrinter.CStr();
    wrLog("platString:%s",platString.c_str());
	//ƽ̨ǩ����Ϣתutf-8
	ANSItoUTF8(platString);
	//����ǩ��

	Ccert::data_sign("privateKey.pem",platString,data_sign);
	data_sign_base64=base64_encode(data_sign);
	//��ȡ��Կ��Ϣ
	Ccert::cert_key("cert.pem",CER_Str);
	//ƽ̨��������ǩ������

	TiXmlDocument *pDoc1=new TiXmlDocument;
	TiXmlDeclaration  *pDelaration=new TiXmlDeclaration("1.0","UTF-8",""); //UTF-8
	pDoc1->LinkEndChild(pDelaration); //��Ӹ��ڵ�
	TiXmlElement *Request=new TiXmlElement("Request"); 
	pDoc1->LinkEndChild(Request); //��Ӹ��ڵ�
	TiXmlElement *VerifyParameter=new TiXmlElement("VerifyParameter");
	Request->LinkEndChild(VerifyParameter);
	
	TiXmlElement *MERIDEle=new TiXmlElement("MERID");
	TiXmlText *MERIDContent=new TiXmlText(MERID); //��������
	MERIDEle->LinkEndChild(MERIDContent);
	
	VerifyParameter->LinkEndChild(MERIDEle);
	TiXmlElement * SIGNEle=new TiXmlElement("SIGN");
	VerifyParameter->LinkEndChild(SIGNEle);
	TiXmlText * SIGN_Content=new TiXmlText(data_sign_base64.c_str());
	wrLog("data_sign_base64:%s",data_sign_base64.c_str());
	SIGNEle->LinkEndChild(SIGN_Content);
	
	TiXmlElement *TMNNUMNOELe=new TiXmlElement("TMNNUM");
	VerifyParameter->LinkEndChild(TMNNUMNOELe);
	TiXmlText *TMNNUM_CONTENT=new TiXmlText(TMNNUM); //������Ӧ�Ľ����ն˺�
	TMNNUMNOELe->LinkEndChild(TMNNUM_CONTENT);
	
	TiXmlElement * CHANNELCODEEle =new TiXmlElement("CHANNELCODE"); //��������
	VerifyParameter->LinkEndChild(CHANNELCODEEle);
	TiXmlText *CHANNEL_CODE_CONTENT=new TiXmlText(CHANNELCODE);
	CHANNELCODEEle->LinkEndChild(CHANNEL_CODE_CONTENT);
	
	TiXmlElement *CER=new TiXmlElement("CER");	//��Կ��Ϣ
	VerifyParameter->LinkEndChild(CER);
	TiXmlText *CER_CONTENT=new TiXmlText(CER_Str.c_str());
	CER->LinkEndChild(CER_CONTENT);


	TiXmlElement *PayPlatRequestParameter1=new TiXmlElement("PayPlatRequestParameter");
	Request->LinkEndChild(PayPlatRequestParameter1);
	TiXmlElement *CTRL_INFO1 =new TiXmlElement("CTRL-INFO");	
	CTRL_INFO1->SetAttribute("WEBSVRNAME",WEBSVRNAME); //��������
	CTRL_INFO1->SetAttribute("WEBSVRCODE",WEBSVRCODE); //����������
	CTRL_INFO1->SetAttribute("APPFROM",APPFROMTmp); //���󱾷����Ӧ�ñ�ʶ��
	CTRL_INFO1->SetAttribute("KEEP",keep_str); //���η���ı�ʶ��ˮ���ο�����:�ն˺�+YYMMDDHHMMSS+�����
	CTRL_INFO1->SetAttribute("REQUESTTIME",ACCEPTDATE); //����ʱ��
	
	PayPlatRequestParameter1->LinkEndChild(CTRL_INFO1);
	
	TiXmlElement *PARAMETERS1 =new TiXmlElement("PARAMETERS");	
	PayPlatRequestParameter1->LinkEndChild(PARAMETERS1);

	TiXmlElement *ORDERSEQEle1=new TiXmlElement("ORDERSEQ");
	TiXmlElement *OUTTMNNUMNOEle1=new TiXmlElement("TMNNUMNO");
	TiXmlElement *SELECTTYPEEle1=new TiXmlElement("SELECTTYPE");
	TiXmlElement *SELECTVALUEEle1=new TiXmlElement("SELECTVALUE");
	TiXmlElement *ACCEPTDATEEle1=new TiXmlElement("ACCEPTDATE");
	TiXmlElement *ACCEPTAREACODEEle1=new TiXmlElement("ACCEPTAREACODE");//�������
	TiXmlElement *ADDITEM1Ele1=new TiXmlElement("ADDITEM1");//��ѯ�ĵ���

	PARAMETERS1->LinkEndChild(ORDERSEQEle1);
	PARAMETERS1->LinkEndChild(OUTTMNNUMNOEle1);
	PARAMETERS1->LinkEndChild(SELECTTYPEEle1);
	PARAMETERS1->LinkEndChild(SELECTVALUEEle1);
	PARAMETERS1->LinkEndChild(ACCEPTDATEEle1);
	PARAMETERS1->LinkEndChild(ACCEPTAREACODEEle1);
	PARAMETERS1->LinkEndChild(ADDITEM1Ele1);

	//��ֵ
	TiXmlText *ORDERSEQTxt1=new TiXmlText(orderNo);
	ORDERSEQEle1->LinkEndChild(ORDERSEQTxt1);
	TiXmlText *OUTTMNNUMNOTxt1=new TiXmlText(OUTTMNNUMNO);
	OUTTMNNUMNOEle1->LinkEndChild(OUTTMNNUMNOTxt1);
	TiXmlText *SELECTTYPETxt1=new TiXmlText(SELECTTYPE);
	SELECTTYPEEle1->LinkEndChild(SELECTTYPETxt1);
	TiXmlText *SELECTVALUETxt1=new TiXmlText(SELECTVALUE);
	SELECTVALUEEle1->LinkEndChild(SELECTVALUETxt1);
	TiXmlText *ACCEPTDATETxt1=new TiXmlText(ACCEPTDATE);
	ACCEPTDATEEle1->LinkEndChild(ACCEPTDATETxt1);
	TiXmlText *ACCEPTAREACODETxt1=new TiXmlText(ACCEPTAREACODE);
	ACCEPTAREACODEEle1->LinkEndChild(ACCEPTAREACODETxt1);
	TiXmlText *ADDITEM1Txt1=new TiXmlText(ADDITEM1);
	ADDITEM1Ele1->LinkEndChild(ADDITEM1Txt1);

	TiXmlElement *ADDITEM2Ele1=new TiXmlElement("ADDITEM2");//��ѯ���
	PARAMETERS1->LinkEndChild(ADDITEM2Ele1);
	TiXmlText *ADDITEM2Txt1=new TiXmlText(ADDITEM2);
	ADDITEM2Ele1->LinkEndChild(ADDITEM2Txt1);

	//ƽ̨���ݽ���

	//������ַ���
	TiXmlPrinter printer;
	printer.SetLineBreak(0);
	printer.SetIndent(0);
	pDoc1->Accept(&printer);
	strReq1=printer.CStr();
	wrLog("strReq1:%s",strReq1.c_str());
	wrLog("strReq0:%s",strReq0.c_str());
	ANSItoUTF8(strReq1);
	ANSItoUTF8(strReq0);
	return 0;

}




//ANSIתUTF8  
void CxmlParser:: ANSItoUTF8(string &strAnsi)  
{  
    //��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�������936Ϊ��������GB2312����ҳ  
    UINT nLen = MultiByteToWideChar(936,NULL,strAnsi.c_str(),-1,NULL,NULL);  
    WCHAR *wszBuffer = new WCHAR[nLen+1];  
    nLen = MultiByteToWideChar(936,NULL,strAnsi.c_str(),-1,wszBuffer,nLen);  
    wszBuffer[nLen] = 0;  
    //��ȡתΪUTF8���ֽں���Ҫ�Ļ�������С���������ֽڻ�����  
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
    CHAR *szBuffer = new CHAR[nLen+1];  
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
    szBuffer[nLen] = 0;  
	
    strAnsi = szBuffer;  
    //�ڴ�����  
    delete []wszBuffer;  
    delete []szBuffer;  
}  


//UTF8תANSI  
void CxmlParser:: UTF8toANSI(string &strUTF8)  
{  
    //��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����  
    UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8.c_str(),-1,NULL,NULL);  
    WCHAR *wszBuffer = new WCHAR[nLen+1];  
    nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8.c_str(),-1,wszBuffer,nLen);  
    wszBuffer[nLen] = 0;  
	
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);  
    CHAR *szBuffer = new CHAR[nLen+1];  
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);  
    szBuffer[nLen] = 0;  
	
    strUTF8 = szBuffer;  
    //�����ڴ�  
    delete []szBuffer;  
    delete []wszBuffer;  
}  


int CxmlParser::unpack(const string &_strSrc, string m_strTransCode, map<string, string> &_mapData, string &_strErr)
{
    //�Ȼ�ȡ����ǩ������
	
    string::size_type uiStart = 0, uiEnd = 0;
    string strStart="";
	string strEnd="";
	string RESPONSECODE_str="";
    string strUnsign="";
	string sign_decode64=""; 
	string strCert="";
	string showMsg="";

	showMsg=_strSrc;
	UTF8toANSI(showMsg);
	wrLog("�������:%s",showMsg.c_str());

    strStart = "<PayPlatResponseParameter>";
    strEnd = "</PayPlatResponseParameter>";        
    uiStart = _strSrc.find(strStart);
    uiEnd = _strSrc.find(strEnd);
	
    if (uiStart == string::npos) 
    {        
        uiStart = 0;
    }  
    /*   
    else
    {
        uiStart = uiStart + strStart.size();
    } 
    */          
    if (uiEnd == string::npos) 
    {        
        uiEnd = _strSrc.size();
    }          
    else
    {
        uiEnd = uiEnd + strEnd.size();
    } 
    
    strUnsign = _strSrc.substr(uiStart, uiEnd-uiStart);

    //���            
    int i=0, iFind=0, iRet=0;
    string strGbkValue, strTmp;
    const field_cfg * pFieldCfg = NULL;
	map<string, string>::iterator iter;
	TiXmlDocument	docXml;
    TiXmlElement *peleRoot = NULL;
    TiXmlElement *peleGrant = NULL;
    TiXmlElement *peleParent = NULL;
    TiXmlElement *peleSelf = NULL;
    
    docXml.Parse(_strSrc.c_str(), 0, TIXML_ENCODING_UTF8);
    //docXml.Parse(_strSrc.c_str()); //��2��������Ĭ��ֵ����ֶδ�����???
    //���Ҹ��ڵ�
    if ( (peleRoot=docXml.RootElement()) == NULL ) //Request
	{
	    _strErr = "���ڵ㲻����";
		return -1;
	}        
    
	CxmlParser xmlPaser;
	pFieldCfg=xmlPaser.get_cfg(2,m_strTransCode);
    if( pFieldCfg == NULL )
    {
        _strErr = "��ȡ��������ʧ��";
        return -1;
    }        

    for(; strcmp(pFieldCfg[i].m_strKey,"")!=0; ++i)
    {      
        iFind = 0;
        if (strcmp(pFieldCfg[i].m_strGrant,"")!=0)//�游�ڵ�������������������游�ڵ���
        {
            for (peleGrant=peleRoot->FirstChildElement(); peleGrant!=NULL; peleGrant=peleGrant->NextSiblingElement())    
            {    
                if (strcmp(peleGrant->Value(), pFieldCfg[i].m_strGrant)==0) 
                { 
					/*
					if (TRADE_FLAG==0)
					{
						wrLog("m_strGrant:%s",pFieldCfg[i].m_strGrant);
						wrLog("peleGrant->Value():%s",peleGrant->Value());
					}
					*/

                    iFind = 1;
                    break;  
                }  
				
	
            }             
        }            
        if (iFind == 0)
        {
            peleGrant = peleRoot;                
        }   
              
        //���������ĸ��ڵ���    
        iFind = 0;    
        for (peleParent=peleGrant->FirstChildElement(); peleParent!=NULL; peleParent=peleParent->NextSiblingElement())    
        {   
            if (strcmp(peleParent->Value(),pFieldCfg[i].m_strParent)==0) 
            {  
                iFind = 1;
                break;  
            }  
	
        }             
        if (iFind == 0)
        {
            //_strErr = "�������ݴ���, ���ڵ㲻����(" + pFieldCfg[i].m_strKey + ")";
            //return -1;
            continue;
        }                       
        //���ҽڵ㱾���Ƿ����
        iFind = 0;
        for (peleSelf=peleParent->FirstChildElement(); peleSelf!=NULL; peleSelf=peleSelf->NextSiblingElement())    
        {    
            if (strcmp(peleSelf->Value(),pFieldCfg[i].m_strSelf)==0) 
            {  

                iFind = 1;
                break;  
            }  


        }         
        if (iFind == 0)
        {
            //_strErr = "�������ݴ���, ����ڵ㲻����(" + pFieldCfg[i].m_strKey + ")";
            //return -1;
            continue;
        }                       
        //if (peleSelf->NoChildren())//���ò����ָ�Ƿ�������
        if (peleSelf->FirstChildElement() == NULL)
        {            
            if (strcmp(pFieldCfg[i].m_strAttr,"")==0) //������
            {
                if (peleSelf->GetText() != NULL)
                {  
                    strTmp = string(peleSelf->GetText());

					if (pFieldCfg[i].m_iIsCN==1)
					{

						CxmlParser::UTF8toANSI(strTmp);
					}

					  strGbkValue = strTmp;
                     _mapData[pFieldCfg[i].m_strKey] = strGbkValue.c_str();
                }   
			
            }
            else
            {
                if (peleSelf->Attribute(pFieldCfg[i].m_strAttr) != NULL) //���������ֵ
                {
                 
					strTmp = peleSelf->Attribute(pFieldCfg[i].m_strAttr);
					
                     if (pFieldCfg[i].m_iIsCN==1)
                     {
						 CxmlParser::UTF8toANSI(strTmp);
                     }
					
					  strGbkValue = strTmp;
	                _mapData[pFieldCfg[i].m_strKey] = strGbkValue.c_str();
                }                                                         
            } 
        }
        else
        {       
            //cout << "do specfield" << endl;
        	TiXmlPrinter *ppriXml = new TiXmlPrinter(); 
            peleSelf->Accept(ppriXml); 
        	//cout << string(ppriXml->CStr()) << endl;
            strTmp = string(ppriXml->CStr()); 
			if (pFieldCfg[i].m_iIsCN==1)
			{
				CxmlParser::UTF8toANSI(strTmp);
			}

			 strGbkValue = strTmp;
			 _mapData[pFieldCfg[i].m_strKey] = strGbkValue.c_str();
            //do_specfield(peleSelf, _mapData);
        }                                  
    }  
	
    string strSign="";
	strCert="";
    if ( (iter=_mapData.find("SIGN")) == _mapData.end() )
    {
        _strErr = "��ǩʧ��,û��ǩ����Ϣ";
        return -1;
    }   

    strSign = iter->second; 
	wrLog("��ǩ����:%s",strSign.c_str());

    //cout << strSign << endl;    
    if ( (iter=_mapData.find("CER")) == _mapData.end() )
    {
        _strErr = "��ǩʧ��,û�й�Կ��Ϣ";
        return -1;
    }

    strCert = iter->second;  
    //cout << strCert << endl;
	sign_decode64=base64_decode(strSign);

    iRet=Ccert::cert_verify(strCert,strUnsign, sign_decode64);        
    if ( iRet != 0)
    {
 
        _strErr = "��ǩʧ��";
        return -1;
    } 
	
	//��鷵������Ϣ�����ش�����Ϣ
	if ( (iter=_mapData.find("RSPCODE")) == _mapData.end() )
    {
        _strErr = "�޷�����";
        return -2;
    }   

    RESPONSECODE_str = iter->second; 
	if (strcmp(RESPONSECODE_str.c_str(),"000000")!=0) //��������벻��
	{
	
		if ((iter=_mapData.find("RSPMSG"))==_mapData.end())
		{
			_strErr="�޴�����Ϣ";
			
			return -3;

		}

		_strErr=iter->second;
		wrLog("_strErr:%s",_strErr.c_str());
	
	}

    return 0;
}   


const field_cfg * CxmlParser::get_cfg(int _iReqOrRsp,string m_strTransCode)
{
    if (_iReqOrRsp == 1)
    {
            
       if(m_strTransCode == "INF06002")//���в�ѯ(ˮ��ú)
        {
            return &acReq_PublicTransQuery[0];
        }            
        else if(m_strTransCode == "INF06003")//�˵���ѯ(ˮ��ú)
        {
            return &acReq_PublicBillQuery[0];
        }            
        else if(m_strTransCode == "INF06102")//�ɷ�(ˮ��ú)
        {
            return &acReq_PublicPay[0];
        }  
		else if(m_strTransCode == "INF06006")//����˰�˵���ѯӦ����
        {
            return &acReq_carBillQuery[0];
        }  

		else if(m_strTransCode == "INF06104")//����˰�ɷѲ�ѯӦ����
        {
            return &acReq_carTaxPay[0];
        }  
		else if(m_strTransCode == "INF02021")//3g��������ѯӦ����
        {
            return &acReq_g3Pay[0];
        }  
		else if (m_strTransCode=="INF06004")
		{
			return &acReq_g3PhoneCheck[0];
		}
		else if (m_strTransCode=="INF02011") //ȫ�����ѳ�ֵ
		{
			return  &acReq_phonePay[0];
		}
		else if(m_strTransCode == "INF02038")//�ƶ����㶫ʡ�ڣ��ֻ����ѳ�ֵ������
        {
            return &acReq_phonePayGDYD[0];
        } 
		else if (m_strTransCode=="INF06201") //[��ݸ����]ǩ��
		{
			return &acReq_DGTSignIn[0];
		}
		else if (m_strTransCode=="INF06202") //[��ݸ����]ǩ��
		{
			return &acReq_DGTSignOut[0];
		}
		else if (m_strTransCode=="INF06203") //[��ݸ����]������
		{
			return &acReq_DGTCardOperate[0];
		}
		else if (m_strTransCode=="INF06204") //[��ݸ����]����������
		{
			return &acReq_DGTCardSale[0];
		}
		else if (m_strTransCode=="INF06205") //INF06205	[��ݸ����]��ֵ������
		{
			return &acReq_DGTCardCharge[0];
		}
		else if (m_strTransCode=="INF01022") //INF01022 ��������
		{
			return &acReq_dgt_orderProduce[0];
		}
		else if (m_strTransCode=="INF01023") //INF01023 ����ȷ��
		{
			return &acReq_dgt_orderVerify[0];
		}
		else if (m_strTransCode=="INF06208") //��ݸͨͨ��keep��ѯ
		{
			return &acReq_dgt_keepQry [0];
		}
		else if (m_strTransCode=="INF02037") //��»QQ
		{
			return &acReq_QQ_guanglu_pay [0];
		}
		else if (m_strTransCode=="INF02039") //����qq
		{
			return &acReq_QQ_guanglu_pay [0];
		}
		else if (m_strTransCode=="INF02002") //���п���ѯ
		{
			return &acReq_bankInfoQry [0];
		}

    }
    else
    {
        
       if(m_strTransCode == "INF06002")//���в�ѯ(ˮ��ú)
        {
            return &acRsp_PublicTransQuery[0];
        }            
        else if(m_strTransCode == "INF06003")//�˵���ѯ(ˮ��ú)
        {
            return &acRsp_PublicBillQuery[0];
        }            
        else if(m_strTransCode == "INF06102")//�ɷ�(ˮ��ú)
        {
            return &acRsp_PublicPay[0];
        } 
		else if(m_strTransCode == "INF06006")//����˰�˵���ѯӦ����
        {
            return &acRsp_carBillQuery[0];
        } 
		else if(m_strTransCode == "INF06104")//����˰�ɷ�Ӧ����
        {
            return &acRsp_carTaxPay[0];
        } 
		else if(m_strTransCode == "INF02021")//������
        {
            return &acRsp_G3Pay[0];
        } 
		else if (m_strTransCode=="INF06004")
		{
			return &acRsp_g3PhoneCheck[0];
		}
		else if (m_strTransCode=="INF02011") //ȫ�����ѳ�ֵ
		{
		
			return &acRsp_G3Pay[0];

		}
		else if(m_strTransCode == "INF02038")//�ƶ����㶫ʡ�ڣ��ֻ����ѳ�ֵӦ����
        {
            return &acRsp_phonePayGDYD[0];
        } 
		else if (m_strTransCode=="INF06201") //[��ݸ����]ǩ��
		{
			return &acRsp_DGTSignIn[0];
		}
		else if (m_strTransCode=="INF06202") //[��ݸ����]ǩ��
		{
			return &acRsp_DGTSignOut[0];
		}
		else if (m_strTransCode=="INF06203") //[��ݸ����]������
		{
			return &acRsp_DGTCardOperate[0];
		}
		else if (m_strTransCode=="INF06204") //[��ݸ����]����������
		{
			return &acRsp_DGTCardSale[0];
		}
		else if (m_strTransCode=="INF06205") //INF06205	[��ݸ����]��ֵ������
		{
			return &acRsp_DGTCardCharge[0];
		}
		else if (m_strTransCode=="INF01022") //INF01022	
		{
			return &acRsp_dgt_orderProduce[0];
		}

		else if (m_strTransCode=="INF01023") //INF01023
		{
			return &acRsp_dgt_orderVerify[0];
		}
		else if (m_strTransCode=="INF06208") //INF06208
		{
			return &acRsp_dgt_keepQry[0];
		}
		else if (m_strTransCode=="INF02037") //��»qq
		{
			return &acRsp_QQ_guanglu_pay[0];
		}
		else if (m_strTransCode=="INF02039") //����qq
		{
			return &acRsp_QQ_guanglu_pay[0];
		}

		else if (m_strTransCode=="INF02002") //���п���ѯ
		{
			return &acRsp_bankInfoQry[0];
		}
			 
    }                        
    
    return NULL;                
}


int CxmlParser::getBills( string datas,CGlobal gbl) //��ȡ�û��˵���Ϣ
{

	int totalNum=0;
	string dataXMLBuffer="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	dataXMLBuffer.append(datas);
	wrLog("dataXml:%s",dataXMLBuffer.c_str());

	//����datas xml��ȡarea_datas
	TiXmlDocument	docXml;
    docXml.Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml.RootElement();
	TiXmlElement *entity=rootElement->FirstChildElement();
	int i =0;

	while (entity)
	{

		TiXmlAttribute *indexAttribute=entity->FirstAttribute();
		while(indexAttribute)
		{
		
			const char *attributeName=indexAttribute->Name();
			if (strcmp(attributeName,"CUSTOMERNAME")==0) //�ͻ�����
			{
			
				
				strcpy(gbl.sdmBills.BillDetails[i].CUSTOMERNAME,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLNO")==0) // �ɷѵ���
			{
				strcpy(gbl.sdmBills.BillDetails[i].BILLNO,indexAttribute->Value());

			}
			else if (strcmp(attributeName,"BILLMONTH")) //����
			{
				strcpy(gbl.sdmBills.BillDetails[i].BILLMONTH,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLBATCH"))
			{

				strcpy(gbl.sdmBills.BillDetails[i].BILLBATCH,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"CONTRACTNO"))
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].CONTRACTNO,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"BILLAMOUNT")) // �ɷѽ��
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLAMOUNT,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLDELAY"))
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLDELAY,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLDELAY"))
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BALANCE,indexAttribute->Value());
			}
			else  if (strcmp(attributeName,"BILLDATE")) //��������
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLDATE,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"LASTPAYDATE")) //��������
			{
				strcpy(gbl.sdmBills.BillDetails[i].LASTPAYDATE,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"LASTPAYDATE")) //���ɷ�����
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].LASTPAYDATE,indexAttribute->Value());
			}
			
			else  if (strcmp(attributeName,"BILLSTATUS")) //����״̬
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].BILLSTATUS,indexAttribute->Value());
			}
			
			else  if (strcmp(attributeName,"PASSWORD")) //�˺�����
			{
				
				strcpy(gbl.sdmBills.BillDetails[i].PASSWORD,indexAttribute->Value());
			}

			else  if (strcmp(attributeName,"THIRDCODE")) //�˺�����
			{
				strcpy(gbl.sdmBills.BillDetails[i].THIRDCODE,indexAttribute->Value());
			}

			indexAttribute=indexAttribute->Next();
			i ++;
		
		}

		
		entity=entity->NextSiblingElement;
	}

	return 0;

}

//��ȡ�û����˵���Ϣ
int  CxmlParser::getAreaDatas( string datas,CGlobal gbl)
{

	int iRet;
	TSTxnLogs txnLogs;
	char content[500];
	int content_len=0;
	char index[2+1];
	int datasLen=0;
	bbzero(index);
	BUSDATAS  areasDataBuffer;
	//ANSItoUTF8(datas);
	datasLen=datas.size();
	string  dataXMLBuffer="<?xml version=\".0\" encoding=\"UTF-8\" ?>\n";
	memset(&areasDataBuffer, 0, sizeof(areasDataBuffer));
	bbzero(content);// PAYMENTCODE(16)+BUSCODE(3)+strlen(PAYMENTNAME)+PAYMENTNAME(128)+strlen(BUSNAME)+BUSNAME(64+1)
	memset(&txnLogs, 0, sizeof(txnLogs));
	dataXMLBuffer.append(datas);
	wrLog("datas����:%d",datas.size());
	
	wrLog("dataXml:%s",dataXMLBuffer.c_str());

	
	if(!sdm_datasVector.empty())
	{
		sdm_datasVector.clear();
	}

  
	//����datas xml��ȡarea_datas
	
	TiXmlDocument *docXml=new TiXmlDocument;
    docXml->Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml->RootElement();
	TiXmlElement *entity=NULL;

	for  (entity=rootElement->FirstChildElement();entity!=NULL;entity=entity->NextSiblingElement())
	{
		memset(&areasDataBuffer, 0, sizeof(areasDataBuffer));

		TiXmlAttribute *indexAttribute=new TiXmlAttribute();
		
		for(indexAttribute=entity->FirstAttribute();indexAttribute!=NULL;indexAttribute=indexAttribute->Next())
		{
			string attributeName="";
			attributeName.assign(indexAttribute->Name(),strlen(indexAttribute->Name()));

			//strcpy(attributeName.c_str(),indexAttribute->Name());
			
			if (strcmp(attributeName.c_str(),"PAYMENTCODE")==0)
			{
				bbzero(areasDataBuffer.PAYMENTCODE);
				strcpy(areasDataBuffer.PAYMENTCODE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName.c_str(),"BUSCODE")==0)
			{
	
				bbzero(areasDataBuffer.BUSCODE);
				strcpy(areasDataBuffer.BUSCODE,indexAttribute->Value());

			}
			else if (strcmp(attributeName.c_str(),"PAYMENTNAME")==0)
			{
				bbzero(areasDataBuffer.PAYMENTNAME);
				string PAYMENTNAME="";
				PAYMENTNAME=indexAttribute->Value();
				UTF8toANSI(PAYMENTNAME);
				strcpy(areasDataBuffer.PAYMENTNAME,PAYMENTNAME.c_str());

			}

			else  if (strcmp(attributeName.c_str(),"BUSNAME")==0)
			{

				bbzero(areasDataBuffer.BUSNAME);
				string BUSNAME="";
				BUSNAME=indexAttribute->Value();
				//strcpy(BUSNAME.c_str(),indexAttribute->Value());
 				UTF8toANSI(BUSNAME);
				strcpy(areasDataBuffer.BUSNAME,BUSNAME.c_str());
			
			}
		
		}
		
		sdm_datasVector.push_back(areasDataBuffer);

	}
	
	
	//��ʾ
	txnLogs.colsnum = 5;
	txnLogs.colsWidth[0] = 50;
	txnLogs.colsWidth[1] = 100;
	txnLogs.colsWidth[2] = 250;
	txnLogs.colsWidth[3] = 0;
	txnLogs.colsWidth[4] = 0;
	txnLogs.colsNameLen = 60;
	memcpy(txnLogs.colsName, "���\x00ҵ������\x00�շѵ�λ\x00�˵�ID\x00�˵�����", txnLogs.colsNameLen);
	sprintf(txnLogs.top, "��ѡ��ˮ��úҵ��ɷ���Ŀ");

	for(int i=0; i<sdm_datasVector.size();i++)
	{
		char bus_name[64+1];
		char PAYMENTNAME[128+1];
		bbzero(bus_name);
		bbzero(PAYMENTNAME);
		strcpy(bus_name,sdm_datasVector[i].BUSNAME); 
		strcpy(PAYMENTNAME,sdm_datasVector[i].PAYMENTNAME);
		sprintf(index,"%02d",(i+1));
		if( txnLogs.itemsLen+strlen(index) < MAX_INTERM_BUF ) //���
		{
			strcpy(txnLogs.items+txnLogs.itemsLen, index); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}
	
		if( txnLogs.itemsLen+strlen(bus_name) < MAX_INTERM_BUF ) //ҵ������
		{
			strcpy(txnLogs.items+txnLogs.itemsLen, bus_name); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;

		}

		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(PAYMENTNAME) < MAX_INTERM_BUF ) //�շѵ�λ
		{
			strcpy(txnLogs.items+txnLogs.itemsLen, PAYMENTNAME); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
		}
		
		else
		{
			break;
		}

		
		//�·��˵�
		if( txnLogs.itemsLen+strlen("787") < MAX_INTERM_BUF )  
		{
			
			strcpy(txnLogs.items+txnLogs.itemsLen, "787"); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen("787") + 1;
		}
		
		else
		{
			break;
		}

  
		//�·�����
		char len_buffer[2+1];
		bbzero(content);
		content_len=0;
		bbzero(len_buffer);
		sprintf(len_buffer,"%02d",strlen(sdm_datasVector[i].PAYMENTCODE));
		memcpy(content+content_len,len_buffer,2);
		content_len+=2;
		memcpy(content+content_len,sdm_datasVector[i].PAYMENTCODE,strlen(sdm_datasVector[i].PAYMENTCODE));
		content_len+=strlen(sdm_datasVector[i].PAYMENTCODE);

		memcpy(content+content_len,sdm_datasVector[i].BUSCODE,3);
		content_len+=3;

		bbzero(len_buffer);
		sprintf(len_buffer,"%02d",strlen(PAYMENTNAME));
		memcpy(content+content_len,len_buffer,2);
		content_len+=2;
		memcpy(content+content_len,sdm_datasVector[i].PAYMENTNAME,atoi(len_buffer));
		content_len+=strlen(sdm_datasVector[i].PAYMENTNAME);
		bbzero(len_buffer);
		sprintf(len_buffer,"%02d",strlen(sdm_datasVector[i].BUSNAME));
		memcpy(content+content_len,len_buffer,2);
		content_len+=2;
		memcpy(content+content_len,sdm_datasVector[i].BUSNAME,atoi(len_buffer));
		content_len+=strlen(sdm_datasVector[i].BUSNAME);

		if((txnLogs.itemsLen+strlen(content)) < MAX_INTERM_BUF )  //��Ҫ�˵���ѯ��ֵ strlen(paymentcode)+PAYMENTCODE+BUSCODE(3)+strlen(PAYMENTNAME)+PAYMENTNAME(128)+strlen(BUSNAME)+BUSNAME(64+1)
		{

			strcpy(txnLogs.items+txnLogs.itemsLen, content); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
		}
		
		else
		{
			break;
		}

	}
		sprintf(txnLogs.hint, "��%d���ɷ���Ŀ", i);
		iRet=TS_ShowTxnLogs(gbl.dPosInfo,&txnLogs);
		if (iRet!=0)
		{
			wrLog("TS_ShowTxnLogs���ش���[%d]", iRet); 
			return -1;
		}

		return 0 ;
	
}

//��ȡ�û����˵���Ϣ

int  CxmlParser::getBillDatas(CGlobal *gbl, string datas)
{

	string dataXMLBuffer="";
	int content_len=0;
	dataXMLBuffer="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	dataXMLBuffer.append(datas);

	wrLog("dataXml:%s",datas.c_str());
	wrLog("���ݳ���:%d",datas.size());
  
	//����datas xml��ȡ�˵���ϸ���
	memset(&gbl->sdmBills,0,sizeof(gbl->sdmBills));
	TiXmlDocument *docXml=new TiXmlDocument;
    docXml->Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml->RootElement();
	TiXmlElement *entity=NULL;
	int i=0;

	for  (entity=rootElement->FirstChildElement();entity!=NULL;entity=entity->NextSiblingElement())
	{

		TiXmlAttribute *indexAttribute=new TiXmlAttribute();
		
		for(indexAttribute=entity->FirstAttribute();indexAttribute!=NULL;indexAttribute=indexAttribute->Next())
		{
			char attributeName[50];
			bbzero(attributeName);
			strcpy(attributeName,indexAttribute->Name());
			
			if (strcmp(attributeName,"CUSTOMERNAME")==0)
			{
				
				string CUSTOMERNAME="";
				CUSTOMERNAME=indexAttribute->Value();
				UTF8toANSI(CUSTOMERNAME);
				bbzero(gbl->sdmBills.BillDetails[i].CUSTOMERNAME);
				strcpy(gbl->sdmBills.BillDetails[i].CUSTOMERNAME,CUSTOMERNAME.c_str());
				
			}
			else if (strcmp(attributeName,"BILLBARCODE")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].BILLBARCODE);
				strcpy(gbl->sdmBills.BillDetails[i].BILLBARCODE,indexAttribute->Value());

			}
			else if (strcmp(attributeName,"BILLNO")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLNO);
				strcpy(gbl->sdmBills.BillDetails[i].BILLNO,indexAttribute->Value());

			}
			else  if (strcmp(attributeName,"BILLMONTH")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].BILLMONTH);
				strcpy(gbl->sdmBills.BillDetails[i].BILLMONTH,indexAttribute->Value());
			
			}
			else if (strcmp(attributeName,"BILLBATCH")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].BILLBATCH);
				strcpy(gbl->sdmBills.BillDetails[i].BILLBATCH,indexAttribute->Value());

			}

			else if (strcmp(attributeName,"CONTRACTNO")==0)
			{

				bbzero(gbl->sdmBills.BillDetails[i].CONTRACTNO);
				strcpy(gbl->sdmBills.BillDetails[i].CONTRACTNO,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLAMOUNT")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLAMOUNT);
				strcpy(gbl->sdmBills.BillDetails[i].BILLAMOUNT,indexAttribute->Value());
				wrLog("sdm_bill_detail.BILLAMOUNT:%s",gbl->sdmBills.BillDetails[i].BILLAMOUNT);
				
			}

			else if (strcmp(attributeName,"BILLDELAY")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLDELAY);
				strcpy(gbl->sdmBills.BillDetails[i].BILLDELAY,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BALANCE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BALANCE);
				strcpy(gbl->sdmBills.BillDetails[i].BALANCE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLDATE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLDATE);
				strcpy(gbl->sdmBills.BillDetails[i].BILLDATE,indexAttribute->Value());
				
			}

			else if (strcmp(attributeName,"LASTPAYDATE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].LASTPAYDATE);
				strcpy(gbl->sdmBills.BillDetails[i].LASTPAYDATE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLSTATUS")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].BILLSTATUS);
				strcpy(gbl->sdmBills.BillDetails[i].BILLSTATUS,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"PASSWORD")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].PASSWORD);
				strcpy(gbl->sdmBills.BillDetails[i].PASSWORD,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"THIRDCODE")==0)
			{
				bbzero(gbl->sdmBills.BillDetails[i].THIRDCODE);
				strcpy(gbl->sdmBills.BillDetails[i].THIRDCODE,indexAttribute->Value());
				
			}
			
		
		}

		i ++;

	}

		gbl->sdmBills.nCnt=i;
		wrLog("����:%d",gbl->sdmBills.nCnt);

		return 0 ;
}


/*
 * ��    ��	--	xml���ͱ������
 * �������	-- _mapData	���������
 * �������	-- _strDest	�����ı���
 *          -- _strErr	������Ϣ
 * return	--	0:�ɹ�  -1:ʧ��
 */
int CxmlParser::pack(string m_strTransCode,string &strReq1, map<string, string> &_mapData, string &_strErr)
{   
    int i=0, iFind=0, iRet=0;
    string strXml, strUtfValue;
    const field_cfg * pFieldCfg = NULL;
    map<string, string>::iterator iter;
	TiXmlDocument	docXml;
    TiXmlElement *peleRoot = NULL;
    TiXmlElement *peleGrant = NULL;
    TiXmlElement *peleParent = NULL;
    TiXmlElement *peleSelf = NULL;
	string data_sign_base64="";
	string show_strReq1="";

    if (strcmp(strReq1.c_str(),"")==0)
    {        
        strReq1 = "<?xml version=\"1.0\" encoding=\"utf-8\"?><Request><VerifyParameter></VerifyParameter><PayPlatRequestParameter></PayPlatRequestParameter></Request>"; //��ʼ��
    }        
    docXml.Parse(strReq1.c_str(), 0, TIXML_ENCODING_UTF8);
    //docXml.Parse(_strDest.c_str()); //��2��������Ĭ��ֵ����ֶδ�����???
    /*
    if (docXml.Parse(strXml.c_str()) == NULL)
    {
        _strErr = "���ɳ�ʼXMLʧ��";
        return -1;
    }   
    */     
    //���Ҹ��ڵ�
    if ( (peleRoot=docXml.RootElement()) == NULL ) //Request
	{
	    _strErr = "���ڵ㲻����";
		return -1;
	}        
    
	CxmlParser xmlPaser;
	pFieldCfg=xmlPaser.get_cfg(1,m_strTransCode);

    if( pFieldCfg== NULL )
    {
        _strErr = "��ȡ��������ʧ��";
        return -1;
    }        

	
    for(; strcmp(pFieldCfg[i].m_strKey,"")!=0; ++i)
    {

		//wrLog(" pFieldCfg[%d].m_strKey:%s" ,i,pFieldCfg[i].m_strKey);


        if ( (iter=_mapData.find(pFieldCfg[i].m_strKey)) == _mapData.end() )
        {
			wrLog("not find");
            continue;
        }   
		

        iFind = 0;
        if (strcmp(pFieldCfg[i].m_strGrant,"")!=0)//�游�ڵ�������������������游�ڵ���
        {

            for (peleGrant=peleRoot->FirstChildElement(); peleGrant!=NULL; peleGrant=peleGrant->NextSiblingElement())    
            {    
                if (strcmp(peleGrant->Value(),pFieldCfg[i].m_strGrant)==0) 
                {  
                    iFind = 1;
                    break;  
                }  
            }             
        }            
        if (iFind == 0)
        {
            peleGrant = peleRoot;                
        }   
        iFind = 0;    
        if (strcmp(pFieldCfg[i].m_strParent,"")==0)
        {
			char errBuffer[100];
			bbzero(errBuffer);
			sprintf(errBuffer,"�������ݴ���:%s",pFieldCfg[i].m_strKey);
			_strErr.assign(errBuffer,strlen(errBuffer));
            return -1;
        }            
        //���������ĸ��ڵ���                 
        for (peleParent=peleGrant->FirstChildElement(); peleParent!=NULL; peleParent=peleParent->NextSiblingElement())    
        {    
            if (strcmp(peleParent->Value(), pFieldCfg[i].m_strParent)==0) 
            {  
                iFind = 1;
                break;  
            }  
        }             
        if (iFind == 0)
        {
            //���븸�ڵ�
            if ( (peleParent=(TiXmlElement *)peleGrant->InsertEndChild(TiXmlElement(pFieldCfg[i].m_strParent))) == NULL)
            {
				char errBuffer[100];
				bbzero(errBuffer);
				sprintf(errBuffer,"���븸�ڵ�ʧ��:%s",pFieldCfg[i].m_strKey );
                _strErr.assign(errBuffer,strlen(errBuffer));
            	return -1;
            } 
        }                       
        
        if (pFieldCfg[i].m_iIsCN == 1)
        {
           
			ANSItoUTF8(iter->second);
			strUtfValue=iter->second;
			//��������ģ���ҪתΪUTF-8���б���
            //strUtfValue = iter->second;
  
        }
        else
        {
            strUtfValue = iter->second;
        }                                
        
        //��������ڵ�
        if (strcmp(pFieldCfg[i].m_strAttr,"")==0)
        {
            if ( (peleSelf=(TiXmlElement *)peleParent->InsertEndChild(TiXmlElement(pFieldCfg[i].m_strSelf))) == NULL)
            {
				char errBuffer[100];
				bbzero(errBuffer);
				sprintf(errBuffer,"��������ڵ�ʧ��:%s",pFieldCfg[i].m_strKey);
            	return -1;
            } 
            peleSelf->Clear();
            TiXmlText *ptxtSelf = new TiXmlText(strUtfValue.c_str());
            peleSelf->LinkEndChild(ptxtSelf);                       
        }
        else
        {
            //���ҽڵ㱾���Ƿ����
            iFind = 0;
            for (peleSelf=peleParent->FirstChildElement(); peleSelf!=NULL; peleSelf=peleSelf->NextSiblingElement())    
            {    
                if (strcmp(peleSelf->Value(),pFieldCfg[i].m_strSelf)==0) 
                {  
                    iFind = 1;
                    break;  
                }  
            } 
            if (iFind == 1)
            {
                peleSelf->SetAttribute(pFieldCfg[i].m_strAttr, strUtfValue.c_str());
            }
            else
            {
	            TiXmlElement eleAdd("");
	            eleAdd.SetAttribute(pFieldCfg[i].m_strAttr, strUtfValue.c_str());
	            eleAdd.SetValue(pFieldCfg[i].m_strSelf);
            	if (peleParent->InsertEndChild(eleAdd) == NULL)
            	{
					char errBuffer[100];
					bbzero(errBuffer);
					sprintf(errBuffer, "��������ڵ�ʧ��[%s]",pFieldCfg[i].m_strKey);
					_strErr.assign(errBuffer,strlen(errBuffer));

            		return -1;
            	}                
            }                                                        
        }                                                 
    }            
    
	TiXmlPrinter *ppriXml = new TiXmlPrinter(); 
	ppriXml->SetIndent(0); //ȥ������
	ppriXml->SetLineBreak(NULL);//ȥ������
    docXml.Accept(ppriXml); 
	strReq1.assign(ppriXml->CStr());

    //ǩ��
    string::size_type uiStart = 0, uiEnd = 0;
    string strStart, strEnd;
    string strUnsign, strSign, strCert;
    strStart = "<PayPlatRequestParameter>";
    strEnd = "</PayPlatRequestParameter>";        
    uiStart = strReq1.find(strStart);
    uiEnd = strReq1.find(strEnd);
    //cout << uiStart << endl;
    //cout << uiEnd << endl;
    if (uiStart == string::npos) 
    {        
        uiStart = 0;
    }  
    /*   
    else
    {
        uiStart = uiStart + strStart.size();
    } 
    */          
    if (uiEnd == string::npos) 
    {        
        uiEnd = strReq1.size();
    }          
    else
    {
        uiEnd = uiEnd + strEnd.size();
    } 
           
    //cout << _strDest << endl;
    strUnsign = strReq1.substr(uiStart, uiEnd-uiStart);
    //cout << strUnsign << endl;
	
	Ccert::data_sign("privateKey.pem",strUnsign,strSign);
	Ccert::cert_key("cert.pem",strCert);
	data_sign_base64=base64_encode(strSign);
	wrLog("sizeof(strCer:%d)",strCert.size());
	wrLog("data_sign_base64:%s",data_sign_base64.c_str());
	wrLog("sizeof(data_sign_base64):%d",data_sign_base64.size());

	char strTmp1[500];
	char strTmp2[2049];
	char sign_tmp[50];
	char cer_tmp[50];
	bbzero(cer_tmp);
	bbzero(sign_tmp);
	bbzero(strTmp1);
	bbzero(strTmp2);

	sprintf(strTmp1,"<SIGN>%s</SIGN>",data_sign_base64.c_str());
	sprintf(strTmp2,"<CER>%s</CER>",strCert.c_str());
	strcpy(sign_tmp,"<SIGN>SIGN</SIGN>");
	strcpy(cer_tmp,"<CER>CER</CER>");
	string string_tmp1="";
	string_tmp1.assign(strTmp1,strlen(strTmp1));
	string string_tmp2="";
	string_tmp2.assign(strTmp2,strlen(strTmp2));
	string sign_stringBuffer="";
	string cer_stringBuffer="";
	sign_stringBuffer.assign(sign_tmp,strlen(sign_tmp));
	cer_stringBuffer.assign(cer_tmp,strlen(cer_tmp));
    CTools::replace(strReq1, sign_stringBuffer,string_tmp1);     
    CTools::replace(strReq1, cer_stringBuffer, string_tmp2);
	show_strReq1=strReq1;
	UTF8toANSI(show_strReq1);
	wrLog("�������ݰ�:%s",show_strReq1.c_str());

    return 0;

} 


//��ȡ�û�����˰�˵���Ϣ

int  CxmlParser::getCardTaxBills( string datas,CGlobal gbl)
{

	string dataXMLBuffer="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	int content_len=0;
	int i=0;
	dataXMLBuffer.append(datas);
	wrLog("dataXml:%s",dataXMLBuffer);
	//����datas xml��ȡ�˵���ϸ���
	
	TiXmlDocument *docXml=new TiXmlDocument;
    docXml->Parse(dataXMLBuffer.c_str(), 0, TIXML_ENCODING_UTF8);
	TiXmlElement *rootElement=docXml->RootElement();
	TiXmlElement *entity=NULL;
	
	memset(&gbl.carTaxBills,0,sizeof(gbl.carTaxBills));

	for  (entity=rootElement->FirstChildElement();entity!=NULL;entity=entity->NextSiblingElement())
	{

		TiXmlAttribute *indexAttribute=new TiXmlAttribute();
		
		for(indexAttribute=entity->FirstAttribute();indexAttribute!=NULL;indexAttribute=indexAttribute->Next())
		{
			

			char attributeName[50];
			bbzero(attributeName);
			strcpy(attributeName,indexAttribute->Name());
			
			if (strcmp(attributeName,"CUSTOMERNAME")==0) //��������
			{
				
				string CUSTOMERNAME="";
				CUSTOMERNAME=indexAttribute->Value();
				UTF8toANSI(CUSTOMERNAME);
				strcpy(gbl.carTaxBills.cardTaxDatas[i].CUSTOMERNAME,CUSTOMERNAME.c_str());
				
			}
			else if (strcmp(attributeName,"BILLBARCODE")==0) //�����ɷ���ˮ��
			{
				
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLBARCODE,indexAttribute->Value());

			}
			else if (strcmp(attributeName,"BILLNO")==0) //��˰��
			{
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLNO,indexAttribute->Value());

			}
			else  if (strcmp(attributeName,"BILLMONTH")==0) //���
			{
	
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLMONTH,indexAttribute->Value());
			
			}
			else if (strcmp(attributeName,"BILLBATCH")==0) //����
			{
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLBATCH,indexAttribute->Value());

			}

			else if (strcmp(attributeName,"CONTRACTNO")==0) //Ӧ�����
			{
				
				strcpy(gbl.carTaxBills.cardTaxDatas[i].CONTRACTNO,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"BILLDELAY")==0) //���ɽ��
			{
	
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BILLDELAY,indexAttribute->Value());

				
			}

			else if (strcmp(attributeName,"BALANCE")==0) //������
			{
				strcpy(gbl.carTaxBills.cardTaxDatas[i].BALANCE,indexAttribute->Value());
				
			}
			else if (strcmp(attributeName,"LASTPAYDATE")==0) //�Ƿ񲹽�
			{
				gbl.carTaxBills.cardTaxDatas[i].LASTPAYDATE=atoi(indexAttribute->Value());
				
			}
			
		}
		

		i++;

	}

		gbl.carTaxBills.nCnt=i;

		return 0 ;
	
}

