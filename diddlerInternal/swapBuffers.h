#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include <WinUser.h>

void initHIDsHook();
void initSwapBuffersHook();
void terminateHIDsHook();
void terminateSwapBuffersHook();