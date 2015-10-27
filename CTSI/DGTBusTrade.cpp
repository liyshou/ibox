// DGTBusTrade.cpp: implementation of the CDGBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DGTBusTrade.h"
#include "Tools.h"
#include "WrLog.h"
#include "Def.h"
#include "WrLog.h"
#include "Des.h"
#include "3DES.h"
#include "DB.h"

#define RLD_PIN_MAK			"4163958262438749"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CDGTBusTrade::~CDGTBusTrade()
{
	
}

CDGTBusTrade::CDGTBusTrade()
{ 
	

}


//初始化东莞通信息
void CDGTBusTrade::initField()
{

	memset(dgtFIELD0,0,sizeof(dgtFIELD0));
	memset(dgtFIELD1,0,sizeof(dgtFIELD1));
	memset(dgtFIELD2,0,sizeof(dgtFIELD2));
	memset(dgtFIELD3,0,sizeof(dgtFIELD3));
	memset(dgtFIELD4,0,sizeof(dgtFIELD4));
	memset(dgtFIELD5,0,sizeof(dgtFIELD5));
	memset(dgtFIELD6,0,sizeof(dgtFIELD6));
	memset(dgtFIELD7,0,sizeof(dgtFIELD7));
	memset(dgtFIELD8,0,sizeof(dgtFIELD8));
	memset(dgtFIELD9,0,sizeof(dgtFIELD9));
	memset(dgtFIELD10,0,sizeof(dgtFIELD10));
	memset(dgtFIELD11,0,sizeof(dgtFIELD11));
	memset(dgtFIELD12,0,sizeof(dgtFIELD12));
	memset(dgtFIELD13,0,sizeof(dgtFIELD13));
	memset(dgtFIELD14,0,sizeof(dgtFIELD14));
	memset(dgtFIELD15,0,sizeof(dgtFIELD15));
	memset(dgtFIELD16,0,sizeof(dgtFIELD16));
	memset(dgtFIELD17,0,sizeof(dgtFIELD17));
	memset(dgtFIELD18,0,sizeof(dgtFIELD18));
	memset(dgtFIELD19,0,sizeof(dgtFIELD19));
	memset(dgtFIELD20,0,sizeof(dgtFIELD20));
	memset(dgtFIELD21,0,sizeof(dgtFIELD21));
	memset(dgtFIELD22,0,sizeof(dgtFIELD22));
	memset(dgtFIELD23,0,sizeof(dgtFIELD23));
	memset(dgtFIELD24,0,sizeof(dgtFIELD24));
	memset(dgtFIELD25,0,sizeof(dgtFIELD25));
	memset(dgtFIELD26,0,sizeof(dgtFIELD26));
	memset(dgtFIELD27,0,sizeof(dgtFIELD27));
	memset(dgtFIELD28,0,sizeof(dgtFIELD28));
	memset(dgtFIELD29,0,sizeof(dgtFIELD29));
	memset(dgtFIELD30,0,sizeof(dgtFIELD30));
	memset(dgtFIELD31,0,sizeof(dgtFIELD31));
	memset(dgtFIELD32,0,sizeof(dgtFIELD32));
	memset(dgtFIELD33,0,sizeof(dgtFIELD33));
	memset(dgtFIELD34,0,sizeof(dgtFIELD34));
	memset(dgtFIELD35,0,sizeof(dgtFIELD35));
	memset(dgtFIELD36,0,sizeof(dgtFIELD36));
	memset(dgtFIELD37,0,sizeof(dgtFIELD37));
	memset(dgtFIELD38,0,sizeof(dgtFIELD38));
	memset(dgtFIELD39,0,sizeof(dgtFIELD39));
	memset(dgtFIELD40,0,sizeof(dgtFIELD40));
	memset(dgtFIELD41,0,sizeof(dgtFIELD41));
}


//东莞通打包
//函数名:   packdgtFunc
//输入：	
//输出：	data_source
//返回：	包长度
int CDGTBusTrade::packdgtFunc(CGlobal *pgbl, char *transMessage, char*tranType,char *sendData,int *sendLen)	//打包
{
	int 	i;
	int		iRet;
	int 	TempLen , Send_Length;
	int max_count=0;
	int pack_len=0;// 报文长度
	char syn_info[12+1]; //同步信息
	char zip_flag='0'; //压缩类型
	char encrypt_flag='0'; //不加密
	char message_type[4+1];
	char version[2+1];
	char keyField[500];
	char packDatas[1024];
	char biz_name[20];
	int keyFiledLen=0;
	char CenterSeqno_field[10+1];
	char cardFaceid[16+1];//
	char cardvaldate[8+1]; //卡有效期
	char deposit[8+1];
	char EDCARDID_field[16+1];

	bbzero(EDCARDID_field);
	bbzero(deposit);
	bbzero(cardvaldate);
	bbzero(cardFaceid);
	bbzero(CenterSeqno_field);

	char body_ver[2+1];
	bbzero(biz_name);
	bbzero(keyField);

	char body_messageType[4+1];
	char body_MessageDateTime[14+1];
	char body_MAC[8+1]; //通讯MAC
	char body_rspcode[2+1]; //响应码

	bbzero(packDatas);
	bbzero(body_ver);
	bbzero(keyField);
	bbzero(body_messageType);
	bbzero(body_MessageDateTime);
	bbzero(body_MAC);
	bbzero(body_rspcode);

	bbzero(version);
	bbzero(syn_info);
	bbzero(message_type);
	bbzero(body_ver);

	strcpy(message_type,"5000");
	strcpy(version,"01");
	getSyncinfo(syn_info);
	wrLog("同步信息:%s",syn_info);
	sprintf(EDCARDID_field,"%016s",pgbl->busi_id);

	initField();
	bbzero(packDatas);
	Send_Length=0;


	memcpy( packDatas, syn_info, 12); //同步信息
	Send_Length +=12;
	memcpy(packDatas+Send_Length,"00015000",8);
	Send_Length+=8;

	//包文体中的通讯部分 通讯部分 固定长度是30
	memcpy(body_ver,"10",2);
	bbzero(body_messageType);
	strcpy(body_messageType,transMessage); //固定值

	//获取时间
	CTools::get_termTime(body_MessageDateTime);

	bbzero(pgbl->dgtCard.MESSAGEDATETIME);
	strcpy(pgbl->dgtCard.MESSAGEDATETIME,body_MessageDateTime);
	bbzero(pgbl->dgtCard.tradeDate);
	bbzero(pgbl->dgtCard.tradeTime);
	memcpy(pgbl->dgtCard.tradeDate,pgbl->dgtCard.MESSAGEDATETIME,8);
	memcpy(pgbl->dgtCard.tradeTime,pgbl->dgtCard.MESSAGEDATETIME+8,6);

	bbzero(body_rspcode);
	strcpy(body_rspcode,"00");

	//计算押金
	bbzero(deposit);
	char depositAmount[4+1];
	bbzero(depositAmount);


	//如果是换卡申请，押金是M1的卡片押金
	if (strcmp(transMessage,"3458")==0 ||  strcmp(transMessage,"3481")==0) //如果是卡管理
	{
		bbzero(pgbl->dgtM1Card.city_code);
		strcpy(pgbl->dgtM1Card.city_code,"5230");
		strcpy(deposit,pgbl->dgtM1Card.deposit);
	}
	else if ( strcmp(transMessage,"3480")==0 ) //如果是结果获取撤销 取CPU卡信息
	{

		bbzero(pgbl->dgtM1Card.city_code);
		strcpy(pgbl->dgtM1Card.city_code,"5230");
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.deposit,(BYTE *) depositAmount,2);
		
		if (strcmp(depositAmount,"FFFF")==0 ) //如果没有押金
		{
			strcpy(deposit,"000000");
			
		}
		else
		{
			char deposit_temp[2+1];
			bbzero(deposit_temp);
			memcpy(deposit_temp,depositAmount+2,2);
			sprintf(deposit,"%06d",(atoi(deposit_temp)*100));
			
		}
	
	}
	else //其他报文是CPU卡的信息
	{
	
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.deposit,(BYTE *) depositAmount,2);
		
		if (strcmp(depositAmount,"FFFF")==0 ) //如果没有押金
		{
			strcpy(deposit,"000000");
			
		}
		else
		{
			char deposit_temp[2+1];
			bbzero(deposit_temp);
			memcpy(deposit_temp,depositAmount+2,2);
			sprintf(deposit,"%06d",(atoi(deposit_temp)*100));
			
		}

	}


	init_dgt_pack(transMessage,tranType,pgbl);

	//计算MAC
	switch (atoi(transMessage))
	{

	case 2051:
		strcpy(biz_name,"签到");
		max_count=17; //联机签到
		bbzero(body_MAC);
		strcpy(body_MAC,"00000000");

		break;
	case 2061: //卡操作请求
		strcpy(biz_name,"卡操作请求");
		max_count=42;

		//计算通讯MAC，关键域
		//messagedatetime报文发送时间(14) + transtype业务类型(2) + posid(12) + possequence(10)+edcardid(16)
		
		memcpy(keyField,body_MessageDateTime,14);
		strcat(keyField,tranType);
		strcat(keyField,pgbl->posId);

		char posSerial[10+1];
		bbzero(posSerial);
		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcat(keyField,posSerial); //前补0
	
		//ecardid(16)
		strcat(keyField,EDCARDID_field);
		
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
		
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //通讯出错
		}

		break;
	case 2063: //售卡
		strcpy(biz_name,"售卡");
		max_count=30;
		//messagedatetime报文发送时间(14) + transmessage交易类型(4) + posid(12) + possequence(10位) +edcardid(16)+ cardsalemode(2) + deposit(8) + befbalance(8) + txnamt(8) + txndate(8) + txntime(6) + cardid(16)
		memcpy(keyField,body_MessageDateTime,14);
		strcat(keyField,transMessage);
		strcat(keyField,pgbl->posId);
		bbzero(posSerial);
		wrLog("终端流水号:%s",pgbl->dgt_posSerial);

		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcat(keyField,posSerial); //前补0
		
		strcat(keyField,EDCARDID_field);
		strcat(keyField,pgbl->dgtCard.sale_type);  //售卡方式 押金卡是2，其他均为1

		//deposit(8) + befbalance(8) + txnamt(8) 
		//
		char deposit_buffer[8+1];
		bbzero(deposit_buffer);
		CTools::addZero(deposit,8,1,deposit_buffer);
		strcat(keyField,deposit_buffer); //前补0

		strcat(keyField,pgbl->dgtCard.tradeBeForeMny); //交易前余额
		strcat(keyField,pgbl->dgtCard.tradeMny);
		strcat(keyField,pgbl->dgtCard.tradeDate);
		strcat(keyField,pgbl->dgtCard.tradeTime);
		strcat(keyField,pgbl->dgtCard.cardShowId);
		
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
		
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //通讯出错
		}

		break;
	case 3458: //换卡请求 
		//计算通讯mac，关键域
	case 3480: //结果获取
	case 3481://结果获取撤销

		if (strcmp(transMessage,"3458")==0)
		{
			strcpy(biz_name,"换卡请求");
			bbzero(CenterSeqno_field);
			CTools::addZero(CenterSeqno_field,10,0,CenterSeqno_field);

		}
		else if (strcmp(transMessage,"3480")==0)
		{
			 bbzero(CenterSeqno_field);
			memcpy(CenterSeqno_field,pgbl->Centerseqno+8,10);
			strcpy(biz_name,"结果获取");
		}
		else
		{
			memcpy(CenterSeqno_field,pgbl->Centerseqno+8,10);
			strcpy(biz_name,"结果获取撤销");

		}
	
		max_count=39;
		memcpy(keyField,transMessage,4);
		char cardno[16+1];
		bbzero(cardno);
		char bus_mny[8+1];
		bbzero(bus_mny);

		CTools::addZero(pgbl->dgtM1Card.bus_showCard,16,1,cardno);

		bbzero(cardFaceid);
		CTools::addZero(pgbl->dgtM1Card.bus_showCard,16,1,cardFaceid);
		strcpy(bus_mny,pgbl->dgtM1Card.bus_amt);

		strcat(keyField,body_MessageDateTime);
		strcat(keyField,CenterSeqno_field);
		strcat(keyField,pgbl->posId);
		bbzero(posSerial);
		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcat(keyField,posSerial); //前补0
			
		//edcardid
		strcat(keyField,EDCARDID_field);

		strcat(keyField,cardFaceid);
		strcat(keyField,cardno);
		//strcat(keyField,gbl.dgtM1Card.city_code);
		strcat(keyField,pgbl->dgtM1Card.city_code);
		strcat(keyField,pgbl->dgtM1Card.card_parent_type);
		strcat(keyField,pgbl->dgtM1Card.card_child_type);
		strcat(keyField,pgbl->dgtM1Card.cardVadate); //卡有效期 8位数
			
		strcat(keyField,bus_mny);
		strcat(keyField,deposit);

		wrLog("参与计算MAC的域");
		wrLog("bus_amt:%s",bus_mny);
		wrLog("cardno:%s",cardno);
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
	
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //通讯出错
		}
		
		break;

	case 3460://换卡完成
		max_count=39;
		//mac = messagetype交易类型(4) + messagedatetime(14) + centerseqno(10) + 
		//+posid(12) + possequence(10) + edcardid(16) + cardfaceid(16) + cardid(16) 
		//+ citycode(4) + cardmkind(2) + cardskind(2) + cardvaldate(8) 
		//+ befbalance(8) + depositamount(6)
		strcpy(biz_name,"换卡完成");
		char city_code[4+1];
		char card_parentType[2+1];
		char card_childType[2+1];
		char effectiveDate[8+1];
		char ecardid[16+1];

		bbzero(card_childType);
		bbzero(card_parentType);
		bbzero(city_code);
		bbzero(effectiveDate);
		bbzero(posSerial);
		bbzero(ecardid);
		bbzero(cardFaceid);

		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.CItyCode,(BYTE *) city_code,2);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardParent_type,(BYTE *) card_parentType,1);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardChild_type,(BYTE *) card_childType,1);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *) effectiveDate,4);
	
		CTools::addZero(pgbl->dgt_posSerial,10,1,posSerial);
		strcpy(cardFaceid,pgbl->dgtCard.cardShowId);

		bbzero(CenterSeqno_field);
		memcpy(CenterSeqno_field,pgbl->Centerseqno+8,10);

		memcpy(keyField,transMessage,4);
		strcat(keyField,body_MessageDateTime);
		strcat(keyField,CenterSeqno_field); //中心号码四位
		strcat(keyField,pgbl->posId);
		strcat(keyField,posSerial); //前补0
		strcat(keyField,EDCARDID_field);
		strcat(keyField,cardFaceid);
		strcat(keyField,pgbl->dgtCard.cardShowId);
		strcat(keyField,city_code);
		strcat(keyField,card_parentType);
		strcat(keyField,card_childType);
		strcat(keyField,effectiveDate);
		
		strcat(keyField,pgbl->dgtCard.tradeBeForeMny);
		strcat(keyField,deposit);
		
		wrLog("keyField[%d]:%s ",strlen(keyField),keyField);
		
		iRet=getCommuMac(pgbl->posKey,pgbl->dgtSignInfo.KEYSET,keyField,body_MAC);
		wrLog("body_mac:%s",body_MAC);
		
		if (iRet!=0)
		{
			return -3; //通讯出错
		}

		break;
	default:
		return -1;
	}

	strcpy(body_rspcode,"00"); //发送报文的响应码是00
	strcat(packDatas,body_ver);
	strcat(packDatas,body_messageType);
	strcat(packDatas,body_MessageDateTime);
	strcat(packDatas,body_MAC);
	strcat(packDatas,body_rspcode);
	Send_Length+=30;

	wrLog("开始打包[%s]……",biz_name);


	for ( i = 0; i < max_count; i++)
	{	

			//如果是定长，实际长度就是最大长度。不过不是定长，实际实际是real_length
			switch (Pack_dgt_table[i].Format)
				{
					case FIXED:
						Pack_dgt_table[i].Real_Length=Pack_dgt_table[i].Max_Length;
						break;
					case LLLVAR:
						break;
						Pack_dgt_table[i].Real_Length=atoi(Pack_dgt_table[i-1].Send_Data); //取上一个字段的值

					default:
						Pack_dgt_table[i].Real_Length=Pack_dgt_table[i].Max_Length;

				}
			

				TempLen = Pack_dgt_table[i].Real_Length;
		
				strcat(packDatas,Pack_dgt_table[i].Send_Data);
				Send_Length += TempLen;

			if( DEBUG_LEVEL <= 3)
			{
				wrLog("[%d]-----%s[%d]:  %s",i,Pack_dgt_table[i].name,Pack_dgt_table[i].Real_Length,Pack_dgt_table[i].Send_Data);  //参数名称
			}

	}

	*sendLen=(Send_Length+4); //加报文长度
	bbzero(sendData);
	sprintf(sendData,"%04d%s",Send_Length,packDatas);

	wrLog("发送%s数据包:%s",biz_name,sendData);
	return  0;

}

//东莞通解包
//函数名:   unpackFunc
//功能描述：8583解包函数
//输入：	data_source，data_length
//输出：	无
//返回：	TRUE，成功
int CDGTBusTrade::undgtpackFunc(CGlobal *pgbl, char *transMessage,char *transType,char *rcvData,int rcvLen,char *rsp_code)
{
	BYTE	i;
	int	    offset=0;
	int     Real_Length = 0;
	int max_count=0;
	int unpackLen=0;
	char commuMac[8+1];
	char bizName[100];
	char trans_code[2+1];

	bbzero(rsp_code);
	bbzero(commuMac);
	bbzero(bizName);
	bbzero(rsp_code);
	bbzero(trans_code);

	wrLog("接收报文长度:%d",rcvLen);
	initField();
	
	memcpy(commuMac,rcvData+24+20,8); //通讯mac
	memcpy(trans_code,rcvData+24+28,2); //通讯应答
	wrLog("通讯MAC:%s",commuMac);
	wrLog("应答响应码:%s",trans_code);
	//获取响应码

	if (strcmp(trans_code,"00")!=0) //如果响应失败
	{
		bbzero(rsp_code);
		strcpy(rsp_code,trans_code);
		return -1;// 通讯报文响应失败
	}

	//获取rsp_code
	memcpy(rsp_code,rcvData+rcvLen-5,5);
	if (strcmp(rsp_code,"00000")!=0) //如果响应码不是00000
	{
		return -2;
	}

	wrLog("%s解包开始……",bizName);

	offset =54; //直接获取报文体内容
	

	switch (atoi(transMessage))
	{

	case 2051:
		strcpy(bizName,"签到");
		max_count=17; //联机签到
		break;
	case 2061: //卡操作请求
		strcpy(bizName,"卡操作请求");
		max_count=42;
		//计算通讯MAC，关键域
		//messagedatetime报文发送时间(14) + transtype业务类型(2) + posid(12) + possequence(10)+edcardid(16)
		//	getCommuMac();
		
		break;
	case 2063: //售卡
		max_count=30;
		strcpy(bizName,"售卡");
		//getCommuMac();
		//计算通信MAC,关键域
		break;
	case 3458: //换卡请求
		max_count=39;
		strcpy(bizName,"换卡请求");

		//	getCommuMac();
		//计算通讯mac，关键域
	case 3480: //结果获取
		//计算通讯MAC,关键域
		max_count=39;
		strcpy(bizName,"结果获取");
		//	getCommuMac();
		
	case 3481://结果获取撤销
		max_count=39;
		strcpy(bizName,"结果获取撤销");
		//	getCommuMac();
		break;
	case 3460://换卡完成
		strcpy(bizName,"换卡完成");
		max_count=39;
		//	getCommuMac();
		break;
	default:
		break;
	}


	for ( i = 0; i <max_count; i++)
	{

			memset( Pack_dgt_table[i].Rev_Data, 0, sizeof(Pack_dgt_table[i].Rev_Data) ); 

			switch ( Pack_dgt_table[i].Format) //如果是定长
			{
			case FIXED:
				{
					Pack_dgt_table[i].Real_Length=Pack_dgt_table[i].Max_Length;
					memcpy(Pack_dgt_table[i].Rev_Data,rcvData+offset,Pack_dgt_table[i].Max_Length);
					offset += Pack_dgt_table[i].Max_Length;				
					break;
				}
				
			case LLLVAR: //如果不是定长
				{

					//取真实长度
					
					int real_length=Pack_dgt_table[i].Real_Length;
				
					memcpy(Pack_dgt_table[i].Rev_Data,rcvData+offset,real_length);
					offset+=real_length;

					
				}//****case LLLVAR***
			}//end switch(format)

			if( DEBUG_LEVEL <= 2)
			{
				wrLog("[%d]----%s[%d]:  %s",i,Pack_dgt_table[i].name,Pack_dgt_table[i].Real_Length,Pack_dgt_table[i].Rev_Data);
			}
		
	}//end for


	//获取信息
	switch (atoi(transMessage))
	{
		
	case 2051:
		strcpy(rsp_code,dgtFIELD16);

		break;
	case 2061: //卡操作请求

		//获取授权流水号
		bbzero(pgbl->dgtCard.AUTHSEQ);
		bbzero(pgbl->dgtCard.LIMITEAUTHSEQL);
		strcpy(pgbl->dgtCard.AUTHSEQ,dgtFIELD38);
		strcpy(pgbl->dgtCard.LIMITEAUTHSEQL,dgtFIELD39);
		bbzero(pgbl->dgtCard.SETDATE);
		strcpy(pgbl->dgtCard.SETDATE,dgtFIELD36);
		bbzero(pgbl->dgtCard.BATCHNO);

		
		//如果是售卡获取dicmac 如果是充值获取MAC2
		if (strcmp(transType,"80")==0 || strcmp(transType,"52")==0) //如果收售卡
		{
			
			bbzero(pgbl->dgtCard.DICMAC);
			memcpy(pgbl->dgtCard.DICMAC,dgtFIELD34,8);
			
		}
		else
		{
			bbzero(pgbl->dgtCard.MAC2);
			strcpy(pgbl->dgtCard.MAC2,dgtFIELD25);
			wrLog("东莞通mac2：%s",pgbl->dgtCard.MAC2);
			
		}

		break;
	case 2063: //售卡

		break;
	case 3458: //换卡请求
	case 3480: //结果获取
	case 3481://结果获取撤销

		if (strcmp(transMessage,"3458")==0 )  //在换卡请求的时候返回 
		{

			bbzero(pgbl->Centerseqno);
			strcpy(pgbl->Centerseqno,dgtFIELD3);

			bbzero(pgbl->ReturnAmt);
			strcpy(pgbl->ReturnAmt,dgtFIELD24);
			
			bbzero(pgbl->ReturnDeposit);
			strcpy(pgbl->ReturnDeposit,dgtFIELD25);

			bbzero(pgbl->MonthFeeFlag);
			strcpy(pgbl->MonthFeeFlag,dgtFIELD26);

			bbzero(pgbl->FeeAmtFlag);
			strcpy(pgbl->FeeAmtFlag,dgtFIELD27);

			bbzero(pgbl->MonthFeeAmt);
			strcpy(pgbl->MonthFeeAmt,dgtFIELD28);

			bbzero(pgbl->FeeAmt);
			strcpy(pgbl->FeeAmt,dgtFIELD29);

			bbzero(pgbl->dgtCard.AUTHSEQ);
			strcpy(pgbl->dgtCard.AUTHSEQ,dgtFIELD32);
			
			bbzero(pgbl->dgtCard.BATCHNO);
			strcpy(pgbl->dgtCard.BATCHNO,dgtFIELD31);
			bbzero(pgbl->dgtCard.SETDATE);
			strcpy(pgbl->dgtCard.SETDATE,dgtFIELD30);

			wrLog("中心流水号:%s",pgbl->Centerseqno);
			wrLog("gbl.dgtCard.AUTHSEQ:%s",pgbl->dgtCard.AUTHSEQ);
			wrLog("gbl.FeeAmt:%s",pgbl->FeeAmt);
			wrLog("gbl.MonthFeeAmt:%s",pgbl->MonthFeeAmt);
			wrLog("gbl.ReturnDeposit:%s",pgbl->ReturnDeposit);

		}

		break;
	case 3460://换卡完成
		
		break;
	default:
		break;
	}

	return 0;
}


