#pragma once
//#include "TDObjects.h"
#include "Global.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>

struct snapPoint {
	glm::vec3 position;
	glm::vec3 direction;
};

struct snapPointPackage {
	std::vector<snapPoint> snapPoints;
	glm::quat parentRotation;
};

namespace snapPoints {
	bool pointNearPoint(glm::vec3 pointA, glm::vec3 pointB, float maxDist);
	bool getClosestSnapPoint(glm::vec3 sourcePoint, snapPointPackage snapPackage, float maxDist, snapPoint* out);
	void drawSnapPoints(TDShape* shape);
	snapPointPackage getSnapPoints(TDShape* shape);
}