#include "tgtBalloon.h"
#include "Global.h"
#include "balloons.h"

tgt_balloon::tgt_balloon() {
	tool = TOOL_BALLOON;
}

DWORD tgt_balloon::exec() {
	if (glb::player->isAttacking) {
		if (spawnOnce) {
			spawnOnce = false;
			balloons::spawnBalloon(balloonPower);
		}
	}
	else {
		spawnOnce = true;
	}
	return 0x01;
}