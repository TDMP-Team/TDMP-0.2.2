#pragma once
#include <string>
#include "windows.h"

enum toolnames {
	TOOL_SPAWNER, //done
	TOOL_MINIGUN, //done
	TOOL_CLICKEXPLODE, //done
	TOOL_FLAMETHROWER, 
	TOOL_DAMAGE, 
	TOOL_REMOVE,
	TOOL_ATTRIBUTE,
	TOOL_LEAFBLOWER,
	TOOL_SLICE,
	TOOL_CAMERA, //done
	TOOL_ROPE, //done
	TOOL_WELD, //done
	TOOL_DEBUG,
	TOOL_BALLOON,
	TOOL_DEV
};

struct tgt {
	toolnames tool;
	virtual DWORD exec() = 0;
};
