#include "windows.h"
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "objectSpawner.h"

namespace cHandler {

	int stageCode = 0;

	std::string getDateTime() {
		char* timeBuffer = new char[255];
		time_t now = time(0);
		struct tm  tstruct;
		localtime_s(&tstruct, &now);
		strftime(timeBuffer, 25, "%c", &tstruct);
		std::string consoleOutput = std::string(timeBuffer);
		free(timeBuffer);
		return consoleOutput;
	}

	std::string getDumpFileName() {
		char* timeBuffer = new char[255];
		time_t now = time(0);
		struct tm  tstruct;
		localtime_s(&tstruct, &now);
		strftime(timeBuffer, 25, "%F-%H-%M", &tstruct);
		std::string consoleOutput = "KM-Crash-" + std::string(timeBuffer) + ".txt";
		free(timeBuffer);
		return consoleOutput;
	}

	bool isAscii(byte c) {
		return !(std::isprint(c) == 0);
	}

	char validatePrintable(byte c) {
		if (!(std::isprint(c) == 0)) {
			return c;
		}
		return '.';
	}

	std::string createTextMemoryDump(byte* location, size_t size) {
		std::stringstream dumpStream;
		byte* endAddress = location + size;
		int offset = 0;
		int vOffset = 0;
		int lineCount = ceil(size / 16);
		char tBuffer[70] = {};
		byte validationBuffer[16] = {};

		for (int i = 0; i < lineCount; i++) {

			vOffset = 0;
			for (int c = 0; c < 15; c++) {
				byte* thisLocation = (location + offset + c);
				if (!(thisLocation > endAddress)) {
					validationBuffer[vOffset] = *thisLocation;
				}
				else {
					validationBuffer[vOffset] = 0;
				}
				vOffset++;
			}

			sprintf_s((tBuffer), "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
				validationBuffer[0],
				validationBuffer[1],
				validationBuffer[2],
				validationBuffer[3],
				validationBuffer[4],
				validationBuffer[5],
				validationBuffer[6],
				validationBuffer[7],
				validationBuffer[8],
				validationBuffer[9],
				validationBuffer[10],
				validationBuffer[11],
				validationBuffer[12],
				validationBuffer[13],
				validationBuffer[14],
				validationBuffer[15],
				validatePrintable(validationBuffer[0]),
				validatePrintable(validationBuffer[1]),
				validatePrintable(validationBuffer[2]),
				validatePrintable(validationBuffer[3]),
				validatePrintable(validationBuffer[4]),
				validatePrintable(validationBuffer[5]),
				validatePrintable(validationBuffer[6]),
				validatePrintable(validationBuffer[7]),
				validatePrintable(validationBuffer[8]),
				validatePrintable(validationBuffer[9]),
				validatePrintable(validationBuffer[10]),
				validatePrintable(validationBuffer[11]),
				validatePrintable(validationBuffer[12]),
				validatePrintable(validationBuffer[13]),
				validatePrintable(validationBuffer[14]),
				validatePrintable(validationBuffer[15])
			);

			offset += 16;
			dumpStream << std::string(tBuffer) << std::endl;
		}
		return dumpStream.str();
	}

	static const char* seDescription(DWORD code)
	{
		switch (code) {
		case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";
		default: return "UNKNOWN EXCEPTION";
		}
	}

