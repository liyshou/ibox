#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "3DES.h"

#define IP_TABLE	0
#define IP_1_TABLE	1
#define E_TABLE		2
#define P_TABLE		3
#define PC1_TABLE	4
#define PC2_TABLE	5

typedef struct
{
	unsigned char L[32];
	unsigned char R[32];
}LRStruct;

typedef struct
{
	unsigned char C[28];
	unsigned char D[28];
}CDStruct;

typedef struct
{
#ifdef UX64
	unsigned char b7 :	1;
	unsigned char b6 :	1;
	unsigned char b5 :	1;
	unsigned char b4 :	1;
	unsigned char b3 :	1;
	unsigned char b2 :	1;
	unsigned char b1 :	1;
	unsigned char b0 :	1;
#else
	unsigned char b0 :	1;
	unsigned char b1 :	1;
	unsigned char b2 :	1;
	unsigned char b3 :	1;
	unsigned char b4 :	1;
	unsigned char b5 :	1;
	unsigned char b6 :	1;
	unsigned char b7 :	1;
#endif
}BYTEStruct;

int g_nOut[6] = {64,64,48,32,56,48};

int TABLE_shift[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

int TABLE_ip[64] = {
		57,49,41,33,25,17, 9, 1,59,51,43,35,27,19,11, 3,
		61,53,45,37,29,21,13, 5,63,55,47,39,31,23,15, 7,
		56,48,40,32,24,16, 8, 0,58,50,42,34,26,18,10, 2,
		60,52,44,36,28,20,12, 4,62,54,46,38,30,22,14, 6};
int TABLE_ip_1[64] = {
		39, 7,47,15,55,23,63,31,38, 6,46,14,54,22,62,30,
		37, 5,45,13,53,21,61,29,36, 4,44,12,52,20,60,28,
		35, 3,43,11,51,19,59,27,34, 2,42,10,50,18,58,26,
		33, 1,41, 9,49,17,57,25,32, 0,40, 8,48,16,56,24} ;
int TABLE_p[32] = {
		15, 6,19,20,28,11,27,16, 0,14,22,25, 4,17,30, 9,
		 1, 7,23,13,31,26, 2, 8,18,12,29, 5,21,10, 3,24};
int TABLE_pc_1[56] = {
		56,48,40,32,24,16, 8, 0,57,49,41,33,25,17, 9, 1,
		58,50,42,34,26,18,10, 2,59,51,43,35,62,54,46,38,
		30,22,14, 6,61,53,45,37,29,21,13, 5,60,52,44,36,
		28,20,12, 4,27,19,11, 3};
int TABLE_ePro[48] = {
		31, 4, 0, 1, 2, 3, 3, 8, 4, 5, 6, 7, 7,12, 8, 9,
		10,11,11,16,12,13,14,15,15,20,16,17,18,19,19,24,
		20,21,22,23,23,28,24,25,26,27,27, 0,28,29,30,31} ;
int TABLE_pc_2Pro[48] = {
		13, 4,16,10,23, 0, 2, 9,27,14, 5,20,22, 7,18,11,
		 3,25,15, 1, 6,26,19,12,40,54,51,30,36,46,29,47,
		39,50,44,32,43,52,48,38,55,33,45,31,41,49,35,28} ;
unsigned char TABLE_s[8][64] = 
{
		{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7, 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8, 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13},
		{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10, 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5, 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9},
		{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7, 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12},
		{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4, 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14},
		{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6, 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3},
		{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8, 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6, 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13},
		{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6, 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2, 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12},
		{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7, 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2, 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8, 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}
};

	void BitToByte(unsigned char * src, unsigned char * dst, int n);
	void ByteToBit(unsigned char * src, unsigned char * dst, int n);

	void DesAlgo(unsigned char *key, unsigned char *src, unsigned char *dst, unsigned char bEncrypt);

	void Shift(CDStruct *pCD, int iCircle, unsigned char bEncrypt);
	void RShift(unsigned char *buf28, int nShift);
	void LShift(unsigned char *buf28, int nShift);
	void fFunction(unsigned char *R, unsigned char*K, unsigned char *result);
	void S_Change(unsigned char *src, unsigned char *result);
	void SelectExchangeFromTable(int nTable, unsigned char *src, unsigned char *dst);


void BitToByte(unsigned char *src, unsigned char *dst, int n)
{
	int		i;
	BYTEStruct 	*BS;
	for (i=0; i<n; i++)
	{
		BS = (BYTEStruct *)(&src[i]);
		dst[i*8+7] = BS->b0;
		dst[i*8+6] = BS->b1;
		dst[i*8+5] = BS->b2;
		dst[i*8+4] = BS->b3;
		dst[i*8+3] = BS->b4;
		dst[i*8+2] = BS->b5;
		dst[i*8+1] = BS->b6;
		dst[i*8+0] = BS->b7;
	}	
}

void ByteToBit(unsigned char *src, unsigned char *dst, int n)
{
	int	i;
	BYTEStruct 	*BS;
	for (i=0; i<n; i++)
	{
		BS = (BYTEStruct *)(&dst[i]);
		BS->b0 = src[i*8+7];
		BS->b1 = src[i*8+6];
		BS->b2 = src[i*8+5];
		BS->b3 = src[i*8+4];
		BS->b4 = src[i*8+3];
		BS->b5 = src[i*8+2];
		BS->b6 = src[i*8+1];
		BS->b7 = src[i*8+0];
	}
}


int BitXor(unsigned char *Data1, unsigned char *Data2, int Len, unsigned char *Dest)
{
	int i;

	for (i = 0; i < Len; i++)	Dest[i] = Data1[i] ^ Data2[i];
	return 0;
}

void DesAlgo(unsigned char *key, unsigned char *src, unsigned char *dst, unsigned char bEncrypt)
{
	int  i, j, k;
	unsigned char input[64];
	unsigned char output[64];
	unsigned char KEY[64];
	unsigned char tempA[7];
	unsigned char tempB[64];
	unsigned char tempC[8];
	unsigned char tempD[32];
	unsigned char K[48];
	unsigned char fResult[32];
	
	LRStruct LR;
	CDStruct CD;

	BitToByte(src, input, 8);
	SelectExchangeFromTable(IP_TABLE,input,(unsigned char*)&LR);

	BitToByte(key, KEY, 8);
	SelectExchangeFromTable(PC1_TABLE,KEY,(unsigned char*)&CD);

	for (i=0; i<16; i++)
	{
		Shift(&CD, i, bEncrypt);
		ByteToBit((unsigned char*)&CD,tempA,7);
		SelectExchangeFromTable(PC2_TABLE,(unsigned char*)&CD, K);
		for(k=0; k<8; k++)
		{
			tempB[k*8+0]=0;
			tempB[k*8+1]=0;
			for (j=0; j<6; j++)	tempB[k*8+(j+2)]=K[k*6+j];
		}
		ByteToBit(tempA,tempC,8);
		fFunction(LR.R, K, fResult);
		BitXor (LR.L, fResult, 32, fResult);

		memcpy(LR.L, LR.R, sizeof(unsigned char)*32);
		memcpy(LR.R, fResult, sizeof(unsigned char)*32);
	}
	
	memcpy(tempD, LR.L,sizeof(unsigned char)*32);
	memcpy(LR.L, LR.R,sizeof(unsigned char)*32);
	memcpy(LR.R, tempD,sizeof(unsigned char)*32);

	SelectExchangeFromTable(IP_1_TABLE, (unsigned char*)&LR, output);
	ByteToBit(output, dst, 8);
}

 
void SelectExchangeFromTable(int nTable, unsigned char *src, unsigned char *dst)
{
	static int * table[6] = {TABLE_ip, TABLE_ip_1, TABLE_ePro, TABLE_p, TABLE_pc_1, TABLE_pc_2Pro};

	int i, nOut;
	int *pTable;
	
	nOut = g_nOut[nTable];
	pTable = table[nTable];
	
	for (i=0; i< nOut; i++)		dst[i] = src[pTable[i]];
}

void Shift(CDStruct *pCD, int iCircle, unsigned char bEncrypt)
{
	
	if ( 0 == bEncrypt)
	{
		if ( iCircle != 0)
		{
			RShift(pCD->C, TABLE_shift[iCircle]);
			RShift(pCD->D, TABLE_shift[iCircle]);
		}
	}
	else
	{
		LShift(pCD->C, TABLE_shift[iCircle]);
		LShift(pCD->D, TABLE_shift[iCircle]);
	}
}

void RShift(unsigned char *buf28, int nShift)
{
	int i;
	unsigned char temp[2];

	for (i=0; i<nShift; i++)		temp[(nShift-1)-i] = buf28[27-i];

	for (i=27; i>=nShift; i--)		buf28[i] = buf28[i-nShift];

	for (i=0; i<nShift; i++)		buf28[i] = temp[i];

}

void LShift(unsigned char *buf28, int nShift)
{
	int i;
	unsigned char temp[2];

	for (i=0; i<nShift; i++)		temp[i] = buf28[i];

	for (i=nShift; i<28; i++)		buf28[i-nShift] = buf28[i];

	for (i=0; i<nShift; i++)		buf28[27-(nShift-1)+i] = temp[i];

}

void fFunction(unsigned char *R, unsigned char *K, unsigned char *result)
{
	unsigned char temp[48];
	unsigned char SResult[32];
	
	SelectExchangeFromTable(E_TABLE, R, temp);

	BitXor(K,temp,48,temp);

	S_Change(temp, SResult);
/*	
	printf("\ntemp:");
	for (i=0;i<48;i++) printf("%X ",temp[i]);
	printf("\nResult:");
	for (i=0;i<32;i++) printf("%X ",SResult[i]);
	printf("\n");
*/
	SelectExchangeFromTable(P_TABLE, SResult, result);
}

void S_Change(unsigned char *src, unsigned char *result)
{
	unsigned char 	sTemp;
	unsigned char 	nPos;
	int 	i;
	BYTEStruct	*rBSPos;
	
	for (i=0; i<8; i++)
	{
		rBSPos = (BYTEStruct *)(&nPos) ;
		nPos = 0;

		rBSPos->b0 = src[i*6+5];
		rBSPos->b1 = src[i*6+4];
		rBSPos->b2 = src[i*6+3];
		rBSPos->b3 = src[i*6+2];
		rBSPos->b4 = src[i*6+1];
		rBSPos->b5 = src[i*6+0];

		if (i%2==0)	sTemp= TABLE_s[i][nPos]*16;
		else
		{
			sTemp += TABLE_s[i][nPos];
			BitToByte(&sTemp, &result[i/2*8], 1);
		}
	}
}

void EnDES(unsigned char *pbyKey, unsigned char *pbySrc, unsigned char *pbyDst)
{
	DesAlgo(pbyKey, pbySrc, pbyDst, 1);
}

void DeDES(unsigned char *pbyKey, unsigned char *pbySrc, unsigned char *pbyDst)
{
	DesAlgo(pbyKey, pbySrc, pbyDst, 0);
}

void En3DES(unsigned char *pbyKey, unsigned char *pbySrc, unsigned char *pbyDst)
{
	unsigned char LKey[8];
	unsigned char RKey[8];
	unsigned char TmpDest[8];

	memcpy(LKey,pbyKey,8);
	memcpy(RKey,pbyKey+8,8);

	EnDES(LKey,pbySrc,pbyDst);
	DeDES(RKey,pbyDst,TmpDest);
	EnDES(LKey,TmpDest,pbyDst);
}

void De3DES(unsigned char *pbyKey, unsigned char *pbySrc, unsigned char *pbyDst)
{
	unsigned char LKey[8];
	unsigned char RKey[8];
	unsigned char TmpDest[8];

	memcpy(LKey,pbyKey,8);
	memcpy(RKey,pbyKey+8,8);

	DeDES(LKey,pbySrc,pbyDst);
	EnDES(RKey,pbyDst,TmpDest);
	DeDES(LKey,TmpDest,pbyDst);
}


void MAC(unsigned char *pbyKey, unsigned char *pbySrc, int iSrcLen, unsigned char *pbyInit, unsigned char *pbyDst)
{
	unsigned char	pbySrcTemp[8];
	unsigned char	pbyInitData[8];
	unsigned char	pbyDeaSrc[8];
	int	i, j, n, iAppend;

	n = iSrcLen / 8 + 1;
	iAppend = 8 - ( n * 8 - iSrcLen);

	memcpy (pbyInitData,pbyInit,8);
	for(i = 0; i < n; i++)
	{
		memset(pbySrcTemp, 0x00, 8);
		if(i==(n-1))
		{
			memcpy(pbySrcTemp, &pbySrc[i*8], iAppend);
			pbySrcTemp[iAppend] = 0x80;
			for (j = iAppend + 1; j < 8; j++) pbySrcTemp[j] = 0x00;
		}
		else memcpy(pbySrcTemp, &pbySrc[i*8], 8);

		BitXor(pbySrcTemp, pbyInitData, 8, pbyDeaSrc);
		memset(pbyInitData, 0x00, 8);
		EnDES(pbyKey, pbyDeaSrc, pbyInitData);
	}
	memcpy(pbyDst, pbyInitData, 4);
}


void DES3MAC(unsigned char *pbyKey, unsigned char *pbySrc, int iSrcLen, unsigned char *pbyInit, unsigned char *pbyDst)
{
	unsigned char	pbySrcTemp[8];
	unsigned char	pbyInitData[8];
	unsigned char	pbyDeaSrc[8];
	int	i, j, n, iAppend;

	n = iSrcLen / 8;
	iAppend = 8 - ( n * 8 - iSrcLen);  

	memcpy (pbyInitData,pbyInit,8);
	for(i = 0; i < n; i++)
	{
		memset(pbySrcTemp, 0x00, 8);
		memcpy(pbySrcTemp, &pbySrc[i*8], 8);
		BitXor(pbySrcTemp, pbyInitData, 8, pbyDeaSrc);
		memset(pbyInitData, 0x00, 8);
		EnDES(pbyKey, pbyDeaSrc, pbyInitData);
	}  

	memset(pbyDst,0,sizeof(pbyDst));
	memcpy(pbyDst, pbyInitData, 4); 
}


void MAC16(unsigned char *pbyKey, unsigned char *pbySrc, int iSrcLen, unsigned char *pbyInit, unsigned char *pbyDst)
{
	unsigned char	pbySrcTemp[8+1];
	unsigned char	pbyInitData[8+1];
	unsigned char	pbyDeaSrc[8+1];
	unsigned char	KeyA[8+1];
	unsigned char	KeyB[8+1];
	int	i, j, n, iAppend;

	n = iSrcLen / 8 + 1;
	iAppend = 8 - ( n * 8 - iSrcLen);

	memset(pbyInitData,0x00,8);
	memset(KeyA,0x00,8);
	memset(KeyB,0x00,8);

	memcpy (pbyInitData,pbyInit,8);
	memcpy (KeyA,pbyKey,8);
	memcpy (KeyB,&pbyKey[8],8);

	for(i = 0; i < n; i++)
	{
		memset(pbySrcTemp, 0x00, 8);
		if(i==(n-1))
		{
			memcpy(pbySrcTemp, &pbySrc[i*8], iAppend);
			pbySrcTemp[iAppend] = 0x80;
			for (j = iAppend + 1; j < 8; j++) pbySrcTemp[j] = 0x00;
		}
		else memcpy(pbySrcTemp, &pbySrc[i*8], 8);

		BitXor(pbySrcTemp, pbyInitData, 8, pbyDeaSrc);
		memset(pbyInitData, 0x00, 8);
		EnDES(KeyA, pbyDeaSrc, pbyInitData);
	}

	DeDES(KeyB, pbyInitData, pbyDeaSrc);
	EnDES(KeyA, pbyDeaSrc, pbyInitData);

	memset(pbyDst,0x00,4);
	memcpy(pbyDst, pbyInitData, 4);

}



void DefuseKey(unsigned char *key, unsigned char *factor, unsigned char *DefusedKey)
{

	unsigned char xornum[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	unsigned char XorFactor[8];
	
	En3DES(key,factor,DefusedKey);
	
	BitXor(factor,xornum,8,XorFactor);

	En3DES(key,XorFactor,DefusedKey+8);
}

/*
main()
{
	unsigned char Key[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	unsigned char In[8] =  {0x22,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
	unsigned char Out[8];	
	EnDES(Key, In, Out);
	
	printf("%02x%02x%02x%02x%02x%02x%02x%02x\n",Out[0],Out[1],Out[2],Out[3],Out[4],Out[5],Out[6],Out[7]);
		
}
*/
