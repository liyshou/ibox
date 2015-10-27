
int CYCTYPTrade::P_COMMAND(const TSDLLPosInfo dPosInfo,PYCT_DATA pyct_data ,const unsigned char* infoType,  \
						   const unsigned char* ver, int encryptMethod , int endian , unsigned char* inBuf , int inLen , \
							unsigned char* outBuf, int* outLen)
{
	int sndLen = 0;
	int Len    = 0;
	int iRet   = 0;
	unsigned char headerData[256] = { 0 };					//报文头
	unsigned char bodyData[256] ={0 };						//报文体
	unsigned char sndData[512] = { 0 };						//待发送报文
	unsigned char md5_dig[16] = { 0 };

	CTools::hex_to_byte( infoType , headerData , 4);		//信息类型码
	sndLen += 2;
	CTools::hex_to_byte( ver      ,  headerData+sndLen , 4);//报文版本号
	sndLen += 2;
	headerData[4] = encryptMethod;							//报文体加密算法
	headerData[5] = endian;									//数值内容字节序
	sndLen += 2;

	CTools::hex_to_byte(pyct_data->SHID , bodyData , 16);	//握手流水号
	//memcpy(bodyData,pyct_data->SHID,8);
	Len += 8;
	char seq_NO[10] = { 0 };
	sprintf(seq_NO , "%08X" , pyct_data->yct_sequence_NO);	
	CTools::hex_to_byte( (BYTE*)seq_NO ,  bodyData+Len , 8);//报文序号
	Len += 4;
	memcpy(bodyData + Len , inBuf , inLen);					//报文信息域，传入后直接copy，使用此函数需要注意此处
	Len += inLen;

	//write result message into db

	md5_csum( bodyData , Len ,  md5_dig );
	memcpy(bodyData+Len , md5_dig , 4);

	char encrHandID[30] = { 0 };
	CDes::TripleDesEncrypt((char*)CKEY, (char*)pyct_data->SHID , encrHandID );

	int encrLen = 0;
	unsigned char* encr_packdata =  AESDecryptEx( bodyData+8 , Len - 8 , pyct_data->SKEY , 16, &encrLen );

	char len_tmp[10] = { 0 };
	sprintf( len_tmp , "%04X" , 8 + encrLen);
	CTools::hex_to_byte((unsigned char*)len_tmp  ,  headerData + sndLen , 4);
	sndLen += 2;
	memcpy(sndData , headerData , sndLen);			//报文体准备ok

	CTools::hex_to_byte((BYTE*)encrHandID ,  sndData + sndLen , 16);	//将加密过后的握手流水号转成hex
	sndLen += 8;

	memcpy(sndData + sndLen , encr_packdata , encrLen);
	sndLen += encrLen;

	//通讯羊城通服务器
	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(YCT_IP , YCT_PORT , &sd);
	if (iRet != 0 )
	{
		return 1;
	}
	unsigned char rcvData[256] = { 0 };
	int  rcvLen		  = 0 ;
	iRet = ST_commuWithYCTIC3(dPosInfo, pyct_data , 0 , 0 ,sd , sndData , sndLen ,  rcvData , &rcvLen);
	if (iRet != 0)
	{
		return 1;
	}

	unsigned char header_tag[4] = { 0 };
	CTools::byte_to_hex( rcvData , header_tag , 2);
	if (strcmp((char*)header_tag , "FE04") == 0)
	{
		return 2;									//FE04报文，需要重新签到
	}
	if (rcvData[4] != 0x02)
	{
		return 1;									//暂不支持除0x02 AES-128以外的加密方式
	}

	unsigned char bodyDecrypted[256] = { 0 };		//解密后的报文体
	char tmp_shid[16+2] = { 0 };	
	int pack_len = 0;

	CTools::byte_to_hex( rcvData + 8 , (BYTE*)encrHandID , 8);		//握手流水号
	CDes::TripleDesDec((char*)CKEY , encrHandID , tmp_shid);		//解密握手流水号
	CTools::hex_to_byte((BYTE*)tmp_shid , bodyDecrypted , 16);
	pack_len += 8;

	int encr_len;
	BYTE* des_data = AESDecryptEx( rcvData + 16 , rcvLen - 16 , pyct_data->SKEY , 16 , &encr_len);
	memcpy(bodyDecrypted + pack_len , des_data , encr_len - 4);
	pack_len += encr_len - 4;

	char MAC1[20] = { 0 };
	char MAC2[20] = { 0 };
	memcpy(MAC1 , des_data + (encr_len - 4) , 4);
	md5_csum(bodyDecrypted , pack_len , (BYTE*)MAC2);
	MAC2[4] = 0;

	if (memcmp(MAC1 , MAC2 , 4) != 0 )
	{
		return 1;									//服务器返回MAC校验错误
	}

	memcpy(outBuf , rcvData , 8);					//报文头
	memcpy(outBuf + 8 , bodyDecrypted , pack_len);	//报文体不含校验码
	memcpy(outBuf + 8 + pack_len , MAC2 , 4);		//校验码
	*outLen = 8 + pack_len + 4 ;					//返回长度
	return 0;
}



int CYCTYPTrade::R_COMMAND(const TSDLLPosInfo dPosInfo, const int com, const int comType,  \
						   int commandCode , char* inBuf , int inLength , char* outBuf, int* outLength)
{
	int len=0;
	int sndLen = 0;
	int iRet = 0;
	int rcvLen=0;
	BYTE icData [255] = { 0 } ;
	BYTE sndData[255] = { 0 } ;
	int baudRate=57600;

	sndLen = ST_packData(ST_HEAD, inLength + 2 , commandCode, (BYTE *)inBuf, (BYTE *)sndData);	

	if((iRet = SndRcvIcData(dPosInfo, com, comType, "", (BYTE *)sndData, sndLen, (BYTE*)outBuf, outLength, baudRate)) == 0)
	{
		if( outBuf[3]==0x00)
		{
			return 0;
		}
		else	
		{
			return 1;	
		}
	}
	else
	{
		return 1;
	}
	
	return 0;
}

