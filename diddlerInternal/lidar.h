#pragma once

namespace lidar {
	extern float zoom;
	extern bool colour;
	extern bool enabled;

	void nextFrame();
	void drawLidarWindow(td::Vec2 size);
}
