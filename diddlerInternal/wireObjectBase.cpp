#include "wireObjectBase.h"
#include "drawCube.h"

namespace wireObjects {
	nodeResponse connectNodes(wireNode* nodeA, wireNode* nodeB) {

		wireObjects::nodeResponse nodeAresponse = nodeA->connect(nodeB);
		wireObjects::nodeResponse nodeBresponse = nodeB->connect(nodeA);

		if (nodeAresponse == wireObjects::nodeResponse::NR_Ok && nodeBresponse == wireObjects::nodeResponse::NR_Ok) {
			return wireObjects::nodeResponse::NR_Ok;
		}
		else {
			return wireObjects::nodeResponse::NR_Generic;
		}
	}

	bool wireObj::getClosestNode(glm::vec3 worldPos, wireNode** out) {
		if (!this) { printf_s("THIS is null\n");  return false; }
		if (housing && !this->destroyed) {
			glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(housing->getParentBody()->Rotation), math::q_td2glm(housing->rOffset));
			glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(housing->getParentBody()->Rotation), math::v3_td2glm(housing->getParentBody()->Position), math::v3_td2glm(housing->pOffset));

			glm::vec3 localPosition = math::localisePosition(targetShapeWorldrotation, targetShapeWorldPosition, worldPos);

			printf_s("Local pos: %0.2f , %0.2f , %0.2f", localPosition.x, localPosition.y, localPosition.z);

			float maxDist = 0.1f;
			bool hasClosestNode = false;
			float closestDist = INT32_MAX;
			wireNode* closest = 0;

			for (wireNode* n : this->nodes) {
				glm::vec3 nodePos = n->getPosition();
				float dist = sqrt(pow(localPosition.x - nodePos.x, 2) + pow(localPosition.y - nodePos.y, 2) + pow(localPosition.z - nodePos.z, 2));
				if (dist < closestDist && dist < maxDist) {
					closestDist = dist;
					closest = n;
					hasClosestNode = true;
				}
			}

			if (hasClosestNode) {
				*out = closest;
				return true;
			}
			return false;
		}
		return false;
	}

	DWORD wireObj::drawNodes(wireNode* selected) {
		if (housing && !this->destroyed) {
			glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(housing->getParentBody()->Rotation), math::q_td2glm(housing->rOffset));
			glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(housing->getParentBody()->Rotation), math::v3_td2glm(housing->getParentBody()->Position), math::v3_td2glm(housing->pOffset));

			glm::vec3 vx = targetShapeWorldrotation * glm::vec3(1, 0, 0);
			glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, 1, 0);
			glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, 1); //(UP)

			for (wireNode* n : this->nodes) {
				glm::vec3 testNodeLocalPosition = n->getPosition();
				glm::vec3 assumedTestNodePosition = targetShapeWorldPosition + ((vz * testNodeLocalPosition.z) + (vy * testNodeLocalPosition.y) + (vx * testNodeLocalPosition.x));
				float s = 0.05f;
				if (selected && n == selected) {
					s = 0.1f;
				}
				drawCubeRotated(assumedTestNodePosition, targetShapeWorldrotation, s, nodeColourActive[(int)n->getColour()]);
			}
		}

		return 0x01;
	}
}