#include "Global.h"
#include "Script.h"
#include "Lua.h"
#include "Lua.hpp"
#include "multiplayer/Main.h"
#include "multiplayer/Client.h"
#include "multiplayer/Server.h"
#include "multiplayer/Player.h"
#include "multiplayer/Lobby.h"

#include <vector>
#include <map>
#include <windef.h>
#include <WinBase.h>
#include <detours.h>

#define api_checknelems(L, n)	api_check(L, (n) <= (L->top - L->base))

#define api_checkvalidindex(L, i)	api_check(L, (i) != luaO_nilobject)

#define checkresults(L,na,nr) \
     api_check(L, (nr) == LUA_MULTRET || (L->ci->top - L->top >= (nr) - (na)))

#define adjustresults(L,nres) \
    { if (nres == LUA_MULTRET && L->top >= L->ci->top) L->ci->top = L->top; }

struct CallS {  /* data to `f_call' */
	StkId func;
	int nresults;
};

static void f_call(lua_State* L, void* ud) {
	struct CallS* c = cast(struct CallS*, ud);
	glb::oluaD_call(L, c->func, c->nresults);
}

int pcall(lua_State* L, int nargs, int nresults, int errfunc)
{
	struct CallS c;
	int status;
	ptrdiff_t func;

	lua_lock(L);

	api_checknelems(L, nargs + 1);
	checkresults(L, nargs, nresults);
	func = 0;

	c.func = L->top - (nargs + 1);  /* function to be called */
	c.nresults = nresults;
	status = glb::oluaD_pcall(L, f_call, &c, savestack(L, c.func), func);
	adjustresults(L, nresults);
	lua_unlock(L);

	return status;
}

static TValue* index2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

void settable(lua_State* L, int idx) {
	StkId t;
	lua_lock(L);
	api_checknelems(L, 2);
	t = index2adr(L, idx);//glb::olua_index2adr(L, idx);//
	api_checkvalidindex(L, t);
	glb::oluaV_settable(L, t, L->top - 2, L->top - 1);
	L->top -= 2;  /* pop index and value */
	lua_unlock(L);
}

void pushstring(lua_State* L, const char* str)
{
	glb::olua_pushlstring(L, str, strlen(str));
}

#define api_incr_top(L)   {api_check(L, L->top < L->ci->top); L->top++;}

void Tick(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	TDMP::LuaTick();
}

std::vector<LUA::Callback> LUA::callbacks{};
void RegisterCallback(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
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

		LUA::callbacks.push_back(callback);

		TDMP::Debug::print("Event \"" + std::string(callbackName) + "\" was registered");
	}

	lua_pop(L, top);
}

std::map<std::string, std::map<const char*, LUA::Hook>> LUA::hooks;

struct callHook
{
	LUA::Hook hook;
	std::string json;
};

std::map<int, std::vector<callHook>> hookQueue;

void AddHook(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	const char* hookEvent = luaL_checkstring(L, 1);
	const char* hookName = luaL_checkstring(L, 2);

	if (lua_isfunction(L, 3))
	{
		LUA::Hook hook;

		hook.L = L;
		hook.hook = std::string(hookEvent);
		hook.name = hookName;
		hook.ref = luaL_ref(L, LUA_REGISTRYINDEX);

		if (LUA::hooks.count(hook.hook) <= 0)
			LUA::hooks[hook.hook] = std::map<const char*, LUA::Hook>{};

		LUA::hooks[hook.hook][hook.name] = hook;
	}

	lua_pop(L, top);
}

void LUA::RunLuaHooks(const char* hookName, const char* jsonData)
{
	for (const auto& [hook, hookListeners] : LUA::hooks)
	{
		if (hook == std::string(hookName))
		{
			for (const auto& [hookName, hook] : hookListeners)
			{
				if (hook.L == nullptr)
					continue;

				hookQueue[(int)hook.L].push_back(callHook{
					hook, std::string(jsonData)
				});
			}

			break;
		}
	}
}

