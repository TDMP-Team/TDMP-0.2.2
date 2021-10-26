#include "tgtBase.h"
#include "tgtWeld.h"
#include "Raycaster.h"
#include "drawCube.h"

tgt_weld::tgt_weld() {
	tool = TOOL_WELD;
}

DWORD tgt_weld::exec() {

    td::Color white{ 1.f, 1.f, 1.f, 1.f };
    raycaster::rayData rd = raycaster::castRayPlayer();
    drawCube(rd.worldPos, 0.02f, white);

    if (weldStageOne) {
        if (glb::player->isAttacking) {
            if (weldAttackOnce) {
                weldAttackOnce = false;

                //if (rd.successful) {
                //    std::cout << "Shape count: " << std::to_string(rd.hitShape->getParentBody()->countContainedShapes()) << std::endl;
                //}

                if (rd.successful) {
                    weldTargetShape1 = rd.hitShape;
                    weldTargetBody1 = rd.hitShape->getParentBody();
                    weldStageOne = false;
                }

            }
        }
        else {
            weldAttackOnce = true;
        }
    }
    else {
        if (glb::player->isAttacking) {
            if (weldAttackOnce) {
                weldAttackOnce = false;

                if (rd.successful) {
                    weldTargetShape2 = rd.hitShape;
                    weldTargetBody2 = rd.hitShape->getParentBody();

                    if (weldTargetBody1 == weldTargetBody2 || weldTargetShape1 == weldTargetShape2) {
                        return 0x00;
                    }

                    int body1ShapeCount = weldTargetBody1->countContainedShapes();
                    int body2ShapeCount = weldTargetBody2->countContainedShapes();
                    TDShape* cTargetShape = 0;
                    TDBody* newBody = 0;
                    TDBody* oldBody = 0;

                    std::vector<TDShape*> scheduledShapes = {};
                    int transferCounter = 0;

                    //decide which parent is kept
                    if (body1ShapeCount > body2ShapeCount) {
                        //move shapes from body2 to body1
                        cTargetShape = (TDShape*)weldTargetBody2->pChild;
                        newBody = weldTargetBody1;
                        oldBody = weldTargetBody2;
                    }
                    else {
                        //move shapes from body1 to body2
                        cTargetShape = (TDShape*)weldTargetBody1->pChild;
                        newBody = weldTargetBody2;
                        oldBody = weldTargetBody1;
                    }

                    //iterate children on the doomed parent and schedule them for transfer
                    while (cTargetShape != 0) {
                        std::cout << cTargetShape << " : " << entityTypeStr[(int)cTargetShape->Type - 1] << std::endl;
                        if ((int)cTargetShape->Type == 2) {
                            scheduledShapes.push_back(cTargetShape);
                        }
                        else {
                            weldStageOne = true;
                            return 0x00;
                        }
                        cTargetShape = (TDShape*)cTargetShape->pSibling;
                    }

                    //transfer all children to the new parent, destroy the old one
                    for (TDShape* targetShape : scheduledShapes) {
                        transferCounter++;

                        glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(targetShape->getParentBody()->Rotation), math::v3_td2glm(targetShape->getParentBody()->Position), math::v3_td2glm(targetShape->pOffset));
                        glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(targetShape->getParentBody()->Rotation), math::q_td2glm(targetShape->rOffset));
                        glm::vec3 targetShapeNewPOffset = math::localisePosition(math::q_td2glm(newBody->Rotation), math::v3_td2glm(newBody->Position), targetShapeWorldPosition);
                        glm::quat targetShapeNewROffset = math::localiseRotation(math::q_td2glm(newBody->Rotation), targetShapeWorldrotation);

                        glb::tdUpdateShapeBody((uintptr_t)targetShape, (uintptr_t)newBody);
                        targetShape->pOffset = { targetShapeNewPOffset.x, targetShapeNewPOffset.y, targetShapeNewPOffset.z };
                        *(glm::quat*)&targetShape->rOffset = targetShapeNewROffset;
                    }

                    glb::oUpdateShapes((uintptr_t)newBody);
                    glb::tdUpdateFunc(newBody, 0, 1);
                    std::cout << "(WELD) Transfered " << std::to_string(transferCounter) << "shapes from " << std::hex << oldBody << " to " << newBody << std::endl;
                    oldBody->Destroy(oldBody, true);
                    scheduledShapes.clear();
                    postCompletionBody = newBody;

                    weldStageOne = true;
                }
            }
        }
        else {
            weldAttackOnce = true;
        }
    }

    if (postCompletionBody) {
        if (postCompletionHighlight > 0.f) {
            utils::highlightBody(postCompletionBody, postCompletionHighlight);
            postCompletionHighlight -= 0.1f;
        }
        else {
            postCompletionHighlight = 1.f;
            postCompletionBody = 0;
        }
    }

	return 0x01;
}