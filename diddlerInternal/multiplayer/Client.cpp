#include "Main.h"
#include "Client.h"
#include "Lobby.h"
#include "Player.h"
#include <thread>

#include "../drawCube.h"
#include "../objectSpawner.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/vec3.hpp"

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

	EResult res = SteamNetworkingSockets()->SendMessageToConnection(this->serverHandle, pData, nSizeOfData, nSendFlags, nullptr);

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

void TDMP::Client::Tick()
{
	SteamNetworkingSockets()->RunCallbacks();

	if (serverHandle == k_HSteamNetConnection_Invalid || glb::game->State != gameState::ingame)
		return;

	ReceiveNetData();

	// Player's transform sync

	MsgPlayerTransform msg;

	glm::quat rot(glm::vec3(0, -glb::player->camYaw + 270, -1.57f));
	td::Vec4 finalRot;

	finalRot.x = rot.x;
	finalRot.y = rot.y;
	finalRot.z = rot.z;
	finalRot.w = rot.w;

	msg.SetPlayer(SteamUser()->GetSteamID(), glb::player->position, finalRot);

	client->SendData(&msg, sizeof(msg), k_nSteamNetworkingSend_Unreliable);
}

void TDMP::Client::Frame()
{

	if (serverHandle == k_HSteamNetConnection_Invalid || glb::game->State != gameState::ingame || !TDMP::LevelLoaded)
		return;

	for (uint32 i = 0; i < MaxPlayers; ++i)
	{
		if (players[i].Active)
		{
			drawCube(players[i].Position, 1, td::redColor);

			// Explain: "Set" functions applies last received transform data to the player and his body object. So it won't fall down if there is a latency or something else, and also won't jitter because of the physics
			players[i].SetPosition(players[i].Position);
			players[i].SetRotation(players[i].Rotation);
		}
	}
}

void TDMP::Client::Connect(uint64 serverID, uint32 serverIP)
{
	if (connectionState != k_EClientNotConnected)
	{
		Debug::print("[WARNING] Trying to connect to the server but we're already connecting to another one!", Env::Client);
		return;
	}
	
	SteamNetworkingIdentity identity;
	identity.SetSteamID(serverID);
	serverHandle = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
	/*SteamNetworkingIPAddr address;
	address.Clear();
	address.SetIPv4(serverIP, 27016);*/

	//serverHandle = SteamNetworkingSockets()->ConnectByIPAddress(address, 0, nullptr);

	connectionState = k_EClientConnecting;

	Debug::print("Game started, connecting to the server", Env::Client);
}

void TDMP::Client::Disconnect()
{
	if (serverHandle == k_HSteamNetConnection_Invalid)
		return;

	SteamNetworkingSockets()->CloseConnection(serverHandle, 0, nullptr, false);
	connectionState = k_EClientNotConnected;
	serverHandle = k_HSteamNetConnection_Invalid;
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
	case k_EMsgServerUpdateWorld: // This is made for sending more than one body in one packet. Isn't finished yet
	{
		/*MsgUpdateBodies* pMsg = (MsgUpdateBodies*)message->GetData();
		if (pMsg == nullptr)
		{
			Debug::print("corrupted k_EMsgServerUpdateWorld received");
			break;
		}

		pMsg->Deserialize();
		// temp. Not sure if I can do it better with current api
		// i REALLY hate how it looks like. So I hope there is a better way of doing this
		Debug::print(std::string("Received body data ") + std::to_string(pMsg->GetBodies().size()));
		for (size_t j = 0; j < pMsg->GetBodies().size(); j++)
		{
			MsgBody* netBody = pMsg->GetBodies().data()[j];
		
			for (size_t i = 0; i < TDMP::levelBodies.size(); i++)
			{
				TDBody* body = TDMP::levelBodies[i];

				if (body == 0)
					continue;

				if (netBody->id == body->Id)
				{
					Debug::print(std::string("Applying received data to body ") + std::to_string(body->Id));

					body->Position = netBody->pos;
					body->Rotation = netBody->rot;
					body->Velocity = netBody->vel;
					body->RotationVelocity = netBody->rotVel;

					break;
				}
			}
		}*/

		break;
	}
	case k_EMsgServerUpdateBody:
	{
		MsgUpdateBody* pMsg = (MsgUpdateBody*)message->GetData();
		if (pMsg == nullptr)
		{
			Debug::print("corrupted k_EMsgServerUpdateBody received");
			break;
		}

		for (size_t i = 0; i < TDMP::levelBodies.size(); i++)
		{
			TDBody* body = TDMP::levelBodies[i];

			if (body == 0)
				continue;

			if (pMsg->GetBody().id == body->Id)
			{
				body->isAwake = true;
				body->countDown = 0x0F;

				body->Position = pMsg->GetBody().pos;
				body->Rotation = pMsg->GetBody().rot;
				body->Velocity = pMsg->GetBody().vel;
				body->RotationVelocity = pMsg->GetBody().rotVel;

				break;
			}
		}

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

		int found = -1;
		for (uint32 i = 0; i < MaxPlayers; ++i)
		{
			if (players[i].Active && players[i].SteamId == pMsg->GetSteamID())
			{
				players[i].Position = pMsg->GetPosition();
				players[i].Rotation = pMsg->GetRotation();

				break;
			}
			else if (!players[i].Active && found == -1) // So point is if we haven't found player at all, then we won't go through player array again, to find empty slot to create new player class
			{
				found = i;

				break;
			}
		}

		if (found != -1)
		{
			players[found].SteamId = pMsg->GetSteamID();

			players[found].Position = pMsg->GetPosition();
			players[found].Rotation = pMsg->GetRotation();

			players[found].Active = true;
		}

		break;
	}
	case k_EMsgServerExiting:
	{
		connectionState = k_EClientNotConnected;
		serverHandle = k_HSteamNetConnection_Invalid;
		Debug::print("Server was closed", Env::Client);
		break;
	}
	case k_EMsgServerAuthed:
	{
		connectionState = k_EClientConnectedAndAuthenticated;
		Debug::print("Welcome to the server", Env::Client);
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