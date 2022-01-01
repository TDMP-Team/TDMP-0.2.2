#include "Player.h"
#include "../glm/glm.hpp"
#include "../drawCube.h"
#include "../Lua.h"

TDMP::Player TDMP::players[TDMP::MaxPlayers];
std::map<const char*, int> TDMP::playersBySteamId;

td::Vec3 vec3Zero = { 0, 0, 0 };

void TDMP::Player::SetPosition(td::Vec3 pos)
{
	Position = pos;

	if (!IsMe())
	{
		CreateBodyIfNotExists();

		if (!bodyExists)
			return;

		body.body->countDown = 0x0F;
		body.body->isAwake = true;
		body.body->density = 1;

		glm::quat rotation;
		rotation.x = body.body->Rotation.x;
		rotation.y = body.body->Rotation.y;
		rotation.z = body.body->Rotation.z;
		rotation.w = body.body->Rotation.w;

		//create direction vectors from body rotation
		glm::vec3 vx = rotation * glm::vec3(-1, 0, 0);
		glm::vec3 vz = rotation * glm::vec3(0, 0, -1); 

		float sVoxSizeX = body.shapes.front()->pVox->sizeX / 10.f;
		float sVoxSizeY = body.shapes.front()->pVox->sizeY / 10.f;

		//create a translation to move the rotation point from the corner to bottom center of the players body
		glm::vec3 transX = vz * (sVoxSizeY / 2.f);
		glm::vec3 transZ = vx * (sVoxSizeX / 2.f);

		body.body->Position = { pos.x - transZ.z + transX.z, pos.y, pos.z - transZ.z - transX.z };

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

		if (!bodyExists)
			return;

		body.body->countDown = 0x0F;
		body.body->isAwake = true;

		body.body->Rotation = rot;

		body.body->Velocity = vec3Zero;
		body.body->RotationVelocity = vec3Zero;
	}
}

void TDMP::Player::CreateBodyIfNotExists()
{
	if (hideBody || bodyExists || glb::game->State != gameState::ingame)
		return;

	Debug::print("Creating player's body", Env::Client);

	spawner::freeObjectSpawnParams params = {};
	params.attributes.push_back({ td::small_string("unbreakable"),td::small_string("") });
	params.useUserRotation = false;
	//params.nocull = true; // Nocull would make it so we can see players at any distance, but it also breaks rendering, so let's comment it out for now.

	body = spawner::placeFreeObject("KM_Vox\\Default\\ptest\\object.vox", params);
	bodyExists = true;

	if (body.body)
	{
		TDShape* shape = (TDShape*)body.body->pChild;
		int voxelCount = 0;

		while (shape != 0) {
			if (shape->Type == entityType::Shape && shape->pVox != 0)
				shape->collide = false;

			shape = (TDShape*)shape->pSibling;
		}
	}

	LUA::RunLuaHooks("PlayerBodyCreated", ("[" + steamIdStr + "," + std::to_string(id) + "]").c_str());
}

void TDMP::Player::RemoveBodyIfExists()
{
	if (!bodyExists || glb::game->State != gameState::ingame)
		return;

	body.body->Destroy(body.body, true);
	bodyExists = false;
}

bool TDMP::Player::IsMe()
{
	return SteamId == SteamUser()->GetSteamID();
}

void TDMP::Player::Frame()
{
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