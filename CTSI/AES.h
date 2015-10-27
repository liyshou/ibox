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
AES ���ܣ�֧�����ⳤ�ȵ����ݼ���
@param		aInput			�����ܵ�����
@param		aInputLenth		���������ݵĳ��ȣ��ֽ�Ϊ��λ
@param		aSessionKey		������Կ
@param		aSessionKeyLength	������Կ�����ݳ��ȣ��ֽ�Ϊ��λ
@param		aOutputLength		���ܺ����ݵĳ��ȣ��ֽ�Ϊ��λ
@return		���ؼ��ܺ������
*/
unsigned char* AESEncryptEx(const unsigned char* aInput,
		   int aInputLength,
		   const unsigned char* aSessionKey,
		   int aSessionKeyLength,
		   int* aOutputLength);

/**
AES ���ܣ�֧�����ⳤ�ȵ����ݽ���
@param		aInput			�����ܵ�����
@param		aInputLenth		���������ݵĳ��ȣ��ֽ�Ϊ��λ
@param		aSessionKey		������Կ
@param		aSessionKeyLength	������Կ�����ݳ��ȣ��ֽ�Ϊ��λ
@param		aOutputLength		���ܺ����ݵĳ��ȣ��ֽ�Ϊ��λ
@return		���ؽ��ܺ������
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
