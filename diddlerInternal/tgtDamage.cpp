#include "Global.h"
#include "tgtDamage.h"
#include "Raycaster.h"
#include "drawCube.h"
#include "dotProjector.h"

tgt_damage::tgt_damage() {

}

DWORD tgt_damage::exec() {

    td::Color red{ 1.f, 0.f, 0.f, 1.f };
    td::Color green{ 0.f, 1.f, 0.f, 1.f };
    RaycastFilter rcf = {};

    if (fireConst) {
        fireOnceActive = true;
    }

    dotProjector::pixelResponse* pixelResponse = dotProjector::projectDotMatrix(res, flRadius, 1.f, false, (glm::quat*)&glb::player->cameraQuat, glb::player->cameraPosition, { 0, 0, -1 }, { 0, 1, 0 }, &rcf);
    
    for (int i = 0; i < pixelResponse->size; i++) {
        dotProjector::rayPixel rp = pixelResponse->data[i];

        if (rp.dist <= maxRange) {
            if (glb::player->isAttacking == true) {
                glb::oWrappedDamage(glb::scene, &rp.worldPos, holeSize, holeSize, 0, 0);
                drawCube(rp.worldPos, 0.02, red);
            }
            else {
                drawCube(rp.worldPos, 0.02, green);
            }
        }
    }
    


	return 0x01;
}