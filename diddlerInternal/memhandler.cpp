#include "Global.h"

uintptr_t containedMalloc(size_t size) {
	return glb::oTMalloc(size);
}

void containedFree(void* mem) {
	return glb::oTFree((uintptr_t)mem);
}