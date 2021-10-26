#pragma once
#include "steam/headers/steam_api.h"
#include <windows.h>
#include <vector>
#include <string>
#include "Messages.h"

// Enum for various client connection states
enum EClientConnectionState
{
	k_EClientNotConnected,							// Initial state, not connected to a server
	k_EClientConnecting,
	k_EClientConnectedPendingAuthentication,		// We've established communication with the server, but it hasn't authed us yet
	k_EClientConnectedAndAuthenticated,				// Final phase, server has authed us, we are actually able to play on it
};
namespace TDMP
{
	class Client
	{
	public:
		Client();
		/// <summary>
		/// Sends data to a server. It may be authentication data, game data (like which buttons are pressed right now) and so on.
		/// </summary>
		void SendData(const void* pData, uint32 nSizeOfData, int nSendFlags);

		void CreateLobby();

		/// <summary>
		/// Tick method for client
		/// </summary>
		void Tick();

		void Frame();

		/// <summary>
		/// Recieves incoming network data
		/// </summary>
		void ReceiveNetData();

		/// <summary>
		/// Connects to the server using its SteamID
		/// </summary>
		void Connect(uint64 serverID, uint32 serverIP);

		/// <summary>
		/// Disconnects from the server (NOT lobby)
		/// </summary>
		void Disconnect();

		/// <summary>
		/// Adds packet to the packet list
		/// </summary>

		void HandleData(EMessage eMsg, SteamNetworkingMessage_t* message);

		HSteamNetConnection serverHandle = k_HSteamNetConnection_Invalid;
		EClientConnectionState connectionState;

		void OnLobbyCreated(LobbyCreated_t* pCallback, bool bIOFailure);
		CCallResult<Client, LobbyCreated_t> CallResultLobbyCreated;
	
	private:
		HAuthTicket hAuthTicket;
	};
	/// <summary>
	/// The client. Sends information about pressed buttons, looking direction and etc to the server. Receives all data about entities, itself position and etc
	/// </summary>
	extern Client* client;
	extern std::vector<std::string> packets;
}
