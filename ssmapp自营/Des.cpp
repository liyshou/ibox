/*
                                                                    
    Module: DES.C
                                                                    
    Implemetation of DES Algorithm.
                                                                    
    Includes: Listed below

    Compile options: /AL /W3 /G2 /Gs /Zl /Ox /c
                      |   |   |   |   |   |  |
                      |   |   |   |   |   |  +-- Compile only
                      |   |   |   |   |   +----- Maximal optisation
                      |   |   |   |   +--------- Remove library info
                      |   |   |   +------------- No Stack checking
                      |   |   +----------------- 286 Mode
                      |   +--------------------- Warning level 3
                      +------------------------- Large model
    Compiler used: C 5.1
                                 
*/
//#include <string.h>
#include "StdAfx.h"
#include "des.h"

/*
    Internal defines
*/
void DesEncrypt(
        unsigned char auchOutput[8],
        unsigned char auchInput[8],
        unsigned char auchKey[8])
{
    DesDes(auchOutput, auchInput, auchKey, DES_ENCRYPT);
} /* end DesEncrypt() */

void DesDecrypt(
        unsigned char auchOutput[8],
        unsigned char auchInput[8],
        unsigned char auchKey[8])
{
    DesDes(auchOutput, auchInput, auchKey, DES_DECRYPT);
} /* end DesDecrypt() */

void DesDes(
        unsigned char auchOutput[8],
        unsigned char auchInput[8],
        unsigned char auchKey[8],
        short sMethod)
{
    static BYTE InitPermut[64] =
    {
        58, 50, 42, 34, 26, 18, 10,  2, 60, 52, 44, 36, 28, 20, 12,  4,
        62, 54, 46, 38, 30, 22, 14,  6, 64, 56, 48, 40, 32, 24, 16,  8,
        57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
        61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7
    };

    static BYTE FinPermut[64] =
    {
        8, 40, 16, 48, 24, 56, 32, 64,  7, 39, 15, 47, 23, 55, 31, 63,
        6, 38, 14, 46, 22, 54, 30, 62,  5, 37, 13, 45, 21, 53, 29, 61,
        4, 36, 12, 44, 20, 52, 28, 60,  3, 35, 11, 43, 19, 51, 27, 59,
        2, 34, 10, 42, 18, 50, 26, 58,  1, 33,  9, 41, 17, 49, 25, 57
    };

    static BYTE KeyPermut[56] =
    {
        57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18, 10,  2,
        59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36, 63, 55, 47, 39,
        31, 23, 15,  7, 62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37,
        29, 21, 13,  5, 28, 20, 12,  4
    };

    static BYTE TmpKeyPermut[48] =
    {
        14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10, 23, 19, 12,  4,
        26,  8, 16,  7, 27, 20, 13,  2, 41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
    };

    static BYTE Tmp1Permut[48] =
    {
        32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,  8,  9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
    };

    static BYTE Tmp2Permut[32] =
    {
        16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
         2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25
    };

    static BYTE NumRotations[2][16] =
    {
        {
            1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
        },
        {
            0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
        }
    };

    BYTE KeyBlock[64];
    BYTE WorkBlock[64];
    BYTE TmpKeyBlock[64];
    BYTE TmpWorkBlock[32];
    BYTE TmpBlock[64];
    BYTE HlpBlock[48];
    register short sCount;
    register short ii;

    /*
        Extract binary representation of input data (8 bytes -> 64 bits)
    */
    DesHexToBlock(WorkBlock, auchInput);
    /*
        Extract binary representation of key (8 bytes -> 64 bits)
    */
    DesHexToBlock(KeyBlock, auchKey);

    DesPermut(WorkBlock, InitPermut, 64);
    DesPermut(KeyBlock, KeyPermut, 56);

    for (sCount = 0 ; sCount < 16 ; sCount++)
    {
        memcpy(TmpWorkBlock, WorkBlock, sizeof(TmpWorkBlock));
        memcpy(WorkBlock, &WorkBlock[32], 32 * sizeof(BYTE));

        for (ii = NumRotations[sMethod][sCount] ; ii > 0 ; ii--)
        {
            DesRotate(KeyBlock, sMethod);
        }

        memcpy(TmpBlock, WorkBlock, sizeof(TmpBlock));
        DesPermut(TmpBlock, Tmp1Permut, 48);

        memcpy(TmpKeyBlock, KeyBlock, sizeof(TmpKeyBlock));
        DesPermut(TmpKeyBlock, TmpKeyPermut, 48);

        for (ii = 0 ; ii < 48 ; ii++)
        {
            HlpBlock[ii] = TmpBlock[ii] ^ TmpKeyBlock[ii];
        }

        DesConvert(TmpBlock, HlpBlock);

        DesPermut(TmpBlock, Tmp2Permut, 32);

        for (ii = 0 ; ii < 32 ; ii++)
        {
            WorkBlock[ii + 32] = TmpWorkBlock[ii] ^ TmpBlock[ii];
        }
    } /* end for */

    DesPermut(WorkBlock, FinPermut, 64);
    /*
        Build output data (64 bits -> 8 bytes)
    */
    BYTEToHex(auchOutput, WorkBlock);
} /* end DesDes() */

