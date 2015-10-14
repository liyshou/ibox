#define DEVICE_GTRCI100			1 //国腾
#define DEVICE_IDR210			2 //精伦

//--------------------------------------------------------
//错误码
//公用
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

//国腾
#define ERR_PHOTO_DECODE			50	//相片解码错误
#define ERR_WLT_FILE_EXT			51	//wlt文件后缀错误
#define ERR_WLT_FILE_OPEN			52	//wlt文件打开错误
#define ERR_WLT_FILE_FORMAT			53	//wlt文件格式错误
#define ERR_SOFTWARE_UNAUTHORIZED	54	//软件未授权
#define ERR_DEVICE_CONNECT			55	//设备连接失败

//精伦
#define ERR_FOLDER_EXIST			100	//目录不存在

//错误码
//--------------------------------------------------------