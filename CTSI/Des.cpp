// Des.cpp: implementation of the CDes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Def.h"
#include "Des.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDes::CDes()
{

}

CDes::~CDes()
{

}

const char CDes::IP_Tr[64]={58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
const char CDes::IP_1_Tr[64]={40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};
const char CDes::Etr[48]={32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
const char CDes::PC_1_Tr[56]={57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
const char CDes::PC_2_Tr[48]={14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
const char CDes::SB[8][64]={{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}};
const char CDes::Left_move[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
const char CDes::right_move[16]={0,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};       
const char CDes::swap[64]={33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
const char CDes::P_Tr[32]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};

void CDes::bin_to_hex_char(char *in_buff, char *out_buff, int bin_len)
{
        int i;
        char hex_char[10], temp_hex_char[3];

        *out_buff = '\0';
        for(i=0; i<bin_len; i++) {
                memset(hex_char, 0, sizeof(hex_char));
                memset(temp_hex_char, 0, sizeof(temp_hex_char));
                sprintf(hex_char,"%x",*(in_buff+i));

                if(strlen(hex_char)==(size_t) 2) {
                        strcat(out_buff,hex_char);
                }

                if(strlen(hex_char)==(size_t) 1) {
                        strncat(out_buff,"0",1);
                        strcat(out_buff,hex_char);
                }

                if(strlen(hex_char)>2) {
                        temp_hex_char[0]=hex_char[6];
                        temp_hex_char[1]=hex_char[7];
                        strncat(out_buff,temp_hex_char,2);
                }
        }
        return;
}

/******************************  tobyte  *******************************
 * Description : Convert a string into hex value (e.g. from "abcd12" of
 *                   6 bytes to 0xabcd12 in 3 bytes), result buffer will
 *                   be terminated with binary zero
 * Parameter   : input ASCII string
 *               output buffer
 * Return      : number of bytes output to the buffer
 *
 */
int CDes::tobyte (char *instr, char *outstr)
{
  register int           i, j;
  register char          ch;

  for (i = j = 0; instr[i]; i++)  {
    ch = instr[i];
    if (ch >= '0' && ch <= '9')
      ch -= '0';
    else
      if (ch >= 'a' && ch <= 'f')
        ch = ch - 'a' + 10;
      else
        if (ch >= 'A' && ch <= 'F')
          ch = ch - 'A' + 10;
        else
          ch = 0;

    if (i & 1)
      outstr[j++] += ch;
    else
      outstr[j] = ch << 4;
  }
  outstr[j] = 0;

  return (j);
}

void CDes::str_copy (char *s, char *t, char n) 
{
  while (n-- != 0)
    *(s++) = *(t++);
}


void CDes::outtostr(char *encrypted_pin,char *outstr)
{
//input:   encrypted_pin  --64位加密数据
//output:  outstr         --16位ASCII码字符

//将64位加密数据（二进制）转换成16位ASCII码字符
//依次将每4个比特用16进制以一个ASCII码字符表示
//如：  1010...   --> 10 --> 'A'

	int i, hh,j,vv;
	for (i=0;i<=15;i++){
		vv=0;
		for (j=0,hh=8;j<4;j++,hh=hh/2) {
			vv+=encrypted_pin[i*4+j]*hh;
		}
		if (vv<10)
			outstr[i]=vv+48; //'0'--'9'
		else
			outstr[i]=vv+55; //'A'--
	}
	outstr[16]=0;
}


void CDes::myouttostr(char *encrypted_pin,char *outstr)//2003.7.19
{
//input:   encrypted_pin  --64位加密数据
//output:  outstr         --8位ASCII码字符

//将64位加密数据（二进制）转换成16位ASCII码字符
//依次将每8个比特用16进制以一个ASCII码字符表示
//如：  1010...   --> 10 --> 'A'

	int i, hh,j,vv;
	for (i=0;i<8;i++){
		vv=0;
		for (j=0,hh=128;j<8;j++,hh=hh/2) {
			vv+=encrypted_pin[i*8+j]*hh;
		}
		if (vv<10)
			outstr[i]=vv+48; //'0'--'9'
		else
			outstr[i]=vv+55; //'A'--
	}
	outstr[8]=0;
}


void CDes::inputtobin(char *inputstr,char *outbin)
{
//input:   inputstr(ASCII码字符)    长度：16
//output:  outbin  (2进制数)        长度：64
//将ASCII码字符按16进制数转换成二进制数
//如：  '1' (ascii:49) --> 1  -->  0001
//	    'a' (ascii:97) --> 10 -->  1010
//      'A' (ascii:65) --> 10 -->  1010 
	int i,j,k,m;
    
	for (i=0;i<=15;i++) {
		if (inputstr[i]<=57 &&inputstr[i]>=48) //'0'--'9'
			k=inputstr[i]-48;
//		else if (inputstr[i]<=70 &&inputstr[i]>=65) //'A'--'F'
		else if (inputstr[i]<=90 &&inputstr[i]>=65) //'A'--'Z'
			k=inputstr[i]-55;
//		else if (inputstr[i]<=102 &&inputstr[i]>=97) //'a'--'f'
		else if (inputstr[i]<=122 &&inputstr[i]>=97) //'a'--'z'
			k=inputstr[i]-87;
		for (j=0,m=8;j<4;j++,m=m/2)
			if (((k/m)%2)==1)
				outbin[i*4+j]=1;
			else outbin[i*4+j]=0;
	}
}

void CDes::myinputtobin(char *inputstr,char *outbin)
{
//INPUT:  inputstr  --ascii字符  长度：8
//OUTPUT: outbin    --二进制数   长度：64
//将8数字表示为64个比特的数组
//用其ASCII码（如1的asc码为49=0x31）的二进制数值（如49= 0011,0001(顺序高到低)，用数组表示
//	从低到高 即 1000，1100 即outbin[0]=1,outbin[1]=0... ）
//如   '1' --> 49 --> 0011,0001 --> 10001100( array[0]=1,array[1]=0 )


	int i,j;
	
	int len;
    len= strlen(inputstr);
	for (i = 0;i < len;i++)
	{
		for(j = 0;j <= 7; j++)
		  outbin[i*8+(7-j)] = (inputstr[i] >> j) & 0x01;
	}

}

void CDes::leftmove(char *key)
{
	char x[64],i;

	str_copy((char *)x,(char *)key,64);
	for (i=0;i<55;i++)
		x[i]=x[i+1];
	x[27]=key[0];
	x[55]=key[28];
	str_copy((char *)key,(char *)x,64);
}

void CDes::rightmove(char *key)
{
	char x[64],i;

	str_copy((char *)x,(char *)key,64);
	for(i=0;i<55;i++)
		 x[55-i]=x[54-i];
	x[0] = key[27];
	x[28] =  key[55];
	str_copy((char *)key,(char *)x,64);

}

void CDes::transpose(char *ddata,const char *matrix,char n)
{
	char tem_matrix[64], i;
	str_copy((char *)tem_matrix,(char *)ddata,64);
	for (i=1;i<=n;i++)
		ddata[i-1]=tem_matrix[matrix[i-1]-1];
}


void CDes::des_f(char i,char *ri_1,char *key,char *r_f,char mode)
{
	char j, k, tmp,tem[64],tem_data[64],tem_key[64];

	str_copy((char *)tem_data,(char *)ri_1,64);
	transpose(tem_data,Etr,48);
	if(mode == 1)
	  {
		 for (j=1;j<=Left_move[i];j++)
			 leftmove(key);
	  }
	else
	  {
		 for (j=1;j<=right_move[i];j++)
			  rightmove(key);
	  }


	str_copy((char *)tem_key,(char *)key,64);
	transpose(tem_key,PC_2_Tr,48);
	for (j=0;j<48;j++)
		if (tem_data[j]+tem_key[j]==1)
			tem[j]=1;
		else
			tem[j]=0;
	for(j=1;j<=8;j++) {
		k=32*tem[6*j-6]+16*tem[6*j-1]+8*tem[6*j-5]+4*tem[6*j-4]+2*tem[6*j-3]+tem[6*j-2];
		tmp=SB[j-1][k];
		if (((tmp/8)%2)==1)
			r_f[4*j-3-1]=1;
		else
			r_f[4*j-3-1]=0;
		if (((tmp/4)%2)==1)
			r_f[4*j-2-1]=1;
		else
			r_f[4*j-2-1]=0;
		if (((tmp/2)%2)==1)
			r_f[4*j-1-1]=1;
		else
			r_f[4*j-1-1]=0;
		if ((tmp%2)==1)
			r_f[4*j-1]=1;
		else
			r_f[4*j-1]=0;
	}
	transpose(r_f,P_Tr,32);
}

void CDes::des (char *plaintext, char *key, char *ciphertext,char mode)
{
	char i,j,txx[64],ta[64],tb[64],place_key[64];//  /*,a[64],b[64],x[64]*;
	str_copy((char *)ta,(char *)plaintext, 64);
	transpose (ta, IP_Tr, 64);
    str_copy(place_key,key,64);
	transpose (place_key, PC_1_Tr, 56);
	for (i=0; i<16; i++)
	{ 
//		WDR=0XFF;
		str_copy((char *)tb,(char *)ta,64);
		for (j=0; j<32; j++)
			ta[j] = tb[j+32];
		des_f(i,ta,place_key,txx,mode);
//        add_digit(8);
		for (j=0; j<32; j++)
		  {
				if((tb[j]+txx[j]) == 1)
				  ta[j+32] = 1;
				else
				  ta[j+32] = 0;
		  }
	 }
	transpose (ta, swap, 64);
	transpose (ta, IP_1_Tr, 64);
	str_copy((char *)ciphertext,(char *)ta,64);
}

char CDes::CharToHex(char chr)              // '0'----0,'a'----10
{
	if(chr>='0' && chr<='9')
		return chr-'0';

	if (chr>='A' && chr<='F')
		return chr-'A'+10;

	if (chr >= 'a' && chr<='f')
		return chr-'a'+10;

	return -1;
}


//////////////////////////////////////////////////////////////////

//加密
void CDes::GetDesOutput(char *data,char *key,char *output)
{
	//INPUT :  data:    16个字符密码 "3132333435363738"
	//         key:     16个字符密钥 
	//OUTPUT: output:   16个字符密码

    //如 data="3132333435363738"
	//用inputtobin生成的datamatrix
    //与 data="12345678"
	//用myinputtobin生成的 datamatrix一样

	
	//CString str,str1;
	char datamatrix[64],keymatrix[64],outputmatrix[64];

	inputtobin(data,datamatrix);

    inputtobin(key,keymatrix);//2001.9.6

	des(datamatrix,keymatrix,outputmatrix,1);

	// convert integer matrix to char data
	outtostr( outputmatrix, output); //2001.9.6

}

//解密
void CDes::GetDesInput(char *data,char *key,char *output)
{
	//INPUT :  data:    16个字符密码 "3132333435363738"
	//         key:     16个字符密钥 
	//OUTPUT: output:   16个字符密码

	//CString str,str1;
	char datamatrix[64],keymatrix[64],outputmatrix[64];
	inputtobin(data,datamatrix);
    //如 data="3132333435363738"
	//用inputtobin生成的datamatrix
    //与 data="12345678"
	//用myinputtobin生成的 datamatrix一样

    inputtobin(key,keymatrix);//2001.9.6

	des(datamatrix,keymatrix,outputmatrix,0);

	// convert integer matrix to char data
	outtostr( outputmatrix, output); //2001.9.6

}

void CDes::CharToAsc(char *inputstr,char *outstr)
{
	int i,len;
	len=strlen(inputstr);
	strcpy(outstr,"");
	for(i=0;i<8-len;i++)
		strcat(outstr,"00");
	for(i=0;i<len;i++)
	{
		switch(inputstr[i])
		{
		case '0':
			strcat(outstr,"30");
			break;
		case '1':
			strcat(outstr,"31");
			break;
		case '2':
			strcat(outstr,"32");
			break;
		case '3':
			strcat(outstr,"33");
			break;
		case '4':
			strcat(outstr,"34");
			break;
		case '5':
			strcat(outstr,"35");
			break;
		case '6':
			strcat(outstr,"36");
			break;
		}
	}
}

void CDes::MakeKey(char *inputstr,char *outstr)
{
	int i,len;
	char xor;
	len=strlen(inputstr);
	strcpy(outstr,"00");
	for(i=1;i<7-len;i++){
		strcat(outstr,"00");
	}
	xor=0;
	for(i=0;i<len;i++)
	{
		switch(inputstr[i])
		{
		case '0':
			strcat(outstr,"30");
			xor=xor^0;
			break;
		case '1':
			strcat(outstr,"31");
			xor=xor^1;
			break;
		case '2':
			strcat(outstr,"32");
			xor=xor^2;
			break;
		case '3':
			strcat(outstr,"33");
			xor=xor^3;
			break;
		case '4':
			strcat(outstr,"34");
			xor=xor^4;
			break;
		case '5':
			strcat(outstr,"35");
			xor=xor^5;
			break;
		case '6':
			strcat(outstr,"36");
			xor=xor^6;
			break;
		case '7':
			strcat(outstr,"37");
			xor=xor^7;
			break;
		case '8':
			strcat(outstr,"38");
			xor=xor^8;
			break;
		case '9':
			strcat(outstr,"39");
			xor=xor^9;
			break;
		}
	}
	sprintf(outstr,"%s%02x",outstr,xor);
}



void CDes::MakePass(char *DbPass,char *CardNo,char *outstr)
{
	int i,len1,len2;
	char temp[9];
	len1=strlen(DbPass);
	len2=strlen(CardNo);
	strcpy(temp,DbPass);
	strncat(temp,&CardNo[len2-8+len1],8-len1);
	temp[8]=0;
	outstr[0]=0;
	for(i=0;i<8;i++)
	{
		switch(temp[i])
		{
		case '0':
			strcat(outstr,"30");
			break;
		case '1':
			strcat(outstr,"31");
			break;
		case '2':
			strcat(outstr,"32");
			break;
		case '3':
			strcat(outstr,"33");
			break;
		case '4':
			strcat(outstr,"34");
			break;
		case '5':
			strcat(outstr,"35");
			break;
		case '6':
			strcat(outstr,"36");
			break;
		case '7':
			strcat(outstr,"37");
			break;
		case '8':
			strcat(outstr,"38");
			break;
		case '9':
			strcat(outstr,"39");
			break;
		}
	}
}


int CDes::CheckPass(char *DbPass,char *CardNo,char *key,char *UpPass,char EncryptedPass[9])
{
	char OriPass[17],k[17];
	char Temp[17];

	char HighBit[9],LowBit[9];
	int hh,i,len,len1;
	unsigned long cp1,cp2,cp;


	MakeKey(key,k);

	MakePass(DbPass,CardNo,OriPass);

	GetDesOutput(OriPass,k,Temp);


	strncpy(HighBit,Temp,8);
	HighBit[8]=0;
	strncpy(LowBit,&Temp[8],8);
	LowBit[8]=0;
	

	cp1=0;

	for(i=0;i<8;i++)
	{
		cp=CharToHex(HighBit[i]);
		for(hh=0;hh<7-i;hh++)
			cp*=16;
		cp1+=cp;
	}

	
	cp2=0;

	for(i=0;i<8;i++)
	{
		cp=CharToHex(LowBit[i]);
		for(hh=0;hh<7-i;hh++)
			cp*=16;
		cp2+=cp;
	}

	sprintf(Temp,"%u",cp1^cp2);

	len=strlen(Temp);
	len1=strlen(UpPass);
	strncpy(EncryptedPass,&Temp[len-len1],len1);
	EncryptedPass[len1]=0;

	if (strcmp(EncryptedPass,UpPass)==0)
		return 0;
	else
		return 1;
}


int CDes::AsciiToBinary(char *binarystr ,char *asciistr)
{
//INPUT:   ascciistr   12个字符
//OUTPUT:  binarystr   6个字符
//12个字符-->每个数字按16进制表示成4bit的特征值-->两个数字合成一个字节
//-->6个字符

	int c=0,c1,c2=0;
	char chr,temp1,temp3;

	c1=strlen(asciistr);
	if (c1<=0)
		// if asciistr is an illegal string ,return positive value
		// if asciistr is a null string , return 0;
		return c1;

	*binarystr=0;
	while(c2<c1)
		{
		temp1=*(asciistr+c2);
		if((temp3=CharToHex(temp1))<0)
			// temp1 isn't between '0'-'F' or '0'-'f'
			return -1;
		chr=(temp3<<4)&0xf0;
		c2++;  // move pointer
		temp1= *(asciistr+c2);
		if((temp3=CharToHex(temp1))<0)
			// temp1 isn't between '0'-'F' or '0'-'f'
			return -1;
		chr|=temp3&0x0f;
//		strcat(binarystr,str);
		*(binarystr+c)=chr;
		c2++; // move pointer
		c++;
		} // while

		return c;
}



//3des加密; 要求密钥和数据必须是16的倍数

int CDes::TripleDesEncrypt(char *key, char *data, char *encstr)
{
	char key1[16+1];
	char key2[16+1];
	char key3[16+1];
	char tempBuff1[16+1];
	char tempBuff2[16+1];
	bbzero(key1);
	bbzero(key2);
	bbzero(key3);
	bbzero(tempBuff1);
	bbzero(tempBuff2);
	int data_len = strlen(data);

	if(data_len%16 != 0 || strlen(key)<16)

	{  
	
	      return -1; 
	}
	
	memcpy(key1, key, 16);
	memcpy(key2, key+16, 16);
	if(strlen(key)>=48)
		memcpy(key3,key+32,16);
	else
		memcpy(key3,key,16);
	
	for(int i = 0; i<(data_len/16); i++)
	{
		GetDesOutput(data+i*16,key1,tempBuff1);
		GetDesInput(tempBuff1,key2,tempBuff2);
		GetDesOutput(tempBuff2,key3,encstr+i*16);
		bbzero(tempBuff1);
		bbzero(tempBuff2);
	}

	return 0;
}


int CDes::TripleDesDec(char *key, char *data, char *encstr)
{
	char key1[16+1];
	char key2[16+1];
	char key3[16+1];
	char tempBuff1[16+1];
	char tempBuff2[16+1];
	bbzero(key1);
	bbzero(key2);
	bbzero(key3);
	bbzero(tempBuff1);
	bbzero(tempBuff2);
	int data_len = strlen(data);
	
	if(data_len%16 != 0 || strlen(key)<16)
		return -1;
	
	memcpy(key1, key, 16);
	memcpy(key2, key+16, 16);
	if(strlen(key)>=48)
		memcpy(key3,key+32,16);
	else
		memcpy(key3,key,16);
	
	for(int i = 0; i<(data_len/16); i++)
	{
		GetDesInput(data+i*16,key1,tempBuff1);
		GetDesOutput(tempBuff1,key2,tempBuff2);
		GetDesInput(tempBuff2,key3,encstr+i*16);
		bbzero(tempBuff1);
		bbzero(tempBuff2);
	}
	
	return 0;
}


int CDes::TripleDesDecrypt(char *key, char *data, char *encstr)
{
    char key1[16+1];
    char key2[16+1];
    char key3[16+1];
    char tempBuff1[16+1];
    char tempBuff2[16+1];
    bbzero(key1);
    bbzero(key2);
    bbzero(key3);
    bbzero(tempBuff1);
    bbzero(tempBuff2);
    int data_len = strlen(data);
    
    if(data_len%16 != 0 || strlen(key)<16)
        return -1;
    
    memcpy(key1, key, 16);
    memcpy(key2, key+16, 16);
    if(strlen(key)>=48)
        memcpy(key3,key+32,16);
    else
        memcpy(key3,key,16);
    
    for(int i = 0; i<(data_len/16); i++)
    {
        GetDesInput(data+i*16,key3,tempBuff1);
        GetDesOutput(tempBuff1,key2,tempBuff2);
        GetDesInput(tempBuff2,key1,encstr+i*16);
        bbzero(tempBuff1);
        bbzero(tempBuff2);
    }
    
    return 0;
}

void CDes::makeMac(char *data, int len, char *key, char *mac)
{
	char hkey[32+1];
	unsigned char in[32+1], out[32+1];
	memset(hkey, '0', sizeof(hkey));
	memset(in, '0', sizeof(in));
	memset(out, '0', sizeof(out));
	memset(mac, 0, 8);
	for(int i=0; i<len; i++)//相当于后补0形式
	{
		mac[i&7] = mac[i&7]^data[i];
	}
	CTools::byte_to_hex((BYTE*)key,(BYTE*)hkey,16);
	CTools::byte_to_hex((BYTE*)mac,(BYTE*)in,8);	
	
	TripleDesEncrypt((char*)hkey,(char*)in,(char*)out);
	
	CTools::hex_to_byte((BYTE*)out, (BYTE*)mac, 16, 0);
}

inline char _base64_Encode_GetChar(BYTE num)
{
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="[num];
}

//编码
size_t CDes::base64(char *pDest, const char *pSrc, size_t srclen)
{
    BYTE input[3], output[4];
    size_t i, index_src = 0, index_dest = 0;
    for(i = 0; i < srclen; i += 3)
    {
        //char [0]
        input[0] = pSrc[index_src++];
        output[0] = (BYTE)(input[0] >> 2);
        pDest[index_dest++] = _base64_Encode_GetChar(output[0]);
        
        //char [1]
        if(index_src < srclen)
        {
            input[1] = pSrc[index_src++];
            output[1] = (BYTE)(((input[0] & 0x03) << 4) + (input[1] >> 4));
            pDest[index_dest++] = _base64_Encode_GetChar(output[1]);
        }
        else
        {
            output[1] = (BYTE)((input[0] & 0x03) << 4);
            pDest[index_dest++] = _base64_Encode_GetChar(output[1]);
            pDest[index_dest++] = '=';
            pDest[index_dest++] = '=';
            break;
        }
        
        //char [2]
        if(index_src < srclen)
        {
            input[2] = pSrc[index_src++];
            output[2] = (BYTE)(((input[1] & 0x0f) << 2) + (input[2] >> 6));
            pDest[index_dest++] = _base64_Encode_GetChar(output[2]);
        }
        else
        {
            output[2] = (BYTE)((input[1] & 0x0f) << 2);
            pDest[index_dest++] = _base64_Encode_GetChar(output[2]);
            pDest[index_dest++] = '=';
            break;
        }
        
        //char [3]
        output[3] = (BYTE)(input[2] & 0x3f);
        pDest[index_dest++] = _base64_Encode_GetChar(output[3]);
    }
    //null-terminator
    pDest[index_dest] = 0;
    return index_dest;
}

