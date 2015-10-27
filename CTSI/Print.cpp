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
	m_prtType = 0;	//��ӡ���ͣ�0,�޴�ӡ; 1, �����״�; 2, �������״�; 3, ��ʽ�״�; 4, ��ʽ���״�
	m_prtCom  = 0;	//��ӡ���ڣ�0,�޴�ӡ����; 1, �󴮿�; 2, �Ҵ���; 3, ���ô���;
}

CPrint::~CPrint()
{

}

void CPrint::set_in_par(CGlobal in_gbl, CPack in_pack) //���ô������
{
	gbl  = in_gbl;
	pack = in_pack;
	getPrintPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &m_prtCom, &m_prtType);
	wrLog("m_prtCom: %d", m_prtCom);
	wrLog("m_prtType: %d", m_prtType);
}

void CPrint::set_in_par(CGlobal in_gbl) //���ô������
{
	gbl  = in_gbl;
	getPrintPar(gbl.posInfo.leftType, gbl.posInfo.rightType, gbl.posInfo.inType, &m_prtCom, &m_prtType);
	wrLog("m_prtCom: %d", m_prtCom);
	wrLog("m_prtType: %d", m_prtType);
}


//��ȡ��ӡ����
//prtCom: ��ӡ����
//prtType:��ӡ����
int CPrint::getPrintPar(unsigned char leftType, unsigned char rightType, unsigned char inType, int *prtCom, int *prtType)
{
	int flag = 0; //��ʼ��
	//���ô���
	if(inType>0 && inType<=4 || inType>=8 && inType<=14)
	{
		*prtType = inType;
		*prtCom  = 0x03;
		flag = 1;
	}
	//�󴮿�
	else if(leftType>0 && leftType<=4 || leftType>=8 && leftType<=14)
	{
		*prtType = leftType;		
		*prtCom  = 0x01;
		flag = 1;
	}
	//�Ҵ���
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

//flag: 0-�ڱ궨λ(Ĭ��)
//		1-��ӡ����
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
		sprintf(msg,"\n\n\n\n\n�ڱ궨λ\n*******************\n%s   %s\n�ն˺ţ�%s\n\n\n\n\n", cDate, cTime, gbl.pos_no);
	else
	{
		strcpy(msg, "\n\n\n����ӡ���ԡ�\n");
		strcat(msg, "----------------------------\n");
		sprintf(msg+strlen(msg), "�ն˺ţ�%s\n", gbl.pos_no);
		sprintf(msg+strlen(msg), "���ڣ�%s\n", cDate);
		sprintf(msg+strlen(msg), "ʱ�䣺%s\n", cTime);
		strcat(msg, "\n\n\n\n\n");
		wrLog("msgLen:[%d]", strlen(msg));
	}
    
	iPrintLen = strlen(msg);
	iRet = print_func(msg,iPrintLen);
	wrLog("����ӡ��iRet=%d", iRet);
}


//��ӡ����
//����ֵ��SUCC �ɹ���FAIL ʧ��
int CPrint::print_func(char *pMsg,int msg_len, int page, int cpyReprint,int acount,int current_count)
{
	int iRet;
	char temp[100];
	bbzero(temp);
// 	if (page == 1)
// 	{
// 		sprintf(temp,"����%d�ŷ�Ʊ���ڴ�ӡ\n     �����ĵȴ�",acount-current_count);
// 	}else
		sprintf(temp,"   ���ڴ�ӡ��....");
	
	iRet = TS_B4Print(gbl.dPosInfo, m_prtCom, m_prtType, page, temp, (BYTE *)pMsg, msg_len);
	if(iRet)	//�����־
		wrMonitorLog(11, 1, iRet, gbl, pack);
	else
		wrMonitorLog(11, 0, 0, gbl, pack);
	if( cpyReprint==1 )
	{

			//bbzero(gbl.stan_no);
			//memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
			CDB::setRePrint(gbl.stan_no,gbl.dPosInfo.pos_no,gbl.card_no,0,pMsg,gbl.tgroup); 
		
	}
	wrLog("����ӡ������ֵ��%d", iRet);
	return iRet;
}
 
//��ȫ��ӡ����
//����ֵ��SUCC �ɹ���FAIL ʧ��
int CPrint::print_func_safe(char *pMsg,int msg_len, int cpyReprint,int acount,int current_count)
{
	int iRet;
	TSB4Print_S printMsg;
	char temp[512];
	bbzero(temp);
//	sprintf(temp,"����%d�ŷ�Ʊ���ڴ�ӡ\n     �����ĵȴ�",acount-current_count);
	memset(&printMsg, 0, sizeof(printMsg));
	sprintf(temp,"   ���ڴ�ӡ��....");
	printMsg.uCardType = CARD_TYPE;	//��
	if(gbl.posInfo.enyFlag)
	{
		printMsg.uKeyIndex = TS_TEK_KEY;		//��ԿTEK
		printMsg.uDesMode = TS_DES_3_TEK_DEC;		//�����㷨
	}
	else
	{
		printMsg.uKeyIndex = 12;		//��Կ���12
		printMsg.uDesMode = TS_DES_2_DOWN;			//�����㷨
	}
	memcpy(printMsg.uPrintData, pMsg, msg_len);
	printMsg.iDataLen = msg_len;

	iRet = TS_B4Print_S(gbl.dPosInfo, m_prtCom, m_prtType, 1, temp, &printMsg);
	if(iRet)	//�����־
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


//���캯��
//��63���ж���ϸ��Ϣ������gbl.detail�ṹ���й���ӡ��
//���������������ͨԤ����
//flag��Ĭ��Ϊ1���ⲿ���ã�0���ڲ�ʹ��
int CPrint::rdField63(CPack pack,CGlobal &out_gbl, int flag)  
{
	int len , i , j,iRet;
	int count=0;
	char temp[20];

	if(flag)
		gbl = out_gbl;

//	wrLog("��������rdField63");
//	wrLog("BIT63:[%s]",pack.FIELD63);
//	strcpy((char *)pack.FIELD63,"A21100523577          ��С��                        20080807299�¹̶���  00000000600099����ͨ�ŷ�00000000882099������    0000000-30081100523577          ��С��                        20080907299�¹̶���  00000000600099����ͨ�ŷ�00000000530099������    0000000-3008000000001952000000018152");
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//׼��д����ϸ
	/********************************************/
	if(!memcmp(gbl.fee_type,"0049",4) )	//��ͨ
	{
		count+=1;	//����������'A'
		if(!memcmp(gbl.uni_flag, "0", 1))	//�󸶷ѣ������·���
		{
			memset( temp,0,sizeof(temp) );
			memcpy(temp,(char *)(pack.FIELD63+count),1);	
			count+=1;
			gbl.detail.nMonCnt = atoi(temp);
			if(gbl.detail.nMonCnt>3 || gbl.detail.nMonCnt<1)
			{
				wrLog("Ƿ�ѽ����·ݴ���%d",gbl.detail.nMonCnt);
				return -1;
			}
		}
		//20�ֽڵĵ绰���루�����ʻ���
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
		wrLog("�����ʺ�:%s",gbl.pay_no);
		//30�ֽڵ�����
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
		wrLog("�û���:%s",gbl.user_name);

		switch(atoi(gbl.uni_flag))
		{
		case 0:
			for(i=0;i<gbl.detail.nMonCnt;i++)	
			{
				strncpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				
				//��ȡ��ǰ����ʱ����ϸ����(3λ)
				memset(temp, 0, sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),3);
				count+=3;
				len = atoi(temp);
				//wrLog("��ͨlen :%s",temp);		
				j=0;	//��ϸ������
				while(len>0)
				{
					if(len > 512)
						return -1;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),2);	//��ϸ��ʶ��
					count+=2;
					len-=2;
					//	wrLog("gbl.detail code :%s",temp);
					if( !strncmp(temp,"99",2) )		//��ý�����Ŀ����99����������Ŀ��(10λ)
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
		memset(gbl.pre_fee_buffer,0,sizeof(gbl.pre_fee_buffer)); //Ԥ�������
		memcpy(gbl.pre_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
		memset(gbl.real_fee_buffer,0,sizeof(gbl.real_fee_buffer));
		memcpy(gbl.real_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
	} //end if
	/********************************************/
	else if(!memcmp(gbl.fee_type, "0065", 4))	//����˰
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
		gbl.detail.nMonCnt = atoi(temp);	//�����
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			memcpy(gbl.detail.monDetail[i].tmFrom, pack.FIELD63+count, 4);	//���
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			wrLog("��˰��ݣ�[%s]",gbl.detail.monDetail[i].tmFrom);
			count += 4;
			memset(temp, 0, sizeof(temp));
			memcpy(temp,pack.FIELD63+count,3);
			count += 3;
			len = atoi(temp);
			wrLog("len��[%d]",len);
			j=0;	//��ϸ������
			wrLog("��ϸ��%s",pack.FIELD63+count);
			while(len>0)
			{
				if(len > 512)
					return -1;
				count+=2;//����2λ��ϸ��ʶ��"99"
				len-=2;
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
		
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);	//ת��Ϊ�������ַ���,���ޡ�Ԫ��
				count+=12;
				len-=12;
				j++;
//wrLog("��Ŀ%d��[%s]",j, gbl.detail.monDetail[i].cnt[j].type);
//wrLog("��[%s]",gbl.amount);
//wrLog("len��[%d]",len);
			}
			gbl.detail.monDetail[i].nCnt = j;
		}
	   memset(gbl.amount,0,sizeof(gbl.amount));
	   CTools::byte_to_hex(pack.FIELD4,(BYTE *)gbl.amount,6);
	
	}
	/********************************************/
    else if(!memcmp(gbl.fee_type, "0131", 4)){//��ݸ���ź󸶷�

		if(atoi(gbl.fee_type2)==149)
		{
			count=gbl.num;
		}
		bbzero(temp);	//3λǷ����Ϣ����
		memcpy(temp, (char *)(pack.FIELD63+count), 3);
		if(atoi(temp))	//��Ƿ����ϸ
			count+=3;
		else
			return 1;
		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),2);	//��������
		    count+=2;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("�����·�����%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
                
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
	else	//����ͨ
	{   
		/*-----���߿�Ƶ�Ż�-----*/
		if(atoi(gbl.fee_type) == 172)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
				return 1;
		}
		/*---------------------*/
		if(atoi(gbl.fee_type) == 100)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
			{
			//	iRet =  print_pre_detail(pack,"���ֵ��ӽɷ�"); //cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 112)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
			{
			//	iRet = print_pre_detail(pack,"�齭��Ƶ�ɷ�"); cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 114)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)pack.FIELD63, 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
			{
				//iRet = print_pre_detail("ģ�����(�޻�����)"); //cut by liuxiao
			//	return iRet;
			}
		}

		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),1);	//��������
		count+=1;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("�����·�����%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			//���뽻����
			switch(atoi(gbl.fee_type))
			{
			case 1:		//�̻�
			case 13:	//ADSL
			case 137:	//ITV
			case 241:   //CDMA�󸶷�
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				break;
				
			case 3012:	//�ƶ�
			case 100:	//�齭���������
			case 112:	//�齭��Ƶ�����
			case 114:	//���ߵ��������
			case 172:	//�齭��Ƶ�Ż�
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),6);
				count+=6;
		/*		strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��"); */
				break;
			default:
				return -1;
			}
//	wrLog("�����£�[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
			//��ȡ��ǰ����ʱ����ϸ���ȣ�3λ��
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),3);
			count+=3;
			len = atoi(temp);
			j=0;	//��ϸ������
			while(len>0)
			{
				if(len > 512)
					return -1;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),2);	//��ϸ��ʶ��
				count+=2;
				len-=2;
				if( !strncmp(temp,"99",2) )		//��ý�����Ŀ��
				{
					memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
					count+=10;
					len-=10;
				}
				else if( !strncmp(temp,"98",2) )		//���ŷ�Ʊ�޸�20071227
				{
					memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
					count+=10;
					len-=10;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
					CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//ת��Ϊ�������ַ���
					count+=12;
					len-=12;
					continue;
				}
				else
					ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);

				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//ת��Ϊ�������ַ���
				count+=12;
				len-=12;
				j++;
			}
			gbl.detail.monDetail[i].nCnt = j;
		} //end for

		if(atoi(gbl.fee_type) == 3012)	//�ƶ�(lihk,2006-10-25)
		{
			if( gbl.mbl_flag == 2 )	//��Ƿ���½�Ԥ���ѣ�����һ����Ԥ���ѵ���ϸ
			{
				gbl.detail.nMonCnt++;
				gbl.detail.monDetail[i].nCnt = 1;	//һ����ϸ
				memcpy(gbl.detail.monDetail[i].cnt[0].type, "������  ",10);
				CTools::chgToMny(gbl.tmp_pre_amount, gbl.detail.monDetail[i].cnt[0].amo , 0);//ת��Ϊ�������ַ���
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
				//iRet =  print_pre_detail(pack,"���ֵ��ӽɷ�"); //change by print_pre_detail("���ֵ��ӽɷ�") cut by liuxiao
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
				//iRet = print_pre_detail(pack,"�齭��Ƶ�ɷ�"); //cut by liuxiao
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
				//iRet = print_pre_detail(pack,"ģ�����(�޻�����)"); //cut by liuxiao
				//return iRet;
			}
		}
		wrLog("��ӡ��Ŀ��%d\n", gbl.detail.nMonCnt);
	}//end else
	if(flag)
		out_gbl = gbl;
	return 0;
}

//end of ���캯��
//��63���ж���ϸ��Ϣ������gbl.detail�ṹ���й���ӡ��
//���������������ͨԤ����
//flag��Ĭ��Ϊ1���ⲿ���ã�0���ڲ�ʹ��
int CPrint::rdField63(CGlobal &out_gbl, int flag)  
{
	int len , i , j,iRet;
	int count=0;
	char temp[20];

	if(flag)
		gbl = out_gbl;

//	wrLog("��������rdField63");
//	wrLog("BIT63:[%s]",pack.FIELD63);
//	strcpy((char *)pack.FIELD63,"A21100523577          ��С��                        20080807299�¹̶���  00000000600099����ͨ�ŷ�00000000882099������    0000000-30081100523577          ��С��                        20080907299�¹̶���  00000000600099����ͨ�ŷ�00000000530099������    0000000-3008000000001952000000018152");
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//׼��д����ϸ
	/********************************************/
	if(!memcmp(gbl.fee_type,"0049",4) )	//��ͨ
	{
		count+=1;	//����������'A'
		if(!memcmp(gbl.uni_flag, "0", 1))	//�󸶷ѣ������·���
		{
			memset( temp,0,sizeof(temp) );
			memcpy(temp,(char *)(pack.FIELD63+count),1);	
			count+=1;
			gbl.detail.nMonCnt = atoi(temp);
			if(gbl.detail.nMonCnt>3 || gbl.detail.nMonCnt<1)
			{
				wrLog("Ƿ�ѽ����·ݴ���%d",gbl.detail.nMonCnt);
				return -1;
			}
		}
		//20�ֽڵĵ绰���루�����ʻ���
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
		wrLog("�����ʺ�:%s",gbl.pay_no);
		//30�ֽڵ�����
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
		wrLog("�û���:%s",gbl.user_name);

		switch(atoi(gbl.uni_flag))
		{
		case 0:
			for(i=0;i<gbl.detail.nMonCnt;i++)	
			{
				strncpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				
				//��ȡ��ǰ����ʱ����ϸ����(3λ)
				memset(temp, 0, sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),3);
				count+=3;
				len = atoi(temp);
				//wrLog("��ͨlen :%s",temp);		
				j=0;	//��ϸ������
				while(len>0)
				{
					if(len > 512)
						return -1;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),2);	//��ϸ��ʶ��
					count+=2;
					len-=2;
					//	wrLog("gbl.detail code :%s",temp);
					if( !strncmp(temp,"99",2) )		//��ý�����Ŀ����99����������Ŀ��(10λ)
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
		memset(gbl.pre_fee_buffer,0,sizeof(gbl.pre_fee_buffer)); //Ԥ�������
		memcpy(gbl.pre_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
		memset(gbl.real_fee_buffer,0,sizeof(gbl.real_fee_buffer));
		memcpy(gbl.real_fee_buffer, pack.FIELD63+count, 12);
		count += 12;
	} //end if
	/********************************************/
	else if(!memcmp(gbl.fee_type, "0065", 4))	//����˰
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
		gbl.detail.nMonCnt = atoi(temp);	//�����
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			memcpy(gbl.detail.monDetail[i].tmFrom, pack.FIELD63+count, 4);	//���
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			wrLog("��˰��ݣ�[%s]",gbl.detail.monDetail[i].tmFrom);
			count += 4;
			memset(temp, 0, sizeof(temp));
			memcpy(temp,pack.FIELD63+count,3);
			count += 3;
			len = atoi(temp);
			wrLog("len��[%d]",len);
			j=0;	//��ϸ������
			wrLog("��ϸ��%s",pack.FIELD63+count);
			while(len>0)
			{
				if(len > 512)
					return -1;
				count+=2;//����2λ��ϸ��ʶ��"99"
				len-=2;
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
		
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);	//ת��Ϊ�������ַ���,���ޡ�Ԫ��
				count+=12;
				len-=12;
				j++;
//wrLog("��Ŀ%d��[%s]",j, gbl.detail.monDetail[i].cnt[j].type);
//wrLog("��[%s]",gbl.amount);
//wrLog("len��[%d]",len);
			}
			gbl.detail.monDetail[i].nCnt = j;
		}
	   memset(gbl.amount,0,sizeof(gbl.amount));
	   CTools::byte_to_hex(pack.FIELD4,(BYTE *)gbl.amount,6);
	
	}
	/********************************************/
    else if(!memcmp(gbl.fee_type, "0131", 4)){//��ݸ���ź󸶷�

		if(atoi(gbl.fee_type2)==149)
		{
			count=gbl.num;
		}
		bbzero(temp);	//3λǷ����Ϣ����
		memcpy(temp, (char *)(pack.FIELD63+count), 3);
		if(atoi(temp))	//��Ƿ����ϸ
			count+=3;
		else
			return 1;
		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),2);	//��������
		    count+=2;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("�����·�����%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
                
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
	else	//����ͨ
	{   
		/*-----���߿�Ƶ�Ż�-----*/
		if(atoi(gbl.fee_type) == 172)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
				return 1;
		}
		/*---------------------*/
		if(atoi(gbl.fee_type) == 100)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
			{
				//iRet =  print_pre_detail(pack,"���ֵ��ӽɷ�"); cut by liuxiao
			//	return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 112)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)(pack.FIELD63+count), 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
			{
				iRet = print_pre_detail(pack,"�齭��Ƶ�ɷ�");
				return iRet;
			}
		}
		if(atoi(gbl.fee_type) == 114)
		{
			bbzero(temp);	//3λǷ����Ϣ����
			memcpy(temp, (char *)pack.FIELD63, 3);
			if(atoi(temp))	//��Ƿ����ϸ
				count+=3;
			else
			{
				iRet = print_pre_detail("ģ�����(�޻�����)");
				return iRet;
			}
		}

		memset( temp,0,sizeof(temp) );
		memcpy(temp,(char *)(pack.FIELD63+count),1);	//��������
		count+=1;
		gbl.detail.nMonCnt = atoi(temp);
//		wrLog("�����·�����%d", gbl.detail.nMonCnt);
		for(i=0;i<gbl.detail.nMonCnt;i++)		
		{
			//���뽻����
			switch(atoi(gbl.fee_type))
			{
			case 1:		//�̻�
			case 13:	//ADSL
			case 137:	//ITV
			case 241:   //CDMA�󸶷�
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��");
				
				memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
				count+=4;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmTo,"��");
				break;
				
			case 3012:	//�ƶ�
			case 100:	//�齭���������
			case 112:	//�齭��Ƶ�����
			case 114:	//���ߵ��������
			case 172:	//�齭��Ƶ�Ż�
				memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),6);
				count+=6;
		/*		strcat(gbl.detail.monDetail[i].tmFrom,"��");
				strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
				count+=2;
				strcat(gbl.detail.monDetail[i].tmFrom,"��"); */
				break;
			default:
				return -1;
			}
//	wrLog("�����£�[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
			//��ȡ��ǰ����ʱ����ϸ���ȣ�3λ��
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),3);
			count+=3;
			len = atoi(temp);
			j=0;	//��ϸ������
			while(len>0)
			{
				if(len > 512)
					return -1;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),2);	//��ϸ��ʶ��
				count+=2;
				len-=2;
				if( !strncmp(temp,"99",2) )		//��ý�����Ŀ��
				{
					memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
					count+=10;
					len-=10;
				}
				else if( !strncmp(temp,"98",2) )		//���ŷ�Ʊ�޸�20071227
				{
					memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
					count+=10;
					len-=10;
					memset(temp,0,sizeof(temp));
					memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
					CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//ת��Ϊ�������ַ���
					count+=12;
					len-=12;
					continue;
				}
				else
					ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);

				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
				CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//ת��Ϊ�������ַ���
				count+=12;
				len-=12;
				j++;
			}
			gbl.detail.monDetail[i].nCnt = j;
		} //end for

		if(atoi(gbl.fee_type) == 3012)	//�ƶ�(lihk,2006-10-25)
		{
			if( gbl.mbl_flag == 2 )	//��Ƿ���½�Ԥ���ѣ�����һ����Ԥ���ѵ���ϸ
			{
				gbl.detail.nMonCnt++;
				gbl.detail.monDetail[i].nCnt = 1;	//һ����ϸ
				memcpy(gbl.detail.monDetail[i].cnt[0].type, "������  ",10);
				CTools::chgToMny(gbl.tmp_pre_amount, gbl.detail.monDetail[i].cnt[0].amo , 0);//ת��Ϊ�������ַ���
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
				iRet =  print_pre_detail(pack,"���ֵ��ӽɷ�");
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
				iRet = print_pre_detail(pack,"�齭��Ƶ�ɷ�");
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
				iRet = print_pre_detail(pack ,"ģ�����(�޻�����)");
				return iRet;
			}
		}
		wrLog("��ӡ��Ŀ��%d\n", gbl.detail.nMonCnt);
	}//end else
	if(flag)
		out_gbl = gbl;
	return 0;
}


//����Ʊʱ��63���ж���ϸ��Ϣ������gbl.detail�ṹ���й���ӡ��
int CPrint::rdField63_2()
{
	int len , i , j;
	int count=0;
	char temp[20];
	
	wrLog("��������rdField63_2");
	wrLog("BIT63:[%s]",pack.FIELD63);
	memset(gbl.fee_type, 0, sizeof(gbl.fee_type));//ҵ������
	memcpy(gbl.fee_type, pack.FIELD63, 4);
	count = 4;
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//׼��д����ϸ
	memset( temp,0,sizeof(temp) );
	memcpy(temp,(char *)pack.FIELD63 + count,1);	//��������
	count += 1;
	gbl.detail.nMonCnt = atoi(temp);
	wrLog("gbl.detail.nMonCnt:%d",gbl.detail.nMonCnt);
	memset(gbl.user_name, 0, sizeof(gbl.user_name));
	memcpy(gbl.user_name, pack.FIELD63+count, 40); //�û���
	count += 40;
	i = 0;
	while(gbl.user_name[i] != 0x20)
		i++;
	gbl.user_name[i] = '\0';
	gbl.user_name_len = i;

	for(i=0;i<gbl.detail.nMonCnt;i++)//���·���ȡֵ		
	{
		//���뽻����
		switch(atoi(gbl.fee_type))
		{
		case 3002:		//�ƶ�
		case 218:       //���ŷ�Ʊ����
		case 49:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			break;
		case 65:	//����˰
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count += 4;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			count += 2;	//����ʽ������2��0
			break;
		default:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			
			memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmTo,"��");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"��");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"��");
			break;
		}
		wrLog("�����£�[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
		//��ȡ��ǰ����ʱ����ϸ���ȣ�3λ��
		wrLog("��ϸ��%s",pack.FIELD63+count);
		memset(temp,0,sizeof(temp));
		memcpy(temp,(char *)(pack.FIELD63+count),3);
		count+=3;
		len = atoi(temp);
		j=0;	//��ϸ������
		while(len>0)
		{
			if(len > 512)
				return -1;
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),2);	//��ϸ��ʶ��
			count+=2;
			len-=2;
			if( !strncmp(temp,"99",2) )		//��ý�����Ŀ��
			{
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
			}
			else if( !strncmp(temp,"98",2) )		//���ŷ�Ʊ�޸�20071227
			{
				memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
				count+=10;
				len-=10;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
				CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//ת��Ϊ�������ַ���
				count+=12;
				len-=12;
				continue;
			}
			else
				ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);
			
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
			CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//ת��Ϊ�������ַ���
			count += 12;
			len -= 12;
			
			j++;
			/*wrLog("��Ŀ%d��%s",j, gbl.detail.monDetail[i].cnt[j].type);
			wrLog("��%s",temp);
			wrLog("len��[%d]",len);*/
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

//���캯��

