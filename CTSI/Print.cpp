// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TissonCTSI.h"
#include "Print.h"

#include "TSCTSI.h"
#include "pack.h"
#include "packet.h"
#include "DB.h"
#include "Def.h"
#include <winsock2.h>
#include "Tools.h"
#include "Global.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrint::CPrint()
{
	m_prtType = 0;	//打印类型：0,无打印; 1, 热敏套打; 2, 热敏非套打; 3, 针式套打; 4, 针式非套打
	m_prtCom  = 0;	//打印串口：0,无打印串口; 1, 左串口; 2, 右串口; 3, 内置串口;
}

CPrint::~CPrint()
{

}

void CPrint::set_in_par(CGlobal in_gbl, CPack in_pack) //设置传入参数
{
	gbl  = in_gbl;
	pack = in_pack;
	getPrintPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &m_prtCom, &m_prtType);
	wrLog("m_prtCom: %d", m_prtCom);
	wrLog("m_prtType: %d", m_prtType);
}

void CPrint::set_in_par(CGlobal in_gbl) //设置传入参数
{
	gbl  = in_gbl;
	getPrintPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &m_prtCom, &m_prtType);
	wrLog("m_prtCom: %d", m_prtCom);
	wrLog("m_prtType: %d", m_prtType);
}


//获取打印参数
//prtCom: 打印串口
//prtType:打印类型
int CPrint::getPrintPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int *prtCom, int *prtType)
{
	int flag = 0; //初始化
	//内置串口
	if(inType>0 && inType<=4 || inType>=8 && inType<=14)
	{
		*prtType = inType;
		*prtCom  = 0x03;
		flag = 1;
	}
	//左串口
	else if(leftType>0 && leftType<=4 || leftType>=8 && leftType<=14)
	{
		*prtType = leftType;		
		*prtCom  = 0x01;
		flag = 1;
	}
	//右串口
	else if(rightType>0 && rightType<=4 || rightType>=8 && rightType<=14)
	{
		*prtType = rightType;		
		*prtCom  = 0x02;
		flag = 1;
	}
	if(flag)
		return 0;
	else
		return -1;
}

//flag: 0-黑标定位(默认)
//		1-打印测试
void CPrint::print_heibiao(int flag)
{
	char msg[512];
	char cDate[8+1], cTime[8+1];
	int iPrintLen, iRet;

	bbzero(cDate);
	bbzero(cTime);
	get_time8(cTime);
	get_date8(cDate);
	memset(msg,0,sizeof(msg));

	if(!flag)
		sprintf(msg,"\n\n\n\n\n黑标定位\n*******************\n%s   %s\n终端号：%s\n\n\n\n\n", cDate, cTime, gbl.pos_no);
	else
	{
		strcpy(msg, "\n\n\n【打印测试】\n");
		strcat(msg, "----------------------------\n");
		sprintf(msg+strlen(msg), "终端号：%s\n", gbl.pos_no);
		sprintf(msg+strlen(msg), "日期：%s\n", cDate);
		sprintf(msg+strlen(msg), "时间：%s\n", cTime);
		strcat(msg, "\n\n\n\n\n");
		wrLog("msgLen:[%d]", strlen(msg));
	}
    
	iPrintLen = strlen(msg);
	iRet = print_func(msg,iPrintLen);
	wrLog("【打印】iRet=%d", iRet);
}


//打印函数
//返回值：SUCC 成功；FAIL 失败
int CPrint::print_func(char *pMsg,int msg_len, int page, int cpyReprint,int acount,int current_count)
{
	int iRet;
	char temp[100];
	bbzero(temp);
// 	if (page == 1)
// 	{
// 		sprintf(temp,"您有%d张发票正在打印\n     请耐心等待",acount-current_count);
// 	}else
		sprintf(temp,"   正在打印中....");
	
	iRet = TS_B4Print(gbl.dPosInfo, m_prtCom, m_prtType, page, temp, (BYTE *)pMsg, msg_len);
	if(iRet)	//监控日志
		wrMonitorLog(11, 1, iRet, gbl, pack);
	else
		wrMonitorLog(11, 0, 0, gbl, pack);
	if( cpyReprint==1 )
	{

			//bbzero(gbl.stan_no);
			//memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
			CDB::setRePrint(gbl.stan_no,gbl.dPosInfo.pos_no,gbl.card_no,0,pMsg,gbl.tgroup); 
		
	}
	wrLog("【打印】返回值：%d", iRet);
	return iRet;
}
 
//安全打印函数
//返回值：SUCC 成功；FAIL 失败
int CPrint::print_func_safe(char *pMsg,int msg_len, int cpyReprint,int acount,int current_count)
{
	int iRet;
	TSB4Print_S printMsg;
	char temp[512];
	bbzero(temp);
//	sprintf(temp,"您有%d张发票正在打印\n     请耐心等待",acount-current_count);
	memset(&printMsg, 0, sizeof(printMsg));
	sprintf(temp,"   正在打印中....");
	printMsg.uCardType = CARD_TYPE;	//大卡
	if(gbl.posInfo.enyFlag)
	{
		printMsg.uKeyIndex = TS_TEK_KEY;		//密钥TEK
		printMsg.uDesMode = TS_DES_3_TEK_DEC;		//加密算法
	}
	else
	{
		printMsg.uKeyIndex = 12;		//密钥组号12
		printMsg.uDesMode = TS_DES_2_DOWN;			//加密算法
	}
	memcpy(printMsg.uPrintData, pMsg, msg_len);
	printMsg.iDataLen = msg_len;

	iRet = TS_B4Print_S(gbl.dPosInfo, m_prtCom, m_prtType, 1, temp, &printMsg);
	if(iRet)	//监控日志
		wrMonitorLog(11, 1, iRet, gbl, pack);
	else
		wrMonitorLog(11, 0, 0, gbl, pack);
	if( cpyReprint==1 ){
		//bbzero(gbl.stan_no);
		//memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
		CDB::setRePrint(gbl.stan_no,gbl.dPosInfo.pos_no,gbl.card_no,0,pMsg,gbl.tgroup);
	}

	return iRet;
}


//改造函数
//从63域中读明细信息，放入gbl.detail结构体中供打印用
//本函数不处理非联通预付费
//flag：默认为1，外部调用；0，内部使用
int CPrint::rdField63(CPack pack,CGlobal &out_gbl, int flag)  
{
	int len , i , j,iRet;
	int count=0;
	char temp[20];

	if(flag)
		gbl = out_gbl;

//	wrLog("【函数】rdField63");
//	wrLog("BIT63:[%s]",pack.FIELD63);
//	strcpy((char *)pack.FIELD63,"A21100523577          邓小扬                        20080807299月固定费  00000000600099本月通信费00000000882099其它费    0000000-30081100523577          邓小扬                        20080907299月固定费  00000000600099本月通信费00000000530099其它费    0000000-3008000000001952000000018152");
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//准备写入明细
	/********************************************/
	if(!memcmp(gbl.fee_type,"0049",4) )	//联通
	{
		count+=1;	//跳过区域码'A'
		if(!memcmp(gbl.uni_flag, "0", 1))	//后付费，交费月份数
		{
			memset( temp,0,sizeof(temp) );
			memcpy(temp,(char *)(pack.FIELD63+count),1);	
			count+=1;
			gbl.detail.nMonCnt = atoi(temp);
			if(gbl.detail.nMonCnt>3 || gbl.detail.nMonCnt<1)
			{
				wrLog("欠费交费月份错误%d",gbl.detail.nMonCnt);
				return -1;
			}
		}
		//20字节的电话号码（付费帐户）
		memset(gbl.pay_no, 0, sizeof(gbl.pay_no));
		memcpy(gbl.pay_no, (char *)(pack.FIELD63+count), 20);	
		count+=20;
		for(i=0; i<20; i++)
		{
			if(gbl.pay_no[i] == 0x20)
				break;
		}
		gbl.pay_no_len = i;
		gbl.pay_no[i] = '\0';
		wrLog("付费帐号:%s",gbl.pay_no);
		//30字节的姓名
		memset(gbl.user_name, 0, sizeof(gbl.user_name));
		memcpy(gbl.user_name, (char *)(pack.FIELD63+count), 30);
		count+=30;
		for(i=0; i<30; i++)
		{
			if(gbl.user_name[i] == 0x20)
				break;
		}
		gbl.user_name_len = i;
		gbl.user_name[i] = '\0';
		wrLog("用户名:%s",gbl.user_name);

		switch(atoi(gbl.uni_flag))
		{
		case 0:
			for(i=0;i<gbl.detail.nMonCnt;i++)	
			{
				strncpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月");
				
				//获取当前交费时段明细长度(3位)
				memset(temp, 0, sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),3);
				count+=3;
				len = atoi(temp);
				//wrLog("联通len :%s",temp);		
				j=0;	//明细计数器
				while(len>0)
				{
					if(len > 512)
						return -1;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),2);	//明细标识码
					count+=2;
					len-=2;
					//	wrLog("gbl.detail code :%s",temp);
					if( !strncmp(temp,"99",2) )		//获得交费条目名，99则后跟具体条目名(10位)
					{
						memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
						count+=10;
						len-=10;
					}
					else
						ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);
					//wrLog("gbl.detail.monDetail[%d].cnt[%d].type:%s",i,j,gbl.detail.monDetail[i].cnt[j].type);
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);
					CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo,0);	
					count+=12;
					len-=12;
					j++;
				}
				gbl.detail.monDetail[i].nCnt = j;
				count+=50;
			} //end for
			break;
			
		case 1:
			break;
		}// end switch
		memset(gbl.pre_fee_buffer,0,sizeof(gbl.pre_fee_buffer)); //预付款余额
		memcpy(gbl.pre_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
		memset(gbl.real_fee_buffer,0,sizeof(gbl.real_fee_buffer));
		memcpy(gbl.real_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
	} //end if
	/********************************************/
	else if(!memcmp(gbl.fee_type, "0065", 4))	//车船税
	{
		memset(gbl.tax_serial, 0, sizeof(gbl.tax_serial));
		memcpy(gbl.tax_serial, pack.FIELD63, 11);
		count =11;
		for(i=0;i<3;i++)
		{
			memset(temp, 0, sizeof(temp));
			memcpy(temp, pack.FIELD63+count, 2);
			count += 2;
			switch(atoi(temp))
			{
			case 1:
				memset(gbl.pay_tax, 0, sizeof(gbl.pay_tax));
				memcpy(gbl.pay_tax, pack.FIELD63+count, 12);
				count += 12;
				break;
			case 2:
				memset(gbl.mul_tax, 0, sizeof(gbl.mul_tax));
				memcpy(gbl.mul_tax, pack.FIELD63+count, 12);
				count += 12;
				break;
			case 3:
				memset(gbl.late_tax, 0, sizeof(gbl.late_tax));
				memcpy(gbl.late_tax, pack.FIELD63+count, 12);
				count += 12;
				break;
			}
		}
		memset(gbl.user_name, 0, sizeof(gbl.user_name));
		memcpy(gbl.user_name, pack.FIELD63+count, 60);
		count += 60;
		i = 0;
		while(gbl.user_name[i] != 0x20)
			i++;
		gbl.user_name[i] = '\0';
		gbl.user_name_len = i;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, pack.FIELD63+count, 1);
		count += 1;
		gbl.detail.nMonCnt = atoi(temp);	//年份数
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			memcpy(gbl.detail.monDetail[i].tmFrom, pack.FIELD63+count, 4);	//年份
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			wrLog("缴税年份：[%s]",gbl.detail.monDetail[i].tmFrom);
			count += 4;
			memset(temp, 0, sizeof(temp));
			memcpy(temp,pack.FIELD63+count,3);
			count += 3;
			len = atoi(temp);
			wrLog("len：[%d]",len);
			j=0;	//明细计数器
			wrLog("明细：%s",pack.FIELD63+count);
			while(len>0)
			{
				if(len > 512)
					return -1;
				count+=2;//跳过2位明细标识码"99"
				len-=2;
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
		
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);	//转换为浮点数字符串,后无“元”
				count+=12;
				len-=12;
				j++;
//wrLog("条目%d：[%s]",j, gbl.detail.monDetail[i].cnt[j].type);
//wrLog("金额：[%s]",gbl.amount);
//wrLog("len：[%d]",len);
			}
			gbl.detail.monDetail[i].nCnt = j;
		}
	   memset(gbl.amount,0,sizeof(gbl.amount));
	   CTools::byte_to_hex(pack.FIELD4,(BYTE *)gbl.amount,6);
	
	}
	/********************************************/
    else if(!memcmp(gbl.fee_type, "0131", 4)){//东莞电信后付费

		if(atoi(gbl.fee_type2)==149)
		{
			count=gbl.num;
		}
		bbzero(temp);	//3位欠费信息长度
		memcpy(temp, (char *)(pack.FIELD63+count), 3);
		if(atoi(temp))	//有欠费明细
			count+=3;
		else
			return 1;
		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),2);	//交费月数
		    count+=2;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("交费月份数：%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"日");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"年");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"月");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"日");
                
				memset(temp, 0, sizeof(temp));
                memcpy(temp, (char *)(pack.FIELD63+count), 2);
                count+=2;
				memset(gbl.user_name, 0, sizeof(gbl.user_name));
		        memcpy(gbl.user_name, (char *)(pack.FIELD63+count), atoi(temp));
				count+=atoi(temp);
                
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);
				CTools::chgToMny(temp, gbl.real_fee_buffer, 0);
				count+=12;
                
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);
				CTools::chgToMny(temp, gbl.amount, 0);
				count+=12;
                
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),2);
                gbl.detail.monDetail[i].nCnt=atoi(temp);
				count+=2;

				for(int j=0;j<gbl.detail.monDetail[i].nCnt;j++)
				{
                    memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),2);
					count+=2;
					memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),atoi(temp));
					count+=atoi(temp);

					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);
				    CTools::chgToMny(temp, gbl.detail.monDetail[i].cnt[j].amo, 0);
                    count+=12;

				}



		}

	}
	/********************************************/
	else	//非联通
	{   
		/*-----有线宽频优惠-----*/
		if(atoi(gbl.fee_type) == 172)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
				return 1;
		}
		/*---------------------*/
		if(atoi(gbl.fee_type) == 100)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
			{
			//	iRet =  print_pre_detail(pack,"数字电视缴费"); //cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 112)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
			{
			//	iRet = print_pre_detail(pack,"珠江宽频缴费"); cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 114)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)pack.FIELD63, 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
			{
				//iRet = print_pre_detail("模拟电视(无机顶盒)"); //cut by liuxiao
			//	return iRet;
			}
		}

		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),1);	//交费月数
		count+=1;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("交费月份数：%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			//读入交费月
			switch(atoi(gbl.fee_type))
			{
			case 1:		//固话
			case 13:	//ADSL
			case 137:	//ITV
			case 241:   //CDMA后付费
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"日");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"年");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"月");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"日");
				break;
				
			case 3012:	//移动
			case 100:	//珠江数码月租费
			case 112:	//珠江宽频月租费
			case 114:	//有线电视月租费
			case 172:	//珠江宽频优惠
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),6);
				count+=6;
		/*		strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月"); */
				break;
			default:
				return -1;
			}
//	wrLog("交费月：[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
			//获取当前交费时段明细长度（3位）
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),3);
			count+=3;
			len = atoi(temp);
			j=0;	//明细计数器
			while(len>0)
			{
				if(len > 512)
					return -1;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),2);	//明细标识码
				count+=2;
				len-=2;
				if( !strncmp(temp,"99",2) )		//获得交费条目名
				{
					memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
					count+=10;
					len-=10;
				}
				else if( !strncmp(temp,"98",2) )		//电信发票修改20071227
				{
					memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
					count+=10;
					len-=10;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
					CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//转换为浮点数字符串
					count+=12;
					len-=12;
					continue;
				}
				else
					ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);

				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//转换为浮点数字符串
				count+=12;
				len-=12;
				j++;
			}
			gbl.detail.monDetail[i].nCnt = j;
		} //end for

		if(atoi(gbl.fee_type) == 3012)	//移动(lihk,2006-10-25)
		{
			if( gbl.mbl_flag == 2 )	//若欠费下交预付费，增加一个打预付费的明细
			{
				gbl.detail.nMonCnt++;
				gbl.detail.monDetail[i].nCnt = 1;	//一项明细
				memcpy(gbl.detail.monDetail[i].cnt[0].type, "存入金额  ",10);
				CTools::chgToMny(gbl.tmp_pre_amount, gbl.detail.monDetail[i].cnt[0].amo , 0);//转换为浮点数字符串
			}
		}
		if(atoi(gbl.fee_type) == 100)
		{
			bbzero(temp);
			memcpy(temp, (char *)(pack.FIELD63+count), 12);
			if(atoi(temp)==0)
				return 0;
			else
			{
				//iRet =  print_pre_detail(pack,"数字电视缴费"); //change by print_pre_detail("数字电视缴费") cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 112)
		{
			bbzero(temp);
			memcpy(temp, (char *)(pack.FIELD63+count), 12);
			if(atoi(temp)==0)
				return 0;
			else
			{
				//iRet = print_pre_detail(pack,"珠江宽频缴费"); //cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 114)
		{
			bbzero(temp);
			memcpy(temp, (char *)(pack.FIELD63+count), 12);
			if(atoi(temp)==0)
				return 0;
			else
			{
				//iRet = print_pre_detail(pack,"模拟电视(无机顶盒)"); //cut by liuxiao
				//return iRet;
			}
		}
		wrLog("打印数目：%d\n", gbl.detail.nMonCnt);
	}//end else
	if(flag)
		out_gbl = gbl;
	return 0;
}

//end of 改造函数
//从63域中读明细信息，放入gbl.detail结构体中供打印用
//本函数不处理非联通预付费
//flag：默认为1，外部调用；0，内部使用
int CPrint::rdField63(CGlobal &out_gbl, int flag)  
{
	int len , i , j,iRet;
	int count=0;
	char temp[20];

	if(flag)
		gbl = out_gbl;

//	wrLog("【函数】rdField63");
//	wrLog("BIT63:[%s]",pack.FIELD63);
//	strcpy((char *)pack.FIELD63,"A21100523577          邓小扬                        20080807299月固定费  00000000600099本月通信费00000000882099其它费    0000000-30081100523577          邓小扬                        20080907299月固定费  00000000600099本月通信费00000000530099其它费    0000000-3008000000001952000000018152");
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//准备写入明细
	/********************************************/
	if(!memcmp(gbl.fee_type,"0049",4) )	//联通
	{
		count+=1;	//跳过区域码'A'
		if(!memcmp(gbl.uni_flag, "0", 1))	//后付费，交费月份数
		{
			memset( temp,0,sizeof(temp) );
			memcpy(temp,(char *)(pack.FIELD63+count),1);	
			count+=1;
			gbl.detail.nMonCnt = atoi(temp);
			if(gbl.detail.nMonCnt>3 || gbl.detail.nMonCnt<1)
			{
				wrLog("欠费交费月份错误%d",gbl.detail.nMonCnt);
				return -1;
			}
		}
		//20字节的电话号码（付费帐户）
		memset(gbl.pay_no, 0, sizeof(gbl.pay_no));
		memcpy(gbl.pay_no, (char *)(pack.FIELD63+count), 20);	
		count+=20;
		for(i=0; i<20; i++)
		{
			if(gbl.pay_no[i] == 0x20)
				break;
		}
		gbl.pay_no_len = i;
		gbl.pay_no[i] = '\0';
		wrLog("付费帐号:%s",gbl.pay_no);
		//30字节的姓名
		memset(gbl.user_name, 0, sizeof(gbl.user_name));
		memcpy(gbl.user_name, (char *)(pack.FIELD63+count), 30);
		count+=30;
		for(i=0; i<30; i++)
		{
			if(gbl.user_name[i] == 0x20)
				break;
		}
		gbl.user_name_len = i;
		gbl.user_name[i] = '\0';
		wrLog("用户名:%s",gbl.user_name);

		switch(atoi(gbl.uni_flag))
		{
		case 0:
			for(i=0;i<gbl.detail.nMonCnt;i++)	
			{
				strncpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月");
				
				//获取当前交费时段明细长度(3位)
				memset(temp, 0, sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),3);
				count+=3;
				len = atoi(temp);
				//wrLog("联通len :%s",temp);		
				j=0;	//明细计数器
				while(len>0)
				{
					if(len > 512)
						return -1;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),2);	//明细标识码
					count+=2;
					len-=2;
					//	wrLog("gbl.detail code :%s",temp);
					if( !strncmp(temp,"99",2) )		//获得交费条目名，99则后跟具体条目名(10位)
					{
						memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
						count+=10;
						len-=10;
					}
					else
						ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);
					//wrLog("gbl.detail.monDetail[%d].cnt[%d].type:%s",i,j,gbl.detail.monDetail[i].cnt[j].type);
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);
					CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo,0);	
					count+=12;
					len-=12;
					j++;
				}
				gbl.detail.monDetail[i].nCnt = j;
				count+=50;
			} //end for
			break;
			
		case 1:
			break;
		}// end switch
		memset(gbl.pre_fee_buffer,0,sizeof(gbl.pre_fee_buffer)); //预付款余额
		memcpy(gbl.pre_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
		memset(gbl.real_fee_buffer,0,sizeof(gbl.real_fee_buffer));
		memcpy(gbl.real_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
	} //end if
	/********************************************/
	else if(!memcmp(gbl.fee_type, "0065", 4))	//车船税
	{
		memset(gbl.tax_serial, 0, sizeof(gbl.tax_serial));
		memcpy(gbl.tax_serial, pack.FIELD63, 11);
		count =11;
		for(i=0;i<3;i++)
		{
			memset(temp, 0, sizeof(temp));
			memcpy(temp, pack.FIELD63+count, 2);
			count += 2;
			switch(atoi(temp))
			{
			case 1:
				memset(gbl.pay_tax, 0, sizeof(gbl.pay_tax));
				memcpy(gbl.pay_tax, pack.FIELD63+count, 12);
				count += 12;
				break;
			case 2:
				memset(gbl.mul_tax, 0, sizeof(gbl.mul_tax));
				memcpy(gbl.mul_tax, pack.FIELD63+count, 12);
				count += 12;
				break;
			case 3:
				memset(gbl.late_tax, 0, sizeof(gbl.late_tax));
				memcpy(gbl.late_tax, pack.FIELD63+count, 12);
				count += 12;
				break;
			}
		}
		memset(gbl.user_name, 0, sizeof(gbl.user_name));
		memcpy(gbl.user_name, pack.FIELD63+count, 60);
		count += 60;
		i = 0;
		while(gbl.user_name[i] != 0x20)
			i++;
		gbl.user_name[i] = '\0';
		gbl.user_name_len = i;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, pack.FIELD63+count, 1);
		count += 1;
		gbl.detail.nMonCnt = atoi(temp);	//年份数
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			memcpy(gbl.detail.monDetail[i].tmFrom, pack.FIELD63+count, 4);	//年份
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			wrLog("缴税年份：[%s]",gbl.detail.monDetail[i].tmFrom);
			count += 4;
			memset(temp, 0, sizeof(temp));
			memcpy(temp,pack.FIELD63+count,3);
			count += 3;
			len = atoi(temp);
			wrLog("len：[%d]",len);
			j=0;	//明细计数器
			wrLog("明细：%s",pack.FIELD63+count);
			while(len>0)
			{
				if(len > 512)
					return -1;
				count+=2;//跳过2位明细标识码"99"
				len-=2;
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
		
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);	//转换为浮点数字符串,后无“元”
				count+=12;
				len-=12;
				j++;
//wrLog("条目%d：[%s]",j, gbl.detail.monDetail[i].cnt[j].type);
//wrLog("金额：[%s]",gbl.amount);
//wrLog("len：[%d]",len);
			}
			gbl.detail.monDetail[i].nCnt = j;
		}
	   memset(gbl.amount,0,sizeof(gbl.amount));
	   CTools::byte_to_hex(pack.FIELD4,(BYTE *)gbl.amount,6);
	
	}
	/********************************************/
    else if(!memcmp(gbl.fee_type, "0131", 4)){//东莞电信后付费

		if(atoi(gbl.fee_type2)==149)
		{
			count=gbl.num;
		}
		bbzero(temp);	//3位欠费信息长度
		memcpy(temp, (char *)(pack.FIELD63+count), 3);
		if(atoi(temp))	//有欠费明细
			count+=3;
		else
			return 1;
		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),2);	//交费月数
		    count+=2;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("交费月份数：%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"日");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"年");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"月");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"日");
                
				memset(temp, 0, sizeof(temp));
                memcpy(temp, (char *)(pack.FIELD63+count), 2);
                count+=2;
				memset(gbl.user_name, 0, sizeof(gbl.user_name));
		        memcpy(gbl.user_name, (char *)(pack.FIELD63+count), atoi(temp));
				count+=atoi(temp);
                
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);
				CTools::chgToMny(temp, gbl.real_fee_buffer, 0);
				count+=12;
                
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);
				CTools::chgToMny(temp, gbl.amount, 0);
				count+=12;
                
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),2);
                gbl.detail.monDetail[i].nCnt=atoi(temp);
				count+=2;

				for(int j=0;j<gbl.detail.monDetail[i].nCnt;j++)
				{
                    memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),2);
					count+=2;
					memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),atoi(temp));
					count+=atoi(temp);

					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);
				    CTools::chgToMny(temp, gbl.detail.monDetail[i].cnt[j].amo, 0);
                    count+=12;

				}



		}

	}
	/********************************************/
	else	//非联通
	{   
		/*-----有线宽频优惠-----*/
		if(atoi(gbl.fee_type) == 172)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
				return 1;
		}
		/*---------------------*/
		if(atoi(gbl.fee_type) == 100)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
			{
				//iRet =  print_pre_detail(pack,"数字电视缴费"); cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 112)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
			{
				iRet = print_pre_detail(pack,"珠江宽频缴费");
				return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 114)
		{
			bbzero(temp);	//3位欠费信息长度
			memcpy(temp, (char *)pack.FIELD63, 3);
			if(atoi(temp))	//有欠费明细
				count+=3;
			else
			{
				iRet = print_pre_detail("模拟电视(无机顶盒)");
				return iRet;
			}
		}

		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),1);	//交费月数
		count+=1;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("交费月份数：%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			//读入交费月
			switch(atoi(gbl.fee_type))
			{
			case 1:		//固话
			case 13:	//ADSL
			case 137:	//ITV
			case 241:   //CDMA后付费
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"日");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"年");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"月");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"日");
				break;
				
			case 3012:	//移动
			case 100:	//珠江数码月租费
			case 112:	//珠江宽频月租费
			case 114:	//有线电视月租费
			case 172:	//珠江宽频优惠
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),6);
				count+=6;
		/*		strcat(gbl.detail.monDetail[i].tmFrom,"年");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"月"); */
				break;
			default:
				return -1;
			}
//	wrLog("交费月：[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
			//获取当前交费时段明细长度（3位）
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),3);
			count+=3;
			len = atoi(temp);
			j=0;	//明细计数器
			while(len>0)
			{
				if(len > 512)
					return -1;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),2);	//明细标识码
				count+=2;
				len-=2;
				if( !strncmp(temp,"99",2) )		//获得交费条目名
				{
					memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
					count+=10;
					len-=10;
				}
				else if( !strncmp(temp,"98",2) )		//电信发票修改20071227
				{
					memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
					count+=10;
					len-=10;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
					CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//转换为浮点数字符串
					count+=12;
					len-=12;
					continue;
				}
				else
					ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);

				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//转换为浮点数字符串
				count+=12;
				len-=12;
				j++;
			}
			gbl.detail.monDetail[i].nCnt = j;
		} //end for

		if(atoi(gbl.fee_type) == 3012)	//移动(lihk,2006-10-25)
		{
			if( gbl.mbl_flag == 2 )	//若欠费下交预付费，增加一个打预付费的明细
			{
				gbl.detail.nMonCnt++;
				gbl.detail.monDetail[i].nCnt = 1;	//一项明细
				memcpy(gbl.detail.monDetail[i].cnt[0].type, "存入金额  ",10);
				CTools::chgToMny(gbl.tmp_pre_amount, gbl.detail.monDetail[i].cnt[0].amo , 0);//转换为浮点数字符串
			}
		}
		if(atoi(gbl.fee_type) == 100)
		{
			bbzero(temp);
			memcpy(temp, (char *)(pack.FIELD63+count), 12);
			if(atoi(temp)==0)
				return 0;
			else
			{
				iRet =  print_pre_detail(pack,"数字电视缴费");
				return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 112)
		{
			bbzero(temp);
			memcpy(temp, (char *)(pack.FIELD63+count), 12);
			if(atoi(temp)==0)
				return 0;
			else
			{
				iRet = print_pre_detail(pack,"珠江宽频缴费");
				return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 114)
		{
			bbzero(temp);
			memcpy(temp, (char *)(pack.FIELD63+count), 12);
			if(atoi(temp)==0)
				return 0;
			else
			{
				iRet = print_pre_detail(pack ,"模拟电视(无机顶盒)");
				return iRet;
			}
		}
		wrLog("打印数目：%d\n", gbl.detail.nMonCnt);
	}//end else
	if(flag)
		out_gbl = gbl;
	return 0;
}


