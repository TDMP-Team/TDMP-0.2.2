#include "Global.h"
#include "Script.h"
#include "Lua.h"
#include "Lua.hpp"
#include "multiplayer/Main.h"

#include <vector>

void Tick(CScriptCore* pSC, lua_State* L, CRetInfo* ret)
{
	TDMP::LuaTick();
}

std::vector<LUA::Callback> callbacks;
void RegisterCallback(CScriptCore* pSC, lua_State* L, CRetInfo* ret)
{
	TDMP::Debug::print(L);
	int top = lua_gettop(L);

	const char* callbackName = luaL_checkstring(L, 1);

	if (lua_isfunction(L, 2))
	{
		LUA::Callback callback;

		callback.L = L;
		callback.name = callbackName;
		callback.ref = luaL_ref(L, LUA_REGISTRYINDEX);
	
		callbacks.push_back(callback);

		TDMP::Debug::print("Callback \"" + std::string(callbackName) + "\" was registered");
	}

	lua_pop(L, top);
}

void CallCallback(CScriptCore* pSC, lua_State* L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	const char* callbackName = luaL_checkstring(L, 1);
	
	for (size_t i = 0; i < callbacks.size(); i++)
	{
		if (callbacks[i].name == callbackName)
		{
			lua_rawgeti(callbacks[i].L, LUA_REGISTRYINDEX, callbacks[i].ref);
			lua_pushvalue(callbacks[i].L, 2);
			if (lua_pcall(callbacks[i].L, 1, 0, 0))
			{
				const char* err = lua_tolstring(callbacks[i].L, -1, NULL);
				TDMP::Debug::error(err);

				lua_pop(callbacks[i].L, 1);
			}

			lua_pop(callbacks[i].L, lua_gettop(callbacks[i].L));

			break;
		}
	}

	lua_pop(L, top);
}

void LUA::RegisterLuaCFunctions(CScriptCore_LuaState* pSCLS)
{
	RegisterLuaFunction(pSCLS, "TDMP_Tick", Tick);

	RegisterLuaFunction(pSCLS, "TDMP_RegisterCallback", RegisterCallback);
	RegisterLuaFunction(pSCLS, "TDMP_CallCallback", CallCallback);
}

void* luaAlloc(void* userData, void* ptr, size_t oldSize, size_t newSize)
{ 
	if (newSize == 0)
	{
		if (ptr != nullptr)
			glb::oTFree((uintptr_t)ptr);
		
		return NULL;
	}
	else
		return glb::oTRealloc(ptr, newSize);
}


void hRegisterGameFunctions(CScriptCore* pSC)
{
	void* ud;
	lua_Alloc a = lua_getallocf(*pSC->m_SCLuaState.m_LuaState, &ud);
	TDMP::Debug::print(a);

	lua_setallocf(*pSC->m_SCLuaState.m_LuaState, a, ud);

	glb::RegisterGameFunctions(pSC);

	TDMP::Debug::print("Registering lua functions");

	LUA::RegisterLuaCFunctions(&pSC->m_SCLuaState);
}

int hluaL_loadbuffer(lua_State* L, const char* buff, size_t size, const char* name)
{
	return glb::oluaL_loadbuffer(L, buff, size, name);
}

void LUA::HookRegisterGameFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)glb::RegisterGameFunctions, hRegisterGameFunctions);
	DetourTransactionCommit();
}

int CallLuaFunction(lua_State* L) {
	int iRetCount = 0;
	LUA::tLuaFunction pFunction = reinterpret_cast<LUA::tLuaFunction>(lua_topointer(L, lua_upvalueindex(1)));
	const void* pScriptCore = lua_topointer(L, lua_upvalueindex(2));

	pFunction(reinterpret_cast<const CScriptCore*>(pScriptCore), L, &iRetCount);

	return iRetCount;
}

void LUA::RegisterLuaFunction(CScriptCore_LuaState* pSCLS, const char* cFunctionName, void* pFunction)
{
	TDMP::Debug::print("Registering " + std::string(cFunctionName));

	lua_pushlightuserdata(*pSCLS->m_LuaState, pFunction);
	lua_pushlightuserdata(*pSCLS->m_LuaState, pSCLS);
	lua_pushcclosure(*pSCLS->m_LuaState, CallLuaFunction, 2);

	lua_setglobal(*pSCLS->m_LuaState, cFunctionName);
	//glb::tdRegisterLuaFunction(pSCLS, cFunctionName, pFunction);

}