#include "tgtAttribute.h"
#include "windows.h"
#include "Global.h"
#include "Raycaster.h"

struct fadeShapeOutline {
	TDShape* shape;
	int alpha = 255;
};

std::vector<fadeShapeOutline> clickedObjects = {};

tgt_attribute::tgt_attribute() {

}

DWORD tgt_attribute::exec() {

    clearClickedObjects = true;
    td::Color red{ 1.f, 0.f, 0.f, 1.f };

    if (clickedObjects.size() > 0) {
        for (fadeShapeOutline& fso : clickedObjects) {
            glb::oOutlineShape(glb::renderer, fso.shape, &red, (float)(fso.alpha / 255.f));
            if (fso.alpha > 0) {
                fso.alpha -= 4;
                clearClickedObjects = false;
            }
        }

        if (clearClickedObjects) {
            clickedObjects.clear();
            clearClickedObjects = false;
        }
    }

    if (glb::player->isAttacking) {
        if (spawnOnce) {
            spawnOnce = false;
            raycaster::rayData rd = raycaster::castRayPlayer();

            clickedObjects.push_back({ rd.hitShape, 255 });
            td::small_string attrib = setAttributeFirst;
            td::small_string attrib2 = setAttributeSecond;

            if (rd.hitShape) {
                glb::oSOA(rd.hitShape, &attrib, &attrib2);
            }


        }
    }
    else {
        spawnOnce = true;
    }

	return 0x01;
}