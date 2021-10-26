#pragma once
#include <windows.h>
#include <vector>
//https://guidedhacking.com/threads/how-to-hack-any-game-first-internal-hack-dll-tutorial.12142/

namespace mem
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void Nop(BYTE* dst, unsigned int size);
	void Null(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	DWORD64 FindPattern(BYTE* bMask, const char* szMask, HMODULE hModule);
	bool Hook(char* src, char* dst, int len);
	char* TrampHook(char* src, char* dst, unsigned int len);
}