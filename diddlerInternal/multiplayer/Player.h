#pragma once
#include "steam/headers/steam_api.h"
#include "Main.h"
#include "Messages.h"
#include "../Global.h"
#include "../objectSpawner.h"
#include <map>

namespace TDMP
{
	class Player
	{
	public:
		bool Active;

		CSteamID SteamId;
		// cached steamId converted to string
		std::string steamIdStr;
		// cached nickname
		std::string nick;
		std::string heldItem;
		std::string lastItem;

		// id in the player table
		int id;

		HSteamNetConnection conn; // available only on server

		td::Vec3 ToolPosition;
		td::Vec4 ToolRotation;

		td::Vec3 Position;
		td::Vec4 Rotation;

		td::Vec3 CamPosition;
		td::Vec4 CamRotation;

		bool isDead; // needed for calling "PlayerDied" hook only once
		bool hideBody;
		bool bodyExists;
		spawner::spawnedObject body;

		spawner::spawnedObject toolBody;

		TDVehicle* currentVehicle;
		MsgVehicle vehicleInput;

		float hp;
		bool isCtrlPressed;

		/// <summary>
		/// Applies position on player. Don't call it if you're just syncing positions between players, in other case jitter move may happen (or stuck at all?)
		/// </summary>
		void SetPosition(td::Vec3);
		
		/// <summary>
		/// Applies rotation on player. Don't call it if you're just syncing rotations between players, in other case jitter rotating may happen (or stuck at all?)
		/// </summary>
		void SetRotation(td::Vec4);

		void CreateBodyIfNotExists();
		void RemoveBodyIfExists();

		/// <summary>
		/// Returns whether or not this player is a local player
		/// </summary>
		bool IsMe();
	
		void Frame();
		void LuaTick();
	};

	extern Player players[MaxPlayers];
	extern std::map<const char*, int> playersBySteamId;
}