CDGTBusTrade::init_dgt_pack( char *transMessage, char *tranType,CGlobal *pgbl)
{
	int j;
	int max_count=0;
	char AuthSeq_field[18+1];
	char deposit_field[8+1];
	char EDCARDID_field[16+1];
	char depositAmount[4+1];
	bbzero(depositAmount);
	int COMMAND_LEN;

	bbzero(EDCARDID_field);
	bbzero(deposit_field);
	bbzero(AuthSeq_field);
	sprintf(AuthSeq_field,"%s%s",pgbl->dgtCard.SETDATE,pgbl->dgtCard.AUTHSEQ);
	sprintf(EDCARDID_field,"%016s",pgbl->busi_id); //操作元卡号，跟操作元号一致

	memset(&Pack_dgt_table, 0, sizeof(Pack_dgt_table));

	if (strcmp(transMessage,"3458")==0  || strcmp(transMessage,"3481")==0) //如果是卡管理 
	{
			strcpy(deposit_field,pgbl->dgtM1Card.deposit); //取押金
	}

	else  //3480 表示结果获取报文 押金填cpu卡的押金
	{
		
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.deposit,(BYTE *) depositAmount,2);
		
		if (strcmp(depositAmount,"FFFF")==0 ) //如果没有押金
		{
			strcpy(deposit_field,"00000000");
			
		}
		else
		{
			//如果押金
			char deposit_buffer[2+1];
			bbzero(deposit_buffer);
			memcpy(deposit_buffer,depositAmount+2,2);
			bbzero(deposit_field);
			sprintf(deposit_field,"%d",(atoi(deposit_buffer)*100));
			
		}


	}



	switch (atoi(transMessage))
	{

	case 2051: //签到

	//签到请求
	Pack_dgt_table[0].Type = NUMERIC;			Pack_dgt_table[0].Format = FIXED;			Pack_dgt_table[0].Max_Length = 8;		Pack_dgt_table[0].optionFlag='M';	//Unitid
	Pack_dgt_table[1].Type = NUMERIC;			Pack_dgt_table[1].Format = FIXED;			Pack_dgt_table[1].Max_Length = 2;		Pack_dgt_table[1].optionFlag='M';	//交易方式
	Pack_dgt_table[2].Type = NUMERIC;			Pack_dgt_table[2].Format = FIXED;			Pack_dgt_table[2].Max_Length = 16;		Pack_dgt_table[2].optionFlag='C';	//SAM卡号
	Pack_dgt_table[3].Type = NUMERIC;			Pack_dgt_table[3].Format = FIXED;			Pack_dgt_table[3].Max_Length = 16;		Pack_dgt_table[3].optionFlag='C';	//SAM卡授权信息
	Pack_dgt_table[4].Type = NUMERIC;			Pack_dgt_table[4].Format = FIXED;			Pack_dgt_table[4].Max_Length = 16;		Pack_dgt_table[4].optionFlag='C';	//ED卡授权信息
	Pack_dgt_table[5].Type = NUMERIC;			Pack_dgt_table[5].Format = FIXED;			Pack_dgt_table[5].Max_Length = 12;		Pack_dgt_table[5].optionFlag='M';	//清算设备代码
	Pack_dgt_table[6].Type = NUMERIC; 			Pack_dgt_table[6].Format = FIXED;			Pack_dgt_table[6].Max_Length = 12;		Pack_dgt_table[6].optionFlag='M';	//企业设备代码
	Pack_dgt_table[7].Type = NUMERIC; 			Pack_dgt_table[7].Format = FIXED;			Pack_dgt_table[7].Max_Length = 16;		Pack_dgt_table[7].optionFlag='M';	//操作员号
	Pack_dgt_table[8].Type = NUMERIC;			Pack_dgt_table[8].Format = FIXED;  			Pack_dgt_table[8].Max_Length = 16;		Pack_dgt_table[8].optionFlag='C';	//操作主卡卡号
	Pack_dgt_table[9].Type = NUMERIC;			Pack_dgt_table[9].Format = FIXED;			Pack_dgt_table[9].Max_Length = 8;		Pack_dgt_table[9].optionFlag='C';	//结算日期 全0
	Pack_dgt_table[10].Type = NUMERIC;			Pack_dgt_table[10].Format = FIXED;			Pack_dgt_table[10].Max_Length = 6;		Pack_dgt_table[10].optionFlag='C';	//批次号
	Pack_dgt_table[11].Type = NUMERIC;			Pack_dgt_table[11].Format = FIXED;			Pack_dgt_table[11].Max_Length = 14;		Pack_dgt_table[11].optionFlag='C';	//中心时间，可用于设备校正时间,全填0
	Pack_dgt_table[12].Type = NUMERIC;			Pack_dgt_table[12].Format = FIXED;			Pack_dgt_table[12].Max_Length = 24;		Pack_dgt_table[12].optionFlag='C';	//授权认证码（M1）
	Pack_dgt_table[13].Type = NUMERIC;			Pack_dgt_table[13].Format = FIXED;			Pack_dgt_table[13].Max_Length = 32;		Pack_dgt_table[13].optionFlag='C';	//参数更新标志位
	Pack_dgt_table[14].Type = ALPHANUM;			Pack_dgt_table[14].Format = FIXED;			Pack_dgt_table[14].Max_Length = 32;		Pack_dgt_table[14].optionFlag='C';	//密钥(MAC密钥)(联机)
	Pack_dgt_table[15].Type = ALPHANUM;			Pack_dgt_table[15].Format = FIXED;			Pack_dgt_table[15].Max_Length = 20;		Pack_dgt_table[15].optionFlag='C';	//保留域
	Pack_dgt_table[16].Type = NUMERIC;			Pack_dgt_table[16].Format = FIXED;			Pack_dgt_table[16].Max_Length = 5;		Pack_dgt_table[16].optionFlag='C';	//交易应答码
	
	strcpy(Pack_dgt_table[0].name,"Unitid");
	strcpy(Pack_dgt_table[1].name,"TxnMode");
	strcpy(Pack_dgt_table[2].name,"SAMID");
	strcpy(Pack_dgt_table[3].name,"SAMAuthInfo");
	strcpy(Pack_dgt_table[4].name,"EDAuthInfo");
	strcpy(Pack_dgt_table[5].name,"Posid");
	strcpy(Pack_dgt_table[6].name,"Termid");
	strcpy(Pack_dgt_table[7].name,"Operid");
	strcpy(Pack_dgt_table[8].name,"EDCardId");
	strcpy(Pack_dgt_table[9].name,"SettDate");
	strcpy(Pack_dgt_table[10].name,"BatchNo");
	strcpy(Pack_dgt_table[11].name,"SysDatetime");
	strcpy(Pack_dgt_table[12].name,"Authcode");
	strcpy(Pack_dgt_table[13].name,"ParamBit");
	strcpy(Pack_dgt_table[14].name,"KeySet");
	strcpy(Pack_dgt_table[15].name,"Reserved");
	strcpy(Pack_dgt_table[16].name,"RSPCODE");

	initField();
	strcpy(dgtFIELD0,DGT_Unitid);
	strcpy(dgtFIELD1,"01");//交易方式，01表示联机，00表示脱机
	strcpy(dgtFIELD5,pgbl->posId);
	strcpy(dgtFIELD6,pgbl->busi_id);
	strcpy(dgtFIELD7,pgbl->busi_id);


	Pack_dgt_table[0].Rev_Data = dgtFIELD0;			
	Pack_dgt_table[0].Send_Data = dgtFIELD0;

	Pack_dgt_table[1].Rev_Data = dgtFIELD1;			Pack_dgt_table[1].Send_Data = dgtFIELD1;
	Pack_dgt_table[2].Rev_Data = dgtFIELD2;			Pack_dgt_table[2].Send_Data = dgtFIELD2;
	Pack_dgt_table[3].Rev_Data = dgtFIELD3;			Pack_dgt_table[3].Send_Data = dgtFIELD3;
	Pack_dgt_table[4].Rev_Data = dgtFIELD4;			Pack_dgt_table[4].Send_Data= dgtFIELD4;
	Pack_dgt_table[5].Rev_Data = dgtFIELD5;			Pack_dgt_table[5].Send_Data = dgtFIELD5;
	Pack_dgt_table[6].Rev_Data = dgtFIELD6;			Pack_dgt_table[6].Send_Data = dgtFIELD6;
	Pack_dgt_table[7].Rev_Data = dgtFIELD7;			Pack_dgt_table[7].Send_Data = dgtFIELD7;
	Pack_dgt_table[8].Rev_Data = dgtFIELD8;			Pack_dgt_table[8].Send_Data = dgtFIELD8;
	Pack_dgt_table[9].Rev_Data = dgtFIELD9;			Pack_dgt_table[9].Send_Data = dgtFIELD9;
	Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
	Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
	Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
	Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
	Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
	Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
	Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;

	break;

	case 2061: //卡操作请求

		//卡操作请求 中参数赋值
		max_count=42;
		initField();

		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->posId);
		strcpy(dgtFIELD3,pgbl->dgt_posSerial);//PosSequence
		strcpy(dgtFIELD4,pgbl->busi_id);//Termid
		strcpy(dgtFIELD5,pgbl->busi_id);//Operid
		strcpy(dgtFIELD6,EDCARDID_field); 

		if (strcmp(tranType,"53")==0) //转余额充值
		{
			strcpy(dgtFIELD7,"3460");

		}
		else if (strcmp(tranType,"52") ==0 || strcmp(tranType,"80")) // 换卡售卡及普通售卡
		{

			strcpy(dgtFIELD7,"2063");

		}
		else  //其他 02
		{
		
			strcpy(dgtFIELD7,"2062");
		}

		CTools::byte_to_hex((BYTE *)pgbl->dgtCard.CItyCode,(BYTE *)dgtFIELD8,2);
		strcpy(dgtFIELD9,pgbl->dgtCard.cardShowId);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardParent_type,(BYTE *)dgtFIELD10,1);
		CTools::byte_to_hex((BYTE *)pgbl->dgtCard.cardChild_type,(BYTE *)dgtFIELD11,1);
		strcpy(dgtFIELD12,"1"); //1表示CPU卡
		strcpy(dgtFIELD13,tranType); //卡交易类型 默认未开卡

		strcpy(dgtFIELD21,deposit_field);
		strcpy(dgtFIELD37,pgbl->dgtSignInfo.BATCHNO); //修改成签到返回的批次号
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD24,4);
		memcpy(dgtFIELD26,pgbl->dgtCard.cardMny+4,8);
		strcpy(dgtFIELD30,pgbl->dgtCard.CARDRAND);
	
		if (strcmp(tranType,"02")==0 || strcmp(tranType,"53")==0) //普通充值和换卡转移资金充值
		{
			//如果是充值
			strcpy(dgtFIELD27,pgbl->dgtCard.CARDSEQ);
			strcpy(dgtFIELD28,pgbl->dgtCard.KEYVER);
			strcpy(dgtFIELD29,pgbl->dgtCard.ALGIND);
			strcpy(dgtFIELD31,pgbl->dgtCard.MAC1);
			strcpy(dgtFIELD22,pgbl->dgtCard.tradeMny); //取后8位数
			strcpy(dgtFIELD23,pgbl->dgtCard.tradeMny);
			CTools::byte_to_hex((BYTE *)pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD24,4);

			strcpy(dgtFIELD28,pgbl->dgtCard.KEYVER);
			strcpy(dgtFIELD29,pgbl->dgtCard.ALGIND);
			//卡片command 长度是0
			strcpy(dgtFIELD32,"000");
			bbzero(pgbl->dgtCard.CHARGE_DIVDATA);
			CTools::byte_to_hex((BYTE *) pgbl->dgtCard.appSerial,(BYTE *)pgbl->dgtCard.CHARGE_DIVDATA,8); //充值的时候离散因子是卡号
			strcpy(dgtFIELD35,pgbl->dgtCard.CHARGE_DIVDATA);
			strcpy(dgtFIELD40,pgbl->dgtCard.CHARGE_DIVDATA);

		}
		else
		{
			//如果是开卡		
			COMMAND_LEN=strlen(pgbl->dgtCard.APDU);
			sprintf(dgtFIELD32,"%03d",COMMAND_LEN);
			strcpy(dgtFIELD33,pgbl->dgtCard.APDU);
			strcpy(dgtFIELD35,pgbl->dgtCard.CARDSALE_DIVDATA);
			strcpy(dgtFIELD40,pgbl->dgtCard.CARDSALE_DIVDATA);
			memcpy(dgtFIELD22,pgbl->amount+4,8); //取后8位数 //应收金额
			strcpy(dgtFIELD23,pgbl->dgtCard.tradeMny);
			CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD24,4);


		}
	
		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;		Pack_dgt_table[0].Max_Length = 8;       Pack_dgt_table[0].optionFlag='M';	//Unitid
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;		Pack_dgt_table[1].Max_Length = 12;		Pack_dgt_table[1].optionFlag='M';	//Posid
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;		Pack_dgt_table[2].Max_Length = 16;		Pack_dgt_table[2].optionFlag='C';	//SamId
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;		Pack_dgt_table[3].Max_Length = 9;		Pack_dgt_table[3].optionFlag='M';	//POS机流水号
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;		Pack_dgt_table[4].Max_Length = 12;		Pack_dgt_table[4].optionFlag='M';	//企业设备代码
		Pack_dgt_table[5].Type = NUMERIC;		Pack_dgt_table[5].Format = FIXED;		Pack_dgt_table[5].Max_Length = 16;		Pack_dgt_table[5].optionFlag='M';	//操作员号
		Pack_dgt_table[6].Type = NUMERIC; 		Pack_dgt_table[6].Format = FIXED;		Pack_dgt_table[6].Max_Length = 16;		Pack_dgt_table[6].optionFlag='C';	//操作主卡卡号
		Pack_dgt_table[7].Type = NUMERIC; 		Pack_dgt_table[7].Format = FIXED;		Pack_dgt_table[7].Max_Length = 4;		Pack_dgt_table[7].optionFlag='M';	//卡操作交易类型
		Pack_dgt_table[8].Type = NUMERIC;	    Pack_dgt_table[8].Format = FIXED;  		Pack_dgt_table[8].Max_Length = 4;		Pack_dgt_table[8].optionFlag='M';	//城市代码
		Pack_dgt_table[9].Type = NUMERIC;	    Pack_dgt_table[9].Format = FIXED;	    Pack_dgt_table[9].Max_Length = 16;		Pack_dgt_table[9].optionFlag='M';	//卡内号
	
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;		Pack_dgt_table[10].Max_Length = 2;		Pack_dgt_table[10].optionFlag='M';	//主卡类型
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;		Pack_dgt_table[11].Max_Length = 2;		Pack_dgt_table[11].optionFlag='M';	//子卡类型
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;		Pack_dgt_table[12].Max_Length = 1;		Pack_dgt_table[12].optionFlag='M';	//卡型， 1-CPU卡
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;		Pack_dgt_table[13].Max_Length = 2;		Pack_dgt_table[13].optionFlag='M';	//业务类型
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;		Pack_dgt_table[14].Max_Length = 16;		Pack_dgt_table[14].optionFlag='C';	//卡物理标示编号
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;		Pack_dgt_table[15].Max_Length = 8;		Pack_dgt_table[15].optionFlag='C';	//卡校验码（MF1）
		Pack_dgt_table[16].Type = NUMERIC;		Pack_dgt_table[16].Format = FIXED;	Pack_dgt_table[16].Max_Length = 14;			Pack_dgt_table[16].optionFlag='C';	//中心时间，可用于设备校正时间
		Pack_dgt_table[17].Type = NUMERIC;		Pack_dgt_table[17].Format = FIXED;	Pack_dgt_table[17].Max_Length = 2;			Pack_dgt_table[17].optionFlag='C';	//下发key的数量(M1)
		Pack_dgt_table[18].Type = NUMERIC;		Pack_dgt_table[18].Format = FIXED;	Pack_dgt_table[18].Max_Length = 4;			Pack_dgt_table[18].optionFlag='C';	//SectorID
		Pack_dgt_table[19].Type = NUMERIC;		Pack_dgt_table[19].Format = FIXED;	Pack_dgt_table[19].Max_Length = 64;			Pack_dgt_table[19].optionFlag='C';	//KeySet 密钥(KeyB)(M1)
		
		Pack_dgt_table[20].Type = NUMERIC;		Pack_dgt_table[20].Format = FIXED;    Pack_dgt_table[20].Max_Length = 32;		Pack_dgt_table[20].optionFlag='C';	//TACKeySet
		Pack_dgt_table[21].Type = NUMERIC;		Pack_dgt_table[21].Format = FIXED;	Pack_dgt_table[21].Max_Length = 8;			Pack_dgt_table[21].optionFlag='M';	//Deposit
		Pack_dgt_table[22].Type = NUMERIC;		Pack_dgt_table[22].Format = FIXED ;	Pack_dgt_table[22].Max_Length = 8;			Pack_dgt_table[22].optionFlag='M';	//OrigAmt
		Pack_dgt_table[23].Type = NUMERIC;		Pack_dgt_table[23].Format = FIXED;	Pack_dgt_table[23].Max_Length = 8;			Pack_dgt_table[23].optionFlag='M';	//ReloadBal
		Pack_dgt_table[24].Type = NUMERIC;		Pack_dgt_table[24].Format = FIXED;	Pack_dgt_table[24].Max_Length = 8;			Pack_dgt_table[24].optionFlag='M';	//CardValDate
		Pack_dgt_table[25].Type = ALPHANUMS;	Pack_dgt_table[25].Format = FIXED;	Pack_dgt_table[25].Max_Length = 8;			Pack_dgt_table[25].optionFlag='C';	//MAC2
		Pack_dgt_table[26].Type = NUMERIC;		Pack_dgt_table[26].Format = FIXED;	Pack_dgt_table[26].Max_Length = 8;			Pack_dgt_table[26].optionFlag='M';	//SrcBal
		Pack_dgt_table[27].Type = NUMERIC;	 Pack_dgt_table[27].Format = FIXED;		Pack_dgt_table[27].Max_Length = 6;				Pack_dgt_table[27].optionFlag='M';	//CardSeq
		Pack_dgt_table[28].Type = NUMERIC;	 Pack_dgt_table[28].Format = FIXED;	Pack_dgt_table[28].Max_Length = 2;				Pack_dgt_table[28].optionFlag='M';	//KeyVer
		Pack_dgt_table[29].Type = NUMERIC;	Pack_dgt_table[29].Format = FIXED;	Pack_dgt_table[29].Max_Length = 2;				Pack_dgt_table[29].optionFlag='M';	//AlgInd
		
		Pack_dgt_table[30].Type = NUMERIC;  Pack_dgt_table[30].Format = FIXED;	Pack_dgt_table[30].Max_Length = 8;				Pack_dgt_table[30].optionFlag='M';	//CardRand
		Pack_dgt_table[31].Type = NUMERIC;	Pack_dgt_table[31].Format = FIXED;	Pack_dgt_table[31].Max_Length = 8;				Pack_dgt_table[31].optionFlag='M';	//MAC1
		Pack_dgt_table[32].Type = NUMERIC;  Pack_dgt_table[32].Format = FIXED;	Pack_dgt_table[32].Max_Length = 3;				Pack_dgt_table[32].optionFlag='C';	//CommandLen
		Pack_dgt_table[33].Type = ALPHANUMS;	Pack_dgt_table[33].Format = LLLVAR;	Pack_dgt_table[33].Max_Length = 999;		Pack_dgt_table[33].optionFlag='C';	//Command
		Pack_dgt_table[34].Type = ALPHANUMS;  Pack_dgt_table[34].Format = FIXED;	Pack_dgt_table[34].Max_Length = 16;				Pack_dgt_table[34].optionFlag='C';	//DicMAC
		Pack_dgt_table[35].Type = ALPHANUMS;	Pack_dgt_table[35].Format = FIXED;	Pack_dgt_table[35].Max_Length = 18;		Pack_dgt_table[35].optionFlag='C';	//DivData
		Pack_dgt_table[36].Type = ALPHANUMS;  Pack_dgt_table[36].Format = FIXED;	Pack_dgt_table[36].Max_Length = 8;				Pack_dgt_table[36].optionFlag='C';	//SettDate
		Pack_dgt_table[37].Type = ALPHANUMS;	Pack_dgt_table[37].Format = FIXED;	Pack_dgt_table[37].Max_Length = 6;		Pack_dgt_table[37].optionFlag='M';	//BatchNo
		Pack_dgt_table[38].Type = NUMERIC;	Pack_dgt_table[38].Format = FIXED;		Pack_dgt_table[38].Max_Length = 10;		Pack_dgt_table[38].optionFlag='C';	//密钥授权流水号
		Pack_dgt_table[39].Type = NUMERIC;	Pack_dgt_table[39].Format = FIXED;		Pack_dgt_table[39].Max_Length = 10;		Pack_dgt_table[39].optionFlag='C';	//额度授权流水号
	
		Pack_dgt_table[40].Type = ALPHANUMS;	Pack_dgt_table[40].Format = FIXED;	Pack_dgt_table[40].Max_Length = 20;		Pack_dgt_table[40].optionFlag='C';	//保留域
		Pack_dgt_table[41].Type = ALPHANUMS;	Pack_dgt_table[41].Format = FIXED;	Pack_dgt_table[41].Max_Length = 5;		Pack_dgt_table[41].optionFlag='C';	//交易应答码
	
		Pack_dgt_table[0].Rev_Data = dgtFIELD0;		Pack_dgt_table[0].Send_Data = dgtFIELD0;
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;		Pack_dgt_table[1].Send_Data = dgtFIELD1;
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;		Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;		Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;		Pack_dgt_table[4].Send_Data= dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;		Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;		Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;		Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;		Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;		Pack_dgt_table[9].Send_Data = dgtFIELD9;

		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;		Pack_dgt_table[19].Send_Data = dgtFIELD19;
		
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;	Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;	Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;	Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;	Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;	Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;	Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;	Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;	Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;	Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;	Pack_dgt_table[29].Send_Data = dgtFIELD29;

		Pack_dgt_table[30].Rev_Data = dgtFIELD30;	Pack_dgt_table[30].Send_Data = dgtFIELD30;
		Pack_dgt_table[31].Rev_Data = dgtFIELD31;	Pack_dgt_table[31].Send_Data = dgtFIELD31;
		Pack_dgt_table[32].Rev_Data = dgtFIELD32;	Pack_dgt_table[32].Send_Data = dgtFIELD32;
		Pack_dgt_table[33].Rev_Data = dgtFIELD33;	Pack_dgt_table[33].Send_Data = dgtFIELD33;
		Pack_dgt_table[34].Rev_Data=dgtFIELD34;		Pack_dgt_table[34].Send_Data=	dgtFIELD34;
		Pack_dgt_table[35].Rev_Data=dgtFIELD35;		Pack_dgt_table[35].Send_Data=	dgtFIELD35;
		Pack_dgt_table[36].Rev_Data=dgtFIELD36;		Pack_dgt_table[36].Send_Data=	dgtFIELD36;
		Pack_dgt_table[37].Rev_Data=dgtFIELD37;		Pack_dgt_table[37].Send_Data=	dgtFIELD37;
		Pack_dgt_table[38].Rev_Data=dgtFIELD38;		Pack_dgt_table[38].Send_Data=	dgtFIELD38;
		Pack_dgt_table[39].Rev_Data=dgtFIELD39;		Pack_dgt_table[39].Send_Data=	dgtFIELD39;
	
		Pack_dgt_table[40].Rev_Data=dgtFIELD40;		Pack_dgt_table[40].Send_Data=	dgtFIELD40;
		Pack_dgt_table[41].Rev_Data=dgtFIELD41;		Pack_dgt_table[41].Send_Data=	dgtFIELD41;

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"Posid");
		strcpy(Pack_dgt_table[2].name,"SamId");
		strcpy(Pack_dgt_table[3].name,"PosSequence");
		strcpy(Pack_dgt_table[4].name,"Termid");
		strcpy(Pack_dgt_table[5].name,"Operid");
		strcpy(Pack_dgt_table[6].name,"EDCardId");
		strcpy(Pack_dgt_table[7].name,"CardOprType");
		strcpy(Pack_dgt_table[8].name,"CItyCode");
		strcpy(Pack_dgt_table[9].name,"CardId");

		strcpy(Pack_dgt_table[10].name,"CardMKnd");
		strcpy(Pack_dgt_table[11].name,"CardSKnd");
		strcpy(Pack_dgt_table[12].name,"CardModel");
		strcpy(Pack_dgt_table[13].name,"TransType");
		strcpy(Pack_dgt_table[14].name,"CardMac");
		strcpy(Pack_dgt_table[15].name,"CardCSN");
		strcpy(Pack_dgt_table[16].name,"SysDatetime");
		strcpy(Pack_dgt_table[17].name,"KeyCnt");
		strcpy(Pack_dgt_table[18].name,"SectorID");
		strcpy(Pack_dgt_table[19].name,"KeySet");

		strcpy(Pack_dgt_table[20].name,"TACKeySet");
		strcpy(Pack_dgt_table[21].name,"Deposit");
		strcpy(Pack_dgt_table[22].name,"OrigAmt");
		strcpy(Pack_dgt_table[23].name,"ReloadBal");
		strcpy(Pack_dgt_table[24].name,"CardValDate");
		strcpy(Pack_dgt_table[25].name,"MAC2");
		strcpy(Pack_dgt_table[26].name,"SrcBal");
		strcpy(Pack_dgt_table[27].name,"CardSeq");
		strcpy(Pack_dgt_table[28].name,"KeyVer");
		strcpy(Pack_dgt_table[29].name,"AlgInd");

		strcpy(Pack_dgt_table[30].name,"CardRand");
		strcpy(Pack_dgt_table[31].name,"MAC1");
		strcpy(Pack_dgt_table[32].name,"CommandLen");
		strcpy(Pack_dgt_table[33].name,"Command");
		strcpy(Pack_dgt_table[34].name,"DicMAC");
		strcpy(Pack_dgt_table[35].name,"DivData");
		strcpy(Pack_dgt_table[36].name,"SettDate");
		strcpy(Pack_dgt_table[37].name,"BatchNo");
		strcpy(Pack_dgt_table[38].name,"AuthSeq");
		strcpy(Pack_dgt_table[39].name,"LimitedAuthSeql");

		strcpy(Pack_dgt_table[40].name,"Reserved");
		strcpy(Pack_dgt_table[41].name,"rspcode");
		wrLog("pack_dgt_table:%s",Pack_dgt_table[0].Send_Data);

		break;
	case 2063:
		//售卡
		
		max_count=30;

		initField();
		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->dgt_LocalSequence); //流水号
		strcpy(dgtFIELD2,tranType);
		strcpy(dgtFIELD3,pgbl->posId);
		strcpy(dgtFIELD5,pgbl->dgt_posSerial);
		strcpy(dgtFIELD6,pgbl->busi_id);
		strcpy(dgtFIELD7,pgbl->busi_id);
		strcpy(dgtFIELD8,EDCARDID_field);

		strcpy(dgtFIELD9,pgbl->dgtCard.cardShowId);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardParent_type,(BYTE *)dgtFIELD11,1);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.cardChild_type,(BYTE *)dgtFIELD12,1);

		strcpy(dgtFIELD13,"1");
		strcpy(dgtFIELD14,pgbl->dgtCard.sale_type); //售卡类型

		strcpy(dgtFIELD15,deposit_field);
		strcpy(dgtFIELD16,pgbl->dgtCard.tradeBeForeMny);
		strcpy(dgtFIELD17,pgbl->dgtCard.tradeMny);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *)dgtFIELD18,4);
		strcpy(dgtFIELD19,pgbl->dgtCard.tradeDate);
		strcpy(dgtFIELD20,pgbl->dgtCard.tradeTime);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.CItyCode,(BYTE *) dgtFIELD21,2);
		strcpy(dgtFIELD23,pgbl->dgtCard.SETDATE);
		strcpy(dgtFIELD24,pgbl->dgtSignInfo.BATCHNO); //修改为签到返回的批次号
		strcpy(dgtFIELD25,AuthSeq_field);
		strcpy(dgtFIELD26,pgbl->dgtCard.LIMITEAUTHSEQL);

		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;		Pack_dgt_table[0].Max_Length = 8;		Pack_dgt_table[0].optionFlag='M';	//Unitid
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;		Pack_dgt_table[1].Max_Length = 10;		Pack_dgt_table[1].optionFlag='M';	//LocalSequence
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;		Pack_dgt_table[2].Max_Length = 2;		Pack_dgt_table[2].optionFlag='M';	//TransType
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;		Pack_dgt_table[3].Max_Length = 12;		Pack_dgt_table[3].optionFlag='M';	//PosId
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;		Pack_dgt_table[4].Max_Length = 16;		Pack_dgt_table[4].optionFlag='C';	//SamId
		Pack_dgt_table[5].Type = NUMERIC;		Pack_dgt_table[5].Format = FIXED;		Pack_dgt_table[5].Max_Length = 9;		Pack_dgt_table[5].optionFlag='M';	//PosSequence
		Pack_dgt_table[6].Type = NUMERIC; 		Pack_dgt_table[6].Format = FIXED;	    Pack_dgt_table[6].Max_Length = 12;		Pack_dgt_table[6].optionFlag='M';	//TermId
		Pack_dgt_table[7].Type = NUMERIC; 		Pack_dgt_table[7].Format = FIXED;	    Pack_dgt_table[7].Max_Length = 16;		Pack_dgt_table[7].optionFlag='M';	//OperId
		Pack_dgt_table[8].Type = NUMERIC;	    Pack_dgt_table[8].Format = FIXED;		Pack_dgt_table[8].Max_Length = 16;		Pack_dgt_table[8].optionFlag='C';	//EdCardId
		Pack_dgt_table[9].Type = NUMERIC;	    Pack_dgt_table[9].Format = FIXED;	    Pack_dgt_table[9].Max_Length = 16;		Pack_dgt_table[9].optionFlag='M';	//CardId
	
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;		Pack_dgt_table[10].Max_Length = 6;		Pack_dgt_table[10].optionFlag='M';	//CardCnt
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;		Pack_dgt_table[11].Max_Length = 2;		Pack_dgt_table[11].optionFlag='M';	//CardMKnd
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;		Pack_dgt_table[12].Max_Length = 2;		Pack_dgt_table[12].optionFlag='M';	//CardSKind
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;		Pack_dgt_table[13].Max_Length = 1;		Pack_dgt_table[13].optionFlag='M';	//CardModel
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;		Pack_dgt_table[14].Max_Length = 2;		Pack_dgt_table[14].optionFlag='M';	//SaleMode
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;		Pack_dgt_table[15].Max_Length = 8;		Pack_dgt_table[15].optionFlag='M';	//Deposit
		Pack_dgt_table[16].Type = NUMERIC;	Pack_dgt_table[16].Format = FIXED;			Pack_dgt_table[16].Max_Length = 8; 		Pack_dgt_table[16].optionFlag='M';	//BefBalance
		Pack_dgt_table[17].Type = NUMERIC;	Pack_dgt_table[17].Format = FIXED;			Pack_dgt_table[17].Max_Length = 8;		Pack_dgt_table[17].optionFlag='M';	//TxnAmt
		Pack_dgt_table[18].Type = NUMERIC;	Pack_dgt_table[18].Format = FIXED;			Pack_dgt_table[18].Max_Length = 8; 		Pack_dgt_table[18].optionFlag='M';	//CardValDate
		Pack_dgt_table[19].Type = NUMERIC;	Pack_dgt_table[19].Format = FIXED;			Pack_dgt_table[19].Max_Length = 8; 		Pack_dgt_table[19].optionFlag='M';	//TxnDate
		
		Pack_dgt_table[20].Type = NUMERIC;	Pack_dgt_table[20].Format = FIXED;			Pack_dgt_table[20].Max_Length = 6;		Pack_dgt_table[20].optionFlag='M';	//TxnTime
		Pack_dgt_table[21].Type = NUMERIC;	Pack_dgt_table[21].Format = FIXED;			Pack_dgt_table[21].Max_Length = 4;		Pack_dgt_table[21].optionFlag='M';	//CityCode
		Pack_dgt_table[22].Type = NUMERIC;	Pack_dgt_table[22].Format = FIXED ;			Pack_dgt_table[22].Max_Length = 2;		Pack_dgt_table[22].optionFlag='M';	//CardVerNo
		Pack_dgt_table[23].Type = NUMERIC;	Pack_dgt_table[23].Format = FIXED;			Pack_dgt_table[23].Max_Length = 8;		Pack_dgt_table[23].optionFlag='M';	//SettDate
		Pack_dgt_table[24].Type = NUMERIC;  Pack_dgt_table[24].Format = FIXED;			Pack_dgt_table[24].Max_Length = 6;		Pack_dgt_table[24].optionFlag='M';	//BatchNo
		Pack_dgt_table[25].Type = NUMERIC;		Pack_dgt_table[25].Format = FIXED;		Pack_dgt_table[25].Max_Length = 18;		Pack_dgt_table[25].optionFlag='M';	//AuthSeq
		Pack_dgt_table[26].Type = NUMERIC;	Pack_dgt_table[26].Format = FIXED;			Pack_dgt_table[26].Max_Length = 10;		Pack_dgt_table[26].optionFlag='M';	//LimitedAuthSeq
		Pack_dgt_table[27].Type = ALPHANUMS;  Pack_dgt_table[27].Format = FIXED;		Pack_dgt_table[27].Max_Length = 20;		Pack_dgt_table[27].optionFlag='M';	//Reserved
		Pack_dgt_table[28].Type = NUMERIC;  Pack_dgt_table[28].Format = FIXED;			Pack_dgt_table[28].Max_Length = 8;		Pack_dgt_table[28].optionFlag='M';	//TAC
		Pack_dgt_table[29].Type = NUMERIC;  Pack_dgt_table[29].Format = FIXED;			Pack_dgt_table[29].Max_Length = 5;		Pack_dgt_table[29].optionFlag='M';	//Response Code
		

		Pack_dgt_table[0].Rev_Data = dgtFIELD0;			Pack_dgt_table[0].Send_Data = dgtFIELD0;
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;			Pack_dgt_table[1].Send_Data = dgtFIELD1;
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;			Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;			Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;			Pack_dgt_table[4].Send_Data = dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;			Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;			Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;			Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;			Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;			Pack_dgt_table[9].Send_Data = dgtFIELD9;
		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;	Pack_dgt_table[19].Send_Data = dgtFIELD19;
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;	Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;	Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;	Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;	Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;	Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;	Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;	Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;	Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;	Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;	Pack_dgt_table[29].Send_Data = dgtFIELD29;

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"LocalSequence");
		strcpy(Pack_dgt_table[2].name,"TransType");
		strcpy(Pack_dgt_table[3].name,"PosId");
		strcpy(Pack_dgt_table[4].name,"SamId");
		strcpy(Pack_dgt_table[5].name,"PosSequence");
		strcpy(Pack_dgt_table[6].name,"TermId");
		strcpy(Pack_dgt_table[7].name,"OperId");
		strcpy(Pack_dgt_table[8].name,"EdCardId");
		strcpy(Pack_dgt_table[9].name,"CardId");
		strcpy(Pack_dgt_table[10].name,"CardCnt");
		strcpy(Pack_dgt_table[11].name,"CardMKnd");
		strcpy(Pack_dgt_table[12].name,"CardSKind");
		strcpy(Pack_dgt_table[13].name,"CardModel");
		strcpy(Pack_dgt_table[14].name,"SaleMode");
		strcpy(Pack_dgt_table[15].name,"Deposit");
		strcpy(Pack_dgt_table[16].name,"BefBalance");
		strcpy(Pack_dgt_table[17].name,"TxnAmt");
		strcpy(Pack_dgt_table[18].name,"CardValDate");
		strcpy(Pack_dgt_table[19].name,"TxnDate");
		strcpy(Pack_dgt_table[20].name,"TxnTime");
		strcpy(Pack_dgt_table[21].name,"CityCode");
		strcpy(Pack_dgt_table[22].name,"CardVerNo");
		strcpy(Pack_dgt_table[23].name,"SettDate");
		strcpy(Pack_dgt_table[24].name,"BatchNo");
		strcpy(Pack_dgt_table[25].name,"AuthSeq");
		strcpy(Pack_dgt_table[26].name,"LimitedAuthSeq");
		strcpy(Pack_dgt_table[27].name,"Reserved");
		strcpy(Pack_dgt_table[28].name,"TAC");
		strcpy(Pack_dgt_table[29].name,"RSPCODE");

		break;

	case 3458: //换卡请求 
	case 3480: //结果获取
	case 3481: //结果获取撤销

		//均为卡管理 操作

		max_count=39;
		
		initField();
		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->dgt_LocalSequence);
		strcpy(dgtFIELD2,tranType);
		strcpy(dgtFIELD4,pgbl->posId);
		strcpy(dgtFIELD5,pgbl->dgt_posSerial);
		strcpy(dgtFIELD6,pgbl->busi_id);
		strcpy(dgtFIELD7,pgbl->busi_id);
		strcpy(dgtFIELD8,EDCARDID_field);
		strcpy(dgtFIELD10,pgbl->dgtM1Card.bus_showCard);
	
		CTools::addZero(pgbl->dgtM1Card.bus_showCard,16,1,dgtFIELD11);
		strcpy(dgtFIELD12,pgbl->dgtM1Card.city_code);
		strcpy(dgtFIELD13,pgbl->dgtM1Card.card_parent_type);
		strcpy(dgtFIELD14,pgbl->dgtM1Card.card_child_type);
		strcpy(dgtFIELD15,"3"); //卡片类型。是3 M1
		strcpy(dgtFIELD16,pgbl->dgtM1Card.openDate); //开卡时间
		strcpy(dgtFIELD18,pgbl->dgtM1Card.cardVadate);
		strcpy(dgtFIELD20,pgbl->dgtM1Card.bus_amt);
		strcpy(dgtFIELD21,deposit_field);
		strcpy(dgtFIELD22,"0");
		strcpy(dgtFIELD23,"0");
		strcpy(dgtFIELD31,pgbl->dgtSignInfo.BATCHNO); //修改为签到获取的批次号
		strcpy(dgtFIELD34,pgbl->dgtCard.tradeDate);
		strcpy(dgtFIELD35,pgbl->dgtCard.tradeTime);

		if (strcmp(transMessage,"3458")!=0) //如果是换卡申请
		{

			strcpy(dgtFIELD3,pgbl->Centerseqno);
			strcpy(dgtFIELD24,pgbl->ReturnAmt);
			strcpy(dgtFIELD25,pgbl->ReturnDeposit);
			
			strcpy(dgtFIELD26,pgbl->MonthFeeFlag);
			strcpy(dgtFIELD27,pgbl->FeeAmtFlag);
			strcpy(dgtFIELD28,pgbl->MonthFeeAmt);
			strcpy(dgtFIELD29,pgbl->FeeAmt);
			strcpy(dgtFIELD32,AuthSeq_field);

		}

		if (strcmp(transMessage,"3480")==0) //如果是发结果获取报文，该域填写3458(即申请报文的交易类型)，后补16个空格；
		{		
			bbzero(dgtFIELD36);
			CTools::addSpace("3458",20,0,dgtFIELD36); //后面补16个空格

		}
	
		//ANS16 CardCSN，右补空格；3480结果获取交易时，带申请交易类型（如：在换卡完成界面做结果获取，保留域中带3458），右补空格；


		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;	    Pack_dgt_table[0].Max_Length = 8;	Pack_dgt_table[0].optionFlag='M';	//Unitid	
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;	    Pack_dgt_table[1].Max_Length = 10;	Pack_dgt_table[1].optionFlag='M';	//LocalSequence	
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;	    Pack_dgt_table[2].Max_Length = 2;	Pack_dgt_table[2].optionFlag='M';	//TransType	
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;	    Pack_dgt_table[3].Max_Length = 18;	Pack_dgt_table[3].optionFlag='C';	//CenterSeqno	
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;	    Pack_dgt_table[4].Max_Length = 12;	Pack_dgt_table[4].optionFlag='M';	//Posid	
		Pack_dgt_table[5].Type = NUMERIC;		Pack_dgt_table[5].Format = FIXED;	    Pack_dgt_table[5].Max_Length = 9;	Pack_dgt_table[5].optionFlag='M';	//PosSequence	
		Pack_dgt_table[6].Type = NUMERIC;		Pack_dgt_table[6].Format = FIXED;	    Pack_dgt_table[6].Max_Length = 12;	Pack_dgt_table[6].optionFlag='M';	//TremId	
		Pack_dgt_table[7].Type = NUMERIC;		Pack_dgt_table[7].Format = FIXED;	    Pack_dgt_table[7].Max_Length = 16;	Pack_dgt_table[7].optionFlag='M';	//Operid	
		Pack_dgt_table[8].Type = NUMERIC;		Pack_dgt_table[8].Format = FIXED;	    Pack_dgt_table[8].Max_Length = 16;	Pack_dgt_table[8].optionFlag='M';	//EdCardId	
		Pack_dgt_table[9].Type = NUMERIC;		Pack_dgt_table[9].Format = FIXED;	    Pack_dgt_table[9].Max_Length = 16;	Pack_dgt_table[9].optionFlag='C';	//SamId	
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;	    Pack_dgt_table[10].Max_Length = 16;	Pack_dgt_table[10].optionFlag='M';	//CardFaceId	
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;	    Pack_dgt_table[11].Max_Length = 16;	Pack_dgt_table[11].optionFlag='M';	//CardId	
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;	    Pack_dgt_table[12].Max_Length = 4;	Pack_dgt_table[12].optionFlag='M';	//CityCode	
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;	    Pack_dgt_table[13].Max_Length = 2;	Pack_dgt_table[13].optionFlag='M';	//CardMKnd	
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;	    Pack_dgt_table[14].Max_Length = 2;	Pack_dgt_table[14].optionFlag='M';	//CardSKind	
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;	    Pack_dgt_table[15].Max_Length = 1;	Pack_dgt_table[15].optionFlag='M';	//CardModel	
		Pack_dgt_table[16].Type = NUMERIC;		Pack_dgt_table[16].Format = FIXED;	    Pack_dgt_table[16].Max_Length = 8;	Pack_dgt_table[16].optionFlag='M';	//CardSellDate	
		Pack_dgt_table[17].Type = ALPHANUM;		Pack_dgt_table[17].Format = FIXED;	    Pack_dgt_table[17].Max_Length = 8;	Pack_dgt_table[17].optionFlag='C';	//CardBusInessBit	
		Pack_dgt_table[18].Type = NUMERIC;		Pack_dgt_table[18].Format = FIXED;	    Pack_dgt_table[18].Max_Length = 8;	Pack_dgt_table[18].optionFlag='M';	//CardVal	
		Pack_dgt_table[19].Type = NUMERIC;		Pack_dgt_table[19].Format = FIXED;	    Pack_dgt_table[19].Max_Length = 6;	Pack_dgt_table[19].optionFlag='M';	//CardCnt	
		Pack_dgt_table[20].Type = NUMERIC;		Pack_dgt_table[20].Format = FIXED;	    Pack_dgt_table[20].Max_Length = 10;	Pack_dgt_table[20].optionFlag='M';	//Balance before	
		Pack_dgt_table[21].Type = NUMERIC;		Pack_dgt_table[21].Format = FIXED;	    Pack_dgt_table[21].Max_Length = 8;	Pack_dgt_table[21].optionFlag='M';	//Deposit amout	
		Pack_dgt_table[22].Type = NUMERIC;		Pack_dgt_table[22].Format = FIXED;	    Pack_dgt_table[22].Max_Length = 1;	Pack_dgt_table[22].optionFlag='M';	//ChipBadType
		Pack_dgt_table[23].Type = NUMERIC;		Pack_dgt_table[23].Format = FIXED;	    Pack_dgt_table[23].Max_Length = 1;	Pack_dgt_table[23].optionFlag='M';	//CardFaceBadType
		Pack_dgt_table[24].Type = NUMERIC;		Pack_dgt_table[24].Format = FIXED;	    Pack_dgt_table[24].Max_Length = 10;	Pack_dgt_table[24].optionFlag='C';	//Return amount
		Pack_dgt_table[25].Type = NUMERIC;		Pack_dgt_table[25].Format = FIXED;	    Pack_dgt_table[25].Max_Length = 8;	Pack_dgt_table[25].optionFlag='C';	//Return Deposit
		Pack_dgt_table[26].Type = NUMERIC;		Pack_dgt_table[26].Format = FIXED;	    Pack_dgt_table[26].Max_Length = 1;	Pack_dgt_table[26].optionFlag='C';	//Month Fee flag
		Pack_dgt_table[27].Type = NUMERIC;		Pack_dgt_table[27].Format = FIXED;	    Pack_dgt_table[27].Max_Length = 1;	Pack_dgt_table[27].optionFlag='C';	//Fee amount flag
		Pack_dgt_table[28].Type = NUMERIC;		Pack_dgt_table[28].Format = FIXED;	    Pack_dgt_table[28].Max_Length = 8;	Pack_dgt_table[28].optionFlag='C';	//Month Fee amount
		Pack_dgt_table[29].Type = NUMERIC;		Pack_dgt_table[29].Format = FIXED;	    Pack_dgt_table[29].Max_Length = 8;	Pack_dgt_table[29].optionFlag='C';	//Fee amount
		Pack_dgt_table[30].Type = NUMERIC;		Pack_dgt_table[30].Format = FIXED;	    Pack_dgt_table[30].Max_Length = 8;	Pack_dgt_table[30].optionFlag='C';	//settime
		Pack_dgt_table[31].Type = NUMERIC;		Pack_dgt_table[31].Format = FIXED;	    Pack_dgt_table[31].Max_Length = 6;	Pack_dgt_table[31].optionFlag='M';	//BatchNo
		Pack_dgt_table[32].Type = NUMERIC;		Pack_dgt_table[32].Format = FIXED;	    Pack_dgt_table[32].Max_Length = 18;	Pack_dgt_table[32].optionFlag='M';	//AuthSeq
		Pack_dgt_table[33].Type = NUMERIC;		Pack_dgt_table[33].Format = FIXED;	    Pack_dgt_table[33].Max_Length = 1;	Pack_dgt_table[33].optionFlag='C';	//DelayFlag
		Pack_dgt_table[34].Type = NUMERIC;		Pack_dgt_table[34].Format = FIXED;	    Pack_dgt_table[34].Max_Length = 8;	Pack_dgt_table[34].optionFlag='M';	//TxnDate
		Pack_dgt_table[35].Type = NUMERIC;		Pack_dgt_table[35].Format = FIXED;	    Pack_dgt_table[35].Max_Length = 6;	Pack_dgt_table[35].optionFlag='M';	//TxnTime
		Pack_dgt_table[36].Type = ALPHANUM;		Pack_dgt_table[36].Format = FIXED;	    Pack_dgt_table[36].Max_Length = 20;	Pack_dgt_table[36].optionFlag='C';	//Reserved
		Pack_dgt_table[37].Type = ALPHANUM;		Pack_dgt_table[37].Format = FIXED;	    Pack_dgt_table[37].Max_Length = 50;	Pack_dgt_table[37].optionFlag='C';	//MONTHRESERVED  M1换卡卡内数据
		Pack_dgt_table[38].Type = NUMERIC;		Pack_dgt_table[38].Format = FIXED;	    Pack_dgt_table[38].Max_Length = 5;	Pack_dgt_table[38].optionFlag='C';	//RSP_CODE
		
		Pack_dgt_table[0].Rev_Data = dgtFIELD0;		Pack_dgt_table[0].Send_Data = dgtFIELD0;
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;		Pack_dgt_table[1].Send_Data = dgtFIELD1;
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;		Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;		Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;		Pack_dgt_table[4].Send_Data= dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;		Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;		Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;		Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;		Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;		Pack_dgt_table[9].Send_Data = dgtFIELD9;
		
		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;		Pack_dgt_table[19].Send_Data = dgtFIELD19;
		
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;	Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;	Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;	Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;	Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;	Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;	Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;	Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;	Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;	Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;	Pack_dgt_table[29].Send_Data = dgtFIELD29;
		
		Pack_dgt_table[30].Rev_Data = dgtFIELD30;	Pack_dgt_table[30].Send_Data = dgtFIELD30;
		Pack_dgt_table[31].Rev_Data = dgtFIELD31;	Pack_dgt_table[31].Send_Data = dgtFIELD31;
		Pack_dgt_table[32].Rev_Data = dgtFIELD32;	Pack_dgt_table[32].Send_Data = dgtFIELD32;
		Pack_dgt_table[33].Rev_Data = dgtFIELD33;	Pack_dgt_table[33].Send_Data = dgtFIELD33;
		Pack_dgt_table[34].Rev_Data=dgtFIELD34;		Pack_dgt_table[34].Send_Data=	dgtFIELD34;
		Pack_dgt_table[35].Rev_Data=dgtFIELD35;		Pack_dgt_table[35].Send_Data=	dgtFIELD35;
		Pack_dgt_table[36].Rev_Data=dgtFIELD36;		Pack_dgt_table[36].Send_Data=	dgtFIELD36;
		Pack_dgt_table[37].Rev_Data=dgtFIELD37;		Pack_dgt_table[37].Send_Data=	dgtFIELD37;
		Pack_dgt_table[38].Rev_Data=dgtFIELD38;		Pack_dgt_table[38].Send_Data=	dgtFIELD38;

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"LocalSequence");
		strcpy(Pack_dgt_table[2].name,"TransType");
		strcpy(Pack_dgt_table[3].name,"CenterSeqno");
		strcpy(Pack_dgt_table[4].name,"Posid");
		strcpy(Pack_dgt_table[5].name,"PosSequence");
		strcpy(Pack_dgt_table[6].name,"termid");
		strcpy(Pack_dgt_table[7].name,"Operid");
		strcpy(Pack_dgt_table[8].name,"EdCardId");
		strcpy(Pack_dgt_table[9].name,"SamId");
		strcpy(Pack_dgt_table[10].name,"CardFaceId");
		strcpy(Pack_dgt_table[11].name,"CardId");
		strcpy(Pack_dgt_table[12].name,"CityCode");
		strcpy(Pack_dgt_table[13].name,"CardMKnd");
		strcpy(Pack_dgt_table[14].name,"CardSKind");
		strcpy(Pack_dgt_table[15].name,"CardModel");
		strcpy(Pack_dgt_table[16].name,"CardSellDate");
		strcpy(Pack_dgt_table[17].name,"CardBusInessBit");
		strcpy(Pack_dgt_table[18].name,"CardVal");
		strcpy(Pack_dgt_table[19].name,"CardCnt");
		strcpy(Pack_dgt_table[20].name,"BalanceBef");
		strcpy(Pack_dgt_table[21].name,"DepositAmt");
		strcpy(Pack_dgt_table[22].name,"ChipBadType");
		strcpy(Pack_dgt_table[23].name,"CardFaceBadType");
		strcpy(Pack_dgt_table[24].name,"ReturnAmt");
		strcpy(Pack_dgt_table[25].name,"ReturnDeposit");
		strcpy(Pack_dgt_table[26].name,"MonthFeeFlag");
		strcpy(Pack_dgt_table[27].name,"FeeAmtFlag");
		strcpy(Pack_dgt_table[28].name,"MonthFeeAmt");
		strcpy(Pack_dgt_table[29].name,"FeeAmt");
		strcpy(Pack_dgt_table[30].name,"setTime");
		strcpy(Pack_dgt_table[31].name,"BatchNo");
		strcpy(Pack_dgt_table[32].name,"AuthSeq");
		strcpy(Pack_dgt_table[33].name,"DelayFlag");
		strcpy(Pack_dgt_table[34].name,"TxnDate");
		strcpy(Pack_dgt_table[35].name,"TxnTime");
		strcpy(Pack_dgt_table[36].name,"Reserved");
		strcpy(Pack_dgt_table[37].name,"MONTHRESERVED");
		strcpy(Pack_dgt_table[38].name,"RSPCODE");
		
		break;
	case 3460: //换卡完成
		max_count=39;
		
		char openCardDate[8+1];
		bbzero(openCardDate);

		initField();
		strcpy(dgtFIELD0,DGT_Unitid);
		strcpy(dgtFIELD1,pgbl->dgt_LocalSequence); //流水号
		strcpy(dgtFIELD2,tranType);
		strcpy(dgtFIELD3,pgbl->Centerseqno);
		strcpy(dgtFIELD4,pgbl->posId);
		strcpy(dgtFIELD5,pgbl->dgt_posSerial);
		strcpy(dgtFIELD6,pgbl->busi_id);
		strcpy(dgtFIELD7,pgbl->busi_id);
		strcpy(dgtFIELD8,EDCARDID_field);
		strcpy(dgtFIELD10,pgbl->dgtCard.cardShowId);

		strcpy(dgtFIELD11,pgbl->dgtCard.cardShowId);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.CItyCode,(BYTE *)dgtFIELD12,2);
		CTools::byte_to_hex((BYTE *)pgbl->dgtCard.cardParent_type,(BYTE *) dgtFIELD13,1);
		CTools::byte_to_hex((BYTE  *) pgbl->dgtCard.cardChild_type,(BYTE *) dgtFIELD14,1);
		strcpy(dgtFIELD15,"1");

		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.openDate,(BYTE *) dgtFIELD16,4);
		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.effectiveDate,(BYTE *) dgtFIELD18,4);

		strcpy(dgtFIELD19,pgbl->dgtCard.CARDSEQ);
		strcpy(dgtFIELD20,pgbl->dgtCard.tradeBeForeMny);
		strcpy(dgtFIELD21,deposit_field);
		strcpy(dgtFIELD22,"0"); //：0-可读；1-不可读
		strcpy(dgtFIELD23,"0"); //0-卡面完好；1-卡面损坏
		strcpy(dgtFIELD24,pgbl->ReturnAmt); // 
		strcpy(dgtFIELD25,pgbl->ReturnDeposit);
		strcpy(dgtFIELD26,pgbl->MonthFeeFlag);
		strcpy(dgtFIELD27,pgbl->FeeAmtFlag);
		strcpy(dgtFIELD28,pgbl->MonthFeeAmt);
		strcpy(dgtFIELD29,pgbl->FeeAmt);
		strcpy(dgtFIELD31,pgbl->dgtSignInfo.BATCHNO); //修改为签到获取的批次号
		strcpy(dgtFIELD32,AuthSeq_field);
		strcpy(dgtFIELD33,"1"); //延迟
		strcpy(dgtFIELD34,pgbl->dgtCard.tradeDate);
		strcpy(dgtFIELD35,pgbl->dgtCard.tradeTime);
	//	strcpy(dgtFIELD36,pgbl->dgtCard.CHARGE_DIVDATA);
		
		Pack_dgt_table[0].Type = NUMERIC;		Pack_dgt_table[0].Format = FIXED;	    Pack_dgt_table[0].Max_Length = 8;	Pack_dgt_table[0].optionFlag='M';	//Unitid	
		Pack_dgt_table[1].Type = NUMERIC;		Pack_dgt_table[1].Format = FIXED;		Pack_dgt_table[1].Max_Length = 10; 	Pack_dgt_table[1].optionFlag='M'; //LocalSequence
		Pack_dgt_table[2].Type = NUMERIC;		Pack_dgt_table[2].Format = FIXED;		Pack_dgt_table[2].Max_Length = 2;	Pack_dgt_table[2].optionFlag='M'; //TransType
		Pack_dgt_table[3].Type = NUMERIC;		Pack_dgt_table[3].Format = FIXED;		Pack_dgt_table[3].Max_Length = 18;	Pack_dgt_table[3].optionFlag='C'; //中心登记流水号
		Pack_dgt_table[4].Type = NUMERIC;		Pack_dgt_table[4].Format = FIXED;		Pack_dgt_table[4].Max_Length = 12;	Pack_dgt_table[4].optionFlag='M'; //Posid
		Pack_dgt_table[5].Type = NUMERIC; 		Pack_dgt_table[5].Format = FIXED;	    Pack_dgt_table[5].Max_Length = 9;	Pack_dgt_table[5].optionFlag='M'; //PosSeq
		Pack_dgt_table[6].Type = NUMERIC;		Pack_dgt_table[6].Format = FIXED;	    Pack_dgt_table[6].Max_Length = 12;	Pack_dgt_table[6].optionFlag='M'; //企业设备代码
		Pack_dgt_table[7].Type = NUMERIC;	    Pack_dgt_table[7].Format = FIXED;  		Pack_dgt_table[7].Max_Length = 16;	Pack_dgt_table[7].optionFlag='M'; //Operid
		Pack_dgt_table[8].Type = NUMERIC;	    Pack_dgt_table[8].Format = FIXED;	    Pack_dgt_table[8].Max_Length = 16;	Pack_dgt_table[8].optionFlag='C'; //EdCardId
		Pack_dgt_table[9].Type = NUMERIC;		Pack_dgt_table[9].Format = FIXED;		Pack_dgt_table[9].Max_Length = 16;	Pack_dgt_table[9].optionFlag='C'; //SamId
		Pack_dgt_table[10].Type = NUMERIC;		Pack_dgt_table[10].Format = FIXED;		Pack_dgt_table[10].Max_Length = 16;	Pack_dgt_table[10].optionFlag='M'; //CardFaceId
		Pack_dgt_table[11].Type = NUMERIC;		Pack_dgt_table[11].Format = FIXED;		Pack_dgt_table[11].Max_Length = 16;	Pack_dgt_table[11].optionFlag='M'; //CardId
		Pack_dgt_table[12].Type = NUMERIC;		Pack_dgt_table[12].Format = FIXED;		Pack_dgt_table[12].Max_Length = 4;	Pack_dgt_table[12].optionFlag='M'; //CityCode
		Pack_dgt_table[13].Type = NUMERIC;		Pack_dgt_table[13].Format = FIXED;		Pack_dgt_table[13].Max_Length = 2;	Pack_dgt_table[13].optionFlag='M';//CardMKnd
		Pack_dgt_table[14].Type = NUMERIC;		Pack_dgt_table[14].Format = FIXED;		Pack_dgt_table[14].Max_Length = 2;	Pack_dgt_table[14].optionFlag='M';//CardSKind
		
		Pack_dgt_table[15].Type = NUMERIC;		Pack_dgt_table[15].Format = FIXED;		Pack_dgt_table[15].Max_Length = 1;	Pack_dgt_table[15].optionFlag='M'; //CardModel
		Pack_dgt_table[16].Type = NUMERIC;		Pack_dgt_table[16].Format = FIXED;		Pack_dgt_table[16].Max_Length = 8;	Pack_dgt_table[16].optionFlag='M'; //CardSellDate
		
		Pack_dgt_table[17].Type = ALPHANUM;	Pack_dgt_table[17].Format = FIXED;		Pack_dgt_table[17].Max_Length = 8;		Pack_dgt_table[17].optionFlag='C'; //钱包开通业务位图
		Pack_dgt_table[18].Type = NUMERIC;	Pack_dgt_table[18].Format = FIXED;	Pack_dgt_table[18].Max_Length = 8;		Pack_dgt_table[18].optionFlag='M'; //卡有效期
		
		Pack_dgt_table[19].Type = NUMERIC;	Pack_dgt_table[19].Format = FIXED;		Pack_dgt_table[19].Max_Length = 6;		Pack_dgt_table[19].optionFlag='M'; //卡计数器
		Pack_dgt_table[20].Type = NUMERIC;	Pack_dgt_table[20].Format = FIXED;		Pack_dgt_table[20].Max_Length = 10;		Pack_dgt_table[20].optionFlag='M'; //卡余额
		Pack_dgt_table[21].Type = NUMERIC;	Pack_dgt_table[21].Format = FIXED ;		Pack_dgt_table[21].Max_Length = 8;		Pack_dgt_table[21].optionFlag='M'; //押金
		Pack_dgt_table[22].Type = NUMERIC;	Pack_dgt_table[22].Format = FIXED;		Pack_dgt_table[22].Max_Length = 1;		Pack_dgt_table[22].optionFlag='C'; //芯片损坏类别
		Pack_dgt_table[23].Type = NUMERIC;	Pack_dgt_table[23].Format = FIXED;		Pack_dgt_table[23].Max_Length = 1;		Pack_dgt_table[23].optionFlag='M'; //卡面损坏类型
		Pack_dgt_table[24].Type = NUMERIC;			Pack_dgt_table[24].Format = FIXED;		Pack_dgt_table[24].Max_Length = 10;		Pack_dgt_table[24].optionFlag='M';//写入新卡金额
		Pack_dgt_table[25].Type = NUMERIC;		Pack_dgt_table[25].Format = FIXED;		Pack_dgt_table[25].Max_Length = 8;		Pack_dgt_table[25].optionFlag='M'; //退用户押金
		Pack_dgt_table[26].Type = NUMERIC;	Pack_dgt_table[26].Format = FIXED;		Pack_dgt_table[26].Max_Length = 1;		Pack_dgt_table[26].optionFlag='M'; //月费收取标志
		Pack_dgt_table[27].Type = NUMERIC;	Pack_dgt_table[27].Format = FIXED;		Pack_dgt_table[27].Max_Length = 1;		Pack_dgt_table[27].optionFlag='M'; //手续费收取标志
		Pack_dgt_table[28].Type = NUMERIC;	Pack_dgt_table[28].Format = FIXED;		Pack_dgt_table[28].Max_Length = 8;		Pack_dgt_table[28].optionFlag='M'; //卡成本或月费
		
		Pack_dgt_table[29].Type = NUMERIC;	Pack_dgt_table[29].Format = FIXED;	Pack_dgt_table[29].Max_Length = 8;	Pack_dgt_table[29].optionFlag='M';  //手续费
		Pack_dgt_table[30].Type = NUMERIC;	Pack_dgt_table[30].Format = FIXED;	Pack_dgt_table[30].Max_Length = 8;	Pack_dgt_table[30].optionFlag='M'; //结算日期 全0
		Pack_dgt_table[31].Type = NUMERIC;	Pack_dgt_table[31].Format = FIXED;	Pack_dgt_table[31].Max_Length = 6;	Pack_dgt_table[31].optionFlag='M'; //批次号
		Pack_dgt_table[32].Type = NUMERIC;	Pack_dgt_table[32].Format = FIXED;	Pack_dgt_table[32].Max_Length = 18;	Pack_dgt_table[32].optionFlag='M'; //授权流水号
		Pack_dgt_table[33].Type = NUMERIC;	Pack_dgt_table[33].Format = FIXED;	Pack_dgt_table[33].Max_Length = 1;	Pack_dgt_table[33].optionFlag='M'; //延时处理标志位
		Pack_dgt_table[34].Type = NUMERIC;	Pack_dgt_table[34].Format = FIXED;	Pack_dgt_table[34].Max_Length = 8;	Pack_dgt_table[34].optionFlag='M'; //交易日期
		Pack_dgt_table[35].Type = NUMERIC;	Pack_dgt_table[35].Format = FIXED;	Pack_dgt_table[35].Max_Length = 6;	Pack_dgt_table[35].optionFlag='M'; //交易时间
		Pack_dgt_table[36].Type = ALPHANUMS;	Pack_dgt_table[36].Format = FIXED;	Pack_dgt_table[36].Max_Length = 20;	Pack_dgt_table[36].optionFlag='M'; //保留域 全0
		Pack_dgt_table[37].Type = ALPHANUMS;	Pack_dgt_table[37].Format = FIXED;	Pack_dgt_table[37].Max_Length = 50;	Pack_dgt_table[37].optionFlag='M'; //保留使用
		Pack_dgt_table[38].Type = NUMERIC;	Pack_dgt_table[38].Format = FIXED;	Pack_dgt_table[38].Max_Length = 5;	Pack_dgt_table[38].optionFlag='M'; //交易应答码
		
		Pack_dgt_table[0].Rev_Data = dgtFIELD0;	 		Pack_dgt_table[0].Send_Data = dgtFIELD0; 	
		Pack_dgt_table[1].Rev_Data = dgtFIELD1;			Pack_dgt_table[1].Send_Data = dgtFIELD1; 	
		Pack_dgt_table[2].Rev_Data = dgtFIELD2;			Pack_dgt_table[2].Send_Data = dgtFIELD2;
		Pack_dgt_table[3].Rev_Data = dgtFIELD3;			Pack_dgt_table[3].Send_Data = dgtFIELD3;
		Pack_dgt_table[4].Rev_Data = dgtFIELD4;			Pack_dgt_table[4].Send_Data = dgtFIELD4;
		Pack_dgt_table[5].Rev_Data = dgtFIELD5;			Pack_dgt_table[5].Send_Data = dgtFIELD5;
		Pack_dgt_table[6].Rev_Data = dgtFIELD6;			Pack_dgt_table[6].Send_Data = dgtFIELD6;
		Pack_dgt_table[7].Rev_Data = dgtFIELD7;			Pack_dgt_table[7].Send_Data = dgtFIELD7;
		Pack_dgt_table[8].Rev_Data = dgtFIELD8;			Pack_dgt_table[8].Send_Data = dgtFIELD8;
		Pack_dgt_table[9].Rev_Data = dgtFIELD9;			Pack_dgt_table[9].Send_Data = dgtFIELD9;
		Pack_dgt_table[10].Rev_Data = dgtFIELD10;		Pack_dgt_table[10].Send_Data = dgtFIELD10;
		Pack_dgt_table[11].Rev_Data = dgtFIELD11;		Pack_dgt_table[11].Send_Data = dgtFIELD11;
		Pack_dgt_table[12].Rev_Data = dgtFIELD12;		Pack_dgt_table[12].Send_Data = dgtFIELD12;
		Pack_dgt_table[13].Rev_Data = dgtFIELD13;		Pack_dgt_table[13].Send_Data = dgtFIELD13;
		Pack_dgt_table[14].Rev_Data = dgtFIELD14;		Pack_dgt_table[14].Send_Data = dgtFIELD14;
		Pack_dgt_table[15].Rev_Data = dgtFIELD15;		Pack_dgt_table[15].Send_Data = dgtFIELD15;
		Pack_dgt_table[16].Rev_Data = dgtFIELD16;		Pack_dgt_table[16].Send_Data = dgtFIELD16;
		Pack_dgt_table[17].Rev_Data = dgtFIELD17;		Pack_dgt_table[17].Send_Data = dgtFIELD17;
		Pack_dgt_table[18].Rev_Data = dgtFIELD18;		Pack_dgt_table[18].Send_Data = dgtFIELD18;
		
		Pack_dgt_table[19].Rev_Data = dgtFIELD19;		Pack_dgt_table[19].Send_Data = dgtFIELD19;
		Pack_dgt_table[20].Rev_Data = dgtFIELD20;		Pack_dgt_table[20].Send_Data = dgtFIELD20;
		Pack_dgt_table[21].Rev_Data = dgtFIELD21;		Pack_dgt_table[21].Send_Data = dgtFIELD21;
		Pack_dgt_table[22].Rev_Data = dgtFIELD22;		Pack_dgt_table[22].Send_Data = dgtFIELD22;
		Pack_dgt_table[23].Rev_Data = dgtFIELD23;		Pack_dgt_table[23].Send_Data = dgtFIELD23;
		Pack_dgt_table[24].Rev_Data = dgtFIELD24;		Pack_dgt_table[24].Send_Data = dgtFIELD24;
		Pack_dgt_table[25].Rev_Data = dgtFIELD25;		Pack_dgt_table[25].Send_Data = dgtFIELD25;
		Pack_dgt_table[26].Rev_Data = dgtFIELD26;		Pack_dgt_table[26].Send_Data = dgtFIELD26;
		Pack_dgt_table[27].Rev_Data = dgtFIELD27;		Pack_dgt_table[27].Send_Data = dgtFIELD27;
		Pack_dgt_table[28].Rev_Data = dgtFIELD28;		Pack_dgt_table[28].Send_Data = dgtFIELD28;
		Pack_dgt_table[29].Rev_Data = dgtFIELD29;		Pack_dgt_table[29].Send_Data = dgtFIELD29;
		Pack_dgt_table[30].Rev_Data = dgtFIELD30;		Pack_dgt_table[30].Send_Data = dgtFIELD30;
		Pack_dgt_table[31].Rev_Data = dgtFIELD31;		Pack_dgt_table[31].Send_Data = dgtFIELD31;
		Pack_dgt_table[32].Rev_Data = dgtFIELD32;		Pack_dgt_table[32].Send_Data = dgtFIELD32;
		Pack_dgt_table[33].Rev_Data = dgtFIELD33;		Pack_dgt_table[33].Send_Data = dgtFIELD33;
		Pack_dgt_table[34].Rev_Data = dgtFIELD34;		Pack_dgt_table[34].Send_Data = dgtFIELD34;
		Pack_dgt_table[35].Rev_Data = dgtFIELD35;		Pack_dgt_table[35].Send_Data = dgtFIELD35;
		Pack_dgt_table[36].Rev_Data = dgtFIELD36;		Pack_dgt_table[36].Send_Data = dgtFIELD36;
		Pack_dgt_table[37].Rev_Data = dgtFIELD37;		Pack_dgt_table[37].Send_Data = dgtFIELD37;
		Pack_dgt_table[38].Rev_Data = dgtFIELD38;		Pack_dgt_table[38].Send_Data = dgtFIELD38;

		//换卡完成 报文

		strcpy(Pack_dgt_table[0].name,"Unitid");
		strcpy(Pack_dgt_table[1].name,"LocalSequence");
		strcpy(Pack_dgt_table[2].name,"TransType");
		strcpy(Pack_dgt_table[3].name,"CenterSeqno");
		strcpy(Pack_dgt_table[4].name,"Posid");
		strcpy(Pack_dgt_table[5].name,"PosSeq");
		strcpy(Pack_dgt_table[6].name,"TermId");
		strcpy(Pack_dgt_table[7].name,"Operid");
		strcpy(Pack_dgt_table[8].name,"EdCardId");
		strcpy(Pack_dgt_table[9].name,"SamId");
		strcpy(Pack_dgt_table[10].name,"CardFaceId");
		strcpy(Pack_dgt_table[11].name,"CardId");
		strcpy(Pack_dgt_table[12].name,"CityCode");
		strcpy(Pack_dgt_table[13].name,"CardMKnd");
		strcpy(Pack_dgt_table[14].name,"CardSKind");
		strcpy(Pack_dgt_table[15].name,"CardModel");
		strcpy(Pack_dgt_table[16].name,"CardSellDate");
		strcpy(Pack_dgt_table[17].name,"CardBusInessBit");
		strcpy(Pack_dgt_table[18].name,"CardVal");
		strcpy(Pack_dgt_table[19].name,"CardCnt");
		strcpy(Pack_dgt_table[20].name,"BalanceBef");
		strcpy(Pack_dgt_table[21].name,"Deposit");
		strcpy(Pack_dgt_table[22].name,"ChipBadType");
		strcpy(Pack_dgt_table[23].name,"CardFaceBadType");
		strcpy(Pack_dgt_table[24].name,"ReturnAmt");
		strcpy(Pack_dgt_table[25].name,"ReturnDeposit");
		strcpy(Pack_dgt_table[26].name,"MonthFeeflag");
		strcpy(Pack_dgt_table[27].name,"FeeAmtFlag");
		strcpy(Pack_dgt_table[28].name,"MonthFeeAmt");
		strcpy(Pack_dgt_table[29].name,"FeeAmt");
		strcpy(Pack_dgt_table[30].name,"setTime");
		strcpy(Pack_dgt_table[31].name,"BatchNo");
		strcpy(Pack_dgt_table[32].name,"AuthSeq");
		strcpy(Pack_dgt_table[33].name,"LagFlag");
		strcpy(Pack_dgt_table[34].name,"TxnDate");
		strcpy(Pack_dgt_table[35].name,"TxnTime");
		strcpy(Pack_dgt_table[36].name,"Reserved");
		strcpy(Pack_dgt_table[37].name,"MONTHRESERVED");
		strcpy(Pack_dgt_table[38].name,"RSPCODE");

		break;

	default:
		return -1;
	}

	//进行初始化

	for ( j =0;j<max_count;j++)
	{
		Pack_dgt_table[j].Real_Length = 0; 
		char sendDataField[100];
		bbzero(sendDataField);
		strcpy(sendDataField,Pack_dgt_table[j].Send_Data);
		wrLog("%s:%s",Pack_dgt_table[j].name,Pack_dgt_table[j].Send_Data);

		switch (Pack_dgt_table[j].optionFlag) 
		{
		case 'M': //如果是必填
			
			if (Pack_dgt_table[j].Type==NUMERIC) //如果是数字。
			{
				int fix_length=Pack_dgt_table[j].Max_Length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Max_Length,1,Pack_dgt_table[j].Send_Data);
			
				
			}
			else //如果是ansc
			{
				
				int fix_length=Pack_dgt_table[j].Max_Length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Max_Length,0,Pack_dgt_table[j].Send_Data);
				
			}
			break;
		case 'C': //如果是选填选项 

			if (Pack_dgt_table[j].Format==FIXED) //如果是定长。
			{
				int fix_length=Pack_dgt_table[j].Max_Length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Max_Length,0,Pack_dgt_table[j].Send_Data);
				
			}
			else //如果是可变长度，获取上一个字段的值作为长度
			{
				int real_length=atoi(Pack_dgt_table[j-1].Send_Data);
				Pack_dgt_table[j].Real_Length=real_length;
				CTools::addZero(sendDataField,Pack_dgt_table[j].Real_Length,0,Pack_dgt_table[j].Send_Data);
			}
			
			break;
			
		}
	}





	return 0;
}



