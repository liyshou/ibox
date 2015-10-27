#ifndef  _H_CTSI_PASSWORD_H_
#define  _H_CTSI_PASSWORD_H_
#include <string>

class  Ctsipass
{
private:
    static int snd_rcv_msg(const std::string& ip , int port , char* inBuf , size_t bufLen , \
        char* outBuf , size_t outMaxLen , int* outLen);
    static std::string get_datetime();
public:
    Ctsipass();
    ~Ctsipass();
    enum FLAGS
    {
        CHK_PAY_PWD = 1,
        CHK_BOSS_PWD,
        CHK_TRANS_PWD,
        CHG_PAY_PWD,
        CHG_BOSS_PWD,
        CHG_TRANS_PWD
    };
public:
    //获取用于密码验证的IP和端口
    static int getpassaddr(std::string& ip);
    //获取随机数接口，返回定长随机数
    //static std::string getrand(const std::string& termid);

    static int checkpass(const char* termid ,const char* ecard , const char* pwd , FLAGS flags);
    static std::string pep_checkpass(const std::string& pass_type, const std::string& staff, const std::string& termid, const std::string& ecard, const std::string& password);
    
    //加密接口，此处需要工号、随机数等
    static std::string encrypt(const std::string& staff , const std::string& pass);
    
    //密码修改
    static std::string modifypass( const std::string& pass_type , const std::string& termid ,const std::string& ecard , \
        const std::string& staff , const std::string& oldpass , const std::string& newpass, const std::string& cell);
    static int modifypass(const char* termid , const char* ecard , const char* oldpass , const char* newpass ,\
         FLAGS flags , const char* cell = "");
};
#endif