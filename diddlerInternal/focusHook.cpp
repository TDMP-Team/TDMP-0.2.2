#include "TDFuncs.h"
#include "types.h"
#include "Global.h"
#include <cfloat>

namespace focusHook {

    bool hkGetFocus(void* a1){
        glb::isGameFocused = glb::oIsActive(a1);

        return true;
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