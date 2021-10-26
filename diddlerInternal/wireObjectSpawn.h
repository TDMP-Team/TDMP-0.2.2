#pragma once
#include "wireObjectBase.h"
#include "wirePlacements.h"
#include <vector>

namespace wireObjects {
	enum class wireObjectName {
		OBJ_IntBus,
		OBJ_ConstantValue,
		OBJ_Explosive,
		OBJ_GreaterThan,
		OBJ_Lamp,
		OBJ_LessThan,
		OBJ_Raycast
	};

	struct wireObjectInfo {
		int id;
		wireObjectName name;
		const char* path;
	};

	const int wireObjectCount = 6;
	static wireObjectInfo validWireObjects[] = {
		{0, wireObjectName::OBJ_IntBus, "KM_Misc\\KM_Wire\\w_bus\\object.vox"},
		{1, wireObjectName::OBJ_ConstantValue, "KM_Misc\\KM_Wire\\w_constantValue\\object.vox"},
		{2, wireObjectName::OBJ_Explosive, "KM_Misc\\KM_Wire\\w_explosive\\object.vox"},
		{3, wireObjectName::OBJ_GreaterThan, "KM_Misc\\KM_Wire\\w_greaterThan\\object.vox"},
		{4, wireObjectName::OBJ_Lamp, "KM_Misc\\KM_Wire\\w_lamp\\object.vox"},
		{5, wireObjectName::OBJ_LessThan, "KM_Misc\\KM_Wire\\w_lessThan\\object.vox"},
		{6, wireObjectName::OBJ_Raycast, "KM_Misc\\KM_Wire\\w_raycast\\object.vox"},
	};

	//wireobjects are always spawned using the toolgun and always get placed on a target object
	wireObj* spawnWireObject(int objectID);
}