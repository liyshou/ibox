#include <cassert>
#include "ctsipass.h"
#include "Connect.h"
#include "wrLog.h"
#include "Tools.h"

using namespace std;

Ctsipass::Ctsipass()
{}
Ctsipass::~Ctsipass()
{}

int Ctsipass::getpassaddr(std::string& ip)
{
    char passip[512] = { 0 };
    char passport[512] = { 0 };
    
    GetPrivateProfileString ("PE","PASS_IP", NULL, (char *)passip, sizeof(passip), ".\\ctsi.ini"); 
    GetPrivateProfileString ("PE","PASS_PORT", NULL, (char *)passport, sizeof(passport), ".\\ctsi.ini"); 
    
    wrLog("密码验证服务器:[%s:%s]" , passip , passport);
    ip = passip;
    return atoi(passport);
}

/*
string Ctsipass::getrand(const string& termid)
{
    char inBuf[128]     = { 0 };
    char outBuf[128]    = { 0 };
    char datetime[16]   = { 0 };
    char bufLen_buf[10] ={ 0 };
    size_t bufLen       = 0;
    int  iRet           = 0;
    int port            = 0;
    int out_len         = 0;
    int timeout         = 100;//默认超时100秒
    string ip           ;
    CConnect connector  ;

    port = getpassaddr(ip);
    if (0 == port || -1 == port)
    {
        wrLog("从配置文件中读取密码验证服务器错误，请检查。[%s:%d]" , __FILE__ , __LINE__);
        return "";
    }

    //组包
    CTools::get_termTime(datetime);
    sprintf(inBuf , "FFFF%14.14s%8.8s%08dPEP000D0RAND" , datetime , termid , 0);
    sprintf(bufLen_buf , "%08d" , strlen(inBuf));
    memcpy(inBuf + 4 + 22 , bufLen_buf , 8);
    
    //发包，收包；
    iRet = snd_rcv_msg(ip , port , inBuf , strlen(inBuf) , outBuf , sizeof(outBuf) , out_len );
    if (-1 == iRet)
    {
        wrLog("与远程服务器 [%s:%d] 通讯出现故障:[%s:%d]" , ip.c_str() , port , __FILE__ , __LINE__);
        return "";
    }
    assert(out_len > 0);

    //处理返回，判断返回码，取出随机数
    char rspCode_buf[6] = { 0 };
    char rspMsg[128]    = { 0 };
    char rspMsg_lbuf[6] = { 0 };
    char rndstr_lbuf[6] = { 0 };
    char rndstr[16]     = { 0 };
    int tmpLen          = 0;
    int offset          = 0;

    memcpy(rspCode_buf , outBuf + 42 , 4);
    offset += 42 + 4 ;
    memcpy(rspMsg_lbuf , outBuf + offset , 2);
    offset += 2;
    tmpLen = atoi(rspMsg_lbuf);
    assert(tmpLen > 0 && tmpLen < 100);

    memcpy(rspMsg      , outBuf + offset , tmpLen);
    offset += tmpLen;
    if (strcmp(rspCode_buf , "0000") != 0)
    {
        wrLog("从服务器返回错误:[%s]->[%s] at %s:%d" , rspCode_buf , rspMsg , __FILE__ , __LINE__);
        return "";
    }

    memcpy(rndstr_lbuf , outBuf + offset , 2);
    offset += 2;
    tmpLen = atoi(rndstr_lbuf);
    assert(tmpLen > 0 && tmpLen < 16);//限定最长16位
    memcpy(rndstr , outBuf + offset , tmpLen);

    return rndstr;
}
*/

