#include "Global.h"
#include "tgtRemove.h"
#include "Raycaster.h"

tgt_remove::tgt_remove() {

}

DWORD tgt_remove::exec() {

    raycaster::rayData rd = raycaster::castRayPlayer();
    if (rd.hitShape != (TDShape*)0xCCCCCCCCCCCCCCCC) {
        glb::oHighlightShape(glb::renderer, rd.hitShape, 1.f);
        if (glb::player->isAttacking) {
            if (spawnOnce) {
                spawnOnce = false;
                std::cout << rd.hitShape << std::endl;
                rd.hitShape->Destroy(rd.hitShape, true);

            }
        }
        else {
            spawnOnce = true;
        }
    }

	return 0x01;
}