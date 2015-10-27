// DGTBusTrade.h: interface for the CDGTBusTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGTBUSTRADE1_H__47A997F1_DC93_4908_93A0_009B169B27FA__INCLUDED_)
#define AFX_DGTBUSTRADE1_H__47A997F1_DC93_4908_93A0_009B169B27FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TSCTSI.h"
#include "Def.h"
#include "Global.h"

class CDGTBusTrade  
{


private:

public:
	CDGTBusTrade();
	virtual ~CDGTBusTrade();
	int static Package0203(UCHAR *sData,UCHAR *dData,int DataLen);
	int static unPackage0203(UCHAR *sData,UCHAR *dData,int *DataLen) ;
	UCHAR static getXor(UCHAR *Data, int DataLen);
	void static returnHIDGTFail(char *rsp_code,char *errMsg) ; //��ʾ������ʾ
	int static dgtSndRcvHID_IcData(const TSDLLPosInfo dPosInfo,const BYTE *downData, const int downLen, BYTE *upData, int &upLen,char *errMsg);//��ݸͨͨѶ
	int static cardSearch(const TSDLLPosInfo dPosInfo,char *rsp_code,char *errMsg,PDGTCard pDgtCard ,int flag); //��ݸͨ���ϵ硣Ѱ����
	int static  readCardBasicInfo(const TSDLLPosInfo dPosInfo,char *errMsg,PDGTCard pdgtCard,int flag); //��ȡ��Ƭ������Ϣ
	int static accessWalletDir(const TSDLLPosInfo dPosInfo,char *errMsg);
	int static getWalletMny(const TSDLLPosInfo dPosInfo,char *errMsg,PDGTCard pdgtCard);
	int static cardQuery(const TSDLLPosInfo dPosInfo,char *rsp_code,char *errMsg,PDGTCard pdgtCard,int flag);
	int static buzzer(const TSDLLPosInfo dPosInfo,char *errMsg); //��������һ��
	int static clearLEDMsg(const TSDLLPosInfo dPosInfo,char *errMsg); //��ն�������ʾ��Ϣ
	int static showLEDMsg(const TSDLLPosInfo dPosInfo,char *showMsg,char *errMsg);
	int static init_dgtCard_data(PDGTCard pdgtCard);
	int static readCardInfo(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char *errMsg);//��ȡ��Ƭ��δ����Ǯ��Ŀ¼
	int  static getRandom(const TSDLLPosInfo dPosInfo,char *errMsg,char *Random) ; //��ȡ�����
	int static  openCard(const TSDLLPosInfo dPosInfo,char *APDU,char *dicMac,char *errMsg);
	int static openCard(const TSDLLPosInfo dPosInfo,char *dicMac,char *errMsg,DGTCard dgtCard);
	int static initCPUCard(const TSDLLPosInfo dPosInfo,char *POSID,char *payAmount,char *cardSeq,char *keyVer,char *ALGIND,char *initRandom,char *cardAmount,char *MAC1,char *errMsg);
	int static cpuCard_load(const TSDLLPosInfo dPosInfo,char *tradeTime,char *MAC2,char *TAC,char *errMsg );
	int static checkPIN2(const TSDLLPosInfo dPosInfo,char *appSerial,char *errMsg ); //pin2У��
	int static  getPIN2(char *appSerial,char *PIN2);// ��ȡPIN2
	int static wrTxnLogs(const TSDLLPosInfo dPosInfo,char *posId,DGTCard dgtCard,char *tradeType,char *errMsg); //д��ֵ��¼
	int static readAllTxnLogs(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char *rsp_code,char *errMsg);
	int static  readChargeLogs(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,char *rsp_code,char *errMsg);
	int  static wrCardTxnLog(const TSDLLPosInfo dPosInfo,char *posId,DGTCard dgtCard,char *tradeType,char *errMsg);
	int  static getCardShowId(char *cardID,char *cardShowID);
	int  static  commuWithDGT(const TSDLLPosInfo dPosInfo, char *sndData, int sndLen, char *rcvData, int *rcvLen);
	int static getSyncinfo(char *syncInfo);
	int static getCommuMac(char *poskey,char *keyset,char *keyField,char *commuMac);

