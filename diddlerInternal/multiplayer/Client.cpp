#include "Main.h"
#include "Client.h"
#include "Lobby.h"
#include "Player.h"
#include <thread>
#include <Windows.h>
#include <sstream>
#include <mutex>

#include "../drawCube.h"
#include "../objectSpawner.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/vec3.hpp"

#include "../Lua.h"

TDMP::Client* TDMP::client;
std::vector<std::string> TDMP::packets;

/// <summary>
/// Creates lobby, initializes all needed stuff.
/// </summary>
TDMP::Client::Client()
{
	Debug::print("Initializing client..", Env::Client);
	SteamNetworking()->AllowP2PPacketRelay(true); // allows us to send and receive p2p packets

	CreateLobby();

	TDMP::client = this;
	connectionState = k_EClientNotConnected;

	Debug::print("Initialized client!", Env::Client);
}

void TDMP::Client::CreateLobby()
{
	Debug::print("Creating lobby..", Env::Client);
	SteamAPICall_t hSteamAPICall = SteamMatchmaking()->CreateLobby(ELobbyType::k_ELobbyTypeFriendsOnly, MaxPlayers);
	CallResultLobbyCreated.Set(hSteamAPICall, this, &Client::OnLobbyCreated);
}

/// <summary>
/// Calls when lobby was created
/// </summary>
void TDMP::Client::OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure)
{
	// record which lobby we're in
	if (pCallback->m_eResult == k_EResultOK)
	{
		// success
		new Lobby(pCallback->m_ulSteamIDLobby);

		SteamFriends()->SetRichPresence("status", "Chilling in the main menu");
		
		Debug::print("Created lobby! Waiting for game start...", Env::Client);

		//TDMP::lobby->SendChatMessage("Hi everyone!! This is working chat! Сука блять!");
	}
	else
	{
		// failed, show error
		Debug::error("Failed to create lobby (lost connection to Steam back-end servers.");
	}
}

void TDMP::Client::SendData(const void* pData, uint32 nSizeOfData, int nSendFlags)
{
	if (connectionState == k_EClientNotConnected)
	{
		Debug::print("Trying to send data with bad connection state", Env::Client);
		return;
	}

	EResult res = SteamNetworkingSockets()->SendMessageToConnection(serverHandle, pData, nSizeOfData, nSendFlags, nullptr);

	switch (res)
	{
	case k_EResultOK:
	case k_EResultIgnored:
		break;

	case k_EResultInvalidParam:
		Debug::error("Failed sending data to server: Invalid connection handle, or the individual message is too big\n");
		return;
	case k_EResultInvalidState:
		Debug::error("Failed sending data to server: Connection is in an invalid state\n");
		return;
	case k_EResultNoConnection:
		Debug::error("Failed sending data to server: Connection has ended\n");
		Disconnect();
		return;
	case k_EResultLimitExceeded:
		Debug::error("Failed sending data to server: There was already too much data queued to be sent\n");
		return;
	default:
		{
		char msg[256];
		sprintf(msg, "SendMessageToConnection returned %d\n", res);
		Debug::error(msg);
		return;
		}
	}
}

std::vector<LuaCallbackQueue> clientCallbackQueue;
std::mutex clientCallbackMutex;

std::vector<MsgSledgeHit> sledgeQueue;
std::mutex sledgeQueueMutex;

