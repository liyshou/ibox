// ssmApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ssmApp.h"
#include "ssmAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CSsmAppApp

BEGIN_MESSAGE_MAP(CSsmAppApp, CWinApp)
	//{{AFX_MSG_MAP(CSsmAppApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSsmAppApp construction

CSsmAppApp::CSsmAppApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	g_hTrayWnd=NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSsmAppApp object

CSsmAppApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSsmAppApp initialization
//升级历史
/*
升级必改：
KTViewFields.Atm_Ver，资源Version，启动打印版本号。

Ver3.3.3
001. 加入状态监控功能。以KTView3.6.9为服务器，报文版本为1.0.4。
002. 加入KTView_dog(1.0.8)远程控制功能。支持远程提取日志、远程升级、远程开关程序。
003. 整理日志系统。将日志分为交易日志(logyyyymmdd)、监控日志(ktviewlogyyyymmdd)、通信日志(commyyyymmdd)三种。全部统一放在LOG目录下。
004. 将发布文件制作成自动安装包，可选有无装备存折补登机。据此决定使用不同的flash(main.swf)文件。区别在于界面有没有“存折交易”项。
005. 发布新的参数配置工具。可配置sysconfig.ini文件和KTView_dog所需的注册表项。
006. 代码中加入版本。
					黄嘉亮 2009.11.12
Ver3.3.5
007. 修正补登机端口设置为0还初始化的问题。
008. 修正当打印凭条而凭条机故障时不能返回主界面的问题。
					黄嘉亮 2009.11.17

009. 修正读取补登机端口错误导致初始化失败的问题。
010. 修正初始化后即发送状态包，检测凭条机时间过长，导致不能响应flash申请密钥命令的问题。
					黄嘉亮 2009.11.18

011. 检查log、trandata、tracedata目录，无则增加。
012. 所有硬件状态按接口返回原样上送，不作处理。错误信息在KTView服务器端配置。
013. 每次打印凭条前重新初始化凭条机，否则获取状态不正常。
					黄嘉亮 2009.11.19

014. 凭条机少纸为警告状态。
015. 读卡器驱动返回状态1为正常，0为失败。
					黄嘉亮 2009.11.20

Ver3.5.1.0	2010.03.13
016.全省程序统一版本。flashfile\bak目录里保存供选择的文件：有1秒退出或10秒退出、东莞或其他地方可提供的交易种类、有无存折补登机。
017.修改：不能打印凭条则不能提供交易。
018.修改：关闭程序密码由固定的“912912”改成可配置。
019.修正：打印存折异常造成程序不能退出的问题。
020.修改：PassBookErrPro()里不退折。ExecFunc()里的EjectCard和EjectBook流程退折。
					
Ver3.5.1.1	2010.03.19
021.修改：原设计交易中不发状态包，交易量大的机器可能会因为长时间不发包，监控端报离线。修改成定时发包，交易中不获取硬件状态。
					
Ver3.5.1.2	2010.03.22
022.修改：支持新版存折，去掉后三位校验码，“>”改成“=”。
023.增加：版本号增加到4位。前三位对外版本号，第四位内部版本号。

Ver3.5.1.3	2010.03.23
024.修改：可配置是否上送PVN（DeletePVN）。
025.修改：全省除东莞外没有监控，KTViewInterval默认值0，即不发送。
026.修改：TraceFlag，OPTimeOut，ShutPassWd，ExitPassword加上默认值。

Ver3.5.1.4	2010.03.24
027.增加：监控状态报文ATM版本域增加银行地址。

Ver3.5.1.5	2010.04.07
028.更正：监控状态报文ATM版本域3.5.1.4误为3.1.5.4。
029.修改：凭条机少纸，不能交易。
030.修正：无补登项不能退折。

Ver3.5.1.6	2010.04.22
031.更正：存折读磁失败或者进白纸，3磁为空仍然发送补登请求到P端。P端无响应，不退折。增加校验3磁长度，少于30位看作无效折。

Ver3.5.1.7	2010.05.12
032.修改：设置刷卡器默认端口为1。
033.增加：刷卡失败时重新初始化刷卡器。

Ver3.5.1.8	2010.05.12
034.修改：进折检测到存折磁道信息无效，发送指令给flash后增加一秒钟延时。防止flash下发指令太快造成退折失败。

Ver3.5.1.9	2010.05.13
035.修正：main.swf改动。退折指令后增加一秒延时，然后下发进折指令。防止退折没完成又进折，造成不退折的问题。

Ver3.5.2.0  2010.05.14
036.修改：存折3磁少于30位时，提示信息改为：“存折补登不成功,请到柜台查询办理!”。

Ver3.5.2.1  2010.05.17
037.修改：存折补登失败，不论存折或是补登机的问题，统一提示信息为“存折补登不成功,请到柜台查询或办理!”。具体错误原因打印到电子日志。
038.修改：更新main.swf。支持补登失败直接退折，不再重试三次。
039.修改：更新yewu_card.swf。增加非税业务界面。
040.修改：更新yewu_card.xml。增加非税业务按钮。具体为在缴费下增加一项：<button code="102" codeTxt="102 非税" lateFee="yes" />。用记事本直接编辑亦可。 
041.修改：更新MainTran.dll。增加非税业务报文。
  
Ver3.5.2.2  2010.06.09
042.初始化刷卡器后打印设备状态到日志。
043.修正打印凭条的流程中一个分支没有返回操作结果给flash。
044.仅检测凭条机状态，并不打印凭条的时候，提示信息原为“正在给您打印交易转账凭条,请稍候...”，改为“凭条机状态正常.”
045.修正补登时，除成功补登和无未登折项之外的情况会导致flash无响应，直到超时。
046.修正记录日志串行的问题。
047.存折补登机正常状态误报故障到监控。
048.汉化部分日志。
049.验证平台交易码的合法性，修正"业务配置文件不存在"的问题。
050.同时发布"8583报文解析器"和"状态监控报文解析器"。

Ver3.5.2.3  2010.06.23
051.复位刷卡器前先关闭端口。

Ver3.5.2.4  2010.07.05
052.计算工作密钥前校验主机返回随机数密匙的合法性，防止主机不下发密匙造成程序崩溃。

Ver3.5.2.5  2010.07.09
053.存折补登流程增加多处对异常情况的处理，防止程序崩溃。

Ver3.5.2.6  2010.07.13
054.存折补登交易恢复3524取消掉的重计算工作密钥。

Ver3.5.2.7  2010.07.15
055.删除无用的信号量。
056.创建刷卡线程后不关闭句柄，防止崩溃。
057.刷卡线程里的goto改成for循环。去掉控制信号量。限制最大尝试十次。只对-1错误作复位。

Ver3.5.2.8  2010.08.20
058.增加便民查询功能。
059.升级yewucard.swf，超时不重新下载密钥。
060.重写存折补登流程。打印参数可配置。
061.主界面的“存折”按钮可在main.xml中配置。

Ver3.5.2.9  2010.09.20
062.修正:存折补登,互掉未打印总行数和此次下载行数。

Ver3.5.3.0  2010.09.28
063.修正:把启动TranProThread移到flash初始化之后。
        解决TranProThread不初始化、TranProDlg不能最顶、打印机初始化失败的问题。
		怀疑跟flash下发消息、按键输入错乱也有关系。

Ver3.5.3.1  2010.11.15
064.把存折补登第一个参数理解为已打印行数(文档上是当前打印行)，下一行才是当前打印行。效果是比修改前下移一行。

Ver3.5.3.2  2010.10.25
065.TRSHOW等五个函数，参数类型char*，传地址而非值。PostMessage()有延时，在处理之前该变量被修改或被释放等将导致错误。
	不传地址，改为把字符串放入队列中。
066.增加:邮政自助查询（IE网页）。
067.日志时间精确到毫秒。
068.关闭键盘后马上打开会自动输入密码。因关闭键盘后，键盘会输出7个字符（在清空缓冲区？），每0.125秒一个。
069.加密tracelog，另附解密工具。
070.启动自动隐藏任务栏，退出恢复。
071.启动不打印凭条。
072.设置当前工作目录。
073.交易前先清空遗留交易数据。
074.修改maintran，区分两种类型交易的标识码，便于解析器分析。
075.ad.xml中可配置是否有抽奖交易，适应东莞以外地区。
076.修正在Administrator下不能关机和重启的问题。
077.flash:刷卡超时后下发CancelCard。
078.自动补登功能。

Ver3.5.3.3  2011.05.01
079.软加密。
080.代缴费默认缴费改为充值。
081.修复重新下载密钥时不能关闭抽奖功能。
082.键盘长开。

Ver3.5.3.4  2011.06.03
083.修正几个字符串数组空间太小导致溢出，改用CString。
084.升级到VS2010。

Ver3.5.3.5  2011.06.28
085.更换东莞新界面flash。
086.汇款成功后接着做查询余额交易，此处不清空密码。密码的复位由每次输入密码前的ClearPin指令来保证。
087.增加资费查询，用网页形式显示图片。
089.版本号在stdafx定义g_AppVersion。
090.增加TranProDlg的OnOK、OnCancle处理函数。防止网页查询中客户按键盘确认，dlg退出。


-----------------------
中国建设银行自助发卡机
在邮政3.5.3.5基础上修改而来
-----------------------
Ver1.0.0.1  2011.10.09 黄嘉亮
001.删除大量无用代码。
002.增加发卡机、扫描仪、身份证读卡器、指纹仪。
003.整合通信部分，删除MainTran。

-----------------------
中国建设银行自助填单机
在中国建设银行自助发卡机1.0.0.1基础上修改而来
-----------------------
Ver1.0.0.0  2012 黄嘉亮
001.初始创立

Ver1.0.0.1  2012.04.26 黄嘉亮
002.重新整理表格输入项，界面不再照搬原表格样式。
003.增加十几个表格。

Ver1.0.0.2  2013.05.08 黄嘉亮
004.增加百惠龙卡开户。
005.修正几个管理员菜单bug。

Ver1.0.0.3  2013.06.19 黄嘉亮
005.历史记录。
006.修正表格几个bug。
007.管理菜单。
-----------------------

-----------------------
IBOX
在建行填单机1.0.0.3基础上修改而来
-----------------------
Ver1.0.0.1  2013.07.10 黄嘉亮
基本功能

Ver1.0.0.2  2014.02.27 杨建雄
支持协达后台系统。

Ver1.0.0.3  2014.02.27 杨建雄
1.支持邮政后台系统
2.修改密码加密方式（des加密）
3.优化界面操作方式（投递失败时，不直接退出，改为重新投放）   

Ver1.0.0.4  2014.03.10 杨建雄
1.修改硬件状态报文上送的条件，改为在空闲状态下定时上送。
2.修改故障时的界面提示语句。 
3.增加日志打印，在日志上明确显示交易类型。
4.修改上送的箱状态，调硬件接口失败或门开时不置为故障。
5.修改投递时可用箱的判断条件，门开时也判为可用。
6.投递确认界面超时，当作确认处理。

Ver1.0.0.5  2014.03.17 黄嘉亮
1.修改硬件状态报文上送的条件，超时后进入空闲状态马上发送，而不是等到下一个超时。
2.修改故障时的界面提示语句。 
3.屏蔽“修改管理员密码”功能。
4.明文报文记录在TraceLog里。
5.唯一化错误提示信息。
Flash修改内容：
1.后台“周转箱”与“修改密码”按钮，改为配置显示，默认不显示。
2.确认投递超时，增加下发ExecFunc_ConfirmDeposit。
3.ExecFunc_ConfirmDeposit响应“00”不重新开箱。
4.管理员登录界面与退出程序界面，密码错误时，清空密码输入框内容。
逻辑身份证读卡器：
升级到1.0.0.5。

Ver1.0.0.6  2014.04.18 黄嘉亮
1.删除周转箱功能。
2.包裹信息保存到Package.xml。
3.合并设备状态和格口状态交易线程，避免冲突。
4.重新封装邮政后台交易组包解包函数，统一不同后台的交易流程。
5.抽离编码转换类函数到Tool库。
6.增加“增加管理员”、“管理员取件”功能。
7.修改密钥处理方式。比较密文。
8.日志打印箱状态。
9.增加交易重发机制。
10.取件校验以本地信息为准。
11.投递交易物件进箱后不开箱，通信失败定时重发交易。
12.管理员取件发客户取件交易。
13.客户取件只验证本地信息，取件成功后补发取件交易。
14.界面美观性调整。
15.Flash主界面显示版本信息。
16.修正同时多次获取箱状态造成的状态异常。
17.日志超长时打印警告信息。
18.加载xml文件时校验，加载失败不做写入操作，避免丢失原内容。
19.删除BankName配置项。
20.启动时加载配置文件一次，加载失败退出应用。程序运行时只写入不加载。
21.取门状态和取物状态之间增加延时。防止控制板超时。
门锁控制板驱动：升级到1.0.0.3。
公用工具库：升级到1.0.0.5。

Ver1.0.0.7  2014.04.18 黄嘉亮
1.增加内部版本号。
2.开箱测试只开没包裹信息的箱。
3.发送到Flash消息的处理从定时器改为线程。
4.增加Flash消息流水号。
5.发送到Flash的消息增加重发机制，接收到确认响应才发送下一条消息。每500毫秒重试1次，最多10次。
6.Flash优化界面提示信息。
7.Flash版本号201404191720。
8.邮政后台箱包裹状态以本地包裹数据为准。
9.增加投递报文延发开关。
10.启动时打印配置文件中功能选项开关信息。
11.增加是否向flash发送重复身份证号和IC卡号的配置项。
12.通讯超时时间可配置。
13.条码唯一性的检查，放在开箱操作前。
14.运行期间只对时一次。
15.增加配置项，投递交易实时处理时，交易失败是否开箱。
16.自营系统不取包裹信息，直接进入修改手机号。
add by yjx
17、“交易发送失败”和“交易接收失败”等因网络故障原因引起的错误提示全部改为“网络异常”。
18、修改取件时姓名没赋值的问题。
19、修改两个队列线程同时运行的问题，改为限制运行一个。
20、管理员取件，邮政后台不上传取件密码，协达后台上传取件密码
21、有修改内容时，保存transaction文件；没有内容修改时，不对transaction文件进行操作
22、程序启动时，先同步一次后台数据
23、包裹重复投放时，界面提示改为：包裹信息已存在，无法重复投递
24、程序启动时若文件格式错，不退出程序，界面提示：加载配置信息失败
25、管理员取件后界面提示改为：管理员取件后，该邮件不能再用于信包箱投件，取出后请以其他方式进行投派

Ver1.0.0.8   2014.05.27
1.内部版本升为1008
2.增加单机版功能，交易不发报文
3.增加mysql数据库支持（数据表支持一箱多件存储，程序暂不支持）
4.增加短信发送功能

1.0.7.1
20140722
1、没放包裹，关门显示获取参数错误[8];
2、手机登录时获取动态密码成功返回，但是界面卡在那里。
3、手机登陆，IC卡登陆，在登录后，显示文字“xxxx”；
20140725
4、如果没有空箱，投递员无法查询包裹
5、界面查询字体太小位置对不上
6、投递员查询 和 管理员取件用了同一flash指令，需要进行区分
7、投递员查询后，增加取走查询包裹功能（查询所得的是后台数据，还需要判断前后台数据是否一致，否则会出问题）
8、输错动态码后，flash没响应
9、投递后查询到手机号，确认时又要重新输
10、IC卡登录和手机登录选择界面，增加退出按钮
11、投递时手机号修改界面，退出键改为返回键
20140730
12.投件流程分为3种方式：脱机、联机辅助、联机验证(分别对应：投递时不联机查询客户信息、投递时联机查询客户信息并可修改、投递时联机查询客户信息并且不可修改)
13、投递查询客户手机号时，增加判断该邮件在后台的状态，如若是重复投的或待取的，则界面提示错误
14、ExecFunc_CheckPackageInfo|00|该邮件不允许重复投递  flash的错误提示语显示一段时间后需隐藏
15、管理员取件，邮件列表增加显示邮件状态（待取、投递失败、正在处理）
20140805
投递员登陆、管理员登陆界面，只播放一次“请刷投递员卡”、“请刷管理员卡”提示语音。

备注：
第12条未实现，其余已修改或实现。

1.0.7.2
20140808
1.修改队列，后台返回错误响应时不再重发交易
2.修改流水号更新顺序，保证组包时的流水号和入交易队列及发交易时的流水号一致。
3.将下载脱机数据中的死循环等待改为3秒等待
20140811
4.报文组包时先不进行字符编码转换，在报文发送前再进行转换
5.增加检测数据库链接情况，若数据库链接失效则尝试重新链接，并且界面暂停对外服务，直至链接恢复正常

1.0.7.3
1.数据库名改为ibox_gz。
2.支持密码键盘。
3.数据库丢失连接自动复位。
4.去除所有刷卡相关文字和语音提示。
5.修改箱配置文件结构。

升级必改：
g_AppVersion、g_AppInternalVersion、资源Version。
-----------------------
*/

