#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include "noclip.h"
#include "lantern.h"

void hkMovementLoop(uintptr_t a1, float a2, uintptr_t a3, uintptr_t a4) {



    return glb::oCamPos(a1, a2, a3, a4);
}

void terminateMovementHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oCamPos, hkMovementLoop);
    DetourTransactionCommit();
}

void initMovementHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oCamPos, hkMovementLoop);
    DetourTransactionCommit();
}
