// Pack.h: interface for the CPack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACK_H__928AD72D_163D_4949_B193_E1D2E3B5E42D__INCLUDED_)
#define AFX_PACK_H__928AD72D_163D_4949_B193_E1D2E3B5E42D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Def.h"
#include "Global.h"



class CPack  
{
public:
	CPack();
	virtual ~CPack();

	void set_gbl(CGlobal in_gbl);
	void init8583();

	void wrField(int flag, CGlobal in_gbl);
	int Comm_Pack(int flag, BYTE *data_source);  //2007-01-15 by lihk
	int unpackFunc(BYTE *data_source,int data_length);

private:
	int packFunc(BYTE *data_source);
	BYTE Exchange_INQUIRY();
	BYTE Exchange_PAYMENT();
	BYTE Exchange_LOGON();
	BYTE Exchange_ACK(); //羊城通确认，2007-03-14
	BYTE Exchange_REV(); //冲正，2007-03-15

	void SetAllBitMap(BYTE *destmap,BYTE *resmap);
	void SetBitMap( BYTE *bmap, BYTE bitnum);
	BYTE CheckBitMap( BYTE *BitMap, int Bit_No);

	int Comm_Inquiry(BYTE *data_source);	
	int Comm_Payment(BYTE *data_source);
	int Comm_Logon(BYTE *data_source);	
	int Comm_Ack(BYTE *data_source); //羊城通确认，2007-03-14	
	int Comm_Rev(BYTE *data_source); //冲正，2007-03-15

	void wrInqField();
	void wrPayField();
	void wrLogonField();	//2006-10-15
	void wrAckField();	//写确认域, 2007-3-15
	void wrRevField();	//写冲正域, 2007-3-15
	void setNoBit(BYTE *str, int value);	

	
private:
	CGlobal gbl;
	BYTE MTI_Code[2+1];
	
public:
	Packet_Struct Packet_Table[65];

	BYTE	FIELD0_1[16+1];
	BYTE	FIELD2[22+1];
	BYTE	FIELD3[3+1];
	BYTE	FIELD4[6+1];
	BYTE	FIELD7[5+1];
	BYTE	FIELD11[3+1];
	BYTE	FIELD12[3+1];
	BYTE	FIELD13[4+1];
	BYTE	FIELD15[2+1];
	BYTE	FIELD24[2+1];
	BYTE	FIELD32[3+1];
	BYTE	FIELD33[3+1];
	BYTE	FIELD34[16+1];
	BYTE	FIELD35[37+1];
	BYTE	FIELD36[104+1];
	BYTE	FIELD37[12+1];
	BYTE	FIELD38[6+1];
	BYTE	FIELD39[2+1];
	BYTE	FIELD41[8+1];
	BYTE	FIELD42[15+1];
	BYTE	FIELD44[22+1];
	BYTE	FIELD45[99+1];
	BYTE	FIELD46[999+1];
	BYTE    FIELD47[999+1];
	BYTE	FIELD48[42+1];
	BYTE	FIELD52[8+1];
	BYTE	FIELD53[8+1];
	BYTE	FIELD54[39+1];
	BYTE	FIELD55[999+1];
	BYTE	FIELD60[999+1];
	BYTE	FIELD61[15+1];
	BYTE	FIELD62[9+1];
	BYTE	FIELD63[999+1];
	BYTE	FIELD64[8+1];


};

#endif // !defined(AFX_PACK_H__928AD72D_163D_4949_B193_E1D2E3B5E42D__INCLUDED_)
