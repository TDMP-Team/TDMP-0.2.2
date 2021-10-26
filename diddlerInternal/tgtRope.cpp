#include "tgtBase.h"
#include "tgtRope.h"
#include "Raycaster.h"
#include "drawCube.h"

tgt_rope::tgt_rope() {
	tool = TOOL_ROPE;
}

DWORD tgt_rope::exec() {
    td::Color white{ 1.f, 1.f, 1.f, 1.f };


    raycaster::rayData rd = raycaster::castRayPlayer();
    drawCube(rd.worldPos, 0.02f, white);
    if (ropeToolFirstPos) {
        if (glb::player->isAttacking) {
            if (ropeAttackOnce) {
                ropeAttackOnce = false;
                ropeFirstPos = rd.worldPos;
                shape1 = rd.hitShape;
                worldPos1 = rd.worldPos;
                ropeToolFirstPos = false;
            }
        }
        else {
            ropeAttackOnce = true;
        }
    }
    else {
        drawCube(ropeFirstPos, 0.02f, white);
        glb::oFDL(glb::renderer, ropeFirstPos, rd.worldPos, ropeColor, ropeColor, false);
        if (glb::player->isAttacking) {
            if (ropeAttackOnce) {
                ropeAttackOnce = false;
                ropeSecondPos = rd.worldPos;
                TDJoint* newJoint = (TDJoint*)glb::oTMalloc(208);
                glb::tdConstructJoint(newJoint, nullptr);
                shape2 = rd.hitShape;
                worldPos2 = rd.worldPos;
                glb::tdInitWire(newJoint, &ropeFirstPos, &ropeSecondPos, newJoint->m_Size, ropeColor, ropeSlack, ropeStrength, ropeMaxStretch);
                ropeToolFirstPos = true;
            }
        }
        else {
            ropeAttackOnce = true;
        }
    }

	return 0x01;
}