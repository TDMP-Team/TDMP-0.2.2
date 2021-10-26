#pragma once
#include "objectSpawner.h"

namespace lantern {
	extern float a1;
	extern float a2;
	extern float a3;
	extern float a4;
	extern float a5;
	extern float lightR;
	extern float lightG;
	extern float lightB;

	void spawnLantern();
	void updateLantern();
}


namespace smoker {
	extern td::particleInfo pInfo;
	extern td::Vec3 velocity;
	extern float lifetime;

    void spawnSmoker();
	void updateSmoker();
}