//补打发票时从63域中读明细信息，放入gbl.detail结构体中供打印用
int CPrint::rdField63_2()
{
	int len , i , j;
	int count=0;
	char temp[20];
	
	wrLog("【函数】rdField63_2");
	wrLog("BIT63:[%s]",pack.FIELD63);
	memset(gbl.fee_type, 0, sizeof(gbl.fee_type));//业务类型
	memcpy(gbl.fee_type, pack.FIELD63, 4);
	count = 4;
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//准备写入明细
	memset( temp,0,sizeof(temp) );
	memcpy(temp,(char *)pack.FIELD63 + count,1);	//交费月数
	count += 1;
	gbl.detail.nMonCnt = atoi(temp);
	wrLog("gbl.detail.nMonCnt:%d",gbl.detail.nMonCnt);
	memset(gbl.user_name, 0, sizeof(gbl.user_name));
	memcpy(gbl.user_name, pack.FIELD63+count, 40); //用户名
	count += 40;
	i = 0;
	while(gbl.user_name[i] != 0x20)
		i++;
	gbl.user_name[i] = '\0';
	gbl.user_name_len = i;

	for(i=0;i<gbl.detail.nMonCnt;i++)//按月份数取值		
	{
		//读入交费月
		switch(atoi(gbl.fee_type))
		{
		case 3002:		//移动
		case 218:       //电信发票补打
		case 49:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"月");
			break;
		case 65:	//车船税
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count += 4;
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			count += 2;	//长格式，后有2个0
			break;
		default:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"月");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"日");
			
			memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmTo,"年");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"月");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"日");
			break;
		}
		wrLog("交费月：[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
		//获取当前交费时段明细长度（3位）
		wrLog("明细：%s",pack.FIELD63+count);
		memset(temp,0,sizeof(temp));
		memcpy(temp,(char *)(pack.FIELD63+count),3);
		count+=3;
		len = atoi(temp);
		j=0;	//明细计数器
		while(len>0)
		{
			if(len > 512)
				return -1;
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),2);	//明细标识码
			count+=2;
			len-=2;
			if( !strncmp(temp,"99",2) )		//获得交费条目名
			{
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
			}
			else if( !strncmp(temp,"98",2) )		//电信发票修改20071227
			{
				memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
				count+=10;
				len-=10;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
				CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//转换为浮点数字符串
				count+=12;
				len-=12;
				continue;
			}
			else
				ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);
			
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
			CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//转换为浮点数字符串
			count += 12;
			len -= 12;
			
			j++;
			/*wrLog("条目%d：%s",j, gbl.detail.monDetail[i].cnt[j].type);
			wrLog("金额：%s",temp);
			wrLog("len：[%d]",len);*/
			if((atoi(gbl.fee_type) == 65)&&(len==11))
			{
				break;
			}
		}//end while(len>0)
		if(atoi(gbl.fee_type) == 65)
		{
			bbzero(gbl.tax_serial);
			memcpy(gbl.tax_serial, (char *)(pack.FIELD63+count), 11);
			count += 11;
			len -= 11;
		}
		gbl.detail.monDetail[i].nCnt = j;

	} //end for
	return 0;
} 

//改造函数

//补打发票时从63域中读明细信息，放入gbl.detail结构体中供打印用
int CPrint::rdField63_2(CPack pack)
{
	int len , i , j;
	int count=0;
	char temp[20];
	
	wrLog("【函数】rdField63_2");
	wrLog("BIT63:[%s]",pack.FIELD63);
	memset(gbl.fee_type, 0, sizeof(gbl.fee_type));//业务类型
	memcpy(gbl.fee_type, pack.FIELD63, 4);
	count = 4;
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//准备写入明细
	memset( temp,0,sizeof(temp) );
	memcpy(temp,(char *)pack.FIELD63 + count,1);	//交费月数
	count += 1;
	gbl.detail.nMonCnt = atoi(temp);
	wrLog("gbl.detail.nMonCnt:%d",gbl.detail.nMonCnt);
	memset(gbl.user_name, 0, sizeof(gbl.user_name));
	memcpy(gbl.user_name, pack.FIELD63+count, 40); //用户名
	count += 40;
	i = 0;
	while(gbl.user_name[i] != 0x20)
		i++;
	gbl.user_name[i] = '\0';
	gbl.user_name_len = i;

	for(i=0;i<gbl.detail.nMonCnt;i++)//按月份数取值		
	{
		//读入交费月
		switch(atoi(gbl.fee_type))
		{
		case 3002:		//移动
		case 218:       //电信发票补打
		case 49:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"月");
			break;
		case 65:	//车船税
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count += 4;
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			count += 2;	//长格式，后有2个0
			break;
		default:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"年");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"月");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"日");
			
			memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmTo,"年");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"月");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"日");
			break;
		}
		wrLog("交费月：[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
		//获取当前交费时段明细长度（3位）
		wrLog("明细：%s",pack.FIELD63+count);
		memset(temp,0,sizeof(temp));
		memcpy(temp,(char *)(pack.FIELD63+count),3);
		count+=3;
		len = atoi(temp);
		j=0;	//明细计数器
		while(len>0)
		{
			if(len > 512)
				return -1;
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),2);	//明细标识码
			count+=2;
			len-=2;
			if( !strncmp(temp,"99",2) )		//获得交费条目名
			{
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
			}
			else if( !strncmp(temp,"98",2) )		//电信发票修改20071227
			{
				memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
				count+=10;
				len-=10;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
				CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//转换为浮点数字符串
				count+=12;
				len-=12;
				continue;
			}
			else
				ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);
			
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),12);	//明细金额（12位）
			CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//转换为浮点数字符串
			count += 12;
			len -= 12;
			
			j++;
			/*wrLog("条目%d：%s",j, gbl.detail.monDetail[i].cnt[j].type);
			wrLog("金额：%s",temp);
			wrLog("len：[%d]",len);*/
			if((atoi(gbl.fee_type) == 65)&&(len==11))
			{
				break;
			}
		}//end while(len>0)
		if(atoi(gbl.fee_type) == 65)
		{
			bbzero(gbl.tax_serial);
			memcpy(gbl.tax_serial, (char *)(pack.FIELD63+count), 11);
			count += 11;
			len -= 11;
		}
		gbl.detail.monDetail[i].nCnt = j;

	} //end for
	return 0;
} 
//end of 改造函数
//读售卡业务返回明细——2006-11-1,lihk
int CPrint::rdField63_3(char *card_num, char *card_pwd, char *card_serial, char *amount, char *eff_yymmdd,char *xm_serial)
{
	int tol_len , len, i ,pwd_len,j,k;
	int count=0;
	char tmp[20],card_pwd_buff[128];
	
	//wrLog("【函数】rdField63_3");
	//wrLog("BIT63:[%s]",pack.FIELD63);
	
	//取得总长度
	tol_len = pack.Packet_Table[63].Real_Length;
	if(tol_len > 512)
		return -1;
	if (atoi(gbl.fee_type)==182)//东莞按stan号售卡补打
	{   
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63, 2);
		count += 2;
		len = atoi(tmp);
        bbzero(gbl.sell_card_type);
		memcpy(gbl.sell_card_type, pack.FIELD63 + 2, len);
		count=count+len;

		switch (atoi(gbl.sell_card_type))
		{  
		   case 807:
			   strcpy(gbl.card_amount,"000000001000");
			   break;
		   case 808:
			   strcpy(gbl.card_amount,"000000001500");
			   break;
		   case 128:
			   strcpy(gbl.card_amount,"000000002000");
			   break;
		   case 207:
		   case 310:
		   case 127:
		   case 124:
		   case 809:
		   case 311:
		   case 315:
			   strcpy(gbl.card_amount,"000000003000");
			   break;
		   case 205:
		   case 308:
		   case 122:
		   case 125:
		   case 129:
		   case 300:
		   case 136:
		   case 200:
			   strcpy(gbl.card_amount,"000000005000");
			   break;
		   case 106:
		   case 309:
		   case 123:
		   case 126:
		   case 130:
		   case 301:
		   case 139:
		   case 201:
			   strcpy(gbl.card_amount,"000000010000");
			   break;
		   case 142:
			   strcpy(gbl.card_amount,"000000020000");
			   break;
		   case 143:
			   strcpy(gbl.card_amount,"000000030000");
			   break;
		   case 144:
			   strcpy(gbl.card_amount,"000000050000");
			   break;
		   default:
			   break;			   
		}
	}
	for(i=0; i<3; i++)	//读卡号、密码和卡序列号
	{
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63 + count, 2);
		count += 2;
		len = atoi(tmp);
		if(len)
		{
			switch(i)
			{
			case 0:	//卡号
			//	bbzero(gbl.card_num);
				memcpy(card_num, pack.FIELD63 + count, len);
				break;
			case 1:	//密码
			//	bbzero(gbl.card_pwd);
				bbzero( tmp );
				bbzero(card_pwd_buff);
				memcpy(tmp, pack.FIELD63 + count, len);
				CTools::Encrypt(tmp, card_pwd_buff, 1);
				pwd_len = strlen(card_pwd_buff);
				for(k=0,j=0; k<=pwd_len/6; k++)
				{
					if(k==pwd_len/6)
						strcat(card_pwd+j, card_pwd_buff+k*6);
					else
					{
						memcpy(card_pwd+j, card_pwd_buff+k*6, 6);
						strcat(card_pwd, " ");
						j+=7;
					}
				}
				break;
			case 2:	//卡序列号
			//	bbzero(gbl.card_serial);
				memcpy(card_serial, pack.FIELD63 + count, len);
				break;
			}
			count += len;
		}//end if
	}//end for
	if (atoi(gbl.fee_type)==245)
	{
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63 + count, 2);
		count += 2;
		len = atoi(tmp);
		memcpy(xm_serial, pack.FIELD63 + count, len);
		strcpy(amount,gbl.card_amount);
	}
	else{
		memcpy(eff_yymmdd,  pack.FIELD63 + count, 8);
		count += 8;
		//扣款金额
		//	bbzero(gbl.amount);
		memcpy(amount, pack.FIELD63 + count, 12);
		count += 12;
	}
	return 0;
}

//改造函数
//读售卡业务返回明细——2006-11-1,lihk
int CPrint::rdField63_3(CPack pack,char *card_num, char *card_pwd, char *card_serial, char *amount, char *eff_yymmdd,char *xm_serial)
{
	int tol_len , len, i ,pwd_len,j,k;
	int count=0;
	char tmp[20],card_pwd_buff[128];
	
	//wrLog("【函数】rdField63_3");
	//wrLog("BIT63:[%s]",pack.FIELD63);
	
	//取得总长度
	tol_len = pack.Packet_Table[63].Real_Length;
	if(tol_len > 512)
		return -1;
	if (atoi(gbl.fee_type)==182)//东莞按stan号售卡补打
	{   
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63, 2);
		count += 2;
		len = atoi(tmp);
        bbzero(gbl.sell_card_type);
		memcpy(gbl.sell_card_type, pack.FIELD63 + 2, len);
		count=count+len;

		switch (atoi(gbl.sell_card_type))
		{  
		   case 807:
			   strcpy(gbl.card_amount,"000000001000");
			   break;
		   case 808:
			   strcpy(gbl.card_amount,"000000001500");
			   break;
		   case 128:
			   strcpy(gbl.card_amount,"000000002000");
			   break;
		   case 207:
		   case 310:
		   case 127:
		   case 124:
		   case 809:
		   case 311:
		   case 315:
			   strcpy(gbl.card_amount,"000000003000");
			   break;
		   case 205:
		   case 308:
		   case 122:
		   case 125:
		   case 129:
		   case 300:
		   case 136:
		   case 200:
			   strcpy(gbl.card_amount,"000000005000");
			   break;
		   case 106:
		   case 309:
		   case 123:
		   case 126:
		   case 130:
		   case 301:
		   case 139:
		   case 201:
			   strcpy(gbl.card_amount,"000000010000");
			   break;
		   case 142:
			   strcpy(gbl.card_amount,"000000020000");
			   break;
		   case 143:
			   strcpy(gbl.card_amount,"000000030000");
			   break;
		   case 144:
			   strcpy(gbl.card_amount,"000000050000");
			   break;
		   default:
			   break;			   
		}
	}
	for(i=0; i<3; i++)	//读卡号、密码和卡序列号
	{
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63 + count, 2);
		count += 2;
		len = atoi(tmp);
		if(len)
		{
			switch(i)
			{
			case 0:	//卡号
			//	bbzero(gbl.card_num);
				memcpy(card_num, pack.FIELD63 + count, len);
				break;
			case 1:	//密码
			//	bbzero(gbl.card_pwd);
				bbzero( tmp );
				bbzero(card_pwd_buff);
				memcpy(tmp, pack.FIELD63 + count, len);
				CTools::Encrypt(tmp, card_pwd_buff, 1);
				pwd_len = strlen(card_pwd_buff);
				for(k=0,j=0; k<=pwd_len/6; k++)
				{
					if(k==pwd_len/6)
						strcat(card_pwd+j, card_pwd_buff+k*6);
					else
					{
						memcpy(card_pwd+j, card_pwd_buff+k*6, 6);
						strcat(card_pwd, " ");
						j+=7;
					}
				}
				break;
			case 2:	//卡序列号
			//	bbzero(gbl.card_serial);
				memcpy(card_serial, pack.FIELD63 + count, len);
				break;
			}
			count += len;
		}//end if
	}//end for
	if (atoi(gbl.fee_type)==245)
	{
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63 + count, 2);
		count += 2;
		len = atoi(tmp);
		memcpy(xm_serial, pack.FIELD63 + count, len);
		strcpy(amount,gbl.card_amount);
	}
	else{
		memcpy(eff_yymmdd,  pack.FIELD63 + count, 8);
		count += 8;
		//扣款金额
		//	bbzero(gbl.amount);
		memcpy(amount, pack.FIELD63 + count, 12);
		count += 12;
	}
	return 0;
}

//end of 改造函数


void CPrint::ckCode(const char *code, char *type)	//查询业务代码所代表的业务名，写入type中
{
	switch(atoi(gbl.fee_type))
	{
	case 3012:
		if(!strncmp(code,"01",2) || !strncmp(code,"31",2) || !strncmp(code,"51",2) ||
			!strncmp(code,"61",2) || !strncmp(code,"U0",2) )
			strcpy(type,"月租");
		else if(!strncmp(code,"02",2) || !strncmp(code,"U3",2))
			strcpy(type,"市话费");
		else if(!strncmp(code,"03",2))
			strcpy(type,"长话费");
		else if(!strncmp(code,"04",2))
			strcpy(type,"市话退补");
		else if(!strncmp(code,"05",2))
			strcpy(type,"长话退补");
		else if(!strncmp(code,"06",2) || !strncmp(code,"36",2) || !strncmp(code,"62",2) || !strncmp(code,"XA",2))
			strcpy(type,"信息费");
		else if(!strncmp(code,"07",2) || !strncmp(code,"35",2) || !strncmp(code,"28",2) || !strncmp(code,"U7",2))
			strcpy(type,"滞纳金");
		else if(!strncmp(code,"08",2))
			strcpy(type,"市话其它");
		else if(!strncmp(code,"09",2))
			strcpy(type,"长话其它");
		else if(!strncmp(code,"11",2))
			strcpy(type,"市话月租");
		else if(!strncmp(code,"12",2))
			strcpy(type,"区间移动");
		else if(!strncmp(code,"13",2))
			strcpy(type,"区间话费");
		else if(!strncmp(code,"14",2))
			strcpy(type,"郊市话费");
		else if(!strncmp(code,"15",2))
			strcpy(type,"新业务费");
		else if(!strncmp(code,"16",2))
			strcpy(type,"800月费");
		else if(!strncmp(code,"17",2))
			strcpy(type,"200/800费");
		else if(!strncmp(code,"18",2))
			strcpy(type,"市话补退");
		else if(!strncmp(code,"19",2))
			strcpy(type,"市话其他");
		else if(!strncmp(code,"20",2))
			strcpy(type,"国内话费");
		else if(!strncmp(code,"21",2))
			strcpy(type,"国际话费");
		else if(!strncmp(code,"22",2) || !strncmp(code,"UB",2))
			strcpy(type,"港澳台费");
		else if(!strncmp(code,"23",2))
			strcpy(type,"IP909费");
		else if(!strncmp(code,"24",2))
			strcpy(type,"长途补退");
		else if(!strncmp(code,"25",2))
			strcpy(type,"长途其它");
		else if(!strncmp(code,"32",2))
			strcpy(type,"移动话费");
		else if(!strncmp(code,"33",2) || !strncmp(code,"U5",2))
			strcpy(type,"漫游话费");
		else if(!strncmp(code,"34",2))
			strcpy(type,"长途话费");
		else if(!strncmp(code,"37",2))
			strcpy(type,"补或退");
		else if(!strncmp(code,"38",2))
			strcpy(type,"其它");
		else if(!strncmp(code,"SA",2))
			strcpy(type,"月基本费");
		else if(!strncmp(code,"SB",2))
			strcpy(type,"新功能费");
		else if(!strncmp(code,"TX",2))
			strcpy(type,"通信费");
		else if(!strncmp(code,"SC",2))
			strcpy(type,"其它");
		else if(!strncmp(code,"TS",2))
			strcpy(type,"代收费");
		else if(!strncmp(code,"ZZ",2))
			strcpy(type,"补收费");
		else if(!strncmp(code,"ZN",2))
			strcpy(type,"违约金");
		else if(!strncmp(code,"cK",2))
			strcpy(type,"话费卡冲销");
		else if(!strncmp(code,"cJ",2))
			strcpy(type,"本次预付费");
		else if(!strncmp(code,"YF",2))
			strcpy(type,"原预付费");
		else if(!strncmp(code,"TM",2))
			strcpy(type,"存入金额");
		else if(!strncmp(code,"TU",2))
			strcpy(type,"卡内余额");
		else if(!strncmp(code,"U1",2))
			strcpy(type,"特服费");
		else if(!strncmp(code,"U2",2))
			strcpy(type,"无委费");
		else if(!strncmp(code,"U4",2))
			strcpy(type,"长途费");
		else if(!strncmp(code,"U6",2))
			strcpy(type,"减免费");
		else if(!strncmp(code,"U8",2))
			strcpy(type,"短信费");
		else if(!strncmp(code,"UA",2))
			strcpy(type,"国内费用");
		else if(!strncmp(code,"UC",2))
			strcpy(type,"国际费用");
		else if(!strncmp(code,"UD",2))
			strcpy(type,"优惠套餐");
		else if(!strncmp(code,"UN",2))
			strcpy(type,"赠款金额");
		else if(!strncmp(code,"PF",2))
			strcpy(type,"费用");
		else if(!strncmp(code,"JX",2))
			strcpy(type,"赠费");
		else if(!strncmp(code,"YJ",2))
			strcpy(type,"余额");
		break;
		
	case 100:	//珠江数码月租费
	case 112:	//珠江宽频月租费
	case 114:	//有线电视月租费
	case 172:	//珠江宽频优惠 
		if(!memcmp(code,"01",2))
			strcpy(type,"维护费");
		if(!memcmp(code,"02",2))
			strcpy(type,"节目费");
		if(!memcmp(code,"03",2))
			strcpy(type,"滞纳金");
		if(!memcmp(code,"04",2))
			strcpy(type,"通信费");
		break;
	case 3002:
		if(!strncmp(code,"01",2) || !strncmp(code,"31",2) || !strncmp(code,"51",2) ||
			!strncmp(code,"61",2) || !strncmp(code,"U0",2) )
			strcpy(type,"月租");
		else if(!strncmp(code,"02",2) || !strncmp(code,"U3",2))
			strcpy(type,"市话费");
		else if(!strncmp(code,"03",2))
			strcpy(type,"长话费");
		else if(!strncmp(code,"04",2))
			strcpy(type,"市话退补");
		else if(!strncmp(code,"05",2))
			strcpy(type,"长话退补");
		else if(!strncmp(code,"06",2) || !strncmp(code,"36",2) || !strncmp(code,"62",2) || !strncmp(code,"XA",2))
			strcpy(type,"信息费");
		else if(!strncmp(code,"07",2) || !strncmp(code,"35",2) || !strncmp(code,"28",2) || !strncmp(code,"U7",2))
			strcpy(type,"滞纳金");
		else if(!strncmp(code,"08",2))
			strcpy(type,"市话其它");
		else if(!strncmp(code,"09",2))
			strcpy(type,"长话其它");
		else if(!strncmp(code,"11",2))
			strcpy(type,"市话月租");
		else if(!strncmp(code,"12",2))
			strcpy(type,"区间移动");
		else if(!strncmp(code,"13",2))
			strcpy(type,"区间话费");
		else if(!strncmp(code,"14",2))
			strcpy(type,"郊市话费");
		else if(!strncmp(code,"15",2))
			strcpy(type,"新业务费");
		else if(!strncmp(code,"16",2))
			strcpy(type,"800月费");
		else if(!strncmp(code,"17",2))
			strcpy(type,"200/800费");
		else if(!strncmp(code,"18",2))
			strcpy(type,"市话补退");
		else if(!strncmp(code,"19",2))
			strcpy(type,"市话其他");
		else if(!strncmp(code,"20",2))
			strcpy(type,"国内话费");
		else if(!strncmp(code,"21",2))
			strcpy(type,"国际话费");
		else if(!strncmp(code,"22",2) || !strncmp(code,"UB",2))
			strcpy(type,"港澳台费");
		else if(!strncmp(code,"23",2))
			strcpy(type,"IP909费");
		else if(!strncmp(code,"24",2))
			strcpy(type,"长途补退");
		else if(!strncmp(code,"25",2))
			strcpy(type,"长途其它");
		else if(!strncmp(code,"32",2))
			strcpy(type,"移动话费");
		else if(!strncmp(code,"33",2) || !strncmp(code,"U5",2))
			strcpy(type,"漫游话费");
		else if(!strncmp(code,"34",2))
			strcpy(type,"长途话费");
		else if(!strncmp(code,"37",2))
			strcpy(type,"补或退");
		else if(!strncmp(code,"38",2))
			strcpy(type,"其它");
		else if(!strncmp(code,"SA",2))
			strcpy(type,"月基本费");
		else if(!strncmp(code,"SB",2))
			strcpy(type,"新功能费");
		else if(!strncmp(code,"TX",2))
			strcpy(type,"通信费");
		else if(!strncmp(code,"SC",2))
			strcpy(type,"其它");
		else if(!strncmp(code,"TS",2))
			strcpy(type,"代收费");
		else if(!strncmp(code,"ZZ",2))
			strcpy(type,"补收费");
		else if(!strncmp(code,"ZN",2))
			strcpy(type,"违约金");
		else if(!strncmp(code,"cK",2))
			strcpy(type,"话费卡冲销");
		else if(!strncmp(code,"cJ",2))
			strcpy(type,"本次预付费");
		else if(!strncmp(code,"YF",2))
			strcpy(type,"原预付费");
		else if(!strncmp(code,"TM",2))
			strcpy(type,"存入金额");
		else if(!strncmp(code,"TU",2))
			strcpy(type,"卡内余额");
		else if(!strncmp(code,"U1",2))
			strcpy(type,"特服费");
		else if(!strncmp(code,"U2",2))
			strcpy(type,"无委费");
		else if(!strncmp(code,"U4",2))
			strcpy(type,"长途费");
		else if(!strncmp(code,"U6",2))
			strcpy(type,"减免费");
		else if(!strncmp(code,"U8",2))
			strcpy(type,"短信费");
		else if(!strncmp(code,"UA",2))
			strcpy(type,"国内费用");
		else if(!strncmp(code,"UC",2))
			strcpy(type,"国际费用");
		else if(!strncmp(code,"UD",2))
			strcpy(type,"优惠套餐");
		else if(!strncmp(code,"UN",2))
			strcpy(type,"赠款金额");
		else if(!strncmp(code,"PF",2))
			strcpy(type,"费用");
		else if(!strncmp(code,"JX",2))
			strcpy(type,"赠费");
		else if(!strncmp(code,"YJ",2))
			strcpy(type,"余额");
		break;
	}
}

/**********************************************************************/
/*--------------------------------打印函数----------------------------*/
/**********************************************************************/

//打印明细
//flag：1, 欠费明细
//      2, 预付费明细
//      3, 售卡明细
//      4, 商户明细（目前为天天）
//      5, 打印有线明细
//      6, 打印picc明细
//      7, 打印电影票明细
//type：业务名称
int CPrint::print_detail(int flag, const char *type, char *field48, char *field63, char *priCont)
{
	int ret;
	switch( flag )
	{
	case 1:
		wrLog("print_pym_detail打印开始……");
		ret = print_pym_detail( type );  //打印欠费明细
		break;
	case 2:
		wrLog("print_pre_detail打印开始……");
		ret = print_pre_detail( type );	 //打印预付费明细
		break;
	case 3:
		wrLog("print_card_detail打印开始……");
		ret = print_card_detail( type ); //打印售卡明细	
		break;
	case 4:
		wrLog("print_mer_detail打印开始……");
		ret = print_mer_detail( type );	 //打印商户明细（目前为天天）	
		break;
	case 5:
		wrLog("print_CATV_detail打印开始……");
		ret = print_CATV_detail( type ); //打印有线欠费明细
		break;
	case 6:
		wrLog("print_PICC_detail打印开始……");
		ret = print_PICC_detail( type ); //打印picc明细
		break;
	case 7:
		wrLog("print_film_detail打印开始……");
		ret = print_film_detail( type ); //打印电影票明细
		break;
	case 8:
		wrLog("print_ysq_detail打印开始……");
		ret = print_ysq_detail( type ); //打印预授权明细
		break;
	case 9:
		wrLog("print_traffic_detail打印开始……");
		ret = print_traffic_detail( type, field48, field63,0 ); //打印交罚明细
		break;
   
	case 10://交罚凭证补打
		wrLog("print_traffic_detail打印开始……");
		ret = print_traffic_detail( type, field48, field63,54 ); 
		break;
		
    case 11:
		wrLog("print_CATVDiscount_detail打印开始……");
		ret = print_CATVDiscount_detail(type); //打印珠江宽频优惠
		break;
	case 12:
		wrLog("print_ADD_ADSL打印开始……");
		ret = print_ADD_ADSL(type); //打印固话加装ADSL
		break;
	case 13:
		wrLog("print_SELL_POS打印开始……");
		ret = print_SELL_POS(type); //终端销售
		break;
	case 14:
		wrLog("print_chk_amo打印开始……");
		ret = print_chk_amo(type, field48); //结算
		break;
	case 15:
		wrLog("reprint_bus_detail东莞公交开卡补打打印开始……");
		ret = reprint_bus_detail(type); 
		break;
	case 16:
		wrLog("print_yct_detail羊城通充值打印开始……");
		ret = print_yct_detail(type, priCont); 
		break;
	case 17:
		wrLog("print_province_traffic_detail全省交罚打印开始……");
		ret = print_province_traffic_detail(type); 
		break;
	case 18:
		wrLog("print_bestpay_detail 翼支付充值打印开始……");
		ret=print_bestpay_detail(type);
		break;
	case 19:
		wrLog("print_xjx_detail 薪加薪资金划拨打印开始……");
		ret=print_xjx_detail(type);
		break;
	case 20:
		wrLog("print_yctthird_detail 羊城通充值第三代打印开始……");
		ret=print_yctthird_detail(type,priCont);
		break;	
	default:
		wrLog("print_pym_detail打印开始……");
		ret = print_pym_detail( type );
	}
	return ret;
}


int CPrint::reprint_bus_detail(const char *type)		//公交开卡补打印
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	PrintTemp prtTemp;   //打印模板

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.phone, gbl.card_num);
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"#%s",gbl.stan_no);//系统参考号				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细
	bbzero(prtTemp.type);
	bbzero(prtTemp.content);
	prtTemp.n=0;
	if( atoi(gbl.amount)==4500 ){
		strcpy(prtTemp.type, "东莞公交开卡工本费收据");
		strcpy(prtTemp.content, "出售：15.00元");
	}else if(  atoi(gbl.amount)==4800 ){
		strcpy(prtTemp.type, "东莞公交开卡租金收据");
		strcpy(prtTemp.content, "租金：18.00元");
	}else if(  atoi(gbl.amount)==5500  ){
		strcpy(prtTemp.type, "东莞公交开卡押金收据");
		strcpy(prtTemp.content, "押金：25.00元");
	}else{
		strcpy(prtTemp.type, "未知");
		strcpy(prtTemp.content, "未知");
	}
	strcat(prtTemp.content, "\n卡原有金额：0.00元");
	strcat(prtTemp.content, "\n卡充入金额：30.00元");
	strcat(prtTemp.content, "\n卡现有金额：30.00元\n");
	if(atoi(gbl.amount)==4500){
		strcat(prtTemp.content, "备注：此卡一次性买断，一概不做退卡操作\n");
	}else if( atoi(gbl.amount)==4800 ){
		strcat(prtTemp.content, "备注：退还租金时需要持IC卡及本收据方可办理\n");
	}else if( atoi(gbl.amount)==5500 ){
		strcat(prtTemp.content, "备注：退还押金时需要持IC卡及本收据方可办理\n");
	}
	else{
		strcat(prtTemp.content, "备注：未知！\n");
	}
	
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);
	strcat(prtTemp.content,advertisement);
	prtTemp.n += adv_count;
	prtTemp.n += 5;
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//改造函数
int CPrint::reprint_bus_detail(CPack pack,const char *type)		//公交开卡补打印
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	PrintTemp prtTemp;   //打印模板
	
	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.phone, gbl.card_num);
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"#%s",gbl.stan_no);//系统参考号				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细
	bbzero(prtTemp.type);
	bbzero(prtTemp.content);
	prtTemp.n=0;
	if( atoi(gbl.amount)==4500 ){
		strcpy(prtTemp.type, "东莞公交开卡工本费收据");
		strcpy(prtTemp.content, "出售：15.00元");
	}else if(  atoi(gbl.amount)==4800 ){
		strcpy(prtTemp.type, "东莞公交开卡租金收据");
		strcpy(prtTemp.content, "租金：18.00元");
	}else if(  atoi(gbl.amount)==5500  ){
		strcpy(prtTemp.type, "东莞公交开卡押金收据");
		strcpy(prtTemp.content, "押金：25.00元");
	}else{
		strcpy(prtTemp.type, "未知");
		strcpy(prtTemp.content, "未知");
	}
	strcat(prtTemp.content, "\n卡原有金额：0.00元");
	strcat(prtTemp.content, "\n卡充入金额：30.00元");
	strcat(prtTemp.content, "\n卡现有金额：30.00元\n");
	if(atoi(gbl.amount)==4500){
		strcat(prtTemp.content, "备注：此卡一次性买断，一概不做退卡操作\n");
	}else if( atoi(gbl.amount)==4800 ){
		strcat(prtTemp.content, "备注：退还租金时需要持IC卡及本收据方可办理\n");
	}else if( atoi(gbl.amount)==5500 ){
		strcat(prtTemp.content, "备注：退还押金时需要持IC卡及本收据方可办理\n");
	}
	else{
		strcat(prtTemp.content, "备注：未知！\n");
	}
	
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);
	strcat(prtTemp.content,advertisement);
	prtTemp.n += adv_count;
	prtTemp.n += 5;
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据，此票不作报销凭证");
	prtTemp.n ++;
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}


