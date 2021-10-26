#pragma once
#include "steam/headers/steam_api.h"
#include "Main.h"
#define LittleDWord( val )	( val )

namespace TDMP
{
	class Server
	{
		struct ClientConnectionData_t
		{
			bool Active; // Is this slot in use? Or is it available for new connections?
			CSteamID SteamIDUser; // What is the steamid of the player?
			HSteamNetConnection handle; // The handle for the connection to the player
		};

		ClientConnectionData_t ConnectionNone;
	public:
		/// <summary>
		/// Creates an actual server. Should be called after level was loaded.
		/// </summary>
		Server();

		bool ConnectedToSteam;
		
		ClientConnectionData_t ClientData[MaxPlayers]; // Vector to keep track of client connections
		ClientConnectionData_t PendingClientData[MaxPlayers]; // Vector to keep track of client connections which are pending auth

		HSteamListenSocket socket;
		HSteamNetPollGroup pollGroup;

		/// <summary>
		/// Sends compressed string(json data) to a player
		/// </summary>
		/// <param name="json">Data to be compressed and sent</param>
		void SendDataToConnection(const void* pData, uint32 nSizeOfData, HSteamNetConnection hConn, int nSendFlags);

		/// <summary>
		/// Broadcasts(Sends to all) data to all players
		/// </summary>
		void BroadcastData(const void* pData, uint32 nSizeOfData, int nSendFlags);

		/// <summary>
		/// Gets the player with the specified SteamID. Returns nullptr if player wasn't found
		/// </summary>
		ClientConnectionData_t GetPlayerByID(uint64_t id);

		/// <summary>
		/// Handle a new client connecting
		/// </summary>
		void OnClientBeginAuthentication(CSteamID steamID, HSteamNetConnection connectionID, void* pToken, uint32 uTokenLen);

		/// <summary>
		/// Finally client authed and can play!
		/// </summary>
		/// <param name="index">Index at what client was authed</param>
		void OnAuthCompleted(int index);

		/// <summary>
		/// Tick method for server
		/// </summary>
		void Tick();

		void Frame();

		/// <summary>
		/// Receives incoming network data
		/// </summary>
		void ReceiveNetData();

		/// <summary>
		/// Closes all connections, shutdowns the server and marks lobby as joinable
		/// </summary>
		void Shutdown();
	private:
		// Called when we get new connections, or the state of a connection changes
		STEAM_GAMESERVER_CALLBACK(Server, OnConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t, CallbackConnectionStatus);

		STEAM_GAMESERVER_CALLBACK(Server, OnSteamServersConnected, SteamServersConnected_t, CallbackServersConnected);

		//void OnRequestEncryptedAppTicket(EncryptedAppTicketResponse_t* pEncryptedAppTicketResponse, bool bIOFailure);
		//CCallResult<Server, EncryptedAppTicketResponse_t> m_SteamCallResultEncryptedAppTicket;
	};
	/// <summary>
	/// The server. Handler of receiving and sending information between clients. Receives input data from clients
	/// </summary>
	extern Server* server;
}
