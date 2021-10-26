//#include "Raycaster.h"
//#include "objectSpawner.h"
//#include "maths.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include "drawCube.h"
//#include "toolgun.h"
//
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_opengl3.h"
//#include "imgui/imgui_impl_win32.h"
//#include "camera.h"
//
//namespace HaropDrone {
//    spawner::KMSpawnedObject camera = {};
//    RaycastFilter rcf = {};
//    const float pi = 3.1415926535f;
//    bool genFilter = true;
//    float minDist = 1000.f;
//    float maxDist = 0.f;
//    int deadCameraframes = 0;
//
//    byte* pixelsColor = nullptr;
//    float* pixels = nullptr;
//
//    void spawnHarop() {
//        spawner::objectSpawnerParams osp = {};
//        //osp.unbreakable = true;
//        osp.nocull = true;
//        camera = spawner::spawnObjectProxy("KM_Vox/Default/harop/object.vox", osp);
//        glb::setObjectAttribute(camera.shape, "explosive", "10");
//
//        rcf.m_IgnoredBodies.push_back(camera.body);
//        rcf.m_IgnoredShapes.push_back(camera.shape);
//    }
//
//    void destroyHarop() {
//        camera.body->Destroy(camera.body, true);
//        camera.shape->Destroy(camera.shape, true);
//        camera = {};
//    }
//
//    float randFloat(float min, float max) {
//        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
//    }
//
//    void updateHaropCamera() {
//        int res = toolgun::cameraResolution;
//        float fov = toolgun::cameraFov;
//        int pixelOffset = 0;
//
//        if (!camera.body || !camera.shape) {
//
//            return;
//        }
//
//        if (camera.shape->isBroken) {
//            if (deadCameraframes > 0) {
//                free(pixelsColor);
//                pixelsColor = new byte[(res * res) * 4];
//
//                for (int y = res; y > 0; y--) {
//                    for (int x = 0; x < res; x++) {
//                        int shade = rand() % 255;
//                        pixelsColor[pixelOffset] = (byte)(shade);
//                        pixelsColor[pixelOffset + 1] = (byte)(shade);
//                        pixelsColor[pixelOffset + 2] = (byte)(shade);
//                        pixelsColor[pixelOffset + 3] = (byte)(255);
//                        pixelOffset += 4;
//                    }
//                }
//                //camera::updateCameraFrameColor(pixelsColor, res, 0, 0, false);
//                camera::drawCameraWindow(0.f);
//                deadCameraframes--;
//            }
//            else {
//                destroyHarop();
//            }
//
//            return;
//        }
//
//        td::Vec3 objectMin = camera.shape->posMin;
//        td::Vec3 objectMax = camera.shape->posMax;
//        td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
//        deadCameraframes = 30;
//
//        minDist = 1000.f;
//        maxDist = 0.f;
//
//        if (camera::colourMode) {
//            free(pixelsColor);
//            pixelsColor = new byte[(res * res) * 4];
//        }
//        else {
//            free(pixels);
//            pixels = new float[(res * res)];
//        }
//
//        camera::drawCameraWindow(0.f);
//
//        td::Color red{ 1.f, 0.f, 0.f, 1.f };
//        td::Color green{ 0.f, 1.f, 0.f, 1.f };
//        td::Color blue{ 0.f, 0.f, 1.f, 1.f };
//        td::Color white{ 1.f, 1.f, 1.f, 1.f };
//
//        RaycastFilter filter{ 0 };
//        filter.m_Mask = -1;
//        filter.m_RejectTransparent = true;
//        filter.m_IgnoredBodies.push_back(camera.body);
//
//        glm::quat camera_rotation_bl = *(glm::quat*)(&camera.body->Rotation);
//        glm::vec3 raycast_dir_bl = camera_rotation_bl * glm::vec3(1, 0, 0);
//        glm::vec3 raycasterUp = camera_rotation_bl * glm::vec3(0, 0, 1);
//
//        raycaster::rayData rd = raycaster::castRayManual(centerpoint, { raycast_dir_bl.x, raycast_dir_bl.y, raycast_dir_bl.z }, &filter);
//
//        glm::vec3 glCameraPos = glm::vec3(centerpoint.x, centerpoint.y, centerpoint.z);
//        glm::vec3 glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
//        glm::mat4x4 vmatrix = glm::lookAt(glCameraPos, glTarget, raycasterUp);
//        glm::mat4x4 pmatrix = glm::perspective(50.f, 1.f, 1.f, 150.f);
//
//        glm::mat4 invProjMat = glm::inverse(pmatrix);
//        glm::mat4 invViewMat = glm::inverse(vmatrix);
//
//        for (int y = res; y > 0; y--) {
//            for (int x = 0; x < res; x++) {
//                float pxSize = (fov / res);
//                float comX = (fov / 2.f) - (x * pxSize) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));
//                float comY = (fov / 2.f) - (y * pxSize) + randFloat(-(pxSize / 3.f), (pxSize / 3.f));
//
//                glm::vec2 ray_nds = glm::vec2(comX, comY);
//                glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
//
//                glm::vec4 eyeCoords = invProjMat * ray_clip;
//                eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
//
//                glm::vec4 rayWorld = invViewMat * eyeCoords;
//                glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
//
//                rd = raycaster::castRayManual(centerpoint, { rayDirection.x, rayDirection.y, rayDirection.z }, &filter);
//
//                float thisDist = rd.distance;
//                if (thisDist < minDist) {
//                    minDist = thisDist;
//                }
//                if (thisDist > maxDist && thisDist < 1000.f) {
//                    maxDist = thisDist;
//                }
//
//                if (camera::colourMode) {
//                    if (thisDist >= 1000.f) {
//                        pixelsColor[pixelOffset] = (byte)(0);
//                        pixelsColor[pixelOffset + 1] = (byte)(77);
//                        pixelsColor[pixelOffset + 2] = (byte)(77);
//                        pixelsColor[pixelOffset + 3] = (byte)(255);
//                    }
//                    else {
//                        int iThisDist = ((int)thisDist) / 2;
//
//                        int iColourR = ((rd.palette.m_Color.m_R * 255) - iThisDist);
//                        if (iColourR < 0) { iColourR = 0; }
//                        int iColourG = ((rd.palette.m_Color.m_G * 255) - iThisDist);
//                        if (iColourG < 0) { iColourG = 0; }
//                        int iColourB = ((rd.palette.m_Color.m_B * 255) - iThisDist);
//                        if (iColourB < 0) { iColourB = 0; }
//
//                        pixelsColor[pixelOffset] = (byte)iColourR;
//                        pixelsColor[pixelOffset + 1] = (byte)iColourG;
//                        pixelsColor[pixelOffset + 2] = (byte)iColourB;
//                        pixelsColor[pixelOffset + 3] = (byte)(rd.palette.m_Color.m_A * 255);
//                    }
//                    pixelOffset += 4;
//                }
//                else {
//                    if (thisDist >= 1000.f) {
//                        pixels[pixelOffset] = -1.f;
//
//                    }
//                    else {
//                        pixels[pixelOffset] = thisDist;
//
//                    }
//                    pixelOffset++;
//                }
//            }
//        }
//
//        if (camera::colourMode) {
//            //camera::updateCameraFrameColor(pixelsColor, res, minDist, maxDist, toolgun::takeSnapshot);
//        }
//        else {
//            //camera::updateCameraFrame(pixels, res, minDist, maxDist, toolgun::takeSnapshot);
//        }
//    }
//
//}