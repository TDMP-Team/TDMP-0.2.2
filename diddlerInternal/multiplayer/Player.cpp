#include "Player.h"
#include "../glm/glm.hpp"

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
	if (body.body != 0x00)
		return;

	Debug::print("Creating player's body", Env::Client);

	spawner::freeObjectSpawnParams params = {};
	params.attributes.push_back({ td::small_string("unbreakable"),td::small_string("") });
	params.useUserRotation = false;
	//params.nocull = true; // Nocull would make it so we can see players at any distance, but it also breaks rendering, so let's comment it out for now.

	// TODO: Make player's body not collidable

	body = spawner::placeFreeObject("KM_Vox\\Default\\ptest\\object.vox", params);
}

bool TDMP::Player::IsMe()
{
	return SteamId == SteamUser()->GetSteamID();
}