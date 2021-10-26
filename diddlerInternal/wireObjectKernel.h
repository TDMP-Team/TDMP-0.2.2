#pragma once
#include "wireObjectBase.h"
#include <vector>

namespace wireObjects {
	extern std::vector<wireObj*> wireObjectStack;
	bool getWireObjectByShape(TDShape* shape, wireObj* out);
	void highlightTarget();
	int updateExistingWireObjects();
}