//����Ʊʱ��63���ж���ϸ��Ϣ������gbl.detail�ṹ���й���ӡ��
int CPrint::rdField63_2(CPack pack)
{
	int len , i , j;
	int count=0;
	char temp[20];
	
	wrLog("��������rdField63_2");
	wrLog("BIT63:[%s]",pack.FIELD63);
	memset(gbl.fee_type, 0, sizeof(gbl.fee_type));//ҵ������
	memcpy(gbl.fee_type, pack.FIELD63, 4);
	count = 4;
	memset(&gbl.detail,0,sizeof(gbl.detail) );	//׼��д����ϸ
	memset( temp,0,sizeof(temp) );
	memcpy(temp,(char *)pack.FIELD63 + count,1);	//��������
	count += 1;
	gbl.detail.nMonCnt = atoi(temp);
	wrLog("gbl.detail.nMonCnt:%d",gbl.detail.nMonCnt);
	memset(gbl.user_name, 0, sizeof(gbl.user_name));
	memcpy(gbl.user_name, pack.FIELD63+count, 40); //�û���
	count += 40;
	i = 0;
	while(gbl.user_name[i] != 0x20)
		i++;
	gbl.user_name[i] = '\0';
	gbl.user_name_len = i;

	for(i=0;i<gbl.detail.nMonCnt;i++)//���·���ȡֵ		
	{
		//���뽻����
		switch(atoi(gbl.fee_type))
		{
		case 3002:		//�ƶ�
		case 218:       //���ŷ�Ʊ����
		case 49:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			break;
		case 65:	//����˰
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count += 4;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			count += 2;	//����ʽ������2��0
			break;
		default:
			memcpy(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			strncat(gbl.detail.monDetail[i].tmFrom,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmFrom,"��");
			
			memcpy(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),4);
			count+=4;
			strcat(gbl.detail.monDetail[i].tmTo,"��");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"��");
			strncat(gbl.detail.monDetail[i].tmTo,(char *)(pack.FIELD63+count),2);
			count+=2;
			strcat(gbl.detail.monDetail[i].tmTo,"��");
			break;
		}
		wrLog("�����£�[%s - %s]",gbl.detail.monDetail[i].tmFrom,gbl.detail.monDetail[i].tmTo);
		//��ȡ��ǰ����ʱ����ϸ���ȣ�3λ��
		wrLog("��ϸ��%s",pack.FIELD63+count);
		memset(temp,0,sizeof(temp));
		memcpy(temp,(char *)(pack.FIELD63+count),3);
		count+=3;
		len = atoi(temp);
		j=0;	//��ϸ������
		while(len>0)
		{
			if(len > 512)
				return -1;
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),2);	//��ϸ��ʶ��
			count+=2;
			len-=2;
			if( !strncmp(temp,"99",2) )		//��ý�����Ŀ��
			{
				memcpy(gbl.detail.monDetail[i].cnt[j].type,(char *)(pack.FIELD63+count),10);
				count+=10;
				len-=10;
			}
			else if( !strncmp(temp,"98",2) )		//���ŷ�Ʊ�޸�20071227
			{
				memcpy(gbl.detail.monDetail[i].detail98Name, (char *)(pack.FIELD63+count), 10);
				count+=10;
				len-=10;
				memset(temp,0,sizeof(temp));
				memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
				CTools::chgToMny(temp, gbl.detail.monDetail[i].detail98Amo, 0);//ת��Ϊ�������ַ���
				count+=12;
				len-=12;
				continue;
			}
			else
				ckCode(temp,gbl.detail.monDetail[i].cnt[j].type);
			
			memset(temp,0,sizeof(temp));
			memcpy(temp,(char *)(pack.FIELD63+count),12);	//��ϸ��12λ��
			CTools::chgToMny(temp,gbl.detail.monDetail[i].cnt[j].amo , 0);		//ת��Ϊ�������ַ���
			count += 12;
			len -= 12;
			
			j++;
			/*wrLog("��Ŀ%d��%s",j, gbl.detail.monDetail[i].cnt[j].type);
			wrLog("��%s",temp);
			wrLog("len��[%d]",len);*/
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
//end of ���캯��
//���ۿ�ҵ�񷵻���ϸ����2006-11-1,lihk
int CPrint::rdField63_3(char *card_num, char *card_pwd, char *card_serial, char *amount, char *eff_yymmdd,char *xm_serial)
{
	int tol_len , len, i ,pwd_len,j,k;
	int count=0;
	char tmp[20],card_pwd_buff[128];
	
	//wrLog("��������rdField63_3");
	//wrLog("BIT63:[%s]",pack.FIELD63);
	
	//ȡ���ܳ���
	tol_len = pack.Packet_Table[63].Real_Length;
	if(tol_len > 512)
		return -1;
	if (atoi(gbl.fee_type)==182)//��ݸ��stan���ۿ�����
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
	for(i=0; i<3; i++)	//�����š�����Ϳ����к�
	{
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63 + count, 2);
		count += 2;
		len = atoi(tmp);
		if(len)
		{
			switch(i)
			{
			case 0:	//����
			//	bbzero(gbl.card_num);
				memcpy(card_num, pack.FIELD63 + count, len);
				break;
			case 1:	//����
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
			case 2:	//�����к�
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
		//�ۿ���
		//	bbzero(gbl.amount);
		memcpy(amount, pack.FIELD63 + count, 12);
		count += 12;
	}
	return 0;
}

//���캯��
//���ۿ�ҵ�񷵻���ϸ����2006-11-1,lihk
int CPrint::rdField63_3(CPack pack,char *card_num, char *card_pwd, char *card_serial, char *amount, char *eff_yymmdd,char *xm_serial)
{
	int tol_len , len, i ,pwd_len,j,k;
	int count=0;
	char tmp[20],card_pwd_buff[128];
	
	//wrLog("��������rdField63_3");
	//wrLog("BIT63:[%s]",pack.FIELD63);
	
	//ȡ���ܳ���
	tol_len = pack.Packet_Table[63].Real_Length;
	if(tol_len > 512)
		return -1;
	if (atoi(gbl.fee_type)==182)//��ݸ��stan���ۿ�����
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
	for(i=0; i<3; i++)	//�����š�����Ϳ����к�
	{
		bbzero( tmp );
		memcpy(tmp, pack.FIELD63 + count, 2);
		count += 2;
		len = atoi(tmp);
		if(len)
		{
			switch(i)
			{
			case 0:	//����
			//	bbzero(gbl.card_num);
				memcpy(card_num, pack.FIELD63 + count, len);
				break;
			case 1:	//����
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
			case 2:	//�����к�
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
		//�ۿ���
		//	bbzero(gbl.amount);
		memcpy(amount, pack.FIELD63 + count, 12);
		count += 12;
	}
	return 0;
}

//end of ���캯��


void CPrint::ckCode(const char *code, char *type)	//��ѯҵ������������ҵ������д��type��
{
	switch(atoi(gbl.fee_type))
	{
	case 3012:
		if(!strncmp(code,"01",2) || !strncmp(code,"31",2) || !strncmp(code,"51",2) ||
			!strncmp(code,"61",2) || !strncmp(code,"U0",2) )
			strcpy(type,"����");
		else if(!strncmp(code,"02",2) || !strncmp(code,"U3",2))
			strcpy(type,"�л���");
		else if(!strncmp(code,"03",2))
			strcpy(type,"������");
		else if(!strncmp(code,"04",2))
			strcpy(type,"�л��˲�");
		else if(!strncmp(code,"05",2))
			strcpy(type,"�����˲�");
		else if(!strncmp(code,"06",2) || !strncmp(code,"36",2) || !strncmp(code,"62",2) || !strncmp(code,"XA",2))
			strcpy(type,"��Ϣ��");
		else if(!strncmp(code,"07",2) || !strncmp(code,"35",2) || !strncmp(code,"28",2) || !strncmp(code,"U7",2))
			strcpy(type,"���ɽ�");
		else if(!strncmp(code,"08",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"09",2))
			strcpy(type,"��������");
		else if(!strncmp(code,"11",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"12",2))
			strcpy(type,"�����ƶ�");
		else if(!strncmp(code,"13",2))
			strcpy(type,"���仰��");
		else if(!strncmp(code,"14",2))
			strcpy(type,"���л���");
		else if(!strncmp(code,"15",2))
			strcpy(type,"��ҵ���");
		else if(!strncmp(code,"16",2))
			strcpy(type,"800�·�");
		else if(!strncmp(code,"17",2))
			strcpy(type,"200/800��");
		else if(!strncmp(code,"18",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"19",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"20",2))
			strcpy(type,"���ڻ���");
		else if(!strncmp(code,"21",2))
			strcpy(type,"���ʻ���");
		else if(!strncmp(code,"22",2) || !strncmp(code,"UB",2))
			strcpy(type,"�۰�̨��");
		else if(!strncmp(code,"23",2))
			strcpy(type,"IP909��");
		else if(!strncmp(code,"24",2))
			strcpy(type,"��;����");
		else if(!strncmp(code,"25",2))
			strcpy(type,"��;����");
		else if(!strncmp(code,"32",2))
			strcpy(type,"�ƶ�����");
		else if(!strncmp(code,"33",2) || !strncmp(code,"U5",2))
			strcpy(type,"���λ���");
		else if(!strncmp(code,"34",2))
			strcpy(type,"��;����");
		else if(!strncmp(code,"37",2))
			strcpy(type,"������");
		else if(!strncmp(code,"38",2))
			strcpy(type,"����");
		else if(!strncmp(code,"SA",2))
			strcpy(type,"�»�����");
		else if(!strncmp(code,"SB",2))
			strcpy(type,"�¹��ܷ�");
		else if(!strncmp(code,"TX",2))
			strcpy(type,"ͨ�ŷ�");
		else if(!strncmp(code,"SC",2))
			strcpy(type,"����");
		else if(!strncmp(code,"TS",2))
			strcpy(type,"���շ�");
		else if(!strncmp(code,"ZZ",2))
			strcpy(type,"���շ�");
		else if(!strncmp(code,"ZN",2))
			strcpy(type,"ΥԼ��");
		else if(!strncmp(code,"cK",2))
			strcpy(type,"���ѿ�����");
		else if(!strncmp(code,"cJ",2))
			strcpy(type,"����Ԥ����");
		else if(!strncmp(code,"YF",2))
			strcpy(type,"ԭԤ����");
		else if(!strncmp(code,"TM",2))
			strcpy(type,"������");
		else if(!strncmp(code,"TU",2))
			strcpy(type,"�������");
		else if(!strncmp(code,"U1",2))
			strcpy(type,"�ط���");
		else if(!strncmp(code,"U2",2))
			strcpy(type,"��ί��");
		else if(!strncmp(code,"U4",2))
			strcpy(type,"��;��");
		else if(!strncmp(code,"U6",2))
			strcpy(type,"�����");
		else if(!strncmp(code,"U8",2))
			strcpy(type,"���ŷ�");
		else if(!strncmp(code,"UA",2))
			strcpy(type,"���ڷ���");
		else if(!strncmp(code,"UC",2))
			strcpy(type,"���ʷ���");
		else if(!strncmp(code,"UD",2))
			strcpy(type,"�Ż��ײ�");
		else if(!strncmp(code,"UN",2))
			strcpy(type,"������");
		else if(!strncmp(code,"PF",2))
			strcpy(type,"����");
		else if(!strncmp(code,"JX",2))
			strcpy(type,"����");
		else if(!strncmp(code,"YJ",2))
			strcpy(type,"���");
		break;
		
	case 100:	//�齭���������
	case 112:	//�齭��Ƶ�����
	case 114:	//���ߵ��������
	case 172:	//�齭��Ƶ�Ż� 
		if(!memcmp(code,"01",2))
			strcpy(type,"ά����");
		if(!memcmp(code,"02",2))
			strcpy(type,"��Ŀ��");
		if(!memcmp(code,"03",2))
			strcpy(type,"���ɽ�");
		if(!memcmp(code,"04",2))
			strcpy(type,"ͨ�ŷ�");
		break;
	case 3002:
		if(!strncmp(code,"01",2) || !strncmp(code,"31",2) || !strncmp(code,"51",2) ||
			!strncmp(code,"61",2) || !strncmp(code,"U0",2) )
			strcpy(type,"����");
		else if(!strncmp(code,"02",2) || !strncmp(code,"U3",2))
			strcpy(type,"�л���");
		else if(!strncmp(code,"03",2))
			strcpy(type,"������");
		else if(!strncmp(code,"04",2))
			strcpy(type,"�л��˲�");
		else if(!strncmp(code,"05",2))
			strcpy(type,"�����˲�");
		else if(!strncmp(code,"06",2) || !strncmp(code,"36",2) || !strncmp(code,"62",2) || !strncmp(code,"XA",2))
			strcpy(type,"��Ϣ��");
		else if(!strncmp(code,"07",2) || !strncmp(code,"35",2) || !strncmp(code,"28",2) || !strncmp(code,"U7",2))
			strcpy(type,"���ɽ�");
		else if(!strncmp(code,"08",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"09",2))
			strcpy(type,"��������");
		else if(!strncmp(code,"11",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"12",2))
			strcpy(type,"�����ƶ�");
		else if(!strncmp(code,"13",2))
			strcpy(type,"���仰��");
		else if(!strncmp(code,"14",2))
			strcpy(type,"���л���");
		else if(!strncmp(code,"15",2))
			strcpy(type,"��ҵ���");
		else if(!strncmp(code,"16",2))
			strcpy(type,"800�·�");
		else if(!strncmp(code,"17",2))
			strcpy(type,"200/800��");
		else if(!strncmp(code,"18",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"19",2))
			strcpy(type,"�л�����");
		else if(!strncmp(code,"20",2))
			strcpy(type,"���ڻ���");
		else if(!strncmp(code,"21",2))
			strcpy(type,"���ʻ���");
		else if(!strncmp(code,"22",2) || !strncmp(code,"UB",2))
			strcpy(type,"�۰�̨��");
		else if(!strncmp(code,"23",2))
			strcpy(type,"IP909��");
		else if(!strncmp(code,"24",2))
			strcpy(type,"��;����");
		else if(!strncmp(code,"25",2))
			strcpy(type,"��;����");
		else if(!strncmp(code,"32",2))
			strcpy(type,"�ƶ�����");
		else if(!strncmp(code,"33",2) || !strncmp(code,"U5",2))
			strcpy(type,"���λ���");
		else if(!strncmp(code,"34",2))
			strcpy(type,"��;����");
		else if(!strncmp(code,"37",2))
			strcpy(type,"������");
		else if(!strncmp(code,"38",2))
			strcpy(type,"����");
		else if(!strncmp(code,"SA",2))
			strcpy(type,"�»�����");
		else if(!strncmp(code,"SB",2))
			strcpy(type,"�¹��ܷ�");
		else if(!strncmp(code,"TX",2))
			strcpy(type,"ͨ�ŷ�");
		else if(!strncmp(code,"SC",2))
			strcpy(type,"����");
		else if(!strncmp(code,"TS",2))
			strcpy(type,"���շ�");
		else if(!strncmp(code,"ZZ",2))
			strcpy(type,"���շ�");
		else if(!strncmp(code,"ZN",2))
			strcpy(type,"ΥԼ��");
		else if(!strncmp(code,"cK",2))
			strcpy(type,"���ѿ�����");
		else if(!strncmp(code,"cJ",2))
			strcpy(type,"����Ԥ����");
		else if(!strncmp(code,"YF",2))
			strcpy(type,"ԭԤ����");
		else if(!strncmp(code,"TM",2))
			strcpy(type,"������");
		else if(!strncmp(code,"TU",2))
			strcpy(type,"�������");
		else if(!strncmp(code,"U1",2))
			strcpy(type,"�ط���");
		else if(!strncmp(code,"U2",2))
			strcpy(type,"��ί��");
		else if(!strncmp(code,"U4",2))
			strcpy(type,"��;��");
		else if(!strncmp(code,"U6",2))
			strcpy(type,"�����");
		else if(!strncmp(code,"U8",2))
			strcpy(type,"���ŷ�");
		else if(!strncmp(code,"UA",2))
			strcpy(type,"���ڷ���");
		else if(!strncmp(code,"UC",2))
			strcpy(type,"���ʷ���");
		else if(!strncmp(code,"UD",2))
			strcpy(type,"�Ż��ײ�");
		else if(!strncmp(code,"UN",2))
			strcpy(type,"������");
		else if(!strncmp(code,"PF",2))
			strcpy(type,"����");
		else if(!strncmp(code,"JX",2))
			strcpy(type,"����");
		else if(!strncmp(code,"YJ",2))
			strcpy(type,"���");
		break;
	}
}

/**********************************************************************/
/*--------------------------------��ӡ����----------------------------*/
/**********************************************************************/

//��ӡ��ϸ
//flag��1, Ƿ����ϸ
//      2, Ԥ������ϸ
//      3, �ۿ���ϸ
//      4, �̻���ϸ��ĿǰΪ���죩
//      5, ��ӡ������ϸ
//      6, ��ӡpicc��ϸ
//      7, ��ӡ��ӰƱ��ϸ
//type��ҵ������
int CPrint::print_detail(int flag, const char *type, char *field48, char *field63, char *priCont)
{
	int ret;
	switch( flag )
	{
	case 1:
		wrLog("print_pym_detail��ӡ��ʼ����");
		ret = print_pym_detail( type );  //��ӡǷ����ϸ
		break;
	case 2:
		wrLog("print_pre_detail��ӡ��ʼ����");
		ret = print_pre_detail( type );	 //��ӡԤ������ϸ
		break;
	case 3:
		wrLog("print_card_detail��ӡ��ʼ����");
		ret = print_card_detail( type ); //��ӡ�ۿ���ϸ	
		break;
	case 4:
		wrLog("print_mer_detail��ӡ��ʼ����");
		ret = print_mer_detail( type );	 //��ӡ�̻���ϸ��ĿǰΪ���죩	
		break;
	case 5:
		wrLog("print_CATV_detail��ӡ��ʼ����");
		ret = print_CATV_detail( type ); //��ӡ����Ƿ����ϸ
		break;
	case 6:
		wrLog("print_PICC_detail��ӡ��ʼ����");
		ret = print_PICC_detail( type ); //��ӡpicc��ϸ
		break;
	case 7:
		wrLog("print_film_detail��ӡ��ʼ����");
		ret = print_film_detail( type ); //��ӡ��ӰƱ��ϸ
		break;
	case 8:
		wrLog("print_ysq_detail��ӡ��ʼ����");
		ret = print_ysq_detail( type ); //��ӡԤ��Ȩ��ϸ
		break;
	case 9:
		wrLog("print_traffic_detail��ӡ��ʼ����");
		ret = print_traffic_detail( type, field48, field63,0 ); //��ӡ������ϸ
		break;
   
	case 10://����ƾ֤����
		wrLog("print_traffic_detail��ӡ��ʼ����");
		ret = print_traffic_detail( type, field48, field63,54 ); 
		break;
		
    case 11:
		wrLog("print_CATVDiscount_detail��ӡ��ʼ����");
		ret = print_CATVDiscount_detail(type); //��ӡ�齭��Ƶ�Ż�
		break;
	case 12:
		wrLog("print_ADD_ADSL��ӡ��ʼ����");
		ret = print_ADD_ADSL(type); //��ӡ�̻���װADSL
		break;
	case 13:
		wrLog("print_SELL_POS��ӡ��ʼ����");
		ret = print_SELL_POS(type); //�ն�����
		break;
	case 14:
		wrLog("print_chk_amo��ӡ��ʼ����");
		ret = print_chk_amo(type, field48); //����
		break;
	case 15:
		wrLog("reprint_bus_detail��ݸ�������������ӡ��ʼ����");
		ret = reprint_bus_detail(type); 
		break;
	case 16:
		wrLog("print_yct_detail���ͨ��ֵ��ӡ��ʼ����");
		ret = print_yct_detail(type, priCont); 
		break;
	case 17:
		wrLog("print_province_traffic_detailȫʡ������ӡ��ʼ����");
		ret = print_province_traffic_detail(type); 
		break;
	case 18:
		wrLog("print_bestpay_detail ��֧����ֵ��ӡ��ʼ����");
		ret=print_bestpay_detail(type);
		break;
	case 19:
		wrLog("print_xjx_detail н��н�ʽ𻮲���ӡ��ʼ����");
		ret=print_xjx_detail(type);
		break;
	case 20:
		wrLog("print_yctthird_detail ���ͨ��ֵ��������ӡ��ʼ����");
		ret=print_yctthird_detail(type,priCont);
		break;	
	default:
		wrLog("print_pym_detail��ӡ��ʼ����");
		ret = print_pym_detail( type );
	}
	return ret;
}


int CPrint::reprint_bus_detail(const char *type)		//������������ӡ
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	PrintTemp prtTemp;   //��ӡģ��

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.phone, gbl.card_num);
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"#%s",gbl.stan_no);//ϵͳ�ο���				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ
	bbzero(prtTemp.type);
	bbzero(prtTemp.content);
	prtTemp.n=0;
	if( atoi(gbl.amount)==4500 ){
		strcpy(prtTemp.type, "��ݸ���������������վ�");
		strcpy(prtTemp.content, "���ۣ�15.00Ԫ");
	}else if(  atoi(gbl.amount)==4800 ){
		strcpy(prtTemp.type, "��ݸ������������վ�");
		strcpy(prtTemp.content, "���18.00Ԫ");
	}else if(  atoi(gbl.amount)==5500  ){
		strcpy(prtTemp.type, "��ݸ��������Ѻ���վ�");
		strcpy(prtTemp.content, "Ѻ��25.00Ԫ");
	}else{
		strcpy(prtTemp.type, "δ֪");
		strcpy(prtTemp.content, "δ֪");
	}
	strcat(prtTemp.content, "\n��ԭ�н�0.00Ԫ");
	strcat(prtTemp.content, "\n�������30.00Ԫ");
	strcat(prtTemp.content, "\n�����н�30.00Ԫ\n");
	if(atoi(gbl.amount)==4500){
		strcat(prtTemp.content, "��ע���˿�һ������ϣ�һ�Ų����˿�����\n");
	}else if( atoi(gbl.amount)==4800 ){
		strcat(prtTemp.content, "��ע���˻����ʱ��Ҫ��IC�������վݷ��ɰ���\n");
	}else if( atoi(gbl.amount)==5500 ){
		strcat(prtTemp.content, "��ע���˻�Ѻ��ʱ��Ҫ��IC�������վݷ��ɰ���\n");
	}
	else{
		strcat(prtTemp.content, "��ע��δ֪��\n");
	}
	
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);
	strcat(prtTemp.content,advertisement);
	prtTemp.n += adv_count;
	prtTemp.n += 5;
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//���캯��
int CPrint::reprint_bus_detail(CPack pack,const char *type)		//������������ӡ
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	PrintTemp prtTemp;   //��ӡģ��
	
	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.phone, gbl.card_num);
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"#%s",gbl.stan_no);//ϵͳ�ο���				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ
	bbzero(prtTemp.type);
	bbzero(prtTemp.content);
	prtTemp.n=0;
	if( atoi(gbl.amount)==4500 ){
		strcpy(prtTemp.type, "��ݸ���������������վ�");
		strcpy(prtTemp.content, "���ۣ�15.00Ԫ");
	}else if(  atoi(gbl.amount)==4800 ){
		strcpy(prtTemp.type, "��ݸ������������վ�");
		strcpy(prtTemp.content, "���18.00Ԫ");
	}else if(  atoi(gbl.amount)==5500  ){
		strcpy(prtTemp.type, "��ݸ��������Ѻ���վ�");
		strcpy(prtTemp.content, "Ѻ��25.00Ԫ");
	}else{
		strcpy(prtTemp.type, "δ֪");
		strcpy(prtTemp.content, "δ֪");
	}
	strcat(prtTemp.content, "\n��ԭ�н�0.00Ԫ");
	strcat(prtTemp.content, "\n�������30.00Ԫ");
	strcat(prtTemp.content, "\n�����н�30.00Ԫ\n");
	if(atoi(gbl.amount)==4500){
		strcat(prtTemp.content, "��ע���˿�һ������ϣ�һ�Ų����˿�����\n");
	}else if( atoi(gbl.amount)==4800 ){
		strcat(prtTemp.content, "��ע���˻����ʱ��Ҫ��IC�������վݷ��ɰ���\n");
	}else if( atoi(gbl.amount)==5500 ){
		strcat(prtTemp.content, "��ע���˻�Ѻ��ʱ��Ҫ��IC�������վݷ��ɰ���\n");
	}
	else{
		strcat(prtTemp.content, "��ע��δ֪��\n");
	}
	
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);
	strcat(prtTemp.content,advertisement);
	prtTemp.n += adv_count;
	prtTemp.n += 5;
	strcat(prtTemp.content,"\n�����ݽ����ο��������������ݣ���Ʊ��������ƾ֤");
	prtTemp.n ++;
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}


//end of ���캯��


