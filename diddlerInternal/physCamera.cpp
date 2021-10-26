#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "drawCube.h"
#include "toolgun.h"
#include "camera.h"
#include "threadedCamera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "camera.h"
#include "dotProjector.h"
#include "glm/gtx/quaternion.hpp"


namespace physCamera {

	spawner::spawnedObject camera = {};
	RaycastFilter rcf = {};
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	const float pi = 3.1415926535f;

    bool useExistingParentBody = false;

    threadCamera::KMCamera* objCamera = 0;

	void spawnCameraObject() {
        if (!objCamera) {
            objCamera = new threadCamera::KMCamera(math::q_td2glm(glb::player->cameraQuat), math::v3_td2glm(glb::player->cameraPosition), { 0, 0, -1 }, { 0, -1, 0 }, nToolgun::instance_camera->resolutionX, nToolgun::instance_camera->resolutionY);
        }   
        objCamera->cameraDestroyed = false;
		spawner::objectSpawnerParams osp = {};
		osp.nocull = true;
        camera = spawner::placeFreeObject("KM_Vox/Default/camera/object.vox");
        glb::setObjectAttribute(camera.shapes[0], "nocull", "");
        objCamera->parent = camera.shapes[0];

        objCamera->rcf.m_IgnoredShapes.push_back(camera.shapes[0]);
	}

	bool genFilter = true;
    float* pixels = nullptr;
    float minDist = 1000.f;
    float maxDist = 0.f;
    int deadCameraframes = 0;

    float randFloat(float min, float max) {
        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }

    int lastResolution = 64;
    bool flip = true;
    byte* frameBuffer;
    byte* pixelsColor = nullptr;
    std::chrono::steady_clock::time_point FRAMESTART;
    std::chrono::steady_clock::time_point FRAMEEND;
    std::chrono::high_resolution_clock execTimer;
    float fps = 0;

    void destroyCamera() {
        objCamera->destroy();
        camera = {};
    }

	void updateCamera() {
        if (!objCamera) { return; }

        if (objCamera->DCF < 60) {
            if (!threadCamera::runInSeperateThread && objCamera->cameraDestroyed) {
                objCamera->updateImage();
            }
            threadCamera::drawCameraWndw(objCamera);
            objCamera->cameraActive = true;
        }
        else {
            objCamera->cameraActive = false;
        }

        TDBody* cameraBody = camera.body;
        if (!cameraBody) { return; }
        TDShape* cameraShape = camera.shapes[0];
		if (!cameraShape) { return; }
         
        if (cameraShape->isBroken) {
            objCamera->cameraDestroyed = true;
            camera = {};
            return;
        }

        if (cameraShape->getParentBody() != camera.body) {
            //if the physcamera object is stuck to something and the something becomes disconnected from its origional body, then update the physcameras offsets to its new body.
            camera.body = cameraShape->getParentBody();
            if (!camera.body) { return;  }
            glb::oUpdateShapes((uintptr_t)cameraBody);
            glb::tdUpdateFunc(cameraBody, 0, 1);
        }

		td::Vec3 objectMin = cameraShape->posMin;
		td::Vec3 objectMax = cameraShape->posMax;
        glm::vec3 bodyPos = glm::vec3(camera.body->Position.x, camera.body->Position.y, camera.body->Position.z);
        glm::quat bodyQuat = *(glm::quat*)&cameraBody->Rotation;
        glm::vec3 cameraPosOffset = *(glm::vec3*)&cameraShape->pOffset;
        glm::quat cameraRotOffset = *(glm::quat*)&cameraShape->rOffset;   

        glm::vec3 vx = bodyQuat * glm::vec3(1, 0, 0);
        glm::vec3 vy = bodyQuat * glm::vec3(0, 1, 0);
        glm::vec3 vz = bodyQuat * glm::vec3(0, 0, 1); //(UP)
        glm::vec3 offsetPos = bodyPos + ((vz * cameraPosOffset.z) + (vy * cameraPosOffset.y) + (vx * cameraPosOffset.x)); //this is the position of the camera IN RELATION TO THE WORLD

        bodyQuat = bodyQuat * cameraRotOffset;
        vx = bodyQuat * glm::vec3(1, 0, 0);
        vy = bodyQuat * glm::vec3(0, 1, 0);
        vz = bodyQuat * glm::vec3(0, 0, 1); //(UP)

        //find the "sensor" pos, static translations are doodoo but whatever i dont care
        glm::vec3 centerpoint = offsetPos + ((vz * 0.15f) + (vy * 0.15f) + (vx * 0.15f));

        deadCameraframes = 60;
        glm::vec3 cameraUp = bodyQuat * glm::vec3(0, 0, 1);;

        objCamera->cameraActive = true;
        objCamera->updateCameraSpecs(bodyQuat, centerpoint, { -1, 0, 0 }, -cameraUp);
        if (!threadCamera::runInSeperateThread) {
            objCamera->updateImage();
        }
	}
}