void RunGlobalHook(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	const char* hookName = luaL_checkstring(L, 1);
	const char* jsonData = luaL_checkstring(L, 2);
	lua_pop(L, top);

	LUA::RunLuaHooks(hookName, jsonData);
}

void RunHooks(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	for (size_t i = 0; i < hookQueue[(int)L].size(); i++)
	{
		callHook* cH = &hookQueue[(int)L][i];
		lua_rawgeti(cH->hook.L, LUA_REGISTRYINDEX, cH->hook.ref);

		pushstring(cH->hook.L, cH->json.c_str());
		if (pcall(cH->hook.L, 1, 0, 0))
		{
			const char* err = lua_tolstring(cH->hook.L, -1, NULL);
			TDMP::Debug::error(err);

			lua_pop(cH->hook.L, 1);
		}
	}

	hookQueue[(int)L].clear();
}

void LUA::CallCallbacks(const char* callbackName)
{
	for (size_t i = 0; i < callbacks.size(); i++)
	{
		if (std::string(callbacks[i].name) == std::string(callbackName))
		{
			Callback* callback = &callbacks.data()[i];

			if (callback->L == nullptr)
				continue;

			lua_rawgeti(callback->L, LUA_REGISTRYINDEX, callback->ref);

			if (pcall(callback->L, 0, 0, 0))
			{
				const char* err = lua_tolstring(callback->L, -1, NULL);
				TDMP::Debug::error(err);

				lua_pop(callback->L, 1);
			}
		}
	}
}

// TODO: Push SteamId of sender to the server
void LUA::CallEvent(const char* eventName, const char* jsonData, std::string steamId)
{
	for (size_t i = 0; i < callbacks.size(); i++)
	{
		Callback* callback = &callbacks.data()[i];

		if (std::string(callback->name) == std::string(eventName))
		{
			if (callback->L == nullptr)
				continue;

			lua_rawgeti(callback->L, LUA_REGISTRYINDEX, callback->ref);

			pushstring(callback->L, jsonData);
			
			bool isServ = TDMP::IsServer();
			if (isServ)
				pushstring(callback->L, steamId.c_str());

			if (pcall(callback->L, isServ ? 2 : 1, 0, 0))
			{
				const char* err = lua_tolstring(callback->L, -1, NULL);
				TDMP::Debug::error(err);

				lua_pop(callback->L, 1);
			}
		}
	}
}

void SendCallback(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	const char* callbackName = luaL_checkstring(L, 1);

	bool callbackFound = false;
	for (size_t i = 0; i < LUA::callbacks.size(); i++)
	{
		if (LUA::callbacks[i].name == callbackName)
		{
			callbackFound = true;

			break;
		}
	}

	if (!callbackFound)
	{
		lua_pop(L, top);

		glb::oluaL_error(L, "Unkown callback! (%s)", callbackName);

		return;
	}

	int secondArgType = lua_type(L, 2);
	bool reliable = lua_toboolean(L, 3);
	const char* json = luaL_checkstring(L, 4);

	int sendType = reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable;

	MsgLuaCallback msg;
	msg.SetCallback(callbackName);
	msg.SetJson(json);
	msg.SetReliable(reliable);

	// If server sends message to the client
	if (secondArgType == LUA_TSTRING)
	{
		if (TDMP::server == nullptr)
		{
			lua_pop(L, top);
			TDMP::Debug::error("Tried to SendEvent without server initialised");

			return;
		}

		const char* steamId = luaL_checkstring(L, 2);
		lua_pop(L, top);
		if (TDMP::playersBySteamId.count(steamId))
		{
			TDMP::server->SendDataToConnection(&msg, sizeof(msg), TDMP::players[TDMP::playersBySteamId[steamId]].conn, sendType);
		}
		else
		{
			TDMP::Debug::error("Tried to send event to unknown SteamID!");
		}

		return;
	}
	lua_pop(L, top);

	// Otherwise client sends message to the server
	TDMP::client->SendData(&msg, sizeof(msg), sendType);
}

