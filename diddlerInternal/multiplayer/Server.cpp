#include "steam/headers/steam_api.h"
#include "steam/headers/steam_gameserver.h"
#include "steam/headers/isteamgameserver.h"
#include "steam/headers/isteamnetworkingsockets.h"
#include "steam/headers/isteamnetworkingutils.h"

#include "Lobby.h"
#include "Server.h"
#include "Main.h"
#include "Messages.h"
#include "Client.h" // unusual, huh?

#include "Player.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "../drawCube.h"
#include "../objectSpawner.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/vec3.hpp"

#include "../Lua.h"

#include <mutex>
#include <cmath>

TDMP::Server* TDMP::server;

// Thanks to Valve for making source code of Spacewar public

uint16 port = 27016;
TDMP::Server::Server()
	:CallbackConnectionStatus(this, &Server::OnConnectionStatusChanged),
	CallbackServersConnected(this, &Server::OnSteamServersConnected)
{
	ConnectedToSteam = false;
	TDMP::Debug::print("Initializing server..", Env::Server);
	SteamGameServer_Init(INADDR_ANY, 27015, 27016, eServerModeAuthentication, TDMP::Version.c_str());

	if (SteamGameServer())
	{
		Debug::print("Game server initialized", Env::Server);

		SteamGameServer()->SetModDir("Teardown");
		SteamGameServer()->SetProduct("Teardown");
		SteamGameServer()->SetGameDescription("A Teardown server");

		SteamGameServer()->LogOnAnonymous();
		//SteamNetworkingUtils()->InitRelayNetworkAccess();

		SteamGameServer()->EnableHeartbeats(true);
	}

	TDMP::server = this;

	socket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
	if (socket == k_HSteamListenSocket_Invalid)
	{
		Debug::error("Failed to listen on port " + std::to_string(port));
		return;
	}

	pollGroup = SteamGameServerNetworkingSockets()->CreatePollGroup();
	if (pollGroup == k_HSteamNetPollGroup_Invalid)
	{
		Debug::error("Failed to listen on port " + std::to_string(port));
		return;
	}

	TDMP::Debug::print("Server initialized!", Env::Server);

	ClientConnectionData_t ConnectionNone;
	ConnectionNone.Active = false;
}

void TDMP::Server::OnSteamServersConnected(SteamServersConnected_t* pCallback)
{
	SteamGameServer()->SetMaxPlayerCount(MaxPlayers);

	ConnectedToSteam = true;
	TDMP::Debug::print("Server connected to Steam servers!", Env::Server);

	uint64 serverId = SteamGameServer()->GetSteamID().ConvertToUint64();
	//SteamIPAddress_t ip = SteamGameServer()->GetPublicIP();

	SteamMatchmaking()->SetLobbyGameServer(TDMP::lobby->id, 0, 0, serverId);//ip.m_unIPv4, port, serverId);
	//SteamMatchmaking()->SetLobbyJoinable(lobby->id, false);
	Debug::print("Marked lobby as started", Env::Server);

	LUA::CallCallbacks("ServerInitialised");
}

void TDMP::Server::Tick()
{
	SteamGameServerNetworkingSockets()->RunCallbacks();
	SteamGameServer_RunCallbacks();

	if (!ConnectedToSteam)
		return;

	ReceiveNetData();
}

std::vector<LuaCallbackQueue> callbackQueue;
void TDMP::Server::LuaTick()
{
	if (!ConnectedToSteam)
		return;

	if (glb::game->State != gameState::ingame || !TDMP::LevelLoaded)
		return;
	
	int callbacks = callbackQueue.size();
	if (callbacks > 0)
	{
		for (size_t i = 0; i < callbacks; i++)
		{
			LUA::CallEvent(callbackQueue[i].callback.c_str(), callbackQueue[i].json.c_str(), callbackQueue[i].steamId);
		}

		callbackQueue.clear();
	}
}

