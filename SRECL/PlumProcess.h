/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumProcess.h
* @brief	This Program is PlumProcess DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2018-1-30	v1.00a	alopex	Create Project.
*/
#pragma once

#ifndef __PLUMPROCESS_H_
#define __PLUMPROCESS_H_

//Include Windows Header File
#include <Windows.h>

//Include C/C++ Running Header File
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <tchar.h>

//Macro Definition
#ifdef SRECL_EXPORTS
#define PLUMPROCESS_API	__declspec(dllexport)
#else
#define PLUMPROCESS_API	__declspec(dllimport)
#endif

//Class Definition
class PLUMPROCESS_API CPlumProcess
{
private:


public:
	CPlumProcess();
	~CPlumProcess();
};

#endif // !__PLUMPROCESS_H_
