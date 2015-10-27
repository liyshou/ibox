#include "soapH.h"  
#include "DealProcessorSoapBinding.nsmap"  
#include "ks_soap.h"

int ks_soap::get_response(int tradeFlag,const string &_strReq0, const string &_strReq1, string &_strRsp, string &_strErr)
{

    int bw=time(NULL);
    struct soap soap; 
    soap_init(&soap);  
    soap_set_mode(&soap, SOAP_C_UTFSTRING);
	char soap_endPort[100];
	memset(soap_endPort,0,sizeof(soap_endPort));

	if (tradeFlag==1)
	{
		strcpy(soap_endPort,"http://132.97.120.19:4443/bppf_inf/services/DealProcessor");
	}
	else
	{
	//	strcpy(soap_endPort,"http://183.63.191.42:8081/bppf_inf/services/DealProcessor");
	//	strcpy(soap_endPort,"http://172.26.3.8:8081/bppf_inf/services/DealProcessor"); //42
		strcpy(soap_endPort,"http://172.26.8.5:8081/bppf_inf/services/DealProcessor"); //46环境 
	//	strcpy(soap_endPort,"http://172.25.132.19:8081/bppf_inf/services/DealProcessor"); //44
	//	strcpy(soap_endPort,"http://192.168.87.4:8081/bppf_inf/services/DealProcessor"); //develop environment
	
	}
    
    soap.connect_timeout=45;//这个时间控制上面需要研究一下
    soap.send_timeout=45;
    soap.recv_timeout=45;

    if (soap_call_ns1__dispatchCommand(&soap, soap_endPort, NULL,  _strReq0, _strReq1, _strRsp) == SOAP_OK) 
    {
        soap_destroy(&soap);  
        soap_end(&soap);  
        soap_done(&soap); 
        int ew=time(NULL);
        cout << "soap time=" << ew-bw << endl;
        return 0;      
    }            
    else
    {  
        char acErr[1024] = {0};
        soap_sprint_fault(&soap, acErr, 1024);
        _strErr.assign(acErr);
        soap_destroy(&soap);  
        soap_end(&soap);  
        soap_done(&soap); 
        int ew=time(NULL);
        cout << "soap time=" << ew-bw << endl; 
        return -1;      
    }    
    return 0;        
}