//计算通讯mac 3des加密

int CDGTBusTrade::getCommuMac(char *poskey,char *keyset,char *keyField,char *commuMac)
{
	char uncipherkeySet[32+1];
	unsigned char key_field_byte[1024];
	unsigned char  uncipherkeySet_byte[16+1];
	unsigned char iv[8]={0};
	int i=0;
	int totalLen=0;
	int iSrcLen=0;
	unsigned char commuMac_temp[4+1];

	bbzero(commuMac_temp);
	bbzero(commuMac);
	bbzero(uncipherkeySet);
	bbzero(uncipherkeySet_byte);
	bbzero(key_field_byte);
	
	wrLog("poskey:%s",poskey);
	wrLog("keyset:%s",keyset);
	wrLog("加密关键域:%s",keyField);
	
	bbzero(uncipherkeySet);
	//用终端密钥解密keyset
	if (strlen(keyField) %2 !=0) //如果不是2的倍数
	{
		wrLog("关键域不是2的倍数");
		return -1; //
	}
	
	CDes::TripleDesDec((char*)poskey , keyset , uncipherkeySet);
	wrLog("uncipherkeySet:%s",uncipherkeySet);
	wrLog("keyFiled:%s",keyField);
	
	CTools::hex_to_byte((BYTE *) uncipherkeySet,(BYTE *)uncipherkeySet_byte,strlen(uncipherkeySet));
	CTools::hex_to_byte((BYTE *) keyField,(BYTE *) key_field_byte,strlen(keyField));
	//对关键域进行加密
	
	iSrcLen=strlen(keyField)/2;
	MAC16(uncipherkeySet_byte, key_field_byte,  iSrcLen, iv, commuMac_temp);
	CTools::byte_to_hex( commuMac_temp,(BYTE *)commuMac,4);
	wrLog("comMac:%s",commuMac);
	if (strlen(commuMac)!=8)
	{
		return -1;//计算失败
	}
	
	return 0;
}


