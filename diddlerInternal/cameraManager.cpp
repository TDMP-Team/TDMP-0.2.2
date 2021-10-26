#include <cstdint>
#include "Global.h"
namespace cameraManager{
	byte xBuffer[5] = { 0xF2, 0x0F, 0x10, 0x43, 0x7C };
	byte yBuffer[8] = { 0xF3, 0x0F, 0x11, 0x93, 0x80, 0x00, 0x00, 0x00 };
	byte yBuffer2[5] = { 0xF2, 0x0F, 0x11, 0x43, 0x7C };
	byte zBuffer[8] = { 0xF3, 0x0F, 0x11, 0x83, 0x84, 0x00, 0x00, 0x00 };

	void takeCamera() {
		uintptr_t baseAddy = (uintptr_t)glb::oCamPos;
		uintptr_t xInstruction = baseAddy + 0x66E; // ok
		uintptr_t yInstruction = baseAddy + 0x65D; // ok 
		uintptr_t yInstruction2 = baseAddy + 0x66E; // ok 
		uintptr_t zInstruction = baseAddy + 0x621; // ok

		mem::Nop((byte*)xInstruction, 5);
		mem::Nop((byte*)yInstruction, 8);
		mem::Nop((byte*)yInstruction2, 5);
		mem::Nop((byte*)zInstruction, 8);
	}

	void returnCamera() {
		uintptr_t baseAddy = (uintptr_t)glb::oCamPos;
		uintptr_t xInstruction = baseAddy + 0x66E; // ok
		uintptr_t yInstruction = baseAddy + 0x65D; // ok 
		uintptr_t yInstruction2 = baseAddy + 0x66E; // ok 
		uintptr_t zInstruction = baseAddy + 0x621; // ok

		mem::Patch((byte*)xInstruction, xBuffer, 5);
		mem::Patch((byte*)yInstruction, yBuffer, 8);
		mem::Patch((byte*)yInstruction2, yBuffer2, 5);
		mem::Patch((byte*)zInstruction, zBuffer, 8);
	}
}