	int  static init_dgtM1Card_data(PDGTM1Card pdgtM1Card);
	int  static cardM1Search(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg,PDGTM1Card pDgtM1Card);
	int  static cardM1Query(const TSDLLPosInfo dPosInfo,char *rsp_code,int type,char *errMsg,PDGTM1Card pdgtM1Card);
	int static getCardParentType(const TSDLLPosInfo dPosInfo,char *cardCSN,char *cardParentType,char *cardChildType,char *errMsg);	
	int static getM1BasicInfo(const TSDLLPosInfo dPosInfo,int type,PDGTM1Card pdgtM1Card);  //��ȡ������Ϣ
	int  dgtCardOperate(CGlobal *pgbl,char *tranMessage,char *transType,char *rsp_code,char *rsp_message);
	int  cardManagement(CGlobal *pgbl, char *tranmessage, char *tranType,char *rspp_code,char *rsp_message);
	int	cardSaleTrade(CGlobal *pgbl,char *tranMessage,char *tranType);
	int cardTransFinish(CGlobal *pgbl, char *tranmessage, char *tranType,char *rsp_code,char *rsp_message);
	int cardTrans_OpenCard(CGlobal *pgbl,char *rsp_code,char * rsp_message);
	int trans_card_Amt(CGlobal *pgbl,char *rsp_code,char *rsp_message);
	int dgtCPUInqHdl(TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,int flag) ;//��ȡ��Ƭ������Ϣ
	int static getCardType(const TSDLLPosInfo dPosInfo,char * rsp_code,char *errMsg); //�ϵ�
	int  static getM1SBInfo(const TSDLLPosInfo dPosInfo,PDGTM1Card pdgtM1Card,char * errMsg); //��ȡS1B1��Ϣ
	int  dgtCardInqHdl(const TSDLLPosInfo dPosInfo,PDGTCard pdgtCard,int flag); //��ȡ��Ƭ������Ϣ
	int packdgtFunc(CGlobal *pgbl, char *transMessage, char*tranType,char *sendData,int *sendLen);	//���

private:

	
	int undgtpackFunc(CGlobal *pgbl, char *tranMessage,char *tranType,char *rcvData,int rcvLen,char *rsp_code);					//���
	int  init_dgt_pack( char *transMessage, char *tranType,CGlobal *gbl);
	void  initField();
	int  dgt_trade(CGlobal *pgbl, char *tranMessage, char *tranType,char *rsp_code);
	int  getCommuErr(char *rsp_code,char *rsp_message);
public:

	Pack_dgt_Struct Pack_dgt_table[42]; //��ݸͨ����

	char	dgtFIELD0[65];
	char	dgtFIELD1[65];
	char	dgtFIELD2[65];
	char	dgtFIELD3[65];
	char	dgtFIELD4[65];
	char	dgtFIELD5[65];
	char	dgtFIELD6[65];
	char	dgtFIELD7[65];
	char	dgtFIELD8[65];
	char	dgtFIELD9[65];
	char	dgtFIELD10[65];
	char	dgtFIELD11[65];
	char	dgtFIELD12[65];
	char	dgtFIELD13[65];
	char	dgtFIELD14[65];
	char	dgtFIELD15[65];
	char	dgtFIELD16[65];
	char	dgtFIELD17[65];
	char	dgtFIELD18[65];
	char	dgtFIELD19[65];
	char	dgtFIELD20[65];
	char	dgtFIELD21[65];
	char	dgtFIELD22[65];
	char	dgtFIELD23[65];
	char    dgtFIELD24[65];
	char	dgtFIELD25[65];
	char	dgtFIELD26[65];
	char	dgtFIELD27[65];
	char	dgtFIELD28[65];
	char	dgtFIELD29[65];
	char	dgtFIELD30[65];
	char	dgtFIELD31[65];
	char	dgtFIELD32[65];
	char	dgtFIELD33[65];
	char	dgtFIELD34[65];
	char	dgtFIELD35[65];
	char	dgtFIELD36[65];
	char	dgtFIELD37[65];
	char	dgtFIELD38[65];
	char	dgtFIELD39[65];
	char	dgtFIELD40[65];
	char	dgtFIELD41[65];
};


#endif // !defined(AFX_DGBUSTRADE1_H__47A997F1_DC93_4908_93A0_009B169B27FA__INCLUDED_)
