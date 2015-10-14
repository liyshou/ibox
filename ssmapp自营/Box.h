#include "StdAfx.h"

/*
1.0.0.1
增加版本号
增加开门失败、关门失败、放物失败、取物失败四个状态。
20140905 黄嘉亮
*/
class CBox
{
	class CBoxSize
	{
	public:
		int l;
		int w;
		int h;
	};

public:
	CBox()
	{
		m_iCabinetCount=0;
		m_iBoxCountTotal=0;
		m_iBoxSizeCount=0;
		m_iOpenFailMaxTimes=3;
		m_iEmptyFailMaxTimes=3;
		m_piCabinetID=NULL;
		m_piBoxCount=NULL;
		m_piBoxHostTimeoutAvailable=NULL;
		m_piBoxHostPackageAvailable=NULL;
		m_piBoxLocalPackage=NULL;
		m_piBoxHardwareNotEmpty=NULL;
		m_piBoxHardwareClose=NULL;
		m_piBoxSpec=NULL;
		m_piBoxHigh=NULL;
		m_piStatus=NULL;
		m_piBoxLength=NULL;
		m_piBoxWidth=NULL;
		m_piBoxHeight=NULL;
		m_pCBoxSize=NULL;
		m_piOpenFail=NULL;
		m_piCloseFail=NULL;
		m_piEmptyFail=NULL;
		m_piNotEmptyFail=NULL;
		m_pBoxName=NULL;
		g_Log.WriteLogEx(LOG_JOURNAL,"逻辑箱状态类版本1.0.0.2");
	}
	~CBox()
	{
		Release();
	}
	void Release()
	{
		_Delete(m_piBoxSpec);
		_Delete(m_piStatus);
		_Delete(m_piBoxHigh);
		_Delete(m_piBoxHardwareNotEmpty);
		_Delete(m_piBoxHardwareClose);
		_Delete(m_piBoxLocalPackage);
		_Delete(m_piBoxHostPackageAvailable);
		_Delete(m_piBoxHostTimeoutAvailable);
		_Delete(m_piBoxLength);
		_Delete(m_piBoxWidth);
		_Delete(m_piBoxHeight);
		_Delete(m_piBoxCount);
		_Delete(m_piCabinetID);
		_Delete(m_piOpenFail);
		_Delete(m_piCloseFail);
		_Delete(m_piEmptyFail);
		_Delete(m_piNotEmptyFail);
		if(m_pCBoxSize)
		{
			delete[] m_pCBoxSize;
			m_pCBoxSize=NULL;
		}
		if(m_pBoxName)
		{
			delete[] m_pBoxName;
			m_pBoxName=NULL;
		}
	}
	bool Init(int iCabinetCount,int iBoxCountTotal,int iBoxSizeCount)
	{
		if(!iCabinetCount||!iBoxCountTotal||!iBoxSizeCount)
			return false;

		Release();

		m_iCabinetCount=iCabinetCount;
		m_piBoxCount=new int[m_iCabinetCount];
		m_piCabinetID=new int[m_iCabinetCount];
		memset(m_piBoxCount,0,m_iCabinetCount*sizeof(int));
		memset(m_piCabinetID,0,m_iCabinetCount*sizeof(int));
		
		m_iBoxCountTotal=iBoxCountTotal;
		m_piBoxHostTimeoutAvailable=new int[m_iBoxCountTotal];
		m_piBoxHostPackageAvailable=new int[m_iBoxCountTotal];
		m_piBoxLocalPackage=new int[m_iBoxCountTotal];
		m_piBoxHardwareNotEmpty=new int[m_iBoxCountTotal];
		m_piBoxHardwareClose=new int[m_iBoxCountTotal];
		m_piBoxSpec=new int[m_iBoxCountTotal];
		m_piBoxHigh=new int[m_iBoxCountTotal];
		m_piBoxLength=new int[m_iBoxCountTotal];
		m_piBoxWidth=new int[m_iBoxCountTotal];
		m_piBoxHeight=new int[m_iBoxCountTotal];
		m_piStatus=new int[m_iBoxCountTotal];
		m_piBoxHide=new int[m_iBoxCountTotal];
		m_piOpenFail=new int[m_iBoxCountTotal];
		m_piCloseFail=new int[m_iBoxCountTotal];
		m_piEmptyFail=new int[m_iBoxCountTotal];
		m_piNotEmptyFail=new int[m_iBoxCountTotal];
		m_pBoxName=new CString[m_iBoxCountTotal];
		memset(m_piBoxHostTimeoutAvailable,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxHostPackageAvailable,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxLocalPackage,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxHardwareNotEmpty,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxHardwareClose,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piStatus,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxSpec,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxHigh,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxHide,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxLength,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxWidth,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piBoxHeight,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piOpenFail,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piCloseFail,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piEmptyFail,0,m_iBoxCountTotal*sizeof(int));
		memset(m_piNotEmptyFail,0,m_iBoxCountTotal*sizeof(int));

		m_iBoxSizeCount=iBoxSizeCount;
		m_pCBoxSize=new CBoxSize[m_iBoxSizeCount];
		memset(m_pCBoxSize,0,m_iBoxSizeCount*sizeof(CBoxSize));

		return true;
	}
	bool InitBoxCountBatch(CString data)
	{
		if(!m_piBoxCount||_GetFieldCount(data,',')>m_iCabinetCount)
			return false;
		for(int i=0;i<m_iCabinetCount;++i)
			*(m_piBoxCount+i)=atoi(_GetField(data,i+1,','));
		return true;
	}
	bool InitBoxTypeBatch(CString data)
	{
		if(!m_piBoxSpec||_GetFieldCount(data,',')<m_iBoxCountTotal||!m_pCBoxSize)
			return false;
		for(int i=0;i<m_iBoxCountTotal;++i)
		{
			*(m_piBoxSpec+i)=atoi(_GetField(data,i+1,','));
			if(*(m_piBoxSpec+i)>0&&*(m_piBoxSpec+i)<=m_iBoxSizeCount)
			{
				*(m_piBoxLength+i)=(m_pCBoxSize+*(m_piBoxSpec+i)-1)->l;
				*(m_piBoxHeight+i)=(m_pCBoxSize+*(m_piBoxSpec+i)-1)->h;
				*(m_piBoxWidth+i)=(m_pCBoxSize+*(m_piBoxSpec+i)-1)->w;
			}
		}
		return true;
	}
	bool InitBoxHighBatch(CString data)
	{
		if(!m_piBoxHigh||_GetFieldCount(data,',')>m_iBoxCountTotal)
			return false;
		for(int i=0;i<m_iBoxCountTotal;++i)
			*(m_piBoxHigh+i)=atoi(_GetField(data,i+1,','));
		return true;
	}
	bool InitBoxNameBatch(CString data)
	{
		if(!m_pBoxName)
			return false;
		if(_GetFieldCount(data,',')>m_iBoxCountTotal)/**/
			return false;
		/*for(int i=0;i<m_iCabinetCount)*/
		for(int i=0;i<m_iBoxCountTotal;++i)
			*(m_pBoxName+i)=_GetField(data,i+1,',');
		return true;
	}
	bool InitBoxSize(int no,int l,int w,int h)
	{//no从1起
		if(!m_iBoxSizeCount||no>m_iBoxSizeCount)
			return false;
		(m_pCBoxSize+no-1)->h=h;
		(m_pCBoxSize+no-1)->l=l;
		(m_pCBoxSize+no-1)->w=w;
		return true;
	}