void TDMP::Server::LuaUpdate()
{
	if (!TDMP::LevelLoaded)
		return;

	int bodiesId = 0;
	int packs = 0;

	MsgUpdateBodies msg;

	if (glb::steamDrmIntegrity >= 90) // if not cracked
	{
		msg.SetGlobal(false);
		for (size_t i = 0; i < TDMP::levelBodies.size(); i++)
		{
			TDBody* body = TDMP::levelBodies[i];

			if (body == 0)
				continue;

			// This is needed for sending currently grabbed body only once without any brainfuck or addditional "ifs"
			if (glb::player->grabbedBody == body)
			{
				MsgUpdateBody oneBody;

				oneBody.SetBody(body);
				server->BroadcastData(&oneBody, sizeof(oneBody), k_nSteamNetworkingSend_Unreliable);

				continue;
			}

			float len = pow(body->Velocity.x, 2) + pow(body->Velocity.y, 2) + pow(body->Velocity.z, 2);

			if (body->Type == entityType::Vehicle)
			{
				TDVehicle* veh = (TDVehicle*)body;

				if ((len == 0 || len <= .005f) && !veh->m_RemoteDrive)
					continue;
			}
			else if (len == 0 || len <= 0.005f)
				continue;

			msg.PushBody(bodiesId, MsgBody{ body->Position, body->Rotation, body->Velocity, body->RotationVelocity, body->Id });
			bodiesId++;

			// if next body would reach limit of packet
			if (bodiesId + 1 >= 40)
			{
				server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable); // then send it

				packs++;
				bodiesId = 0;
			}
		}
	}
	else // otherwise if cracked then fun happens after first destruction. It would send each new created body, what's not synced between clients because of random. In result it would look like a bug
	{
		msg.SetGlobal(true);
		for (size_t i = 0; i < glb::game->m_Scene->m_Bodies->size(); i++)
		{
			TDBody* body = glb::game->m_Scene->m_Bodies->data()[i];

			if (body == 0)
				continue;

			// This is needed for sending currently grabbed body only once without any brainfuck or addditional "ifs"
			if (glb::player->grabbedBody == body)
			{
				MsgUpdateBody oneBody;

				oneBody.SetBody(body);
				server->BroadcastData(&oneBody, sizeof(oneBody), k_nSteamNetworkingSend_Unreliable);

				continue;
			}

			float len = pow(body->Velocity.x, 2) + pow(body->Velocity.y, 2) + pow(body->Velocity.z, 2);

			if (body->Type == entityType::Vehicle)
			{
				TDVehicle* veh = (TDVehicle*)body;

				if ((len == 0 || len <= .005f) && !veh->m_RemoteDrive)
					continue;
			}
			else if (len == 0 || len <= 0.005f)
				continue;

			msg.PushBody(bodiesId, MsgBody{ body->Position, body->Rotation, body->Velocity, body->RotationVelocity, body->Id });
			bodiesId++;

			// if next body would reach limit of packet
			if (bodiesId + 1 >= 40)
			{
				server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable); // then send it

				packs++;
				bodiesId = 0;
			}
		}
	}

	// Sending last body array
	if (bodiesId > 0)
	{
		packs++;

		server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable);
	}
}

