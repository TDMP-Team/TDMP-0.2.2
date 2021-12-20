#include "mem.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <winternl.h>
#include <iostream>

//https://guidedhacking.com/threads/how-to-hack-any-game-first-internal-hack-dll-tutorial.12142/

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::Null(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x00, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}


bool mem::Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	}
	return true;
}

bool compareExact(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	}
	return true;
}

int countMatchingChars(const char* szMask) {
	int cCount = 0;
	for (; *szMask; ++szMask)
	{
		if (*szMask == 'x') {
			cCount++;
		}
	}
	return cCount;
}

int compareClosest(const BYTE* pData, const BYTE* bMask, const char* szMask) {
	int cCount = 0;

	for (; *szMask; ++szMask, ++pData, ++bMask)
	{
		if (*szMask == 'x' && *pData == *bMask) {
			cCount++;
		}
	}

	return cCount;

	//for (; *szMask; ++szMask, ++pData, ++bMask)
	//{
	//	if (*szMask == 'x') {
	//		if (*pData == *bMask) {
	//			cCount++;
	//		}
	//	}
	//	else {
	//		return cCount;
	//	}
	//}
	//return cCount;
}

DWORD64 mem::FindPattern(BYTE* bMask, const char* szMask, HMODULE hModule, float* integrity)
{
	MODULEINFO moduleInfo = { 0 };
	GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));

	DWORD64 dwBaseAddress = (DWORD64)hModule;
	DWORD64 dwModuleSize = (DWORD64)moduleInfo.SizeOfImage;
	
	int matchLen = countMatchingChars(szMask);
	int searchLen = strlen(szMask);
	int high = searchLen / 4;
	DWORD64 cPtr = 0;

	//attempt sigscanning using the faster exact match method
	for (DWORD64 i = 0; i < dwModuleSize - searchLen; i++)
	{
		if (compareExact((BYTE*)(dwBaseAddress + i), bMask, szMask)) {
			if (integrity) { *integrity = 100.f; }
			return (DWORD64)(dwBaseAddress + i);
		}
	}

	//if exact scanning fails, begin scanning for closest match
	for (DWORD64 i = 0; i < dwModuleSize - searchLen; i++)
	{
		int c = compareClosest((BYTE*)(dwBaseAddress + i), bMask, szMask);
		if (c > high) {
			high = c;
			cPtr = (DWORD64)(dwBaseAddress + i);
		}

		if (high == searchLen) {
			if (integrity) { *integrity = ((float)high / (float)matchLen) * 100.f; }
			return cPtr;
		}
	}

	//return closest match, or 0 if nothing found
	if (integrity) { *integrity = ((float)high / (float)matchLen) * 100.f; }
	return cPtr;
}

bool mem::Hook(char* src, char* dst, int len)
{
	if (len < 5) return false;
	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);
	memset(src, 0x90, len);
	uintptr_t relativeAddress = (uintptr_t)(dst - src - 5);
	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddress;
	DWORD temp;
	VirtualProtect(src, len, curProtection, &temp);
	return true;
}

char* mem::TrampHook(char* src, char* dst, unsigned int len)
{
	if (len < 5) return 0;
	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t gateJmpAddy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xE9;
	*(uintptr_t*)(gateway + len + 1) = gateJmpAddy;
	if (Hook(src, dst, len)) { return gateway; }
	else return nullptr;
}