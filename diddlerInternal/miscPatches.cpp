#include "Global.h"

namespace miscPatches {
	bool plankPatch = false;

	void updatePlankPatch () {
		byte origionalAsm[9] = {0xF3, 0x0F, 0x5F, 0xF7, 0xF3, 0x41, 0x0F, 0x5D, 0xF0}; //MAXSS XMM6, XMM7 - MINSS XMMM6, XMM8
		byte patchAsm[9]     = {0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0x66, 0x0F, 0x6E, 0xF2}; //MOV EDX, FFFFFF - MOVD XMM6, EDX
		uintptr_t patchLocation = glb::plankPatchFunction + 0x466;

		if (plankPatch) {
			mem::Patch((byte*)patchLocation, patchAsm, 9);
		}
		else {
			mem::Patch((byte*)patchLocation, origionalAsm, 9);
		}
	}
}