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
// @Purpose: CConvallariaLua构造函数
// @Since: v1.00a
// @Para: const char* szFile	// Lua文件路径(相对)
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
// @Purpose: CConvallariaLua析构函数
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
// @Purpose: CConvallariaLua获取Lua虚拟机
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
// @Purpose: CConvallariaLua初始化
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
// @Purpose: CConvallariaLua获取全局变量(Int)
// @Since: v1.00a
// @Para: const char * szVar	// Lua字符变量
// @Para: int & nValue			// Int整型值
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Int(const char * szVar, int & nValue)
{
	lua_getglobal(L, szVar);
	nValue = (int)lua_tointeger(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalLong()
// @Purpose: CConvallariaLua获取全局变量(Long)
// @Since: v1.00a
// @Para: const char * szVar	// Lua字符变量
// @Para: long & lValue			// Long长整型值
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Long(const char * szVar, long & lValue)
{
	lua_getglobal(L, szVar);
	lValue = (long)lua_tointeger(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLua_GetGlobal_LongLong()
// @Purpose: CConvallariaLua获取全局变量(Long Long)
// @Since: v1.00a
// @Para: const char * szVar	// Lua字符变量
// @Para: long long & llValue	// Long Long长整型值
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_LongLong(const char * szVar, long long & llValue)
{
	lua_getglobal(L, szVar);
	llValue = (long long)lua_tointeger(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalFloat()
// @Purpose: CConvallariaLua获取全局变量(Float)
// @Since: v1.00a
// @Para: const char * szVar	// Lua字符变量
// @Para: float & fValue		// Float单精度值
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Float(const char * szVar, float & fValue)
{
	lua_getglobal(L, szVar);
	fValue = (float)lua_tonumber(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLuaGetGlobalDouble()
// @Purpose: CConvallariaLua获取全局变量(Double)
// @Since: v1.00a
// @Para: const char * szVar	// Lua字符变量
// @Para: double & dValue		// Doublt双精度值
// @Return: None
//------------------------------------------------------------------
void CONVALLARIALUA_CALLMODE CConvallariaLua::ConvallariaLua_GetGlobal_Double(const char * szVar, double & dValue)
{
	lua_getglobal(L, szVar);
	dValue = (double)lua_tonumber(L, -1);
}

//------------------------------------------------------------------
// @Function:	 ConvallariaLua_GetGlobal_String()
// @Purpose: CConvallariaLua获取全局字符串(String)
// @Since: v1.00a
// @Para: const char * szVar	// Lua字符变量
// @Para: const char * pStr		// 字符数组地址
// @Para: int nSize				// 字符数组长度
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
// @Purpose: CConvallariaLua获取表字符串(String)
// @Since: v1.00a
// @Para: const char * szVar1	// Lua表名称
// @Para: const char * szVar2	// Lua元素名称
// @Para: const char * pStr		// 字符数组地址
// @Para: int nSize				// 字符数组长度
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
// @Purpose: CConvallariaLua运行函数(lua)
// @Since: v1.00a
// @Para: const char * szVar	// Lua函数名称
// @Para: int nMessageID		// Lua函数消息ID
// @Para: LPVOID pTransBuf		// 发送参数结构体地址
// @Para: int nTransSize		// 发送参数结构体长度
// @Para: LPVOID pRecevBuf		// 接收参数结构体地址
// @Para: int nRecevSize		// 接收参数结构体长度
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