//end of 改造函数


//打印交费明细
int CPrint::print_pym_detail(const char *type)		
{
	BYTE pMsg[1024];
	char temp[100], mny[20][MAXAMOLEN];
	char preMny[15], amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet;
	int i,j,k,flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板

	/* 保存特定的系统参考号 */
	char stan_serial[300];
	char unique_stan[6][30];

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);

	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.amount: [%s]", gbl.amount);
	//读入具体交费条目
	if(!memcmp(gbl.fee_type,"0012",4) || !memcmp(gbl.fee_type,"3065",4)||!memcmp(gbl.fee_type,"0218",4))
	{
		flag = 1;
		iRet = rdField63_2();
		if(iRet)
			return -1;
	}
	//非联通、车船税(查询时已返回明细)
	else if(memcmp(gbl.fee_type,"0049",4) && memcmp(gbl.fee_type,"0065",4) && memcmp(gbl.fee_type,"0092",4) && memcmp(gbl.fee_type,"0210",4)&& memcmp(gbl.fee_type,"0310",4)&& memcmp(gbl.fee_type,"0321",4)&& memcmp(gbl.fee_type,"0337",4)&& memcmp(gbl.fee_type,"0344",4))
	{
		iRet = rdField63(gbl, 0);
		if(iRet)
		{
			wrLog("【rdField63】错误");
			return -1;
		}
	}
	strcpy(prtTemp.type, type);
	if(!strcmp(gbl.fee_type,"0049"))
	{
		if(!strcmp(gbl.sub_code,"01"))
			strcat(prtTemp.type,"(130/131)");
		else if(!strcmp(gbl.sub_code,"02"))
			strcat(prtTemp.type,"(CDMA)");
		else if(!strcmp(gbl.sub_code,"03"))
			strcat(prtTemp.type,"(193/17911)");
	}
	wrLog("交费项目：%s", prtTemp.type);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	if(!memcmp(gbl.fee_type2,"0149",4)){
       sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	   memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);					 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	strcpy(prtTemp.name, gbl.user_name);			 //用户名
	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if(!flag)	//非补打发票
		strcpy(prtTemp.amo, amoMny); //实扣
	else		//补打发票
		strcat(prtTemp.stan, "       补打发票");

	/***  for test  ***/
	/* 记录返回的特殊系统参考号 */
	if((!memcmp(gbl.fee_type, "0092", 4) || !memcmp(gbl.fee_type, "0337", 4)) && gbl.print_paper)
	{
		bbzero(unique_stan);
		bbzero(stan_serial);
		strcpy(stan_serial, (char*)pack.FIELD63);
		
		for(i = 0, j = 0; i < 6 && j < strlen(stan_serial); i++)
		{
			iRet = CTools::findChar(stan_serial, ';', j);
			memcpy(unique_stan[i], stan_serial + j, iRet - j);
			j = iRet + 1;
			wrLog("%i %i %s", j, strlen(stan_serial), unique_stan[i]);
		}
	}

	/******************/

	/*-------------分别打印各个交费时段-------------*/
	for(j=0;j<gbl.detail.nMonCnt;j++)		
	{
		//获取返回为"98"标识的明细
		strcpy(prtTemp.detail98Amo,  gbl.detail.monDetail[j].detail98Amo);
		strcpy(prtTemp.detail98Name, gbl.detail.monDetail[j].detail98Name);
		bbzero(prtTemp.content);
		sprintf(prtTemp.count,"第%d张/共%d张", j+1, gbl.detail.nMonCnt);
		//获取合计金额
		memset(mny,0,sizeof(mny));
		for(k=0; k<gbl.detail.monDetail[j].nCnt; k++)
			strcpy(mny[k], gbl.detail.monDetail[j].cnt[k].amo);
		bbzero(prtTemp.total);
		CTools::getTalAmo(mny, prtTemp.total, gbl.detail.monDetail[j].nCnt);
		//转换为大写金额
		bbzero(prtTemp.capsTotal);
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
		prtTemp.n=0;
		for(i=0;i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//最多打9行明细项（2列）
		{
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//左对齐
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			if(m_prtType == 1 || m_prtType == 3 || m_prtType == 8 || m_prtType == 9)  //发票打印纸
			{
				if(i%2 || !strcmp(gbl.fee_type, "0092"))
				{
					strcat(prtTemp.content,"\n");
					prtTemp.n++;
				}
				else
				{
					strcat(prtTemp.content,"  ");
					if(i==gbl.detail.monDetail[j].nCnt-1)
					{
						strcat(prtTemp.content,"\n");
						prtTemp.n++;
					}
				}
			}
			else  //白纸打印
				strcat(prtTemp.content,"\n");
		}

		//移动在有欠费情况下交预付费
		if(gbl.mbl_flag == 2)	
		{
			if( strcmp(gbl.tmp_pre_amount, "") && (j == gbl.detail.nMonCnt-1) )
			{
				strcpy(prtTemp.type, "移动预付费");
				strcat(prtTemp.content, "原预付费余额：");
				bbzero(preMny);
				CTools::chgToMny(gbl.pre_fee_buffer, preMny, 0);
				strcat(prtTemp.content, preMny);
				strcat(prtTemp.content, "\n");
				strcat(prtTemp.content, "\n");
				memset(amoMny,0,sizeof(amoMny));
				CTools::chgToMny(gbl.tmp_pre_amount, amoMny);
				prtTemp.n+=2;
			}
		}
		//2006-10-30，付费易卡
		if(!memcmp(gbl.card_no, "8828", 4))
		{
			if(i%2)
			{
				strcat(prtTemp.content, "\n\n");
				prtTemp.n+= 2;
			}
			else
			{
				strcat(prtTemp.content, "\n");
				prtTemp.n++;
			}
		}
		////////////////////////////////////////////////////
		//佛山后付费新发票纸修改
		if(!strcmp(gbl.fee_type, "0092") && gbl.print_paper)
		{
			char strFromDate[9], strToDate[9], strTemp[9];
			bbzero(strFromDate);
			bbzero(strToDate);
			bbzero(strTemp);
			memcpy(strTemp, gbl.detail.monDetail[j].tmFrom, 4);
			memcpy(strTemp+4, gbl.detail.monDetail[j].tmFrom+6, 2);
			memcpy(strTemp+6, gbl.detail.monDetail[j].tmFrom+10, 2);
			strcpy(strFromDate, strTemp);
			bbzero(strTemp);
			memcpy(strTemp, gbl.detail.monDetail[j].tmTo, 4);
			memcpy(strTemp+4, gbl.detail.monDetail[j].tmTo+6, 2);
			memcpy(strTemp+6, gbl.detail.monDetail[j].tmTo+10, 2);
			strcpy(strToDate, strTemp);
			sprintf(prtTemp.month, "%s:%s", strFromDate, strToDate);
		}
		//固话，佛山综合收费,东莞电信后付费
		else if(!strcmp(gbl.fee_type,"0131") || !strcmp(gbl.fee_type,"0241") ||!strcmp(gbl.fee_type,"0001") || !strcmp(gbl.fee_type,"0013") || !strcmp(gbl.fee_type,"0092") || !strcmp(gbl.fee_type,"0210")|| !strcmp(gbl.fee_type,"0310")||!memcmp(gbl.fee_type,"0337",4))	
		{
			sprintf(prtTemp.month, "%s－%s", gbl.detail.monDetail[j].tmFrom, gbl.detail.monDetail[j].tmTo);
		}
		else if(!strcmp(gbl.fee_type,"0065"))
		{
			memset(temp, 0, sizeof(temp));	//车牌号＋税务流水
			memcpy(temp, "粤A",3);
			memcpy(temp+3, gbl.car_no+3, 5);
			strcat(temp, "---");
			strcat(temp, gbl.tax_serial);
			strcpy(prtTemp.phone, temp);
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
		else	//移动或联通
		{
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
        

		if(!memcmp(gbl.fee_type,"0131",4)){//东莞电信后付费
            strcat(prtTemp.content, advertisement);
			prtTemp.n+=adv_count;
			prtTemp.n--;
		}
		
		/***  for test  ***/
		/*0092,0337 系统参考号改变*/
		if((!memcmp(gbl.fee_type, "0092", 4) || !memcmp(gbl.fee_type, "0337", 4)) && gbl.print_paper)
		{
			strcpy(prtTemp.stan, unique_stan[j]);
			if(!flag)	//非补打发票
			{
				strcpy(prtTemp.amo, amoMny); //实扣
			}
			else		//补打发票
			{	
				strcat(prtTemp.stan, "    补打发票");
			}
		}
		/******************/

		strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
		prtTemp.n++;
		
		memset(pMsg,0,sizeof(pMsg));
		if(strcmp(prtTemp.detail98Amo, ""))
			iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType, 1);
		else if( !memcmp(gbl.fee_type,"0210",4)||!memcmp(gbl.fee_type,"0310",4)||!memcmp(gbl.fee_type,"0337",4) )
			iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType, 3);
		else
			iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType);
		iPrintLen = strlen((char*)pMsg);
		iRet = print_func((char*)pMsg,iPrintLen);
	}
	return iRet;
}

//打印改造函数
//打印交费明细
int CPrint::print_pym_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char temp[100], mny[20][MAXAMOLEN];
	char preMny[15], amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet;
	int i,j,k,flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板

	/* 保存特定的系统参考号 */
	char stan_serial[300];
	char unique_stan[6][30];

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);

	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.amount: [%s]", gbl.amount);
	//读入具体交费条目
	if(!memcmp(gbl.fee_type,"0012",4) || !memcmp(gbl.fee_type,"3065",4)||!memcmp(gbl.fee_type,"0218",4))
	{
		flag = 1;
		iRet = rdField63_2(pack);
		if(iRet)
			return -1;
	}
	//非联通、车船税(查询时已返回明细)
	else if(memcmp(gbl.fee_type,"0049",4) && memcmp(gbl.fee_type,"0065",4) && memcmp(gbl.fee_type,"0092",4) && memcmp(gbl.fee_type,"0210",4)&& memcmp(gbl.fee_type,"0310",4)&& memcmp(gbl.fee_type,"0321",4)&& memcmp(gbl.fee_type,"0337",4)&& memcmp(gbl.fee_type,"0344",4))
	{
		iRet = rdField63(pack,gbl, 0);
		if(iRet)
		{
			wrLog("【rdField63】错误");
			return -1;
		}
	}
	strcpy(prtTemp.type, type);
	if(!strcmp(gbl.fee_type,"0049"))
	{
		if(!strcmp(gbl.sub_code,"01"))
			strcat(prtTemp.type,"(130/131)");
		else if(!strcmp(gbl.sub_code,"02"))
			strcat(prtTemp.type,"(CDMA)");
		else if(!strcmp(gbl.sub_code,"03"))
			strcat(prtTemp.type,"(193/17911)");
	}
	wrLog("交费项目：%s", prtTemp.type);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	if(!memcmp(gbl.fee_type2,"0149",4)){
       sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	   memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);					 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	strcpy(prtTemp.name, gbl.user_name);			 //用户名
	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if(!flag)	//非补打发票
		strcpy(prtTemp.amo, amoMny); //实扣
	else		//补打发票
		strcat(prtTemp.stan, "       补打发票");

	/***  for test  ***/
	/* 记录返回的特殊系统参考号 */
	if((!memcmp(gbl.fee_type, "0092", 4) || !memcmp(gbl.fee_type, "0337", 4)) && gbl.print_paper)
	{
		bbzero(unique_stan);
		bbzero(stan_serial);
		strcpy(stan_serial, (char*)pack.FIELD63);
		
		for(i = 0, j = 0; i < 6 && j < strlen(stan_serial); i++)
		{
			iRet = CTools::findChar(stan_serial, ';', j);
			memcpy(unique_stan[i], stan_serial + j, iRet - j);
			j = iRet + 1;
			wrLog("%i %i %s", j, strlen(stan_serial), unique_stan[i]);
		}
	}

	/******************/

	/*-------------分别打印各个交费时段-------------*/
	for(j=0;j<gbl.detail.nMonCnt;j++)		
	{
		//获取返回为"98"标识的明细
		strcpy(prtTemp.detail98Amo,  gbl.detail.monDetail[j].detail98Amo);
		strcpy(prtTemp.detail98Name, gbl.detail.monDetail[j].detail98Name);
		bbzero(prtTemp.content);
		sprintf(prtTemp.count,"第%d张/共%d张", j+1, gbl.detail.nMonCnt);
		//获取合计金额
		memset(mny,0,sizeof(mny));
		for(k=0; k<gbl.detail.monDetail[j].nCnt; k++)
			strcpy(mny[k], gbl.detail.monDetail[j].cnt[k].amo);
		bbzero(prtTemp.total);
		CTools::getTalAmo(mny, prtTemp.total, gbl.detail.monDetail[j].nCnt);
		//转换为大写金额
		bbzero(prtTemp.capsTotal);
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
		prtTemp.n=0;
		for(i=0;i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//最多打9行明细项（2列）
		{
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//左对齐
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			if(m_prtType == 1 || m_prtType == 3 || m_prtType == 8 || m_prtType == 9)  //发票打印纸
			{
				if(i%2 || !strcmp(gbl.fee_type, "0092"))
				{
					strcat(prtTemp.content,"\n");
					prtTemp.n++;
				}
				else
				{
					strcat(prtTemp.content,"  ");
					if(i==gbl.detail.monDetail[j].nCnt-1)
					{
						strcat(prtTemp.content,"\n");
						prtTemp.n++;
					}
				}
			}
			else  //白纸打印
				strcat(prtTemp.content,"\n");
		}

		//移动在有欠费情况下交预付费
		if(gbl.mbl_flag == 2)	
		{
			if( strcmp(gbl.tmp_pre_amount, "") && (j == gbl.detail.nMonCnt-1) )
			{
				strcpy(prtTemp.type, "移动预付费");
				strcat(prtTemp.content, "原预付费余额：");
				bbzero(preMny);
				CTools::chgToMny(gbl.pre_fee_buffer, preMny, 0);
				strcat(prtTemp.content, preMny);
				strcat(prtTemp.content, "\n");
				strcat(prtTemp.content, "\n");
				memset(amoMny,0,sizeof(amoMny));
				CTools::chgToMny(gbl.tmp_pre_amount, amoMny);
				prtTemp.n+=2;
			}
		}
		//2006-10-30，付费易卡
		if(!memcmp(gbl.card_no, "8828", 4))
		{
			if(i%2)
			{
				strcat(prtTemp.content, "\n\n");
				prtTemp.n+= 2;
			}
			else
			{
				strcat(prtTemp.content, "\n");
				prtTemp.n++;
			}
		}
		////////////////////////////////////////////////////
		//佛山后付费新发票纸修改
		if(!strcmp(gbl.fee_type, "0092") && gbl.print_paper)
		{
			char strFromDate[9], strToDate[9], strTemp[9];
			bbzero(strFromDate);
			bbzero(strToDate);
			bbzero(strTemp);
			memcpy(strTemp, gbl.detail.monDetail[j].tmFrom, 4);
			memcpy(strTemp+4, gbl.detail.monDetail[j].tmFrom+6, 2);
			memcpy(strTemp+6, gbl.detail.monDetail[j].tmFrom+10, 2);
			strcpy(strFromDate, strTemp);
			bbzero(strTemp);
			memcpy(strTemp, gbl.detail.monDetail[j].tmTo, 4);
			memcpy(strTemp+4, gbl.detail.monDetail[j].tmTo+6, 2);
			memcpy(strTemp+6, gbl.detail.monDetail[j].tmTo+10, 2);
			strcpy(strToDate, strTemp);
			sprintf(prtTemp.month, "%s:%s", strFromDate, strToDate);
		}
		//固话，佛山综合收费,东莞电信后付费
		else if(!strcmp(gbl.fee_type,"0131") || !strcmp(gbl.fee_type,"0241") ||!strcmp(gbl.fee_type,"0001") || !strcmp(gbl.fee_type,"0013") || !strcmp(gbl.fee_type,"0092") || !strcmp(gbl.fee_type,"0210")|| !strcmp(gbl.fee_type,"0310")||!memcmp(gbl.fee_type,"0337",4))	
		{
			sprintf(prtTemp.month, "%s－%s", gbl.detail.monDetail[j].tmFrom, gbl.detail.monDetail[j].tmTo);
		}
		else if(!strcmp(gbl.fee_type,"0065"))
		{
			memset(temp, 0, sizeof(temp));	//车牌号＋税务流水
			memcpy(temp, "粤A",3);
			memcpy(temp+3, gbl.car_no+3, 5);
			strcat(temp, "---");
			strcat(temp, gbl.tax_serial);
			strcpy(prtTemp.phone, temp);
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
		else	//移动或联通
		{
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
        

		if(!memcmp(gbl.fee_type,"0131",4)){//东莞电信后付费
            strcat(prtTemp.content, advertisement);
			prtTemp.n+=adv_count;
			prtTemp.n--;
		}
		
		/***  for test  ***/
		/*0092,0337 系统参考号改变*/
		if((!memcmp(gbl.fee_type, "0092", 4) || !memcmp(gbl.fee_type, "0337", 4)) && gbl.print_paper)
		{
			strcpy(prtTemp.stan, unique_stan[j]);
			if(!flag)	//非补打发票
			{
				strcpy(prtTemp.amo, amoMny); //实扣
			}
			else		//补打发票
			{	
				strcat(prtTemp.stan, "    补打发票");
			}
		}
		/******************/

		strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
		prtTemp.n++;

		memset(pMsg,0,sizeof(pMsg));
		if(strcmp(prtTemp.detail98Amo, ""))
			iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType, 1);
		else if( !memcmp(gbl.fee_type,"0210",4)||!memcmp(gbl.fee_type,"0310",4)||!memcmp(gbl.fee_type,"0337",4) )
			iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType, 3);
		else
			iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType);
		iPrintLen = strlen((char*)pMsg);
		iRet = print_func((char*)pMsg,iPrintLen);
	}
	return iRet;
}

//end of 打印改造函数

int CPrint::print_pre_detail(const char *type)		//打印预付费明细
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char preMny[15], amoMny[15],phone_mny[12+1];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	char month_count[1+1];
	char temp[12+1];
	char tmp[256];
	PrintTemp prtTemp;   //打印模板

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	switch(atoi(gbl.fee_type))				//用户名、号码
	{
	case 100:	//珠江数码
	case 112:	//珠江宽频
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s（%s）", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//有线电视
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[0].user_name);
		strcpy(prtTemp.phone, gbl.CATV_manual_num);
		break;
	case 281:
		bbzero(prtTemp.name);
		bbzero(prtTemp.phone);
		break;
    case 176:
		bbzero(prtTemp.name);
		bbzero(prtTemp.phone);
// 		bbzero(temp);
// 		memcpy(temp, (char *)(pack.FIELD63+gbl.num), 2);
// 		memcpy(prtTemp.name, (char *)(pack.FIELD63+gbl.num+2), atoi(temp));
        strcpy(prtTemp.phone, gbl.phone_no);
		break;
	case 265:
	case 300:
//	case 296:
	case 297:
	case 342: //省联通低费率
	case 356: //省联通高费率
		bbzero(prtTemp.name);
		strcpy(prtTemp.phone, gbl.phone_no);
		break;
	default:
		strcpy(prtTemp.name, gbl.user_name);
		strcpy(prtTemp.phone, gbl.phone_no);
	}
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	if(!memcmp(gbl.fee_type2,"0149",4)){
		sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);						 //业务类型
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细

	prtTemp.n=0;
//     if (atoi(gbl.fee_type) == 176 && gbl.tgroup == 40)
// 	{
// 		sprintf(prtTemp.content,"收银金额：%s\n", prtTemp.total);
// 	}
// 	else
	sprintf(prtTemp.content, "存入金额：%s\n", prtTemp.total);
	prtTemp.n++;
	switch(atoi(gbl.fee_type))
	{
	case 3012:
		strcat(prtTemp.content, "原预付费余额：");
		bbzero(preMny);
		CTools::chgToMny(gbl.pre_fee_buffer, preMny, 0);
		strcat(prtTemp.content, preMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;

	case 71:
		strcat(prtTemp.content, "宽带预缴费优惠赠送：");
		switch(atol(gbl.amount))
		{
		case 50000:
			strcat(prtTemp.content, "100.00\n");
			break;
		case 100000:
			strcat(prtTemp.content, "300.00\n");
			break;
		case 180000:
			strcat(prtTemp.content, "600.00\n");
			break;
		}
		prtTemp.n++;
		break;
	case 96:	//长市话费预付金
	case 97:	//ADSL话费预付金
	case 138:	//ITV预付金 add by kf
	case 242:  //CDMA预付金
		bbzero(amoMny);		
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+12),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "现可用余额:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;
	case 100:  //数字电视
	case 112:  //珠江宽频
	case 114:  //有线电视
		bbzero(amoMny);
		len += 3;
		bbzero(month_count);
		memcpy(month_count,(char*)(pack.FIELD63+len),1);
		len += 1;
		len +=atoi(month_count)*23;
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+len),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "预存余额:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		bbzero(prtTemp.total);
		bbzero(prtTemp.capsTotal);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
		break;
	case 176://东莞移动预付费
		strcat(prtTemp.content,"可发送短信001到1861查询余额\n");
