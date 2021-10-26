#pragma once
#include "Global.h"
#include "mem.h"

namespace noclip {
	extern bool enabled;
	extern bool inNoclip;

	void setCameraEnabled(bool enable);

	void backupInstructions();
	void ToggleNoclip();
	void update();
}