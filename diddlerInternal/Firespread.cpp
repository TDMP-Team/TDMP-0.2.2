#include "mem.h"
#include "Firespread.h"
#include <iostream>

namespace fireSpreadMod {
	const byte ignorePlayerRules1[] = { 0x75, 0x59, 0x44, 0x0F, 0x2F, 0x44, 0x37, 0x14, 0x73, 0x51 }; //signature for NOP position 1
	const byte ignorePlayerRules2[] = { 0x75, 0x5F, 0xF3, 0x0F, 0x10, 0x05, 0x91, 0xB7, 0x21, 0x00 }; //signature for NOP position 2
	const byte ignorePlayerRules3[] = { 0x0F, 0x85, 0x05, 0x01, 0x00, 0x00, 0x66, 0xC7  }; //signature for NOP position 3

	const byte normalByteCode1[] =    { 0x75, 0x59, 0x44, 0x0F, 0x2F, 0x44, 0x37, 0x14, 0x73, 0x51 }; //default data for position 1
	const byte normalByteCode2[] =    { 0x75, 0x5F };												  //default data for position 2
	const byte normalByteCode3[] =    { 0x0F, 0x85, 0x05, 0x01, 0x00, 0x00, 0x66, 0xC7 };             //default data for position 3

	byte* ptr_playerRules1 = 0;
	byte* ptr_playerRules2 = 0;
	byte* ptr_playerRules3 = 0;

	bool isFireModEnabled = false;

	void enableEverythingBurns() {
		byte* ptr_playerRules1 = (byte*)mem::FindPattern((byte*)ignorePlayerRules1, "xxxxxxxxxx", GetModuleHandle(NULL));
		byte* ptr_playerRules2 = (byte*)mem::FindPattern((byte*)ignorePlayerRules2, "xxxxxxxxxx", GetModuleHandle(NULL));
		byte* ptr_playerRules3 = (byte*)mem::FindPattern((byte*)ignorePlayerRules3, "xxxxxxxx", GetModuleHandle(NULL));
		
		std::cout << ":ENABLE:" << (uintptr_t)ptr_playerRules1 << ":" << (uintptr_t)ptr_playerRules2 << ":" << (uintptr_t)ptr_playerRules3 << std::endl;

		mem::Nop(ptr_playerRules1, 10);
		mem::Nop(ptr_playerRules1, 2);
		mem::Nop(ptr_playerRules1, 8);
	}

	void disableEverythingBurns() {
		mem::Patch(ptr_playerRules1, (byte*)normalByteCode1, 10);
		mem::Patch(ptr_playerRules1, (byte*)normalByteCode2, 2);
		mem::Patch(ptr_playerRules1, (byte*)normalByteCode3, 8);
	}

	void toggleFireMod() {
		if (isFireModEnabled) {
			disableEverythingBurns();
		}
		else {
			enableEverythingBurns();
		}
	}
}