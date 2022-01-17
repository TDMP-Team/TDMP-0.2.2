#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <thread>
#include <stdio.h>
#include <assert.h>

#pragma comment(lib, "steam/bin/win64/steam_api64.lib") 
#include "steam/headers/steam_api.h"
#include "steam/headers/isteammatchmaking.h"
#include "steam/headers/steam_gameserver.h"

#include "Main.h"
#include "Client.h"
#include "Server.h"
#include "Lobby.h"
#include "Messages.h"
#include "Player.h"

#include "../Lua.h"

std::string TDMP::CurrentTool = "";
td::Vec3 TDMP::toolPos;
td::Vec4 TDMP::toolRot;

std::string TDMP::Version = "0.2.1";

bool TDMP::LevelLoaded = false;

std::vector<MsgBody> TDMP::bodyQueue;
std::vector<TDBody*> TDMP::levelBodies{};
std::map<int, int> TDMP::levelBodiesById{};

TDMP::Input TDMP::localInputData;

void TDMP::Init()
{
	setlocale(LC_ALL, "rus"); // adds russian symbols support. Actually I don't know if it even needed because it's only for chat and chat would appear in game. Let's just leave it in here.
	SetEnvironmentVariable(L"SteamAppId", L"1167630");
	SetEnvironmentVariable(L"SteamGameId", L"1167630");

	if (SteamAPI_Init())
	{
		Debug::print("Steam's API was initialized(" + std::to_string(SteamUser()->GetSteamID().ConvertToUint64()) + ")");
		std::setlocale(LC_NUMERIC, "C");

		const DWORD buffSize = 65535;
		static char buffer[buffSize];
		GetEnvironmentVariableA("SteamAppId", buffer, buffSize);

		Debug::print(buffer);
		
		new Client();
	}
	else {
		Debug::print("Something wrong with the Steam API!");
		return;
	}
}

void TDMP::Host()
{
	if (lobby != nullptr)
	{
		if (TDMP::server != nullptr)
			return;
		
		if (SteamMatchmaking()->GetNumLobbyMembers(lobby->id) <= 1) // if we're alone in the lobby then don't launch the server but mark lobby as not joinable
		{
			//SteamMatchmaking()->SetLobbyJoinable(TDMP::lobby->id, false);
			//return;
		}

		new Server();
	}
}

void TDMP::Leave()
{
	if (TDMP::lobby != nullptr)
	{
		TDMP::lobby->Leave();
		delete TDMP::lobby;
		TDMP::lobby = nullptr;
	}
}

bool TDMP::ServerExists()
{
	return client->connectionState == k_EClientConnectedAndAuthenticated;
}

bool TDMP::IsServer()
{
	if (TDMP::lobby == nullptr)
		return false;

	return SteamMatchmaking()->GetLobbyOwner(lobby->id) == SteamUser()->GetSteamID();
}

void OnUnLoadLevel()
{
	TDMP::bodyQueue.clear();
	TDMP::levelBodies.clear();
	TDMP::levelBodiesById.clear();
	LUA::callbacks.clear();
	LUA::hooks.clear();
	LUA::spawnedBodies.clear();

	if (TDMP::server != nullptr)
	{
		for (uint32 i = 0; i < TDMP::MaxPlayers; ++i)
		{
			if (TDMP::server->ClientData[i].Active)
			{
				SteamGameServerNetworkingSockets()->FlushMessagesOnConnection(TDMP::server->ClientData[i].handle);
			}
		}
	}

	for (uint32 i = 0; i < TDMP::MaxPlayers; ++i)
	{
		TDMP::players[i].hideBody = false;
		TDMP::players[i].bodyExists = false;
		TDMP::players[i].lastItem = "";
		TDMP::players[i].heldItem = "";
		TDMP::players[i].toolBody.body = 0;
		TDMP::players[i].lastItem.clear();
	}

	TDMP::Debug::print("Level unloaded");
}

void OnLoadLevel()
{
	for (size_t i = 0; i < glb::game->m_Scene->m_Bodies->size(); i++)
	{
		TDBody* body = glb::game->m_Scene->m_Bodies->data()[i];

		entityType t = body->Type;

		if (t == entityType::Body || t == entityType::Vehicle)
		{
			TDMP::levelBodies.push_back(body);
			TDMP::levelBodiesById[body->Id] = TDMP::levelBodies.size() - 1;
		}
	}

	TDMP::Debug::print("Level loaded");
}

void TDMP::Tick()
{
	SteamAPI_RunCallbacks();

	if (glb::game->m_LoadingEffect == 0) // Level is reloading
	{
		if (!TDMP::LevelLoaded)
		{
			OnLoadLevel();
		}

		TDMP::LevelLoaded = true;
	}
	else
	{
		if (TDMP::LevelLoaded)
		{
			OnUnLoadLevel();
		}

		TDMP::LevelLoaded = false;
	}

	if (glb::game->State == gameState::ingame && TDMP::server == nullptr && TDMP::lobby != nullptr && TDMP::lobby->IsHost(SteamUser()->GetSteamID()))
	{
		TDMP::Host();
	}
	else if (glb::game->State != gameState::ingame && TDMP::server != nullptr)
	{
		TDMP::server->Shutdown();
	}

	if (TDMP::client != nullptr)
		TDMP::client->Tick();

	if (TDMP::server != nullptr)
		TDMP::server->Tick();
}

void TDMP::LuaTick()
{
	if (TDMP::client != nullptr)
		TDMP::client->LuaTick();
	if (TDMP::server != nullptr)
		TDMP::server->LuaTick();
}

void TDMP::LuaUpdate()
{
	if (TDMP::server != nullptr)
		TDMP::server->LuaUpdate();
}

void TDMP::SetGameState(int state)
{
	Debug::print("Changing game's state to " + std::to_string(state));
	switch (state)
	{
		case (eGameState::Playing): {
			SteamFriends()->SetRichPresence("status", "Playing on the map");
			break;
		}
		case (eGameState::Menu): {
			SteamFriends()->SetRichPresence("status", "Chilling in the main menu");

			if (client != nullptr)
				client->Disconnect();

			if (server != nullptr)
				server->Shutdown();
		}
		default:
			break;
	}
	Debug::print("State set!");
}

std::string TDMP::GetPlayerNick(uint64_t steamID)
{
	return std::string(SteamFriends()->GetFriendPersonaName(steamID));
}

uint64_t TDMP::GetSteamID()
{
	return SteamUser()->GetSteamID().ConvertToUint64();
}