//		prtTemp.n++;
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
	    prtTemp.n += adv_count;
		break;

	case 167://东莞QQ充值
	case 168://东莞QQ包月
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		prtTemp.n--;
		break;
	case 281:
		bbzero(tmp);
		sprintf(tmp,"充值账户：%s",gbl.pay_easy_zh);
		strcat(prtTemp.content, tmp);
		break;
	case 180:
	case 169:
	case 174:
	case 255:
	case 257:
	case 296:
	case 321:
	case 344:
	case 343:
	case 305:
		if (atoi(gbl.phone_amount)>0)
		{
			bbzero(tmp);
			bbzero(phone_mny);
			CTools::chgToMny(gbl.phone_amount,phone_mny);
			sprintf(tmp,"号码余额：%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		break;
	case 213:
	case 297:
	case 316:
		if (atoi(gbl.phone_amount)>0)
		{
			bbzero(tmp);
			bbzero(phone_mny);
			CTools::chgToMny(gbl.phone_amount,phone_mny);
			sprintf(tmp,"号码余额：%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		prtTemp.n--;
		break;
	case 256:
		if (atoi(gbl.phone_amount)>0)
		{
			bbzero(tmp);
			bbzero(phone_mny);
			CTools::chgToMny(gbl.phone_amount,phone_mny);
			sprintf(tmp,"号码余额：%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		strcat(prtTemp.content,"请拨打11888查询充值到账信息\n");
		//		prtTemp.n++;
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		break;
		
	}

	if(atoi(gbl.fee_type)!=176 && atoi(gbl.fee_type)!=256 && atoi(gbl.fee_type)!=213){//非东莞移动预付费、电信直充、移动直充
		strcat(prtTemp.content,"\n");
		prtTemp.n++;
	}
	switch(atoi(gbl.fee_type))
	{
	case 71:
		strcat(prtTemp.content,"赠送的金额由第三个计费月开始，\n分十个月平均赠送。详情请咨询：10000\n");
		prtTemp.n += 2;
		break;
	case 240:
		strcat(prtTemp.content,"\n说明：\n预交100元送300元话费，预交的本金话费即时到账\n赠送的话费从12月起分15个月赠送，每月20元\n其中首笔赠送话费12月26日后到账\n");
		prtTemp.n += 5;
		break;
	}
//	for(i=0;i<9-n;i++)
//		strcat(prtTemp.content,"\n");
	
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n ++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen); 

	return iRet;
}


//改造函数 by liuxiao
int CPrint::print_pre_detail(CPack pack,const char *type)		//打印预付费明细
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char preMny[15], amoMny[15],phone_mny[12+1];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	char month_count[1+1];
	char temp[12+1];
	char tmp[256];
	PrintTemp prtTemp;   //打印模板

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	switch(atoi(gbl.fee_type))				//用户名、号码
	{
	case 100:	//珠江数码
	case 112:	//珠江宽频
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s（%s）", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//有线电视
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[0].user_name);
		strcpy(prtTemp.phone, gbl.CATV_manual_num);
		break;
	case 281:
		bbzero(prtTemp.name);
		bbzero(prtTemp.phone);
		break;
    case 176:
		bbzero(prtTemp.name);
		bbzero(prtTemp.phone);
// 		bbzero(temp);
// 		memcpy(temp, (char *)(pack.FIELD63+gbl.num), 2);
// 		memcpy(prtTemp.name, (char *)(pack.FIELD63+gbl.num+2), atoi(temp));
        strcpy(prtTemp.phone, gbl.phone_no);
		break;
	case 265:
	case 300:
//	case 296:
	case 297:
	case 342: //省联通低费率
	case 356: //省联通高费率
		bbzero(prtTemp.name);
		strcpy(prtTemp.phone, gbl.phone_no);
		break;
	default:
		strcpy(prtTemp.name, gbl.user_name);
		strcpy(prtTemp.phone, gbl.phone_no);
	}
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	if(!memcmp(gbl.fee_type2,"0149",4)){
		sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);						 //业务类型
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细

	prtTemp.n=0;
//     if (atoi(gbl.fee_type) == 176 && gbl.tgroup == 40)
// 	{
// 		sprintf(prtTemp.content,"收银金额：%s\n", prtTemp.total);
// 	}
// 	else
	sprintf(prtTemp.content, "存入金额：%s\n", prtTemp.total);
	prtTemp.n++;
	switch(atoi(gbl.fee_type))
	{
	case 3012:
		strcat(prtTemp.content, "原预付费余额：");
		bbzero(preMny);
		CTools::chgToMny(gbl.pre_fee_buffer, preMny, 0);
		strcat(prtTemp.content, preMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;

	case 71:
		strcat(prtTemp.content, "宽带预缴费优惠赠送：");
		switch(atol(gbl.amount))
		{
		case 50000:
			strcat(prtTemp.content, "100.00\n");
			break;
		case 100000:
			strcat(prtTemp.content, "300.00\n");
			break;
		case 180000:
			strcat(prtTemp.content, "600.00\n");
			break;
		}
		prtTemp.n++;
		break;
	case 96:	//长市话费预付金
	case 97:	//ADSL话费预付金
	case 138:	//ITV预付金 add by kf
	case 242:  //CDMA预付金
		bbzero(amoMny);		
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+12),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "现可用余额:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;
	case 100:  //数字电视
	case 112:  //珠江宽频
	case 114:  //有线电视
		bbzero(amoMny);
		len += 3;
		bbzero(month_count);
		memcpy(month_count,(char*)(pack.FIELD63+len),1);
		len += 1;
		len +=atoi(month_count)*23;
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+len),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "预存余额:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		bbzero(prtTemp.total);
		bbzero(prtTemp.capsTotal);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
		break;
	case 176://东莞移动预付费
		strcat(prtTemp.content,"可发送短信001到1861查询余额\n");
//		prtTemp.n++;
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
	    prtTemp.n += adv_count;
		break;

	case 167://东莞QQ充值
	case 168://东莞QQ包月
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		prtTemp.n--;
		break;
	case 281:
		bbzero(tmp);
		sprintf(tmp,"充值账户：%s",gbl.pay_easy_zh);
		strcat(prtTemp.content, tmp);
		break;
	case 180:
	case 169:
	case 174:
	case 255:
	case 257:
	case 296:
	case 321:
	case 344:
	case 343:
	case 305:
		if (atoi(gbl.phone_amount)>0)
		{
			bbzero(tmp);
			bbzero(phone_mny);
			CTools::chgToMny(gbl.phone_amount,phone_mny);
			sprintf(tmp,"号码余额：%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		break;
	case 213:
	case 297:
	case 316:
		if (atoi(gbl.phone_amount)>0)
		{
			bbzero(tmp);
			bbzero(phone_mny);
			CTools::chgToMny(gbl.phone_amount,phone_mny);
			sprintf(tmp,"号码余额：%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		prtTemp.n--;
		break;
	case 256:
		if (atoi(gbl.phone_amount)>0)
		{
			bbzero(tmp);
			bbzero(phone_mny);
			CTools::chgToMny(gbl.phone_amount,phone_mny);
			sprintf(tmp,"号码余额：%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		strcat(prtTemp.content,"请拨打11888查询充值到账信息\n");
		//		prtTemp.n++;
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		break;
		
	}

	if(atoi(gbl.fee_type)!=176 && atoi(gbl.fee_type)!=256 && atoi(gbl.fee_type)!=213){//非东莞移动预付费、电信直充、移动直充
		strcat(prtTemp.content,"\n");
		prtTemp.n++;
	}
	switch(atoi(gbl.fee_type))
	{
	case 71:
		strcat(prtTemp.content,"赠送的金额由第三个计费月开始，\n分十个月平均赠送。详情请咨询：10000\n");
		prtTemp.n += 2;
		break;
	case 240:
		strcat(prtTemp.content,"\n说明：\n预交100元送300元话费，预交的本金话费即时到账\n赠送的话费从12月起分15个月赠送，每月20元\n其中首笔赠送话费12月26日后到账\n");
		prtTemp.n += 5;
		break;
	}
//	for(i=0;i<9-n;i++)
//		strcat(prtTemp.content,"\n");
	
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n ++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen); 

	return iRet;
}



//售卡打印明细——2006-11-1,lihk
int CPrint::print_card_detail(const char *type)	
{
	int iPrintLen, iRet, i;
	BYTE pMsg[1024];
	char cardMny[15],amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	char eff_yymmdd[8+1]; //有效日期
	char xm_serial[30+1], card_no[20+1], card_pwd[20+1], m_type[20+1];
	char show_buff[1024];
	PrintTemp prtTemp;

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);

	bbzero(m_type);
	strcpy(m_type, type);
	
	//提取打印数据
	bbzero(gbl.card_num);
	bbzero(gbl.card_pwd);
	bbzero(gbl.card_serial);
	bbzero(gbl.amount);	
	bbzero(eff_yymmdd);
	bbzero(xm_serial);
	bbzero(card_pwd);
	bbzero(card_no);
	if( rdField63_3(gbl.card_num, gbl.card_pwd, gbl.card_serial, gbl.amount, eff_yymmdd,xm_serial) )
		return -1;
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(cardMny);	//充值面值
	CTools::chgToMny(gbl.card_amount, cardMny);
	bbzero(show_buff);
	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==200||atoi(gbl.sell_card_type)==201){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"移动充值卡");
		
	}
	if(atoi(gbl.sell_card_type)==311||atoi(gbl.sell_card_type)==300||atoi(gbl.sell_card_type)==301){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"联通充值卡");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"小灵通充值卡");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"宽带充值卡");

	}
	if(atoi(gbl.sell_card_type)==135||atoi(gbl.sell_card_type)==136||atoi(gbl.sell_card_type)==139||atoi(gbl.sell_card_type)==142||atoi(gbl.sell_card_type)==143||atoi(gbl.sell_card_type)==144){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"电信充值付费卡");

	}
	if(atoi(gbl.sell_card_type)==807||atoi(gbl.sell_card_type)==808||atoi(gbl.sell_card_type)==809){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"众网游戏一卡通");

	}
	if(atoi(gbl.sell_card_type)==810||atoi(gbl.sell_card_type)==811||atoi(gbl.sell_card_type)==812||atoi(gbl.sell_card_type)==813||atoi(gbl.sell_card_type)==814){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"天下通卡");	
	}

	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==205||atoi(gbl.sell_card_type)==206){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"移动充值卡");

	}
	if(atoi(gbl.sell_card_type)==310||atoi(gbl.sell_card_type)==308||atoi(gbl.sell_card_type)==309){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"联通充值卡");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"小灵通充值卡");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"宽带充值卡");

	}
	if(atoi(gbl.sell_card_type)==128||atoi(gbl.sell_card_type)==129||atoi(gbl.sell_card_type)==130){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"电信充值付费卡");

	}
	if(atoi(gbl.sell_card_type)==804||atoi(gbl.sell_card_type)==805||atoi(gbl.sell_card_type)==806){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"众网游戏一卡通");

	}

	bbzero(amoMny);  //金额
	CTools::chgToMny(gbl.amount, amoMny);

	//填写打印模板
	memset(&prtTemp,0,sizeof(prtTemp));
	if(!memcmp(gbl.fee_type2,"0182",4)){
		sprintf(prtTemp.stan,"#%s",gbl.phone_no);
	}
    else
	    memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);//读入系统参考号
	CTools::formatDate(yymmdd, prtTemp.date);	//日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");	//计数
	strcpy(prtTemp.amo, amoMny);			//实扣
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//合计
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//转换为大写金额
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//不显示完整银行卡号
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);		//终端流水
	strcpy(prtTemp.pos, gbl.pos_no);		//终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if (memcmp(gbl.sell_card_type,"10",2)==0)
	{

		sprintf(prtTemp.phone, "%s", gbl.card_serial);//卡号
	}
	else
	{
		sprintf(prtTemp.phone, "%s(卡序列号)", gbl.card_serial);//卡号

	}

	if (atoi(gbl.fee_type)==245)
	{
		strcpy(prtTemp.type, gbl.xmcard_name);
		strcpy(show_buff,gbl.xmcard_name);
		strcat(show_buff,"\n");
	}else{
		strcpy(prtTemp.type, m_type);  //类型
		strcpy(show_buff,m_type);
		strcat(show_buff,"\n");
	}
	//填写打印明细
	
	prtTemp.n=0;
	if (atoi(gbl.fee_type)==245)
	{
		sprintf(prtTemp.content, "卡流水号:%s", xm_serial);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "面值:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.请您注意面额，尽快充值\n");
		strcat(prtTemp.content, "2.充值方法：登陆相关游戏官方网站按提示充值\n");
		strcat(prtTemp.content, "3.本券售出后不提供退换、挂失服务\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");

		strcat(show_buff,"卡流水号:");
		strcat(show_buff,xm_serial);
		strcat(show_buff,"\n");
		strcat(show_buff,"卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff,"\n");
		strcat(show_buff,"密码:");
		strcat(show_buff,gbl.card_pwd);
		strcat(show_buff,"\n");

		prtTemp.n += 8;
	}
	else if (atoi(gbl.fee_type)==153||atoi(gbl.fee_type)==182){ //东莞售卡及售卡补打
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
        sprintf(prtTemp.content, "面值:");
		
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");

	//	prtTemp.n++;
	}
	else if (atoi(gbl.sell_card_type)==900)
	{
		sprintf(prtTemp.content, "有效日期:%s", eff_yymmdd);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
	else
	{
		sprintf(prtTemp.content, "有效日期:%s", eff_yymmdd);
		strcat(prtTemp.content, "面值:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
		

	switch(atoi(gbl.sell_card_type))
	{
	case 200:	//移动
	case 201:
	case 205:
	case 206:
	case 207:
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请您注意面额和有效期，尽快充值\n");
		strcat(prtTemp.content, "2.充值方法：中国移动13800138000\n");
		strcat(prtTemp.content, "3.本券售出后不提供退换、挂失服务\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 300:	//联通
	case 301:
	case 311:
	case 308:
	case 309:
	case 310:
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.请您注意面额和有效期，尽快充值\n");
		strcat(prtTemp.content, "2.密码19位的卡充值方法：10011");
		strcat(prtTemp.content, "  其他卡充值方法：10012\n");
		strcat(prtTemp.content, "3.本券售出后不提供退换、挂失服务\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");

		strcat(show_buff, "卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 112:	//付费易卡
	case 113:
		break;
	case 109:	//小灵通充值卡
	case 110:	
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.本券仅限于广州市内对预付费小灵通充值使用\n");
		strcat(prtTemp.content, "2.请在有效期内使用本机拨打免费充值电话96338");
		strcat(prtTemp.content, "按语音提示操作。客户热线：10000\n");
		strcat(prtTemp.content, "3.本卷不挂失，请立即充值或妥善保管，失密自负\n");
		strcat(prtTemp.content, "4.广东天讯电信科技有限公司拥有对本券的最终解");
		strcat(prtTemp.content, "释权\n");

		strcat(show_buff, "卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 7;
		break;

	case 136:	//电信充值付费卡
	case 139:
	case 135:
	case 142:
	case 143:
	case 144:
	case 128:
	case 129:
	case 130:
	case 137:
		strcat(prtTemp.content, "帐号:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.本卡适用于广东省电信用户充值付费，充值请拨打11888\n");
		strcat(prtTemp.content, "2.本券不挂失，请妥善保管，失密自负。客服热线：10000\n");
		strcat(prtTemp.content, "“交费易”客服热线0400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "帐号:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 800:	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本卷不挂失，请妥善保管，失密自负\n");
		strcat(prtTemp.content, "有疑问请拨打400-80-11888咨询\n");
		strcat(prtTemp.content, "3.使用方法：登陆http://sde.game.sohu.com/按提示充值\n");		
		prtTemp.n +=8;

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 801:	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本卷不挂失，请妥善保管，失密自负\n");
		strcat(prtTemp.content, "有疑问请拨打400-80-11888咨询\n");
		strcat(prtTemp.content, "3.使用方法：登陆http://sde.game.sohu.com/按提示充值\n");
		prtTemp.n += 8;

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 810:
	case 811:
	case 812:
	case 813:
	case 814:
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本卷不挂失，请妥善保管，失密自负\n");
		strcat(prtTemp.content, "有疑问请拨打400-80-11888咨询\n");
		strcat(prtTemp.content, "3.使用方法：请登陆天下通网站 www.txtong.com.cn进行充值\n");		
		prtTemp.n += 8;

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;
	case 807:
	case 808:
	case 809:
	case 804:
	case 805:
	case 806:
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content,"温馨提示：\n    请在发票打印后立即登陆http://www.dg8888.com网站，根据提示和说明输入卡号和密码进行充值.客服热线：400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "卡号：");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码：");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 127://东莞小灵通售卡
	case 122:
    case 123:
		strcat(prtTemp.content, "密码：");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "免费充值电话：96338\n");
		strcat(prtTemp.content,"温馨提示：\n    请在电子券票据打印后24小时内及时充值\n");
		prtTemp.n += 6;

		strcat(show_buff, "密码：");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

    case 124://东莞宽带售卡
	case 125:
    case 126:
		strcat(prtTemp.content, "密码：");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "充值网址：http://www.dgkd.cn\n");
		strcat(prtTemp.content,"温馨提示：\n    请在电子券票据打印后24小时内及时充值\n");
		prtTemp.n += 6;

		strcat(show_buff, "密码：");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;
		
	case 900:
		strcat(prtTemp.content, "帐号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.本券可通过下列方式兑换：\n拨打114占位\n或登陆http://gd.118114.cn/movie\n或天翼手机登陆院线通软件\n");
		strcat(prtTemp.content, "2.本券不挂失，请妥善保管，失密自负。\n");
		prtTemp.n += 7;

		strcat(show_buff, "帐号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	}

	strcat(show_buff,"请按【确认】打印");
	if (atoi(gbl.dPosInfo.prmVer)<9000)
	{
		int iSelect;
		TS_OutPutMsg(gbl.dPosInfo,show_buff,0,iSelect);
	}

//	for(i=0;i<9-n;i++)
//		strcat(content,"\n");
    if (atoi(gbl.fee_type)==153||atoi(gbl.fee_type)==182){
       	strcat(prtTemp.content,"\n");
		prtTemp.n += 1;
		strcat(prtTemp.content,advertisement);
	    prtTemp.n += adv_count;
	}

	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n ++ ;

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType );	
	
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func_safe((char*)pMsg,iPrintLen);
	wrLog("【print_func_safe】iRet = %d", iRet);
	return iRet;
}

//售卡，改造
//售卡打印明细——2006-11-1,lihk
int CPrint::print_card_detail(CPack pack,const char *type)	
{
	int iPrintLen, iRet, i;
	BYTE pMsg[1024];
	char cardMny[15],amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	char eff_yymmdd[8+1]; //有效日期
	char xm_serial[30+1], card_no[20+1], card_pwd[20+1], m_type[20+1];
	char show_buff[1024];
	PrintTemp prtTemp;
	int iSelect;

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);

	bbzero(m_type);
	strcpy(m_type, type);
	
	//提取打印数据
	bbzero(gbl.card_num);
	bbzero(gbl.card_pwd);
	bbzero(gbl.card_serial);
	bbzero(gbl.amount);	
	bbzero(eff_yymmdd);
	bbzero(xm_serial);
	bbzero(card_pwd);
	bbzero(card_no);
	if( rdField63_3(pack,gbl.card_num, gbl.card_pwd, gbl.card_serial, gbl.amount, eff_yymmdd,xm_serial) )
		return -1;
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(cardMny);	//充值面值
	CTools::chgToMny(gbl.card_amount, cardMny);
	bbzero(show_buff);
	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==200||atoi(gbl.sell_card_type)==201){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"移动充值卡");
		
	}
	if(atoi(gbl.sell_card_type)==311||atoi(gbl.sell_card_type)==300||atoi(gbl.sell_card_type)==301){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"联通充值卡");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"小灵通充值卡");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"宽带充值卡");

	}
	if(atoi(gbl.sell_card_type)==135||atoi(gbl.sell_card_type)==136||atoi(gbl.sell_card_type)==139||atoi(gbl.sell_card_type)==142||atoi(gbl.sell_card_type)==143||atoi(gbl.sell_card_type)==144){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"电信充值付费卡");

	}
	if(atoi(gbl.sell_card_type)==807||atoi(gbl.sell_card_type)==808||atoi(gbl.sell_card_type)==809){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"众网游戏一卡通");

	}
	if(atoi(gbl.sell_card_type)==810||atoi(gbl.sell_card_type)==811||atoi(gbl.sell_card_type)==812||atoi(gbl.sell_card_type)==813||atoi(gbl.sell_card_type)==814){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"天下通卡");	
	}

	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==205||atoi(gbl.sell_card_type)==206){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"移动充值卡");

	}
	if(atoi(gbl.sell_card_type)==310||atoi(gbl.sell_card_type)==308||atoi(gbl.sell_card_type)==309){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"联通充值卡");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"小灵通充值卡");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"宽带充值卡");

	}
	if(atoi(gbl.sell_card_type)==128||atoi(gbl.sell_card_type)==129||atoi(gbl.sell_card_type)==130){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"电信充值付费卡");

	}
	if(atoi(gbl.sell_card_type)==804||atoi(gbl.sell_card_type)==805||atoi(gbl.sell_card_type)==806){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"众网游戏一卡通");

	}

	if (memcmp(gbl.sell_card_type,"10",2)==0) //如果开头是10 代表的是翼支付卡
	{
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"翼支付卡");
	}

	bbzero(amoMny);  //金额
	CTools::chgToMny(gbl.amount, amoMny);

	//显示界面信息
	if(memcmp(gbl.sell_card_type,"10",2)==0)
	{
		char showMsg[100];
		sprintf(showMsg,"交易成功!\n系统参考号:%s\n翼支付卡面值:%02d元\n卡号:%s\n密码:%s\n有效期:%s",gbl.stan_no,atoi(gbl.amount)/100,gbl.card_num,gbl.card_pwd,eff_yymmdd);
		iRet=TS_OutPutMsg(gbl.dPosInfo,showMsg,0,iSelect);
		if (iRet!=0 || iSelect!=0)
		{
			return 0;
		}

	}
	
	//填写打印模板
	memset(&prtTemp,0,sizeof(prtTemp));
	if(!memcmp(gbl.fee_type2,"0182",4)){
		sprintf(prtTemp.stan,"#%s",gbl.phone_no);
	}
    else
	    memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);//读入系统参考号
	CTools::formatDate(yymmdd, prtTemp.date);	//日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");	//计数
	strcpy(prtTemp.amo, amoMny);			//实扣
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//合计
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//转换为大写金额
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//不显示完整银行卡号
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);		//终端流水
	strcpy(prtTemp.pos, gbl.pos_no);		//终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if (memcmp(gbl.sell_card_type,"10",2)==0)
	{

		sprintf(prtTemp.phone, "%s", gbl.card_serial);//卡号
	}
	else
	{
	
		sprintf(prtTemp.phone, "%s(卡序列号)", gbl.card_serial);//卡号
	}

	if (atoi(gbl.fee_type)==245)
	{
		strcpy(prtTemp.type, gbl.xmcard_name);
		strcpy(show_buff,gbl.xmcard_name);
		strcat(show_buff,"\n");
	}else{
		strcpy(prtTemp.type, m_type);  //类型
		strcpy(show_buff,m_type);
		strcat(show_buff,"\n");
	}
	//填写打印明细
	
	prtTemp.n=0;
	if (atoi(gbl.fee_type)==245)
	{
		sprintf(prtTemp.content, "卡流水号:%s", xm_serial);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "面值:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.请您注意面额，尽快充值\n");
		strcat(prtTemp.content, "2.充值方法：登陆相关游戏官方网站按提示充值\n");
		strcat(prtTemp.content, "3.本券售出后不提供退换、挂失服务\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");

		strcat(show_buff,"卡流水号:");
		strcat(show_buff,xm_serial);
		strcat(show_buff,"\n");
		strcat(show_buff,"卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff,"\n");
		strcat(show_buff,"密码:");
		strcat(show_buff,gbl.card_pwd);
		strcat(show_buff,"\n");

		prtTemp.n += 8;
	}
	else if (atoi(gbl.fee_type)==153||atoi(gbl.fee_type)==182){ //东莞售卡及售卡补打
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
        sprintf(prtTemp.content, "面值:");
		
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");

	//	prtTemp.n++;
	}
	else if (atoi(gbl.sell_card_type)==900)
	{
		sprintf(prtTemp.content, "有效日期:%s", eff_yymmdd);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
	else
	{
		sprintf(prtTemp.content, "有效日期:%s", eff_yymmdd);
		strcat(prtTemp.content, "面值:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
		

	switch(atoi(gbl.sell_card_type))
	{
	case 200:	//移动
	case 201:
	case 205:
	case 206:
	case 207:
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请您注意面额和有效期，尽快充值\n");
		strcat(prtTemp.content, "2.充值方法：中国移动13800138000\n");
		strcat(prtTemp.content, "3.本券售出后不提供退换、挂失服务\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 300:	//联通
	case 301:
	case 311:
	case 308:
	case 309:
	case 310:
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.请您注意面额和有效期，尽快充值\n");
		strcat(prtTemp.content, "2.密码19位的卡充值方法：10011");
		strcat(prtTemp.content, "  其他卡充值方法：10012\n");
		strcat(prtTemp.content, "3.本券售出后不提供退换、挂失服务\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");

		strcat(show_buff, "卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 112:	//付费易卡
	case 113:
		break;
	case 109:	//小灵通充值卡
	case 110:	
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.本券仅限于广州市内对预付费小灵通充值使用\n");
		strcat(prtTemp.content, "2.请在有效期内使用本机拨打免费充值电话96338");
		strcat(prtTemp.content, "按语音提示操作。客户热线：10000\n");
		strcat(prtTemp.content, "3.本卷不挂失，请立即充值或妥善保管，失密自负\n");
		strcat(prtTemp.content, "4.广东天讯电信科技有限公司拥有对本券的最终解");
		strcat(prtTemp.content, "释权\n");

		strcat(show_buff, "卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 7;
		break;

	case 136:	//电信充值付费卡
	case 139:
	case 135:
	case 142:
	case 143:
	case 144:
	case 128:
	case 129:
	case 130:
	case 137:
		strcat(prtTemp.content, "帐号:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.本卡适用于广东省电信用户充值付费，充值请拨打11888\n");
		strcat(prtTemp.content, "2.本券不挂失，请妥善保管，失密自负。客服热线：10000\n");
		strcat(prtTemp.content, "“交费易”客服热线0400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "帐号:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 800:	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本卷不挂失，请妥善保管，失密自负\n");
		strcat(prtTemp.content, "有疑问请拨打400-80-11888咨询\n");
		strcat(prtTemp.content, "3.使用方法：登陆http://sde.game.sohu.com/按提示充值\n");		
		prtTemp.n +=8;

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 801:	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本卷不挂失，请妥善保管，失密自负\n");
		strcat(prtTemp.content, "有疑问请拨打400-80-11888咨询\n");
		strcat(prtTemp.content, "3.使用方法：登陆http://sde.game.sohu.com/按提示充值\n");
		prtTemp.n += 8;

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 810:
	case 811:
	case 812:
	case 813:
	case 814:
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本卷不挂失，请妥善保管，失密自负\n");
		strcat(prtTemp.content, "有疑问请拨打400-80-11888咨询\n");
		strcat(prtTemp.content, "3.使用方法：请登陆天下通网站 www.txtong.com.cn进行充值\n");		
		prtTemp.n += 8;

		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;
	case 807:
	case 808:
	case 809:
	case 804:
	case 805:
	case 806:
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content,"温馨提示：\n    请在发票打印后立即登陆http://www.dg8888.com网站，根据提示和说明输入卡号和密码进行充值.客服热线：400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "卡号：");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码：");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 127://东莞小灵通售卡
	case 122:
    case 123:
		strcat(prtTemp.content, "密码：");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "免费充值电话：96338\n");
		strcat(prtTemp.content,"温馨提示：\n    请在电子券票据打印后24小时内及时充值\n");
		prtTemp.n += 6;

		strcat(show_buff, "密码：");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

    case 124://东莞宽带售卡
	case 125:
    case 126:
		strcat(prtTemp.content, "密码：");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "充值网址：http://www.dgkd.cn\n");
		strcat(prtTemp.content,"温馨提示：\n    请在电子券票据打印后24小时内及时充值\n");
		prtTemp.n += 6;

		strcat(show_buff, "密码：");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 1001: //翼支付卡10元
	case 1002://翼支付卡20元
	case 1003:
	case 1005:
	case 1011:
	case 1012:
	case 1013:
	case 1015:
	case 1021:
		
		strcat(prtTemp.content, "卡号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");	
		strcat(prtTemp.content, " 密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.请在有效期内使用\n");
		strcat(prtTemp.content, "2.本券可为翼支付帐户充值\n");
		strcat(prtTemp.content, "3.本券不挂失，请妥善保管，失密自负\n");
		strcat(show_buff, "卡号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");
		
		prtTemp.n += 7;
		break;

	case 900:
		strcat(prtTemp.content, "帐号:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "密码:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.本券可通过下列方式兑换：\n拨打114占位\n或登陆http://gd.118114.cn/movie\n或天翼手机登陆院线通软件\n");
		strcat(prtTemp.content, "2.本券不挂失，请妥善保管，失密自负。\n");
		prtTemp.n += 7;

		strcat(show_buff, "帐号:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "密码:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	}

	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;

	strcat(show_buff,"请按【确认】打印");
	if (atoi(gbl.dPosInfo.prmVer)<9000)
	{
		int iSelect;
		TS_OutPutMsg(gbl.dPosInfo,show_buff,0,iSelect);
	}

//	for(i=0;i<9-n;i++)
//		strcat(content,"\n");
    if (atoi(gbl.fee_type)==153||atoi(gbl.fee_type)==182){
       	strcat(prtTemp.content,"\n");
		prtTemp.n += 1;
		strcat(prtTemp.content,advertisement);
	    prtTemp.n += adv_count;
	}

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType );	
	
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func_safe((char*)pMsg,iPrintLen);
	wrLog("【print_func_safe】iRet = %d", iRet);
	return iRet;
}

//end of 售卡 改造
//打印天天明细
int CPrint::print_mer_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------提取打印数据------------------*/
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //金额
	CTools::chgToMny(gbl.amount, amoMny);	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	strcpy(prtTemp.name, type);					     //用户名位置
	sprintf(prtTemp.phone, "%s(订单号)", gbl.order_no);	 //电话号码位置
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//打印内容
	prtTemp.n=0;
	sprintf(prtTemp.content,"\n洗衣费： %s\n\n", prtTemp.total);
	prtTemp.n+=3;
	strcat(prtTemp.content,"本单据仅为交易存根和客户查询受理之依据。（不作顾客报销凭证）\n");
	prtTemp.n+=2;
    


	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//改造函数
//打印天天明细
int CPrint::print_mer_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------提取打印数据------------------*/
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //金额
	CTools::chgToMny(gbl.amount, amoMny);	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	strcpy(prtTemp.name, type);					     //用户名位置
	sprintf(prtTemp.phone, "%s(订单号)", gbl.order_no);	 //电话号码位置
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//打印内容
	prtTemp.n=0;
	sprintf(prtTemp.content,"\n洗衣费： %s\n\n", prtTemp.total);
	prtTemp.n+=3;
	strcat(prtTemp.content,"本单据仅为交易存根和客户查询受理之依据。（不作顾客报销凭证）\n");
	prtTemp.n+=2;
    
	
	
	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

//end of 改造函数
//打印珠江宽频优惠明细, 2008-01-31 by lihk
int CPrint::print_CATVDiscount_detail(const char *type)		
{
	int iPrintLen, iRet, i, j, k, flag, len;
	BYTE pMsg[1024];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	char periods[100], temp[100], amoMny[15],mny_buff[12+1];
	char mny[20][MAXAMOLEN];
	char len_buff[2+1];
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.fee_type: [%s]", gbl.fee_type);
	
	iRet = rdField63(gbl, 0);	//无欠费返回0，有欠费返回1
	//wrLog("rdField63返回值: [%d]", iRet);
	if(iRet != 0 && iRet != 1)
	{
		//wrLog("rdField63返回错误");
		return -1;
	}
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");	//张数
	strcpy(prtTemp.type, type);	//交费类型
	strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);	//用户名
	sprintf(prtTemp.phone, "%s（%s）", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
	//银行卡号
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)
		prtTemp.card[i] = '*';
	strcpy(prtTemp.amo, amoMny);					 //实扣
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*-----------------------------------------------*/
	flag = 1;	//循环打印标识
	while(flag)
	{
		if(iRet == 1 || flag == 2)//iRet=0, 无欠费明细直接打印续费金额; flag=2, 打完欠费后打印续费金额
		{
			len = 0;
			if(flag == 2)
			{
				bbzero(temp);
				memcpy(temp, pack.FIELD63, 3);	//3位明细长
				len = atoi(temp);
			}
			len += 3;
			bbzero(mny_buff);
			bbzero(prtTemp.total);
			memcpy(mny_buff,pack.FIELD63+len,12);
			len += 12;
			CTools::chgToMny(mny_buff,prtTemp.total,0);
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
			//打印内容
			prtTemp.n=0;
			sprintf(prtTemp.content, "续费金额：%s元\n",prtTemp.total);
			prtTemp.n++;
			bbzero(len_buff);
			memcpy(len_buff, pack.FIELD63+len, 2); //周期长度
			len += 2;
			bbzero(periods);
			memcpy(periods,pack.FIELD63+len,atoi(len_buff));
			wrLog("优惠周期：[%s]", periods);
			sprintf(prtTemp.content+strlen(prtTemp.content), "优惠周期：%s\n", periods);
			prtTemp.n++;
			flag = 0;

			bbzero(pMsg);
			wrLog("开始打印……");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}
		else if(iRet == 0)	//有欠费明细
		{
			//获取合计金额
			memset(mny,0,sizeof(mny));
			j = 0;	//金额个数
			for(i=0; i<gbl.detail.nMonCnt; i++)
			{
				for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
					strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
			}
			CTools::getTalAmo(mny, prtTemp.total, j);;
			//转换为大写金额
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
			//分别打印各个交费时段
			prtTemp.n=0;
			for(j = 0; j<gbl.detail.nMonCnt; j++)
			{
				memset(temp,0,sizeof(temp));
				sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//左对齐
				strcat(prtTemp.content,temp);
				for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//最多打9行明细项（2列）
				{
					if(i)	//除第一项外都要缩进
						strcat(prtTemp.content,"            ");
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//左对齐
					strcat(prtTemp.content,temp);
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
					strcat(prtTemp.content,temp);
					strcat(prtTemp.content,"\n");
					prtTemp.n++;
				}
			}	//end for(j = 0;
			flag = 2;	//还要打印续费金额

			strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
			prtTemp.n++;

			bbzero(pMsg);
			wrLog("开始打印……");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}	//end else if(iRet == 1)
		else
			return -1;
	}
	
	return iRet;
}

//改造函数
//打印珠江宽频优惠明细, 2008-01-31 by lihk
int CPrint::print_CATVDiscount_detail(CPack pack,const char *type)		
{
	int iPrintLen, iRet, i, j, k, flag, len;
	BYTE pMsg[1024];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	char periods[100], temp[100], amoMny[15],mny_buff[12+1];
	char mny[20][MAXAMOLEN];
	char len_buff[2+1];
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.fee_type: [%s]", gbl.fee_type);
	
	iRet = rdField63(gbl, 0);	//无欠费返回0，有欠费返回1
	//wrLog("rdField63返回值: [%d]", iRet);
	if(iRet != 0 && iRet != 1)
	{
		//wrLog("rdField63返回错误");
		return -1;
	}
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");	//张数
	strcpy(prtTemp.type, type);	//交费类型
	strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);	//用户名
	sprintf(prtTemp.phone, "%s（%s）", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
	//银行卡号
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)
		prtTemp.card[i] = '*';
	strcpy(prtTemp.amo, amoMny);					 //实扣
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*-----------------------------------------------*/
	flag = 1;	//循环打印标识
	while(flag)
	{
		if(iRet == 1 || flag == 2)//iRet=0, 无欠费明细直接打印续费金额; flag=2, 打完欠费后打印续费金额
		{
			len = 0;
			if(flag == 2)
			{
				bbzero(temp);
				memcpy(temp, pack.FIELD63, 3);	//3位明细长
				len = atoi(temp);
			}
			len += 3;
			bbzero(mny_buff);
			bbzero(prtTemp.total);
			memcpy(mny_buff,pack.FIELD63+len,12);
			len += 12;
			CTools::chgToMny(mny_buff,prtTemp.total,0);
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
			//打印内容
			prtTemp.n=0;
			sprintf(prtTemp.content, "续费金额：%s元\n",prtTemp.total);
			prtTemp.n++;
			bbzero(len_buff);
			memcpy(len_buff, pack.FIELD63+len, 2); //周期长度
			len += 2;
			bbzero(periods);
			memcpy(periods,pack.FIELD63+len,atoi(len_buff));
			wrLog("优惠周期：[%s]", periods);
			sprintf(prtTemp.content+strlen(prtTemp.content), "优惠周期：%s\n", periods);
			prtTemp.n++;
			flag = 0;

			bbzero(pMsg);
			wrLog("开始打印……");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}
		else if(iRet == 0)	//有欠费明细
		{
			//获取合计金额
			memset(mny,0,sizeof(mny));
			j = 0;	//金额个数
			for(i=0; i<gbl.detail.nMonCnt; i++)
			{
				for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
					strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
			}
			CTools::getTalAmo(mny, prtTemp.total, j);;
			//转换为大写金额
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
			//分别打印各个交费时段
			prtTemp.n=0;
			for(j = 0; j<gbl.detail.nMonCnt; j++)
			{
				memset(temp,0,sizeof(temp));
				sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//左对齐
				strcat(prtTemp.content,temp);
				for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//最多打9行明细项（2列）
				{
					if(i)	//除第一项外都要缩进
						strcat(prtTemp.content,"            ");
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//左对齐
					strcat(prtTemp.content,temp);
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
					strcat(prtTemp.content,temp);
					strcat(prtTemp.content,"\n");
					prtTemp.n++;
				}
			}	//end for(j = 0;
			flag = 2;	//还要打印续费金额

			strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
			prtTemp.n++;

			bbzero(pMsg);
			wrLog("开始打印……");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}	//end else if(iRet == 1)
		else
			return -1;
	}
	
	return iRet;
}

//end of 改造函数

//打印有线明细,2006-11-21 by lihk
int CPrint::print_CATV_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15], mny[20][MAXAMOLEN], temp[20];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen, iRet, i, j, k;
	PrintTemp prtTemp;   //打印模板

	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);	
	//明细
	rdField63(gbl, 0);
	if (memcmp((char*)pack.FIELD63,"000",3)==0)
	{
		return 0;
	}
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count,"第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);						 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
//	strcpy(prtTemp.name, gbl.user_name);			 //用户名
	strcpy(prtTemp.amo, amoMny);					 //实扣