void TDMP::Server::ReceiveNetData()
{
	SteamNetworkingMessage_t* msgs[128];
	int numMessages = SteamGameServerNetworkingSockets()->ReceiveMessagesOnPollGroup(pollGroup, msgs, 128);

	for (int i = 0; i < numMessages; i++)
	{
		SteamNetworkingMessage_t* message = msgs[i];
		CSteamID steamID = message->m_identityPeer.GetSteamID();
		HSteamNetConnection connection = message->m_conn;

		if (!lobby->IsMember(steamID))
			continue;

		if (message->GetSize() < sizeof(DWORD))
		{
			// got garbage
			Debug::print("Received garbage data", Env::Server);
			message->Release();
			message = nullptr;
			continue;
		}

		EMessage eMsg = (EMessage)LittleDWord(*(DWORD*)message->GetData());

		switch (eMsg)
		{
		case k_EMsgServerUpdateWorld:
		{
			break;
		}
		case k_EMsgClientExiting:
		{
			break;
		}
		case k_EMsgPlayerSledgeHit:
		{
			MsgSledgeHit* pMsg = (MsgSledgeHit*)message->GetData();
			if (pMsg == nullptr)
			{
				Debug::print("corrupted k_EMsgPlayerSledgeHit received");
				break;
			}

			MsgSledgeHit msg;
			msg.SetData(pMsg->GetPos(), pMsg->GetDamageA(), pMsg->GetDamageB());

			server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable, true);

			break;
		}
		case k_EMsgPlayerTransform:
		{
			MsgPlayerData* pMsg = (MsgPlayerData*)message->GetData();
			if (pMsg == nullptr)
			{
				Debug::print("corrupted k_EMsgPlayerTransform received");
				break;
			}

			// Here we're setting steamid of client which sent to us information about him. We can also make it here sending position from server's side (Like if we were simulating player movement on serverside),
			// but this mod would be used by friends so we don't really need to use anti-exploit/cheat things here?

			// looks terrible i know
			MsgPlayerData msg;
			msg.SetPlayer(steamID, pMsg->GetPosition(), pMsg->GetRotation(), pMsg->GetCamPosition(), pMsg->GetCamRotation(),
				pMsg->GetVehicle(), pMsg->GetHealth(), pMsg->GetCtrl(), pMsg->GetHeldItem(), pMsg->ToolExists(), pMsg->GetToolPosition(), pMsg->GetToolRotation());

			if (pMsg->GetGrabbed().id != 0 && TDMP::levelBodiesById.count(pMsg->GetGrabbed().id))
			{
				TDMP::bodyQueue.push_back(MsgBody{
					pMsg->GetGrabbed().pos,
					pMsg->GetGrabbed().rot,
					pMsg->GetGrabbed().vel,
					pMsg->GetGrabbed().rotVel,

					pMsg->GetGrabbed().id
					});
			}

			server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable);
			TDMP::client->HandlePlayerData(&msg, &connection);

			break;
		}
		case k_EMsgLuaCallback:
		{
			MsgLuaCallback* pMsg = (MsgLuaCallback*)message->GetData();
			if (pMsg == nullptr)
			{
				Debug::print("corrupted k_EMsgLuaCallback received");
				break;
			}

			// pcall must be called from game loop
			callbackQueue.push_back(LuaCallbackQueue{ std::string(pMsg->GetCallback()), std::string(pMsg->GetJson()), std::to_string(steamID.ConvertToUint64()), pMsg->GetJsonLength() });

			break;
		}
		case k_EMsgClientBeginAuthentication:
		{
			if (message->GetSize() != sizeof(MsgClientBeginAuthentication))
			{
				Debug::error("Bad connection attempt msg");
				break;
			}
			MsgClientBeginAuthentication* pMsg = (MsgClientBeginAuthentication*)message->GetData();

			OnClientBeginAuthentication(steamID, connection, (void*)pMsg->GetTokenPtr(), pMsg->GetTokenLen());
		}
		break;
		default:
			Debug::print("Received invalid data", Env::Server);
		}

		message->Release();
		message = nullptr;
	}
}

