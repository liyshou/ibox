// Pack.cpp: implementation of the CPack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "Pack.h"
#include "Tools.h"

#include "Def.h"
#include "packet.h"
#include "Global.h"
#include "Def.h"
#include <winsock2.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	 PACKET_ERROR	3
#define  NO_BIT 130


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPack::CPack()
{
	memset(&Packet_Table, 0, sizeof(Packet_Table));
	int i;
	for(i = 0; i<65; i++)
	{
		Packet_Table[i].Bit_Name = i;
		Packet_Table[i].Real_Length = 0; 
	}

	Packet_Table[0].Type = BITS;		Packet_Table[0].Format = FIXED;		Packet_Table[0].Max_Length = 8;
	Packet_Table[1].Type = BITS;		Packet_Table[1].Format = FIXED;		Packet_Table[1].Max_Length = 8;
	Packet_Table[2].Type = NUMERIC;		Packet_Table[2].Format = LLVAR;		Packet_Table[2].Max_Length = 22;
	Packet_Table[3].Type = NUMERIC;		Packet_Table[3].Format = FIXED;		Packet_Table[3].Max_Length = 3;
	Packet_Table[4].Type = NUMERIC;		Packet_Table[4].Format = FIXED;		Packet_Table[4].Max_Length = 6;
	Packet_Table[7].Type = NUMERIC;		Packet_Table[7].Format = FIXED;		Packet_Table[7].Max_Length = 5;
	Packet_Table[11].Type = NUMERIC;	Packet_Table[11].Format = FIXED;	Packet_Table[11].Max_Length = 3;
	Packet_Table[12].Type = NUMERIC;	Packet_Table[12].Format = FIXED;	Packet_Table[12].Max_Length = 3;
	Packet_Table[13].Type = NUMERIC;	Packet_Table[13].Format = FIXED;	Packet_Table[13].Max_Length = 4;
	Packet_Table[15].Type = NUMERIC;	Packet_Table[15].Format = FIXED;	Packet_Table[15].Max_Length = 2;
	Packet_Table[24].Type = NUMERIC;	Packet_Table[24].Format = FIXED;	Packet_Table[24].Max_Length = 2;
	Packet_Table[32].Type = NUMERIC;	Packet_Table[32].Format = LLVAR;	Packet_Table[32].Max_Length = 3;
	Packet_Table[33].Type = NUMERIC;	Packet_Table[33].Format = LLVAR;	Packet_Table[33].Max_Length = 3;
	Packet_Table[34].Type = ALPHANUM;	Packet_Table[34].Format = LLVAR;	Packet_Table[34].Max_Length = 16;
	Packet_Table[35].Type = ALPHANUM;	Packet_Table[35].Format = LLVAR;	Packet_Table[35].Max_Length = 37;
	Packet_Table[36].Type = ALPHANUM;	Packet_Table[36].Format = LLLVAR;	Packet_Table[36].Max_Length = 104;
	Packet_Table[37].Type = ALPHANUM;	Packet_Table[37].Format = FIXED;	Packet_Table[37].Max_Length = 12;
	Packet_Table[38].Type = ALPHANUM;	Packet_Table[38].Format = FIXED;	Packet_Table[38].Max_Length = 6;
	Packet_Table[39].Type = ALPHANUM;	Packet_Table[39].Format = FIXED;	Packet_Table[39].Max_Length = 2;
	Packet_Table[41].Type = ALPHANUMS;	Packet_Table[41].Format = FIXED;	Packet_Table[41].Max_Length = 8;
	Packet_Table[42].Type = ALPHANUMS;	Packet_Table[42].Format = FIXED;    Packet_Table[42].Max_Length = 15;
	Packet_Table[44].Type = ALPHANUMS;	Packet_Table[44].Format = LLVAR;	Packet_Table[44].Max_Length = 19;
	Packet_Table[45].Type = ALPHANUMS;	Packet_Table[45].Format = LLVAR ;	Packet_Table[45].Max_Length = 99;
	Packet_Table[46].Type = ALPHANUMS;	Packet_Table[46].Format = LLLVAR;	Packet_Table[46].Max_Length = 999;
	Packet_Table[48].Type = ALPHANUMS;  Packet_Table[48].Format = LLLVAR;	Packet_Table[48].Max_Length = 42;
	Packet_Table[52].Type = BITS;		Packet_Table[52].Format = FIXED;	Packet_Table[52].Max_Length = 8;
	Packet_Table[53].Type = NUMERIC;	Packet_Table[53].Format = FIXED;	Packet_Table[53].Max_Length = 8;
	Packet_Table[54].Type = ALPHANUMS;  Packet_Table[54].Format = LLLVAR;	Packet_Table[54].Max_Length = 39;
	Packet_Table[55].Type = ALPHANUMS;  Packet_Table[55].Format = LLLVAR;	Packet_Table[55].Max_Length = 999;
	Packet_Table[60].Type = ALPHANUMS;  Packet_Table[60].Format = LLLVAR;	Packet_Table[60].Max_Length = 999;
	Packet_Table[61].Type = ALPHANUMS;  Packet_Table[61].Format = LLLVAR;	Packet_Table[61].Max_Length = 15;
	Packet_Table[62].Type = ALPHANUMS;	Packet_Table[62].Format = LLLVAR;	Packet_Table[62].Max_Length = 9;
	Packet_Table[63].Type = ALPHANUMS;	Packet_Table[63].Format = LLLVAR;	Packet_Table[63].Max_Length = 999;
	Packet_Table[64].Type = BITS;		Packet_Table[64].Format = FIXED;	Packet_Table[64].Max_Length = 8;
	
	Packet_Table[1].Rev_Data = FIELD0_1;	Packet_Table[1].Send_Data = FIELD0_1;
	Packet_Table[2].Rev_Data = FIELD2;		Packet_Table[2].Send_Data = FIELD2;
	Packet_Table[3].Rev_Data = FIELD3;		Packet_Table[3].Send_Data = FIELD3;
	Packet_Table[4].Rev_Data = FIELD4;		Packet_Table[4].Send_Data = FIELD4;
	Packet_Table[7].Rev_Data = FIELD7;		Packet_Table[7].Send_Data = FIELD7;
	Packet_Table[11].Rev_Data = FIELD11;	Packet_Table[11].Send_Data = FIELD11;
	Packet_Table[12].Rev_Data = FIELD12;	Packet_Table[12].Send_Data = FIELD12;
	Packet_Table[13].Rev_Data = FIELD13;	Packet_Table[13].Send_Data = FIELD13;
	Packet_Table[15].Rev_Data = FIELD15;	Packet_Table[15].Send_Data = FIELD15;
	Packet_Table[24].Rev_Data = FIELD24;	Packet_Table[24].Send_Data = FIELD24;
	Packet_Table[32].Rev_Data = FIELD32;	Packet_Table[32].Send_Data = FIELD32;
	Packet_Table[33].Rev_Data = FIELD33;	Packet_Table[33].Send_Data = FIELD33;
	Packet_Table[34].Rev_Data = FIELD34;	Packet_Table[34].Send_Data = FIELD34;
	Packet_Table[35].Rev_Data = FIELD35;	Packet_Table[35].Send_Data = FIELD35;
	Packet_Table[36].Rev_Data = FIELD36;	Packet_Table[36].Send_Data = FIELD36;
	Packet_Table[37].Rev_Data = FIELD37;	Packet_Table[37].Send_Data = FIELD37;
	Packet_Table[38].Rev_Data = FIELD38;	Packet_Table[38].Send_Data = FIELD38;
	Packet_Table[39].Rev_Data = FIELD39;	Packet_Table[39].Send_Data = FIELD39;
	Packet_Table[41].Rev_Data = FIELD41;	Packet_Table[41].Send_Data = FIELD41;
	Packet_Table[42].Rev_Data = FIELD42;	Packet_Table[42].Send_Data = FIELD42;
	Packet_Table[44].Rev_Data = FIELD44;	Packet_Table[44].Send_Data = FIELD44;
	Packet_Table[45].Rev_Data = FIELD45;	Packet_Table[45].Send_Data = FIELD45;
	Packet_Table[46].Rev_Data = FIELD46;	Packet_Table[46].Send_Data = FIELD46;
	Packet_Table[48].Rev_Data = FIELD48;	Packet_Table[48].Send_Data = FIELD48;
	Packet_Table[52].Rev_Data = FIELD52;	Packet_Table[52].Send_Data = FIELD52;
	Packet_Table[53].Rev_Data = FIELD53;	Packet_Table[53].Send_Data = FIELD53;
	Packet_Table[54].Rev_Data = FIELD54;	Packet_Table[54].Send_Data = FIELD54;
	Packet_Table[55].Rev_Data = FIELD55;	Packet_Table[55].Send_Data = FIELD55;
	Packet_Table[60].Rev_Data = FIELD60;	Packet_Table[60].Send_Data = FIELD60;
	Packet_Table[61].Rev_Data = FIELD61;	Packet_Table[61].Send_Data = FIELD61;
	Packet_Table[62].Rev_Data = FIELD62;	Packet_Table[62].Send_Data = FIELD62;
	Packet_Table[63].Rev_Data = FIELD63;	Packet_Table[63].Send_Data = FIELD63;
	Packet_Table[64].Rev_Data = FIELD64;	Packet_Table[64].Send_Data = FIELD64;
}