int Ctsipass::checkpass(const char* termid, const char* ecard, const char* pwd, FLAGS flags)
{
    string pass_type , staff , retstr ;

    if (flags == CHK_BOSS_PWD)
    {
        staff = termid;
        pass_type = "0002";
    }
    else if (flags == CHK_PAY_PWD)
    {
        staff = termid;
        staff += "_01";
        pass_type = "0001";
    }
    else if (flags == CHK_TRANS_PWD)
    {
        staff = termid;
        pass_type = "0001";
    }
    else
    {
        wrLog("参数错误:[%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }
    
    retstr = pep_checkpass(pass_type, staff, termid, ecard, pwd);
    if (strcmp(retstr.c_str() , "0000") == 0)
    {
        return 0;
    }

    return -1;
}

string Ctsipass::pep_checkpass(const string& pass_type, const string& staff, const string& termid, const string& ecard, const string& password)
{
    //获取IP和端口
    string ip  ;
    string e_flag = "0001";//3DES加密
    char inBuf[512] = {0};
    char len_buf[10] = {0};
    char outBuf[512]= {0};
    int bufLen = 0;
    int port = 0;
    int iRet = 0;
    int outLen = 0;
    
    port = getpassaddr(ip);
    if (0 == port || -1 == port || ip.empty())
    {
        wrLog("从配置文件中读取密码验证服务器错误，请检查。[%s:%d]" , __FILE__ , __LINE__);
        return "";
    }
    
    //组密码验证包
    sprintf(inBuf , "FFFF%014s%08s%08dPEP000B0%04s2000000000000001    %-16s%02d%s%-30s",\
        get_datetime().c_str(), "88300003", 0, pass_type.c_str(), staff.c_str(), password.size(), password.c_str(), ecard.c_str() );
    sprintf(len_buf , "%08d" , strlen(inBuf));
    memcpy(inBuf + 4 + 22 , len_buf , 8);
    wrLog("发送PEP0000B报文到FASACC:[%s]" , inBuf);
    //连接到远程主机    //发包    //收包
    iRet = snd_rcv_msg(ip, port, inBuf, strlen(inBuf), outBuf, sizeof(outBuf), &outLen);
    if (-1 == iRet || outLen == 0)
    {
        wrLog("与远程服务器通讯错误。[%s:%d]", __FILE__ , __LINE__);
        return "8001";
    }
    
    //获取返回码
    int offset = 42 ;//包头
    string rspCode = string(outBuf + offset , 4);
    offset += 4;
    string msg_len = string(outBuf + offset , 2);
    offset += 2;
    string rspMsg  = string(outBuf + offset , atoi(msg_len.c_str()));

    wrLog("服务器返回:[%s:%s]" , rspCode.c_str() , rspMsg.c_str());
    return rspCode;
}

string Ctsipass::modifypass(const std::string& pass_type, const std::string& termid, const std::string& ecard, const std::string& staff, \
                            const std::string& oldpass, const std::string& newpass, const std::string& cell)
{
    char inBuf[2048] = {0};
    char outBuf[2048]= {0};
    char len_buf[10] = {0};
    string e_flag = "0001" ; //3DES加密
    string rndstring;
    string ip;
    int bufLen = 0;
    int port = 0;
    int iRet = 0;
    int outLen = 0;

    //获得IP和端口
    port = getpassaddr(ip);
    if (0 == port || -1 == port || ip.empty())
    {
        wrLog("从配置文件中读取密码验证服务器错误，请检查。[%s:%d]" , __FILE__ , __LINE__);
        return "";
    }

    //组包
    sprintf(inBuf, "FFFF%014s%08s%08dPEP000C0%04s2000000000000001    %-16s%02d%s%02d%s%02d%s%-32s%-30s",\
        get_datetime().c_str(), "88300003", 0, \
        pass_type.c_str(), staff.c_str(), newpass.size(), newpass.c_str(), \
        oldpass.size(), oldpass.c_str(), cell.size(), cell.c_str(), ecard.c_str(), " ");
    sprintf(len_buf, "%08d" , strlen(inBuf));
    memcpy(inBuf + 4 + 22, len_buf, 8);
    
    //连接主机
    wrLog("发送PEP000C0报文到FASACC:[%s]" , inBuf);
    iRet = snd_rcv_msg(ip, port, inBuf, strlen(inBuf), outBuf, sizeof(outBuf), &outLen);
    if (-1 == iRet || 0 == outLen)
    {
        wrLog("与远程服务器通讯错误。[%s:%d]", __FILE__ , __LINE__);
        return "8002";
    }
    wrLog("接收FASACC应答:[%s]", outBuf);
    //解返回码
    int offset = 42 ;//包头
    string rspCode = string(outBuf + offset , 4);
    offset += 4;
    string msg_len = string(outBuf + offset , 2);
    offset += 2;
    string rspMsg  = string(outBuf + offset , atoi(msg_len.c_str()));
    
    wrLog("服务器返回:[%s:%s]" , rspCode.c_str() , rspMsg.c_str());
    return rspCode;
}

int Ctsipass::modifypass(const char* termid, const char* ecard, const char* oldpass, const char* newpass, FLAGS flags, const char* cell)
{
    string pass_type, staff;
    
    if (flags == CHG_BOSS_PWD) {
        staff = termid;
        pass_type = "0002";
    } else if (flags == CHG_PAY_PWD) {
        staff = termid ;
        staff += "_01";
        pass_type = "0001";
    } else if (flags == CHG_TRANS_PWD) {
        staff = termid ;
        pass_type = "0001";
    } else {
        wrLog("参数错误:[%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }

    string ret = modifypass(pass_type, termid, ecard, staff, oldpass, newpass, cell);
    if (ret == "0000") {//成功
        return 0;
    } else if (ret == "2135") {//密码验证失败
		return -1;
	} else if (ret == "8002") {//通信故障
		return -2;
	} else if (ret == "2136") {//密码错误次数过多
		return -3;
	}
	else if (ret == "2230") {
		return -5;
	} else {//其他错误
		return -4;
	}
    return 0;
}

string Ctsipass::encrypt(const std::string& staff , const std::string& pass)
{
    //此处调用外部API
    return "0123456789";
}

int Ctsipass::snd_rcv_msg(const string& ip, int port, char* inBuf, size_t bufLen, char* outBuf, size_t outMaxLen, int* outLen)
{
    CConnect connector;
    SOCKET sd;
    int iRet = 0;
    int timeout = 100;

    //连接到主机
    iRet = connector.conToHost(const_cast<char*>(ip.c_str()), port, &sd);
    if (-1 == iRet)
    {
        wrLog("连接到服务器[%s:%d]失败,[%s:%d]" , ip.c_str() , port , __FILE__ , __LINE__);
        closesocket(sd);
        return -1;
    }
    
    //发包
    iRet = connector.sndMsg(sd, inBuf, bufLen, timeout);
    if (-1 == iRet)
    {
        wrLog("发送数据到目标服务器失败,[%s:%d]" , __FILE__ , __LINE__);
        closesocket(sd);
        return-1;
    }

    //接收应答头
    iRet = recv(sd, outBuf, 4 + 22 + 8, 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8)
    {
        wrLog("从服务器接收数据出错%s:%d" , __FILE__ , __LINE__);
        closesocket(sd);
        return -1;
    }

	//接收应答内容
    int pack_len = atoi(outBuf + 4 + 22) - 4 - 22 - 8 ;//余下包体长度
    int need_rcv = pack_len;
    int has_rcv  = 0;
    while (need_rcv > has_rcv)
    {
        iRet = recv(sd, outBuf + 4 + 22 + 8 + has_rcv, need_rcv - has_rcv, 0);
        if (iRet == INVALID_SOCKET)
        {
            if (errno == EAGAIN)
            {
                Sleep(1);
                continue;
            }
            else
            {
                wrLog("与服务器通信出现故障 %s:%d" , __FILE__ , __LINE__);
                closesocket(sd);
                return -1;
            }
        }
        
        has_rcv += iRet ;
    }

    *outLen = pack_len + 34 ;
    wrHex("接收ACC应答包:" , (BYTE*)outBuf , *outLen);
    closesocket(sd);
    return 0;
}

string Ctsipass::get_datetime()
{
    char datetime[16] = { 0 };
    CTools::get_termTime(datetime);
    return datetime;
}