//��ӡ������ϸ
int CPrint::print_pym_detail(const char *type)		
{
	BYTE pMsg[1024];
	char temp[100], mny[20][MAXAMOLEN];
	char preMny[15], amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet;
	int i,j,k,flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��

	/* �����ض���ϵͳ�ο��� */
	char stan_serial[300];
	char unique_stan[6][30];

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);

	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.amount: [%s]", gbl.amount);
	//������彻����Ŀ
	if(!memcmp(gbl.fee_type,"0012",4) || !memcmp(gbl.fee_type,"3065",4)||!memcmp(gbl.fee_type,"0218",4))
	{
		flag = 1;
		iRet = rdField63_2();
		if(iRet)
			return -1;
	}
	//����ͨ������˰(��ѯʱ�ѷ�����ϸ)
	else if(memcmp(gbl.fee_type,"0049",4) && memcmp(gbl.fee_type,"0065",4) && memcmp(gbl.fee_type,"0092",4) && memcmp(gbl.fee_type,"0210",4)&& memcmp(gbl.fee_type,"0310",4)&& memcmp(gbl.fee_type,"0321",4)&& memcmp(gbl.fee_type,"0337",4)&& memcmp(gbl.fee_type,"0344",4))
	{
		iRet = rdField63(gbl, 0);
		if(iRet)
		{
			wrLog("��rdField63������");
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
	wrLog("������Ŀ��%s", prtTemp.type);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	if(!memcmp(gbl.fee_type2,"0149",4)){
       sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	   memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);					 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	strcpy(prtTemp.name, gbl.user_name);			 //�û���
	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if(!flag)	//�ǲ���Ʊ
		strcpy(prtTemp.amo, amoMny); //ʵ��
	else		//����Ʊ
		strcat(prtTemp.stan, "       ����Ʊ");

	/***  for test  ***/
	/* ��¼���ص�����ϵͳ�ο��� */
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

	/*-------------�ֱ��ӡ��������ʱ��-------------*/
	for(j=0;j<gbl.detail.nMonCnt;j++)		
	{
		//��ȡ����Ϊ"98"��ʶ����ϸ
		strcpy(prtTemp.detail98Amo,  gbl.detail.monDetail[j].detail98Amo);
		strcpy(prtTemp.detail98Name, gbl.detail.monDetail[j].detail98Name);
		bbzero(prtTemp.content);
		sprintf(prtTemp.count,"��%d��/��%d��", j+1, gbl.detail.nMonCnt);
		//��ȡ�ϼƽ��
		memset(mny,0,sizeof(mny));
		for(k=0; k<gbl.detail.monDetail[j].nCnt; k++)
			strcpy(mny[k], gbl.detail.monDetail[j].cnt[k].amo);
		bbzero(prtTemp.total);
		CTools::getTalAmo(mny, prtTemp.total, gbl.detail.monDetail[j].nCnt);
		//ת��Ϊ��д���
		bbzero(prtTemp.capsTotal);
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
		prtTemp.n=0;
		for(i=0;i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//����9����ϸ�2�У�
		{
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//�����
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			if(m_prtType == 1 || m_prtType == 3 || m_prtType == 8 || m_prtType == 9)  //��Ʊ��ӡֽ
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
			else  //��ֽ��ӡ
				strcat(prtTemp.content,"\n");
		}

		//�ƶ�����Ƿ������½�Ԥ����
		if(gbl.mbl_flag == 2)	
		{
			if( strcmp(gbl.tmp_pre_amount, "") && (j == gbl.detail.nMonCnt-1) )
			{
				strcpy(prtTemp.type, "�ƶ�Ԥ����");
				strcat(prtTemp.content, "ԭԤ������");
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
		//2006-10-30�������׿�
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
		//��ɽ�󸶷��·�Ʊֽ�޸�
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
		//�̻�����ɽ�ۺ��շ�,��ݸ���ź󸶷�
		else if(!strcmp(gbl.fee_type,"0131") || !strcmp(gbl.fee_type,"0241") ||!strcmp(gbl.fee_type,"0001") || !strcmp(gbl.fee_type,"0013") || !strcmp(gbl.fee_type,"0092") || !strcmp(gbl.fee_type,"0210")|| !strcmp(gbl.fee_type,"0310")||!memcmp(gbl.fee_type,"0337",4))	
		{
			sprintf(prtTemp.month, "%s��%s", gbl.detail.monDetail[j].tmFrom, gbl.detail.monDetail[j].tmTo);
		}
		else if(!strcmp(gbl.fee_type,"0065"))
		{
			memset(temp, 0, sizeof(temp));	//���ƺţ�˰����ˮ
			memcpy(temp, "��A",3);
			memcpy(temp+3, gbl.car_no+3, 5);
			strcat(temp, "---");
			strcat(temp, gbl.tax_serial);
			strcpy(prtTemp.phone, temp);
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
		else	//�ƶ�����ͨ
		{
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
        

		if(!memcmp(gbl.fee_type,"0131",4)){//��ݸ���ź󸶷�
            strcat(prtTemp.content, advertisement);
			prtTemp.n+=adv_count;
			prtTemp.n--;
		}
		
		/***  for test  ***/
		/*0092,0337 ϵͳ�ο��Ÿı�*/
		if((!memcmp(gbl.fee_type, "0092", 4) || !memcmp(gbl.fee_type, "0337", 4)) && gbl.print_paper)
		{
			strcpy(prtTemp.stan, unique_stan[j]);
			if(!flag)	//�ǲ���Ʊ
			{
				strcpy(prtTemp.amo, amoMny); //ʵ��
			}
			else		//����Ʊ
			{	
				strcat(prtTemp.stan, "    ����Ʊ");
			}
		}
		/******************/

		strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
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

//��ӡ���캯��
//��ӡ������ϸ
int CPrint::print_pym_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char temp[100], mny[20][MAXAMOLEN];
	char preMny[15], amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet;
	int i,j,k,flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��

	/* �����ض���ϵͳ�ο��� */
	char stan_serial[300];
	char unique_stan[6][30];

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);

	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.amount: [%s]", gbl.amount);
	//������彻����Ŀ
	if(!memcmp(gbl.fee_type,"0012",4) || !memcmp(gbl.fee_type,"3065",4)||!memcmp(gbl.fee_type,"0218",4))
	{
		flag = 1;
		iRet = rdField63_2(pack);
		if(iRet)
			return -1;
	}
	//����ͨ������˰(��ѯʱ�ѷ�����ϸ)
	else if(memcmp(gbl.fee_type,"0049",4) && memcmp(gbl.fee_type,"0065",4) && memcmp(gbl.fee_type,"0092",4) && memcmp(gbl.fee_type,"0210",4)&& memcmp(gbl.fee_type,"0310",4)&& memcmp(gbl.fee_type,"0321",4)&& memcmp(gbl.fee_type,"0337",4)&& memcmp(gbl.fee_type,"0344",4))
	{
		iRet = rdField63(pack,gbl, 0);
		if(iRet)
		{
			wrLog("��rdField63������");
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
	wrLog("������Ŀ��%s", prtTemp.type);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	if(!memcmp(gbl.fee_type2,"0149",4)){
       sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	   memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);					 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	strcpy(prtTemp.name, gbl.user_name);			 //�û���
	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if(!flag)	//�ǲ���Ʊ
		strcpy(prtTemp.amo, amoMny); //ʵ��
	else		//����Ʊ
		strcat(prtTemp.stan, "       ����Ʊ");

	/***  for test  ***/
	/* ��¼���ص�����ϵͳ�ο��� */
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

	/*-------------�ֱ��ӡ��������ʱ��-------------*/
	for(j=0;j<gbl.detail.nMonCnt;j++)		
	{
		//��ȡ����Ϊ"98"��ʶ����ϸ
		strcpy(prtTemp.detail98Amo,  gbl.detail.monDetail[j].detail98Amo);
		strcpy(prtTemp.detail98Name, gbl.detail.monDetail[j].detail98Name);
		bbzero(prtTemp.content);
		sprintf(prtTemp.count,"��%d��/��%d��", j+1, gbl.detail.nMonCnt);
		//��ȡ�ϼƽ��
		memset(mny,0,sizeof(mny));
		for(k=0; k<gbl.detail.monDetail[j].nCnt; k++)
			strcpy(mny[k], gbl.detail.monDetail[j].cnt[k].amo);
		bbzero(prtTemp.total);
		CTools::getTalAmo(mny, prtTemp.total, gbl.detail.monDetail[j].nCnt);
		//ת��Ϊ��д���
		bbzero(prtTemp.capsTotal);
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
		prtTemp.n=0;
		for(i=0;i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//����9����ϸ�2�У�
		{
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//�����
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			if(m_prtType == 1 || m_prtType == 3 || m_prtType == 8 || m_prtType == 9)  //��Ʊ��ӡֽ
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
			else  //��ֽ��ӡ
				strcat(prtTemp.content,"\n");
		}

		//�ƶ�����Ƿ������½�Ԥ����
		if(gbl.mbl_flag == 2)	
		{
			if( strcmp(gbl.tmp_pre_amount, "") && (j == gbl.detail.nMonCnt-1) )
			{
				strcpy(prtTemp.type, "�ƶ�Ԥ����");
				strcat(prtTemp.content, "ԭԤ������");
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
		//2006-10-30�������׿�
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
		//��ɽ�󸶷��·�Ʊֽ�޸�
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
		//�̻�����ɽ�ۺ��շ�,��ݸ���ź󸶷�
		else if(!strcmp(gbl.fee_type,"0131") || !strcmp(gbl.fee_type,"0241") ||!strcmp(gbl.fee_type,"0001") || !strcmp(gbl.fee_type,"0013") || !strcmp(gbl.fee_type,"0092") || !strcmp(gbl.fee_type,"0210")|| !strcmp(gbl.fee_type,"0310")||!memcmp(gbl.fee_type,"0337",4))	
		{
			sprintf(prtTemp.month, "%s��%s", gbl.detail.monDetail[j].tmFrom, gbl.detail.monDetail[j].tmTo);
		}
		else if(!strcmp(gbl.fee_type,"0065"))
		{
			memset(temp, 0, sizeof(temp));	//���ƺţ�˰����ˮ
			memcpy(temp, "��A",3);
			memcpy(temp+3, gbl.car_no+3, 5);
			strcat(temp, "---");
			strcat(temp, gbl.tax_serial);
			strcpy(prtTemp.phone, temp);
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
		else	//�ƶ�����ͨ
		{
			strcpy(prtTemp.month, gbl.detail.monDetail[j].tmFrom);
		}
        

		if(!memcmp(gbl.fee_type,"0131",4)){//��ݸ���ź󸶷�
            strcat(prtTemp.content, advertisement);
			prtTemp.n+=adv_count;
			prtTemp.n--;
		}
		
		/***  for test  ***/
		/*0092,0337 ϵͳ�ο��Ÿı�*/
		if((!memcmp(gbl.fee_type, "0092", 4) || !memcmp(gbl.fee_type, "0337", 4)) && gbl.print_paper)
		{
			strcpy(prtTemp.stan, unique_stan[j]);
			if(!flag)	//�ǲ���Ʊ
			{
				strcpy(prtTemp.amo, amoMny); //ʵ��
			}
			else		//����Ʊ
			{	
				strcat(prtTemp.stan, "    ����Ʊ");
			}
		}
		/******************/

		strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
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

//end of ��ӡ���캯��

int CPrint::print_pre_detail(const char *type)		//��ӡԤ������ϸ
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char preMny[15], amoMny[15],phone_mny[12+1];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	char month_count[1+1];
	char temp[12+1];
	char tmp[256];
	PrintTemp prtTemp;   //��ӡģ��

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	switch(atoi(gbl.fee_type))				//�û���������
	{
	case 100:	//�齭����
	case 112:	//�齭��Ƶ
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s��%s��", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//���ߵ���
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
	case 342: //ʡ��ͨ�ͷ���
	case 356: //ʡ��ͨ�߷���
		bbzero(prtTemp.name);
		strcpy(prtTemp.phone, gbl.phone_no);
		break;
	default:
		strcpy(prtTemp.name, gbl.user_name);
		strcpy(prtTemp.phone, gbl.phone_no);
	}
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	if(!memcmp(gbl.fee_type2,"0149",4)){
		sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);						 //ҵ������
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ

	prtTemp.n=0;
//     if (atoi(gbl.fee_type) == 176 && gbl.tgroup == 40)
// 	{
// 		sprintf(prtTemp.content,"������%s\n", prtTemp.total);
// 	}
// 	else
	sprintf(prtTemp.content, "�����%s\n", prtTemp.total);
	prtTemp.n++;
	switch(atoi(gbl.fee_type))
	{
	case 3012:
		strcat(prtTemp.content, "ԭԤ������");
		bbzero(preMny);
		CTools::chgToMny(gbl.pre_fee_buffer, preMny, 0);
		strcat(prtTemp.content, preMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;

	case 71:
		strcat(prtTemp.content, "���Ԥ�ɷ��Ż����ͣ�");
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
	case 96:	//���л���Ԥ����
	case 97:	//ADSL����Ԥ����
	case 138:	//ITVԤ���� add by kf
	case 242:  //CDMAԤ����
		bbzero(amoMny);		
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+12),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "�ֿ������:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;
	case 100:  //���ֵ���
	case 112:  //�齭��Ƶ
	case 114:  //���ߵ���
		bbzero(amoMny);
		len += 3;
		bbzero(month_count);
		memcpy(month_count,(char*)(pack.FIELD63+len),1);
		len += 1;
		len +=atoi(month_count)*23;
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+len),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "Ԥ�����:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		bbzero(prtTemp.total);
		bbzero(prtTemp.capsTotal);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
		break;
	case 176://��ݸ�ƶ�Ԥ����
		strcat(prtTemp.content,"�ɷ��Ͷ���001��1861��ѯ���\n");
//		prtTemp.n++;
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
	    prtTemp.n += adv_count;
		break;

	case 167://��ݸQQ��ֵ
	case 168://��ݸQQ����
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		prtTemp.n--;
		break;
	case 281:
		bbzero(tmp);
		sprintf(tmp,"��ֵ�˻���%s",gbl.pay_easy_zh);
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
			sprintf(tmp,"������%s\n",phone_mny);
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
			sprintf(tmp,"������%s\n",phone_mny);
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
			sprintf(tmp,"������%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		strcat(prtTemp.content,"�벦��11888��ѯ��ֵ������Ϣ\n");
		//		prtTemp.n++;
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		break;
		
	}

	if(atoi(gbl.fee_type)!=176 && atoi(gbl.fee_type)!=256 && atoi(gbl.fee_type)!=213){//�Ƕ�ݸ�ƶ�Ԥ���ѡ�����ֱ�䡢�ƶ�ֱ��
		strcat(prtTemp.content,"\n");
		prtTemp.n++;
	}
	switch(atoi(gbl.fee_type))
	{
	case 71:
		strcat(prtTemp.content,"���͵Ľ���ɵ������Ʒ��¿�ʼ��\n��ʮ����ƽ�����͡���������ѯ��10000\n");
		prtTemp.n += 2;
		break;
	case 240:
		strcat(prtTemp.content,"\n˵����\nԤ��100Ԫ��300Ԫ���ѣ�Ԥ���ı��𻰷Ѽ�ʱ����\n���͵Ļ��Ѵ�12�����15�������ͣ�ÿ��20Ԫ\n�����ױ����ͻ���12��26�պ���\n");
		prtTemp.n += 5;
		break;
	}
//	for(i=0;i<9-n;i++)
//		strcat(prtTemp.content,"\n");
	
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n ++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen); 

	return iRet;
}


//���캯�� by liuxiao
int CPrint::print_pre_detail(CPack pack,const char *type)		//��ӡԤ������ϸ
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char preMny[15], amoMny[15],phone_mny[12+1];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	char month_count[1+1];
	char temp[12+1];
	char tmp[256];
	PrintTemp prtTemp;   //��ӡģ��

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	switch(atoi(gbl.fee_type))				//�û���������
	{
	case 100:	//�齭����
	case 112:	//�齭��Ƶ
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s��%s��", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//���ߵ���
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
	case 342: //ʡ��ͨ�ͷ���
	case 356: //ʡ��ͨ�߷���
		bbzero(prtTemp.name);
		strcpy(prtTemp.phone, gbl.phone_no);
		break;
	default:
		strcpy(prtTemp.name, gbl.user_name);
		strcpy(prtTemp.phone, gbl.phone_no);
	}
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	if(!memcmp(gbl.fee_type2,"0149",4)){
		sprintf(prtTemp.stan,"#%s",gbl.stan_no);
	}
    else
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);						 //ҵ������
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ

	prtTemp.n=0;
//     if (atoi(gbl.fee_type) == 176 && gbl.tgroup == 40)
// 	{
// 		sprintf(prtTemp.content,"������%s\n", prtTemp.total);
// 	}
// 	else
	sprintf(prtTemp.content, "�����%s\n", prtTemp.total);
	prtTemp.n++;
	switch(atoi(gbl.fee_type))
	{
	case 3012:
		strcat(prtTemp.content, "ԭԤ������");
		bbzero(preMny);
		CTools::chgToMny(gbl.pre_fee_buffer, preMny, 0);
		strcat(prtTemp.content, preMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;

	case 71:
		strcat(prtTemp.content, "���Ԥ�ɷ��Ż����ͣ�");
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
	case 96:	//���л���Ԥ����
	case 97:	//ADSL����Ԥ����
	case 138:	//ITVԤ���� add by kf
	case 242:  //CDMAԤ����
		bbzero(amoMny);		
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+12),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "�ֿ������:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		break;
	case 100:  //���ֵ���
	case 112:  //�齭��Ƶ
	case 114:  //���ߵ���
		bbzero(amoMny);
		len += 3;
		bbzero(month_count);
		memcpy(month_count,(char*)(pack.FIELD63+len),1);
		len += 1;
		len +=atoi(month_count)*23;
		bbzero(temp);
		memcpy(temp,(char*)(pack.FIELD63+len),12);
		CTools::chgToMny(temp, amoMny);
		strcat(prtTemp.content, "Ԥ�����:");
		strcat(prtTemp.content, amoMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		bbzero(prtTemp.total);
		bbzero(prtTemp.capsTotal);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
		break;
	case 176://��ݸ�ƶ�Ԥ����
		strcat(prtTemp.content,"�ɷ��Ͷ���001��1861��ѯ���\n");
//		prtTemp.n++;
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
	    prtTemp.n += adv_count;
		break;

	case 167://��ݸQQ��ֵ
	case 168://��ݸQQ����
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		prtTemp.n--;
		break;
	case 281:
		bbzero(tmp);
		sprintf(tmp,"��ֵ�˻���%s",gbl.pay_easy_zh);
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
			sprintf(tmp,"������%s\n",phone_mny);
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
			sprintf(tmp,"������%s\n",phone_mny);
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
			sprintf(tmp,"������%s\n",phone_mny);
			strcat(prtTemp.content, tmp);
			prtTemp.n += 1;
		}
		strcat(prtTemp.content,"�벦��11888��ѯ��ֵ������Ϣ\n");
		//		prtTemp.n++;
		Add_advertisement(gbl.fee_type,advertisement,&adv_count);
		strcat(prtTemp.content,advertisement);
		prtTemp.n += adv_count;
		break;
		
	}

	if(atoi(gbl.fee_type)!=176 && atoi(gbl.fee_type)!=256 && atoi(gbl.fee_type)!=213){//�Ƕ�ݸ�ƶ�Ԥ���ѡ�����ֱ�䡢�ƶ�ֱ��
		strcat(prtTemp.content,"\n");
		prtTemp.n++;
	}
	switch(atoi(gbl.fee_type))
	{
	case 71:
		strcat(prtTemp.content,"���͵Ľ���ɵ������Ʒ��¿�ʼ��\n��ʮ����ƽ�����͡���������ѯ��10000\n");
		prtTemp.n += 2;
		break;
	case 240:
		strcat(prtTemp.content,"\n˵����\nԤ��100Ԫ��300Ԫ���ѣ�Ԥ���ı��𻰷Ѽ�ʱ����\n���͵Ļ��Ѵ�12�����15�������ͣ�ÿ��20Ԫ\n�����ױ����ͻ���12��26�պ���\n");
		prtTemp.n += 5;
		break;
	}
//	for(i=0;i<9-n;i++)
//		strcat(prtTemp.content,"\n");
	
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n ++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen); 

	return iRet;
}



//�ۿ���ӡ��ϸ����2006-11-1,lihk
int CPrint::print_card_detail(const char *type)	
{
	int iPrintLen, iRet, i;
	BYTE pMsg[1024];
	char cardMny[15],amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	char eff_yymmdd[8+1]; //��Ч����
	char xm_serial[30+1], card_no[20+1], card_pwd[20+1], m_type[20+1];
	char show_buff[1024];
	PrintTemp prtTemp;

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);

	bbzero(m_type);
	strcpy(m_type, type);
	
	//��ȡ��ӡ����
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
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(cardMny);	//��ֵ��ֵ
	CTools::chgToMny(gbl.card_amount, cardMny);
	bbzero(show_buff);
	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==200||atoi(gbl.sell_card_type)==201){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�ƶ���ֵ��");
		
	}
	if(atoi(gbl.sell_card_type)==311||atoi(gbl.sell_card_type)==300||atoi(gbl.sell_card_type)==301){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"С��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�����ֵ��");

	}
	if(atoi(gbl.sell_card_type)==135||atoi(gbl.sell_card_type)==136||atoi(gbl.sell_card_type)==139||atoi(gbl.sell_card_type)==142||atoi(gbl.sell_card_type)==143||atoi(gbl.sell_card_type)==144){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"���ų�ֵ���ѿ�");

	}
	if(atoi(gbl.sell_card_type)==807||atoi(gbl.sell_card_type)==808||atoi(gbl.sell_card_type)==809){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"������Ϸһ��ͨ");

	}
	if(atoi(gbl.sell_card_type)==810||atoi(gbl.sell_card_type)==811||atoi(gbl.sell_card_type)==812||atoi(gbl.sell_card_type)==813||atoi(gbl.sell_card_type)==814){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"����ͨ��");	
	}

	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==205||atoi(gbl.sell_card_type)==206){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�ƶ���ֵ��");

	}
	if(atoi(gbl.sell_card_type)==310||atoi(gbl.sell_card_type)==308||atoi(gbl.sell_card_type)==309){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"С��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�����ֵ��");

	}
	if(atoi(gbl.sell_card_type)==128||atoi(gbl.sell_card_type)==129||atoi(gbl.sell_card_type)==130){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"���ų�ֵ���ѿ�");

	}
	if(atoi(gbl.sell_card_type)==804||atoi(gbl.sell_card_type)==805||atoi(gbl.sell_card_type)==806){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"������Ϸһ��ͨ");

	}

	bbzero(amoMny);  //���
	CTools::chgToMny(gbl.amount, amoMny);

	//��д��ӡģ��
	memset(&prtTemp,0,sizeof(prtTemp));
	if(!memcmp(gbl.fee_type2,"0182",4)){
		sprintf(prtTemp.stan,"#%s",gbl.phone_no);
	}
    else
	    memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
	CTools::formatDate(yymmdd, prtTemp.date);	//����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");	//����
	strcpy(prtTemp.amo, amoMny);			//ʵ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//�ϼ�
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//ת��Ϊ��д���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//����ʾ�������п���
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);		//�ն���ˮ
	strcpy(prtTemp.pos, gbl.pos_no);		//�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if (memcmp(gbl.sell_card_type,"10",2)==0)
	{

		sprintf(prtTemp.phone, "%s", gbl.card_serial);//����
	}
	else
	{
		sprintf(prtTemp.phone, "%s(�����к�)", gbl.card_serial);//����

	}

	if (atoi(gbl.fee_type)==245)
	{
		strcpy(prtTemp.type, gbl.xmcard_name);
		strcpy(show_buff,gbl.xmcard_name);
		strcat(show_buff,"\n");
	}else{
		strcpy(prtTemp.type, m_type);  //����
		strcpy(show_buff,m_type);
		strcat(show_buff,"\n");
	}
	//��д��ӡ��ϸ
	
	prtTemp.n=0;
	if (atoi(gbl.fee_type)==245)
	{
		sprintf(prtTemp.content, "����ˮ��:%s", xm_serial);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "��ֵ:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.����ע���������ֵ\n");
		strcat(prtTemp.content, "2.��ֵ��������½�����Ϸ�ٷ���վ����ʾ��ֵ\n");
		strcat(prtTemp.content, "3.��ȯ�۳����ṩ�˻�����ʧ����\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");

		strcat(show_buff,"����ˮ��:");
		strcat(show_buff,xm_serial);
		strcat(show_buff,"\n");
		strcat(show_buff,"����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff,"\n");
		strcat(show_buff,"����:");
		strcat(show_buff,gbl.card_pwd);
		strcat(show_buff,"\n");

		prtTemp.n += 8;
	}
	else if (atoi(gbl.fee_type)==153||atoi(gbl.fee_type)==182){ //��ݸ�ۿ����ۿ�����
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
        sprintf(prtTemp.content, "��ֵ:");
		
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");

	//	prtTemp.n++;
	}
	else if (atoi(gbl.sell_card_type)==900)
	{
		sprintf(prtTemp.content, "��Ч����:%s", eff_yymmdd);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
	else
	{
		sprintf(prtTemp.content, "��Ч����:%s", eff_yymmdd);
		strcat(prtTemp.content, "��ֵ:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
		

	switch(atoi(gbl.sell_card_type))
	{
	case 200:	//�ƶ�
	case 201:
	case 205:
	case 206:
	case 207:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.����ע��������Ч�ڣ������ֵ\n");
		strcat(prtTemp.content, "2.��ֵ�������й��ƶ�13800138000\n");
		strcat(prtTemp.content, "3.��ȯ�۳����ṩ�˻�����ʧ����\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 300:	//��ͨ
	case 301:
	case 311:
	case 308:
	case 309:
	case 310:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.����ע��������Ч�ڣ������ֵ\n");
		strcat(prtTemp.content, "2.����19λ�Ŀ���ֵ������10011");
		strcat(prtTemp.content, "  ��������ֵ������10012\n");
		strcat(prtTemp.content, "3.��ȯ�۳����ṩ�˻�����ʧ����\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 112:	//�����׿�
	case 113:
		break;
	case 109:	//С��ͨ��ֵ��
	case 110:	
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.��ȯ�����ڹ������ڶ�Ԥ����С��ͨ��ֵʹ��\n");
		strcat(prtTemp.content, "2.������Ч����ʹ�ñ���������ѳ�ֵ�绰96338");
		strcat(prtTemp.content, "��������ʾ�������ͻ����ߣ�10000\n");
		strcat(prtTemp.content, "3.������ʧ����������ֵ�����Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "4.�㶫��Ѷ���ſƼ����޹�˾ӵ�жԱ�ȯ�����ս�");
		strcat(prtTemp.content, "��Ȩ\n");

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 7;
		break;

	case 136:	//���ų�ֵ���ѿ�
	case 139:
	case 135:
	case 142:
	case 143:
	case 144:
	case 128:
	case 129:
	case 130:
	case 137:
		strcat(prtTemp.content, "�ʺ�:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.���������ڹ㶫ʡ�����û���ֵ���ѣ���ֵ�벦��11888\n");
		strcat(prtTemp.content, "2.��ȯ����ʧ�������Ʊ��ܣ�ʧ���Ը����ͷ����ߣ�10000\n");
		strcat(prtTemp.content, "�������ס��ͷ�����0400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "�ʺ�:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 800:	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.������ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "�������벦��400-80-11888��ѯ\n");
		strcat(prtTemp.content, "3.ʹ�÷�������½http://sde.game.sohu.com/����ʾ��ֵ\n");		
		prtTemp.n +=8;

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 801:	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.������ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "�������벦��400-80-11888��ѯ\n");
		strcat(prtTemp.content, "3.ʹ�÷�������½http://sde.game.sohu.com/����ʾ��ֵ\n");
		prtTemp.n += 8;

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 810:
	case 811:
	case 812:
	case 813:
	case 814:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.������ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "�������벦��400-80-11888��ѯ\n");
		strcat(prtTemp.content, "3.ʹ�÷��������½����ͨ��վ www.txtong.com.cn���г�ֵ\n");		
		prtTemp.n += 8;

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;
	case 807:
	case 808:
	case 809:
	case 804:
	case 805:
	case 806:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content,"��ܰ��ʾ��\n    ���ڷ�Ʊ��ӡ��������½http://www.dg8888.com��վ��������ʾ��˵�����뿨�ź�������г�ֵ.�ͷ����ߣ�400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "���ţ�");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "���룺");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 127://��ݸС��ͨ�ۿ�
	case 122:
    case 123:
		strcat(prtTemp.content, "���룺");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "��ѳ�ֵ�绰��96338\n");
		strcat(prtTemp.content,"��ܰ��ʾ��\n    ���ڵ���ȯƱ�ݴ�ӡ��24Сʱ�ڼ�ʱ��ֵ\n");
		prtTemp.n += 6;

		strcat(show_buff, "���룺");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

    case 124://��ݸ����ۿ�
	case 125:
    case 126:
		strcat(prtTemp.content, "���룺");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "��ֵ��ַ��http://www.dgkd.cn\n");
		strcat(prtTemp.content,"��ܰ��ʾ��\n    ���ڵ���ȯƱ�ݴ�ӡ��24Сʱ�ڼ�ʱ��ֵ\n");
		prtTemp.n += 6;

		strcat(show_buff, "���룺");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;
		
	case 900:
		strcat(prtTemp.content, "�ʺ�:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.��ȯ��ͨ�����з�ʽ�һ���\n����114ռλ\n���½http://gd.118114.cn/movie\n�������ֻ���½Ժ��ͨ���\n");
		strcat(prtTemp.content, "2.��ȯ����ʧ�������Ʊ��ܣ�ʧ���Ը���\n");
		prtTemp.n += 7;

		strcat(show_buff, "�ʺ�:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	}

	strcat(show_buff,"�밴��ȷ�ϡ���ӡ");
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

	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n ++ ;

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType );	
	
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func_safe((char*)pMsg,iPrintLen);
	wrLog("��print_func_safe��iRet = %d", iRet);
	return iRet;
}

//�ۿ�������
//�ۿ���ӡ��ϸ����2006-11-1,lihk
int CPrint::print_card_detail(CPack pack,const char *type)	
{
	int iPrintLen, iRet, i;
	BYTE pMsg[1024];
	char cardMny[15],amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	char eff_yymmdd[8+1]; //��Ч����
	char xm_serial[30+1], card_no[20+1], card_pwd[20+1], m_type[20+1];
	char show_buff[1024];
	PrintTemp prtTemp;
	int iSelect;

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);

	bbzero(m_type);
	strcpy(m_type, type);
	
	//��ȡ��ӡ����
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
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(cardMny);	//��ֵ��ֵ
	CTools::chgToMny(gbl.card_amount, cardMny);
	bbzero(show_buff);
	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==200||atoi(gbl.sell_card_type)==201){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�ƶ���ֵ��");
		
	}
	if(atoi(gbl.sell_card_type)==311||atoi(gbl.sell_card_type)==300||atoi(gbl.sell_card_type)==301){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"С��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�����ֵ��");

	}
	if(atoi(gbl.sell_card_type)==135||atoi(gbl.sell_card_type)==136||atoi(gbl.sell_card_type)==139||atoi(gbl.sell_card_type)==142||atoi(gbl.sell_card_type)==143||atoi(gbl.sell_card_type)==144){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"���ų�ֵ���ѿ�");

	}
	if(atoi(gbl.sell_card_type)==807||atoi(gbl.sell_card_type)==808||atoi(gbl.sell_card_type)==809){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"������Ϸһ��ͨ");

	}
	if(atoi(gbl.sell_card_type)==810||atoi(gbl.sell_card_type)==811||atoi(gbl.sell_card_type)==812||atoi(gbl.sell_card_type)==813||atoi(gbl.sell_card_type)==814){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"����ͨ��");	
	}

	if(atoi(gbl.sell_card_type)==207||atoi(gbl.sell_card_type)==205||atoi(gbl.sell_card_type)==206){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�ƶ���ֵ��");

	}
	if(atoi(gbl.sell_card_type)==310||atoi(gbl.sell_card_type)==308||atoi(gbl.sell_card_type)==309){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==127||atoi(gbl.sell_card_type)==122||atoi(gbl.sell_card_type)==123){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"С��ͨ��ֵ��");

	}
	if(atoi(gbl.sell_card_type)==124||atoi(gbl.sell_card_type)==125||atoi(gbl.sell_card_type)==126){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"�����ֵ��");

	}
	if(atoi(gbl.sell_card_type)==128||atoi(gbl.sell_card_type)==129||atoi(gbl.sell_card_type)==130){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"���ų�ֵ���ѿ�");

	}
	if(atoi(gbl.sell_card_type)==804||atoi(gbl.sell_card_type)==805||atoi(gbl.sell_card_type)==806){
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"������Ϸһ��ͨ");

	}

	if (memcmp(gbl.sell_card_type,"10",2)==0) //�����ͷ��10 ���������֧����
	{
		memset(m_type,0,sizeof(m_type));
		strcpy(m_type,"��֧����");
	}

	bbzero(amoMny);  //���
	CTools::chgToMny(gbl.amount, amoMny);

	//��ʾ������Ϣ
	if(memcmp(gbl.sell_card_type,"10",2)==0)
	{
		char showMsg[100];
		sprintf(showMsg,"���׳ɹ�!\nϵͳ�ο���:%s\n��֧������ֵ:%02dԪ\n����:%s\n����:%s\n��Ч��:%s",gbl.stan_no,atoi(gbl.amount)/100,gbl.card_num,gbl.card_pwd,eff_yymmdd);
		iRet=TS_OutPutMsg(gbl.dPosInfo,showMsg,0,iSelect);
		if (iRet!=0 || iSelect!=0)
		{
			return 0;
		}

	}
	
	//��д��ӡģ��
	memset(&prtTemp,0,sizeof(prtTemp));
	if(!memcmp(gbl.fee_type2,"0182",4)){
		sprintf(prtTemp.stan,"#%s",gbl.phone_no);
	}
    else
	    memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
	CTools::formatDate(yymmdd, prtTemp.date);	//����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");	//����
	strcpy(prtTemp.amo, amoMny);			//ʵ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//�ϼ�
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//ת��Ϊ��д���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//����ʾ�������п���
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);		//�ն���ˮ
	strcpy(prtTemp.pos, gbl.pos_no);		//�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	if (memcmp(gbl.sell_card_type,"10",2)==0)
	{

		sprintf(prtTemp.phone, "%s", gbl.card_serial);//����
	}
	else
	{
	
		sprintf(prtTemp.phone, "%s(�����к�)", gbl.card_serial);//����
	}

	if (atoi(gbl.fee_type)==245)
	{
		strcpy(prtTemp.type, gbl.xmcard_name);
		strcpy(show_buff,gbl.xmcard_name);
		strcat(show_buff,"\n");
	}else{
		strcpy(prtTemp.type, m_type);  //����
		strcpy(show_buff,m_type);
		strcat(show_buff,"\n");
	}
	//��д��ӡ��ϸ
	
	prtTemp.n=0;
	if (atoi(gbl.fee_type)==245)
	{
		sprintf(prtTemp.content, "����ˮ��:%s", xm_serial);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "��ֵ:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.����ע���������ֵ\n");
		strcat(prtTemp.content, "2.��ֵ��������½�����Ϸ�ٷ���վ����ʾ��ֵ\n");
		strcat(prtTemp.content, "3.��ȯ�۳����ṩ�˻�����ʧ����\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");

		strcat(show_buff,"����ˮ��:");
		strcat(show_buff,xm_serial);
		strcat(show_buff,"\n");
		strcat(show_buff,"����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff,"\n");
		strcat(show_buff,"����:");
		strcat(show_buff,gbl.card_pwd);
		strcat(show_buff,"\n");

		prtTemp.n += 8;
	}
	else if (atoi(gbl.fee_type)==153||atoi(gbl.fee_type)==182){ //��ݸ�ۿ����ۿ�����
	    Add_advertisement(gbl.fee_type,advertisement,&adv_count);
        sprintf(prtTemp.content, "��ֵ:");
		
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");

	//	prtTemp.n++;
	}
	else if (atoi(gbl.sell_card_type)==900)
	{
		sprintf(prtTemp.content, "��Ч����:%s", eff_yymmdd);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
	else
	{
		sprintf(prtTemp.content, "��Ч����:%s", eff_yymmdd);
		strcat(prtTemp.content, "��ֵ:");
		strcat(prtTemp.content, cardMny);
		strcat(prtTemp.content, "\n");
		prtTemp.n++;
	}
		

	switch(atoi(gbl.sell_card_type))
	{
	case 200:	//�ƶ�
	case 201:
	case 205:
	case 206:
	case 207:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.����ע��������Ч�ڣ������ֵ\n");
		strcat(prtTemp.content, "2.��ֵ�������й��ƶ�13800138000\n");
		strcat(prtTemp.content, "3.��ȯ�۳����ṩ�˻�����ʧ����\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 300:	//��ͨ
	case 301:
	case 311:
	case 308:
	case 309:
	case 310:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.����ע��������Ч�ڣ������ֵ\n");
		strcat(prtTemp.content, "2.����19λ�Ŀ���ֵ������10011");
		strcat(prtTemp.content, "  ��������ֵ������10012\n");
		strcat(prtTemp.content, "3.��ȯ�۳����ṩ�˻�����ʧ����\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 8;
		break;
	case 112:	//�����׿�
	case 113:
		break;
	case 109:	//С��ͨ��ֵ��
	case 110:	
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.��ȯ�����ڹ������ڶ�Ԥ����С��ͨ��ֵʹ��\n");
		strcat(prtTemp.content, "2.������Ч����ʹ�ñ���������ѳ�ֵ�绰96338");
		strcat(prtTemp.content, "��������ʾ�������ͻ����ߣ�10000\n");
		strcat(prtTemp.content, "3.������ʧ����������ֵ�����Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "4.�㶫��Ѷ���ſƼ����޹�˾ӵ�жԱ�ȯ�����ս�");
		strcat(prtTemp.content, "��Ȩ\n");

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		prtTemp.n += 7;
		break;

	case 136:	//���ų�ֵ���ѿ�
	case 139:
	case 135:
	case 142:
	case 143:
	case 144:
	case 128:
	case 129:
	case 130:
	case 137:
		strcat(prtTemp.content, "�ʺ�:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.���������ڹ㶫ʡ�����û���ֵ���ѣ���ֵ�벦��11888\n");
		strcat(prtTemp.content, "2.��ȯ����ʧ�������Ʊ��ܣ�ʧ���Ը����ͷ����ߣ�10000\n");
		strcat(prtTemp.content, "�������ס��ͷ�����0400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "�ʺ�:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 800:	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.������ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "�������벦��400-80-11888��ѯ\n");
		strcat(prtTemp.content, "3.ʹ�÷�������½http://sde.game.sohu.com/����ʾ��ֵ\n");		
		prtTemp.n +=8;

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 801:	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.������ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "�������벦��400-80-11888��ѯ\n");
		strcat(prtTemp.content, "3.ʹ�÷�������½http://sde.game.sohu.com/����ʾ��ֵ\n");
		prtTemp.n += 8;

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 810:
	case 811:
	case 812:
	case 813:
	case 814:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.������ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(prtTemp.content, "�������벦��400-80-11888��ѯ\n");
		strcat(prtTemp.content, "3.ʹ�÷��������½����ͨ��վ www.txtong.com.cn���г�ֵ\n");		
		prtTemp.n += 8;

		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;
	case 807:
	case 808:
	case 809:
	case 804:
	case 805:
	case 806:
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content,"��ܰ��ʾ��\n    ���ڷ�Ʊ��ӡ��������½http://www.dg8888.com��վ��������ʾ��˵�����뿨�ź�������г�ֵ.�ͷ����ߣ�400-80-11888\n");
		prtTemp.n += 8;

		strcat(show_buff, "���ţ�");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "���룺");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 127://��ݸС��ͨ�ۿ�
	case 122:
    case 123:
		strcat(prtTemp.content, "���룺");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "��ѳ�ֵ�绰��96338\n");
		strcat(prtTemp.content,"��ܰ��ʾ��\n    ���ڵ���ȯƱ�ݴ�ӡ��24Сʱ�ڼ�ʱ��ֵ\n");
		prtTemp.n += 6;

		strcat(show_buff, "���룺");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

    case 124://��ݸ����ۿ�
	case 125:
    case 126:
		strcat(prtTemp.content, "���룺");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n\n");
		strcat(prtTemp.content, "��ֵ��ַ��http://www.dgkd.cn\n");
		strcat(prtTemp.content,"��ܰ��ʾ��\n    ���ڵ���ȯƱ�ݴ�ӡ��24Сʱ�ڼ�ʱ��ֵ\n");
		prtTemp.n += 6;

		strcat(show_buff, "���룺");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	case 1001: //��֧����10Ԫ
	case 1002://��֧����20Ԫ
	case 1003:
	case 1005:
	case 1011:
	case 1012:
	case 1013:
	case 1015:
	case 1021:
		
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");	
		strcat(prtTemp.content, " ����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.������Ч����ʹ��\n");
		strcat(prtTemp.content, "2.��ȯ��Ϊ��֧���ʻ���ֵ\n");
		strcat(prtTemp.content, "3.��ȯ����ʧ�������Ʊ��ܣ�ʧ���Ը�\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");
		
		prtTemp.n += 7;
		break;

	case 900:
		strcat(prtTemp.content, "�ʺ�:");
		strcat(prtTemp.content, gbl.card_num);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "����:");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "1.��ȯ��ͨ�����з�ʽ�һ���\n����114ռλ\n���½http://gd.118114.cn/movie\n�������ֻ���½Ժ��ͨ���\n");
		strcat(prtTemp.content, "2.��ȯ����ʧ�������Ʊ��ܣ�ʧ���Ը���\n");
		prtTemp.n += 7;

		strcat(show_buff, "�ʺ�:");
		strcat(show_buff, gbl.card_num);
		strcat(show_buff, "\n");
		strcat(show_buff, "����:");
		strcat(show_buff, gbl.card_pwd);
		strcat(show_buff, "\n");

		break;

	}

	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;

	strcat(show_buff,"�밴��ȷ�ϡ���ӡ");
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
	
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func_safe((char*)pMsg,iPrintLen);
	wrLog("��print_func_safe��iRet = %d", iRet);
	return iRet;
}