CPack::~CPack()
{
	
}

void CPack::init8583()
{
    memset(MTI_Code,0,sizeof(MTI_Code));
	memset(FIELD0_1,0,sizeof(FIELD0_1));
	memset(FIELD2,0,sizeof(FIELD2));
	memset(FIELD3,0,sizeof(FIELD3));
	memset(FIELD4,0,sizeof(FIELD4));
	memset(FIELD7,0,sizeof(FIELD7));
	memset(FIELD11,0,sizeof(FIELD11));
	memset(FIELD12,0,sizeof(FIELD12));
	memset(FIELD13,0,sizeof(FIELD13));
	memset(FIELD15,0,sizeof(FIELD15));
	memset(FIELD32,0,sizeof(FIELD32));
	memset(FIELD33,0,sizeof(FIELD33));
	memset(FIELD37,0,sizeof(FIELD37));
	memset(FIELD39,0,sizeof(FIELD39));
	memset(FIELD41,0,sizeof(FIELD41));
	memset(FIELD42,0,sizeof(FIELD42));
	memset(FIELD45,0,sizeof(FIELD45));
	memset(FIELD46,0,sizeof(FIELD46));
	memset(FIELD47,0,sizeof(FIELD47));
	memset(FIELD48,0,sizeof(FIELD48));
	memset(FIELD60,0,sizeof(FIELD60));
	memset(FIELD62,0,sizeof(FIELD62));
	memset(FIELD63,0,sizeof(FIELD63));
	memset(FIELD64,0,sizeof(FIELD64));
}

void CPack::set_gbl(CGlobal in_gbl)
{
	gbl = in_gbl;
}

//打包函数
//flag:1，查询；2，交费；3，签到; 4，羊城通确认; 5, 冲正
int CPack::Comm_Pack(int flag, BYTE *data_source)
{
	int ret;
	switch(flag)
	{
	case 1:
		ret = Comm_Inquiry(data_source);
		break;
	case 2:
		ret = Comm_Payment(data_source);
		break;
	case 3:
		ret = Comm_Logon(data_source);
		break;
	case 4:
		ret = Comm_Ack(data_source);
		break;
	case 5:
		ret = Comm_Rev(data_source);
		break;
	}
	return ret;
} 


//设置位元为空
void CPack::setNoBit(BYTE *str, int value)
{
	int i=0;
	while(str[i] != 0)
	{
		if(str[i] == value)
			str[i] = NO_BIT;
		i++;
	}

}

//-------------------------2007-3-14-----------------------------------------------------
//函数名:       Comm_Ack
//功能描述：   羊城通确认打包
//输入：	无
//输出：	无
//返回：	无
int CPack::Comm_Ack(BYTE *data_source)
{
	BYTE ret_exc;
	int  length=0;
	
	if ( (ret_exc=Exchange_ACK())!=0 )
		return ret_exc;
	length =packFunc(data_source);
	return length;
}

//功能描述：羊城通确认位元
//输入：	无
//输出：	无
//返回：	无
BYTE CPack::Exchange_ACK()
{
	
	BYTE bitmap_ack[15] =
	{Bit2,Bit3,Bit4,Bit11,Bit12,Bit13,Bit24,Bit37,Bit41,Bit48,Bit61,Bit62,Bit63,Bit64,0};
//	CTools::LongToBcd(350, MTI_Code, 4);
	CTools::hex_to_byte((BYTE*)"0350", MTI_Code, 4);
	SetAllBitMap(FIELD0_1,bitmap_ack);
		
	return 0;
}

