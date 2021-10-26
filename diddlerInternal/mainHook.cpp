#include "TDFuncs.h"
#include "types.h"
#include "Global.h"

void MAINHOOK(TDGame* game, void* a2) {

    std::cout << "game: " << std::hex << (uintptr_t)game << std::endl;

    return glb::oTDmain(game, a2);
}


void initMainHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oTDmain, MAINHOOK);
    DetourTransactionCommit();
}

void terminateMainHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oTDmain, MAINHOOK);
    DetourTransactionCommit();
}