// DB.cpp: implementation of the CDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cassert>
#include "TissonCTSI.h"
#include "DB.h"
#include "Global.h"
#include "Pack.h"
#include "ADOConn.h"
#include "Print.h"
#include "Def.h"
#include <cerrno>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB::CDB()
{

}

CDB::~CDB()
{

}

//�����ݿ���ȡ����bank_info����Ϣ����ṹ��Bank_info��
int CDB::getBankInfo() 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;

	wrLog("��getBankInfo��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from bank_info";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!bank_info.empty())
	{
		bank_info.clear();
	}
	
	Bank_info bank_info_tmp;

	while(!record->adoEOF)
	{
		memset(&bank_info_tmp, 0, sizeof(bank_info_tmp));

		strcpy(bank_info_tmp.bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank"));
		bank_info_tmp.key = atoi( (LPCSTR)(_bstr_t)(record->GetCollect("key")) );
		
		bank_info.push_back(bank_info_tmp);

		record->MoveNext();
	}

	wrLog("��getBankInfo�� ������%d��", bank_info.size());

	g_adoConnInfo[index].connStat = 0;

	return 0;
}


//�����ݿ���ȡ����Bank_card����Ϣ����ṹ��Bank_card��
int CDB::getBankCard() 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getBankCard��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from bank_card";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!bank_card.empty())
	{
		bank_card.clear();
	}
	
	Bank_card bank_card_tmp;

	while(!record->adoEOF)
	{
		memset(&bank_card_tmp, 0, sizeof(bank_card_tmp));

		strcpy(bank_card_tmp.card, (LPCTSTR)(_bstr_t)record->GetCollect("card_no"));
		bank_card_tmp.card_len = strlen(bank_card_tmp.card);
		strcpy(bank_card_tmp.bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank"));
		bank_card_tmp.track_no = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("track_no") );
		bank_card_tmp.offset = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("offset") );
		
		bank_card.push_back(bank_card_tmp);

		record->MoveNext();
	}

	wrLog("��getBankCard�� ������%d��", bank_card.size());

	g_adoConnInfo[index].connStat = 0;

	return 0;
}


//�����ݿ���ȡ����bank_busi����Ϣ����ṹ��Bank_busi��
int CDB::getBankBusi() 
{
	int ret, status, index;
	_bstr_t vSQL;
	_RecordsetPtr record;

	wrLog("��getBankBusi��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from bank_busi";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!bank_busi.empty())
	{
		bank_busi.clear();
	}

	Bank_busi bank_busi_tmp;

	while(!record->adoEOF)
	{
		status = atoi( (LPCSTR)(_bstr_t)(record->GetCollect("status")) );
		if(status == 0) //��ͨ��־Ϊ1
		{
			record->MoveNext();
			continue;
		}

		memset(&bank_busi_tmp, 0, sizeof(bank_busi_tmp));

		strcpy(bank_busi_tmp.bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank"));
		strcpy(bank_busi_tmp.biz_type, (LPCTSTR)(_bstr_t)record->GetCollect("biz_type"));
		strcpy(bank_busi_tmp.acc_area, (LPCTSTR)(_bstr_t)record->GetCollect("acc_area"));
		bank_busi_tmp.pos_type = atoi((LPCTSTR)(_bstr_t)record->GetCollect("pos_type"));

		bank_busi.push_back(bank_busi_tmp);
		record->MoveNext();
	}
	
	wrLog("��getBankBusi�� ������%d��", bank_busi.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����Epos_group����Ϣ����ṹ��Epos_group��
int CDB::getEposGroup() 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposGroup��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_group order by tgroup";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!epos_group.empty())
	{
		epos_group.clear();
	}

	Epos_group epos_group_tmp;
	while(!record->adoEOF)
	{
		memset(&epos_group_tmp, 0 ,sizeof(epos_group_tmp));

		epos_group_tmp.tgroup = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("tGroup") );
		epos_group_tmp.group_type = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("group_type") );
		epos_group_tmp.eCard = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("eCard") );
		epos_group_tmp.phs_ticket = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("PHS_TICKET") );
		epos_group_tmp.get_card = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("GET_CARD") );
		epos_group_tmp.DianXin_Turn = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("DianXin_Turn") );
		if((record->GetCollect("welcome")).vt!=VT_NULL)
			strcpy(epos_group_tmp.welcome,(LPCTSTR)(_bstr_t)record->GetCollect("welcome"));
		else
			memset(epos_group_tmp.welcome,0,sizeof(epos_group_tmp.welcome));
		if((record->GetCollect("update_info")).vt!=VT_NULL)
			strcpy(epos_group_tmp.update_info,(LPCTSTR)(_bstr_t)record->GetCollect("update_info"));
		else
			memset(epos_group_tmp.update_info,0,sizeof(epos_group_tmp.update_info));

		epos_group.push_back(epos_group_tmp);

		record->MoveNext();
	}

	wrLog("��getEposGroup�� ������%d��", epos_group.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����Epos_group����Ϣ����ṹ��Epos_group��
int CDB::getFavorableAirPay() 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getFavorableAirPay��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select area,biz_type,name,amount,info from favorable_air_pay where sysdate between begin_date and end_date order by area asc,biz_type asc,priority asc";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	
	if(!favorable_air_pay.empty())
	{
		favorable_air_pay.clear();
	}

	Favorable_air_pay favorable_air_pay_tmp;

	while(!record->adoEOF)
	{   
		memset(&favorable_air_pay_tmp, 0, sizeof(favorable_air_pay_tmp));

		strcpy(favorable_air_pay_tmp.area, (LPCTSTR)(_bstr_t)record->GetCollect("area"));
		strcpy(favorable_air_pay_tmp.biz_type,(LPCTSTR)(_bstr_t)record->GetCollect("biz_type"));
		strcpy(favorable_air_pay_tmp.name,(LPCTSTR)(_bstr_t)record->GetCollect("name"));
		favorable_air_pay_tmp.amount = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("amount") );
		if((record->GetCollect("info")).vt!=VT_NULL)
		{
			strcpy(favorable_air_pay_tmp.info, (LPCTSTR)(_bstr_t)record->GetCollect("info") );
		}else
		   memset(favorable_air_pay_tmp.info,0,sizeof(favorable_air_pay_tmp.info));

		favorable_air_pay.push_back(favorable_air_pay_tmp);

		record->MoveNext();
	}
	
	wrLog("��getFavorableAirPay�� ������%d��", favorable_air_pay.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����epos_menu_get����Ϣ����ṹ��epos_menu_get��
int CDB::getEposMenuGet() 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposMenuGet��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_menu_get order by asktype desc, bank desc";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!epos_menu_get.empty())
	{
		epos_menu_get.clear();
	}

	Epos_menu_get epos_menu_get_tmp;

	while(!record->adoEOF)
	{
		memset(&epos_menu_get_tmp, 0, sizeof(epos_menu_get_tmp));

		epos_menu_get_tmp.tgroup = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("tGroup") );
		epos_menu_get_tmp.asktype = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("asktype") );
		strcpy( epos_menu_get_tmp.bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank") );
		epos_menu_get_tmp.menu_group = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("menu_group") );
		
		epos_menu_get.push_back(epos_menu_get_tmp);

		record->MoveNext();
	}

	wrLog("��getEposMenuGet�� ������%d��", epos_menu_get.size());
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����epos_menu_group����Ϣ����ṹ��epos_menu_group��
int CDB::getEposMenuGroup() 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposMenuGroup��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_menu_group order by menu_group";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if(!epos_menu_group.empty())
	{
		epos_menu_group.clear();
	}

	Epos_menu_group epos_menu_group_tmp;
	
	while(!record->adoEOF)
	{
		memset(&epos_menu_group_tmp, 0, sizeof(epos_menu_group_tmp));

		epos_menu_group_tmp.menu_group = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("menu_group") );
		strcpy( epos_menu_group_tmp.menu_ver, (LPCTSTR)(_bstr_t)record->GetCollect("menu_ver") );
		epos_menu_group_tmp.mbl_flag = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("mbl_flag") );
		
		epos_menu_group.push_back(epos_menu_group_tmp);

		record->MoveNext();
	}

	wrLog("��getEposMenuGroup�� ������%d��", epos_menu_group.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����Epos_menu����Ϣ����ṹ��Epos_menu��
int CDB::getEposMenu() 
{
	int menu_id, ret, index, i;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposMenu��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_menu order by menu_id desc";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!epos_menu.empty())
	{
		epos_menu.clear();
	}

	menu_id = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("menu_id") );

	Epos_menu epos_menu_tmp;
	for(i = 0; i < menu_id + 1; i++)
	{
		memset(&epos_menu_tmp, 0, sizeof(epos_menu_tmp));
		epos_menu.push_back(epos_menu_tmp);
	}

	while(!record->adoEOF)
	{
		menu_id = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("menu_id") );
		epos_menu[menu_id].menu_id = menu_id; 
		strcpy(epos_menu[menu_id].name, (LPCTSTR)(_bstr_t)record->GetCollect("name") );
		if((record->GetCollect("info")).vt!=VT_NULL)
		{
			strcpy(epos_menu[menu_id].info, (LPCTSTR)(_bstr_t)record->GetCollect("info") );
		}

		record->MoveNext();
	}

	wrLog("��getEposMenu�� ������%d��", epos_menu.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����Epos_menu_conf����Ϣ����ṹ��Epos_menu_conf��
int CDB::getMenuConf() 
{
	int i, menu_group, cnt, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getMenuConf��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;
	vSQL = "select * from epos_menu_conf order by priority"; //�����ȼ�����

	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if(!menu_cnt.empty())
	{
		menu_cnt.clear();
	}

	for(i  =0; i <= epos_menu.size(); i++) //��ʼ�����˵���Ĳ˵���Ŀ
		menu_cnt.push_back(0);

	Epos_menu_conf epos_menu_conf_tmp;
	memset(&epos_menu_conf_tmp, 0, sizeof(epos_menu_conf_tmp));
	vector<Epos_menu_conf> menu_conf_tmp;

	if(!menu_conf_tmp.empty())
	{
		menu_conf_tmp.clear();
	}

	for(i = 0; i <= MAX_MENU_ITEM; i++)
	{
		menu_conf_tmp.push_back(epos_menu_conf_tmp);
	}


	if(!menu_conf.empty())
	{
		menu_conf.clear();
	}

	for(i = 0; i <= epos_menu.size(); i++)
	{
		menu_conf.push_back(menu_conf_tmp);
	}

	while(!record->adoEOF)
	{
		menu_group = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("menu_group") );
		cnt = menu_cnt[menu_group];
		menu_conf[menu_group][cnt].menu_group = menu_group;
		menu_conf[menu_group][cnt].parent = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("parent") );
		menu_conf[menu_group][cnt].currentm = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("currentm") );
		menu_conf[menu_group][cnt].child = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("child") );
		menu_cnt[menu_group] += 1;
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

// int CDB::getPcardConf()
// {
// 	int ret, index;
// 	_bstr_t vSQL;
// 	_RecordsetPtr record;
// 	
// 	wrLog("��getPcardConf��");
// 	index = getConnStat();  //��ȡ��������
// 	wrLog("�������Ӻţ�[%d]", index);
// 	if( index<0 )
// 		return -1;
// 	vSQL = "select row_id,epos_pcard.card_type,card_name,card_price from epos_pcard,epos_pcard_conf where epos_pcard_conf.card_type=epos_pcard.card_type order by row_id"; //�����ȼ�����
// 	
// 	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
// 	if(ret)
// 	{
// 		g_adoConnInfo[index].connStat = 2;
// 		return -2;
// 	}
// 	if(record->adoEOF)
// 	{
// 		g_adoConnInfo[index].connStat = 0;
// 		return -3;
// 	}
// 
// 	if(!pcard_conf.empty())
// 	{
// 		pcard_conf.clear();
// 	}
// 
// 	Epos_pcard_conf pcard_conf_tmp;
// 	
// 	while(!record->adoEOF)
// 	{
// 		memset(&pcard_conf_tmp, 0, sizeof(pcard_conf_tmp));
// 
// 		strcpy(pcard_conf_tmp.card_type, (LPCTSTR)(_bstr_t)record->GetCollect("card_type") );
// 		strcpy(pcard_conf_tmp.card_name, (LPCTSTR)(_bstr_t)record->GetCollect("card_name") );
// 		strcpy(pcard_conf_tmp.card_price, (LPCTSTR)(_bstr_t)record->GetCollect("card_price") );
// 
// 		pcard_conf.push_back(pcard_conf_tmp);
// 
// 		record->MoveNext();
// 	}
// 
// 	wrLog("��getPcardConf�� ������%d��", pcard_conf.size());
// 
// 	g_adoConnInfo[index].connStat = 0;
// 	
// 	return 0;
// }



// int CDB::get_pcardnum()
// {
// 	int pcard_num = 0;
// 	for (int i=0;i<25;i++)
// 	{
// 		if (memcmp(pcard_conf[i].card_type,"",12))
// 		{
// 			pcard_num ++;
// 		}
// 	}
// 	wrLog("pcard_num:��%d��",pcard_num);
// 	return pcard_num;
// }

//�����ݿ���ȡ����Print_adv����Ϣ����ṹ��Print_adv��
int CDB::get_print_adv() 
{
	int i, tgroup, cnt, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��get_print_adv��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	vSQL = "select * from print_adv order by fee_group"; //��������������
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 0;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	memset(&print_adv, 0, sizeof(print_adv));

	for(i=0; i<MAX_PRINT_ADV; i++) //��ʼ��������������ҵ����Ŀ
		print_adv_cnt[i] = 0;
	
	while(!record->adoEOF)
	{
		tgroup = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("fee_group") );
		cnt = print_adv_cnt[tgroup];
		print_adv[tgroup][cnt].fee_group = tgroup;
		if((record->GetCollect("fee_type")).vt!=VT_NULL)
			strcpy(print_adv[tgroup][cnt].fee_type,(LPCTSTR)(_bstr_t)record->GetCollect("fee_type"));
		else
			strcpy(print_adv[tgroup][cnt].fee_type,"0000");
		strcpy(print_adv[tgroup][cnt].advertisement,(LPCTSTR)(_bstr_t)record->GetCollect("advertisement"));
		print_adv_cnt[tgroup] += 1;
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}
/**************************����ӿ�********************************/

//��ȡ���ݿ����Ϣ
int CDB::getTable()
{
	int iRet;
	
	wrLog("�������ݿ�[%s]", DB_CON_STR);
	iRet = getEposMenu();
	wrLog("getEposMenu����ֵ��[%d]", iRet);
	if(  iRet == 0  )
	{
		iRet = getEposGroup();
		wrLog("getEposGroup����ֵ��[%d]", iRet);
	}
	if(  iRet == 0  )
	{
		iRet = getEposMenuGet();
		wrLog("getEposMenuGet����ֵ��[%d]", iRet);
	}
	if(  iRet == 0  )
	{
		iRet = getEposMenuGroup();
		wrLog("getEposMenuGroup����ֵ��[%d]", iRet);
	}
	if(  iRet == 0  )
	{
		iRet = getBankInfo();
		wrLog("getBankInfo����ֵ��[%d]", iRet);
	}
	if(  iRet == 0  )
	{
		iRet = getBankCard();
		wrLog("getBankCard����ֵ��[%d]", iRet);
	}
	if(  iRet == 0  )
	{
		iRet = getBankBusi();
		wrLog("getBankBusi����ֵ��[%d]", iRet);
	}
	if( iRet == 0 )
	{
		iRet = getMenuConf();
		wrLog("getMenuConf����ֵ��[%d]", iRet);
	}
// 	if( iRet == 0 )
// 	{
// 		iRet = getPcardConf();
// 		wrLog("getPcardConf����ֵ��[%d]", iRet);
// 	}
	if ( iRet == 0)
	{
		iRet = get_print_adv();
		wrLog("get_print_adv����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getEposTips();
		wrLog("getEposTips����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getSellCardAmo();
		wrLog("getSellCardAmo����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getEposMobile();
		wrLog("getEposMobile����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getEposTurn();
		wrLog("getEposTurn����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getEposNewMessage();
		wrLog("getEposNewMessage����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getAreaBiz();
		wrLog("getAreaBiz����ֵ��[%d]", iRet);
	}
	if ( iRet == 0)
	{
		iRet = getFavorableAirPay();
		wrLog("getFavorableAirPay����ֵ��[%d]", iRet);
	}

	if(iRet == 0)
	{
		iRet = getFailMessage();
		wrLog("getFailMessage����ֵ��[%d]", iRet);
	}
	if(iRet == 0)
	{
		iRet = getPosAttr();
		wrLog("getPosAttr����ֵ��[%d]", iRet);
	}



	return iRet;
}




//�����ݿ��ж�ȡ�ն���Ϣ
//������Ϣ 	
//tgroup���ն˶�Ӧ���ն˰汾
//phs_ticket�������׿��Ƿ��ӡ��־
//get_card��һ��ˢ����־
int CDB::getEposTerm(char *pos_no, char *call_tel, int *tgroup, int *group_type, int *phs_ticket, int *get_card, char *mer_ecard, int *logonFlag, char *welcome,char *workid, PTSDLLPosInfo pDllPosInfo, char *usbid, int *dayAmo, char *city_no, char *acc_city_no, int *title_flag, char *pki_card, char *signin_serial, int *DianXin_Turn, int *Reversal_count, int *term_level,char*update_message, int* input_times, int* print_paper, int* commission_free,int * hid_type,int *busFlag)
{
//	wrLog("����ȡ�ն���Ϣ��");
	int status, i, ret, index,dayamo_tmp,sys_dayamo_tmp;
	_bstr_t vSQL;
	_RecordsetPtr record;
	_variant_t   var;
	char sql[1024];
	CString pos;
	char dayamt[16] = {0};
	char sysamt[16] = {0};
	int shlevel;

	if( isLock() )
	{
		return 9;
	}

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return 1;
	pos.Format(pos_no);
	//yyx-20130129�޸�
	//vSQL = "select tgroup, phone, card_no, dayAmo,sys_dayamo, curworkid,reversal, epos_term.status, epos_term.term_level, epos_term.input_times, epos_term.print_paper, NVL(usbid, 'FFFFFFFFFFFFFFFF') usbid, area.city_no city_no, acc_area.city_no acc_city_no, psamNum, tmk, tpk, tak, tek ,title_flag, pki_card, signin_serial, commission_free from epos_term,soft_term,area area, area acc_area, YCTYP_signin where epos_term.term_id = '" + pos + "' and epos_term.area=area.area and epos_term.accarea=acc_area.area and epos_term.term_id=soft_term.term_id(+) and epos_term.term_id=YCTYP_signin.term_id(+) and signin_time(+)>trunc(sysdate)";
	vSQL = "select tgroup, phone, card_no, curworkid,reversal, epos_term.status, epos_term.input_times, epos_term.print_paper, NVL(usbid, 'FFFFFFFFFFFFFFFF') usbid, area.city_no city_no, acc_area.city_no acc_city_no, psamNum, tmk, tpk, tak, tek ,title_flag, pki_card, signin_serial, commission_free,HID_TYPE from epos_term,soft_term,area area, area acc_area, YCTYP_signin where epos_term.term_id = '" + pos + "' and epos_term.area=area.area and epos_term.accarea=acc_area.area and epos_term.term_id=soft_term.term_id(+) and epos_term.term_id=YCTYP_signin.term_id(+) and signin_time(+)>trunc(sysdate)";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

    wrLog("vSQL:[%s]" , (char*)vSQL);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return 2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 3;
	}
	
	//��Ϊ"8888"���ж��Ƿ��
	if( atoi(pDllPosInfo->prmVer)>=9000 )
	{
		if( strcmp("8888", (LPCSTR)(_bstr_t)(record->GetCollect("phone")))==0 || strcmp("0000000000000000", excMAC)==0)//���������绰����
		{
			NULL;//OK
		}
		else
		{
			if( strcmp(call_tel, (LPCSTR)(_bstr_t)(record->GetCollect("phone")))==0 )//���������󶨵ĺ���һ��
			{
				NULL;//OK
			}
			else
			{
				g_adoConnInfo[index].connStat = 0;
				return 4;
			}
		}
		if( strcmp(usbid, (LPCSTR)(_bstr_t)(record->GetCollect("usbid")))==0 )
		{
			NULL;//OK
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return 5;
		}
	}
	else
	{
		if( strcmp("8888", (LPCSTR)(_bstr_t)(record->GetCollect("phone")))==0 )//���������绰����
		{
			NULL;
		}
		else
		{
			if( strcmp(call_tel, (LPCSTR)(_bstr_t)(record->GetCollect("phone")))==0 )//���������󶨵ĺ���һ��
			{
				NULL;
			}
			else
			{
				g_adoConnInfo[index].connStat = 0;
				return 6;
			}
		}
	}

	if((record->GetCollect("city_no")).vt!=VT_NULL)
		strcpy(city_no, (LPCSTR)(_bstr_t)(record->GetCollect("city_no")) );
	else
		strcpy(city_no,"");

	if((record->GetCollect("acc_city_no")).vt!=VT_NULL)
		strcpy(acc_city_no, (LPCSTR)(_bstr_t)(record->GetCollect("acc_city_no")) );
	else
		strcpy(acc_city_no,"");

	if((record->GetCollect("card_no")).vt!=VT_NULL)
		strcpy(mer_ecard, (LPCSTR)(_bstr_t)(record->GetCollect("card_no")) );
	else
		strcpy(mer_ecard,"");
	wrLog("�󶨵�e��[%d]: [%s]", strlen(mer_ecard), mer_ecard);
	if( strlen(mer_ecard) > 18 /*16 modified 20120801 for test*/ )
	{
		g_adoConnInfo[index].connStat = 0;
		return 7;
	}

	//��ƱPKI������
	if((record->GetCollect("pki_card")).vt!=VT_NULL)
		strcpy(pki_card, (LPCSTR)(_bstr_t)(record->GetCollect("pki_card")) );
	else
		strcpy(pki_card,"00000000");

	//��Ʊ������ˮ��
	if((record->GetCollect("signin_serial")).vt!=VT_NULL)
		strcpy(signin_serial, (LPCSTR)(_bstr_t)(record->GetCollect("signin_serial")) );
	else
		strcpy(signin_serial,"0000000000000000");
	//�̻������ս����޶�
	//dayamo_tmp=atoi((LPCSTR)(_bstr_t)(record->GetCollect("dayAmo")));
	//��������ȡ�����ս����޶ϵͳ�ս����޶�,20130105
	//yyx-20130129�޸�

	if( atoi(pDllPosInfo->prmVer)>=9000) 
	{
			
		if (CDB::get_dayAmt(pos_no, dayamt, sizeof(dayamt), sysamt, sizeof(sysamt), &shlevel) < 0) {
			g_adoConnInfo[index].connStat = 0;
			return 8;
		}
		dayamo_tmp = atoi(dayamt);
		wrLog("�����ķ����ս����޶�:%d��", dayamo_tmp);
		//ϵͳ�ս����޶�
		//sys_dayamo_tmp=atoi((LPCSTR)(_bstr_t)(record->GetCollect("sys_dayamo")));
		sys_dayamo_tmp = atoi(sysamt);
		wrLog("�����ķ���ϵͳ�ս����޶�:%d��", sys_dayamo_tmp);
		if(dayamo_tmp==0){
			*dayAmo=sys_dayamo_tmp;
		}else if(sys_dayamo_tmp==0){
			*dayAmo=dayamo_tmp;
		}else{
			*dayAmo = dayamo_tmp>=sys_dayamo_tmp?sys_dayamo_tmp:dayamo_tmp;
		}
		wrLog("�����ս����޶[%d]��ϵͳ�ս����޶[%d]���ս����޶�: [%d]", dayamo_tmp,sys_dayamo_tmp,*dayAmo);
		

	}
	

	strcpy(workid, (LPCSTR)(_bstr_t)(record->GetCollect("curworkid")));

	status = atoi((LPCSTR)(_bstr_t)(record->GetCollect("status")));
	if(status == 0 || status == 7) //δ��ͨ
	{
		g_adoConnInfo[index].connStat = 0;
		return 8;
	}

	else {
		if(status == 2)
			*logonFlag =9;
		else if(status == 3)
			*logonFlag =4;
		else if(status == 4)
			*logonFlag =1;
		else if(status == 5)
			*logonFlag =2;
		else if(status == 6)
			*logonFlag =3;
		else if(status==9)
			*busFlag =1; //�����۷�״̬�����ܽ��й�������
	}
	*tgroup = atoi((LPCSTR)(_bstr_t)(record->GetCollect("tGroup")));
	*title_flag = atoi((LPCSTR)(_bstr_t)(record->GetCollect("title_flag")));
	*Reversal_count = atoi((LPCSTR)(_bstr_t)(record->GetCollect("reversal")));
	//yyx-20130129�޸�
	//*term_level = atoi((LPCSTR)(_bstr_t)(record->GetCollect("term_level")));
	*term_level = shlevel;
	*input_times = atoi((LPCSTR)(_bstr_t)(record->GetCollect("input_times")));
	*print_paper = atoi((LPCSTR)(_bstr_t)(record->GetCollect("print_paper")));
	*commission_free = atoi((LPCSTR)(_bstr_t)(record->GetCollect("commission_free")));
	*hid_type= atoi((LPCSTR)(_bstr_t)(record->GetCollect("HID_TYPE")));

	for(i = 0; i < epos_group.size(); i++)	//�ҵ��˵����
	{
		if( *tgroup == epos_group[i].tgroup)
			break;
	}
	if(i == epos_group.size()) //��epos_group�������Ҳ�����Ӧ��group
	{
		g_adoConnInfo[index].connStat = 0;
		return 10;
	}
	*group_type = epos_group[i].group_type;
	*phs_ticket = epos_group[i].phs_ticket;
	*get_card = epos_group[i].get_card;
	*DianXin_Turn = epos_group[i].DianXin_Turn;
	sprintf(welcome,"%s",epos_group[i].welcome);
	sprintf(update_message,"%s",epos_group[i].update_info);

	if( atoi(pDllPosInfo->prmVer)>=9000 )
	{
		if((record->GetCollect("psamNum")).vt!=VT_NULL)
			CTools::hex_to_byte((const unsigned char *)(LPCSTR)(_bstr_t)(record->GetCollect("psamNum")), (unsigned char *)pDllPosInfo->psamNum, 16);
		if((record->GetCollect("tmk")).vt!=VT_NULL)
			CTools::hex_to_byte((const unsigned char *)(LPCSTR)(_bstr_t)(record->GetCollect("tmk")), (unsigned char *)pDllPosInfo->tskey[0], 32);
		if((record->GetCollect("tpk")).vt!=VT_NULL)
			CTools::hex_to_byte((const unsigned char *)(LPCSTR)(_bstr_t)(record->GetCollect("tpk")), (unsigned char *)pDllPosInfo->tskey[1], 32);
		if((record->GetCollect("tak")).vt!=VT_NULL)
			CTools::hex_to_byte((const unsigned char *)(LPCSTR)(_bstr_t)(record->GetCollect("tak")), (unsigned char *)pDllPosInfo->tskey[2], 32);
		if((record->GetCollect("tek")).vt!=VT_NULL)
			CTools::hex_to_byte((const unsigned char *)(LPCSTR)(_bstr_t)(record->GetCollect("tek")), (unsigned char *)pDllPosInfo->tskey[3], 32);
	}
	
	//�����8888���Ҳ����ڲ��������Ͱ�
	if( atoi(pDllPosInfo->prmVer)>=9000 && strcmp("8888", (LPCSTR)(_bstr_t)(record->GetCollect("phone")))==0 && strstr(excMAC, call_tel)==NULL && strstr(excMAC, "0000000000000000")==NULL )
	{
		bbzero(sql);
		sprintf(sql, "update epos_term set phone='%s' where term_id='%s'", call_tel, pos_no);
		vSQL = sql;
		ret = g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL);
		if(ret!=0)
		{
			wrLog("�����������ݿ�ʧ��");
			wrLog("SQL��䣺[%s]", sql);
			g_adoConnInfo[index].connStat = 2;
			return 11;
		}
	}

	g_adoConnInfo[index].connStat = 0;

	return 0;
}


//����ʱ����͸������ݿ�
//status: 1, ��ѯ����
//		  2, ���Ѳ���
//		  3, ǩ������
//		  11,��ѯ���ظ���
//		  22,���ѷ��ظ���
//		  33,ǩ�����ظ���
//pack������ȡ�÷��ذ���
//gbl �������������ݿ�����Ҫ�ı�����
//seq ������ʱ�������кţ�����ʱ����
int CDB::setEposTxn(int status, CGlobal gbl, CPack pack, char *seq)
{
	int ret, index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512], pay_no[30];
	char mti_code[4+1], rsp_code[2+1], amount[12+1], stan_no[12+1];

	if( status == 1 || status == 2 || status == 3 ) //�������ݿ�ʱ����������к�
	{
		if(status != 3)
		{
			bbzero( pay_no );
			if( !memcmp(gbl.fee_type, "0123", 4) )
				strcpy(pay_no, gbl.order_no);
			else
				strcpy(pay_no, gbl.phone_no);
			bbzero(amount);
			strcpy(amount, gbl.amount);
		}
		
		index = getConnStat();  //��ȡ��������
		if( index<0 )
			return -1;
		wrLog("���������ݿ�ͨ�� %d��", index);
		vSQL = "select lpad(epos_txn_seq.nextval, 10,0) aaa from dual";
		record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
		if(ret)
		{
			g_adoConnInfo[index].connStat = 2;
			return -2;
		}
		if(record->adoEOF)
		{
			g_adoConnInfo[index].connStat = 0;
			return -3;
		}
		strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
		wrLog("���кţ�[%s]", seq);

		//cut by liuxiao
		/*
		bbzero(date);
		CTools::byte_to_hex(pack.FIELD13, (BYTE *)date, 4);
		bbzero(time);
        CTools::byte_to_hex(pack.FIELD12, (BYTE *)time, 3);
		*/
		//end of the cut by liuxiao 
	}
	else
	{
		wrLog("���ظ������ݿ⡭��");
		if( status == 11)
		{
			bbzero(amount);
			CTools::byte_to_hex(pack.FIELD4, (BYTE *)amount, 6);
		}
		bbzero(rsp_code);
		memcpy(rsp_code, (char *)pack.FIELD39, 2);
		bbzero(stan_no);
		memcpy(stan_no, (char *)pack.FIELD37, 12);
	}

	bbzero(mti_code);
	bbzero(tmp);
	switch( status )
	{
	case 1: //��ѯ
		strcpy(mti_code, "0100");
		sprintf(tmp, "INSERT INTO epos_txn(epos_seq, mti, term_id, term_serial, txn_time, biz_type, pay_no, phone) VALUES('%s', '%s','%s','%s',SYSDATE, '%s','%s','%s')", 
	        seq, mti_code, gbl.pos_no, gbl.pos_serial,gbl.fee_type, pay_no, gbl.call_tel);
		vSQL = tmp;
		break;

	case 2: //����
		strcpy(mti_code, "0200");
		//sprintf(tmp, "INSERT INTO epos_txn(epos_seq, mti, term_id, term_serial, txn_time, amount, biz_type, bank, pay_no, card_no, phone) VALUES('%s','%s','%s','%s',to_date('%s%s','yyyymmddhh24miss'), %s,'%s','%s','%s','%s','%s')", 
	    //    seq, mti_code, gbl.pos_no, gbl.pos_serial, date,time,amount,gbl.fee_type, gbl.bank, pay_no, gbl.card_no, gbl.call_tel);
		sprintf(tmp, "INSERT INTO epos_txn(epos_seq, mti, term_id, term_serial, txn_time, amount, biz_type, bank, pay_no, card_no, phone) VALUES('%s','%s','%s','%s',SYSDATE, %s,'%s','%s','%s','%s','%s')", 
		seq, mti_code, gbl.pos_no, gbl.pos_serial,amount,gbl.fee_type, gbl.bank, pay_no, gbl.card_no, gbl.call_tel);
		vSQL = tmp;
		break;

	case 3: //ǩ��
		strcpy(mti_code, "0800");
		sprintf(tmp, "INSERT INTO epos_txn(epos_seq, mti, term_id, txn_time, phone) VALUES('%s', '%s','%s',SYSDATE, '%s')", 
	            seq, mti_code, gbl.pos_no, gbl.call_tel);
		vSQL = tmp;
		break;

	case 11: //��ѯ����
		strcpy(mti_code, "0100");
		sprintf(tmp, "UPDATE epos_txn SET stan='%s', rsp_code ='%s', amount =%s WHERE epos_seq='%s'",
			    stan_no, rsp_code, amount, seq);
		vSQL = tmp;
		break;

	case 22: //���ѷ���
		strcpy(mti_code, "0200");
		sprintf(tmp, "UPDATE epos_txn SET stan = '%s', rsp_code ='%s' WHERE epos_seq='%s'",	
			    stan_no, rsp_code, seq);
		vSQL = tmp;
		break;

	case 33: //ǩ������
		strcpy(mti_code, "0800");
		sprintf(tmp, "UPDATE epos_txn SET rsp_code ='%s' WHERE epos_seq='%s'",
			    rsp_code, seq);
		vSQL = tmp;
		break;
	}

	if(index == -1)  //û��ȡ�����ݿ�����
	{
		index = getConnStat();  //��ȡ��������
		if( index<0 )
			return -1;
		wrLog("���������ݿ�ͨ�� %d��", index);
	}
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		wrLog("SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//���뵽epos_txn ���׼�¼����
int CDB::insertEposTxn( CGlobal gbl,char *tradeStatus)
{
	int ret, index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	char mti_code[4+1];

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("���������ݿ�ͨ�� %d��", index);
	vSQL = "select lpad(epos_txn_seq.nextval, 10,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	char seq[10+1];
	bbzero(seq);
	strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	wrLog("���кţ�[%s]", seq);
	bbzero(mti_code);
	bbzero(tmp);
	strcpy(mti_code, "0200");
	switch (atoi(gbl.fee_type))
	{
		case 365:

			sprintf(tmp, "INSERT INTO epos_txn(epos_seq,stan, mti, term_id, term_serial, txn_time,rsp_code, amount, biz_type, bank, pay_no, card_no, phone) VALUES('%s','%s','%s','%s','%s',SYSDATE,'%s',%s,'%s','%s','%s','%s','%s')", 
				seq,gbl.stan_no, mti_code, gbl.pos_no, gbl.pos_serial,tradeStatus,gbl.amount,gbl.fee_type, gbl.bank, gbl.SDM_BILLNO, gbl.card_no, gbl.call_tel);
			break;
		case 366: //3g������
		case 367: //ȫ������ֱ��
		case 369://ȫ���ƶ���ֵ
		case 370: //ȫ����ͨ��ֵ
		case 180: //�㶫�ƶ���ֵ
		case 76: //qq 76-341
		case 77:
		case 78:
		case 88:
		case 167:
		case 168:
		case 273:
		case 274:
		case 275:
		case 277:
		case 278:
		case 293:
		case 294:
		case 322:
		case 341: //qq
			sprintf(tmp, "INSERT INTO epos_txn(epos_seq,stan, mti, term_id, term_serial, txn_time,rsp_code, amount, biz_type, bank, pay_no, card_no, phone) VALUES('%s','%s','%s','%s','%s',SYSDATE,'%s',%s,'%s','%s','%s','%s','%s')", 
				seq,gbl.stan_no, mti_code, gbl.pos_no, gbl.pos_serial,tradeStatus,gbl.amount,gbl.fee_type, gbl.bank, gbl.phone_no, gbl.card_no, gbl.call_tel);		
			break;

		default:
			break;

	}

	vSQL = tmp;
	if(index == -1)  //û��ȡ�����ݿ�����
	{
		index = getConnStat();  //��ȡ��������
		if( index<0 )
			return -1;
		wrLog("���������ݿ�ͨ�� %d��", index);
	}
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		wrLog("SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;

}


//�����ۻ��ս��׶�
int CDB::setSumDayAmo(int amount, char *term_id)
{
	int index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	bbzero(tmp);
	sprintf(tmp, "UPDATE epos_term SET sum_dayamo=sum_dayamo+%d WHERE Term_id='%s'",
		amount, term_id);
	wrLog("setSumDayAmo:%s",tmp);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��setSumDayAmo �������ݿ�ͨ�� %d��", index);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("setSumDayAmo �����������ݿ�ʧ��");
		wrLog("setSumDayAmo SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//����ʱ����bit46�������ݿ�ǩ��״̬
//status: 2, TMK
//		  3, WK
//		  4, TPK
//		  5, TAK
//		  6, TEK
int CDB::setEposLogonStatus(int status, char *term_id)
{
	int index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];

	bbzero(tmp);
	sprintf(tmp, "UPDATE epos_term SET status='%d' WHERE Term_id='%s'",
			    status, term_id);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("���������ݿ�ͨ�� %d��", index);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		wrLog("SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//����ֵ��0����������1��������ʱ
int CDB::isLock()  //�ж��Ƿ���������ȫ�ֱ���
{	
	int j = 0;
	while( syn_flag!=0 && j<12) //���ݿ�ˢ��״̬���ȴ�6�룬��ʱ�򷵻�
	{
		Sleep(500); //�ȴ�0.5��
		j++;
	}
	if( syn_flag==0 )
		return 0;
	else
		return 1;
}

//ȡ�����ݿ�����״̬
//-1: �޿�������
//����: �������ӵ�λ��
int CDB::getConnStat()
{
	int i=0, j=0;
	while(j<5)  //���޿������ӣ��ȴ�300������ٲ���
	{
		cs_g_connStat.Lock();
		for(i=0; i<MAX_CONNECT_NUM; i++)
		{
			if( g_adoConnInfo[i].connStat==0 )
			{
				g_adoConnInfo[i].connStat = 1;  //������Ϊ��ռ��
				break;
			}
			else if( g_adoConnInfo[i].connStat==2 )  //���Ӵ�����������
			{
				g_adoConnInfo[i].connStat = 1;
				g_adoConnInfo[i].adoConn.ExitConnect();
				if( !g_adoConnInfo[i].adoConn.OnInitADOConn() )
				{
					g_adoConnInfo[i].connStat = 1;
					break;
				}
				else
				{
					g_adoConnInfo[i].connStat = 2;
				}				
			}
		}
		cs_g_connStat.Unlock();
		if(i!=MAX_CONNECT_NUM)
		{
			
			bbzero(g_adoConnInfo[i].termId);
			bbzero(g_adoConnInfo[i].connDate);
			bbzero(g_adoConnInfo[i].connTime);			
			strncpy(g_adoConnInfo[i].termId, CWrLog::g_pos_no, 8);
			CTools::get_date8(g_adoConnInfo[i].connDate);
			CTools::get_time(g_adoConnInfo[i].connTime);
			g_adoConnInfo[i].threadID=::GetCurrentThreadId();
			return i;
		}
		else
		{
			j++;
			wrLog("�����������ȴ���һ������ ������[%d]��", j);
			Sleep(300);
		}		
	}
	for(i=0; i<MAX_CONNECT_NUM; i++)
	{
		wrLog("g_connInfo[%d]: connStat[%d], threadID[%ld], termId[%s], connDate[%s], connTime[%s]", i, g_adoConnInfo[i].connStat, g_adoConnInfo[i].threadID, g_adoConnInfo[i].termId, g_adoConnInfo[i].connDate, g_adoConnInfo[i].connTime);
	}
	return -1;
}

//�ȴ��������ݿ������ͷ�
int CDB::waitForidlesse()
{
	int i;
	while(1)
	{
		cs_g_connStat.Lock();
		for(i=0; i<MAX_CONNECT_NUM; i++)
		{
			if( g_adoConnInfo[i].connStat==1 )
			{				
				break;
			}
			else
			{
				NULL;
			}
		}
		cs_g_connStat.Unlock();
		if(i==MAX_CONNECT_NUM)
		{
			return 0;
		}
		else
		{
			Sleep(1000);
		}
	}
}


//�������ͨ�������ݿ�
//seq: �������к�
int CDB::setYctTxn(CGlobal gbl, CPack pack, char *process, char *pk_card, char *YCT_card, char *seq)  //lihk,2007-3-16
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	char amount[12+1], stan_no[12+1];

	//���ѳɹ����������ݿ�
	bbzero(stan_no);
	memcpy(stan_no, (char *)pack.FIELD37, 12);
	bbzero(amount);
	strcpy(amount, gbl.amount);
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select lpad(YCT_txn_seq.nextval, 10,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	wrLog("���кţ�[%s]", seq);

	bbzero(tmp);
	sprintf(tmp, "INSERT INTO YCT_txn(YCT_seq, pos_serial, stan, process, pk_card, YCT_card, amount, txn_time, bank, card_no, pos_no, phone) VALUES('%s', '%s','%s','%s','%s','%s','%s',sysdate, '%s','%s','%s','%s')", 
	       seq, gbl.pos_serial, stan_no, process, pk_card, YCT_card, gbl.amount, gbl.bank,  gbl.card_no, gbl.pos_no, gbl.call_tel);
	vSQL = tmp;

	wrLog("����YCT_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}


//�������ͨ�������ݿ�״̬λ
//stat: 0����ֵ�ɹ���ȷ��
//      1����ֵ�ɹ�δȷ��
//      2����ֵʧ���ѳ���
//      3����ֵʧ��δ����
//      4����������������
//      5����ֵʧ�ܳ���ʧ��
//      6����ֵ�ɹ�ȷ��ʧ��
int CDB::setYctStat(int stat, char *seq)  //lihk,2007-3-16
{
	int index;
	char tmp[512];
	_bstr_t vSQL;
	_RecordsetPtr record;

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "UPDATE YCT_txn SET status=%d WHERE YCT_seq='%s'",stat, seq);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//���ö�ݸͨ���һ��ǩ����¼��״̬Ϊ0��һ���ڽ���ҵ���ʱ����д���������ǩ��
int CDB::updateLastSigninStatus(char *term_id) 
{
	int index;
	char tmp[512];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update dgt_sign_log set sign_status='0' where log_seq =(select max(log_seq)  from dgt_sign_log   where term_id='%s')",term_id);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("���¡����ݸ������ǩ����¼Ϊ0��ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//�������ͨ�������ݿ�PK���߼����ţ�ϵͳ��ˮ�ź����ͨ�߼�����
//process: ��ֵ��ˮ��
int CDB::updateYct(char *PKLogCard, char *process, char *YCTLogCard, char *seq)  //kf,20080718
{
	int index;
	char tmp[512];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "UPDATE YCT_txn SET pk_card='%s', process='%s', yct_card='%s' WHERE YCT_seq='%s'",PKLogCard, process, YCTLogCard, seq);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�ж��ն��Ƿ��в���������
//���أ�0, �У�1, �ޣ�����, ����
int CDB::getYctStat(char *pos_no, char *yctStan, char *seq, char *stan)  //lihk,2007-3-23
{
	int ret, index;
	char tmp[512];
	_bstr_t vSQL;
	_RecordsetPtr record;

	
	bbzero(tmp);
	sprintf(tmp, "select * from YCT_txn where pos_no='%s' and status=4 ", pos_no);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 1;
	}

	strcpy(yctStan, (LPCTSTR)(_bstr_t)record->GetCollect("process") );
	strcpy(seq, (LPCTSTR)(_bstr_t)record->GetCollect("yct_seq") );
	strcpy(stan, (LPCTSTR)(_bstr_t)record->GetCollect("stan") );
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//�����ݿ��ж�ȡ���ͨ������Ϣ
int CDB::getYctRev(char *seq, char *pos_no, char *pos_serial, char *card, char *amo, char *YCT_Card)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;

//	wrLog("����ȡ���ͨ������Ϣ��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from YCT_txn where status = 3 order by stan";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	strcpy(seq, (LPCTSTR)(_bstr_t)record->GetCollect("yct_seq") );
	strcpy(pos_no, (LPCTSTR)(_bstr_t)record->GetCollect("pos_no") );
	strcpy(pos_serial, (LPCTSTR)(_bstr_t)record->GetCollect("pos_serial") );
	strcpy(YCT_Card, (LPCTSTR)(_bstr_t)record->GetCollect("YCT_card") );
	strcpy(card, (LPCTSTR)(_bstr_t)record->GetCollect("card_no") );
	strcpy(amo, (LPCTSTR)(_bstr_t)record->GetCollect("amount") );

	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//�����ݿ��ж�ȡ���ͨδȷ����Ϣ
int CDB::getYctAck(char *stan, char *pos_no, char *pos_serial, char *intAmo, char *hexAmo, char *YCT_Card, char *stat, char *seq)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[50];

	wrLog("����ȡ���ͨ������Ϣ��YCT_txn");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "select * from YCT_txn where status = 1 and stan='%s'", stan);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	strcpy(seq, (LPCTSTR)(_bstr_t)record->GetCollect("yct_seq") );
	strcpy(pos_no, (LPCTSTR)(_bstr_t)record->GetCollect("pos_no") );
	strcpy(pos_serial, (LPCTSTR)(_bstr_t)record->GetCollect("pos_serial") );
	strcpy(YCT_Card, (LPCTSTR)(_bstr_t)record->GetCollect("YCT_card") );
	strcpy(intAmo, (LPCTSTR)(_bstr_t)record->GetCollect("amount") );

	wrLog("����ȡ���ͨ������Ϣ��YCT_Ack");
	bbzero(tmp);
	sprintf(tmp, "select * from YCT_Ack where yct_seq = '%s'", seq);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		setYctStat(0, seq);
		return -3;
	}
	strcpy(stat, (LPCTSTR)(_bstr_t)record->GetCollect("status") );
	strcpy(hexAmo, (LPCTSTR)(_bstr_t)record->GetCollect("hex_amount") );

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�������ͨȷ�ϱ�־
int CDB::setYctAck(char *seq, char *stat, char *hexAmo)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[100];

	bbzero(tmp);
	sprintf(tmp, "insert into YCT_Ack(yct_seq, status, hex_amount) values('%s', '%s', '%s')", seq, stat, hexAmo);
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	wrLog("����YCT_Ack�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�������ͨȷ�ϱ�־
int CDB::updateYctAck(char *seq, char *stat)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[200];
	
	bbzero(tmp);
	sprintf(tmp, "update YCT_Ack set status='%s' where yct_seq='%s'", stat, seq);
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ��ж�ȡ�ն˰󶨵Ŀ���
int CDB::getEcard(char *pos_no, char *card,char *business_name)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	CString pos;
	_variant_t   var;   
    
	wrLog("��getEcard����ȡ��e��");
	index = getConnStat();  //��ȡ��������	
	if( index<0 )
		return -1;
	pos.Format(pos_no);
	vSQL = "select e.card_no,m.name from epos_term e,merchant m,merchant_term t where e.term_id=t.def_term_id(+) and t.mid=m.mid and t.def_term_id = '" + pos + "'";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	var = record->GetCollect("card_no");
	if(var.vt == VT_NULL )
	{
		g_adoConnInfo[index].connStat = 0;
		return -4;
	}
	strcpy(card, (LPCSTR)(_bstr_t)(record->GetCollect("card_no")) );
	strcpy(business_name, (LPCSTR)(_bstr_t)(record->GetCollect("name")) );
	g_adoConnInfo[index].connStat = 0;
	
	wrLog("�ɹ���ȡ��e��: [%s],�̻���:", card,business_name);
	return 0;
}

//�����ݿ��ж�ȡ�ն˰󶨵Ŀ���
int CDB::getEcard(char *pos_no, char *card)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	CString pos;
	_variant_t   var;   
    
	wrLog("��getEcard����ȡ��e��");
	index = getConnStat();  //��ȡ��������	
	if( index<0 )
		return -1;
	pos.Format(pos_no);
	vSQL = "select e.card_no from epos_term e  where  term_id = '" + pos + "'";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	var = record->GetCollect("card_no");
	if(var.vt == VT_NULL )
	{
		g_adoConnInfo[index].connStat = 0;
		return -4;
	}
	strcpy(card, (LPCSTR)(_bstr_t)(record->GetCollect("card_no")) );
	g_adoConnInfo[index].connStat = 0;
	
	wrLog("�ɹ���ȡ��e��: [%s]", card);
	return 0;
}

//��ȡ��E�����ն˺�
int CDB::getPosno(char *card,char *pos_no)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	CString ecard;
	_variant_t   var;   
    
	wrLog("��getEcard����ȡ��e��");
	index = getConnStat();  //��ȡ��������	
	if( index<0 )
		return -1;
	ecard.Format(card);
	vSQL = "select term_id from epos_term where card_no = '" + ecard + "'";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	var = record->GetCollect("term_id");
	if(var.vt == VT_NULL )
	{
		g_adoConnInfo[index].connStat = 0;
		return -4;
	}
	strcpy(pos_no, (LPCSTR)(_bstr_t)(record->GetCollect("term_id")) );
	g_adoConnInfo[index].connStat = 0;
	
	wrLog("�ɹ���ȡ��e�����ն˺ţ���%s��", pos_no);
	return 0;
}

//��ȡ���ݿ�ʱ�䣺��ʽ20130521000000

int CDB::getDBtime(char *dBTime)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	_variant_t   var;   
    
	wrLog("����ȡ���ݿ�ʱ�䡿");
	index = getConnStat();  //��ȡ��������	
	if( index<0 )
		return -1;

	vSQL = "select to_char(sysdate,'yyyymmddhh24miss') aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	strcpy(dBTime, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//д���ش�ӡ����
int CDB::setRePrint(char *stan,char *term_id,char *card_no,int iseny,char *pMsg,int tgroup)
{
	int index;

	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[2300];

	char newStan[15];
	bbzero(newStan);

	string pMsgStr="";
	string stanStr="";
	string newStanStr="";
	
	pMsgStr.assign(pMsg,strlen(pMsg));
	stanStr.assign(stan,strlen(stan));
	sprintf(newStan,"#%s",stan);
	newStanStr.assign(newStan,strlen(newStan));


	//��pMsg�в����Ƿ���ϵͳ�ο��ţ�����ϵͳ�ο����滻Ϊ#stan
	
	CTools::CSreplace(pMsgStr,stanStr,newStanStr,1);

	bbzero(tmp);
	sprintf(tmp, "insert into reprint(stan,txn_date,tgroup,term_id,card_no,iseny,isprint,pMsg) values('%s', sysdate, %d, '%s', '%s', %d, 0, '%s')", stan, tgroup, term_id, card_no, iseny, pMsgStr.c_str());
	//wrLog("tmp:%s",tmp);
	vSQL = tmp;

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��в����ش�ӡ����ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//��ȡ�ۿ���ӡ����
int CDB::getCardPrint(char *stan, char *field63, char *fee_type, char *sell_card_type, char *card_amount, char *card_no)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[500], detail[1024+1];
	
	wrLog("��getCardPrint��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "select biz_type,detail,NVL(substr(pay_no, 1, 4), '0000') sell_card_type, NVL(amount, 0) amount, card_no from txn_log, txn_detail where txn_log.stan='%s' and txn_log.stan=txn_detail.stan", stan);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if((record->GetCollect("biz_type")).vt!=VT_NULL)
	{
		strncpy(fee_type, (LPCTSTR)(_bstr_t)record->GetCollect("biz_type"), 4);
	}
	else
	{
		g_adoConnInfo[index].connStat = 0;
		return -4;
	}

	if(strstr("[0089][0153][0245][0266]", fee_type)!=NULL)
	{
		NULL;
	}
	else
	{
		g_adoConnInfo[index].connStat = 0;
		return -5;
	}

	bbzero(detail);
	if((record->GetCollect("detail")).vt!=VT_NULL)
	{
		strcpy(detail, (LPCTSTR)(_bstr_t)record->GetCollect("detail") );
		CTools::hex_to_byte((BYTE *)detail, (BYTE *)field63, strlen(detail), 0);
	}
	else
	{
		g_adoConnInfo[index].connStat = 0;
		return -6;
	}

	if((record->GetCollect("sell_card_type")).vt!=VT_NULL)
	{
		strcpy(sell_card_type, (LPCTSTR)(_bstr_t)record->GetCollect("sell_card_type") );
	}
	else
	{
		g_adoConnInfo[index].connStat = 0;
		return -7;
	}

	if((record->GetCollect("amount")).vt!=VT_NULL)
	{
		sprintf(card_amount, "%012d", atoi((LPCTSTR)(_bstr_t)record->GetCollect("amount")));
	}
	else
	{
		g_adoConnInfo[index].connStat = 0;
		return -8;
	}

	if(strcmp(sell_card_type, "0207")==0 || strcmp(sell_card_type, "0311")==0)
	{
		strcpy(card_amount, "000000003000");
	}
	else
	{
		NULL;
	}

	if((record->GetCollect("card_no")).vt!=VT_NULL)
	{
		strcpy(card_no, (LPCTSTR)(_bstr_t)record->GetCollect("card_no") );
	}
	else
	{
		strcpy(card_no, "0000");
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	wrLog("��getCardPrint succ��");
	return 0;
}

//��ȡ�ش�ӡ����
int CDB::getRePrint(char *stan,char *term_id,char *card_no,int &iseny,char pMsg[10][2048], char *prmVer, int *num, int mbl_flag)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[500];

	wrLog("��getRePrint��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	if(atoi(prmVer)>=9000)
	{
		if(memcmp(stan, "000000000000", 12)==0) //Ĭ�ϲ��������һ�ʽ��ס�
		{
			if (mbl_flag == 1) //�ƶ��˵��汾
			{
				sprintf(tmp, "select iseny,pMsg,biz_type,pay_no from ( select iseny,pMsg,biz_type,pay_no from reprint,epos_txn where reprint.term_id='%s'and epos_txn.stan=reprint.stan and ((epos_txn.biz_type  in('0174','0169','0213','0180','3002','0002','0176','0175','0343','0305','0334','0323','0327','0335','0316')) or ((epos_txn.biz_type in ('0089','0153'))and (epos_txn.pay_no like '02%')))order by reprint.txn_date desc ) where rownum=1",term_id);

			}else if (mbl_flag == 0) //���˵��汾
			{
				sprintf(tmp, "select iseny,pMsg from ( select iseny,pMsg from reprint where term_id='%s' order by txn_date desc ) where rownum=1 ",term_id);
			
			}else
				sprintf(tmp, "select iseny,pMsg,biz_type,pay_no from ( select iseny,pMsg,biz_type,pay_no from reprint,epos_txn where reprint.term_id='%s'and epos_txn.stan=reprint.stan and ((epos_txn.biz_type not  in('0174','0169','0213','0180','3002','0002','0176','0175','0343','0305','0334','0323','0327','0335','0316')) and (substr(epos_txn.biz_type||epos_txn.pay_no,1,6) not in ('008902','015302')))order by reprint.txn_date desc ) where rownum=1 ",term_id);
		
		}
		else
		{
			sprintf(tmp, "select iseny,pMsg from ( select iseny,pMsg from reprint,epos_group where txn_date>trunc(sysdate)-7 and term_id='%s' and (epos_group.group_type=1 or stan = '%s') and reprint.tgroup=epos_group.tgroup order by txn_date desc )",term_id,stan);
		
		}
	}
	else
	{
		//sprintf(tmp, "select iseny,pMsg from ( select iseny,pMsg from reprint,epos_group where isprint=0 and trunc(txn_date)=trunc(sysdate) and term_id='%s' and card_no='%s' and (epos_group.group_type=1 or stan = '%s') and reprint.tgroup=epos_group.tgroup order by txn_date desc ) where rownum=1",term_id,card_no,stan);
		
		//epos�����޸� for test
		if(memcmp(stan, "000000000000", 12)==0) //Ĭ�ϲ��������һ�ʽ���
		{
			sprintf(tmp, "select iseny,pMsg from ( select iseny,pMsg from reprint,epos_group where isprint=0 and trunc(txn_date)=trunc(sysdate) and term_id='%s' and card_no='%s' and (epos_group.group_type=1 or stan = '%s') and reprint.tgroup=epos_group.tgroup order by txn_date desc ) where rownum=1",term_id,card_no,stan);
		}
		else
		{

			sprintf(tmp, "select iseny,pMsg from reprint,epos_group where txn_date>trunc(sysdate)-7 and term_id='%s' and stan = '%s' and reprint.tgroup=epos_group.tgroup order by txn_date desc",term_id,stan);	
		}
		//============
	}
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	iseny = atoi( (LPCSTR)(_bstr_t)(record->GetCollect("iseny")) );

	*num = 0;
	while(!record->adoEOF)
	{
		wrLog("%d", *num);
		if((record->GetCollect("pMsg")).vt!=VT_NULL)
			strcpy(pMsg[*num], (LPCTSTR)(_bstr_t)record->GetCollect("pMsg") );
		else
			strcpy(pMsg[*num], "");

		record->MoveNext();
		(*num)++;
	}

	g_adoConnInfo[index].connStat = 0;

	wrLog("��getRePrint succ��");
	return 0;
}

//�����ش�ӡ����
int CDB::updateRePrint(char *stan,char *term_id,char *card_no,int &iseny){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[500];

	wrLog("�������ش�ӡ���ݡ�");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update reprint set isprint=1 where term_id='%s' and txn_date = (select max(txn_date) from reprint,epos_group where isprint=0 and txn_date>trunc(sysdate)-7 and term_id='%s' and card_no='%s' and (epos_group.group_type=1 or stan = '%s') and reprint.tgroup=epos_group.tgroup )",term_id,term_id,card_no,stan);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и����ش�ӡ����ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//��ȡecard����ϵͳ��������
//flag-0:��ȡδ������5����Ϣ;1:��ȡ���5����Ϣ
int CDB::getEcardTxn(char *pe_card_no, int flag, char *last_event_seq, char *event_seq, char *txn_type, char *txn_time, char *chg_amount){
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("����ȡecard����ϵͳ�������ݡ�");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	if(flag)
		sprintf(tmp, "select * from (select event_seq, txn_type, to_char(txn_time,'yyyy-mm-dd hh24:mi:ss') as mytime, chg_amount from mag_ecard_txn where pe_card_no='%s' and rsp_code='0000' and txn_type in('03','05') and event_seq<'%s' order by event_seq desc) where rownum=1", pe_card_no, last_event_seq);
	else
		sprintf(tmp, "select * from (select event_seq, txn_type, to_char(txn_time,'yyyy-mm-dd hh24:mi:ss') as mytime, chg_amount from mag_ecard_txn where pe_card_no='%s' and rsp_code='0000' and txn_type in('03','05') and show_flag=1    order by event_seq     ) where rownum=1",pe_card_no);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy(event_seq, (LPCTSTR)(_bstr_t)record->GetCollect("event_seq") );
	strcpy(txn_type, (LPCTSTR)(_bstr_t)record->GetCollect("txn_type") );
	strcpy(txn_time, (LPCTSTR)(_bstr_t)record->GetCollect("mytime") );
	strcpy(chg_amount, (LPCTSTR)(_bstr_t)record->GetCollect("chg_amount") );
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//����ecard����ϵͳ��������
int CDB::updateEcardTxn(char *event_seq){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("������ecard����ϵͳ�������ݡ�");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update mag_ecard_txn set show_flag=0 where event_seq = '%s'", event_seq);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и���ecard����ϵͳ��������ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�������ͨPIN
int CDB::getYctPIN(char *eposid,char *pin)
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	_variant_t   var;
	CString pos;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	pos.Format(eposid);
	vSQL = "select * from epos_setpin where epos_id = '" + pos + "'";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return 4;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 4;
	}
	
	if((record->GetCollect("pin")).vt!=VT_NULL)
		strcpy(pin, (LPCSTR)(_bstr_t)(record->GetCollect("pin")) );
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::getYctPIN2(const char *eposid, unsigned char *pin)
{
	int  ret, index, status;
	_bstr_t vSQL;
	_RecordsetPtr record;
	_variant_t   var;
	char temp[500];
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(temp);
	sprintf(temp, "select * from yct_isam where term_id = '%s'", eposid);
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return 4;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 4;
	}
	
	if((record->GetCollect("status")).vt!=VT_NULL)
		status = atoi((LPCSTR)(_bstr_t)(record->GetCollect("status")));

	if(status)
		return 1;

	if((record->GetCollect("isam_pwd")).vt!=VT_NULL)
		strcpy((char*)pin, (LPCSTR)(_bstr_t)(record->GetCollect("isam_pwd")) );
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::setYCTPinErr(const char *eposid)
{
	int index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	bbzero(tmp);
	sprintf(tmp, "UPDATE yct_isam SET status = 1, err_times = err_times+1 WHERE Term_id='%s'", eposid);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("���������ݿ�ͨ�� %d��", index);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		wrLog("SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//��˹����Ƿ�����ն�ƥ��
int CDB::checkworkID(CGlobal gbl,char *workid)
{
	int ret, index, j ;
	char work_ID[1024][8+1];
	char tmp[512];
	_bstr_t vSQL;
	_RecordsetPtr record;
	bbzero(tmp);

	for (int i=0;i<1024;i++)
	{
		bbzero(work_ID[i]);
	}
	wrLog("��checkworkID��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	sprintf(tmp,"select * from epos_term_workid where term_id='%s'",gbl.pos_no);
	vSQL = tmp;
	
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	memset(&work_ID, 0, sizeof(work_ID));
	
	j=0;
	
	while(!record->adoEOF)
	{
		strcpy(work_ID[j], (LPCTSTR)(_bstr_t)record->GetCollect("workid"));
		j++;		
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;

	for (i=0;i<=j;i++)
	{
		if (memcmp(work_ID[i],workid,8) == 0)
		{
			return 0;
		}
	}
	
	return -1;
}

int CDB::setworkID(CGlobal gbl,char *workid)
{
	int index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	bbzero(tmp);
	sprintf(tmp, "UPDATE epos_term SET curworkid='%s' WHERE Term_id='%s'",
		workid, gbl.pos_no);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("���������ݿ�ͨ�� %d��", index);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		wrLog("SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getMenu(int tgroup, int askType, char *bank, int *menu_group, int *main_group, char * main_ver, char * menu_ver)
{
	int  i;

	if( isLock() )
	{
		return -1;
	}
	for(i=0; i<epos_menu_get.size(); i++)	//�ҵ��˵����
	{
		if( epos_menu_get[i].tgroup == tgroup && ( epos_menu_get[i].asktype == askType ||  epos_menu_get[i].asktype == 0 ) && ( strcmp(epos_menu_get[i].bank, bank)==0 ||  strcmp(epos_menu_get[i].bank, "000000")==0 ) )
			break;
	}
	if(i == epos_menu_get.size()) //��epos_menu_get�������Ҳ�����Ӧ�Ĳ˵�
	{
		wrLog("�ҵ�����Ӧ�Ĳ˵���");
		return -4;
	}
	*menu_group = epos_menu_get[i].menu_group;


	for(i=0; i<epos_menu_get.size(); i++)	//�ҵ����˵����
	{
		if( epos_menu_get[i].tgroup == tgroup && epos_menu_get[i].asktype == 0 && strcmp(epos_menu_get[i].bank, "000000")==0 )
			break;
	}
	if(i == epos_menu_get.size()) //��epos_menu_get�������Ҳ�����Ӧ�Ĳ˵�
	{
		wrLog("�ҵ�����Ӧ�����˵���");
		return -4;
	}
	*main_group = epos_menu_get[i].menu_group;


	for(i=0; i<epos_menu_get.size(); i++)	//�ҵ����˵��İ汾��
	{
		if( epos_menu_group[i].menu_group == *main_group )
			break;
	}
	if(i == epos_menu_group.size()) //��epos_menu_get�������Ҳ�����Ӧ�Ĳ˵�
	{
		wrLog("�ҵ�����Ӧ�����˵���汾��");
		return -4;
	}
	strcpy(main_ver, epos_menu_group[i].menu_ver);

	for(i=0; i<epos_menu_get.size(); i++)	//�ҵ��˵��İ汾��
	{
		if( epos_menu_group[i].menu_group == *menu_group )
			break;
	}
	if(i == epos_menu_group.size()) //��epos_menu_get�������Ҳ�����Ӧ�Ĳ˵�
	{
		wrLog("�ҵ�����Ӧ�Ĳ˵���汾��");
		return -4;
	}
	strcpy(menu_ver, epos_menu_group[i].menu_ver);

	return 0;
}

//�����ݿ��Epos_menu_conf��ȡ��ĳһ���Ĳ˵�����˳������
//��0������һ��ռ䲻����Ϊ0
int CDB::getGroupMenus(int menu_group, Epos_menu_conf mymenus[], int *total) 
{
	int i, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024], intBuf[10+1];
	
	wrLog("��getGroupMenus��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;
	bbzero(tmp);
	//strcpy(tmp, "select case parent when 0 then (case currentm when 0 then child else currentm end) else parent end parent,case parent when 0 then (case currentm when 0 then 0 else child end) else currentm end currentm,case parent when 0 then 0 else child end child from (select * from epos_menu_conf where menu_group=");
	strcpy(tmp, "select case parent when 0 then (case currentm when 0 then child else currentm end) else parent end parent,case parent when 0 then (case currentm when 0 then 0 else child end) else currentm end currentm,case parent when 0 then 0 else child end child from (select * from epos_menu_conf where menu_group=");
	bbzero(intBuf);
	sprintf(intBuf, "%d", menu_group);
	strcat(tmp, intBuf);
	strcat(tmp, ") start with  parent=0 and currentm=0 connect by prior child=currentm order siblings by priority");
	vSQL = tmp;
	wrLog("[%s]",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	mymenus[0].menu_group = menu_group;
	mymenus[0].parent = -1;
	mymenus[0].currentm = -1;
	mymenus[0].child = -1;
	for(i=1; i<MAX_MENU_ITEM-1 && !record->adoEOF; i++)
	{
		mymenus[i].menu_group = menu_group;
		mymenus[i].parent = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("parent") );
		mymenus[i].currentm = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("currentm") );
		mymenus[i].child = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("child") );
		record->MoveNext();
	}
	mymenus[i].menu_group = menu_group;
	mymenus[i].parent = -1;
	mymenus[i].currentm = -1;
	mymenus[i].child = -1;

	g_adoConnInfo[index].connStat = 0;
	*total = i-1;
	
	return 0;
}

namespace
{
    string trim(const string& str)
    {
        char buf[256] = { 0 };
        strcpy(buf , str.c_str());
        CTools::trim(buf);
        return buf;
    }

    //ʱ�����ڸ�ʽ������,14λ��ʽ����ָ�� 2012-02-02 14:12:32
    string datetime_format(const string& str)
    {
        char outbuf[32] = { 0 };
        int year = 0,month = 0 , day = 0, hour = 0 , min = 0 , sec = 0;
        if (str.size() != 14)
        {
            return "0000-00-00 00:00:00";
        }

        sscanf(str.c_str() , "%04d%02d%02d%02d%02d%02d" , &year , &month , &day , &hour , &min , &sec);
        
        sprintf(outbuf , "%04d-%02d-%02d %02d:%02d:%02d" , year , month , day , hour , min , sec);
        return outbuf;
    }
}

//E���ʽ��˲�ѯ
int CDB::getEcardTxnLogs(char* ecard_no, char* term_no,PTSTxnLogs ptxnLogs, int *itemNum)
{
    char inBuf[512]     = { 0 };//PEP000A����γ���Ϊ66
    char buf_len_buf[9]= { 0 };
    char outBuf[81920]   = { 0 };
    char monthHead[32] = { 0 };
    char monthTail[32] = { 0 };
    char datetime[32]  = { 0 };
    int bufLen         = 0;
    int iRet           = 0;
    int timeout        = 10; //Ĭ�ϳ�ʱʱ��
    int i              = 0;
    int q_num          = 15;
    CConnect connector;
    SOCKET sd;

	wrLog("��E���ʽ��˲�ѯ -- ��FAS����");

    //1��׼��PEP000A�����
    CTools::getTheMonth(monthHead);
    strcat(monthHead, "01");
    CTools::getDate(monthTail);
    CTools::get_termTime(datetime);

    sprintf(inBuf, "FFFF%014s%08s%08dPEP000A0%-32s%08s%08s0001%04d", datetime, term_no, bufLen, ecard_no, monthHead, monthTail, q_num);
    bufLen = strlen(inBuf);
    sprintf(buf_len_buf, "%08d", bufLen);
    memcpy(inBuf + 4 + 22, buf_len_buf, 8);
    wrLog("CTSI���������FASACC:[%s]", inBuf);
    //2����������������
    iRet = connector.conToHost(PE_IP, PE_PORT, &sd);
    if (iRet != 0) {
        wrLog("���ӵ��������������飡%s:%d", __FILE__ , __LINE__);
        return -1;
    }
    
    iRet = connector.sndMsg(sd, inBuf, bufLen, timeout);
    if (iRet != 0)
    {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

    bufLen = 0;
    //�˴�APIû�г��Ȼ��ơ�����һ��ȫ�գ������ó�ʱ����
    iRet = recv(sd , outBuf , 4 + 22 + 8 , 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8) {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    int pack_len = atoi(outBuf + 4 + 22) - 4 - 22 - 8;
    
    int need_rcv = pack_len;
    int has_rcv  = 0;
    while (need_rcv > has_rcv) {
        iRet = recv(sd, outBuf + 4 + 22 + 8 + has_rcv, pack_len - has_rcv, 0);
        has_rcv += iRet;
    }
    bufLen = pack_len;
    closesocket(sd);

    //3�������εĽ�����룬ʧ�����˳�
    char errCode[6] = { 0 };
    char errmsg_lenbuf[4] = { 0 } ;
    char errMsg[512] = { 0 };
    int  errLen = 0;

    memcpy(errCode, outBuf + 4 + 22 + 8 + 8 , 4);
    memcpy(errmsg_lenbuf, outBuf + 4 + 22 + 8 + 8 + 4, 2);
    errLen = atoi(errmsg_lenbuf);
    if (strcmp(errCode, "0000") != 0) {
        memcpy(errMsg, outBuf +4+22+8+8 + 4 + 2, errLen);
        wrLog("�ӷ��������ش���:[%s]", errMsg);
        return -1;
    }

    //4����ȡ��¼������
    int count = 0;//��¼���������ݺ��ԡ���ѯ��ʼ��š��͡���ѯ��¼����
    char buf_count[6] = {0};
    memcpy(buf_count, outBuf + 42 + 4 + 2 + errLen, 4);
    count = atoi(buf_count);
    if (count == 0) {
        wrLog("δ�ҵ����������ļ�¼:[%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }

    //5��ʹ�ò�ѯ���صļ�¼��
    memcpy(buf_count, outBuf + 42 + 4 + 2 + errLen + 8, 4);
    count = atoi(buf_count);
    if (count == 0) {
        wrLog("δ�ҵ����������ļ�¼ : [%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }

    //6�����ñ�����
    ptxnLogs->colsnum = 9;
    ptxnLogs->colsWidth[0] = 90;
    ptxnLogs->colsWidth[1] = 50;
    ptxnLogs->colsWidth[2] = 56;
    ptxnLogs->colsWidth[3] = 70;
    ptxnLogs->colsWidth[4] = 70;
    ptxnLogs->colsWidth[5] = 70;
    ptxnLogs->colsWidth[6] = 50;
    ptxnLogs->colsWidth[7] = 80;
    ptxnLogs->colsWidth[8] = 70;
	ptxnLogs->colsNameLen = 80; //�����colsname�ĳ��ȣ�������strlen
    strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Esc������ ��F������excel��");
    memcpy(ptxnLogs->colsName, "����ʱ��\x00����\x00����ǰ���\x00���׽��\x00���׺����\x00���׽��\x00���׺���\x00ϵͳ�ο���\x00�ն˺�", ptxnLogs->colsNameLen);
    
    //7�����ñ������
    memset(ptxnLogs->items, 0, sizeof(ptxnLogs->items));
    ptxnLogs->itemsLen = 0;
    char* ptr = outBuf + 60 + errLen; //��¼��ʼ��ƫ��λ��
	wrLog("�ʽ��˼�¼��Ϣ:[%s]", ptr);
	wrLog("�ʽ��˼�¼��:[%d]", count);
    for (i = 0; i < count; i++) {
        string stan, cardno, flag, flag_msg, biz_type, biz_type_msg, amount;
        string beamount, biz_sour, biz_date, biz_time, time2, biz_code, biz_msg;
		string txnobj;
        char len_buf[4] = {0};
        char amount_buf[16] = {0};
        int len = 0;
        int ptr_offset = 0;
        
        //ϵͳ�ο���
        stan = string(ptr + ptr_offset, 12);
		wrLog("stan[%d]:[%s]", i, stan.c_str());
        ptr_offset += 12;
        stan = trim(stan);
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, stan.c_str());
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + stan.length() + 1;

        //������
        memset(len_buf, 0, sizeof(len_buf));
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        assert(len >=0 && len <= 99);//���ֽڣ����100
        cardno = string(ptr + ptr_offset, len);
		wrLog("cardno[%d]:[%s]", i, cardno.c_str());
        ptr_offset += len;
        cardno = trim(cardno);
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, cardno.c_str());
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + cardno.length() + 1;

        //��������
        memset(len_buf, 0, sizeof(len_buf));
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        assert(len >=0 && len <= 99);//���ֽڣ����100
        biz_type_msg = string(ptr + ptr_offset, len);
		wrLog("biz_type_msg[%d]:[%s]", i, biz_type_msg.c_str());
		biz_type_msg = trim(biz_type_msg);
        ptr_offset += len;
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, biz_type_msg.c_str());
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + biz_type_msg.length() + 1;

		//���׶���
		memset(len_buf, 0, sizeof(len_buf));
		memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        assert(len >=0 && len <= 99);//���ֽڣ����100
        txnobj = string(ptr + ptr_offset, len);
		wrLog("txnobj[%d]:[%s]", i, txnobj.c_str());
		txnobj = trim(txnobj);
        ptr_offset += len;
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, txnobj.c_str());
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + txnobj.length() + 1;

        //���׽��,�������Է�Ϊ��λ
        string amount_tmp = string(ptr + ptr_offset, 12);
		wrLog("amount_tmp[%d]:[%s]", i, amount_tmp.c_str());
        ptr_offset += 12;
		amount_tmp = trim(amount_tmp);
        sprintf(amount_buf, "%.2f", atof(amount_tmp.c_str())/100.0f);
        amount = amount_buf;
        double d_amount = atof(amount.c_str());
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, amount_buf);
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(amount_buf) + 1;

        //���׺����,�������Է�Ϊ��λ
        string beamount_tmp = string(ptr + ptr_offset, 12);
		wrLog("beamount_tmp[%d]:[%s]", i, beamount_tmp.c_str());
        ptr_offset += 12;
		beamount_tmp = trim(beamount_tmp);
        memset(amount_buf, 0 , sizeof(amount_buf));
        sprintf(amount_buf, "%.2f", atof(beamount_tmp.c_str())/100.0f);
        beamount = amount_buf;
        double d_beamount = atof(beamount.c_str());
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, amount_buf);
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(amount_buf) + 1;

        //����ǰ���
        char buf_amount[32] = {0};
        sprintf(buf_amount, "%.2f", d_beamount - d_amount);
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, buf_amount);
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(buf_amount) + 1;

        //���׼�¼ʱ��
        time2 = string(ptr + ptr_offset, 14);
		wrLog("time2[%d]:[%s]", i, time2.c_str());
		time2 = trim(time2);
        ptr_offset += 14;
        time2 = datetime_format(time2.c_str());
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, time2.c_str());
	    //ptxnLogs->itemsLen = ptxnLogs->itemsLen + time2.length() + 1;

        //���׽��
        memset(len_buf, 0, sizeof(len_buf));
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        assert(len >=0 && len <= 99);//���ֽڣ����100
        biz_msg = string(ptr+ ptr_offset, len);
		wrLog("biz_msg[%d]:[%s]", i, biz_msg.c_str());
		biz_msg = trim(biz_msg);
        ptr_offset += len;
		//strcpy(ptxnLogs->items+ptxnLogs->itemsLen, biz_msg.c_str());
	    //ptxnLogs->itemsLen = ptxnLogs->itemsLen + biz_msg.length() + 1;

#if 1
        sprintf(ptxnLogs->items + ptxnLogs->itemsLen, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c", \
            time2.c_str(), 0, biz_type_msg.c_str(), 0, buf_amount, 0, amount.c_str(), 0, beamount.c_str(), 0, \
            biz_msg.c_str(), 0, txnobj.c_str(), 0, stan.c_str(), 0, term_no, 0);

        ptxnLogs->itemsLen += time2.size() + \
            biz_type_msg.size() + \
            strlen(buf_amount) + \
            amount.size() + \
            beamount.size() +\
            stan.size() +\
            strlen(term_no) +\
            biz_msg.size() +\
            txnobj.length() + 9;
#endif
		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		//if (i == 3) {
		//break;
		//}
        ptr += ptr_offset;
    }

    wrHex("ITEMS", (BYTE*)ptxnLogs->items, ptxnLogs->itemsLen);
	wrLog("�ܳ��ȡ�%d��\n" , ptxnLogs->itemsLen);

    return 0;
}

//E���ʽ��ʲ�ѯ
int CDB::getEcardTrade(char* ecard_no,PTSTxnLogs ptxnLogs, int *itemNum)
{
	int ret=0, index=-1, i=0, s_commision=0, amo=0, sucnum=0, rev_status=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql[2048], temp1[20], temp2[20];
	CTime cTxnTime;

	wrLog("��E���ʽ��˲�ѯ -- ���ݿ��ѯ��");

	ptxnLogs->colsnum = 10;
	ptxnLogs->colsWidth[0] = 100;
	ptxnLogs->colsWidth[1] = 60;
	ptxnLogs->colsWidth[2] = 56;
	ptxnLogs->colsWidth[3] = 56;
	ptxnLogs->colsWidth[4] = 56;
	ptxnLogs->colsWidth[5] = 50;
	ptxnLogs->colsWidth[6] = 110;
	ptxnLogs->colsWidth[7] = 70;
	ptxnLogs->colsWidth[8] = 70;
	ptxnLogs->colsWidth[9] = 0;
	ptxnLogs->colsNameLen = 83;
	memcpy(ptxnLogs->colsName, "����ʱ��\x00����\x00����ǰ���\x00���׽��\x00���׺����\x00���׽��\x00���׺���\x00ϵͳ�ο���\x00�ն˺�\x00����", ptxnLogs->colsNameLen);
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Esc������ ��F������excel��");

	index = getConnStat();  //��ȡ��������

	if( index<0 )
		return -1;

	//��ȡ����
	bbzero(sql);
	sprintf(sql, "select count(*) num,sum(decode(a.rsp_code,NULL,0,'0000',1,0)) sucnum , \
		sum(decode(a.rsp_code,NULL,0,'0000',NVL(b.chg_amount,0),0)) sucamo \
		from pe_card_event a, pe_card_txn b \
		where a.txn_time > sysdate-31 and a.event_seq=b.event_seq(+) \
		and a.pe_card_no='%s' and ( (a.txn_type='05' \
		and a.biz_type in ('0108','0074','0075', '0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) \
		or (a.txn_type in ('02','03','04') and a.source='1000')  \
		or (a.txn_type='02' and a.biz_type='0359'))", ecard_no);
	vSQL = sql;
	recordcount = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordcount->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((recordcount->GetCollect("num")).vt!=VT_NULL)
		{
			*itemNum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("num"));
			if(  *itemNum>MAX_TXN_LOG || *itemNum<=0)
			{
				g_adoConnInfo[index].connStat = 0;
				return -4;
			}
			else
			{
				if((recordcount->GetCollect("sucnum")).vt!=VT_NULL)
				{
					sucnum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("sucnum"));
				}
				else
				{
					sucnum = 0;
				}
				if((recordcount->GetCollect("sucamo")).vt!=VT_NULL)
				{
					amo = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("sucamo"));
				}
				else
				{
					amo = 0;
				}
				sprintf(ptxnLogs->hint, "��%d�� �ɹ�%d��/%ld.%02ldԪ", *itemNum, sucnum, amo/100, amo%100);
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -4;
		}
	}
	bbzero(sql);
	sprintf(sql, "select a.txn_time ����ʱ��,  decode(a.txn_type||a.biz_type,'050075','ת��','050074','ת��','050083','��ֵ','050221','��ֵ','050108','��ֵ','050109','��ֵ','050268','��ֵ','050339','��ֵ','050340','��ֵ','050301','��ֵ','050302','��ֵ','050170','���','020359','����������',decode(txn_type,'03','����','04','����','����')) ����,b.pay_no ���׺���, b.org_amount/100 ����ǰ���, decode(a.txn_type||a.biz_type, '020359', -b.chg_amount/100, b.chg_amount/100) ���׽��, b.fin_amount/100  ���׺����, decode(a.rsp_code,'0000','�ɹ�','ʧ��') ���׽��,a.stan ϵͳ�ο���, a.txn_type type, a.source source, a.tid term_id from pe_card_event a, pe_card_txn b where 	a.txn_time > sysdate-31 and a.event_seq=b.event_seq(+) and a.pe_card_no='%s' and ( (a.txn_type='05' and a.biz_type in ('0108','0074','0075', '0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340'))  or (a.txn_type in ('02','03','04') and a.source='1000') or (a.txn_type='02' and a.biz_type='0359')) order by a.txn_time desc, a.stan desc",ecard_no);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	
	for(i=0; !record->adoEOF && i<*itemNum; i++)
	{
		
		if((record->GetCollect("����ʱ��")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("����ʱ��"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("����ʱ��")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("����")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("����"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("����")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		

		if((record->GetCollect("����ǰ���")).vt!=VT_NULL)
		{
			bbzero(temp1);
			bbzero(temp2);
			strcpy(temp1, (LPCSTR)(_bstr_t)(record->GetCollect("����ǰ���")));
			if(temp1[0] == '.')
			{
				strcpy(temp2, "0");
				strcat(temp2, temp1);
			}
			else
			{
				strcpy(temp2, temp1);
			}

			if( ptxnLogs->itemsLen+strlen(temp2) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp2);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("���׽��")).vt!=VT_NULL)
		{
			bbzero(temp1);
			bbzero(temp2);
			strcpy(temp1, (LPCSTR)(_bstr_t)(record->GetCollect("���׽��")));
			if(temp1[0] == '.')
			{
				strcpy(temp2, "0");
				strcat(temp2, temp1);
			}
			else
			{
				strcpy(temp2, temp1);
			}

			if( ptxnLogs->itemsLen+strlen(temp2) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp2);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				NULL;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("���׺����")).vt!=VT_NULL)
		{
			bbzero(temp1);
			bbzero(temp2);
			strcpy(temp1, (LPCSTR)(_bstr_t)(record->GetCollect("���׺����")));
			if(temp1[0] == '.')
			{
				strcpy(temp2, "0");
				strcat(temp2, temp1);
			}
			else
			{
				strcpy(temp2, temp1);
			}

			if( ptxnLogs->itemsLen+strlen(temp2) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp2);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("���׽��")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("���׽��"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("���׽��")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("���׺���")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("���׺���"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("���׺���")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}


		if((record->GetCollect("ϵͳ�ο���")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("ϵͳ�ο���"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("ϵͳ�ο���")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//��������ӱ�ע��Ϣ
// 		if(!strcmp((LPCSTR)(_bstr_t)(record->GetCollect("type")), "02") && !strcmp((LPCSTR)(_bstr_t)(record->GetCollect("source")), "1000") )
// 		{
// 			if( ptxnLogs->itemsLen+strlen("����������") < MAX_INTERM_BUF )
// 			{
// 				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "����������");
// 				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
// 		}
		//�ն˺�
		if((record->GetCollect("term_id")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("term_id"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("term_id")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
    
    wrHex("ITEMS:" , (BYTE*)ptxnLogs->items , ptxnLogs->itemsLen);
    wrLog("�ܳ��ȡ�%d��\n" , ptxnLogs->itemsLen);

	return 0;
}

//�����ݿ���ȡ��������ϸ��Ϣ����ṹ��TSTxnLogs��
//inout:0���� 1��ֵ mode:0�ϰ�ģʽ 1Ա��ģʽ
int CDB::getTxnLogs(char *term_id, char *date, int status, int inout, int mode, PTSTxnLogs ptxnLogs, int *itemNum, int reversal_count,int mbl_flag) 
{
	int ret=0, index=-1, i=0, s_commision=0, amo=0,rev_amo = 0, sucnum=0, rev_status=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char biz_type[4+1], revmsg[100], sql[2048];
	char start_date[8+1],end_date[8+1];

	wrLog("��getTxnLogs��");

	if(mode==0)
	{
		ptxnLogs->colsnum = 10;
		ptxnLogs->colsWidth[0] = 80;
		ptxnLogs->colsWidth[1] = 126;
		ptxnLogs->colsWidth[2] = 100;
		ptxnLogs->colsWidth[3] = 100;
		ptxnLogs->colsWidth[4] = 66;
		ptxnLogs->colsWidth[5] = 38;
		ptxnLogs->colsWidth[6] = 60;
		ptxnLogs->colsWidth[7] = 38;
		ptxnLogs->colsWidth[8] = 0;
		ptxnLogs->colsWidth[9] = 0;
		ptxnLogs->colsNameLen = 81;
		memcpy(ptxnLogs->colsName, "ϵͳ�ο���\x00����ʱ��\x00ҵ������\x00���׺���\x00���׽��\x00���\x00Ǯ�����\x00���\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);

	}
	else
	{
		ptxnLogs->colsnum = 9;
		ptxnLogs->colsWidth[0] = 80;
		ptxnLogs->colsWidth[1] = 126;
		ptxnLogs->colsWidth[2] = 100;
		ptxnLogs->colsWidth[3] = 95;
		ptxnLogs->colsWidth[4] = 66;
		ptxnLogs->colsWidth[5] = 45;
		ptxnLogs->colsWidth[6] = 150;
		ptxnLogs->colsWidth[7] = 0;
		ptxnLogs->colsWidth[8] = 0;
		ptxnLogs->colsNameLen = 72;
		memcpy(ptxnLogs->colsName, "ϵͳ�ο���\x00����ʱ��\x00ҵ������\x00���׺���\x00���׽��\x00���\x00˵��\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	}

	index = getConnStat();  //��ȡ��������
                                                                                                                                     
	if( index<0 )
		return -1;

	//��ȡ����
	bbzero(sql);
	//strcpy(sql, "select count(*) num, sum(decode(rsp_code,NULL,0,'00',1,0)) sucnum, sum(decode(rev_status, 2, 0, 4, 0, decode(rsp_code,NULL,0,'00',NVL(amount,0),0))) sucamo from txn_log where mti='0200'");
	//�µ�SQL���
	strcpy(sql, "select count(*) num, sum(decode(NVL(rsp_code, '96'), '00', decode(status, 0, decode(rev_status, 2, 0, 4, 0, 1), 0), 0)) sucnum, sum(decode(NVL(rsp_code, '96'), '00', decode(status, 0, decode(rev_status, 2, 0, 4, 0, NVL(amount,0)), 0), 0)) sucamo from txn_log where mti='0200'");
	if(strlen(date)==8)
	{
		strcat(sql, " and txn_date='");
		strcat(sql, date);
		strcat(sql, "'");
	}
	else if (strlen(date)==16)
	{
		bbzero(start_date);
		memcpy(start_date,date,8);
		bbzero(end_date);
		memcpy(end_date,date+8,8);
		strcat(sql, " and txn_date between'");
		strcat(sql, start_date);
		strcat(sql, "' and '");
		strcat(sql, end_date);
		strcat(sql, "'");
	}
	else if(strlen(date) == 6)
	{
		strcat(sql, " and (txn_date = to_char(sysdate, 'yyyymmdd') or (txn_date = to_char(sysdate-1, 'yyyymmdd') and txn_time > '");
		strcat(sql, date);
		strcat(sql, "'))");
	}
	else
	{
		strcat(sql, " and txn_date>=to_char(sysdate-30, 'yyyymmdd')");
	}
	strcat(sql, " and term_id='");
	strcat(sql, term_id);
	if( inout==0)//����
	{
		strcat(sql, "' and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340') and mti='0200'");

	}
	else//��ֵ
	{
		strcat(sql, "' and (biz_party = '200024' or mti='0400' )");
	}

 	if (mbl_flag == 1)
		strcat(sql,"and (txn_log.biz_type  in('0174','0169','0213','0180','3002','0002','0176','0175','0343','0305','0334','0323','0327','0335','0316')  or ((txn_log.biz_type in ('0089','0153'))and (txn_log.pay_no like '02%')) )");

	if(status==1)//�ɹ�
	{
		strcat(sql, " and status='0'");
	}
	else if(status==2)//ʧ��
	{
		strcat(sql, " and rsp_code<>'00'");
	}
	else//ȫ��
	{
		NULL;
	}
	
	vSQL = sql;
	wrLog("SQL:%s",sql);
	recordcount = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordcount->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((recordcount->GetCollect("num")).vt!=VT_NULL)
		{
			*itemNum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("num"));
			if(  *itemNum>MAX_TXN_LOG || *itemNum<=0)
			{
				g_adoConnInfo[index].connStat = 0;
				return -4;
			}
			else
			{
				if((recordcount->GetCollect("sucnum")).vt!=VT_NULL)
				{
					sucnum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("sucnum"));
				}
				else
				{
					sucnum = 0;
				}
				if((recordcount->GetCollect("sucamo")).vt!=VT_NULL)
				{
					amo = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("sucamo"));
				}
				else
				{
					amo = 0;
				}
				sprintf(ptxnLogs->hint, "��%d�� �ɹ�%d��/%ld.%02ldԪ", *itemNum, sucnum, amo/100, amo%100);
				if(strlen(date) != 6)	//24Сʱ��ѯ�޸�
				{
					strcpy(ptxnLogs->top, "���������ʹ�á�825��");
				}
				else
				if (reversal_count <= 1)
				{
					sprintf(ptxnLogs->top, "ʣ��������� %d ��,�������߿ͷ���Ctrl+H������", reversal_count);
				}else
					sprintf(ptxnLogs->top, "�ʽ�����ϸ��顾815��    ʣ������������ %d ��", reversal_count);
				strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter������ ��Esc������ ��F������excel��");
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -4;
		}
	}

	bbzero(sql);

	strcpy(sql, "select stan, decode(txn_date, NULL, '0000-00-00', substr(txn_date, 1, 4)||'-'||substr(txn_date, 5, 2)||'-'||substr(txn_date, 7, 2))||' '||decode(txn_time, NULL, '00:00:00', substr(txn_time, 1, 2)||':'||substr(txn_time, 3, 2)||':'||substr(txn_time, 5, 2)) txntime, substr(to_char(amount,'000000000000'),2,12) amount,decode(NVL(rsp_code, '96'), '00', decode(status, 0, decode(rev_status, 2, '�ѳ���', 4, '�ѳ���', '�ɹ�'), 'ʧ��'), 'ʧ��') status, NVL(biz_name,txn_log.biz_type) bizname,decode(txn_log.biz_type, '0065', substr(pay_no, length(pay_no)-5, 6), pay_no) payno, substr(to_char(nvl(balance,0),'000000000000'),2,12) balance,NVL(s_commision, 0) s_commision, rev_status, txn_log.biz_type from txn_log,biz_group where mti='0200'");

	if(strlen(date)==8)
	{
		strcat(sql, " and txn_date='");
		strcat(sql, date);
		strcat(sql, "'");
	}
	else if (strlen(date)==16)
	{
		bbzero(start_date);
		bbzero(end_date);
		memcpy(start_date,date,8);
		memcpy(end_date,date+8,8);
		strcat(sql, " and txn_date between'");
		strcat(sql, start_date);
		strcat(sql, "' and '");
		strcat(sql, end_date);
		strcat(sql, "'");
	}
	else if(strlen(date) == 6)
	{
		strcat(sql, " and (txn_date = to_char(sysdate, 'yyyymmdd') or (txn_date = to_char(sysdate-1, 'yyyymmdd') and txn_time > '");
		strcat(sql, date);
		strcat(sql, "'))");
	}
	else
	{
		strcat(sql, " and txn_date>=to_char(sysdate-30, 'yyyymmdd')");
	}
	strcat(sql, " and term_id='");
	strcat(sql, term_id);
	if(inout==0)//����
	{
		strcat(sql, "' and txn_log.biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340') and mti='0200'");
	
	}
	else//��ֵ
	{
		//strcat(sql, "' and (txn_log.biz_type in('0083', '0221', '0109', '0268', '0301', '0302', '0170') or mti='0400' )");
		strcat(sql, "' and (txn_log.biz_party = '200024' or mti='0400' )");
	}

 	if (mbl_flag == 1)	//�ƶ������
 		strcat(sql,"and (txn_log.biz_type  in('0174','0169','0213','0180','3002','0002','0176','0175','0343','0305','0334','0323','0327','0335','0316')  or ((txn_log.biz_type in ('0089','0153'))and (txn_log.pay_no like '02%')) )");

	if(status==1)//�ɹ�
	{
		strcat(sql, " and status='0' and txn_log.biz_type=biz_group.biz_type(+) order by txn_log.txn_date,txn_log.txn_time");
	
	}
	else if(status==2)//ʧ��
	{
		strcat(sql, " and status='0' and txn_log.biz_type=biz_group.biz_type(+) order by txn_log.txn_date,txn_log.txn_time");
	
	}
	else//ȫ��
	{
		if(strlen(date) == 6)
		{
			strcat(sql, " and txn_log.biz_type=biz_group.biz_type(+) order by txn_log.txn_date desc,txn_log.txn_time desc");
		}
		else
		{
			strcat(sql, " and txn_log.biz_type=biz_group.biz_type(+) order by txn_log.txn_date,txn_log.txn_time");
		}		
	}
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	
	for(i=0; !record->adoEOF && i<*itemNum; i++)
	{
		int zjhb_flag=0,mbl_flag_flag=0;
		
		//ϵͳ�ο���
		if((record->GetCollect("stan")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("stan"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("stan")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//����ʱ��
		if((record->GetCollect("txntime")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("txntime"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("txntime")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//��������
		if((record->GetCollect("bizname")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("bizname"))) < MAX_INTERM_BUF )
			{
				if (strcmp((LPCSTR)(_bstr_t)(record->GetCollect("bizname")),"�ʽ𻮲�")==0)
				{
					zjhb_flag = 1;
				}
				
				if (strstr((LPCSTR)(_bstr_t)(record->GetCollect("bizname")),"�ƶ�")!=NULL && mbl_flag == 2)
				{
					mbl_flag_flag = 1;
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "����");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}else if ((memcmp((LPCSTR)(_bstr_t)(record->GetCollect("biz_type")),"0089",4)==0 || memcmp((LPCSTR)(_bstr_t)(record->GetCollect("biz_type")),"0153",4)==0 )&&( memcmp((LPCSTR)(_bstr_t)(record->GetCollect("payno")),"02",2)==0 )&& mbl_flag == 2)
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "����");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}else
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("bizname")));
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}
				
			}
			else
			{
				NULL;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		
		//���׺���
		if((record->GetCollect("payno")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("payno"))) < MAX_INTERM_BUF )
			{
				if (mbl_flag_flag == 1 && mbl_flag == 2)
				{
					char no_buff[50];
					bbzero(no_buff);
					memcpy(no_buff,(LPCSTR)(_bstr_t)(record->GetCollect("payno"))+7,4);
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, no_buff);
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}else  if ((memcmp((LPCSTR)(_bstr_t)(record->GetCollect("biz_type")),"0089",4)==0 || memcmp((LPCSTR)(_bstr_t)(record->GetCollect("biz_type")),"0153",4)==0 )&&( memcmp((LPCSTR)(_bstr_t)(record->GetCollect("payno")),"02",2)==0 )&& mbl_flag == 2)
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "�ۿ�");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}else{
					if (zjhb_flag == 1)
					{
						char pay_no[50],pos_no[8+1];
						bbzero(pay_no);
						bbzero(pos_no);
						strcpy(pay_no, (LPCSTR)(_bstr_t)(record->GetCollect("payno")));
						ret = getPosno(pay_no,pos_no);
						if (ret == 0)
						{
							strcpy(ptxnLogs->items+ptxnLogs->itemsLen, pos_no);
							ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
						}else{
							strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payno")));
							ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
						}
						
					}else{
						strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payno")));
						ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
					}
				}
				
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���׽��
		//�޸Ľ��׽��ȡ�����ҵ�λ��Ԫ��
		if((record->GetCollect("amount")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("amount"))) < MAX_INTERM_BUF )
			{
				amo = atoi((LPCSTR)(_bstr_t)(record->GetCollect("amount")));
				rev_amo = amo;
				sprintf(ptxnLogs->items+ptxnLogs->itemsLen, "%d.%02d", amo/100, amo%100);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���׽��
		if((record->GetCollect("status")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("status"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("status")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		
		if(mode==0)
		{
			if((record->GetCollect("balance")).vt!=VT_NULL)
			{

				if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("balance"))) < MAX_INTERM_BUF )
				{

					amo = atoi((LPCSTR)(_bstr_t)(record->GetCollect("balance")));
					
					sprintf(ptxnLogs->items+ptxnLogs->itemsLen, "%d.%02d", amo/100, amo%100);
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;

				}
				else
				{
					break;
				}
			}
			else
			{
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
			}
			
			if((record->GetCollect("s_commision")).vt!=VT_NULL)
			{
				amo = atoi((LPCSTR)(_bstr_t)(record->GetCollect("s_commision")));
				if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("s_commision")))+4 < MAX_INTERM_BUF )
				{
					if(amo>=0)
					{
						sprintf(ptxnLogs->items+ptxnLogs->itemsLen, "%d.%02d", amo/100, amo%100);
					}
					else
					{
						sprintf(ptxnLogs->items+ptxnLogs->itemsLen, "-%d.%02d", -amo/100, -amo%100);
					}
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
			}

			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;

			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		else
		{
			if((record->GetCollect("rev_status")).vt!=VT_NULL && (record->GetCollect("biz_type")).vt!=VT_NULL && (record->GetCollect("txntime")).vt!=VT_NULL)
			{
				bbzero(biz_type);
				strncpy(biz_type, (LPCSTR)(_bstr_t)(record->GetCollect("biz_type")), 4);
				rev_status = atoi((LPCSTR)(_bstr_t)(record->GetCollect("rev_status")));
				bbzero(revmsg);

				if( strstr("[0089][0153][0245][0266]", biz_type)!=NULL )//�ۿ�
				{
					if( strcmp((LPCTSTR)(_bstr_t)record->GetCollect("status"), "�ɹ�")==0)
					{
						int nYear=0, nMonth=0, nDay=0, nHour=0, nMin=0, nSec=0;
						char cDateBuf[4+1], txntime[19+1];
						bbzero(txntime);
						strncpy(txntime, (LPCTSTR)(_bstr_t)record->GetCollect("txntime"), 19);
						bbzero(cDateBuf);
						strncpy(cDateBuf, txntime, 4);
						nYear = atoi(cDateBuf);
						bbzero(cDateBuf);
						strncpy(cDateBuf, txntime+5, 2);
						nMonth = atoi(cDateBuf);
						bbzero(cDateBuf);
						strncpy(cDateBuf, txntime+8, 2);
						nDay = atoi(cDateBuf);
						bbzero(cDateBuf);
						strncpy(cDateBuf, txntime+11, 2);
						nHour = atoi(cDateBuf);
						bbzero(cDateBuf);
						strncpy(cDateBuf, txntime+14, 2);
						nMin = atoi(cDateBuf);
						bbzero(cDateBuf);
						strncpy(cDateBuf, txntime+17, 2);
						nSec = atoi(cDateBuf);
						if(CTools::chkDate(nYear, nMonth, nDay)==0)
						{
							CTime cTxnTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
							CTime cCurTime = CTime::GetCurrentTime();
							CTime cCurDate = CTime(cCurTime.GetYear(), cCurTime.GetMonth(), cCurTime.GetDay(), 0, 0, 0);
							if( cTxnTime < cCurDate - CTimeSpan(7,0,0,0) )
							{
								strcpy(revmsg, "�ѹ�7�첻�ܲ���");
							}
							else if (memcmp((LPCSTR)(_bstr_t)(record->GetCollect("payno")),"02",2)==0 && mbl_flag==2)
							{
								strcpy(revmsg, "��ʹ��ipos_m���в���");
							}else
							{
								strcpy(revmsg, "�ɲ���");
							}
						}
						else
						{
							strcpy(revmsg, "");
						}
					}
					else
					{
						strcpy(revmsg, "");
					}
				}
				else if( strstr("[0180][0213][0255][0256][0257][0296][0297][0265][0300][0169][0174][0323][0327][0321][0342][0344][0343][0305][0356][0316]", biz_type)!=NULL )//�ճ�
				{
					if( rev_status==0 )//���������
					{
						if( strcmp((LPCTSTR)(_bstr_t)record->GetCollect("status"), "�ɹ�")==0)
						{
							int nYear=0, nMonth=0, nDay=0, nHour=0, nMin=0, nSec=0;
							char cDateBuf[4+1], txntime[19+1];
							bbzero(txntime);
							strncpy(txntime, (LPCTSTR)(_bstr_t)record->GetCollect("txntime"), 19);
							bbzero(cDateBuf);
							strncpy(cDateBuf, txntime, 4);
							nYear = atoi(cDateBuf);
							bbzero(cDateBuf);
							strncpy(cDateBuf, txntime+5, 2);
							nMonth = atoi(cDateBuf);
							bbzero(cDateBuf);
							strncpy(cDateBuf, txntime+8, 2);
							nDay = atoi(cDateBuf);
							bbzero(cDateBuf);
							strncpy(cDateBuf, txntime+11, 2);
							nHour = atoi(cDateBuf);
							bbzero(cDateBuf);
							strncpy(cDateBuf, txntime+14, 2);
							nMin = atoi(cDateBuf);
							bbzero(cDateBuf);
							strncpy(cDateBuf, txntime+17, 2);
							nSec = atoi(cDateBuf);
							if(CTools::chkDate(nYear, nMonth, nDay)==0)
							{
								CTime cTxnTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
								if( cTxnTime < CTime::GetCurrentTime() - CTimeSpan(0,24,0,0) )
								{
									strcpy(revmsg, "�ѹ�24Сʱ���ܳ���");
								}
								/*else if( cTxnTime > CTime::GetCurrentTime() - CTimeSpan(0,0,15,0) )
								{
									strcpy(revmsg, "���׺�15�����ڲ��ܳ���");
								}*/
								else if(cTxnTime.GetDay() != 1 && (CTime::GetCurrentTime()).GetDay() == 1)
								{
									strcpy(revmsg, "���ɿ��³���");
								}
								else if( cTxnTime > CTime::GetCurrentTime() - CTimeSpan(0,24,0,0)&& cTxnTime < CTime::GetCurrentTime() - CTimeSpan(0,12,0,0))
								{
									strcpy(revmsg, "����12Сʱ����ͨ��[861]�������");
								}
								else
								{
									if (mbl_flag == 2 && mbl_flag_flag == 1)
									{
										strcpy(revmsg, "��ʹ��ipos_m���г���");
									}else
										strcpy(revmsg, "���������");
								}
							}
							else
							{
								strcpy(revmsg, "");
							}
						}
						else
						{
							strcpy(revmsg, "");
						}
					}
					else if( rev_status==1 )
					{
						strcpy(revmsg, "���ύ��������");
					}				
					else if( rev_status==2 )
					{
						strcpy(revmsg, "�ѳ���");
						//sprintf(revmsg, "����%d.%02dԪ", rev_amo/100, rev_amo%100);
					}
					else if( rev_status==3 )
					{
						strcpy(revmsg, "����ʧ��");
					}
					else if( rev_status==4 )
					{
						strcpy(revmsg, "�ѳ���");
						//sprintf(revmsg, "����%d.%02dԪ", rev_amo/100, rev_amo%100);
					}
					else if( rev_status == 5)
					{
						strcpy(revmsg, "����������");
					}
					else
					{
						strcpy(revmsg, "δ֪״̬");
					}
				}
				else
				{
					strcpy(revmsg, "");
				}

				if( ptxnLogs->itemsLen+strlen(revmsg) < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, revmsg);
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(revmsg) + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
			}


			if( strcmp(revmsg, "�ɲ���")==0 )
			{
				if( ptxnLogs->itemsLen+strlen("206") < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "206");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("206") + 1;
				}
				else
				{
					break;
				}
			}
			else if(strlen(date) != 6)
			{
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
			}
			else if( strcmp(revmsg, "���������")==0 )
			{
				if( ptxnLogs->itemsLen+strlen("584") < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "584");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("584") + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
			}

			//�˵����͵�����
			if((record->GetCollect("stan")).vt!=VT_NULL)
			{
				if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("stan"))) < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("stan")));
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
			}
		}
		record->MoveNext();
	}

	g_adoConnInfo[index].connStat = 0;

	return 0;
}

//����3���ڵ����ۼ�¼
int CDB::countSaleTxn(char *term_id, char *product_id,int *cnt)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	
	wrLog("��countSaleTxn��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;
	bbzero(tmp);
	strcpy(tmp, "select count(*) cnt from sale_txn_table where txn_date||txn_time>to_char(sysdate-3,'yyyymmddhh24miss') and term_id='");
	strcat(tmp, term_id);
	strcat(tmp, "' and product_id='");
    strcat(tmp, product_id);
	strcat(tmp, "' and rsp='00'");
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((record->GetCollect("cnt")).vt!=VT_NULL)
		{
			*cnt = atoi((LPCTSTR)(_bstr_t)record->GetCollect("cnt"));
			wrLog("3����%s��%s���ۼ�¼����[%d]",term_id,product_id,*cnt);
		}
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

#if 0
//������Ӻ��Ķ�ȡ���ճɹ������ܽ��(�Է�Ϊ��λ)
int CDB::pep_getDayAmo(char *term_id, int* dayAmt)
{
	CConnect cont;
	SOCKET sd;
	char sendbuf[1024] = {0};
	char curdatetime[16] = {0};
	char lenbuf[16] = {0};
	int timeout = 10; //Ĭ�ϳ�ʱʱ��
	int buflen = 0;
	int recvlen;
	char outBuf[2048] = {0};
	char rspcpde[16] = {0};
	int iRet;
	char summon[16] = {0};

	//����׼��
	CTools::get_termTime(curdatetime);
	sprintf(sendbuf, "FFFF%014s%s%08d%s%s",curdatetime,term_id,buflen,"PEP000I0",term_id);
	buflen = strlen(sendbuf);
	sprintf(lenbuf, "%08d", buflen);
	memcpy(sendbuf+4+22, lenbuf, 8);
	wrLog("���ͻ�ȡ��ǰ�ɹ����׽��ĵ�ACC:[%s]", sendbuf);

	iRet = cont.conToHost(PE_IP, PE_PORT, &sd);
    if (iRet != 0) {
        wrLog("���ӵ��������������飡%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    
    iRet = cont.sndMsg(sd, sendbuf, buflen, timeout);
    if (iRet != 0) {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

    recvlen = 0;
    iRet = recv(sd, outBuf, 4+22+8, 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8) {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

	int pack_len = atoi(outBuf+4+22) - 4 - 22 - 8 ;//���°��峤��
    int need_rcv = pack_len;
    int has_rcv  = 0;
    while (need_rcv > has_rcv) {
        iRet = recv(sd, outBuf + 4 + 22 + 8 + has_rcv, pack_len - has_rcv, 0);
        if (iRet == -1) {
            if (errno == EAGAIN) {
                continue;
			} else {
                wrLog("�������ͨ�ų��ֹ��� %s:%d" , __FILE__ , __LINE__);
                return -1;
            }
        }
        has_rcv += iRet;
    }
	wrLog("����ACCӦ��:[%s]", outBuf);
    closesocket(sd);
    recvlen = pack_len;

	//����
	int pos = 42;
	char rspcode[8] = {0};
	char desplen[8] = {0};
	char desp[1024] = {0};
	int tmplen = 0;
	//��Ӧ��
	memcpy(rspcpde, outBuf + pos, 4);
	pos += 4;
	//��������
	memcpy(desplen, outBuf + pos, 2);
	pos += 2;
	tmplen = atoi(desplen);
	//��������
	memcpy(desp, outBuf + pos, tmplen);
	pos += tmplen;
	wrLog("��Ӧ��:[%s]:[%s]", rspcpde, desp);
	if (atoi(rspcpde) == 0) {
		//�ɹ����н����Ϣ
		memcpy(summon, outBuf + pos, 12);
		pos += 12;
		*dayAmt = atoi(summon);
		wrLog("��ǰ�ճɹ����׽�[%d]",*dayAmt);
		return 0;
	} 
	return -1;	
}
#endif

#if 1
//�����ݿ���ȡ��ĳ�ն˵ĵ��ճɹ������ܶ�(�Է�Ϊ��λ)
int CDB::getDayAmo(char *term_id, int *dayAmo)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	
	wrLog("��getDayAmo��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;
	bbzero(tmp);
	strcpy(tmp, "select sum_dayamo from epos_term where term_id='");
	strcat(tmp, term_id);
	strcat(tmp, "'");
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((record->GetCollect("sum_dayamo")).vt!=VT_NULL)
		{
			*dayAmo = atoi(((LPCTSTR)(_bstr_t)record->GetCollect("sum_dayamo")));
			wrLog("��ǰ�ս����ܶ[%d]",*dayAmo);
		}
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}
#endif

//�����ս������ƵĽ���С
int CDB::updateDayAmo(char *term_id, int dayAmo)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("��updateDayAmo��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update epos_term set dayamo=%d where term_id = '%s'", dayAmo, term_id);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и���epos_term����ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::select_cdma_mh(CDMA_INFO* cdma_info,char *phone_num,int t_flag)
{
	int ret, index, i;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	
	wrLog("��getDayAmo��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;
	bbzero(tmp);
	if (t_flag == 1)
	{
		strcpy(tmp, "select b.* from MobileNo_Branch a,MobileNo_Chose b where a.C_SORT1='153' and a.C_SORT2='���Ŀ�' and a.I_BRANCHID=b.I_BRANCHID and b.C_SALESTATUS=1 and b.C_MobileNo like '%");
		strcat(tmp,phone_num);
		strcat(tmp,"%'");
	}else if (t_flag == 2)
	{
		strcpy(tmp, "select b.* from MobileNo_Branch a,MobileNo_Chose b where a.C_SORT1='153' and a.C_SORT2='���ڿ�' and a.I_BRANCHID=b.I_BRANCHID and b.C_SALESTATUS=1 and b.C_MobileNo like '%");
		strcat(tmp,phone_num);
		strcat(tmp,"%'");
	}else 
		return -1;
	vSQL = tmp;
	wrLog("[%s]",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	

	i = 0;
	while(!record->adoEOF)
	{
		if((record->GetCollect("C_MobileNo")).vt!=VT_NULL)
			strcpy(cdma_info[i].phone_num,(LPCTSTR)(_bstr_t)record->GetCollect("C_MobileNo"));
		else
			memset(cdma_info[i].phone_num,0,sizeof(cdma_info[i].phone_num));
		if((record->GetCollect("C_PRICE")).vt!=VT_NULL)
			strcpy(cdma_info[i].phone_price,(LPCTSTR)(_bstr_t)record->GetCollect("C_PRICE"));
		else
			memset(cdma_info[i].phone_price,0,sizeof(cdma_info[i].phone_price));
		if((record->GetCollect("C_EXPIREDATE")).vt!=VT_NULL)
			strcpy(cdma_info[i].yx_date,(LPCTSTR)(_bstr_t)record->GetCollect("C_EXPIREDATE"));
		else
			memset(cdma_info[i].yx_date,0,sizeof(cdma_info[i].yx_date));
		if((record->GetCollect("C_NOTE")).vt!=VT_NULL)
			strcpy(cdma_info[i].phone_message,(LPCTSTR)(_bstr_t)record->GetCollect("C_NOTE"));
		else
			memset(cdma_info[i].phone_message,0,sizeof(cdma_info[i].phone_message));
		cdma_info[i].flag = 1;
		record->MoveNext();
		i++;
	}
	
	g_adoConnInfo[index].connStat = 0;
	return i;
}
int CDB::select_cdma_sj(CDMA_INFO* cdma_info,int t_flag)
{
	int ret, index, i;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];

	wrLog("��getDayAmo��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;
	bbzero(tmp);
	if (t_flag == 1)
	{
		strcpy(tmp, "select *from (select b.* from MobileNo_Branch a,MobileNo_Chose b where a.C_SORT1='153' and a.C_SORT2='���Ŀ�' and a.I_BRANCHID=b.I_BRANCHID and b.C_SALESTATUS=1 order by dbms_random.value) where rownum<=100");
	}else if (t_flag == 2)
	{
		strcpy(tmp, "select *from (select b.* from MobileNo_Branch a,MobileNo_Chose b where a.C_SORT1='153' and a.C_SORT2='���ڿ�' and a.I_BRANCHID=b.I_BRANCHID and b.C_SALESTATUS=1 order by dbms_random.value) where rownum<=100");
	}else 
		return -1;
	vSQL = tmp;
	wrLog("[%s]",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	
	i = 0;
	while(!record->adoEOF)
	{
		if((record->GetCollect("C_MobileNo")).vt!=VT_NULL)
			strcpy(cdma_info[i].phone_num,(LPCTSTR)(_bstr_t)record->GetCollect("C_MobileNo"));
		else
			memset(cdma_info[i].phone_num,0,sizeof(cdma_info[i].phone_num));
		if((record->GetCollect("C_PRICE")).vt!=VT_NULL)
			strcpy(cdma_info[i].phone_price,(LPCTSTR)(_bstr_t)record->GetCollect("C_PRICE"));
		else
			memset(cdma_info[i].phone_price,0,sizeof(cdma_info[i].phone_price));
		if((record->GetCollect("C_EXPIREDATE")).vt!=VT_NULL)
			strcpy(cdma_info[i].yx_date,(LPCTSTR)(_bstr_t)record->GetCollect("C_EXPIREDATE"));
		else
			memset(cdma_info[i].yx_date,0,sizeof(cdma_info[i].yx_date));
		if((record->GetCollect("C_NOTE")).vt!=VT_NULL)
			strcpy(cdma_info[i].phone_message,(LPCTSTR)(_bstr_t)record->GetCollect("C_NOTE"));
		else
			memset(cdma_info[i].phone_message,0,sizeof(cdma_info[i].phone_message));
		cdma_info[i].flag = 1;
		record->MoveNext();
		i++;
	}

	g_adoConnInfo[index].connStat = 0;
	return i;
}

//��ѯ׼�����������
int CDB::showchktxn(char *term_id, char *detail)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char mny[12+1], amount[12+1];
	char sql[1024];
	
	wrLog("��showchktxn��");
	index = getConnStat();  //��ȡ��������
	wrLog("�������Ӻţ�[%d]", index);
	if( index<0 )
		return -1;

	bbzero(mny);
	bbzero(amount);
	bbzero(sql);
	strcpy(sql, "select inchknum,inchkamo,outchknum,outchkamo,nschknum,nschkamo from (select count(*) inchknum, lpad(NVL(sum(amount),0), 12, 0) inchkamo from txn_log where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and rsp_code='00' and status=0 and chk_status=0 and biz_type in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) inchk, (select count(*) outchknum, lpad(NVL(sum(amount),0), 12, 0) outchkamo from txn_log where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and rsp_code='00' and status=0 and chk_status=0 and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) outchk, (select count(*) nschknum, lpad(NVL(sum(amount),0), 12, 0) nschkamo from txn_log where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and rsp_code<>'00' and (info='20004372' or info='20005872') and chk_status=0 and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) nschk");
	vSQL = sql;
	wrLog("[%s]", sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		strcpy(detail, "������ϸ��\n1.�˻��ɹ�ת�ˣ���");
		strcat(detail, (LPCTSTR)(_bstr_t)record->GetCollect("inchknum"));
		strcat(detail, "�� ");		
		strcpy(amount, (LPCTSTR)(_bstr_t)record->GetCollect("inchkamo"));
		CTools::chgToMny(amount, mny);
		strcat(detail, mny);

		strcat(detail, "\n2.�ɹ����ѣ���");
		strcat(detail, (LPCTSTR)(_bstr_t)record->GetCollect("outchknum"));
		strcat(detail, "�� ");
		strcpy(amount, (LPCTSTR)(_bstr_t)record->GetCollect("outchkamo"));
		CTools::chgToMny(amount, mny);
		strcat(detail, mny);

// 		strcat(detail, "\n3.δȷ�ϵĽ��ѣ���");
// 		strcat(detail, (LPCTSTR)(_bstr_t)record->GetCollect("nschknum"));
// 		strcat(detail, "�� ");
// 		strcpy(amount, (LPCTSTR)(_bstr_t)record->GetCollect("nschkamo"));
// 		CTools::chgToMny(amount, mny);
// 		strcat(detail, mny);
		strcat(detail, "\n");
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//����
int CDB::chktxn(char *term_id, char *ecardamo, char *bankamo, char *chktermseq, char *chksysseq)
{
	int index, ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[2048];
	
	wrLog("��chktxn��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	//ȡϵͳ������ˮ
	vSQL = "select lpad(txn_log_check_seq.nextval,12,0) chksysseq from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( chksysseq, (LPCSTR)(_bstr_t)(record->GetCollect("chksysseq")) );
	wrLog("ϵͳ�������κţ�[%s]", chksysseq);

	//��������¼��
	bbzero(sql);
	strcpy(sql, "insert into txn_log_check (chk_seq, term_id, term_chk_seq, chk_time, ecard_in_num, ecard_in_amo, pay_num, pay_amo, pay_num_unsure, pay_amo_unsure, ecard_amo, bcard_amo) select '");
	strcat(sql, chksysseq);
	strcat(sql, "', '");
	strcat(sql, term_id);
	strcat(sql, "', (select nvl(max(term_chk_seq),0)+1 from txn_log_check where term_id='");
	strcat(sql, term_id);
	strcat(sql, "'), sysdate, chkamo.*,");
	strcat(sql, ecardamo);
	strcat(sql, ",");
	strcat(sql, bankamo);
	strcat(sql, " from dual,(select inchknum,inchkamo,outchknum,outchkamo,nschknum,nschkamo from (select count(*) inchknum, lpad(NVL(sum(amount),0), 12, 0) inchkamo from txn_log where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and rsp_code='00' and chk_status=0 and biz_type in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) inchk, (select count(*) outchknum, lpad(NVL(sum(amount),0), 12, 0) outchkamo from txn_log where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and rsp_code='00' and chk_status=0 and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) outchk, (select count(*) nschknum, lpad(NVL(sum(amount),0), 12, 0) nschkamo from txn_log where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and rsp_code<>'00' and (info='20004372' or info='20005872') and chk_status=0 and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')) nschk) chkamo");
	vSQL = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��в���txn_log_check����ʧ��sql[%s]", sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	//ȡ�ն˽�����ˮ
	strcpy(sql, "select lpad(nvl(max(term_chk_seq),0),12,0) chktermseq from txn_log_check where term_id='");
	strcat(sql, term_id);
	strcat(sql, "'");
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( chktermseq, (LPCSTR)(_bstr_t)(record->GetCollect("chktermseq")) );
	wrLog("�ն˽������κţ�[%s]", chksysseq);

	//����:����TXN_LOG��
	//�����ʻ���ֵ
	strcpy(sql, "update txn_log set chk_status=2 where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and rsp_code='00' and chk_status=0 and biz_type in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')");
	vSQL = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и���txn_log����ʧ��sql[%s]", sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	//���½���
	strcpy(sql, "update txn_log set chk_status=2 where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and rsp_code='00' and chk_status=0 and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')");
	vSQL = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и���txn_log����ʧ��sql[%s]", sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	//����δȷ�ϵĽ���
	strcpy(sql, "update txn_log set chk_status=3 where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and rsp_code<>'00' and (info='20004372' or info='20005872') and chk_status=0 and biz_type not in('0083', '0221', '0109', '0268', '0301', '0302', '0170', '0339', '0340')");
	vSQL = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и���txn_log����ʧ��sql[%s]", sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	//����ʣ�µ�
	strcpy(sql, "update txn_log set chk_status=4 where term_id='");
	strcat(sql, term_id);
	strcat(sql, "' and txn_date>='20090522' and chk_status=0");
	vSQL = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и���txn_log����ʧ��sql[%s]", sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

////////////////////////////////��ݸ����////////////////////////////////
int CDB::get_DGBus_stan(char *seq)
{
	int ret, index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	wrLog("�����ݿ��ȡ����STAN����");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��index=%d��", index);
	vSQL = "select lpad(busstan.nextval, 12,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 0;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	memcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")), 12);
	wrLog("���кţ�[%s]", seq);
	g_adoConnInfo[index].connStat = 0;
	return 0;
}
//���붫ݸ�������ݿ�
//seq: �������к�
int CDB::set_DGBus_Txn(CGlobal gbl)  //kf,2008-6-6
{
	int index;
	_bstr_t vSQL;
	char tmp[10000];
	char bus_card_infoBCD[96+1];
	char dkq_code[8+1];

	wrLog("��set_DGBus_Txn��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��index=%d��", index);

	bbzero(bus_card_infoBCD);
	bbzero(dkq_code);

	CTools::byte_to_hex((const BYTE*)gbl.bus_card_info,(BYTE*)bus_card_infoBCD,48);

	CTools::byte_to_hex((const BYTE*)gbl.dkq_code,(BYTE*)dkq_code,4);

	bbzero(tmp);
	sprintf(tmp, "INSERT INTO DGBus_txn(term_id, term_serial, phone, stan, bus_enq_stan, bus_biz, bank, bank_card, bus_card, old_amo, pay_amo, new_amo, bus_date, bus_time, status, amount, bus_card_info, bus_selltype,dkq_code,dkq_psam,yj_amo) VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '00000000', '%s', '%s', '0', '%s', '%s', '%s', '%s', '%s', '%s')",
		gbl.pos_no, gbl.bus_serial, gbl.call_tel, gbl.stan_no, gbl.bus_enq_stan, gbl.fee_type, gbl.bank, gbl.card_no, gbl.bus_card,gbl.bus_old_amo,
		gbl.bus_pay_amo,gbl.bus_date, gbl.bus_time, gbl.amount, bus_card_infoBCD, gbl.bus_selltype,dkq_code,gbl.dkq_PSAM,gbl.bus_yj_amo);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//���붫ݸ�������ݿ�
//seq: �������к�
int CDB::update_DGBus_Txn(CGlobal gbl, char *status)  //kf,2008-6-6
{
	int index;
	_bstr_t vSQL;
	char tmp[1024];
	char bus_card_infoBCD[96+1];
	
	wrLog("��update_DGBus_Txn��");
	index = getConnStat();  //��ȡ��������
	wrLog("��index=%d��", index);
	if( index<0 )
		return -1;

	bbzero(bus_card_infoBCD);
	CTools::byte_to_hex((const BYTE*)gbl.bus_card_info,(BYTE*)bus_card_infoBCD,48);
	bbzero(tmp);
	sprintf(tmp, "update DGBus_txn set new_amo='%s', status='%s', bus_card_info='%s' where status not in ('2','5','10') and bus_enq_stan='%s'",
		gbl.bus_new_amo, status, bus_card_infoBCD, gbl.bus_enq_stan );
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ݿ���ȡ����DGBus_Txn��δ������Ϣ����ȫ�ֱ�����
int CDB::get_DGBus_Txn_NotEnd(CGlobal &gbl, char *status) 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��get_DGBus_Txn_NotEnd��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��index=%d��", index);
	bbzero(tmp);
	sprintf(tmp, "select * from DGBus_txn where term_id='%s' and status in ('0','1','3','4','6','7','8','9') and stan=(select max(stan) from DGBus_txn where term_id='%s')", gbl.pos_no, gbl.pos_no);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 0;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!record->adoEOF)
	{
		wrLog("��begin copy dgbus txn info��");
		bbzero(gbl.bus_serial);		
		bbzero(gbl.stan_no);
		bbzero(gbl.bus_enq_stan);
		bbzero(gbl.fee_type);
		bbzero(gbl.bank);
		bbzero(gbl.card_no);
		bbzero(gbl.bus_card);
		bbzero(gbl.bus_old_amo);
		bbzero(gbl.bus_pay_amo);
		bbzero(gbl.bus_new_amo);
		bbzero(gbl.bus_date);
		bbzero(gbl.bus_time);
		bbzero(gbl.amount);
		bbzero(gbl.bus_card_info);
		bbzero(gbl.bus_selltype);
		bbzero(gbl.dkq_code);
		bbzero(gbl.dkq_PSAM);
		bbzero(gbl.bus_yj_amo);
		strcpy(gbl.bus_serial, (LPCTSTR)(_bstr_t)record->GetCollect("term_serial"));
		strcpy(gbl.stan_no, (LPCTSTR)(_bstr_t)record->GetCollect("stan"));
		strcpy(gbl.bus_enq_stan, (LPCTSTR)(_bstr_t)record->GetCollect("bus_enq_stan"));
		strcpy(gbl.fee_type, (LPCTSTR)(_bstr_t)record->GetCollect("bus_biz"));
		strcpy(gbl.bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank"));
		strcpy(gbl.card_no, (LPCTSTR)(_bstr_t)record->GetCollect("bank_card"));
		gbl.card_no_len = strlen(gbl.card_no);
		strcpy(gbl.bus_card, (LPCTSTR)(_bstr_t)record->GetCollect("bus_card"));
		strcpy(gbl.bus_old_amo, (LPCTSTR)(_bstr_t)record->GetCollect("old_amo"));
		strcpy(gbl.bus_pay_amo, (LPCTSTR)(_bstr_t)record->GetCollect("pay_amo"));
		strcpy(gbl.bus_new_amo, (LPCTSTR)(_bstr_t)record->GetCollect("new_amo"));
		strcpy(gbl.bus_yj_amo, (LPCTSTR)(_bstr_t)record->GetCollect("yj_amo"));
		strcpy(gbl.bus_date, (LPCTSTR)(_bstr_t)record->GetCollect("bus_date"));
		strcpy(gbl.bus_time, (LPCTSTR)(_bstr_t)record->GetCollect("bus_time"));
		strcpy(status, (LPCTSTR)(_bstr_t)record->GetCollect("status"));
		strcpy(gbl.amount, (LPCTSTR)(_bstr_t)record->GetCollect("amount"));
		//strcpy(gbl.dkq_code, (LPCTSTR)(_bstr_t)record->GetCollect("dkq_code"));
		CTools::hex_to_byte((const BYTE *)(LPCTSTR)(_bstr_t)record->GetCollect("dkq_code"), (BYTE *)gbl.dkq_code, 8);
		strcpy(gbl.dkq_PSAM, (LPCTSTR)(_bstr_t)record->GetCollect("dkq_PSAM"));
		CTools::hex_to_byte((const BYTE *)(LPCTSTR)(_bstr_t)record->GetCollect("bus_card_info"), (BYTE *)gbl.bus_card_info, 96);
		strcpy(gbl.bus_selltype, (LPCTSTR)(_bstr_t)record->GetCollect("bus_selltype"));
		ret = 0;
		wrLog("��end copy dgbus txn info��");
	}else{
		ret = -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return ret;
}

//�����Ƿ��г����Ľ���
int CDB::get_yctptxn(CGlobal &gbl,PYCT_DATA  yctData,char * charge_amo) 
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��index=%d��", index);
	bbzero(tmp);
	sprintf(tmp, "select * from yctyp_txn t where to_char(t.txn_time,'yyyymmdd')=to_char(sysdate,'yyyymmdd') and t.charge_type='0' and t.status='2' and yct_ack='1' and t.yct_serial='%s' and t.term_id='%s'",yctData->yct_serial,gbl.pos_no);
	wrLog("��ѯ�Ƿ����������ͨ����:%s",tmp);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 0;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!record->adoEOF)
	{
	
		strcpy(yctData->amount, (LPCTSTR)(_bstr_t)record->GetCollect("AMOUNT")); //���׽��
		if(atof(yctData->amount)!=atof(charge_amo))
		{
			return -1; //ԭ��ֵ����
		}

		strcpy(yctData->stan, (LPCTSTR)(_bstr_t)record->GetCollect("STAN")); //������ԭ��ˮ�ţ���ҵ�˻���
		strcpy(yctData->ticket_amo_aft, (LPCTSTR)(_bstr_t)record->GetCollect("TICKET_AMO_AFT")); //����ֵ�����
		strcpy(yctData->ticket_amo_bef, (LPCTSTR)(_bstr_t)record->GetCollect("TICKET_AMO_BEF")); //����ֵǰ���
		strcpy(yctData->term_id, (LPCTSTR)(_bstr_t)record->GetCollect("term_id")); //�ն˺�
		strcpy(yctData->bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank")); //��ֵ����
		strcpy(yctData->card_no, (LPCTSTR)(_bstr_t)record->GetCollect("card_no")); //��ֵ����
		strcpy(yctData->pki_card, (LPCTSTR)(_bstr_t)record->GetCollect("pki_card")); //pki����
		strcpy(yctData->amount, (LPCTSTR)(_bstr_t)record->GetCollect("amount")); //pki����
		strcpy(yctData->ticket_no, (LPCTSTR)(_bstr_t)record->GetCollect("ticket_no")); //���ͨ����
		strcpy(yctData->term_serial, (LPCTSTR)(_bstr_t)record->GetCollect("TERM_SERIAL")); //�ն���ˮ��

		ret = 0;
		wrLog("��end copy dgbus txn info��");
	}else{
		ret = -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return ret;
}

//�����ն�״̬
int CDB::update_epos_term(char *term_id, char *status){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("�������ն�״̬��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update epos_term set status='%s' where term_id = '%s'", status, term_id);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и����ն�״̬ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}
////////////////////////////////��ݸ����////////////////////////////////
int CDB::getEposTips()
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposTips��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_tips";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}

	if(!epos_tips.empty())
	{
		epos_tips.clear();
	}

	Epos_tips epos_tips_tmp;

	while(!record->adoEOF)
	{
		memset(&epos_tips_tmp, 0, sizeof(epos_tips_tmp));

		if((record->GetCollect("return")).vt!=VT_NULL)
			strcpy(epos_tips_tmp.return_flag,(LPCTSTR)(_bstr_t)record->GetCollect("return") );
		else
			strcpy(epos_tips_tmp.return_flag,"1");
		if((record->GetCollect("tips")).vt!=VT_NULL)
			strcpy(epos_tips_tmp.tips, (LPCTSTR)(_bstr_t)record->GetCollect("tips") );
		else
			strcpy(epos_tips_tmp.tips,"");
		if((record->GetCollect("epos_or_ipos")).vt!=VT_NULL)
			strcpy(epos_tips_tmp.pos_flag, (LPCTSTR)(_bstr_t)record->GetCollect("epos_or_ipos") );
		else
			strcpy(epos_tips_tmp.pos_flag,"0");
		epos_tips_tmp.menu_id = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("menu_id") );
		
		epos_tips.push_back(epos_tips_tmp);
		record->MoveNext();
	}
	
	wrLog("��getEposTips�� ������%d��", epos_tips.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}


int CDB::getSellCardAmo()
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getSellCardAmo��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from sell_card_amo order by priority";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}
	
	if(!sell_card_amo.empty())
	{
		sell_card_amo.clear();
	}
	
	Sell_Card_Amo sell_card_amo_tmp;

	while(!record->adoEOF)
	{
		memset(&sell_card_amo_tmp, 0, sizeof(sell_card_amo_tmp));

		strcpy(sell_card_amo_tmp.biz_type, (LPCTSTR)(_bstr_t)record->GetCollect("biz_type") );
		strcpy(sell_card_amo_tmp.card_type, (LPCTSTR)(_bstr_t)record->GetCollect("card_type") );
		sell_card_amo_tmp.amount = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("amount") );		
		
		sell_card_amo.push_back(sell_card_amo_tmp);

		record->MoveNext();
	}

	wrLog("��getSellCardAmo�� ������%d��", sell_card_amo.size());
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getEposMobile()
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposMobile��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_mobile";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}
	
	if(!epos_mobile.empty())
	{
		epos_mobile.clear();
	}

	Epos_mobile epos_mobile_tmp;
	
	while(!record->adoEOF)
	{
		memset(&epos_mobile_tmp, 0, sizeof(epos_mobile_tmp));

		if((record->GetCollect("mobile_field")).vt!=VT_NULL)
			strcpy(epos_mobile_tmp.mobile_field, (LPCTSTR)(_bstr_t)record->GetCollect("mobile_field") );
		else
			strcpy(epos_mobile_tmp.mobile_field,"");
		epos_mobile_tmp.mobile_owner = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("mobile_owner") );
		
		epos_mobile.push_back(epos_mobile_tmp);

		record->MoveNext();
	}

	wrLog("��getEposMobile�� ������%d��", epos_mobile.size());
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//ȡ�����ݿ��м�¼�ļ�������
int CDB::getEncryptType(int *encrypt_type,char *term_id)
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];	
	bbzero(tmp);
	
	wrLog("��getEncryptType��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	sprintf(tmp,"select encrypt_type from tv_user_info where payeasyid='%s'",term_id);
    vSQL=tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{   
		*encrypt_type=1;
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}
	if(!record->adoEOF)
	{   
		if((record->GetCollect("encrypt_type")).vt!=VT_NULL)
		   *encrypt_type=atoi((LPCTSTR)(_bstr_t)record->GetCollect("encrypt_type"));
		else
           *encrypt_type=3;

	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::setEncryptType(int encrypt_type,int sqlType,char *term_id)
{
	int index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	bbzero(tmp);
	if(sqlType==1)
	  sprintf(tmp, "UPDATE tv_user_info SET encrypt_type='%d',mark_time=sysdate WHERE payeasyid='%s'",encrypt_type, term_id);
	else
		sprintf(tmp, "INSERT INTO tv_user_info(payeasyid,encrypt_type,mark_time) VALUES('%s','%d',sysdate)",term_id,encrypt_type);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��setEncryptType �������ݿ�ͨ�� %d��", index);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("setEncryptType �����������ݿ�ʧ��");
		wrLog("setEncryptType SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//��ȡ���ݿ��е��Ƿ�ͬ��ļ�¼
int CDB::getAgreeFlag(int *agreeFlag,char *term_id)  
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];	
	bbzero(tmp);
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��index=%d��", index);
	sprintf(tmp,"select AGREE_FLAG from  epos_term  where term_id='%s'",term_id);
    vSQL=tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{   
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if(!record->adoEOF)
	{   
			*agreeFlag=atoi((LPCTSTR)(_bstr_t)record->GetCollect("AGREE_FLAG")); //�޸�agreeFlag
	
	}	

	g_adoConnInfo[index].connStat = 0;
	return 0;

}

int CDB::setAgreeFlag(int agreeFlag,char *term_id)
{
	int index = -1;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	bbzero(tmp);
	sprintf(tmp, "UPDATE  epos_term SET agree_flag='%d' WHERE term_id='%s'",agreeFlag, term_id);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��setAgreeFlag �������ݿ�ͨ�� %d��", index);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("setAgreeFlag �������ݿ�ʧ��");
		wrLog("setAgreeFlag SQL��䣺[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;

}

int CDB::getEposTurn()
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposTurn��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_turn";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}
	while(!record->adoEOF)
	{
		
		if (atoi( (LPCTSTR)(_bstr_t)record->GetCollect("operators") )==2)
		{
			YiDong_Turn = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("turn") );
		}else if (atoi( (LPCTSTR)(_bstr_t)record->GetCollect("operators") )==3)
		{
			LianTong_Turn = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("turn") );
		}else {
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}
		record->MoveNext();
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//����Ƿ��ʽ�ص��ӿ������򷵻�0�����򷵻�-1
int CDB::isSonCard(char *cardno){
	int  ret=0, index=0;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(sql);
	sprintf(sql, "select pe_card_no from pe_card_parent where pe_card_no='%s' and status=0 ", cardno);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		wrLog("�����ݿ��в���ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}
	else
	{		
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}
}

//20121229-����ǰ�ýӿڻ�ȡ�Ƿ�Ϊ���ն˱�ʶ
int CDB::pep_isSonCard(char *termmid)
{
	CConnect cont;
	SOCKET sd;
	char sendbuf[1024] = {0};
	char curdatetime[16] = {0};
	char lenbuf[16] = {0};
	int timeout = 10; //Ĭ�ϳ�ʱʱ��
	int buflen = 0;
	int recvlen;
	char outBuf[2048] = {0};
	char rspcpde[16] = {0};
	int iRet;
	char isson[8] = {0};
	int b_son;

	//����׼��
	CTools::get_termTime(curdatetime);
	sprintf(sendbuf, "FFFF%014s%s%08d%s%s",curdatetime,termmid,buflen,"PEP000H0",termmid);
	buflen = strlen(sendbuf);
	sprintf(lenbuf, "%08d", buflen);
	memcpy(sendbuf + 4 + 22, lenbuf, 8);
	wrLog("���͵�PEP000H���ĵ�ACC:[%s]", sendbuf);

	iRet = cont.conToHost(PE_IP, PE_PORT, &sd);
    if (iRet != 0) {
        wrLog("���ӵ��������������飡%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    
    iRet = cont.sndMsg(sd, sendbuf, buflen, timeout);
    if (iRet != 0) {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

    recvlen = 0;
    iRet = recv(sd, outBuf, 4+22+8, 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8) {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

	int pack_len = atoi(outBuf+4+22) - 4 - 22 - 8 ;//���°��峤��
    int need_rcv = pack_len;
    int has_rcv  = 0;
    while (need_rcv > has_rcv) {
        iRet = recv(sd, outBuf + 4 + 22 + 8 + has_rcv, pack_len - has_rcv, 0);
        if (iRet == -1) {
            if (errno == EAGAIN) {
                continue;
			} else {
                wrLog("�������ͨ�ų��ֹ��� %s:%d" , __FILE__ , __LINE__);
                return -1;
            }
        }
        has_rcv += iRet;
    }
	wrLog("FASACCӦ��PEPOOOH:[%s]", outBuf);
    closesocket(sd);
    recvlen = pack_len;

	//����
	int pos = 42;
	char rspcode[8] = {0};
	char desplen[8] = {0};
	char desp[1024] = {0};
	int tmplen = 0;
	//��Ӧ��
	memcpy(rspcpde, outBuf + pos, 4);
	pos += 4;
	//��������
	memcpy(desplen, outBuf + pos, 2);
	pos += 2;
	tmplen = atoi(desplen);
	//��������
	memcpy(desp, outBuf + pos, tmplen);
	pos += tmplen;
	wrLog("��Ӧ��:[%s]:[%s]", rspcpde, desp);
	if (atoi(rspcpde) == 0) {
		//�ɹ����б�ʾ
		memcpy(isson, outBuf + pos, 1);
		b_son = atoi(isson);
		wrLog("�Ƿ�Ϊ�ӿ�[%s]:[%s]", isson, (b_son == 1 ? "��" : "��"));
		return b_son;
	} 
	return -1;	
}

//����Ƿ�ת�����նˣ����򷵻�0�����򷵻�-1
int CDB::isTransferParentTerm(char *term_id){
    return 0;

	int  ret=0, index=0;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(sql);
	sprintf(sql, "select team from card_mag_user where id='%s' and team=31", term_id);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		wrLog("�����ݿ��в���ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}
	else
	{		
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}
}

//����Ƿ�ת���ӿ������򷵻�0�����򷵻�-1
int CDB::isTransferSonCard(char *p_cardno,char *s_cardno){
    return 0;
	int  ret=0, index=0;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(sql);
	sprintf(sql, "select * from pe_cardno_merchantid where card_no='%s' and merchant_id=(select merchant_id from pe_cardno_merchantid where card_no='%s')", s_cardno,p_cardno);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		wrLog("�����ݿ��в���ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}
	else
	{		
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}
}

//��ȡ�����޶�
int CDB::get_dayAmt(char* ipos_no, char* self_buf, int selfbuflen, char* sys_buf, int sysbuflen, int* shlevel)
{
	CConnect cont;
	SOCKET sd;
	char sendbuf[1024] = {0};
	char curdatetime[16] = {0};
	char lenbuf[16] = {0};
	int timeout = 10; //Ĭ�ϳ�ʱʱ��
	int buflen = 0;
	int recvlen;
	char outBuf[2048] = {0};
	char rspcpde[16] = {0};
	char sh_level[8] = {0};
	int iRet;

	memset(self_buf, 0, selfbuflen); 
	memset(sys_buf, 0, sysbuflen); 
	if (selfbuflen <= 12 || sysbuflen <= 12) {
		wrLog("���뻺����̫��");
		return -1;
	} else if (strlen(ipos_no) != 8) {
		wrLog("�����쳣��get_dayAmt��");
		return -1;
	}

	//����׼��
	CTools::get_termTime(curdatetime);
	sprintf(sendbuf, "FFFF%014s%s%08d%s%s%",curdatetime,ipos_no,buflen,"PEP000G0",ipos_no);
	buflen = strlen(sendbuf);
	sendbuf[buflen] = '\0';
	sprintf(lenbuf, "%08d", buflen);
	memcpy(sendbuf + 4 + 22, lenbuf, 8);
	wrLog("���ͻ�ȡ�ն���Ϣ����[����:%d]��ACC:[%s]", buflen, sendbuf);
	
	//����ACC
	iRet = cont.conToHost(PE_IP, PE_PORT, &sd);
    if (iRet != 0) {
        wrLog("���ӵ��������������飡%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    //��ACC���Զ���������
    iRet = cont.sndMsg(sd, sendbuf, buflen, timeout);
    if (iRet != 0) {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
	//���ձ���ͷ
    recvlen = 0;
    iRet = recv(sd, outBuf, 4+22+8, 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8) {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

	int pack_len = atoi(outBuf+4+22) - 4 - 22 - 8 ;//���°��峤��
    int need_rcv = pack_len;
    int has_rcv  = 0;
	//���ձ�����
    while (need_rcv > has_rcv) {
        iRet = recv(sd, outBuf + 4 + 22 + 8 + has_rcv, pack_len - has_rcv, 0);
        if (iRet == -1) {
            if (errno == EAGAIN) {
                continue;
			} else {
                wrLog("�������ͨ�ų��ֹ��� %s:%d" , __FILE__ , __LINE__);
                return -1;
            }
        }
        has_rcv += iRet ;
    }

	wrLog("CTSI�յ�ACCӦ��:[%s]", outBuf);

    closesocket(sd);
    recvlen = pack_len;
	//����
	int pos = 42;
	char rspcode[8] = {0};
	char desplen[8] = {0};
	char desp[1024] = {0};
	char day_amt[32] = {0};
	char sys_amt[32] = {0};
	int tmplen = 0;
	int level_len = 0 ;
	char level_len_tmp [8] = {0} ;

	//��Ӧ��
	memcpy(rspcpde, outBuf + pos, 4);
	pos += 4;
	//��������
	memcpy(desplen, outBuf + pos, 2);
	pos += 2;
	tmplen = atoi(desplen);
	//��������
	memcpy(desp, outBuf + pos, tmplen);
	pos += tmplen;
	wrLog("��Ӧ��:[%s]:[%s]", rspcpde, desp);
	if (atoi(rspcpde) == 0) {
		//��ȡ�����ս����޶�
		memcpy(self_buf, outBuf + pos, 12);
		pos += 12;
		//��ȡϵͳ�ս����޶�
		memcpy(sys_buf, outBuf + pos, 12);
		pos += 12;
		//��ȡ�̻��ȼ� 150114�ڹ������޸��̻��ȼ���
		//��ȡ�ȼ�����

		memcpy(level_len_tmp, outBuf + pos, 2);
		pos += 2;
		level_len = atoi(level_len_tmp);
		
		memcpy(sh_level, outBuf + pos, level_len);

		if (strcmp(sh_level, "COM") == 0) {
			*shlevel = 1; //1�Ǽ�
		} else if (strcmp(sh_level, "EXP") == 0) {
			*shlevel = 2; //2�Ǽ�
		} else if (strcmp(sh_level, "IMP") == 0) {
			*shlevel = 3; //���Ǽ�
		} else if (strcmp(sh_level, "VIP") == 0) {
			*shlevel = 4; //���Ǽ�
		}
		else if (strcmp(sh_level, "SVIP") == 0) {
			*shlevel = 5; //���Ǽ�
		}
		else if (strcmp(sh_level, "FUND") == 0) {
			*shlevel = 0; //�����̻�
		}
		else //����̻����Ͳ�����������������Ĭ���̻��ȼ�����Ĭ���̻�һ�Ǽ�
		{
			
			*shlevel = 1; //һ�Ǽ�
		
		}

		//����Ǽ���־
		if (*shlevel == 0)
		{
			wrLog("�̻��ȼ�Ϊ: �����̻� [%s]", *shlevel,sh_level);
		}
		else
		{
			wrLog("�̻��ȼ�Ϊ: %d�Ǽ� [%s]", *shlevel,sh_level);

		}
	
		return 0;
	} 
	return -1;	
}

//�����ս����޶�
int CDB::set_dayAmt(char* ipos_no,char* dayamt)
{
	CConnect cont;
	SOCKET sd;
	char sendbuf[2048] = {0};
	char curdatetime[16] = {0};
	char lenbuf[16] = {0};
	int timeout = 10; //Ĭ�ϳ�ʱʱ��
	int buflen = 0;
	int recvlen;
	char outBuf[2048] = {0};
	char rspcpde[16] = {0};
	int iRet;

	//����׼��
	CTools::get_termTime(curdatetime);

	sprintf(sendbuf, "FFFF%014s%s%08d%s%s%012s",curdatetime,ipos_no,buflen,"PEP000F0",ipos_no,dayamt);
	buflen = strlen(sendbuf);
	sprintf(lenbuf, "%08d", buflen);
	memcpy(sendbuf + 4 + 22, lenbuf, 8);
	wrLog("���͵�ACC:[%s]", sendbuf);

	iRet = cont.conToHost(PE_IP, PE_PORT, &sd);
    if (iRet != 0) {
        wrLog("���ӵ��������������飡%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    
    iRet = cont.sndMsg(sd, sendbuf, buflen, timeout);
    if (iRet != 0) {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

    recvlen = 0;
    iRet = recv(sd, outBuf, 4+22+8, 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8) {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

	int pack_len = atoi(outBuf+4+22) - 4 - 22 - 8 ;//���°��峤��
    int need_rcv = pack_len;
    int has_rcv  = 0;
    while (need_rcv > has_rcv) {
        iRet = recv(sd, outBuf + 4 + 22 + 8 + has_rcv, pack_len - has_rcv, 0);
        if (iRet == -1) {
            if (errno == EAGAIN) {
                continue;
			} else {
                wrLog("�������ͨ�ų��ֹ��� %s:%d" , __FILE__ , __LINE__);
                return -1;
            }
        }
        has_rcv += iRet ;
    }
    wrHex("ACCӦ��:", (BYTE*)outBuf, strlen(outBuf));
    closesocket(sd);
    recvlen = pack_len;

	//����
	int pos = 42;
	char rspcode[8] = {0};
	char desplen[8] = {0};
	char desp[1024] = {0};
	int tmplen = 0;
	//��Ӧ��
	memcpy(rspcpde, outBuf + pos, 4);
	pos += 4;
	//�����̶�
	memcpy(desplen, outBuf + pos, 2);
	pos += 2;
	tmplen = atoi(desplen);
	//��������
	memcpy(desp, outBuf + pos, tmplen);
	wrLog("����:[%s]:[%s]", rspcpde, desp);
	if (atoi(rspcpde) == 0) {
		return 0;
	} 
	return -1;	
}

//�汾1
#if 0
int CDB::pep_getSonCard(char* ecard_no , char* term_id , char * busiName,PTSTxnLogs ptxnlogs)
{
    //�������
    char inBuf[1024]    = { 0 };
    char buf_len_buf[9]= { 0 };
    char outBuf[81920] = { 0 };
    char monthHead[32] = { 0 };
    char monthTail[32] = { 0 };
    char datetime[32]  = { 0 };
    int bufLen         = 0;
    int iRet           = 0;
    int timeout        = 10; //Ĭ�ϳ�ʱʱ��
    int i              = 0;
    int q_num          = 100;
    CConnect connector;
    SOCKET sd;

    //1��׼��PEP000A�����
    CTools::getTheMonth(monthHead);
    strcat(monthHead , "01");
    CTools::getDate(monthTail);
    CTools::get_termTime(datetime);

    sprintf(inBuf, "FFFF%14.14s%8.8s%08dPEP000E0%8.8s%32.32s%04d%04d" ,\
        datetime , term_id , bufLen , term_id , ecard_no , 1 , 20);
    bufLen = strlen(inBuf);
    sprintf(buf_len_buf , "%08d" , bufLen);
    memcpy(inBuf + 4 + 22 , buf_len_buf , 8);
    wrLog("CTSI��PEP000E0���ĵ�FAS[%s]" , inBuf);
    //2����������������
    //iRet = connector.conToHost("127.0.0.1" , 20011 , &sd);
    iRet = connector.conToHost(PE_IP , PE_PORT , &sd);
    if (iRet != 0)
    {
        wrLog("���ӵ��������������飡%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    
    iRet = connector.sndMsg(sd , inBuf , bufLen , timeout);
    if (iRet != 0)
    {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

    bufLen = 0;
    iRet = recv(sd , outBuf , 4 + 22 + 8 , 0);
	//wrLog(" iRet = %d  GetLastError = %d" , iRet, GetLastError());
    if (iRet == INVALID_SOCKET || iRet != 4+22+8)
    {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    int pack_len = atoi(outBuf + 4 + 22) - 4 - 22 - 8 ;//���°��峤��
    
    int need_rcv = pack_len;
    int has_rcv  = 0;
    while (need_rcv > has_rcv)
    {
        iRet = recv(sd , outBuf + 4 + 22 + 8 + has_rcv , pack_len - has_rcv , 0);
        if (iRet == -1)
        {
            if (errno == EAGAIN)
                continue;
            else
            {
                wrLog("�������ͨ�ų��ֹ��� %s:%d" , __FILE__ , __LINE__);
                return -1;
            }
        }

        has_rcv += iRet ;
    }
    //wrHex("OUTBUF:" , (BYTE*)outBuf , strlen(outBuf));
	wrLog("CTSI����FAS��PEP000E1����[%d]:[%s]", pack_len+4+22+8,outBuf);
    closesocket(sd);
    bufLen = pack_len;
    assert(bufLen > 0);
    
    //3�������εĽ�����룬ʧ�����˳�
    char errCode[6] = { 0 };
    char errmsg_lenbuf[4] = { 0 } ;
    char errMsg[512] = { 0 };
    int  errLen = 0;
    int  pos    = 0;

    pos += 42 ;
    memcpy(errCode , outBuf + pos , 4);
    pos += 4  ;
    memcpy(errmsg_lenbuf , outBuf + pos , 2);
    pos += 2  ;
    errLen = atoi(errmsg_lenbuf);
    if (strcmp(errCode , "0000") != 0)
    {
        
        assert(errLen > 0);
        memcpy(errMsg , outBuf + pos , errLen);
        wrLog("�ӷ��������ش���[%s]" , errMsg);
        return -1;
    }

    //4����ȡ��¼������
    int count = 0;//��¼���������ݺ��ԡ���ѯ��ʼ��š��͡���ѯ��¼����
    char buf_count[6] = { 0 };
    
    pos += errLen; 
    memcpy(buf_count ,  outBuf + pos , 4);
    pos += 4;

    count = atoi(buf_count);
    assert(count >= 0 && count <= 10000);
    if (count == 0)
    {
        //�ܼ�¼��Ϊ0
        wrLog("δ�ҵ����������ļ�¼ : [%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }

    //ʹ�ò�ѯ���صļ�¼��
    pos += 4;//��������ѯ��ʼ��š�
    memcpy(buf_count , outBuf + pos  , 4);
    pos += 4;
    count = atoi(buf_count);
    if (count == 0)
    {
        //��ҳ��¼��Ϊ0
        wrLog("δ�ҵ����������ļ�¼ : [%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }
    
    //���ü�¼������
    ptxnlogs->colsnum = 8;
    ptxnlogs->colsWidth[0] = 60;
    ptxnlogs->colsWidth[1] = 130;
    ptxnlogs->colsWidth[2] = 80;
    ptxnlogs->colsWidth[3] = 120;
    ptxnlogs->colsWidth[4] = 100;
    ptxnlogs->colsWidth[5] = 80;
    ptxnlogs->colsWidth[6] = 0;
    ptxnlogs->colsWidth[7] = 0;
    ptxnlogs->colsNameLen = 50;
    memcpy(ptxnlogs->colsName, \
        "�ն˺�\x00����\x00��ǰ�˻����\x00�̻�����\x00��ϵ��\x00��ϵ�绰\x00�˵�ID\x00�˵�����", \
        ptxnlogs->colsNameLen);
    strcpy(ptxnlogs->bottom, "��<-����->����ҳ ��Esc������ ��Enter��ȷ�� ��F������excel��");

    memset(ptxnlogs->items , 0 , sizeof(ptxnlogs->items));
    ptxnlogs->itemsLen = 0;
    int n = 0;

    char* ptr = outBuf + pos ;//��һ����¼���ڵ�
    for (i=0; i < count; ++i)
    {
        string card_no, bal, bizname, contact, phone, curtermid;
        char len_buf[4] = {0};
        int len = 0;
        int ptr_offset = 0;//��¼ƫ��

        //����
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        card_no = string(ptr + ptr_offset, len);
		//wrLog("card_no:[%s]", card_no.c_str());
        ptr_offset += len;

        //���
        bal = string(ptr + ptr_offset, 12);
		//wrLog("bal:[%s]",bal.c_str());
        ptr_offset += 12;
        double d_bal = atof(bal.c_str()) / 100.0f; //ע���Է�Ϊ��λ
        char buf_bal[16] = {0};
        sprintf(buf_bal, "%8.2f", d_bal);     //���óɿɶ�����ʽ
        bal = buf_bal;

        //�̻�����
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        bizname = string(ptr + ptr_offset, len);
		//wrLog("busiName:[%s]", bizname.c_str());
        ptr_offset += len;

        //��ϵ��
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        contact = string(ptr + ptr_offset, len);
		//wrLog("contact:[%s]", contact.c_str());
        ptr_offset += len;

        //�绰����ϵ��ʽ
        memcpy(len_buf , ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        phone = string(ptr + ptr_offset, len);
		//wrLog("phone:[%s]",phone.c_str());
        ptr_offset += len;

        //�ն˺�
        curtermid = string(ptr + ptr_offset, 8);
		//wrLog("curtermid:[%s]", curtermid.c_str());
        ptr_offset += 8;

        //�ն˺�\x00����\x00��ǰ�˻����\x00�̻�����\x00��ϵ��\x00��ϵ�绰
        sprintf(ptxnlogs->items + ptxnlogs->itemsLen, "%s%c%s%c%s%c%s%c%s%c%s%c", \
            curtermid.c_str(), 0, card_no.c_str(), 0, bal.c_str(), 0, bizname.c_str(), \
			0, contact.c_str(), 0, phone.c_str(), 0);

        ptxnlogs->itemsLen += curtermid.size() +\
            card_no.size() +\
            bal.size() +\
            bizname.size() +\
            contact.size() +\
            phone.size() + 6; //�����ֽڵġ�000���������ֽڵġ��ո񡻣�6�ֽڵ�0x00
        

		if( ptxnlogs->itemsLen+strlen("684") < MAX_INTERM_BUF )
		{
			strcpy(ptxnlogs->items+ptxnlogs->itemsLen, "684");
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + strlen("684") + 1;
		}
		else
		{
			break;
		}
		
		if( ptxnlogs->itemsLen+ curtermid.size()< MAX_INTERM_BUF )
		{
			sprintf(ptxnlogs->items + ptxnlogs->itemsLen , "%s",curtermid.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + strlen(ptxnlogs->items+ptxnlogs->itemsLen) + 1;
		}
		else
			
		{
			break;
		}

        ptr += ptr_offset;
        ++n;
    }

    char temp[6] = { 0 };
    sprintf(temp, "��%d����ת���ն� ", i);
	strcpy(ptxnlogs->hint, temp);

    return 0;
}

#else
int CDB::pep_getSonCard(char* ecard_no, char* term_id, char* busiName, PTSTxnLogs ptxnlogs)
{
    //�������
    char inBuf[1024]    = { 0 };
    char buf_len_buf[9]= { 0 };
    char outBuf[81920] = { 0 };
    char monthHead[32] = { 0 };
    char monthTail[32] = { 0 };
    char datetime[32]  = { 0 };
    int bufLen         = 0;
    int iRet           = 0;
    int timeout        = 10; //Ĭ�ϳ�ʱʱ��
    int i              = 0;
    int q_num          = 100;
    CConnect connector;
    SOCKET sd;
    char temp[6] = {0};

    //1��׼��PEP000A�����
    CTools::getTheMonth(monthHead);
    strcat(monthHead , "01");
    CTools::getDate(monthTail);
    CTools::get_termTime(datetime);

    sprintf(inBuf, "FFFF%14.14s%8.8s%08dPEP000E0%8.8s%32.32s%04d%04d", datetime, term_id, bufLen, term_id, ecard_no, 1, 20);
    bufLen = strlen(inBuf);
    sprintf(buf_len_buf, "%08d", bufLen);
    memcpy(inBuf + 4 + 22, buf_len_buf, 8);
    wrLog("CTSI��PEP000E0���ĵ�FAS[%s]", inBuf);
    //2��������������
    iRet = connector.conToHost(PE_IP, PE_PORT, &sd);
    if (iRet != 0) {
        wrLog("���ӵ��������������飡%s:%d", __FILE__ , __LINE__);
        return -1;
    }
    
    iRet = connector.sndMsg(sd, inBuf, bufLen, timeout);
    if (iRet != 0) {
        wrLog("�������ݵ���������������%s:%d" , __FILE__ , __LINE__);
        return -1;
    }

    bufLen = 0;
    iRet = recv(sd, outBuf, 4 + 22 + 8, 0);
    if (iRet == INVALID_SOCKET || iRet != 4+22+8) {
        wrLog("�ӷ������������ݳ���%s:%d" , __FILE__ , __LINE__);
        return -1;
    }
    int pack_len = atoi(outBuf + 4 + 22) - 4 - 22 - 8 ;//���°��峤��
    int need_rcv = pack_len;
    int has_rcv = 0;
    while (need_rcv > has_rcv) {
        iRet = recv(sd , outBuf + 4 + 22 + 8 + has_rcv , pack_len - has_rcv , 0);
        if (iRet == -1)
        {
            if (errno == EAGAIN)
                continue;
            else
            {
                wrLog("�������ͨ�ų��ֹ��� %s:%d" , __FILE__ , __LINE__);
                return -1;
            }
        }

        has_rcv += iRet ;
    }
    //wrHex("OUTBUF:" , (BYTE*)outBuf , strlen(outBuf));
	wrLog("CTSI����FAS��PEP000E1����[%d]:[%s]", pack_len+4+22+8,outBuf);
    closesocket(sd);
    bufLen = pack_len;
    assert(bufLen > 0);
    
    //3�������εĽ�����룬ʧ�����˳�
    char errCode[6] = { 0 };
    char errmsg_lenbuf[4] = { 0 } ;
    char errMsg[512] = { 0 };
    int  errLen = 0;
    int  pos    = 0;

    pos += 42 ;
    memcpy(errCode , outBuf + pos , 4);
    pos += 4  ;
    memcpy(errmsg_lenbuf , outBuf + pos , 2);
    pos += 2  ;
    errLen = atoi(errmsg_lenbuf);
    if (strcmp(errCode , "0000") != 0)
    {
        
        assert(errLen > 0);
        memcpy(errMsg , outBuf + pos , errLen);
        wrLog("�ӷ��������ش���[%s]" , errMsg);
        return -1;
    }

    //4����ȡ��¼������
    int count = 0;//��¼���������ݺ��ԡ���ѯ��ʼ��š��͡���ѯ��¼����
    char buf_count[6] = { 0 };
    
    pos += errLen; 
    memcpy(buf_count ,  outBuf + pos , 4);
    pos += 4;

    count = atoi(buf_count);
    assert(count >= 0 && count <= 1000);
    if (count == 0) {
        //�ܼ�¼��Ϊ0
        wrLog("δ�ҵ����������ļ�¼ : [%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }

    //ʹ�ò�ѯ���صļ�¼��
    pos += 4;//��������ѯ��ʼ��š�
    memcpy(buf_count, outBuf + pos, 4);
    pos += 4;
    count = atoi(buf_count);
    if (count == 0)  {
        //��ҳ��¼��Ϊ0
        wrLog("δ�ҵ����������ļ�¼ : [%s:%d]" , __FILE__ , __LINE__);
        return -1;
    }

	wrLog("����ĸ��ת�� -- ��ȡ�ӿ���");	
	ptxnlogs->colsnum = 8;
	ptxnlogs->colsWidth[0] = 60;
	ptxnlogs->colsWidth[1] = 110;
	ptxnlogs->colsWidth[2] = 80;
	ptxnlogs->colsWidth[3] = 120;
	ptxnlogs->colsWidth[4] = 100;
	ptxnlogs->colsWidth[5] = 80;
	ptxnlogs->colsWidth[6] = 0;
	ptxnlogs->colsWidth[7] = 0;
	ptxnlogs->colsNameLen = 83;
	memcpy(ptxnlogs->colsName, "�ն˺�\x00����\x00��ǰ�˻����\x00�̻�����\x00��ϵ��\x00��ϵ�绰\x00�˵�ID\x00�˵�����", ptxnlogs->colsNameLen);
	strcpy(ptxnlogs->bottom, "��<-����->����ҳ ��Esc������ ��F������excel��");

	//��һ����¼���ڵ�
    char* ptr = outBuf + pos;
    for (i = 0; i < count; ++i) {
        string card_no, bal, bizname, contact, phone, curtermid;
        char len_buf[4] = {0};
        int len = 0;
        int ptr_offset = 0;

        //����
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        card_no = string(ptr + ptr_offset, len);
		ptr_offset += len;

        //���
        bal = string(ptr + ptr_offset, 12);
        ptr_offset += 12;
        double d_bal = atof(bal.c_str())/100.0f; //ע���Է�Ϊ��λ
        char buf_bal[16] = {0};
        sprintf(buf_bal, "%.2f", d_bal);     //���óɿɶ�����ʽ
        bal = string(buf_bal);

        //�̻�����
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        bizname = string(ptr + ptr_offset, len);
		if (bizname.length() >= 60) {
			memcpy(busiName, bizname.c_str(), 60);
		} else {
			strcpy(busiName, bizname.c_str());
		}
		//wrLog("busiName:[%s]", bizname.c_str());
        ptr_offset += len;

        //��ϵ��
        memcpy(len_buf, ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        contact = string(ptr + ptr_offset, len);
		//wrLog("contact:[%s]", contact.c_str());
        ptr_offset += len;

        //�绰����ϵ��ʽ
        memcpy(len_buf , ptr + ptr_offset, 2);
        ptr_offset += 2;
        len = atoi(len_buf);
        phone = string(ptr + ptr_offset, len);
		//wrLog("phone:[%s]",phone.c_str());
        ptr_offset += len;
		
        //�ն˺�
        curtermid = string(ptr + ptr_offset, 8);
		//wrLog("curtermid:[%s]", curtermid.c_str());
        ptr_offset += 8;
		//ƫ��������		
        ptr += ptr_offset;


		//�ն˺�
		if (ptxnlogs->itemsLen + curtermid.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, curtermid.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + curtermid.length() + 1;
		} else {
			break;
		}
		//����
		if (ptxnlogs->itemsLen + card_no.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, card_no.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + card_no.length() + 1;
		} else {
			break;
		}
		//���
		if (ptxnlogs->itemsLen + bal.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, bal.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + bal.length() + 1;
		} else {
			break;
		}
		//�̻�����
		if (ptxnlogs->itemsLen + bizname.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, bizname.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + bizname.length() + 1;
		} else {
			break;
		}
		//��ϵ��
		if (ptxnlogs->itemsLen + contact.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, contact.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + contact.length() + 1;
		} else {
			break;
		}
		//��ϵ�绰
		if (ptxnlogs->itemsLen + phone.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, phone.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + phone.length() + 1;
		} else {
			break;
		}
		//�˵�
		if( ptxnlogs->itemsLen+strlen("684") < MAX_INTERM_BUF) {
	   		strcpy(ptxnlogs->items+ptxnlogs->itemsLen, "684");
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + strlen("684") + 1;
		} else {
			break;
		}
		//�ն˺�
		if (ptxnlogs->itemsLen + curtermid.length() < MAX_INTERM_BUF) {
			strcpy(ptxnlogs->items + ptxnlogs->itemsLen, curtermid.c_str());
			ptxnlogs->itemsLen = ptxnlogs->itemsLen + curtermid.length() + 1;
		} else {
			break;
		}
    }

    sprintf(temp, "��%d����ת���ն� ", count);
	strcpy(ptxnlogs->hint, temp);

    return 0;
}
#endif

//����Ƿ�ת���ӿ������򷵻�0�����򷵻�-1 //�ʽ𻮲����ܲ�ѯ�ӿ� ����ʹ��
int CDB::getSonCard(char *p_cardno, PTSTxnLogs ptxnLogs){
	int  ret=0, index=0, i=0;
	char temp[50];
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	bbzero(temp);

	wrLog("����ĸ��ת�� -- ��ȡ�ӿ���");
	
	ptxnLogs->colsnum = 7;
	ptxnLogs->colsWidth[0] = 60;
	ptxnLogs->colsWidth[1] = 110;
	//ptxnLogs->colsWidth[2] = 80;
	ptxnLogs->colsWidth[2] = 120;
	ptxnLogs->colsWidth[3] = 100;
	ptxnLogs->colsWidth[4] = 80;
	ptxnLogs->colsWidth[5] = 0;
	ptxnLogs->colsWidth[6] = 0;
	ptxnLogs->colsNameLen = 83;
	memcpy(ptxnLogs->colsName, "�ն˺�\x00����\x00�̻�����\x00��ϵ��\x00��ϵ�绰\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Esc������ ��F������excel��");

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	bbzero(sql);
	sprintf(sql, "select d.def_term_id def_term_id, a.pe_card_no pe_card_no, c.name name, c.linkman linkman, c.tel tel from pe.pe_card_info a, epos_term b, merchant c, merchant_term d where a.pe_card_no=b.card_no and b.term_id=d.def_term_id and d.mid=c.mid and  a.pe_card_no in ( select card_no from pe_cardno_merchantid where merchant_id in (select merchant_id from ecard.pe_id_merchantid where id='%s') and parent_flag = 0) order by def_term_id", p_cardno);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	
	while(!record->adoEOF)
	{
		
		if((record->GetCollect("def_term_id")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("def_term_id"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("def_term_id")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("pe_card_no")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("pe_card_no"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("pe_card_no")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}


		if((record->GetCollect("name")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("name"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("name")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("linkman")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("linkman"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("linkman")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("tel")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("tel"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("tel")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if( ptxnLogs->itemsLen+strlen("684") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "684");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("684") + 1;
		}
		else
		{
			break;
		}

		if((record->GetCollect("def_term_id")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("def_term_id"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("def_term_id")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
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

		i++;
		record->MoveNext();
	}
	
	bbzero(temp);
	sprintf(temp, "��%d����ת���ն� ", i);
	strcpy(ptxnLogs->hint, temp);
	
	g_adoConnInfo[index].connStat = 0;
	return 0;

}

//����������
int CDB::revAppliChk(char *stan,char *input_amount, char *msg,CGlobal* gbl)
{
	int  ret=0, index=0, amount=0;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char term_id[8+1], date8[8+1], time6[6+1], date19[19+1], sql[1024];
	
	wrLog("CDB��revAppliChk��");
	strcpy(msg, "");	
	bbzero(term_id);

	CTime cNow = CTime::GetCurrentTime();
	CTime cLastDate = CTime::GetCurrentTime() + CTimeSpan(0,8,0,0);
	if( cNow.GetDay()!=1 && cLastDate.GetDay()==1 )//����
	{
		strcat(msg, "ÿ�����8Сʱ�����������\n");
		return -1;
	}
	else
	{
		NULL;
	}

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(sql);
	sprintf(sql, "select pay_no, amount, biz_name, txn_date, txn_time, term_id, rev_status, a.biz_type from txn_log a, biz_group b where stan='%s' and STATUS=0 and RSP_CODE='00' and a.biz_type=b.biz_type(+)", stan);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		wrLog("�����ݿ��в��ҽ���ʧ��sql[%s]",sql);
		strcat(msg, "���Ҹý��׼�¼����\n");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	if(!record->adoEOF)
	{
		if((record->GetCollect("pay_no")).vt!=VT_NULL)
		{
			sprintf(msg+strlen(msg), "��ֵ����: %s\n", (LPCTSTR)(_bstr_t)record->GetCollect("pay_no"));
		}
		else
		{
			strcat(msg, "���Ҹý��׵ĳ�ֵ�������\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}
		if((record->GetCollect("amount")).vt!=VT_NULL)
		{
			amount = atoi((LPCTSTR)(_bstr_t)record->GetCollect("amount"));
			if(amount!=atoi(input_amount)){
				strcat(msg, "��������ñʽ��׵Ľ���\n");
				g_adoConnInfo[index].connStat = 0;
			    return -1;
			}
			if(amount>=0)
			{
				sprintf(msg+strlen(msg), "��ֵ���: %02d.%02dԪ\n", amount/100, amount%100);
			}
			else
			{
				sprintf(msg+strlen(msg), "��ֵ���: -%02d.%02dԪ\n", -amount/100, -amount%100);
			}
		}
		else
		{
			strcat(msg, "���Ҹý��׵Ľ�����\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}

		sprintf(msg+strlen(msg), "\nϵͳ�ο���:%s\n", stan);
		if((record->GetCollect("biz_name")).vt!=VT_NULL)
		{
			sprintf(msg+strlen(msg), "ҵ��: %s\n", (LPCTSTR)(_bstr_t)record->GetCollect("biz_name"));
		}
		else
		{
			strcat(msg, "���Ҹý��׵�ҵ�����Ƴ���\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}
		if((record->GetCollect("txn_date")).vt!=VT_NULL && (record->GetCollect("txn_time")).vt!=VT_NULL)
		{
			bbzero(date8);
			strncpy(date8, (LPCTSTR)(_bstr_t)record->GetCollect("txn_date"), 8);
			bbzero(time6);
			strncpy(time6, (LPCTSTR)(_bstr_t)record->GetCollect("txn_time"), 6);
			bbzero(date19);
			memcpy(date19, date8, 4);
			strcat(date19, "-");
			memcpy(date19+strlen(date19), date8+4, 2);
			strcat(date19, "-");
			memcpy(date19+strlen(date19), date8+6, 2);
			strcat(date19, " ");
			memcpy(date19+strlen(date19), time6, 2);
			strcat(date19, ":");
			memcpy(date19+strlen(date19), time6+2, 2);
			strcat(date19, ":");
			memcpy(date19+strlen(date19), time6+4, 2);
			sprintf(msg+strlen(msg), "��ֵʱ��: %s\n", date19);
		}
		else
		{
			strcat(msg, "���Ҹý��׵ĳ�ֵʱ�����\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}
		
		if((record->GetCollect("term_id")).vt!=VT_NULL)
		{
			sprintf(msg+strlen(msg), "�ն˺�: %s\n", (LPCTSTR)(_bstr_t)record->GetCollect("term_id"));
			strcpy(term_id, (LPCTSTR)(_bstr_t)record->GetCollect("term_id"));
		}
		else
		{
			strcat(msg, "���Ҹý��׵��ն˺ų���\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}

		int nYear=0, nMonth=0, nDay=0, nHour=0, nMin=0, nSec=0;
		char cDateBuf[4+1];
		bbzero(cDateBuf);
		strncpy(cDateBuf, (LPCTSTR)(_bstr_t)record->GetCollect("txn_date"), 4);
		nYear = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, (LPCTSTR)(_bstr_t)record->GetCollect("txn_date")+4, 2);
		nMonth = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, (LPCTSTR)(_bstr_t)record->GetCollect("txn_date")+6, 2);
		nDay = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, (LPCTSTR)(_bstr_t)record->GetCollect("txn_time"), 2);
		nHour = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, (LPCTSTR)(_bstr_t)record->GetCollect("txn_time")+2, 2);
		nMin = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, (LPCTSTR)(_bstr_t)record->GetCollect("txn_time")+4, 2);
		nSec = atoi(cDateBuf);
		if(CTools::chkDate(nYear, nMonth, nDay)==0)
		{
			CTime cTxnTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
			if( cTxnTime < CTime::GetCurrentTime() - CTimeSpan(0,24,0,0) )
			{
				strcat(msg, "����ֻ����24Сʱ���������\n�ѳ�������ʱ��\n");
				g_adoConnInfo[index].connStat = 0;
				return -1;
			}
			else if(cNow.GetDay() == 1 && cTxnTime.GetDay() != 1)
			{
				strcat(msg, "���ײ��ܿ��³���\n�ѳ�������ʱ��\n��������ѯ��821�����߿ͷ�");
				g_adoConnInfo[index].connStat = 0;
				return -1;
			}
			else if (cTxnTime > CTime::GetCurrentTime() - CTimeSpan(0,24,0,0)&&cTxnTime < CTime::GetCurrentTime() - CTimeSpan(0,12,0,0))
			{
				strcat(msg, "���׳���12Сʱ����ͨ�����߿ͷ���Ctrl+H���������\n");
				g_adoConnInfo[index].connStat = 0;
				return -1;
			}
			else
			{
				NULL;
			}
		}
		else
		{
			strcat(msg, "�������ʧ�ܣ��밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}
		
		

		if((record->GetCollect("rev_status")).vt!=VT_NULL)
		{
			if(atoi((LPCTSTR)(_bstr_t)record->GetCollect("rev_status"))!=0)
			{
				strcat(msg, "�����ײ����������\n");
				g_adoConnInfo[index].connStat = 0;
				return -1;
			}
			else
			{
				NULL;
			}
		}
		else
		{
			strcat(msg, "���Ҹý��׵ĳ���״̬����\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}

		if((record->GetCollect("biz_type")).vt!=VT_NULL)
		{
			if(strstr("[0180][0213][0255][0256][0257][0265][0296][0297][0300][0169][0174][0323][0327][0321][0342][0344][0343][0305][0356][0316]", (LPCTSTR)(_bstr_t)record->GetCollect("biz_type"))==NULL)
			{
				strcat(msg, "��ҵ�����������\n");
			}
			else
			{
				NULL;
			}
		}
		else
		{
			strcat(msg, "���Ҹý��׵ĳ���״̬����\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}


		if(gbl->Reversal_count<=0)
		{
			strcat(msg, "���¿��������������Ϊ0\n�������������\n");
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}
		else
		{
			gbl->Reversal_count--;
		}

	}
	else
	{
		wrLog("�����ݿ���û���ҵ��ý���sql[%s]",sql);		
		sprintf(msg+strlen(msg), "ϵͳ�ο���:%s\nû���ҵ��ý��׼�¼\n", stan);		
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�ύ��������
int CDB::revAppli(char *stan, char *msg,CGlobal gbl)
{
	int index,ret;
	_bstr_t vSQL,vSQL_a,vSQL_b,vSQL_c;
	_RecordsetPtr record;
	char sql[512];
	
	wrLog("CDB��revAppli��");
	strcpy(msg, "");

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(sql);
	sprintf(sql,"select * from jf_hhkc_log where stan='%s' and state=4",stan);
	vSQL_a = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL_a, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		bbzero(sql);
		sprintf(sql, "update txn_log set rev_status=1 where stan='%s' and rev_status=0 and biz_type in('0180','0213','0255','0256','0257','0296','0297','0265','0300','0169','0174','0176','0323','0327','0321', '0342', '0344','0343', '0305', '0356', '0316')", stan);
		vSQL = sql;	
		if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
		{
			wrLog("�����ݿ��и��³���ʧ��sql[%s]",sql);
			strcat(msg, "���ݿ����ʧ��\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n");
			g_adoConnInfo[index].connStat = 2;
			return -1;
		}
		bbzero(sql);
		sprintf(sql, "update epos_term set reversal=%d where term_id='%s' ",gbl.Reversal_count,gbl.pos_no);
		vSQL_c = sql;	
		if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL_c) )
		{
			wrLog("�����ݿ��и��³���ʧ��sql[%s]",sql);
			strcat(msg, "���ݿ����ʧ��\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n");
			g_adoConnInfo[index].connStat = 2;
			return -1;
		}
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}
	
	bbzero(sql);
	sprintf(sql, "update jf_hhkc_log set state=5,operator='%s',adddate=to_char(sysdate, 'yyyymmddhh24miss') where stan='%s' and state=4",gbl.pos_no, stan);
	vSQL_b = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL_b) )
	{
		wrLog("�����ݿ��и��³���ʧ��sql[%s]",sql);
		strcat(msg, "���ݿ����ʧ��\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	bbzero(sql);
	sprintf(sql, "update txn_log set rev_status=4 where stan='%s' and rev_status=0 and biz_type in('0180','0213','0255','0256','0257','0296','0297','0265','0300','0169','0174','0176','0323','0327','0321', '0342', '0344','0343','0305', '0356', '0316')", stan);
	vSQL = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и��³���ʧ��sql[%s]",sql);
		strcat(msg, "���ݿ����ʧ��\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	bbzero(sql);
	sprintf(sql, "update epos_term set reversal=%d where term_id='%s' ",gbl.Reversal_count,gbl.pos_no);
	vSQL_c = sql;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL_c) )
	{
		wrLog("�����ݿ��и��³���ʧ��sql[%s]",sql);
		strcat(msg, "���ݿ����ʧ��\n�밴��Ctrl+H����ϵ���߿ͷ����� ���µ�400-80-11888��ѯ\n");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::update_title_flag(CGlobal gbl)
{
	int index;
	_bstr_t vSQL;
	char tmp[1024];
	char bus_card_infoBCD[96+1];
	
	wrLog("��update_DGBus_Txn��");
	index = getConnStat();  //��ȡ��������
	wrLog("��index=%d��", index);
	if( index<0 )
		return -1;
	
	bbzero(bus_card_infoBCD);
	CTools::byte_to_hex((const BYTE*)gbl.bus_card_info,(BYTE*)bus_card_infoBCD,48);
	bbzero(tmp);
	sprintf(tmp, "update epos_term set title_flag=%d where term_id='%s'",gbl.title_flag,gbl.pos_no);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getEposNewMessage()
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getEposNewMessage��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_new_message order by tgroup";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -1;
	}

	if(!new_message.empty())
	{
		new_message.clear();
	}
	
	Epos_new_message new_message_tmp;

	while(!record->adoEOF)
	{		
		memset(&new_message_tmp, 0, sizeof(new_message_tmp));

		if((record->GetCollect("sys_message")).vt!=VT_NULL)
			strcpy(new_message_tmp.sys_new_message, (LPCTSTR)(_bstr_t)record->GetCollect("sys_message") );
		else
			strcpy(new_message_tmp.sys_new_message,"");
		if((record->GetCollect("title_message")).vt!=VT_NULL)
			strcpy(new_message_tmp.title_message, (LPCTSTR)(_bstr_t)record->GetCollect("title_message") );
		else
			strcpy(new_message_tmp.title_message,"");
		new_message_tmp.my_group = atoi( (LPCTSTR)(_bstr_t)record->GetCollect("tgroup") );

		new_message.push_back(new_message_tmp);

		record->MoveNext();
	}
	
	wrLog("��getEposNewMessage�� ������%d��", new_message.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�ж��ն��Ƿ��в���������
//���أ�0, �У�1, �ޣ�����, ����
int CDB::getYctNotFinish(char *term_id, PYCT_DATA pyct_data)
{
	int ret, index;
	char tmp[512];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getYctNotFinish��");

	bbzero(tmp);
	sprintf(tmp, "select stan, to_char(txn_time, 'yyyymmdd') txn_date, to_char(txn_time, 'hh24miss') txn_time, biz_type, status, term_id, term_serial, bank, card_no, amount, yct_serial, pki_card, ticket_no, ticket_amo_bef, ticket_amo_aft, result_message,yct_ack  from yctyp_txn where term_id='%s' and (YCT_ACK =0 or status in ('4','9') ) and rownum=1 order by txn_time  desc ", term_id);
	wrLog("��ѯ���ͨ����������:%s",tmp);
	vSQL = tmp;
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 1;
	}
	if((record->GetCollect("stan")).vt!=VT_NULL)
		strcpy(pyct_data->stan, (LPCTSTR)(_bstr_t)record->GetCollect("stan") );
	else
		NULL;
	if((record->GetCollect("txn_date")).vt!=VT_NULL)
		strcpy(pyct_data->txn_date, (LPCTSTR)(_bstr_t)record->GetCollect("txn_date") );
	else
		NULL;
	if((record->GetCollect("txn_time")).vt!=VT_NULL)
		strcpy(pyct_data->txn_time, (LPCTSTR)(_bstr_t)record->GetCollect("txn_time") );
	else
		NULL;
	if((record->GetCollect("biz_type")).vt!=VT_NULL)
		strcpy(pyct_data->biz_type, (LPCTSTR)(_bstr_t)record->GetCollect("biz_type") );
	else
		NULL;
	if((record->GetCollect("status")).vt!=VT_NULL)
		pyct_data->status = atoi((LPCTSTR)(_bstr_t)record->GetCollect("status"));
	else
		NULL;
	if((record->GetCollect("term_id")).vt!=VT_NULL)
		strcpy(pyct_data->term_id, (LPCTSTR)(_bstr_t)record->GetCollect("term_id") );
	else
		NULL;
	if((record->GetCollect("term_serial")).vt!=VT_NULL)
		strcpy(pyct_data->term_serial, (LPCTSTR)(_bstr_t)record->GetCollect("term_serial") );
	else
		NULL;
	if((record->GetCollect("bank")).vt!=VT_NULL)
		strcpy(pyct_data->bank, (LPCTSTR)(_bstr_t)record->GetCollect("bank") );
	else
		NULL;
	if((record->GetCollect("card_no")).vt!=VT_NULL)
		strcpy(pyct_data->card_no, (LPCTSTR)(_bstr_t)record->GetCollect("card_no") );
	else
		NULL;
	if((record->GetCollect("amount")).vt!=VT_NULL)
		strcpy(pyct_data->amount, (LPCTSTR)(_bstr_t)record->GetCollect("amount") );
	else
		NULL;
	if((record->GetCollect("yct_serial")).vt!=VT_NULL)
		strcpy(pyct_data->yct_serial, (LPCTSTR)(_bstr_t)record->GetCollect("yct_serial") );
	else
		NULL;
	if((record->GetCollect("pki_card")).vt!=VT_NULL)
		strcpy(pyct_data->pki_card, (LPCTSTR)(_bstr_t)record->GetCollect("pki_card") );
	else
		NULL;
	if((record->GetCollect("ticket_no")).vt!=VT_NULL)
		strcpy(pyct_data->ticket_no, (LPCTSTR)(_bstr_t)record->GetCollect("ticket_no") );
	else
		NULL;
	if((record->GetCollect("ticket_amo_bef")).vt!=VT_NULL)
		strcpy(pyct_data->ticket_amo_bef, (LPCTSTR)(_bstr_t)record->GetCollect("ticket_amo_bef") );
	else
		NULL;
	if((record->GetCollect("ticket_amo_aft")).vt!=VT_NULL)
		strcpy(pyct_data->ticket_amo_aft, (LPCTSTR)(_bstr_t)record->GetCollect("ticket_amo_aft") );
	else
		NULL;
	if((record->GetCollect("result_message")).vt!=VT_NULL)
		strcpy(pyct_data->result_message, (LPCTSTR)(_bstr_t)record->GetCollect("result_message") );

	if((record->GetCollect("yct_ack")).vt!=VT_NULL)
		pyct_data->yct_ack=atoi((LPCTSTR)(_bstr_t)record->GetCollect("yct_ack"));
	else
		NULL;
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�������ͨ����
int CDB::updateYctYPData(YCT_DATA yct_data)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[2000];
	char dec_yct_stan[16+1];

	wrLog("��updateYctYPData��");
	bbzero(dec_yct_stan);
	if(YCT_VER==3)
	{
		CTools::HexToDec(yct_data.yct_serial,dec_yct_stan,16);
	}
	else
	{
		memcpy(dec_yct_stan,yct_data.yct_serial,strlen(yct_data.yct_serial));
	}
	
	bbzero(tmp);
	sprintf(tmp, "update yctyp_txn set status=%d, yct_ack=%d", yct_data.status,yct_data.yct_ack);
	strcat(tmp, ", term_id='");
	strcat(tmp, yct_data.term_id);
	strcat(tmp, "', term_serial='");
	strcat(tmp, yct_data.term_serial);
	strcat(tmp, "', bank='");
	strcat(tmp, yct_data.bank);
	strcat(tmp, "', card_no='");
	strcat(tmp, yct_data.card_no);
	strcat(tmp, "', amount='");
	strcat(tmp, yct_data.amount);
	strcat(tmp, "', yct_serial='");
	strcat(tmp, dec_yct_stan);
	strcat(tmp, "', pki_card='");
	strcat(tmp, yct_data.pki_card);
	strcat(tmp, "', ticket_no='");
	strcat(tmp, yct_data.ticket_no);
	strcat(tmp, "', ticket_amo_bef='");
	strcat(tmp, yct_data.ticket_amo_bef);
	strcat(tmp, "', ticket_amo_aft='");
	strcat(tmp, yct_data.ticket_amo_aft);
	strcat(tmp, "', result_message='");
	strcat(tmp, yct_data.result_message);
	strcat(tmp, "' where stan='");
	strcat(tmp, yct_data.stan);
	strcat(tmp, "'");
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	wrLog("updateYctYPData����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�������ͨ�������ݿ�
int CDB::setYctYPData(YCT_DATA yct_data)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	char dec_yct_stan[16+1];

	wrLog("��setYctYPData��");
	bbzero(dec_yct_stan);
	if(YCT_VER==3)
	{
		CTools::HexToDec(yct_data.yct_serial,dec_yct_stan,16);
	}
	else 
	{
		memcpy(dec_yct_stan,yct_data.yct_serial,strlen(yct_data.yct_serial));
	}

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
		
	bbzero(tmp);
	strcpy(tmp, "INSERT INTO YCTYP_txn(stan,txn_time,biz_type,status,term_id,term_serial,bank,card_no,amount,yct_serial,pki_card,ticket_no,ticket_amo_bef,ticket_amo_aft,result_message) VALUES('");
	strcat(tmp, yct_data.stan);
	strcat(tmp, "',to_date('");
	strcat(tmp, yct_data.txn_date);
	strcat(tmp, yct_data.txn_time);
	strcat(tmp, "','yyyymmddhh24miss'),'");
	strcat(tmp, yct_data.biz_type);
	sprintf(tmp+strlen(tmp), "',%d,'", yct_data.status);
	strcat(tmp, yct_data.term_id);
	strcat(tmp, "','");
	strcat(tmp, yct_data.term_serial);
	strcat(tmp, "','");
	strcat(tmp, yct_data.bank);
	strcat(tmp, "','");
	strcat(tmp, yct_data.card_no);
	strcat(tmp, "','");
	strcat(tmp, yct_data.amount);
	strcat(tmp, "','");
	strcat(tmp, dec_yct_stan);
	strcat(tmp, "','");
	strcat(tmp, yct_data.pki_card);
	strcat(tmp, "','");
	strcat(tmp, yct_data.ticket_no);
	strcat(tmp, "','");
	strcat(tmp, yct_data.ticket_amo_bef);
	strcat(tmp, "','");
	strcat(tmp, yct_data.ticket_amo_aft);
	strcat(tmp, "','");
	strcat(tmp, yct_data.result_message);
	strcat(tmp, "')");
	vSQL = tmp;
	
	wrLog("����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

//�������ͨ�������ݿ�
int CDB::setYctYPSignIn(const char *term_id, char *pki_card, char *signin_serial)
{
	int index=-1, iRet=0;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��setYctYPSignIn��");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;	

	//���±��ն˵�PKI��ǩ������
	bbzero(tmp);
	strcpy(tmp, "select 1 from yctyp_signin where term_id='");
	strcat(tmp, term_id);
	strcat(tmp, "'");
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, iRet);	
	if(iRet)
	{
		wrLog("�������ݿ�ʧ��[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{
		bbzero(tmp);
		strcpy(tmp, "update yctyp_signin set term_id='");
		strcat(tmp, term_id);
		strcat(tmp, "',pki_card='");
		strcat(tmp, pki_card);
		strcat(tmp, "',signin_serial='");
		strcat(tmp, signin_serial);
		strcat(tmp, "',signin_time=sysdate where term_id='");
		strcat(tmp, term_id);
		strcat(tmp, "'");
		vSQL = tmp;
	}
	else
	{
		bbzero(tmp);
		strcpy(tmp, "INSERT INTO yctyp_signin(term_id,pki_card,signin_serial,signin_time) VALUES('");
		strcat(tmp, term_id);
		strcat(tmp, "','");
		strcat(tmp, pki_card);
		strcat(tmp, "','");
		strcat(tmp, signin_serial);
		strcat(tmp, "', sysdate");
		strcat(tmp, ")");
		vSQL = tmp;
	}
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}

	//���ͬһ��PKI������ͬ�ն˺ŵ�PKI��ǩ������
	bbzero(tmp);
	strcpy(tmp, "select 1 from yctyp_signin where pki_card='");
	strcat(tmp, pki_card);
	strcat(tmp, "' and term_id<>'");
	strcat(tmp, term_id);
	strcat(tmp, "'");
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, iRet);	
	if(iRet)
	{
		wrLog("�������ݿ�ʧ��[%s]", tmp);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{
		bbzero(tmp);
		strcpy(tmp, "update yctyp_signin set signin_serial='0000000000000000' where pki_card='");
		strcat(tmp, pki_card);
		strcat(tmp, "' and term_id<>'");
		strcat(tmp, term_id);
		strcat(tmp, "'");
		vSQL = tmp;
		if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
		{
			wrLog("�����������ݿ�ʧ��[%s]", tmp);
			g_adoConnInfo[index].connStat = 2;
			return -4;
		}
	}
	else
	{
		NULL;
	}

	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::getAreaBiz()
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getAreaBiz��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from epos_area_biz order by biz_flag,area desc,accarea desc";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!area_biz.empty())
	{
		area_biz.clear();
	}

	Epos_area_biz area_biz_tmp;

	while(!record->adoEOF)
	{
		memset(&area_biz_tmp, 0, sizeof(area_biz_tmp));

		area_biz_tmp.biz_flag = atoi((LPCTSTR)(_bstr_t)record->GetCollect("biz_flag"));
		strcpy(area_biz_tmp.area, (LPCTSTR)(_bstr_t)record->GetCollect("area"));
		strcpy(area_biz_tmp.accarea, (LPCTSTR)(_bstr_t)record->GetCollect("accarea"));
		strcpy(area_biz_tmp.biz_type, (LPCTSTR)(_bstr_t)record->GetCollect("biz_type"));
		area_biz_tmp.status = atoi((LPCTSTR)(_bstr_t)record->GetCollect("status"));
		
		area_biz.push_back(area_biz_tmp);

		record->MoveNext();
	}
	
	wrLog("��getAreaBiz��������%d��", area_biz.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//����0��ʾͨ��
int CDB::chKChinaUnicom(char *bizs, char *phone_no, int amo)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��chKChinaUnicom��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	bbzero(tmp);
	sprintf(tmp, "select to_char(NVL(txn_time, sysdate), 'yyyy-mm-dd hh24:mi:ss') txn_time from epos_txn where txn_time>trunc(sysdate) and mti='0200' and biz_type in('%s') and pay_no='%s' and amount=%d and (trim(rsp_code)='00' or rsp_code is NULL) order by txn_time desc", bizs, phone_no, amo);
	if (TRADE_FLAG==0)
	{
		wrLog("chKChinaUnicom sql:%s",tmp);
	}
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{
		int nYear=0, nMonth=0, nDay=0, nHour=0, nMin=0, nSec=0;
		char cDateBuf[4+1], txntime[19+1];
		bbzero(txntime);
		strncpy(txntime, (LPCTSTR)(_bstr_t)record->GetCollect("txn_time"), 19);
		bbzero(cDateBuf);
		strncpy(cDateBuf, txntime, 4);
		nYear = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, txntime+5, 2);
		nMonth = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, txntime+8, 2);
		nDay = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, txntime+11, 2);
		nHour = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, txntime+14, 2);
		nMin = atoi(cDateBuf);
		bbzero(cDateBuf);
		strncpy(cDateBuf, txntime+17, 2);
		nSec = atoi(cDateBuf);
		if(CTools::chkDate(nYear, nMonth, nDay)==0)
		{
			CTime cTxnTime = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
			if( cTxnTime > CTime::GetCurrentTime() - CTimeSpan(0, 0, FORBID_TIME, 0) )//�����ظ���ֵʱ��
			{
				g_adoConnInfo[index].connStat = 0;
				return 1;
			}
			else
			{
				g_adoConnInfo[index].connStat = 0;
				return 0;
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return 1;
		}
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::deleteBatch(char *batch_value)
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	strcpy(temp,"select * from T_PE_BATCH where batch_value='");
	strcat(temp,batch_value);
	strcat(temp,"'");
	
	wrLog("��deleteBatch��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(!record->adoEOF)
	{		
		if (memcmp((LPCTSTR)(_bstr_t)record->GetCollect("STAT"),"S0P",3)==0)
		{
			g_adoConnInfo[index].connStat = 0;
			return 1;
		}else{
			bbzero(temp);
			strcpy(temp,"delete from T_PE_BATCH_LIST where batch_value='");
			strcat(temp,batch_value);
			strcat(temp,"'");
			if(index == -1)  //û��ȡ�����ݿ�����
			{
				index = getConnStat();  //��ȡ��������
				if( index<0 )
					return -1;
				wrLog("���������ݿ�ͨ�� %d��", index);
			}
			vSQL = temp;
			if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
			{
				wrLog("ɾ�����ݿ�ʧ��");
				wrLog("SQL��䣺[%s]", temp);
				g_adoConnInfo[index].connStat = 2;
				return -4;
			}
			
			bbzero(temp);
			strcpy(temp,"delete from T_PE_BATCH where batch_value='");
			strcat(temp,batch_value);
			strcat(temp,"'");
			if(index == -1)  //û��ȡ�����ݿ�����
			{
				index = getConnStat();  //��ȡ��������
				if( index<0 )
					return -1;
				wrLog("���������ݿ�ͨ�� %d��", index);
			}
			vSQL = temp;
			if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
			{
				wrLog("ɾ�����ݿ�ʧ��");
				wrLog("SQL��䣺[%s]", temp);
				g_adoConnInfo[index].connStat = 2;
				return -4;
			}
		}		
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::checkBatch(char *term_id)
{
	int i, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	strcpy(temp,"select * from T_PE_BATCH where cust_code='");
	strcat(temp,term_id);
	strcat(temp,"'order by BATCH_VALUE desc");

	wrLog("��checkBatch��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	i = 0;
	while(!record->adoEOF)
	{		
		if (memcmp((LPCTSTR)(_bstr_t)record->GetCollect("STAT"),"S0P",3)==0)
		{
			g_adoConnInfo[index].connStat = 0;
			return 1;
		}
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getBatch_value(char *term_id,Batch_Value_Info *batch_info)
{
	int i, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];

	bbzero(temp);
	strcpy(temp,"select * from T_PE_BATCH where cust_code='");
	strcat(temp,term_id);
	strcat(temp,"'order by BATCH_VALUE desc");
	
	wrLog("��getBatch_value��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	i = 0;
	while(!record->adoEOF)
	{
		if (i>=8)
		{
			break;
		}else{
			if((record->GetCollect("BATCH_VALUE")).vt!=VT_NULL)
				strcpy(batch_info[i].batch_value,(LPCTSTR)(_bstr_t)record->GetCollect("BATCH_VALUE"));
			else
				strcpy(batch_info[i].batch_value,"");

			if((record->GetCollect("pay_money")).vt!=VT_NULL)
				strcpy(batch_info[i].pay_money,(LPCTSTR)(_bstr_t)record->GetCollect("pay_money"));
			else
				strcpy(batch_info[i].pay_money,"");

			batch_info[i].pay_count = atoi((LPCTSTR)(_bstr_t)record->GetCollect("COUNTS"));
			record->MoveNext();
			i++;
		}
	}
	g_adoConnInfo[index].connStat = 0;
	return i;
}

int CDB::getBatchList(char *batch_value,char *moneycount,PTSTxnLogs ptxnLogs,int itemcount)
{
	int ret=0, index=-1, i=0, s_commision=0, amo=0, sucnum=0, rev_status=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char  sql[2048];
	char temp[20],amoMny[20];
	
	wrLog("��getBatchList��");

	ptxnLogs->colsnum = 7;
	ptxnLogs->colsWidth[0] = 150;
	ptxnLogs->colsWidth[1] = 110;
	ptxnLogs->colsWidth[2] = 100;
	ptxnLogs->colsWidth[3] = 66;
	ptxnLogs->colsWidth[4] = 66;
	ptxnLogs->colsWidth[5] = 0;
	ptxnLogs->colsWidth[6] = 0;
	ptxnLogs->colsNameLen = 51;
	memcpy(ptxnLogs->colsName, "���κ�\x00���Ѻ���\x00����ҵ��\x00���ѽ��\x00���ѽ��\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	bbzero(sql);
	sprintf(sql,"select obj_code ���Ѻ���,decode(action_code,'0001','����ֱ��','0002','�ƶ�ֱ��','0003','��ֱͨ��') ����ҵ��,pay_money ���ѽ��,decode(stat,'S0P','�ɹ�','S1P','ʧ��','S2P','����','S3P','δ����')���ѽ�� from T_PE_BATCH_LIST where  batch_value='%s'",batch_value);
	vSQL = sql;

	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "���Ѻ��빲%d�� ���ѽ��%ld.%02ldԪ", itemcount, atoi(moneycount)/100, atoi(moneycount)%100);
	sprintf(ptxnLogs->top, "�緢�ֺ��벻��ȷ���������ϴ��ĵ�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");

	for(i=0; !record->adoEOF && i<itemcount; i++)
	{
		strcpy(ptxnLogs->items+ptxnLogs->itemsLen, batch_value);
		ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;

		if((record->GetCollect("���Ѻ���")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("���Ѻ���"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("���Ѻ���")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("����ҵ��")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("����ҵ��"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("����ҵ��")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("����ҵ��")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("���ѽ��"))) < MAX_INTERM_BUF )
			{
				bbzero(temp);
				bbzero(amoMny);
				strcpy(temp,(LPCSTR)(_bstr_t)(record->GetCollect("���ѽ��")));
				CTools::chgToMny(temp,amoMny);
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, amoMny);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("���ѽ��")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("���ѽ��"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("���ѽ��")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if( ptxnLogs->itemsLen+strlen("620") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "620");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("620") + 1;
		}else
		{
			break;
		}
		//�˵����͵�����

		if( ptxnLogs->itemsLen+strlen(batch_value) < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, batch_value);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
		}
		else
		{
			break;
		}

		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

/*
field_group��¼ÿ�����͵ĺ������ʼ�����λ��
gd_field��¼���к����¼
*/
int CDB::getMobileArea()
{
	int  i, ret, index, iind, iind_bak;
	char ind[3];
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	
	bbzero(temp);
	strcpy(temp,"select * from epos_mobile_area order by mobile_field");
	
	wrLog("��epos_mobile_area��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!gd_field.empty())
	{
		gd_field.clear();
	}
	
	memset(&field_group, -1, sizeof(field_group));
	
	Mobile_Field_Area gd_field_tmp;
	
	iind = 0;
	iind_bak = 0;
	i=0;
	while(!record->adoEOF)
	{
		memset(&gd_field_tmp, 0, sizeof(gd_field_tmp));
		
		bbzero(temp);
		bbzero(ind);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("mobile_field"));
		memcpy(ind, temp + 1, 2);
		iind = atoi(ind);
		if(i == 0)
		{
			iind_bak = iind;
		}
		
		if(field_group[iind] == -1)
			field_group[iind] = i;
		
		strcpy(gd_field_tmp.mobile_field, temp);
		gd_field_tmp.field_len = strlen(temp);
		
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("area"));
		strcpy(gd_field_tmp.area, temp);
		
		if(iind != iind_bak)
		{
			if(field_group[iind_bak + 100] == -1)
				field_group[iind_bak + 100] = i;
			iind_bak = iind;
		}
		
		gd_field.push_back(gd_field_tmp);
		
		i++;
		record->MoveNext();
	}
	
	
	if(field_group[iind_bak + 100] == -1)
		field_group[iind_bak + 100] = i;
	
	wrLog("��epos_mobile_area��������%d��",i);
	epos_mobile_totalNum=i;
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//��ѯ���������
//����ֵΪ0 �ǹ㶫���� 1Ϊʡ�����,-1 �ǲ�ѯ�����ú���
int CDB::phone_district_qry(char *phone_no,char *phone_province,char *phone_city,int *mobile_owner)

{

	int i, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	char mobile_field[7+1];
	bbzero(mobile_field);
	memcpy(mobile_field,phone_no,7);
	bbzero(temp);
	sprintf(temp,"select m.area,m.province ,n.mobile_owner from epos_mobile_area m ,epos_mobile n where  substr(m.mobile_field,0,3)=n.mobile_field and  m.mobile_field='%s'",mobile_field);
	vSQL=temp;
	wrLog("sql:%s",temp);
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	i = 0;
	if(!record->adoEOF)
	{		
		if((record->GetCollect("area")).vt!=VT_NULL)
			strcpy(phone_city,(LPCTSTR)(_bstr_t)record->GetCollect("area"));	
		if((record->GetCollect("province")).vt!=VT_NULL)
			strcpy(phone_province,(LPCTSTR)(_bstr_t)record->GetCollect("province"));	
		if((record->GetCollect("mobile_owner")).vt!=VT_NULL)
			*mobile_owner=atoi((LPCTSTR)(_bstr_t)record->GetCollect("mobile_owner")) ;	
	}else
		return -4;

	g_adoConnInfo[index].connStat = 0;
	//����ǹ㶫ʡ���뷵��0 ��������ǹ㶫ʡ���룬����1
	if(memcmp(phone_province,"�㶫",4)==0)
	{
		return 0;

	}
	else
		return 1;

}



//��ѯ�Ƿ��ǹ㶫���룬����Ƿ������ڵ�����
int CDB::is_gd_district(char *city_no,char *city_name)

{
	
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	bbzero(temp);
	bbzero(city_name);
	sprintf(temp,"select t.city_name from gd_district t where t.city_no ='%s'",city_no);
	vSQL=temp;
	wrLog("sql:%s",temp);
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if(!record->adoEOF)
	{		
		if((record->GetCollect("city_name")).vt!=VT_NULL)
			strcpy(city_name,(LPCTSTR)(_bstr_t)record->GetCollect("city_name"));	
	
	}else
		return -4;
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::setIposPromo(char*term_id,char*pro_id,int remain_count)
{
	int index;
	_bstr_t vSQL;
	char tmp[1024];
	
	wrLog("��update_ipos_promo��");
	index = getConnStat();  //��ȡ��������
	wrLog("��index=%d��", index);
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update ipos_promo set remain_account=%d where term_id='%s'and promot_id='%s'",remain_count,term_id,pro_id);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getIposPromo(char*term_id,char*start_date,char*end_date,char*donate_count,char*remain_count,char*all_amount,char*pro_id)
{
	int i, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	strcpy(temp,"select * from ipos_promo where term_id='");
	strcat(temp,term_id);
	strcat(temp,"'order by promot_id desc");
	
	wrLog("��getIposPromo��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	i = 0;
	if(!record->adoEOF)
	{		
		if((record->GetCollect("START_DATE")).vt!=VT_NULL)
			strcpy(start_date,(LPCTSTR)(_bstr_t)record->GetCollect("START_DATE"));	
		if((record->GetCollect("END_DATE")).vt!=VT_NULL)
			strcpy(end_date,(LPCTSTR)(_bstr_t)record->GetCollect("END_DATE"));	
		if((record->GetCollect("donate_account")).vt!=VT_NULL)
			strcpy(donate_count,(LPCTSTR)(_bstr_t)record->GetCollect("donate_account"));	
		if((record->GetCollect("remain_account")).vt!=VT_NULL)
			strcpy(remain_count,(LPCTSTR)(_bstr_t)record->GetCollect("remain_account"));	
		if((record->GetCollect("RECHARGE_AMOUNT")).vt!=VT_NULL)
			strcpy(all_amount,(LPCTSTR)(_bstr_t)record->GetCollect("RECHARGE_AMOUNT"));
		if((record->GetCollect("promot_id")).vt!=VT_NULL)
			strcpy(pro_id,(LPCTSTR)(_bstr_t)record->GetCollect("promot_id"));
	}else
		return -4;
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getTel_part(PHONE_HEAD_NO*tel_part,char*city_no)
{
	int i, ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	sprintf(temp,"select distinct substr(phone,0,7) phone from t_tel_stocks where stat='1' and area_code='%s' group by phone",city_no);
	wrLog("��getTel_part��%s",temp);
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	i = 0;
	while(!record->adoEOF)
	{
		if (i>=8)
		{
			break;
		}else{
			if((record->GetCollect("phone")).vt!=VT_NULL)
				strcpy(tel_part[i].phone_head_no,(LPCTSTR)(_bstr_t)record->GetCollect("phone"));
			else
				strcpy(tel_part[i].phone_head_no,"");
			
			record->MoveNext();
			i++;
		}
	}
	g_adoConnInfo[index].connStat = 0;
	return i;
}

//select_type  1����ͨ���� 2�����Ż�˳�Ż������� 3�����Ż�˳�Ż������ţ�2/3/6/8/9�� 4�������Ż�˫������(2/3/6/8/9)  5��������� 6����ѡ����
int CDB::getTEL_STOCKS(int select_type,PTSTxnLogs ptxnLogs,char*city_no,char*phone_head,char*phone_part)
{
	int ret=0, index=-1, i=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql[2048];
	char type_buff[2];

	bbzero(type_buff);
	sprintf(type_buff,"%d",select_type);
	wrLog("�����зź� -- �ֻ��������ۡ�");

	ptxnLogs->colsnum = 4;
	ptxnLogs->colsWidth[0] = 126;
	ptxnLogs->colsWidth[1] = 66;
	ptxnLogs->colsWidth[2] = 0;
	ptxnLogs->colsWidth[3] = 0;
	ptxnLogs->colsNameLen = 29;
	memcpy(ptxnLogs->colsName, "�ֻ�����\x00�۸�\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	bbzero(sql);
	
	if(select_type == 7)
	{
		strcpy(sql,"select a.PHONE,b.FEE/100 mynum from T_TEL_STOCKS a,T_PHONE_TYPE b where a.PHONE_TYPE=b.PHONE_TYPE and a.TYPE='02'");
	}
	else if (select_type<6)
	{
		strcpy(sql,"select a.PHONE,b.FEE/100 mynum from T_TEL_STOCKS a,T_PHONE_TYPE b where a.PHONE_TYPE=b.PHONE_TYPE and a.PHONE_TYPE='");
		strcat(sql,type_buff);
		strcat(sql,"'");
	}else{
		strcpy(sql,"select a.PHONE,b.FEE/100 mynum from T_TEL_STOCKS a,T_PHONE_TYPE b where a.PHONE_TYPE=b.PHONE_TYPE and PHONE like'");
		strcat(sql,phone_head);
		strcat(sql,phone_part);
		strcat(sql,"'");
	}
	strcat(sql,"and a.area_code='");
	strcat(sql,city_no);
	strcat(sql,"' ");
	strcat(sql,"and a.stat='1' order by a.phone");
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "��ѡ�ֻ�����");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");
	for(i=0; !record->adoEOF && i<500; i++)
	{
		if((record->GetCollect("PHONE")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("PHONE"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("PHONE")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("mynum")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("mynum"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("mynum")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//�˵����͵�����
		if((record->GetCollect("PHONE")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen("662") < MAX_INTERM_BUF )
			{
				if(select_type == 7)
				{
					//��ͨ
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "709");
				}
				else
				{
					//����
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "662");
				}		
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("662") + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//�˵����͵�����
		if((record->GetCollect("PHONE")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("PHONE"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("PHONE")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getUimLog(char*term_id,PTSTxnLogs ptxnLogs, int type)
{
	int ret=0, index=-1, i=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql[512], biztype[10];
	char revmsg[64];
	wrLog("�����зź� -- �׿�����ȷ�ϡ�");

	ptxnLogs->colsnum = 7;
	ptxnLogs->colsWidth[0] = 126;
	ptxnLogs->colsWidth[1] = 80;
	ptxnLogs->colsWidth[2] = 66;
	ptxnLogs->colsWidth[3] = 66;
	ptxnLogs->colsWidth[4] = 250;
	ptxnLogs->colsWidth[5] = 0;
	ptxnLogs->colsWidth[6] = 0;
	ptxnLogs->colsNameLen = 62;
	memcpy(ptxnLogs->colsName, "�ɹ�����\x00ϵͳ�ο���\x00�ɹ�����\x00�ɹ����\x00����״̬\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);

	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	bbzero(sql);
	bbzero(biztype);

	if(type == 0)
	{
		strcpy(biztype, "0329");
	}
	else if(type == 1)
	{
		strcpy(biztype, "0353");
	}
	else
	{
		NULL;
	}

	sprintf(sql,"select TXN_DATE,STAN,NUM,TOTAL_FEE/100 mynum,decode(stat,1,'�յ��С����տ�����ȷ�ϵ���',2,'�����͡�����տ���ȷ�ϵ���',3,'��ȷ��','δ֪') mystat from t_uim_log where term='%s' and biztype = '%s' order by txn_date desc",term_id,biztype);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");
	for(i=0; !record->adoEOF && i<500; i++)
	{
		if((record->GetCollect("TXN_DATE")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("TXN_DATE"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("TXN_DATE")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("STAN")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("STAN"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("STAN")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("num")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("num"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("num")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("mynum")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("mynum"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("mynum")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("mystat")).vt!=VT_NULL)
		{
			bbzero(revmsg);
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("mystat"))) < MAX_INTERM_BUF )
			{
				strcpy(revmsg,(LPCSTR)(_bstr_t)(record->GetCollect("mystat")));
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("mystat")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if( strcmp(revmsg, "�����͡�����տ���ȷ�ϵ���")==0 )
		{
			if(type == 0)
			{
				if( ptxnLogs->itemsLen+strlen("661") < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "661");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("661") + 1;
				}
				else
				{
					break;
				}
			}
			else if(type == 1)
			{
				if( ptxnLogs->itemsLen+strlen("708") < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "708");
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("708") + 1;
				}
				else
				{
					break;
				}
			}
		}else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//�˵����͵�����
		if((record->GetCollect("stan")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("stan"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("stan")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getUimLog_detail(char*stan,char*acount,char*total_fee,char*name,char*telphone,char*address)
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	strcpy(temp,"select * from T_UIM_LOG where STAN='");
	strcat(temp,stan);
	strcat(temp,"'");
	
	wrLog("��getUimLog_detail��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return 2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 3;
	}

	if((record->GetCollect("num")).vt!=VT_NULL)
		strcpy(acount, (LPCSTR)(_bstr_t)(record->GetCollect("num")) );
	else
		strcpy(acount,"");

	if((record->GetCollect("total_fee")).vt!=VT_NULL)
		strcpy(total_fee, (LPCSTR)(_bstr_t)(record->GetCollect("total_fee")) );
	else
		strcpy(total_fee,"");

	if((record->GetCollect("name")).vt!=VT_NULL)
		strcpy(name, (LPCSTR)(_bstr_t)(record->GetCollect("name")) );
	else
		strcpy(name,"");

	if((record->GetCollect("tel")).vt!=VT_NULL)
		strcpy(telphone, (LPCSTR)(_bstr_t)(record->GetCollect("tel")) );
	else
		strcpy(telphone,"");

	if((record->GetCollect("touch_add")).vt!=VT_NULL)
		strcpy(address, (LPCSTR)(_bstr_t)(record->GetCollect("touch_add")) );
	else
		strcpy(address,"");
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getTEL_STOCKS_detail(char*phone,char*fee,char*type)
{
	int  ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	strcpy(temp,"select b.fee,a.PHONE_TYPE from T_TEL_STOCKS a,T_PHONE_TYPE b where  a.PHONE_TYPE=b.PHONE_TYPE and a.phone='");
	strcat(temp,phone);
	strcat(temp,"'and a.stat='1'");
	
	wrLog("��getTEL_STOCKS_detail��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return 2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 3;
	}
	
	if((record->GetCollect("fee")).vt!=VT_NULL)
		strcpy(fee, (LPCSTR)(_bstr_t)(record->GetCollect("fee")) );
	else
		strcpy(fee,"");

	if((record->GetCollect("PHONE_TYPE")).vt!=VT_NULL)
		strcpy(type, (LPCSTR)(_bstr_t)(record->GetCollect("PHONE_TYPE")) );
	else
		strcpy(type,"");

	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//��ȡ�ն˿��ð׿���
int CDB::getTermUim(PTSTxnLogs ptxnLogs, char* phone_no, char* term_id, int type)
{
	int ret=0, index=-1, i=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql[512];
	char revmsg[64], temp[100];
	wrLog("�����зź� -- ѡ����ð׿���");
	
	ptxnLogs->colsnum = 3;
	ptxnLogs->colsWidth[0] = 150;
	ptxnLogs->colsWidth[1] = 0;
	ptxnLogs->colsWidth[2] = 0;
	ptxnLogs->colsNameLen = 22;
	memcpy(ptxnLogs->colsName, "�׿���\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	
	sprintf(sql,"select * from t_uim_stocks where term='%s' and stat = 3 and type = %d", term_id, type);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "��ѡ�׿�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");
	for(i=0; !record->adoEOF && i<500; i++)
	{
		if((record->GetCollect("ICCID")).vt!=VT_NULL)
		{
			bbzero(revmsg);
			strcpy(revmsg, (LPCSTR)(_bstr_t)(record->GetCollect("ICCID")));
			if( ptxnLogs->itemsLen+strlen(revmsg) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, revmsg);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		
		if( ptxnLogs->itemsLen+strlen("710") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "710");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("710") + 1;
		}
		else
		{
			break;
		}
		
		//�˵����͵�����
		bbzero(temp);
		strcat(temp, phone_no);
		strcat(temp, revmsg);
		if( ptxnLogs->itemsLen+strlen(temp) < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
		}
		else
		{
			break;
		}
		
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getGameList(PTSTxnLogs ptxnLogs, const char* sql)
{
	int ret=0, index=-1, i=0;
	char game_name[40], temp[200], len[10], memo[200];
	_bstr_t vSQL;
	_RecordsetPtr record;
		
	ptxnLogs->colsnum = 3;
	ptxnLogs->colsWidth[0] = 620;
	ptxnLogs->colsWidth[1] = 0;
	ptxnLogs->colsWidth[2] = 0;

	ptxnLogs->colsNameLen = 59;
	memcpy(ptxnLogs->colsName, "��� ��Ϸ����                          ��ע\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "��Ϸ�б�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");

	for(i = 0; !record->adoEOF && i < 500; i++)
	{
		if((record->GetCollect("GAME_NAME")).vt!=VT_NULL)
		{
			bbzero(game_name);
			bbzero(memo);
			bbzero(temp);
			bbzero(len);
			strcpy(game_name, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME")));
			
			//����������⣬ȥ����Ӱ��
			int name_len = strlen(game_name);
			for(int ti=0; ti < (34 - name_len)/2; ti++)
			{
				strcat(game_name, "��");//�����Unicode�Ŀհ��ַ����������ֶ���
			}

			sprintf(len, "%d", i+1);
			if((record->GetCollect("REWARD")).vt!=VT_NULL)
			{
				sprintf(memo, "���%s��", (LPCSTR)(_bstr_t)(record->GetCollect("REWARD")));
			}
			if((record->GetCollect("MEMO")).vt!=VT_NULL)
			{
				strcat(memo, (LPCSTR)(_bstr_t)(record->GetCollect("MEMO")));
			}
			sprintf(temp, "%-5s%-34s%-85s", len, game_name, memo);
			if( ptxnLogs->itemsLen+strlen(temp)< MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
	/*	//��ע��Ϣ
		if((record->GetCollect("MEMO")).vt!=VT_NULL && (record->GetCollect("REWARD")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(memo);
			sprintf(memo, "���%s��%s", (LPCSTR)(_bstr_t)(record->GetCollect("REWARD")), (LPCSTR)(_bstr_t)(record->GetCollect("MEMO")));
			sprintf(temp, "%-86s", memo);
			if( ptxnLogs->itemsLen+strlen(temp)< MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
*/
		if( ptxnLogs->itemsLen+strlen("691") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "691");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("691") + 1;
		}
		else
		{
			break;
		}

		bbzero(temp);
		bbzero(game_name);
		strcpy(game_name, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME")));
		sprintf(len, "%02d", strlen(game_name));
		strcat(temp, len);
		strcat(temp, game_name);
		strcat(temp, (LPCSTR)(_bstr_t)(record->GetCollect("SERVER_FLAG")));

		if( ptxnLogs->itemsLen+strlen(temp) < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(temp) + 1;
		}
		else
		{
			break;
		}

		record->MoveNext();
	}
	sprintf(ptxnLogs->hint, "��%d����Ϸ", i);
	g_adoConnInfo[index].connStat = 0;

	return 0;
}


int CDB::getServerList(PTSTxnLogs ptxnLogs, const char* game_name, const char* game_id, const char* autogame_id, const char* game_value, const char* amount)
{
	int ret=0, index=-1, i=0;
	char content[60], temp[40], len[10], sql[512];
	_bstr_t vSQL;
	_RecordsetPtr record;
		
	ptxnLogs->colsnum = 4;
	ptxnLogs->colsWidth[0] = 180;
	ptxnLogs->colsWidth[1] = 180;
	ptxnLogs->colsWidth[2] = 0;
	ptxnLogs->colsWidth[3] = 0;

	ptxnLogs->colsNameLen = 31;
	memcpy(ptxnLogs->colsName, "��Ϸ����\x00������\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;


	sprintf(sql, "select * from xka_game a, xkl_game_server b where a.game_name = b.game_name and a.game_name = '%s' and amount = '%s' ", game_name, amount);


	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "�������б�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");

	for(i = 0; !record->adoEOF && i < 500; i++)
	{
		if((record->GetCollect("GAME_NAME")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		if((record->GetCollect("SERVER_NAME")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("SERVER_NAME"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("SERVER_NAME")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if( ptxnLogs->itemsLen+strlen("693") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "693");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("693") + 1;
		}
		else
		{
			break;
		}
		
		//=====================��������=======================
		bbzero(content);
		bbzero(temp);
		bbzero(len);
		strcpy(temp, game_name);
		sprintf(len, "%02d", strlen(temp));
		strcat(content, len);
		strcat(content, temp);

		bbzero(temp);
		bbzero(len);
		strcpy(temp, game_id);
		sprintf(len, "%02d", strlen(temp));
		strcat(content, len);
		strcat(content, temp);

		bbzero(temp);
		bbzero(len);
		strcpy(temp, autogame_id);
		sprintf(len, "%02d", strlen(temp));
		strcat(content, len);
		strcat(content, temp);
	
		bbzero(temp);
		bbzero(len);
		strcpy(temp, game_value);
		sprintf(len, "%02d", strlen(temp));
		strcat(content, len);
		strcat(content, temp);

		bbzero(temp);
		bbzero(len);
		strcpy(temp, amount);
		sprintf(len, "%02d", strlen(temp));
		strcat(content, len);
		strcat(content, temp);

		if((record->GetCollect("SERVER_VALUE")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("SERVER_VALUE")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		else
		{
			strcat(content, "00");
		}
		if((record->GetCollect("SERVER_NAME")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("SERVER_NAME")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		else
		{
			strcat(content, "00");
		}

		if( ptxnLogs->itemsLen+strlen(content) < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, content);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(content) + 1;
		}
		else
		{
			break;
		}

		record->MoveNext();
	}

	g_adoConnInfo[index].connStat = 0;

	return 0;
}


int CDB::getMoneyList(PTSTxnLogs ptxnLogs, const char* sql)
{
	int ret=0, index=-1, i=0;
	char content[100], temp[40], len[10];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	ptxnLogs->colsnum = 4;
	ptxnLogs->colsWidth[0] = 180;
	ptxnLogs->colsWidth[1] = 180;
	ptxnLogs->colsWidth[2] = 0;
	ptxnLogs->colsWidth[3] = 0;

	ptxnLogs->colsNameLen = 37;
	memcpy(ptxnLogs->colsName, "��Ϸ����\x00��ֵ���(Ԫ)\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "��ֵ����б�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");

	for(i = 0; !record->adoEOF && i < 500; i++)
	{
		if((record->GetCollect("GAME_NAME")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		if((record->GetCollect("AMOUNT")).vt!=VT_NULL)
		{
			bbzero(temp);
			memcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("AMOUNT")), strlen((LPCSTR)(_bstr_t)(record->GetCollect("AMOUNT"))) - 2);
			if( ptxnLogs->itemsLen+strlen(temp) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if( ptxnLogs->itemsLen+strlen("692") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "692");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("692") + 1;
		}
		else
		{
			break;
		}
	
		//------------------------------��������------------------------------------------
		bbzero(content);
		if((record->GetCollect("GAME_NAME")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_NAME")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		if((record->GetCollect("GAME_ID")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_ID")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		if((record->GetCollect("AUTOGAME_ID")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("AUTOGAME_ID")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		if((record->GetCollect("GAME_VALUE")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("GAME_VALUE")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		if((record->GetCollect("AMOUNT")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(len);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("AMOUNT")));
			sprintf(len, "%02d", strlen(temp));
			strcat(content, len);
			strcat(content, temp);
		}
		if((record->GetCollect("SERVER_FLAG")).vt!=VT_NULL)
		{
			bbzero(temp);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("SERVER_FLAG")));
			strcat(content, temp);
		}
		if( ptxnLogs->itemsLen+strlen(content) < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, content);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(content) + 1;
		}
		else
		{
			break;
		}

		record->MoveNext();
	}

	g_adoConnInfo[index].connStat = 0;

	return 0;
}


int CDB::getTrafficCode(PTSTxnLogs ptxnLogs)
{
	int ret=0, index=-1, i=0;
	char sql[512], temp[10], fee[13];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	ptxnLogs->colsnum = 5;
	ptxnLogs->colsWidth[0] = 130;
	ptxnLogs->colsWidth[1] = 80;
	ptxnLogs->colsWidth[2] = 80;
	ptxnLogs->colsWidth[3] = 0;
	ptxnLogs->colsWidth[4] = 0;
	
	ptxnLogs->colsNameLen = 46;
	memcpy(ptxnLogs->colsName, "Υ�³���\x00��������\x00�������(Ԫ)\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	strcpy(sql, "select * from traffic_area_code order by area_name");
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	sprintf(ptxnLogs->hint, "");
	sprintf(ptxnLogs->top, "Υ�µ����б�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");
	
	for(i = 0; !record->adoEOF && i < 500; i++)
	{
		if((record->GetCollect("area_name")).vt!=VT_NULL)
		{
	
			if( ptxnLogs->itemsLen+ strlen((LPCSTR)(_bstr_t)(record->GetCollect("area_name")))< MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("area_name")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		if((record->GetCollect("area_id")).vt!=VT_NULL)
		{
			bbzero(temp);
			sprintf(temp, "%02s", (LPCSTR)(_bstr_t)(record->GetCollect("area_id")));
			if( ptxnLogs->itemsLen+strlen(temp) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		if((record->GetCollect("fee")).vt!=VT_NULL)
		{
			bbzero(fee);
			sprintf(fee, "%d", atoi((LPCSTR)(_bstr_t)(record->GetCollect("fee")))/100);
			if( ptxnLogs->itemsLen+ strlen(fee)< MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, fee);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		
		
		if( ptxnLogs->itemsLen+strlen("684") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "684");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("684") + 1;
		}
		else
		{
			break;
		}

		if( ptxnLogs->itemsLen+strlen(temp) < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
		}
		else
		{
			break;
		}
			
			record->MoveNext();
	}
	
	sprintf(ptxnLogs->hint, "��%d����ѡ����" , i);
	
	g_adoConnInfo[index].connStat = 0;

	return 0;
}

int CDB::getAreaName(int area_id, char* area_name)
{
	int index = -1, ret;
	char sql[512];
	_bstr_t vSQL;
	_RecordsetPtr record;

	sprintf(sql, "select * from traffic_area_code where area_id = %d", area_id);
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if((record->GetCollect("area_name")).vt!=VT_NULL)
	{
		strcpy(area_name, (LPCSTR)(_bstr_t)(record->GetCollect("area_name")));
	}

	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::getFailMessage()
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("��getFailMessage��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select * from biz_rsp order by rsp_id";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!fail_message.empty())
	{
		fail_message.clear();
	}

	Fail_Message fail_message_tmp;

	while(!record->adoEOF)
	{
		memset(&fail_message_tmp, 0, sizeof(fail_message_tmp));

		//��ȡ��Ӧ��ID
		strcpy(fail_message_tmp.rsp_id, (LPCTSTR)(_bstr_t)record->GetCollect("rsp_id"));
		//��ȡparty_code
		if((record->GetCollect("party_code")).vt!=VT_NULL)
		{
			strcpy(fail_message_tmp.party_code, (LPCTSTR)(_bstr_t)record->GetCollect("party_code"));
		}
		else
		{
			strcpy(fail_message_tmp.party_code, " ");
		}
		//��ȡ39����Ӧ��
		strcpy(fail_message_tmp.bit39_rsp, (LPCTSTR)(_bstr_t)record->GetCollect("bit39_rsp"));
		//��ȡ62����Ӧ��
		if((record->GetCollect("bit62_rsp")).vt!=VT_NULL)
		{
			strcpy(fail_message_tmp.bit62_rsp, (LPCTSTR)(_bstr_t)record->GetCollect("bit62_rsp"));
		}
		else
		{
			strcpy(fail_message_tmp.bit62_rsp, " ");
		}
		//��ȡ��Ӧ������Ϣ
		if((record->GetCollect("busi_desc")).vt!=VT_NULL)
		{
			strcpy(fail_message_tmp.busi_desc, (LPCTSTR)(_bstr_t)record->GetCollect("busi_desc"));
		}
		else
		{
			strcpy(fail_message_tmp.busi_desc, " ");
		}

		fail_message.push_back(fail_message_tmp);
	
		record->MoveNext();
	}

	wrLog("��getFailMessage�� ������%d��", fail_message.size());

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getBusStop(PTSTxnLogs ptxnLogs, char* area, char* scont)
{
	int ret=0, index=-1, i=0;
	char sql[512], temp[30], info[100], name[20], memo[201];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	ptxnLogs->colsnum = 3;
	ptxnLogs->colsWidth[0] = 600;
	ptxnLogs->colsWidth[1] = 0;
	ptxnLogs->colsWidth[2] = 0;
	
	ptxnLogs->colsNameLen = 48;
	memcpy(ptxnLogs->colsName, "��վ����                    ��ע\x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	
	index = getConnStat();  //��ȡ��������
	
	if(index < 0)
		return -1;
	
	sprintf(sql, "select * from coach_ticket_bus_stop where bus_stop_area = '%s' order by bus_stop_id", area);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	strcpy(ptxnLogs->top, "��վ�б�");
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");
	
	for(i = 0; !record->adoEOF && i < 500; i++)
	{
		bbzero(name);
		if((record->GetCollect("bus_stop_name")).vt!=VT_NULL)
		{
			strcpy(name, (LPCSTR)(_bstr_t)(record->GetCollect("bus_stop_name")));
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -1;
		}

		if((record->GetCollect("bus_stop_memo")).vt!=VT_NULL)
		{
			bbzero(memo);
			strcpy(memo, (LPCSTR)(_bstr_t)(record->GetCollect("bus_stop_memo")));	
		}
		else
		{
			bbzero(memo);
			strcpy(memo, "���µ�����н�ͨ��������96900��ȡ�˿���վ��ϸ��Ϣ");
		}

		bbzero(temp);
		sprintf(temp, "%-28s%s", name, memo);
		if( ptxnLogs->itemsLen+ strlen(temp)< MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, temp);
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
		}
		else
		{
			break;
		}
		
		if( ptxnLogs->itemsLen+strlen("713") < MAX_INTERM_BUF )
		{
			strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "713");
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("713") + 1;
		}
		else
		{
			break;
		}
		
		if((record->GetCollect("bus_stop_id")).vt!=VT_NULL)
		{
			bbzero(temp);
			bbzero(info);
			strcpy(temp, (LPCSTR)(_bstr_t)(record->GetCollect("bus_stop_id")));
			strcat(info, scont);
			strcat(info, temp);
			if( ptxnLogs->itemsLen+ strlen(info)< MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, info);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
		
		record->MoveNext();
	}
	
	sprintf(ptxnLogs->hint, "��%d����ѡ��վ" , i - 1);
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}


//����ֱ����ҵ������������
int CDB::update_input_times(char *term_id, int input_times){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("�����º������������");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update epos_term set input_times='%d' where term_id = '%s'", input_times, term_id);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и����ն�״̬ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//����HID�豸��ʶ
int CDB::update_HID_type(char *term_id, int HID_TYPE){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("�����¶�ݸ�����������豸��ʶ��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update epos_term set HID_TYPE='%d' where term_id = '%s'", HID_TYPE, term_id);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и����ն�״̬ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//���·�Ʊ����
int CDB::update_print_paper(char *term_id, int print_paper){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("�����·�Ʊֽ���͡�");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update epos_term set print_paper='%d' where term_id = '%s'", print_paper, term_id);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и����ն�״̬ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//���·�Ʊ����
int CDB::update_commission_free(char *term_id, int commission_free){
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("��������������Ѵ�����");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update epos_term set commission_free='%d' where term_id = '%s'", commission_free, term_id);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и����ն�״̬ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getPosAttr()
{
	int index, ret;
	char name_tmp[20], value_tmp[30];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("����ȡ������Ϣ��");

	index = getConnStat();
	if(index < 0)
	{
		return -1;
	}
	vSQL = "select * from t_pos_attr";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	
	while(!record->adoEOF)
	{
		bbzero(name_tmp);
		strcpy(name_tmp, (LPCSTR)(_bstr_t)(record->GetCollect("ATTR_NAME")));
		bbzero(value_tmp);
		strcpy(value_tmp, (LPCSTR)(_bstr_t)(record->GetCollect("ATTR_VALUE")));

		if(!strcmp(name_tmp, "CS_GBL_URL"))
		{
			bbzero(CS_GBL_URL);
			strcpy(CS_GBL_URL, value_tmp);
		}
		if(!strcmp(name_tmp, "CS_DCN_URL"))
		{
			bbzero(CS_DCN_URL);
			strcpy(CS_DCN_URL, value_tmp);
		}
		
		record->MoveNext();
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getRealNum(char *orig_num, char *real_num,char * username)
{
	int index, ret;
	char tmp[300];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	wrLog("����ȡ��Ҫת��������Ϣ��");
	
	index = getConnStat();
	if(index < 0)
	{
		return -1;
	}
	sprintf(tmp, "select * from t_num_chg_list where upper(orig_num)=upper('%s')", orig_num);
	vSQL = tmp;	
	wrLog("tmp:%s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if((record->GetCollect("real_num")).vt!=VT_NULL)
		
	{

	bbzero(real_num);
	strcpy(real_num, (LPCSTR)(_bstr_t)(record->GetCollect("real_num")));

	}
	else
	{

		wrLog("û���ҵ����ڵ绰����");
		g_adoConnInfo[index].connStat = 0;
		return -1;
	
	}

	if(record->GetCollect("user_name").vt!=VT_NULL)
	{
		bbzero(username);
		strcpy(username,(LPCTSTR)(_bstr_t)record->GetCollect("user_name"));
	}

	else

	{
		bbzero(username);
		strcpy(username,"");
		
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
}



//E���ʽ��ʲ�ѯ
int  CDB::getYctTxn(char *term_id, char *yct_date, PTSTxnLogs ptxnLogs)
{
	int ret=0, index=-1, i=0, s_commision=0, sucnum=0, rev_status=0, itemNum=0;
	float amo=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql[2048],fee[12+1];
	CTime cTxnTime;

	wrLog("�����ͨ�ճɹ����׻���-- ���ݿ��ѯ��");

	ptxnLogs->colsnum = 6;
	ptxnLogs->colsWidth[0] = 60;
	ptxnLogs->colsWidth[1] = 60;
	ptxnLogs->colsWidth[2] = 80;
	ptxnLogs->colsWidth[3] = 100;
	ptxnLogs->colsWidth[4] = 56;
	ptxnLogs->colsWidth[5] = 56;
	ptxnLogs->colsNameLen = 68;
	memcpy(ptxnLogs->colsName, "��������\x00����ʱ��\x00��������\x00���׺���\x00���׽��\x00���׽��", ptxnLogs->colsNameLen);
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Esc������ ��F������excel��");

	index = getConnStat();  //��ȡ��������

	if( index<0 )
		return -1;

	//��ȡ����
	bbzero(sql);
	sprintf(sql, "select count(*) cnt, NVL(sum(to_number(amount)),0) sucamo from yctyp_txn where status in (1,2,3,7,0) and term_id = '%s' and to_char(txn_time,'YYYYMMDD') = '%s'", term_id, yct_date);
	vSQL = sql;
	wrLog("%s",sql);
	recordcount = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordcount->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((recordcount->GetCollect("cnt")).vt!=VT_NULL)
		{
			itemNum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("cnt"));
			if(  itemNum>MAX_TXN_LOG || itemNum<=0)
			{
				g_adoConnInfo[index].connStat = 0;
				return -4;
			}
			else
			{
				if((recordcount->GetCollect("sucamo")).vt!=VT_NULL)
				{
					amo = atof((LPCTSTR)(_bstr_t)recordcount->GetCollect("sucamo"))/100;
				}
				else
				{
					amo = 0;
				}
				sprintf(ptxnLogs->hint, "���ɹ�%d��/%.2fԪ", itemNum, amo);
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -4;
		}
	}
    
	bbzero(sql);
	sprintf(sql, "select to_char(a.txn_time,'YYYYMMDD') yct_date, to_char(a.txn_time,'hh24miss') yct_time, b.biz_name yct_name, a.ticket_no yct_card, to_number(a.amount) yct_amount from yctyp_txn a, biz_group b where a.biz_type = b.biz_type and a.status in (1,2,3,7,0) and a.term_id = '%s' and to_char(a.txn_time,'YYYYMMDD') = '%s'", term_id, yct_date);
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��������ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��������ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	for(i=0; !record->adoEOF && i<itemNum; i++)
	{
		if((record->GetCollect("yct_date")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("yct_date"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("yct_date")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("yct_time")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("yct_time"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("yct_time")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("yct_name")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("yct_name"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("yct_name")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("yct_card")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("yct_card"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("yct_card")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		if((record->GetCollect("yct_amount")).vt!=VT_NULL)
		{
			bbzero(fee);
			sprintf(fee, "%.2f", atof((LPCSTR)(_bstr_t)(record->GetCollect("yct_amount")))/100);
			if( ptxnLogs->itemsLen+ strlen(fee)< MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, fee);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}


		strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "�ɹ�");
		ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
		
		record->MoveNext();
	}
	g_adoConnInfo[index].connStat = 0;

	return 0;
}

//д�����ͨ�Ϳ
int CDB::setYctBill(char *date, char *normal_amount, char *plus_amount)
{
	int index, ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[2300], seq[10+1];
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("���������ݿ�ͨ�� %d��", index);
	vSQL = "select lpad(YCT_BILL_SEQ.nextval, 10,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	wrLog("���кţ�[%s]", seq);

	bbzero(tmp);
	sprintf(tmp, "insert into yct_bill(bill_seq, bill_date, bill_amo, bill_amo_plus) values('%s', to_date('%s','YYYYMMDD'), '%s', '%s')", seq, date, normal_amount, plus_amount);
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��в����ش�ӡ����ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//н��н����Ա���뽻�׼�¼
int CDB::setXjxTxn(CGlobal gbl)
{  
    int index,ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[10000];
	char mti_code[4+1];
	char seq[10+1];
   
	wrLog("��set_XJX_Txn��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	wrLog("��index=%d��", index);
	
	bbzero(mti_code);
	strcpy(mti_code,"0200");

	vSQL = "select lpad(xjx_seq_id.nextval, 10,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	wrLog("���кţ�[%s]", seq);

	bbzero(tmp);
	sprintf(tmp, "INSERT INTO XJX_TXN\
               (insert_term_id, term_serial, xjx_seq, mti,\
			     bank, out_account, status, \
				 amount, xjx_biz, in_account, abstract, payee_info,payer_info)\
		        VALUES('%s', '%s', '%s', '%s',\
				       '%s', '%s', '%s',\
					   '%s', '%s', '%s', '%s', '%s', '%s')",\
		 gbl.pos_no, gbl.pos_serial, seq, mti_code, \
		 gbl.bank, gbl.out_account,XJX_NOT_CHECKED,\
		 gbl.amount,gbl.fee_type, gbl.in_account, gbl.abstract,gbl.payee_info,gbl.payer_info); 

	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;

	return 0;
}


//н��н���ײ�ѯ
//flag: 0 - �ɹ����ף�1 - ���н��ף�2 - ʧ�ܽ���
int  CDB::getXjxTxn(int flag,char *from_date,char *to_date, PTSTxnLogs ptxnLogs)
{
	int ret=0, index=-1, i=0, s_commision=0, sucnum=0, rev_status=0, itemNum=0;
	float amo=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql1[2048],sql2[2048];
	char amount[12+1];
	char amo_temp[12+1];
	CTime cTxnTime;
	
	//sql1 ���ڵõ���¼������sql2 ���ڻ�ȡ��¼��
	bbzero(sql1);
	bbzero(sql2);
	switch (flag)
	{
	case 0:
		wrLog("��н��н�ɹ����ײ�ѯ��");
		sprintf(sql1, "select count(*) cnt\
										   from xjx_txn \
										   where txn_datetime >= to_date('%s','yyyy-MM-DD hh24:mi:ss') \
										   and txn_datetime <= to_date('%s','YYYY-MM-DD hh24:mi:ss') and status = '0'",\
                from_date, to_date);
		sprintf(sql2, "select * from xjx_txn \
											where txn_datetime >= to_date('%s','yyyy-MM-DD hh24:mi:ss') \
											and txn_datetime <= to_date('%s','YYYY-MM-DD hh24:mi:ss') \
											and status = '0' order by txn_datetime",
                from_date, to_date);
		break;
	case 1:
		wrLog("��н��н���н��ײ�ѯ��");
		sprintf(sql1, "select count(*) cnt\
										   from xjx_txn \
										   where txn_datetime >= to_date('%s','yyyy-MM-DD hh24:mi:ss') \
										   and txn_datetime <= to_date('%s','YYYY-MM-DD hh24:mi:ss')",\
                from_date, to_date);
		sprintf(sql2, "select * from xjx_txn \
							where txn_datetime >= to_date('%s','yyyy-MM-DD hh24:mi:ss') \
							and txn_datetime <= to_date('%s','YYYY-MM-DD hh24:mi:ss') \
							 order by txn_datetime",
                from_date, to_date);
		break;
	case 2:
		wrLog("��н��нʧ�ܽ��ײ�ѯ��");
		sprintf(sql1, "select count(*) cnt\
										   from xjx_txn \
										   where txn_datetime >= to_date('%s','yyyy-MM-DD hh24:mi:ss') \
										   and txn_datetime <= to_date('%s','YYYY-MM-DD hh24:mi:ss') and status = '1'",\
                from_date, to_date);
		sprintf(sql2, "select * from xjx_txn \
								where txn_datetime >= to_date('%s','yyyy-MM-DD hh24:mi:ss') \
								and txn_datetime <= to_date('%s','YYYY-MM-DD hh24:mi:ss') \
								and status = '1' order by txn_datetime",
                from_date, to_date);
		break;
	default:
		wrLog("��н��н���ײ�ѯ��");
		break;
	}

	
	ptxnLogs->colsnum = 8;
	ptxnLogs->colsWidth[0] = 60;
	ptxnLogs->colsWidth[1] = 60;
	ptxnLogs->colsWidth[2] = 100;
	ptxnLogs->colsWidth[3] = 80;
	ptxnLogs->colsWidth[4] = 80;
	ptxnLogs->colsWidth[5] = 80;
	ptxnLogs->colsWidth[6] = 80;
	ptxnLogs->colsWidth[7] = 80;
	ptxnLogs->colsNameLen = 68;
	memcpy(ptxnLogs->colsName, "���\x00��ˮ��\x00�ύʱ��\x00�ۿ\x00�տ\x00���\x00ժҪ\x00��ǰ״̬\x00", ptxnLogs->colsNameLen);
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter���鿴���� ��Esc������ ��F������excel��");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	vSQL = sql1;
	wrLog("%s",sql1);
	recordcount = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordcount->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((recordcount->GetCollect("cnt")).vt!=VT_NULL)
		{
			itemNum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("cnt"));
			//wrLog("�鵽�ļ�¼����%s",itemNum);
			if(  itemNum>MAX_TXN_LOG || itemNum<=0)
			{
				g_adoConnInfo[index].connStat = 0;
				return -4;
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -4;
		}
	}

	
	vSQL = sql2;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ�����׼�¼sql[%s]",sql2);
	if(ret)
	{
		wrLog("�����ݿ���ȡ�����׼�¼ʧ��sql[%s]",sql2);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	
	for(i=0; !record->adoEOF && i<itemNum; i++)
	{
		//����ֶ�
		if((record->GetCollect("xjx_seq")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("xjx_seq"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("xjx_seq")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//������ˮ��
		if((record->GetCollect("stan")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("stan"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("stan")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
        
		//�ύʱ��
		if((record->GetCollect("txn_datetime")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���
		if((record->GetCollect("payer_info")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("payer_info"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payer_info")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//�տ
		if((record->GetCollect("payee_info")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("payee_info"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payee_info")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���
		if((record->GetCollect("amount")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("amount"))) < MAX_INTERM_BUF )
			{
				bbzero(amount);
				bbzero(amo_temp);
				strcpy(amo_temp,(LPCSTR)(_bstr_t)(record->GetCollect("amount")));
				CTools::chgToMny(amo_temp,amount,1);
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, amount);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//ժҪ
		if((record->GetCollect("abstract")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("abstract"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("abstract")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//��ǰ״̬
		if((record->GetCollect("status")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("status"))) < MAX_INTERM_BUF )
			{
				char state[1];
				strcpy(state,(LPCSTR)(_bstr_t)(record->GetCollect("status")));
				switch (atoi(state))
				{
				case 0:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "���׳ɹ�");
					break;
				case 1:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "����ʧ��");
					break;
				case 2:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "δ���");
					break;
				case 3:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "��ͨ���������");
					break;
				case 4:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "������˲�ͨ��");
					break;
				case 5:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "��ͨ���������");
					break;
				case 6:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "������˲�ͨ��");
					break;
				case 7:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "���׾ܾ������׽���������˽�");
					break;
				default:
					break;
				}
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		record->MoveNext();
	}
    
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::getUnreviewedXjxTxn(PTSTxnLogs ptxnLogs)
{
    int ret=0, index=-1, i=0, s_commision=0, sucnum=0, rev_status=0, itemNum=0;
	float amo=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql[2048];
	char amount[20];//12λ���֣���һ���㣬�ټ�һ����Ԫ����
	char amo_temp[12+1];
	CTime cTxnTime;
	
	wrLog("����ȡδ��˵�н��н�ʽ𻮲���¼��");
	
	ptxnLogs->colsnum = 7;
	ptxnLogs->colsWidth[0] = 60;
	ptxnLogs->colsWidth[1] = 100;
	ptxnLogs->colsWidth[2] = 120;
	ptxnLogs->colsWidth[3] = 100;
	ptxnLogs->colsWidth[4] = 80;
	ptxnLogs->colsWidth[5] = 100;
	ptxnLogs->colsWidth[6] = 60;
	ptxnLogs->colsNameLen = 68;
	memcpy(ptxnLogs->colsName, "���\x00��ˮ��\x00�ύʱ��\x00�տ��Ϣ\x00���\x00ժҪ\x00��ǰ״̬\x00", ptxnLogs->colsNameLen);
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter��������� ��Esc������");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	bbzero(sql);
	strcpy(sql, "select count(*) cnt from xjx_txn where status = '0'");
	vSQL = sql;
	wrLog("%s",sql);
	recordcount = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordcount->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((recordcount->GetCollect("cnt")).vt!=VT_NULL)
		{
			itemNum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("cnt"));
			//wrLog("�鵽�ļ�¼����%s",itemNum);
			if(  itemNum>MAX_TXN_LOG || itemNum<=0)
			{
				g_adoConnInfo[index].connStat = 0;
				return -4;
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -4;
		}
	}

	
	bbzero(sql);
	strcpy(sql, "select xjx_seq,stan,txn_datetime,payee_info, amount money, abstract, status \
				from xjx_txn where status = '0' order by txn_datetime");
	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ��δ��˵Ľ��׼�¼sql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��δ��˵Ľ��׼�¼ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	for(i=0; !record->adoEOF && i<itemNum; i++)
	{
		//����ֶ�
		if((record->GetCollect("xjx_seq")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("xjx_seq"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("xjx_seq")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//������ˮ��
		if((record->GetCollect("stan")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("stan"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("stan")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
        
		//�ύʱ��
		if((record->GetCollect("txn_datetime")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//�տ
		if((record->GetCollect("payee_info")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("payee_info"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payee_info")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���
		if((record->GetCollect("money")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("money"))) < MAX_INTERM_BUF )
			{
				bbzero(amount);
				bbzero(amo_temp);
				strcpy(amo_temp,(LPCSTR)(_bstr_t)(record->GetCollect("money")));
				CTools::chgToMny(amo_temp,amount,1);
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, amount);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//ժҪ
		if((record->GetCollect("abstract")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("abstract"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("abstract")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//��ǰ״̬
		if((record->GetCollect("status")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("status"))) < MAX_INTERM_BUF )
			{
				char state[1];
				strcpy(state,(LPCSTR)(_bstr_t)(record->GetCollect("status")));
				switch (atoi(state))
				{
				case 0:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "δ���");
					break;
				case 1:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "�����");
					break;
				case 2:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "���׳ɹ�");
					break;
				case 3:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "����ʧ��");
					break;
				default:
					break;
				}
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		record->MoveNext();
	}
    
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//���½��׼�¼״̬
//iStatus:  0 - δ���
//          1 - �����
//          2 - ���׳ɹ�
//          3 - ����ʧ��
//XjxSeq: н��н���׼�¼���
int CDB::updateXjxTxnStatus(int iStatus,char* XjxSeq)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[300];
	
	wrLog("������н��н���׼�¼״̬��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update xjx_txn set status='%d' where xjx_seq = '%s'", iStatus, XjxSeq);
	vSQL = tmp;	
	wrLog("sql[%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и��½���״̬ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//��ȡδ��˽��׼�¼������recordset
int CDB::getXjxUnreviewedRecordSet(int &conn_index, _RecordsetPtr &recordset)
{
	int ret=0, index=-1, i=0;
	float amo=0;
	_bstr_t vSQL;  
	char sql[2048];
    
	wrLog("�������ݿ���ȡ��δ��˵Ľ��׼�¼��");

	index = getConnStat();  //��ȡ��������
	conn_index = index;
	if( index<0 )
		return -1;
    
	bbzero(sql);
	strcpy(sql, "select xjx_seq,term_serial,stan,txn_datetime,payee_info, amount, abstract zhaiyao, status,bank,out_account,in_account \
		from xjx_txn where status = '0' order by txn_datetime");
	vSQL = sql;
	recordset = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("sql[%s]",sql);
	wrLog("ret[%d]",ret);
	if(ret)
	{
		wrLog("�����ݿ���ȡ��δ��˵Ľ��׼�¼ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordset->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
  
	//g_adoConnInfo[index].connStat = 0;
	
	//������ϸ��������
	return 0;
}

int CDB::getXjxInaccount(char* XjxSeq,char *inaccount)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("����ȡн��н���׼�¼ת���˺š�");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "select in_account from xjx_txn where xjx_seq = '%s'",XjxSeq);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((record->GetCollect("in_account")).vt!=VT_NULL)
		{
			bbzero(inaccount);
			strcpy(inaccount,(LPCSTR)(_bstr_t)(record->GetCollect("in_account")));
		}
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::updXjxStanRspcode(char* XjxSeq,char* rspcode,char *stan)
{
	int index;
	_bstr_t vSQL;
	char tmp[300];
	
	wrLog("������н��н���׼�¼ϵͳ�ο��š�");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	bbzero(tmp);
	sprintf(tmp, "update xjx_txn set stan = '%s', rsp_code = '%s' where xjx_seq = '%s'",stan,rspcode,XjxSeq);
	vSQL = tmp;	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����ݿ��и��½���ϵͳ�ο���ʧ��sql[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::closeXjxDBConn(int &index)
{
	if (g_adoConnInfo[index].connStat != 0)
	{
		g_adoConnInfo[index].connStat = 0;
	}
	return 0;
}

//У������
//iusertype:  0 - ����Ա
//            1 - ¼��Ա
//            2 - ���Ա
//            3 - �������Ա
//term_id: н��н�ն˺�
int CDB::xjx_get_password(int iusertype, char *term_id, char *password)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];

	switch(iusertype)
	{
	case 0:
		wrLog("��У��н��н����Ա���롿");
		break;
	case 1:
		wrLog("��У��н��н¼��Ա¼�����롿");
		break;
	case 2:
	case 3:
		wrLog("��У��н��н���Ա������롿");
		break;
	default:
		wrLog("��У�����롿");
		break;
	}
	
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "select password from xjx_user where term_id = '%s'",term_id);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((record->GetCollect("password")).vt!=VT_NULL)
		{
			bbzero(password);
			strcpy(password,(LPCSTR)(_bstr_t)(record->GetCollect("password")));
		}
	}
	g_adoConnInfo[index].connStat = 0;

	return 0;
}

//������˽��
int CDB::xjx_set_checkamount(int ilevel,char *amount)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];

	switch(ilevel)
	{
	case 2:
		wrLog("���ö�����˽������");
		break;
	case 3:
		wrLog("����������˽������");
		break;
	default:
		wrLog("�������˵ȼ�");
		break;
	}
   
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update xjx_check_cfg set check_amount = '%s', recently_upd_date = sysdate where check_level = '%d'",amount,ilevel);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("������˽��ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

//����Ա������
int CDB::xjx_update_staff(int ilevel,char *staff_id)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];

	wrLog("xjx_update_staff");

	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update xjx_user set user_id = '%s', recently_upd_date = sysdate where user_type = '%d'",staff_id,ilevel);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����Ա������ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

//����˺��Ƿ����
int CDB::xjx_check_account(char *account,char *account_name,char *account_bank)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];

	wrLog("xjx_check_account");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "select * from xjx_account where account_num = '%s'",account);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret < 0)
	{
		g_adoConnInfo[index].connStat = 2;
		return ret;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 2;
	}

	if((record->GetCollect("account_name")).vt!=VT_NULL)
	{
		bbzero(account_name);
		strcpy(account_name,(LPCSTR)(_bstr_t)(record->GetCollect("account_name")));
	}

	if((record->GetCollect("account_bank")).vt!=VT_NULL)
	{
		bbzero(account_bank);
		strcpy(account_bank,(LPCSTR)(_bstr_t)(record->GetCollect("account_bank")));
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//¼���˺�
int CDB::xjx_add_account(char *account,char *acc_name,char *acc_bank)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("xjx_add_account");

	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "insert into xjx_account values ('%s','%s','%s')",account,acc_name,acc_bank);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����˺�ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}

//���û����˺�
//flag: 0 - �����˺ţ�1 - �����˺�
int CDB::xjx_set_account(int flag,char *account)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];

	wrLog("xjx_set_account");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);

	switch(flag)
	{
	case 0:
		sprintf(sql, "update xjx_account_cfg set out_acc_num = '%s' where row_id = 1",account);
		break;
	case 1:
		sprintf(sql, "update xjx_account_cfg set in_acc_num = '%s' where row_id = 1",account);
		break;
	default:
		break;
	}
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("���»����˺�����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

//������ò�ѯ
//flag: 0 - ������ˣ�1 - �������
int CDB::xjx_get_checkconfig(int flag, char *checker_id,char *amount,char *term_num)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	int user_type = 0;

	switch(flag)
	{
	case 0:
		user_type = 2;
	    wrLog("������������ò�ѯ��");
		break;
	case 1:
		user_type = 3;
		wrLog("������������ò�ѯ��");
		break;
	default:
		break;
	}
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "select user_id, check_amount,term_id from (select * from xjx_user a join xjx_check_cfg b on b.check_level = a.user_type) where user_type = %d",user_type);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret < 0)
	{
		g_adoConnInfo[index].connStat = 2;
		return ret;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 2;
	}
	
	if((record->GetCollect("user_id")).vt!=VT_NULL)
	{	
		bbzero(checker_id);
		strcpy(checker_id,(LPCSTR)(_bstr_t)(record->GetCollect("user_id")));
	}
	
	if((record->GetCollect("check_amount")).vt!=VT_NULL)
	{
		bbzero(amount);
		strcpy(amount,(LPCSTR)(_bstr_t)(record->GetCollect("check_amount")));
	}
	if((record->GetCollect("term_id")).vt!=VT_NULL)
	{
		bbzero(term_num);
		strcpy(term_num,(LPCSTR)(_bstr_t)(record->GetCollect("term_id")));
	}
	
	g_adoConnInfo[index].connStat = 0;

	return 0;
}

//�����˺����ò�ѯ
//flag: 0 - �����˺ţ�1 - �����˺�
int CDB::xjx_get_accountconfig(int flag, char *acc_num,char *acc_name, char *acc_bank)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	bbzero(sql);
	switch(flag)
	{
	case 0:
		wrLog("�������˺����ò�ѯ��");
		sprintf(sql, "select b.account_num, b.account_name, b.account_bank\
						from xjx_account_cfg a, xjx_account b\
						where a.row_id = 1\
						and a.out_acc_num = b.account_num");
		break;
	case 1:
		wrLog("�������˺����ò�ѯ��");
		sprintf(sql, "select b.account_num, b.account_name, b.account_bank\
						from xjx_account_cfg a, xjx_account b\
						where a.row_id = 1\
						and a.in_acc_num = b.account_num");
		break;
	default:
		break;
	}
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret < 0)
	{
		g_adoConnInfo[index].connStat = 2;
		return ret;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 2;
	}
	
	if((record->GetCollect("account_num")).vt!=VT_NULL)
	{	
		bbzero(acc_num);
		strcpy(acc_num,(LPCSTR)(_bstr_t)(record->GetCollect("account_num")));
	}
	
	if((record->GetCollect("account_name")).vt!=VT_NULL)
	{
		bbzero(acc_name);
		strcpy(acc_name,(LPCSTR)(_bstr_t)(record->GetCollect("account_name")));
	}
	
	if((record->GetCollect("account_bank")).vt!=VT_NULL)
	{
		bbzero(acc_bank);
		strcpy(acc_bank,(LPCSTR)(_bstr_t)(record->GetCollect("account_bank")));
	}

	g_adoConnInfo[index].connStat = 0;

	return 0;
}


//�޸�����
int CDB::xjx_upd_password(int iusertype, char *new_pwd)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("xjx_upd_password");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update xjx_user set password = '%s' where user_type = %d",new_pwd,iusertype);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("��������ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}

//�ý����������ȡ���׼�¼
int CDB::xjx_gettxnfromSeq(CGlobal *gbl)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("xjx_gettxnfromSeq");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "select * from xjx_txn where xjx_seq = '%s'",gbl->xjx_seq);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret < 0)
	{
		g_adoConnInfo[index].connStat = 2;
		return ret;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 2;
	}

	bbzero(gbl->xjx_stan);
	if((record->GetCollect("stan")).vt!=VT_NULL)
	{
		strcpy(gbl->xjx_stan,(LPCSTR)(_bstr_t)(record->GetCollect("stan")));
	}
	
	if((record->GetCollect("status")).vt!=VT_NULL)
	{
		char state[2+1];
		bbzero(state);
		bbzero(gbl->status_info);
		strcpy(state,(LPCSTR)(_bstr_t)(record->GetCollect("status")));
		switch (atoi(state))
		{
		case 0:
			strcpy(gbl->status_info, "���׳ɹ�");
			break;
		case 1:
			strcpy(gbl->status_info, "����ʧ��");
			break;
		case 2:
			strcpy(gbl->status_info, "δ���");
			break;
		case 3:
			strcpy(gbl->status_info, "��ͨ���������");
			break;
		case 4:
			strcpy(gbl->status_info, "������˲�ͨ��");
			break;
		case 5:
			strcpy(gbl->status_info, "��ͨ���������");
			break;
		case 6:
			strcpy(gbl->status_info, "������˲�ͨ��");
			break;
		case 7:
			strcpy(gbl->status_info, "���׾ܾ������׽���������˽�");
			break;
		default:
			break;
		}
	}

	if((record->GetCollect("txn_datetime")).vt!=VT_NULL)
	{
		bbzero(gbl->txn_datetime);
		strcpy(gbl->txn_datetime,(LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime")));
	}

	if((record->GetCollect("amount")).vt!=VT_NULL)
	{
		bbzero(gbl->amount);
		strcpy(gbl->amount,(LPCSTR)(_bstr_t)(record->GetCollect("amount")));
	}

	if((record->GetCollect("abstract")).vt!=VT_NULL)
	{
		bbzero(gbl->abstract);
		strcpy(gbl->abstract,(LPCSTR)(_bstr_t)(record->GetCollect("abstract")));
	}

	bbzero(gbl->out_account);
	if((record->GetCollect("out_account")).vt!=VT_NULL)
	{
		strcpy(gbl->out_account,(LPCSTR)(_bstr_t)(record->GetCollect("out_account")));
	}

	bbzero(gbl->payer_info);
	if((record->GetCollect("payer_info")).vt!=VT_NULL)
	{
		strcpy(gbl->payer_info,(LPCSTR)(_bstr_t)(record->GetCollect("payer_info")));
	}

	bbzero(gbl->in_account);
	if((record->GetCollect("in_account")).vt!=VT_NULL)
	{
		strcpy(gbl->in_account,(LPCSTR)(_bstr_t)(record->GetCollect("in_account")));
	}

	bbzero(gbl->payee_info);
	if((record->GetCollect("payee_info")).vt!=VT_NULL)
	{
		strcpy(gbl->payee_info,(LPCSTR)(_bstr_t)(record->GetCollect("payee_info")));
	}

	bbzero(gbl->xjx_rspcode);
	if((record->GetCollect("rsp_code")).vt!=VT_NULL)
	{
		strcpy(gbl->xjx_rspcode,(LPCSTR)(_bstr_t)(record->GetCollect("rsp_code")));
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//�����ն˺Ż�ȡ��˵ȼ�
int CDB::xjx_get_CheckLevel(char *term_id,char *level)
{
	int index;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("xjx_get_CheckLevel");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "select user_type from xjx_user where term_id = '%s'",term_id);
	vSQL = sql;
	wrLog("sql[%s]",sql);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("ret[%d]",ret);
	if(ret < 0)
	{
		g_adoConnInfo[index].connStat = 2;
		return ret;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 2;
	}
	
	if((record->GetCollect("user_type")).vt!=VT_NULL)
	{
		bbzero(level);
		strcpy(level,(LPCSTR)(_bstr_t)(record->GetCollect("user_type")));
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//������˵ȼ���ȡҪ��˵Ľ��׼�¼
int CDB::xjx_get_UncheckTxnLogs(int level,PTSTxnLogs ptxnLogs)
{
	int ret=0, index=-1, i=0, s_commision=0, sucnum=0, rev_status=0, itemNum=0;
	float amo=0;
	_bstr_t vSQL;
	_RecordsetPtr recordcount, record;
	char sql0[2048],sql1[2048],sql2[2048];
	char amount[12+1];
	char amo_temp[12+1];
	CTime cTxnTime;
	
	//sql0 ���ڵõ���¼������sql1 ���ڻ�ȡ��¼��,sql2����ɾ����¼
	bbzero(sql0);
	bbzero(sql1);
	bbzero(sql2);

	switch(level)
	{
	case 2:
		wrLog("����ȡδ����ҽ��С�ڵ���������˽�����޵Ľ��׼�¼��");
		sprintf(sql1, "select *\
			from xjx_txn\
			where status = '2'\
			and amount <=\
			(select check_amount from xjx_check_cfg where check_level = 3)\
		order by txn_datetime");

		sprintf(sql0, " select count(*) cnt\
											 from xjx_txn\
											 where status = '2'\
											 and amount <=\
											 (select check_amount from xjx_check_cfg where check_level = 3)\
		order by txn_datetime");
		break;
	case 3:
		wrLog("����ȡ��ͨ��������˵Ľ��׼�¼��");
		sprintf(sql1, "select *\
			from xjx_txn\
			where amount between\
			(select check_amount from xjx_check_cfg where check_level = 2) and\
			(select check_amount from xjx_check_cfg where check_level = 3)\
			and status = '3'\
			order by txn_datetime");

		sprintf(sql0, " select count(*) cnt\
				from xjx_txn\
				where amount between\
				(select check_amount from xjx_check_cfg where check_level = 2) and\
				(select check_amount from xjx_check_cfg where check_level = 3)\
				and status = '3'\
			order by txn_datetime");

		//������δ����ҽ�����������˽��Ľ��׼�¼���½���״̬Ϊ���׾ܾ�
		sprintf(sql2, "update xjx_txn\
									   set status = '7'\
									   where amount >\
									   (select check_amount from xjx_check_cfg where check_level = 3)");
		wrLog("%s",sql2);
		vSQL = sql2;
		if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
		{
			wrLog("����ʧ��sql[%s]",sql2);
			g_adoConnInfo[index].connStat = 2;
			return -1;
		}
		break;
	default:
		break;
	}


	ptxnLogs->colsnum = 8;
	ptxnLogs->colsWidth[0] = 60;
	ptxnLogs->colsWidth[1] = 60;
	ptxnLogs->colsWidth[2] = 100;
	ptxnLogs->colsWidth[3] = 80;
	ptxnLogs->colsWidth[4] = 80;
	ptxnLogs->colsWidth[5] = 80;
	ptxnLogs->colsWidth[6] = 80;
	ptxnLogs->colsWidth[7] = 80;
	ptxnLogs->colsNameLen = 68;
	memcpy(ptxnLogs->colsName, "���\x00��ˮ��\x00�ύʱ��\x00�ۿ\x00�տ\x00���\x00ժҪ\x00��ǰ״̬\x00", ptxnLogs->colsNameLen);
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter��������� ��Esc������");

	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	vSQL = sql0;
	wrLog("%s",sql0);
	recordcount = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(recordcount->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	else
	{
		if((recordcount->GetCollect("cnt")).vt!=VT_NULL)
		{
			itemNum = atoi((LPCTSTR)(_bstr_t)recordcount->GetCollect("cnt"));
			wrLog("�鵽�ļ�¼����%d",itemNum);
			if(  itemNum>MAX_TXN_LOG || itemNum<=0)
			{
				g_adoConnInfo[index].connStat = 0;
				return -4;
			}
		}
		else
		{
			g_adoConnInfo[index].connStat = 0;
			return -4;
		}
	}
	
	vSQL = sql1;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ������˵Ľ��׼�¼sql[%s]",sql1);
	if(ret)
	{
		wrLog("�����ݿ���ȡ������˵Ľ��׼�¼ʧ��sql[%s]",sql1);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	for(i=0; !record->adoEOF && i<itemNum; i++)
	{
		//����ֶ�
		if((record->GetCollect("xjx_seq")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("xjx_seq"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("xjx_seq")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//������ˮ��
		if((record->GetCollect("stan")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("stan"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("stan")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}
        
		//�ύʱ��
		if((record->GetCollect("txn_datetime")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("txn_datetime")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���
		if((record->GetCollect("payer_info")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("payer_info"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payer_info")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//�տ
		if((record->GetCollect("payee_info")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("payee_info"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("payee_info")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//���
		if((record->GetCollect("amount")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("amount"))) < MAX_INTERM_BUF )
			{
				bbzero(amount);
				bbzero(amo_temp);
				strcpy(amo_temp,(LPCSTR)(_bstr_t)(record->GetCollect("amount")));
				CTools::chgToMny(amo_temp,amount,1);
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, amount);
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//ժҪ
		if((record->GetCollect("abstract")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("abstract"))) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, (LPCSTR)(_bstr_t)(record->GetCollect("abstract")));
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		//��ǰ״̬
		if((record->GetCollect("status")).vt!=VT_NULL)
		{
			if( ptxnLogs->itemsLen+strlen((LPCSTR)(_bstr_t)(record->GetCollect("status"))) < MAX_INTERM_BUF )
			{
				char state[1];
				strcpy(state,(LPCSTR)(_bstr_t)(record->GetCollect("status")));
				switch (atoi(state))
				{
				case 0:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "���׳ɹ�");
					break;
				case 1:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "����ʧ��");
					break;
				case 2:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "δ���");
					break;
				case 3:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "��ͨ���������");
					break;
				case 4:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "������˲�ͨ��");
					break;
				case 5:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "��ͨ���������");
					break;
				case 6:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "������˲�ͨ��");
					break;
				case 7:
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "���׾ܾ������׽���������˽�");
					break;
				default:
					break;
				}
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(ptxnLogs->items+ptxnLogs->itemsLen) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			ptxnLogs->itemsLen = ptxnLogs->itemsLen + 1;
		}

		record->MoveNext();
	}
    
	g_adoConnInfo[index].connStat = 0;
	return 0;
}


//������˽���״̬�����Ա�ն˺ţ���ͨ������
int CDB::xjx_update_checkstatus(char *xjx_seq,char *level,char *status,char *term_id,char *reason)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("xjx_update_checkstatus");
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	switch(atoi(status))
	{
		case 0:
		case 1:
			if (atoi(level) == 2)
			{
                sprintf(sql, "update xjx_txn set status = '%s',LLcheck_term_id = '%s' where xjx_seq = '%s'",status,term_id,xjx_seq);
			}
			if (atoi(level) == 3)
			{
				sprintf(sql, "update xjx_txn set status = '%s',LLLcheck_term_id = '%s' where xjx_seq = '%s'",status,term_id,xjx_seq);
			}
			break;
		case 3:
		case 5:
			sprintf(sql, "update xjx_txn set status = '%s',LLcheck_term_id = '%s' where xjx_seq = '%s'",status,term_id,xjx_seq);
			break;	
		case 4:
		case 6:
			sprintf(sql, "update xjx_txn set status = '%s',LLLcheck_term_id = '%s',not_pass_reason = '%s' where xjx_seq = '%s'",status,term_id,reason,xjx_seq);
			break;
		default:
			break;
	}
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("���¼�¼ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::get_Last_Biz(unsigned char& status , unsigned char* stan)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	unsigned char byteTmp[64] = { 0 };

	wrLog("��get_Last_Biz��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	vSQL = "select t.status , t.stan from (select * from yctyp_txn t order by t.txn_time desc) t where rownum=1";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	status = record->GetCollect("STATUS");
	memcpy(byteTmp , (LPCTSTR)(_bstr_t)record->GetCollect("STAN"),12);
	CTools::hex_to_byte(byteTmp , stan , 12);
	return 0;
}

int CDB::insetYctYPtxnDATA(YCT_DATA *yct_data,char *stan,int charge_type)
{
    int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	char dec_stan[12+1];
	char dec_yctstan[16+1];
	
	wrLog("��insetYctYPtxnDATA��");
	
	bbzero(dec_yctstan);
	bbzero(dec_stan);
	CTools::HexToDec(stan,dec_stan,12);
	CTools::HexToDec(yct_data->yct_serial ,dec_yctstan,16);
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(tmp);
	sprintf(tmp,"INSERT INTO YCTYP_TXN(stan,txn_time,biz_type,status,term_id,term_serial,bank,card_no,\
		amount,yct_serial,pki_card,ticket_no,ticket_amo_bef,ticket_amo_aft,result_message,charge_type)\
		VALUES('%s',sysdate,'%s','%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%d')",\
		dec_stan,yct_data->biz_type,yct_data->status,yct_data->term_id,yct_data->term_serial,\
		yct_data->bank,yct_data->card_no,yct_data->amount,dec_yctstan,yct_data->pki_card,\
		yct_data->ticket_no,yct_data->ticket_amo_bef,\
		yct_data->ticket_amo_aft,yct_data->result_message,charge_type);
	
	vSQL = tmp;
	
	wrLog("����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::updateYctuntxn(YCT_DATA *yct_data,char* stan)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��updateYctuntxn��");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(tmp);
	sprintf(tmp,"UPDATE YCTYP_TXN SET status='%d',ticket_amo_aft='%s' where stan='%s'",yct_data->status,yct_data->ticket_amo_aft,stan);
	vSQL = tmp;
	
	wrLog("����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

//����ʱ����YCTYP_TXN ��result_message�ֶΣ�����Ϊ�����ı���
int CDB::updateYctRollback(YCT_DATA *yct_data, char *message)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��updateYctuntxn��");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(tmp);
	sprintf(tmp,"UPDATE YCTYP_TXN SET result_message='%s' where stan='%s'",message,yct_data->yct_stan);
	vSQL = tmp;
	
	wrLog("����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

int CDB::updateYctResultMsg(YCT_DATA *yct_data, char *message)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��updateYctResultMsg��");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(tmp);
	sprintf(tmp,"UPDATE YCTYP_TXN SET result_message='%s' where stan='%s'",message,yct_data->yct_stan);
	vSQL = tmp;
	
	wrLog("����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
}

	//�������ͨ�������ݿ�״̬λ
	//stat: 0����ֵ�ɹ���ȷ��
	//      1����ֵ�ɹ�δȷ��
	//      2����ֵʧ���ѳ���
	//      3����ֵʧ��δ����
	//      4����������������
	//      5����ֵʧ�ܳ���ʧ��
	//      6����ֵ�ɹ�ȷ��ʧ��
int CDB::setYctThirdStatus(int stat, char *seq)
{
		int index;
		char tmp[512];
		_bstr_t vSQL;
		_RecordsetPtr record;
		
		index = getConnStat();  //��ȡ��������
		if( index<0 )
			return -1;
		bbzero(tmp);
		sprintf(tmp, "UPDATE yctyp_txn SET status=%d WHERE stan='%s'",stat, seq);
		vSQL = tmp;
		if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
		{
			wrLog("�����������ݿ�ʧ��");
			g_adoConnInfo[index].connStat = 2;
			return -1;
		}
		g_adoConnInfo[index].connStat = 0;
		return 0;
}


//�������ͨ���������׼�¼
int CDB::updateYctThirdData(YCT_DATA yct_data)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	
	wrLog("��updateYctThirdData��");
	
	bbzero(tmp);
	sprintf(tmp, "update yctyp_txn set status=%d,yct_ack=%d", yct_data.status,yct_data.yct_ack);
	strcat(tmp, ", term_id='");
	strcat(tmp, yct_data.term_id);
	strcat(tmp, "', term_serial='");
	strcat(tmp, yct_data.term_serial);
	strcat(tmp, "', bank='");
	strcat(tmp, yct_data.bank);
	strcat(tmp, "', card_no='");
	strcat(tmp, yct_data.card_no);
	strcat(tmp, "', amount='");
	strcat(tmp, yct_data.amount);
	strcat(tmp, "', yct_serial='");
	strcat(tmp, yct_data.yct_serial);
	strcat(tmp, "', pki_card='");
	strcat(tmp, yct_data.pki_card);
	strcat(tmp, "', ticket_no='");
	strcat(tmp, yct_data.ticket_no);
	strcat(tmp, "', ticket_amo_bef='");
	strcat(tmp, yct_data.ticket_amo_bef);
	strcat(tmp, "', ticket_amo_aft='");
	strcat(tmp, yct_data.ticket_amo_aft);
	strcat(tmp, "', result_message='");
	strcat(tmp, yct_data.result_message_third);
	strcat(tmp, "' where stan='");
	strcat(tmp, yct_data.stan);
	strcat(tmp, "'");
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	wrLog("updateYctThirdData ����YCTYP_TXN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

//��ȡ��ֵ���ͣ�
int CDB::getChargeType(char *stan,int charge_type,char *pe_stan,char *pos_serial)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	char dec_yctstan[16+1];

	wrLog("��getChargeType��");
	bbzero(dec_yctstan);
	CTools::HexToDec(stan,dec_yctstan,16);

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select stan,term_serial,charge_type from yctyp_txn where yct_serial='%s'",dec_yctstan);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	bbzero(tmp);
	strcpy(tmp,(LPCTSTR)(_bstr_t)record->GetCollect("charge_type"));
	charge_type = atoi(tmp);

	bbzero(pe_stan);
	strcpy(pe_stan,(LPCTSTR)(_bstr_t)record->GetCollect("stan"));

	bbzero(pos_serial);
	strcpy(pos_serial,(LPCTSTR)(_bstr_t)record->GetCollect("term_serial"));

	return 0;
}

int CDB::getYct3SignInfo(char *term_id,PYCT_DATA pyct_data,char *cur_signTime)
{	
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	char SKEY[32+1];
	char SHID[16+1];
	
	wrLog("��getYct3SignInfo��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select * from yctyp_signin where term_id='%s'",term_id);
	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		//�޸��ն˶�Ӧ��¼�����һ��ǩ��
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	bbzero(pyct_data->pki_card);
	strcpy(pyct_data->pki_card,(LPCTSTR)(_bstr_t)record->GetCollect("pki_card"));
	wrLog("�����ţ�%s",pyct_data->pki_card);

 	bbzero(SHID);
// 	strcpy(SHID,(LPCTSTR)(_bstr_t)record->GetCollect("signin_serial"));
// 	CTools::hex_to_byte((BYTE *)SHID,pyct_data->SHID,16);
// 	wrHex("������ˮ�ţ�",pyct_data->SHID,8);

	bbzero(pyct_data->SHID);
	bbzero(SHID);
	strcpy(SHID,(LPCTSTR)(_bstr_t)record->GetCollect("signin_serial"));
	//CTools::tobyte((char *)(LPCTSTR)(_bstr_t)record->GetCollect("signin_serial"),SHID);
	//CTools::byte_to_hex((BYTE *)SHID,pyct_data->SHID,8); //������ˮ��
	memcpy(pyct_data->SHID,(BYTE *)SHID,16);
	wrLog("������ˮ�ţ�%s",pyct_data->SHID);

// 	bbzero(SKEY);
// 	strcpy(SKEY,(LPCTSTR)(_bstr_t)record->GetCollect("skey"));
// 	CTools::hex_to_byte((BYTE *)SKEY,pyct_data->SKEY,32);
// 	wrLog("SKEY��%s",SKEY);
// 	wrHex("pyct_data->SKEY��",pyct_data->SKEY,16);

	bbzero(pyct_data->SKEY);
	bbzero(SKEY);
	strcpy(SKEY,(LPCTSTR)(_bstr_t)record->GetCollect("skey"));
	//CTools::tobyte((char *)(LPCTSTR)(_bstr_t)record->GetCollect("skey"),SKEY);
	//CTools::byte_to_hex((BYTE *)SKEY,pyct_data->SKEY,16); //������ˮ��
	CTools::hex_to_byte((BYTE *)SKEY,pyct_data->SKEY,32);
	//memcpy(pyct_data->SKEY,(BYTE *)SKEY,32);
	//wrLog("SKEY��%s",pyct_data->SKEY);
	wrHex("pyct_data->SKEY��",pyct_data->SKEY,16);

	pyct_data->yct_sequence_NO = atol((LPCTSTR)(_bstr_t)record->GetCollect("sequence_no"));
	wrLog("������ţ�%d",pyct_data->yct_sequence_NO);

	bbzero(cur_signTime);
	strcpy(cur_signTime,(LPCTSTR)(_bstr_t)record->GetCollect("signin_time"));
	wrLog("ǩ��ʱ�䣺%s",cur_signTime);

	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::insertYct3SignInfo(char *term_id,PYCT_DATA pyct_data,char *cur_signTime)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	char sequence_NO[8+1];
	char SKEY[32+1];
	char SHID[16+1];
	
	wrLog("��insertYct3SignInfo��");
	
	bbzero(tmp);
	bbzero(sequence_NO);
	sprintf(sequence_NO,"%ld",pyct_data->yct_sequence_NO);
	bbzero(SKEY);
	CTools::byte_to_hex(pyct_data->SKEY,(BYTE *)SKEY,16);
	bbzero(SHID);
	CTools::byte_to_hex(pyct_data->SHID,(BYTE *)SHID,8);

	strcat(tmp, "insert into yctyp_signin values('");
	strcat(tmp, term_id);
	strcat(tmp,"','");
	strcat(tmp, pyct_data->pki_card);
	strcat(tmp,"','");
	strcat(tmp, (char *)pyct_data->SHID);
	strcat(tmp, "',sysdate,'");
	strcat(tmp, SKEY);
	strcat(tmp,"','");
	strcat(tmp, sequence_NO);
	strcat(tmp,"')");

	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	wrLog("����YCTYP_SIGNIN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::updateYct3SignInfo(char *term_id,PYCT_DATA pyct_data,char *cur_signTime)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	char sequence_NO[8+1];
	char SKEY[32+1];
	char SHID[16+1];
	
	wrLog("��updateYct3SignInfo��");
	
	bbzero(tmp);
	bbzero(sequence_NO);
	sprintf(sequence_NO,"%ld",pyct_data->yct_sequence_NO);
	bbzero(SKEY);
	CTools::byte_to_hex(pyct_data->SKEY,(BYTE *)SKEY,16);
	bbzero(SHID);
	CTools::byte_to_hex(pyct_data->SHID,(BYTE *)SHID,8);

	strcat(tmp, "update yctyp_signin set ");
	strcat(tmp, "signin_serial = '");
	strcat(tmp, (char *)pyct_data->SHID);
	strcat(tmp,"',signin_time = '");
	strcat(tmp, cur_signTime);
	strcat(tmp,"',skey = '");
	strcat(tmp, SKEY);
	strcat(tmp,"',sequence_no = '");
	strcat(tmp, sequence_NO);
	strcat(tmp,"' where term_id = '");
	strcat(tmp, term_id);
	strcat(tmp,"'");
	
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	wrLog("����YCTYP_SIGNIN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::checkYct3SignTimeInterval(char *term_id,char *interval)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	char db_interval[20];
	
	wrLog("��check_Yct3SignTime_Interval��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	bbzero(tmp);
	sprintf(tmp,"select * from yctyp_signin where term_id='%s'",term_id);
	vSQL = tmp;
	wrLog("sql:%s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		//��¼��Ϊ�գ���û�и��ն˼�¼�������ǩ��
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}

	bbzero(tmp);
	sprintf(tmp,"select to_char(sysdate,'YYYYMMDDHH24MI') - \
				(select to_char(signin_time,'YYYYMMDDHH24MI') from yctyp_signin where term_id='%s')\
				as interval from dual",term_id);
	vSQL = tmp;
	wrLog("sql:%s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if((record->GetCollect("interval")).vt!=VT_NULL)
	{
		bbzero(db_interval);
		strcpy(db_interval,(LPCTSTR)(_bstr_t)record->GetCollect("interval"));
	}
	
	if (atol(db_interval) < atol(interval))
	{
		//δ������Ч����������ǩ��
		g_adoConnInfo[index].connStat = 0;
		return 1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

int CDB::updateYct3SequenceNo(char *term_id,int sequence_no)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	char strSequence_NO[8+1];
	
	wrLog("��updateYct3SequenceNo��");
	
	bbzero(tmp);
	bbzero(strSequence_NO);
	sprintf(strSequence_NO,"%ld",sequence_no);
	
	strcat(tmp, "update yctyp_signin set ");
	strcat(tmp, "sequence_no = '");
	strcat(tmp, strSequence_NO);
	strcat(tmp,"' where term_id = '");
	strcat(tmp, term_id);
	strcat(tmp,"'");
	
	vSQL = tmp;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	wrLog("����YCTYP_SIGNIN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

long CDB::getTimeInterval(const char *startTime,const char *endTime)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
    char interval[512];
	
	wrLog("��getTimeInterval��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(tmp);
	sprintf(tmp,"select round(to_number(TO_DATE('%s','yyyy-mm-dd hh24:mi:ss') -\
					TO_DATE('%s','yyyy-mm-dd hh24:mi:ss')) * 24) as Hours\
					from dual",endTime,startTime);
	vSQL = tmp;
	wrLog("sql:%s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return 0;
	}

	if((record->GetCollect("Hours")).vt!=VT_NULL)
	{
		bbzero(interval);
		strcpy(interval,(LPCTSTR)(_bstr_t)record->GetCollect("Hours"));
	}
    wrLog("interval = %s",interval);
	g_adoConnInfo[index].connStat = 0;
	return atol(interval);
}

//�������ͨ���׼�¼״̬
int CDB::updateYctTxnStatus(char *stan,char *status)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];
	char dec_yctstan[16+1];
	
	wrLog("��updateYctTxnStatus��");
	bbzero(dec_yctstan);
	bbzero(tmp);
	if(YCT_VER==3)
	{
		CTools::HexToDec(stan,dec_yctstan,16);
	
		bbzero(tmp);
		sprintf(tmp, "update yctyp_txn set status='%s' where stan = '%s'  or yct_serial = '%s'", status,dec_yctstan,dec_yctstan);
		vSQL = tmp;
		
		index = getConnStat();  //��ȡ��������
		if( index<0 )
			return -1;
		
		wrLog("updateYctTxnStatus ����YCTYP_TXN�� [%s]",tmp);
		if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
		{
			wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
			g_adoConnInfo[index].connStat = 2;
			return -1;
		}

		g_adoConnInfo[index].connStat = 0;
		return 0;
	}

	return -1;
}


/*
��ȡ��ͨˮ��ú��ʡ��

*/
/*
int CDB::getSDM_Area()
{
	int  i, ret,index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	SDM_AREA sdm_area;

	bbzero(temp);
	strcpy(temp,"select AREACODE,DX_CODE,AREA_NAME,PARENT_CODE,AREA_FLAG,SHORTNAME from sdm_area ");
	
	wrLog("[��ȡ��ͨ�ĵ���]");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	if(!sdm_area.empty())
	{
		sdm_area.clear();
	}
	
	
	while(!record->adoEOF)
	{
		memset(&sdm_area, 0, sizeof(sdm_area));
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("areacode"));
		strcmp(sdm_area.AREACODE,temp);

		if((record->GetCollect("DX_CODE")).vt!=VT_NULL)
		{
			bbzero(temp);
			strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("DX_CODE"));
			strcmp(sdm_area.DX_CODE,temp);

		}

		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("AREA_NAME"));
		strcmp(sdm_area.AREA_NAME,temp);
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("PARENT_CODE"));
		strcmp(sdm_area.PARENT_CODE,temp);

		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("AREA_FLAG"));
		sdm_area.AREA_FLAG=atoi(temp);
		if(sdm_area.AREA_FLAG==2)
		{
		
			sdm_area_sum++;

		}
	
		if((record->GetCollect("SHORTNAME")).vt!=VT_NULL)
		{
		
			bbzero(temp);
			strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("SHORTNAME"));
			strcmp(sdm_area.SHORTNAME,temp);

		}

		sdm_area.push_back(sdm_area);
		record->MoveNext();
	}
	
	wrLog("��ˮ��ú��ͨ������������%d��",sdm_area_sum);
	g_adoConnInfo[index].connStat = 0;
	return 0;
}

*/ //cut by liuxiao

//�ж��Ƿ�ͨҵ��
int CDB::isOpenSDMService(char city,SDM_AREA sdm_city)
{

	int  ret,index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char temp[1024];
	
	bbzero(temp);
	strcpy(temp,"select * from sdm_area t where t.area_name like '%city%'");
	
	wrLog("[��ȡ��ͨ�ĵ���]");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	vSQL = temp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	if((record->GetCollect("AREACODE")).vt!=VT_NULL) //��������
	{
		
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("AREACODE"));
		strcmp(sdm_city.AREACODE,temp);
		
	}

	if((record->GetCollect("AREA_NAME")).vt!=VT_NULL)
	{
		
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("AREA_NAME")); //��������
		strcmp(sdm_city.AREA_NAME,temp);
		
	}

	if((record->GetCollect("PARENT_CODE")).vt!=VT_NULL)
	{
		
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("PARENT_CODE")); //��������
		strcmp(sdm_city.PARENT_CODE,temp);
		
	}

	if((record->GetCollect("SHORTNAME")).vt!=VT_NULL)
	{
		
		bbzero(temp);
		strcpy(temp, (LPCTSTR)(_bstr_t)record->GetCollect("SHORTNAME")); //��������
		strcmp(sdm_city.SHORTNAME,temp);
		
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;

}

/*
 type:ˮ��ú��ѯ���� 0:ֱ���г����е���
 tyoe:ˮ��ú��ѯ���� 1���������������ѯ
*/
int CDB::getSDMList(PTSTxnLogs ptxnLogs, const char* sql,int type, CGlobal gbl) //��ȡˮ��ú�б�
{
	int  index=-1, i=0,content_len=0,ret;
	char content[200], temp[200];
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp_len[2+1];

	bbzero(tmp_len);
	bbzero(content);
	ptxnLogs->colsnum = 3;
	ptxnLogs->colsWidth[0] = 620;
	ptxnLogs->colsWidth[1] = 0;
	ptxnLogs->colsWidth[2] = 0;

	ptxnLogs->colsNameLen = 59;
	if (type==0)
	{

		memcpy(ptxnLogs->colsName, "ʡ���б� \x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	}
	else
	{
	
		memcpy(ptxnLogs->colsName, "�����б� \x00�˵�ID\x00�˵�����", ptxnLogs->colsNameLen);
	}

	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	vSQL = sql;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	wrLog("�����ݿ���ȡ����ϸsql[%s]",sql);
	if(ret)
	{
		wrLog("�����ݿ���ȡ����ϸʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	for(i = 0; !record->adoEOF && i < 500; i++)
	{

		if((record->GetCollect("AREA_NAME")).vt!=VT_NULL)
		{
		
			bbzero(temp);
			strcpy(gbl.AREA_NAME, (LPCSTR)(_bstr_t)(record->GetCollect("AREA_NAME"))); //���׵���

			if((record->GetCollect("AREACODE")).vt!=VT_NULL)
			{
				strcpy(gbl.AREACODE, (LPCSTR)(_bstr_t)(record->GetCollect("AREACODE")));//�������
			}
			if((record->GetCollect("AREA_FLAG")).vt!=VT_NULL)
			{
				gbl.AREA_FLAG=atoi((LPCSTR)(_bstr_t)(record->GetCollect("AREA_FLAG")));
			}
			
			/*

			if((record->GetCollect("SHORTNAME")).vt!=VT_NULL)
			{
				strcpy(sdm_city.SHORTNAME, (LPCSTR)(_bstr_t)(record->GetCollect("SHORTNAME")));//�������
			}
			*/ //��ʱû�д����Ƶ�����

		}
		/*

		if (sdm_city.SHORTNAME!=NULL)
		{

			if( ptxnLogs->itemsLen+strlen(sdm_city.SHORTNAME) < MAX_INTERM_BUF )
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, sdm_city.SHORTNAME); 
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(sdm_city.SHORTNAME) + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
		*/
			
			if( ptxnLogs->itemsLen+strlen(gbl.AREA_NAME) < MAX_INTERM_BUF ) //��ʾ�����б�
			{
				strcpy(ptxnLogs->items+ptxnLogs->itemsLen, gbl.AREA_NAME); 
				ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(gbl.AREA_NAME) + 1;
			}
			else
			{
				break;
			}

		//}
			if (gbl.AREA_FLAG==1  ) //�����ʡ
			{
				if( ptxnLogs->itemsLen+strlen("784") < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "784"); //�����м����ѯ
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("784") + 1;
				}
				else
				{
					break;
				}
				
				
				bbzero(content);
				content_len=0;
				memcpy(content, gbl.AREACODE,6); //��������
				content_len+=6;
				wrLog("content:%s",content);
				
				if( ptxnLogs->itemsLen+strlen(content) < MAX_INTERM_BUF ) 
				{
					
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, content); //�����м����ѯ
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(content) + 1;
				}
				else
				{
					break;
				}

			}

			else if (gbl.AREA_FLAG==2 || gbl.AREA_FLAG==3)  //������л���ֱϽ��.ֱ�ӽ����ѯ

			 //���յ�����ѯ
			{
				if( ptxnLogs->itemsLen+strlen("786") < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, "786"); //���յ�����ѯ
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen("786") + 1;
				}
				else
				{
					break;
				}
				
				bbzero(content); //�������ݰ���ʽ��codearea(6)+PARENT_CODE(6) �������+��������+���������+����������
				int content_len=0;
				strcpy(content,gbl.AREACODE);
				content_len+=6;
				strcpy(content+content_len,gbl.PARENT_CODE);
				content_len+=6;
				if( ptxnLogs->itemsLen+content_len < MAX_INTERM_BUF )
				{
					strcpy(ptxnLogs->items+ptxnLogs->itemsLen, content); 
					ptxnLogs->itemsLen = ptxnLogs->itemsLen + strlen(content) + 1;
				}
				else
				{
					break;
			}

			}

			
		record->MoveNext();
	}

	sprintf(ptxnLogs->hint, "");
	if (gbl.AREA_FLAG==1)
	{
		sprintf(ptxnLogs->top, "ʡ���б�");
		
	}
	else
	{
		
		sprintf(ptxnLogs->top, "��ͨˮ��ú�����б�");
		
	}
	
	strcpy(ptxnLogs->bottom, "��<-����->����ҳ ��Enter����һ�� ��Esc������");

	if (gbl.AREA_FLAG==2)
	{

		sprintf(ptxnLogs->hint, "��%d��������ͨ", i);
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;

}


//��ȡҵ�����
int CDB::getBizName(const char *feeType,char * bizName)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	wrLog("��getBizName��");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select biz_name from biz_group  where biz_type='%s'",feeType);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	bbzero(bizName);
	strcpy(bizName,(LPCTSTR)(_bstr_t)record->GetCollect("biz_name"));
	g_adoConnInfo[index].connStat = 0;
	return 0;

}


//��ݸͨ����һ��ǩ������
//term_id���ն˱���
//signin_status:״̬

int CDB::insert_dgtSignData( DGTSignInfo dgtSignInfo,char *term_id)
{

	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[200];
	char seq[10+1];
	int ret;
	char signin_status[2+1];// 0 ��ʾ�Ѿ�ǩ�� 1��ʾǩ��
	bbzero(signin_status);
	dgtSignInfo.sign_status=0;
	sprintf(signin_status,"%d",dgtSignInfo.sign_status);
	bbzero(seq);

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	wrLog("���������ݿ�ͨ�� %d��", index);
	vSQL = "select lpad(epos_txn_seq.nextval, 10,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	wrLog("���кţ�[%s]", seq);

	wrLog("�����붫ݸͨǩ�����ݡ�");
	bbzero(tmp);
	strcat(tmp, "insert into dgt_sign_log (log_seq,BATCHNO,TERM_ID,POSID,SIGN_STATUS,SIGNIN_TIME,KEYSET,SETDATE)values('");
	strcat(tmp, seq);
	strcat(tmp,"','");
	strcat(tmp, dgtSignInfo.BATCHNO);
	strcat(tmp,"','");
	strcat(tmp, term_id);
	strcat(tmp,"','");
	strcat(tmp, dgtSignInfo.POSID);
	strcat(tmp,"','");
	strcat(tmp, signin_status);
	strcat(tmp, "',sysdate"); //ǩ��ʱ��
	strcat(tmp,",'");
	strcat(tmp, dgtSignInfo.KEYSET);
	strcat(tmp,"','");
	strcat(tmp, dgtSignInfo.SETTDATE);
	strcat(tmp,"')");
	vSQL = tmp;
	
	wrLog("����YCTYP_SIGNIN�� [%s]",tmp);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;

}


//����ǩ����Ϣ

//��ݸͨ���뽻�׼�¼
int CDB::insert_dgtTxnLog(CGlobal gbl,int status)
{
	
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[1024];
	char trade_status[2];
	char PAY_AMO[8+1];
	char tradeDate[8+1];
	char tradeTime[6+1];
	char stan[15+1]; //ϵͳ�ο���
	char child_type[2+1];
	char parent_type[2+1];
	char city_code[4+1];
	char effectiveDate[8+1];


	bbzero(PAY_AMO);
	bbzero(tradeDate);
	bbzero(tradeTime);
	bbzero(stan);
	bbzero(child_type);
	bbzero(parent_type);
	bbzero(city_code);
	bbzero(effectiveDate);

	wrLog("MESSAGEDATETIME:%s",gbl.dgtCard.MESSAGEDATETIME);


	memcpy(PAY_AMO,gbl.amount+4,8); 
	
	if(status==0)
	{
		bbzero(tradeTime);
		CTools::getTime(tradeTime);
		CTools::getDate(tradeDate);
		bbzero(gbl.dgtCard.tradeTime);
		bbzero(gbl.dgtCard.tradeDate);
		CTools::getDate(gbl.dgtCard.tradeDate);
		CTools::getTime(gbl.dgtCard.tradeTime);
		//�����Ƭ��ˮ�ǿ�
		if (strlen(gbl.dgtCard.CARDCNT)==0)
		{
			bbzero(gbl.dgtCard.CARDCNT);
			strcpy(gbl.dgtCard.CARDCNT,"000000");
		}
		
	}
	else
	{
		bbzero(tradeDate);
		bbzero(tradeTime);
		strcpy(tradeDate,gbl.dgtCard.tradeDate);
		strcpy(tradeTime,gbl.dgtCard.tradeTime);

	}

	if (strlen(tradeTime)==0 ) //���ʱ��Ϊ��
	{
		bbzero(tradeTime);
		CTools::getTime(tradeTime);
		bbzero(gbl.dgtCard.tradeTime);
		CTools::getTime(gbl.dgtCard.tradeTime);
	}

	if (strlen(tradeDate) ==0 ) //�����������Ϊ��
	{

		bbzero(tradeDate);
		CTools::getDate(tradeDate);
		bbzero(gbl.dgtCard.tradeDate);
		CTools::getDate(gbl.dgtCard.tradeDate);
	}
	
	if (strlen(gbl.dgtCard.CARDCNT)==0)
	{
		bbzero(gbl.dgtCard.CARDCNT);
		strcpy(gbl.dgtCard.CARDCNT,"000000");
	}
	if(strlen(gbl.dgtCard.CARDCNT) >6)
	{

		char cardCnt[6+1];
		bbzero(cardCnt);
		memcpy(cardCnt,gbl.dgtCard.CARDCNT,6);
		bbzero(gbl.dgtCard.CARDCNT);
		strcpy(gbl.dgtCard.CARDCNT,cardCnt);
		
	}

	CTools::byte_to_hex((BYTE *) gbl.dgtCard.cardParent_type,(BYTE *) parent_type,1);
	CTools::byte_to_hex((BYTE *) gbl.dgtCard.cardChild_type,(BYTE *) child_type,1);
	CTools::byte_to_hex((BYTE *) gbl.dgtCard.CItyCode,(BYTE *) city_code,2);

	if (strcmp(gbl.fee_type,"0372")==0) //������ۿ�����Ƭ����
	{
		bbzero(gbl.dgtCard.tradeAfterMny);
		strcpy(gbl.dgtCard.tradeAfterMny,gbl.dgtCard.tradeBeForeMny);

	}

	
	if (strlen(gbl.SYSTEMNO)==0) //���ϵͳ�ο���Ϊ��
	{
		
		bbzero(gbl.SYSTEMNO);
		memcpy(gbl.SYSTEMNO,gbl.dgtCard.KEEP+(strlen(gbl.dgtCard.KEEP)-15),15);
	}

	if (strlen(gbl.dgtCard.SETDATE)==0) //�����ݸͨ��������Ϊ��
	{

		bbzero(gbl.dgtCard.SETDATE);
		CTools::getDate(gbl.dgtCard.SETDATE);

	}

	bbzero(trade_status);
	sprintf(trade_status,"%d",status);

	CTools::byte_to_hex((BYTE *) gbl.dgtCard.effectiveDate,(BYTE *)effectiveDate,4);
	
	wrLog("�����붫ݸͨ�������ݡ�");
	
	bbzero(tmp);
	strcat(tmp, "insert into dgt_txn_log (STAN,TERM_ID,POSID,BUS_BIZ,BANK,BANK_CARD,BUS_CARD,OLD_AMO,PAY_AMO,NEW_AMO,\
		BUS_DATE,BUS_TIME,STATUS,YJ_AMO,BATCHNO,posseq,SETTIME,cardseq,APTRANSSEQ,LIMITEDAUTHSEQL,AUTHSEQ,ALGIND,\
		KEYVER,CARDRANDOM,CITYCODE,CARD_CHILDYPE,CARD_PARENTTYPE,TAC,CARDVALDATE,LASTPOSSVSEQ,keep)values('");
	
	strcat(tmp,gbl.SYSTEMNO);
	strcat(tmp,"','");
	strcat(tmp, gbl.dPosInfo.pos_no);
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtSignInfo.POSID);
	strcat(tmp,"','");
	strcat(tmp, gbl.fee_type);
	strcat(tmp,"','");
	strcat(tmp, gbl.bank);
	strcat(tmp,"','");
	strcat(tmp, gbl.card_no);
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.cardShowId);
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.tradeBeForeMny);
	strcat(tmp,"','");
	strcat(tmp, PAY_AMO);
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.tradeAfterMny);
	strcat(tmp,"','");
	strcat(tmp, tradeDate);
	strcat(tmp,"','");
	strcat(tmp, tradeTime);
	strcat(tmp,"','");
	strcat(tmp, trade_status);
	strcat(tmp,"','");
	strcat(tmp, gbl.DEPOSIT);
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.BATCHNO);
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.posSeq);  //�ն���ˮ��
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.SETDATE);  //����ʱ��
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.CARDCNT);  //��Ƭ������

	strcat(tmp,"','");
	strcat(tmp, gbl.TRANSSEQ);  //������ˮ��
	
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.LIMITEAUTHSEQL);  

	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.AUTHSEQ);  

	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.ALGIND);  

	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.KEYVER);  

	
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.CARDRAND);  
	
	strcat(tmp,"','");
	strcat(tmp, city_code);  

	strcat(tmp,"','");
	strcat(tmp,child_type);  

	strcat(tmp,"','");
	strcat(tmp, parent_type);  

	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.TAC);  
	
	strcat(tmp,"','");
	strcat(tmp, effectiveDate);  

	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.lastPosSeq);  
	
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtCard.KEEP);  

	strcat(tmp,"')");
	vSQL = tmp;

	wrLog("ִ�����:%s",tmp);

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
	
}


int CDB::update_dgtTxnLog(char *orderId,int status)
{

	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_txn_log set status='%d' where APTRANSSEQ ='%s'",status,orderId);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;


}

//���¿�Ƭ�����б��е���Ϣ

int CDB ::update_cardIgoreStatus (char * bus_card,char * status)
{

	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_card_igoreList set status='%s' where bus_card ='%s'",status,bus_card);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}

//����keep����״̬
int CDB::update_dgtTxnStatusByKeep(char *keep,int status)
{
	
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_txn_log set status='%d' where keep ='%s'",status,keep);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
	
}

//����ϵͳ�ο����޸Ķ���״̬
int CDB::update_dgtTxnLogByStan(char *sysyno,int status)
{
	
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_txn_log set status='%d' where STAN ='%s'",status,sysyno);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
	
}



//�����û���һ�ʳɹ����׵���ˮ�� �����û�в�ѯ����Ĭ��Ϊ0
int CDB::getLastPosSEQ(char * term_id,char * lastPosSEQ)
{

	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	wrLog("��ȡ��һ�ʳ�ֵ�ն���ˮ��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select posseq from (select * from dgt_txn_log t where t.term_id='%s'  and t.bus_biz='0371' and  t.status='2'  order by t.bus_date desc,t.bus_time desc ) where rownum=1",term_id);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	
	bbzero(lastPosSEQ);
	strcpy(lastPosSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("posseq"));
	g_adoConnInfo[index].connStat = 0;
	return 0;

}

//����ǩ������
int CDB::update_dgtSignInfo(char * term_id,DGTSignInfo dgtSignInfo)
{

	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_sign_log set  TOTALSVNUM = '%s',TOTALSVAMT='%s',TOTALSALEDEP='%s' where BATCHNO = '%s' and term_id='%s' and LOG_SEQ='%s' ",dgtSignInfo.TOTALSVNUM,dgtSignInfo.TOTALSVAMT,dgtSignInfo.TOTALSALEDEP,dgtSignInfo.BATCHNO,term_id,dgtSignInfo.LOG_SEQ);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}



//���붫ݸͨ�Ծɻ�������
int CDB::insert_dgtTransferLog(CGlobal gbl,char *status)
{
	int index=-1;
	int ret;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[500];
	char seq[10+1];
	bbzero(seq);
	char tradeDate[8+1];
	char tradeTime[6+1];
	bbzero(tradeDate);
	bbzero(tradeTime);

	CTools::getDate(tradeDate);
	CTools::getTime(tradeTime);
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	wrLog("���������ݿ�ͨ�� %d��", index);
	vSQL = "select lpad(epos_txn_seq.nextval, 10,0) aaa from dual";
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	strcpy( seq, (LPCSTR)(_bstr_t)(record->GetCollect("aaa")) );
	wrLog("���кţ�[%s]", seq);
	
	wrLog("�����붫ݸͨ�Ծɻ��½������ݡ�");
	
	bbzero(tmp);
	strcat(tmp, "insert into dgt_transfer_log (DGT_SEQ,TERM_ID,M1_CARD,STATUS,M1_ORIG_MNY,MONTHFEEAMT,FEEAMT,CENTERSEQNO,RETURNAMT,RETURNDEPOSIT,MONTHFEEFLAG,FEEAMTFLAG,POSSERIAL,BUS_DATE,BUS_TIME,SETTIME,BATCHNO)values('");
	
	strcat(tmp, seq); //ϵͳ��ˮ
	strcat(tmp,"','");

	strcat(tmp, gbl.dPosInfo.pos_no); //�ն˺�
	strcat(tmp,"','");
	strcat(tmp, gbl.dgtM1Card.bus_showCard);
	strcat(tmp,"','");

	strcat(tmp, status);
	strcat(tmp,"','");

	strcat(tmp, gbl.dgtM1Card.bus_amt);
	strcat(tmp,"','");

	strcat(tmp, gbl.MonthFeeAmt);
	strcat(tmp,"','");

	strcat(tmp, gbl.FeeAmt);
	strcat(tmp,"','");

	strcat(tmp, gbl.Centerseqno);
	strcat(tmp,"','");

	strcat(tmp, gbl.ReturnAmt);
	strcat(tmp,"','");

	strcat(tmp, gbl.ReturnDeposit);
	strcat(tmp,"','");

	strcat(tmp, gbl.MonthFeeFlag);
	strcat(tmp,"','");

	strcat(tmp, gbl.FeeAmtFlag);
	strcat(tmp,"','");

	strcat(tmp, gbl.dgt_posSerial);
	strcat(tmp,"','");

	strcat(tmp, tradeDate);
	strcat(tmp,"','");

	strcat(tmp, tradeTime);

	strcat(tmp,"','");
	
	strcat(tmp, gbl.dgtCard.SETDATE);

	strcat(tmp,"','");
	
	strcat(tmp, gbl.dgtCard.BATCHNO);
	strcat(tmp,"')");
	vSQL = tmp;
	
	wrLog("ִ�����:%s",tmp);
	vSQL = tmp;

	if(index == -1)  //û��ȡ�����ݿ�����
	{
		index = getConnStat();  //��ȡ��������
		if( index<0 )
			return -1;
		wrLog("���������ݿ�ͨ�� %d��", index);
	}

	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�������ݿ�ʧ��SQL[%s]",tmp);
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;

}


//��ȡ��ݸͨ��������Ϣ
int CDB::getDGTcodeMessge(char * rsp_code,char *rsp_message)
{
	
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	wrLog("��ȡ��ݸͨ��������Ϣ");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select rsp_message from DGT_TRADE_CODE where rsp_code='%s'",rsp_code);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	bbzero(rsp_message);
	strcpy(rsp_message,(LPCTSTR)(_bstr_t)record->GetCollect("rsp_message"));

	g_adoConnInfo[index].connStat = 0;
	return 0;
	
}


//����ǩ����Ϣ
int CDB::update_signOutInfo(char *term_id,PDGTSignInfo pdgtSignInfo)
{

	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];
	
	wrLog("���½��׽��%");
	pdgtSignInfo->sign_status=1;
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_sign_log set SIGN_STATUS = '%d',sign_out_time=sysdate  where BATCHNO = '%s' and term_id='%s' and LOG_SEQ='%s' ",pdgtSignInfo->sign_status,pdgtSignInfo->BATCHNO,term_id,pdgtSignInfo->LOG_SEQ);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}


// ��ȡ��ݸ�����豸��Ϣ
//������Ϣ����Կ��Ϣ
int CDB::getDGTDeviceInfo(char *termId,char *posKey,char *posId,char *busi_id)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	wrLog("��ȡ��ݸ�����ն���Ϣ");

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select POSID,BUSI_ID,POSKEY from DGT_POSID_BINDING where TERMID='%s'",termId);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	

	strcpy(busi_id,(LPCTSTR)(_bstr_t)record->GetCollect("BUSI_ID"));
	strcpy(posId,(LPCTSTR)(_bstr_t)record->GetCollect("POSID"));
	strcpy(posKey,(LPCTSTR)(_bstr_t)record->GetCollect("POSKEY"));

	g_adoConnInfo[index].connStat = 0;

	return 0;
	
}


//��ȡ��ݸͨ �����������������

int CDB::getCardTransferProcess(CGlobal *pgbl,char *tradeStatus,char *processInfo)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	char openDate[8+1];
	char cpu_card[16+1];
	bbzero(cpu_card);
	bbzero(processInfo);

	wrLog("��ȡ��ݸ������������");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select * from dgt_transfer_log t  where t.m1_card='%s' ",pgbl->dgtM1Card.bus_showCard);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		strcpy(processInfo,"��δ����");
		return -2;
	}
	if(record->adoEOF)
	{
		strcpy(processInfo,"��δ����");
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	bbzero(pgbl->Centerseqno);
	strcpy(pgbl->Centerseqno,(LPCTSTR)(_bstr_t)record->GetCollect("centerseqno")); //��ݸ������ˮ��
	bbzero(tradeStatus);
	strcpy(tradeStatus,(LPCTSTR)(_bstr_t)record->GetCollect("status")); //����״̬

	if((record->GetCollect("RETURNAMT")).vt!=VT_NULL) 
	{
		bbzero(pgbl->ReturnAmt);
		strcpy(pgbl->ReturnAmt,(LPCTSTR)(_bstr_t)record->GetCollect("RETURNAMT")); //���ص�ת�ƽ��
	}
	
	else //���������
	{
		
		bbzero(pgbl->ReturnAmt);
		strcpy(pgbl->ReturnAmt,"0000000000");
	}

	if((record->GetCollect("RETURNDEPOSIT")).vt!=VT_NULL) //��ȡreturndeposit
	{
		
		bbzero(pgbl->ReturnDeposit);
		strcpy(pgbl->ReturnDeposit,(LPCTSTR)(_bstr_t)record->GetCollect("RETURNDEPOSIT"));
	}
	

	else //���������
	{
		
		bbzero(pgbl->ReturnDeposit);
		strcpy(pgbl->ReturnDeposit,"000000");
	}


	if((record->GetCollect("MONTHFEEAMT")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->MonthFeeAmt);
		strcpy(pgbl->MonthFeeAmt,(LPCTSTR)(_bstr_t)record->GetCollect("MONTHFEEAMT"));
	}
	
	else //���������
	{

		bbzero(pgbl->MonthFeeAmt);
		strcpy(pgbl->MonthFeeAmt,"00000000");
	}

		
	if((record->GetCollect("FEEAMT")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->FeeAmt);
		strcpy(pgbl->FeeAmt,(LPCTSTR)(_bstr_t)record->GetCollect("FEEAMT"));
	}
	
	else //���������
	{
		
		bbzero(pgbl->FeeAmt);
		strcpy(pgbl->FeeAmt,"00000000");
	}


	if((record->GetCollect("MONTHFEEFLAG")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->MonthFeeFlag);
		strcpy(pgbl->MonthFeeFlag,(LPCTSTR)(_bstr_t)record->GetCollect("MONTHFEEFLAG"));
	}
	
	else //���������
	{
		
		bbzero(pgbl->MonthFeeFlag);
		strcpy(pgbl->MonthFeeFlag,"1");
	}

	if((record->GetCollect("FEEAMTFLAG")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->FeeAmtFlag);
		strcpy(pgbl->FeeAmtFlag,(LPCTSTR)(_bstr_t)record->GetCollect("FEEAMTFLAG"));
	}
	
	else //���������
	{
		
		bbzero(pgbl->FeeAmtFlag);
		strcpy(pgbl->FeeAmtFlag,"1");
	}
	

	if (strcmp(tradeStatus,"C01")==0 || strcmp(tradeStatus,"FFF")==0) //�Ѿ����룬���ǿ���ʧ��
	{
		strcpy(processInfo," ��������ɹ�������ʧ��");
		return 1;
	}
	else
	{
	
		//����ĳ������ύ����Ҫ֪��cpu������Ϣ���� cpu ����
		if((record->GetCollect("CPU_CARD")).vt!=VT_NULL) //��ȡ�����Ķ�ݸͨ��������
		{
			bbzero(pgbl->dgtCard.cardShowId);
			strcpy(pgbl->dgtCard.cardShowId,(LPCTSTR)(_bstr_t)record->GetCollect("CPU_CARD"));
		}
		else //���������
		{
			
			bbzero(pgbl->dgtCard.cardShowId);
		}

		if((record->GetCollect("OPENDATE")).vt!=VT_NULL)
		{
			bbzero(openDate);
			strcpy(openDate,(LPCTSTR)(_bstr_t)record->GetCollect("OPENDATE"));
		}
		else //���������
		{
			
			bbzero(openDate);
		}

		bbzero(pgbl->dgtCard.openDate);
		CTools::hex_to_byte((BYTE *) openDate,(BYTE *)pgbl->dgtCard.openDate,8);


	 if (strcmp(tradeStatus,"C00")==0) //��������ɹ�����Ҫ��ԭ����cpu����Ϣ

	 {
		 bbzero(processInfo);
		strcpy(processInfo," �����ɹ�");
		return 2;
	 } 

	 if (strcmp(tradeStatus,"C0T")==0) //���������ʱ����Ҫ��ԭ����cpu����Ϣ
		 
	 {
		 bbzero(processInfo);
		 strcpy(processInfo," ������ʱ");
		 return 3;
	 } 

	 
	 if (strcmp(tradeStatus,"C0S")==0) //��������ɹ�����Ҫ��ԭ����cpu����Ϣ
		 
	 {
		 bbzero(processInfo);
		 strcpy(processInfo," �����ɹ���ȷ��ʧ��");
		 return 4;
	 } 

	else if (strcmp(tradeStatus,"A01")==0 || strcmp(tradeStatus,"A00")==0 ) //��ȡ���ʧ�� 
	{
		bbzero(processInfo);
		sprintf(processInfo,"״̬��%s����Ҫ���»�ȡ�����ȡ����",tradeStatus);
		return 5;
	}

	else if (strcmp(tradeStatus,"R00")==0 )
	{
		bbzero(processInfo);
		strcpy(processInfo,"Ȧ��ʧ��,�����ɹ�");
		return 6;
	}
	else if (strcmp(tradeStatus,"R01")==0)
	{
		bbzero(processInfo);
		strcpy(processInfo,"Ȧ��ʧ�ܣ���Ҫ�򹫽���˾���ͳ���");
		return 7;

	}
	else if (strcmp(tradeStatus,"R02")==0)
	{
		bbzero(processInfo);
		strcpy(processInfo,"Ȧ��ʧ�ܣ���Ҫ����ҵ�˻����ͳ���");
		return 8;
	}
	else if (strcmp(tradeStatus,"R03") ==0)
	{
		bbzero(processInfo);
		strcpy(processInfo,"Ȧ��ʧ�ܣ���Ҫ�򹫽���˾����ҵ�˻����ͳ���");
		return 9;
	}
	else if (strcmp(tradeStatus,"S01")==0)
	{
		bbzero(processInfo);
		strcpy(processInfo,"��Ȧ��ɹ�����Ҫ�򹫽���˾����ȷ�ϱ���");
		return 10;
	}
	else if (strcmp(tradeStatus,"S0T")==0)
	{

		if((record->GetCollect("resubmit")).vt!=VT_NULL)
		{
			bbzero(pgbl->dgtCard.resubmitField);
			strcpy(pgbl->dgtCard.resubmitField,(LPCTSTR)(_bstr_t)record->GetCollect("resubmit"));
		}
		
		else //���������
		{
			bbzero(pgbl->dgtCard.resubmitField);
		}

		bbzero(processInfo);
		strcpy(processInfo,"Ȧ�泬ʱ�����״�ȷ��");
		return 11;
	}


	else if (strcmp(tradeStatus,"S02") ==0)
	{

		if((record->GetCollect("SYSORDER")).vt!=VT_NULL)
		{
			bbzero(pgbl->TRANSSEQ);
			strcpy(pgbl->TRANSSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("SYSORDER"));
		}
		
		else //���������
		{
			
			bbzero(pgbl->TRANSSEQ);
		}
		
		bbzero(processInfo);
		strcpy(processInfo,"��Ȧ��ɹ�����Ҫ����ҵ�˻�����ȷ�ϱ���");
		return 12;
	}
	else if (strcmp(tradeStatus,"S03") ==0)
	{
		
		if((record->GetCollect("SYSORDER")).vt!=VT_NULL)
		{
			bbzero(pgbl->TRANSSEQ);
			strcpy(pgbl->TRANSSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("SYSORDER"));
		}
		
		else //���������
		{
			
			bbzero(pgbl->TRANSSEQ);
		}
		
		bbzero(processInfo);
		strcpy(processInfo,"�Ѿ�Ȧ��ɹ�����Ҫ�򹫽���˾����ҵ�˻�����ȷ�ϱ���");
		return 13;

	}

	
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;
	
}
//��ѯ��ݸ��������
int CDB::IsGetM1CardInDgtTransferLog(CGlobal *pgbl)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];


	wrLog("��ȡ��ݸ��������");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select 1 from dgt_transfer_log t  where t.m1_card='%s' ",pgbl->bus_showId);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	return 1;
}

//��ѯ�������Ƿ���Ҫ�ж��俨Ƭ����״̬

int CDB::isCardInIgoreList(char *busCard)
{
	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	
	wrLog("�ж϶�ݸͨ��Ƭ�Ƿ���Ҫ��֤������Ϣ");
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp,"select 1 from dgt_card_igoreList t  where t.bus_card='%s' and status='0' ",busCard);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;
	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}
	
	//���״̬�Ǻ��Եĺ��Ե�״̬

	return 0;
}

//���»�ͨ����״̬��Ϣ
int CDB::updateM1CardStatus(char *M1CardId,char *cardStatus)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char tmp[512];

	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	
	bbzero(tmp);
	sprintf(tmp,"update dgt_transfer_log t set t.card_status='%s' where t.m1_card='%s'",cardStatus,M1CardId);
	vSQL = tmp;
	
	wrLog("���»�ͨ����Ƭ״̬ [%s]",tmp);

	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -4;
	}

	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}


//ͨ��keepֵ�޸Ķ�ݸͨ���׼�¼
int CDB::update_dgtTxnLog_byKeep(CGlobal gbl)
{

	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	bbzero(sql);
	char tradeDate[8+1];
	char tradeTime[6+1];
	bbzero(tradeTime);
	bbzero(tradeDate);
	
	
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	bbzero(sql);
	sprintf(sql, "update DGT_TXN_LOG t set BATCHNO='%s',SETTIME='%s', AUTHSEQ='%s', LIMITEDAUTHSEQL='%s' ,APTRANSSEQ='%s',STAN='%s'where keep='%s'",\
		gbl.dgtCard.BATCHNO,gbl.dgtCard.SETDATE,gbl.dgtCard.AUTHSEQ,gbl.dgtCard.LIMITEAUTHSEQL,gbl.TRANSSEQ,gbl.SYSTEMNO,gbl.dgtCard.KEEP);
	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;

}


//���³�ֵ��¼��
int  CDB::updateTranChargeProcess(CGlobal *pgbl,char *tradeStatus)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[500];
	bbzero(sql);
	char tradeDate[8+1];
	char tradeTime[6+1];
	bbzero(tradeTime);
	bbzero(tradeDate);

	if (strlen(pgbl->dgtCard.tradeDate) ==0) 
	{

		CTools::getDate(tradeDate);
	}
	else
	{
	
	   strcpy(tradeDate,pgbl->dgtCard.tradeDate);

	}
	if (strlen(pgbl->dgtCard.tradeTime) ==0)
	{

		CTools::getTime(tradeTime);
	}
	else
	{
		strcpy(tradeTime,pgbl->dgtCard.tradeTime);
	}

	
	wrLog("���¿�Ƭ����״̬");
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	if ( strcmp(tradeStatus,"C00")==0 || strcmp(tradeStatus,"C0T") ==0)  //���¿����Ŀ��źͿ���ʱ�� ��д����ʱ
	{

		char openDate[8+1];
		bbzero(openDate);
		char openTime[6+1];
		bbzero(openTime);

		CTools::byte_to_hex((BYTE *) pgbl->dgtCard.openDate,(BYTE *)openDate,4);
	
		bbzero(sql);
		sprintf(sql, "update dgt_transfer_log t set t.opendate='%s', t.status='%s', cpu_card='%s' ,BUS_DATE='%s',BUS_TIME='%s',OPENTIME= '%s' where t.centerseqno='%s'",openDate,tradeStatus,pgbl->dgtCard.cardShowId,tradeDate,tradeTime,pgbl->dgtCard.openTime,pgbl->Centerseqno);
	}
	else if (strcmp(tradeStatus,"C01")==0 || strcmp(tradeStatus,"C0S") ==0 ) //����ʧ��
	{
		bbzero(sql);
		sprintf(sql, "update dgt_transfer_log t set t.status='%s',t.cpu_card='%s',BUS_DATE='%s',BUS_TIME='%s' where t.centerseqno='%s'",tradeStatus,pgbl->dgtCard.cardShowId,tradeDate,tradeTime,pgbl->Centerseqno);

	}

	else if (memcmp(tradeStatus,"R",1) ==0 || memcmp(tradeStatus,"S",1) ==0 ) //����ǳ������׻�������ʧ��
	{
	
		if (strlen(pgbl->TRANSSEQ) >0) //��������Ŵ��� 
		{

			bbzero(sql);
			sprintf(sql, "update dgt_transfer_log t set t.CPU_CARD_MNY='%s', t.status='%s', BUS_DATE='%s',BUS_TIME='%s', SYSORDER='%s' where t.centerseqno='%s' ",pgbl->dgtCard.cardMny,tradeStatus,tradeDate,tradeTime,pgbl->TRANSSEQ,pgbl->Centerseqno);
	
		}
		else
		{
			bbzero(sql);
			sprintf(sql, "update dgt_transfer_log t set t.CPU_CARD_MNY='%s', t.status='%s' ,BUS_DATE='%s',BUS_TIME='%s'where t.centerseqno='%s' ",pgbl->dgtCard.cardMny,tradeStatus,tradeDate,tradeTime,pgbl->Centerseqno);
		}
		
	}
	
	else if (memcmp(tradeStatus,"A",1)==0) //������ύ���� 
	{

		bbzero(sql);
		sprintf(sql, "update dgt_transfer_log t set  t.status='%s' ,BUS_DATE='%s',BUS_TIME='%s'  where t.centerseqno='%s'",tradeStatus,tradeDate,tradeTime,pgbl->Centerseqno);
	}

	
	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;


}

//���¶�ݸͨ�Ծɻ��¶���״̬ add by liuxiao

int  CDB::updateTranOrderStatus(char * Centerseqno,char *orderStatus)
{
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[1024];
	bbzero(sql);

	wrLog("���¿�Ƭ����״̬");
	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;

	bbzero(sql);
	sprintf(sql, "update dgt_transfer_log t set  order_status ='%s'  where centerseqno='%s' ",orderStatus,Centerseqno);
	vSQL = sql;
	wrLog("���¶���״̬sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("����ʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	return 0;
	
}

//��ȡ��ݸͨǩ����Ϣ
//�������һ��ǩ����Ϣ����ѯ��ݸͨǩ�����
//����ֵΪ0 
//Type ǩ������ 0 ǩ����1ǩ��
//�ն�ÿ���һ�ʽ��ױ�������ǩ��������KEYSET

int CDB::checkSignStatus(char *term_id,PDGTSignInfo pdgtSignInfo)
{

	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	int sign_status;
	int lastSign_status;
	int lastSignFlag=0;
	int currentSignFlag=0;

	
	//��ʼ��ǩ����Ϣ
	memset(pdgtSignInfo,0,sizeof(pdgtSignInfo));
	
	wrLog("����ѯ���춫ݸͨǩ�������");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	//��ѯ����֮ǰ���һ�ʽ��׶�ݸ��ݸͨǩ����������û��ǩ�ˡ��Զ�ǩ�ˡ��Զ�ǩ��
	bbzero(tmp);
	sprintf(tmp,"select * from (select * from dgt_sign_log  where term_id='%s' and  to_char( signin_time,'yyyymmdd')<to_char(sysdate,'yyyymmdd' ) order by signin_time desc) where rownum=1",term_id);

	wrLog("��ѯ����֮ǰ���һ�ʽ��׵�״̬:%s",tmp);

	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(!record->adoEOF) 
	{
		//����֮ǰû���޸��ն˶�Ӧ��¼����Ҫ��ǩ��
		wrLog("���һ�ʽ��׵�ǩ����Ϣ");
		lastSignFlag=1;

		bbzero(pdgtSignInfo->BATCHNO);
		strcpy(pdgtSignInfo->BATCHNO,(LPCTSTR)(_bstr_t)record->GetCollect("BATCHNO"));
		bbzero(pdgtSignInfo->KEYSET);
		strcpy(pdgtSignInfo->KEYSET,(LPCTSTR)(_bstr_t)record->GetCollect("KEYSET"));
		bbzero(pdgtSignInfo->POSID);
		strcpy(pdgtSignInfo->POSID,(LPCTSTR)(_bstr_t)record->GetCollect("POSID"));
		bbzero(pdgtSignInfo->SETTDATE);
		strcpy(pdgtSignInfo->SETTDATE,(LPCTSTR)(_bstr_t)record->GetCollect("SETDATE"));
		bbzero(pdgtSignInfo->TOTALSVNUM);
		strcpy(pdgtSignInfo->TOTALSVNUM,(LPCTSTR)(_bstr_t)record->GetCollect("TOTALSVNUM"));
		bbzero(pdgtSignInfo->TOTALSVAMT);
		strcpy(pdgtSignInfo->TOTALSVAMT,(LPCTSTR)(_bstr_t)record->GetCollect("TOTALSVAMT"));
		bbzero(pdgtSignInfo->TOTALSALEDEP);
		strcpy(pdgtSignInfo->TOTALSALEDEP,(LPCTSTR)(_bstr_t)record->GetCollect("TOTALSALEDEP"));
		bbzero(pdgtSignInfo->LOG_SEQ);
		strcpy(pdgtSignInfo->LOG_SEQ,(LPCTSTR)(_bstr_t)record->GetCollect("LOG_SEQ"));

		lastSign_status=atoi((LPCTSTR)(_bstr_t)record->GetCollect("SIGN_STATUS"));
		if (lastSign_status==0)
		{
			g_adoConnInfo[index].connStat = 0;
			return -4; //��Ҫǩ��

		}


	}

	//��ѯ�������һ�ʽ���ǩ�����
	bbzero(tmp);
	sprintf(tmp,"select * from (select * from dgt_sign_log where term_id='%s' and  to_char( signin_time,'yyyymmdd')=to_char(sysdate,'yyyymmdd' ) order by signin_time desc) where rownum=1  ",term_id);
	
	wrLog("��ѯ����ǩ�����:%s",tmp);

	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		wrLog("��Ҫǩ��");
		if (lastSignFlag==0)
		{
			return -3;
		}
		else
		{
			return -2;
		}
	}
	if(record->adoEOF)
	{
		//����û���޸��ն˶�Ӧ��¼�����һ��ǩ�� 
		g_adoConnInfo[index].connStat = 0;
		if (lastSignFlag==0)
		{
			return -3; //��һ��ǩ��
		}
		else
		{
		
			return -2;
		}
	
	}
	
	
	//���һ�ʽ����Ѿ�ǩ��
	sign_status=atoi((LPCTSTR)(_bstr_t)record->GetCollect("SIGN_STATUS"));
	if (sign_status==1) //�Ѿ�ǩ��
	{
		return -5;// ��ʾ״̬�Ѿ�ǩ��
	}
	
	wrLog("���Ѿ�ǩ���ɹ�����ȡǩ����Ϣ");
	bbzero(pdgtSignInfo->BATCHNO); //���κ�
	strcpy(pdgtSignInfo->BATCHNO,(LPCTSTR)(_bstr_t)record->GetCollect("BATCHNO"));
	bbzero(pdgtSignInfo->KEYSET);
	strcpy(pdgtSignInfo->KEYSET,(LPCTSTR)(_bstr_t)record->GetCollect("KEYSET")); //ͨѶkeyset
	bbzero(pdgtSignInfo->POSID);
	strcpy(pdgtSignInfo->POSID,(LPCTSTR)(_bstr_t)record->GetCollect("POSID")); //�����豸��Ϣ
	bbzero(pdgtSignInfo->SETTDATE);
	strcpy(pdgtSignInfo->SETTDATE,(LPCTSTR)(_bstr_t)record->GetCollect("SETDATE")); //����ʱ��
	bbzero(pdgtSignInfo->TOTALSVNUM);
	strcpy(pdgtSignInfo->TOTALSVNUM,(LPCTSTR)(_bstr_t)record->GetCollect("TOTALSVNUM")); //��������

	bbzero(pdgtSignInfo->TOTALSVAMT);
	strcpy(pdgtSignInfo->TOTALSVAMT,(LPCTSTR)(_bstr_t)record->GetCollect("TOTALSVAMT")); //���׽��

	bbzero(pdgtSignInfo->TOTALSALEDEP);
	strcpy(pdgtSignInfo->TOTALSALEDEP,(LPCTSTR)(_bstr_t)record->GetCollect("TOTALSALEDEP")); //����Ѻ��

	bbzero(pdgtSignInfo->LOG_SEQ);
	strcpy(pdgtSignInfo->LOG_SEQ,(LPCTSTR)(_bstr_t)record->GetCollect("LOG_SEQ")); //�������к�

	g_adoConnInfo[index].connStat = 0;
	return 0; 
}


//��ȡ��Ƭ״̬��Ϣ
int CDB::checkM1CardStatus(char *m1CardId)
{

	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	char card_status[2+1];
	
	wrLog("����ѯ��ͨ���Ŀ�Ƭ��Ϣ��");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	//��ѯ����֮ǰ���һ�ʽ��׶�ݸ��ݸͨǩ����������û��ǩ�ˡ��Զ�ǩ�ˡ��Զ�ǩ��
	bbzero(tmp);
	sprintf(tmp,"select * from dgt_transfer_log where m1_card='%s' and card_status !='02' ",m1CardId); //���û������

	wrLog("��ȡ����״̬��Ϣ:%s",tmp);

	vSQL = tmp;
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);

	if(record->adoEOF)  //����Ϣ
	{
	
		return -1; 

	}


	bbzero(card_status);
	strcpy(card_status,(LPCTSTR)(_bstr_t)record->GetCollect("card_status"));
	if (strcmp(card_status,"02")==0) //����Ѿ�����
	{
		return 1;
	}
	else
	{
	
		return 0;
	}


	return 0; 
}

//
int CDB::setdgtResubmit(char *centerseqno,char *unSubmitField)
{
	int index;
	char tmp[1024];
	_bstr_t vSQL;
	_RecordsetPtr record;
	
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;
	bbzero(tmp);
	sprintf(tmp, "update dgt_transfer_log t  set t.resubmit='%s' where t.centerseqno='%s'",unSubmitField, centerseqno);
	wrLog("���ڸ����ύ����SQL:%s",tmp);
	vSQL = tmp;
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("�����������ݿ�ʧ��");
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	g_adoConnInfo[index].connStat = 0;
		return 0;


}


//��ȡ��һ��δǩ����Ϣ

//���¸����ε�ǩ������ �ۿ��ͳ�ֵ�ɹ�֮����Ҫ����ͳ��
int CDB::update_dgtSignData( DGTSignInfo dgtSignInfo,char *term_id)
{
	
	int index;
	_bstr_t vSQL;
	_RecordsetPtr record;
	char sql[300];

	wrLog("���½��׽��%");

	index = getConnStat();  //��ȡ��������
	
	if( index<0 )
		return -1;
	
	bbzero(sql);
	sprintf(sql, "update dgt_sign_log set TOTALSVNUM = '%s', TOTALSVAMT ='%s' ,TOTALSALEDEP='%s' where BATCHNO = '%s' and term_id='%s' and LOG_SEQ='%s' ",dgtSignInfo.TOTALSVNUM,dgtSignInfo.TOTALSVAMT,dgtSignInfo.TOTALSALEDEP,dgtSignInfo.BATCHNO,term_id,dgtSignInfo.LOG_SEQ);

	vSQL = sql;
	wrLog("sql[%s]",sql);
	if( g_adoConnInfo[index].adoConn.ExecuteSQL(vSQL) )
	{
		wrLog("���¶�ݸͨǩ����Ϣʧ��sql[%s]",sql);
		g_adoConnInfo[index].connStat = 2;
		return -1;
	}
	
	g_adoConnInfo[index].connStat = 0;
	
	return 0;
	
}


//��ݸ���������ͳ�ֵ���ף�û��ȷ�ϵ�״̬�����ύ ע���ύʱ������ ����
int CDB::getdgtUnsubmit( CGlobal *pgbl,char * biz_type,int tradeFlag,int *tradeStatus)
{

	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];
	char YJ_AMO[8+1];
	char YJ_AMO_TEMP[4+1];
	char deposit[4+1];
	char city_code[4+1];
	char card_parent_type[2+1];
	char card_child_type[2+1];
	char CARDVALDATE[8+1];
	char tradeDate[8+1];
	char tradeTime[6+1];


	bbzero(tradeDate);
	bbzero(tradeTime);
	bbzero(biz_type);

	bbzero(YJ_AMO);
	bbzero(YJ_AMO_TEMP);
	bbzero(city_code);
	bbzero(card_child_type);
	bbzero(card_parent_type);
	bbzero(CARDVALDATE);


	wrLog("��ȡ���һ�ʶ�ݸͨ�������");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	switch(tradeFlag)
	{
		case 0:
			bbzero(tmp);
			sprintf(tmp,"select * from ( select * from dgt_txn_log t where t.term_id='%s' and bus_biz='0372' and t.status not in ('1','2')  and bus_card='%s' order by t.bus_date desc,t.bus_time desc) where rownum=1",pgbl->dPosInfo.pos_no,pgbl->dgtCard.cardShowId);
			vSQL = tmp;
			wrLog("sql: %s",tmp);
			break;
		case 2:
			bbzero(tmp);
			sprintf(tmp,"select * from ( select * from dgt_txn_log t where t.term_id='%s' and t.status not in ('1','2') order by t.bus_date desc,t.bus_time desc) where rownum=1",pgbl->dPosInfo.pos_no);
			vSQL = tmp;
			wrLog("sql: %s",tmp);
			break;
		default:
			bbzero(tmp);
			sprintf(tmp,"select * from ( select * from dgt_txn_log t where t.term_id='%s' and t.status not in ('1','2') order by t.bus_date desc,t.bus_time desc) where rownum=1",pgbl->dPosInfo.pos_no);
			vSQL = tmp;
			wrLog("sql: %s",tmp);
	}

	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;

	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}


	*tradeStatus=atoi((LPCTSTR)(_bstr_t)record->GetCollect("STATUS"));
	
	if((record->GetCollect("YJ_AMO")).vt!=VT_NULL) //�����ֶ�
	{
		
		bbzero(YJ_AMO);
		strcpy(YJ_AMO,(LPCTSTR)(_bstr_t)record->GetCollect("YJ_AMO")); //����״̬
		
		bbzero(YJ_AMO_TEMP);
		memcpy(YJ_AMO_TEMP,YJ_AMO+4,4);
		
		sprintf(deposit,"FF%d",atoi(YJ_AMO_TEMP)/100);
		bbzero(pgbl->dgtCard.deposit);
		CTools::hex_to_byte((BYTE *) deposit,(BYTE *) pgbl->dgtCard.deposit,4);
	}
	else
	{
	
		if (*tradeStatus!=0 ) //���������г���д����ʱ
		{
			return -5;
		}
	
	}

	//

	//��ȡ��������
	if((record->GetCollect("bus_biz")).vt!=VT_NULL) //�����ֶ�
	{
		
		bbzero(biz_type);
		strcpy(biz_type,(LPCTSTR)(_bstr_t)record->GetCollect("bus_biz")); //��������
	}
	else
	{
		
		return -5; //��������
	}

	if((record->GetCollect("BUS_DATE")).vt!=VT_NULL) //�����ֶ�
	{
		bbzero(pgbl->dgtCard.tradeDate);
		strcpy(pgbl->dgtCard.tradeDate,(LPCTSTR)(_bstr_t)record->GetCollect("BUS_DATE")); 
	}
	else
	{
		if (*tradeStatus!=0) 
		{

			return -5;
		}
	
	
	}
	

	if((record->GetCollect("BUS_TIME")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.tradeTime);
		strcpy(pgbl->dgtCard.tradeTime,(LPCTSTR)(_bstr_t)record->GetCollect("BUS_TIME")); 

	}
	else
	{
	
		if (*tradeStatus!=0)
		{
			return -5;
		}

	}

	if((record->GetCollect("CITYCODE")).vt!=VT_NULL) //�����ֶ�
	{
		bbzero(pgbl->dgtCard.CItyCode);
		bbzero(city_code);
		strcpy(city_code,(LPCTSTR)(_bstr_t)record->GetCollect("CITYCODE")); 
		CTools::hex_to_byte((BYTE *) city_code,(BYTE *)pgbl->dgtCard.CItyCode,4);
	}
	else
	{
		if (*tradeStatus!=0 )
		{
			return -5;
		}
	}


	bbzero(pgbl->dgtCard.MESSAGEDATETIME);
	sprintf(pgbl->dgtCard.MESSAGEDATETIME,"%s%s",pgbl->dgtCard.tradeDate,pgbl->dgtCard.tradeTime);


	if((record->GetCollect("CARD_PARENTTYPE")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.cardParent_type);
		strcpy(card_parent_type,(LPCTSTR)(_bstr_t)record->GetCollect("CARD_PARENTTYPE")); 
		CTools::hex_to_byte((BYTE *) card_parent_type,(BYTE *) pgbl->dgtCard.cardParent_type,2);
	}
	else
	{
	
		if (*tradeStatus!=0)
		{
			return -5;
		}
	}

	if((record->GetCollect("CARD_CHILDYPE")).vt!=VT_NULL) //�����ֶ�
	{
		
		bbzero(pgbl->dgtCard.cardChild_type);
		strcpy(card_child_type,(LPCTSTR)(_bstr_t)record->GetCollect("CARD_CHILDYPE")); 
		CTools::hex_to_byte((BYTE *) card_child_type,(BYTE *) pgbl->dgtCard.cardChild_type,2);

	}
	else
	{
		if (*tradeStatus!=0)
		{
			return -5;
		}
	
	}

	if((record->GetCollect("BUS_CARD")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.cardShowId);
		strcpy(pgbl->dgtCard.cardShowId,(LPCTSTR)(_bstr_t)record->GetCollect("BUS_CARD")); 
	}
	else
	{
		if (*tradeStatus!=0)
		{
			return -5;
		}


	}

	if((record->GetCollect("BATCHNO")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.BATCHNO);
		strcpy(pgbl->dgtCard.BATCHNO,(LPCTSTR)(_bstr_t)record->GetCollect("BATCHNO"));  
	}
	else
	{
		if (*tradeStatus!=0)
		{
			return -5;
		}
	
	}

	if((record->GetCollect("OLD_AMO")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.tradeBeForeMny);
		strcpy(pgbl->dgtCard.tradeBeForeMny,(LPCTSTR)(_bstr_t)record->GetCollect("OLD_AMO"));  

	}
	else
	{
		if (*tradeStatus!=0)
		{
			return -5;
		}
	}

	if((record->GetCollect("PAY_AMO")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.tradeMny);
		strcpy(pgbl->dgtCard.tradeMny,(LPCTSTR)(_bstr_t)record->GetCollect("PAY_AMO"));  
		
		bbzero(pgbl->amount);
		sprintf(pgbl->amount,"%012s",pgbl->dgtCard.tradeMny);
	}
	else
	{
	
		if (*tradeStatus!=0)
		{
			return -5;
		}

	}

	if((record->GetCollect("APTRANSSEQ")).vt!=VT_NULL) //�����ֶ�
	{
		bbzero(pgbl->TRANSSEQ);
		strcpy(pgbl->TRANSSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("APTRANSSEQ"));  
	
	}
	else
	{
		if (*tradeStatus!=0)
		{
			return -5;
		}
	}

	if((record->GetCollect("STAN")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->SYSTEMNO);
		strcpy(pgbl->SYSTEMNO,(LPCTSTR)(_bstr_t)record->GetCollect("STAN"));  
	}
	else
	{
		return -5;//��������
	}

	bbzero(pgbl->dgtCard.KEYSET);
	strcpy(pgbl->dgtCard.KEYSET,pgbl->dgtSignInfo.KEYSET);
	//strcpy(pgbl->dgtCard.KEYSET,(LPCTSTR)(_bstr_t)record->GetCollect("KEYSET"));  
	
	if((record->GetCollect("AUTHSEQ")).vt!=VT_NULL) //�����ֶ�
	{
		bbzero(pgbl->dgtCard.AUTHSEQ);
		strcpy(pgbl->dgtCard.AUTHSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("AUTHSEQ"));  

	}
	else
	{
	
		if (*tradeStatus!=0)
		{
			return -5;
		}

	}

	if((record->GetCollect("LIMITEDAUTHSEQL")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.LIMITEAUTHSEQL);
		strcpy(pgbl->dgtCard.LIMITEAUTHSEQL,(LPCTSTR)(_bstr_t)record->GetCollect("LIMITEDAUTHSEQL"));  
	}
	else
	{
		if (*tradeStatus!=0)
		{
			return -5;
		}

	}

	if((record->GetCollect("POSSEQ")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.posSeq);
		strcpy(pgbl->dgtCard.posSeq,(LPCTSTR)(_bstr_t)record->GetCollect("POSSEQ"));  

	}
	else
	{
	
		return -5;
	}

	if((record->GetCollect("SETTIME")).vt!=VT_NULL) //�����ֶ�
	{
	
		bbzero(pgbl->dgtCard.SETDATE);
		strcpy(pgbl->dgtCard.SETDATE,(LPCTSTR)(_bstr_t)record->GetCollect("SETTIME"));  

	}
	else
	{
	
		if (*tradeStatus!=0)
		{
			return -5;
		}

	}

	if((record->GetCollect("CARDRANDOM")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->dgtCard.CARDRAND);
		strcpy(pgbl->dgtCard.CARDRAND,(LPCTSTR)(_bstr_t)record->GetCollect("CARDRANDOM"));
	}
	
	else //���������
	{
		bbzero(pgbl->dgtCard.CARDRAND);
		strcpy(pgbl->dgtCard.CARDRAND,"00000000");
	}

	if((record->GetCollect("CARDSEQ")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->dgtCard.CARDCNT);
		strcpy(pgbl->dgtCard.CARDCNT,(LPCTSTR)(_bstr_t)record->GetCollect("CARDSEQ"));
	}
	
	else //���������
	{
		bbzero(pgbl->dgtCard.CARDCNT);
		strcpy(pgbl->dgtCard.CARDCNT,"000000");
	}

	
	if((record->GetCollect("LASTPOSSVSEQ")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->dgtCard.lastPosSeq);
		strcpy(pgbl->dgtCard.lastPosSeq,(LPCTSTR)(_bstr_t)record->GetCollect("LASTPOSSVSEQ"));
	}
	
	else //���������
	{
		bbzero(pgbl->dgtCard.lastPosSeq);
		strcpy(pgbl->dgtCard.lastPosSeq,"000000000");
	}

	if((record->GetCollect("TAC")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->dgtCard.TAC);
		strcpy(pgbl->dgtCard.TAC,(LPCTSTR)(_bstr_t)record->GetCollect("TAC"));
	}
	
	else //���������
	{
		bbzero(pgbl->dgtCard.TAC);
		strcpy(pgbl->dgtCard.TAC,"00000000");
	}
	

	if((record->GetCollect("TAC")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->dgtCard.TAC);
		strcpy(pgbl->dgtCard.TAC,(LPCTSTR)(_bstr_t)record->GetCollect("TAC"));
	}
	
	else //���������
	{
		bbzero(pgbl->dgtCard.TAC);
		strcpy(pgbl->dgtCard.TAC,"00000000");
	}
	
	if((record->GetCollect("CARDVALDATE")).vt!=VT_NULL)
	{
		
		bbzero(pgbl->dgtCard.effectiveDate);
		bbzero(CARDVALDATE);
		strcpy(CARDVALDATE,(LPCTSTR)(_bstr_t)record->GetCollect("CARDVALDATE"));
		CTools::hex_to_byte((BYTE *) CARDVALDATE,(BYTE *)pgbl->dgtCard.effectiveDate,8);

	}
	
	else //���������
	{
		bbzero(pgbl->dgtCard.effectiveDate);
		CTools::hex_to_byte((BYTE *) "20901231",(BYTE *)pgbl->dgtCard.effectiveDate,8);

	}
	
	if(*tradeStatus==0) //������ײ���ȷ����ȡkeep ֵ�����keepֵ�����ڣ�����Ҫ����
	{
	
		if((record->GetCollect("KEEP")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgtCard.KEEP);
			strcpy(pgbl->dgtCard.KEEP,(LPCTSTR)(_bstr_t)record->GetCollect("KEEP"));
			
		}
		
		else //���������
		{
			return -5;
			
		}
	
	}

	g_adoConnInfo[index].connStat = 0;
	return 0;

}


//��ȡ���һ�ʽ���û���ύ�����
int CDB::getUnsubmitTransfer( CGlobal *pgbl,char *tradeStatus,char * orderStatus,char *unSubmitField)
{

	int ret, index;
	_bstr_t vSQL;
	_RecordsetPtr record;
    char tmp[1024];

	wrLog("��ȡ�Ծɻ������һ�ʶ�ݸͨ�������");
	index = getConnStat();  //��ȡ��������
	if( index<0 )
		return -1;

	bbzero(tmp);
	sprintf(tmp,"select * from (select STATUS,CENTERSEQNO,SYSORDER, POSSERIAL,RESUBMIT ,order_status,m1_card,monthfeeflag,feeamtflag,\
		cpu_card,returnamt ,bus_date, bus_time ,m1_orig_mny, feeamt ,returndeposit ,monthfeeamt  from \
		dgt_transfer_log where status in ('R01','R02','R03','S02','S03','S01','C0S') \
		or order_status in ('S0X','S0F') and term_id ='%s' order by dgt_seq desc ) where rownum=1",pgbl->pos_no);
	vSQL = tmp;
	wrLog("sql: %s",tmp);
	record = g_adoConnInfo[index].adoConn.GetRecordSet(vSQL, ret);
	if(ret)
	{
		g_adoConnInfo[index].connStat = 2;
		return -2;

	}
	if(record->adoEOF)
	{
		g_adoConnInfo[index].connStat = 0;
		return -3;
	}

	//������ˮ��

	bbzero(tradeStatus);
	bbzero(orderStatus);
	strcpy(tradeStatus,(LPCTSTR)(_bstr_t)record->GetCollect("STATUS")); //����״̬
	bbzero(pgbl->Centerseqno);
	strcpy(pgbl->Centerseqno,(LPCTSTR)(_bstr_t)record->GetCollect("CENTERSEQNO")); //������ˮ��

	bbzero(orderStatus);
	strcpy(orderStatus,(LPCTSTR)(_bstr_t)record->GetCollect("order_status")); //����״̬

	if(strcmp(orderStatus,"S0X") ==0 ) //���û�����ɶ���
	{

		bbzero(pgbl->dgtCard.cardMny);
		strcpy(pgbl->dgtCard.cardMny,"000000000000");
		if((record->GetCollect("POSSERIAL")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgt_posSerial);
			strcpy(pgbl->dgt_posSerial,(LPCTSTR)(_bstr_t)record->GetCollect("POSSERIAL"));
		}
		
		else //���������
			bbzero(pgbl->dgt_posSerial);

		if((record->GetCollect("m1_card")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgtM1Card.bus_showCard);
			strcpy(pgbl->dgtM1Card.bus_showCard,(LPCTSTR)(_bstr_t)record->GetCollect("m1_card"));
		}
		
		else //���������
			bbzero(pgbl->dgtM1Card.bus_showCard);

		if((record->GetCollect("cpu_card")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgtCard.cardShowId);
			strcpy(pgbl->dgtCard.cardShowId,(LPCTSTR)(_bstr_t)record->GetCollect("cpu_card"));
		}
		
		else //���������
			bbzero(pgbl->dgtCard.cardShowId);
		if((record->GetCollect("bus_date")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgtCard.tradeDate);
			strcpy(pgbl->dgtCard.tradeDate,(LPCTSTR)(_bstr_t)record->GetCollect("bus_date"));
		}
		
		else //���������
			bbzero(pgbl->dgtCard.tradeDate);

		if((record->GetCollect("bus_time")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgtCard.tradeTime);
			strcpy(pgbl->dgtCard.tradeTime,(LPCTSTR)(_bstr_t)record->GetCollect("bus_time"));
		}
		
		else //���������
			bbzero(pgbl->dgtCard.tradeTime);

		if((record->GetCollect("returnamt")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->ReturnAmt);
			strcpy(pgbl->ReturnAmt,(LPCTSTR)(_bstr_t)record->GetCollect("returnamt"));

		}
		
		else //���������
			bbzero(pgbl->ReturnAmt);

		if((record->GetCollect("m1_orig_mny")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgtM1Card.bus_amt);
			strcpy(pgbl->dgtM1Card.bus_amt,(LPCTSTR)(_bstr_t)record->GetCollect("m1_orig_mny"));
			
		}
		
		else //���������
			bbzero(pgbl->dgtM1Card.bus_amt);

		if((record->GetCollect("feeamt")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->FeeAmt);
			strcpy(pgbl->FeeAmt,(LPCTSTR)(_bstr_t)record->GetCollect("feeamt"));
			
		}
		
		else //���������
			bbzero(pgbl->FeeAmt);

		if((record->GetCollect("returndeposit")).vt!=VT_NULL)
		{
			
			char deposit[4+1];
			bbzero(deposit);
			bbzero(pgbl->ReturnDeposit); //Ѻ��
			strcpy(pgbl->ReturnDeposit,(LPCTSTR)(_bstr_t)record->GetCollect("returndeposit"));
			sprintf(deposit,"FF%d",atoi(pgbl->ReturnDeposit) /100) ;
			bbzero(pgbl->dgtCard.deposit);
			CTools::hex_to_byte((BYTE *) deposit,(BYTE *)pgbl->dgtCard.deposit,4);
			
		}
		
		else //���������
			bbzero(pgbl->ReturnDeposit);

		if((record->GetCollect("monthfeeflag")).vt!=VT_NULL)
		{
			
			strcpy(pgbl->MonthFeeFlag,(LPCTSTR)(_bstr_t)record->GetCollect("monthfeeflag"));
			
		}
		
		else //���������
			bbzero(pgbl->MonthFeeFlag);

		if((record->GetCollect("feeamtflag")).vt!=VT_NULL)
		{
			
			strcpy(pgbl->FeeAmtFlag,(LPCTSTR)(_bstr_t)record->GetCollect("feeamtflag"));
			
		}
		
		else //���������
			bbzero(pgbl->FeeAmtFlag);
		if((record->GetCollect("monthfeeamt")).vt!=VT_NULL)
		{
			
			strcpy(pgbl->MonthFeeAmt,(LPCTSTR)(_bstr_t)record->GetCollect("monthfeeamt"));
			
		}
		
		else //���������
			bbzero(pgbl->MonthFeeAmt);

	}

	if (strcmp(orderStatus,"S0F") == 0 ) //�������ȷ�ϳɹ�
	{
		if((record->GetCollect("SYSORDER")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->TRANSSEQ);
			strcpy(pgbl->TRANSSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("SYSORDER"));
		}
		
		else //���������
			bbzero(pgbl->TRANSSEQ);
		
		if((record->GetCollect("POSSERIAL")).vt!=VT_NULL)
		{
			
			bbzero(pgbl->dgt_posSerial);
			strcpy(pgbl->dgt_posSerial,(LPCTSTR)(_bstr_t)record->GetCollect("POSSERIAL"));
		}
		
		else //���������
			bbzero(pgbl->dgt_posSerial);

	}

	if (strcmp(tradeStatus,"S01")==0 || strcmp(tradeStatus,"R01")==0 || strcmp(tradeStatus,"C0S") ==0  ) //�ɹ������ύ
	{

		if((record->GetCollect("RESUBMIT")).vt!=VT_NULL)
		{

			bbzero(unSubmitField);
			strcpy(unSubmitField,(LPCTSTR)(_bstr_t)record->GetCollect("RESUBMIT"));
		}
		
		else //���������
			bbzero(unSubmitField);
	}
	else if (strcmp(tradeStatus,"S03") ==0 || strcmp(tradeStatus,"R03") ==0 ) //�����û��ȷ�ϳɹ�
	{

		if((record->GetCollect("SYSORDER")).vt!=VT_NULL)
		{

			bbzero(pgbl->TRANSSEQ);
			strcpy(pgbl->TRANSSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("SYSORDER"));
		}
		
		else //���������
			bbzero(pgbl->TRANSSEQ);
		
		if((record->GetCollect("POSSERIAL")).vt!=VT_NULL)
		{

			bbzero(pgbl->dgt_posSerial);
			strcpy(pgbl->dgt_posSerial,(LPCTSTR)(_bstr_t)record->GetCollect("POSSERIAL"));
		}
		
		else //���������
			bbzero(pgbl->dgt_posSerial);

		if((record->GetCollect("RESUBMIT")).vt!=VT_NULL)
		{

			bbzero(unSubmitField);
			strcpy(unSubmitField,(LPCTSTR)(_bstr_t)record->GetCollect("RESUBMIT"));
		}
	
		else //���������
			bbzero(unSubmitField);

	}
	else if (strcmp(tradeStatus,"S02")==0 || strcmp(tradeStatus,"R02") ==0 ) //�����ҵ�˻�û��ȷ�ϳɹ�
	{
		
		if((record->GetCollect("SYSORDER")).vt!=VT_NULL)
		{

			bbzero(pgbl->TRANSSEQ);
			strcpy(pgbl->TRANSSEQ,(LPCTSTR)(_bstr_t)record->GetCollect("SYSORDER"));
		}
		
		else //���������
			bbzero(pgbl->TRANSSEQ);

		if((record->GetCollect("POSSERIAL")).vt!=VT_NULL)
		{

			bbzero(pgbl->dgt_posSerial);
			strcpy(pgbl->dgt_posSerial,(LPCTSTR)(_bstr_t)record->GetCollect("POSSERIAL"));
		}
		
		else //���������
			bbzero(pgbl->dgt_posSerial);

	}
		


	g_adoConnInfo[index].connStat = 0;
	return 0;


}