//end of �ۿ� ����
//��ӡ������ϸ
int CPrint::print_mer_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------��ȡ��ӡ����------------------*/
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //���
	CTools::chgToMny(gbl.amount, amoMny);	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	strcpy(prtTemp.name, type);					     //�û���λ��
	sprintf(prtTemp.phone, "%s(������)", gbl.order_no);	 //�绰����λ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ӡ����
	prtTemp.n=0;
	sprintf(prtTemp.content,"\nϴ�·ѣ� %s\n\n", prtTemp.total);
	prtTemp.n+=3;
	strcat(prtTemp.content,"�����ݽ�Ϊ���״���Ϳͻ���ѯ����֮���ݡ��������˿ͱ���ƾ֤��\n");
	prtTemp.n+=2;
    


	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//���캯��
//��ӡ������ϸ
int CPrint::print_mer_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------��ȡ��ӡ����------------------*/
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //���
	CTools::chgToMny(gbl.amount, amoMny);	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	strcpy(prtTemp.name, type);					     //�û���λ��
	sprintf(prtTemp.phone, "%s(������)", gbl.order_no);	 //�绰����λ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ӡ����
	prtTemp.n=0;
	sprintf(prtTemp.content,"\nϴ�·ѣ� %s\n\n", prtTemp.total);
	prtTemp.n+=3;
	strcat(prtTemp.content,"�����ݽ�Ϊ���״���Ϳͻ���ѯ����֮���ݡ��������˿ͱ���ƾ֤��\n");
	prtTemp.n+=2;
    
	
	
	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

//end of ���캯��
//��ӡ�齭��Ƶ�Ż���ϸ, 2008-01-31 by lihk
int CPrint::print_CATVDiscount_detail(const char *type)		
{
	int iPrintLen, iRet, i, j, k, flag, len;
	BYTE pMsg[1024];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	char periods[100], temp[100], amoMny[15],mny_buff[12+1];
	char mny[20][MAXAMOLEN];
	char len_buff[2+1];
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.fee_type: [%s]", gbl.fee_type);
	
	iRet = rdField63(gbl, 0);	//��Ƿ�ѷ���0����Ƿ�ѷ���1
	//wrLog("rdField63����ֵ: [%d]", iRet);
	if(iRet != 0 && iRet != 1)
	{
		//wrLog("rdField63���ش���");
		return -1;
	}
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");	//����
	strcpy(prtTemp.type, type);	//��������
	strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);	//�û���
	sprintf(prtTemp.phone, "%s��%s��", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
	//���п���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)
		prtTemp.card[i] = '*';
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*-----------------------------------------------*/
	flag = 1;	//ѭ����ӡ��ʶ
	while(flag)
	{
		if(iRet == 1 || flag == 2)//iRet=0, ��Ƿ����ϸֱ�Ӵ�ӡ���ѽ��; flag=2, ����Ƿ�Ѻ��ӡ���ѽ��
		{
			len = 0;
			if(flag == 2)
			{
				bbzero(temp);
				memcpy(temp, pack.FIELD63, 3);	//3λ��ϸ��
				len = atoi(temp);
			}
			len += 3;
			bbzero(mny_buff);
			bbzero(prtTemp.total);
			memcpy(mny_buff,pack.FIELD63+len,12);
			len += 12;
			CTools::chgToMny(mny_buff,prtTemp.total,0);
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
			//��ӡ����
			prtTemp.n=0;
			sprintf(prtTemp.content, "���ѽ�%sԪ\n",prtTemp.total);
			prtTemp.n++;
			bbzero(len_buff);
			memcpy(len_buff, pack.FIELD63+len, 2); //���ڳ���
			len += 2;
			bbzero(periods);
			memcpy(periods,pack.FIELD63+len,atoi(len_buff));
			wrLog("�Ż����ڣ�[%s]", periods);
			sprintf(prtTemp.content+strlen(prtTemp.content), "�Ż����ڣ�%s\n", periods);
			prtTemp.n++;
			flag = 0;

			bbzero(pMsg);
			wrLog("��ʼ��ӡ����");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}
		else if(iRet == 0)	//��Ƿ����ϸ
		{
			//��ȡ�ϼƽ��
			memset(mny,0,sizeof(mny));
			j = 0;	//������
			for(i=0; i<gbl.detail.nMonCnt; i++)
			{
				for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
					strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
			}
			CTools::getTalAmo(mny, prtTemp.total, j);;
			//ת��Ϊ��д���
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
			//�ֱ��ӡ��������ʱ��
			prtTemp.n=0;
			for(j = 0; j<gbl.detail.nMonCnt; j++)
			{
				memset(temp,0,sizeof(temp));
				sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//�����
				strcat(prtTemp.content,temp);
				for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//����9����ϸ�2�У�
				{
					if(i)	//����һ���ⶼҪ����
						strcat(prtTemp.content,"            ");
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//�����
					strcat(prtTemp.content,temp);
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
					strcat(prtTemp.content,temp);
					strcat(prtTemp.content,"\n");
					prtTemp.n++;
				}
			}	//end for(j = 0;
			flag = 2;	//��Ҫ��ӡ���ѽ��

			strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
			prtTemp.n++;

			bbzero(pMsg);
			wrLog("��ʼ��ӡ����");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}	//end else if(iRet == 1)
		else
			return -1;
	}
	
	return iRet;
}

//���캯��
//��ӡ�齭��Ƶ�Ż���ϸ, 2008-01-31 by lihk
int CPrint::print_CATVDiscount_detail(CPack pack,const char *type)		
{
	int iPrintLen, iRet, i, j, k, flag, len;
	BYTE pMsg[1024];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	char periods[100], temp[100], amoMny[15],mny_buff[12+1];
	char mny[20][MAXAMOLEN];
	char len_buff[2+1];
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	wrLog("gbl.fee_type: [%s]", gbl.fee_type);
	
	iRet = rdField63(gbl, 0);	//��Ƿ�ѷ���0����Ƿ�ѷ���1
	//wrLog("rdField63����ֵ: [%d]", iRet);
	if(iRet != 0 && iRet != 1)
	{
		//wrLog("rdField63���ش���");
		return -1;
	}
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");	//����
	strcpy(prtTemp.type, type);	//��������
	strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);	//�û���
	sprintf(prtTemp.phone, "%s��%s��", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
	//���п���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)
		prtTemp.card[i] = '*';
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*-----------------------------------------------*/
	flag = 1;	//ѭ����ӡ��ʶ
	while(flag)
	{
		if(iRet == 1 || flag == 2)//iRet=0, ��Ƿ����ϸֱ�Ӵ�ӡ���ѽ��; flag=2, ����Ƿ�Ѻ��ӡ���ѽ��
		{
			len = 0;
			if(flag == 2)
			{
				bbzero(temp);
				memcpy(temp, pack.FIELD63, 3);	//3λ��ϸ��
				len = atoi(temp);
			}
			len += 3;
			bbzero(mny_buff);
			bbzero(prtTemp.total);
			memcpy(mny_buff,pack.FIELD63+len,12);
			len += 12;
			CTools::chgToMny(mny_buff,prtTemp.total,0);
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
			//��ӡ����
			prtTemp.n=0;
			sprintf(prtTemp.content, "���ѽ�%sԪ\n",prtTemp.total);
			prtTemp.n++;
			bbzero(len_buff);
			memcpy(len_buff, pack.FIELD63+len, 2); //���ڳ���
			len += 2;
			bbzero(periods);
			memcpy(periods,pack.FIELD63+len,atoi(len_buff));
			wrLog("�Ż����ڣ�[%s]", periods);
			sprintf(prtTemp.content+strlen(prtTemp.content), "�Ż����ڣ�%s\n", periods);
			prtTemp.n++;
			flag = 0;

			bbzero(pMsg);
			wrLog("��ʼ��ӡ����");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}
		else if(iRet == 0)	//��Ƿ����ϸ
		{
			//��ȡ�ϼƽ��
			memset(mny,0,sizeof(mny));
			j = 0;	//������
			for(i=0; i<gbl.detail.nMonCnt; i++)
			{
				for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
					strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
			}
			CTools::getTalAmo(mny, prtTemp.total, j);;
			//ת��Ϊ��д���
			bbzero(prtTemp.capsTotal);
			CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
			//�ֱ��ӡ��������ʱ��
			prtTemp.n=0;
			for(j = 0; j<gbl.detail.nMonCnt; j++)
			{
				memset(temp,0,sizeof(temp));
				sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//�����
				strcat(prtTemp.content,temp);
				for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//����9����ϸ�2�У�
				{
					if(i)	//����һ���ⶼҪ����
						strcat(prtTemp.content,"            ");
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//�����
					strcat(prtTemp.content,temp);
					memset(temp,0,sizeof(temp));
					sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
					strcat(prtTemp.content,temp);
					strcat(prtTemp.content,"\n");
					prtTemp.n++;
				}
			}	//end for(j = 0;
			flag = 2;	//��Ҫ��ӡ���ѽ��

			strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
			prtTemp.n++;

			bbzero(pMsg);
			wrLog("��ʼ��ӡ����");
			iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
			iRet = print_func((char*)pMsg, iPrintLen);
		}	//end else if(iRet == 1)
		else
			return -1;
	}
	
	return iRet;
}

//end of ���캯��

//��ӡ������ϸ,2006-11-21 by lihk
int CPrint::print_CATV_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15], mny[20][MAXAMOLEN], temp[20];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen, iRet, i, j, k;
	PrintTemp prtTemp;   //��ӡģ��

	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);	
	//��ϸ
	rdField63(gbl, 0);
	if (memcmp((char*)pack.FIELD63,"000",3)==0)
	{
		return 0;
	}
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count,"��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);						 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
//	strcpy(prtTemp.name, gbl.user_name);			 //�û���
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
//	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ȡ�ϼƽ��
	memset(mny,0,sizeof(mny));
	j = 0;	//������
	for(i=0; i<gbl.detail.nMonCnt; i++)
	{
		for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
			strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
	}
	CTools::getTalAmo(mny, prtTemp.total, j);;
	//ת��Ϊ��д���
	bbzero(prtTemp.capsTotal);
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
	//�û���������
	switch(atoi(gbl.fee_type))				
	{
	case 100:	//�齭����
	case 112:	//�齭��Ƶ
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s��%s��", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//���ߵ���
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[0].user_name);
		strcpy(prtTemp.phone, gbl.CATV_manual_num);
		break;
	}
	//�ֱ��ӡ��������ʱ��
	prtTemp.n=0;
	for(j = 0; j<gbl.detail.nMonCnt; j++)
	{
		memset(temp,0,sizeof(temp));
		sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//�����
		strcat(prtTemp.content,temp);
		for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//����9����ϸ�2�У�
		{
			if(i)	//����һ���ⶼҪ����
				strcat(prtTemp.content,"            ");
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//�����
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			strcat(prtTemp.content,"\n");
			prtTemp.n++;
		}
	}


	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//���ߵ��Ӹ��캯��
//��ӡ������ϸ,2006-11-21 by lihk
int CPrint::print_CATV_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15], mny[20][MAXAMOLEN], temp[20];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen, iRet, i, j, k;
	PrintTemp prtTemp;   //��ӡģ��

	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	//��ϸ
	rdField63(gbl, 0); 

	if (memcmp((char*)pack.FIELD63,"000",3)==0)
	{
		//��Ƿ�� �ڴ˴�ӡ
		iRet =  print_pre_detail(pack,type); //cut by liuxiao
		return iRet;
	}

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count,"��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);						 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
//	strcpy(prtTemp.name, gbl.user_name);			 //�û���
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
//	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ȡ�ϼƽ��
	memset(mny,0,sizeof(mny));
	j = 0;	//������
	for(i=0; i<gbl.detail.nMonCnt; i++)
	{
		for(k=0; k<gbl.detail.monDetail[i].nCnt; k++)
			strcpy(mny[j++],gbl.detail.monDetail[i].cnt[k].amo);
	}
	CTools::getTalAmo(mny, prtTemp.total, j);;
	//ת��Ϊ��д���
	bbzero(prtTemp.capsTotal);
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);
	//�û���������
	switch(atoi(gbl.fee_type))				
	{
	case 100:	//�齭����
	case 112:	//�齭��Ƶ
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[gbl.CATV_choice].user_name);
		sprintf(prtTemp.phone, "%s��%s��", gbl.CATV_manual_num, gbl.Manual_Detail.cnt[gbl.CATV_choice].card2);
		
		break;
	case 114:	//���ߵ���
		strcpy(prtTemp.name, gbl.Manual_Detail.cnt[0].user_name);
		strcpy(prtTemp.phone, gbl.CATV_manual_num);
		break;
	}
	//�ֱ��ӡ��������ʱ��
	prtTemp.n=0;
	for(j = 0; j<gbl.detail.nMonCnt; j++)
	{
		memset(temp,0,sizeof(temp));
		sprintf(temp, "%-12s", gbl.detail.monDetail[j].tmFrom);	//�����
		strcat(prtTemp.content,temp);
		for(i=0; i<gbl.detail.monDetail[j].nCnt && i<18;i++)		//����9����ϸ�2�У�
		{
			if(i)	//����һ���ⶼҪ����
				strcat(prtTemp.content,"            ");
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%-12s",gbl.detail.monDetail[j].cnt[i].type);	//�����
			strcat(prtTemp.content,temp);
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%7s",gbl.detail.monDetail[j].cnt[i].amo);
			strcat(prtTemp.content,temp);
			strcat(prtTemp.content,"\n");
			prtTemp.n++;
		}
	}

	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//end of ���ߵ��Ӹ��캯��

//picc��ӡ��ϸ����2007-4-24,lihk
int CPrint::print_PICC_detail(const char *type)	
{
	int iPrintLen, i, iRet;
	BYTE pMsg[1024];
	char temp[200], amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ��ӡ����
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny);  //���
	CTools::chgToMny(gbl.amount, amoMny);
	//��д��ӡģ��
	memset(&prtTemp,0,sizeof(prtTemp));
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);		//ϵͳ�ο���
	CTools::formatDate(yymmdd, prtTemp.date);				//����
	CTools::formatTime(hhmmss, prtTemp.tim);				//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");				//����
	strcpy(prtTemp.amo, amoMny);						//ʵ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//�ϼ�
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//ת��Ϊ��д���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//����ʾ�������п���
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	sprintf(prtTemp.phone, "%s(�����к�)", gbl.card_serial);	//����
	
	strcpy(prtTemp.type, type);							//����
	
	wrLog("��ʼ��ӡ����");
	//�ֱ��ӡ��������ʱ��
	prtTemp.n=0;
	bbzero(temp);
	sprintf(prtTemp.content, "�������֤���룺%s\n���ƺţ�%s\n��ϵ�绰��%s\nͶ����%s\n", gbl.CATV_id_card, gbl.car_no, gbl.phone_no, amoMny);
	prtTemp.n = 4;
	strcat(prtTemp.content, "���չ�˾���ڽ��Ѵ������������������������Ͷ\n");	
	strcat(prtTemp.content, "�����룬�����绰ȷ��Ͷ��������ʼ���ʽ���յ�\n");
	strcat(prtTemp.content, "PICC�绰��83356783ת1112��1110��1109��1113\n");
	prtTemp.n += 3;
    


	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	iRet = print_func((char*)pMsg,iPrintLen);
	wrLog("��print_func��iRet = %d", iRet);
	return iRet;
}

//���캯��
//picc��ӡ��ϸ����2007-4-24,lihk
int CPrint::print_PICC_detail(CPack pack,const char *type)	
{
	int iPrintLen, i, iRet;
	BYTE pMsg[1024];
	char temp[200], amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ��ӡ����
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny);  //���
	CTools::chgToMny(gbl.amount, amoMny);
	//��д��ӡģ��
	memset(&prtTemp,0,sizeof(prtTemp));
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);		//ϵͳ�ο���
	CTools::formatDate(yymmdd, prtTemp.date);				//����
	CTools::formatTime(hhmmss, prtTemp.tim);				//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");				//����
	strcpy(prtTemp.amo, amoMny);						//ʵ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//�ϼ�
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//ת��Ϊ��д���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//����ʾ�������п���
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	sprintf(prtTemp.phone, "%s(�����к�)", gbl.card_serial);	//����
	strcpy(prtTemp.type, type);							//����
	
	wrLog("��ʼ��ӡ����");
	//�ֱ��ӡ��������ʱ��
	prtTemp.n=0;
	bbzero(temp);
	sprintf(prtTemp.content, "�������֤���룺%s\n���ƺţ�%s\n��ϵ�绰��%s\nͶ����%s\n", gbl.CATV_id_card, gbl.car_no, gbl.phone_no, amoMny);
	prtTemp.n = 4;
	strcat(prtTemp.content, "���չ�˾���ڽ��Ѵ������������������������Ͷ\n");	
	strcat(prtTemp.content, "�����룬�����绰ȷ��Ͷ��������ʼ���ʽ���յ�\n");
	strcat(prtTemp.content, "PICC�绰��83356783ת1112��1110��1109��1113\n");
	prtTemp.n += 3;
    
	
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);	
	iRet = print_func((char*)pMsg,iPrintLen);
	wrLog("��print_func��iRet = %d", iRet);
	return iRet;
}

//end of ���캯��

//��ӡ��Ӱ�ۿ�Ʊƾ��
int CPrint::print_film_detail(const char *type)
{
	BYTE pMsg[1024];
	char film_no[31], amoMny[15], temp[5];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------��ȡ��ӡ����------------------*/
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //���
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type); 					 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	strcpy(prtTemp.name, gbl.user_name);			 //�û���
//	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.month, "���վݲ���Ϊ����ƾ֤");	 //������λ��
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ӡ����
	prtTemp.n=0;
	bbzero(temp);
	memcpy(temp, pack.FIELD63, 2);
	bbzero(film_no);
	memcpy(film_no, pack.FIELD63+2, atoi(temp));

	sprintf(prtTemp.content,"�һ����룺%s\n",film_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "֤�����룺%s\n", gbl.id_num);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�ֻ����룺%s\n", gbl.phone_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "��Ʊ������%d\n", gbl.num);
	prtTemp.n += 4;
	strcat(prtTemp.content, "���վݲ���Ϊ��ȡ��ӰƱ��Ψһƾ֤\n");
	strcat(prtTemp.content, "1.�����๬��ӰԺƾ�˶һ���������Ʊ����Ʊ��\n");
	strcat(prtTemp.content, "2.�ö�����ӰƱ�����๬��ӰԺ�ۿ��κγ��ε�Ӱ");
	strcat(prtTemp.content, "(��ʥ���ڡ����˽���)��\n");
	strcat(prtTemp.content, "3.��Ч�����������·������������ڡ�\n");
	prtTemp.n += 4;
	

	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//���캯��
//��ӡ��Ӱ�ۿ�Ʊƾ��
int CPrint::print_film_detail(CPack pack,const char *type)
{
	BYTE pMsg[1024];
	char film_no[31], amoMny[15], temp[5];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------��ȡ��ӡ����------------------*/
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //���
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type); 					 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	strcpy(prtTemp.name, gbl.user_name);			 //�û���
	//	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.month, "���վݲ���Ϊ����ƾ֤");	 //������λ��
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ӡ����
	prtTemp.n=0;
	bbzero(temp);
	memcpy(temp, pack.FIELD63, 2);
	bbzero(film_no);
	memcpy(film_no, pack.FIELD63+2, atoi(temp));
	
	sprintf(prtTemp.content,"�һ����룺%s\n",film_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "֤�����룺%s\n", gbl.id_num);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�ֻ����룺%s\n", gbl.phone_no);
	sprintf(prtTemp.content+strlen(prtTemp.content), "��Ʊ������%d\n", gbl.num);
	prtTemp.n += 4;
	strcat(prtTemp.content, "���վݲ���Ϊ��ȡ��ӰƱ��Ψһƾ֤\n");
	strcat(prtTemp.content, "1.�����๬��ӰԺƾ�˶һ���������Ʊ����Ʊ��\n");
	strcat(prtTemp.content, "2.�ö�����ӰƱ�����๬��ӰԺ�ۿ��κγ��ε�Ӱ");
	strcat(prtTemp.content, "(��ʥ���ڡ����˽���)��\n");
	strcat(prtTemp.content, "3.��Ч�����������·������������ڡ�\n");
	prtTemp.n += 4;
	
	
	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

