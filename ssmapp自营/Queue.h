// Queue.h: interface for the CQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEUE_H__72502342_D628_4474_A316_132C5A36352D__INCLUDED_)
#define AFX_QUEUE_H__72502342_D628_4474_A316_132C5A36352D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxmt.h"
//#include "Net.h"
class CQueue  
{
public:
	typedef struct _Queue
	{
		CString csData1;
		CString csData2;
		int iData3;
		struct _Queue *next;
	}Queue;
public:
	CQueue();
	virtual ~CQueue();

public:
	CMutex m_QueueTailLock;			//���������
	CMutex m_QueueHeadLock;			//ȡͷ����
	BOOL IsEmpty();					//�����Ƿ�Ϊ��
	BOOL AddTail(CString csData1,CString csData2="",int iData3=0);
	int GetHead(CString &csData1,CString &csData2,int &iData3);//�շ���0
	Queue *pHead;
	Queue *pTail;
public:
	volatile unsigned int m_iProcessNum;
	volatile unsigned int m_iRecvNum;
};

#endif // !defined(AFX_QUEUE_H__72502342_D628_4474_A316_132C5A36352D__INCLUDED_)
