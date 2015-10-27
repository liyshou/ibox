#ifndef	_3DES__H_
#define _3DES__H_

#ifdef __cplusplus
extern "C"
{
#endif

	void DES3MAC(unsigned char *pbyKey, unsigned char *pbySrc, int iSrcLen, unsigned char *pbyInit, unsigned char *pbyDst);
	void MAC16(unsigned char *pbyKey, unsigned char *pbySrc, int iSrcLen, unsigned char *pbyInit, unsigned char *pbyDst);
	void MAC(unsigned char *pbyKey, unsigned char *pbySrc, int iSrcLen, unsigned char *pbyInit, unsigned char *pbyDst);

#ifdef __cplusplus
}
#endif

#endif


