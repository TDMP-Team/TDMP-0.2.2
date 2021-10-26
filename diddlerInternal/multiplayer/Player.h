#pragma once
#include "steam/headers/steam_api.h"
#include "Main.h"
#include "../Global.h"
#include "../objectSpawner.h"

namespace TDMP
{
	class Player
	{
	public:
		bool Active;
		CSteamID SteamId;

		td::Vec3 Position;
		td::Vec4 Rotation;

		spawner::spawnedObject body;

		/// <summary>
		/// Applies position on player. Don't call it if you're just syncing positions between players, in other case jitter move may happen (or stuck at all?)
		/// </summary>
		void SetPosition(td::Vec3);
		
		/// <summary>
		/// Applies rotation on player. Don't call it if you're just syncing rotations between players, in other case jitter rotating may happen (or stuck at all?)
		/// </summary>
		void SetRotation(td::Vec4);

		void CreateBodyIfNotExists();

		bool IsMe();
	};

	extern Player players[MaxPlayers];
}