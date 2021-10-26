#include "steam/headers/steam_api.h"

#include "Client.h"
#include "Lobby.h"
#include "Main.h"

TDMP::Lobby* TDMP::lobby;

TDMP::Lobby::Lobby(uint64 lobbyID) :
	CallbackChatDataUpdate(this, &Lobby::OnMemberState),
	CallbackChatMessage(this, &Lobby::OnChatMessage),
	CallbackLobbyJoin(this, &Lobby::OnLobbyJoinRequest),
	CallbackLobbyGameCreated(this, &Lobby::OnLobbyGameCreated)
	//CallbackLobbyEnter(this, &TDMP::Lobby::OnLobbyEntered)
{
	id = lobbyID;

	TDMP::lobby = (Lobby*)this;
	ChatHistory.clear();

	SteamMatchmaking()->SetLobbyData(this->id, "game_starting", "0");

	UpdateData();
}

TDMP::Lobby::~Lobby()
{
	SteamMatchmaking()->LeaveLobby(this->id);
}

void TDMP::Lobby::OnLobbyGameCreated(LobbyGameCreated_t* pCallback)
{
	ChatHistory.clear();

	TDMP::client->Connect(pCallback->m_ulSteamIDGameServer, pCallback->m_unIP);
}

void TDMP::Lobby::Leave()
{
	SteamMatchmaking()->LeaveLobby(this->id);
	delete TDMP::lobby;
	TDMP::lobby = nullptr;
}

bool TDMP::Lobby::IsMember(CSteamID steamID)
{
	for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(this->id); i++)
	{
		if (SteamMatchmaking()->GetLobbyMemberByIndex(this->id, i) == steamID) {
			return true;
		}
	}

	return false;
}

bool TDMP::Lobby::IsHost(CSteamID steamID)
{
	return steamID == SteamMatchmaking()->GetLobbyOwner(this->id);
}

/// <summary>
/// Calls when we want to connect to the lobby using Steam's friend list. ("Join a lobby" or when we was invited to a lobby and pressed "Join")
/// It supposed to proceed lobby connection
/// </summary>
void TDMP::Lobby::OnLobbyJoinRequest(GameLobbyJoinRequested_t* pCallback)
{
	CallResultLobbyEntered.Set(SteamMatchmaking()->JoinLobby(pCallback->m_steamIDLobby), this, &Lobby::OnLobbyEntered);
}

/// <summary>
/// Calls when we were connected to a lobby
/// </summary>
void TDMP::Lobby::OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure)
{
	if (pCallback->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess)
	{
		if (TDMP::lobby != nullptr) // If we're right now in lobby and lobby is valid
		{
			TDMP::lobby->Leave();
			delete TDMP::lobby;
		}

		new Lobby(pCallback->m_ulSteamIDLobby);
		Debug::print("Welcome to " + GetPlayerNick(SteamMatchmaking()->GetLobbyOwner(pCallback->m_ulSteamIDLobby).ConvertToUint64()) + "'s lobby!");
	}
	else
	{
		Debug::error("Failed to enter lobby");
	}
}

void TDMP::Lobby::SendChatMessage(const char* msg)
{
	SteamMatchmaking()->SendLobbyChatMsg(this->id, msg, strlen(msg));
}

/// <summary>
/// Calls when someone sends message in the chat
/// </summary>
void TDMP::Lobby::OnChatMessage(LobbyChatMsg_t* pCallback)
{
	if (pCallback->m_ulSteamIDLobby != id)
		return; // If some bad guy trying to send message in lobby but he's not in this lobby - get a hell out of hea!

	if (pCallback->m_eChatEntryType != k_EChatEntryTypeChatMsg)
		return; // We somehow got not chat message?

	char data[1024];
	int cubData = sizeof(data);
	EChatEntryType entryType;
	CSteamID speaker;
	int size = SteamMatchmaking()->GetLobbyChatEntry(this->id, pCallback->m_iChatID, &speaker, data, cubData, &entryType);

	std::string msg = "";
	for (size_t i = 0; i < size; i++)
	{
		msg += data[i];
	}

	if (msg == "") // Somehow we've recieved empty string
		return;

	std::string message = GetPlayerNick(speaker.ConvertToUint64()) + ": " + msg;
	Debug::print(message, Env::None);

	ChatHistory.push_back(message);
}

/// <summary>
/// Calls when someone leaves/enters the lobby
/// </summary>
void TDMP::Lobby::OnMemberState(LobbyChatUpdate_t* pCallback)
{
	if (pCallback->m_ulSteamIDLobby != this->id)
		return; // If some bad guy trying to send message in lobby but he's not in this lobby - get a hell out of hea!

	// If somebody (Not we) did something
	if (pCallback->m_ulSteamIDUserChanged != SteamUser()->GetSteamID().ConvertToUint64())
	{
		uint32 nState = pCallback->m_rgfChatMemberStateChange;

		switch (pCallback->m_rgfChatMemberStateChange)
		{
			case (k_EChatMemberStateChangeEntered) : {
				Debug::print(GetPlayerNick(pCallback->m_ulSteamIDUserChanged) + " has connected to the lobby!");
				break;
			}
			default: { // left, disconnected, banned, kicked
				Debug::print(GetPlayerNick(pCallback->m_ulSteamIDUserChanged) + " has left the lobby!");
				break;
			}
		}
		UpdateData();
	}
}

void TDMP::Lobby::UpdateData()
{
	SteamFriends()->SetRichPresence("steam_player_group", std::to_string(this->id).c_str());
	SteamFriends()->SetRichPresence("steam_player_group_size", std::to_string(SteamMatchmaking()->GetNumLobbyMembers(this->id)).c_str());
}