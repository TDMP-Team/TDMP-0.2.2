
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "types.h"
#include "Raycaster.h"
#include "dotProjector.h"

namespace dotProjector {
    td::Color red{ 1.f, 0.f, 0.f, 1.f };
    td::Color green{ 0.f, 1.f, 0.f, 1.f };
    td::Color blue{ 0.f, 0.f, 1.f, 1.f };
    td::Color white{ 1.f, 1.f, 1.f, 1.f };

    float randFloat(float min, float max) {
        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }

    glm::vec3 getSingleScreenVector(float x, float y, glm::mat4x4& vMatrix, glm::mat4x4& pMatrix) {
        glm::vec2 ray_nds = glm::vec2(x, y);
        glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
        glm::mat4 invProjMat = glm::inverse(pMatrix);
        glm::vec4 eyeCoords = invProjMat * ray_clip;
        eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
        glm::mat4 invViewMat = glm::inverse(vMatrix);
        glm::vec4 rayWorld = invViewMat * eyeCoords;
        return glm::normalize(glm::vec3(rayWorld));
    }

    pixelResponse responseContainer = {};
    pixelResponse* projectDotMatrix(int resolution, float fov, float aspect, bool stochastic, glm::quat* camRotation, td::Vec3 camPosition, td::Vec3 forwardVector, td::Vec3 upVector, RaycastFilter* filter) {
        //construct a new viewMatrix and projectionMatrix
        //could use the matrices already created by the game, if i can find them

        glm::vec3 raycast_dir_bl = *camRotation * glm::vec3(forwardVector.x, forwardVector.y, forwardVector.z );
        raycaster::rayData rd = raycaster::castRayManual(camPosition, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, filter);
        glm::vec3 glCameraPos = glm::vec3(camPosition.x, camPosition.y, camPosition.z);
        glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
        glm::mat4x4 vmatrix = glm::lookAt(glCameraPos, glTarget, glm::vec3(upVector.x, upVector.y, upVector.z));
        glm::mat4x4 pmatrix = glm::perspective(50.f, aspect, 1.f, 1000.f);
        glm::mat4 invProjMat = glm::inverse(pmatrix);
        glm::mat4 invViewMat = glm::inverse(vmatrix);

        //construct container. If the data area is a valid pointer we need to free it for the next frame. 
        //should only reconstruct the data region if the size changes, otherwise we can just overwrite it
        free(responseContainer.data);
        responseContainer.size = (size_t)(resolution * resolution);
        responseContainer.data = new rayPixel[responseContainer.size];
        int pixelOffset = 0;
        float minDist = 1000.f;
        float maxDist = 0.f;

        //draw from left to right, bottom to top. This matches openGLs pixel format. 
        for (int y = 0; y < resolution; y++) {
            for (int x = resolution; x > 0; x--) {

                //slightly eh implementation of Stochastic Sampling. Thanks Josh!
                float pxSize = (fov / resolution);
                float comX = 0.f;
                float comY = 0.f;

                if (stochastic) {
                    comX = (fov / 2.f) - ((x * pxSize) - (pxSize / 2.f)) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));
                    comY = (fov / 2.f) - ((y * pxSize) + (pxSize / 2.f)) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));
                }
                else {
                    comX = (fov / 2.f) - ((x * pxSize) - (pxSize / 2.f));
                    comY = (fov / 2.f) - ((y * pxSize) + (pxSize / 2.f));
                }

                glm::vec2 ray_nds = glm::vec2(comX, comY);
                glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
                glm::vec4 eyeCoords = invProjMat * ray_clip;
                eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

                glm::vec4 rayWorld = invViewMat * eyeCoords;
                glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
                rd = raycaster::castRayManual(camPosition, { rayDirection.x, rayDirection.y, rayDirection.z }, filter);

                if (rd.distance < minDist) {
                    minDist = rd.distance;
                }
                if (rd.distance > maxDist && rd.distance < 1000.f) {
                    maxDist = rd.distance;
                }

                responseContainer.data[pixelOffset] = { 
                    { rayDirection.x, rayDirection.y, rayDirection.z }, 
                    rd.worldPos,
                    rd.distance, 
                    (int)(rd.palette.m_Color.m_R * 255), 
                    (int)(rd.palette.m_Color.m_G * 255), 
                    (int)(rd.palette.m_Color.m_B * 255) 
                };

                pixelOffset++;
            }
        }

        responseContainer.maxDist = maxDist;
        responseContainer.minDist = minDist;
        return &responseContainer;
    }
}