void DesPermut(
        BYTE OutData[],
        BYTE PermutTable[],
        short sNumEntries)
{
    BYTE TmpBlock[64 + 1];
    register short ii;

    memcpy(&TmpBlock[1], OutData, 64 * sizeof(BYTE));

    for (ii = 0 ; ii < sNumEntries ; ii++)
    {
        OutData[ii] = TmpBlock[PermutTable[ii]];
    }
} /* end DesPermut () */

void DesRotate(
        BYTE OutBlock[64],
        short sMethod)
{
    BYTE TmpBlock[64];

    memcpy(TmpBlock, OutBlock, sizeof(TmpBlock));

    if (sMethod == DES_ENCRYPT)
    {
        memmove(TmpBlock, &TmpBlock[1], 55 * sizeof(BYTE));

        TmpBlock[27] = OutBlock[0];
        TmpBlock[55] = OutBlock[28];
    } /* end if */
    else
    {
        memmove(&TmpBlock[1], TmpBlock, 55 * sizeof(BYTE));

        TmpBlock[0]  = OutBlock[27];
        TmpBlock[28] = OutBlock[55];
    } /* end else */

    memcpy(OutBlock, TmpBlock, 64 * sizeof(BYTE));
} /* end DesRotate() */

void DesConvert(
        BYTE OutBlock[32],
        BYTE InBlock[48])
{
    static BYTE ConvBits[8][64] =
    {
        {
            14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
             0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
             4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
            15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
        },
        {
            15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
             3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
             0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
            13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
        },
        {
            10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
            13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
            13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
             1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
        },
        {
             7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
            13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
            10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
            03, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
        },
        {
             2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
            14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
             4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
            11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
        },
        {
            12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
            10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
             9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
             4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
        },
        {
             4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
            13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
             1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
             6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
        },
        {
            13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
             1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
             7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
             2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
        },
    };
    register short jj;
    register short ii;

    for (ii = 0 ; ii < 8 ; ii++)
    {
        jj = 0x20 * InBlock[0] + 0x10 * InBlock[5] + 0x08 * InBlock[1] +
                        0x04 * InBlock[2] + 0x02 * InBlock[3] + InBlock[4];
        InBlock += 6;

        jj = ConvBits[ii][jj];

        OutBlock[ii * 4 + 3] = (BYTE) jj & 1;
        jj = jj >> 1;
        OutBlock[ii * 4 + 2] = (BYTE) jj & 1;
        jj = jj >> 1;
        OutBlock[ii * 4 + 1] = (BYTE) jj & 1;
        jj = jj >> 1;
        OutBlock[ii * 4] = (BYTE) jj & 1;
    } /* end for */
} /* end DesConvert() */

void DesHexToBlock(
        BYTE OutBlock[64],
        unsigned char auchInHex[8])
{
    register unsigned char uchByte;
    register short ii;

    memset(OutBlock, 0, 64 * sizeof(BYTE));
    for (ii = 0 ; ii < 8 ; ii++)
    {
        uchByte = auchInHex[ii];

        if (uchByte & 0x80)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x40)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x20)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x10)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x08)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x04)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x02)
        {
            *OutBlock = 1;
        }
        OutBlock++;
        if (uchByte & 0x01)
        {
            *OutBlock = 1;
        }
        OutBlock++;
    } /* end for */
} /* end DesHexToBlock() */

void BYTEToHex(
        unsigned char auchOutHex[8],
        BYTE InBlock[64])
{
    register short ii;

    for (ii = 0; ii < 8; ++ii)
    {
        auchOutHex[ii] = (unsigned char) 0x80 * InBlock[0] +
                                            0x40 * InBlock[1] +
                                            0x20 * InBlock[2] +
                                            0x10 * InBlock[3] +
                                            0x08 * InBlock[4] +
                                            0x04 * InBlock[5] +
                                            0x02 * InBlock[6] +
                                            InBlock[7];
        InBlock += 8;
    } /* end for */
} /* end BYTEToHex() */