//	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//获取合计金额
	memset(mny,0,sizeof(mny));
	j = 0;	//金额个数
	for(i=0; i<gbl.detail.nMonCnt; i++)
	{
		for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
			strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
	}
	CTools::getTalAmo(mny, prtTemp.total, j);;
	//转换为大写金额
	bbzero(prtTemp.capsTotal);
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
	//用户名、号码
	switch(atoi(gbl.fee_type))				
	{
	case 100:	//珠江数码
	case 112:	//珠江宽频
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s（%s）", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//有线电视
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[0].user_name);
		strcpy(prtTemp.phone, gbl.CATV_manual_num);
		break;
	}
	//分别打印各个交费时段
	prtTemp.n=0;
	for(j = 0; j<gbl.detail.nMonCnt; j++)
	{
		memset(temp,0,sizeof(temp));
		sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//左对齐
		strcat(prtTemp.content,temp);
		for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//最多打9行明细项（2列）
		{
			if(i)	//除第一项外都要缩进
				strcat(prtTemp.content,"            ");
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//左对齐
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			strcat(prtTemp.content,"\n");
			prtTemp.n++;
		}
	}


	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//有线电视改造函数
//打印有线明细,2006-11-21 by lihk
int CPrint::print_CATV_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15], mny[20][MAXAMOLEN], temp[20];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen, iRet, i, j, k;
	PrintTemp prtTemp;   //打印模板

	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	//明细
	rdField63(gbl, 0); 

	if (memcmp((char*)pack.FIELD63,"000",3)==0)
	{
		//不欠费 在此打印
		iRet =  print_pre_detail(pack,type); //cut by liuxiao
		return iRet;
	}

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count,"第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);						 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
//	strcpy(prtTemp.name, gbl.user_name);			 //用户名
	strcpy(prtTemp.amo, amoMny);					 //实扣
//	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//获取合计金额
	memset(mny,0,sizeof(mny));
	j = 0;	//金额个数
	for(i=0; i<gbl.detail.nMonCnt; i++)
	{
		for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
			strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
	}
	CTools::getTalAmo(mny, prtTemp.total, j);;
	//转换为大写金额
	bbzero(prtTemp.capsTotal);
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
	//用户名、号码
	switch(atoi(gbl.fee_type))				
	{
	case 100:	//珠江数码
	case 112:	//珠江宽频
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s（%s）", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//有线电视
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[0].user_name);
		strcpy(prtTemp.phone, gbl.CATV_manual_num);
		break;
	}
	//分别打印各个交费时段
	prtTemp.n=0;
	for(j = 0; j<gbl.detail.nMonCnt; j++)
	{
		memset(temp,0,sizeof(temp));
		sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//左对齐
		strcat(prtTemp.content,temp);
		for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//最多打9行明细项（2列）
		{
			if(i)	//除第一项外都要缩进
				strcat(prtTemp.content,"            ");
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//左对齐
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			strcat(prtTemp.content,"\n");
			prtTemp.n++;
		}
	}

	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//end of 有线电视改造函数

//picc打印明细——2007-4-24,lihk
int CPrint::print_PICC_detail(const char *type)	
{
	int iPrintLen, i, iRet;
	BYTE pMsg[1024];
	char temp[200], amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取打印内容
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny);  //金额
	CTools::chgToMny(gbl.amount, amoMny);
	//填写打印模板
	memset(&prtTemp,0,sizeof(prtTemp));
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);		//系统参考号
	CTools::formatDate(yymmdd, prtTemp.date);				//日期
	CTools::formatTime(hhmmss, prtTemp.tim);				//时间
	strcpy(prtTemp.count, "第1张/共1张");				//计数
	strcpy(prtTemp.amo, amoMny);						//实扣
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//合计
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//转换为大写金额
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//不显示完整银行卡号
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	sprintf(prtTemp.phone, "%s(卡序列号)", gbl.card_serial);	//卡号
	
	strcpy(prtTemp.type, type);							//类型
	
	wrLog("开始打印……");
	//分别打印各个交费时段
	prtTemp.n=0;
	bbzero(temp);
	sprintf(prtTemp.content, "车主身份证号码：%s\n车牌号：%s\n联系电话：%s\n投保金额：%s\n", gbl.CATV_id_card, gbl.car_no, gbl.phone_no, amoMny);
	prtTemp.n = 4;
	strcat(prtTemp.content, "保险公司将于交费次日起三个工作日内审查您的投\n");	
	strcat(prtTemp.content, "保申请，与您电话确认投保结果并邮寄正式保险单\n");
	strcat(prtTemp.content, "PICC电话：83356783转1112，1110，1109，1113\n");
	prtTemp.n += 3;
    


	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	iRet = print_func((char*)pMsg,iPrintLen);
	wrLog("【print_func】iRet = %d", iRet);
	return iRet;
}

//改造函数
//picc打印明细——2007-4-24,lihk
int CPrint::print_PICC_detail(CPack pack,const char *type)	
{
	int iPrintLen, i, iRet;
	BYTE pMsg[1024];
	char temp[200], amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取打印内容
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny);  //金额
	CTools::chgToMny(gbl.amount, amoMny);
	//填写打印模板
	memset(&prtTemp,0,sizeof(prtTemp));
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);		//系统参考号
	CTools::formatDate(yymmdd, prtTemp.date);				//日期
	CTools::formatTime(hhmmss, prtTemp.tim);				//时间
	strcpy(prtTemp.count, "第1张/共1张");				//计数
	strcpy(prtTemp.amo, amoMny);						//实扣
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//合计
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//转换为大写金额
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//不显示完整银行卡号
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	sprintf(prtTemp.phone, "%s(卡序列号)", gbl.card_serial);	//卡号
	strcpy(prtTemp.type, type);							//类型
	
	wrLog("开始打印……");
	//分别打印各个交费时段
	prtTemp.n=0;
	bbzero(temp);
	sprintf(prtTemp.content, "车主身份证号码：%s\n车牌号：%s\n联系电话：%s\n投保金额：%s\n", gbl.CATV_id_card, gbl.car_no, gbl.phone_no, amoMny);
	prtTemp.n = 4;
	strcat(prtTemp.content, "保险公司将于交费次日起三个工作日内审查您的投\n");	
	strcat(prtTemp.content, "保申请，与您电话确认投保结果并邮寄正式保险单\n");
	strcat(prtTemp.content, "PICC电话：83356783转1112，1110，1109，1113\n");
	prtTemp.n += 3;
    
	
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	iRet = print_func((char*)pMsg,iPrintLen);
	wrLog("【print_func】iRet = %d", iRet);
	return iRet;
}

//end of 改造函数

//打印电影折扣票凭据
int CPrint::print_film_detail(const char *type)
{
	BYTE pMsg[1024];
	char film_no[31], amoMny[15], temp[5];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------提取打印数据------------------*/
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //金额
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type); 					 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	strcpy(prtTemp.name, gbl.user_name);			 //用户名
//	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.month, "本收据不作为报销凭证");	 //交费月位置
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//打印内容
	prtTemp.n=0;
	bbzero(temp);
	memcpy(temp, pack.FIELD63, 2);
	bbzero(film_no);
	memcpy(film_no, pack.FIELD63+2, atoi(temp));

	sprintf(prtTemp.content,"兑换号码：%s\n",film_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "证件号码：%s\n", gbl.id_num);
	sprintf(prtTemp.content+strlen(prtTemp.content), "手机号码：%s\n", gbl.phone_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "购票张数：%d\n", gbl.num);
	prtTemp.n += 4;
	strcat(prtTemp.content, "本收据不作为领取电影票的唯一凭证\n");
	strcat(prtTemp.content, "1.请在青宫电影院凭此兑换号码或短信票号领票。\n");
	strcat(prtTemp.content, "2.该订购电影票可在青宫电影院观看任何场次电影");
	strcat(prtTemp.content, "(除圣诞节、情人节外)。\n");
	strcat(prtTemp.content, "3.有效期是自密码下发日起三个月内。\n");
	prtTemp.n += 4;
	

	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//改造函数
//打印电影折扣票凭据
int CPrint::print_film_detail(CPack pack,const char *type)
{
	BYTE pMsg[1024];
	char film_no[31], amoMny[15], temp[5];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------提取打印数据------------------*/
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //金额
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type); 					 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	strcpy(prtTemp.name, gbl.user_name);			 //用户名
	//	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.month, "本收据不作为报销凭证");	 //交费月位置
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//打印内容
	prtTemp.n=0;
	bbzero(temp);
	memcpy(temp, pack.FIELD63, 2);
	bbzero(film_no);
	memcpy(film_no, pack.FIELD63+2, atoi(temp));
	
	sprintf(prtTemp.content,"兑换号码：%s\n",film_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "证件号码：%s\n", gbl.id_num);
	sprintf(prtTemp.content+strlen(prtTemp.content), "手机号码：%s\n", gbl.phone_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "购票张数：%d\n", gbl.num);
	prtTemp.n += 4;
	strcat(prtTemp.content, "本收据不作为领取电影票的唯一凭证\n");
	strcat(prtTemp.content, "1.请在青宫电影院凭此兑换号码或短信票号领票。\n");
	strcat(prtTemp.content, "2.该订购电影票可在青宫电影院观看任何场次电影");
	strcat(prtTemp.content, "(除圣诞节、情人节外)。\n");
	strcat(prtTemp.content, "3.有效期是自密码下发日起三个月内。\n");
	prtTemp.n += 4;
	
	
	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

//end of 改造函数
//打印银行转帐明细
//type: 0, 普通交费返回明细（默认）；1, 上笔交易查询返回明细; 2, 日查询/历史查询
int CPrint::print_ban_detail(int type)		
{
	int iPrintLen , iRet, len = 0;
	char pMsg[1024];
	char tmp[100], tmpLen[10], rePrtBuff[15], uniStan[13], peStan[13];
	char cDate[11], cTime[9], yymmdd[9], hhmmss[7] ;
	char merName[1024], merNum[16], uniPosNum[9], sndBank[30], rcvBank[30];
	char card[51], batNum[7], serial[7], liqDate[9];
	char prc_amo[13], prc_amo_buff[13];	//手续费
	char tra_amo[13], tra_amo_buff[13];	//交易金额
	char tal_amo[13], tal_amo_buff[13];	//总金额
		
	/*-----------------------test----------------------*
	strcpy(gbl.card_no, "6228480080498205717");
	bbzero((char *)pack.FIELD12);
	bbzero((char *)pack.FIELD13);
	CTools::hex_to_byte((BYTE *)"101123", pack.FIELD12, 6);
	CTools::hex_to_byte((BYTE *)"20071203", pack.FIELD13, 8);
	bbzero((char *)pack.FIELD37);
	strcpy((char *)pack.FIELD37, "000181981540");
	bbzero((char *)pack.FIELD63);
	strcpy((char *)pack.FIELD63, "045交通银行广州分行（广州白马物业管理有限公司）\n301440183989010020099101101035800   1103015810   1962226007100004831330000010001071203101332954408000000000001000000000000");
	/*-------------------------------------------------*/
	bbzero(peStan);
	memcpy(peStan, (char *)pack.FIELD37, 12);//读入系统参考号
	memset(pMsg,0,sizeof(pMsg));
	if(type == 1)
	{
		bbzero(tmp);
		memcpy(tmp, pack.FIELD63, 2);
		len = 2 + atoi(tmp);
		bbzero(yymmdd);
		bbzero(hhmmss);
		memcpy(yymmdd, pack.FIELD63+len, 8);
		len += 8;
		memcpy(hhmmss, pack.FIELD63+len, 6);
		len += 6;
		len += 12 + 6;
		bbzero(peStan);
		memcpy(peStan, pack.FIELD63+len, 12);
		len += 12;
		len += 12+ 3;
	}
	else if(type == 2)
	{
		bbzero(tmp);
		memcpy(tmp, pack.FIELD63+6, 2);
		len = 6 + 2 + atoi(tmp);
		bbzero(yymmdd);
		bbzero(hhmmss);
		memcpy(yymmdd, pack.FIELD63+len, 8);
		len += 8;
		memcpy(hhmmss, pack.FIELD63+len, 6);
		len += 6;
		len += 12 + 6;
		bbzero(peStan);
		memcpy(peStan, pack.FIELD63+len, 12);
		len += 12;
		len += 12 + 3;
	}
	else 
	{
		len = 0;
		bbzero(yymmdd);
		bbzero(hhmmss);
		CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
		CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	}
	bbzero(cDate);	//转换日期和时间
	bbzero(cTime);
	CTools::formatDate(yymmdd, cDate);
	CTools::formatTime(hhmmss, cTime);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//wrLog("field63: [%s]", pack.FIELD63+len);
	//商户名
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 3);
	len+=3;
	bbzero(merName);
	memcpy(merName, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	/*客户名
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 3);
	len+=3;
	bbzero(custName);
	memcpy(custName, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen); */
	//商户号
	bbzero(merNum);
	memcpy(merNum, pack.FIELD63+len, 15);
	len+=15;
	//银联终端号
	bbzero(uniPosNum);
	memcpy(uniPosNum, pack.FIELD63+len, 8);
	len+=8;
	//发卡行
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(sndBank);
	memcpy(sndBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//收单行
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(rcvBank);
	memcpy(rcvBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//卡号
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	if(atoi(tmpLen) > 50)
	{
		wrLog("卡号过长");
		return -1;
	}
	bbzero(card);
	memcpy(card, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	wrLog("card: [%s]", card);
	//批次号
	bbzero(batNum);
	memcpy(batNum, pack.FIELD63+len, 6);
	len+=6;
	//流水号
	bbzero(serial);
	memcpy(serial, pack.FIELD63+len, 6);
	len+=6;
	//清算日期
	bbzero(tmp);
	memcpy(tmp, pack.FIELD63+len, 4);
	len+=4;
	bbzero(liqDate);
	CTools::formatDate(tmp, liqDate, 2);
	//12位系统参考号
	bbzero(uniStan);
	memcpy(uniStan, pack.FIELD63+len, 12);
	len+=12;	
	//金额
	bbzero(tal_amo);
	memcpy(tal_amo, pack.FIELD63+len, 12);
	len+=12;
	bbzero(tal_amo_buff);
	CTools::chgToMny(tal_amo, tal_amo_buff);
	//手续费
	bbzero(prc_amo);
	memcpy(prc_amo, pack.FIELD63+len, 12);
	bbzero(prc_amo_buff);
	CTools::chgToMny(prc_amo, prc_amo_buff);

	bbzero(rePrtBuff);
	if(type)	//重打印标志
		strcpy(rePrtBuff, "(重打印票据)");
	//交易本金
	bbzero(tra_amo);
	sprintf(tra_amo, "%012d", atoi(tal_amo)-atoi(prc_amo));
	bbzero(tra_amo_buff);
	CTools::chgToMny(tra_amo, tra_amo_buff);
	/*--------------打印内容----------------*/
	len = 0;
	sprintf((char*)pMsg+len, "【交通银行】\n商户名：%s\n商户号：%s\n", merName, merNum);
	len=strlen((char*)pMsg);
//	sprintf((char*)pMsg+len, "客户名：%s\n", custName);
//	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "终端号：%s\n操作员：01\n", uniPosNum);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "发卡行：%s\n收单行：%s\n收款卡号：%s\n", sndBank, rcvBank, card);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "交易类型：转帐\n批次号：%s\n流水号：%s\n", batNum, serial);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "时间：%s %s\n清算日期：%s\n系统参考号：%s\n银联参考号：%s\n", cDate, cTime, liqDate, peStan, uniStan);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "交易帐号：%s\n交易金额：%s\n手续费：%s\n总计：%s\n持卡人签字%s：\n", gbl.card_no, tra_amo_buff, prc_amo_buff, tal_amo_buff, rePrtBuff);
	len=strlen((char*)pMsg);
	strcat((char*)pMsg, "\n\n------------------------------\n");
	strcat((char*)pMsg, "本人确认以上交易 同意将其记入本卡帐户\n");
	strcat((char*)pMsg, "------------------------------\n");
	strcat((char*)pMsg, "______________________________\n");
	/*--------------------------------------*/
	wrLog("打印明细：\n%s", pMsg);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);

	if(!type)
		iRet = print_func((char*)pMsg, iPrintLen, 3,1);
	else
		iRet = print_func((char*)pMsg, iPrintLen, 1);

	return iRet;
}

//打印银行转帐明细
//type: 0, 普通交费返回明细（默认）；1, 上笔交易查询返回明细; 2, 日查询/历史查询
int CPrint::print_Unionban_detail(int type)
{
	int iPrintLen , iRet, len = 0;
	char pMsg[1024];
	char tmp[100], tmpLen[10], rePrtBuff[15], uniStan[13], peStan[13];
	char cDate[11], cTime[9], yymmdd[9], hhmmss[7] ;
	char merName[1024], merNum[16], uniPosNum[9], sndBank[30], rcvBank[30];
	char card[51], batNum[7], serial[7], liqDate[9];
	char prc_amo[13], prc_amo_buff[13];	//手续费
	char tra_amo[13], tra_amo_buff[13];	//交易金额
	char tal_amo[13], tal_amo_buff[13];	//总金额
	char bgstan[20+1];//后台处理号
		

	bbzero(peStan);
	memcpy(peStan, (char *)pack.FIELD37, 12);//读入系统参考号
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no, (char *)pack.FIELD37, 12);//读入系统参考号
	memset(pMsg,0,sizeof(pMsg));
	if(type == 1)
	{
		bbzero(tmp);
		memcpy(tmp, pack.FIELD63, 2);
		len = 2 + atoi(tmp);
		bbzero(yymmdd);
		bbzero(hhmmss);
		memcpy(yymmdd, pack.FIELD63+len, 8);
		len += 8;
		memcpy(hhmmss, pack.FIELD63+len, 6);
		len += 6;
		len += 12 + 6;
		bbzero(peStan);
		memcpy(peStan, pack.FIELD63+len, 12);
		len += 12;
		len += 12+ 3;
	}
	else if(type == 2)
	{
		bbzero(tmp);
		memcpy(tmp, pack.FIELD63+6, 2);
		len = 6 + 2 + atoi(tmp);
		bbzero(yymmdd);
		bbzero(hhmmss);
		memcpy(yymmdd, pack.FIELD63+len, 8);
		len += 8;
		memcpy(hhmmss, pack.FIELD63+len, 6);
		len += 6;
		len += 12 + 6;
		bbzero(peStan);
		memcpy(peStan, pack.FIELD63+len, 12);
		len += 12;
		len += 12 + 3;
	}
	else 
	{
		len = 0;
		bbzero(yymmdd);
		bbzero(hhmmss);
		CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
		CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	}
	bbzero(cDate);	//转换日期和时间
	bbzero(cTime);
	CTools::formatDate(yymmdd, cDate);
	CTools::formatTime(hhmmss, cTime);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//wrLog("field63: [%s]", pack.FIELD63+len);
	//商户名
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(merName);
	memcpy(merName, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//商户号
	bbzero(merNum);
	memcpy(merNum, pack.FIELD63+len, 15);
	len+=15;
	//银联终端号
	bbzero(uniPosNum);
	memcpy(uniPosNum, pack.FIELD63+len, 8);
	len+=8;
	//发卡行
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(sndBank);
	memcpy(sndBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//收单行
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(rcvBank);
	memcpy(rcvBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//卡号
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	if(atoi(tmpLen) > 50)
	{
		wrLog("卡号过长");
		return -1;
	}
	bbzero(card);
	memcpy(card, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	wrLog("card: [%s]", card);
	//批次号
	bbzero(batNum);
	memcpy(batNum, pack.FIELD63+len, 6);
	len+=6;
	//流水号
	bbzero(serial);
	memcpy(serial, pack.FIELD63+len, 6);
	len+=6;
	//清算日期
	bbzero(tmp);
	memcpy(tmp, pack.FIELD63+len, 4);
	len+=4;
	bbzero(liqDate);
	CTools::formatDate(tmp, liqDate, 2);
	//12位系统参考号
	bbzero(uniStan);
	memcpy(uniStan, pack.FIELD63+len, 12);
	len+=12;
	bbzero(bgstan);
	memcpy(bgstan, pack.FIELD63+len, 20);
	len+=20;
	//金额
	bbzero(tal_amo);
	memcpy(tal_amo, pack.FIELD63+len, 12);
	len+=12;
	bbzero(tal_amo_buff);
	CTools::chgToMny(tal_amo, tal_amo_buff);
	//手续费
	bbzero(prc_amo);
	memcpy(prc_amo, pack.FIELD63+len, 12);
	bbzero(prc_amo_buff);
	CTools::chgToMny(prc_amo, prc_amo_buff);

	bbzero(rePrtBuff);
	if(type)	//重打印标志
		strcpy(rePrtBuff, "(重打印票据)");
	//交易本金
	bbzero(tra_amo);
	sprintf(tra_amo, "%012d", atoi(tal_amo)-atoi(prc_amo));
	bbzero(tra_amo_buff);
	CTools::chgToMny(tra_amo, tra_amo_buff);
	/*--------------打印内容----------------*/
	len = 0;
	sprintf((char*)pMsg+len, "【银联】\n商户名：%s\n商户号：%s\n", merName, merNum);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "终端号：%s\n操作员：01\n", uniPosNum);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "发卡行：%s\n收单行：%s\n收款卡号：%s\n", sndBank, rcvBank, card);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "交易类型：转帐\n批次号：%s\n流水号：%s\n", batNum, serial);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "时间：%s %s\n清算日期：%s\n系统参考号：%s\n银联参考号：%s\n后台处理号：%s\n", cDate, cTime, liqDate, peStan, uniStan,bgstan);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "交易帐号：%s\n交易金额：%s\n手续费：%s\n总计：%s\n持卡人签字%s：\n", gbl.card_no, tra_amo_buff, prc_amo_buff, tal_amo_buff, rePrtBuff);
	len=strlen((char*)pMsg);
	strcat((char*)pMsg, "\n\n------------------------------\n");
	strcat((char*)pMsg, "本人确认以上交易 同意将其记入本卡帐户\n");
	strcat((char*)pMsg, "------------------------------\n");
	strcat((char*)pMsg, "______________________________\n");
	/*--------------------------------------*/
	wrLog("打印明细：\n%s", pMsg);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);

	if(!type)
		iRet = print_func((char*)pMsg, iPrintLen, 3);
	else
		iRet = print_func((char*)pMsg, iPrintLen, 1);

	return iRet;
}


