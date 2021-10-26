#pragma once
#include "Jetpack.h"
#include "godmode.h"
#include "objectSpawner.h"

namespace mods {
	extern char jetpackKey;
	extern char noclipKey;

	extern bool jetpack;
	extern bool flamethrower;
	extern bool godmode;
	extern bool c4_global_detonation;
	extern bool removeWalls;
	extern bool dontLockWhenOutOfFocus;
	extern bool isGameFocused;

	void execMods();
}