	int handleException(int code, PEXCEPTION_POINTERS ex) {
		std::cout << "Crash detected" << std::endl;

		std::ofstream crashDump;
		crashDump.open(getDumpFileName());
		crashDump << "Crash reported at " << getDateTime() << std::endl;
		crashDump << "Stage                : " << std::to_string(stageCode) << std::endl;
		crashDump << std::endl;

		crashDump << "Error code           : " << std::hex << code << " (" << seDescription(code) << ")" << std::endl;
		crashDump << "Error location       : " << std::hex << (void*)(ex->ExceptionRecord->ExceptionAddress) << std::endl;
		crashDump << "Error params         : " << std::endl;

		for (int i = 0; i < ex->ExceptionRecord->NumberParameters; i++) {
			crashDump << "  Param " << std::to_string(i) << ": " << std::hex << (void*)(ex->ExceptionRecord->ExceptionInformation[i]) << std::endl;
		}

		crashDump << std::endl;

		crashDump << "[Base Addresses:           ]" << std::endl;
		crashDump << "HWND                 : " << glb::gWnd << std::endl;
		crashDump << "baseModuleHandle     : " << (void*)glb::moduleBase << std::endl;

		crashDump << std::endl;
		crashDump << "[Object Addresses:         ]" << std::endl;
		crashDump << "Game                 : " << std::hex << glb::TDOL << std::endl;
		crashDump << "Player               : " << std::hex << glb::player << std::endl;
		crashDump << "Scene                : " << std::hex << glb::scene << std::endl;
		crashDump << "Renderer             : " << std::hex << glb::renderer << std::endl;
		crashDump << std::endl;

		crashDump << "[Function Addresses:       ]" << std::endl;
		crashDump << "cursor               : " << std::hex << glb::ocursor << std::endl;
		crashDump << "tdRaycastCrashB      : " << std::hex << glb::tdRaycastCrashB << std::endl;
		crashDump << "loadResource         : " << std::hex << glb::oLoadResource << std::endl;
		crashDump << "oValidatePath        : " << std::hex << glb::oValidatePath << std::endl;
		crashDump << "oCreateTextureThing  : " << std::hex << glb::oCreateTextureThing << std::endl;
		crashDump << "tdUpdateScreen       : " << std::hex << glb::tdUpdateScreen << std::endl;
		crashDump << "tdConstructJoint     : " << std::hex << glb::tdConstructJoint << std::endl;
		crashDump << "tdInitBall           : " << std::hex << glb::tdInitBall << std::endl;
		crashDump << "tdInitWire           : " << std::hex << glb::tdInitWire << std::endl;
		crashDump << "tdAttachJoint        : " << std::hex << glb::tdAttachJoint << std::endl;
		crashDump << "tPaint               : " << std::hex << glb::oPaint << std::endl;
		crashDump << "drawLine             : " << std::hex << glb::oFDL << std::endl;
		crashDump << "raycast              : " << std::hex << glb::oRC << std::endl;
		crashDump << "create explosion     : " << std::hex << glb::TDcreateExplosion << std::endl;
		crashDump << "spawn vox            : " << std::hex << glb::oSpawnVox << std::endl;
		crashDump << "make hole            : " << std::hex << glb::oWrappedDamage << std::endl;
		crashDump << "create texture       : " << std::hex << glb::oCreateTexture << std::endl;
		crashDump << "create physics       : " << std::hex << glb::oCreatePhysics << std::endl;
		crashDump << "update shapes        : " << std::hex << glb::oUpdateShapes << std::endl;
		crashDump << "construct body       : " << std::hex << glb::oB_Constructor << std::endl;
		crashDump << "construct shape      : " << std::hex << glb::oS_Constructor << std::endl;
		crashDump << "set dynamic          : " << std::hex << glb::oSetDynamic << std::endl;
		crashDump << "malloc               : " << std::hex << glb::oTMalloc << std::endl;
		crashDump << "free                 : " << std::hex << glb::oTFree << std::endl;
		crashDump << "spawn fire           : " << std::hex << glb::oSpawnFire << std::endl;
		crashDump << "shoot projectile     : " << std::hex << glb::oPewpew << std::endl;
		crashDump << "spawn particle       : " << std::hex << glb::TDspawnParticle << std::endl;
		crashDump << "damage player        : " << std::hex << glb::oDamagePlayer << std::endl;
		crashDump << "damage object 2      : " << std::hex << glb::oDamageObject << std::endl;
		crashDump << "cam positioner       : " << std::hex << glb::oCamPos << std::endl;
		crashDump << "fire spread          : " << std::hex << glb::oSpreadFire << std::endl;
		crashDump << "set attribute        : " << std::hex << glb::oSOA << std::endl;
		crashDump << "set attribute single : " << std::hex << glb::oSOAS << std::endl;
		crashDump << "highlight shape      : " << std::hex << glb::oHighlightShape << std::endl;
		crashDump << "outline shape        : " << std::hex << glb::oOutlineShape << std::endl;
		crashDump << "outline body         : " << std::hex << glb::oOutlineBody << std::endl;
		crashDump << "QuatEuler            : " << std::hex << glb::oQuatEul << std::endl;
		crashDump << "createPlankVox       : " << std::hex << glb::oSetBody << std::endl;
		crashDump << "Read subobjects      : " << std::hex << glb::TDreadSubobjects << std::endl;
		crashDump << "oIsActiveWindow      : " << std::hex << glb::oIsActive << std::endl;
		crashDump << "update shape body    : " << std::hex << glb::tdUpdateShapeBody << std::endl;
		crashDump << std::endl;

		crashDump << "Vox database size     : " << spawner::spawnerObjectsDatabase.size() << std::endl;
		for (spawner::spawnerCatagory& sc : spawner::spawnerObjectsDatabase) {
			crashDump << "  [" << sc.name << " : " << std::to_string(sc.objects.size()) << "]" << std::endl;
		}

		crashDump << std::endl;
		crashDump << "BEGIN MODULE MEMORY DUMPS:" << std::endl;
		crashDump << std::endl;

		crashDump << "PLAYER:" << std::endl;
		crashDump << cHandler::createTextMemoryDump((byte*)glb::player, sizeof(TDPlayer)) << std::endl;

		crashDump << "SCENE:" << std::endl;
		crashDump << cHandler::createTextMemoryDump((byte*)glb::scene, sizeof(TDScene)) << std::endl;

		crashDump << "GAME:" << std::endl;
		crashDump << cHandler::createTextMemoryDump((byte*)glb::game, sizeof(TDGame)) << std::endl;

		crashDump.close();
		return EXCEPTION_EXECUTE_HANDLER;
	}
}