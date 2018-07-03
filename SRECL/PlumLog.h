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
	CRITICAL_SECTION m_csThreadSafe;		//Critical Section(~线程安全)

public:
	bool m_bIsUseLog;		//Use PlumLog Flag(~是否使用)
	FILE* m_fp;				//File Pointer(~文件指针)

public:
	CPlumLog();				//Construction Function(~构造函数)
	~CPlumLog();			//Destruction Function(~析构函数)

	//构造
	CPlumLog(bool bIsUseLog);				//Construction Function(~构造函数)

	//访问
	virtual bool PLUMLOG_CALLMODE PlumLogGetUse() const;		//PlumLog Get UseLog(~获取使用日志)

	//控制
	virtual void PLUMLOG_CALLMODE PlumLogSetUse(bool IsUseLog);	//PlumLog Set UseLog(~设置使用日志)

	//初始化
	virtual void PLUMLOG_CALLMODE PlumLogInit();				//PlumLog Initialize(~初始化)(通用初始化<可重载>)

	//退出
	virtual void PLUMLOG_CALLMODE PlumLogExit();				//PlumLog Exit(~退出)
	virtual void PLUMLOG_CALLMODE PlumLogClose();				//PlumLog Close(~关闭文件系统)<需要调用Exit或Close>

	//写入
	virtual void PLUMLOG_CALLMODE PlumLogWrite(LPCSTR lpcstr);					//Plum Write(~写入)(ASCII)
	virtual void PLUMLOG_CALLMODE PlumLogWriteElongate(LPCSTR lpcstr, ...);		//Plum Write(~写入)(变参数长度)
	virtual void PLUMLOG_CALLMODE PlumLogWriteNormal(LPCSTR lpcstr, ...);		//Plum Write(~写入)(标准格式)<时间/线程>
	virtual void PLUMLOG_CALLMODE PlumLogWriteExtend(LPCSTR lpcstr, ...);		//Plum Write(~写入)(扩展格式)<时间/线程/文件/行号>
	virtual void PLUMLOG_CALLMODE PlumLogWriteLine(LPCSTR lpcstr);				//Plum Write Line(~写入)(ASCII)
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineElongate(LPCSTR lpcstr, ...);	//Plum Write Line(~写入)(变参数长度)
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineNormal(LPCSTR lpcstr, ...);	//Plum Write Line(~写入)(标准格式)<时间/线程>
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineExtend(LPCSTR lpcstr, ...);	//Plum Write Line(~写入)(扩展格式)<时间/线程/文件/行号>

	virtual void PLUMLOG_CALLMODE PlumLogWriteExtend(LPCSTR file, LONG line, LPCSTR lpcstr, ...);		//Plum Write(~写入)(扩展格式)<时间/线程/文件/行号>(传入__FILE__,__LINE__)
	virtual void PLUMLOG_CALLMODE PlumLogWriteLineExtend(LPCSTR file, LONG line, LPCSTR lpcstr, ...);	//Plum Write Line(~写入)(扩展格式)<时间/线程/文件/行号>(传入__FILE__,__LINE__)

protected:
	virtual void PLUMLOG_CALLMODE WriteLineComputerName();	//PlumLog Write Line Computer Name(~写入计算机名称)
	virtual void PLUMLOG_CALLMODE WriteLineUserName();		//PlumLog Write Line User Name(~写入用户名称)
	virtual void PLUMLOG_CALLMODE WriteLineWindowsVision();	//PlumLog Write Line Windows Vision(~写入Windows版本)
	virtual void PLUMLOG_CALLMODE WriteLineProcessID();		//PlumLog Write Line Process ID(~写入进程ID)
	virtual void PLUMLOG_CALLMODE WriteLineCurrentTime();	//PlumLog Write Line Current Time(~写入当前时间)

};

#endif // !__PLUMLOG_H_