void TDMP::Client::LuaTick()
{
	if (glb::game->State != gameState::ingame)
		return;

	ReceiveNetData();
	
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (players[i].Active)
		{
			players[i].LuaTick();
		}
	}

	int callbacks = clientCallbackQueue.size();
	if (callbacks > 0)
	{
		for (size_t i = 0; i < callbacks; i++)
		{
			LUA::CallEvent(clientCallbackQueue[i].callback.c_str(), clientCallbackQueue[i].json.c_str());
		}

		clientCallbackQueue.clear();
	}

	int bodies = bodyQueue.size();
	if (bodies > 0)
	{
		for (size_t i = 0; i < bodies; i++)
		{
			MsgBody data = bodyQueue[i];
			TDBody* body = TDMP::levelBodies[levelBodiesById[bodyQueue[i].id]];
			if (body == glb::player->grabbedBody)
				continue;

			body->isAwake = true;
			body->countDown = 0x0F;

			body->Position = bodyQueue[i].pos;
			body->Rotation = bodyQueue[i].rot;
			body->Velocity = bodyQueue[i].vel;
			body->RotationVelocity = bodyQueue[i].rotVel;
		}

		bodyQueue.clear();
	}

	int holes = sledgeQueue.size();
	if (holes > 0)
	{
		for (size_t i = 0; i < holes; i++)
		{
			td::Vec3 pos{};
			pos.x = sledgeQueue[i].GetPos().x;
			pos.y = sledgeQueue[i].GetPos().y;
			pos.z = sledgeQueue[i].GetPos().z;

			glb::oWrappedDamage(glb::scene, &pos, sledgeQueue[i].GetDamageA(), sledgeQueue[i].GetDamageB(), 0, 0);
		}

		sledgeQueue.clear();
	}

	if (serverHandle == k_HSteamNetConnection_Invalid)
		return;

	ReceiveNetData();

	// Player's transform sync
	MsgPlayerData msg;

	glm::quat rot(glm::vec3(0, (-glb::player->camYaw), -1.57f));
	td::Vec4 finalRot;

	finalRot.x = rot.x;
	finalRot.y = rot.y;
	finalRot.z = rot.z;
	finalRot.w = rot.w;

	msg.SetPlayer(glb::player->position, finalRot, glb::player->cameraPosition, glb::player->cameraQuat);

	client->SendData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable);

	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (players[i].Active)
		{
			players[i].Frame();
		}
	}
}

void TDMP::Client::Tick()
{
	SteamNetworkingSockets()->RunCallbacks();

	if (serverHandle == k_HSteamNetConnection_Invalid)
		return;

	if (glb::game->State == gameState::ingame)
		return;

	// We need to receieve amy data evem of we're in main menu
	ReceiveNetData();
}

void TDMP::Client::Connect(CSteamID serverID)
{
	if (connectionState != k_EClientNotConnected)
	{
		Debug::print("[WARNING] Trying to connect to the server but we're already connecting to another one!", Env::Client);
		return;
	}
	
	SteamNetworkingIdentity identity;
	identity.SetSteamID(serverID);
	serverHandle = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);

	connectionState = k_EClientConnecting;

	Debug::print("Connecting to the server", Env::Client);
}

void TDMP::Client::Disconnect()
{
	if (serverHandle == k_HSteamNetConnection_Invalid)// || TDMP::IsServer())
	{
		connectionState = k_EClientNotConnected;
		serverHandle = k_HSteamNetConnection_Invalid;

		return;
	}

	Debug::print("Leaving the server", Env::Client);

	SteamNetworkingSockets()->CloseConnection(serverHandle, 0, nullptr, false);
	connectionState = k_EClientNotConnected;
	serverHandle = k_HSteamNetConnection_Invalid;
}

