#include "MyDes.h"
#include "MyCrypto.h"
#include <time.h>

void to16(const char src,char dest[])//singal ans to 16
{
	char temp = src;
	dest[0] = (temp>>4)&0x0f ;
	if(dest[0]>9) dest[0] = dest[0] -10+'A';
	else dest[0] = dest[0] +'0';
	dest[1] = (temp & 0x0f) ;
	if(dest[1]>9) dest[1] = dest[1] -10 + 'A';
	else dest[1] =dest[1] + '0';
	//printf("%c to_16 is %c%c\n",src,dest[0],dest[1]);
}

void toansc(const char src[],char &desc)//singal 16 to ans
{
	char temp1 = src[0],temp2 = src[1];
	if(temp1 > '9') temp1 =temp1 -'A' + 10;
	else temp1 = temp1 -'0';
	if(temp2 > '9') temp2 = temp2 -'A'+10;
	else temp2 = temp2 -'0';
	desc = temp1 <<4 ;
	desc +=temp2;
}

void transfrom_to_16(const char *src,int len,char *desc,int &len2)//ans to 16
{
	int i;
	for(i = 0 ;i< len;i++) to16(src[i],desc+2*i);
	len2 = 2*len;
}

void transfrom_to_ans(const char *src,int len,char *desc,int &len2)//16 to ans
{
	int i;
	len2 = len/2;
	for(i = 0 ;i< len2;i++) toansc(src + i+i,desc[i]);
}

char * base_encrypt(const  char * plaintext,int len_plain,char key[8])//返回16进制,error 返回NULL
{
	int len_out = (len_plain/8+(len_plain%8!=0))*8;
	char *out = new char[len_out +1] ;

	if(out == NULL) return NULL;
	memset(out,0,len_out+1);
	memcpy(out,plaintext,len_plain);

	int len = len_out/8;
	Des_SetKey(key);
	for(int i = 0 ;i < len; i ++ )
		Des_Run(out + i * 8, out + i * 8, ENCRYPT);

	//cout<<"after base_encrypt : "<<out<<endl;
	int len_16 = len_out * 2 + 1;
	char *out_16 = new char[len_16];
	if(out_16 ==NULL) return NULL;

	memset(out_16,0,len_16);
	transfrom_to_16(out,len*8,out_16,len_16);

	//cout<<"after base_encrypt_16 : "<<out_16<<endl;
	free(out);
	return out_16;
}

char *base_decrypt(const char *ciphertext,char key[8])//if error:return NULL;else plaintext
{
	int len_ans = strlen(ciphertext)/2;
	char *plaintext = new char[len_ans+1];
	if(plaintext ==NULL) return NULL;
	memset(plaintext,0,len_ans+1);

	transfrom_to_ans(ciphertext,strlen(ciphertext),plaintext,len_ans);
	//cout<<"after crypt_ans : "<<plaintext<<endl;
	Des_SetKey(key);
	int len2 = len_ans / 8 ;
	for(int i = 0;i < len2; i++)
		Des_Run(plaintext + i *8, plaintext + i * 8, DECRYPT);
	//cout<<"after crypt: "<<plaintext<<endl;
	return plaintext;
}

void trans_key(const  long key,char c[8])
{
	memset(c,'F',8);
	for(int i = 0 ;i< 8;i++) c[i] = (key>>(i*8))&0xff;
}

void produce_key(char private_key[],char key[])//private_key[]变成64位整数->key[0..7]
{
	 long int_key = 0;
	int len = strlen(private_key);
	for(int i = 0 ;i< len;i++) int_key = int_key*10+private_key[i] - '0';
	//cout<<"private key = "<<private_key<<endl<<"int_key = "<<int_key<<endl;
	trans_key(int_key,key);
}

string encrypt(const char *plaintext,char private_key[])
{
	char key[9];
	memset(key,0,sizeof(key));
	produce_key(private_key,key);
	//cout<<"key = "<<key<<endl;

	char rand_key[]="12345678";
	srand(unsigned(time(0)));
	for(int i = 0 ;i< 8;i++) rand_key[i] = rand()%100+1;
	char *once_encrypt = base_encrypt(plaintext,strlen(plaintext),rand_key);
	int len_16 = strlen(once_encrypt);


	int len_s_2 = strlen(once_encrypt) + 8 + 1;
	char *s_2 = new char [len_s_2];
	memset(s_2,0,len_s_2);
	memcpy(s_2,once_encrypt,strlen(once_encrypt));
	memcpy(s_2 + strlen(once_encrypt),rand_key,8);

	free(once_encrypt);

	char *p = base_encrypt(s_2,strlen(s_2),key);
	free(s_2);
	string ret(p);
	free(p);
	//cout<<"ret = "<<ret<<endl;
	//cout<<"ret's c_str = "<<ret.c_str()<<endl;
	return ret;
}

string decrypt(const char *ciphertext,char private_key[])//private_key[]变成64位整数->key[0..7]
{
	char key[9];
	memset(key,0,sizeof(key));
	produce_key(private_key,key);

	char *once_decrypt = base_decrypt(ciphertext,key);
	int len = strlen(once_decrypt);

	char rand_key[9];
	memcpy(rand_key,once_decrypt + len -8,8);

	int len_s_2 = len - 8 +1;
	char *s_2 = new char[len_s_2];
	if(s_2 == NULL ) return NULL;
	memcpy(s_2,once_decrypt,len-8);
	s_2[len_s_2 -1 ] = 0;
	free(once_decrypt);


	char *p = base_decrypt(s_2,rand_key);
	free(s_2);

	string ret(p);
	free(p);

	return ret;
}
