#include "tgtBase.h"
#include "tgtCamera.h"
#include <iostream>
#include "threadedCamera.h"

tgt_camera::tgt_camera() {
	tool = TOOL_CAMERA;
    playerCamera = new threadCamera::KMCamera(math::q_td2glm(glb::player->cameraQuat), math::v3_td2glm(glb::player->cameraPosition), { 0, 0, -1 }, { 0, -1, 0 }, resolutionX, resolutionY);
}

DWORD tgt_camera::exec() {

    if (!playerCamera) {
        playerCamera = new threadCamera::KMCamera(math::q_td2glm(glb::player->cameraQuat), math::v3_td2glm(glb::player->cameraPosition), { 0, 0, -1 }, { 0, -1, 0 }, resolutionX, resolutionY);
        return 0x02;
    }

    if ((glb::player->isAttacking && !glb::displayMenu)) {
        playerCamera->cameraActive = true;
        playerCamera->updateCameraSpecs(math::q_td2glm(glb::player->cameraQuat), math::v3_td2glm(glb::player->cameraPosition), { 0, 0, -1 }, { 0, -1, 0 });
        if (!threadCamera::runInSeperateThread) {
            playerCamera->updateImage();
        }
    }
    else {
        playerCamera->cameraActive = false;
    }

    threadCamera::drawCameraWndw(playerCamera);

    return 0x01;
}

void tgt_camera::setResolution(){
    if (playerCamera) {
        playerCamera->setResolution(resolutionX, resolutionY);
    }
}