//end of ���캯��
//��ӡ����ת����ϸ
//type: 0, ��ͨ���ѷ�����ϸ��Ĭ�ϣ���1, �ϱʽ��ײ�ѯ������ϸ; 2, �ղ�ѯ/��ʷ��ѯ
int CPrint::print_ban_detail(int type)		
{
	int iPrintLen , iRet, len = 0;
	char pMsg[1024];
	char tmp[100], tmpLen[10], rePrtBuff[15], uniStan[13], peStan[13];
	char cDate[11], cTime[9], yymmdd[9], hhmmss[7] ;
	char merName[1024], merNum[16], uniPosNum[9], sndBank[30], rcvBank[30];
	char card[51], batNum[7], serial[7], liqDate[9];
	char prc_amo[13], prc_amo_buff[13];	//������
	char tra_amo[13], tra_amo_buff[13];	//���׽��
	char tal_amo[13], tal_amo_buff[13];	//�ܽ��
		
	/*-----------------------test----------------------*
	strcpy(gbl.card_no, "6228480080498205717");
	bbzero((char *)pack.FIELD12);
	bbzero((char *)pack.FIELD13);
	CTools::hex_to_byte((BYTE *)"101123", pack.FIELD12, 6);
	CTools::hex_to_byte((BYTE *)"20071203", pack.FIELD13, 8);
	bbzero((char *)pack.FIELD37);
	strcpy((char *)pack.FIELD37, "000181981540");
	bbzero((char *)pack.FIELD63);
	strcpy((char *)pack.FIELD63, "045��ͨ���й��ݷ��У����ݰ�����ҵ�������޹�˾��\n301440183989010020099101101035800   1103015810   1962226007100004831330000010001071203101332954408000000000001000000000000");
	/*-------------------------------------------------*/
	bbzero(peStan);
	memcpy(peStan, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
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
	bbzero(cDate);	//ת�����ں�ʱ��
	bbzero(cTime);
	CTools::formatDate(yymmdd, cDate);
	CTools::formatTime(hhmmss, cTime);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//wrLog("field63: [%s]", pack.FIELD63+len);
	//�̻���
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 3);
	len+=3;
	bbzero(merName);
	memcpy(merName, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	/*�ͻ���
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 3);
	len+=3;
	bbzero(custName);
	memcpy(custName, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen); */
	//�̻���
	bbzero(merNum);
	memcpy(merNum, pack.FIELD63+len, 15);
	len+=15;
	//�����ն˺�
	bbzero(uniPosNum);
	memcpy(uniPosNum, pack.FIELD63+len, 8);
	len+=8;
	//������
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(sndBank);
	memcpy(sndBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//�յ���
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(rcvBank);
	memcpy(rcvBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//����
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	if(atoi(tmpLen) > 50)
	{
		wrLog("���Ź���");
		return -1;
	}
	bbzero(card);
	memcpy(card, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	wrLog("card: [%s]", card);
	//���κ�
	bbzero(batNum);
	memcpy(batNum, pack.FIELD63+len, 6);
	len+=6;
	//��ˮ��
	bbzero(serial);
	memcpy(serial, pack.FIELD63+len, 6);
	len+=6;
	//��������
	bbzero(tmp);
	memcpy(tmp, pack.FIELD63+len, 4);
	len+=4;
	bbzero(liqDate);
	CTools::formatDate(tmp, liqDate, 2);
	//12λϵͳ�ο���
	bbzero(uniStan);
	memcpy(uniStan, pack.FIELD63+len, 12);
	len+=12;	
	//���
	bbzero(tal_amo);
	memcpy(tal_amo, pack.FIELD63+len, 12);
	len+=12;
	bbzero(tal_amo_buff);
	CTools::chgToMny(tal_amo, tal_amo_buff);
	//������
	bbzero(prc_amo);
	memcpy(prc_amo, pack.FIELD63+len, 12);
	bbzero(prc_amo_buff);
	CTools::chgToMny(prc_amo, prc_amo_buff);

	bbzero(rePrtBuff);
	if(type)	//�ش�ӡ��־
		strcpy(rePrtBuff, "(�ش�ӡƱ��)");
	//���ױ���
	bbzero(tra_amo);
	sprintf(tra_amo, "%012d", atoi(tal_amo)-atoi(prc_amo));
	bbzero(tra_amo_buff);
	CTools::chgToMny(tra_amo, tra_amo_buff);
	/*--------------��ӡ����----------------*/
	len = 0;
	sprintf((char*)pMsg+len, "����ͨ���С�\n�̻�����%s\n�̻��ţ�%s\n", merName, merNum);
	len=strlen((char*)pMsg);
//	sprintf((char*)pMsg+len, "�ͻ�����%s\n", custName);
//	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�ն˺ţ�%s\n����Ա��01\n", uniPosNum);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�����У�%s\n�յ��У�%s\n�տ�ţ�%s\n", sndBank, rcvBank, card);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�������ͣ�ת��\n���κţ�%s\n��ˮ�ţ�%s\n", batNum, serial);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "ʱ�䣺%s %s\n�������ڣ�%s\nϵͳ�ο��ţ�%s\n�����ο��ţ�%s\n", cDate, cTime, liqDate, peStan, uniStan);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�����ʺţ�%s\n���׽�%s\n�����ѣ�%s\n�ܼƣ�%s\n�ֿ���ǩ��%s��\n", gbl.card_no, tra_amo_buff, prc_amo_buff, tal_amo_buff, rePrtBuff);
	len=strlen((char*)pMsg);
	strcat((char*)pMsg, "\n\n------------------------------\n");
	strcat((char*)pMsg, "����ȷ�����Ͻ��� ͬ�⽫����뱾���ʻ�\n");
	strcat((char*)pMsg, "------------------------------\n");
	strcat((char*)pMsg, "______________________________\n");
	/*--------------------------------------*/
	wrLog("��ӡ��ϸ��\n%s", pMsg);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);

	if(!type)
		iRet = print_func((char*)pMsg, iPrintLen, 3,1);
	else
		iRet = print_func((char*)pMsg, iPrintLen, 1);

	return iRet;
}

//��ӡ����ת����ϸ
//type: 0, ��ͨ���ѷ�����ϸ��Ĭ�ϣ���1, �ϱʽ��ײ�ѯ������ϸ; 2, �ղ�ѯ/��ʷ��ѯ
int CPrint::print_Unionban_detail(int type)
{
	int iPrintLen , iRet, len = 0;
	char pMsg[1024];
	char tmp[100], tmpLen[10], rePrtBuff[15], uniStan[13], peStan[13];
	char cDate[11], cTime[9], yymmdd[9], hhmmss[7] ;
	char merName[1024], merNum[16], uniPosNum[9], sndBank[30], rcvBank[30];
	char card[51], batNum[7], serial[7], liqDate[9];
	char prc_amo[13], prc_amo_buff[13];	//������
	char tra_amo[13], tra_amo_buff[13];	//���׽��
	char tal_amo[13], tal_amo_buff[13];	//�ܽ��
	char bgstan[20+1];//��̨�����
		

	bbzero(peStan);
	memcpy(peStan, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
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
	bbzero(cDate);	//ת�����ں�ʱ��
	bbzero(cTime);
	CTools::formatDate(yymmdd, cDate);
	CTools::formatTime(hhmmss, cTime);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	//wrLog("field63: [%s]", pack.FIELD63+len);
	//�̻���
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(merName);
	memcpy(merName, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//�̻���
	bbzero(merNum);
	memcpy(merNum, pack.FIELD63+len, 15);
	len+=15;
	//�����ն˺�
	bbzero(uniPosNum);
	memcpy(uniPosNum, pack.FIELD63+len, 8);
	len+=8;
	//������
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(sndBank);
	memcpy(sndBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//�յ���
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	bbzero(rcvBank);
	memcpy(rcvBank, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	//����
	bbzero(tmpLen);
	memcpy(tmpLen, pack.FIELD63+len, 2);
	len+=2;
	if(atoi(tmpLen) > 50)
	{
		wrLog("���Ź���");
		return -1;
	}
	bbzero(card);
	memcpy(card, pack.FIELD63+len, atoi(tmpLen));
	len+=atoi(tmpLen);
	wrLog("card: [%s]", card);
	//���κ�
	bbzero(batNum);
	memcpy(batNum, pack.FIELD63+len, 6);
	len+=6;
	//��ˮ��
	bbzero(serial);
	memcpy(serial, pack.FIELD63+len, 6);
	len+=6;
	//��������
	bbzero(tmp);
	memcpy(tmp, pack.FIELD63+len, 4);
	len+=4;
	bbzero(liqDate);
	CTools::formatDate(tmp, liqDate, 2);
	//12λϵͳ�ο���
	bbzero(uniStan);
	memcpy(uniStan, pack.FIELD63+len, 12);
	len+=12;
	bbzero(bgstan);
	memcpy(bgstan, pack.FIELD63+len, 20);
	len+=20;
	//���
	bbzero(tal_amo);
	memcpy(tal_amo, pack.FIELD63+len, 12);
	len+=12;
	bbzero(tal_amo_buff);
	CTools::chgToMny(tal_amo, tal_amo_buff);
	//������
	bbzero(prc_amo);
	memcpy(prc_amo, pack.FIELD63+len, 12);
	bbzero(prc_amo_buff);
	CTools::chgToMny(prc_amo, prc_amo_buff);

	bbzero(rePrtBuff);
	if(type)	//�ش�ӡ��־
		strcpy(rePrtBuff, "(�ش�ӡƱ��)");
	//���ױ���
	bbzero(tra_amo);
	sprintf(tra_amo, "%012d", atoi(tal_amo)-atoi(prc_amo));
	bbzero(tra_amo_buff);
	CTools::chgToMny(tra_amo, tra_amo_buff);
	/*--------------��ӡ����----------------*/
	len = 0;
	sprintf((char*)pMsg+len, "��������\n�̻�����%s\n�̻��ţ�%s\n", merName, merNum);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�ն˺ţ�%s\n����Ա��01\n", uniPosNum);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�����У�%s\n�յ��У�%s\n�տ�ţ�%s\n", sndBank, rcvBank, card);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�������ͣ�ת��\n���κţ�%s\n��ˮ�ţ�%s\n", batNum, serial);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "ʱ�䣺%s %s\n�������ڣ�%s\nϵͳ�ο��ţ�%s\n�����ο��ţ�%s\n��̨����ţ�%s\n", cDate, cTime, liqDate, peStan, uniStan,bgstan);
	len=strlen((char*)pMsg);
	sprintf((char*)pMsg+len, "�����ʺţ�%s\n���׽�%s\n�����ѣ�%s\n�ܼƣ�%s\n�ֿ���ǩ��%s��\n", gbl.card_no, tra_amo_buff, prc_amo_buff, tal_amo_buff, rePrtBuff);
	len=strlen((char*)pMsg);
	strcat((char*)pMsg, "\n\n------------------------------\n");
	strcat((char*)pMsg, "����ȷ�����Ͻ��� ͬ�⽫����뱾���ʻ�\n");
	strcat((char*)pMsg, "------------------------------\n");
	strcat((char*)pMsg, "______________________________\n");
	/*--------------------------------------*/
	wrLog("��ӡ��ϸ��\n%s", pMsg);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);

	if(!type)
		iRet = print_func((char*)pMsg, iPrintLen, 3);
	else
		iRet = print_func((char*)pMsg, iPrintLen, 1);

	return iRet;
}


//���ô�ӡ����
//type: ��ӡ����
//flag: ��ӡ��ʽ��Ĭ��Ϊ0����ͨ��ӡ��1Ϊ�����·�Ʊ��2���޽�3������ͷ�10000��
int CPrint::set_content(PPrintTemp prt, char *cont, int type, int flag)
{
	int len = 0, i;
	char enter[20] = "\n\n\n\n", nameEnter[5];	//�û������û�����Ļ���
	char amoMny[100];

	bbzero(nameEnter);
	if(strlen(prt->name) > 37)
		strcpy(nameEnter, "\n");
	else
		strcpy(nameEnter, "\n\n");
	/*------------�����޸ķ�Ʊ20080129--------------*/
	bbzero(amoMny);
	if(flag==1)
	{
			sprintf(amoMny, "(%s: %s, ʵ��: %.2f)", prt->detail98Name, prt->detail98Amo+1, atof(prt->total) + atof(prt->detail98Amo));
	}

	if(flag==1&&!strcmp(gbl.fee_type,"0361"))

	{
	
		sprintf(amoMny,"(ʵ�գ�%.2fԪ)",atof(prt->total));
	}
	
	/*---------------------------------------------*/
	if(type == 1 || type == 3 || type == 8)	//�״�Ʊ
	{
		wrLog("prt->n = %d", prt->n);
		for(i=0; i<9 - prt->n; i++)
			strcat(prt->content,"\n");
	
		if(gbl.print_paper)	// �·�Ʊ
		{
			if(!strcmp(gbl.fee_type, "0092")) //��ɽ�ۺ��շ�
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
					sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n                 %s\n\n       ��Ʊ%s ��ѯ�绰400-80-11888", prt->total, prt->amo, prt->capsTotal, prt->pos);
				else if(flag==1)
					sprintf(cont+len, "        %sԪ\n                 %s\n             %s\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->total, prt->capsTotal, amoMny, prt->pos);
				else if(flag==2)
					sprintf(cont+len, "\n\n\n\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->pos);
				else
				sprintf(cont+len, "\n\n\n\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->pos);
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
					sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n\n             %s\n     ��Ʊ%s �绰�ͷ���400-882-1180", prt->total, prt->amo, prt->capsTotal, prt->pos);
					}
					else
					{
					sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n\n             %s\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->total, prt->amo, prt->capsTotal, prt->pos);
					}
				}
				else if(flag==1)
				{
					if(strcmp(gbl.city_no,"0086")==0)
					{
						sprintf(cont+len, "        %sԪ\n                 %s\n             %s\n     ��Ʊ%s �绰�ͷ���400-882-1180", prt->total, prt->capsTotal, amoMny, prt->pos);
					}
					else
					{

					sprintf(cont+len, "        %sԪ\n                 %s\n             %s\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->total, prt->capsTotal, amoMny, prt->pos);
				
					}
				}
				else if(flag==2)
				{
					if(strcmp(gbl.city_no,"0086")==0)
					{
					sprintf(cont+len, "\n\n\n\n     ��Ʊ%s �绰�ͷ���400-882-1180", prt->pos);
					}
					else
					{
					sprintf(cont+len, "\n\n\n\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->pos);
					}
				}
				else
				{
					if(strcmp(gbl.city_no,"0086")==0)
					{
						sprintf(cont+len, "\n\n\n\n     ��Ʊ%s �绰�ͷ���400-882-1180", prt->pos);
					}
					else
					{
						sprintf(cont+len, "\n\n\n\n     ��Ʊ%s ��ѯ�绰400-80-11888", prt->pos);
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
					sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n\n%s\n\n\n\n     %s\n         400-882-1180", prt->total, prt->amo, prt->capsTotal, prt->pos);

				}
				else
				{
					sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n\n%s\n\n\n\n     %s\n         400-80-11888", prt->total, prt->amo, prt->capsTotal, prt->pos);
				}
				
			
			}
			else if(flag==1)
			{
				if(strcmp(gbl.city_no,"0086")==0)
				{
					sprintf(cont+len, "        %sԪ\n                 %s\n%s\n\n\n\n     %s\n         400-882-1180", prt->total, prt->capsTotal, amoMny, prt->pos);
				}
				else
				{
					sprintf(cont+len, "        %sԪ\n                 %s\n%s\n\n\n\n     %s\n         400-80-11888", prt->total, prt->capsTotal, amoMny, prt->pos);
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
	else if(type == 2 || type == 4 || type>=10 && type<=14)	//�հ״�ӡֽ
	{
		strcat(prt->content,"\n\n");
		sprintf(cont+len, "�������ס�\n��Ʊʱ�䣺%s %s\nƱ  ����%s\n", prt->date, prt->tim, prt->count);
		len = strlen(cont);
		sprintf(cont+len, "��  ����%s\n�û����룺%s\n", prt->name, prt->phone);
		len = strlen(cont);
		sprintf(cont+len, "ϵͳ�ο��ţ�%s\n�����£�%s\n", prt->stan, prt->month);
		len = strlen(cont);
		sprintf(cont+len, "������Ŀ��%s\n%s\n", prt->type, prt->content);
		len = strlen(cont);
// 		sprintf(cont+len,"  ʹ�ý�������վ�����Ѹ����ɣ�\n      www.PayEasy.com.cn\n\n\n");
// 		len = strlen(cont);

		if(flag==0){

			if (strcmp(gbl.acc_city_no,"0769")==0)
			{
				sprintf(cont+len, "��  �ƣ�%s��ʵ�ۣ�%s��\n����Һϼƣ���д����\n%s\n��  Ʊ��%s\n�����׿ͷ�: 400-80-11888\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
			}
			
			else if(strcmp(gbl.acc_city_no,"0086")==0) //�����epos ȫ���ƹ�� ���޸ķ�Ʊ �绰�ͷ����绰�ͷ���400-882-1180
			
			{
				sprintf(cont+len, "��  �ƣ�%s��ʵ�ۣ�%s��\n����Һϼƣ���д����\n%s\n��  Ʊ��%s\n�绰�ͷ�: 400-882-1180\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
			
			}

			else
				sprintf(cont+len, "��  �ƣ�%s��ʵ�ۣ�%s��\n����Һϼƣ���д����\n%s\n��  Ʊ��%s\n�����׿ͷ�: 400-80-11888\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
		

		}

		else if(flag==1)
		{
			if (strcmp(gbl.acc_city_no,"0769")==0)
			{
				sprintf(cont+len, "��  �ƣ�%s\n����Һϼƣ���д����\n%s\n%s\n��  Ʊ��%s\n�����׿ͷ�: 400-80-11888\n\n%s", prt->total, prt->capsTotal, amoMny, prt->pos, enter);
			}
			else if(strcmp(gbl.city_no,"0086")==0)
			{
					sprintf(cont+len, "��  �ƣ�%s��ʵ�ۣ�%s��\n����Һϼƣ���д����\n%s\n��  Ʊ��%s\n�绰�ͷ�: 400-882-1180\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);

			}

			else
				sprintf(cont+len, "��  �ƣ�%s\n����Һϼƣ���д����\n%s\n%s\n��  Ʊ��%s\n�����׿ͷ�: 400-80-11888\n\n%s", prt->total, prt->capsTotal, amoMny, prt->pos, enter);
		}

		else if(flag==2)
			sprintf(cont+len, "\n\n\n\n\n     %s", prt->pos);
		else if(flag==3)
			sprintf(cont+len, "��  �ƣ�%s��ʵ�ۣ�%s��\n����Һϼƣ���д����\n%s\n��  Ʊ��%s\n���콻���׿ͷ�: 10000\n\n%s", prt->total, prt->amo, prt->capsTotal, prt->pos, enter);
		else
			sprintf(cont+len, "\n\n\n\n\n     %s", prt->pos);
		len = strlen(cont);
		
	}
	else if(type == 9)	//��ɽ�״�Ʊ
	{
		wrLog("=========��ɽ�״�Ʊ=========");
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
			sprintf(cont+len, "        %sԪ\n                 %s\n%s\n\n\n\n     %s", prt->total, prt->capsTotal, amoMny, prt->pos);
		else if(flag==0)
			sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n\n%s\n\n\n     %s", prt->total, prt->amo, prt->capsTotal, prt->pos);
		else
			sprintf(cont+len, "\n\n\n\n\n\n     %s", prt->pos);
		len = strlen(cont);
	}

	//wrLog("��ӡ���ݣ�\n%s", cont);

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

int CPrint::print_ysq_detail(const char *type)		//���л��������ӡ
{
	int iPrintLen , iRet, i;
	BYTE pMsg[1024];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");

    if (!memcmp(gbl.fee_type,"0217",4)) 
	{
	strcpy(prtTemp.name, gbl.user_name);//�û��� �绰����
	strcpy(prtTemp.phone, gbl.phone_no);
	}

	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	strcpy(prtTemp.type, type);						 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ
    if (!memcmp(gbl.fee_type,"0217",4)) //����
	{
	prtTemp.n=0;
	sprintf(prtTemp.content, "�绰���룺%s\n�ʺţ�%s\n�ʻ����ƣ�%s\n�������У�", gbl.phone_no, gbl.card_no, prtTemp.name);
	prtTemp.n += 3;
	memcpy( &(prtTemp.content)[strlen(prtTemp.content)], pack.FIELD63, 60);
	prtTemp.n++;
	strcat(prtTemp.content, "\n˵����\n1.����ɹ��󣬵��Ź�˾���������ʺ�\n�����Զ������û�����ĵ��ŷ���;\n2.�����˶������ٵ���Ӫҵ������;\n3.��ѯ�绰10000.\n");
	prtTemp.n += 6;

    }else if (!memcmp(gbl.fee_type,"0222",4))
    {
		prtTemp.n=0;
		sprintf(prtTemp.content,"�ֻ����룺%s\n���֤��%s\n\n��������ǩԼ���ɹ�\n�ֻ���/���֤���ǽ��ѵ��ʻ������Ʊ���\n������������ӽ�����޸�\n���������밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n",gbl.phone_no,gbl.CATV_id_card);
		prtTemp.n += 7;
    }else  //���ߵ���
	{
	prtTemp.n=0;
	sprintf(prtTemp.content,"�ֻ����룺%s\n���֤��%s\n",gbl.phone_no,gbl.CATV_id_card);
	prtTemp.n += 2;	
	}
    
   	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType, 2);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//���캯��
int CPrint::print_ysq_detail(CPack pack,const char *type)		//���л��������ӡ
{
	int iPrintLen , iRet, i;
	BYTE pMsg[1024];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	
    if (!memcmp(gbl.fee_type,"0217",4)) 
	{
		strcpy(prtTemp.name, gbl.user_name);//�û��� �绰����
		strcpy(prtTemp.phone, gbl.phone_no);
	}
	
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type);						 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ
    if (!memcmp(gbl.fee_type,"0217",4)) //����
	{
		prtTemp.n=0;
		sprintf(prtTemp.content, "�绰���룺%s\n�ʺţ�%s\n�ʻ����ƣ�%s\n�������У�", gbl.phone_no, gbl.card_no, prtTemp.name);
		prtTemp.n += 3;
		memcpy( &(prtTemp.content)[strlen(prtTemp.content)], pack.FIELD63, 60);
		prtTemp.n++;
		strcat(prtTemp.content, "\n˵����\n1.����ɹ��󣬵��Ź�˾���������ʺ�\n�����Զ������û�����ĵ��ŷ���;\n2.�����˶������ٵ���Ӫҵ������;\n3.��ѯ�绰10000.\n");
		prtTemp.n += 6;
		
    }else if (!memcmp(gbl.fee_type,"0222",4))
    {
		prtTemp.n=0;
		sprintf(prtTemp.content,"�ֻ����룺%s\n���֤��%s\n\n��������ǩԼ���ɹ�\n�ֻ���/���֤���ǽ��ѵ��ʻ������Ʊ���\n������������ӽ�����޸�\n���������밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n",gbl.phone_no,gbl.CATV_id_card);
		prtTemp.n += 7;
    }else  //���ߵ���
	{
		prtTemp.n=0;
		sprintf(prtTemp.content,"�ֻ����룺%s\n���֤��%s\n",gbl.phone_no,gbl.CATV_id_card);
		prtTemp.n += 2;	
	}
    
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;

	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType, 2);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//end of ���캯��

int CPrint::print_traffic_detail(const char *type, char *field48, char *field63,int b63Idx)//��ͨ����
{
	int iPrintLen=0 , iRet=0, i=0;
	char name_buf[20],shortname_buf[20],carNum_buf[20],paperNum_buf[20],time_buf[25],lateFee_buf[13],originalFee_buf[13],phone_buf[20],totalFee_buf[13],pMsg[1024], detail_buf[100], amount[12+1], yymmdd[9], hhmmss[7], date[14+1], len_buf[2+1];
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
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
	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	
	strcpy(prtTemp.phone, gbl.phone_no);

	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	//memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//ϵͳ�ο���	
	if(b63Idx==54){
          strcat(prtTemp.stan," (��������ƾ֤) ");
	}
	strcpy(prtTemp.type, type);						 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ

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
//	sprintf(prtTemp.content+strlen(prtTemp.content), "Υ��˳���:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "���֤:%s\n", detail_buf);
	
	b63Idx += 2;//��������
	memcpy(len_buf, field63+b63Idx, 2);//���Ƽ��
	b63Idx += 2;
	bbzero(shortname_buf);
	memcpy(shortname_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
	
	memcpy(len_buf, field48+4, 2);
	bbzero(detail_buf);
	memcpy(detail_buf, field48+6, 4);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "�������:%s\n", detail_buf);
	bbzero(paperNum_buf);
	memcpy(paperNum_buf, field48+10, atoi(len_buf)-4);	
//	sprintf(prtTemp.content+strlen(prtTemp.content), "�����:%s\n", detail_buf);
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(carNum_buf);
	memcpy(carNum_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.phone, "���ƺ���:%s%s", shortname_buf,detail_buf);
	
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
	sprintf(prtTemp.name, "%s", name_buf);//������
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(phone_buf);
	memcpy(phone_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "��ϵ�绰:%s\n", detail_buf);
	
	bbzero(date);
	memcpy(date, field63+b63Idx, 14);
	b63Idx += 14;
	bbzero(time_buf);
	CTools::formatDate(date, time_buf);
	sprintf(time_buf+10,"  ",2);
	CTools::formatTime(date+8, time_buf+12);
//	sprintf(prtTemp.month, "��������:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(originalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, originalFee_buf);			
//	sprintf(prtTemp.content+strlen(prtTemp.content), "������:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(totalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, totalFee_buf);
	memcpy(prtTemp.total, totalFee_buf, strlen(totalFee_buf)-2); //�ϼ�
	strcpy(prtTemp.amo, totalFee_buf);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	bbzero(amount);
	bbzero(lateFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, lateFee_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "���ɽ�:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "ִ����λ:%s\n", detail_buf);

    sprintf(prtTemp.content+strlen(prtTemp.content), "���ƺ���:%s%s\n", shortname_buf,carNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�����:%s\n", paperNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "��������:%s\n", time_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "ԭ������:%s\n", originalFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "���ɽ�:%s\n", lateFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�ϼƽ��:%s\n", totalFee_buf);
/*	sprintf(prtTemp.content+strlen(prtTemp.content), "��ϵ�绰:%s\n", phone_buf);*/

	prtTemp.n += 6;
   
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//���캯��
int CPrint::print_traffic_detail(CPack pack,const char *type, char *field48, char *field63,int b63Idx)//��ͨ����
{
	int iPrintLen=0 , iRet=0, i=0;
	char name_buf[20],shortname_buf[20],carNum_buf[20],paperNum_buf[20],time_buf[25],lateFee_buf[13],originalFee_buf[13],phone_buf[20],totalFee_buf[13],pMsg[1024], detail_buf[100], amount[12+1], yymmdd[9], hhmmss[7], date[14+1], len_buf[2+1];
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	
	strcpy(prtTemp.phone, gbl.phone_no);

	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	if(b63Idx==54){
          strcat(prtTemp.stan," (��������ƾ֤) ");
	}
	strcpy(prtTemp.type, type);						 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ

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
//	sprintf(prtTemp.content+strlen(prtTemp.content), "Υ��˳���:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "���֤:%s\n", detail_buf);
	
	b63Idx += 2;//��������
	memcpy(len_buf, field63+b63Idx, 2);//���Ƽ��
	b63Idx += 2;
	bbzero(shortname_buf);
	memcpy(shortname_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
	
	memcpy(len_buf, field48+4, 2);
	bbzero(detail_buf);
	memcpy(detail_buf, field48+6, 4);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "�������:%s\n", detail_buf);
	bbzero(paperNum_buf);
	memcpy(paperNum_buf, field48+10, atoi(len_buf)-4);	
//	sprintf(prtTemp.content+strlen(prtTemp.content), "�����:%s\n", detail_buf);
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(carNum_buf);
	memcpy(carNum_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.phone, "���ƺ���:%s%s", shortname_buf,detail_buf);
	
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
	sprintf(prtTemp.name, "%s", name_buf);//������
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(phone_buf);
	memcpy(phone_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "��ϵ�绰:%s\n", detail_buf);
	
	bbzero(date);
	memcpy(date, field63+b63Idx, 14);
	b63Idx += 14;
	bbzero(time_buf);
	CTools::formatDate(date, time_buf);
	sprintf(time_buf+10,"  ",2);
	CTools::formatTime(date+8, time_buf+12);
//	sprintf(prtTemp.month, "��������:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(originalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, originalFee_buf);			
//	sprintf(prtTemp.content+strlen(prtTemp.content), "������:%s\n", detail_buf);
	
	bbzero(amount);
	bbzero(totalFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, totalFee_buf);
	memcpy(prtTemp.total, totalFee_buf, strlen(totalFee_buf)-2); //�ϼ�
	strcpy(prtTemp.amo, totalFee_buf);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	bbzero(amount);
	bbzero(lateFee_buf);
	memcpy(amount, field63+b63Idx,sizeof(amount)-1);
	b63Idx += 12;
	CTools::chgToMny(amount, lateFee_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "���ɽ�:%s\n", detail_buf);
	
	memcpy(len_buf, field63+b63Idx, 2);
	b63Idx += 2;
	bbzero(detail_buf);
	memcpy(detail_buf, field63+b63Idx, atoi(len_buf));
	b63Idx += atoi(len_buf);
//	sprintf(prtTemp.content+strlen(prtTemp.content), "ִ����λ:%s\n", detail_buf);

    sprintf(prtTemp.content+strlen(prtTemp.content), "���ƺ���:%s%s\n", shortname_buf,carNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�����:%s\n", paperNum_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "��������:%s\n", time_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "ԭ������:%s\n", originalFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "���ɽ�:%s\n", lateFee_buf);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�ϼƽ��:%s\n", totalFee_buf);
/*	sprintf(prtTemp.content+strlen(prtTemp.content), "��ϵ�绰:%s\n", phone_buf);*/

	prtTemp.n += 6;
   
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
/*	strcat(prtTemp.content, "�˵���������ƾ֤\n");*/
	prtTemp.n++;
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

//end of ���캯��
int CPrint::print_ADD_ADSL(const char *type)
{
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------��ȡ��ӡ����------------------*/
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //���
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type); 					 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	bbzero(prtTemp.name);			 //�û���
	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	bbzero(prtTemp.month);	 
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ӡ����
	prtTemp.n=0;
	
	sprintf(prtTemp.content,"�ͻ����ͣ�%s\n",gbl.adsl_custom_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�������ͣ�%s\n", gbl.adsl_by_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "MODEM���ͣ�%s\n", gbl.adsl_modem_type);
	prtTemp.n += 3;
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n ++;
	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//���캯��
int CPrint::print_ADD_ADSL(CPack pack,const char *type)
{
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet;
	int i;
	PrintTemp prtTemp;   //��ӡģ��
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	/*-----------------��ȡ��ӡ����------------------*/
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(amoMny); //���
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");			 //����
	strcpy(prtTemp.serial, gbl.pos_serial);			 //�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	strcpy(prtTemp.type, type); 					 //ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	bbzero(prtTemp.name);			 //�û���
	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	bbzero(prtTemp.month);	 
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	//��ӡ����
	prtTemp.n=0;
	
	sprintf(prtTemp.content,"�ͻ����ͣ�%s\n",gbl.adsl_custom_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "�������ͣ�%s\n", gbl.adsl_by_type);
	sprintf(prtTemp.content+strlen(prtTemp.content), "MODEM���ͣ�%s\n", gbl.adsl_modem_type);
	prtTemp.n += 3;
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n++;
	bbzero(pMsg);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

//end of ���캯��


int CPrint::print_sx_ysl(int flag)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));

	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);
	prtTemp.n = 2;
	strcpy(prtTemp.phone,gbl.phone_no);
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial,"009310");
	sprintf(prtTemp.stan,"%s","000084606364");
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	bbzero(prtTemp.month);
	sprintf(content,"���֤�ţ�%s\n�ֻ����룺%s\n",gbl.id_num,gbl.telphone);
	strcpy(prtTemp.content,content);
	bbzero(prtTemp.total);
	bbzero(prtTemp.amo);
	bbzero(prtTemp.capsTotal);
		strcpy(prtTemp.pos,gbl.pos_no);
	if (flag == 1)
	{
		strcpy(prtTemp.type,"���л��������ִ");		
	}else if (flag == 2)
	{
		strcpy(prtTemp.type,"���й̻���װADSL�����ִ");		
	}else if (flag == 3)
	{
		strcpy(prtTemp.type,"e8�ײ����������ִ");		
	}else if (flag == 4)
	{
		strcpy(prtTemp.type,"e8���������ײ������ִ");		
	}else if (flag == 5)
	{
		strcpy(prtTemp.type,"e6�ײ����������ִ");		
	}else
		return -1;
	memset(pMsg,0,sizeof(pMsg));

	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

int CPrint::print_ibss(char *telno,char *id_card,char *yyt_name)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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

	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	//����
	bbzero(prtTemp.card);
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	sprintf(content,"ѡ��ĺ��룺%s\n���֤�ţ�%s\nӪҵ�����ƣ�%s\n˵��:\n1.�����Ʊ��ܴ�ӡ��ѡ��ֽ��Я�����֤��ָ������Ӫҵ������\n2.ѡ����Ϣ������Ч������δ��ָ������Ӫҵ������ҵ���������Ч��\n��ѯ����:10000"
		,telno,id_card,yyt_name);
	strcpy(prtTemp.content,content);
	prtTemp.n = 7;
	bbzero(prtTemp.amo);
	bbzero(prtTemp.capsTotal);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"С��ͨѡ�ŵ�");		
	memset(pMsg,0,sizeof(pMsg));
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

int CPrint::print_selectcdma(int flag,char *yyt_name)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);

	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	prtTemp.n = 8;	
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"CDMAѡ�ŵ�");		
	memset(pMsg,0,sizeof(pMsg));
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
		bbzero(prtTemp.card);
	}
	//����
	if (flag == 1)
	{
		if(!memcmp(gbl.card_no, "8828", 4))
			memcpy(prtTemp.card, "888888", 6);
		else
		{
			memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
			for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
				prtTemp.card[i] = '*';
		}
		if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
		{
			bbzero(prtTemp.card);
		}
		strcpy(prtTemp.stan,"000000011099");
		bbzero(amoMny);
		CTools::chgToMny("000000005000", amoMny);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
		strcpy(prtTemp.amo, amoMny);					 //ʵ��
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
		sprintf(content,"��ѡ���룺%s\nѡ�ŷѣ�50.00Ԫ\nӪҵ�����ƣ�%s\n���֤�ţ�%s\n\n˵��:\n1.�����Ʊ��ܴ�ӡ��ѡ��ֽ��ƾ�Ǽǵ����֤����ѡ���Ӫҵ����ȡ�ֻ�SIM����\n2.���ҵ��涨�Ե���Ӫҵ���涨Ϊ׼\n��ѯ����:10000"
		,gbl.telphone,yyt_name,gbl.id_num);
		strcpy(prtTemp.content,content);
		strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

		iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	}else{
		bbzero(prtTemp.card);
		bbzero(prtTemp.amo);
		bbzero(prtTemp.capsTotal);
		bbzero(prtTemp.total);
		sprintf(content,"ѡ��ĺ��룺%s\nӪҵ�����ƣ�%s\n���֤�ţ�%s\n\n˵��:\n1.�����Ʊ��ܴ�ӡ��ѡ��ֽ����24Сʱ��ƾ�Ǽǵ����֤����ѡ���Ӫҵ������ԤԼҵ�����ں��뽫���ٱ���\n2.���ҵ��涨�Ե���Ӫҵ���涨Ϊ׼\n��ѯ����:10000"
		,gbl.telphone,yyt_name,gbl.id_num);
		strcpy(prtTemp.content,content);
		strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

		iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	}

	
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}


int CPrint::print_SELL_POS(const char *type,char *field63)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	//strcpy(prtTemp.stan,gbl.stan_no);
    strcpy(prtTemp.stan , (char*)pack.FIELD37 + strlen((char*)pack.FIELD37) - 12);//ֱ��ʹ��37���12λ ֵ��Ϊstan��
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if (gbl.product_flag == 1||gbl.product_flag == 81||gbl.product_flag == 82)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d̨\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 21||gbl.product_flag == 22||gbl.product_flag == 5)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 3)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 9)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 6)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 7)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
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

	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	if (gbl.product_flag == 4)
	{
		iRet = print_func((char*)pMsg,iPrintLen);
	}else
		iRet = print_func((char*)pMsg,iPrintLen,2);

	return iRet;
}

//���캯��
int CPrint::print_SELL_POS(CPack pack,const char *type,char *field63)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}


	//strcpy(prtTemp.stan,gbl.stan_no);
   // strcpy(prtTemp.stan , (char*)pack.FIELD37 + strlen((char*)pack.FIELD37) - 12);//ֱ��ʹ��37���12λ ֵ��Ϊstan��
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//ϵͳ�ο���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if (gbl.product_flag == 1||gbl.product_flag == 81||gbl.product_flag == 82)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d̨\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 21||gbl.product_flag == 22||gbl.product_flag == 5)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
		,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 3)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 9)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 6)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n������%d��\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
			,gbl.product_name,atoi(gbl.product_num),amoMny,gbl.telphone);
	}else if (gbl.product_flag == 7)
	{
		sprintf(content,"��Ʒ���ƣ�%s\n�ϼƽ�%s\n��ϵ�绰��%s\n\n�ͻ�ǩ����\n"
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
	
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	if (gbl.product_flag == 4)
	{
		iRet = print_func((char*)pMsg,iPrintLen);
	}else
		iRet = print_func((char*)pMsg,iPrintLen,2);

	return iRet;
}
//end of ���캯��

int CPrint::print_payonce_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��

	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, "000084821782", 12);			//ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.phone, gbl.phone_no);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}

	bbzero(amoMny);
	strcpy(gbl.amount, "000000035000");
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	strcpy(prtTemp.content,"һ���Խ���\n��װ�绰��:300.00\n��װ���Ϸ�:50.00\n");
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

int CPrint::print_province_traffic_detail(const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);

	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(��������ƾ֤)");
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name, gbl.user_name);
	strcpy(prtTemp.phone, gbl.telphone);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if(atoi(gbl.transfer_flag)==1){
	    sprintf(prtTemp.content,"���ƺţ���%s\nΥ��ʱ�䣺%s\nΥ����Ϊ��%s\nΥ�µص㣺%s\n ��ַ��%s\n���%s  �ʷѣ�%s\n����ѣ�%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.address,gbl.traffic_mny,gbl.transfer_mny,gbl.service_mny);
	}else{
		sprintf(prtTemp.content,"���ƺţ���%s\nΥ��ʱ�䣺%s\nΥ����Ϊ��%s\nΥ�µص㣺%s\n ���%s\n����ѣ�%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.traffic_mny,gbl.service_mny);
	}

	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//���캯��
int CPrint::print_province_traffic_detail(CPack pack,const char *type)		
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(��������ƾ֤)");
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name, gbl.user_name);
	strcpy(prtTemp.phone, gbl.telphone);			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if(atoi(gbl.transfer_flag)==1){
		sprintf(prtTemp.content,"���ƺţ���%s\nΥ��ʱ�䣺%s\nΥ����Ϊ��%s\nΥ�µص㣺%s\n ��ַ��%s\n���%s  �ʷѣ�%s\n����ѣ�%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.address,gbl.traffic_mny,gbl.transfer_mny,gbl.service_mny);
	}else{
		sprintf(prtTemp.content,"���ƺţ���%s\nΥ��ʱ�䣺%s\nΥ����Ϊ��%s\nΥ�µص㣺%s\n ���%s\n����ѣ�%s\n", gbl.car_no,gbl.traffic_date,gbl.traffic_code,gbl.traffic_place,gbl.traffic_mny,gbl.service_mny);
	}
	
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//end of ���캯��

int CPrint::print_bestpay_detail(const char *type)	//��֧����ӡ
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(��������ƾ֤)");
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name,"");			//�û���
	strcpy(prtTemp.phone, "");			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}

	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, gbl.amount, strlen(gbl.amount)-2);
	strcpy(prtTemp.amo, gbl.amount);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	sprintf(prtTemp.content,"��ֵ�˺�:%s\n��ֵ���:%s\n�˻����:%s\n",gbl.bestpayAccount,gbl.amount,gbl.bestpayTotalMny);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;

}

//���ͨ����

int CPrint::print_yct_sellDetail(const char *type)	//
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count,"��1��/��1��");
	memcpy(prtTemp.stan, (char *)gbl.stan_no, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name,"");			//�û���
	strcpy(prtTemp.phone, "");			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	strcpy(prtTemp.content,"���ͨ������\n���ͨ������һ��1100Ԫ����PKI��Կ����PSAM����һ�ţ�");
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}


//���캯��
int CPrint::print_bestpay_detail(CPack pack,const char *type)	//��֧����ӡ
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(��������ƾ֤)");
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name,"");			//�û���
	strcpy(prtTemp.phone, "");			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, gbl.amount, strlen(gbl.amount)-2);
	strcpy(prtTemp.amo, gbl.amount);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	sprintf(prtTemp.content,"��ֵ�˺�:%s\n��ֵ���:%s\n�˻����:%s\n",gbl.bestpayAccount,gbl.amount,gbl.bestpayTotalMny);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

//end of ���캯��


int CPrint::print_xjx_detail(const char *type)//��ӡн��н�ʽ𻮲���Ʊ
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(��������ƾ֤)");
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name,"");			//�û���
	strcpy(prtTemp.phone, "");			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	strcpy(prtTemp.amo, gbl.amount);					 //ʵ��
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny));
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���


		sprintf(prtTemp.content,"ת���˺�:%s\n���׽��:%s\nת���˻�:%s\n",gbl.out_account,amoMny,gbl.in_account);
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//���캯��
int CPrint::print_xjx_detail(CPack pack,const char *type)//��ӡн��н�ʽ𻮲���Ʊ
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);	 //ϵͳ�ο���
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcat(prtTemp.type,"(��������ƾ֤)");
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name,"");			//�û���
	strcpy(prtTemp.phone, "");			 //�绰����
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	strcpy(prtTemp.amo, gbl.amount);					 //ʵ��
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny));
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	
	sprintf(prtTemp.content,"ת���˺�:%s\n���׽��:%s\nת���˻�:%s\n",gbl.out_account,amoMny,gbl.in_account);
	
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//end of ���캯��
int CPrint::print_NHBank_detail(char*card_num,char*recv_name,char*kh_bank,char*tra_amo_buff)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"ũ��ת��");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,"000086192633");
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	sprintf(content,"�տ��˺ţ�%s\n�տ���������%s\n���������ƣ�%s\nת�˽�%s\n",card_num,recv_name,kh_bank,tra_amo_buff);
	strcpy(prtTemp.content,content);
	prtTemp.n = 4;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_customsign_detail(char*id_card,char*custom_no,char*tel_phone)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"�̻�ǩԼ");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if (memcmp(gbl.pos_no+1,"8",1)==0)
	{
		sprintf(content,"�û��ֻ��ţ�%s\n�û����֤�ţ�%s\n�û��ն˺ţ�%s\n��ʾ��1.��Ȩ�������������û��밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888 �Ǽ����Ľ��п���Ӧ���ʻ�����,ϵͳ����һ��������������ʻ���˹���.\n2.������ʹ��ǩԼ�ɹ����ֻ����뷢�͡�1111����106573025157(0.1Ԫ/��)�����Ժ󼴿���ѻ�ȡÿ��e���ɹ���ֵ����Ϣ\n�û�ǩ����",tel_phone,id_card,custom_no);
	}else
		sprintf(content,"�û��ֻ��ţ�%s\n�û����֤�ţ�%s\n�û��ն˺ţ�%s\n��ʾ��\n1.�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888�Ǽ��������п��û�������ǩԼ����Ϊ���ṩ�����ݵ�e����ֵ����\n2.������ʹ��ǩԼ�ɹ����ֻ�����(�����ƶ��û�)���͡�1111����106573025157(0.1Ԫ/��)�����Ժ󼴿���ѻ�ȡÿ��e���ɹ���ֵ����Ϣ\n�û�ǩ����",tel_phone,id_card,custom_no);
	strcpy(prtTemp.content,content);
	prtTemp.n = 9;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,2);
	
	iRet = print_func((char*)pMsg,iPrintLen,2);
	
	return iRet;
}

