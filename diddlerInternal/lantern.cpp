#include "objectSpawner.h"
#include "lantern.h"

namespace lantern {
    spawner::KMSpawnedObject lantern;
    float a1 = 1.f;
    float a2 = 1.f;
    float a3 = 1.f;
    float a4 = 1.f;
    float a5 = 1.f;
    float lightR = 1.f;
    float lightG = 1.f;
    float lightB = 1.f;

    void spawnLantern() {
        if (lantern.body) {
            lantern.body->Destroy(lantern.body, true);
            lantern.shape->Destroy(lantern.shape, true);
        }

        spawner::objectSpawnerParams params = {};
        params.nocull = true;
        params.unbreakable = true;
        params.pushSpawnList = false;
        lantern = spawner::spawnObjectProxy("vox\\default\\lantern\\object.vox", params);
    }

    void updateLantern() {
        if (glb::game->State == gameState::menu) {
            lantern = {};
        }

        if (lantern.body) {
            td::Vec3 objectMin = lantern.shape->posMin;
            td::Vec3 objectMax = lantern.shape->posMax;
            td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
            td::Vec3 col = { lightR, lightG, lightB };
            //glb::oCreateLight(glb::renderer, &centerpoint, &col, a1, a2, a3, a4, 0.1f);
            glb::oCreateLight(glb::renderer, &centerpoint, &col, 0.1f, 0.1f, 1.5f, 1.5f);
        }
    }
}

namespace smoker {
    float lifetime = 1.f;
    spawner::KMSpawnedObject smoker;
    td::particleInfo pInfo;
    td::Vec3 velocity;

    void spawnSmoker() {
        if (smoker.body) {
            smoker.body->Destroy(smoker.body, true);
            smoker.shape->Destroy(smoker.shape, true);
        }
        pInfo = { 0.f, 0.f, 0.f, 0.7f, 0.7f, 0.7f, 1.f, 0.7f, 0.7f, 0.7f, 1.f, 0.f, 0.f, 0.f, 0.2f, 0.f, 0.f, 0.f, 0.f, 0.15f, 0.25f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
        spawner::objectSpawnerParams params = {};
        params.nocull = true;
        params.unbreakable = true;
        params.pushSpawnList = false;
        smoker = spawner::spawnObjectProxy("vox\\default\\smoker\\object.vox", params);
    }

    void updateSmoker() {
        if (glb::game->State == gameState::menu) {
            smoker = {};
        }

        if (smoker.body) {
            td::Vec3 objectMin = smoker.shape->posMin;
            td::Vec3 objectMax = smoker.shape->posMax;
            td::Vec3 centerpoint = { objectMax.x - ((objectMax.x - objectMin.x) / 2), objectMax.y - ((objectMax.y - objectMin.y) / 2), objectMax.z - ((objectMax.z - objectMin.z) / 2) };
            uintptr_t special = *(uintptr_t*)((uintptr_t)glb::scene + 0x68);
            glb::TDspawnParticle((DWORD*)special, &pInfo, centerpoint, velocity, lifetime);

        }
    }
}