void BroadcastCallback(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	if (TDMP::server == nullptr)
	{
		lua_pop(L, top);

		TDMP::Debug::error("Tried to broadcast without server initialised");

		return;
	}

	const char* callbackName = luaL_checkstring(L, 1);
	bool reliable = lua_toboolean(L, 2);
	bool sendSelf = lua_toboolean(L, 3);

	size_t realLength;
	const char* json = lua_tolstring(L, 4, &realLength);

	lua_pop(L, top);

	// We're checking does this callback exists at all, so we won't overflow network with empty callbacks 
	for (size_t i = 0; i < LUA::callbacks.size(); i++)
	{
		if (LUA::callbacks[i].name == callbackName)
		{
			MsgLuaCallback msg;
			msg.SetCallback(callbackName);
			msg.SetJson(json);

			TDMP::server->BroadcastData(&msg, sizeof(msg), reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable, sendSelf);

			return;
		}
	}

	glb::oluaL_error(L, "Unkown callback! (%s)", callbackName);
}

void pushplayer(lua_State* L, int plyId)
{
	TDMP::Player ply = TDMP::players[plyId];

	glb::olua_createtable(L, 0, 4);

	pushstring(L, "steamId");
	pushstring(L, ply.steamIdStr.c_str());
	settable(L, -3);

	pushstring(L, "nick");
	pushstring(L, ply.nick.c_str());
	settable(L, -3);

	pushstring(L, "id");
	lua_pushinteger(L, plyId);
	settable(L, -3);

	pushstring(L, "hp");
	lua_pushnumber(L, ply.hp);
	settable(L, -3);

	pushstring(L, "heldItem");
	pushstring(L, ply.heldItem.c_str());
	settable(L, -3);

	if (ply.currentVehicle != 0)
	{
		pushstring(L, "veh");
		lua_pushnumber(L, ply.currentVehicle->Id);
		settable(L, -3);
	}

	if (ply.body.body != 0 && ply.bodyExists)
	{
		pushstring(L, "body");
		lua_pushnumber(L, ply.body.body->Id);
		settable(L, -3);
	}
}

void GetPlayerTransform(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, 1);

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	TDMP::Player ply = TDMP::players[id];

	glb::olua_createtable(L, 0, 2);

	pushstring(L, "pos");
		glb::olua_createtable(L, 0, 3);

		lua_pushnumber(L, 1);
		lua_pushnumber(L, ply.Position.x);
		settable(L, -3);

		lua_pushnumber(L, 2);
		lua_pushnumber(L, ply.Position.y);
		settable(L, -3);

		lua_pushnumber(L, 3);
		lua_pushnumber(L, ply.Position.z);
		settable(L, -3);
	settable(L, -3);

	pushstring(L, "rot");
		glb::olua_createtable(L, 0, 4);

		lua_pushnumber(L, 1);
		lua_pushnumber(L, ply.Rotation.x);
		settable(L, -3);

		lua_pushnumber(L, 2);
		lua_pushnumber(L, ply.Rotation.y);
		settable(L, -3);

		lua_pushnumber(L, 3);
		lua_pushnumber(L, ply.Rotation.z);
		settable(L, -3);

		lua_pushnumber(L, 4);
		lua_pushnumber(L, ply.Rotation.w);
		settable(L, -3);
	settable(L, -3);

	ret->retCount = 1;
}

void GetPlayerCameraTransform(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, 1);

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	TDMP::Player ply = TDMP::players[id];

	glb::olua_createtable(L, 0, 2);

	pushstring(L, "pos");
		glb::olua_createtable(L, 0, 3);

		lua_pushnumber(L, 1);
		lua_pushnumber(L, ply.CamPosition.x);
		settable(L, -3);

		lua_pushnumber(L, 2);
		lua_pushnumber(L, ply.CamPosition.y);
		settable(L, -3);

		lua_pushnumber(L, 3);
		lua_pushnumber(L, ply.CamPosition.z);
		settable(L, -3);
	settable(L, -3);

	pushstring(L, "rot");
		glb::olua_createtable(L, 0, 4);

		lua_pushnumber(L, 1);
		lua_pushnumber(L, ply.CamRotation.w);
		settable(L, -3);

		lua_pushnumber(L, 2);
		lua_pushnumber(L, ply.CamRotation.x);
		settable(L, -3);

		lua_pushnumber(L, 3);
		lua_pushnumber(L, ply.CamRotation.y);
		settable(L, -3);

		lua_pushnumber(L, 4);
		lua_pushnumber(L, ply.CamRotation.z);
		settable(L, -3);
	settable(L, -3);

	ret->retCount = 1;
}

