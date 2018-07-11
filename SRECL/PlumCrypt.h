/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumCrypt.h
* @brief	This Program is PlumCrypt DLL Project.
* @author	Alopex/Helium
* @version	v1.02a
* @date		2017-12-13	v1.00a	alopex	Create Project.
* @date		2017-12-17	v1.01a	alopex	Add Function EnCrypt/DeCrypt In Memory.
* @date		2018-07-05	v1.02a	alopex	Add Function Crypt Cat Function.
*/
#pragma once

#ifndef __PLUMCRYPT_H_
#define __PLUMCRYPT_H_

//Include Plum Common Header File
#include "PlumCommon.h"

//Include Crypt Header File
#include "PlumCipherA.h"

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMCRYPT_API	__declspec(dllexport)
#else
#define PLUMCRYPT_API	__declspec(dllimport)
#endif

#define CRYPTARRAYSIZE	1024

//Macro Point
#define SAFE_DELETE(Pointer)	{if(Pointer){delete(Pointer);(Pointer) = NULL;}}
#define SAFE_DELETE_ARRAY(Pointer) {if(Pointer){delete[](Pointer);(Pointer) = NULL;}}

//Struct Definition
typedef struct
{
	char cFileName[24];
	char cCodeAuthor[8];
	DWORD dwFileSize;
	DWORD dwCryptFileSize;
	DWORD dwLuckyNum[4];
} PlumFileInfo;

//Class Definition
class PLUMCRYPT_API CPlumCrypt
{
private:
	char* m_pArray;			//内存资源数组地址
	DWORD m_dwArrSize;		//内存资源数组长度

public:
	CPlumCrypt();
	virtual ~CPlumCrypt();

	//访问
	virtual void PLUMCRYPT_CALLMODE PlumGetArray(char** ppArr, DWORD* pArrSize) const;

	//AES Crypt(AES加密)
	//File(文件)
	virtual void PLUMCRYPT_CALLMODE PlumEnCryptFileA(const char* pSrc, const char* pDest, DWORD* pLuckyArr);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileA(const char* pSrc, const char* pDest, DWORD* pLuckyArr);
	virtual void PLUMCRYPT_CALLMODE PlumEnCryptFileExA(const char* pSrc, const char* pDest, DWORD* pLuckyArr);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileExA(const char* pSrc, const char* pDest);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileNoExA(const char* pSrc, const char* pDest);
	//Memory(内存)
	virtual void PLUMCRYPT_CALLMODE PlumEnCryptFileExtractFromMemoryExA(const char* pDest, DWORD* pLuckyArr);
	virtual void PLUMCRYPT_CALLMODE PlumEnCryptFileExtractFromMemoryExA(const char* pDest, const void* pArray, int nSize, DWORD* pLuckyArr);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileStoreInMemoryExA(const char* pSrc);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileStoreInMemoryExA(const char* pSrc, const void* pArray, int nSize);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileInMemoryStoreInFileExA(const void* pArray, PlumFileInfo sSrcArrayInfo, const char* pDest);
	virtual void PLUMCRYPT_CALLMODE PlumDeCryptFileInMemoryStoreInMemoryExA(const void* pArray, PlumFileInfo sSrcArrayInfo);
};

#endif