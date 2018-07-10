/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumPath.cpp
* @brief	This Program is PlumPath DLL Project.
* @author	Alopex/Helium
* @version	v1.01a
* @date		2018-01-18	v1.00a	alopex	Create Project.
* @date		2018-07-10	v1.01a	alopex	Add Function.
*/
#include "PlumPath.h"

//This Class is FilePath Correlation(~·�����...)

//------------------------------------------------------------------
// @Function:	 CPlumPath()
// @Purpose: CPlumPath���캯��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumPath::CPlumPath()
{
}

//------------------------------------------------------------------
// @Function:	 ~CPlumPath()
// @Purpose: CPlumPath��������
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumPath::~CPlumPath()
{
}

//------------------------------------------------------------------
// @Function:	 PlumPathConvertA2W()
// @Purpose: CPlumPath����ת��A2W
// @Since: v1.00a
// @Para: const char* szArr		// Դ�ַ�����
// @Para: const wchar_t* wszArr	// Ŀ���ַ�����(���鴫��)
// @Para: int nSize				// Ŀ�����鳤��
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathConvertA2W(const char* szArr, const wchar_t* wszArr, int nSize)
{
	int nSrcSize = 0;
	wchar_t* pwszArr = NULL;
	nSrcSize = ::MultiByteToWideChar(CP_ACP, 0, szArr, -1, NULL, NULL);
	if (nSize < nSrcSize + 1)
	{
		return FALSE;
	}
	pwszArr = new wchar_t[nSrcSize + 1];
	memset(pwszArr, 0, (nSrcSize + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, szArr, -1, (LPWSTR)pwszArr, nSrcSize);
	memset((void*)wszArr, 0, nSize * sizeof(wchar_t));
	memcpy_s((void*)wszArr, nSize * sizeof(wchar_t), pwszArr, (nSrcSize + 1) * sizeof(wchar_t));
	delete[] pwszArr;
	pwszArr = NULL;
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathConvertW2A()
// @Purpose: CPlumPath����ת��W2A
// @Since: v1.00a
// @Para: const wchar_t* wszArr	// Դ�ַ�����
// @Para: const char* szArr		// Ŀ���ַ�����(���鴫��)
// @Para: int nSize				// Ŀ�����鳤��
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathConvertW2A(const wchar_t* wszArr, const char* szArr, int nSize)
{
	int nSrcSize = 0;
	char* pszArr = NULL;
	nSrcSize = ::WideCharToMultiByte(CP_ACP, 0, wszArr, -1, NULL, 0, NULL, NULL);
	if (nSize < nSrcSize + 1)
	{
		return FALSE;
	}
	pszArr = new char[nSrcSize + 1];
	memset(pszArr, 0, (nSrcSize + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, wszArr, -1, pszArr, nSrcSize, NULL, NULL);
	memset((void*)szArr, 0, nSize * sizeof(char));
	memcpy_s((void*)szArr, nSize * sizeof(char), pszArr, (nSrcSize + 1) * sizeof(char));
	delete[] pszArr;
	pszArr = NULL;
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathGetModulePathA()
// @Purpose: CPlumPath��ȡģ�鵱ǰ·��
// @Since: v1.00a
// @Para: const char* szArr		//ģ��·����ַ
// @Para: int nSize				//ģ��·������
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathGetModulePathA(const char* szArr, int nSize)
{
	char chArr[MAX_PATH] = { 0 };
	char* pTemp = NULL;
	int nArrSize = 0;
	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	nArrSize = strlen(chArr);
	if (nSize < nArrSize + 1)
	{
		return FALSE;
	}
	memset((void*)szArr, 0, nSize * sizeof(char));
	memcpy_s((void*)szArr, nSize * sizeof(char), chArr, (nArrSize + 1) * sizeof(char));
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathGetModulePathW()
// @Purpose: CPlumPath��ȡģ�鵱ǰ·��
// @Since: v1.00a
// @Para: const wchar_t* wszArr	//ģ��·����ַ
// @Para: int nSize				//ģ��·������
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathGetModulePathW(const wchar_t* wszArr, int nSize)
{
	char chArr[MAX_PATH] = { 0 };
	char* pTemp = NULL;
	int nArrSize = 0;
	USES_CONVERSION;
	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	BOOL bReturn = FALSE;
	bReturn = PlumPathConvertA2W(chArr, wszArr, nSize);
	if (!bReturn)
	{
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathGetLocalPathA()
// @Purpose: CPlumPath��ȡģ�鵱ǰ·���ļ���
// @Since: v1.00a
// @Para: const char* szArr		//ģ��·����ַ
// @Para: int nSize				//ģ��·������
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathGetLocalPathA(const char* szArr, int nSize)
{
	char chArr[MAX_PATH] = { 0 };
	char* pTemp = NULL;
	int nArrSize = 0;
	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	pTemp = strrchr(chArr, '\\');
	if (pTemp != NULL)
	{
		*pTemp = '\0';
	}
	nArrSize = strlen(chArr);
	if (nSize < nArrSize + 1)
	{
		return FALSE;
	}
	memset((void*)szArr, 0, nSize * sizeof(char));
	memcpy_s((void*)szArr, nSize * sizeof(char), chArr, (nArrSize + 1) * sizeof(char));
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathGetLocalPathW()
// @Purpose: CPlumPath��ȡģ�鵱ǰ·���ļ���
// @Since: v1.00a
// @Para: const wchar_t* wszArr	//ģ��·����ַ
// @Para: int nSize				//ģ��·������
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathGetLocalPathW(const wchar_t* wszArr, int nSize)
{
	char chArr[MAX_PATH] = { 0 };
	char* pTemp = NULL;
	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	pTemp = strrchr(chArr, '\\');
	if (pTemp != NULL)
	{
		*pTemp = '\0';
	}
	BOOL bReturn = FALSE;
	bReturn = PlumPathConvertA2W(chArr, wszArr, nSize);
	if (!bReturn)
	{
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathGetParentPathA()
// @Purpose: CPlumPath��ȡģ�鸸��·���ļ���
// @Since: v1.00a
// @Para: const char* szArr		//ģ��·����ַ
// @Para: int nSize				//ģ��·������
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathGetParentPathA(const char* szArr, int nSize, int nDeep)
{
	char chArr[MAX_PATH] = { 0 };
	int nArrSize = 0;
	if (nDeep <= 0)
	{
		return FALSE;
	}
	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	for (int i = 0; i < nDeep; ++i)
	{
		char* pTemp = NULL;
		pTemp = strrchr(chArr, '\\');
		if (pTemp != NULL)
		{
			*pTemp = '\0';
		}
	}
	nArrSize = strlen(chArr);
	if (nSize < nArrSize + 1)
	{
		return FALSE;
	}
	memset((void*)szArr, 0, nSize * sizeof(char));
	memcpy_s((void*)szArr, nSize * sizeof(char), chArr, (nArrSize + 1) * sizeof(char));
	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 PlumPathGetParentPathW()
// @Purpose: CPlumPath��ȡģ�鸸��·���ļ���
// @Since: v1.00a
// @Para: const wchar_t* wszArr	//ģ��·����ַ
// @Para: int nSize				//ģ��·������
// @Return: BOOL(true:�ɹ�, false:ʧ��)
//------------------------------------------------------------------
BOOL PLUMPATH_CALLMODE CPlumPath::PlumPathGetParentPathW(const wchar_t* wszArr, int nSize, int nDeep)
{
	char chArr[MAX_PATH] = { 0 };
	if (nDeep <= 0)
	{
		return FALSE;
	}
	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	for (int i = 0; i < nDeep; ++i)
	{
		char* pTemp = NULL;
		pTemp = strrchr(chArr, '\\');
		if (pTemp != NULL)
		{
			*pTemp = '\0';
		}
	}
	BOOL bReturn = FALSE;
	bReturn = PlumPathConvertA2W(chArr, wszArr, nSize);
	if (!bReturn)
	{
		return FALSE;
	}
	return TRUE;
}