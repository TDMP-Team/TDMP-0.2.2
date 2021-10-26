#pragma once
#include <iomanip>

uintptr_t containedMalloc(size_t size);
void containedFree(void* mem);