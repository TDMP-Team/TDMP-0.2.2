#include "objectSpawner.h"
#include "drawCube.h"
#include "glm/gtx/quaternion.hpp"

namespace objectTesting {
	spawner::KMSpawnedObject dbgObject;
	td::Color white{ 1.f, 1.f, 1.f, 1.f };
	td::Color red{ 1.f, 0.f, 0.f, 1.f };
	td::Color green{ 0.f, 1.f, 0.f, 1.f };
	td::Color blue{ 0.f, 0.f, 1.f, 1.f };

	void spawnDebugObject() {
		spawner::objectSpawnerParams osp{};		
		dbgObject = spawner::spawnObjectProxy("vox\\Default\\knedcube\\object.vox", osp);
	}

	void destroyDebugObject() {
		dbgObject = {};
	}

	void drawCircleAroundPoint(td::Vec3 vPoint, td::Vec3 vUp) {
		glm::vec3 gvPoint = glm::vec3(vPoint.x, vPoint.y, vPoint.z);
		glm::vec3 gvUp = glm::vec3(vUp.x, vUp.y, vUp.z);
		glm::quat qUp = glm::quat(gvUp);

		glm::vec3 dir1 = qUp * glm::vec3(1, 0, 0);
		glm::vec3 pos1 = { gvPoint.x + dir1.x, gvPoint.y + dir1.y, gvPoint.z + dir1.z };
		glb::oFDL(glb::renderer, vPoint, { pos1.x, pos1.y, pos1.z }, red, red, false);

		glm::vec3 dir2 = qUp * glm::vec3(-1, 0, 0);
		glm::vec3 pos2 = { gvPoint.x + dir2.x, gvPoint.y + dir2.y, gvPoint.z + dir2.z };
		glb::oFDL(glb::renderer, vPoint, { pos2.x, pos2.y, pos2.z }, red, red, false);
	}
	void drawTargetCircle() {
		raycaster::rayData rd = raycaster::castRayPlayer();
		drawCircleAroundPoint(rd.worldPos, rd.angle);
	}
	void drawPointerThing() {
		raycaster::rayData rd = raycaster::castRayPlayer();

		glm::vec3 hitPos = {rd.worldPos.x, rd.worldPos.y, rd.worldPos.z};
		td::Vec3 target = { hitPos.x, hitPos.y, hitPos.z };

		if (rd.angle.x == 0.f) {
			rd.angle.x += 0.0001f;
		}

		if (rd.angle.y == 0.f) {
			rd.angle.y += 0.0001f;
		}

		if (rd.angle.z == 0.f) {
			rd.angle.z += 0.0001f;
		}

		float voxSizeX = 0.2f;
		float voxSizeY = 0.8f;
		float voxSizeZ = 0.2f;

		glm::quat facePlayer = glm::quat(glm::vec3(4.71238898025f, glb::player->camYaw, 0));
		glm::vec3 vxTmp = facePlayer * glm::vec3(-1, 0, 0);

		glm::vec3 hitDir = glm::vec3(rd.angle.x, rd.angle.y, rd.angle.z);

		std::cout << std::to_string(rd.angle.x) << " : " << std::to_string(rd.angle.y) << " : " << std::to_string(rd.angle.z) << std::endl;

		hitDir = glm::normalize(hitDir);

		glm::quat q = glm::conjugate(glm::quat(glm::lookAt(hitPos, hitPos + hitDir, vxTmp))); //this is kinda inverted, with "up" facing the player and "forward" facing away from the surface. "fixing" this makes it work less good so eh.

		glm::vec3 vx = q * glm::vec3(-1, 0, 0);
		glm::vec3 vy = q * glm::vec3(0, -1, 0);
		glm::vec3 vz = q * glm::vec3(0, 0, -1); //(UP)

		//draw x line
		td::Vec3 vx_p2 = { hitPos.x + vx.x, hitPos.y + vx.y, hitPos.z + vx.z };
		glb::oFDL(glb::renderer, rd.worldPos, vx_p2, red, red, false);

		//draw y line
		td::Vec3 vy_p2 = { hitPos.x + vy.x, hitPos.y + vy.y, hitPos.z + vy.z};
		glb::oFDL(glb::renderer, rd.worldPos, vy_p2, green, green, false);

		//draw z line
		td::Vec3 vz_p2 = { hitPos.x + vz.x, hitPos.y + vz.y, hitPos.z + vz.z };
		glb::oFDL(glb::renderer, rd.worldPos, vz_p2, blue, blue, false);

		glm::vec3 translationFBL = ((vz * 0.f) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBBR = ((vz * 0.f) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBBL = ((vz * 0.f) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationFBR = ((vz * 0.f) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));


		glm::vec3 translationFTL = ((vz * (voxSizeZ * -1.f)) + (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBTR = ((vz * (voxSizeZ * -1.f)) - (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationBTL = ((vz * (voxSizeZ * -1.f)) - (vy * (voxSizeY * 0.5f)) + (vx * (voxSizeX * 0.5f)));
		glm::vec3 translationFTR = ((vz * (voxSizeZ * -1.f)) + (vy * (voxSizeY * 0.5f)) - (vx * (voxSizeX * 0.5f)));


		td::Vec3 FBL = { target.x - translationFBL.x, target.y - translationFBL.y, target.z - translationFBL.z };
		td::Vec3 BBR = { target.x - translationBBR.x, target.y - translationBBR.y, target.z - translationBBR.z };
		td::Vec3 BBL = { target.x - translationBBL.x, target.y - translationBBL.y, target.z - translationBBL.z };
		td::Vec3 FBR = { target.x - translationFBR.x, target.y - translationFBR.y, target.z - translationFBR.z };


		td::Vec3 FTL = { target.x - translationFTL.x, target.y - translationFTL.y, target.z - translationFTL.z };
		td::Vec3 BTR = { target.x - translationBTR.x, target.y - translationBTR.y, target.z - translationBTR.z };
		td::Vec3 BTL = { target.x - translationBTL.x, target.y - translationBTL.y, target.z - translationBTL.z };
		td::Vec3 FTR = { target.x - translationFTR.x, target.y - translationFTR.y, target.z - translationFTR.z };

		//bottom square
		glb::oFDL(glb::renderer, FBL, FBR, white, white, false);
		glb::oFDL(glb::renderer, FBL, BBL, white, white, false);
		glb::oFDL(glb::renderer, BBL, BBR, white, white, false);
		glb::oFDL(glb::renderer, BBR, FBR, white, white, false);

		//top square
		glb::oFDL(glb::renderer, FTL, FTR, white, white, false);
		glb::oFDL(glb::renderer, FTL, BTL, white, white, false);
		glb::oFDL(glb::renderer, BTL, BTR, white, white, false);
		glb::oFDL(glb::renderer, BTR, FTR, white, white, false);

		//walls
		glb::oFDL(glb::renderer, FTL, FBL, white, white, false);
		glb::oFDL(glb::renderer, FTR, FBR, white, white, false);
		glb::oFDL(glb::renderer, BTL, BBL, white, white, false);
		glb::oFDL(glb::renderer, BTR, BBR, white, white, false);

	}

	td::Vec3 glm2td(glm::vec3 in) {
		return { in.x, in.y, in.z };
	}

	glm::vec3 td2glm(td::Vec3 in) {
		return glm::vec3(in.x, in.y, in.z);
	}

	void updateDebugObject() {
		if (!dbgObject.body)
		{
			return;
		}
		//-----pointer thing-----
		//drawPointerThing();



		//-----debugobject vectors-----

		raycaster::rayData getTarget = raycaster::castRayPlayer();
		if (getTarget.distance > 100.f) {
			return;
		}

		dbgObject.shape = getTarget.hitShape;
		dbgObject.body = getTarget.hitShape->getParentBody();
		dbgObject.vox = getTarget.hitShape->pVox;

		drawCube(dbgObject.body->Position, 0.05f, white);
		td::Vec3 oSizeObj = { float(dbgObject.vox->sizeX), float(dbgObject.vox->sizeY), float(dbgObject.vox->sizeZ) };
		glm::vec3 boundaries = glm::vec3(dbgObject.vox->sizeX / 10.f, dbgObject.vox->sizeY / 10.f, dbgObject.vox->sizeZ / 10.f);

		glm::vec3 bodyPos = glm::vec3(dbgObject.body->Position.x, dbgObject.body->Position.y, dbgObject.body->Position.z);
		glm::quat bodyQuat = *(glm::quat*)&dbgObject.body->Rotation;
		glm::mat4 RotationMatrix = glm::toMat4(bodyQuat);

		//Local -> World
		glm::vec3 bodyLineX = bodyPos + glm::vec3(RotationMatrix * glm::vec4((oSizeObj.x) / 10.f, 0.f, 0.f, 0.f));
		glb::oFDL(glb::renderer, glm2td(bodyPos), glm2td(bodyLineX), red, red, false);

		glm::vec3 bodyLineY = bodyPos + glm::vec3(RotationMatrix * glm::vec4(0.f, (oSizeObj.y) / 10.f, 0.f, 0.f));
		glb::oFDL(glb::renderer, glm2td(bodyPos), glm2td(bodyLineY), green, green, false);

		glm::vec3 bodyLineZ = bodyPos + glm::vec3(RotationMatrix * glm::vec4(0.f, 0.f, (oSizeObj.z) / 10.f, 0.f));
		glb::oFDL(glb::renderer, glm2td(bodyPos), glm2td(bodyLineZ), blue, blue, false);

		//World -> Local
		raycaster::rayData rd = raycaster::castRayPlayer();
		td::Vec3 worldPos = rd.worldPos;
		td::Vec3 worldRot = rd.angle;

		glb::oFDL(glb::renderer, worldPos, { worldPos.x + worldRot.x, worldPos.y + worldRot.y, worldPos.z + worldRot.z }, white, white, false);

		glm::vec3 localPos = glm::vec3((glm::inverse(RotationMatrix)) * -glm::vec4(bodyPos.x - worldPos.x, bodyPos.y - worldPos.y, bodyPos.z - worldPos.z, 0.f));

		glm::vec3 convertedWorldPos = bodyPos + glm::vec3(RotationMatrix * glm::vec4(localPos.x, localPos.y, localPos.z, 0.f));

		std::cout << std::to_string(localPos.x) << " : " << std::to_string(localPos.y) << " : " << std::to_string(localPos.z) << std::endl;
		drawCube(glm2td(convertedWorldPos), 0.05f, red);

	}
}

