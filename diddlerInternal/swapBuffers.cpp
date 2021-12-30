#define GLEW_STATIC
#include "Global.h"
#include "TDObjects.h"
#include "swapBuffers.h"
#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include "constClock.h"
#include "console.h"
#include "multiplayer/Main.h"

#pragma comment(lib, "glew32s.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;
ImGuiWindowFlags infoBoxFlags;
std::once_flag swapBuffersInit;

LRESULT APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool lockoutScroll = false;

	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == 0x53)
		{
			TDMP::localInputData.S = true;
		}
		if (wParam == 0x57)
		{
			TDMP::localInputData.W = true;
		}
		if (wParam == VK_CONTROL)
		{
			TDMP::localInputData.Ctrl = true;
		}

		break;
	case WM_KEYUP:
		if (wParam == 0x53)
		{
			TDMP::localInputData.S = false;
		}
		if (wParam == 0x57)
		{
			TDMP::localInputData.W = false;
		}
		if (wParam == VK_CONTROL)
		{
			TDMP::localInputData.Ctrl = false;
		}
	}

	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	if (glb::displayMenu || lockoutScroll || console::consoleOpen)
	{
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool hwCursor(int x, int y) {

	if (glb::displayMenu || !glb::isGameFocused || console::consoleOpen) {
		return false;
	}

	return glb::ocursor(x, y);
}

bool terminator = true;
void onSwapBuffersInit()
{
	if (!ImGui::GetCurrentContext()) {
		std::cout << "[i] Creating ImGui instance" << std::endl;
		glewInit(); // initialize glew
		ImGui::CreateContext(); // create ImGui's context
		ImGui_ImplWin32_Init(glb::gWnd);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.85f);

		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

		style.Colors[ImGuiCol_Tab] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);

		style.Colors[ImGuiCol_Header] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

		style.Colors[ImGuiCol_Separator] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

		style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);

		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);

		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9f, 0.9f, 0.9f, 1.00f);
	}
}

void initHIDsHook() {
	HMODULE u32 = GetModuleHandle(L"user32.dll");
	glb::ocursor = (hk_SetCursorPos)GetProcAddress(u32, "SetCursorPos");

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(glb::gWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)glb::ocursor, hwCursor);
	DetourTransactionCommit();
}

void terminateHIDsHook() {
	SetWindowLongPtrA(glb::gWnd, GWLP_WNDPROC, LONG_PTR(oWndProc));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)glb::ocursor, hwCursor);
	DetourTransactionCommit();
}


bool needToLoadObjects = true;

char position[64];
char velocity[64];
char cPosition[64];
char health[64];
static ImGuiTextFilter filter;
char tempSkyboxPath[128] = {};

bool bTRUE = true;
bool bFALSE = false;
bool showingWelcome = true;

void showWelcomeMessage() {
	ImGui::SetNextWindowSize({ 500, 125 });
	ImGui::SetNextWindowPos({ (float)((glb::game->m_ResX / 2) - 250), (float)((glb::game->m_ResY / 2) - 62) });
	ImGui::Begin("Welcome");

	ImGui::Text("Welcome to TDMP");
	ImGui::Text("This mod is in alpha version");
	ImGui::Text("Be ready for crashes and bugs");
	ImGui::Separator();
	ImGui::Text("Made for Teardown 0.9.2");
	ImGui::Text("Press [ESC] to close this message");

	if (((((GetAsyncKeyState(VK_ESCAPE) >> 15) & 0x0001) == 0x0001))) {
		showingWelcome = false;
	}


	ImGui::End();
}

byte* pixelBuffer;
int lightness = 0;

int newRes = 100;
bool hwglSwapBuffers(_In_ HDC hDc)
{
	std::call_once(swapBuffersInit, onSwapBuffersInit);

	if (needToLoadObjects) {
		needToLoadObjects = false;
		spawner::spawnerObjectsDatabase = spawner::enumerateSpawnableObjects();
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiIO* IO = &ImGui::GetIO();
	IO->MouseDrawCursor = (glb::displayMenu || console::consoleOpen);
	console::drawConsole();

	if (showingWelcome) {
		showWelcomeMessage();
	}

	/*if (showBounds) {
		*(byte*)(glb::renderer + 0x86E0) = 0x01;
	}
	else {
		*(byte*)(glb::renderer + 0x86E0) = 0x00;
	}

	if (showBodes) {
		*(byte*)(glb::renderer + 0x86E1) = 0x01;
	}
	else {
		*(byte*)(glb::renderer + 0x86E1) = 0x00;
	}

	if (showShapes) {
		*(byte*)(glb::renderer + 0x86E2) = 0x01;
	}
	else {
		*(byte*)(glb::renderer + 0x86E2) = 0x00;
	}*/

	TDMP::Frame();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return glb::owglSwapBuffers(hDc);
}

void terminateSwapBuffersHook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)glb::owglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();

}

void initSwapBuffersHook() {
	HMODULE OpenGL = GetModuleHandle(L"opengl32.dll");
	glb::owglSwapBuffers = (twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)glb::owglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}