//-------------------------2007-3-15-----------------------------------------------------
//函数名:       Comm_Rev
//功能描述：   冲正打包
//输入：	无
//输出：	无
//返回：	无
int CPack::Comm_Rev(BYTE *data_source)
{
	BYTE ret_exc;
	int  length=0;
	
	if ( (ret_exc=Exchange_REV())!=0 )
		return ret_exc;
	length =packFunc(data_source);
	return length;
}

//功能描述：冲正位元
//输入：	无
//输出：	无
//返回：	无
BYTE CPack::Exchange_REV()
{
	
	BYTE bitmap_ack[15] =
	{Bit2,Bit3,Bit4,Bit11,Bit12,Bit13,Bit24,Bit35,Bit36,Bit41,Bit48,Bit61,Bit62,Bit64,0};

	if(gbl.track2_len == 0)	//二磁道无信息，屏蔽掉二磁道bit35
//		bitmap_ack[7] = NO_BIT;
		setNoBit(bitmap_ack, Bit35);

	if(gbl.track3_len == 0)	//三磁道无信息，屏蔽掉三磁道bit36
//		bitmap_ack[8] = NO_BIT;
		setNoBit(bitmap_ack, Bit36);
	
//	CTools::LongToBcd(400, MTI_Code, 4);
	CTools::hex_to_byte((BYTE*)"0400", MTI_Code, 4);
	SetAllBitMap(FIELD0_1,bitmap_ack);
		
	return 0;
}


//--------------------------2006-10-15----------------------------------------------------
//函数名:       Comm_Logon
//功能描述：    签到打包
//输入：	无
//输出：	无
//返回：	无

int CPack::Comm_Logon(BYTE *data_source)
{
	BYTE ret_exc;
	int  length=0;
	
	if ( (ret_exc=Exchange_LOGON())!=0 )
		return ret_exc;
	length =packFunc(data_source);
	return length;
}

//功能描述：签到位元
//输入：	无
//输出：	无
//返回：	无
BYTE CPack::Exchange_LOGON()
{
	
	BYTE bitmap_logon[13] =
	{Bit3,Bit11,Bit12,Bit13,Bit24,Bit41,Bit46,Bit53,Bit61,Bit62,Bit63,Bit64,0};

	CTools::hex_to_byte((BYTE *)"0800", MTI_Code, 4);
	if(!gbl.posInfo.enyFlag)		//加密改造，旧加密体系
	{
	//	bitmap_logon[7]  = NO_BIT;
	//	bitmap_logon[10] = NO_BIT;
		setNoBit(bitmap_logon, Bit53);
		setNoBit(bitmap_logon, Bit63);
	}
	SetAllBitMap(FIELD0_1,bitmap_logon);
		
	return 0;
}

//------------------------------------------------------------------------------
//函数名:       Comm_Inquiry
//功能描述：    查询打包
//输入：	无
//输出：	无
//返回：	无
int CPack::Comm_Inquiry(BYTE *data_source)
{
	BYTE ret_exc;
	int  length=0;
	
	if ( (ret_exc=Exchange_INQUIRY())!=0 )
		return ret_exc;
	length =packFunc(data_source);
	return length;
}

//功能描述：查询位元
//输入：	无
//输出：	无
//返回：	无
BYTE CPack::Exchange_INQUIRY()
{
	wrLog(gbl.fee_type);
	BYTE bitmap_inquiry[13] =
		{Bit2,Bit3,Bit7,Bit11,Bit12,Bit13,Bit24,Bit41,Bit48,Bit61,Bit62,Bit64,0};
        
	//小灵通付费易卡查询、修改密码
	BYTE bitmap_inquiry2[19] =
		{Bit2,Bit3,Bit7,Bit11,Bit12,Bit13,Bit24,Bit35,Bit36,Bit41,Bit46,Bit48,Bit52,Bit53,Bit61,Bit62,Bit63,Bit64,0};
    //银行划扣受理
    BYTE bitmap_inquiry3[14] =
		{Bit2,Bit3,Bit7,Bit11,Bit12,Bit13,Bit24,Bit41,Bit48,Bit61,Bit62,Bit63,Bit64,0};
	//加密机查询
	BYTE bitmap_inquiry4[15] =
		{Bit2,Bit3,Bit7,Bit11,Bit12,Bit13,Bit24,Bit41,Bit46,Bit48,Bit53,Bit61,Bit62,Bit64,0};
	//电信直充 余额查询
	BYTE bitmap_inquiry5[15] =
		{Bit2,Bit3,Bit7,Bit11,Bit12,Bit13,Bit24,Bit41,Bit46,Bit48,Bit61,Bit62,Bit63,Bit64,0};
	CTools::hex_to_byte((BYTE*)"0100", MTI_Code, 4);
	//小灵通付费易卡或e卡查询、修改密码
	switch(atoi(gbl.fee_type))
	{
	case 83:
	case 84:
	case 145:  //2007-6-11，商户查询
	case 146:
	case 171:  //2007-8-1，e卡充值登记
	case 185:
	case 238:
	case 228:
	case 248:
	case 250:
	case 235://东莞交罚日查询
	case 236://东莞交罚月查询
	case 312:
	case 333:
		if(gbl.track2_len == 0)	//二磁道无信息，屏蔽掉二磁道bit35
			setNoBit(bitmap_inquiry2, Bit35);
		if(gbl.track3_len == 0)	//三磁道无信息，屏蔽掉三磁道bit36
			setNoBit(bitmap_inquiry2, Bit36);
		if(!gbl.posInfo.enyFlag)	//加密改造。旧加密方式和不需要加密的业务无46,53域。
		{
			setNoBit(bitmap_inquiry2, Bit46);
			setNoBit(bitmap_inquiry2, Bit53);
		}
		SetAllBitMap(FIELD0_1,bitmap_inquiry2);
		break;
	case 182://东莞售卡按stan号补打
          //屏蔽掉二磁道bit35
			setNoBit(bitmap_inquiry2, Bit35);
	       //屏蔽掉三磁道bit36
			setNoBit(bitmap_inquiry2, Bit36);
		if(!gbl.posInfo.enyFlag)	//加密改造。旧加密方式和不需要加密的业务无46,53域。
		{
			setNoBit(bitmap_inquiry2, Bit46);
			setNoBit(bitmap_inquiry2, Bit53);
		}
		SetAllBitMap(FIELD0_1,bitmap_inquiry2);
		break;
	case 94:
	case 95:
	case 181:  //2007-11-05，交行业务
	case 183:
	case 186:
	case 188:
	case 189:
	case 190:
	case 227:
	case 229:
	case 230:
	case 231:
		if(gbl.track2_len == 0)	//二磁道无信息，屏蔽掉二磁道bit35
			setNoBit(bitmap_inquiry2, Bit35);
		if(gbl.track3_len == 0)	//三磁道无信息，屏蔽掉三磁道bit36
			setNoBit(bitmap_inquiry2, Bit36);
		setNoBit(bitmap_inquiry2, Bit46);
		setNoBit(bitmap_inquiry2, Bit53);
		SetAllBitMap(FIELD0_1,bitmap_inquiry2);
		break;
	case 216:
	case 217:
	case 218:
	case 246:
	case 249:
	case 261:
	case 226:
	case 320:
	case 321:
	case 326:
	case 341:
	case 344:
		SetAllBitMap(FIELD0_1,bitmap_inquiry3);
	    break;
	
	case 222:  //交费易签约
	case 269:  //商户签约
		if(gbl.track2_len == 0)	//二磁道无信息，屏蔽掉二磁道bit35
			setNoBit(bitmap_inquiry2, Bit35);
		if(gbl.track3_len == 0)	//三磁道无信息，屏蔽掉三磁道bit36
			setNoBit(bitmap_inquiry2, Bit36);
		if(!gbl.posInfo.enyFlag)	//加密改造。旧加密方式和不需要加密的业务无46,53域。
		{
			setNoBit(bitmap_inquiry2, Bit46);
			setNoBit(bitmap_inquiry2, Bit53);
		}
		SetAllBitMap(FIELD0_1,bitmap_inquiry2);
	    break;
	
	case 3012:
		if(!gbl.posInfo.enyFlag)	//加密改造。旧加密方式无46,53域
		{
			setNoBit(bitmap_inquiry4, Bit46);
			setNoBit(bitmap_inquiry4, Bit53);
		}
		SetAllBitMap(FIELD0_1,bitmap_inquiry4);
	    break;
	case 296:
	case 297:
	
		SetAllBitMap(FIELD0_1,bitmap_inquiry5);
	    break;
	default:
		SetAllBitMap(FIELD0_1,bitmap_inquiry);
		break;
	}
	
	return 0;
}

