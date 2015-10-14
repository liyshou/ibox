// Queue.cpp: implementation of the CQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueue::CQueue()
{
	m_iProcessNum = 0;
	m_iRecvNum = 0;
	pHead = NULL;
	pTail = NULL;
}

CQueue::~CQueue()
{
	CString csData1,csData2;
	int iData3;
	while(GetHead(csData1,csData2,iData3));

	pHead = NULL;
	pTail = NULL;
}

int CQueue::GetHead(CString &csData1,CString &csData2,int &iData3)
{
	///////////////////////////////////////////////////////////
	//���ܣ�ȡ������ͷ��
	///////////////////////////////////////////////////////////
	int iFlag;
	Queue *p;

	if(pHead == NULL) 
	{
		return 0;
	}

	this->m_QueueHeadLock.Lock();

	m_iProcessNum++;
	iFlag = 0;

	if(m_iRecvNum - m_iProcessNum <= 3 )	//��������ĳ���С�ڵ���3��
	{
		this->m_QueueTailLock.Lock();
		iFlag = 1;
	}

	p=pHead;
	csData1=pHead->csData1;
	csData2=pHead->csData2;
	iData3=pHead->iData3;
	
	pHead = pHead->next;
	
	delete p;

	if(iFlag)
	{
		this->m_QueueTailLock.Unlock();
	}
	this->m_QueueHeadLock.Unlock();
	
	return 1;
}	

BOOL CQueue::AddTail(CString csData1,CString csData2,int iData3)
{
	////////////////////////////////////////////////////////////
	//���ܣ�ѹ�����β��
	////////////////////////////////////////////////////////////
	Queue *p = new Queue;

	if(NULL == p) 
	{
		return FALSE;
	}

	//memset(p,0x00,sizeof(Queue));
	
	if(m_iRecvNum - m_iProcessNum >= MAX_QUEUE_SIZE/*g_Config.m_iQueueLength*/)
	{
//		g_Log.WriteLogEx(LOG_JOURNAL,"Queue length is too long[%d],insert into queue error.",m_iRecvNum-m_iProcessNum);	
		return FALSE;
	}
	
	this->m_QueueTailLock.Lock();
	m_iRecvNum++;

	if(NULL == pHead) 
	{
		if(m_iRecvNum > 1000000)
		{
			m_iRecvNum = 1;
			m_iProcessNum = 0;
//			g_Log.WriteLogEx(LOG_JOURNAL,"The Queue Length is Reset.");
		}

		p->csData1=csData1;
		p->csData2=csData2;
		p->iData3=iData3;
		p->next = NULL;
		pTail = p;
		pHead = p;
	}
	else
	{
		pTail->next = p;
		p->csData1=csData1;
		p->csData2=csData2;
		p->iData3=iData3;
		p->next =NULL;
		pTail = p;
	}
//	g_Log.WriteLogEx(LOG_JOURNAL,"Queue Length:RecvNum[%d],ProcessNum[%d].",m_iRecvNum,m_iProcessNum);
	this->m_QueueTailLock.Unlock();
	return TRUE;
}

BOOL CQueue::IsEmpty()
{
	///////////////////////////////////////////////////////////
	//���ܣ������Ƿ�Ϊ��
	///////////////////////////////////////////////////////////
	if(NULL == pHead)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
