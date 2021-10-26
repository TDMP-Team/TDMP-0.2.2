#include "tgtBase.h"
#include "tgtSpawner.h"
#include <iostream>
#include "Raycaster.h"
#include "snapPoints.h"

tgt_spawner::tgt_spawner() {
	tool = TOOL_SPAWNER;

    for (spawner::spawnerCatagory catig : spawner::spawnerObjectsDatabase) {
        for (spawner::LoadedSpawnableObject object : catig.objects) {
            if (object.objectName == "brick_metal") {
                currentSpawngunObject = object;
                break;
            }
        }
    }
}

DWORD tgt_spawner::exec() {
    raycaster::rayData rd = raycaster::castRayPlayer();

    td::Vec3 target = rd.worldPos;
    TDShape* tShape = rd.hitShape;

    //if (rd.successful) {
    //    snapPoints::drawSnapPoints(tShape);
    //}

    if (!spawner::thrownMode) {


        snapPoint currentSnapPoint = {};
        //if (snapPoints::getClosestSnapPoint(math::v3_td2glm(target), snapPoints::getSnapPoints(tShape), 1.f, &currentSnapPoint)) {
        //    //rd.worldPos = math::v3_glm2td(out);

        //    glm::quat simRotation = math::expandRotation(math::q_td2glm(rd.hitShape->getParentBody()->Rotation), math::q_td2glm(rd.hitShape->rOffset));

        //    spawner::drawSpawngunObjectOutline(currentSpawngunObject.voxObject, rd, true, simRotation, currentSnapPoint);
        //}
        //else {
        //    spawner::drawSpawngunObjectOutline(currentSpawngunObject.voxObject, rd, false, {0, 0, 0, 0}, currentSnapPoint);
        //} 

        spawner::drawSpawngunObjectOutline(currentSpawngunObject.voxObject, rd, false, { 0, 0, 0, 0 }, currentSnapPoint);

    }

    if (glb::player->isAttacking) {
        if (spawnOnce || constSpawn) {
            spawnOnce = false;
            if (spawner::freeMode) {
                spawner::freeObjectSpawnParams params = {};
                params.attributes = currentSpawngunObject.attributes;
                params.useUserRotation = true;
                params.nocull = true;

                //glm::vec3 out = { 0, 0, 0 };
                //if (snapPoints::getClosestSnapPoint(math::v3_td2glm(target), snapPoints::getSnapPoints(tShape), 1.f, &out)) {
                //    params.useSnaps = true;
                //    params.snapPosition = out;
                //}

                spawner::placeFreeObject(currentSpawngunObject.voxPath, params);
            }
            else if (spawner::childMode) {
                spawner::childObjectSpawnParams params = {};
                params.attributes = currentSpawngunObject.attributes;
                params.useUserRotation = true;
                params.nocull = true;

                glm::vec3 out = { 0, 0, 0 };
                //if (snapPoints::getClosestSnapPoint(math::v3_td2glm(target), snapPoints::getSnapPoints(tShape), 1.f, &out)) {
                //    params.useSnaps = true;
                //    params.snapPosition = out;
                //}

                spawner::placeChildObject(currentSpawngunObject.voxPath, params);
            }
            else if (spawner::thrownMode) {
                spawner::thrownObjectSpawnParams params = {};
                params.attributes = currentSpawngunObject.attributes;
                params.power = throwPower;
                params.nocull = true;
                spawner::throwFreeObject(currentSpawngunObject.voxPath, params);
            }
        }
    }
    else {
        spawnOnce = true;
    }

	return 0x01;
}
