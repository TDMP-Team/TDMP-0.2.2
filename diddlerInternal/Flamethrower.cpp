#include "Global.h"
#include "Raycaster.h"
#include "drawCube.h"

namespace flamethrower {
	float flRadius = 1.5f;

	void execFlamethrower() {
		const char* name = "blowtorch";

		if (memcmp(glb::player->heldItemName, name, 9) == 0) {
			if (glb::player->isAttacking == true) {
				
				raycaster::rayData rayDat = raycaster::castRayPlayer();
				td::Color red{ 1.f, 0.f, 0.f, 1.f };
				drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, flRadius, red);

				for (float ty = (rayDat.worldPos.y - flRadius); ty < (rayDat.worldPos.y + flRadius); ty += 0.1f) {
					for (float tx = (rayDat.worldPos.x - flRadius); tx < (rayDat.worldPos.x + flRadius); tx += 0.1f) {
						for (float tz = (rayDat.worldPos.z - flRadius); tz < (rayDat.worldPos.z + flRadius); tz += 0.1f) {
							if ((rand() % 100) == 0) {
								td::Vec3 firepos = { tx, ty, tz };
								glb::oSpawnFire(*(uintptr_t*)(glb::scene + 0xA8), &firepos);
							}
						}
					}
				}
			}
			else {
				raycaster::rayData rayDat = raycaster::castRayPlayer();
				td::Color green{ 0.f, 1.f, 0.f, 1.f };
				drawCube({ rayDat.worldPos.x, rayDat.worldPos.y, rayDat.worldPos.z }, flRadius, green);
			}
		}
	}
}
