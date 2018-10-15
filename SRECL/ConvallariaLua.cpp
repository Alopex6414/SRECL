/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		ConvallariaLua.cpp
* @brief	This Program is C/C++ -- Lua Script Project.
* @author	alopex
* @version	v1.02a
* @date		2018-08-27	v1.00a alopex Create this file.
* @date		2018-08-30	v1.01a alopex Add function.
* @date		2018-09-02	v1.02a alopex Add function.
*/
#include "ConvallariaLua.h"
#include "ConvallariaLuaMsg.h"
#include "ConvallariaLuaType.h"

//C/C++ -- Lua Script Dynamic Link Library.

//------------------------------------------------------------------
// @Function:	 CConvallariaLua()
// @Purpose: CConvallariaLua���캯��
// @Since: v1.00a
// @Para: const char* szFile	// Lua�ļ�·��(���)
// @Return: None
//------------------------------------------------------------------
CConvallariaLua::CConvallariaLua(const char* szFile)
{
	L = luaL_newstate();
	memset(m_chFile, 0, MAX_PATH);
	memset(m_chPath, 0, MAX_PATH);
	memcpy_s(m_chFile, MAX_PATH, szFile, strlen(szFile));
}

//------------------------------------------------------------------
// @Function:	 ~CConvallariaLua()
// @Purpose: CConvallariaLua��������
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CConvallariaLua::~CConvallariaLua()
{
	if (L)
	{
		lua_close(L);
	}
}

//------------------------------------------------------------------
// @Function:	CConvallariaLuaGetLua()
// @Purpose: CConvallariaLua��ȡLua�����
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
lua_State *CONVALLARIALUA_CALLMODE CConvallariaLua::CConvallariaLua_GetLua() const
{
	return L;
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaInit()
// @Purpose: CConvallariaLua��ʼ��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
BOOL CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_Init()
{
	char chArr[MAX_PATH] = { 0 };
	char* pTemp = NULL;

	GetModuleFileNameA(NULL, chArr, MAX_PATH);
	pTemp = strrchr(chArr, '\\');
	if (pTemp != NULL)
	{
		*pTemp = '\0';
	}

	strcat_s(chArr, "\\");
	strcat_s(chArr, m_chFile);
	memcpy_s(m_chPath, MAX_PATH, chArr, MAX_PATH);

	int nRet = -1;

	nRet = luaL_loadfile(L, chArr);
	if (nRet)
	{
		return FALSE;
	}

	nRet = lua_pcall(L, 0, 0, 0);
	if (nRet)
	{
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalInt()
// @Purpose: CConvallariaLua��ȡȫ�ֱ���(Int)
// @Since: v1.00a
// @Para: const char * szVar	// Lua�ַ�����
// @Para: int & nValue			// Int����ֵ
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Int(const char * szVar, int & nValue)
{
	lua_getglobal(L, szVar);
	nValue = (int)lua_tointeger(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalLong()
// @Purpose: CConvallariaLua��ȡȫ�ֱ���(Long)
// @Since: v1.00a
// @Para: const char * szVar	// Lua�ַ�����
// @Para: long & lValue			// Long������ֵ
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Long(const char * szVar, long & lValue)
{
	lua_getglobal(L, szVar);
	lValue = (long)lua_tointeger(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLua_GetGlobal_LongLong()
// @Purpose: CConvallariaLua��ȡȫ�ֱ���(Long Long)
// @Since: v1.00a
// @Para: const char * szVar	// Lua�ַ�����
// @Para: long long & llValue	// Long Long������ֵ
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_LongLong(const char * szVar, long long & llValue)
{
	lua_getglobal(L, szVar);
	llValue = (long long)lua_tointeger(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalFloat()
// @Purpose: CConvallariaLua��ȡȫ�ֱ���(Float)
// @Since: v1.00a
// @Para: const char * szVar	// Lua�ַ�����
// @Para: float & fValue		// Float������ֵ
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Float(const char * szVar, float & fValue)
{
	lua_getglobal(L, szVar);
	fValue = (float)lua_tonumber(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalDouble()
// @Purpose: CConvallariaLua��ȡȫ�ֱ���(Double)
// @Since: v1.00a
// @Para: const char * szVar	// Lua�ַ�����
// @Para: double & dValue		// Doublt˫����ֵ
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Double(const char * szVar, double & dValue)
{
	lua_getglobal(L, szVar);
	dValue = (double)lua_tonumber(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLua_GetGlobal_String()
// @Purpose: CConvallariaLua��ȡȫ���ַ���(String)
// @Since: v1.00a
// @Para: const char * szVar	// Lua�ַ�����
// @Para: const char * pStr		// �ַ������ַ
// @Para: int nSize				// �ַ����鳤��
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_String(const char * szVar, const char * pStr, int nSize)
{
	std::string str;
	lua_getglobal(L, szVar);
	str = lua_tostring(L, -1);
	strcpy_s((char*)pStr, nSize, str.c_str());
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLua_GetTable_String()
// @Purpose: CConvallariaLua��ȡ���ַ���(String)
// @Since: v1.00a
// @Para: const char * szVar1	// Lua������
// @Para: const char * szVar2	// LuaԪ������
// @Para: const char * pStr		// �ַ������ַ
// @Para: int nSize				// �ַ����鳤��
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetTable_String(const char * szVar1, const char * szVar2, const char * pStr, int nSize)
{
	std::string str;
	lua_getglobal(L, szVar1);
	lua_getfield(L, -1, szVar2);
	str = lua_tostring(L, -1);
	strcpy_s((char*)pStr, nSize, str.c_str());
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLua_CallFunc()
// @Purpose: CConvallariaLua���к���(lua)
// @Since: v1.00a
// @Para: const char * szVar	// Lua��������
// @Para: int nMessageID		// Lua������ϢID
// @Para: LPVOID pTransBuf		// ���Ͳ����ṹ���ַ
// @Para: int nTransSize		// ���Ͳ����ṹ�峤��
// @Para: LPVOID pRecevBuf		// ���ղ����ṹ���ַ
// @Para: int nRecevSize		// ���ղ����ṹ�峤��
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_CallFunc(const char * szVar, int nMessageID, LPVOID pTransBuf, int nTransSize, LPVOID pRecevBuf, int nRecevSize)
{
	switch (nMessageID)
	{
	case CONVALLARIALUA_MSG_NONE:
		break;
	default:
		break;
	}
}