//获取同步信息ANS12 使用随机数114523
int CDGTBusTrade::getSyncinfo(char *syncInfo)
{
	char currentTime[9+1];
	char sub_currentTime[6+1];
	char randomNum[6+1];

	bbzero(currentTime);
	bbzero(randomNum);

	bbzero(sub_currentTime);
	CTools::getTime(currentTime,1); //获取到毫秒
	memcpy(sub_currentTime,currentTime+3,6);
	CTools::generate_random_num(randomNum,6);
	bbzero(syncInfo);
	sprintf(syncInfo,"%s%s",sub_currentTime,randomNum);
	return 0;
}




//抑或运算 by liuxiao 20130401 东莞HID设备

UCHAR CDGTBusTrade::getXor(UCHAR *Data, int DataLen)
{
	UCHAR Chk = 0;
	while (DataLen--)
		Chk ^=Data[DataLen];
	return Chk;

}

/********************************************
//by liuxiao 东莞HID设备 
sData：发送的数据包
sDataLen:发送的数据包长度
dData: 数据域
DatatLen: 数据域长度
********************************************/

int CDGTBusTrade::Package0203(UCHAR *sData,UCHAR *dData,int DataLen)
{
	sData[0] = 0x02; //报文头
	if (DataLen>512) return -1;
	sData[1] = DataLen / 0x100;  //报文长度
	sData[2] = DataLen % 0x100;
	memcpy(sData+3,dData,DataLen);
	sData[3+DataLen] =getXor(sData+3,DataLen); //异或运算
	sData[4+DataLen] = 0x03;  //报文尾
	return 0; 
	
}



/****************************************************
*2010-11-09 kevinzhao
*银联协议解包
*return 0 =sucess,
****************************************************/
int CDGTBusTrade::unPackage0203(UCHAR *sData,UCHAR *dData,int *DataLen) 
{
	if (sData[0]!=0x02) return -1;
	*DataLen = sData[1] * 0x100 + sData[2];
	if (*DataLen>1300) return -1;
	if (sData[*DataLen+4]!=0x03) 
		return -1;
	if (sData[*DataLen+3]!=getXor(sData+3,*DataLen)) return -2; //进行mac校验
	memcpy(dData,sData+3,*DataLen);
	return 0;

}


//东莞公交报文交互信息
int CDGTBusTrade::dgtSndRcvHID_IcData(const TSDLLPosInfo dPosInfo,const BYTE *downData, const int downLen, BYTE *upData, int &upLen,char *errMsg)
{
	int ret;
	TSHID_IcReaderCommand icCmd;
	memset(&icCmd, 0, sizeof(icCmd));
	//strcpy(icCmd.acTitle, title);
	icCmd.uDownLen=downLen+5;
	//打包函数
	ret=Package0203((BYTE *)icCmd.uDownData,(BYTE *)downData,downLen);
	if (ret!=0)
	{
		return 1;
	}

	wrHex("下发读卡器数据", icCmd.uDownData, icCmd.uDownLen); //下传读卡器的报文及报文长度
	ret = TS_HID_IcReaderCommand(dPosInfo, &icCmd);  
	if( ret ) //错误，返回出错信息
	{
		upLen = 0;
		wrLog("错误返回值: [%d]", ret);
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互出错，请连接好读卡器");
		//TS_OutPutMsg(gbl.dPosInfo, " \n 与终端交互出错!", 1, iSelect);
		return ret;
	}
	
	wrHex("读卡器返回数据", icCmd.uUpData, icCmd.uUpLen);
	if( icCmd.uUpLen < 6 )
	{
		wrLog("读卡器返回数据小于6");
		return 7;
	}
	//解包函数
	ret=unPackage0203(icCmd.uUpData,upData,&upLen);
	if(ret==-1)
	{
		bbzero(errMsg);
		strcmp(errMsg,"报文格式错误");
		wrLog("报文有问题");
		return 9;
		
	}
	if(ret==-2)
	{
		wrLog("校验码错误");
		bbzero(errMsg);
		strcpy(errMsg,"与读卡器校验出错");
		//TS_OutPutMsg(gbl.dPosInfo, " \n 与读卡器校验出错!", 1, iSelect);
		return 8;
	}
	
	return 0;
}