int CPrint::print_np(char*car_owner,char*car_no,char*tel_phone)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"������Ʊ����");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	if (strlen(gbl.car_no)==5)
	{
		sprintf(content,"���ƺ��룺��A%s\n�������ƣ�%s**   ��ϵ�绰��%s\n�ɷ���ȣ�2010   ��Ʊ��980.00Ԫ\n����ѣ�25.00Ԫ  �ϼƽ�1005.00Ԫ\n",car_no,car_owner,tel_phone);
	}else
		sprintf(content,"���ƺ��룺��%s\n�������ƣ�%s**   ��ϵ�绰��%s\n�ɷ���ȣ�2010   ��Ʊ��980.00Ԫ\n����ѣ�25.00Ԫ  �ϼƽ�1005.00Ԫ\n",car_no,car_owner,tel_phone);
	strcat(content,"ע�����\n");
	strcat(content," 1.�ͷ���Ա��ͨ���µ緽ʽ����ȷ���ʵݵ�ַ;\n");
	strcat(content," 2.��Ʊ�ѳɹ�����,���ǽ���7���������ڴ�ӡ���ҿ����Ʊƾ֤�������ռ���ַ�����������;\n");
	strcat(content," 3.�������Ի��߿ͻ����ϲ�����ԭ��,�����Ʊƾ֤��ӡ���ɹ�,���ǽ���4����������������ϵ.\n");
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
	PrintTemp prtTemp;   //��ӡģ��
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
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

//���캯��
int CPrint::print_chk_amo(CPack pack,const char *type, char *content)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	BYTE pMsg[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	bbzero(prtTemp.phone);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	bbzero(prtTemp.stan);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,type);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

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

//end of ���캯��
/////////////////////////////////��ݸ����/////////////////////////////////////////
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


int CPrint::print_bus_detail(char *bus_card_info, int flag)  //��ӡ��ݸ������ֵ
{
	wrLog("��print_bus_detail��");
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

	PrintTemp prtTemp;   //��ӡģ��	
	memset(&prtTemp, 0, sizeof(prtTemp));

	bbzero(advertisement);
	Add_advertisement(gbl.fee_type,advertisement,&adv_count);

	bbzero(title);
	bbzero(bus_card_infoBCD);
	CTools::byte_to_hex((const BYTE*)bus_card_info,(BYTE*)bus_card_infoBCD,48);

	//��ȡ����
	bbzero(date);	
	strcpy(date,"20");
	memcpy(date+2,bus_card_infoBCD+68,2);
	strcat(date,"/");
	memcpy(date+5,bus_card_infoBCD+70,2);
	strcat(date,"/");
	memcpy(date+8,bus_card_infoBCD+72,2);
	//��ȡʱ��
	bbzero(tim);
	memcpy(tim,bus_card_infoBCD+74,2);
	strcat(tim,":");
	memcpy(tim+3,bus_card_infoBCD+76,2);
	strcat(tim,":");
	memcpy(tim+6,bus_card_infoBCD+78,2);
	//��ȡ�����
	bbzero(bus_card_no_10);
	tranBusCard(bus_card_info+1, bus_card_no_10);
	
	//��ȡ������ϸ
	bbzero(content);
	switch(flag)
	{	
	case 1:

		//bus_amo=(unsigned char)bus_card_info[23] + (unsigned char)bus_card_info[24]*0x0100 + (unsigned char)bus_card_info[25]*0x010000 + (unsigned char)bus_card_info[26]*0x01000000;
		
		if( !strcmp(gbl.bus_selltype,"1") ){
			strcpy(title, "��ݸ���������������վ�");
			strcpy(content, "���ۣ�15.00Ԫ");
		}else if( !strcmp(gbl.bus_selltype,"2") ){
			strcpy(title, "��ݸ������������վ�");
			strcpy(content, "���18.00Ԫ");
		}else if( !strcmp(gbl.bus_selltype,"3") ){
			strcpy(title, "��ݸ��������Ѻ���վ�");
			strcpy(content, "Ѻ��25.00Ԫ");
		}

		if(gbl.HID_TYPE==1) //��λ���󣬵�λ����
		{

			//��ԭ�н��
			bus_old_amo=(unsigned char)bus_card_info[14] + (unsigned char)bus_card_info[13]*0x0100 + (unsigned char)bus_card_info[12]*0x010000 + (unsigned char)bus_card_info[11]*0x01000000;
			//��������
			bus_charge_amo=(unsigned char)bus_card_info[18] + (unsigned char)bus_card_info[17]*0x0100 + (unsigned char)bus_card_info[16]*0x010000 + (unsigned char)bus_card_info[15]*0x01000000;
			//�����н��
			bus_current_amo=(unsigned char)bus_card_info[22] + (unsigned char)bus_card_info[21]*0x0100 + (unsigned char)bus_card_info[20]*0x010000 + (unsigned char)bus_card_info[19]*0x01000000;
	
		}
		else
		{

			bus_old_amo=(unsigned char)bus_card_info[11] + (unsigned char)bus_card_info[12]*0x0100 + (unsigned char)bus_card_info[13]*0x010000 + (unsigned char)bus_card_info[14]*0x01000000;
			bus_charge_amo=(unsigned char)bus_card_info[15] + (unsigned char)bus_card_info[16]*0x0100 + (unsigned char)bus_card_info[17]*0x010000 + (unsigned char)bus_card_info[18]*0x01000000;
			bus_current_amo=(unsigned char)bus_card_info[19] + (unsigned char)bus_card_info[20]*0x0100 + (unsigned char)bus_card_info[21]*0x010000 + (unsigned char)bus_card_info[22]*0x01000000;
			

		}
		
		sprintf(content+strlen(content), "\n��ԭ�н�%d.%02dԪ", bus_old_amo/100, bus_old_amo%100);
		sprintf(content+strlen(content), "\n�������%d.%02dԪ", bus_charge_amo/100, bus_charge_amo%100);
		sprintf(content+strlen(content), "\n�����н�%d.%02dԪ\n", bus_current_amo/100, bus_current_amo%100);

		if( !strcmp(gbl.bus_selltype,"1") ){
			strcat(content, "��ע���˿�һ������ϣ�һ�Ų����˿�����");
		}else if( !strcmp(gbl.bus_selltype,"2") ){
			strcat(content, "��ע���˻����ʱ��Ҫ��IC�������վݷ��ɰ���");
		}else if( !strcmp(gbl.bus_selltype,"3") ){
			strcat(content, "��ע���˻�Ѻ��ʱ��Ҫ��IC�������վݷ��ɰ���");
		}
		n = 4;
		break;
	case 2:
		strcpy(title, "��ݸ����IC����ֵ�վ�");
		strcpy(content, "��ֵ���ͣ���ͨƱ\n�����ͣ�");
		if( !memcmp(bus_card_infoBCD+18, "41", 2) )
			strcat(content, "A��");
		else
			strcat(content, "B��");
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

		sprintf(content+strlen(content), "\n��ԭ�н�%d.%02dԪ", bus_old_amo/100, bus_old_amo%100);
		sprintf(content+strlen(content), "\n�������%d.%02dԪ", bus_charge_amo/100, bus_charge_amo%100);
		sprintf(content+strlen(content), "\n�����н�%d.%02dԪ", bus_current_amo/100, bus_current_amo%100);
		n = 3;
		break;
	}
	strcat(content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	n++;
	strcat(content,advertisement);
	n += adv_count;


	//��ȡ�ϼƽ��
	bbzero(tal);
	tool.chgToMny(gbl.amount, tal, 0);
//	strcat(content,tal);
	
	tool.chgToMny(gbl.amount, amoMny, 0);
	memset(temp,0,sizeof(temp));
//	sprintf(temp,"(ʵ�ۣ�%sԪ)",amoMny);
//	strcat(content,temp);
	//ת��Ϊ��д���
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
		for(int i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//����ʾ�������п���
			tmp_card_no[i] = '*';
	}
	memset(pMsg,0,sizeof(pMsg));
// 	sprintf((char *)pMsg,"\n\n\n\n%17s  %s\n\n\n\n       %s\n    %s\n    %s\n         %s\n\n\n      %s\n\n%s", 
// 		date, tim, bus_card_no_10, tmp_card_no, gbl.bus_serial, gbl.stan, title, content);
	strcpy(prtTemp.date,date);
	strcpy(prtTemp.tim,tim);
	strcpy(prtTemp.count, "��1��/��1��");
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
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

/////////////////////////////////��ݸ����/////////////////////////////////////////

int CPrint::print_dg_adsl_xc(const char *title)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.name,gbl.user_name);	
	strcpy(prtTemp.phone,gbl.phone_no);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,title);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	if (memcmp(gbl.adsl_xc_type,"86431",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1350��15����512K�����˺󸶷ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86443",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1400��14����2M �����˺󸶷ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86434",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1800��12����3M�����˺󸶷ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86433",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1950��15����2M�����˺󸶷ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86422",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1350��15����512K������Ԥ���ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86445",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1400��14����2M ������Ԥ���ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86420",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1800��12����3M������Ԥ���ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else if (memcmp(gbl.adsl_xc_type,"86423",5)==0)
	{
		sprintf(content,"�ײ��������ͣ�\n��1950��15����2M������Ԥ���ѣ�\n�����ţ�%s\n",gbl.adsl_xc_code);
	}else 
		return -1;
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	prtTemp.n = 3;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}
int CPrint::print_new_dg_adsl_xc(const char *title)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.name,gbl.user_name);	
	strcpy(prtTemp.phone,gbl.phone_no);
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no, (char *)pack.FIELD37, 12);//����ϵͳ�ο���

	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,title);		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	sprintf(prtTemp.content,"�û�������%s\nADSL����ţ�%s\n�ײ����ƣ�%s\n�ײͷ��ã�%s\n�ֻ��ţ�%s\n",gbl.user_name,gbl.adsl_number,amoMny,gbl.telphone);
					
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	prtTemp.n = 5;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}
//��ˬ����˫ѡ���ۿ���ӡ����
int CPrint::MyCoolPayEasy_print_card_detail(const char *type)	
{
	int iPrintLen, iRet, i;
	BYTE pMsg[1024];
	char cardMny[15],amoMny[15];
	char yymmdd[8+1],hhmmss[6+1];
	char eff_yymmdd[8+1]; //��Ч����
	char xm_serial[30+1],card_no[20+1],card_pwd[20+1];
	PrintTemp prtTemp;

	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	//��ȡ��ӡ����
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
	bbzero(yymmdd);	//����
	bbzero(hhmmss);	//ʱ��
	CTools::byte_to_hex(pack.FIELD13, (BYTE *)yymmdd, 4);
	CTools::byte_to_hex(pack.FIELD12, (BYTE *)hhmmss, 3);
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,(char *)pack.FIELD37,12);

	bbzero(cardMny);	//��ֵ��ֵ
	CTools::chgToMny(gbl.card_amount, cardMny);

	bbzero(amoMny);  //���
	CTools::chgToMny(gbl.amount, amoMny);

	//��д��ӡģ��
	memset(&prtTemp,0,sizeof(prtTemp));
	if(!memcmp(gbl.fee_type2,"0182",4)){
		sprintf(prtTemp.stan,"#%s",gbl.phone_no);
	}
    else
	    memcpy(prtTemp.stan, (char *)pack.FIELD37, 12);//����ϵͳ�ο���
	CTools::formatDate(yymmdd, prtTemp.date);	//����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");	//����
	strcpy(prtTemp.amo, amoMny);			//ʵ��
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);	//�ϼ�
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal);	//ת��Ϊ��д���
	memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
	for(i=gbl.card_no_len-2; i>gbl.card_no_len-5; i--)	//����ʾ�������п���
		prtTemp.card[i] = '*';
	strcpy(prtTemp.serial, gbl.pos_serial);		//�ն���ˮ
	strcpy(prtTemp.pos, gbl.pos_no);		//�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	sprintf(prtTemp.phone, "%s(�����к�)", gbl.card_serial);//����
	if (atoi(gbl.fee_type)==245)
	{
		strcpy(prtTemp.type, gbl.xmcard_name);
	}else
		strcpy(prtTemp.type, type);  //����
	//��д��ӡ��ϸ
	prtTemp.n=0;

	sprintf(prtTemp.content, "��Ч����:%s", eff_yymmdd);
	strcat(prtTemp.content, "  ��ֵ:");
	strcat(prtTemp.content, cardMny);
	strcat(prtTemp.content, "\n");
	prtTemp.n++;

		

	switch(atoi(gbl.sell_card_type))
	{
	case 136:	//���ų�ֵ���ѿ�
	case 139:
	case 135:
	case 142:
	case 143:
	case 144:
	case 128:
	case 129:
	case 130:
	case 137:
		strcat(prtTemp.content, "�ʺ�:");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, gbl.card_pwd);
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "\n");
		strcat(prtTemp.content, "ϵͳ�Ѿ��Զ�Ϊ�����г�ֵ�����ѯ������Զ���ֵʧ�ܣ����벦��11888�����ֶ���ֵ��\n");
		strcat(prtTemp.content, "�������ס��ͷ�����400-80-11888\n");
		prtTemp.n += 7;
		break;
	}

	strcat(prtTemp.content, "\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	memset(pMsg,0,sizeof(pMsg));
	iPrintLen = set_content(&prtTemp, (char *)pMsg, m_prtType );	
	
	wrLog("��ʼ��ӡ����");
	iPrintLen = strlen((char*)pMsg);
	iRet = print_func_safe((char*)pMsg,iPrintLen);
	wrLog("��print_func_safe��iRet = %d", iRet);
	return iRet;
}

int CPrint::print_yct_detail(const char *type, char *priCont)//���ͨ��ֵ��ӡ
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	PrintTemp prtTemp;   //��ӡģ��
	
	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::getTime(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.phone, gbl.card_num);
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//ϵͳ�ο���				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ
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
		
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤\n�ͷ��绰400-80-11888");
	prtTemp.n += 2;
	
	
	memset(pMsg,0,sizeof(pMsg));
	wrLog("��ʼ��ӡ����");
	

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	iRet = print_func((char*)pMsg, iPrintLen);
	return iRet;
}

int CPrint::print_SZDXTrade_detail(char*qf_amount,char*ss_amount)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.name,gbl.user_name);	
	strcpy(prtTemp.phone,gbl.phone_no);
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"���ź󸶷�");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.stan,"000086192633");
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	sprintf(content,"Ƿ�ѽ�%s\nʵʱ��%s\n",qf_amount,ss_amount);
	strcpy(prtTemp.content,content);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	prtTemp.n = 2;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_credit_payment(char*credit_no)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15],credit_no_buff[50+1];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type,"���ÿ�����");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatDate(yymmdd, prtTemp.month);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	bbzero(credit_no_buff);
	memcpy(credit_no_buff,credit_no,strlen(credit_no));
	for(i=strlen(credit_no)-2;i>strlen(credit_no)-5;i--)	//����ʾ�������п���
			credit_no_buff[i] = '*';
	strcpy(prtTemp.phone,credit_no_buff);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	sprintf(content,"���ÿ����ţ�%s\n��ǿ����ţ�%s\n\n�����%s\n\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤\n",credit_no_buff,prtTemp.card,amoMny);
	strcpy(prtTemp.content,content);
	prtTemp.n = 5;
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	return iRet;
}

int CPrint::print_sell_phone(char *title)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);

	if(strcmp(title, "��ͨ���зź�") == 0)
	{
		strcpy(prtTemp.type,"�й���ͨ���зź�");	
	}
	else
	{
		strcpy(prtTemp.type,"�й�����3G�ź�");	
	}
	
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatDate(yymmdd, prtTemp.month);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	if(strcmp(title, "��ͨ���зź�") == 0)
	{
		sprintf(content,"ѡ�����룺%s\n�۸�%s",gbl.phone_no,amoMny);
	}
	else
	{
		sprintf(content,"�������У���ݸ��\n�ײ����ͣ������÷磻\nѡ�����룺%s��\n�۸�%s��",gbl.phone_no,amoMny);
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
	PrintTemp prtTemp;   //��ӡģ��
	char content[1024];
	bbzero(content);
	BYTE pMsg[1024];
	char  amoMny[15];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan, "8405");
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos, gbl.pos_no);
	strcpy(prtTemp.type,"����ҵ��Ԥ����");		
	memset(pMsg,0,sizeof(pMsg));
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatDate(yymmdd, prtTemp.month);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, "60011022", 8);
		for(i=6;i>3;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
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
		sprintf(timeto, "%s%s01-%s%s%s", year, month, year, month, day);//�Ʒ�����
		
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);

		strcpy(user_name, gbl.bill_list[i].username);//�û���
		strcpy(stan_no, gbl.stan_no);//��ˮ��
		strcpy(phone_no, gbl.phone_no);//�绰����
		strcat(user_name, "(����)");//׷�ӻ���
		strcat(stan_no, "(��ˮ��)");//׷����ˮ��
		strcat(phone_no, "(����)");//׷�ӵ绰����
		strcat(timeto, "(�Ʒ�����)");//׷�ӼƷ�����
		
		bbzero(temp);
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s%-14s%-15s\n%-18s%-30s%-14s%-30s\n\n", " ",user_name, " ", stan_no, " ", phone_no, " ", timeto);
		strcpy(pMsg, temp); 
		bbzero(temp);//��ʼ��temp
		
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
		sprintf(yymmdd,"%-18s%04s��%02s��%02s��(�ɷ�����)", " ",  year, month, day);//�ɷ�ʱ��
		
		sprintf(temp, "%s\n\n", yymmdd);
		strcat(pMsg, temp);

		bbzero(yymmdd);
		strcpy(yymmdd, gbl.bill_list[i].month);

		memset(&content, 0, sizeof(content));
		for(j = 0; j < 8; j++) //��ӡ8��
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
					strcat(item, "��");
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
		

		//����ֲ棬����Ʊ���߶෢Ʊ���෢�Ʒ�Ϊ���һ��������һ��
		//�����˵���������Ѵ�ӡ������Ϊ���һ�ŷ�Ʊ
		//���һ�ŷ�Ʊ��ʵ�ս��Ϊ���ѽ��-ǰ�淢Ʊ���ܽ��
		print_done = 1;
		for(j = 0; j < gbl.bill_list_len; j++)
		{
			if(gbl.bill_list[j].print_flag == 0)
			{
				print_done = 0;
				break;
			}
		}
		
		if(print_done == 1)	//���һ��
		{
			bbzero(fee);
			bbzero(temp);
			CTools::longToMny(totalfee, fee);
			sprintf(temp,"%10s"," ");
			strcat(temp, "����Ӧ�գ�");
			strcat(temp, fee);
			strcat(temp, "Ԫ");		
			strcat(pMsg,temp);

			bbzero(fee);
			bbzero(temp);
			bbzero(temp_fee);
			if(remainfee < 0)
				remainfee = 0;
			CTools::longToMny(remainfee, fee);
			sprintf(temp,"%3s"," ");
			strcat(temp, "����ʵ�գ�");
			strcat(temp, fee);
			strcat(temp, "Ԫ");
			CTools::chgToCap(fee, temp_fee);
			strcat(temp, "   ��Ʊ���(��д)��");
			strcat(temp, temp_fee);
			strcat(pMsg,temp);

		}
		else	//�����һ��
		{
			bbzero(temp);
			bbzero(temp_fee);
			sprintf(temp,"%3s"," ");
			strcat(temp, "����ʵ�գ�");
			strcat(temp, fee);
			strcat(temp, "Ԫ");
			CTools::chgToCap(fee, temp_fee);
			strcat(temp, "   ��Ʊ���(��д)��");
			strcat(temp, temp_fee);

			char sum_temp[50];
			bbzero(sum_temp);
			bbzero(fee);
			bbzero(temp);
			sprintf(temp,"%10s"," ");
			CTools::longToMny(totalfee, fee);
			strcat(sum_temp, "����Ӧ��");
			strcat(sum_temp, fee);
			strcat(sum_temp, "Ԫ");		
			strcat(pMsg,sum_temp);
			strcat(pMsg,temp);
			

		}

		//����remainfee
		remainfee -= totalfee;
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("��ӡ���ݣ�\n%s", pMsg);
	
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
		strcat(user_name, "(����)");
		strcat(stan_no, "(��ˮ��)");
		strcat(phone_no, "(����)");
		strcpy(timeto, "(�Ʒ�����)");
		
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
		sprintf(yymmdd,"%-18s%04s��%02s��%02s��(�ɷ�����)", " ",  year, month, day);
		
		sprintf(temp, "%s\n\n", yymmdd);
		strcat(pMsg, temp);
		strcat(pMsg, "\n\n\n\n\n\n\n\n\n\n");

		bbzero(temp);
		sprintf(temp,"%10s"," ");
		strcat(temp, "��ϸ�ϼƣ�0.00Ԫ");
		strcat(pMsg,temp);
		
		bbzero(fee);
		bbzero(temp);
		if(remainfee < 0)
			remainfee = 0;

		CTools::longToMny(remainfee, fee);
		sprintf(temp,"%3s"," ");
		strcat(temp, "����ʵ�գ�");
		strcat(temp, fee);
		strcat(temp, "Ԫ");
		CTools::chgToCap(fee, temp_fee);
		strcat(temp, "   ����Һϼ�(��д)��");
		strcat(temp, temp_fee);
		strcat(pMsg,temp);
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("��ӡ���ݣ�\n%s", pMsg);

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
		sprintf(timeto, "%s%s01-%s%s%s", year, month, year, month, day);//�Ʒ�����
		
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		bbzero(payCircle);

		strcpy(user_name, "�ͻ�����:");//׷�ӻ���
		strcpy(phone_no, "�ͻ�����:");//׷�ӵ绰����
		strcpy(payCircle, "�Ʒ�����:");//׷�ӼƷ�����
		strcpy(stan_no, "��ˮ��:");//׷����ˮ��
		strcat(user_name, gbl.bill_list[i].username);//�û���
		strcat(stan_no, gbl.stan_no);//��ˮ��
		strcat(phone_no, gbl.phone_no);//�绰����
		strcat(payCircle, timeto);//�Ʒ�����

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
		sprintf(yymmdd,"%-18s%04s��%02s��%02s��(�ɷ�����)", " ",  year, month, day);//�ɷ�ʱ��

		bbzero(temp);
	
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s\n", " ",yymmdd); //�ɷ�ʱ��
		strcpy(pMsg, temp); 
		
		bbzero(temp);
		sprintf(temp, "%-18s%-30s\n%-18s%-30s\n%-18s%-30s\n%-18s%-30s\n\n", " ",user_name, " ", phone_no, " ", stan_no, " ", payCircle);
		strcat(pMsg, temp);
		
		bbzero(yymmdd);
		strcpy(yymmdd, gbl.bill_list[i].month);

		memset(&content, 0, sizeof(content));
		for(j = 0; j < 8; j++) //��ӡ8��
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
					strcat(item, "��");
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
			strcat(pMsg, content[j]);//��content ���������ݴ�ӡ������
			strcat(pMsg, "\n");
		}

		strcat(pMsg, "\n\n\n");
		
		//����ֲ棬����Ʊ���߶෢Ʊ���෢�Ʒ�Ϊ���һ��������һ��
		//�����˵���������Ѵ�ӡ������Ϊ���һ�ŷ�Ʊ
		//���һ�ŷ�Ʊ��ʵ�ս��Ϊ���ѽ��-ǰ�淢Ʊ���ܽ��
		print_done = 1;
		for(j = 0; j < gbl.bill_list_len; j++)
		{
			if(gbl.bill_list[j].print_flag == 0)
			{
				print_done = 0;
				break;
			}
		}
		
		if(print_done == 1)	//���һ��
		{
			//����Ӧ��
			bbzero(fee);
			bbzero(temp);
			CTools::longToMny(totalfee, fee);
			bbzero(temp);
			sprintf(temp, "%18s", " ");
			strcat(temp, "����Ӧ��:");
			strcat(temp, fee);
			strcat(temp, "Ԫ");		
			strcat(pMsg,temp);

			bbzero(fee);
			bbzero(temp);
			if(remainfee < 0)
				remainfee = 0;
			CTools::longToMny(remainfee, fee);
			sprintf(temp,"%3s"," ");
			strcat(temp, "����ʵ�գ�");
			strcat(temp, fee);
			strcat(temp, "Ԫ");
			CTools::chgToCap(fee, temp_fee);
			strcat(temp, "   ��Ʊ���(��д)��");
			strcat(temp, temp_fee);
			strcat(pMsg,temp);
		}
		else	//�����һ��
		{

			char sum_temp[100];
			bbzero(sum_temp);

			sprintf(temp, "%3s", " ");
			strcat(sum_temp, "����ʵ�գ�");
			strcat(sum_temp, fee);
			strcat(sum_temp, "Ԫ");
			CTools::chgToCap(fee, temp_fee);
			strcat(sum_temp, "   ��Ʊ���(��д)��");
			strcat(sum_temp, temp_fee);

			bbzero(fee);
			bbzero(temp);
			CTools::longToMny(totalfee, fee);
			bbzero(temp);
			sprintf(temp, "%18s", " ");
			strcat(temp, "����Ӧ��:");
			strcat(temp, fee);
			strcat(temp, "Ԫ");		
			strcat(pMsg,temp);
			strcat(pMsg,sum_temp);

		}

		//����remainfee
		remainfee -= totalfee;
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("��ӡ���ݣ�\n%s", pMsg);
	
		iRet = print_func(pMsg,iPrintLen);
	}

	}
	//���û���˵���¼��ִ������Ĵ���
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
		
		strcpy(phone_no, "�ͻ�����:");//׷�ӵ绰����
		strcpy(stan_no, "��ˮ��:");//׷����ˮ��
		strcat(stan_no, gbl.stan_no);//��ˮ��
		strcat(phone_no, gbl.phone_no);//�绰����
		
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
		sprintf(yymmdd,"%-18s%04s��%02s��%02s��(�ɷ�����)", " ",  year, month, day);//�ɷ�ʱ��
		
		bbzero(temp);
		
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%-30s\n", " ",yymmdd); //�ɷ�ʱ��
		strcpy(pMsg, temp); 
		bbzero(temp);
		sprintf(temp, "%-18s%-30s\n%-18s%-30s\n\n\n", " ", phone_no, " ", stan_no);
		strcat(pMsg, temp);

		strcat(pMsg, "\n\n\n\n\n\n\n\n\n\n\n");
		bbzero(temp);
		sprintf(temp,"%18s"," ");
		strcat(temp, "��ϸ�ϼƣ�0.00Ԫ");
		strcat(pMsg,temp);

		bbzero(fee);
		bbzero(temp);
		if(remainfee < 0)
			remainfee = 0;

		CTools::longToMny(remainfee, fee);
		sprintf(temp,"%3s"," ");
		strcat(temp, "�ϼƣ�");
		strcat(temp, fee);
		strcat(temp, "(ʵ��)");
		CTools::chgToCap(fee, temp_fee);
		strcat(temp, "   ��Ʊ���(��д)��");
		strcat(temp, temp_fee);
		strcat(pMsg,temp);
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("��ӡ���ݣ�\n%s", pMsg);

		iRet = print_func(pMsg,iPrintLen);
		
		return iRet;
	}

	return iRet;
}


