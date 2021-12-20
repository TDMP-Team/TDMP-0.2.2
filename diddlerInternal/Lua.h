#pragma once
#include "Script.h"

namespace LUA
{
	void RegisterLuaCFunctions(CScriptCore_LuaState* pSCLS);
	void HookRegisterGameFunctions();

	typedef void (*tLuaFunction)(const CScriptCore* pSC, lua_State* L, int* iRetCount);

	void RegisterLuaFunction(CScriptCore_LuaState* pSCLS, const char* cFunctionName, void* pFunction);

	struct Callback
	{
		lua_State* L;
		const char* name;
		int ref;
	};
}