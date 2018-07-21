/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumThread.h
* @brief	This Program is PlumThread DLL Project.
* @author	Alopex/Helium
* @version	v1.01a
* @date		2018-1-14	v1.00a	alopex	Create Project.
* @date		2018-7-20	v1.01a	alopex	Modify Call Mode.
*/
#pragma once

#ifndef __PLUMTHREAD_H_
#define __PLUMTHREAD_H_

//Include Plum Common Header File
#include "PlumCommon.h"

//Macro Definition
#ifdef SRECL_EXPORTS
#define PLUMTHREAD_API	__declspec(dllexport)
#else
#define PLUMTHREAD_API	__declspec(dllimport)
#endif

#define PLUMTHREAD_CALLMODE	__stdcall

//Class Definition
//CPlumThreadBase����
class PLUMTHREAD_API CPlumThreadBase
{
private:

public:
	CPlumThreadBase();													//����
	virtual ~CPlumThreadBase();											//����
	virtual void PLUMTHREAD_CALLMODE PlumThreadRun() = 0;				//���麯��
};

//CPlumThread������
class PLUMTHREAD_API CPlumThread :public CPlumThreadBase
{
private:
	HANDLE m_hThread;													//�߳̾��
	DWORD m_dwThreadID;													//�̺߳�
	volatile bool m_bThreadRun;											//�߳�����
	CPlumThreadBase* m_pThreadBase;										//�̻߳���ָ��

private:
	static DWORD PLUMTHREAD_CALLMODE PlumThreadProc(LPVOID pThreadPara);//�̴߳�����(��̬)

public:
	CPlumThread();														//����
	~CPlumThread();														//����
	CPlumThread(CPlumThreadBase* pThreadBase);							//����

	DWORD PLUMTHREAD_CALLMODE PlumThreadGetID() const;								//�߳�ID��ȡ

	virtual bool PLUMTHREAD_CALLMODE PlumThreadInit(bool bSuspend = false);			//�̳߳�ʼ��(����:true)
	virtual void PLUMTHREAD_CALLMODE PlumThreadSetPriority(int nPriority);			//�߳��������ȼ�(THREAD_PRIORITY_ABOVE_NORMAL)
	virtual void PLUMTHREAD_CALLMODE PlumThreadRun();								//�߳�����(��д)
	virtual void PLUMTHREAD_CALLMODE PlumThreadJoin(int nTimeOut = -1);				//�̵߳ȴ�����
	virtual void PLUMTHREAD_CALLMODE PlumThreadResume();							//�ָ̻߳�
	virtual void PLUMTHREAD_CALLMODE PlumThreadSuspend();							//�̹߳���
	virtual bool PLUMTHREAD_CALLMODE PlumThreadTerminate(DWORD dwExitCode);			//�߳���ֹ
	virtual void PLUMTHREAD_CALLMODE PlumThreadExit();								//�߳��˳�
};

#endif