/************************************************************************/
//���ͨ��ֵ������                                                     
//author:        Wudp                                                        
//create date:   2012-06-27                                                    
//version:       1.0.0  
//modified:                                                        
/************************************************************************/
#include "YCTYPTrade.h"

class CYCT3Trade:public CYCTYPTrade
{
public:
	CYCT3Trade();
	virtual ~CYCT3Trade();

	//���ͨ��ֵ���������뽻��ǰ��׼��
	//���� ��������ʼ�����ն�ǩ����֤�����ͼ������
	int static Yct3_Init(CGlobal gbl,YCT_DATA yct_data);

	//������
	//tradeType: 0:�Զ���ֵ 1:���ת�� 2:��ֵ���� 3:��Ƭ��չӦ��
	// ����ֵ��
	// 0���ɹ�
	// 1��ʧ��
	int static Yct3_TradeHdl(CGlobal gbl,YCT_DATA yct_data, char *title, int tradeType);


};