BOOL CSsmAppApp::InitInstance()
{
	AfxBeginThread(CLog::__GetQueueMessage,NULL);//写日志线程
	char acbuf[512]={0};
	if(GetModuleFileName(NULL,acbuf,sizeof(acbuf)))
    {
		g_AppPathName=acbuf;
		g_AppPath=g_AppPathName.Left(g_AppPathName.ReverseFind('\\'));
		SetCurrentDirectory(g_AppPath);
		g_MainSwf.Format("%s\\flashfile\\main.swf",g_AppPath);
    }	

	g_Log.WriteLogEx(LOG_JOURNAL," ");
	g_Log.WriteLogEx(LOG_JOURNAL,"-------------程序启动-------------");	
	g_Log.WriteLogEx(LOG_JOURNAL,"广州邮政信包自助投递终端%s",g_AppVersion);
	g_Log.WriteLogEx(LOG_JOURNAL,"编译时间:%s.",__TIMESTAMP__);

	AfxEnableControlContainer();
	
	HANDLE hMutex=OpenMutex(MUTEX_ALL_ACCESS,FALSE,"XDIBox");
	if(hMutex==NULL)
		hMutex=CreateMutex(NULL,TRUE,"XDIBox");//给DOG检测
	else
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"**检测到已运行一个实例.");
		return false;
	}

	//结束应用程序信号
	if((g_SystemCmdDown = CreateEvent(NULL, FALSE, FALSE, "DogQuit")) == NULL)
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"创建狗退出事件失败.");
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// 文件属性处理
	char tmpbuf[512]={0};	

	g_ConfigFile.Format("%s\\syscfg\\Config.xml",g_AppPath);
	g_ConfigFile_bak.Format("%s\\syscfg\\backup\\Config.xml",g_AppPath);
	//是否存在目录
	sprintf(tmpbuf,"%s\\log",g_AppPath);
	if(!_FolderExist(tmpbuf))
		CreateDirectory(tmpbuf,NULL);

	sprintf(tmpbuf,"%s\\syscfg\\backup",g_AppPath);
	if(!_FolderExist(tmpbuf))
		CreateDirectory(tmpbuf,NULL);
	
	SetWindowsHookEx(WH_MOUSE,MouseProc,NULL,GetCurrentThreadId());
	CSsmAppDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.

	return FALSE;
}

int CSsmAppApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if(g_hTrayWnd)
		::ShowWindow(g_hTrayWnd,SW_SHOW);

	g_Log.WriteLogEx(LOG_JOURNAL,"-------------程序关闭-------------");
	
	/*char acPath[256]={0};
	sprintf(acPath,"%s\\trandata\\*.*",g_AppPath);
	DeleteFile(acPath);*/
	Sleep(200);//因用队列处理日志 稍加延时 写完日志后关闭
	return CWinApp::ExitInstance();
}

BOOL CSsmAppApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_RBUTTONDOWN||pMsg->message==WM_RBUTTONUP)
		return FALSE;
	return CWinApp::PreTranslateMessage(pMsg);
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if(wParam==WM_RBUTTONDOWN||wParam==WM_RBUTTONUP)
		return 1;
	return 0;
}
