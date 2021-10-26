#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include <cfloat>
#include "Mods.h"

namespace focusHook {

    bool hkGetFocus(void* a1){
        mods::isGameFocused = glb::oIsActive(a1);

        if (mods::dontLockWhenOutOfFocus) {
            return true;
        }
        else {
            return mods::isGameFocused;
        }
    }

    void initFocusHook() {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)glb::oIsActive, hkGetFocus);
        DetourTransactionCommit();
    }

    void terminateFocusHook() {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)glb::oIsActive, hkGetFocus);
        DetourTransactionCommit();
    }
}