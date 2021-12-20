#pragma once
#include "types.h"

struct lua_State;

class CRetInfo
{
public:
	lua_State* L;
	int retCount;
	int maxRet;
};

class CScriptCore_LuaState
{
public:
    char pad_0000[56];    //0x0000
    lua_State** m_LuaState;
};

class CScriptCore
{
public:
    float m_RunTime;
    float m_UpdateTimeStep;
    td::small_string m_ScriptPath;
    td::small_string m_ScriptLocation;
    CScriptCore_LuaState m_SCLuaState;
    bool m_HasCallbacks;
    bool m_HasInit;
    bool m_HasTick;
    bool m_HasUpdate;
    bool m_HasDraw;
    char pad_0075[3];
    float m_TickProcessed;
    float m_UpdateProcessed;
    char pad_0080[3032];

    virtual void Destroy(CScriptCore* pScriptCore, bool freeMemory);
    virtual void RegisterGameFunctions(CScriptCore* pScriptCore);
    virtual void LoadScript(CScriptCore* pScriptCore, td::small_string* ssScriptPath);
    virtual void Function3();
    virtual void Init(CScriptCore* pScriptCore);
    virtual void Tick(CScriptCore* pScriptCore, float fTimeStep, void* pUnknown);
    virtual void Update(CScriptCore* pScriptCore, float fTimeStep);
    virtual void Function7();
    virtual void Function8();
};


class Script : public Entity
{
public:
	char* m_ScriptName;			//0x0028
	char pad_0030[8];			//0x0030
	CScriptCore m_ScriptCore;	//0x0038
}; //Size: 0xBE0