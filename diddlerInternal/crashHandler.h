#pragma once
#include <winnt.h>
#include <string>
namespace cHandler {
	extern int stageCode;
	int handleException(int code, PEXCEPTION_POINTERS ex);
	std::string createTextMemoryDump(byte* location, int size);
}