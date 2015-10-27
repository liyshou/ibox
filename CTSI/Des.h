// Des.h: interface for the CDes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DES_H__64A8CA26_79E4_47B4_AD20_44AD7E56011D__INCLUDED_)
#define AFX_DES_H__64A8CA26_79E4_47B4_AD20_44AD7E56011D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDes  
{
public:
	CDes();
	virtual ~CDes();
	void static GetDesInput(char *data,char *key,char *output);
	void static GetDesOutput(char *data,char *key,char *output);
	int static TripleDesEncrypt(char *key, char *data, char *encstr);
	int static TripleDesDec(char *key, char *data, char *encstr);
    //���溯��TripleDesDec�����е����⣬��KEYΪ16�ֽ�ʱ��ȷ��KEYΪ24�ֽ�ʱ�Ǵ���ġ�
    //��Ϊ3DES�㷨�У����16λKEY��ʵ����ֻ������DES���ܣ����ε�DES���ܹ�����KEY1��KEY3�ǵ�ͬ��
    //������������㷨����ȷ�ģ���������㷨��"��ʱ��Ҳ����ȷ��"���������ͨ����16λKEY
    int static TripleDesDecrypt(char *key, char *data, char *encstr);

	void static makeMac(char *data, int len, char *key, char *mac);
	void static CharToAsc(char *inputstr,char *outstr);
	size_t static base64(char *pDest, const char *pSrc, size_t srclen);
private:
	const static char IP_Tr[64];
	const static char IP_1_Tr[64];
	const static char Etr[48];
	const static char PC_1_Tr[56];
	const static char PC_2_Tr[48];
	const static char SB[8][64];
	const static char Left_move[16];
	const static char right_move[16];
	const static char swap[64];
	const static char P_Tr[32];

	int static AsciiToBinary(char *binarystr ,char *asciistr);
	void static bin_to_hex_char(char *in_buff, char *out_buff, int bin_len);
	
	char static CharToHex(char chr);
	int static CheckPass(char *DbPass,char *CardNo,char *key,char *UpPass,char EncryptedPass[9]);
	void static des (char *plaintext, char *key, char *ciphertext,char mode);
	void static des_f(char i,char *ri_1,char *key,char *r_f,char mode);
	void static inputtobin(char *inputstr,char *outbin);
	void static leftmove(char *key);
	void static MakeKey(char *inputstr,char *outstr);
	void static MakePass(char *DbPass,char *CardNo,char *outstr);
	void static myinputtobin(char *inputstr,char *outbin);
	void static myouttostr(char *encrypted_pin,char *outstr);
	void static outtostr(char *encrypted_pin,char *outstr);
	void static rightmove(char *key);
	void static str_copy (char *s, char *t, char n);
	int static tobyte (char *instr, char *outstr);
	void static transpose(char *ddata,const char *matrix,char n);
};

#endif // !defined(AFX_DES_H__64A8CA26_79E4_47B4_AD20_44AD7E56011D__INCLUDED_)
