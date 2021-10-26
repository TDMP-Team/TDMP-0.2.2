#include "tgtFlamethrower.h"
#include "Global.h"
#include "drawCube.h"
#include "dotProjector.h"

tgt_flamethrower::tgt_flamethrower() {

}

DWORD tgt_flamethrower::exec() {

    RaycastFilter rcf = {};
    td::Color green{ 0.f, 1.f, 0.f, 1.f };
    td::Color red{ 1.f, 0.f, 0.f, 1.f };

    if (glb::player->isAttacking == true) {
        uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);
        dotProjector::pixelResponse* pixelResponse = dotProjector::projectDotMatrix(16, flRadius, 1.f, false, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
        for (int i = 0; i < pixelResponse->size; i++) {
            td::Vec3 firepos = pixelResponse->data[i].worldPos;
            if (chance == 100) {
                glb::oSpawnFire(sceneSpecial, &firepos);
            }
            else if ((rand() % (100 - chance)) == 0) {
                glb::oSpawnFire(sceneSpecial, &firepos);
            }
            drawCube(firepos, 0.02, red);
        }
    }
    else {
        uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);
        dotProjector::pixelResponse* pixelResponse = dotProjector::projectDotMatrix(16, flRadius, 1.f, false, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
        for (int i = 0; i < pixelResponse->size; i++) {
            drawCube(pixelResponse->data[i].worldPos, 0.02, green);
        }
    }

	return 0x01;
}