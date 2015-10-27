/************************************************************************/
//羊城通充值第三代                                                     
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

	//羊城通充值第三代进入交易前的准备
	//包括 读卡器初始化、终端签到认证、发送监控配置
	int static Yct3_Init(CGlobal gbl,YCT_DATA yct_data);

	//主函数
	//tradeType: 0:自动充值 1:余额转移 2:充值撤销 3:卡片扩展应用
	// 返回值：
	// 0：成功
	// 1：失败
	int static Yct3_TradeHdl(CGlobal gbl,YCT_DATA yct_data, char *title, int tradeType);


};