	void UpdateStatus()
	{
		for(int i=0;i<m_iBoxCountTotal;++i)
		{
			if(!*(m_piBoxSpec+i))//屏蔽的箱
				*(m_piStatus+i)=3;//故障
			else if(*(m_piBoxHostTimeoutAvailable+i)&&*(m_piBoxLocalPackage+i))//后台有超时数据 并且 本地有数据
				*(m_piStatus+i)=1;//超时			
			else if(*(m_piBoxLocalPackage+i))//本地有包裹数据
				*(m_piStatus+i)=2;//可取
			else if(*(m_piBoxHardwareNotEmpty+i))//感应器有物
				*(m_piStatus+i)=3;//故障
			else if(m_iEmptyFailMaxTimes<=GetNotEmptyFail(i+1))//投递放物感应异常超过上限数
				*(m_piStatus+i)=3;//故障
			else if(m_iOpenFailMaxTimes<=GetOpenFail(i+1))//开门失败超过上限数
				*(m_piStatus+i)=3;//故障
			else
				*(m_piStatus+i)=0;//空箱
		}
	}
	void PrintAllStatus()
	{
		g_Log.WriteLogEx(LOG_JOURNAL,"┌──────柜状态──────┐");
		g_Log.WriteLogEx(LOG_JOURNAL,"│     柜  ID  箱数 │");
		g_Log.WriteLogEx(LOG_JOURNAL,"├──────────────────┤");
		for(int i=0;i<m_iCabinetCount;++i)
			g_Log.WriteLogEx(LOG_JOURNAL,"│     %02d  %02d  %02d   │",i+1,*(m_piCabinetID+i),*(m_piBoxCount+i));
		g_Log.WriteLogEx(LOG_JOURNAL,"├────┬─────────────┤");
		g_Log.WriteLogEx(LOG_JOURNAL,"│合计│%-4dx   %-5d│",m_iCabinetCount,m_iBoxCountTotal);
		g_Log.WriteLogEx(LOG_JOURNAL,"├────┴─────────────┴────────────────────────────────────────────────箱状态─────────────────────────────────────────────────────────────────────────┐");
		g_Log.WriteLogEx(LOG_JOURNAL,"│     逻辑箱  名称        柜  箱  后台超时  后台可取  本地可取  有物  门关  规格  高度  状态  隐藏  关门失败 开门失败 取物失败 放物失败  长宽高(cm)│");
		g_Log.WriteLogEx(LOG_JOURNAL,"├──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤");
		int iBoxHostTimeoutAvailableCount=0;
		int iBoxHostPackageAvailableCount=0;
		int iBoxLocalPackageCount=0;
		int iBoxHardwareNotEmptyCount=0;
		int iBoxHardwareCloseCount=0;
		int iBoxHideCount=0;
		int iOpenFailCount=0;
		int iCloseFailCount=0;
		int iEmptyFailCount=0;
		int iNotEmptyFailCount=0;
		for(int i=0;i<m_iBoxCountTotal;++i)
		{
			iBoxHostTimeoutAvailableCount+=*(m_piBoxHostTimeoutAvailable+i);
			iBoxHostPackageAvailableCount+=*(m_piBoxHostPackageAvailable+i);
			iBoxLocalPackageCount+=*(m_piBoxLocalPackage+i);
			iBoxHardwareNotEmptyCount+=*(m_piBoxHardwareNotEmpty+i);
			iBoxHardwareCloseCount+=*(m_piBoxHardwareClose+i);
			iBoxHideCount+=*(m_piBoxHide+i);
			iOpenFailCount+=*(m_piOpenFail+i);
			iCloseFailCount+=*(m_piCloseFail+i);
			iEmptyFailCount+=*(m_piEmptyFail+i);
			iNotEmptyFailCount+=*(m_piNotEmptyFail+i);			
			g_Log.WriteLogEx(LOG_JOURNAL,"│     %02d      %-12s%02d  %02d  %-10d%-10d%-10d%-6d%-6d%-6d%-6d%-6d%-6d%-9d%-9d%-9d%-9d %-2dx%-2dx%-2d  │",i+1,*(m_pBoxName+i),GetPhysicalCabinet(i+1),GetPhysicalBox(i+1),*(m_piBoxHostTimeoutAvailable+i),*(m_piBoxHostPackageAvailable+i),*(m_piBoxLocalPackage+i),*(m_piBoxHardwareNotEmpty+i),*(m_piBoxHardwareClose+i),*(m_piBoxSpec+i),*(m_piBoxHigh+i),*(m_piStatus+i),*(m_piBoxHide+i),*(m_piOpenFail+i),*(m_piCloseFail+i),*(m_piEmptyFail+i),*(m_piNotEmptyFail+i),*(m_piBoxLength+i),*(m_piBoxWidth+i),*(m_piBoxHeight+i));
		}
		g_Log.WriteLogEx(LOG_JOURNAL,"├────┬─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤");
		g_Log.WriteLogEx(LOG_JOURNAL,"│合计│%-8dx           %-4d%-4d%-10d%-10d%-10d%-6d%-6dx     x     x     %-6d%-9d%-9d%-9d%-20d│",m_iBoxCountTotal,m_iCabinetCount,m_iBoxCountTotal,iBoxHostTimeoutAvailableCount,iBoxHostPackageAvailableCount,iBoxLocalPackageCount,iBoxHardwareNotEmptyCount,iBoxHardwareCloseCount,iBoxHideCount,iOpenFailCount,iCloseFailCount,iEmptyFailCount,iNotEmptyFailCount);
		g_Log.WriteLogEx(LOG_JOURNAL,"├────┼─────────────────────────────┬────────────────────┬───────────────────────────────────┬──────────────────────────────────────────────────────┤");
		g_Log.WriteLogEx(LOG_JOURNAL,"│说明│规格:0屏蔽1迷你2小3中4大5巨型│高度:1底层2中层3高层│状态:0空箱1超时包裹2正常包裹3不可用│");
		g_Log.WriteLogEx(LOG_JOURNAL,"└────┴─────────────────────────────┴────────────────────┴───────────────────────────────────┘");
	}
	void PrintWarning(int iLogicalBox)
	{
		int a=*(m_piBoxHostTimeoutAvailable+iLogicalBox-1)|*(m_piBoxHostPackageAvailable+iLogicalBox-1);
		if(a!=*(m_piBoxLocalPackage+iLogicalBox-1)||a!=*(m_piBoxHardwareNotEmpty+iLogicalBox-1))
		{
			g_Log.WriteLogEx(LOG_JOURNAL,"警告:逻辑箱[%d]物件状态不同步",iLogicalBox);
			g_Log.WriteLogEx(LOG_JOURNAL,"逻辑箱  柜  箱  后台超时  后台可取  本地可取  有物");
			g_Log.WriteLogEx(LOG_JOURNAL,"%02d      %02d  %02d  %-10d%-10d%-10d%d",iLogicalBox,GetPhysicalCabinet(iLogicalBox),GetPhysicalBox(iLogicalBox),*(m_piBoxHostTimeoutAvailable+iLogicalBox-1),*(m_piBoxHostPackageAvailable+iLogicalBox-1),*(m_piBoxLocalPackage+iLogicalBox-1),*(m_piBoxHardwareNotEmpty+iLogicalBox-1));
		}
	}

