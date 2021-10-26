#pragma once
#include "types.h"

namespace dotProjector {
	struct rayPixel {
		td::Vec3 dirVec = { 0.f, 0.f, 0.f };
		td::Vec3 worldPos = { 0.f, 0.f, 0.f };
		float dist = 0.f;
		int color_R = 0;
		int color_G = 0;
		int color_B = 0;
	};

	struct pixelResponse {
		float minDist = 0.f;
		float maxDist = 0.f;
		size_t size;
		rayPixel* data;
	};

	pixelResponse* projectDotMatrix(int resolution, float fov, float aspect, bool stochastic, glm::quat* camRotation, td::Vec3 camPosition, td::Vec3 forwardVector, td::Vec3 upVector, RaycastFilter* filter);
}