#include "wireObjectBase.h"
#include "Raycaster.h"
#include <vector>

namespace wireObjects {
	std::vector<wireObj*> wireObjectStack = {};

	bool getWireObjectByShape(TDShape* shape, wireObj** out) {
		for (wireObj* wObj : wireObjectStack) {
			if (wObj->housing == shape) {
				*out = wObj;
				return true;
			}
		}
		return false;
	}

	void highlightTarget() {
		raycaster::rayData rd = raycaster::castRayPlayer();
		if (rd.successful) {
			wireNode* tNode = 0;
			wireObj* tObj = 0;
			if (getWireObjectByShape(rd.hitShape, &tObj)) {
				printf_s("Hit tObj: %p\n", tObj);
				if(tObj->getClosestNode(math::v3_td2glm(rd.worldPos), &tNode)) {
					tObj->drawNodes(tNode);
				}
				else {
					tObj->drawNodes(0);
				}
			}
		}
	}

	int updateExistingWireObjects() {
		if (glb::game->m_LoadingEffect != 0 && wireObjectStack.size() > 0) {
			for (wireObj* wObj : wireObjectStack) {
				wObj->dispose();
			}

			wireObjectStack.clear();
			return -1;
		}

		int c = 0;
		for (wireObj* wObj : wireObjectStack) {
			wObj->exec();
			c++; //haha
		}

		return c;
	}
}