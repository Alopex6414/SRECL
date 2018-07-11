/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumCipherA.h
* @brief	This Program is PlumCipherA DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2017-12-13	v1.00a	alopex	Create Project
*/
#pragma once

#ifndef __PLUMCIPHERA_H_
#define __PLUMCIPHERA_H_

//Include Plum Common Header File
#include "PlumCommon.h"

//Macro Definition
#ifdef	SRECL_EXPORTS
#define PLUMCRYPT_API	__declspec(dllexport)
#else
#define PLUMCRYPT_API	__declspec(dllimport)
#endif

#define PLUMCRYPT_CALLMODE	__stdcall

//Class Definition
class PLUMCRYPT_API CPlumCipherA
{
private:
	unsigned char m_cSBox[256];
	unsigned char m_cInvSBox[256];
	unsigned char m_cExMatrix[11][4][4];

public:
	CPlumCipherA();														//构造
	~CPlumCipherA();													//析构

	CPlumCipherA(unsigned char* Key);									//构造

	void PLUMCRYPT_CALLMODE KeyExMatrix(unsigned char* Key, unsigned char Ex[][4][4]);
	unsigned char PLUMCRYPT_CALLMODE FFMul(unsigned char Var1, unsigned char Var2);

	void PLUMCRYPT_CALLMODE SubBytes(unsigned char* pState);
	void PLUMCRYPT_CALLMODE ShiftRows(unsigned char State[][4]);
	void PLUMCRYPT_CALLMODE MixColumns(unsigned char State[][4]);
	void PLUMCRYPT_CALLMODE AddRoundKey(unsigned char* pState, unsigned char* pK);
	
	void PLUMCRYPT_CALLMODE InvSubBytes(unsigned char* pState);
	void PLUMCRYPT_CALLMODE InvShiftRows(unsigned char State[][4]);
	void PLUMCRYPT_CALLMODE InvMixColumns(unsigned char State[][4]);
	
	void PLUMCRYPT_CALLMODE EnCipher(const unsigned char* pSrc, unsigned char* pDest);		//加密数组Array[4][4]	
	void PLUMCRYPT_CALLMODE DeCipher(const unsigned char* pSrc, unsigned char* pDest);		//解密数组Array[4][4]
	void PLUMCRYPT_CALLMODE EnCrypt(const void* pSrc, void* pDest, int nLen);				//加密数组
	void PLUMCRYPT_CALLMODE DeCrypt(const void* pSrc, void* pDest, int nLen);				//解密数组
};

#endif