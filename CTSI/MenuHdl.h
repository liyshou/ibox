// MenuHdl.h: interface for the CMenuHdl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUHDL_H__76295DC7_1FF6_4FFB_8347_DE786754921D__INCLUDED_)
#define AFX_MENUHDL_H__76295DC7_1FF6_4FFB_8347_DE786754921D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "Global.h"


//UINT YCT3Monitor(LPVOID lpv);
//DWORD WINAPI YCT3Monitor(LPVOID lpv);
class CMenuHdl  
{
public:
	static int yct3_flag;
	CMenuHdl();
	virtual ~CMenuHdl();
	int Menu_main_hdl(Coninfo coninfo);	//�˵����������
	int HangUp(SOCKET in_sd);	//�һ�

private:
	int Menu_HOTKEY_hdl(int &iSelect, int &loop,TSMENULIST &menuList, int &par_menu, int &cur_menu, int *list);
	int MenuHdl(int *list, int &par_menu,int &cur_menu,int &iSelect, TSMENULIST &menuList);
	int get_menu_list(int cur_menu, PTSMENULIST pMenuList, int *list);
	void oper_hdl( int id );
// 	int isYctNotFull(char *pos_no);
	int UpdateFiles();

	int get_menu_group_mbl_flag(int tgroup,int askType,int *menu_group_mbl_flag);
	//��ȡ����˵�ǰ����ʾ��
	int get_epos_tips(char*pos_no,int menu_id,char*return_flag,char*tips);
private:
	char thisPrmMD5[16+1];
	SOCKET sd;
	CGlobal gbl;					//ȫ�ֱ���
	TSTradeInfoRecv tradeInfoRecv;	//���ز˵��ķ���Ч����
	YCT_DATA yct_data;
};

#endif // !defined(AFX_MENUHDL_H__76295DC7_1FF6_4FFB_8347_DE786754921D__INCLUDED_)
