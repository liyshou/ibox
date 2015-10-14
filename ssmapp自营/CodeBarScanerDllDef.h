#pragma once

#define DEVICE_LV1000R			1 //一维码扫描仪
#define DEVICE_LV420R			2 //2维码扫描仪

#define MAX_RESPONSE_LENGTH 1024

#define WORKMODE_AUTO				1 //自动识读模式
#define WORKMODE_INTERMITTENCE		2 //间歇识读模式
#define WORKMODE_INDUCTION			3 //感应识读模式
#define WORKMODE_CONTINUATION		4 //连续识读模式
#define WORKMODE_DELAY_INDUCTION	5 //延迟式感应识读模式
#define WORKMODE_COMMAND			6 //命令触发识读模式

#define LOOP_RUNNING		0
#define LOOP_PAUSE			1
#define LOOP_END			2
#define LOOP_WAIT_PAUSE		3
#define	LOOP_WAIT_CONTINUE	4
#define LOOP_WAIT_END		5

#define ERR_UNKNOWN					-1
#define ERR_OK						0
#define ERR_UNKNOWN_DEVICE			1
#define ERR_INVALID_PORT			2
#define ERR_ALREADY_SET_TYPE		3	//重复设置设备类型
#define ERR_EXECUTE					4	//执行失败
#define ERR_COM						5	//串口错
#define ERR_ABORT					6	//中断
#define ERR_COMMUNICATION			7	//通讯失败
#define ERR_PARAMETER				8	//参数错误
#define ERR_UNSUPPORT				9	//命令不能执行
#define ERR_CARDTYPE				10	//错误的卡
#define ERR_DLL						11	//加载动态库失败
#define ERR_DLLDIRECTORY			12	//设置动态库目录失败
#define ERR_INOPERATE				13	//执行上一个操作中
#define ERR_TIMEOUT					14	//超时
#define ERR_BUFFER_SMALL			15	//缓冲区太小
#define ERR_VARIFY					16	//数据校验失败
#define ERR_LRC						17	//校验LRC失败