//设置打印内容
//type: 打印类型
//flag: 打印方式，默认为0，普通打印；1为电信新发票；2：无金额；3：肇庆客服10000号
int CPrint::set_content(PPrintTemp prt, char *cont, int type, int flag)
{
	int len = 0, i;
	char enter[20] = "\n\n\n\n", nameEnter[5];	//用户名与用户号码的换行
	char amoMny[100];

	bbzero(nameEnter);
	if(strlen(prt->name) > 37)
		strcpy(nameEnter, "\n");
	else
		strcpy(nameEnter, "\n\n");
	/*------------电信修改发票20080129--------------*/
	bbzero(amoMny);
	if(flag==1)
	{
			sprintf(amoMny, "(%s: %s, 实收: %.2f)", prt->detail98Name, prt->detail98Amo+1, atof(prt->total) + atof(prt->detail98Amo));
	}

	if(flag==1&&!strcmp(gbl.fee_type,"0361"))

	{
	
		sprintf(amoMny,"(实收：%.2f元)",atof(prt->total));
	}
	
	/*---------------------------------------------*/
	if(type == 1 || type == 3 || type == 8)	//套打发票
	{
		wrLog("prt->n = %d", prt->n);
		for(i=0; i<9 - prt->n; i++)
			strcat(prt->content,"\n");
	
		if(gbl.print_paper)	// 新发票
		{
			if(!strcmp(gbl.fee_type, "0092")) //佛山综合收费
			{
				strcat(prt->content,"\n\n\n\n");
				sprintf(cont+len, "\n\n\n\n     %17s %s %s\n", prt->date, prt->tim, prt->count);
				len = strlen(cont);
				sprintf(cont+len, "            %s%s\n            %s\n           %s\n\n", prt->name, nameEnter, prt->phone, prt->stan);
				len = strlen(cont);
				sprintf(cont+len, "        %s\n            %s\n\n", prt->card, prt->month);
				len = strlen(cont);
				
				sprintf(cont+len, "             %s\n%s", prt->type, prt->content);
				len = strlen(cont);
				
				if(flag==0)
					sprintf(cont+len, "        %s（实扣：%s）\n                 %s\n\n       开票%s 咨询电话400-80-11888", prt->total, prt->amo, prt->capsTotal, prt->pos);
				else if(flag==1)
					sprintf(cont+len, "        %s元\n                 %s\n             %s\n     开票%s 咨询电话400-80-11888", prt->total, prt->capsTotal, amoMny, prt->pos);
				else if(flag==2)
					sprintf(cont+len, "\n\n\n\n     开票%s 咨询电话400-80-11888", prt->pos);
				else
				sprintf(cont+len, "\n\n\n\n     开票%s 咨询电话400-80-11888", prt->pos);
			}
			else
			{//
				//strcat(prt->content,"\n\n\n\n\n");
				sprintf(cont+len, "\n\n\n\n\n   %17s %s %s\n", prt->date, prt->tim, prt->count);
				len = strlen(cont);
				sprintf(cont+len, "          %s%s\n          %s\n            %s\n\n", prt->name, nameEnter, prt->phone, prt->stan);
				len = strlen(cont);
				sprintf(cont+len, "      %s\n          %s\n", prt->card, prt->month);
				len = strlen(cont);
				
				sprintf(cont+len, "        %s         \n%s", prt->type, prt->content);
				len = strlen(cont);
				
				if(flag==0)
				{

					if(strcmp(gbl.city_no,"0086")==0)
					{
					sprintf(cont+len, "        %s（实扣：%s）\n\n             %s\n     开票%s 电话客服：400-882-1180", prt->total, prt->amo, prt->capsTotal, prt->pos);
					}
					else
					{
					sprintf(cont+len, "        %s（实扣：%s）\n\n             %s\n     开票%s 咨询电话400-80-11888", prt->total, prt->amo, prt->capsTotal, prt->pos);
					}
				}
				else if(flag==1)
				{
					if(strcmp(gbl.city_no,"0086")==0)
					{
						sprintf(cont+len, "        %s元\n                 %s\n             %s\n     开票%s 电话客服：400-882-1180", prt->total, prt->capsTotal, amoMny, prt->pos);
					}
					else
					{

					sprintf(cont+len, "        %s元\n                 %s\n             %s\n     开票%s 咨询电话400-80-11888", prt->total, prt->capsTotal, amoMny, prt->pos);
				
					}
				}
				else if(flag==2)
				{
					if(strcmp(gbl.city_no,"0086")==0)
					{
					sprintf(cont+len, "\n\n\n\n     开票%s 电话客服：400-882-1180", prt->pos);
					}
					else
					{
					sprintf(cont+len, "\n\n\n\n     开票%s 咨询电话400-80-11888", prt->pos);
					}
				}
				else
				{
					if(strcmp(gbl.city_no,"0086")==0)
					{
						sprintf(cont+len, "\n\n\n\n     开票%s 电话客服：400-882-1180", prt->pos);
					}
					else
					{
						sprintf(cont+len, "\n\n\n\n     开票%s 咨询电话400-80-11888", prt->pos);
					}
					

				}
			}
		}
		else
		{
			sprintf(cont+len, "\n\n\n\n%17s %s  %s\n\n", prt->date, prt->tim, prt->count);
			len = strlen(cont);
			sprintf(cont+len, "       %s%s        %s\n      %s\n", prt->name, nameEnter, prt->phone, prt->card);
			len = strlen(cont);
			sprintf(cont+len, "      %s\n          %s\n      %s\n", prt->serial, prt->stan, prt->month);
			len = strlen(cont);

			sprintf(cont+len, "        %s         \n%s", prt->type, prt->content);
			len = strlen(cont);

			if(flag==0)
			{
				if(strcmp(gbl.city_no,"0086")==0)
				{
					sprintf(cont+len, "        %s（实扣：%s）\n\n%s\n\n\n\n     %s\n         400-882-1180", prt->total, prt->amo, prt->capsTotal, prt->pos);

				}
				else
				{
					sprintf(cont+len, "        %s（实扣：%s）\n\n%s\n\n\n\n     %s\n         400-80-11888", prt->total, prt->amo, prt->capsTotal, prt->pos);
				}
				
			
			}
			else if(flag==1)
			{
				if(strcmp(gbl.city_no,"0086")==0)
				{
					sprintf(cont+len, "        %s元\n                 %s\n%s\n\n\n\n     %s\n         400-882-1180", prt->total, prt->capsTotal, amoMny, prt->pos);
				}
				else
				{
					sprintf(cont+len, "        %s元\n                 %s\n%s\n\n\n\n     %s\n         400-80-11888", prt->total, prt->capsTotal, amoMny, prt->pos);
				}
				
			}
			else if(flag==2)
			{
				if(strcmp(gbl.city_no,"0086")==0)
				{
					sprintf(cont+len, "\n\n\n\n\n     %s\n         400-882-1180", prt->pos);
				}
				else
				{
					sprintf(cont+len, "\n\n\n\n\n     %s\n         400-80-11888", prt->pos);
				}
				
			}
			else
			{
				if(strcmp(gbl.city_no,"0086")==0)
				{
					sprintf(cont+len, "\n\n\n\n\n     %s\n         400-882-1180", prt->pos);
				}
				else
				{
					sprintf(cont+len, "\n\n\n\n\n     %s\n         400-80-11888", prt->pos);
				}
				
			}
			
		}
		len = strlen(cont);

	}
	else if(type == 2 || type == 4 || type>=10 && type<=14)	//空白打印纸
	{
		strcat(prt->content,"\n\n");
		sprintf(cont+len, "【交费易】\n填票时间：%s %s\n票  数：%s\n", prt->date, prt->tim, prt->count);
		len = strlen(cont);
		sprintf(cont+len, "户  名：%s\n用户号码：%s\n", prt->name, prt->phone);
		len = strlen(cont);
		sprintf(cont+len, "系统参考号：%s\n交费月：%s\n", prt->stan, prt->month);
		len = strlen(cont);
		sprintf(cont+len, "交费项目：%s\n%s\n", prt->type, prt->content);
		len = strlen(cont);
// 		sprintf(cont+len,"  使用交费易网站，交费更轻松！\n      www.PayEasy.com.cn\n\n\n");
// 		len = strlen(cont);

		if(flag==0){

			if (strcmp(gbl.acc_city_no,"0769")==0)
			{
				sprintf(cont+len, "合  计：%s（实扣：%s）\n人民币合计（大写）：\n%s\n开  票：%s\n交费易客服: 400-80-11888\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
			}
			
			else if(strcmp(gbl.acc_city_no,"0086")==0) //如果是epos 全国推广版 并修改发票 电话客服：电话客服：400-882-1180
			
			{
				sprintf(cont+len, "合  计：%s（实扣：%s）\n人民币合计（大写）：\n%s\n开  票：%s\n电话客服: 400-882-1180\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
			
			}

			else
				sprintf(cont+len, "合  计：%s（实扣：%s）\n人民币合计（大写）：\n%s\n开  票：%s\n交费易客服: 400-80-11888\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
		

		}

		else if(flag==1)
		{
			if (strcmp(gbl.acc_city_no,"0769")==0)
			{
				sprintf(cont+len, "合  计：%s\n人民币合计（大写）：\n%s\n%s\n开  票：%s\n交费易客服: 400-80-11888\n\n%s", prt->total, prt->capsTotal, amoMny, prt->pos, enter);
			}
			else if(strcmp(gbl.city_no,"0086")==0)
			{
					sprintf(cont+len, "合  计：%s（实扣：%s）\n人民币合计（大写）：\n%s\n开  票：%s\n电话客服: 400-882-1180\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);

			}

			else
				sprintf(cont+len, "合  计：%s\n人民币合计（大写）：\n%s\n%s\n开  票：%s\n交费易客服: 400-80-11888\n\n%s", prt->total, prt->capsTotal, amoMny, prt->pos, enter);
		}

		else if(flag==2)
			sprintf(cont+len, "\n\n\n\n\n     %s", prt->pos);
		else if(flag==3)
			sprintf(cont+len, "合  计：%s（实扣：%s）\n人民币合计（大写）：\n%s\n开  票：%s\n肇庆交费易客服: 10000\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
		else
			sprintf(cont+len, "\n\n\n\n\n     %s", prt->pos);
		len = strlen(cont);
		
	}
	else if(type == 9)	//佛山套打发票
	{
		wrLog("=========佛山套打发票=========");
		wrLog("prt->n = %d", prt->n);
		for(i=0; i<9 - prt->n; i++)
			strcat(prt->content,"\n");

		sprintf(cont+len, "\n\n\n%17s %s  %s\n\n", prt->date, prt->tim, prt->count);
		len = strlen(cont);
		sprintf(cont+len, "       %s%s        %s\n      %s\n", prt->name, nameEnter, prt->phone, prt->card);
		len = strlen(cont);
		sprintf(cont+len, "      %s\n          %s\n      %s\n", prt->serial, prt->stan, prt->month);
		len = strlen(cont);
		sprintf(cont+len, "\n            %s         \n%s", prt->type, prt->content);
		len = strlen(cont);
		if(flag==1)
			sprintf(cont+len, "        %s元\n                 %s\n%s\n\n\n\n     %s", prt->total, prt->capsTotal, amoMny, prt->pos);
		else if(flag==0)
			sprintf(cont+len, "        %s（实扣：%s）\n\n%s\n\n\n     %s", prt->total, prt->amo, prt->capsTotal, prt->pos);
		else
			sprintf(cont+len, "\n\n\n\n\n\n     %s", prt->pos);
		len = strlen(cont);
	}

	//wrLog("打印数据：\n%s", cont);

	return len;
}

int  CPrint::print_fail(int iseny, char pMsg[2048], int cpyReprint){
	int iRet;
	if(iseny){		
		iRet = print_func_safe(pMsg,strlen(pMsg),cpyReprint);
	}else{
		iRet = print_func(pMsg,strlen(pMsg),1,cpyReprint);
	}
	return iRet;
	
}

int CPrint::print_ysq_detail(const char *type)		//银行划扣受理打印
{
	int iPrintLen , iRet, i;
	BYTE pMsg[1024];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");

    if (!memcmp(gbl.fee_type,"0217",4)) 
	{
	strcpy(prtTemp.name, gbl.user_name);//用户名 电话号码
	strcpy(prtTemp.phone, gbl.phone_no);
	}

	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	strcpy(prtTemp.type, type);						 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细
    if (!memcmp(gbl.fee_type,"0217",4)) //电信
	{
	prtTemp.n=0;
	sprintf(prtTemp.content, "电话号码：%s\n帐号：%s\n帐户名称：%s\n开户银行：", gbl.phone_no, gbl.card_no, prtTemp.name);
	prtTemp.n += 3;
	memcpy( &(prtTemp.content)[strlen(prtTemp.content)], pack.FIELD63, 60);
	prtTemp.n++;
	strcat(prtTemp.content, "\n说明：\n1.受理成功后，电信公司将从上述帐号\n定期自动划扣用户号码的电信费用;\n2.如需退订请亲临电信营业厅办理;\n3.咨询电话10000.\n");
	prtTemp.n += 6;

    }else if (!memcmp(gbl.fee_type,"0222",4))
    {
		prtTemp.n=0;
		sprintf(prtTemp.content,"手机号码：%s\n身份证：%s\n\n【交费易签约】成功\n手机号/身份证号是交费的帐户请妥善保管\n交费易密码电视界面可修改\n如有疑问请按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询\n",gbl.phone_no,gbl.CATV_id_card);
		prtTemp.n += 7;
    }else  //有线电视
	{
	prtTemp.n=0;
	sprintf(prtTemp.content,"手机号码：%s\n身份证：%s\n",gbl.phone_no,gbl.CATV_id_card);
	prtTemp.n += 2;	
	}
    
   	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType, 2);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//改造函数
int CPrint::print_ysq_detail(CPack pack,const char *type)		//银行划扣受理打印
{
	int iPrintLen , iRet, i;
	BYTE pMsg[1024];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	
    if (!memcmp(gbl.fee_type,"0217",4)) 
	{
		strcpy(prtTemp.name, gbl.user_name);//用户名 电话号码
		strcpy(prtTemp.phone, gbl.phone_no);
	}
	
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type);						 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细
    if (!memcmp(gbl.fee_type,"0217",4)) //电信
	{
		prtTemp.n=0;
		sprintf(prtTemp.content, "电话号码：%s\n帐号：%s\n帐户名称：%s\n开户银行：", gbl.phone_no, gbl.card_no, prtTemp.name);
		prtTemp.n += 3;
		memcpy( &(prtTemp.content)[strlen(prtTemp.content)], pack.FIELD63, 60);
		prtTemp.n++;
		strcat(prtTemp.content, "\n说明：\n1.受理成功后，电信公司将从上述帐号\n定期自动划扣用户号码的电信费用;\n2.如需退订请亲临电信营业厅办理;\n3.咨询电话10000.\n");
		prtTemp.n += 6;
		
    }else if (!memcmp(gbl.fee_type,"0222",4))
    {
		prtTemp.n=0;
		sprintf(prtTemp.content,"手机号码：%s\n身份证：%s\n\n【交费易签约】成功\n手机号/身份证号是交费的帐户请妥善保管\n交费易密码电视界面可修改\n如有疑问请按【Ctrl+H】联系在线客服中心 或致电400-80-11888咨询\n",gbl.phone_no,gbl.CATV_id_card);
		prtTemp.n += 7;
    }else  //有线电视
	{
		prtTemp.n=0;
		sprintf(prtTemp.content,"手机号码：%s\n身份证：%s\n",gbl.phone_no,gbl.CATV_id_card);
		prtTemp.n += 2;	
	}
    
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType, 2);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//end of 改造函数

int CPrint::print_traffic_detail(const char *type, char *field48, char *field63,int b63Idx)//交通罚款
{
	int iPrintLen=0 , iRet=0, i=0;
	char name_buf[20],shortname_buf[20],carNum_buf[20],paperNum_buf[20],time_buf[25],lateFee_buf[13],originalFee_buf[13],phone_buf[20],totalFee_buf[13],pMsg[1024], detail_buf[100], amount[12+1], yymmdd[9], hhmmss[7], date[14+1], len_buf[2+1];
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	/*
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	*/
	CTools::getDate(yymmdd);
	CTools::getTime(hhmmss);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	
	strcpy(prtTemp.phone, gbl.phone_no);

	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	//memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//系统参考号	
	if(b63Idx==54){
          strcat(prtTemp.stan," (交罚补打凭证) ");
	}
	strcpy(prtTemp.type, type);						 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细

	bbzero(detail_buf);
	bbzero(len_buf);
	bbzero(date);
	bbzero(amount);
	bbzero(prtTemp.content);
	b63Idx += 2;
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "违章顺序号:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "身份证:%s\n", detail_buf);
	
	b63Idx += 2;//号牌种类
	memcpy(len_buf, field63+b63Idx, 2);//号牌简称
	b63Idx += 2;
	bbzero(shortname_buf);
	memcpy(shortname_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
	
	memcpy(len_buf, field48+4, 2);
	bbzero(detail_buf);
	memcpy(detail_buf, field48+6, 4);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "文书类别:%s\n", detail_buf);
	bbzero(paperNum_buf);
	memcpy(paperNum_buf, field48+10, atoi(len_buf)-4);	
//	sprintf(prtTemp.content+strlen(prtTemp.content), "文书号:%s\n", detail_buf);
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(carNum_buf);
	memcpy(carNum_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.phone, "车牌号码:%s%s", shortname_buf,detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(name_buf);
	if(atoi(len_buf)>=2)
	{
		memcpy(name_buf, field63+b63Idx, 2);
		if(atoi(len_buf)>=4)
		{
			memcpy(name_buf+2, "*",1);
			if(atoi(len_buf)>=6)
			{
				memcpy(name_buf+3, field63+b63Idx+4, atoi(len_buf)-4);
			}			
		}		
	}
	b63Idx += atoi(len_buf);
	sprintf(prtTemp.name, "%s", name_buf);//当事人
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(phone_buf);
	memcpy(phone_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "联系电话:%s\n", detail_buf);
	
	bbzero(date);
	memcpy(date, field63+b63Idx, 14);
	b63Idx += 14;
	bbzero(time_buf);
	CTools::formatDate(date, time_buf);
	sprintf(time_buf+10,"  ",2);
	CTools::formatTime(date+8, time_buf+12);
//	sprintf(prtTemp.month, "处罚日期:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(originalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, originalFee_buf);			
//	sprintf(prtTemp.content+strlen(prtTemp.content), "罚款金额:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(totalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, totalFee_buf);
	memcpy(prtTemp.total, totalFee_buf, strlen(totalFee_buf)-2); //合计
	strcpy(prtTemp.amo, totalFee_buf);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	bbzero(amount);
	bbzero(lateFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, lateFee_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "滞纳金:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "执罚单位:%s\n", detail_buf);

    sprintf(prtTemp.content+strlen(prtTemp.content), "车牌号码:%s%s\n", shortname_buf,carNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "文书号:%s\n", paperNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "处罚日期:%s\n", time_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "原罚款金额:%s\n", originalFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "滞纳金:%s\n", lateFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "合计金额:%s\n", totalFee_buf);
/*	sprintf(prtTemp.content+strlen(prtTemp.content), "联系电话:%s\n", phone_buf);*/

	prtTemp.n += 6;
   
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//改造函数
int CPrint::print_traffic_detail(CPack pack,const char *type, char *field48, char *field63,int b63Idx)//交通罚款
{
	int iPrintLen=0 , iRet=0, i=0;
	char name_buf[20],shortname_buf[20],carNum_buf[20],paperNum_buf[20],time_buf[25],lateFee_buf[13],originalFee_buf[13],phone_buf[20],totalFee_buf[13],pMsg[1024], detail_buf[100], amount[12+1], yymmdd[9], hhmmss[7], date[14+1], len_buf[2+1];
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	
	strcpy(prtTemp.phone, gbl.phone_no);

	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	if(b63Idx==54){
          strcat(prtTemp.stan," (交罚补打凭证) ");
	}
	strcpy(prtTemp.type, type);						 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细

	bbzero(detail_buf);
	bbzero(len_buf);
	bbzero(date);
	bbzero(amount);
	bbzero(prtTemp.content);
	b63Idx += 2;
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "违章顺序号:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "身份证:%s\n", detail_buf);
	
	b63Idx += 2;//号牌种类
	memcpy(len_buf, field63+b63Idx, 2);//号牌简称
	b63Idx += 2;
	bbzero(shortname_buf);
	memcpy(shortname_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
	
	memcpy(len_buf, field48+4, 2);
	bbzero(detail_buf);
	memcpy(detail_buf, field48+6, 4);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "文书类别:%s\n", detail_buf);
	bbzero(paperNum_buf);
	memcpy(paperNum_buf, field48+10, atoi(len_buf)-4);	
//	sprintf(prtTemp.content+strlen(prtTemp.content), "文书号:%s\n", detail_buf);
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(carNum_buf);
	memcpy(carNum_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.phone, "车牌号码:%s%s", shortname_buf,detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(name_buf);
	if(atoi(len_buf)>=2)
	{
		memcpy(name_buf, field63+b63Idx, 2);
		if(atoi(len_buf)>=4)
		{
			memcpy(name_buf+2, "*",1);
			if(atoi(len_buf)>=6)
			{
				memcpy(name_buf+3, field63+b63Idx+4, atoi(len_buf)-4);
			}			
		}		
	}
	b63Idx += atoi(len_buf);
	sprintf(prtTemp.name, "%s", name_buf);//当事人
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(phone_buf);
	memcpy(phone_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "联系电话:%s\n", detail_buf);
	
	bbzero(date);
	memcpy(date, field63+b63Idx, 14);
	b63Idx += 14;
	bbzero(time_buf);
	CTools::formatDate(date, time_buf);
	sprintf(time_buf+10,"  ",2);
	CTools::formatTime(date+8, time_buf+12);
//	sprintf(prtTemp.month, "处罚日期:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(originalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, originalFee_buf);			
//	sprintf(prtTemp.content+strlen(prtTemp.content), "罚款金额:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(totalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, totalFee_buf);
	memcpy(prtTemp.total, totalFee_buf, strlen(totalFee_buf)-2); //合计
	strcpy(prtTemp.amo, totalFee_buf);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	bbzero(amount);
	bbzero(lateFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, lateFee_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "滞纳金:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "执罚单位:%s\n", detail_buf);

    sprintf(prtTemp.content+strlen(prtTemp.content), "车牌号码:%s%s\n", shortname_buf,carNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "文书号:%s\n", paperNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "处罚日期:%s\n", time_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "原罚款金额:%s\n", originalFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "滞纳金:%s\n", lateFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "合计金额:%s\n", totalFee_buf);
/*	sprintf(prtTemp.content+strlen(prtTemp.content), "联系电话:%s\n", phone_buf);*/

	prtTemp.n += 6;
   
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
/*	strcat(prtTemp.content, "此单不作报销凭证\n");*/
	prtTemp.n++;
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//end of 改造函数
int CPrint::print_ADD_ADSL(const char *type)
{
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------提取打印数据------------------*/
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //金额
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type); 					 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	bbzero(prtTemp.name);			 //用户名
	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	bbzero(prtTemp.month);	 
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//打印内容
	prtTemp.n=0;
	
	sprintf(prtTemp.content,"客户类型：%s\n",gbl.adsl_custom_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "包月类型：%s\n", gbl.adsl_by_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "MODEM类型：%s\n", gbl.adsl_modem_type);
	prtTemp.n += 3;
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n ++;
	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//改造函数
int CPrint::print_ADD_ADSL(CPack pack,const char *type)
{
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //打印模板
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------提取打印数据------------------*/
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //金额
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");			 //计数
	strcpy(prtTemp.serial, gbl.pos_serial);			 //终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	strcpy(prtTemp.type, type); 					 //业务类型
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	bbzero(prtTemp.name);			 //用户名
	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	bbzero(prtTemp.month);	 
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//打印内容
	prtTemp.n=0;
	
	sprintf(prtTemp.content,"客户类型：%s\n",gbl.adsl_custom_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "包月类型：%s\n", gbl.adsl_by_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "MODEM类型：%s\n", gbl.adsl_modem_type);
	prtTemp.n += 3;
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n++;
	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

//end of 改造函数


int CPrint::print_sx_ysl(int flag)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));

	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);
	prtTemp.n = 2;
	strcpy(prtTemp.phone,gbl.phone_no);
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial,"009310");
	sprintf(prtTemp.stan,"%s","000084606364");
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	bbzero(prtTemp.month);
	sprintf(content,"身份证号：%s\n手机号码：%s\n",gbl.id_num,gbl.telphone);
	strcpy(prtTemp.content,content);
	bbzero(prtTemp.total);
	bbzero(prtTemp.amo);
	bbzero(prtTemp.capsTotal);
		strcpy(prtTemp.pos,gbl.pos_no);
	if (flag == 1)
	{
		strcpy(prtTemp.type,"银行划扣受理回执");		
	}else if (flag == 2)
	{
		strcpy(prtTemp.type,"已有固话新装ADSL受理回执");		
	}else if (flag == 3)
	{
		strcpy(prtTemp.type,"e8套餐申请受理回执");		
	}else if (flag == 4)
	{
		strcpy(prtTemp.type,"e8包年升级套餐受理回执");		
	}else if (flag == 5)
	{
		strcpy(prtTemp.type,"e6套餐申请受理回执");		
	}else
		return -1;
	memset(pMsg,0,sizeof(pMsg));

	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

int CPrint::print_ibss(char *telno,char *id_card,char *yyt_name)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);

	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	//卡号
	bbzero(prtTemp.card);
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	sprintf(content,"选择的号码：%s\n身份证号：%s\n营业厅名称：%s\n说明:\n1.请妥善保管打印的选号纸，携带身份证到指定电信营业厅办理。\n2.选号信息当日生效，次日未到指定电信营业厅办理业务则号码无效。\n咨询热线:10000"
		,telno,id_card,yyt_name);
	strcpy(prtTemp.content,content);
	prtTemp.n = 7;
	bbzero(prtTemp.amo);
	bbzero(prtTemp.capsTotal);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"小灵通选号单");		
	memset(pMsg,0,sizeof(pMsg));
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

int CPrint::print_selectcdma(int flag,char *yyt_name)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);

	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	prtTemp.n = 8;	
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"CDMA选号单");		
	memset(pMsg,0,sizeof(pMsg));
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
		bbzero(prtTemp.card);
	}
	//卡号
	if (flag == 1)
	{
		if(!memcmp(gbl.card_no, "8828", 4))
			memcpy(prtTemp.card, "888888", 6);
		else
		{
			memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
			for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
				prtTemp.card[i] = '*';
		}
		if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
		{
			bbzero(prtTemp.card);
		}
		strcpy(prtTemp.stan,"000000011099");
		bbzero(amoMny);
		CTools::chgToMny("000000005000", amoMny);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
		strcpy(prtTemp.amo, amoMny);					 //实扣
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
		sprintf(content,"所选号码：%s\n选号费：50.00元\n营业厅名称：%s\n身份证号：%s\n\n说明:\n1.请妥善保管打印的选号纸，凭登记的身份证到您选择的营业厅领取手机SIM卡。\n2.相关业务规定以当地营业厅规定为准\n咨询热线:10000"
		,gbl.telphone,yyt_name,gbl.id_num);
		strcpy(prtTemp.content,content);
		strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

		iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	}else{
		bbzero(prtTemp.card);
		bbzero(prtTemp.amo);
		bbzero(prtTemp.capsTotal);
		bbzero(prtTemp.total);
		sprintf(content,"选择的号码：%s\n营业厅名称：%s\n身份证号：%s\n\n说明:\n1.请妥善保管打印的选号纸，于24小时内凭登记的身份证到您选择的营业厅办理预约业务，逾期号码将不再保留\n2.相关业务规定以当地营业厅规定为准\n咨询热线:10000"
		,gbl.telphone,yyt_name,gbl.id_num);
		strcpy(prtTemp.content,content);
		strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

		iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	}

	
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}


int CPrint::print_SELL_POS(const char *type,char *field63)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//strcpy(prtTemp.stan,gbl.stan_no);
    strcpy(prtTemp.stan , (char*)pack.FIELD37 + strlen((char*)pack.FIELD37) - 12);//直接使用37域后12位 值做为stan号
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if (gbl.product_flag == 1||gbl.product_flag == 81||gbl.product_flag == 82)
	{
		sprintf(content,"产品名称：%s\n数量：%d台\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 21||gbl.product_flag == 22||gbl.product_flag == 5)
	{
		sprintf(content,"产品名称：%s\n数量：%d套\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 3)
	{
		sprintf(content,"产品名称：%s\n数量：%d份\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 9)
	{
		sprintf(content,"产品名称：%s\n数量：%d个\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 6)
	{
		sprintf(content,"产品名称：%s\n数量：%d块\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 7)
	{
		sprintf(content,"产品名称：%s\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,amoMny,gbl.telphone);
	}else if (gbl.product_flag == 4)
	{
		sprintf(content,"%s  %s\n"
			,gbl.product_name,gbl.telphone);
	}else
		return -1;
	
	strcpy(prtTemp.content,content);
	if (gbl.product_flag == 4)
	{
		prtTemp.n = 1;
	}else
		prtTemp.n = 6;

	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	if (gbl.product_flag == 4)
	{
		iRet = print_func((char*)pMsg,iPrintLen);
	}else
		iRet = print_func((char*)pMsg,iPrintLen,2);

	return iRet;
}

//改造函数
int CPrint::print_SELL_POS(CPack pack,const char *type,char *field63)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	/*
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	*/
	CTools::getDate(yymmdd);
	CTools::getTime(hhmmss);

	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}


	//strcpy(prtTemp.stan,gbl.stan_no);
   // strcpy(prtTemp.stan , (char*)pack.FIELD37 + strlen((char*)pack.FIELD37) - 12);//直接使用37域后12位 值做为stan号
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//系统参考号
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if (gbl.product_flag == 1||gbl.product_flag == 81||gbl.product_flag == 82)
	{
		sprintf(content,"产品名称：%s\n数量：%d台\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 21||gbl.product_flag == 22||gbl.product_flag == 5)
	{
		sprintf(content,"产品名称：%s\n数量：%d套\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 3)
	{
		sprintf(content,"产品名称：%s\n数量：%d份\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 9)
	{
		sprintf(content,"产品名称：%s\n数量：%d个\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 6)
	{
		sprintf(content,"产品名称：%s\n数量：%d块\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 7)
	{
		sprintf(content,"产品名称：%s\n合计金额：%s\n联系电话：%s\n\n客户签名：\n"
			,gbl.product_name,amoMny,gbl.telphone);
	}else if (gbl.product_flag == 4)
	{
		sprintf(content,"%s  %s\n"
			,gbl.product_name,gbl.telphone);
	}else
		return -1;
	
	strcpy(prtTemp.content,content);
	if (gbl.product_flag == 4)
	{
		prtTemp.n = 1;
	}else
		prtTemp.n = 6;
	
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	if (gbl.product_flag == 4)
	{
		iRet = print_func((char*)pMsg,iPrintLen);
	}else
		iRet = print_func((char*)pMsg,iPrintLen,2);

	return iRet;
}
//end of 改造函数

int CPrint::print_payonce_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板

	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, "000084821782", 12);			//系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.phone, gbl.phone_no);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}

	bbzero(amoMny);
	strcpy(gbl.amount, "000000035000");
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	strcpy(prtTemp.content,"一次性交费\n新装电话费:300.00\n改装工料费:50.00\n");
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

int CPrint::print_province_traffic_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(不作报销凭证)");
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name, gbl.user_name);
	strcpy(prtTemp.phone, gbl.telphone);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if(atoi(gbl.transfer_flag)==1){
	    sprintf(prtTemp.content,"车牌号：粤%s\n违章时间：%s\n违章行为：%s\n违章地点：%s\n 地址：%s\n罚款：%s  邮费：%s\n代办费：%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.address,gbl.traffic_mny,gbl.transfer_mny,gbl.service_mny);
	}else{
		sprintf(prtTemp.content,"车牌号：粤%s\n违章时间：%s\n违章行为：%s\n违章地点：%s\n 罚款：%s\n代办费：%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.traffic_mny,gbl.service_mny);
	}

	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//改造函数
int CPrint::print_province_traffic_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(不作报销凭证)");
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name, gbl.user_name);
	strcpy(prtTemp.phone, gbl.telphone);			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if(atoi(gbl.transfer_flag)==1){
		sprintf(prtTemp.content,"车牌号：粤%s\n违章时间：%s\n违章行为：%s\n违章地点：%s\n 地址：%s\n罚款：%s  邮费：%s\n代办费：%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.address,gbl.traffic_mny,gbl.transfer_mny,gbl.service_mny);
	}else{
		sprintf(prtTemp.content,"车牌号：粤%s\n违章时间：%s\n违章行为：%s\n违章地点：%s\n 罚款：%s\n代办费：%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.traffic_mny,gbl.service_mny);
	}
	
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//end of 改造函数

int CPrint::print_bestpay_detail(const char *type)	//翼支付打印
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(不作报销凭证)");
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name,"");			//用户名
	strcpy(prtTemp.phone, "");			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}

	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, gbl.amount, strlen(gbl.amount)-2);
	strcpy(prtTemp.amo, gbl.amount);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	sprintf(prtTemp.content,"充值账号:%s\n充值金额:%s\n账户余额:%s\n",gbl.bestpayAccount,gbl.amount,gbl.bestpayTotalMny);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;

}

//羊城通销售

int CPrint::print_yct_sellDetail(const char *type)	//
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count,"第1张/共1张");
	memcpy(prtTemp.stan, (char *)gbl.stan_no, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name,"");			//用户名
	strcpy(prtTemp.phone, "");			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	strcpy(prtTemp.content,"羊城通读卡器\n羊城通读卡器一套1100元（含PKI密钥卡及PSAM卡各一张）");
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}


//改造函数
int CPrint::print_bestpay_detail(CPack pack,const char *type)	//翼支付打印
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(不作报销凭证)");
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name,"");			//用户名
	strcpy(prtTemp.phone, "");			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, gbl.amount, strlen(gbl.amount)-2);
	strcpy(prtTemp.amo, gbl.amount);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	sprintf(prtTemp.content,"充值账号:%s\n充值金额:%s\n账户余额:%s\n",gbl.bestpayAccount,gbl.amount,gbl.bestpayTotalMny);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

//end of 改造函数


int CPrint::print_xjx_detail(const char *type)//打印薪加薪资金划拨发票
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(不作报销凭证)");
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name,"");			//用户名
	strcpy(prtTemp.phone, "");			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	strcpy(prtTemp.amo, gbl.amount);					 //实扣
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny));
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额


		sprintf(prtTemp.content,"转出账号:%s\n交易金额:%s\n转入账户:%s\n",gbl.out_account,amoMny,gbl.in_account);
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//改造函数
int CPrint::print_xjx_detail(CPack pack,const char *type)//打印薪加薪资金划拨发票
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //系统参考号
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(不作报销凭证)");
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name,"");			//用户名
	strcpy(prtTemp.phone, "");			 //电话号码
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	strcpy(prtTemp.amo, gbl.amount);					 //实扣
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny));
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	
	sprintf(prtTemp.content,"转出账号:%s\n交易金额:%s\n转入账户:%s\n",gbl.out_account,amoMny,gbl.in_account);
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//end of 改造函数
int CPrint::print_NHBank_detail(char*card_num,char*recv_name,char*kh_bank,char*tra_amo_buff)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"农行转账");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,"000086192633");
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	sprintf(content,"收款账号：%s\n收款人姓名：%s\n开户行名称：%s\n转账金额：%s\n",card_num,recv_name,kh_bank,tra_amo_buff);
	strcpy(prtTemp.content,content);
	prtTemp.n = 4;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_customsign_detail(char*id_card,char*custom_no,char*tel_phone)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"商户签约");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if (memcmp(gbl.pos_no+1,"8",1)==0)
	{
		sprintf(content,"用户手机号：%s\n用户身份证号：%s\n用户终端号：%s\n提示：1.鉴权操作后建设银行用户请按【Ctrl+H】联系在线客服中心 或致电400-80-11888 登记您的建行卡对应的帐户名称,系统将在一个工作日内完成帐户审核工作.\n2.您可以使用签约成功的手机号码发送“1111”到106573025157(0.1元/条)，您以后即可免费获取每次e卡成功充值的信息\n用户签名：",tel_phone,id_card,custom_no);
	}else
		sprintf(content,"用户手机号：%s\n用户身份证号：%s\n用户终端号：%s\n提示：\n1.请按【Ctrl+H】联系在线客服中心 或致电400-80-11888登记您的银行卡用户名，该签约用于为您提供方便快捷的e卡充值服务\n2.您可以使用签约成功的手机号码(仅限移动用户)发送“1111”到106573025157(0.1元/条)，您以后即可免费获取每次e卡成功充值的信息\n用户签名：",tel_phone,id_card,custom_no);
	strcpy(prtTemp.content,content);
	prtTemp.n = 9;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	
	iRet = print_func((char*)pMsg,iPrintLen,2);
	
	return iRet;
}

int CPrint::print_np(char*car_owner,char*car_no,char*tel_phone)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"汽车年票交纳");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	if (strlen(gbl.car_no)==5)
	{
		sprintf(content,"车牌号码：粤A%s\n车主名称：%s**   联系电话：%s\n缴费年度：2010   年票金额：980.00元\n服务费：25.00元  合计金额：1005.00元\n",car_no,car_owner,tel_phone);
	}else
		sprintf(content,"车牌号码：粤%s\n车主名称：%s**   联系电话：%s\n缴费年度：2010   年票金额：980.00元\n服务费：25.00元  合计金额：1005.00元\n",car_no,car_owner,tel_phone);
	strcat(content,"注意事项：\n");
	strcat(content," 1.客服人员将通过致电方式与您确认邮递地址;\n");
	strcat(content," 2.年票已成功代收,我们将在7个工作日内打印并且快递年票凭证到您的收件地址，请留意查收;\n");
	strcat(content," 3.因政策性或者客户资料不符等原因,造成年票凭证打印不成功,我们将在4个工作日内与您联系.\n");
	strcpy(prtTemp.content,content);
	prtTemp.n = 9;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::Add_advertisement(const char *type_fee,char *advertisement,int *adv_num)
{
	int i,k,count=0;
	char temp[1024];
	for (i=0;i<MAX_PRINT_ADV;i++)
	{
		for (k=0;k<print_adv_cnt[i];k++)
		{
			if (print_adv[i][k].fee_group==1)
			{
				bbzero(temp);
				sprintf(temp,"%s\n",print_adv[i][k].advertisement);
				strcat(advertisement,temp);
				count++;
				break;
			}
			else{
				if (print_adv[i][k].fee_group == atoi(type_fee))
				{
					bbzero(temp);
					sprintf(temp,"%s\n",print_adv[i][k].advertisement);
					strcat(advertisement,temp);
					count++;
					break;
				}
			}
		}
	}
	*adv_num = count;
	return 0;
}

int CPrint::print_chk_amo(const char *type, char *content)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	if (gbl.product_flag == 4)
	{
		prtTemp.n = 1;
	}else
		prtTemp.n = 6;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType, 2);
	if (gbl.product_flag == 4)
	{
		iRet = print_func((char*)pMsg,iPrintLen);
	}else
		iRet = print_func((char*)pMsg,iPrintLen,2);
	
	return iRet;
}