	int GetPhysicalBox(int iLogicalBox)
	{//物理箱号 从1起
		for(int i=0;i<m_iCabinetCount;++i)
		{
			if(iLogicalBox<=*(m_piBoxCount+i))
				return iLogicalBox;
			else
				iLogicalBox-=*(m_piBoxCount+i);
		}
		return -1;
	}
	int GetPhysicalCabinet(int iLogicalBox)
	{//物理柜号 从1起
		for(int i=0;i<m_iCabinetCount;++i)
		{
			if(iLogicalBox<=*(m_piBoxCount+i))
			{
				if(*(m_piCabinetID+i)>0)
					return *(m_piCabinetID+i);	
				else
					break;
			}
			else
				iLogicalBox-=*(m_piBoxCount+i);
		}
		return -1;
	}
	int GetLogicalBoxByPhysicalCabinetID(int iPhysicalCabinetID,int iPhysicalBox)
	{//逻辑箱号 从1起
		int iCabinetNO=-1;//箱ID在箱序列中的位置
		int iLogicalBox=-1;
		for(int i=0;i<m_iCabinetCount;++i)
			if(*(m_piCabinetID+i)==iPhysicalCabinetID)
			{
				iCabinetNO=i;
			}

		if(iCabinetNO>-1)
		{
			iLogicalBox=iPhysicalBox;
			for(int i=0;i<iCabinetNO;++i)
				iLogicalBox+=*(m_piBoxCount+i);
		}
		return iLogicalBox;
	}
	int GetLogicalBoxByPhysicalCabinetNO(int iPhysicalCabinetNO,int iPhysicalBox)
	{//逻辑箱号 从1起
		if(iPhysicalCabinetNO<0||iPhysicalCabinetNO>m_iCabinetCount-1||!iPhysicalBox)
			return -1;
		
		int iLogicalBox=iPhysicalBox;
		for(int i=0;i<iPhysicalCabinetNO;++i)
			iLogicalBox+=*(m_piBoxCount+i);

		return iLogicalBox;
	}
	int GetClassifyBoxCount(int iStatus=-1,int iSpec=-1,int iHigh=-1)
	{
		int iCount=0;
		for(int i=0;i<m_iBoxCountTotal;++i)
		{
			if(((-1==iStatus)||(*(m_piStatus+i)==iStatus))&&
				((-1==iSpec)||(*(m_piBoxSpec+i)==iSpec))&&
				((-1==iHigh)||(*(m_piBoxHigh+i)==iHigh))&&
				!*(m_piBoxHide+i))
				++iCount;
		}
		return iCount;
	}
	int GetClassifyBoxID(int iStatus=-1,int iSpec=-1,int iHigh=-1)
	{		
		int iClassifyBoxCount=GetClassifyBoxCount(iStatus,iSpec,iHigh);
		if(iClassifyBoxCount<=0)
			return -1;

		srand(GetTickCount());
		int iClassifyBoxOffset=rand()%iClassifyBoxCount;
		int iCurrentSelectClassifyBox=0;

		for(int i=0;i<m_iBoxCountTotal;++i)
		{
			if(((-1==iStatus)||(*(m_piStatus+i)==iStatus))&&
				((-1==iSpec)||(*(m_piBoxSpec+i)==iSpec))&&
				((-1==iHigh)||(*(m_piBoxHigh+i)==iHigh))&&
				!*(m_piBoxHide+i))
			{
				if(iCurrentSelectClassifyBox==iClassifyBoxOffset)
					return i+1;
				else
					++iCurrentSelectClassifyBox;
			}
		}
		return -1;
	}
	int GetClassifyBoxSpec(int iBoxNo)
	{
		if(iBoxNo>0&&iBoxNo<=m_iBoxCountTotal)
			return *(m_piBoxSpec+iBoxNo-1);
		return -1;
	}
	void SetCabinetID(int iLogicalCabinet,int iValue)
	{
		*(m_piCabinetID+iLogicalCabinet-1)=iValue;
	}
	int GetCabinetID(int iLogicalCabinet)
	{
		if(iLogicalCabinet<=0||iLogicalCabinet>m_iCabinetCount)
			return -1;
		return *(m_piCabinetID+iLogicalCabinet-1);
	}
	void SetBoxCount(int iLogicalCabinet,int iValue)
	{
		*(m_piBoxCount+iLogicalCabinet-1)=iValue;
	}
	int GetBoxCount(int iLogicalCabinet)
	{
		return *(m_piBoxCount+iLogicalCabinet-1);
	}
	