void TDMP::Server::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	HSteamNetConnection hConn = pCallback->m_hConn;
	SteamNetConnectionInfo_t info = pCallback->m_info; /// Full connection info
	ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState; /// Previous state.  (Current state is in m_info.m_eState)

	// if client wants to connects
	if (info.m_hListenSocket &&
		eOldState == k_ESteamNetworkingConnectionState_None &&
		info.m_eState == k_ESteamNetworkingConnectionState_Connecting)
	{
		for (uint32 i = 0; i < MaxPlayers; ++i)
		{
			if (!ClientData[i].Active && !PendingClientData[i].handle) // if slot available
			{
				EResult res = SteamGameServerNetworkingSockets()->AcceptConnection(hConn);
				if (res != k_EResultOK) // connection failed
				{
					char msg[256];
					sprintf(msg, "AcceptConnection returned %d", res);
					Debug::error(msg);
					SteamGameServerNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);

					return;
				}

				PendingClientData[i].handle = hConn; // Setting us as a pending client

				SteamGameServerNetworkingSockets()->SetConnectionPollGroup(hConn, pollGroup);

				MsgServerConnectionResult msg;
				msg.SetSuccess(true);

				SteamGameServerNetworkingSockets()->SendMessageToConnection(hConn, &msg, sizeof(MsgServerConnectionResult), k_nSteamNetworkingSend_Reliable, nullptr);

				Debug::print("Accepted new connection!", Env::Server);

				return;
			}
		}

		Debug::print("Rejecting connection: Server full", Env::Server);
		SteamGameServerNetworkingSockets()->CloseConnection(hConn, k_ESteamNetConnectionEnd_App_Generic, "Server full!", false);
	}
	// client disconects
	else if ((eOldState == k_ESteamNetworkingConnectionState_Connecting || eOldState == k_ESteamNetworkingConnectionState_Connected) &&
		info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer)
	{
		for (uint32 i = 0; i < MaxPlayers; ++i)
		{
			if (!ClientData[i].Active) // empty slot
				continue;

			if (ClientData[i].SteamIDUser == info.m_identityRemote.GetSteamID())
			{
				ClientData[i].Active = false;

				// Flush all others messages of connection
				SteamGameServerNetworkingSockets()->FlushMessagesOnConnection(ClientData[i].handle);

				// and then clean it
				memset(&PendingClientData[i], 0, sizeof(ClientConnectionData_t));
				PendingClientData[i].Active = false;

				Debug::print("Player " + std::to_string(ClientData[i].SteamIDUser.ConvertToUint64()) + " disconnected", Env::Server);
				SteamGameServer()->EndAuthSession(info.m_identityRemote.GetSteamID());

				MsgClientExiting msg;
				msg.SetPlayer(ClientData[i].SteamIDUser);

				BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Reliable);
				SendDataToConnection(&msg, sizeof(msg), ClientData[0].handle, k_nSteamNetworkingSend_Reliable); // letting our client know that player left

				SteamGameServerNetworkingSockets()->CloseConnection(ClientData[i].handle, k_ESteamNetConnectionEnd_App_Min + 1, nullptr, false);
				memset(&ClientData[i], 0, sizeof(ClientConnectionData_t));

				break;
			}
		}
	}
}

// A bit too many loops were used in here... But it's only for connection at level start so doesn't matter
void TDMP::Server::OnClientBeginAuthentication(CSteamID steamID, HSteamNetConnection connectionID, void* pToken, uint32 uTokenLen)
{
	// First we need to check is client already logged
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (ClientData[i].handle == connectionID)
			return; // Yup, already logged in
	}

	// Here we had to check is there a room on our server but.. We're using lobbies, so they already doing it well.

	Debug::print(std::to_string(steamID.ConvertToUint64()) + " begins authentication", Env::Server);

	// we need to tell the server our Steam id in the non-auth case, so we stashed it in the login message, pull it back out

	// bUt, we need to check, is this person even member of the lobby?
	if (!lobby->IsMember(steamID))
	{
		// HA! Impostor
		for (uint32 i = 0; i < MaxPlayers; ++i)
		{
			if (PendingClientData[i].handle == connectionID)
			{
				memset(&PendingClientData[i], 0, sizeof(ClientConnectionData_t)); // get a hell out of here!!!
				break;
			}
		}
		SteamGameServerNetworkingSockets()->CloseConnection(connectionID, k_ESteamNetConnectionEnd_AppException_Min + 2, "You're not member of lobby. https://www.youtube.com/watch?v=dQw4w9WgXcQ", false);
		return;
	}

	// So now all should be good

	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (!PendingClientData[i].Active)
		{
			PendingClientData[i].Active = true;
			PendingClientData[i].handle = connectionID;
			PendingClientData[i].SteamIDUser = steamID;

			EBeginAuthSessionResult res = SteamGameServer()->BeginAuthSession(pToken, uTokenLen, steamID);
			if (res != k_EBeginAuthSessionResultOK)
			{
				SteamGameServerNetworkingSockets()->CloseConnection(connectionID, k_ESteamNetConnectionEnd_AppException_Min + 3, "BeginAuthSession failed", false);
				Debug::error("BeginAuthSession failed");
				break;
			}

			OnAuthCompleted(i);

			break;
		}
	}
}

