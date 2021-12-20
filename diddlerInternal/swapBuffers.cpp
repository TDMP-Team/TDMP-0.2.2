#define GLEW_STATIC
#include "Global.h"
#include "TDObjects.h"
#include "swapBuffers.h"
#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include "miscPatches.h"
#include "constClock.h"
#include "console.h"
#include "multiplayer/Main.h"

#pragma comment(lib, "glew32s.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;
ImGuiWindowFlags infoBoxFlags;
std::once_flag swapBuffersInit;
int killInfoBoxCounter = 0;
bool displayInfoLabel = true;
td::Vec2 displayInfoLabelSizeY = { 0.f, 0.f };

bool showBounds = false;
bool showBodes = false;
bool showShapes = false;
int selectedToolgunTool = 0;

float teleportTargetPosition[3] = { 0, 0, 0 };
int selectedTab = 0;

ImColor ropeColorBuffer = ImColor(255, 255, 255);
const char* tgName = "toolgun";

LRESULT APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool lockoutScroll = false;

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

	ImGui::Text("Welcome to knedmod");
	ImGui::Text("To access the main menu, press [F1]. To enter noclip, press [V]");
	ImGui::Separator();
	ImGui::Text("This build of knedmod is designed for teardown 0.8.0");
	ImGui::Text("Knedmod is in beta, you are likely to encounter bugs / crashes");
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
		//showWelcomeMessage();
	}

	if (glb::displayMenu) {
		ImGui::Begin("KnedMod");
		int counter = 0;

		ImGuiWindowFlags window_flags1 = ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetWindowWidth() * 0.75, 0), true, window_flags1);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Spawn menu"))
			{
				ImGui::MenuItem("Refresh", NULL, &needToLoadObjects);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("Search: ");
		ImGui::SameLine();
		filter.Draw("##search", 210.f);

		int collapse = 0;

		if (filter.IsActive()) {
			collapse = 32;
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::SameLine(); //=======================================================`	

		ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 0), true, window_flags2);

		ImGui::BeginTabBar("CtrlTabs");

		if (ImGui::TabItemButton("Toolgun")) {
			selectedTab = 0;
		}
		if (ImGui::TabItemButton("Settings")) {
			selectedTab = 1;
		}

		if (selectedTab == 0) {
		}
		else {
			if (ImGui::CollapsingHeader("misc")) {
				ImGui::InputFloat3("TP pos (X:Y:Z)", teleportTargetPosition, 2);

				if (ImGui::Button("TP")) {
					glb::player->position = { teleportTargetPosition[0], teleportTargetPosition[1], teleportTargetPosition[2] };
				}

				ImGui::SliderFloat("Vox scale", &spawner::voxScale, 0.1f, 2.0f, "%.2f");
				ImGui::Checkbox("Info popup", &displayInfoLabel);
				ImGui::Checkbox("Show bounds", &showBounds);
				ImGui::Checkbox("Show bodies", &showBodes);
				ImGui::Checkbox("Show shapes", &showShapes);
			}
		}

		ImGui::EndTabBar();

		ImGui::Separator();

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::End();

	}

	if (displayInfoLabel) {
		infoBoxFlags = 0;
		infoBoxFlags |= ImGuiWindowFlags_NoMove;
		infoBoxFlags |= ImGuiWindowFlags_NoResize;
		infoBoxFlags |= ImGuiWindowFlags_NoCollapse;
		infoBoxFlags |= ImGuiWindowFlags_NoTitleBar;
		infoBoxFlags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowPos(ImVec2(5, 5));
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGui::Begin("infoKnedMod", (bool*)false, infoBoxFlags);

		displayInfoLabelSizeY = { 250.f, ImGui::GetWindowHeight() };

		ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", (glb::player->cameraPosition.x), (glb::player->cameraPosition.y), (glb::player->cameraPosition.z));
		ImGui::Text("Pitch: %.2f Yaw: %.2f", glb::player->camPitch, glb::player->camYaw);
		ImGui::Text("Health: %i", (int)(glb::player->health * 100));

		switch (glb::game->State)
		{
		case gameState::ingame:
			ImGui::Text("Game state: In game");
			break;
		case gameState::menu:
			ImGui::Text("Game state: In menu");
			break;
		case gameState::quitting:
			ImGui::Text("Game state: Quitting");
			break;
		case gameState::splash:
			ImGui::Text("Game state: Splash");
			break;
		case gameState::start_ui:
			ImGui::Text("Game state: StartUI");
			break;
		case gameState::editor:
			ImGui::Text("Game state: Editor");
			break;
		}

		ImGui::End();
	}

	if (showBounds) {
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
	}

	//std::cout << "====================" << std::endl;
	//std::cout << "Real QRT: X: " << glb::player->cameraQuat.x << " Y: " << glb::player->cameraQuat.y << " Z: " << glb::player->cameraQuat.z << " W: " << glb::player->cameraQuat.w << std::endl;
	//td::Vec3 cameraEuler = glb::player->cameraEuler();
	//td::Vec4 cameraSimulatedQuat = euler2quat(cameraEuler.x, cameraEuler.y, cameraEuler.z);
	//std::cout << "Simu QRT: X: " << cameraSimulatedQuat.x << " Y: " << cameraSimulatedQuat.y << " Z: " << cameraSimulatedQuat.z << " W: " << cameraSimulatedQuat.w << std::endl;
	//std::cout << "Real EUL: X: " << cameraEuler.x << " Y: " << cameraEuler.y << " Z: " << cameraEuler.z << std::endl;
	//td::Vec3 cameraSimulatedEuler = quat2euler(cameraSimulatedQuat);
	//std::cout << "Simu EUL: X: " << cameraSimulatedEuler.x << " Y: " << cameraSimulatedEuler.y << " Z: " << cameraSimulatedEuler.z << std::endl;
	//std::cout << glb::player->heldItemName << std::endl;

	//td::Vec4 ray = castRayPlayer();
	//std::cout << "X: " << std::to_string(ray.x) << " Y: " << std::to_string(ray.y) << " Z: " << std::to_string(ray.z) << " D: " << std::to_string(ray.w) << std::endl;

	//td::Vec3 boomPos = { ray.x, ray.y, ray.z };
	//glb::TDcreateExplosionWrapped(0, &boomPos, 5);
	//glb::createExplosion((uintptr_t)glb::scene, &boomPos, 5);

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