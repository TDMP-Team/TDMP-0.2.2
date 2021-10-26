
#include "tgtBase.h"
#include "tgtExplode.h"
#include <iostream>
#include "Raycaster.h"
#include "dotProjector.h"

tgt_explode::tgt_explode() {
	tool = TOOL_CLICKEXPLODE;
}

DWORD tgt_explode::exec() {

    if (glb::player->isAttacking) {
        raycaster::rayData rd = raycaster::castRayPlayer();
        td::Vec3 target = rd.worldPos;
        RaycastFilter rcf = {};
        uintptr_t sceneSpecial = *(uintptr_t*)((uintptr_t)glb::scene + 0x88);

        glm::quat rot = math::q_td2glm(glb::player->cameraQuat);
        glm::vec3 pos = math::v3_td2glm(glb::player->cameraPosition);
        glm::vec3 forv = { 0, 0, -1 };
        glm::vec3 upv = { 0, 1, 0 };

        glm::vec3 raycast_dir_bl = rot * forv;
        rd = raycaster::castRayManual(math::v3_glm2td(pos), math::v3_glm2td(raycast_dir_bl), &rcf);
        glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
        glm::mat4x4 vmatrix = glm::lookAt(pos, glTarget, upv);
        glm::mat4x4 pmatrix = glm::perspective(50.f, 1.f, 1.f, 1000.f);
        glm::mat4 invProjMat = glm::inverse(pmatrix);
        glm::mat4 invViewMat = glm::inverse(vmatrix);


        for (int i = 0; i < bulletCount; i++) {
            float comX = math::randFloat(-explosionFov, explosionFov);
            float comY = math::randFloat(-explosionFov, explosionFov);

            glm::vec2 ray_nds = glm::vec2(comX, comY);
            glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
            glm::vec4 eyeCoords = invProjMat * ray_clip;
            eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

            glm::vec4 rayWorld = invViewMat * eyeCoords;
            td::Vec3 rayDirection = math::v3_glm2td(glm::normalize(glm::vec3(rayWorld)));
            rd = raycaster::castRayManual(math::v3_glm2td(pos), rayDirection, &rcf);
            
            glb::TDcreateExplosion((uintptr_t)glb::scene, &rd.worldPos, explosionPower);
        }
    }

    return 0x01;

}