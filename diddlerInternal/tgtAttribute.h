#pragma once
#include "tgtBase.h"
#include <vector>
//#include "TDObjects.h"

struct tgt_attribute : public tgt {
	char setAttributeFirst[128] = {};
	char setAttributeSecond[128] = {};
	//std::vector<fadeShapeOutline> clickedObjects = {};
	bool spawnOnce = true;
	bool clearClickedObjects = true;

	tgt_attribute();
	DWORD exec() override;
};