//------------------------------------------------------------------------------
//功能描述：交费打包
//输入：	无
//输出：	无
//返回：	无
int CPack::Comm_Payment(BYTE *data_source)
{
	BYTE ret_exc;
        int  length;

	if ( (ret_exc = Exchange_PAYMENT())!=0 )
		return ret_exc;

	length = packFunc(data_source);
        
    return length;
}

//函数名:   Exchange_PAYMENT
//功能描述：交费位元
//输入：	无
//输出：	无
//返回：	无
BYTE CPack::Exchange_PAYMENT()
{   
	BYTE bitmap_payment[22] =
	   {
		Bit2,Bit3,Bit4,Bit11,Bit12,Bit13,Bit24,Bit34,Bit35,Bit36,Bit41,Bit45,Bit46,Bit48,Bit52,Bit53,Bit55,Bit61,Bit62,Bit63,Bit64,
	   0};
	if(gbl.track2_len == 0)	//二磁道无信息，屏蔽掉二磁道bit35
		setNoBit(bitmap_payment, Bit35);	

	if(gbl.track3_len == 0)	//三磁道无信息，屏蔽掉三磁道bit36
		setNoBit(bitmap_payment, Bit36);

	  if(!gbl.posInfo.enyFlag)	//加密改造。旧加密方式无53域
	  {
		setNoBit(bitmap_payment, Bit53);
		
		if(atoi(gbl.fee_type) == 184)	//交行转帐20071110，有bit46无bit45
		{
			//	bitmap_payment[10] = NO_BIT;
			setNoBit(bitmap_payment, Bit45);
		}
		else if(atoi(gbl.fee_type) == 224 || atoi(gbl.fee_type) == 225)	//银联转帐20071110，有bit46无bit45
		{
			//	bitmap_payment[10] = NO_BIT;
			setNoBit(bitmap_payment, Bit45);
		}
		else if(gbl.key_num != 8) //非银联，无bit45
		{
			if((atoi(gbl.fee_type) == 177 )||(atoi(gbl.fee_type) == 170 ) )  //20070828
				setNoBit(bitmap_payment, Bit45);
			else if (atoi(gbl.fee_type) == 211 || atoi(gbl.fee_type) == 214)
			{
				setNoBit(bitmap_payment, Bit45);
			}else
			{
				setNoBit(bitmap_payment, Bit45);
				setNoBit(bitmap_payment, Bit46);
			}
		}else if (atoi(gbl.fee_type) == 250)
		{
			setNoBit(bitmap_payment, Bit45);
		}
	  }else{
		setNoBit(bitmap_payment, Bit45);
	  }
	  if(atoi(gbl.fee_type)!=131 && atoi(gbl.fee_type)!=176){
        setNoBit(bitmap_payment, Bit55);
	  }
//	CTools::LongToBcd(200, MTI_Code, 4);
	CTools::hex_to_byte((BYTE*)"0200", MTI_Code, 4);

 /*   if (memcmp(gbl.fee_type,"0103",4)) //非银行卡转帐
	{
        bitmap_payment[10] = '0';
    }
   */
	SetAllBitMap(FIELD0_1,bitmap_payment);

	return 0;
}


//函数名:   SetAllBitMap
//功能描述：设置BitMap
//输入：	resmap
//输出：	destmap
//返回：	无
void CPack::SetAllBitMap(BYTE *destmap,BYTE *resmap)
{
	BYTE i;
	
	memset((char *)destmap, 0, 16);
	
	for( i=0; resmap[i] != 0;i++)
	{
		if(resmap[i] != NO_BIT)
			SetBitMap(destmap, resmap[i]);
	}
}


void CPack::SetBitMap( BYTE *bmap, BYTE bitnum)		//设置bit位
{
	
    BYTE decal;
    BYTE val;
	decal = 0x80;
	val = *(bmap + ((bitnum-1) / 8));
	val |= (decal >> ((bitnum-1)% 8));
	*(bmap + ((bitnum-1) / 8)) = val;
}

