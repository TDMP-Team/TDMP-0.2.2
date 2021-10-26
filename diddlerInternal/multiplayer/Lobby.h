#pragma once
#include "steam/headers/steam_api.h"
#include <string>
#include <vector>

namespace TDMP
{
	class Lobby
	{
	public:
		/// <summary>
		/// Creates all callbacks of our lobby at lets us know that we're in lobby now
		/// </summary>
		/// <param name="lobbyID">SteamID of lobby</param>
		Lobby(uint64 lobbyID);
		~Lobby();

		/// <summary>
		/// Sends a message in lobby's chat. This message would be displayed in-game chatbox
		/// </summary>
		/// <param name="msg">The message to be sent</param>
		void SendChatMessage(const char* msg);

		/// <summary>
		/// Disconnects from current lobby
		/// </summary>
		void Leave();

		/// <summary>
		/// Updates rich presencse of the lobby for the groups: https://partner.steamgames.com/doc/features/enhancedrichpresence
		/// </summary>
		void UpdateData();

		/// <summary>
		/// Whether or not given player is member of the lobby
		/// </summary>
		/// <param name="steamID">Player to check</param>
		bool IsMember(CSteamID steamID);

		/// <summary>
		/// Whetjer or not given player is host of the lobby
		/// </summary>
		/// <param name="steamID">Player to check</param>
		bool IsHost(CSteamID steamID);

		void OnLobbyEntered(LobbyEnter_t* pCallback, bool bIOFailure);
		CCallResult<Lobby, LobbyEnter_t> CallResultLobbyEntered;

		uint64 id;
		std::vector<std::string> ChatHistory;
	private:
		STEAM_CALLBACK(Lobby, OnMemberState, LobbyChatUpdate_t, CallbackChatDataUpdate);
		STEAM_CALLBACK(Lobby, OnChatMessage, LobbyChatMsg_t, CallbackChatMessage);
		STEAM_CALLBACK(Lobby, OnLobbyJoinRequest, GameLobbyJoinRequested_t, CallbackLobbyJoin);
		STEAM_CALLBACK(Lobby, OnLobbyGameCreated, LobbyGameCreated_t, CallbackLobbyGameCreated);
	};

	/// <summary>
	/// Lobby in which we're in right now
	/// </summary>
	extern Lobby* lobby;
}