//卡片上电
int CDGTBusTrade::getCardType(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg)
{
	char rcvData[256];
	int rcvLen;
	char cardType[2+1];

	bbzero(rsp_code);
	bbzero(rcvData);
	bbzero(cardType);
	//cpu卡充值之前需要进行寻卡操作
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("卡片激活失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)cardType,1);

	//0A //cpu 卡 1A //M1卡
	if (strcmp(cardType,"1A")==0) //如果是M1卡
	{
		return 1;
	}
	else if (strcmp(cardType,"0A")==0) //如果是cpu卡
	{
		return 0;
	}
	else
	{
	
		return -1; //查询错误
	}

}

//非接触卡上电寻卡 寻卡失败立即返回操作失败
//函数值 0代表执行指令成功。返回卡信息 卡片执行指令不成功，返回错误信息。
/**********************************************************************
出参：cardType:卡类型
cardInfo:卡片信息
otherData:其他卡片信息（如果是A卡）
//flag=3 不更新信息

**********************************************************************/
int CDGTBusTrade::cardSearch(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg,PDGTCard pDgtCard ,int flag)
{
	char rcvData[256];
	int rcvLen;
	char cardType[2+1];
	//获取激活信息
	int infoLen=0; //卡片信息长度
	char cardInfo[255]; //卡片信息
	int otherLen=0; //其他信息长度
	char otherData[255]; //其他信息(A卡返回的)
	int count=0;
	char len_temp_10[2];

	bbzero(rsp_code);
	bbzero(rcvData);
	bbzero(cardInfo);
	bbzero(otherData);
	bbzero(cardType);
	
	//cpu卡充值之前需要进行寻卡操作
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("卡片激活失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	bbzero(len_temp_10);
	bbzero(cardInfo);
	bbzero(otherData);
	
	char len_temp[2];
	bbzero(len_temp);
	CTools::byte_to_hex((BYTE *)rcvData+2,(BYTE *)cardType,1);
	//如果是m1卡，提示用户卡片类型错误。仅支持东莞通卡片充值
	if (strcmp(cardType,"1A") ==0)
	{
		wrLog("卡片类型错误,非CPU卡");
		bbzero(errMsg);
		strcpy(errMsg,"\n本业务仅支持东莞通卡，汇通卡请进入东莞公交业务\n\n请按【Enter】返回");
		return -2;
	}

	CTools::byte_to_hex((BYTE *) rcvData+3,(BYTE *)len_temp,1);
	CTools::sixten_toten(len_temp,len_temp_10);
	infoLen=atoi(len_temp_10);
	memcpy(cardInfo,rcvData+4,infoLen);
	count+=infoLen+4;

	bbzero(len_temp);
	bbzero(len_temp_10);
	CTools::byte_to_hex((BYTE *)rcvData+count,(BYTE *)len_temp,1);
	CTools::sixten_toten(len_temp,len_temp_10);
	otherLen=atoi(len_temp_10);
	count+=1;

	if (otherLen!=0) //如果其他长度信息不为0
	{
		bbzero(otherData);
		memcpy(otherData,rcvData+count,otherLen);
		count+=otherLen;
	}


	if (flag!=3) //如果不是在交易过程中查询余额
	{

		bbzero(pDgtCard->cardInfo);
		bbzero(pDgtCard->otherData);
		bbzero(pDgtCard->cardType);
		memcpy(pDgtCard->cardInfo,cardInfo,infoLen);
		memcpy(pDgtCard->otherData,otherData,otherLen);
		memcpy(pDgtCard->cardType,cardType,2);
		
		bbzero(pDgtCard->CARDSALE_DIVDATA); //售卡离散因子
		CTools::byte_to_hex((BYTE *) otherData+8,(BYTE *)pDgtCard->CARDSALE_DIVDATA, 8);

	}

	//输出信息
	wrLog("cpu卡激活成功!");
	wrLog("卡类型:%s",cardType);
	wrHex("卡信息:",(BYTE *)cardInfo,infoLen);
	wrHex("其他信息(A卡信息返回):",(BYTE *)otherData,otherLen);
	//wrLog("售卡分散因子:%s",pDgtCard->CARDSALE_DIVDATA);
	
	return 0;
}


//获取M1的信息 (M1上电信息

int CDGTBusTrade::cardM1Search(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg,PDGTM1Card pDgtM1Card)
{
	char rcvData[256];
	int rcvLen;
	char cardType[2+1];
	//获取激活信息
	int infoLen=0; //卡片信息长度
	char cardInfo[255]; //卡片信息
	int otherLen=0; //其他信息长度
	char otherData[255]; //其他信息(A卡返回的)
	int count=0;
	char M1CardCSN[8+1];//	M1卡片cardCSN
	int iRet;
	char cardParentType[2+1];
	char cardChildType[2+1];

	bbzero(cardParentType);
	bbzero(cardChildType);
	bbzero(rsp_code);
	bbzero(rcvData);
	bbzero(cardInfo);
	bbzero(otherData);
	bbzero(cardType);
	bbzero(M1CardCSN);
	bbzero(cardType);
	
	//cpu卡充值之前需要进行寻卡操作
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("卡片激活失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //卡片指令执行不成功。返回错误
	}

	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)cardType,1);
	//如果卡类型错误，不需要进行
	if (strcmp(cardType,"1A") !=0 ) //如果 不是M1卡直接返回
	{
		bbzero(errMsg);
		strcpy(errMsg,"请换汇通智能卡重试");
		return -3;
	}
	CTools::byte_to_hex((BYTE *) rcvData+4,(BYTE *)M1CardCSN,4);
	wrLog("获取M卡信息如下---------------");
	wrLog("cardSCN:%s",M1CardCSN);
	wrLog("cardType:%s",cardType);

	//读取卡主和卡子类型
	iRet=getCardParentType(dPosInfo,M1CardCSN,cardParentType,cardChildType,errMsg);
	if (iRet!=0)
	{
		return -4; //查询卡类型信息错误
	}

	strcpy(pDgtM1Card->bus_card_type,cardType);
	strcpy(pDgtM1Card->M1CardCSN,M1CardCSN);
	strcpy(pDgtM1Card->card_parent_type,cardParentType);
	strcpy(pDgtM1Card->card_child_type,cardChildType);

	return 0;
}



//只用来获取M1卡片获取卡子和卡主信息 

int CDGTBusTrade::getCardParentType(const TSDLLPosInfo dPosInfo,char *cardCSN,char *cardParentType,char *cardChildType,char *errMsg)
{

	char sendData[255];
	char rcvData[255];
	int sendLen=0;
	int rcvLen=0;
	char rsp_code[4+1];
	char parent_type_buff[2+1];
	char child_type_buff[2+1];

	bbzero(sendData);
	bbzero(rcvData);
	bbzero(rsp_code);
	bbzero(parent_type_buff);
	bbzero(child_type_buff);

	memcpy(sendData,"\xC2\x02\x60\x03\xA0\xA1\xA2\xA3\xA4\xA5",10);
	sendLen=10;
	CTools::hex_to_byte((BYTE *) cardCSN,(BYTE *) sendData+sendLen,8);
	sendLen+=4;

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("获取卡子类型失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	//再发送指令
	bbzero(rcvData);
	rcvLen=0;

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x03\x02",3, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("获取卡类型失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	//获取卡子信息及卡主信息
	bbzero(cardChildType);
	bbzero(cardParentType);
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)parent_type_buff,1);
	CTools::byte_to_hex((BYTE *) rcvData+3,(BYTE *)child_type_buff,1);
	//转换卡片类型
	if (strcmp(parent_type_buff,"50") ==0 && ( strcmp(child_type_buff,"01")==0 ))
	{

		bbzero(cardParentType);
		bbzero(cardChildType);
		strcpy(cardParentType,"00");
		strcpy(cardChildType,"01");

	}
	
	if (strcmp(parent_type_buff,"50")==0 && (strcmp(child_type_buff,"11")==0) )
	{
		
		bbzero(cardParentType);
		bbzero(cardChildType);
		strcpy(cardParentType,"00");
		strcpy(cardChildType,"02");
	}

	return 0;


}



//查询M1信息
int CDGTBusTrade::getM1SBInfo(const TSDLLPosInfo dPosInfo,PDGTM1Card pdgtM1Card,char * errMsg)

{
	char sendData[255];
	char rcvData[255];
	int sendLen=0;
	int rcvLen=0;
	char rsp_code[4+1];
	char parent_type_buff[2+1];
	char child_type_buff[2+1];
	char cardCSN[8+1];
	char cardType[2+1];
	char city_code[2+1];
	char rec_code[4+1];
	char SW1SW2[4+1];
	char keyA[6+1];
	char deposit[2+1];
	char saleDate[6+1];
	
	bbzero(saleDate);
	bbzero(parent_type_buff);
	bbzero(child_type_buff);
	bbzero(deposit);
	bbzero(SW1SW2);
	bbzero(keyA);
	bbzero(rsp_code);
	bbzero(city_code);
	bbzero(sendData);
	bbzero(rcvData);
	bbzero(rsp_code);
	bbzero(parent_type_buff);
	bbzero(child_type_buff);
	bbzero(cardType);
	int offset=0;
	
	//step 1

	wrLog("进行非接触卡上电---------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("卡片激活失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //卡片指令执行不成功。返回错误
	}
	
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)cardType,1);
	//如果卡类型错误，不需要进行
	if (strcmp(cardType,"1A") !=0 ) //如果 不是M1卡直接返回
	{
		bbzero(errMsg);
		strcpy(errMsg,"请换汇通智能卡重试");
		return -3;
	}

	CTools::byte_to_hex((BYTE *) rcvData+4,(BYTE *)cardCSN,4);
	wrLog("获取M卡信息如下---------------");
	wrLog("cardSCN:%s",cardCSN);
	wrLog("cardType:%s",cardType);
	strcpy(pdgtM1Card->M1CardCSN,cardCSN);

	//step 2

	bbzero(sendData);
	sendLen=0;
	memcpy(sendData,"\xC2\x02\x60\x03\xA0\xA1\xA2\xA3\xA4\xA5",10);
	sendLen=10;

	CTools::hex_to_byte((BYTE *) cardCSN,(BYTE *) sendData+sendLen,8);
	sendLen+=4;
	
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) )
	{
		return -1;
	}

	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("获取卡子类型失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}


	//获取有效期等信息
	bbzero(rcvData);
	rcvLen=0;
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x03\x01",3, (BYTE *)rcvData, rcvLen,errMsg) )  //获取城市编码和认证吗
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("获取有效期失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	//获取信息有效期等信息
	CTools::byte_to_hex((BYTE *) rcvData+6,(BYTE *)pdgtM1Card->cardVadate,4);
	CTools::byte_to_hex((BYTE *) rcvData+14,(BYTE *) pdgtM1Card->cardStatus,1);
	
	//step 3 发送指令 //c20302 返回数据中取城市代码和认证码
	bbzero(rcvData);
	rcvLen=0;
	wrLog("获取城市编码-----------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x03\x02",3, (BYTE *)rcvData, rcvLen,errMsg) )  //获取城市编码和认证吗
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("获取认证吗失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	//取城市编码和验证码
	memcpy(city_code,rcvData+2+6,2);
	memcpy(rec_code,rcvData+2+10,4);
	wrHex("city_code:",(BYTE *)city_code,2);
	wrHex("rec_code:",(BYTE  *) rec_code,4);

	//step 4 sam上电  32 22 00 00 10
	bbzero(rcvData);
	rcvLen=0;
	wrLog("SAM卡上电-------------------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x22\x00\x00\x10",5, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("SAM卡上电失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	bbzero(rcvData);
	rcvLen=0;

	//5.发送32 26 10 00 a4 00.... 最后两个字节是9000就行
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x26\x10\x00\xa4\x00\x00\x02\x10\x03",10, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("sam卡响应错误");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	//6 发送命令给SAM卡ＡＰＤＵ命令：80FC01 01 10 [城市代码][卡唯一码] 0001[认证码]0a0b0c0d 　；计算密钥，分别是扇区1，2，3,10 KeyA密钥；取返回前６字节为密钥[RESP(0,6)]>10
	bbzero(sendData);
	sendLen=0;
	bbzero(rcvData);
	rcvLen=0;
	wrLog("获取密钥keyA------------------------");
	CTools::hex_to_byte((BYTE *) "32261080FC010110",(BYTE * )sendData,16);
	sendLen+=8;
	memcpy(sendData+sendLen,city_code,2);
	sendLen+=2;

	CTools::hex_to_byte((BYTE  *) cardCSN,(BYTE *) sendData+sendLen,8);
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) "0001",(BYTE *)sendData+sendLen,4);
	sendLen+=2;
	memcpy(sendData+sendLen,rec_code,4);
	sendLen+=4;
	CTools::hex_to_byte((BYTE * )"0a0b0c0d",(BYTE *)sendData+sendLen,8);
	sendLen+=4;
	
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("获取SAM卡密钥失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -1; //卡片指令执行不成功。返回错误
	}

	memcpy(keyA,rcvData+2,6);
	//7.再次执行寻卡
	bbzero(rcvData);
	rcvLen=0;
	wrLog("再次寻卡-------------");
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\x32\x24\x00\x00",4, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("卡片激活失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //卡片指令执行不成功。返回错误
	}

	//C202 6007[VAR10，第６步取得的密钥] [卡唯一码]；

	bbzero(sendData);
	sendLen=0;
	bbzero(rcvData);
	rcvLen=0;
	
	CTools::hex_to_byte((BYTE *) "C2026007",(BYTE * )sendData,8);
	sendLen+=4;
	memcpy(sendData+sendLen,keyA,6);
	sendLen+=6;
	CTools::hex_to_byte((BYTE *) cardCSN,(BYTE *)sendData+sendLen,8);
	sendLen+=4;
	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)sendData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("执行失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //卡片指令执行不成功。返回错误
	}


	wrLog("获取押金等信息------------------");
	bbzero(rcvData);
	rcvLen=0;
	char deposit_temp[6+1];
	bbzero(deposit_temp);

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xc2\x03\x04",3, (BYTE *)rcvData, rcvLen,errMsg) ) 
	{
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	wrLog("执行返回:%s",rsp_code);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("执行失败");
		returnHIDGTFail(rsp_code,errMsg); 
		return -2; //卡片指令执行不成功。返回错误
	}


	//获取信息
	 offset=2;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *)parent_type_buff,1);
	offset+=1;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *)child_type_buff,1);
	offset+=1;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *) pdgtM1Card->appSerial,4);
	offset+=4;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *) saleDate,3);
	offset+=3;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *) pdgtM1Card->openTime,2);
	offset+=2;
	//售卡地点 不取
	offset+=2;
	CTools::byte_to_hex((BYTE *) rcvData+offset,(BYTE *)deposit,1);
	CTools::sixten_toten(deposit,deposit_temp); //押金应该取已分为单位
	sprintf(pdgtM1Card->deposit,"%06d",atoi(deposit_temp)*100);
	if (strcmp(saleDate,"000000")==0) //如果获取的日期为空
	{
		bbzero(pdgtM1Card->openDate);
		sprintf(pdgtM1Card->openDate,"00%s",saleDate);
	}
	else
	{
	
		bbzero(pdgtM1Card->openDate);
		CTools::transYYYMMDD(saleDate,pdgtM1Card->openDate);
	}
	
	//转换卡片类型
	if (strcmp(parent_type_buff,"50") ==0 && ( strcmp(child_type_buff,"01")==0 ))
	{
		
		bbzero(pdgtM1Card->card_parent_type);
		bbzero(pdgtM1Card->card_child_type);
		strcpy(pdgtM1Card->card_parent_type,"00");
		strcpy(pdgtM1Card->card_child_type,"01");
		
	}
	
	if (strcmp(parent_type_buff,"50")==0 && (strcmp(child_type_buff,"11")==0) )
	{
		
		bbzero(pdgtM1Card->card_parent_type);
		bbzero(pdgtM1Card->card_child_type);
		strcpy(pdgtM1Card->card_parent_type,"00");
		strcpy(pdgtM1Card->card_child_type,"02");
	}

	return 0;

}


//没有进入钱包余额，返回的卡片信息 注意这是0005文件下的信息 获取押金信息

int CDGTBusTrade::readCardInfo(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard, char *errMsg)
{


	char rcvData[256];
	int rcvLen;
	char rsp_code[4+1]; //返回码

	bbzero(rcvData);
	bbzero(rsp_code);

	wrLog("读取卡基础信息未进入钱包目录-----------------------------------");
	char CItyCode[2+1]; //城市编码
	char compay_code[2+1]; //发卡方单位
	char busi_code[2+1]; //行业代码
	char RFU[2+1]; //保留
	char appSerial[8+1];//应用序号
	char cardParent_type[1+1]; //卡主类型
	char cardChild_type[1+1]; //卡子类型
	char openDate[4+1]; //发卡日期
	char deviceInfo[6+1]; //发行设备信息
	char appVer[2+1];//应用版本信息

	int dataLen; //数据域名长度

	//初始化变量
	bbzero(CItyCode);
	bbzero(compay_code);
	bbzero(busi_code);
	bbzero(RFU);
	bbzero(appSerial);
	bbzero(cardChild_type);
	bbzero(cardParent_type);
	bbzero(openDate);
	bbzero(deviceInfo);
	bbzero(appVer);

	
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x00\xB0\x85\x00\x1E",8, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
		
	{
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互失败,	请连接好读卡器");
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
		
	}
	
	//解析获取卡信息
	dataLen=2;
	memcpy(compay_code,rcvData+dataLen,2); //发卡方代码
	dataLen+=2;
	memcpy(CItyCode,rcvData+dataLen,2); //城市编码
	dataLen+=2;
	memcpy(busi_code,rcvData+dataLen,2); //行业代码
	dataLen+=2;
	memcpy(RFU,rcvData+dataLen,2);  //RFU
	dataLen+=2;
	memcpy(appSerial,rcvData+dataLen,8); //应用序列号
	dataLen+=8;
	memcpy(cardParent_type,rcvData+dataLen,1);//卡主类型
	dataLen+=1;
	memcpy(cardChild_type,rcvData+dataLen,1); //卡子类型
	dataLen+=1;
	memcpy(openDate,rcvData+dataLen,4); //启动日期
	dataLen+=4;
	memcpy(deviceInfo,rcvData+dataLen,6); //发行设备信息
	memcpy(appVer,rcvData+dataLen,1); //应用版本
	dataLen+=1;

	
	wrLog("输出卡片基本信息(0005)-------------------------");
	wrHex("发卡行:",(BYTE *)compay_code,2);
	wrHex("城市代码:",(BYTE *)CItyCode,2);
	wrHex("行业代码:",(BYTE *)busi_code,2);
	wrHex("押金:",(BYTE *)RFU,2);
	wrHex("应用版本:",(BYTE *)appVer,1);
	wrHex("应用序列号:",(BYTE *)appSerial,8);
	wrHex("发行日期:",(BYTE *)openDate,4);
	wrHex("卡主类型:",(BYTE *)cardParent_type,1);
	wrHex("卡子类型:",(BYTE *)cardChild_type,1);
	wrHex("发行设备信息:",(BYTE *) deviceInfo,6);

	bbzero(pdgtCard->deposit);
	memcpy(pdgtCard->deposit,RFU,2); //卡片押金

	return 0;

}



//读取卡片基本信息 获取卡片基本信息。（进入钱包之后返回的卡片基础信息）
//注意这是0015的文件中的信息

int CDGTBusTrade::readCardBasicInfo(const TSDLLPosInfo dPosInfo,char *errMsg,PDGTCard pdgtCard,int flag)
{
	char rcvData[256];
	int rcvLen;
	char rsp_code[4+1]; //返回码

	bbzero(rcvData);
	bbzero(rsp_code);

	wrLog("读取卡基础信息-----------------------------------");
	char CItyCode[2+1]; //城市编码
	char compay_code[2+1]; //发卡方单位
	char busi_code[2+1]; //行业代码
	char RFU[2+1]; //保留
	char appSerial[8+1];//应用序号
	char cardParent_type[2+1]; //卡主类型
	char cardChild_type[2+1]; //卡子类型
	char openDate[4+1]; //发卡日期
	char effectiveDate[4+1]; //有效日期
	char deviceInfo[6+1]; //发行设备信息
	char appVer[1+1];//应用版本信息
	char SW1SW2[4+1]; //卡片响应码
	int dataLen; //数据域名长度
	char appType[1+1]; //应用类型标志
	char connectCity[2+1]; //互联互通标志。（参与互通城市标志）
	char BusiBitMap[4+1]; //业务位图数据
	char card_no[16+1];
	char sale_type[2+1];

	//初始化变量
	bbzero(sale_type);
	bbzero(appType);
	bbzero(CItyCode);
	bbzero(SW1SW2);
	bbzero(compay_code);
	bbzero(busi_code);
	bbzero(RFU);
	bbzero(appSerial);
	bbzero(cardChild_type);
	bbzero(cardParent_type);
	bbzero(openDate);
	bbzero(deviceInfo);
	bbzero(appVer);
	bbzero(connectCity);
	bbzero(effectiveDate);
	bbzero(BusiBitMap);
	bbzero(card_no);
	
	
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x00\xB0\x95\x00\x22",8, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
		
	{
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互失败，请连接好读卡器");
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
		
	}
	
	//解析获取卡信息
	dataLen=2;
	memcpy(compay_code,rcvData+dataLen,2); //发卡方代码
	dataLen+=2;
	memcpy(CItyCode,rcvData+dataLen,2); //城市编码
	dataLen+=2;
	memcpy(busi_code,rcvData+dataLen,2); //行业代码
	dataLen+=2;
	memcpy(RFU,rcvData+dataLen,2);  //RFU
	dataLen+=2;

	memcpy(appType,rcvData+dataLen,1); //应用类型标志，启动是00,01是未启动
	dataLen+=1;
	memcpy(appVer,rcvData+dataLen,1); //应用版本
	dataLen+=1;
	memcpy(connectCity,rcvData+dataLen,2); //互联互通标志，（参与互联互通的城市）
	dataLen+=2;
	memcpy(appSerial,rcvData+dataLen,8); //应用序列号
	dataLen+=8;
	memcpy(openDate,rcvData+dataLen,4); //启动日期
	dataLen+=4;
	memcpy(effectiveDate,rcvData+dataLen,4); //应用有效日期
	dataLen+=4;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) cardParent_type,1); //卡主类型
	dataLen+=1;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) cardChild_type,1);  //卡子类型
	dataLen+=1;
	memcpy(BusiBitMap,rcvData+dataLen,4); //业务位图
	dataLen+=4;


	//押金卡的售卡方式是02 其他是01
	if (strcmp(cardParent_type,"10") ==0 ) //如果是押金卡
	{

		bbzero(sale_type);
		strcpy(sale_type,"02");
	}
	else
	{
	
		bbzero(sale_type);
		strcpy(sale_type,"01");

	}
	

	wrLog("输出卡片基本信息（0015文件）-------------------------");
	wrHex("发卡行:",(BYTE *)compay_code,2);
	wrHex("城市代码:",(BYTE *)CItyCode,2);
	wrHex("行业代码:",(BYTE *)busi_code,2);
	wrHex("RFU:",(BYTE *)RFU,2);
	wrHex("应用类型标志",(BYTE *) appType,1);
	wrHex("应用版本:",(BYTE *)appVer,1);
	wrHex("互联互通标志",(BYTE *)connectCity,2);
	wrHex("应用序列号:",(BYTE *)appSerial,8);
	wrHex("发卡日期:",(BYTE *)openDate,4);
	wrHex("应用有效日期:",(BYTE *) effectiveDate,4);
	wrLog("卡主类型:%s",cardParent_type);
	wrLog("卡子类型:%s",cardChild_type);
	wrHex("业务位图信息:",(BYTE *)BusiBitMap,4);
	wrLog("售卡方式:%s",sale_type);
	

	if (flag!=3) //如果是卡片交易过程中读取余额，不需要更新这些基本信息
	{

		bbzero(pdgtCard->cardShowId);
		getCardShowId(appSerial,pdgtCard->cardShowId);
		wrLog("卡号(十进制):%s",pdgtCard->cardShowId);
		
		bbzero(pdgtCard->compay_code);
		memcpy(pdgtCard->compay_code,compay_code,2);
		bbzero(pdgtCard->CItyCode);
		memcpy(pdgtCard->CItyCode,CItyCode,2);
		bbzero(pdgtCard->busi_code);
		memcpy(pdgtCard->busi_code,busi_code,2);
		bbzero(pdgtCard->appFlag);
		memcpy(pdgtCard->appFlag,appType,1);
		memcpy(pdgtCard->appVer,appVer,1);
		bbzero(pdgtCard->connectCity);
		memcpy(pdgtCard->connectCity,connectCity,2);
		bbzero(pdgtCard->appSerial);
		memcpy(pdgtCard->appSerial,appSerial,8);
		bbzero(pdgtCard->cardParent_type);
		CTools::hex_to_byte((BYTE *) cardParent_type,(BYTE *) pdgtCard->cardParent_type,2);
		bbzero(pdgtCard->cardChild_type);
		CTools::hex_to_byte((BYTE *) cardChild_type,(BYTE *) pdgtCard->cardChild_type,2);
		bbzero(pdgtCard->openDate);
		memcpy(pdgtCard->openDate,openDate,4);
		bbzero(pdgtCard->effectiveDate);
		memcpy(pdgtCard->effectiveDate,effectiveDate,4);
		bbzero(pdgtCard->deviceInfo);
		memcpy(pdgtCard->deviceInfo,deviceInfo,6);
		bbzero(pdgtCard->sale_type);
		strcpy(pdgtCard->sale_type,sale_type);

	}

	return 0;

}


int CDGTBusTrade::getCardShowId(char *cardID,char *cardShowID)
{
	char card_first8[8+1];
	char card_last8[8+1];
	char  card_id16[16+1];
	char showId_last8[16+1];

	bbzero(card_first8);
	bbzero(card_last8);
	bbzero(card_id16);
	bbzero(showId_last8);

	CTools::byte_to_hex((BYTE *) cardID,(BYTE *)card_id16,8);
	
	memcpy(card_first8,card_id16,8);
	memcpy(card_last8,card_id16+8,8);

	CTools::HexToDec(card_last8,showId_last8,8);
	bbzero(cardShowID);
	sprintf(cardShowID,"%s%s",card_first8,showId_last8);

	wrLog("卡片的卡号(十进制):%s",cardShowID);

	return 0;

}
//进入钱包目录，进行查询。返回卡片信息。

