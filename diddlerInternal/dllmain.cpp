#define _WIN32_WINNT 0x0500
#define GLEW_STATIC
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "mem.h"
#include "glew.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#include "detours.h"
#include <mutex>
#include "Global.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>
#include "swapBuffers.h"
#include "Global.h"
#include "TDFuncs.h"
#include "hookTests.h"
#include "mainHook.h"
#include "movementLoop.h"
#include "focusHook.h"
#include <Psapi.h>
#include "windows.h"
#include "Psapi.h"
#include "Shlwapi.h"
#include "constClock.h"
#include "multiplayer/Main.h"
#include "Lua.h"

#pragma comment(lib, "psapi.lib")

/*

    welcome to the knedmod source code.
    i begun writing this program when i was incredibly new to c++
    as a result it is almost exclusively bad programming practeces, spaghettified global variables and unmanagable mess. 
    enjoy, and make sure vodka is on hand.

*/

FILE* makeConsole() {
    AllocConsole();
    std::wstring strW = L"DEBUG";
    SetConsoleTitle(strW.c_str());
    HWND console = GetConsoleWindow();
    DeleteMenu(GetSystemMenu(console, false), SC_CLOSE, MF_BYCOMMAND);
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.precision(2);
    SMALL_RECT tmp = { 0, 0, 120, 15 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmp);
    return f;
}

DWORD currentPID = 0;
const wchar_t* tdName = L"Teardown";

BOOL CALLBACK enumWndwsCallback(HWND hWnd, LPARAM lParam) {

    int length = GetWindowTextLength(hWnd);
    if (length == 0) {
        return TRUE;
    }

    wchar_t* buffer;
    buffer = new wchar_t[length + 1];
    memset(buffer, 0, (length + 1) * sizeof(wchar_t));

    GetWindowText(hWnd, buffer, length + 1);

    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hWnd, &lpdwProcessId);

    if (lpdwProcessId == currentPID && memcmp(tdName, buffer, 1) == 0) {
        glb::gWnd = hWnd;
    }

    return true;
}

bool getTeardownHwnd() {
    currentPID = GetCurrentProcessId();
    EnumWindows(enumWndwsCallback, NULL);
    return 0;
}

DWORD WINAPI main(HMODULE hModule)
{
    glb::hMdl = hModule;
    FILE* cnsl = makeConsole();
    std::cout << " __  __ _______ _______ _____  _______ _______ _____  " << std::endl;
    std::cout << "|  |/  |    |  |    ___|     \\|   |   |       |     \\ " << std::endl;
    std::cout << "|     <|       |    ___|  --  |       |   -   |  --  |" << std::endl;
    std::cout << "|__|\\__|__|____|_______|_____/|__|_|__|_______|_____/ " << std::endl;
    std::cout << "For teardown MAIN BRANCH version 0.8.0" << std::endl;
    std::cout << "KnedMod is C to Knedit and is released under the GNU public license" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;

    //glb::gWnd = FindWindow(0, L"Teardown");
    //
    //wchar_t buffer[MAX_PATH] = { 0 };
    //DWORD procID;
    //GetWindowThreadProcessId(glb::gWnd, &procID);
    //HINSTANCE Pr = (HINSTANCE)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, procID);
    //int FNlen = GetModuleFileNameEx(Pr, 0, buffer, MAX_PATH);
    //CloseHandle(Pr);
    //std::wcout << "M NAME: " << buffer << std::endl;

    HANDLE mainHandle = GetModuleHandle(NULL);
    getTeardownHwnd();

    std::cout << "[I] HWND           : " << glb::gWnd << std::endl;
    std::cout << "[I] baseModuleHandle: " << mainHandle << std::endl;
    glb::moduleBase = (uintptr_t)mainHandle;

    initSwapBuffersHook();

    std::cout << "[I] Hooked swapBuffers" << std::endl;

    initHIDsHook();

    std::cout << "[I] Hooked WNDPROC" << std::endl;
    std::cout << "[I] Hooked setCursorPos : " << std::hex << glb::ocursor << std::endl;

    sigscanItems();

    std::cout << "[I] Completed sigscanning" << std::endl;

    LUA::HookRegisterGameFunctions();

    std::cout << "[I] Hooked Lua" << std::endl;

    initTestHook();
    focusHook::initFocusHook();
    constClock::beginConstantClock(16.6f*2);

    TDMP::Init();
    //initMovementHook();

    while (true) {
        if (((GetAsyncKeyState(VK_END) >> 15) & 0x0001) == 0x0001) {
            if (true) {

                ////undo hooks
                terminateSwapBuffersHook();
                fclose(cnsl);
                FreeConsole();

                terminateMovementHook();
                terminateHIDsHook();
                terminateTestHook();
                focusHook::terminateFocusHook();
                constClock::endConstantClock();

                //sleep
                Sleep(250);

                //exit
                FreeLibraryAndExitThread(hModule, 0);
            }
        }
        Sleep(1);
    }
}


//idk, dont touch this
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, hModule, 0, nullptr));
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

