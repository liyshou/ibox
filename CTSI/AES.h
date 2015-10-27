#ifndef     __PKI_AES_H__
#ifdef __cplusplus
extern "C" {
#endif
#define     __PKI_AES_H__

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 erk[64];     /* encryption round keys */
    uint32 drk[64];     /* decryption round keys */
    int nr;             /* number of rounds */
}
aes_context;


/**
AES 加密，支持任意长度的数据加密
@param		aInput			待加密的数据
@param		aInputLenth		待加密数据的长度，字节为单位
@param		aSessionKey		加密密钥
@param		aSessionKeyLength	加密密钥的数据长度，字节为单位
@param		aOutputLength		加密后数据的长度，字节为单位
@return		返回加密后的数据
*/
unsigned char* AESEncryptEx(const unsigned char* aInput,
		   int aInputLength,
		   const unsigned char* aSessionKey,
		   int aSessionKeyLength,
		   int* aOutputLength);

/**
AES 解密，支持任意长度的数据解密
@param		aInput			待解密的数据
@param		aInputLenth		待解密数据的长度，字节为单位
@param		aSessionKey		解密密钥
@param		aSessionKeyLength	解密密钥的数据长度，字节为单位
@param		aOutputLength		解密后数据的长度，字节为单位
@return		返回解密后的数据
*/
unsigned char* AESDecryptEx(const unsigned char* aInput,
		  int aInputLength,
		  unsigned char* aSessionKey,
		  int aSessionKeyLength,
		  int* aOutputLength);


int  aes_set_key( aes_context *ctx,const uint8 *key, int nbits );
void aes_encrypt( aes_context *ctx, const uint8 input[16], uint8 output[16] );
void aes_decrypt( aes_context *ctx, const uint8 input[16], uint8 output[16] );

#ifdef __cplusplus
}
#endif
#endif /*__PKI_AES_H__*/