/// <summary>
/// Returns table of active players
/// </summary>
void GetPlayers(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	glb::olua_createtable(L, 0, 0);

	int k = 0;
	for (uint32 i = 0; i < TDMP::MaxPlayers; ++i)
	{
		TDMP::Player ply = TDMP::players[i];
	
		if (ply.Active)
		{
			lua_pushinteger(L, ++k);
			pushplayer(L, i);
			settable(L, -3);
		}
	}

	ret->retCount = 1;
}

/// <summary>
/// Returns player at specific id
/// </summary>
void GetPlayer(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, 1);
	
	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	pushplayer(L, id);

	ret->retCount = 1;
}

/// <summary>
/// This function returns not cached player's nick
/// </summary>
void GetNick(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, lua_gettop(L));

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	pushstring(L, SteamFriends()->GetPlayerNickname(TDMP::players[id].SteamId));

	ret->retCount = 1;
}

void GetLocalNick(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	pushstring(L, SteamFriends()->GetFriendPersonaName(SteamUser()->GetSteamID()));

	ret->retCount = 1;
}

void GetHeldItem(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, lua_gettop(L));

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	pushstring(L, TDMP::players[id].heldItem.c_str());

	ret->retCount = 1;
}

void GetToolTransform(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, lua_gettop(L));

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	TDMP::Player ply = TDMP::players[id];

	glb::olua_createtable(L, 0, 2);

	pushstring(L, "pos");
	glb::olua_createtable(L, 0, 3);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, ply.ToolPosition.x);
	settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushnumber(L, ply.ToolPosition.y);
	settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushnumber(L, ply.ToolPosition.z);
	settable(L, -3);
	settable(L, -3);

	pushstring(L, "rot");
	glb::olua_createtable(L, 0, 4);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, ply.ToolRotation.w);
	settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushnumber(L, ply.ToolRotation.x);
	settable(L, -3);

	lua_pushnumber(L, 3);
	lua_pushnumber(L, ply.ToolRotation.y);
	settable(L, -3);

	lua_pushnumber(L, 4);
	lua_pushnumber(L, ply.ToolRotation.z);
	settable(L, -3);
	settable(L, -3);

	ret->retCount = 1;
}

void GetTableId(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	const char* steamId = luaL_checkstring(L, 1);
	lua_pop(L, top);

	if (TDMP::playersBySteamId.count(steamId))
	{
		lua_pushinteger(L, TDMP::playersBySteamId[steamId]);

		ret->retCount = 1;
	}
}

void IsMe(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, 1);
	
	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	lua_pushboolean(L, TDMP::players[id].IsMe());

	ret->retCount = 1;
}

bool notCached = true;
std::string localSteamId;
void LocalSteamId(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	if (notCached)
	{
		notCached = false;

		localSteamId = std::to_string(SteamUser()->GetSteamID().ConvertToUint64());
	}

	pushstring(L, localSteamId.c_str());

	ret->retCount = 1;
}

void IsServerInitialised(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	lua_pushboolean(L, TDMP::server != nullptr);

	ret->retCount = 1;
}

void IsServer(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	lua_pushboolean(L, TDMP::IsServer());

	ret->retCount = 1;
}

void SetPlayerModelEnabled(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	bool enable = lua_toboolean(L, 2);

	lua_pop(L, lua_gettop(L));

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	if (TDMP::players[id].Active)
	{
		if (enable)
		{
			TDMP::players[id].hideBody = false;
			TDMP::players[id].CreateBodyIfNotExists();
		}
		else
		{
			TDMP::players[id].hideBody = true;
			TDMP::players[id].RemoveBodyIfExists();
		}
	}
}