void ASCII2BCD(BYTE *auchOutput,char *szInput,int iInputLen)
{
	BYTE b[4];
	for(int i=0;i<iInputLen/2;i++)
	{
		sscanf(szInput+i*2,"%02X",b);
		auchOutput[i]=b[0];
	}
}

void BCD2ASCII(char *szOutput,BYTE *auchInput,int iInputlen)
{
	for(int i=0;i<iInputlen;i++)
	{
		sprintf(szOutput+i*2,"%02X",auchInput[i]);
	}
}

void MAC(BYTE *auchOutput,BYTE *auchInput,int iInputLen,BYTE *auchKey,int iKeyLen)
{
	BYTE auchTemp1[8]={0};//des输入数据
	BYTE auchTemp2[8]={0};//des输出数据
	BYTE auchTemp3[8]={0};//后8位异或数据
	int n=0;
	memcpy(auchTemp1,auchInput,min(8,iInputLen));
	while(true)
	{		
		DesDes(auchTemp2,auchTemp1,auchKey,DES_ENCRYPT);
		//与后8位异或
		n+=8;
		if(n>=iInputLen)
			break;
		ZeroMemory(auchTemp3,8);
		memcpy(auchTemp3,auchInput+n,min(8,iInputLen-n));
		for(int i=0;i<8;i++)
			auchTemp1[i]=auchTemp2[i]^auchTemp3[i];
	}
	if(8==iKeyLen)
		memcpy(auchOutput,auchTemp2,8);
	else if(16==iKeyLen)
	{
		DesDes(auchTemp1,auchTemp2,auchKey+8,DES_DECRYPT);
		DesDes(auchOutput,auchTemp1,auchKey,DES_ENCRYPT);
	}
	else if(24==iKeyLen)
	{
		DesDes(auchTemp1,auchTemp2,auchKey+8,DES_DECRYPT);
		DesDes(auchOutput,auchTemp1,auchKey+16,DES_ENCRYPT);
	}
}

void Des3(BYTE *auchOutput,BYTE *auchInput,int iInputLen,BYTE *auchKey,int iKeyLen,short sMethod)
{
	if(8==iKeyLen)//64bit
	{
		BYTE auchInput1[8],auchOutput1[8];
		for(int i=0;i<iInputLen;i+=8)
		{
			ZeroMemory(auchInput1,8);
			ZeroMemory(auchOutput1,8);
			memcpy(auchInput1,auchInput+i,min(8,iInputLen-i));
			DesDes(auchOutput1,auchInput1,auchKey,sMethod);
			memcpy(auchOutput+i,auchOutput1,8);
		}
	}
	if(16==iKeyLen)//128bit
	{
		BYTE auchInput1[8];		
		BYTE auchTemp1[8],auchTemp2[8],auchTemp3[8];
		for(int i=0;i<iInputLen;i+=8)
		{
			ZeroMemory(auchInput1,8);
			ZeroMemory(auchTemp1,8);
			ZeroMemory(auchTemp2,8);
			ZeroMemory(auchTemp3,8);
			memcpy(auchInput1,auchInput+i,min(8,iInputLen-i));
			DesDes(auchTemp1,auchInput1,auchKey,sMethod);
			DesDes(auchTemp2,auchTemp1,auchKey+8,!sMethod);
			DesDes(auchTemp3,auchTemp2,auchKey,sMethod);
			memcpy(auchOutput+i,auchTemp3,8);
		}
	}
	else if(24==iKeyLen)//196bit
	{
		BYTE auchInput1[8];		
		BYTE auchTemp1[8],auchTemp2[8],auchTemp3[8];
		for(int i=0;i<iInputLen;i+=8)
		{
			ZeroMemory(auchInput1,8);
			ZeroMemory(auchTemp1,8);
			ZeroMemory(auchTemp2,8);
			ZeroMemory(auchTemp3,8);
			memcpy(auchInput1,auchInput+i,min(8,iInputLen-i));
			DesDes(auchTemp1,auchInput1,auchKey,sMethod);
			DesDes(auchTemp2,auchTemp1,auchKey+8,!sMethod);
			DesDes(auchTemp3,auchTemp2,auchKey+16,sMethod);
			memcpy(auchOutput+i,auchTemp3,8);
		}
	}
	else
	{
		;
	}
}

void XOR(BYTE *auchOutput,BYTE *auchInput1,BYTE *auchInput2,int iInputLen)
{
	for(int i=0;i<iInputLen;i++)
		auchOutput[i]=auchInput1[i]^auchInput2[i];
}