void TDMP::Server::OnAuthCompleted(int index)
{
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (!ClientData[i].Active)
		{
			// copy over the data from the pending array
			memcpy(&ClientData[i], &PendingClientData[index], sizeof(ClientConnectionData_t));
			memset(&PendingClientData[index], 0, sizeof(ClientConnectionData_t));

			Debug::print("Player " + GetPlayerNick(ClientData[i].SteamIDUser.ConvertToUint64()) + " has authed!", Env::Server);

			MsgServerAuthed msg;
			SteamGameServerNetworkingSockets()->SendMessageToConnection(ClientData[i].handle, &msg, sizeof(msg), k_nSteamNetworkingSend_Reliable, nullptr);

			return;
		}
	}

	Debug::error("Something went totally wrong in OnAuthCompleted!");
}

void TDMP::Server::SendDataToConnection(const void* pData, uint32 nSizeOfData, HSteamNetConnection hConn, int nSendFlags)
{
	SteamGameServerNetworkingSockets()->SendMessageToConnection(hConn, pData, nSizeOfData, nSendFlags, nullptr);
}

TDMP::Server::ClientConnectionData_t TDMP::Server::GetPlayerByID(uint64_t id)
{
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		ClientConnectionData_t client = ClientData[i];
		if (client.Active && client.SteamIDUser == id)
			return client;
	}

	return ConnectionNone;
}

void TDMP::Server::BroadcastData(const void* pData, uint32 nSizeOfData, int nSendFlags, bool sendSelf)
{
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		ClientConnectionData_t client = ClientData[i];
		if (client.Active)
		{
			if (sendSelf || client.SteamIDUser != GetSteamID())
				SendDataToConnection(pData, nSizeOfData, client.handle, nSendFlags);
		}
	}
}

void TDMP::Server::Shutdown()
{
	if (ConnectedToSteam)
	{
		for (uint32 i = 0; i < MaxPlayers; ++i)
		{
			if (ClientData[i].Active)
			{
				// Flush all others messages of connection
				SteamGameServerNetworkingSockets()->FlushMessagesOnConnection(ClientData[i].handle);
				MsgServerExiting msg;
				// And send new one
				SteamGameServerNetworkingSockets()->SendMessageToConnection(ClientData[i].handle, &msg, sizeof(msg), k_nSteamNetworkingSend_Reliable, nullptr);

				ClientData[i].Active = false;
			}

			if (players[i].body.body != 0x00)
			{
				players[i].Active = false;
			}
		}

		SteamGameServer()->EnableHeartbeats(false);

		SteamGameServerNetworkingSockets()->CloseListenSocket(socket);
		SteamGameServerNetworkingSockets()->DestroyPollGroup(pollGroup);
		SteamGameServer()->LogOff();

		SteamGameServer_Shutdown();
	}

	SteamMatchmaking()->SetLobbyJoinable(TDMP::lobby->id, true);
	
	Debug::print("Server is closed. Lobby is joinable now", Env::Server);

	delete TDMP::server;
	TDMP::server = nullptr;
}