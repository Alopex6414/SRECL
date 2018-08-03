/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumIni.h
* @brief	This Program is PlumIni DLL Project.
* @author	Alopex/Helium
* @version	v1.02a
* @date		2018-01-17	v1.00a	alopex	Create Project.
* @date		2018-07-30	v1.02a	alopex	Add Call Mode.
*/
#pragma once

#ifndef __PLUMINI_H_
#define __PLUMINI_H_

//Include Plum Common Header File
#include "PlumCommon.h"

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMINI_API	__declspec(dllexport)
#else
#define PLUMINI_API	__declspec(dllimport)
#endif

#define PLUMINI_CALLMODE	__stdcall

//Class Definition
class PLUMINI_API CPlumIni
{
private:


public:
	CPlumIni();
	~CPlumIni();

	static bool PLUMINI_CALLMODE PlumIniReadStringKeyValue(char* pStrFileName, char* pStrSection, char* pStrKey, char* pStrDefault, char** ppStrValue);
	static bool PLUMINI_CALLMODE PlumIniReadIntKeyValue(char* pStrFileName, char* pStrSection, char* pStrKey, int nDefault, int* pValue);
	static bool PLUMINI_CALLMODE PlumIniWriteStringKeyValue(char* pStrFileName, char* pStrSection, char* pStrKey, char* pStrValue);
	static bool PLUMINI_CALLMODE PlumIniWriteIntKeyValue(char* pStrFileName, char* pStrSection, char* pStrKey, int nValue);
};

#endif