void TDMP::Client::HandlePlayerData(MsgPlayerData* pData, HSteamNetConnection* conn)
{
	if (!LevelLoaded)
		return;

	int found = -1;
	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (players[i].Active && players[i].SteamId == pData->GetSteamID())
		{
			players[i].Position = pData->GetPosition();
			players[i].Rotation = pData->GetRotation();

			players[i].CamPosition = pData->GetCamPosition();
			players[i].CamRotation = pData->GetCamRotation();

			players[i].hp = pData->GetHealth();
			players[i].isCtrlPressed = pData->GetCtrl();
			players[i].heldItem = std::string(pData->GetHeldItem());
			//if (players[i].heldItem != std::string(pData->GetHeldItem()))
			//	playerQueue.push_back(playerInGameLoopSync{ std::string(pData->GetHeldItem()), (int)i });

			if (pData->ToolExists())
			{
				players[i].ToolPosition = pData->GetToolPosition();
				players[i].ToolRotation = pData->GetToolRotation();
			}

			// welocme to the hell
			MsgVehicle v = pData->GetVehicle();
			if (v.id != 0) // Player is driving a vehicle
			{
				for (size_t j = 0; i < glb::scene->m_Vehicles->size(); j++)
				{
					TDVehicle* veh = glb::scene->m_Vehicles->data()[j];

					if (veh == 0 || veh->Id != v.id)
						continue;

					players[i].currentVehicle = veh;
					players[i].vehicleInput = v;

					break;
				}
			}
			else if (players[i].currentVehicle != 0) // player isn't driving a vehicle but he was driving one of them
			{
				players[i].currentVehicle = nullptr;
			}

			break;
		}
		else if (!players[i].Active) // So point is if we haven't found player at all, then we won't go through player array again, to find empty slot to create new player class
		{
			found = i;

			break;
		}
	}

	if (found != -1)
	{
		players[found].id = found;
		players[found].SteamId = pData->GetSteamID();
		players[found].steamIdStr = std::to_string(players[found].SteamId.ConvertToUint64());
		players[found].nick = SteamFriends()->GetFriendPersonaName(pData->GetSteamID());

		playersBySteamId[players[found].steamIdStr.c_str()] = found;

		players[found].Position = pData->GetPosition();
		players[found].Rotation = pData->GetRotation();

		players[found].CamPosition = pData->GetCamPosition();
		players[found].CamRotation = pData->GetCamRotation();

		players[found].hp = pData->GetHealth();
		players[found].isCtrlPressed = pData->GetCtrl();
		players[found].heldItem = std::string(pData->GetHeldItem());

		players[found].Active = true;
		players[found].bodyExists = false;
		players[found].hideBody = false;

		if (pData->ToolExists())
		{
			players[found].ToolPosition = pData->GetToolPosition();
			players[found].ToolRotation = pData->GetToolRotation();
		}

		LUA::RunLuaHooks("PlayerConnected", players[found].steamIdStr.c_str());

		if (conn != nullptr)
			players[found].conn = (HSteamNetConnection)conn;
	}
}

