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

#include <cmath>

TDMP::Server* TDMP::server;

// Thanks to Valve for making source code of Spacewar public

/*static void DebugOutput(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
{
	if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
	{
		TDMP::Debug::error(pszMsg);
		return;
	}

	TDMP::Debug::print(pszMsg, TDMP::Env::Server);
}*/

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
		SteamGameServer()->SetGameDescription("A Teardown server"); // damn a lot things to fill

		SteamGameServer()->LogOnAnonymous();
		//SteamNetworkingUtils()->InitRelayNetworkAccess();

		SteamGameServer()->EnableHeartbeats(true);
	}
	//SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, DebugOutput);

	/*SteamNetworkingIPAddr address;
	address.Clear();
	address.m_port = port;*/

	TDMP::server = this;

	socket = SteamGameServerNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
	//socket = SteamGameServerNetworkingSockets()->CreateListenSocketIP(address, 0, 0);
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
	//Debug::print(serverId);

	//SteamIPAddress_t ip = SteamGameServer()->GetPublicIP();

	SteamMatchmaking()->SetLobbyGameServer(TDMP::lobby->id, 0, 0, serverId);//ip.m_unIPv4, port, serverId);
	//SteamMatchmaking()->SetLobbyJoinable(lobby->id, false);
	Debug::print("Marked lobby as started", Env::Server);
}

void TDMP::Server::Tick()
{
	SteamGameServerNetworkingSockets()->RunCallbacks();
	SteamGameServer_RunCallbacks();

	if (!ConnectedToSteam)
		return;

	ReceiveNetData();

	if (!TDMP::LevelLoaded)
		return;

	//std::vector<MsgUpdateBodies> msgs;
	//for (size_t i = 0; i < glb::game->m_Scene->m_Bodies->size(); i++)

	//std::vector<MsgBody> bodies;
	for (size_t i = 0; i < TDMP::levelBodies.size(); i++)
	{
		TDBody* body = TDMP::levelBodies[i];
		//TDBody* body = glb::game->m_Scene->m_Bodies->data()[i];

		if (body == 0)
			continue;

		//drawCube(body->Position, 0.3f, td::redColor);

		//drawCube(body->Position, 0.2f, td::blueColor);

		float len = pow(body->Velocity.x, 2) + pow(body->Velocity.y, 2) + pow(body->Velocity.z, 2);
		
		if (len == 0 || len <= 0.007f)
			continue;

		// Game crashes when trying to access `VoxelCount` of a vehicle
		if (body->Type == entityType::Body)
		{
			TDShape* shape = (TDShape*)body->pChild;
			int voxelCount = 0;

			// Expensive, I guess?
			while (shape != 0) {
				if (shape->Type == entityType::Shape && shape->pVox != 0)
					voxelCount += shape->pVox->VoxelCount;

				shape = (TDShape*)shape->pSibling;
			}

			if (voxelCount <= 9)
				continue;
		}

		MsgUpdateBody msg;
		msg.SetBody(body);
		server->BroadcastData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable);
		//drawCube(body->Position, 0.1f, td::greenColor);
		
		/*if (bodies.size() >= 100)
		{
			MsgUpdateBodies msg;
			
			msg.Serialize(bodies);
			//msg.PushBodies(bodies);
			msgs.push_back(msg);

			bodies.clear();
		}

		bodies.push_back(MsgBody{ body->Position, body->Rotation, body->Velocity, body->RotationVelocity, body->Id });*/
	}

	/*Debug::print(std::string("Bodies vector: ") + std::to_string(bodies.size()));
	Debug::print(std::string("Packets vector: ") + std::to_string(msgs.size()));

	if (bodies.size() > 0)
	{
		MsgUpdateBodies msg;

		msg.Serialize(bodies);
		//msg.PushBodies(bodies);
		msgs.push_back(msg);
	}

	if (msgs.size() > 0)
	{
		for (size_t i = 0; i < msgs.size(); i++)
		{
			server->BroadcastData(&msgs[i], sizeof(msgs[i]), k_nSteamNetworkingSend_Unreliable);
		}

		Debug::print(std::string("Sending ") + std::to_string(msgs.size()) + " packet(s)");
	}*/
}

void TDMP::Server::Frame() { }

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
		case k_EMsgPlayerTransform:
		{
			MsgPlayerTransform* pMsg = (MsgPlayerTransform*)message->GetData();
			if (pMsg == nullptr)
			{
				Debug::print("corrupted k_EMsgPlayerTransform received");
				break;
			}

			server->BroadcastData(message->GetData(), message->GetSize(), k_nSteamNetworkingSend_Unreliable);
			TDMP::client->HandleData(eMsg, message);

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

			OnClientBeginAuthentication(pMsg->GetSteamID(), connection, (void*)pMsg->GetTokenPtr(), pMsg->GetTokenLen());
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
				SteamGameServerNetworkingSockets()->CloseConnection(ClientData[i].handle, k_ESteamNetConnectionEnd_App_Min + 1, nullptr, false);
				memset(&ClientData[i], 0, sizeof(ClientConnectionData_t));
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

void TDMP::Server::BroadcastData(const void* pData, uint32 nSizeOfData, int nSendFlags)
{
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		ClientConnectionData_t client = ClientData[i];
		if (client.Active)
		{
			if (client.SteamIDUser != GetSteamID())
				SendDataToConnection(pData, nSizeOfData, client.handle, nSendFlags);
		}
	}
}

void TDMP::Server::Shutdown()
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

	SteamMatchmaking()->SetLobbyJoinable(TDMP::lobby->id, true);

	delete TDMP::server;
	TDMP::server = nullptr;
	
	Debug::print("Server is closed. Lobby is joinable now", Env::Server);
}