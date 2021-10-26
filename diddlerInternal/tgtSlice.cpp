#include "tgtSlice.h"
#include "Global.h"
#include "glm/glm.hpp"
#include "Raycaster.h"
#include "drawCube.h"

tgt_slice::tgt_slice() {

}

DWORD tgt_slice::exec() {

    const float pi = 3.1415926535f;
    float currentOffset = 0.f;
    td::Color red{ 1.f, 0.f, 0.f, 1.f };
    td::Color white{ 1.f, 1.f, 1.f, 1.f };

    for (int i = 0; i < slicer_resolution; i++) {
        RaycastFilter filter{ 0 };
        filter.m_Mask = -1;
        glm::quat camera_rotation_tl = {};

        if (slicerHorizontal) {
            camera_rotation_tl = glm::quat(glm::vec3(0, (currentOffset + glb::player->camYaw), 0));
        }
        else {
            camera_rotation_tl = glm::quat(glm::vec3((currentOffset + glb::player->camPitch), glb::player->camYaw, 0));
        }

        glm::vec3 raycast_dir_tl = camera_rotation_tl * glm::vec3(0, 0, -1);

        raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tl.x, raycast_dir_tl.y, raycast_dir_tl.z }, &filter);
        if (rd.distance <= slicer_maxDist) {
            if (glb::player->isAttacking) {
                glb::oWrappedDamage(glb::scene, &rd.worldPos, 0.2f, 0.2f, 0, 0);
            }
            drawCube(rd.worldPos, 0.05f, red);
        }
        else {
            drawCube(rd.worldPos, 0.05f, white);
        }

        td::Vec3 target = rd.worldPos;
        currentOffset += ((pi * 2) / slicer_resolution);
    }

	return 0x01;
}