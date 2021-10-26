#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include "Mods.h"

void hkGodmode(TDPlayer* player, float damage) {
    if (mods::godmode) {
        return;
    }

    return glb::oDamagePlayer(player, damage);
}

void initGodmodeHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)glb::oDamagePlayer, hkGodmode);
    DetourTransactionCommit();
}

void terminateGodmodeHook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)glb::oDamagePlayer, hkGodmode);
    DetourTransactionCommit();
}