int CPrint::print_new_pep_detail(long print_fee, int print_type) //�°淢Ʊ(��Զ�п�ʼʹ��)
{
	int iPrintLen=0 , iRet=0, i=0, j=0, k=0, line=0, print_done = 1, max_num = 0;
	long totalfee, billfee, payfee, remainfee;
	char temp[1024], pMsg[2048], item[30], fee[20], mny[20], timeto[30], yymmdd[100], year[5], month[3], day[3];
	char prefee[20];
	char user_name[40], stan_no[30], phone_no[20],card_no[31];
	char content[10][512];//����ʾ�˵���Ϣ������

	remainfee = atol(gbl.amount);
	payfee = atol(gbl.amount); //�ɷѽ��
	bbzero(prefee);
	strcpy(prefee,"0.00");//��ʼ��prefee

	for(i = 0; i < gbl.bill_list_len; i++)
	{
		if(gbl.bill_list[i].print_flag != 0)
			continue;

		bbzero(pMsg);
		bbzero(temp);
		max_num = 0; //��ʼ�����������0

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
		sprintf(yymmdd,"%-20s%04s��%02s��%02s��", " ",  year, month, day);
		sprintf(temp, "\n\n\n\n\n\n\n%-18s%\n\n",yymmdd);//�ɷ�����
		strcpy(pMsg,temp);
	
		bbzero(temp);
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		bbzero(card_no);
		strcpy(user_name, gbl.bill_list[i].username);//�û���
		strcpy(stan_no, gbl.stan_no);//��ˮ��
		strcpy(phone_no, gbl.phone_no);//�绰����

		bbzero(temp);
		strcpy(temp, gbl.bill_list[i].month);
		bbzero(year);
		bbzero(month);
		bbzero(day);
		bbzero(timeto);
		memcpy(year, temp, 4);//��ȡ��
		memcpy(month, temp+4, 2);//��ȡ��
		memcpy(day, temp+6, 2);//��ȡ��
		sprintf(timeto, "%s-%s-01: %s-%s-%s", year, month, year, month, day);//�Ʒ�����
		

		//����û�������ˮ�ţ����룬���Ʒ�����.�������к������˺���ʱ���ա�
		/*sprintf(temp, "%-20s%-30s%-25s%-30s\n\n\n\n%-20s%-30s%-25s%-30s\n\n\n\n\n", " ",user_name, " ", phone_no," ",timeto," ",stan_no);*/
		
		//�޸ĺ�
		sprintf(temp, "%-15s%-30s%-20s%-30s\n%-15s%-30s%-20s%-30s\n\n", "�ͻ�����:",user_name, "�ͻ�����:", phone_no,"�Ʒ�����:",timeto,"ϵͳ��ˮ��:",stan_no);
		strcat(pMsg,temp); 
		
		bbzero(yymmdd);
		strcpy(yymmdd,gbl.bill_list[i].month);
		memset(&content, 0, sizeof(content));
		
		for(j = 0; j < 8; j++)  //��ӡ8��
		{
			bbzero(temp);
			sprintf(temp, "%10s", " ");
			strcat(content[j], temp);
		}

		line = 0;//������
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
				
				strcpy(item, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].item_name);//��ȡk�����Ŀ����
				strcpy(fee, gbl.bill_item_list[gbl.bill_list[j].item_index[k]].fee);//��ȡk�����Ŀ���� 
				

				if (strcmp(item,"Ԥ����")==0)//�����k ����Ԥ����
				{

				CTools::chgToPreMny(fee,prefee);//��Ԥ����ת��������.
				continue;//���ڼ����˵��ܼƽ���С�

				}

					
				totalfee += atol(fee);
				billfee += atol(fee);
				CTools::chgToMny(fee, mny, 0); //�����ת����ԪΪ��λ.

				//��ʾ�б��е�����.
				
				if(strlen(item) > 16) //��ʾ������������
				{
					bbzero(temp);
					strcpy(temp,  item);
					bbzero(item);
					memcpy(item, temp, 13);
					strcat(item, "��");
				}

	
				bbzero(temp);

				if(line==0)
				{
					sprintf(temp,"%-20s%-25s","��  Ŀ","��Ԫ��");
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

				if(line>7) //�ж��Ƿ����8��
				{
                    line=0;
				}
			}

			if(max_num > 16) //�鿴�����Ϊ16��

				{
				wrLog("�鿴�����:%d",max_num);
				break;

				}

		//�˴�ʡ���˵�С��
		}

		if(max_num > 16)
		{
			break;
		}
		

		for(j = 0; j < 8; j++) //�ж��Ƿ����8��
		{
		
			strcat(pMsg, content[j]);//��content ���������ݴ�ӡ������
			strcat(pMsg, "\n");

		}


		strcat(pMsg, "\n\n\n\n");//����
		bbzero(fee);
		bbzero(temp);
		CTools::longToMny(totalfee, fee);
		
	    /*sprintf(temp,"%-20s%-10s%-22s%-7s%-18s%-7s"," ",fee," ",prefee," ","0.00");*///��ϸ�ϼ�.���û��Ԥ����Ԥ����Ϊ0.00
	
		//modify �޸�Ϊ
		sprintf(temp,"%-10s%-7s%-10s%-10s%-7s%-10s%-10s%-5s%-10s","���ºϼ�:",fee,"Ԫ","����Ԥ����֧��: ",prefee,"Ԫ","���п�֧��: ","0.00","Ԫ");
		//end modify
		strcat(pMsg, temp);	

		//����ֲ棬����Ʊ���߶෢Ʊ���෢�Ʒ�Ϊ���һ��������һ��
		//�����˵���������Ѵ�ӡ������Ϊ���һ�ŷ�Ʊ
		//���һ�ŷ�Ʊ��ʵ�ս��Ϊ���ѽ��-ǰ�淢Ʊ���ܽ��
		print_done = 1;
		for(j = 0; j < gbl.bill_list_len; j++)
		{
			if(gbl.bill_list[j].print_flag == 0)
			{
				print_done = 0;
				break;
			}
		}
		
		if(print_done == 1)	//���һ��
		{
			bbzero(fee);
			bbzero(temp);
			if(remainfee < 0)
				remainfee = 0;
            
			CTools::longToMny(remainfee, fee);
			//sprintf(temp,"%-18s%-7s\n\n"," ",fee); //�ϼ�
            //modify 2011-12-1
			sprintf(temp,"%-10s%-7sԪ\n\n","�ϼ�:",fee); //�ϼ�
            //end modify

			strcat(pMsg,temp);
			
			bbzero(temp);
			CTools::chgToCap(fee, temp);//��ʵ�գ���д
			char newTemp[1024];
			bbzero(newTemp);
			//sprintf(newTemp,"%-42s%-20s"," ",temp);
			//modify 2011-12-1
			sprintf(newTemp,"%-20s%-20s","(ʵ��)��д: ����� ",temp);
			//end modify

			strcat(pMsg, newTemp);
			
			bbzero(temp);
			//sprintf(temp,"%-30s%-20s"," ",fee);
			//modify 2011-12-1
			sprintf(temp,"%-20s%-20s","(ʵ��)Сд: �� ",fee);
			//end modify
			strcat(pMsg,temp);

		}
		else	//�����һ��
		{
			bbzero(temp);
			//sprintf(temp,"%-12s%-2s\n\n"," ",fee);//�ϼ�
            //modify 2011-12-5
			sprintf(temp,"%-8s%-2sԪ\n\n","�ϼ�:",fee);//�ϼ�
			//end modify
			strcat(pMsg,temp);

			bbzero(temp);
			CTools::chgToCap(fee, temp);//��д(ʵ��)
			char newTemp[1024];
			//sprintf(newTemp,"%-42s%-20s"," ",temp);
			//modify 2011-12-1
			sprintf(newTemp,"%-20s%-20s","��д(ʵ��): ����� ",temp);
			//end modify
			strcat(pMsg, newTemp);

			bbzero(temp);
			//sprintf(temp,"%-25s%-20s"," ",fee);//Сд��ʵ�գ�
			//modify 2011-12-1
			sprintf(temp,"%-20s%-20s","Сд(ʵ��): �� ",fee);
			//end modify
			strcat(pMsg,temp);

		}
		
		//����remainfee
		remainfee -= totalfee;
		
		iPrintLen = strlen(pMsg);
		
		//wrLog("��ӡ���ݣ�\n%s", pMsg);
	
		iRet = print_func(pMsg,iPrintLen);
	}

	//���û���˵���¼��ִ������Ĵ���
	if(gbl.bill_list_len <=0 || max_num > 30)
	{
		
		bbzero(timeto);
		bbzero(user_name);
		bbzero(stan_no);
		bbzero(phone_no);
		strcpy(user_name, gbl.user_name);//�˻�����
		strcpy(stan_no, gbl.stan_no);//��ˮ��
		strcpy(phone_no, gbl.phone_no); //����
		strcpy(timeto," "); //�Ʒ���������Ϊ��

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
		sprintf(yymmdd,"%-20s%04s��%02s��%02s��", " ",  year, month, day);
		sprintf(temp, "\n\n\n\n\n\n\n%-20s%\n\n",yymmdd);//�ɷ�����
		strcpy(pMsg,temp);
			
		bbzero(temp);
		//�����˻�����/�绰����,�Ʒ�����,��
		//sprintf(temp, "%-20s%-30s%-25s%-30s\n\n\n\n%-20s%-30s%-25s%-30s\n\n\n\n", " ",user_name, " ", phone_no," "," "," ",stan_no);
		//�޸ĺ�
		sprintf(temp, "%-15s%-30s%-20s%-30s\n%-15s%-30s%-20s%-30s\n", "�ͻ�����:",user_name, "�ͻ�����:", phone_no,"�Ʒ�����:",timeto,"ϵͳ��ˮ��:",stan_no);
		strcat(pMsg,temp);
		
		strcat(pMsg, "\n\n\n\n\n\n\n\n\n\n\n\n");//ת��Ʒ�ͳ����

		bbzero(fee);
		bbzero(temp);
		if(remainfee < 0)
			remainfee = 0;

		CTools::longToMny(remainfee, fee); 
		bbzero(temp);
	    //sprintf(temp,"%-78s%-7s%-14s%-7s\n\n"," ","0.00"," ",fee);//�ֽ�֧��
		sprintf(temp,"%-15s%-10sԪ%-10s%-10sԪ\n\n","���п�֧��:","0.00","�ֽ�֧��: ",fee);//�ֽ�֧��
		strcat(pMsg,temp);

		bbzero(temp);
		CTools::chgToCap(fee, temp);
		char newTemp[1024];
		bbzero(newTemp);
		sprintf(newTemp,"%-10s%-20s","��д(ʵ��): ����� ",temp);//��д(ʵ��)
		strcat(pMsg, newTemp);

		bbzero(temp);
		sprintf(temp,"%-10s%-20s","Сд(ʵ��): �� ",fee); //Сд(ʵ��)
		strcat(pMsg,temp);

		iPrintLen = strlen(pMsg);	
	//	wrLog("��ӡ���ݣ�\n%s", pMsg);

		iRet = print_func(pMsg,iPrintLen);
		//AfxMessageBox(pMsg,MB_OK);
		return iRet;
	}

	return iRet;
}


int CPrint::print_game_recharge(const char* title, const char* message)
{
	int iPrintLen=0 , iRet=0, i=0;
	PrintTemp prtTemp;   //��ӡģ��
	char  amoMny[15];
	BYTE* pMsg = new BYTE[1024];
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
	
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);	
	
	
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type, title);		
	memset(pMsg,0, 1024);
	
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatDate(yymmdd, prtTemp.month);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	strcpy(prtTemp.content, message);
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	
	iRet = print_func((char*)pMsg,iPrintLen);
	
	delete[] pMsg;
	return iRet;
}

int CPrint::print_coach_ticket_detail(const char* title, const char* message)
{
	int iPrintLen=0 , iRet=0, i=0, k=0;
	PrintTemp prtTemp;   //��ӡģ��
	char  amoMny[15];
	BYTE pMsg[2048];
	

	memset(&prtTemp, 0, sizeof(prtTemp));

	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
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
		
	strcpy(prtTemp.count, "��1��/��1��");
	bbzero(prtTemp.name);		
		
	strcpy(prtTemp.serial,gbl.pos_serial);
	strcpy(prtTemp.stan,gbl.stan_no);
	bbzero(prtTemp.month);
	strcpy(prtTemp.pos,gbl.pos_no);
	strcpy(prtTemp.type, title);		
	memset(pMsg,0, 2048);
		
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	//CTools::formatDate(yymmdd, prtTemp.month);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}	
	bbzero(prtTemp.phone);
	bbzero(prtTemp.total);
	bbzero(prtTemp.amo);
	bbzero(prtTemp.capsTotal);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
		
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
		
	strcpy(prtTemp.content, message);
	prtTemp.n = 9;

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
		
	iRet = print_func((char*)pMsg,iPrintLen);

	return iRet;
}

//flag: 0-�ڱ궨λ(Ĭ��)
//		1-��ӡ����
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
	
	strcpy(msg, "\n\n\n���������ͨ�Ϳ�Ǽǡ�\n");
	strcat(msg, "----------------------------\n");
	sprintf(msg+strlen(msg), "�ն˺ţ�%s\n", gbl.pos_no);
	sprintf(msg+strlen(msg), "�������ڣ�%s\n", date);
	sprintf(msg+strlen(msg), "�������׽�%.2f\n", atof(normal_amount)/100);
	sprintf(msg+strlen(msg), "���ս��׽�%.2f\n", atof(plus_amount)/100);
	strcat(msg, "\n\n\n\n\n");
	wrLog("msgLen:[%d]", strlen(msg));
    
	iPrintLen = strlen(msg);
	iRet = print_func(msg,iPrintLen);
	wrLog("����ӡ��iRet=%d", iRet);
}

//���ͨ��ֵ��������ӡ
int CPrint::print_yctthird_detail(const char *type, char *priCont)
{
	int iPrintLen , iRet, i,len = 0;
	BYTE pMsg[1024];
	char  amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	PrintTemp prtTemp;   //��ӡģ��
	
	int adv_count=0;
	char advertisement[4096];
	bbzero(advertisement);
	
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::getTime(hhmmss);
	//���
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);  //����
	CTools::formatTime(hhmmss, prtTemp.tim);	//ʱ��
	strcpy(prtTemp.count, "��1��/��1��");
	strcpy(prtTemp.phone, gbl.card_num);
	//����
	if(!memcmp(gbl.card_no, "8828", 4))
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ�������п���
			prtTemp.card[i] = '*';
	}
	strcpy(prtTemp.serial, gbl.pos_serial);
	sprintf(prtTemp.stan,"%s",gbl.stan_no);//ϵͳ�ο���				
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2); //�ϼ�
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	strcpy(prtTemp.pos, gbl.pos_no);				 //�ն˺�
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	/*----------------------------------------------*/
	//��ӡ��ϸ
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
	wrLog("��ʼ��ӡ����");

	iPrintLen = set_yctthird_content(&prtTemp, (char*)pMsg);
	iRet = print_func((char*)pMsg, iPrintLen);

	return iRet;
}

int CPrint::set_yctthird_content(PPrintTemp prt, char *cont)
{

	int len = 0, i;
	char enter[20] = "\n\n\n\n", nameEnter[5];	//�û������û�����Ļ���

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
			
	if (strncmp(prt->type,"���ͨ���",5) != 0)
	{	
		sprintf(cont+len, "        %s��ʵ�ۣ�%s��\n\n             %s\n     ", prt->total, prt->amo, prt->capsTotal);
	}

	len = strlen(cont);
//	wrLog("��ӡ���ݣ�\n%s", cont);
	return len;
}


//��ӡˮ��ú
int CPrint::print_sdm_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	bbzero(prtTemp.name);								//�û���
	strcpy(prtTemp.name,gbl.sdm_CUSTOMERNAME);			//�û���
	strcpy(prtTemp.phone, gbl.sdm_SELECTVALUE);			 //�û�����
	bbzero(prtTemp.month);
	strcpy(prtTemp.month,gbl.SDM_BILLMONTH); 

	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}

	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	sprintf(prtTemp.content,"�շѵ�λ:%s\n��  ��:%s\n�ͻ�����:%s\n�ɷѽ��:%.2fԪ",gbl.sdm_PAYMENTNAME,gbl.SDM_BILLMONTH,gbl.sdm_CUSTOMERNAME,atof(gbl.amount)/100);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
}

//��ӡ3G������

int CPrint::print_g3chargeCard_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	strcpy(prtTemp.phone, gbl.phone_no);			 //��ֵ�ֻ�����
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	sprintf(prtTemp.content,"��ֵ�ֻ���:%s\n��ֵ��ֵ:%sԪ\n��ֵ����:%sM",gbl.phone_no,amoMny,gbl.RECHARGEFLOW);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}


//��ӡ��»qq
//��ӡ3G������

int CPrint::print_glQQ_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	memcpy(stan_no,gbl.TRANSSEQ+(15-strlen(gbl.TRANSSEQ)),12);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	strcpy(prtTemp.phone, gbl.glQQPayInfo.ACCTCODE);			 //��ֵ�ֻ�����
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	
	sprintf(prtTemp.content,"��ֵ����:%s\n��ֵ���:%s\n",gbl.glQQPayInfo.ACCTCODE,amoMny);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");
	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

//��ӡ��ݸͨ�ۿ�����ֵƱ�ݴ�ӡ
//flag:0 ���ۿ���1�ǳ�ֵ ��ֵ��ʱ������Ϊ��ֵƾ֤ 2 ��ʾ�Ծɻ��´�ӡ��Ʊ

int CPrint::print_dgtBus_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	bbzero(gbl.stan_no);
	memcpy(gbl.stan_no,gbl.TRANSSEQ+(strlen(gbl.TRANSSEQ)-12),12);
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	strcpy(prtTemp.phone, gbl.phone_no);			 //��ֵ�ֻ�����
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
	if (strcmp(gbl.fee_type,"0372")==0)
	{

		sprintf(prtTemp.content,"�ۿ�������:%.2fԪ\nӦ�ս��:%.2fԪ\nʵ�ս��:%.2fԪ",atof(gbl.amount)/100,atof(gbl.amount)/100,atof(gbl.amount)/100);
	
	}
	else if (strcmp(gbl.fee_type,"0371") ==0) //����ǳ�ֵ
	{
	
		sprintf(prtTemp.content,"��ԭ�н��:%.2fԪ\n��������:%.2fԪ\n�����н��:%.2fԪ",atof(gbl.dgtCard.tradeBeForeMny)/100,atof(gbl.amount)/100,atof(gbl.dgtCard.cardMny)/100);
		strcat(prtTemp.content, "\nע: ");
		strcat(prtTemp.content, "�����ݽ����ο���������������\n");
		strcat(prtTemp.content,"\n��Ʊ��������ƾ֤\n�ͷ��绰:400-80-11888");
	}

	else
	{
		sprintf(prtTemp.content,"ת��������:%s\nת�뿨����:%s\nת�뿨ԭ�н��:%.2fԪ\nת����:%.2fԪ\nת�뿨���н��:%.2fԪ",gbl.dgtM1Card.bus_showCard,gbl.dgtCard.cardShowId,atof(gbl.dgtCard.tradeBeForeMny)/100,atof(gbl.dgtCard.tradeMny)/100,atof(gbl.dgtCard.cardMny)/100);
		
		strcat(prtTemp.content, "\nע: ");
		strcat(prtTemp.content, "�����ݽ����ο���������������\n");
		strcat(prtTemp.content,"\n��Ʊ��������ƾ֤\n�ͷ��绰:400-80-11888");

	}

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}


//��ӡȫ����ֵƱ��
int CPrint::print_phonePay_detail(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	strcpy(prtTemp.phone, gbl.phone_no);			 //��ֵ�ֻ�����
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	sprintf(prtTemp.content,"��ֵ����:%s",gbl.phone_no);
	//strcat(prtTemp.content,"��ֵ��ֵ:%.2fԪ",atof(gbl.origAmount)/100);
	//strcat(prtTemp.content,"ʵ�ս��:%s",amoMny);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

//��ӡȫ����ֵƱ��
int CPrint::print_phonePay_detailForGDYD(const char *type)
{
	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , iRet, i;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);
	CTools::chgToMny(gbl.amount, amoMny);
	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	strcpy(prtTemp.phone, gbl.phone_no);			 //��ֵ�ֻ�����
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
	strcpy(prtTemp.amo, amoMny);					 //ʵ��
	CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���

	int iorigAmount = atoi(gbl.origAmount);
	double dorigAmount = iorigAmount/100;
	sprintf(prtTemp.content,"��ֵ����:%s \n��ֵ��ֵ:%.2fԪ",gbl.phone_no,dorigAmount);
	
	//strcat(prtTemp.content,"��ֵ��ֵ:%.2fԪ",atof(gbl.origAmount)/100);
	//strcat(prtTemp.content,"ʵ�ս��:%s",amoMny);
	strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

	iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType);
	wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
	iRet = print_func((char*)pMsg,iPrintLen);
	return iRet;
	
}

int CPrint::print_carTax_detail(char *type)
{

	BYTE pMsg[1024];
	char amoMny[15];
	char yymmdd[9];		//8λ������
	char hhmmss[7];		//6λʱ����
	int iPrintLen , i;
	int iRet=0;
	int flag = 0;	//flag = 1������Ʊ
	char stan_no[13];
	bbzero(stan_no);
	PrintTemp prtTemp;   //��ӡģ��
	
	/*----------------��ȡ��ӡ��Ϣ--------------*/
	memset(&prtTemp, 0, sizeof(prtTemp));
	//��ȡ���ں�ʱ��
	bbzero(yymmdd);
	bbzero(hhmmss);
	CTools::getDate(yymmdd);
	CTools::get_time(hhmmss);
	bbzero(amoMny);

	/*-----------------��д��ӡģ��------------------*/
	CTools::formatDate(yymmdd, prtTemp.date);			 //����
	CTools::formatTime(hhmmss, prtTemp.tim);			 //ʱ��
	strcpy(prtTemp.serial, gbl.pos_serial);				//�ն���ˮ
	strcpy(prtTemp.count, "��1��/��1��");
	memcpy(prtTemp.stan,gbl.stan_no,12);
	bbzero(prtTemp.type);						//ҵ������
	strcpy(prtTemp.type,type);
	strcpy(prtTemp.pos, gbl.pos_no);					//�ն˺�
	strcpy(prtTemp.phone, gbl.card_no);			 //��ֵ�ֻ�����
	strcpy(prtTemp.name,gbl.user_name);			//�û���
	
	if(!memcmp(gbl.card_no, "8828", 4))				 //����
		memcpy(prtTemp.card, "888888", 6);
	else
	{
		memcpy(prtTemp.card, gbl.card_no, gbl.card_no_len);
		for(i=gbl.card_no_len-2;i>gbl.card_no_len-5;i--)	//����ʾ��������
			prtTemp.card[i] = '*';
	}
	
	if(m_prtType != 1 && m_prtType != 3 && m_prtType != 8 && m_prtType != 9)  //�Ƿ�Ʊ��ӡֽ����ӡ���ż���ˮ��
	{
        bbzero(prtTemp.serial);
		bbzero(prtTemp.card);
	}
	
	//��ӡ����ʱ�εĳ���˰��ϸ
	for(int j=0;j<gbl.carTaxBills.nCnt;j++)	//��ӡ������	
	{

		char BILLMONTH[10];
		long chargeAmount=0; //Ӧ�ɷ���
		long delayAmount=0; //���ɽ����
		long punishAmount=0; //�������

		bbzero(BILLMONTH);
		bbzero(prtTemp.content);

		sprintf(BILLMONTH,"%s��",gbl.carTaxBills.cardTaxDatas[j].BILLMONTH);
		chargeAmount=atof(gbl.carTaxBills.cardTaxDatas[j].BILLNO)/100;
		delayAmount=atof(gbl.carTaxBills.cardTaxDatas[j].BILLDELAY)/100;
		punishAmount=atof(gbl.carTaxBills.cardTaxDatas[j].BALANCE)/100;

		sprintf(prtTemp.count,"��%d��/��%d��", j+1, gbl.carTaxBills.nCnt);
		sprintf(prtTemp.content,"�������:%s\n�ɷѽ��:%.2fԪ\n���ɽ�:%.2fԪ\n������:%sԪ",BILLMONTH,chargeAmount,delayAmount,punishAmount);
		strcat(prtTemp.content,"\n�����ݽ����ο���������������\n��Ʊ��������ƾ֤");

		CTools::longToMny(chargeAmount,amoMny);
		memcpy(prtTemp.total, amoMny, strlen(amoMny)-2);
		strcpy(prtTemp.amo, amoMny);					 //ʵ��
		CTools::chgToCap(prtTemp.total, prtTemp.capsTotal); //ת��Ϊ��д���
		iPrintLen = set_content(&prtTemp, (char*)pMsg, m_prtType,1);
		wrLog("iPrintLen��%d",iPrintLen);//�����ӡ����
		iRet = print_func((char*)pMsg,iPrintLen);
		return iRet;
	}

	return iRet;
}