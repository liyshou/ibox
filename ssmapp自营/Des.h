#ifndef __DES_H__
#define __DES_H__

#define DES_ENCRYPT     0
#define DES_DECRYPT     1

void BCD2ASCII(char *szOutput,BYTE *auchInput,int iInputlen);
void ASCII2BCD(BYTE *auchOutput,char *szInput,int iInputLen);
void DesDes(BYTE *auchOutput, BYTE *auchInput, BYTE *auchKey, short sMethod);
void DesPermut(BYTE *, BYTE *, short);
void DesRotate(BYTE *, short);
void DesConvert(BYTE *, BYTE *);
void DesHexToBlock(BYTE *, BYTE *);
void BYTEToHex(BYTE *, BYTE *);
void DesEncrypt(BYTE *auchOutput, BYTE *auchInput, BYTE *auchKey);
void DesDecrypt(BYTE *auchOutput, BYTE *auchInput, BYTE *auchKey);
void MAC(BYTE *auchOutput,BYTE *auchInput,int iInputLen,BYTE *auchKey,int iKeyLen);
void Des3(BYTE *auchOutput, BYTE *auchInput,int iInputLen,BYTE *auchKey, int iKeyLen, short sMethod);
void XOR(BYTE *auchOutput,BYTE *auchInput1,BYTE *auchInput2,int iInputLen);
#endif //__DES_H__
