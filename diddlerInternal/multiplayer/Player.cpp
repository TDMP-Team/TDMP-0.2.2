#include "Player.h"
#include "../glm/glm.hpp"
#include "../drawCube.h"

TDMP::Player TDMP::players[TDMP::MaxPlayers];

td::Vec3 vec3Zero = { 0, 0, 0 };

void TDMP::Player::SetPosition(td::Vec3 pos)
{
	Position = pos;

	if (!IsMe())
	{
		CreateBodyIfNotExists();

		body.body->countDown = 0x0F;
		body.body->isAwake = true;

		body.body->Position = pos;

		body.body->Velocity = vec3Zero;
		body.body->RotationVelocity = vec3Zero;
	}
}

void TDMP::Player::SetRotation(td::Vec4 rot)
{
	Rotation = rot;
	
	if (!IsMe())
	{
		CreateBodyIfNotExists();

		body.body->countDown = 0x0F;
		body.body->isAwake = true;

		body.body->Rotation = rot;

		body.body->Velocity = vec3Zero;
		body.body->RotationVelocity = vec3Zero;
	}
}

void TDMP::Player::CreateBodyIfNotExists()
{
	if (body.body != 0x00 || glb::game->State != gameState::ingame)
		return;

	Debug::print("Creating player's body", Env::Client);

	spawner::freeObjectSpawnParams params = {};
	params.attributes.push_back({ td::small_string("unbreakable"),td::small_string("") });
	params.useUserRotation = false;
	//params.nocull = true; // Nocull would make it so we can see players at any distance, but it also breaks rendering, so let's comment it out for now.

	// TODO: Make player's body not collidable

	body = spawner::placeFreeObject("KM_Vox\\Default\\ptest\\object.vox", params);

	// This makes it easier to find player bodies through Lua. So you can just do "FindBodies" and get nickname, steamid, and even position of player using this. Isn't it tricky? ;)
	//glb::setObjectAttribute((TDShape*)body.body, "Player", "true");
	//glb::setObjectAttribute((TDShape*)body.body, "Nickname", GetPlayerNick(SteamId.ConvertToUint64()).c_str());
	//glb::setObjectAttribute((TDShape*)body.body, "SteamId", std::to_string(SteamId.ConvertToUint64()).c_str());
}

void TDMP::Player::RemoveBodyIfExists()
{
	if (body.body == 0x00 || glb::game->State != gameState::ingame)
		return;

	if (currentVehicle != 0)
	{
		for (size_t i = 0; i < glb::scene->m_Vehicles->size(); i++)
		{
			TDVehicle* veh = glb::scene->m_Vehicles->data()[i];

			//if (veh == 0 || veh->Id != currentVehicle->Id)
			//	continue;

			glb::setObjectAttribute((TDShape*)veh, "mp_steer", "");
			glb::setObjectAttribute((TDShape*)veh, "mp_throttle", "");
			glb::setObjectAttribute((TDShape*)veh, "mp_handbrake", "");

			glb::setObjectAttribute((TDShape*)veh, "mp_driven", "");

			break;
		}
	}

	body.body->Destroy(body.body, true);
}

bool TDMP::Player::IsMe()
{
	return SteamId == SteamUser()->GetSteamID();
}

void TDMP::Player::Frame()
{
	drawCube(Position, 1, td::redColor);

	// Explain: "Set" functions applies last received transform data to the player and his body object.
	// So it won't fall down if there is a latency or something else, and also won't jitter because of the physics
	SetPosition(Position);
	SetRotation(Rotation);
}

void TDMP::Player::LuaTick()
{
	if (!IsMe() && currentVehicle != 0)
	{
		currentVehicle->m_RemoteDrive = true;
		currentVehicle->m_RemoteThrottle = vehicleInput.throttle;
		currentVehicle->m_RemoteSteering = vehicleInput.steer;
		currentVehicle->m_RemoteHandbrake = vehicleInput.handbrake;
	}
}