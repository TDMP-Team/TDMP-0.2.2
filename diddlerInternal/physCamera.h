#pragma once

#include "threadedCamera.h"

namespace physCamera {
	extern threadCamera::KMCamera* objCamera;
	void spawnCameraObject();
	void updateCamera();
	void destroyCamera();
}
