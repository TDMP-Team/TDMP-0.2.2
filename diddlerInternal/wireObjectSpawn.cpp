#include "wireObjectBase.h"
#include "wireObjectSpawn.h"
#include "wireObjectKernel.h"
#include "wirePlacements.h"
#include "objectSpawner.h"

namespace wireObjects {
	wireObj* spawnWireObject(int objectID) {
		if (objectID <= wireObjectCount) {
			wireObjectInfo object = validWireObjects[objectID];
			wireObj* newObject = 0;

			switch (object.id) {
			case 0:
				newObject = new wirePlacementIntBus();
				break;
			}

			if (!newObject) { return 0; };

			spawner::childObjectSpawnParams params = {};
			params.useUserRotation = true;
			params.nocull = true;
			spawner::spawnedObject sp = spawner::placeChildObject(object.path, params);

			newObject->init(sp.shapes[0], 0);

			wireObjectStack.push_back(newObject);

			return newObject;
		}
		return 0;
	}
}