//函数名:   CheckBitMap
//功能描述：检查是否有Bit_No位
//输入：	BitMap，Bit_No
//输出：	无
//返回：	0，无此位；非0，有此位
BYTE CPack::CheckBitMap( BYTE *BitMap, int Bit_No)
{
    BYTE tempvalue,tempoffset;
	
    tempvalue = *( BitMap + ( (Bit_No - 1) / 8 ) );
    tempoffset = ( Bit_No - 1) % 8;
    return ( (tempvalue << tempoffset) & 0x80 );
}


//函数名:   packFunc
//功能描述：8583打包函数
//输入：	无
//输出：	data_source
//返回：	包长度
int CPack::packFunc(BYTE *data_source)	//打包
{
	int 	i;
	int 	TempLen , Send_Length;
	BYTE	BCD_Content[3], tmp[4+1];
	BYTE    *Send_Packet_Pointer;
	
	Send_Packet_Pointer = data_source;
	Send_Length=0;
	TempLen = 0;

	memcpy( Send_Packet_Pointer, MTI_Code, 2);
	Send_Packet_Pointer += 2;
	Send_Length +=2;
//bitmap
	memcpy( Send_Packet_Pointer,Packet_Table[Bit1].Send_Data, Packet_Table[Bit1].Max_Length);
	Send_Packet_Pointer += Packet_Table[Bit1].Max_Length;
	Send_Length +=Packet_Table[Bit1].Max_Length;
	wrLog("开始打包……");
	for ( i = 2; i <= 64; i++)
	{	
		if ( CheckBitMap( FIELD0_1, i) )	//若有bit位
		{
			if (Packet_Table[i].Real_Length==0)
				TempLen = Packet_Table[i].Max_Length;
			else
				TempLen = Packet_Table[i].Real_Length;

			switch ( Packet_Table[i].Format)
			{
			case FIXED:
				{
					memcpy( Send_Packet_Pointer, Packet_Table[i].Send_Data, TempLen);
					Send_Packet_Pointer += TempLen;
					Send_Length +=TempLen;
					break;
				}
				
			case LLVAR:
				{
					memset(BCD_Content,0,3);
					switch ( Packet_Table[i].Type)
					{
					case NUMERIC:
						if (i==2) 
						{
						//	CTools::LongToBcd(TempLen,BCD_Content,4);
							bbzero(tmp);
							sprintf((char *)tmp, "%04d", TempLen);
							CTools::hex_to_byte(tmp, BCD_Content, 4);
							if (TempLen%2)
							{
								TempLen =(TempLen+1)/2;
							} 
							else
							{
								TempLen =TempLen/2;
							}
						} 
						else 
						{
							CTools::hex_to_byte((BYTE *)(2*TempLen), BCD_Content, 2);
						}
						break;
					case BITS:
						CTools::hex_to_byte((BYTE *)(8*TempLen), BCD_Content, 2);
						break;
					case ALPHANUM:
					case ALPHANUMS:
						//	CTools::LongToBcd(TempLen,BCD_Content,4);
						bbzero(tmp);
						sprintf((char *)tmp, "%04d", TempLen);
						CTools::hex_to_byte(tmp, BCD_Content, 4);
						break;
					}

					memcpy( Send_Packet_Pointer, &BCD_Content[1], 1);
			
					Send_Packet_Pointer += 1;
					Send_Length +=1;
	
					memcpy( Send_Packet_Pointer, Packet_Table[i].Send_Data, TempLen);
					Send_Packet_Pointer += TempLen;
					Send_Length +=TempLen;
			
					break;
				}
				
			case LLLVAR:
				{
					memset(BCD_Content,0,2);
					switch ( Packet_Table[i].Type)
					{
					case NUMERIC:
						CTools::hex_to_byte((BYTE *)(2*TempLen), BCD_Content, 2);
						break;
					case BITS:
						CTools::hex_to_byte((BYTE *)(8*TempLen), BCD_Content, 2);
						break;
					case ALPHANUM:
					case ALPHANUMS:
					//	CTools::LongToBcd(TempLen,BCD_Content,4);
						bbzero(tmp);
						sprintf((char *)tmp, "%04d", TempLen);
						CTools::hex_to_byte(tmp, BCD_Content, 4);
						break;
					}
					
					memcpy( Send_Packet_Pointer, BCD_Content, 2);
					Send_Packet_Pointer += 2;
					Send_Length +=2;
					
					memcpy( Send_Packet_Pointer, Packet_Table[i].Send_Data, TempLen);
					Send_Packet_Pointer += TempLen;
					Send_Length += TempLen;
					
					break;
				}
			} //end switch ( Packet_Table[i].Format)
			if( DEBUG_LEVEL <= 3)
			{
				wrLog("---FIELD%d---",i);
				wrHex("",Packet_Table[i].Send_Data,TempLen);
			}
		}//end if

	}//*for	

	wrHex("上送包内容",data_source,(Send_Length));
	return  Send_Length;
}

