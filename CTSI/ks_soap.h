#ifndef __KS_SOAP_H__
#define __KS_SOAP_H__

#include <string>
using namespace std;
class ks_soap
{
public:
    ks_soap() {}
    virtual ~ks_soap() {}
	
    static int get_response(int tradeFlag,const string &_strReq0, const string &_strReq1, string &_strRsp, string &_strErr);

};
#endif
