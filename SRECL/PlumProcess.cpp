/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumProcess.cpp
* @brief	This Program is PlumProcess DLL Project.
* @author	Alopex/Helium
* @version	v1.01a
* @date		2018-1-30	v1.00a	alopex	Create Project.
* @date		2018-7-13	v1.01a	alopex	Add Static Function.
*/
#include "PlumProcess.h"

//This Project is about Muti-Process/New Process.(Win32进程相关)

//------------------------------------------------------------------
// @Function:	 CPlumProcess()
// @Purpose: PlumProcess构造函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumProcess::CPlumProcess()
{
	m_dwProcessID = 0;
	m_dwThreadID = 0;
	m_hProcess = NULL;
	m_hThread = NULL;
}

//------------------------------------------------------------------
// @Function:	 ~CPlumProcess()
// @Purpose: PlumProcess析构函数
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumProcess::~CPlumProcess()
{
}

//------------------------------------------------------------------
// @Function:	 PlumProcessGetProcessID()
// @Purpose: PlumProcess获取进程ID
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
DWORD PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessGetProcessID(void) const
{
	return m_dwProcessID;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessGetThreadID()
// @Purpose: PlumProcess获取线程ID
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
DWORD PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessGetThreadID(void) const
{
	return m_dwThreadID;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessGetProcessHandle()
// @Purpose: PlumProcess获取进程句柄
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
HANDLE PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessGetProcessHandle(void) const
{
	return m_hProcess;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessGetThreadHandle()
// @Purpose: PlumProcess获取线程句柄
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
HANDLE PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessGetThreadHandle(void) const
{
	return m_hThread;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessSetProcessID(DWORD dwProcessID)
// @Purpose: PlumProcess设置进程ID
// @Since: v1.00a
// @Para: DWORD dwProcessID		//进程ID
// @Return: None
//------------------------------------------------------------------
void PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessSetProcessID(DWORD dwProcessID)
{
	m_dwProcessID = dwProcessID;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessSetThreadID(DWORD dwThreadID)
// @Purpose: PlumProcess设置线程ID
// @Since: v1.00a
// @Para: DWORD dwThreadID		//线程ID
// @Return: None
//------------------------------------------------------------------
void PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessSetThreadID(DWORD dwThreadID)
{
	m_dwThreadID = dwThreadID;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessA(const char* strProcessName)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Return: None
//------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessA(const char* strProcessName)
{
	BOOL bRet = FALSE;

	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, strProcessName);

	bRet = CreateProcessA(chProcessPath, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	m_dwProcessID = pi.dwProcessId;
	m_dwThreadID = pi.dwThreadId;

	return bRet;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessW(const wchar_t* strProcessName)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const wchar_t* strProcessName		//进程名称(Unicode)
// @Return: None
//------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessW(const wchar_t* strProcessName)
{
	BOOL bRet = FALSE;

	int nSize = 0;
	char* pstrProcessName = NULL;

	nSize = ::WideCharToMultiByte(CP_ACP, 0, strProcessName, -1, NULL, 0, NULL, NULL);
	pstrProcessName = new char[nSize + 1];
	memset(pstrProcessName, 0, (nSize + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, strProcessName, -1, pstrProcessName, nSize, NULL, NULL);


	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, pstrProcessName);

	bRet = CreateProcessA(chProcessPath, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	m_dwProcessID = pi.dwProcessId;
	m_dwThreadID = pi.dwThreadId;

	delete[] pstrProcessName;
	pstrProcessName = NULL;

	return bRet;
}

//--------------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessNoCloseA(const char* strProcessName)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Return: None
//--------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessNoCloseA(const char* strProcessName)
{
	BOOL bRet = FALSE;

	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, strProcessName);

	bRet = CreateProcessA(chProcessPath, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	m_dwProcessID = pi.dwProcessId;
	m_dwThreadID = pi.dwThreadId;
	m_hProcess = pi.hProcess;
	m_hThread = pi.hThread;

	return bRet;
}

//-----------------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessNoCloseW(const wchar_t* strProcessName)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const wchar_t* strProcessName		//进程名称(Unicode)
// @Return: None
//-----------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessNoCloseW(const wchar_t* strProcessName)
{
	BOOL bRet = FALSE;

	int nSize = 0;
	char* pstrProcessName = NULL;

	nSize = ::WideCharToMultiByte(CP_ACP, 0, strProcessName, -1, NULL, 0, NULL, NULL);
	pstrProcessName = new char[nSize + 1];
	memset(pstrProcessName, 0, (nSize + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, strProcessName, -1, pstrProcessName, nSize, NULL, NULL);


	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, pstrProcessName);

	bRet = CreateProcessA(chProcessPath, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	m_dwProcessID = pi.dwProcessId;
	m_dwThreadID = pi.dwThreadId;
	m_hProcess = pi.hProcess;
	m_hThread = pi.hThread;

	delete[] pstrProcessName;
	pstrProcessName = NULL;

	return bRet;
}

//-----------------------------------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessA(const char* strProcessName, const char * strProcessCmd)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Para: const char* strProcessCmd		//命令行(ASCII)
// @Return: None
//-----------------------------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessA(const char * strProcessName, const char * strProcessCmd)
{
	BOOL bResult;

	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, strProcessName);

	CHAR chCmdLine[MAX_PATH] = { 0 };
	strcat_s(chCmdLine, chProcessPath);
	strcat_s(chCmdLine, " ");
	strcat_s(chCmdLine, strProcessCmd);

	bResult = CreateProcessA(chProcessPath, chCmdLine, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bResult)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	m_dwProcessID = pi.dwProcessId;
	m_dwThreadID = pi.dwThreadId;

	return bResult;
}

//-----------------------------------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessA(const char* strProcessName, const char * strProcessCmd)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Para: const char* strProcessCmd		//命令行(ASCII)
// @Return: None
//-----------------------------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessNoCloseA(const char * strProcessName, const char * strProcessCmd)
{
	BOOL bResult;

	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, strProcessName);

	CHAR chCmdLine[MAX_PATH] = { 0 };
	strcat_s(chCmdLine, chProcessPath);
	strcat_s(chCmdLine, " ");
	strcat_s(chCmdLine, strProcessCmd);

	bResult = CreateProcessA(chProcessPath, chCmdLine, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bResult)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	m_dwProcessID = pi.dwProcessId;
	m_dwThreadID = pi.dwThreadId;
	m_hProcess = pi.hProcess;
	m_hThread = pi.hThread;

	return bResult;
}

//--------------------------------------------------------------------------
// @Function:	 PlumProcessIsProcessExistA(const char* strProcessName)
// @Purpose: PlumProcess检测进程是否存在
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Return: None
//--------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessIsProcessExistA(const char * strProcessName)
{
	BOOL bRet = FALSE;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	int nSize = 0;
	wchar_t* pstrProcessName = NULL;

	nSize = ::MultiByteToWideChar(CP_ACP, 0, strProcessName, -1, NULL, NULL);
	pstrProcessName = new wchar_t[nSize + 1];
	memset(pstrProcessName, 0, (nSize + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, strProcessName, -1, (LPWSTR)pstrProcessName, nSize);

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do {
			if (_wcsicmp(pe32.szExeFile, pstrProcessName) == 0)
			{
				bRet = TRUE;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}
	else {
		bRet = FALSE;
	}
	CloseHandle(hProcessSnap);

	delete[] pstrProcessName;
	pstrProcessName = NULL;

	return bRet;
}

//--------------------------------------------------------------------------
// @Function:	 PlumProcessIsProcessExistW(const wchar_t* strProcessName)
// @Purpose: PlumProcess检测进程是否存在
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(Unicode)
// @Return: None
//--------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessIsProcessExistW(const wchar_t * strProcessName)
{
	BOOL bRet = FALSE;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do {
			if (_wcsicmp(pe32.szExeFile, strProcessName) == 0)
			{
				bRet = TRUE;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}
	else {
		bRet = FALSE;
	}
	CloseHandle(hProcessSnap);

	return bRet;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessA(const char* strProcessName)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Return: None
//------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessExA(const char * strProcessName)
{
	BOOL bRet = FALSE;

	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, strProcessName);

	bRet = CreateProcessA(chProcessPath, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	return bRet;
}

//------------------------------------------------------------------
// @Function:	 PlumProcessStartProcessW(const wchar_t* strProcessName)
// @Purpose: PlumProcess启动进程
// @Since: v1.00a
// @Para: const wchar_t* strProcessName		//进程名称(Unicode)
// @Return: None
//------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessStartProcessExW(const wchar_t * strProcessName)
{
	BOOL bRet = FALSE;

	int nSize = 0;
	char* pstrProcessName = NULL;

	nSize = ::WideCharToMultiByte(CP_ACP, 0, strProcessName, -1, NULL, 0, NULL, NULL);
	pstrProcessName = new char[nSize + 1];
	memset(pstrProcessName, 0, (nSize + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, strProcessName, -1, pstrProcessName, nSize, NULL, NULL);


	STARTUPINFOA si = { 0 };
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	GetStartupInfoA(&si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi = { 0 };
	ZeroMemory(&pi, sizeof(pi));

	char chProcessPath[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chProcessPath, MAX_PATH);
	pTemp = strrchr(chProcessPath, '\\');
	if (pTemp) *pTemp = '\0';
	strcat_s(chProcessPath, "\\");
	strcat_s(chProcessPath, pstrProcessName);

	bRet = CreateProcessA(chProcessPath, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);

	if (bRet)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	delete[] pstrProcessName;
	pstrProcessName = NULL;

	return bRet;
}

//--------------------------------------------------------------------------
// @Function:	 PlumProcessIsProcessExistA(const char* strProcessName)
// @Purpose: PlumProcess检测进程是否存在
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(ASCII)
// @Return: None
//--------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessIsProcessExistExA(const char * strProcessName)
{
	BOOL bRet = FALSE;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	int nSize = 0;
	wchar_t* pstrProcessName = NULL;

	nSize = ::MultiByteToWideChar(CP_ACP, 0, strProcessName, -1, NULL, NULL);
	pstrProcessName = new wchar_t[nSize + 1];
	memset(pstrProcessName, 0, (nSize + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, strProcessName, -1, (LPWSTR)pstrProcessName, nSize);

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do {
			if (_wcsicmp(pe32.szExeFile, pstrProcessName) == 0)
			{
				bRet = TRUE;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}
	else {
		bRet = FALSE;
	}
	CloseHandle(hProcessSnap);

	delete[] pstrProcessName;
	pstrProcessName = NULL;

	return bRet;
}

//--------------------------------------------------------------------------
// @Function:	 PlumProcessIsProcessExistW(const wchar_t* strProcessName)
// @Purpose: PlumProcess检测进程是否存在
// @Since: v1.00a
// @Para: const char* strProcessName	//进程名称(Unicode)
// @Return: None
//--------------------------------------------------------------------------
BOOL PLUMPROCESS_CALLMODE CPlumProcess::PlumProcessIsProcessExistExW(const wchar_t * strProcessName)
{
	BOOL bRet = FALSE;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do {
			if (_wcsicmp(pe32.szExeFile, strProcessName) == 0)
			{
				bRet = TRUE;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}
	else {
		bRet = FALSE;
	}
	CloseHandle(hProcessSnap);

	return bRet;
}
