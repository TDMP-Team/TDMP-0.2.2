#pragma once
#include "Script.h"
#include <map>

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

	struct Hook
	{
		lua_State* L;
		std::string hook;
		const char* name;
		int ref;
	};

	extern std::vector<Callback> callbacks;
	extern std::map<std::string, std::map<const char*, LUA::Hook>> hooks;

	void RunLuaHooks(const char* hookName, const char* jsonData);
	void CallCallbacks(const char* callbackName);
	void CallEvent(const char* eventName, const char* jsonData);
}