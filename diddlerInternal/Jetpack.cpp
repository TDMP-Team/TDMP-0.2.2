#include "Global.h"
#include "Raycaster.h"
#include "objectSpawner.h"

namespace jetpack {
	void executeJetpack() {

		if ((((GetAsyncKeyState(VK_SPACE) >> 15) & 0x0001) == 0x0001)) {
			if (glb::player->velocity.y < 15.f) {
				glb::player->velocity.y += 1.f;
			}
		}
	}
}