void GetPlayerModelEnabled(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, lua_gettop(L));

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	lua_pushboolean(L, !TDMP::players[id].hideBody);

	ret->retCount = 1;
}

void ConsolePrint(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int top = lua_gettop(L);

	std::string output = "";
	for (size_t i = 1; i <= top; i++)
	{
		output += lua_tostring(L, i) + std::string("\t");
	}
	
	TDMP::Debug::print(output);
}

void Update(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	TDMP::LuaUpdate();
}

void Time(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	lua_pushnumber(L, ((lua_Number)clock()) / (lua_Number)CLOCKS_PER_SEC);

	ret->retCount = 1;
}

std::map<std::string, std::string> toolsModels{};
std::string LUA::GetToolPath(std::string toolName)
{
	if (toolsModels.count(toolName))
		return toolsModels[toolName];
	else
		return "";
}

void RegisterTool(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	const char* tool = luaL_checkstring(L, 1);
	const char* path = luaL_checkstring(L, 2);

	std::string luaPath = std::string(path);
	std::string realPath = std::string(pSC->m_ScriptLocation.c_str()) + luaPath.substr(3, luaPath.size());

	toolsModels[tool] = realPath;
}

void GetToolBody(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	int id = luaL_checkinteger(L, 1);
	lua_pop(L, lua_gettop(L));

	if (id > TDMP::MaxPlayers - 1 || id < 0)
	{
		glb::oluaL_error(L, "incorrect id (must be 0-%d)", TDMP::MaxPlayers - 1);

		return;
	}

	TDMP::Player ply = TDMP::players[id];
	lua_pushnumber(L, ply.toolBodyExists && ply.toolBody.body != 0 ? (ply.toolBody.body->Id) : 0);

	ret->retCount = 1;
}

void GetTDMPVersion(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	pushstring(L, TDMP::Version.c_str());

	ret->retCount = 1;
}

void GetLobbyMembers(CScriptCore* pSC, lua_State*& L, CRetInfo* ret)
{
	if (TDMP::lobby == 0)
		return;

	uint64 id = TDMP::lobby->id;
	int memCount = SteamMatchmaking()->GetNumLobbyMembers(id);

	glb::olua_createtable(L, 0, memCount);
	for (int i = 0; i < memCount; i++)
	{
		CSteamID steamId = SteamMatchmaking()->GetLobbyMemberByIndex(id, i);
		lua_pushnumber(L, i + 1);
		
		glb::olua_createtable(L, 0, 3);
			pushstring(L, "steamId");
				pushstring(L, std::to_string(steamId.ConvertToUint64()).c_str());
			settable(L, -3);

			pushstring(L, "nick");
				pushstring(L, SteamFriends()->GetFriendPersonaName(steamId));
			settable(L, -3);

			pushstring(L, "host");
			lua_pushboolean(L, TDMP::lobby->IsHost(steamId));
			settable(L, -3);
		settable(L, -3);
	}

	ret->retCount = 1;
}

