#include "global.h"
#include "Raycaster.h"
#include "objectSpawner.h"
#include "TDObjects.h"
#include "Mods.h"
#include "c4.h"

namespace c4 {

    static const char* bombSizeStr[] = {
        "KM_Vox\\Default\\c4_small\\object.vox",
        "KM_Vox\\Default\\c4_medium\\object.vox",
        "KM_Vox\\Default\\c4_large\\object.vox"
    };

    static const int bombSizePowers[] = {
        1,
        2,
        5
    };

    struct spawnedC4 {
        spawner::spawnedObject object;
        int explosionSize;
    };

    float firecrackerExplosionSize = 0.f;
    int selectedBombSizeInt = 2;
    std::vector<spawnedC4> explosiveObjects = {};
    bool isDetonating = false;

    void cleanup() {
        explosiveObjects.clear();
    }

    int frameCount = 0;
    bool runOnce = true;
    bool runOnceImp = true;
    spawner::KMSpawnedObject impNade;

    void runC4() {
        if (frameCount == 0) {
            runOnce = true;
        }
        else {
            frameCount--;
        }

        if (glb::game) {
            if (glb::game->State == gameState::menu) {
                if (explosiveObjects.size() > 0) {
                    std::cout << "Cleaning up c4 objects" << std::endl;
                    cleanup();
                }
            }
        }

        const char* c4name = "cfour";
        if (memcmp(glb::player->heldItemName, c4name, 5) == 0) {
            if (glb::player->isAttacking == true && !isDetonating) {
                if (runOnce) {
                    runOnce = false;
                    frameCount = 10;

                    raycaster::rayData rd = raycaster::castRayPlayer();

                    if (rd.distance <= 3.f) {
                        const char* currentPath = bombSizeStr[selectedBombSizeInt - 1];
                        spawner::spawnedObject object = spawner::placeChildObject(currentPath);
                        glb::setObjectAttribute(object.shapes[0], "unbreakable", "");
                        glb::setObjectAttribute(object.shapes[0], "nocull", "");
                        explosiveObjects.push_back({ object,  bombSizePowers[selectedBombSizeInt - 1] });
                    }
                    else {
                        const char* currentPath = bombSizeStr[selectedBombSizeInt - 1];

                        spawner::thrownObjectSpawnParams params = {};
                        params.attributes.push_back({ "unbreakable", "" });
                        params.power = 20.f;
                        params.nocull = true;
                        spawner::spawnedObject object = spawner::throwFreeObject(currentPath, params);
                        explosiveObjects.push_back({ object,  bombSizePowers[selectedBombSizeInt - 1] });
                    }

                    //osp.customRotation = rd.angle;


                }
            }
            else {
                frameCount = 0;
                runOnce = true;
            }
        }

        const char* c4detName = "cfDet";
        if (memcmp(glb::player->heldItemName, c4detName, 5) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnce) {
                    raycaster::rayData rd = raycaster::castRayPlayer();
                    td::Vec3 target = rd.worldPos;

                    runOnce = false;
                    isDetonating = true;
                    if (mods::c4_global_detonation) {
                        for (spawnedC4 curC4 : explosiveObjects) {
                            td::Vec3 objectMin = curC4.object.shapes[0]->posMin;
                            td::Vec3 objectMax = curC4.object.shapes[0]->posMax;
                            td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                            glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, curC4.explosionSize);
                            //curC4.object.shape->attributes = 0x00;
                            curC4.object.shapes[0]->Destroy(curC4.object.shapes[0], true);
                        }
                        explosiveObjects.clear();
                    }
                }
            }
            else {
                runOnce = true;
            }
        }

        const char* impname = "cracker";
        if (memcmp(glb::player->heldItemName, impname, 7) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnceImp) {
                    runOnceImp = false;

                    raycaster::rayData rd = raycaster::castRayPlayer();
                    td::Vec3 target = rd.worldPos;

                    td::Vec3 cameraDirection = glb::player->cameraEuler();
                    spawner::objectSpawnerParams osp;
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    osp.startVelocity = { cameraDirection.x * 20, cameraDirection.y * 20, cameraDirection.z * 20 };
                    osp.rotateFacePlayer = true;
                    osp.unbreakable = false;
                    osp.nocull = true;
                    osp.pushSpawnList = false;
                    
                    const char* currentPath = "KM_Vox\\Default\\Cracker\\object.vox";
                    
                    spawner::thrownObjectSpawnParams params = {};
                    params.power = 20.f;
                    params.nocull = true;
                    spawner::spawnedObject object = spawner::throwFreeObject(currentPath, params);

                    glb::setObjectAttribute(object.shapes[0], "unbreakable", "");
                    glb::setObjectAttribute(object.shapes[0], "bombstrength", std::to_string(firecrackerExplosionSize).c_str());
                    glb::setObjectAttribute(object.shapes[0], "bomb", "1.5");
                    //glb::setObjectAttribute(impNade.shape, "smoke", "");
                }
            }
            else {
                frameCount = 0;
                runOnceImp = true;
            }
        }

        const char* hhgName = "handBomb";
        if (memcmp(glb::player->heldItemName, hhgName, 8) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnceImp) {
                    runOnceImp = false;

                    raycaster::rayData rd = raycaster::castRayPlayer();
                    td::Vec3 target = rd.worldPos;

                    td::Vec3 cameraDirection = glb::player->cameraEuler();
                    spawner::objectSpawnerParams osp;
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    osp.startVelocity = { cameraDirection.x * 40, cameraDirection.y * 40, cameraDirection.z * 40 };
                    osp.rotateFacePlayer = true;
                    osp.unbreakable = false;
                    osp.nocull = true;
                    osp.pushSpawnList = false;

                    const char* currentPath = "KM_Vox\\Default\\handBomb\\object.vox";
                    impNade = spawner::spawnObjectProxy(currentPath, osp);

                    glb::setObjectAttribute(impNade.shape, "unbreakable", "");
                    glb::setObjectAttribute(impNade.shape, "bombstrength", "6");
                    glb::setObjectAttribute(impNade.shape, "bomb", "2");
                    //glb::setObjectAttribute(impNade.shape, "smoke", "");
                }
            }
            else {
                frameCount = 0;
                runOnceImp = true;
            }
        }

        const char* brickname = "brick";
        if (memcmp(glb::player->heldItemName, brickname, 5) == 0) {
            if (glb::player->isAttacking == true) {
                if (runOnceImp) {
                    runOnceImp = false;

                    td::Vec3 cameraDirection = glb::player->cameraEuler();
                    spawner::objectSpawnerParams osp;
                    osp.spawnType = spawner::objectSpawnType::thrown;
                    osp.startVelocity = { cameraDirection.x * 50, cameraDirection.y * 50, cameraDirection.z * 50 };
                    osp.rotateFacePlayer = true;
                    osp.unbreakable = false;
                    osp.nocull = true;
                    osp.pushSpawnList = false;

                    const char* currentPath = "KM_Vox\\Default\\brick\\object.vox";
                    spawner::KMSpawnedObject brick = spawner::spawnObjectProxy(currentPath, osp);

                    glb::setObjectAttribute(brick.shape, "explosive", "1");
                    glb::setObjectAttribute(brick.shape, "impactexplode", "");

                    *(byte*)(brick.shape + 8) = 1;
                    *(byte*)(brick.shape + 229) = 1;

                    brick.body->density = 50.f;
                    brick.shape->Density = 50.f;
                    brick.shape->Hardness = 50.f;
                }
            }
            else {
                frameCount = 0;
                runOnceImp = true;
            }
        }


        if (isDetonating) {
            if (!mods::c4_global_detonation) {
                if (explosiveObjects.size() > 0) {
                    spawnedC4 curC4 = explosiveObjects.front();
                    td::Vec3 objectMin = curC4.object.shapes[0]->posMin;
                    td::Vec3 objectMax = curC4.object.shapes[0]->posMax;
                    td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
                    glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, curC4.explosionSize);
                    //curC4.object.shape->attributes = 0x00;
                    curC4.object.shapes[0]->Destroy(curC4.object.shapes[0], true);
                    explosiveObjects.erase(explosiveObjects.begin());
                }
                else {
                    isDetonating = false;
                }
            }
            else {
                isDetonating = false;
            }
        }
    }
}