//改造函数
int CPrint::print_chk_amo(CPack pack,const char *type, char *content)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	if (gbl.product_flag == 4)
	{
		prtTemp.n = 1;
	}else
		prtTemp.n = 6;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType, 2);
	if (gbl.product_flag == 4)
	{
		iRet = print_func((char*)pMsg,iPrintLen);
	}else
		iRet = print_func((char*)pMsg,iPrintLen,2);
	
	return iRet;
}

//end of 改造函数
/////////////////////////////////东莞公交/////////////////////////////////////////
void CPrint::tranBusCard(char * const bus_card_4, char *bus_card_no_10){
	unsigned long lBus_card=0;
	int i=0;
	char temp,result[10+1];
	lBus_card = (unsigned char)bus_card_4[3] + (unsigned char)bus_card_4[2]*0x0100 + (unsigned char)bus_card_4[1]*0x010000 + (unsigned char)bus_card_4[0]*0x01000000;
	bbzero(result);
	for(i=9; i>=0; i--){
		result[i] = (char)('0' + lBus_card%10);
		lBus_card = lBus_card /10;
	}
	for(i=0; i<2; i++){
		temp = result[i*2];
		result[i*2] = result[(4-i)*2];
		result[(4-i)*2] = temp;
		temp = result[i*2+1];
		result[i*2+1] = result[(4-i)*2+1];
		result[(4-i)*2+1] = temp;
	}
	for(i=0; i<10; i++){
		if(result[i]>='5')
			result[i] = result[i] - 5;
		else
			result[i] = result[i] + 5;
	}
	memcpy(bus_card_no_10,result,10);
}


int CPrint::print_bus_detail(char *bus_card_info, int flag)  //打印东莞公交充值
{
	wrLog("【print_bus_detail】");
	int iPrintLen, iRet ,n = 0;
	char date[11], tim[9], bus_card_no_10[10+1], capMny[50], tal[20], amoMny[15], temp[100], tmp_card_no[25];
	char bus_card_infoBCD[96+1], content[1024];
	char title[100];
	unsigned long bus_current_amo=0;
	unsigned long bus_charge_amo=0;
	unsigned long bus_old_amo=0;
	BYTE pMsg[1024];
	CTools tool;
	int adv_count=0;
	char advertisement[4096];

	PrintTemp prtTemp;   //打印模板	
	memset(&prtTemp, 0, sizeof(prtTemp));

	bbzero(advertisement);
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);

	bbzero(title);
	bbzero(bus_card_infoBCD);
	CTools::byte_to_hex((const BYTE*)bus_card_info,(BYTE*)bus_card_infoBCD,48);

	//获取日期
	bbzero(date);	
	strcpy(date,"20");
	memcpy(date+2,bus_card_infoBCD+68,2);
	strcat(date,"/");
	memcpy(date+5,bus_card_infoBCD+70,2);
	strcat(date,"/");
	memcpy(date+8,bus_card_infoBCD+72,2);
	//获取时间
	bbzero(tim);
	memcpy(tim,bus_card_infoBCD+74,2);
	strcat(tim,":");
	memcpy(tim+3,bus_card_infoBCD+76,2);
	strcat(tim,":");
	memcpy(tim+6,bus_card_infoBCD+78,2);
	//获取卡编号
	bbzero(bus_card_no_10);
	tranBusCard(bus_card_info+1, bus_card_no_10);
	
	//获取内容明细
	bbzero(content);
	switch(flag)
	{	
	case 1:

		//bus_amo=(unsigned char)bus_card_info[23] + (unsigned char)bus_card_info[24]*0x0100 + (unsigned char)bus_card_info[25]*0x010000 + (unsigned char)bus_card_info[26]*0x01000000;
		
		if( !strcmp(gbl.bus_selltype,"1") ){
			strcpy(title, "东莞公交开卡工本费收据");
			strcpy(content, "出售：15.00元");
		}else if( !strcmp(gbl.bus_selltype,"2") ){
			strcpy(title, "东莞公交开卡租金收据");
			strcpy(content, "租金：18.00元");
		}else if( !strcmp(gbl.bus_selltype,"3") ){
			strcpy(title, "东莞公交开卡押金收据");
			strcpy(content, "押金：25.00元");
		}

		if(gbl.HID_TYPE==1) //高位在左，低位在右
		{

			//卡原有金额
			bus_old_amo=(unsigned char)bus_card_info[14] + (unsigned char)bus_card_info[13]*0x0100 + (unsigned char)bus_card_info[12]*0x010000 + (unsigned char)bus_card_info[11]*0x01000000;
			//卡充入金额
			bus_charge_amo=(unsigned char)bus_card_info[18] + (unsigned char)bus_card_info[17]*0x0100 + (unsigned char)bus_card_info[16]*0x010000 + (unsigned char)bus_card_info[15]*0x01000000;
			//卡现有金额
			bus_current_amo=(unsigned char)bus_card_info[22] + (unsigned char)bus_card_info[21]*0x0100 + (unsigned char)bus_card_info[20]*0x010000 + (unsigned char)bus_card_info[19]*0x01000000;
	
		}
		else
		{

			bus_old_amo=(unsigned char)bus_card_info[11] + (unsigned char)bus_card_info[12]*0x0100 + (unsigned char)bus_card_info[13]*0x010000 + (unsigned char)bus_card_info[14]*0x01000000;
			bus_charge_amo=(unsigned char)bus_card_info[15] + (unsigned char)bus_card_info[16]*0x0100 + (unsigned char)bus_card_info[17]*0x010000 + (unsigned char)bus_card_info[18]*0x01000000;
			bus_current_amo=(unsigned char)bus_card_info[19] + (unsigned char)bus_card_info[20]*0x0100 + (unsigned char)bus_card_info[21]*0x010000 + (unsigned char)bus_card_info[22]*0x01000000;
			

		}
		
		sprintf(content+strlen(content), "\n卡原有金额：%d.%02d元", bus_old_amo/100, bus_old_amo%100);
		sprintf(content+strlen(content), "\n卡充入金额：%d.%02d元", bus_charge_amo/100, bus_charge_amo%100);
		sprintf(content+strlen(content), "\n卡现有金额：%d.%02d元\n", bus_current_amo/100, bus_current_amo%100);

		if( !strcmp(gbl.bus_selltype,"1") ){
			strcat(content, "备注：此卡一次性买断，一概不做退卡操作");
		}else if( !strcmp(gbl.bus_selltype,"2") ){
			strcat(content, "备注：退还租金时需要持IC卡及本收据方可办理");
		}else if( !strcmp(gbl.bus_selltype,"3") ){
			strcat(content, "备注：退还押金时需要持IC卡及本收据方可办理");
		}
		n = 4;
		break;
	case 2:
		strcpy(title, "东莞公交IC卡充值收据");
		strcpy(content, "充值类型：普通票\n卡类型：");
		if( !memcmp(bus_card_infoBCD+18, "41", 2) )
			strcat(content, "A卡");
		else
			strcat(content, "B卡");
		if(gbl.HID_TYPE==1)
		{
			bus_old_amo=(unsigned char)bus_card_info[14] + (unsigned char)bus_card_info[13]*0x0100 + (unsigned char)bus_card_info[12]*0x010000 + (unsigned char)bus_card_info[11]*0x01000000;
			bus_charge_amo=(unsigned char)bus_card_info[18] + (unsigned char)bus_card_info[17]*0x0100 + (unsigned char)bus_card_info[16]*0x010000 + (unsigned char)bus_card_info[15]*0x01000000;
			bus_current_amo=(unsigned char)bus_card_info[22] + (unsigned char)bus_card_info[21]*0x0100 + (unsigned char)bus_card_info[20]*0x010000 + (unsigned char)bus_card_info[19]*0x01000000;
			
		}
		else
		{
			bus_old_amo=(unsigned char)bus_card_info[11] + (unsigned char)bus_card_info[12]*0x0100 + (unsigned char)bus_card_info[13]*0x010000 + (unsigned char)bus_card_info[14]*0x01000000;
			bus_charge_amo=(unsigned char)bus_card_info[15] + (unsigned char)bus_card_info[16]*0x0100 + (unsigned char)bus_card_info[17]*0x010000 + (unsigned char)bus_card_info[18]*0x01000000;
			bus_current_amo=(unsigned char)bus_card_info[19] + (unsigned char)bus_card_info[20]*0x0100 + (unsigned char)bus_card_info[21]*0x010000 + (unsigned char)bus_card_info[22]*0x01000000;

		}

		sprintf(content+strlen(content), "\n卡原有金额：%d.%02d元", bus_old_amo/100, bus_old_amo%100);
		sprintf(content+strlen(content), "\n卡充入金额：%d.%02d元", bus_charge_amo/100, bus_charge_amo%100);
		sprintf(content+strlen(content), "\n卡现有金额：%d.%02d元", bus_current_amo/100, bus_current_amo%100);
		n = 3;
		break;
	}
	strcat(content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	n++;
	strcat(content,advertisement);
	n += adv_count;


	//获取合计金额
	bbzero(tal);
	tool.chgToMny(gbl.amount, tal, 0);
//	strcat(content,tal);
	
	tool.chgToMny(gbl.amount, amoMny, 0);
	memset(temp,0,sizeof(temp));
//	sprintf(temp,"(实扣：%s元)",amoMny);
//	strcat(content,temp);
	//转换为大写金额
	memset(capMny,0,sizeof(capMny));
	tool.chgToCap(tal,capMny);	
// 	strcat(content,"\n\n\n\n");
// 	strcat(content,capMny);
// 	strcat(content,"\n\n\n     ");
// 	strcat(content,gbl.pos_no);	
	/*wrLog("content:%s",content);*/	
	////////////////////////////////////////////////////
	if(!memcmp(gbl.card_no, "8828", 4))
	{
		bbzero(tmp_card_no);
		memcpy(tmp_card_no, "888888", 6);
	}
	else
	{
		bbzero(tmp_card_no);
		memcpy(tmp_card_no, gbl.card_no, gbl.card_no_len);
		for(int i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//不显示完整银行卡号
			tmp_card_no[i] = '*';
	}
	memset(pMsg,0,sizeof(pMsg));
// 	sprintf((char *)pMsg,"\n\n\n\n%17s  %s\n\n\n\n       %s\n    %s\n    %s\n         %s\n\n\n      %s\n\n%s", 
// 		date, tim, bus_card_no_10, tmp_card_no, gbl.bus_serial, gbl.stan, title, content);
	strcpy(prtTemp.date,date);
	strcpy(prtTemp.tim,tim);
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);
	prtTemp.n = n;
	strcpy(prtTemp.phone,bus_card_no_10);
	strcpy(prtTemp.card,tmp_card_no);
	strcpy(prtTemp.serial,gbl.bus_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.content,content);
	strcpy(prtTemp.type,title);
	strcpy(prtTemp.total,tal);
	strcpy(prtTemp.amo,amoMny);
	strcpy(prtTemp.capsTotal,capMny);
	strcpy(prtTemp.pos,gbl.pos_no);
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

/////////////////////////////////东莞公交/////////////////////////////////////////

int CPrint::print_dg_adsl_xc(const char *title)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.name,gbl.user_name);	
	strcpy(prtTemp.phone,gbl.phone_no);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no, (char *)pack.FIELD37, 12);//读入系统参考号
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,title);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	if (memcmp(gbl.adsl_xc_type,"86431",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1350包15个月512K（个人后付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86443",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1400包14个月2M （个人后付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86434",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1800包12个月3M（个人后付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86433",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1950包15个月2M（个人后付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86422",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1350包15个月512K（个人预付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86445",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1400包14个月2M （个人预付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86420",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1800包12个月3M（个人预付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86423",5)==0)
	{
		sprintf(content,"套餐续存类型：\n存1950包15个月2M（个人预付费）\n订单号：%s\n",gbl.adsl_xc_code);
	}else 
		return -1;
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	prtTemp.n = 3;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}
int CPrint::print_new_dg_adsl_xc(const char *title)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.name,gbl.user_name);	
	strcpy(prtTemp.phone,gbl.phone_no);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no, (char *)pack.FIELD37, 12);//读入系统参考号

	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,title);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	sprintf(prtTemp.content,"用户姓名：%s\nADSL接入号：%s\n套餐名称：%s\n套餐费用：%s\n手机号：%s\n",gbl.user_name,gbl.adsl_number,amoMny,gbl.telphone);
					
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	prtTemp.n = 5;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}
//超爽电信双选择售卡打印函数
int CPrint::MyCoolPayEasy_print_card_detail(const char *type)	
{
	int iPrintLen, iRet, i;
	BYTE pMsg[1024];
	char cardMny[15],amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	char eff_yymmdd[8+1]; //有效日期
	char xm_serial[30+1],card_no[20+1],card_pwd[20+1];
	PrintTemp prtTemp;

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	//提取打印数据
	bbzero(gbl.card_num);
	bbzero(gbl.card_pwd);
	bbzero(gbl.card_serial);
	bbzero(gbl.amount);	
	bbzero(eff_yymmdd);
	bbzero(xm_serial);
	bbzero(card_pwd);
	bbzero(card_no);
	if( rdField63_3(gbl.card_num, gbl.card_pwd, gbl.card_serial, gbl.amount, eff_yymmdd,xm_serial) )
		return -1;
	bbzero(yymmdd);	//日期
	bbzero(hhmmss);	//时间
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(cardMny);	//充值面值
	CTools::chgToMny(gbl.card_amount, cardMny);

	bbzero(amoMny);  //金额
	CTools::chgToMny(gbl.amount, amoMny);

	//填写打印模板
	memset(&prtTemp,0,sizeof(prtTemp));
	if(!memcmp(gbl.fee_type2,"0182",4)){
		sprintf(prtTemp.stan,"#%s",gbl.phone_no);
	}
    else
	    memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);//读入系统参考号
	CTools::formatDate(yymmdd, prtTemp.date);	//日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");	//计数
	strcpy(prtTemp.amo, amoMny);			//实扣
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//合计
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//转换为大写金额
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//不显示完整银行卡号
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);		//终端流水
	strcpy(prtTemp.pos, gbl.pos_no);		//终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	sprintf(prtTemp.phone, "%s(卡序列号)", gbl.card_serial);//卡号
	if (atoi(gbl.fee_type)==245)
	{
		strcpy(prtTemp.type, gbl.xmcard_name);
	}else
		strcpy(prtTemp.type, type);  //类型
	//填写打印明细
	prtTemp.n=0;

	sprintf(prtTemp.content, "有效日期:%s", eff_yymmdd);
	strcat(prtTemp.content, "  面值:");
	strcat(prtTemp.content, cardMny);
	strcat(prtTemp.content, "\n");
	prtTemp.n++;

		

	switch(atoi(gbl.sell_card_type))
	{
	case 136:	//电信充值付费卡
	case 139:
	case 135:
	case 142:
	case 143:
	case 144:
	case 128:
	case 129:
	case 130:
	case 137:
		strcat(prtTemp.content, "帐号:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "系统已经自动为您进行充值，请查询余额，如果自动充值失败，请请拨打11888进行手动充值。\n");
		strcat(prtTemp.content, "“交费易”客服热线400-80-11888\n");
		prtTemp.n += 7;
		break;
	}

	strcat(prtTemp.content, "\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType );	
	
	wrLog("开始打印……");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func_safe((char*)pMsg,iPrintLen);
	wrLog("【print_func_safe】iRet = %d", iRet);
	return iRet;
}

int CPrint::print_yct_detail(const char *type, char *priCont)//羊城通充值打印
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	PrintTemp prtTemp;   //打印模板
	
	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::getTime(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.phone, gbl.card_num);
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//系统参考号				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细
	bbzero(prtTemp.type);
	strcpy(prtTemp.type, type);
	bbzero(prtTemp.content);
	strcpy(prtTemp.content, priCont);
	char *pPriCont=priCont;
	for(prtTemp.n=0; *pPriCont!=(char)0x00; pPriCont++)
	{
		if(*pPriCont=='\n')
		{
			prtTemp.n++;
		}
		else
		{
			NULL;
		}
	}
		
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证\n客服电话400-80-11888");
	prtTemp.n += 2;
	
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");
	

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

int CPrint::print_SZDXTrade_detail(char*qf_amount,char*ss_amount)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.name,gbl.user_name);	
	strcpy(prtTemp.phone,gbl.phone_no);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"电信后付费");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.stan,"000086192633");
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	sprintf(content,"欠费金额：%s\n实时金额：%s\n",qf_amount,ss_amount);
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	prtTemp.n = 2;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_credit_payment(char*credit_no)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15],credit_no_buff[50+1];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"信用卡还款");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatDate(yymmdd, prtTemp.month);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	bbzero(credit_no_buff);
	memcpy(credit_no_buff,credit_no,strlen(credit_no));
	for(i=strlen(credit_no)-2;i>strlen(credit_no)-5;i--)	//不显示完整银行卡号
			credit_no_buff[i] = '*';
	strcpy(prtTemp.phone,credit_no_buff);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	sprintf(content,"信用卡卡号：%s\n借记卡卡号：%s\n\n还款金额：%s\n\n本单据仅供参考，不作结算依据\n此票不作报销凭证\n",credit_no_buff,prtTemp.card,amoMny);
	strcpy(prtTemp.content,content);
	prtTemp.n = 5;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_sell_phone(char *title)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);

	if(strcmp(title, "联通空中放号") == 0)
	{
		strcpy(prtTemp.type,"中国联通空中放号");	
	}
	else
	{
		strcpy(prtTemp.type,"中国电信3G放号");	
	}
	
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatDate(yymmdd, prtTemp.month);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	if(strcmp(title, "联通空中放号") == 0)
	{
		sprintf(content,"选定号码：%s\n价格：%s",gbl.phone_no,amoMny);
	}
	else
	{
		sprintf(content,"入网城市；东莞；\n套餐类型：天翼华厦风；\n选定号码：%s；\n价格：%s；",gbl.phone_no,amoMny);
	}

	strcpy(prtTemp.content,content);
	prtTemp.n = 5;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}


int CPrint::print_telaffair_preaccept(int tradeType, const char* title)
{
	
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan, "8405");
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos, gbl.pos_no);
	strcpy(prtTemp.type,"电信业务预受理");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatDate(yymmdd, prtTemp.month);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, "60011022", 8);
		for(i=6;i>3;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	if(tradeType == 1){
		strcpy(prtTemp.content, title);
		prtTemp.n = 2;
	}
	else{
		strcpy(prtTemp.content, title);
		prtTemp.n = 2;
	}
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_pep_detail(long print_fee, int print_type)
{
	int iPrintLen=0 , iRet=0, i=0, j=0, k=0, line=0, print_done = 1, max_num = 0;
	long totalfee, billfee, payfee, remainfee;
	char temp[1024], pMsg[2048], item[30], fee[20], mny[20], timeto[30], yymmdd[100], year[5], month[3], day[3];
	char user_name[40], stan_no[30], phone_no[20];
	char content[10][512];
	char temp_fee[20];

	remainfee = atol(gbl.amount);
	payfee = atol(gbl.amount);

	for(i = 0; i < gbl.bill_list_len; i++)
	{
		if(gbl.bill_list[i].print_flag != 0)
			continue;

		bbzero(pMsg);
		bbzero(temp);
		max_num = 0;

		strcpy(temp, gbl.bill_list[i].month);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		bbzero(timeto);
		memcpy(year, temp, 4);
		memcpy(month, temp+4, 2);
		memcpy(day, temp+6, 2);
		sprintf(timeto, "%s%s01-%s%s%s", year, month, year, month, day);//计费周期
		
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);

		strcpy(user_name, gbl.bill_list[i].username);//用户名
		strcpy(stan_no, gbl.stan_no);//流水号
		strcpy(phone_no, gbl.phone_no);//电话号码
		strcat(user_name, "(户名)");//追加户名
		strcat(stan_no, "(流水号)");//追加流水号
		strcat(phone_no, "(号码)");//追加电话号码
		strcat(timeto, "(计费周期)");//追加计费周期
		
		bbzero(temp);
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s%-14s%-15s\n%-18s%-30s%-14s%-30s\n\n", " ",user_name, " ", stan_no, " ", phone_no, " ", timeto);
		strcpy(pMsg, temp); 
		bbzero(temp);//初始化temp
		
		long ii;
		struct tm *tim;
		time(&ii);
		tim=localtime(&ii);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		sprintf(year, "%04d", tim->tm_year+1900);
		sprintf(month, "%02d", tim->tm_mon+1);
		sprintf(day, "%02d", tim->tm_mday);
		
		bbzero(yymmdd);
		sprintf(yymmdd,"%-18s%04s年%02s月%02s日(缴费日期)", " ",  year, month, day);//缴费时间
		
		sprintf(temp, "%s\n\n", yymmdd);
		strcat(pMsg, temp);

		bbzero(yymmdd);
		strcpy(yymmdd, gbl.bill_list[i].month);

		memset(&content, 0, sizeof(content));
		for(j = 0; j < 8; j++) //打印8行
		{
			bbzero(temp);
			sprintf(temp, "%10s", " ");
			strcat(content[j], temp);
		}

		line = 0;
		totalfee = 0;

		for(j = i; j < gbl.bill_list_len; j++)
		{
			if(strcmp(yymmdd, gbl.bill_list[j].month) || gbl.bill_list[j].print_flag != 0)
				continue;

			gbl.bill_list[j].print_flag = 1;
			billfee = 0;
			for(k = 0; k < gbl.bill_list[j].item_num; k++)
			{
				bbzero(item);
				bbzero(fee);
				bbzero(mny);
				strcpy(item, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].item_name);
				strcpy(fee, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].fee);
				totalfee += atol(fee);
				billfee += atol(fee);
				CTools::chgToMny(fee, mny, 0);
				if(strlen(item) > 16)
				{
					bbzero(temp);
					strcpy(temp,  item);
					bbzero(item);
					memcpy(item, temp, 13);
					strcat(item, "…");
				}

				bbzero(temp);
				sprintf(temp, "%-18s%-14s", item, mny);
				strcat(content[line], temp);
				line++;
				if(++max_num > 24)
				{
					break;
				}
				if(line > 7)
					line = 0;
			}
			if(max_num > 24)
			{
				break;
			}

		}

		if(max_num > 24)
		{
			break;
		}

		for(j = 0; j < 8; j++)
		{
			strcat(pMsg, content[j]);
			strcat(pMsg, "\n");
		}

		strcat(pMsg, "\n\n");
		

		//这里分叉，单发票或者多发票，多发牌分为最后一张与非最后一张
		//遍历账单，如果都已打印，即此为最后一张发票
		//最后一张发票的实收金额为交费金额-前面发票的总金额
		print_done = 1;
		for(j = 0; j < gbl.bill_list_len; j++)
		{
			if(gbl.bill_list[j].print_flag == 0)
			{
				print_done = 0;
				break;
			}
		}
		
		if(print_done == 1)	//最后一张
		{
			bbzero(fee);
			bbzero(temp);
			CTools::longToMny(totalfee, fee);
			sprintf(temp,"%10s"," ");
			strcat(temp, "本月应收：");
			strcat(temp, fee);
			strcat(temp, "元");		
			strcat(pMsg,temp);

			bbzero(fee);
			bbzero(temp);
			bbzero(temp_fee);
			if(remainfee < 0)
				remainfee = 0;
			CTools::longToMny(remainfee, fee);
			sprintf(temp,"%3s"," ");
			strcat(temp, "本月实收：");
			strcat(temp, fee);
			strcat(temp, "元");
			CTools::chgToCap(fee, temp_fee);
			strcat(temp, "   发票金额(大写)：");
			strcat(temp, temp_fee);
			strcat(pMsg,temp);

		}
		else	//非最后一张
		{
			bbzero(temp);
			bbzero(temp_fee);
			sprintf(temp,"%3s"," ");
			strcat(temp, "本月实收：");
			strcat(temp, fee);
			strcat(temp, "元");
			CTools::chgToCap(fee, temp_fee);
			strcat(temp, "   发票金额(大写)：");
			strcat(temp, temp_fee);

			char sum_temp[50];
			bbzero(sum_temp);
			bbzero(fee);
			bbzero(temp);
			sprintf(temp,"%10s"," ");
			CTools::longToMny(totalfee, fee);
			strcat(sum_temp, "本月应收");
			strcat(sum_temp, fee);
			strcat(sum_temp, "元");		
			strcat(pMsg,sum_temp);
			strcat(pMsg,temp);
			

		}

		//更新remainfee
		remainfee -= totalfee;
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("打印数据：\n%s", pMsg);
	
		iRet = print_func(pMsg,iPrintLen);
	}

	if(gbl.bill_list_len <=0 || max_num > 24)
	{
		bbzero(timeto);
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		strcpy(user_name, gbl.user_name);
		strcpy(stan_no, gbl.stan_no);
		strcpy(phone_no, gbl.phone_no);
		strcat(user_name, "(户名)");
		strcat(stan_no, "(流水号)");
		strcat(phone_no, "(号码)");
		strcpy(timeto, "(计费周期)");
		
		bbzero(temp);
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s%-14s%-15s\n%-18s%-30s%-14s%-30s\n\n", " ",user_name, " ", stan_no, " ", phone_no, " ", timeto);
		strcpy(pMsg, temp);
		bbzero(temp);
		
		long ii;
		struct tm *tim;
		time(&ii);
		tim=localtime(&ii);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		sprintf(year, "%04d", tim->tm_year+1900);
		sprintf(month, "%02d", tim->tm_mon+1);
		sprintf(day, "%02d", tim->tm_mday);
		
		bbzero(yymmdd);
		sprintf(yymmdd,"%-18s%04s年%02s月%02s日(缴费日期)", " ",  year, month, day);
		
		sprintf(temp, "%s\n\n", yymmdd);
		strcat(pMsg, temp);
		strcat(pMsg, "\n\n\n\n\n\n\n\n\n\n");

		bbzero(temp);
		sprintf(temp,"%10s"," ");
		strcat(temp, "明细合计：0.00元");
		strcat(pMsg,temp);
		
		bbzero(fee);
		bbzero(temp);
		if(remainfee < 0)
			remainfee = 0;

		CTools::longToMny(remainfee, fee);
		sprintf(temp,"%3s"," ");
		strcat(temp, "本月实收：");
		strcat(temp, fee);
		strcat(temp, "元");
		CTools::chgToCap(fee, temp_fee);
		strcat(temp, "   人民币合计(大写)：");
		strcat(temp, temp_fee);
		strcat(pMsg,temp);
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("打印数据：\n%s", pMsg);

		iRet = print_func(pMsg,iPrintLen);
		
		return iRet;
	}

	return iRet;
}

