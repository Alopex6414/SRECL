/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumLog.h
* @brief	This Program is PlumLog DLL Project.
* @author	Alopex/Helium
* @version	v1.03a
* @date		2018-1-26	v1.00a	alopex	Create Project.
* @date		2018-3-20	v1.01a	alopex	Modify Code Bug.
* @date		2018-3-24	v1.02a	alopex	Add Auto Create File Path.
* @date		2018-7-03	v1.03a	alopex	Add Call Mode.
*/
#pragma once

#ifndef __PLUMLOG_H_
#define __PLUMLOG_H_

//Include Plum Common Header File
#include "PlumCommon.h"

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMLOG_API	__declspec(dllexport)
#else
#define PLUMLOG_API	__declspec(dllimport)
#endif

#define PLUMLOG_CALLMODE	__stdcall

//Class Definition
class PLUMLOG_API CPlumLog
{
private:
	CRITICAL_SECTION m_csThreadSafe;		//Critical Section(~�̰߳�ȫ)

public:
	bool m_bIsUseLog;		//Use PlumLog Flag(~�Ƿ�ʹ��)
	FILE* m_fp;				//File Pointer(~�ļ�ָ��)

public:
	CPlumLog();				//Construction Function(~���캯��)
	~CPlumLog();			//Destruction Function(~��������)

	//����
	CPlumLog(bool bIsUseLog);				//Construction Function(~���캯��)

	//����
	virtual bool PLUMLOG_CALLMODE PlumLogGetUse() const;		//PlumLog Get UseLog(~��ȡʹ����־)

	//����
	virtual void PLUMLOG_CALLMODE PlumLogSetUse(bool IsUseLog);	//PlumLog Set UseLog(~����ʹ����־)

	//��ʼ��
	virtual void PLUMLOG_CALLMODE PlumLogInit();				//PlumLog Initialize(~��ʼ��)(ͨ�ó�ʼ��<������>)

	//�˳�
	virtual void PLUMLOG_CALLMODE PlumLogExit();				//PlumLog Exit(~�˳�)
	virtual void PLUMLOG_CALLMODE PlumLogClose();				//PlumLog Close(~�ر��ļ�ϵͳ)<��Ҫ����Exit��Close>

	//д��
	virtual void PLUMLOG_CALLMODE PlumLogWrite(LPCSTR lpcstr);					//Plum Write(~д��)(ASCII)
	virtual void PLUMLOG_CALLMODE PlumLogWriteElongate(LPCSTR lpcstr, ...);		//Plum Write(~д��)(���������)
	virtual void PLUMLOG_CALLMODE PlumLogWriteNormal(LPCSTR lpcstr, ...);		//Plum Write(~д��)(��׼��ʽ)<ʱ��/�߳�>
	virtual void PLUMLOG_CALLMODE PlumLogWriteExtend(LPCSTR lpcstr, ...);		//Plum Write(~д��)(��չ��ʽ)<ʱ��/�߳�/�ļ�/�к�>
	virtual void PLUMLOG_CALLMODE PlumLogWriteLine(LPCSTR lpcstr);				//Plum Write Line(~д��)(ASCII)
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineElongate(LPCSTR lpcstr, ...);	//Plum Write Line(~д��)(���������)
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineNormal(LPCSTR lpcstr, ...);	//Plum Write Line(~д��)(��׼��ʽ)<ʱ��/�߳�>
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineExtend(LPCSTR lpcstr, ...);	//Plum Write Line(~д��)(��չ��ʽ)<ʱ��/�߳�/�ļ�/�к�>

	virtual void PLUMLOG_CALLMODE PlumLogWriteExtend(LPCSTR file, LONG line, LPCSTR lpcstr, ...);		//Plum Write(~д��)(��չ��ʽ)<ʱ��/�߳�/�ļ�/�к�>(����__FILE__,__LINE__)
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineExtend(LPCSTR file, LONG line, LPCSTR lpcstr, ...);	//Plum Write Line(~д��)(��չ��ʽ)<ʱ��/�߳�/�ļ�/�к�>(����__FILE__,__LINE__)

protected:
	virtual void PLUMLOG_CALLMODE WriteLineComputerName();	//PlumLog Write Line Computer Name(~д����������)
	virtual void PLUMLOG_CALLMODE WriteLineUserName();		//PlumLog Write Line User Name(~д���û�����)
	virtual void PLUMLOG_CALLMODE WriteLineWindowsVision();	//PlumLog Write Line Windows Vision(~д��Windows�汾)
	virtual void PLUMLOG_CALLMODE WriteLineProcessID();		//PlumLog Write Line Process ID(~д�����ID)
	virtual void PLUMLOG_CALLMODE WriteLineCurrentTime();	//PlumLog Write Line Current Time(~д�뵱ǰʱ��)

};

#endif // !__PLUMLOG_H_