//函数名:   unpackFunc
//功能描述：8583解包函数
//输入：	data_source，data_length
//输出：	无
//返回：	TRUE，成功
int CPack::unpackFunc(BYTE *data_source,int data_length)
{
	BYTE	i;
	int	    TempLen,offset;
	int     Real_Length = 0;
	char	response_code[3];
	BYTE	*Rev_Packet_Pointer;

	wrLog("解包开始……");
	Rev_Packet_Pointer = data_source;

//	init8583();
	// 跳过长度
	Rev_Packet_Pointer +=2;
	memset(MTI_Code, 0, sizeof(MTI_Code));
	memcpy( MTI_Code, Rev_Packet_Pointer, 2);
	Rev_Packet_Pointer += 2;
	Real_Length +=2;
	
	/*Get Sub_Bitmap in Packet_Table[Bit1].Rev_Data*/
	memcpy( Packet_Table[Bit1].Rev_Data, Rev_Packet_Pointer, Packet_Table[Bit1].Max_Length);
	Rev_Packet_Pointer += Packet_Table[Bit1].Max_Length;
	
	Real_Length +=Packet_Table[Bit1].Max_Length;
	
	for ( i = 2; i <=64; i++)
	{
		if ( CheckBitMap( FIELD0_1, i) )
		{
		//	wrLog("BIT[%d]",i);
			//add 2007-01-30 by lihk, 写收到的域前先清零
			memset( Packet_Table[i].Rev_Data, 0, sizeof(*Packet_Table[i].Rev_Data) ); 
			switch ( Packet_Table[i].Format)
			{
			case FIXED:
				{
					offset=0;
					TempLen = Packet_Table[i].Max_Length;
					break;
				}
				
			case LLVAR:
				{
					//*取真实长度*
					Packet_Table[i].Real_Length = CTools::BCD_To_INT_10( Rev_Packet_Pointer, 1);
					offset=1;
					Rev_Packet_Pointer += 1;
					
					Real_Length +=1;

					TempLen = Packet_Table[i].Real_Length;
					switch ( Packet_Table[i].Type)
					{
					case NUMERIC:
						/*  判断卡号长度是否为基数  */
						if (i==2) 
						{
							if (TempLen%2)			// 改"/"为"％"
							{
								TempLen +=1;
							}
						}
						
						TempLen = TempLen /2;
						break;
					case BITS:
						TempLen = TempLen /8;
						break;
					}
					if (TempLen>Packet_Table[i].Max_Length)
						return PACKET_ERROR;
					Packet_Table[i].Real_Length = TempLen;
					break;
				}//****case LLVAR***
				
			case LLLVAR:
				{
					Packet_Table[i].Real_Length = CTools::BCD_To_INT_10( Rev_Packet_Pointer, 2);
					offset=2;
					Rev_Packet_Pointer += 2;
					
					Real_Length +=2;
					
					TempLen = Packet_Table[i].Real_Length;
					switch ( Packet_Table[i].Type)
					{
					case NUMERIC:
						TempLen = TempLen /2;
						break;
					case BITS:
						TempLen = TempLen /8;
						break;
					}
					if (TempLen>Packet_Table[i].Max_Length)
						return PACKET_ERROR;
					Packet_Table[i].Real_Length = TempLen;
					break;
				}//****case LLLVAR***
			}//end switch(format)
			memcpy( Packet_Table[i].Rev_Data, Rev_Packet_Pointer, TempLen);
			
			Rev_Packet_Pointer += TempLen;
			Real_Length +=TempLen;

			if( DEBUG_LEVEL <= 2)
			{
				wrLog("FIELD[%d]",i);
				wrHex("",Packet_Table[i].Rev_Data,TempLen);
			}
		
		}//end if
	}//end for
	
	wrLog("系统参考号：[%s]",(char *)FIELD37);

	memset(response_code,0,sizeof(response_code));
	memcpy(response_code,FIELD39,2);
	wrLog("返回码response_code:%s",response_code);
	
	return TRUE;
}

//写域，add by lihk 2007-01-17
//flag:1，查询；2，交费；3，签到；4，确认；5，冲正
void CPack::wrField(int flag, CGlobal in_gbl)	
{
	set_gbl(in_gbl);
	//针对0602版程序830F新加密
	if(memcmp(gbl.pos_serial, "\x00\x00\x00", 3)==0)
	{
		CTools::get_time(gbl.pos_serial);
	}
	switch(flag)
	{
	case 1:
		wrInqField();
		break;
	case 2:
		wrPayField();
		break;
	case 3:
		wrLogonField();
		break;
	case 4:
		wrAckField();
		break;
	case 5:
		wrRevField();
		break;
	}
}

