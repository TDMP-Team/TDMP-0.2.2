#include "snapPoints.h"
#include "drawCube.h"
#include "Raycaster.h"

namespace snapPoints {

	bool pointNearPoint(glm::vec3 pointA, glm::vec3 pointB, float maxDist) {
		float dist = sqrt(pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2) + pow(pointA.z - pointB.z, 2));
		if (dist < maxDist) {
			return true;
		}
		return false;
	}

	bool getClosestSnapPoint(glm::vec3 sourcePoint, snapPointPackage snapPackage, float maxDist, snapPoint* out) {
		bool hasSnapPoint = false;
		snapPoint closest = { };
		float closestDist = INT32_MAX;
		for (snapPoint sp : snapPackage.snapPoints) {
			float dist = sqrt(pow(sourcePoint.x - sp.position.x, 2) + pow(sourcePoint.y - sp.position.y, 2) + pow(sourcePoint.z - sp.position.z, 2));
			if (dist < closestDist && dist < maxDist) {
				closestDist = dist;
				closest = sp;
				hasSnapPoint = true;
			}
		}

		if (hasSnapPoint) {
			*out = closest;
			return true;
		}
		else {
			return false;
		}
	}

	void drawSnapPoints(TDShape* shape) {
		td::Color green{ 0.f, 1.f, 0.f, 1.f };

		snapPointPackage points = getSnapPoints(shape);
		for (snapPoint sp : points.snapPoints) {
			drawCube(math::v3_glm2td(sp.position), 0.05f, green);
		}
	}

	snapPointPackage getSnapPoints(TDShape* shape) {
		static snapPointPackage response = {};
		RaycastFilter rcf = { };

		if (!shape) { return response; }
		if (shape->Type != entityType::Shape) { return response; }

		float voxSizeX = (shape->pVox->sizeX / 10.f);
		float voxSizeY = (shape->pVox->sizeY / 10.f);
		float voxSizeZ = (shape->pVox->sizeZ / 10.f);

		glm::quat targetShapeWorldrotation = math::expandRotation(math::q_td2glm(shape->getParentBody()->Rotation), math::q_td2glm(shape->rOffset));
		glm::vec3 targetShapeWorldPosition = math::expandPosition(math::q_td2glm(shape->getParentBody()->Rotation), math::v3_td2glm(shape->getParentBody()->Position), math::v3_td2glm(shape->pOffset));

		glm::vec3 vx = targetShapeWorldrotation * glm::vec3(-1, 0, 0);
		glm::vec3 vy = targetShapeWorldrotation * glm::vec3(0, -1, 0);
		glm::vec3 vz = targetShapeWorldrotation * glm::vec3(0, 0, -1); //(UP)

		glm::vec3 translationCenter = ((vx * (voxSizeX * 0.5f)) + (vy * (voxSizeY * 0.5f)) + (vz * (voxSizeZ * 0.5f)));
		glm::vec3 targetShapeCenterPosition = targetShapeWorldPosition - translationCenter;

		//drawCube(math::v3_glm2td(targetShapeCenterPosition), 0.05f, red);

		glm::vec3 target0 = targetShapeCenterPosition - (vy * (voxSizeY * 0.55f));
		raycaster::rayData rdTarget0 = raycaster::castRayManual(math::v3_glm2td(target0), math::v3_glm2td(vy), &rcf);
		if (rdTarget0.hitShape == shape) {
			target0 = math::v3_td2glm(rdTarget0.worldPos);
		}
		snapPoint sp0 = { target0, -vy };

		glm::vec3 target1 = targetShapeCenterPosition + (vy * (voxSizeY * 0.55f));
		raycaster::rayData rdTarget1 = raycaster::castRayManual(math::v3_glm2td(target1), math::v3_glm2td(-vy), &rcf);
		if (rdTarget1.hitShape == shape) {
			target1 = math::v3_td2glm(rdTarget1.worldPos);
		}
		snapPoint sp1 = { target1, vy };

		glm::vec3 target2 = targetShapeCenterPosition - (vx * (voxSizeX * 0.55f));
		raycaster::rayData rdTarget2 = raycaster::castRayManual(math::v3_glm2td(target2), math::v3_glm2td(vx), &rcf);
		if (rdTarget2.hitShape == shape) {
			target2 = math::v3_td2glm(rdTarget2.worldPos);
		}
		snapPoint sp2 = { target2, -vx };

		glm::vec3 target3 = targetShapeCenterPosition + (vx * (voxSizeX * 0.55f));
		raycaster::rayData rdTarget3 = raycaster::castRayManual(math::v3_glm2td(target3), math::v3_glm2td(-vx), &rcf);
		if (rdTarget3.hitShape == shape) {
			target3 = math::v3_td2glm(rdTarget3.worldPos);
		}
		snapPoint sp3 = { target3, vx };

		glm::vec3 target4 = targetShapeCenterPosition - (vz * (voxSizeZ * 0.55f));
		raycaster::rayData rdTarget4 = raycaster::castRayManual(math::v3_glm2td(target4), math::v3_glm2td(vz), &rcf);
		if (rdTarget4.hitShape == shape) {
			target4 = math::v3_td2glm(rdTarget4.worldPos);
		}
		snapPoint sp4 = { target4, -vz };

		glm::vec3 target5 = targetShapeCenterPosition + (vz * (voxSizeZ * 0.55f));
		raycaster::rayData rdTarget5 = raycaster::castRayManual(math::v3_glm2td(target5), math::v3_glm2td(-vz), &rcf);
		if (rdTarget5.hitShape == shape) {
			target5 = math::v3_td2glm(rdTarget5.worldPos);
		}
		snapPoint sp5 = { target5, vz };

		response.snapPoints = { sp0, sp1, sp2, sp3, sp4, sp5 };
		response.parentRotation = targetShapeWorldrotation;
		return response;
	}

}