	void SetBoxCountTotal(int iBoxCountTotal)
	{
		m_iBoxCountTotal=min(m_iBoxCountTotal,iBoxCountTotal);
	}
	int GetBoxCountTotal()
	{
		return m_iBoxCountTotal;
	}
	void SetCabinetCount(int iCabinetCount)
	{
		m_iCabinetCount=min(m_iCabinetCount,iCabinetCount);
	}
	int GetCabinetCount()
	{
		return m_iCabinetCount;
	}
	void SetBoxSizeCount(int iBoxSizeCount)
	{
		m_iBoxSizeCount=min(m_iBoxSizeCount,iBoxSizeCount);
	}
	int GetBoxSizeCount()
	{
		return m_iBoxSizeCount;
	}
	void SetStatus(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piStatus+i)=iValue;
		else
			*(m_piStatus+iLogicalBox-1)=iValue;
	}
	int GetStatus(int iLogicalBox)
	{
		if(!m_piStatus||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piStatus+iLogicalBox-1);
	}
	void SetBoxHostTimeoutAvailable(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piBoxHostTimeoutAvailable+i)=iValue;
		else
			*(m_piBoxHostTimeoutAvailable+iLogicalBox-1)=iValue;
	}
	int GetBoxHostTimeoutAvailable(int iLogicalBox)
	{
		if(!m_piBoxHostTimeoutAvailable||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxHostTimeoutAvailable+iLogicalBox-1);
	}
	void SetBoxHostPackageAvailable(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piBoxHostPackageAvailable+i)=iValue;
		else
			*(m_piBoxHostPackageAvailable+iLogicalBox-1)=iValue;
	}
	int GetBoxHostPackageAvailable(int iLogicalBox)
	{
		if(!m_piBoxHostPackageAvailable||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxHostPackageAvailable+iLogicalBox-1);
	}
	void SetBoxLocalPackage(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piBoxLocalPackage+i)=iValue;
		else
			*(m_piBoxLocalPackage+iLogicalBox-1)=iValue;
	}
	int GetBoxLocalPackage(int iLogicalBox)
	{
		if(!m_piBoxLocalPackage||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxLocalPackage+iLogicalBox-1);
	}
	void SetBoxHardwareClose(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piBoxHardwareClose+i)=iValue;
		else
			*(m_piBoxHardwareClose+iLogicalBox-1)=iValue;
	}
	int GetBoxHardwareClose(int iLogicalBox)
	{
		if(!m_piBoxHardwareClose||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxHardwareClose+iLogicalBox-1);
	}
	void SetBoxHardwareNotEmpty(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piBoxHardwareNotEmpty+i)=iValue;
		else
			*(m_piBoxHardwareNotEmpty+iLogicalBox-1)=iValue;
	}
	int GetBoxHardwareNotEmpty(int iLogicalBox)
	{
		if(!m_piBoxHardwareNotEmpty||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxHardwareNotEmpty+iLogicalBox-1);
	}
	void SetBoxHide(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piBoxHide+i)=iValue;
		else
			*(m_piBoxHide+iLogicalBox-1)=iValue;
	}
	int GetBoxLength(int iLogicalBox)
	{
		if(!m_piBoxLength||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxLength+iLogicalBox-1);
	}
	int GetBoxWidth(int iLogicalBox)
	{
		if(!m_piBoxWidth||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxWidth+iLogicalBox-1);
	}
	int GetBoxHeight(int iLogicalBox)
	{
		if(!m_piBoxHeight||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxHeight+iLogicalBox-1);
	}
	int GetBoxSpec(int iLogicalBox)
	{
		if(!m_piBoxSpec||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piBoxSpec+iLogicalBox-1);
	}
	void SetOpenFail(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piOpenFail+i)=iValue;
		else
			*(m_piOpenFail+iLogicalBox-1)=iValue;
	}
	int GetOpenFail(int iLogicalBox)
	{
		if(!m_piOpenFail||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piOpenFail+iLogicalBox-1);
	}
	void SetCloseFail(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piCloseFail+i)=iValue;
		else
			*(m_piCloseFail+iLogicalBox-1)=iValue;
	}
	int GetCloseFail(int iLogicalBox)
	{
		if(!m_piCloseFail||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piCloseFail+iLogicalBox-1);
	}
	void SetEmptyFail(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piEmptyFail+i)=iValue;
		else
			*(m_piEmptyFail+iLogicalBox-1)=iValue;
	}
	int GetEmptyFail(int iLogicalBox)
	{
		if(!m_piEmptyFail||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piEmptyFail+iLogicalBox-1);
	}
	void SetNotEmptyFail(int iLogicalBox=0,int iValue=0)
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				*(m_piNotEmptyFail+i)=iValue;
		else
			*(m_piNotEmptyFail+iLogicalBox-1)=iValue;
	}
	int GetNotEmptyFail(int iLogicalBox)
	{
		if(!m_piNotEmptyFail||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return -1;
		return *(m_piNotEmptyFail+iLogicalBox-1);
	}
	void SetBoxName(int iLogicalBox=0,CString name="")
	{
		if(!iLogicalBox)
			for(int i=0;i<m_iBoxCountTotal;++i)
				(m_pBoxName+i)->Empty();
		else
			*(m_pBoxName+iLogicalBox-1)=name;
	}
	CString GetBoxName(int iLogicalBox)
	{
		if(!m_pBoxName||iLogicalBox<=0||iLogicalBox>m_iBoxCountTotal)
			return "";
		return *(m_pBoxName+iLogicalBox-1);
	}
	int GetLogicalBoxByBoxName(CString BoxName)
	{
		if(!BoxName.IsEmpty())
		{
			for(int i=0;i<m_iBoxCountTotal;i++)
			{
				if(!BoxName.Compare(*(m_pBoxName+i)))
					return i+1;	
			}
		}
		return -1;
	}
	void SetAllowErrorTimes(int iOpenFailMaxTimes=0,int iEmptyFailMaxTimes=0)
	{
		if(0<iOpenFailMaxTimes)
			m_iOpenFailMaxTimes = iOpenFailMaxTimes;
		if(0<iEmptyFailMaxTimes)
			m_iEmptyFailMaxTimes = iEmptyFailMaxTimes;
	}
	int GetAllowOpenFailTimes()
	{
		return m_iOpenFailMaxTimes;
	}
	int GetAllowEmptyFailTimes()
	{
		return m_iEmptyFailMaxTimes;
	}

	void _Delete(int* pi)
	{
		if(pi)
		{
			delete[] pi;
			pi=NULL;
		}
	}
private:
	int m_iCabinetCount;				//装备的柜子总数	 
	int* m_piBoxCountByCabinetID;		//每个板配置的格口数
	int* m_piCabinetID;					//柜ID
	int* m_piBoxCount;					//柜配置的箱总数

	int m_iBoxCountTotal;				//所有箱总数
	int* m_piBoxHostTimeoutAvailable;	//后台超时件箱
	int* m_piBoxHostPackageAvailable;	//后台未超時件
	int* m_piBoxLocalPackage;			//本地有包裹信息的箱
	int* m_piBoxHardwareClose;			//控制板门关状态
	int* m_piBoxHardwareNotEmpty;		//控制板空状态
	int* m_piBoxSpec;					//箱类型：0屏蔽1很小2小3中4大5超大
	int* m_piBoxLength;					//长cm
	int* m_piBoxWidth;					//宽cm
	int* m_piBoxHeight;					//高cm
	int* m_piBoxHigh;					//箱高度：1底层箱2中层箱3高层箱
	int* m_piStatus;					//综合状态：0空箱1到期2未到期3故障4当前物流商预定空箱5其他物流商预定空箱
	int* m_piBoxHide;					//1分类选箱时隐藏该箱
	int* m_piOpenFail;					//开门失败记数
	int* m_piCloseFail;					//关门失败记数
	int* m_piEmptyFail;					//取物失败记数
	int* m_piNotEmptyFail;				//放物失败记数
	int m_iOpenFailMaxTimes;			//开箱失败最大的错误上限数，超过则记为故障
	int m_iEmptyFailMaxTimes;			//投递时物品感应最大的错误上限数，超过则记为故障
	CString* m_pBoxName;				//格口名称

	int m_iBoxSizeCount;				//箱尺寸总数
	CBoxSize* m_pCBoxSize;				//箱尺寸
};
