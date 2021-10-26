#pragma once
#include "tgtBase.h"
#include "objectSpawner.h"

struct tgt_spawner : public tgt {
	spawner::LoadedSpawnableObject currentSpawngunObject;
	bool spawnOnce = true;
	bool constSpawn = false;
	float throwPower = 5.f;

	tgt_spawner();
	DWORD exec() override;
};