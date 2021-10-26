#include "Global.h"
#include "envOptions.h"

namespace envOptions {

	enum class skyboxesClean : int8_t {
		cloudy = 0,
		day = 1,
		moonlit = 2,
		night = 3,
		night_clear = 4,
		sunset = 5
	};

	const char* skyboxes[] = {
		"data/env/cloudy.dds",
		"data/env/day.dds",
		"data/env/moonlit.dds",
		"data/env/night.dds",
		"data/env/night_clear.dds",
		"data/env/sunset.dds",
	};

	void update() {
		glb::oEnvUpdate((uintptr_t)glb::scene->pEnvironment);
	}

	void setNight() {
		//glb::scene->env->skyboxpath = skyboxes[(int)skyboxesClean::night];
		//glb::scene->pEnvironment->sunBrightness = 0.f;
		//glb::scene->pEnvironment->sunSpread = 0.f;
		//glb::scene->pEnvironment->sunLength = 0.f;
		//glb::scene->pEnvironment->sunFogScale = 0.f;
		//glb::scene->pEnvironment->sunGlare = 0.f;
		//glb::scene->pEnvironment->sunColor = { 0, 0, 0 };
		//glb::scene->pEnvironment->sunColorTint = { 0, 0, 0 };
		//glb::scene->pEnvironment->exposure = { 5.f, 0.f };
		//glb::scene->pEnvironment->fogColor = { 0, 0, 0 };
		//glb::scene->pEnvironment->ambient = 0.f;
		//glb::scene->pEnvironment->brightness = -2.f;
		//update();
	}
}