void TDMP::Client::HandleData(EMessage eMsg, SteamNetworkingMessage_t* message)
{

	uint32 cubMsgSize = message->GetSize();

	switch (eMsg)
	{
	case k_EMsgServerBegin:
		break;
	case k_EMsgServerSendInfo:
	{
		if (cubMsgSize != sizeof(MsgServerConnectionResult))
		{
			Debug::error("Bad server info");
			break;
		}
		MsgServerConnectionResult* pMsg = (MsgServerConnectionResult*)message->GetData();

		if (pMsg->GetSuccess())
		{
			MsgClientBeginAuthentication msg;
			char rgchToken[1024];
			uint32 unTokenLen = 0;
			hAuthTicket = SteamUser()->GetAuthSessionTicket(rgchToken, sizeof(rgchToken), &unTokenLen);

			msg.SetToken(rgchToken, unTokenLen);
			msg.SetSteamID(SteamUser()->GetSteamID().ConvertToUint64());

			SendData(&msg, sizeof(msg), k_nSteamNetworkingSend_Reliable);
		}

		break;
	}
	case k_EMsgServerUpdateWorld: // This is made for sending more than one body in one packet.
	{
		MsgUpdateBodies* pMsg = (MsgUpdateBodies*)message->GetData();
		if (pMsg == nullptr)
		{
			Debug::print("corrupted k_EMsgServerUpdateWorld received");
			break;
		}

		for (size_t i = 0; i < pMsg->GetBodiesCount(); i++)
		{
			if (levelBodiesById.count(pMsg->GetBodies()[i].id))
			{
				/*TDMP::bodyQueue.push_back(MsgBody{
					pMsg->GetBodies()[i].pos,
					pMsg->GetBodies()[i].rot,
					pMsg->GetBodies()[i].vel,
					pMsg->GetBodies()[i].rotVel,

					pMsg->GetBodies()[i].id
				});*/
				TDBody* body = TDMP::levelBodies[levelBodiesById[pMsg->GetBodies()[i].id]];

				body->isAwake = true;
				body->countDown = 0x0F;

				body->Position = pMsg->GetBodies()[i].pos;
				body->Rotation = pMsg->GetBodies()[i].rot;
				body->Velocity = pMsg->GetBodies()[i].vel;
				body->RotationVelocity = pMsg->GetBodies()[i].rotVel;
			}
		}

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

		sledgeQueue.push_back(*pMsg);

		break;
	}
	case k_EMsgServerUpdateBody: // Sending one body only
	{
		MsgUpdateBody* pMsg = (MsgUpdateBody*)message->GetData();
		if (pMsg == nullptr)
		{
			Debug::print("corrupted k_EMsgServerUpdateBody received");
			break;
		}

		if (levelBodiesById.count(pMsg->GetBody().id) && levelBodiesById[pMsg->GetBody().id] != 0)
		{
			TDMP::bodyQueue.push_back(MsgBody{
				pMsg->GetBody().pos,
				pMsg->GetBody().rot,
				pMsg->GetBody().vel,
				pMsg->GetBody().rotVel,

				pMsg->GetBody().id
			});
		}

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

		clientCallbackQueue.push_back(LuaCallbackQueue{
			std::string(pMsg->GetCallback()),
			std::string(pMsg->GetJson())
		});
		
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

		HandlePlayerData(pMsg);

		break;
	}
	case k_EMsgClientExiting:
	{
		MsgClientExiting* pMsg = (MsgClientExiting*)message->GetData();
		if (pMsg == nullptr)
		{
			Debug::print("corrupted k_EMsgClientExiting received");
			break;
		}
		if (TDMP::LevelLoaded)
			for (uint32 i = 0; i < MaxPlayers; ++i)
			{
				if (players[i].Active && players[i].SteamId == pMsg->GetSteamID())
				{
					Debug::print("Player " + players[i].steamIdStr + " disconnected", Env::Client);

					LUA::RunLuaHooks("PlayerDisconnected", players[i].steamIdStr.c_str());

					players[i].RemoveBodyIfExists();
					players[i].Active = false;

					break;
				}
			}

		break;
	}
	case k_EMsgServerExiting:
	{
		connectionState = k_EClientNotConnected;
		serverHandle = k_HSteamNetConnection_Invalid;
		Debug::print("Server was closed", Env::Client);
		
		LUA::RunLuaHooks("ServerConnectionClosed", "");

		break;
	}
	case k_EMsgServerAuthed:
	{
		connectionState = k_EClientConnectedAndAuthenticated;
		Debug::print("Welcome to the server", Env::Client);

		LUA::RunLuaHooks("ConnectedToServer", "");

		break;
	}
	default:
		Debug::print("Received invalid data", Env::Client);
	}
}

void TDMP::Client::ReceiveNetData()
{
	SteamNetworkingMessage_t* msgs[64];
	int numMessages = SteamNetworkingSockets()->ReceiveMessagesOnConnection(serverHandle, msgs, 64);

	for (int i = 0; i < numMessages; i++)
	{
		SteamNetworkingMessage_t* message = msgs[i];
		uint32 cubMsgSize = message->GetSize();

		// make sure we're connected
		if (connectionState == k_EClientNotConnected)
		{
			message->Release();
			continue;
		}

		if (cubMsgSize < sizeof(DWORD))
		{
			// got garbage
			Debug::print("Received garbage data", Env::Client);
			message->Release();
			continue;
		}

		HandleData((EMessage)LittleDWord(*(DWORD*)message->GetData()), message);

		message->Release();
		message = nullptr;
	}
}