int CPrint::print_huizhou_pep_detail(long print_fee, int print_type)
{
	int iPrintLen=0 , iRet=0, i=0, j=0, k=0, line=0, print_done = 1, max_num = 0;
	long totalfee, billfee, payfee, remainfee;
	char temp[1024], pMsg[2048], item[30], fee[20], mny[20], timeto[30], yymmdd[100], year[5], month[3], day[3];
	char user_name[40], stan_no[30], phone_no[20];
	char content[10][512];
	char temp_fee[20];
	char payCircle[20];


	remainfee = atol(gbl.amount);

	payfee = atol(gbl.amount);

	for(i = 0; i < gbl.bill_list_len; i++)
	{
		if(gbl.bill_list[i].print_flag != 0)
			continue;

		bbzero(pMsg);
		bbzero(temp);
		max_num = 0;

		strcpy(temp, gbl.bill_list[i].month);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		bbzero(timeto);
		memcpy(year, temp, 4);
		memcpy(month, temp+4, 2);
		memcpy(day, temp+6, 2);
		sprintf(timeto, "%s%s01-%s%s%s", year, month, year, month, day);//计费周期
		
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		bbzero(payCircle);

		strcpy(user_name, "客户姓名:");//追加户名
		strcpy(phone_no, "客户号码:");//追加电话号码
		strcpy(payCircle, "计费周期:");//追加计费周期
		strcpy(stan_no, "流水号:");//追加流水号
		strcat(user_name, gbl.bill_list[i].username);//用户名
		strcat(stan_no, gbl.stan_no);//流水号
		strcat(phone_no, gbl.phone_no);//电话号码
		strcat(payCircle, timeto);//计费周期

		long ii;
		struct tm *tim;
		time(&ii);
		tim=localtime(&ii);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		sprintf(year, "%04d", tim->tm_year+1900);
		sprintf(month, "%02d", tim->tm_mon+1);
		sprintf(day, "%02d", tim->tm_mday);
		
		bbzero(yymmdd);
		sprintf(yymmdd,"%-18s%04s年%02s月%02s日(缴费日期)", " ",  year, month, day);//缴费时间

		bbzero(temp);
	
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s\n", " ",yymmdd); //缴费时间
		strcpy(pMsg, temp); 
		
		bbzero(temp);
		sprintf(temp, "%-18s%-30s\n%-18s%-30s\n%-18s%-30s\n%-18s%-30s\n\n", " ",user_name, " ", phone_no, " ", stan_no, " ", payCircle);
		strcat(pMsg, temp);
		
		bbzero(yymmdd);
		strcpy(yymmdd, gbl.bill_list[i].month);

		memset(&content, 0, sizeof(content));
		for(j = 0; j < 8; j++) //打印8行
		{
			bbzero(temp);
			sprintf(temp, "%18s", " ");
			strcat(content[j], temp);
		}

		line = 0;
		totalfee = 0;

		for(j = i; j < gbl.bill_list_len; j++)
		{
			if(strcmp(yymmdd, gbl.bill_list[j].month) || gbl.bill_list[j].print_flag != 0)
				continue;

			gbl.bill_list[j].print_flag = 1;
			billfee = 0;
			for(k = 0; k < gbl.bill_list[j].item_num; k++)
			{
				bbzero(item);
				bbzero(fee);
				bbzero(mny);
				strcpy(item, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].item_name);
				strcpy(fee, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].fee);
				totalfee += atol(fee);
				billfee += atol(fee);
				CTools::chgToMny(fee, mny, 0);
				if(strlen(item) > 16)
				{
					bbzero(temp);
					strcpy(temp,  item);
					bbzero(item);
					memcpy(item, temp, 13);
					strcat(item, "…");
				}

				bbzero(temp);
				sprintf(temp, "%-20s%-25s", item, mny);
				strcat(content[line], temp);
				line++;
				if(++max_num > 24)
				{
					break;
				}
				if(line > 7)
					line = 0;
			}

			if(max_num > 24)
			{
				break;
			}

		for(j = 0; j < 8; j++)
		{
			strcat(pMsg, content[j]);//将content 数组中数据打印出来。
			strcat(pMsg, "\n");
		}

		strcat(pMsg, "\n\n\n");
		
		//这里分叉，单发票或者多发票，多发牌分为最后一张与非最后一张
		//遍历账单，如果都已打印，即此为最后一张发票
		//最后一张发票的实收金额为交费金额-前面发票的总金额
		print_done = 1;
		for(j = 0; j < gbl.bill_list_len; j++)
		{
			if(gbl.bill_list[j].print_flag == 0)
			{
				print_done = 0;
				break;
			}
		}
		
		if(print_done == 1)	//最后一张
		{
			//本月应收
			bbzero(fee);
			bbzero(temp);
			CTools::longToMny(totalfee, fee);
			bbzero(temp);
			sprintf(temp, "%18s", " ");
			strcat(temp, "本月应收:");
			strcat(temp, fee);
			strcat(temp, "元");		
			strcat(pMsg,temp);

			bbzero(fee);
			bbzero(temp);
			if(remainfee < 0)
				remainfee = 0;
			CTools::longToMny(remainfee, fee);
			sprintf(temp,"%3s"," ");
			strcat(temp, "本月实收：");
			strcat(temp, fee);
			strcat(temp, "元");
			CTools::chgToCap(fee, temp_fee);
			strcat(temp, "   发票金额(大写)：");
			strcat(temp, temp_fee);
			strcat(pMsg,temp);
		}
		else	//非最后一张
		{

			char sum_temp[100];
			bbzero(sum_temp);

			sprintf(temp, "%3s", " ");
			strcat(sum_temp, "本月实收：");
			strcat(sum_temp, fee);
			strcat(sum_temp, "元");
			CTools::chgToCap(fee, temp_fee);
			strcat(sum_temp, "   发票金额(大写)：");
			strcat(sum_temp, temp_fee);

			bbzero(fee);
			bbzero(temp);
			CTools::longToMny(totalfee, fee);
			bbzero(temp);
			sprintf(temp, "%18s", " ");
			strcat(temp, "本月应收:");
			strcat(temp, fee);
			strcat(temp, "元");		
			strcat(pMsg,temp);
			strcat(pMsg,sum_temp);

		}

		//更新remainfee
		remainfee -= totalfee;
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("打印数据：\n%s", pMsg);
	
		iRet = print_func(pMsg,iPrintLen);
	}

	}
	//如果没有账单记录则执行下面的代码
	if(gbl.bill_list_len <=0 || max_num > 24)
	{


		bbzero(year);
		bbzero(month);
		bbzero(day);
		memcpy(year, temp, 4);
		memcpy(month, temp+4, 2);
		memcpy(day, temp+6, 2);
		bbzero(stan_no);
		bbzero(phone_no);
		
		strcpy(phone_no, "客户号码:");//追加电话号码
		strcpy(stan_no, "流水号:");//追加流水号
		strcat(stan_no, gbl.stan_no);//流水号
		strcat(phone_no, gbl.phone_no);//电话号码
		
		long ii;
		struct tm *tim;
		time(&ii);
		tim=localtime(&ii);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		sprintf(year, "%04d", tim->tm_year+1900);
		sprintf(month, "%02d", tim->tm_mon+1);
		sprintf(day, "%02d", tim->tm_mday);
		
		bbzero(yymmdd);
		sprintf(yymmdd,"%-18s%04s年%02s月%02s日(缴费日期)", " ",  year, month, day);//缴费时间
		
		bbzero(temp);
		
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s\n", " ",yymmdd); //缴费时间
		strcpy(pMsg, temp); 
		bbzero(temp);
		sprintf(temp, "%-18s%-30s\n%-18s%-30s\n\n\n", " ", phone_no, " ", stan_no);
		strcat(pMsg, temp);

		strcat(pMsg, "\n\n\n\n\n\n\n\n\n\n\n");
		bbzero(temp);
		sprintf(temp,"%18s"," ");
		strcat(temp, "明细合计：0.00元");
		strcat(pMsg,temp);

		bbzero(fee);
		bbzero(temp);
		if(remainfee < 0)
			remainfee = 0;

		CTools::longToMny(remainfee, fee);
		sprintf(temp,"%3s"," ");
		strcat(temp, "合计：");
		strcat(temp, fee);
		strcat(temp, "(实收)");
		CTools::chgToCap(fee, temp_fee);
		strcat(temp, "   发票金额(大写)：");
		strcat(temp, temp_fee);
		strcat(pMsg,temp);
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("打印数据：\n%s", pMsg);

		iRet = print_func(pMsg,iPrintLen);
		
		return iRet;
	}

	return iRet;
}


int CPrint::print_new_pep_detail(long print_fee, int print_type) //新版发票(清远市开始使用)
{
	int iPrintLen=0 , iRet=0, i=0, j=0, k=0, line=0, print_done = 1, max_num = 0;
	long totalfee, billfee, payfee, remainfee;
	char temp[1024], pMsg[2048], item[30], fee[20], mny[20], timeto[30], yymmdd[100], year[5], month[3], day[3];
	char prefee[20];
	char user_name[40], stan_no[30], phone_no[20],card_no[31];
	char content[10][512];//来显示账单信息的数组

	remainfee = atol(gbl.amount);
	payfee = atol(gbl.amount); //缴费金额
	bbzero(prefee);
	strcpy(prefee,"0.00");//初始化prefee

	for(i = 0; i < gbl.bill_list_len; i++)
	{
		if(gbl.bill_list[i].print_flag != 0)
			continue;

		bbzero(pMsg);
		bbzero(temp);
		max_num = 0; //初始化最大数字是0

		long ii;
		struct tm *tim;
		time(&ii);
		tim=localtime(&ii);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		sprintf(year, "%04d", tim->tm_year+1900);
		sprintf(month, "%02d", tim->tm_mon+1);
		sprintf(day, "%02d", tim->tm_mday);
		
		bbzero(yymmdd);
		sprintf(yymmdd,"%-20s%04s年%02s月%02s日", " ",  year, month, day);
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%\n\n",yymmdd);//缴费日期
		strcpy(pMsg,temp);
	
		bbzero(temp);
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		bbzero(card_no);
		strcpy(user_name, gbl.bill_list[i].username);//用户名
		strcpy(stan_no, gbl.stan_no);//流水号
		strcpy(phone_no, gbl.phone_no);//电话号码

		bbzero(temp);
		strcpy(temp, gbl.bill_list[i].month);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		bbzero(timeto);
		memcpy(year, temp, 4);//获取年
		memcpy(month, temp+4, 2);//获取月
		memcpy(day, temp+6, 2);//获取日
		sprintf(timeto, "%s-%s-01: %s-%s-%s", year, month, year, month, day);//计费周期
		

		//输出用户名，流水号，号码，及计费周期.开户银行和银行账号暂时留空。
		/*sprintf(temp, "%-20s%-30s%-25s%-30s\n\n\n\n%-20s%-30s%-25s%-30s\n\n\n\n\n", " ",user_name, " ", phone_no," ",timeto," ",stan_no);*/
		
		//修改后
		sprintf(temp, "%-15s%-30s%-20s%-30s\n%-15s%-30s%-20s%-30s\n\n", "客户名称:",user_name, "客户号码:", phone_no,"计费周期:",timeto,"系统流水号:",stan_no);
		strcat(pMsg,temp); 
		
		bbzero(yymmdd);
		strcpy(yymmdd,gbl.bill_list[i].month);
		memset(&content, 0, sizeof(content));
		
		for(j = 0; j < 8; j++)  //打印8行
		{
			bbzero(temp);
			sprintf(temp, "%10s", " ");
			strcat(content[j], temp);
		}

		line = 0;//总行数
		totalfee = 0;

		for(j = i; j < gbl.bill_list_len; j++)
		{
			if(strcmp(yymmdd, gbl.bill_list[j].month) || gbl.bill_list[j].print_flag != 0)
				continue;
			
			gbl.bill_list[j].print_flag = 1;
			billfee = 0;		
			for(k = 0; k < gbl.bill_list[j].item_num; k++)
			{
				bbzero(item);
				bbzero(fee);
				bbzero(mny);
				
				strcpy(item, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].item_name);//获取k项的项目名称
				strcpy(fee, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].fee);//获取k项的项目费用 
				

				if (strcmp(item,"预付金")==0)//如果第k 项是预付金
				{

				CTools::chgToPreMny(fee,prefee);//将预付金转换成正数.
				continue;//不在计入账单总计金额中。

				}

					
				totalfee += atol(fee);
				billfee += atol(fee);
				CTools::chgToMny(fee, mny, 0); //将金额转换成元为单位.

				//显示列表中的内容.
				
				if(strlen(item) > 16) //显示不出具体内容
				{
					bbzero(temp);
					strcpy(temp,  item);
					bbzero(item);
					memcpy(item, temp, 13);
					strcat(item, "…");
				}

	
				bbzero(temp);

				if(line==0)
				{
					sprintf(temp,"%-20s%-25s","项  目","金额（元）");
					strcat(content[0],temp);
					line++;	
				}

				bbzero(temp);
				//sprintf(temp, "%-30s%-25s", item, mny); 
				//modify 2011-12-5
				sprintf(temp, "%-20s%-25s", item,mny); 
				//end modify

				strcat(content[line], temp);
				line++;
				
				if(++max_num > 16)
				{
					
					break;
				}

				if(line>7) //判断是否大于8行
				{
                    line=0;
				}
			}

			if(max_num > 16) //查看最大数为16个

				{
				wrLog("查看最大数:%d",max_num);
				break;

				}

		//此处省略账单小计
		}

		if(max_num > 16)
		{
			break;
		}
		

		for(j = 0; j < 8; j++) //判断是否大于8行
		{
		
			strcat(pMsg, content[j]);//将content 数组中数据打印出来。
			strcat(pMsg, "\n");

		}


		strcat(pMsg, "\n\n\n\n");//换行
		bbzero(fee);
		bbzero(temp);
		CTools::longToMny(totalfee, fee);
		
	    /*sprintf(temp,"%-20s%-10s%-22s%-7s%-18s%-7s"," ",fee," ",prefee," ","0.00");*///明细合计.如果没有预付金，预付金为0.00
	
		//modify 修改为
		sprintf(temp,"%-10s%-7s%-10s%-10s%-7s%-10s%-10s%-5s%-10s","本月合计:",fee,"元","其中预付金支付: ",prefee,"元","银行卡支付: ","0.00","元");
		//end modify
		strcat(pMsg, temp);	

		//这里分叉，单发票或者多发票，多发牌分为最后一张与非最后一张
		//遍历账单，如果都已打印，即此为最后一张发票
		//最后一张发票的实收金额为交费金额-前面发票的总金额
		print_done = 1;
		for(j = 0; j < gbl.bill_list_len; j++)
		{
			if(gbl.bill_list[j].print_flag == 0)
			{
				print_done = 0;
				break;
			}
		}
		
		if(print_done == 1)	//最后一张
		{
			bbzero(fee);
			bbzero(temp);
			if(remainfee < 0)
				remainfee = 0;
            
			CTools::longToMny(remainfee, fee);
			//sprintf(temp,"%-18s%-7s\n\n"," ",fee); //合计
            //modify 2011-12-1
			sprintf(temp,"%-10s%-7s元\n\n","合计:",fee); //合计
            //end modify

			strcat(pMsg,temp);
			
			bbzero(temp);
			CTools::chgToCap(fee, temp);//（实收）大写
			char newTemp[1024];
			bbzero(newTemp);
			//sprintf(newTemp,"%-42s%-20s"," ",temp);
			//modify 2011-12-1
			sprintf(newTemp,"%-20s%-20s","(实收)大写: 人民币 ",temp);
			//end modify

			strcat(pMsg, newTemp);
			
			bbzero(temp);
			//sprintf(temp,"%-30s%-20s"," ",fee);
			//modify 2011-12-1
			sprintf(temp,"%-20s%-20s","(实收)小写: ￥ ",fee);
			//end modify
			strcat(pMsg,temp);

		}
		else	//非最后一张
		{
			bbzero(temp);
			//sprintf(temp,"%-12s%-2s\n\n"," ",fee);//合计
            //modify 2011-12-5
			sprintf(temp,"%-8s%-2s元\n\n","合计:",fee);//合计
			//end modify
			strcat(pMsg,temp);

			bbzero(temp);
			CTools::chgToCap(fee, temp);//大写(实收)
			char newTemp[1024];
			//sprintf(newTemp,"%-42s%-20s"," ",temp);
			//modify 2011-12-1
			sprintf(newTemp,"%-20s%-20s","大写(实收): 人民币 ",temp);
			//end modify
			strcat(pMsg, newTemp);

			bbzero(temp);
			//sprintf(temp,"%-25s%-20s"," ",fee);//小写（实收）
			//modify 2011-12-1
			sprintf(temp,"%-20s%-20s","小写(实收): ￥ ",fee);
			//end modify
			strcat(pMsg,temp);

		}
		
		//更新remainfee
		remainfee -= totalfee;
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("打印数据：\n%s", pMsg);
	
		iRet = print_func(pMsg,iPrintLen);
	}

	//如果没有账单记录则执行下面的代码
	if(gbl.bill_list_len <=0 || max_num > 30)
	{
		
		bbzero(timeto);
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		strcpy(user_name, gbl.user_name);//账户名称
		strcpy(stan_no, gbl.stan_no);//流水号
		strcpy(phone_no, gbl.phone_no); //号码
		strcpy(timeto," "); //计费周期设置为空

		long ii;
		struct tm *tim;
		time(&ii);
		tim=localtime(&ii);
		bbzero(year);
		bbzero(day);
		bbzero(month);
		sprintf(year,"%04d",tim->tm_year+1900);
		sprintf(month,"%02d",tim->tm_mon+1);
		sprintf(day,"%02d",tim->tm_mday);

		bbzero(yymmdd);
		sprintf(yymmdd,"%-20s%04s年%02s月%02s日", " ",  year, month, day);
		sprintf(temp, "\n\n\n\n\n\n\n%-20s%\n\n",yymmdd);//缴费日期
		strcpy(pMsg,temp);
			
		bbzero(temp);
		//输入账户名称/电话号码,计费周期,等
		//sprintf(temp, "%-20s%-30s%-25s%-30s\n\n\n\n%-20s%-30s%-25s%-30s\n\n\n\n", " ",user_name, " ", phone_no," "," "," ",stan_no);
		//修改后
		sprintf(temp, "%-15s%-30s%-20s%-30s\n%-15s%-30s%-20s%-30s\n", "客户名称:",user_name, "客户号码:", phone_no,"计费周期:",timeto,"系统流水号:",stan_no);
		strcat(pMsg,temp);
		
		strcat(pMsg, "\n\n\n\n\n\n\n\n\n\n\n\n");//转入计费统计栏

		bbzero(fee);
		bbzero(temp);
		if(remainfee < 0)
			remainfee = 0;

		CTools::longToMny(remainfee, fee); 
		bbzero(temp);
	    //sprintf(temp,"%-78s%-7s%-14s%-7s\n\n"," ","0.00"," ",fee);//现金支付
		sprintf(temp,"%-15s%-10s元%-10s%-10s元\n\n","银行卡支付:","0.00","现金支付: ",fee);//现金支付
		strcat(pMsg,temp);

		bbzero(temp);
		CTools::chgToCap(fee, temp);
		char newTemp[1024];
		bbzero(newTemp);
		sprintf(newTemp,"%-10s%-20s","大写(实收): 人民币 ",temp);//大写(实收)
		strcat(pMsg, newTemp);

		bbzero(temp);
		sprintf(temp,"%-10s%-20s","小写(实收): ￥ ",fee); //小写(实收)
		strcat(pMsg,temp);

		iPrintLen = strlen(pMsg);	
	//	wrLog("打印数据：\n%s", pMsg);

		iRet = print_func(pMsg,iPrintLen);
		//AfxMessageBox(pMsg,MB_OK);
		return iRet;
	}

	return iRet;
}


int CPrint::print_game_recharge(const char* title, const char* message)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //打印模板
	char  amoMny[15];
	BYTE* pMsg = new BYTE[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	long ii;
    struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
	
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type, title);		
	memset(pMsg,0, 1024);
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatDate(yymmdd, prtTemp.month);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	strcpy(prtTemp.content, message);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	delete[] pMsg;
	return iRet;
}

int CPrint::print_coach_ticket_detail(const char* title, const char* message)
{
	int iPrintLen=0 , iRet=0, i=0, k=0;
	PrintTemp prtTemp;   //打印模板
	char  amoMny[15];
	BYTE pMsg[2048];
	

	memset(&prtTemp, 0, sizeof(prtTemp));

	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
		
	long ii;
	struct tm *tim;
	time(&ii);
	tim=localtime(&ii);
	sprintf(yymmdd,"%04d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday);
	sprintf(hhmmss,"%02d%02d%02d",tim->tm_hour,tim->tm_min,tim->tm_sec);
		
	strcpy(prtTemp.count, "第1张/共1张");
	bbzero(prtTemp.name);		
		
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type, title);		
	memset(pMsg,0, 2048);
		
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	//CTools::formatDate(yymmdd, prtTemp.month);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(prtTemp.total);
	bbzero(prtTemp.amo);
	bbzero(prtTemp.capsTotal);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
		
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
		
	strcpy(prtTemp.content, message);
	prtTemp.n = 9;

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
		
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//flag: 0-黑标定位(默认)
//		1-打印测试
void CPrint::printYctBill(char *date, char *normal_amount, char *plus_amount)
{
	char msg[512];
	char cDate[8+1], cTime[8+1];
	int iPrintLen, iRet;
	
	bbzero(cDate);
	bbzero(cTime);
	get_time8(cTime);
	get_date8(cDate);
	memset(msg,0,sizeof(msg));
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	strcpy(msg, "\n\n\n【天翼羊城通送款单登记】\n");
	strcat(msg, "----------------------------\n");
	sprintf(msg+strlen(msg), "终端号：%s\n", gbl.pos_no);
	sprintf(msg+strlen(msg), "交易日期：%s\n", date);
	sprintf(msg+strlen(msg), "正常交易金额：%.2f\n", atof(normal_amount)/100);
	sprintf(msg+strlen(msg), "多收交易金额：%.2f\n", atof(plus_amount)/100);
	strcat(msg, "\n\n\n\n\n");
	wrLog("msgLen:[%d]", strlen(msg));
    
	iPrintLen = strlen(msg);
	iRet = print_func(msg,iPrintLen);
	wrLog("【打印】iRet=%d", iRet);
}

//羊城通充值第三代打印
int CPrint::print_yctthird_detail(const char *type, char *priCont)
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	PrintTemp prtTemp;   //打印模板
	
	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::getTime(hhmmss);
	//金额
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //日期
	CTools::formatTime(hhmmss, prtTemp.tim);	//时间
	strcpy(prtTemp.count, "第1张/共1张");
	strcpy(prtTemp.phone, gbl.card_num);
	//卡号
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整银行卡号
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//系统参考号				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //合计
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	strcpy(prtTemp.pos, gbl.pos_no);				 //终端号
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//打印明细
	bbzero(prtTemp.type);
	strcpy(prtTemp.type, type);
	bbzero(prtTemp.content);
	strcpy(prtTemp.content, priCont);
	char *pPriCont=priCont;
	for(prtTemp.n=0; *pPriCont!=(char)0x00; pPriCont++)
	{
		if(*pPriCont=='\n')
		{
			prtTemp.n++;
		}
		else
		{
			NULL;
		}
	}
		
	memset(pMsg,0,sizeof(pMsg));
	wrLog("开始打印……");

	iPrintLen = set_yctthird_content(&prtTemp, (char*)pMsg);
	iRet = print_func((char*)pMsg, iPrintLen);

	return iRet;
}

int CPrint::set_yctthird_content(PPrintTemp prt, char *cont)
{

	int len = 0, i;
	char enter[20] = "\n\n\n\n", nameEnter[5];	//用户名与用户号码的换行

	bbzero(nameEnter);
	if(strlen(prt->name) > 37)
		strcpy(nameEnter, "\n");
	else
		strcpy(nameEnter, "\n\n");

	wrLog("prt->n = %d", prt->n);
	for(i=0; i<9 - prt->n; i++)
			strcat(prt->content,"\n");


	sprintf(cont+len, "\n\n\n\n\n\n\n   %17s %s %s\n", prt->date, prt->tim, prt->count);
	len = strlen(cont);
	sprintf(cont+len, "          %s%s\n\n          %s\n            %s\n\n", prt->name, nameEnter, prt->phone, prt->stan);
	len = strlen(cont);
	sprintf(cont+len, "      %s\n\n          %s\n", prt->card, prt->month);
	len = strlen(cont);
				
	sprintf(cont+len, "        %s         \n\n%s", prt->type, prt->content);
	len = strlen(cont);
			
	if (strncmp(prt->type,"羊城通余额",5) != 0)
	{	
		sprintf(cont+len, "        %s（实扣：%s）\n\n             %s\n     ", prt->total, prt->amo, prt->capsTotal);
	}

	len = strlen(cont);
//	wrLog("打印数据：\n%s", cont);
	return len;
}


//打印水电煤
int CPrint::print_sdm_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	bbzero(prtTemp.name);								//用户名
	strcpy(prtTemp.name,gbl.sdm_CUSTOMERNAME);			//用户名
	strcpy(prtTemp.phone, gbl.sdm_SELECTVALUE);			 //用户号码
	bbzero(prtTemp.month);
	strcpy(prtTemp.month,gbl.SDM_BILLMONTH); 

	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}

	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	sprintf(prtTemp.content,"收费单位:%s\n账  期:%s\n客户姓名:%s\n缴费金额:%.2f元",gbl.sdm_PAYMENTNAME,gbl.SDM_BILLMONTH,gbl.sdm_CUSTOMERNAME,atof(gbl.amount)/100);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//打印3G流量卡

int CPrint::print_g3chargeCard_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	strcpy(prtTemp.phone, gbl.phone_no);			 //充值手机号码
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	sprintf(prtTemp.content,"充值手机号:%s\n充值面值:%s元\n充值流量:%sM",gbl.phone_no,amoMny,gbl.RECHARGEFLOW);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}


//打印广禄qq
//打印3G流量卡

int CPrint::print_glQQ_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(stan_no,gbl.TRANSSEQ+(15-strlen(gbl.TRANSSEQ)),12);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	strcpy(prtTemp.phone, gbl.glQQPayInfo.ACCTCODE);			 //充值手机号码
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	
	sprintf(prtTemp.content,"充值号码:%s\n充值金额:%s\n",gbl.glQQPayInfo.ACCTCODE,amoMny);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

//打印东莞通售卡及充值票据打印
//flag:0 是售卡，1是充值 充值暂时不能作为充值凭证 2 表示以旧换新打印发票

int CPrint::print_dgtBus_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,gbl.TRANSSEQ+(strlen(gbl.TRANSSEQ)-12),12);
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	strcpy(prtTemp.phone, gbl.phone_no);			 //充值手机号码
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
	if (strcmp(gbl.fee_type,"0372")==0)
	{

		sprintf(prtTemp.content,"售卡工本费:%.2f元\n应收金额:%.2f元\n实收金额:%.2f元",atof(gbl.amount)/100,atof(gbl.amount)/100,atof(gbl.amount)/100);
	
	}
	else if (strcmp(gbl.fee_type,"0371") ==0) //如果是充值
	{
	
		sprintf(prtTemp.content,"卡原有金额:%.2f元\n卡充入金额:%.2f元\n卡现有金额:%.2f元",atof(gbl.dgtCard.tradeBeForeMny)/100,atof(gbl.amount)/100,atof(gbl.dgtCard.cardMny)/100);
		strcat(prtTemp.content, "\n注: ");
		strcat(prtTemp.content, "本单据仅供参考，不作结算依据\n");
		strcat(prtTemp.content,"\n此票不作报销凭证\n客服电话:400-80-11888");
	}

	else
	{
		sprintf(prtTemp.content,"转出卡卡号:%s\n转入卡卡号:%s\n转入卡原有金额:%.2f元\n转入金额:%.2f元\n转入卡现有金额:%.2f元",gbl.dgtM1Card.bus_showCard,gbl.dgtCard.cardShowId,atof(gbl.dgtCard.tradeBeForeMny)/100,atof(gbl.dgtCard.tradeMny)/100,atof(gbl.dgtCard.cardMny)/100);
		
		strcat(prtTemp.content, "\n注: ");
		strcat(prtTemp.content, "本单据仅供参考，不作结算依据\n");
		strcat(prtTemp.content,"\n此票不作报销凭证\n客服电话:400-80-11888");

	}

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}


//打印全国充值票据
int CPrint::print_phonePay_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	strcpy(prtTemp.phone, gbl.phone_no);			 //充值手机号码
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	sprintf(prtTemp.content,"充值号码:%s",gbl.phone_no);
	//strcat(prtTemp.content,"充值面值:%.2f元",atof(gbl.origAmount)/100);
	//strcat(prtTemp.content,"实收金额:%s",amoMny);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

//打印全国充值票据
int CPrint::print_phonePay_detailForGDYD(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	strcpy(prtTemp.phone, gbl.phone_no);			 //充值手机号码
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //实扣
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额

	int iorigAmount = atoi(gbl.origAmount);
	double dorigAmount = iorigAmount/100;
	sprintf(prtTemp.content,"充值号码:%s \n充值面值:%.2f元",gbl.phone_no,dorigAmount);
	
	//strcat(prtTemp.content,"充值面值:%.2f元",atof(gbl.origAmount)/100);
	//strcat(prtTemp.content,"实收金额:%s",amoMny);
	strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

int CPrint::print_carTax_detail(char *type)
{

	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8位年月日
	char hhmmss[7];		//6位时分秒
	int iPrintLen , i;
	int iRet=0;
	int flag = 0;	//flag = 1，补打发票
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //打印模板
	
	/*----------------获取打印信息--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//获取日期和时间
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);

	/*-----------------填写打印模板------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //日期
	CTools::formatTime(hhmmss, prtTemp.tim);			 //时间
	strcpy(prtTemp.serial, gbl.pos_serial);				//终端流水
	strcpy(prtTemp.count, "第1张/共1张");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//业务类型
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//终端号
	strcpy(prtTemp.phone, gbl.card_no);			 //充值手机号码
	strcpy(prtTemp.name,gbl.user_name);			//用户名
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //卡号
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//不显示完整卡号
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //非发票打印纸不打印卡号及流水号
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	//打印各个时段的车船税明细
	for(int j=0;j<gbl.carTaxBills.nCnt;j++)	//打印的张数	
	{

		char BILLMONTH[10];
		long chargeAmount=0; //应缴费用
		long delayAmount=0; //滞纳金费用
		long punishAmount=0; //罚款费用

		bbzero(BILLMONTH);
		bbzero(prtTemp.content);

		sprintf(BILLMONTH,"%s年",gbl.carTaxBills.cardTaxDatas[j].BILLMONTH);
		chargeAmount=atof(gbl.carTaxBills.cardTaxDatas[j].BILLNO)/100;
		delayAmount=atof(gbl.carTaxBills.cardTaxDatas[j].BILLDELAY)/100;
		punishAmount=atof(gbl.carTaxBills.cardTaxDatas[j].BALANCE)/100;

		sprintf(prtTemp.count,"第%d张/共%d张", j+1, gbl.carTaxBills.nCnt);
		sprintf(prtTemp.content,"缴纳年度:%s\n缴费金额:%.2f元\n滞纳金:%.2f元\n罚款金额:%s元",BILLMONTH,chargeAmount,delayAmount,punishAmount);
		strcat(prtTemp.content,"\n本单据仅供参考，不作结算依据\n此票不作报销凭证");

		CTools::longToMny(chargeAmount,amoMny);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
		strcpy(prtTemp.amo, amoMny);					 //实扣
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //转换为大写金额
		iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
		wrLog("iPrintLen：%d",iPrintLen);//输出打印长度
		iRet = print_func((char*)pMsg,iPrintLen);
		return iRet;
	}

	return iRet;
}