int CDGTBusTrade::accessWalletDir(const TSDLLPosInfo dPosInfo,char *errMsg)
{
	
	char rsp_code[4+1];
	char rcvData[255];
	int rcvLen;

	bbzero(rsp_code);
	bbzero(rcvData);

	wrLog("选钱包应用目录-------------------------------------");
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01",17, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互失败，请连接好读卡器");
		return -1;
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE * )rsp_code,2);
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
		
	}
 
	return 0;
}

//读取卡片余额,需要先进入钱包目录

int CDGTBusTrade::getWalletMny(const TSDLLPosInfo dPosInfo,char *errMsg,PDGTCard pdgtCard)
{

	char rsp_code[4+1];
	int dataLen;
	int rcvLen;
	char rcvData[255];
	char SW1SW2[4+1];

	bbzero(rcvData);
	bbzero(rsp_code);
	bbzero(SW1SW2);

	//读取卡片余额；
	wrLog("读取卡片余额--------------------------------------------");
	
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x32\x26\xFF\x80\x5C\x00\x02\x04",8, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互失败，请连接好读卡器");
		return -1;
	}
	

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	//读取卡片余额
	char cardMny[8+1];
	char mny[12+1];
	bbzero(cardMny);
	bbzero(SW1SW2);
	
	wrLog("输出卡片余额信息");
	dataLen=2;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE * )cardMny,4);
	dataLen+=4;

	CTools::byte_to_hex((BYTE *)rcvData+dataLen,(BYTE *)SW1SW2,2);
	
	CTools::HexToDec(cardMny,mny);
	wrLog("卡片余额:%s",(BYTE *)cardMny);
	wrLog("SW1SW2:%s",SW1SW2);
	
	wrLog("输出卡片余额信息");
	wrLog("卡片余额:%.2f",atof(mny)/100);

	if (strcmp(SW1SW2,"9000")!=0) //SW1SW2正常情况是9000
	{
		bbzero(errMsg);
		strcpy(errMsg,"卡片响应失败");
		return -2;
	}

	//卡片余额。

	bbzero(pdgtCard->cardMny); //更新钱包余额
	strcpy(pdgtCard->cardMny,mny);

	return 0;
}

//flag 表示查询的类型，flag 等于默认只查询余额。flag=1要查询余额等其他基础信息

int CDGTBusTrade::cardQuery(const TSDLLPosInfo dPosInfo,char *rsp_code,char *errMsg,PDGTCard pdgtCard,int flag)
{
	int iRet;

	//1寻卡
	iRet=cardSearch(dPosInfo,rsp_code,errMsg,pdgtCard,flag);
	if (iRet!=0)
	{
		return -1; //寻卡失败
	}


	 //读取0005文件，押金在此

	//查询卡信息。 //直接读取余额的时候，不需要读取
	iRet=readCardInfo(dPosInfo,pdgtCard,errMsg);
	if (iRet!=0)
	{
		wrLog("读取卡片信息错误");
		return -4;
	}


	//进入卡片钱包目录下
	iRet=accessWalletDir(dPosInfo,errMsg);
	if (iRet!=0)
	{
		return -2;
	}

	iRet=getWalletMny(dPosInfo,errMsg,pdgtCard); //进入钱包余额
	if (iRet!=0)
	{
		return -3; //进入钱包余额失败

	}


	//读取卡片基本信息
	iRet=readCardBasicInfo(dPosInfo,errMsg,pdgtCard,flag);
	
	if (iRet!=0)
		
	{
			wrLog("读取卡片物理信息失败");
	}

	return 0;
}


//获取东莞通M1卡余额信息 type 0 进行查询余额 1进行交易
int CDGTBusTrade::getM1BasicInfo(const TSDLLPosInfo dPosInfo,int type,PDGTM1Card pdgtM1Card)
{
	char rcvData[255];
	char sndIcData[255];
	char errMsg[200];
	char rsp_code[4+1];
	bbzero(rcvData);
	char date[8+1];
	char check_date[6+1];
	char cardMny[8+1];
	char outPutMsg[50];
	int iSelect;

	int rcvLen=0;
	int sendLen=0;

	bbzero(sndIcData);
	bbzero(date);
	bbzero(errMsg);
	bbzero(rsp_code);
	bbzero(check_date);
	bbzero(cardMny);
	bbzero(outPutMsg);


	CTools::get_date8(date);
	memcpy(check_date,date+2,6);

	if( dgtSndRcvHID_IcData(dPosInfo,(const BYTE *)"\xC2\x81",2, (BYTE *)rcvData, rcvLen,errMsg ) )  //充值验卡是读卡器之间的。
		
	{
		TS_OutPutMsg(dPosInfo," 终端交互出错,请连接好读卡器!",0,iSelect);
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		return -3;
	}
		

	CTools::byte_to_hex((const BYTE *)rcvData+2,(BYTE*)pdgtM1Card->bus_card,4); //票卡号
	CTools::byte_to_hex((BYTE *) rcvData+6,(BYTE *)pdgtM1Card->recogise_code,4); //票卡认证码
	CTools::byte_to_hex((const BYTE *)rcvData+10, (BYTE *)cardMny, 4); //票卡金额（充值前）
	CTools::byte_to_hex((const BYTE *)rcvData+14, (BYTE *)pdgtM1Card->bus_card_type, 1); //应用子类型，卡片类型
	CTools::byte_to_hex((const BYTE *)rcvData+17, (BYTE *)pdgtM1Card->check_date, 3); //年审日期

	wrLog("年审日期：%s",pdgtM1Card->check_date);

	if (type==1)
	{

		if (((unsigned char)(rcvData[16])==0xAA)) //新卡是0XAA
		{
			TS_OutPutMsg(dPosInfo," \n该卡为空白卡，不能进行进行该业务!\n   按【确认】返回",0,iSelect);
			return -6;
		}
		
		if (strcmp(pdgtM1Card->bus_card_type,"01")&&strcmp(pdgtM1Card->bus_card_type,"A4"))
		{
			TS_OutPutMsg(dPosInfo," \n卡类型错误或卡已过期\n    不能进行该业务!\n   按【确认】返回",0,iSelect);
			return -7;
		}
		if ((atoi(pdgtM1Card->check_date)!=0) && (atoi(check_date)>atoi(pdgtM1Card->check_date)))
		{
			TS_OutPutMsg(dPosInfo,"  \n     已过年审期!\n    不能进行该业务!\n   按【确认】返回",0,iSelect);
			return -8;
		}
	
	}
	
	//查询余额
	if (type==0)
	{
		if (((unsigned char)(rcvData[16])==0xAA)) //新卡是0XAA
		{
			TS_OutPutMsg(dPosInfo," \n该卡为空白卡!\n   按【确认】返回",0,iSelect);
			return -6;
		}
	}

	memcpy(pdgtM1Card->dkq_code,rcvData+20,4);
	CTools::byte_to_hex((const BYTE *)rcvData+24, (BYTE *)pdgtM1Card->psam, 8); //psam卡号
	
	wrHex("充值验卡返回机具编号",(BYTE *)pdgtM1Card->psam,4);
	wrLog("PSAM卡号是:%s",pdgtM1Card->psam);
	//转换成10进制的卡号
	CTools::getDGTM1CardId(pdgtM1Card->bus_card,pdgtM1Card->bus_showCard);
	
	//清空读卡器信息
	bbzero(rcvData);
	rcvLen=0;
	wrLog("清空读卡器显示信息");
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\xF0\x40\x00\x01",4, (BYTE *)rcvData, rcvLen,errMsg )) //充值验卡是读卡器之间的。
	{
			
		TS_OutPutMsg(dPosInfo," 终端交互出错，请连接读卡器!\n按【确认】返回",0,iSelect);
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		return -3;
	}
		

	//发送读卡器显示余额信息
	wrLog("发送给读卡器显示信息的指令");
	char bus_amt[8+1];
	int msg_len=0;
	bbzero(outPutMsg);
	bbzero(pdgtM1Card->bus_amt);
	CTools::sixten_toten(cardMny,bus_amt);
	sprintf(pdgtM1Card->bus_amt,"%08s",bus_amt);

	sprintf(outPutMsg,"余额:%.2f元",atof(pdgtM1Card->bus_amt)/100); //在读卡器上面显示的
	bbzero(sndIcData);
	sendLen=0;

	memcpy(sndIcData,"\xF0\x41\x01\x01",4); //显示一行信息
	sendLen+=4;
	memcpy(sndIcData+sendLen,outPutMsg,strlen(outPutMsg)); 
	sendLen+=strlen(outPutMsg);
	
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen ,errMsg) )//充值验卡是读卡器之间的。
	{
		TS_OutPutMsg(dPosInfo,"终端交互出错，请连接好读卡器",0,iSelect);
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
		return -3;
	}
		

	return 0;


}

//查询M1卡信息 0 ,查询余额。1 是表示交易
int CDGTBusTrade::cardM1Query(const TSDLLPosInfo dPosInfo,char *rsp_code,int type,char *errMsg,PDGTM1Card pdgtM1Card)
{
	int iRet;
	
	//查询卡片有效期等信息
	iRet=getM1SBInfo(dPosInfo,pdgtM1Card,errMsg);
	if (iRet!=0)
	{
		return iRet; //寻卡失败
	}

	//读取卡片余额等信息

	iRet=getM1BasicInfo(dPosInfo,type,pdgtM1Card);
	if (iRet!=0)
	{
		if (iRet==-6)
		{
			return -4; //对卡片是空白卡进行特殊返回码处理
		}
		else
		{
			return -5;

		}
	
	}

	//输入M1卡基础信息

	wrLog("卡片余额:%s",pdgtM1Card->bus_amt);
	wrLog("卡号:%s",pdgtM1Card->bus_card);
	wrLog("可显示卡号:%s",pdgtM1Card->bus_showCard);
	wrLog("卡片类型:%s",pdgtM1Card->bus_card_type);
	wrLog("子卡类型:%s",pdgtM1Card->card_child_type);
	wrLog("卡主类型:%s",pdgtM1Card->card_parent_type);
	wrLog("年审日期:%s",pdgtM1Card->check_date);
	wrLog("cardCSN:%s",pdgtM1Card->M1CardCSN);
	wrLog("认证码:%s",pdgtM1Card->recogise_code);
	wrLog("押金:%s",pdgtM1Card->deposit);
	wrLog("开卡日期:%s",pdgtM1Card->openDate);
	wrLog("开卡时间:%s",pdgtM1Card->openTime);
	wrLog("卡片有效期:%s",pdgtM1Card->cardVadate);
	wrLog("卡片状态:%s",pdgtM1Card->cardStatus);

	return 0;
}


//蜂鸣器响一下

int CDGTBusTrade::buzzer(const TSDLLPosInfo dPosInfo,char *errMsg)
{

	char rsp_code[4+1];
	char rcvData[255];
	int rcvLen;

	bbzero(rcvData);
	bbzero(rsp_code);

	//峰鸣器响一下,表示成功。
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\x31\x13\x00\x32\x01",5, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互失败，请连接好读卡器");
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	return 0;


}

int CDGTBusTrade::clearLEDMsg(const TSDLLPosInfo dPosInfo,char *errMsg)
{

	char rcvData[255];
	int rcvLen;
	char rsp_code[4+1];

	bbzero(rcvData);
	bbzero(rsp_code);

	rcvLen=0;
	wrLog("清空读卡器显示信息");
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)"\xF0\x40\x00\x01",4, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"与终端交互失败，请连接好读卡器");
		return -1;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	return 0;

}



int CDGTBusTrade::showLEDMsg(const TSDLLPosInfo dPosInfo,char *showMsg,char *errMsg)
{

	//发送读卡器显示余额信息
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int iRet;

	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);

	//需要先执行清空信息的操作
	iRet=clearLEDMsg(dPosInfo,errMsg);
	if (iRet!=0)
	{
		return -2; //表示清空显示屏失败
	}

	wrLog("发送给读卡器显示信息");
	
	memcpy(sndIcData,"\xF0\x41\x01\x01",4); //显示一行信息
	sendLen+=4;
	
	memcpy(sndIcData+sendLen,showMsg,strlen(showMsg)); 
	sendLen+=strlen(showMsg);
	
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;

	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	return 0;

}


//开卡操作
/************************************************************************/
/*                                                                      */
//openDate :开卡时间
//effectiveDate:有效时间
//cardStatus:卡片状态
//cardInfo ：卡片基础信息 0015文件中的10-20
//dicMac:指令mac
/************************************************************************/

int CDGTBusTrade::openCard(const TSDLLPosInfo dPosInfo,char *APDU,char *dicMac,char *errMsg)
{
	
	//获取随机数
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char APDU_COMMAND[100];
	char DIC_MAC_COMAND[8+1];
	char SW1SW2[4+1];

	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(APDU_COMMAND);
	bbzero(DIC_MAC_COMAND);
	bbzero(SW1SW2);

	memcpy(DIC_MAC_COMAND,dicMac,8); 
	wrLog("激活卡片");
	wrLog("apdu_command222:%s",APDU);

	wrHex("DICMAC:",(BYTE *)dicMac,strlen(dicMac));
	memcpy(sndIcData,"\x32\x26\xff ",3); 
	sendLen+=3;
	memcpy(sndIcData+sendLen,APDU,strlen(APDU));
	sendLen+=strlen(APDU);

	CTools::hex_to_byte((BYTE *) DIC_MAC_COMAND,(BYTE *) sndIcData+sendLen,8);
	sendLen+=4;
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误 明确失败
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
	}
	
	//售卡执行执行成功
	CTools::byte_to_hex((BYTE *)rcvData+2,(BYTE *)SW1SW2,2);
	wrLog("sw1sw2HAHH:%s",SW1SW2);
	if (strcmp(SW1SW2,"9000") !=0) //如果sw1sw2不等于9000 则表示 卡执行失败 
	{
		return -2; //写卡失败 明确失败
	}

	return 0;
	
}


//开卡操作
/************************************************************************/
/*                                                                      */
//openDate :开卡时间
//effectiveDate:有效时间
//cardStatus:卡片状态
//cardInfo ：卡片基础信息 0015文件中的10-20
//dicMac:指令mac
/************************************************************************/

int CDGTBusTrade::openCard(const TSDLLPosInfo dPosInfo,char *dicMac,char *errMsg,DGTCard dgtCard)
{
	
	//获取随机数
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char APDU_COMMAND[100];
	int APDU_LEN=0;
	char APDU[100];
	char DICMAC_COMMAND[8+1];
	char SW1SW2[4+1];
	char openTime[6+1];
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(APDU_COMMAND);
	bbzero(APDU);
	bbzero(DICMAC_COMMAND);
	bbzero(SW1SW2);
	bbzero(openTime);

	wrLog("激活卡片");
	char openDate[14+1]; //开卡日期
	char effectiveDate[14+1];//有效日期
	bbzero(openDate);
	bbzero(effectiveDate);
	//设置开卡日期
	CTools::getDate(openDate);
	CTools::getTime(openTime);
	
	bbzero(dgtCard.openTime);
	strcpy(dgtCard.openTime,openTime);

	//设置有效日期 有效日期固定为表示普通卡
	strcpy(effectiveDate,"20901231");

	//调用服务进行处理 之前组装APDU命令

	//wrlog("DICMAC:%s",dicMac);
	memcpy(DICMAC_COMMAND,dicMac,8); 
	memcpy(sndIcData,"\x32\x26\xff\x04\xd6\x95\x08\x18 ",8); 
	sendLen+=8;
	memcpy(sndIcData+sendLen,"\x01",1); //开通卡片
	sendLen+=1;
	memcpy(sndIcData+sendLen,dgtCard.appVer,1);
	sendLen+=1;
	memcpy(sndIcData+sendLen,dgtCard.connectCity,2);
	sendLen+=2;
	memcpy(sndIcData+sendLen,dgtCard.appSerial,8); //卡序列号
	sendLen+=8;
	CTools::hex_to_byte((BYTE *)openDate,(BYTE *)sndIcData+sendLen,8); //开卡时间
	sendLen+=4;
	CTools::hex_to_byte((BYTE *)effectiveDate, (BYTE *)sndIcData+sendLen,8);
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) DICMAC_COMMAND,(BYTE *) sndIcData+sendLen,8);
	sendLen+=4;
	
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -2;
	}
	
	//售卡执行执行成功
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *)SW1SW2,2);

	if (strcmp(SW1SW2,"9000")!=0) //如果卡返回的数据是9000 test
	{
		//卡执行错误.
		return -2;

	}
	
	return 0;
	
}


//初始化CPU卡
//02 00 00 32 26 ff 805000020B[02充值密钥索引]  00000001  000000000002  10 [xor] 03
//cardAmount :表示卡片充值前余额
//cardCount:卡片计数器
//initRandom:随机数
//东莞通卡片密钥索引是:01.
//执行成功的时候返回

int CDGTBusTrade::initCPUCard(const TSDLLPosInfo dPosInfo,char *POSID,char *payAmount,char *cardSeq,char *keyVer,char *ALGIND,char *initRandom,char *cardAmount,char *MAC1,char *errMsg)
{
	//获取随机数
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char initMNY[8+1];
	char  POS_ID_BYTE[6+1];
	char cardCount[4+1];

	bbzero(initMNY);
	bbzero(cardCount);
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(POS_ID_BYTE);

	wrLog("payAmount:%s",payAmount);
	CTools::DecToHex(payAmount, initMNY,8); 
	wrLog("获取随机数");
	wrLog("获取随机数POSID:%s",POSID);

	memcpy(sndIcData,"\x32\x26\xff\x80\x50\x00\x02\x0B\x01",9); //密钥索引是01
	sendLen+=9;
	//金额
	CTools::hex_to_byte((BYTE *) initMNY,(BYTE *)sndIcData+sendLen,8);
	sendLen+=4;
	//终端变化
	CTools::hex_to_byte((BYTE *)POSID,(BYTE *)sndIcData+sendLen,strlen(POSID));
	sendLen+=6;
	memcpy(sndIcData+sendLen,POS_ID_BYTE,strlen(POS_ID_BYTE));
	sendLen+=strlen(POS_ID_BYTE);
	CTools::hex_to_byte((BYTE *) "10",(BYTE *)sndIcData+sendLen,2);
	sendLen+=1;
	
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;
		
	}
	
	if (rcvLen<=4) //如果读卡器及卡响应错误
	{
		bbzero(errMsg);
		strcpy(errMsg,"CPU卡初始化失败");
		return -2;
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	//获取交易前余额，卡计数器，随机数，MAC1
	dataLen=2;
	bbzero(cardAmount);
	bbzero(cardCount);
	bbzero(initRandom);
	bbzero(MAC1);
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)cardAmount,4);
	dataLen+=4;

	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)cardCount,2); //交易序号
	dataLen+=2;

	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)keyVer,1); //密钥版本
	dataLen+=1;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)ALGIND,1); //算法标志
	dataLen+=1;

	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)initRandom,4); //随机数
	dataLen+=4;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)MAC1,4);
	dataLen+=4;
	
	char CARD_trade_seq_tmp[8+1];
	bbzero(CARD_trade_seq_tmp);
	sprintf(CARD_trade_seq_tmp,"0000%s",cardCount);
	bbzero(cardCount);
	CTools::HexToDec(CARD_trade_seq_tmp,cardCount,8); //转换成10进制的6位数字。
	memcpy(cardSeq,cardCount+2,6);
	

	//输出信息

	wrLog("cpu初始化输出信息");

	wrLog("交易前余额:%s",cardAmount);
	wrLog("卡片交易序号:%s",cardSeq);
	wrLog("密钥版本:%s",keyVer);
	wrLog("算法标志:%s",ALGIND);
	wrLog("交易序号:%s",cardSeq);

	wrLog("随机数:%s",initRandom);
	wrLog("MAC1:%s",MAC1);

	return 0;

}


//getPIN2 //计算PIN2
int CDGTBusTrade::getPIN2(char *appSerial,char *PIN2)
{

	char szMAK[17] = {0};
	char szCardNo[17] = {0};

	int a = 0;
	int b = 0;

	wrLog("appserial:%s",appSerial);

	memcpy(szMAK, RLD_PIN_MAK, 16);
	memcpy(szCardNo, appSerial + 0, 16);
	
	memset(PIN2, 0, sizeof(PIN2));
	for (int nCur = 0; nCur < 16; nCur++)
	{
		a = szCardNo[nCur] - '0';
		b = szMAK[nCur] - '0';
		PIN2[nCur] = (a + b) % 10 + '0';
	}

	wrLog("pin2:%s",PIN2);

	return 0;
}


//校验PIN2

int CDGTBusTrade::checkPIN2(const TSDLLPosInfo dPosInfo,char *appSerial,char *errMsg )
{
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int iRet;
	int dataLen=0;
	char PIN2[17+1];
	char SW1SW2[4+1];
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(PIN2);
	bbzero(SW1SW2);
	
	//结算PIN2:
	iRet=getPIN2(appSerial,PIN2);
	wrLog("校验PIN2");
	
	memcpy(sndIcData,"\x32\x26\xff\x00\x20\x00\x01\x06",8); 
	sendLen+=8;
	//pin2
	CTools::hex_to_byte((BYTE *)PIN2,(BYTE *)sndIcData+sendLen,12);
	sendLen+=6;

	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		wrLog("校验pin失败");
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	bbzero(SW1SW2);
	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *) SW1SW2,2);
	if (strcmp(SW1SW2,"9000")!=0) //如果SW1SW2不是9000.表示卡响应失败
	{
		wrLog("校验pin失败");
		return -2;

	}

	wrLog("校验PIN成功");
	return 0;

}


//读取所有记录
//读取交易之前需要上电.

int CDGTBusTrade::readAllTxnLogs(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char* rsp_code,char *errMsg)
{

	char sndIcData[255];
	char rcvData[255];
	char tradeTime[14+1]; 
	int rcvLen=0;
	int sendLen=0;
	int dataLen=0;
	char SW1SW2[4+1];
	char index_temp[2+1];
	int iRet;
	int iSelect;

	TSTxnLogs txnLogs;
	dgtALLTrades dgtALLTxnLogs;
	memset(&dgtALLTxnLogs,0,sizeof(dgtALLTxnLogs));
	memset(&txnLogs,0,sizeof(txnLogs));
	bbzero(tradeTime);
	bbzero(SW1SW2);

	//卡片上线

	//三次读卡
	for(int q=0;q<3;q++)
	{
		
		iRet=CDGTBusTrade::cardQuery(dPosInfo,rsp_code,errMsg,pdgtCard,1);
		if (iRet!=0) //如果读取错误
		{
			if (strcmp(rsp_code,"3005")==0 || strcmp(rsp_code,"3006")==0 ) //如果出现写卡超时的情况
			{
				
				bbzero(errMsg);
				strcpy(errMsg,"查询读卡器失败，请将卡放在读卡器上重试\n 请按【Enter】继续");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;
				
			}
			
			
		}
		else //读卡成功直接退出寻卡
		{
			
			break;
		}
		
		
	}
	
	if (iRet!=0)
	{
		return -3; //查询失败
	}


	int j=0;
	for(int i=0;i<10;i++)
	{
		char tradeDate[8+1];
		char tradeTime[6+1];
		bbzero(tradeDate);
		bbzero(tradeTime);

		wrLog("读取充值交易记录");
		bbzero(sndIcData);
		bbzero(rcvData);
		rcvLen=0;
		sendLen=0;
		bbzero(index_temp);
		sprintf(index_temp,"%02d",(i+1));
		
		memcpy(sndIcData,"\x32\x26\xff\x00\xB2 ",5);
		sendLen+=5;
		//index
		CTools::hex_to_byte((BYTE *)index_temp,(BYTE *)sndIcData+sendLen,2);
		sendLen+=1;
		CTools::hex_to_byte((BYTE *)"C400",(BYTE *)sndIcData+sendLen,4);//00 表示读取至文件尾部
		sendLen+=2;

		bbzero(rcvData);
		bbzero(rsp_code);
		rcvLen=0;
		//发信息到终端 
		if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
		{
			bbzero(errMsg);
			strcpy(errMsg,"终端交互出错，请连接好读卡器");
			return -1;
			
		}
		
		if (rcvLen<=4) //长度是4，则响应失败
		{
			continue;
		}

		bbzero(rsp_code);
		CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
		
		if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
			
		{
			continue; 
		}
		
		//执行成功。返回明细内容
		dataLen=2;
		CTools::byte_to_hex((BYTE *)rcvData+18,(BYTE *)tradeDate,4);
		CTools::byte_to_hex((BYTE *) rcvData+22,(BYTE *)tradeTime,3);
		if (atoi(tradeDate)==0 && atoi(tradeTime)==0)
		{
			continue;
		}
		
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtALLTxnLogs.cardAllTradeDetail[j].cardSEQ,2);
		dataLen+=2;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtALLTxnLogs.cardAllTradeDetail[j].limitMny,3);
		dataLen+=3;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].tradeMny,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].tradeType,1);
		dataLen+=1;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtALLTxnLogs.cardAllTradeDetail[j].POS_ID,6);
		dataLen+=6;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].trade_date,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtALLTxnLogs.cardAllTradeDetail[j].trade_time,3);
		dataLen+=3;
		j++;
		
	}
	
	dgtALLTxnLogs.nCnt=j;
	if (dgtALLTxnLogs.nCnt==0)
	{
		return -2; //明确无交易记录
	}

	//显示列表
	txnLogs.colsnum = 7;
	txnLogs.colsWidth[0] = 100;
	txnLogs.colsWidth[1] = 60;
	txnLogs.colsWidth[2] = 60;
	txnLogs.colsWidth[3] = 60;
	txnLogs.colsWidth[4] = 60;
	txnLogs.colsWidth[5] = 60;
	txnLogs.colsWidth[6] = 60;

	txnLogs.colsNameLen = 200;
	memcpy(txnLogs.colsName, "公交卡号\x00交易序号\x00透支限额(元)\x00交易金额(元)\x00交易类型\x00交易日期\x00交易时间", txnLogs.colsNameLen);
	
	for (int m =0 ;m <dgtALLTxnLogs.nCnt;m++)
	{

		char cardSEQ[6+1];
		bbzero(cardSEQ);
		char limitMny[12+1];
		char showLimitMny[10];
		char showTradeMny[10];
		char tradeMny[12+1];
		char tradeType[10];
		char index[2+1];

		bbzero(limitMny);
		bbzero(tradeMny);
		bbzero(showLimitMny);
		bbzero(tradeType);
		bbzero(showTradeMny);
		bbzero(index);

		if( txnLogs.itemsLen+strlen(pdgtCard->cardShowId) < MAX_INTERM_BUF ) //公交卡号
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,pdgtCard->cardShowId); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		CTools::HexToDec(dgtALLTxnLogs.cardAllTradeDetail[m].cardSEQ,cardSEQ,6);
		if (strcmp(dgtALLTxnLogs.cardAllTradeDetail[m].tradeType,"02")==0)
		{
			strcpy(tradeType,"充值");
		}
		else
		{
		
			strcpy(tradeType,"其他");
		}


		CTools::HexToDec(dgtALLTxnLogs.cardAllTradeDetail[m].tradeMny,tradeMny,12);
		sprintf(showTradeMny,"%.2f",atof(tradeMny)/100);
		CTools::HexToDec(dgtALLTxnLogs.cardAllTradeDetail[m].limitMny,limitMny,12);
		sprintf(showLimitMny,"%.2f",atof(limitMny)/100);
		

		if( txnLogs.itemsLen+strlen(cardSEQ) < MAX_INTERM_BUF ) //交易序号
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,cardSEQ); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(showLimitMny) < MAX_INTERM_BUF ) //透支金额
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showLimitMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}
		
		if( txnLogs.itemsLen+strlen(showTradeMny) < MAX_INTERM_BUF ) //交易金额
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showTradeMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(tradeType) < MAX_INTERM_BUF ) //交易类型
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,tradeType); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtALLTxnLogs.cardAllTradeDetail[m].trade_date) < MAX_INTERM_BUF ) //交易日期
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtALLTxnLogs.cardAllTradeDetail[m].trade_date); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtALLTxnLogs.cardAllTradeDetail[m].trade_time) < MAX_INTERM_BUF ) //交易时间
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtALLTxnLogs.cardAllTradeDetail[m].trade_time); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


	}

	sprintf(txnLogs.bottom," 查询最近10笔公交充值交易 返回请按【Esc】") ;
	iRet=TS_ShowTxnLogs(dPosInfo,&txnLogs);

	if (iRet!=0)
	{
		wrLog("TS_ShowTxnLogs返回错误[%d]", iRet); 
		return -1;
	}

	return iRet;

	
}


