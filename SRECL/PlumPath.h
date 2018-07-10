/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumPath.h
* @brief	This Program is PlumPath DLL Project.
* @author	Alopex/Helium
* @version	v1.01a
* @date		2018-01-18	v1.00a	alopex	Create Project.
* @date		2018-07-10	v1.01a	alopex	Add Function.
*/
#pragma once

#ifndef __PLUMPATH_H_
#define __PLUMPATH_H_

//Include Windows Header File
#include <Windows.h>

//Include C/C++ Run Header File
#include <stdio.h>
#include <stdlib.h>

//Include ATL Header File
#include <atlconv.h>

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMPATH_API	__declspec(dllexport)
#else
#define PLUMPATH_API	__declspec(dllimport)
#endif

#define PLUMPATH_CALLMODE	__stdcall

//Class Definition
class PLUMPATH_API CPlumPath
{
private:
	static BOOL PLUMPATH_CALLMODE PlumPathConvertA2W(const char* szArr, const wchar_t* wszArr, int nSize);
	static BOOL PLUMPATH_CALLMODE PlumPathConvertW2A(const wchar_t* wszArr, const char* szArr, int nSize);

public:
	CPlumPath();
	~CPlumPath();

	static BOOL PLUMPATH_CALLMODE PlumPathGetModulePathA(const char* szArr, int nSize);
	static BOOL PLUMPATH_CALLMODE PlumPathGetModulePathW(const wchar_t* wszArr, int nSize);
	static BOOL PLUMPATH_CALLMODE PlumPathGetLocalPathA(const char* szArr, int nSize);
	static BOOL PLUMPATH_CALLMODE PlumPathGetLocalPathW(const wchar_t* wszArr, int nSize);
	static BOOL PLUMPATH_CALLMODE PlumPathGetParentPathA(const char* szArr, int nSize, int nDeep);
	static BOOL PLUMPATH_CALLMODE PlumPathGetParentPathW(const wchar_t* wszArr, int nSize, int nDeep);
};

#endif