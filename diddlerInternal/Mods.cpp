#include "Jetpack.h"
#include "Flamethrower.h"
#include "Global.h"
#include "cutter.h"
#include "objectSpawner.h"
#include "noclip.h"
#include "c4.h"
#include "camera.h"
#include "toolgun.h"
#include "lantern.h"
#include "missileLauncher.h"
#include "physCamera.h"
#include "Harop.h"
#include "objectTranslationTest.h"
#include "balloons.h"
#include "wireObjectKernel.h"

namespace mods {
	char jetpackKey = VK_SPACE;
	char noclipKey = 0x56;

	bool jetpack = false;
	bool flamethrower = true;
	bool godmode = true;
	bool c4_global_detonation = false;
	bool removeWalls = false;
	bool dontLockWhenOutOfFocus = true;
	bool isGameFocused = false;

	void execMods() {
		spawner::processMostRecentObject();
		nToolgun::runToolgun();
		wireObjects::updateExistingWireObjects();
		wireObjects::highlightTarget();

		c4::runC4();
		noclip::update();
		balloons::updateAllBalloons();
		
		if (jetpack) {
			jetpack::executeJetpack();
		}

		if (removeWalls) {
			
			int count = 0;

			if (*(byte*)&(glb::scene->m_Boundaries) != 0x00) {
				for (td::Vec2* pt : glb::scene->m_Boundaries) {
					count++;
				}
				mem::Null((byte*)&(glb::scene->m_Boundaries), 8 * count);
			}
		}

		physCamera::updateCamera();
		//HaropDrone::updateHaropCamera();
		objectTesting::updateDebugObject();
		//camera::quatCameraOutline();

		//if (flamethrower) {
		//	flamethrower::execFlamethrower();
		//}
		//spawner::handleSpawnerWeapon();

		//camera::runCamera();
	}
}