void LUA::RegisterLuaCFunctions(CScriptCore_LuaState* pSCLS)
{
	hookQueue[(int)*pSCLS->m_LuaState] = std::vector<callHook>{};


	RegisterLuaFunction(pSCLS, "TDMP_RegisterToolVox", RegisterTool);

	RegisterLuaFunction(pSCLS, "TDMP_GetLobbyMembers", GetLobbyMembers);

	RegisterLuaFunction(pSCLS, "TDMP_ConPrint", ConsolePrint);
	RegisterLuaFunction(pSCLS, "TDMP_UnixTime", Time);
	RegisterLuaFunction(pSCLS, "TDMP_Version", GetTDMPVersion);

	RegisterLuaFunction(pSCLS, "TDMP_Tick", Tick);
	RegisterLuaFunction(pSCLS, "TDMP_Update", Update);
	RegisterLuaFunction(pSCLS, "TDMP_IsServerInitialised", IsServerInitialised);
	RegisterLuaFunction(pSCLS, "TDMP_IsServer", IsServer);

	RegisterLuaFunction(pSCLS, "TDMP_RegisterEvent", RegisterCallback);
	RegisterLuaFunction(pSCLS, "TDMP_SendEvent", SendCallback);
	RegisterLuaFunction(pSCLS, "TDMP_BroadcastEvent", BroadcastCallback);

	RegisterLuaFunction(pSCLS, "TDMP_GetPlayers", GetPlayers);

	RegisterLuaFunction(pSCLS, "TDMP_GetPlayer", GetPlayer);
	RegisterLuaFunction(pSCLS, "TDMP_GetPlayerTransform", GetPlayerTransform);
	RegisterLuaFunction(pSCLS, "TDMP_GetPlayerCameraTransform", GetPlayerCameraTransform);
	RegisterLuaFunction(pSCLS, "TDMP_GetPlayerBodyEnabled", GetPlayerModelEnabled);
	RegisterLuaFunction(pSCLS, "TDMP_SetPlayerBodyEnabled", SetPlayerModelEnabled);
	RegisterLuaFunction(pSCLS, "TDMP_GetPlayerToolTransform", GetToolTransform);
	RegisterLuaFunction(pSCLS, "TDMP_GetPlayerToolBody", GetToolBody);

	RegisterLuaFunction(pSCLS, "TDMP_LocalNickname", GetLocalNick);
	RegisterLuaFunction(pSCLS, "TDMP_LocalSteamId", LocalSteamId);
	RegisterLuaFunction(pSCLS, "TDMP_IsMe", IsMe);

	RegisterLuaFunction(pSCLS, "TDMP_RunGlobalHook", RunGlobalHook);
	RegisterLuaFunction(pSCLS, "TDMP_AddGlobalHookListener", AddHook);
	RegisterLuaFunction(pSCLS, "TDMP_Hook_Queue", RunHooks);
}

void hRegisterGameFunctions(CScriptCore* pSC)
{
	glb::RegisterGameFunctions(pSC);

	TDMP::Debug::print("Registering lua functions");

	LUA::RegisterLuaCFunctions(&pSC->m_SCLuaState);
}

void makehole(TDScene* scene, td::Vec3* position, float damageA, float damageB, int unkn1, int* unkn2)
{
	MsgSledgeHit msg;
	msg.SetData(*position, damageA, damageB);

	if (TDMP::server == nullptr)
		TDMP::client->SendData(&msg, sizeof(msg), k_nSteamNetworkingSend_Reliable);
	else
		TDMP::server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Reliable);

	glb::oWrappedDamage(scene, position, damageA, damageB, unkn1, unkn2);
}

void LUA::HookRegisterGameFunctions()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)glb::RegisterGameFunctions, hRegisterGameFunctions);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)glb::oWrappedDamage, makehole);
	DetourTransactionCommit();
}

int CallLuaFunction(lua_State* L) {
	int iRetCount = 0;
	LUA::tLuaFunction pFunction = reinterpret_cast<LUA::tLuaFunction>(lua_topointer(L, lua_upvalueindex(1)));
	const void* pScriptCore = lua_topointer(L, lua_upvalueindex(2));

	const CScriptCore* sc = reinterpret_cast<const CScriptCore*>(pScriptCore);

	pFunction(sc, L, &iRetCount);

	return iRetCount;
}

void LUA::RegisterLuaFunction(CScriptCore_LuaState* pSCLS, const char* cFunctionName, void* pFunction)
{
	//lua_pushlightuserdata(*pSCLS->m_LuaState, pFunction);
	//lua_pushlightuserdata(*pSCLS->m_LuaState, pSCLS);
	//lua_pushcclosure(*pSCLS->m_LuaState, CallLuaFunction, 2);

	//lua_setglobal(*pSCLS->m_LuaState, cFunctionName);
	td::small_string cFuncName(cFunctionName);
	glb::tdRegisterLuaFunction(pSCLS, &cFuncName, pFunction);
}