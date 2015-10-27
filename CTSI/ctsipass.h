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
    //��ȡ����������֤��IP�Ͷ˿�
    static int getpassaddr(std::string& ip);
    //��ȡ������ӿڣ����ض��������
    //static std::string getrand(const std::string& termid);

    static int checkpass(const char* termid ,const char* ecard , const char* pwd , FLAGS flags);
    static std::string pep_checkpass(const std::string& pass_type, const std::string& staff, const std::string& termid, const std::string& ecard, const std::string& password);
    
    //���ܽӿڣ��˴���Ҫ���š��������
    static std::string encrypt(const std::string& staff , const std::string& pass);
    
    //�����޸�
    static std::string modifypass( const std::string& pass_type , const std::string& termid ,const std::string& ecard , \
        const std::string& staff , const std::string& oldpass , const std::string& newpass, const std::string& cell);
    static int modifypass(const char* termid , const char* ecard , const char* oldpass , const char* newpass ,\
         FLAGS flags , const char* cell = "");
};
#endif