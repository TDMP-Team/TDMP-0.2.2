#include <vector>
#include "balloons.h"
#include "Global.h"
#include "objectSpawner.h"
#include "Raycaster.h"
#include "drawCube.h"

namespace balloons {

	struct balloon {
		TDShape* shape;
		TDBody* body;
		float upVelocity;
		
		bool destroyed = false;
		bool hasString = false;
		int wait = 1;
		td::Vec3 origin = { 0, 0, 0 };
	};

	float r2dp(float var)
	{
		float value = (int)(var * 100 + .5);
		return (float)value / 100;
	}

	std::vector<balloon> balloons = {};
	td::Color ropeColor = { 1.f, 1.f, 1.f, 1.f };

	void updateAllBalloons() {


		bool validBalloons = false;
		for (balloon& b : balloons) {
			if (b.shape && !b.shape->isBroken && !b.destroyed) {
				validBalloons = true;
				if (b.body->Velocity.y < b.upVelocity) {
					//b.body->Velocity.y += 5.f;
					b.body->Velocity.y = b.upVelocity;
					//printf_s("New velo on %p: %0.2f\n", b, b.body->Velocity.y);
				}

				if (!b.hasString) {
					if (b.wait > 0) {
						b.wait--;
						continue;
					}
					TDJoint* newJoint = (TDJoint*)glb::oTMalloc(208);
					glb::tdConstructJoint(newJoint, nullptr);

					td::Vec3 floorPos = b.origin;

					glm::quat q = *(glm::quat*)&b.body->Rotation;
					glm::vec3 vx = q * glm::vec3(-1, 0, 0);
					glm::vec3 vy = q * glm::vec3(0, -1, 0);
					glm::vec3 vz = q * glm::vec3(0, 0, -1); //(UP)

					glm::vec3 centerpoint = math::v3_td2glm(b.body->Position) + ((vz * 0.f) + (vy * -0.5f) + (vx * -0.5f));

					td::Vec3 bPos = math::v3_glm2td(centerpoint);

					//td::Vec3 bPos = { b.body->Position.x, b.body->Position.y + 0.5f, b.body->Position.z };

					drawCube(floorPos, 0.05f, ropeColor);
					drawCube(bPos, 0.05f, ropeColor);

					glb::tdInitWire(newJoint, &floorPos, &bPos, newJoint->m_Size, ropeColor, 2.5f, 100.f, 1.f);

					b.hasString = true;
				}

				if (b.body->Position.y > 500.f) {
					td::Vec3 centerpoint = { b.shape->posMax.x - ((b.shape->posMax.x - b.shape->posMin.x) / 2.f), b.shape->posMax.y - ((b.shape->posMax.y - b.shape->posMin.y) / 2.f), b.shape->posMax.z - ((b.shape->posMax.z - b.shape->posMin.z) / 2.f) };
					glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, 0.5f);
					b.destroyed = true;
				}

				if (r2dp(b.body->Velocity.x) > 0.f) { b.body->Velocity.x -= 0.5f; }
				else if (r2dp(b.body->Velocity.x) < 0.f) { b.body->Velocity.x += 0.5f; }
				else { b.body->Velocity.x == 0.f; }

				if (r2dp(b.body->Velocity.z) > 0.f) { b.body->Velocity.z -= 0.5f; }
				else if (r2dp(b.body->Velocity.z) < 0.f) { b.body->Velocity.z += 0.5f; }
				else { b.body->Velocity.z == 0.f; }
			}
		}

		if ((glb::game->m_LoadingEffect != 0 || !validBalloons) && balloons.size() > 0) {
			printf_s("Clearing balloons\n");
			balloons.clear();
			return;
		}
	}

	void spawnBalloon(float power) {
		spawner::spawnedObject obj = spawner::placeFreeObject("KM_Vox\\Default\\balloon\\object.vox");
		obj.body->Position.y += 1.f;

		raycaster::rayData rd = raycaster::castRayPlayer();


		//drawCube(floorPos, 0.05f, ropeColor);
		//drawCube(bPos, 0.05f, ropeColor);


		balloon b = { obj.shapes[0], obj.body, power, false, false, 2, rd.worldPos };
		balloons.push_back(b);
	}
}