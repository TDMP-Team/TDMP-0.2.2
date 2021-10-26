#pragma once
#include "windows.h"
#include <string>

#define COMM_OK          0x01
#define COMM_ERROR_ARGS  0x02
#define COMM_ERROR_EXEC  0x03

#define MSG_GENERIC      0x10
#define MSG_WARNING      0x11
#define MSG_ERROR        0x12
#define MSG_BACKGROUND   0x13

namespace console {
	extern bool consoleOpen;
	extern bool firstFocus;
	extern int commandReversePtr;
	void drawConsole();
	void writeConsole(std::string text, DWORD messageType);
}
