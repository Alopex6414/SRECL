/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumFile.h
* @brief	This Program is PlumFile DLL Project.
* @author	Alopex/Helium
* @version	v1.02a
* @date		2017-12-25	v1.00a	alopex	Create Project.
* @date		2018-07-12	v1.02a	alopex	Add Call Mode.
*/
#pragma once

#ifndef __PLUMFILE_H_
#define __PLUMFILE_H_

//Include Plum Common Header File
#include "PlumCommon.h"

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMFILE_API	__declspec(dllexport)
#else
#define PLUMFILE_API	__declspec(dllimport)
#endif

#define PLUMFILE_CALLMODE	__stdcall

//Macro Point
#define SAFE_DELETE(Pointer)	{if(Pointer){delete(Pointer);(Pointer) = NULL;}}
#define SAFE_DELETE_ARRAY(Pointer) {if(Pointer){delete[](Pointer);(Pointer) = NULL;}}

//Class Definition
class PLUMFILE_API CPlumFile
{
private:
	

public:
	CPlumFile();
	virtual ~CPlumFile();

	//访问
	//文件路径
	virtual void PLUMFILE_CALLMODE PlumFileGetCurrentDirectoryA(char* pArr, DWORD dwSize);
	virtual void PLUMFILE_CALLMODE PlumFileGetCurrentDirectoryW(wchar_t* pArr, DWORD dwSize);
	virtual void PLUMFILE_CALLMODE PlumFileGetModuleFileNameA(char* pArr, DWORD dwSize);
	virtual void PLUMFILE_CALLMODE PlumFileGetModuleFileNameW(wchar_t* pArr, DWORD dwSize);

	//文件夹路径
	virtual void PLUMFILE_CALLMODE PlumFileCreateDirectoryA(const char* pSrc);
	virtual void PLUMFILE_CALLMODE PlumFileCreateDirectoryW(const wchar_t* pSrc);

	//文件操作
	virtual void PLUMFILE_CALLMODE PlumFileCopyFileA(const char* pSrc, const char* pDest, BOOL bNoCover = TRUE);
	virtual void PLUMFILE_CALLMODE PlumFileCopyFileW(const wchar_t* pSrc, const wchar_t* pDest, BOOL bNoCover = TRUE);
	virtual void PLUMFILE_CALLMODE PlumFileDeleteFileA(const char* pSrc);
	virtual void PLUMFILE_CALLMODE PlumFileDeleteFileW(const wchar_t* pSrc);

	//文件读写
	virtual bool PLUMFILE_CALLMODE PlumFileReadFileSaveInMemory(const char* pSrc, char** ppDest, DWORD* pSize);
	virtual bool PLUMFILE_CALLMODE PlumFileReadFileSaveInMemory(const wchar_t* pSrc, char** ppDest, DWORD* pSize);
	virtual bool PLUMFILE_CALLMODE PlumFileWriteMemorySaveInFile(const char* pSrc, DWORD dwSize, const char* pDest);
	virtual bool PLUMFILE_CALLMODE PlumFileWriteMemorySaveInFile(const char* pSrc, DWORD dwSize, const wchar_t* pDest);
};

#endif