void CPack::wrInqField()	//写查询域，chang by lihk 2006-11-3
{
	int  b48_len=0;
	char date_time_buff[20];
	char len_buff[10];
	char sign_buff[15+1];
	long ii;
    struct tm *tim;
	
	init8583();
	time(&ii);
	tim=localtime(&ii);
	sprintf(date_time_buff,"%04d%02d%02d%02d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,tim->tm_sec);

	memset(FIELD2,0x00,8);
	Packet_Table[2].Real_Length=8;
	CTools::hex_to_byte((BYTE *)"310000", FIELD3, 6);	//查询处理码
	CTools::hex_to_byte((BYTE *)(date_time_buff+4), FIELD7, 10);		//日期时间（月日时分秒）
	CTools::hex_to_byte((BYTE *)gbl.pos_serial, FIELD11, 6);		
	CTools::hex_to_byte((BYTE *)(date_time_buff+8), FIELD12, 6);		//时间（时分秒）
	CTools::hex_to_byte((BYTE *)date_time_buff, FIELD13, 8);			//日期（年月日）
	CTools::hex_to_byte((BYTE *)"0003", FIELD24, 4);
	memcpy(FIELD41,gbl.pos_no,8);	//终端号

	/******************bit48位元*****************/	
	//4位信息码＋电话号码长度＋电话号码
	memcpy(FIELD48,gbl.fee_type,4);	//4位信息码
	b48_len =4;
	memset(len_buff,0,sizeof(len_buff));
	sprintf(len_buff,"%02d",gbl.phone_len);	//电话号码长度（2位）
	memcpy(FIELD48+b48_len,len_buff,2);	
	b48_len +=2;
	memcpy(FIELD48+b48_len,gbl.phone_no,gbl.phone_len);	//电话号码
	b48_len +=gbl.phone_len;
	Packet_Table[48].Real_Length=b48_len;
	/******************end bit48位元*****************/	

	bbzero(sign_buff);
	sprintf(sign_buff,"0000000%s",gbl.curworkid);
	memcpy(FIELD61,sign_buff,15);
	memset(FIELD62,'P',1);
	Packet_Table[62].Real_Length=1;
	memcpy(FIELD64,"88888888",8);
}


//写交费域，chang by lihk 2006-11-3
//查询交易也可在此写域，因一些查询交易用到交费有的域
void CPack::wrPayField()	
{
	int  b48_len=0,temp_len;
	char date_time_buff[20],temp_buff[1024];
	char len_buff[10];
	char sign_buff[15+1];
	long ii;
    struct tm *tim;

	init8583();
	time(&ii);
	tim=localtime(&ii);
	sprintf(date_time_buff,"%04d%02d%02d%02d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	if(!memcmp(gbl.card_no,"8828",4))	//付费易卡
	{
		CTools::hex_to_byte((BYTE *)"888888", FIELD2, 6);
		Packet_Table[2].Real_Length = 6;
		CTools::hex_to_byte((BYTE *)"600000", FIELD3, 6);
	}

	else
	{
		temp_len = 0;
		memset(temp_buff,0,sizeof(temp_buff));
		strcpy(temp_buff,gbl.card_no);
		if(gbl.card_no_len == 0)
			gbl.card_no_len = 18;
		if(gbl.card_no_len%2)		//卡号是奇数，后补"F"
		{
			strcat(temp_buff,"F");
		}
		CTools::hex_to_byte((BYTE *)temp_buff, FIELD2, gbl.card_no_len, 1);	//变为bcd码（后补0）
		Packet_Table[2].Real_Length=gbl.card_no_len; //第二域卡号
		
		switch(atoi(gbl.fee_type))
		{
		case 1:
		case 13:
			CTools::hex_to_byte((BYTE *)"600000", FIELD3, 6);	//打印时间明细为长格式
			break;
		default:
			CTools::hex_to_byte((BYTE *)"000000", FIELD3, 6); //第三域处理码000000
			break;
		}
		if(gbl.key_num == 8)
			CTools::hex_to_byte((BYTE *)"000080", FIELD3, 6);
	}
	CTools::hex_to_byte((BYTE *)gbl.amount, FIELD4, 12);//第四域数据
	CTools::hex_to_byte((BYTE *)(date_time_buff+4), FIELD7, 10);
	CTools::hex_to_byte((BYTE *)gbl.pos_serial, FIELD11, 6);
	CTools::hex_to_byte((BYTE *)(date_time_buff+8), FIELD12, 6);
	CTools::hex_to_byte((BYTE *)date_time_buff, FIELD13, 8);
	CTools::hex_to_byte((BYTE *)"0003", FIELD24, 4);

	memcpy(FIELD34, gbl.ip, strlen(gbl.ip));	
	Packet_Table[34].Real_Length = strlen(gbl.ip);
	
	if (!memcmp(gbl.card_no,"8828",4))	//小灵通付费易卡
	{
		if( gbl.track2_len)
		{
			memcpy(FIELD35,gbl.track2,26);
			Packet_Table[35].Real_Length =26;
		}
	}
	else
	{
		memcpy(FIELD35,gbl.track2,gbl.track2_len);	
		Packet_Table[35].Real_Length =gbl.track2_len;
		memcpy(FIELD36,gbl.track3,gbl.track3_len);
		Packet_Table[36].Real_Length =gbl.track3_len;
	}
		
	strcpy((char*)FIELD41,gbl.pos_no);

	if(gbl.posInfo.enyFlag)		//加密改造
	{
		FIELD46[0] = 0x39;
		if (atoi(gbl.fee_type) == 177)
		{
			FIELD46[1] = 0x42;	//原来为0x39，改为0x42走银联
		}else if(atoi(gbl.fee_type) == 184 || atoi(gbl.fee_type) == 284 || atoi(gbl.fee_type) == 286||atoi(gbl.fee_type) == 170  )
			FIELD46[1] = 0x35;
		else if((atoi(gbl.fee_type) == 99 || atoi(gbl.fee_type) == 92 || atoi(gbl.fee_type) == 210) && strcmp(gbl.bank,"105810")!=0)
			FIELD46[1] = 0x66;
		else if(atoi(gbl.fee_type) == 224 || atoi(gbl.fee_type) == 225)
			FIELD46[1] = 0x35;
		else if(atoi(gbl.fee_type) == 211 || atoi(gbl.fee_type) == 214)
		{
			if (memcmp(gbl.card_no,"1088",4))
			{
				FIELD46[1] = 0x62;
			}else{
				FIELD46[1] = 0x39;
			}
		}
		else
			FIELD46[1] = 0x64;
		Packet_Table[46].Real_Length = 2;
		memcpy((char *)FIELD53, gbl.posInfo.psamNum, 8);
		Packet_Table[53].Real_Length = 8;
	}
	/*-------------银联-------------*/
	else if(gbl.key_num == 8)
	{
		if(atoi(gbl.fee_type) == 211 || atoi(gbl.fee_type) == 214|| atoi(gbl.fee_type) == 177)
		{
			wrHex("签到key",(BYTE *)logon_random2,8);
			memcpy((char *)FIELD45,logon_random2,8);
			Packet_Table[45].Real_Length=8;
			memcpy((char *)FIELD46,"6",1);
			Packet_Table[46].Real_Length=1;
		}
		else{
			wrHex("签到key",(BYTE *)logon_random2,8);
			memcpy((char *)FIELD45,logon_random2,8);
			Packet_Table[45].Real_Length=8;
			memcpy((char *)FIELD46,"5",1);
			Packet_Table[46].Real_Length=1;
		}		
//		wrHex("FIELD45",(BYTE *)FIELD45,8);
	}
	/*--------交行转帐20071110-------*/
	else if(atoi(gbl.fee_type) == 184)
	{
		memcpy((char *)FIELD46,"1",1);
		Packet_Table[46].Real_Length=1;
	}
	/*--------银联转帐20081016-------*/
	else if(atoi(gbl.fee_type) == 224 || atoi(gbl.fee_type) == 225)
	{
		memcpy((char *)FIELD46,"1",1);
		Packet_Table[46].Real_Length=1;
	}
	/*--------东莞公交20090611-------*/
	else if(atoi(gbl.fee_type) == 211 || atoi(gbl.fee_type) == 214)
	{
		memcpy((char *)FIELD46,"2",1);
		Packet_Table[46].Real_Length=1;
	}
	/*------------------－－－－－－-*/
	else if(atoi(gbl.fee_type) == 177)
	{
		memcpy((char *)FIELD46,"2",1);
		Packet_Table[46].Real_Length=1;
	}
	/*------------------－－－－－－-*/
	else if(atoi(gbl.fee_type) == 170)
	{
		memcpy((char *)FIELD46,"1",1);
		Packet_Table[46].Real_Length=1;
	}

	/***********************bit48*******************/	
	memcpy(FIELD48,gbl.fee_type,4);
	b48_len =4;
	sprintf(len_buff,"%02d",gbl.phone_len);
	memcpy(FIELD48+4,len_buff,2);
	b48_len +=2;
	memcpy(FIELD48+6,gbl.phone_no,gbl.phone_len);
	b48_len +=gbl.phone_len;
	Packet_Table[48].Real_Length=b48_len;
	/***********************end bit48***************/

	if (!memcmp(gbl.card_no,"8828",4) || !memcmp(gbl.card_no,"2028",4))	//小灵通付费易卡
		memcpy(FIELD52, "00000000", 8);
	else
		memcpy(FIELD52,gbl.password,8);
	bbzero(sign_buff);
	sprintf(sign_buff,"0000000%s",gbl.curworkid);
	memcpy(FIELD61,sign_buff,15);
	memset(FIELD62,'P',1);
	Packet_Table[62].Real_Length=1;

	if (!memcmp(gbl.card_no,"8828",4) || !memcmp(gbl.card_no,"2028",4))	//小灵通付费易卡，bit63
	{
		memset((char *)FIELD63,0,sizeof(FIELD63));
		memcpy(FIELD63,"00000000",8);
		strcat((char *)FIELD63,gbl.xlt_card);
		Packet_Table[63].Real_Length = 8 + strlen(gbl.xlt_card);
	}
	else if( gbl.key_num % 2 )	//奇数key
	{
		memcpy(FIELD63,gbl.random_num,8);
		Packet_Table[63].Real_Length = 8;
	} 
	else
	{
		memcpy(FIELD63,"00000000",8);
		Packet_Table[63].Real_Length = 8;
	}
	strcpy((char *)FIELD64,"88888888");
//wrLog("FIELD63:%s",FIELD63);
}

void CPack::wrLogonField()	//写签到域,2006-10-15
{
	int  b48_len=0;
	char date_time_buff[20];
	char tmp[16+1];
	char sign_buff[15+1];
	long ii;
    struct tm *tim;

	init8583();
	time(&ii);
	tim=localtime(&ii);
	sprintf(date_time_buff,"%04d%02d%02d%02d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,tim->tm_sec);

	CTools::hex_to_byte((BYTE *)"960000", FIELD3, 6);	//查询处理码
	CTools::hex_to_byte((BYTE *)gbl.pos_serial, FIELD11, 6);		
	CTools::hex_to_byte((BYTE *)(date_time_buff+8), FIELD12, 6);		//时间（时分秒）
	CTools::hex_to_byte((BYTE *)date_time_buff, FIELD13, 8);			//日期（年月日）
	CTools::hex_to_byte((BYTE *)"0003", FIELD24, 4);
	memcpy(FIELD41,gbl.pos_no,8);	//终端号

	memcpy(FIELD46,"5",1);
	Packet_Table[46].Real_Length = 1;
	bbzero(sign_buff);
	sprintf(sign_buff,"0000000%s",gbl.curworkid);
	memcpy(FIELD61,sign_buff,15);
	memset(FIELD62,'P',1);
	Packet_Table[62].Real_Length=1;

	char mac_result[16+1];
	memset(mac_result,0,sizeof(mac_result));
	memset(tmp,0,sizeof(tmp));
	strcpy((char *)FIELD64, "11111111");
	memcpy(FIELD64,tmp,8);
}


//写确认域，lihk 2007-3-15
void CPack::wrAckField()	
{
	int  b48_len=0;
	char date_time_buff[20];
	char sign_buff[15+1];
	long ii;
    struct tm *tim;
	
	init8583();
	time(&ii);
	tim=localtime(&ii);
	sprintf(date_time_buff,"%04d%02d%02d%02d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,tim->tm_sec);

	memset(FIELD2,0x00,8);
	Packet_Table[2].Real_Length=8;
	
	CTools::hex_to_byte((BYTE *)"000000", FIELD3, 6);	//查询处理码
	CTools::hex_to_byte((BYTE *)gbl.amount, FIELD4, 12);
	CTools::hex_to_byte((BYTE *)gbl.pos_serial, FIELD11, 6);		
	CTools::hex_to_byte((BYTE *)(date_time_buff+8), FIELD12, 6);		//时间（时分秒）
	CTools::hex_to_byte((BYTE *)date_time_buff, FIELD13, 8);			//日期（年月日）
	CTools::hex_to_byte((BYTE *)"0003", FIELD24, 4);
	memcpy(FIELD37,gbl.stan_no, 12);	//交费系统参考号
	memcpy(FIELD41,gbl.pos_no,8);	//终端号

	/******************bit48位元*****************/	
	//在外部补写
	/******************bit63位元*****************/	
	//在外部补写

	bbzero(sign_buff);
	sprintf(sign_buff,"0000000%s",gbl.curworkid);
	memcpy(FIELD61,sign_buff,15);
	memset(FIELD62,'P',1);
	Packet_Table[62].Real_Length=1;
	memcpy(FIELD64,"88888888",8);
}


//写冲正域，lihk 2007-3-15
void CPack::wrRevField()	
{
	int  b48_len=0, temp_len;
	char date_time_buff[20], temp_buff[25];
	char sign_buff[15+1];
	long ii;
    struct tm *tim;
	
	init8583();
	time(&ii);
	tim=localtime(&ii);
	sprintf(date_time_buff,"%04d%02d%02d%02d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,tim->tm_sec);

	temp_len = 0;
	memset(temp_buff,0,sizeof(temp_buff));
	strcpy(temp_buff,gbl.card_no);
	if(gbl.card_no_len%2)		//卡号是奇数，后补"F"
	{
		strcat(temp_buff,"F");
	}
	CTools::hex_to_byte((BYTE *)temp_buff, FIELD2, gbl.card_no_len, 1);	//变为bcd码（后补0）
	Packet_Table[2].Real_Length=gbl.card_no_len;
	
	CTools::hex_to_byte((BYTE *)"090000", FIELD3, 6);	//处理码
	CTools::hex_to_byte((BYTE *)gbl.amount, FIELD4, 12);
	CTools::hex_to_byte((BYTE *)gbl.pos_serial,FIELD11, 6);		//gbl.pos_serial
	CTools::hex_to_byte((BYTE *)(date_time_buff+8), FIELD12, 6);		//时间（时分秒）
	CTools::hex_to_byte((BYTE *)date_time_buff, FIELD13, 8);			//日期（年月日）
	CTools::hex_to_byte((BYTE *)"0003", FIELD24, 4);

	memcpy(FIELD35,gbl.track2,gbl.track2_len);	
	Packet_Table[35].Real_Length =gbl.track2_len;
	memcpy(FIELD36,gbl.track3,gbl.track3_len);
	Packet_Table[36].Real_Length =gbl.track3_len;

	memcpy(FIELD41,gbl.pos_no,8);	//终端号
	/******************bit48位元*****************/	
	//在外部补写

	bbzero(sign_buff);
	sprintf(sign_buff,"0000000%s",gbl.curworkid);
	memcpy(FIELD61,sign_buff,15);
	memset(FIELD62,'P',1);
	Packet_Table[62].Real_Length=1;
	memcpy(FIELD64,"88888888",8);
}