//充值记录 
//获取充值记录之前需要进行寻卡操作并且进入钱包目录下

int CDGTBusTrade::readChargeLogs(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char *rsp_code,char *errMsg)
{

	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	int dataLen=0;
	char SW1SW2[4+1];
	char index_temp[2+1];
	int iRet;
	TSTxnLogs txnLogs;
	dgtChargeTrades dgtChargeLogs;
	int iSelect;
	char tradeTime[8+1];
	char tradeDate[6+1];

	dgtChargeLogs.nCnt=0;
	memset(&txnLogs,0, sizeof(txnLogs));
	memset(&dgtChargeLogs,0,sizeof(dgtChargeLogs));
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(tradeTime);
	bbzero(SW1SW2);
	bbzero(tradeTime);
	bbzero(tradeDate);

	//三次读卡
	for(int q=0;q<3;q++)
	{
	
		iRet=CDGTBusTrade::cardQuery(dPosInfo,rsp_code,errMsg,pdgtCard,1);
		if (iRet!=0) //如果读取错误
		{
			if (strcmp(rsp_code,"3005")==0 || strcmp(rsp_code,"3006")==0 ) //如果出现写卡超时的情况
			{

				bbzero(errMsg);
				strcpy(errMsg,"查询读卡器失败，请将卡放在读卡器上重试\n 请按【Enter】继续");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;

			}
			
		
		}
		else //读卡成功直接退出寻卡
		{
			
			break;
		}

		
	}

	if (iRet!=0)
	{
		return -3; //查询失败
	}

	memset(&dgtChargeLogs,0,sizeof(dgtChargeLogs));

	int j=0;

	for(int i=0;i<4;i++)
	{
		wrLog("读取充值交易记录");
		bbzero(index_temp);
		bbzero(sndIcData);
		bbzero(rcvData);
		rcvLen=0;
		sendLen=0;

		sprintf(index_temp,"%02d",(i+1));
		memcpy(sndIcData,"\x32\x26\xff\x00\xB2",5);
		sendLen+=5;
		//index
		CTools::hex_to_byte((BYTE *)index_temp,(BYTE *)sndIcData+sendLen,2);
		sendLen+=1;
		CTools::hex_to_byte((BYTE *)"DC00",(BYTE *)sndIcData+sendLen,4); //00表示知道文件结束为止
		sendLen+=2;
		
		//发信息到终端 
		if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) )//充值验卡是读卡器之间的。
		{
			bbzero(errMsg);
			strcpy(errMsg,"终端交互出错，请连接好读卡器");
			return -1;
			
		}
		
		if (rcvLen<=4) //如果小于四个字节 //无返回数据
		{
			continue;
		}


		bbzero(rsp_code);
		CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

		if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
			
		{
			//bbzero(errMsg);
			//returnHIDGTFail(rsp_code,errMsg);  
			continue; 
		}

		//如果执行不成功的话，数据长度是2个字节读卡器响应+2个字节SW1SW2 =4个字节
		//执行成功。返回明细内容
		dataLen=2;
		//	判断交易时间和交易日期为0则说明是空记录
		
		CTools::byte_to_hex((BYTE *)rcvData+18,(BYTE *)tradeDate,4);
		CTools::byte_to_hex((BYTE *) rcvData+22,(BYTE *)tradeTime,3);
		if (atoi(tradeDate)==0 && atoi(tradeTime)==0)
		{
			continue;
		}
		

		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].cardSEQ,2);
		dataLen+=2;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].tradeAfterMny,3);
		dataLen+=3;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].tradeMny,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].tradeType,1);
		dataLen+=1;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)dgtChargeLogs.cardChargeDetail[j].POS_ID,6);
		dataLen+=6;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].trade_date,4);
		dataLen+=4;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].trade_time,3);
		dataLen+=3;
		CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *) dgtChargeLogs.cardChargeDetail[j].TAC,4);
		dataLen+=4;
		j++;
	
	}

	dgtChargeLogs.nCnt=j;

	if (dgtChargeLogs.nCnt==0)
	{
		return -2; //明确无交易记录
	}

	//显示列表
	txnLogs.colsnum = 7;
	txnLogs.colsWidth[0] = 100;
	txnLogs.colsWidth[1] = 60;
	txnLogs.colsWidth[2] = 60;
	txnLogs.colsWidth[3] = 60;
	txnLogs.colsWidth[4] = 60;
	txnLogs.colsWidth[5] = 60;
	txnLogs.colsWidth[6] = 60;
	txnLogs.colsNameLen = 200;
	memcpy(txnLogs.colsName, "公交卡号\x00交易序号\x00交易后余额(元)\x00交易金额(元)\x00交易类型\x00交易日期\x00交易时间", txnLogs.colsNameLen);
	
	for (int m =0 ;m <dgtChargeLogs.nCnt;m++)
	{

		char cardSEQ[6+1];
		bbzero(cardSEQ);
		char tradeAfterMny[12+1];
		char showCardMny[10];
		char showTradeMny[10];
		bbzero(showCardMny);
		char tradeMny[12+1];
		bbzero(tradeMny);
		bbzero(showCardMny);
		char tradeType[10];
		bbzero(tradeType);
		bbzero(showTradeMny);
		char index[2+1];
		bbzero(index);

		if( txnLogs.itemsLen+strlen(pdgtCard->cardShowId) < MAX_INTERM_BUF ) //公交卡号
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,pdgtCard->cardShowId); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		CTools::HexToDec(dgtChargeLogs.cardChargeDetail[m].cardSEQ,cardSEQ,6);
		if (strcmp(dgtChargeLogs.cardChargeDetail[m].tradeType,"02")==0)
		{
			strcpy(tradeType,"充值");
		}
		else
		{
		
			strcpy(tradeType,"其他");
		}


		CTools::HexToDec(dgtChargeLogs.cardChargeDetail[m].tradeAfterMny,tradeAfterMny,12);
		sprintf(showCardMny,"%.2f",atof(tradeAfterMny)/100);
		
		CTools::HexToDec(dgtChargeLogs.cardChargeDetail[m].tradeMny,tradeMny,12);
		sprintf(showTradeMny,"%.2f",atof(tradeMny)/100);

		if( txnLogs.itemsLen+strlen(cardSEQ) < MAX_INTERM_BUF ) //交易序号
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,cardSEQ); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(showCardMny) < MAX_INTERM_BUF ) //交易后余额
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showCardMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}
		
		if( txnLogs.itemsLen+strlen(showTradeMny) < MAX_INTERM_BUF ) //交易金额
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,showTradeMny); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


		if( txnLogs.itemsLen+strlen(tradeType) < MAX_INTERM_BUF ) //交易类型
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,tradeType); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtChargeLogs.cardChargeDetail[m].trade_date) < MAX_INTERM_BUF ) //交易日期
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtChargeLogs.cardChargeDetail[m].trade_date); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}

		if( txnLogs.itemsLen+strlen(dgtChargeLogs.cardChargeDetail[m].trade_time) < MAX_INTERM_BUF ) //交易时间
		{
			strcpy(txnLogs.items+txnLogs.itemsLen,dgtChargeLogs.cardChargeDetail[m].trade_time); 
			txnLogs.itemsLen = txnLogs.itemsLen + strlen(txnLogs.items+txnLogs.itemsLen) + 1;
			
		}
		
		else
		{
			break;
		}


	}

	sprintf(txnLogs.bottom," 查询公交卡最近4笔充值交易 返回请按【Esc】") ;
	iRet=TS_ShowTxnLogs(dPosInfo,&txnLogs);

	if (iRet!=0)
	{
		wrLog("TS_ShowTxnLogs返回错误[%d]", iRet); 
		return -1;
	}

	return iRet;

}

//写充值记录

int CDGTBusTrade::wrTxnLogs(const TSDLLPosInfo dPosInfo,char *posId,DGTCard dgtCard,char *tradeType,char *errMsg)
{

	int iRet;
	bbzero(errMsg);
	char appSerial[16+1];
	bbzero(appSerial);
	CTools::byte_to_hex((BYTE *) dgtCard.appSerial,(BYTE *)appSerial,8);

	//首先检验PIN2
	
	iRet=checkPIN2(dPosInfo,appSerial,errMsg);
	if (iRet!=0)
	{
		bbzero(errMsg);
		strcpy(errMsg,"PIN2校验错误");
		wrLog("PIN2校验错误");
		return -1;
	}

  
	iRet=wrCardTxnLog(dPosInfo,posId,dgtCard,tradeType,errMsg);
	if (iRet!=0)
	{
		bbzero(errMsg);
		strcpy(errMsg,"写卡失败");
		return -2; //写卡失败
	}

	wrLog("写卡充值记录成功");

	return 0;

}

//将交易结果写到卡片里面
//tradeType：交易类型
int CDGTBusTrade::wrCardTxnLog(const TSDLLPosInfo dPosInfo,char * posId,DGTCard dgtCard,char *tradeType,char *errMsg)
{

	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char SW1SW2[4+1];
	char tradeAfterMny[6+1]; //交易后金额
	char tradeMny[8+1]; //交易金额
	char cardCNT[4+1];

	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(SW1SW2);
	bbzero(tradeAfterMny);
	bbzero(tradeMny);
	bbzero(cardCNT);
	
	CTools::DecToHex(dgtCard.CARDCNT,cardCNT,4);
	CTools::DecToHex(dgtCard.cardMny,tradeAfterMny,6);
	CTools::DecToHex(dgtCard.tradeMny,tradeMny,8);
	wrLog("cardCNT:%s",cardCNT);
	wrLog("tradeAfterMny:%s",tradeAfterMny);
	wrLog("tradeMny:%s",tradeMny);

	memcpy(sndIcData,"\x32\x26\xff\x00\xE2\x00\xD8\x1F",8); 
	sendLen+=8;
	//添加交易记录
	CTools::hex_to_byte((BYTE *) cardCNT,(BYTE *)sndIcData+sendLen,4);
	sendLen+=2;
	CTools::hex_to_byte((BYTE *)tradeAfterMny,(BYTE *) sndIcData+sendLen,6 ); //充值后金额
	sendLen+=3;
	CTools::hex_to_byte((BYTE *) tradeMny,(BYTE *) sndIcData+sendLen,8); //交易金额 
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) tradeType,(BYTE *) sndIcData+sendLen,2); //交易类型
	sendLen+=1;

	CTools::hex_to_byte((BYTE *) posId,(BYTE *) sndIcData+sendLen,12); //交易终端号
	sendLen+=6;

	CTools::hex_to_byte((BYTE *) dgtCard.tradeDate,(BYTE *) sndIcData+sendLen,8); //交易日期
	sendLen+=4;
	CTools::hex_to_byte((BYTE *) dgtCard.tradeTime,(BYTE *) sndIcData+sendLen,6); //交易时间
	sendLen+=3;

	CTools::hex_to_byte((BYTE *) dgtCard.TAC,(BYTE *)sndIcData+sendLen,8);
	sendLen+=4;

	memcpy(sndIcData+sendLen,"\x00\x00\x00\x00",4);
	sendLen+=4;

	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}

	CTools::byte_to_hex((BYTE *) rcvData+2,(BYTE *) SW1SW2,2);

	if (strcmp(SW1SW2,"9000")!=0) //如果卡状态响应不是9000
	{
		return -2;
	}


	return 0;
	
}


//圈存
//02 00 14 32 26 FF 80 52 00 00 0B [20131213 15 10 09交易日期][xx xx xx xx MAC2]04 [xor] 03
//02 00 08 00 00 [xx xx xx xx TAC 4字节]9000 [xor]03

int CDGTBusTrade::cpuCard_load(const TSDLLPosInfo dPosInfo,char *tradeTime,char *MAC2,char *TAC,char *errMsg )
{
	
	char sndIcData[1024];
	char rcvData[1024];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	char SW1SW2[4+1];
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	bbzero(SW1SW2);
	wrLog("cpu钱包圈存");

	memcpy(sndIcData+sendLen,"\x32\x26\xFF\x80\x52\x00\x00\x0B",8); 
	sendLen+=8;
	//交易日期
	CTools::hex_to_byte((BYTE *)tradeTime,(BYTE *)sndIcData+sendLen,14);
	sendLen+=7;
	//MAC2
	CTools::hex_to_byte((BYTE *)MAC2,(BYTE *)sndIcData+sendLen,8);
	sendLen+=4;

	sndIcData[sendLen]='\x04';
	sendLen+=1;
	
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1; //超时的情况
		
	}
	
	wrLog("rcvLen：%d",rcvLen);
	if (rcvLen<4) // 响应码2个字节，卡片响应 2个字节 无tac （4个字节）
	{
		return -2;//充值失败
	}

	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);

	if (strcmp(rsp_code,"0000")!=0)
	{
		return -2;//读卡器响应错误,明确失败的情况
	}

	//TAC交易验证码

	bbzero(TAC);
	dataLen=2;

	CTools::byte_to_hex((BYTE *)rcvData+dataLen,(BYTE *)TAC,4);
	dataLen+=4;

	bbzero(SW1SW2);
	CTools::byte_to_hex((BYTE *)rcvData+dataLen,(BYTE *) SW1SW2,2);
	if (strcmp(SW1SW2,"9000")!=0) //如果sw1sw2不是9000
	{
		return -2; // 读卡器返回错误 明确失败

	}
	
	return 0;

}

//获取随机数Random

int CDGTBusTrade::getRandom(const TSDLLPosInfo dPosInfo,char *errMsg,char *Random)
{

	//获取随机数
	char sndIcData[255];
	char rcvData[255];
	int rcvLen=0;
	int sendLen=0;
	char rsp_code[4+1];
	int dataLen=0;
	
	bbzero(rsp_code);
	bbzero(sndIcData);
	bbzero(rcvData);
	
	wrLog("获取随机数");
	
	memcpy(sndIcData,"\x32\x26\xff\x00\x84\x00\x00\x04 ",8); //显示一行信息
	sendLen+=8;
	
	//发信息到终端 
	if( dgtSndRcvHID_IcData(dPosInfo, (const BYTE *)sndIcData,sendLen, (BYTE *)rcvData, rcvLen,errMsg) ) //充值验卡是读卡器之间的。
	{
		bbzero(errMsg);
		strcpy(errMsg,"终端交互出错，请连接好读卡器");
		return -1;
		
	}
	
	bbzero(rsp_code);
	CTools::byte_to_hex((BYTE *) rcvData,(BYTE *)rsp_code,2);
	
	if (strcmp(rsp_code,"0000")!=0) //执行指令不成功,提示错误
		
	{
		bbzero(errMsg);
		returnHIDGTFail(rsp_code,errMsg);  
		return -3;
	}
	
	//获取random
	bbzero(Random);
	dataLen=2;
	CTools::byte_to_hex((BYTE *) rcvData+dataLen,(BYTE *)Random,4);
	return 0;

}


//显示公交卡错误信息
void CDGTBusTrade::returnHIDGTFail(char *rsp_code,char *errMsg) //显示错误提示
{
	switch(atoi(rsp_code))
	{
		
	case 3001:
		bbzero(errMsg);
		strcpy(errMsg,"不支持非接触用户卡!");
		break;
	case 3005:
		bbzero(errMsg);
		strcpy(errMsg,"非接触用户卡激活失败");
		break;
	case 3006:
		bbzero(errMsg);
		strcpy(errMsg,"等待卡进入感应区超时");
		break;
	case 3009:
		bbzero(errMsg);
		strcpy(errMsg,"有多张卡在感应区");
		break;
	default:
		bbzero(errMsg);
		strcpy(errMsg,"未知错误");
		break;
	}
	
}


//初始化羊城通结构体 读卡的时候需要更新卡片基础信息
int CDGTBusTrade::init_dgtCard_data(PDGTCard pdgtCard)
{
	bbzero(pdgtCard->cardType); //卡片累心
	bbzero(pdgtCard->cardInfo); //卡片信息
	bbzero(pdgtCard->otherData);  //卡片其他信息
	bbzero(pdgtCard->CItyCode);  //城市编码
	bbzero(pdgtCard->compay_code); //合作城市编码
	bbzero(pdgtCard->busi_code);  
	bbzero(pdgtCard->deposit); //押金
	bbzero(pdgtCard->cardParent_type); //卡子
	bbzero(pdgtCard->cardChild_type);   //卡主
	bbzero(pdgtCard->openDate);   //开卡时间
	bbzero(pdgtCard->effectiveDate); //有效期
	bbzero(pdgtCard->deviceInfo); //设备信息
	bbzero(pdgtCard->appVer);   //卡片版本
	bbzero(pdgtCard->cardMny);  //卡片余额
	bbzero(pdgtCard->KEYVER);   //卡片密钥版本
	bbzero(pdgtCard->ALGIND);    //卡片算法
	bbzero(pdgtCard->cardShowId); //卡号
	bbzero(pdgtCard->CHARGE_DIVDATA);// 分散因子
	bbzero(pdgtCard->CARDSALE_DIVDATA); //分散因子
	bbzero(pdgtCard->CARDID); 
	bbzero(pdgtCard->CARDVERNO);  //卡内号
	bbzero(pdgtCard->EDCARDID);   
	bbzero(pdgtCard->connectCity);
	return 0;
}



//初始化东莞通M1卡信息
int CDGTBusTrade::init_dgtM1Card_data(PDGTM1Card pdgtM1Card)
{
	bbzero(pdgtM1Card->bus_card) ;//公交卡号
	bbzero(pdgtM1Card->bus_amt);
	bbzero(pdgtM1Card->bus_card_type); //应用子卡类型
	bbzero(pdgtM1Card->check_date) ;//年审日期
	bbzero(pdgtM1Card->psam); //psam卡号
	bbzero(pdgtM1Card->recogise_code); //认证码
	bbzero(pdgtM1Card->dkq_code);
	bbzero(pdgtM1Card->card_parent_type);
	bbzero(pdgtM1Card->card_child_type);
	bbzero(pdgtM1Card->M1CardCSN);
	bbzero(pdgtM1Card->bus_showCard);
	bbzero(pdgtM1Card->city_code);
	bbzero(pdgtM1Card->cardVadate); //有效期
	bbzero(pdgtM1Card->appSerial); //应用卡编号
	bbzero(pdgtM1Card->openDate) ; //开卡日期
	bbzero(pdgtM1Card->openTime) ; //开卡时间
	bbzero(pdgtM1Card->deposit) ; //押金

	return 0;
}


//东莞公交通讯报文
int CDGTBusTrade::commuWithDGT(const TSDLLPosInfo dPosInfo, char *sndData, int sndLen, char *rcvData, int *rcvLen)
{
	int iRet=0;
	*rcvLen=0;

	CConnect con;
	SOCKET sd;
	iRet = con.conToHost(DGTBUS_IP , DGTBUS_PORT , &sd);
	if (iRet != 0 )
	{
		return 1;
	}

	wrLog("向东莞通发送数据:%s", sndData);
	
	iRet = con.sndMsg(sd, sndData, sndLen);

	if( iRet!=0 )
	{
		return 1;
	}

	iRet = con.rcvMsg(sd, rcvData, rcvLen,1024);
	if( iRet!=0 )
	{
		closesocket(sd);
		return 2;
	}
	
	wrLog("从东莞通接收数据接收数据:%s",rcvData);

	closesocket(sd);

	if(*rcvLen == 0)
		return 1;

	return 0;
	
}


//type:交易类型 修改记录发送报文
int CDGTBusTrade::dgt_trade(CGlobal *pgbl, char *tranMessage, char *tranType,char *rsp_code)
{
	int iRet;
	char sndData[1024]={0};
	char rcvData[1024] = { 0 };

	bbzero(sndData);
	bbzero(rcvData);

	int rcvLen;
	int sndLen=0;

	iRet=packdgtFunc(pgbl,tranMessage,tranType,sndData,&sndLen);	//打包
	if (iRet!=0)
	{

		//发包错误
		return -1;

	}

	if (strcmp(tranMessage,"3460")==0  || strcmp(tranMessage,"3481") ==0 || strcmp(tranMessage,"2063")==0 || strcmp(tranMessage,"2480") ==0 || strcmp(tranMessage,"2481") ==0 \
		|| strcmp(tranMessage,"2062") ==0  ) //如果是卡完成或者卡撤销 售卡及充值的确认 在出现网络问题的时候，需要保存报文数据
	{
		
		CDB::setdgtResubmit(pgbl->Centerseqno,sndData); //resubmit 
		
	}

	for(int i=0;i<3;i++) //如果提交不成功。不用记录信息。及交易状态。并在下次进入业务的时候确认。
	{
	
		iRet=commuWithDGT( pgbl->dPosInfo,sndData,sndLen,rcvData, &rcvLen); //通讯 //如果因为通讯问题没有收到返回包，
		if (iRet!=0)
		{
			continue;
		}
		else
		{
			break;
		}

	}

	//如果三次执行返回，开卡确认，撤销和换卡完成，需要记录在数据库中执行的数据

	if(iRet!=0)
	{
		
		return -2;

	}

	//通讯错误

	 iRet=undgtpackFunc(pgbl,tranMessage,tranType,rcvData,rcvLen,rsp_code); //解包
	 if (iRet!=0)
	 {
		 return -3; //解包错误
	 }

	 return 0;

}


//卡管理
int CDGTBusTrade::cardManagement(CGlobal *pgbl, char *tranmessage, char *tranType,char *rsp_code,char * rsp_message)
{
	int iRet;
	char showMsg[500];
	bbzero(rsp_code);
	bbzero(rsp_message);
	bbzero(showMsg);

	iRet=dgt_trade(pgbl,tranmessage,tranType,rsp_code);
	if (iRet!=0)
	{
		//如果是网络繁忙
		if (iRet== -2)
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"网络繁忙，请稍后再试\n请联系在线客服或致电400-80-11888咨询!");
			return -2;
		}
		if (strlen(rsp_code)==2)
		{
			
			iRet=getCommuErr(rsp_code,rsp_message);
			
			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"网络繁忙，请稍后再试\n请联系在线客服或致电400-80-11888咨询!");
				
			}

			
		}
		else if (strlen(rsp_code)==5)
		{
			//对于报文中出现20002 报文完全成功，是成功的意思
			if (strcmp(rsp_code,"20002") ==0 )
			{
				return 0;
			}
			else
			{
			
				iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
				if (iRet!=0)
				{
					bbzero(rsp_message);
					strcpy(rsp_message,"网络繁忙，请稍后再试!");
					
				}
			}
		
			
		}
		
		else
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"网络繁忙，请稍后再试!");
			
		}

		return -1; //发送旧卡信息失败
		
	}
	
	return 0;
	
}


//换卡完成报文 可以多次提交。
int CDGTBusTrade::cardTransFinish(CGlobal *pgbl, char *tranmessage, char *tranType,char * rsp_code,char * rsp_message)
{
	int iRet;

	bbzero(rsp_code);
	bbzero(rsp_message);
	
	iRet=dgt_trade(pgbl,tranmessage,tranType,rsp_code);
	if (iRet!=0)
	{

		if (iRet==-2) //网络问题
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"网络繁忙，请稍后再试!");
			return -2;
		}

		if (strlen(rsp_code)==2)
		{
			
			iRet=getCommuErr(rsp_code,rsp_message);
			
			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"网络繁忙，请稍后再试!");
				
			}
			
		}
		else if (strlen(rsp_code)==5)
		{
			if (strcmp(rsp_code,"20002") ==0) //对于报文中返回20002的进行处理 是成功的意思
			{
				return 0;
			}
			else
			{
				iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
				if (iRet!=0)
				{
					bbzero(rsp_message);
					strcpy(rsp_message,"网络繁忙，请稍后再试!");
					
				}

			}
		
			
		}
		
		else
		{
			bbzero(rsp_message);
			strcpy(rsp_message,"网络繁忙，请稍后再试!");
			
		}
		
		return -1; //发送旧卡信息失败
		
	}
	
	return 0;
	
}


//售卡报文 
int CDGTBusTrade::cardSaleTrade(CGlobal *pgbl,char *tranMessage,char *tranType)
{
	int iRet;
	char rsp_code[5+1];
	char rsp_message[500];
	int iSelect;
	
	bbzero(rsp_code);
	bbzero(rsp_message);
	
	iRet=dgt_trade(pgbl,tranMessage,tranType,rsp_code);

	if (iRet!=0)
	{
		if (iRet== -2)
		{
	       TS_OutPutMsg(pgbl->dPosInfo,"网络繁忙，请稍后再试!",1,iSelect);
		   return  -2;

		}
		if (strlen(rsp_code)==2)
		{
			
			iRet=getCommuErr(rsp_code,rsp_message);
			
			if (iRet!=0)
			{
				TS_OutPutMsg(pgbl->dPosInfo,"网络繁忙，请稍后再试!",1,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(pgbl->dPosInfo,rsp_message,1,iSelect);
				
			}
			
		}
		else if (strlen(rsp_code)==5)
		{
			iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
			if (iRet!=0)
			{
				TS_OutPutMsg(pgbl->dPosInfo,"网络繁忙，请稍后再试!",1,iSelect);
				
			}
			else
			{
				//对于完全成功，就是成功
				if (strcmp(rsp_code,"20002") ==0 ) //对于开卡过程中出现报文完全成功的情况，就是成功，不需要再发送
				{
					return 0;

				}
				else
				{
					TS_OutPutMsg(pgbl->dPosInfo,rsp_message,1,iSelect);
					return -1;
				}
				
			
			}
			
		}
		
		else
		{
			TS_OutPutMsg(pgbl->dPosInfo,"网络繁忙，请稍后再试!",0,iSelect);
			
		}

		return -1; //发送旧卡信息失败
		
	}
	
	return 0;
	
}

//东莞通开卡

int CDGTBusTrade::dgtCardOperate(CGlobal *pgbl,char *tranMessage,char *transType,char *rsp_code,char *rsp_message)
{
	
	int APDU_LEN=0;
	int iRet;
	char openDate[8+1];
	char effectiveDate[8+1];
	char errMsg[200];
	char APDU[70];
	bbzero(APDU);
	
	bbzero(errMsg);
	bbzero(rsp_message);
	bbzero(effectiveDate);
	bbzero(rsp_code);
	
	strcpy(effectiveDate,"20901231");
	bbzero(pgbl->dgtCard.effectiveDate);
	CTools::hex_to_byte((BYTE *) effectiveDate,(BYTE *) pgbl->dgtCard.effectiveDate,8);
	//设置开卡日期
	CTools::getDate(openDate);

	if (strcmp(transType,"80") ==0  || (strcmp(transType,"52")==0)) //如果不是充值 卡片交易金额为0
	{
		//如果是开卡
		CTools::hex_to_byte((BYTE *) openDate,(BYTE *) pgbl->dgtCard.openDate,8);

		//调用服务进行处理 之前组装APDU命令
		bbzero(pgbl->dgtCard.APDU);
		bbzero(APDU);
		//金额
		bbzero(pgbl->dgtCard.tradeBeForeMny);
		bbzero(pgbl->dgtCard.tradeMny);
		memcpy(pgbl->dgtCard.tradeBeForeMny,pgbl->dgtCard.cardMny+4,8);
		strcpy(pgbl->dgtCard.tradeMny,"00000000");


		memcpy(APDU+APDU_LEN,"\x04\xd6\x95\x08\x18",5);
		APDU_LEN+=5;
		memcpy(APDU+APDU_LEN,"\x01",1); //开通卡片
		APDU_LEN+=1;
		memcpy(APDU+APDU_LEN,pgbl->dgtCard.appVer,1);
		APDU_LEN+=1;
		memcpy(APDU+APDU_LEN,pgbl->dgtCard.connectCity,2);
		APDU_LEN+=2;
		memcpy(APDU+APDU_LEN,pgbl->dgtCard.appSerial,8); //卡序列号
		APDU_LEN+=8;
		CTools::hex_to_byte((BYTE *)openDate,(BYTE *)APDU+APDU_LEN,8); //开卡时间
		APDU_LEN+=4;
		CTools::hex_to_byte((BYTE *)effectiveDate, (BYTE *)APDU+APDU_LEN,8);
		APDU_LEN+=4;
		
		CTools::byte_to_hex((BYTE *) APDU,(BYTE *)pgbl->dgtCard.APDU,APDU_LEN);
		wrLog("APDU_COMMAND:%s",pgbl->dgtCard.APDU);
		
		//取卡片随机数
		bbzero(pgbl->dgtCard.CARDRAND);
		iRet=getRandom(pgbl->dPosInfo,errMsg,pgbl->dgtCard.CARDRAND); //取随机数
		wrLog("售卡取卡片随机数为:%s",pgbl->dgtCard.CARDRAND);
		
		if (iRet!=0)
		{
			//TS_OutPutMsg(pgbl->dPosInfo,"开卡失败",1,iSelect);
			bbzero(rsp_code);
			bbzero(rsp_message);
			strcpy(rsp_code,"00002");
			strcpy(rsp_message,"读卡器取随机数失败");
			return -1; //取随机数失败
		}

	}

	//开始组装卡操作请求报文。
	iRet=dgt_trade(pgbl,tranMessage,transType,rsp_code);


	if (iRet!=0)
	{
		if (iRet==-2)
		{	
			bbzero(rsp_message);
			strcpy(rsp_message,"网络繁忙，请稍后再试!");
			return -2;

		}
		//根据rsp_code 查询错误名称返回
		if (strlen(rsp_code)==2)
		{

			iRet=getCommuErr(rsp_code,rsp_message);

			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"网络繁忙，请稍后再试!");
				
			}
		}
		else if (strlen(rsp_code)==5)
		{
			iRet=CDB::getDGTcodeMessge(rsp_code,rsp_message);
			if (iRet!=0)
			{
				bbzero(rsp_message);
				strcpy(rsp_message,"网络繁忙，请稍后再试!");

			}

		}

		else

		{
			bbzero(rsp_message);
			strcpy(rsp_message,"网络繁忙，请稍后再试!");

		}

		return -1;
		
	}

	
	return 0;
	
}

int CDGTBusTrade::getCommuErr(char *rsp_code,char *rsp_message)
{
	bbzero(rsp_message);

	switch (atoi(rsp_code))
	{
		case 0x01:
			strcpy(rsp_message,"无效的加密压缩算法");
			break;
		case 0x02:
			strcpy(rsp_message,"无效的消息类型码");
			break;
		case 0x03:
			strcpy(rsp_message,"无效的消息版本");
			break;
		case 0x04:
			strcpy(rsp_message,"报文长度无效");
			break;
		case 0x05:
			strcpy(rsp_message,"通讯MAC错");
			break;
		case 0x06:
			strcpy(rsp_message,"通讯超时");
			break;
		case 0x07:
			strcpy(rsp_message,"通讯目标节点不可到达");
			break;
		case 0x08:
			strcpy(rsp_message,"报文格式错误");
			break;
				
		case 0x09:
			strcpy(rsp_message,"操作员不存在");
			break;
		case 0x0A:
			strcpy(rsp_message,"操作员口令错误");
			break;
		case 0x0B:
			strcpy(rsp_message,"POS机号不存在");
			break;
		case 0x0C:
			strcpy(rsp_message,"POS机已禁用");
			break;
		case 0x0D:
			strcpy(rsp_message,"SAM卡号不存在");
			break;
		case 0x0E:
			strcpy(rsp_message,"SAM卡号已禁用");
			break;
		case 0x12:
			strcpy(rsp_message,"操作员无效操作SAM卡");
			break;
		case 0x13:
			strcpy(rsp_message,"SAM卡与ED对应关系错");
			break;
		case 0x14:
			strcpy(rsp_message,"SAM卡尚未签退");
			break;
		case 0x15:
			strcpy(rsp_message,"SAM卡尚未签到");
			break;
		case 0x16:
			strcpy(rsp_message,"上一日充值交易未上送完毕");
			break;
		case 0x17:
			strcpy(rsp_message,"签到应答写入机具失败");
			break;
		case 0x18:
			strcpy(rsp_message,"签退应答写入机具失败");
			break;
		case 0x19:
			strcpy(rsp_message,"文件名已存在");
			break;
		case 0x1A:
			strcpy(rsp_message,"文件打开/读写失败");
			break;
		case 0x1B:
			strcpy(rsp_message,"文件写入失败");
			break;
		case 0x1C:
			strcpy(rsp_message,"文件不完整");
			break;
		case 0x1D:
			strcpy(rsp_message,"无效的参数代码");
			 break;
		case 0x1E:
			strcpy(rsp_message,"参数未配置");
			break;
		case 0x1F:
			strcpy(rsp_message,"用户卡号不存在");
			break;
		case 0x20:
			strcpy(rsp_message,"用户卡号已禁用");
			break;
		case 0x21:
			strcpy(rsp_message,"用户卡认证信息非法");
			break;
		case 0x22:
			strcpy(rsp_message,"不允许退卡");
			break;
		case 0x23:
			strcpy(rsp_message,"其它未定义的错误");
			break;
		case 0x24:
			strcpy(rsp_message,"参数未下载");
			break;
		case 0x25:
			strcpy(rsp_message,"无效的交易类型码");
			break;
		case 0x26:
			strcpy(rsp_message,"参数版本过旧");
			break;
		case 0x27:
			strcpy(rsp_message,"参数版本已最新");
			break;
		case 0x50:
			strcpy(rsp_message,"系统错误");
			break;
		default:
			return -1;
	}

	return 0;

}


int CDGTBusTrade::cardTrans_OpenCard(CGlobal *pgbl,char *rsp_code,char *rsp_message)
{

	int iRet;
	int iRet1;
	int iSelect;
	int cardStatus_iRet;
	int openCardStatus=0;
	int readCount=0;

	char errMsg[200];
	bbzero(errMsg);
	int read_iRet;

	//这个地方进行修改 可以重复进行三次 对于出现卡状态错误的错误码进行处理，允许用户可以重试三次
	for (int i=0;i<3;i++)
	{

		//重新读卡
		if (readCount >0) //第一次不需要读卡 因为在函数外边已经度过
		{

			memset(&pgbl->dgtCard,0,sizeof(pgbl->dgtCard));
			read_iRet=dgtCardInqHdl(pgbl->dPosInfo,&pgbl->dgtCard,2); //查询卡片物理信息
			if (read_iRet!=0) 
			{
				CDB::updateTranChargeProcess(pgbl,"C01");
				wrLog("读取东莞通卡片查询出错"); //进行异常处理
				TS_OutPutMsg(pgbl->dPosInfo,"读取东莞通卡片出错,换卡失败 请进入业务重试\n如有疑问,请咨询在线客服或致电客服电话400-80-11888",1,iSelect);
				return CANCLE_KEY;
				
			}
		}
		
		iRet=dgtCardOperate(pgbl,"2061","52",rsp_code,rsp_message); //卡操作请求 

		if (iRet!=0)
		{
			//判断开卡的状态是否是卡状态错误
			if (strcmp(rsp_code,"21014") ==0 )
			{
				TS_OutPutMsg(pgbl->dPosInfo,"请换张东莞通白卡 放在读卡器上重试",1,iSelect);
				if (iSelect!=0)
				{
					return CANCLE_KEY;
				}
				else
				{
					readCount++;
					continue;

				}

			}
			else
			{
			
				break;
			}
		}
		else
		{
		
			break;
		}
	}

	if (iRet!=0) // 
	{

		//换卡失败
		CDB::updateTranChargeProcess(pgbl,"C01");
		
		if (iRet== -2) //如果因为网络原因引起换卡失败
		{

			bbzero(errMsg);
			sprintf(errMsg,"\n开卡失败，%s",rsp_message);

		}
		else
		{
			//换卡失败
			bbzero(errMsg);
			sprintf(errMsg,"\n开卡失败，%s \n请换张东莞通白卡 请进入业务重试",rsp_message);

		}

		TS_OutPutMsg(pgbl->dPosInfo,errMsg,1,iSelect); 
		return -1;
		
	}
	
	//输出卡片dicmac 信息
	//进行开卡 开卡随机数只能用一次
	//开卡的卡操作请求成功之后，发送企业账户生成订单

	iRet=CDGTBusTrade::openCard(pgbl->dPosInfo,pgbl->dgtCard.DICMAC,errMsg,pgbl->dgtCard); //开卡失败的情况要

	if (iRet!=0) //开卡不成功，进行冲正
	{
	
		//三次读取卡片的状态信息
		for (int i=0;i<3;i++)
			
		{
			//读取卡片基础信息。
			cardStatus_iRet=dgtCardInqHdl(pgbl->dPosInfo,&pgbl->dgtCard,1); //查询卡片物理信息
			if (cardStatus_iRet==0)
			{
				//读取卡片信息
				if (memcmp(pgbl->dgtCard.appFlag,"\x01",1)!=0 ) //卡片激活不成功

				{
						
					CDB::updateTranChargeProcess(pgbl,"C01");
					TS_OutPutMsg(pgbl->dPosInfo,"开卡失败，请换张东莞通白卡 请进入业务重试!",1,iSelect);
					return -2;
				}

					
					break;

			}

		}

		 //如果开卡过程中，并且在读取卡片的过程中也出现超时。
		if (cardStatus_iRet !=0) 
		
		{
				
			CDB::updateTranChargeProcess(pgbl,"C0T");
			TS_OutPutMsg(pgbl->dPosInfo,"开卡超时 请进入以旧换新业务重试!",1,iSelect);
			return -5;
		}
	
	}

	//如果开卡成功
	wrLog("开卡成功");
	bbzero(pgbl->dgtCard.tradeBeForeMny);
	memcpy(pgbl->dgtCard.tradeBeForeMny,pgbl->dgtCard.cardMny+4,8);
	//如果提交不成功，三次提交。三次提交都不成功。保留开卡
	CDB::updateTranChargeProcess(pgbl,"C00");

	iRet1=cardSaleTrade(pgbl,"2063","52");  // 52   提交开卡信息 对错误码完全重复进行判断。完全重复则不需要再发送确认信息

	if (iRet1!=0)
	{
		CDB::updateTranChargeProcess(pgbl,"C0S"); //开卡确认
		TS_OutPutMsg(pgbl->dPosInfo,"开卡成功，确认失败，请进入以旧换新业务进行交易确认\n 如有疑问，请咨询在线客服或者致电客服电话400-80-11888",1,iSelect);
		return -3;

	}
	
	CDB::updateTranChargeProcess(pgbl,"C00");
	TS_OutPutMsg(pgbl->dPosInfo,"开卡成功,请按【Enter】继续交易",1,iSelect);

	return 0;

}


//读取卡片余额 	//查询类型，flag 1 查询余额等基础信息 0 只查询余额用于界面显示 。3交易过程中查询余额 更新余额
//4只在开卡的时候更新
int CDGTBusTrade::dgtCardInqHdl(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,int flag) //读取卡片余额等信息

{
	int iRet;
	int iSelect;
	char errMsg[255];
	char reader_rsp[4+1];
	bbzero(reader_rsp); //公交读卡器返回的错误吗
	bbzero(errMsg);
		//初始化东莞通卡
		
	if (flag!=3) //交易过程中更新卡片余额，不初始化
	
	{
	     init_dgtCard_data(pdgtCard); //初始化卡片信息
			
	}
		
		//三次读卡
		for(int i=0;i<3;i++)
		{
			
			iRet=cardQuery(dPosInfo,reader_rsp,errMsg,pdgtCard,flag);
			if (iRet!=0) //如果读取错误
			{
				
				bbzero(errMsg);
				strcpy(errMsg,"查询读卡器失败，请将卡放在读卡器上重试\n 请按【Enter】继续");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else //读卡成功直接退出寻卡
			{
				
				break;
			}
			
		}
		
		if (iRet!=0) //三次读卡均失败
		{
			return -1;
		}
		
		
		//显示卡片余额；
		char showMsg[200];
		char showLEDMsg[500];
		char cardMny[12+1];
		
		bbzero(cardMny);
		bbzero(showMsg);
		bbzero(showLEDMsg);
		
		strcpy(cardMny,pdgtCard->cardMny);
		sprintf(showLEDMsg,"余额:%.2f元",atof(cardMny)/100);
		wrLog("需要显示的信息:%s",showLEDMsg);
		
		if (flag==0) //如果只是查询 需要在界面上面显示余额
		{
			
			bbzero(errMsg);
			iRet=CDGTBusTrade::showLEDMsg(dPosInfo,showLEDMsg,errMsg);
			
			if (iRet!=0)
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;
			}
			
			sprintf(showMsg,"卡片余额:%.2f元 请按【Esc】返回",atof(cardMny)/100);
			TS_OutPutMsg(dPosInfo,showMsg,1,iSelect);
		}
		
		else
		{
			
			return 0;
			
		}
		
		return 0;
		
}

//转移卡片余额

int CDGTBusTrade::trans_card_Amt(CGlobal *pgbl,char *rsp_code,char *rsp_message)
{

	char errMsg[200];
	int iRet;
	char cardMNY[8+1];
	bbzero(cardMNY);
	bbzero(errMsg);
	int iRet2;
	int write_iRet;

	//进行cpu初始化
	//校验pin2
	char appSerial[16+1];
	bbzero(appSerial);
	CTools::byte_to_hex((BYTE *) pgbl->dgtCard.appSerial,(BYTE *)appSerial,8);
	iRet=CDGTBusTrade::checkPIN2(pgbl->dPosInfo,appSerial,errMsg);
	if (iRet!=0)
	{
		//读卡器错误
		bbzero(rsp_code);
		strcpy(rsp_code,"00001");
		bbzero(rsp_message);
		strcpy(rsp_message,errMsg);

		return -1;
	}

	iRet=initCPUCard(pgbl->dPosInfo,pgbl->dgtSignInfo.POSID,pgbl->dgtCard.tradeMny,pgbl->dgtCard.CARDSEQ,pgbl->dgtCard.KEYVER,pgbl->dgtCard.ALGIND,pgbl->dgtCard.CARDRAND,cardMNY,pgbl->dgtCard.MAC1,errMsg);

	if (iRet!=0)
	{
		//读卡器错误
		bbzero(rsp_code);
		strcpy(rsp_code,"00002"); //读卡器类错误
		bbzero(rsp_message);
		strcpy(rsp_message,errMsg);
		return -1;
	}

	bbzero(pgbl->dgtCard.cardMny);
	CTools::HexToDec(cardMNY,pgbl->dgtCard.cardMny);

	//发送换卡完成卡操作请求 加强对这部分的判断，防止出现重复加值的情况。

	iRet=dgtCardOperate(pgbl,"2061","53",rsp_code,rsp_message); //发送换卡完成操作请求报文 transtype=53,cardtype=3460
	if (iRet!=0)
	{
		//换卡失败

		return -2;
	
	}

	//进行圈存只能够一次，随机数只能使用一次

	write_iRet=cpuCard_load(pgbl->dPosInfo,pgbl->dgtCard.MESSAGEDATETIME,pgbl->dgtCard.MAC2,pgbl->dgtCard.TAC,errMsg); 
	//读卡。如果卡内余额变化。并且圈存返回成功。则交易成功。//对返回超时的情况进行特别处理

	//再次查询余额 需要三次读取卡片余额信息

	iRet2 = dgtCPUInqHdl(pgbl->dPosInfo,&pgbl->dgtCard,3); //卡片充值过程中更新余额查询操作
	
	/************************************************************************/
	/* 充值成功后读取卡片信息，并更新余额信息                               */
	/************************************************************************/
	if (iRet2==0)
	{
		bbzero(pgbl->dgtCard.tradeAfterMny);
		memcpy(pgbl->dgtCard.tradeAfterMny,pgbl->dgtCard.cardMny+4,8);
		
		wrLog("比对充值前后金额");
		wrLog("充值前余额:%d",atoi(pgbl->dgtCard.tradeBeForeMny));
		wrLog("充值后余额:%d",atoi(pgbl->dgtCard.tradeAfterMny));
		wrLog("交易金额:%d",atoi(pgbl->dgtCard.tradeMny)); 
	}

	//特殊情况处理。
	//写卡超时的情况并且三次读卡均失败的情况
	//写卡成功，三次读卡均失败的情况。

	//查询余额
	if (write_iRet==0 || ( iRet2==0 && (atoi(pgbl->dgtCard.tradeAfterMny)==atoi(pgbl->dgtCard.tradeMny)+atoi(pgbl->dgtCard.tradeBeForeMny)) ))//成功
	{
		//更新数据 如果返回成功或者读卡器成功
		
		iRet=CDGTBusTrade::wrTxnLogs(pgbl->dPosInfo,pgbl->dgtSignInfo.POSID,pgbl->dgtCard,"02",errMsg);
		if (iRet!=0)
		{
			wrLog("写卡记录失败");
		}
		
		//读卡器 显示信息
		char ledMsg[100];
		char errMsg[200];
		bbzero(errMsg);
		bbzero(ledMsg);
		if (write_iRet==0 && iRet2!=0) //对于特殊情况读卡器写卡成功但是读卡失败的情况认为是成功处理
		{
			int showMny; //更新卡片余额和更新交易后余额
			showMny=atoi(pgbl->dgtCard.tradeBeForeMny)+atoi(pgbl->dgtCard.tradeMny);
			bbzero(pgbl->dgtCard.tradeAfterMny);
			sprintf(pgbl->dgtCard.tradeAfterMny,"%08d",showMny);
			bbzero(pgbl->dgtCard.cardMny);
			sprintf(pgbl->dgtCard.cardMny,"%012d",showMny);
			sprintf(ledMsg,"余额:%.2f元",atof(pgbl->dgtCard.cardMny)/100);
		}
		else
		{
			sprintf(ledMsg,"余额:%.2f元",atof(pgbl->dgtCard.cardMny)/100);
		
		}

		showLEDMsg(pgbl->dPosInfo,ledMsg,errMsg);
		buzzer(pgbl->dPosInfo,errMsg);

		return 0;
	}
	else  //圈存返回失败。并且读卡也失败
	{
		//如果充值超时，并且读卡失败的情况进行处理，提示充值结果未知。请联系在线客服。超时数据库中当失败处理
		if (write_iRet==-1 && iRet2 !=0)
		{
		    //超时当成功
			int showMny; //更新卡片余额和更新交易后余额
			showMny=atoi(pgbl->dgtCard.tradeBeForeMny)+atoi(pgbl->dgtCard.tradeMny);
			bbzero(pgbl->dgtCard.tradeAfterMny);
			sprintf(pgbl->dgtCard.tradeAfterMny,"%08d",showMny);
			bbzero(pgbl->dgtCard.cardMny);
			sprintf(pgbl->dgtCard.cardMny,"%012d",showMny);

			return 1;
		}
		else
		{
		
			return -4; //交易失败
		}

		
	}
	
	return 0;

}


//CPU读卡
//查询类型，flag 1 查询余额等基础信息 0 只查询余额 。3交易过程中查询余额

int CDGTBusTrade::dgtCPUInqHdl(TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,int flag) //读取卡片余额等信息
{

	int iRet;
	int iSelect;
	char errMsg[255];
	char reader_rsp[4+1];
	bbzero(reader_rsp); //公交读卡器返回的错误吗
	bbzero(errMsg);
	//初始化东莞通卡
	
	if (flag!=3) //交易过程中更新卡片余额，不初始化
	{
		init_dgtCard_data(pdgtCard); //初始化卡片信息
		
	}
	
	
	//三次读卡
	for(int i=0;i<3;i++)
	{
		
		iRet=cardQuery(dPosInfo,reader_rsp,errMsg,pdgtCard,flag);
		if (iRet!=0) //如果读取错误
		{
			if (strcmp(reader_rsp,"3005")==0 || strcmp(reader_rsp,"3006")==0 ) //如果出现写卡超时的情况
			{
				
				bbzero(errMsg);
				strcpy(errMsg,"查询读卡器失败，请将卡放在读卡器上重试\n 请按【Enter】继续");
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				
			}
			else
			{
				TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
				return -1;
				
			}
			
			
		}
		else //读卡成功直接退出寻卡
		{
			
			break;
		}
		
	}
	
	//显示卡片余额；
	char showMsg[200];
	char showLEDMsg[500];
	char cardMny[12+1];
	
	bbzero(cardMny);
	bbzero(showMsg);
	bbzero(showLEDMsg);
	
	strcpy(cardMny,pdgtCard->cardMny);
	sprintf(showLEDMsg,"余额:%.2f元",atof(cardMny)/100);
	wrLog("需要显示的信息:%s",showLEDMsg);
	
	if (flag==0) //如果只是查询
	{
		
		bbzero(errMsg);
		iRet=CDGTBusTrade::showLEDMsg(dPosInfo,showLEDMsg,errMsg);
		
		if (iRet!=0)
		{
			TS_OutPutMsg(dPosInfo,errMsg,0,iSelect);
			return -1;
		}
		
		sprintf(showMsg,"卡片余额:%.2f元 请按【Esc】返回",atof(cardMny)/100);
		TS_OutPutMsg(dPosInfo,showMsg,1,iSelect);
	}
	
	else
	{
		
		return 0;
		
	}
	
	return 0;
	
}


