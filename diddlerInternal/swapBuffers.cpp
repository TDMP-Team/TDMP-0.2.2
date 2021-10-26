#define GLEW_STATIC
#include "Global.h"
#include "TDObjects.h"
#include "swapBuffers.h"
#include "Raycaster.h"
#include "Mods.h"
#include "noclip.h"
#include "objectSpawner.h"
#include "c4.h"
#include "maths.h"
#include "camera.h"
#include "toolgun.h"
#include "lantern.h"
#include "envOptions.h"
#include "miscPatches.h"
#include "lidar.h"
#include "physCamera.h"
#include "Harop.h"
#include "objectTranslationTest.h"
#include "Firespread.h"
#include "physMonitor.h"
#include "constClock.h"
#include "console.h"
#include "tgtMinigun.h"
#include "wireObjectKernel.h"
#include "wireObjectSpawn.h"
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

	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (!console::consoleOpen) {
			if (wParam == VK_F1 || wParam == VK_INSERT)
			{
				glb::displayMenu = !glb::displayMenu;
				return true;
			}

			if (wParam == 0x49) { //I
				killInfoBoxCounter++;
				if (killInfoBoxCounter > 5) {
					displayInfoLabel = false;
					glb::displayMenu = true;
					killInfoBoxCounter = 0;
				}
			}

			if (wParam == mods::noclipKey && !glb::displayMenu) {
				noclip::ToggleNoclip();
			}

			if (wParam == 0x5A) { //Z
				spawner::deleteLastObject();
			}

			if (wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN) {
				if (memcmp(glb::player->heldItemName, tgName, 8) == 0) {
					spawner::switchRotationStep(wParam);
				}
			}
		}

	

		if (wParam == VK_OEM_3 || wParam == 0xDF) {
			console::consoleOpen = !console::consoleOpen;
			console::firstFocus = true;
		}

	case WM_MOUSEWHEEL:
		if (memcmp(glb::player->heldItemName, tgName, 8) == 0 && nToolgun::currentTool == toolnames::TOOL_SPAWNER) {
			if ((short)(HIWORD(wParam)) > 0 && (LOWORD(wParam) & MK_SHIFT)) {
				spawner::voxScale += 0.05f;
				lockoutScroll = true;
			}
			if ((short)(HIWORD(wParam)) < 0 && (LOWORD(wParam) & MK_SHIFT)) {
				spawner::voxScale -= 0.05f;
				if (spawner::voxScale < 0.0f) { spawner::voxScale = 0.05f; }
				lockoutScroll = true;
			}
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

	if (glb::displayMenu || !mods::isGameFocused || console::consoleOpen) {
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

	if (lidar::enabled) {
		lidar::drawLidarWindow(displayInfoLabelSizeY);
	}

	if (showingWelcome) {
		showWelcomeMessage();
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
		for (spawner::spawnerCatagory catigory : spawner::spawnerObjectsDatabase) {
			ImGui::Columns(8);
			for (spawner::LoadedSpawnableObject lso : catigory.objects) {
				if (filter.PassFilter(lso.basePath.c_str())) {
					if (counter % 8 == 0) {
						ImGui::Separator();
					}

					//ImGui::Text(lso.objectName.c_str());
					//ImGui::SetNextItemWidth(25);

					if (ImGui::ImageButton((void*)lso.imageTexture, ImVec2(ImGui::GetColumnWidth() - 23, ImGui::GetColumnWidth() - 23))) {
						spawner::freeObjectSpawnParams params = {};
						params.attributes = lso.attributes;
						params.useUserRotation = false;
						params.nocull = true;
						spawner::placeFreeObject(lso.voxPath, params);
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(lso.objectName.c_str());
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::Selectable("Spawn with toolgun")) {
							nToolgun::currentTool = toolnames::TOOL_SPAWNER;
							selectedToolgunTool = 0;

							nToolgun::instance_spawner->currentSpawngunObject = lso;
						}
						ImGui::EndPopup();
					}




					ImGui::NextColumn();
					counter++;
				}

			}
			//ImGui::Columns(1);
			
			//if (ImGui::CollapsingHeader(catigory.name.c_str(), collapse)) {

			//}
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
			//const char* items[] = { "Spawner", "Minigun", "Explosions", "Flamethrower", "Remover", "Set attribute", "Destroyer", "DebugObject", "Leafblower", "Slicer", "Camera", "Rope" };
			const char* bulletTypes[] = { "Bullet", "Shotgun", "Missile", "???" };
			const char* leafBlowerModes[] = { "Blow", "Succ", "Delete" };
			const char* spawnModesCh[] = { "Placed", "Thrown" };
			static int bullet_current = 0;
			static int blower_current = 0;
			static int spawnr_current = 0;

			bool* bpTrue = &bTRUE;
			bool* bpFalse = &bFALSE;

			if (ImGui::Button("Create test wire object", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { 
				wireObjects::spawnWireObject(0);
			};

			if (ImGui::Button("Spawner", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_SPAWNER; };
			if (nToolgun::currentTool == toolnames::TOOL_SPAWNER) {
				ImGui::Separator();
				//ImGui::Combo("Mode", &spawnr_current, spawnModesCh, IM_ARRAYSIZE(spawnModesCh));
				//toolgun::method = (toolgun::spawngunMethod)spawnr_current;

				if (ImGui::RadioButton("Placed (free)", spawner::freeMode)) {
					spawner::freeMode = true;
					spawner::childMode = false;
					spawner::thrownMode = false;
				}
				if (ImGui::RadioButton("Placed (child)", spawner::childMode)) {
					spawner::freeMode = false;
					spawner::childMode = true;
					spawner::thrownMode = false;
				}
				if (ImGui::RadioButton("Thrown", spawner::thrownMode)) {
					spawner::freeMode = false;
					spawner::childMode = false;
					spawner::thrownMode = true;
				}
				ImGui::Checkbox("Spawn constantly", &nToolgun::instance_spawner->constSpawn);
				ImGui::SliderFloat("Throw power", &nToolgun::instance_spawner->throwPower, 1.f, 100.f, "%.2f");
				ImGui::Separator();
			}

			if (ImGui::Button("Rope", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_ROPE; };
			if (nToolgun::currentTool == toolnames::TOOL_ROPE) {
				ImGui::Separator();
				ImGui::Text("Rope tab");
				ImGui::SliderFloat("Slack", &nToolgun::instance_rope->ropeSlack, -10.f, 10.f, "%.1f");
				ImGui::SliderFloat("Stretchyness", &nToolgun::instance_rope->ropeStrength, 0.f, 10.f, "%.1f");
				ImGui::SliderFloat("Max stretch", &nToolgun::instance_rope->ropeMaxStretch, 0.f, 10.f, "%.1f");
				ImGui::ColorPicker4("Colour", (float*)&nToolgun::instance_rope->ropeColor);
				ImGui::Separator();
			}

			if (ImGui::Button("Weld", ImVec2(ImGui::GetWindowWidth() - 16, 20))) {nToolgun::currentTool = toolnames::TOOL_WELD; };
			if (nToolgun::currentTool == toolnames::TOOL_WELD) {
				ImGui::Separator();
				ImGui::Text("Weld tab");
				ImGui::Separator();
			}


			if (ImGui::Button("Balloon", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_BALLOON; };
			if (nToolgun::currentTool == toolnames::TOOL_BALLOON) {
				ImGui::Separator();
				ImGui::Text("Balloon tab");
				ImGui::SliderFloat("Power", &nToolgun::instance_balloon->balloonPower, 1.f, 100.f, "%.1f");
				ImGui::Separator();
			}

			ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
			if (ImGui::Button("Minigun", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_MINIGUN; };
			if (nToolgun::currentTool == toolnames::TOOL_MINIGUN) {
				ImGui::Separator();
				ImGui::Combo("Bullet type", &bullet_current, bulletTypes, IM_ARRAYSIZE(bulletTypes));
				nToolgun::instance_minigun->bulletType = bullet_current;
				ImGui::SliderFloat("Minigun spread", &nToolgun::instance_minigun->minigunFov, 0.f, 5.f, "%.2f");
				ImGui::SliderInt("Minigun bullet count", &nToolgun::instance_minigun->bulletCount, 1, 50);
				ImGui::Separator();
			}

			ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
			if (ImGui::Button("Explosions", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_CLICKEXPLODE; };
			if (nToolgun::currentTool == toolnames::TOOL_CLICKEXPLODE) {
				ImGui::Separator();
				ImGui::SliderFloat("Explosion spread", &nToolgun::instance_explode->explosionFov, 0, 0.5, "%.2f");
				ImGui::SliderInt("Explosion count", &nToolgun::instance_explode->bulletCount, 1, 25);
				ImGui::SliderFloat("Power", &nToolgun::instance_explode->explosionPower, 0.25f, 25.f, "%.2f");
				ImGui::Separator();
			}

			if (ImGui::Button("Flamethrower", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_FLAMETHROWER; };
			if (nToolgun::currentTool == toolnames::TOOL_FLAMETHROWER) {
				ImGui::Separator();
				ImGui::SliderFloat("Radius", &nToolgun::instance_flamethrower->flRadius, 1.f, 10.f, "%.2f");
				ImGui::SliderInt("Chance %", &nToolgun::instance_flamethrower->chance, 1, 100);
				ImGui::Separator();
			}

			if (ImGui::Button("Remover", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_REMOVE; };
			if (nToolgun::currentTool == toolnames::TOOL_REMOVE) {
				ImGui::Separator();
				ImGui::Text("Remover tab");
				ImGui::Separator();
			}

			if (ImGui::Button("Set attribute", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_ATTRIBUTE; };
			if (nToolgun::currentTool == toolnames::TOOL_ATTRIBUTE) {
				ImGui::Separator();
				ImGui::InputText("Attribute 1", nToolgun::instance_attribute->setAttributeFirst, 128);
				ImGui::InputText("Attribute 2", nToolgun::instance_attribute->setAttributeSecond, 128);
				ImGui::Separator();
			}

			if (ImGui::Button("Destroyer", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_DAMAGE; };
			if (nToolgun::currentTool == toolnames::TOOL_DAMAGE) {
				ImGui::Separator();
				ImGui::SliderFloat("Max Dist", &nToolgun::instance_damage->maxRange, 0.1f, 100.f, "%.2f");
				ImGui::SliderFloat("Hole size", &nToolgun::instance_damage->holeSize, 0.1f, 100.f, "%.2f");
				ImGui::SliderFloat("Radius", &nToolgun::instance_damage->flRadius, 0.1f, 100.f, "%.2f");
				ImGui::SliderInt("Resolution", &nToolgun::instance_damage->res, 1, 1024);
				ImGui::Checkbox("Fire constantly", &nToolgun::instance_damage->fireConst);
				ImGui::Separator();
			}

			//if (ImGui::Button("Leafblower", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_LEAFBLOWER; };
			//if (nToolgun::currentTool == toolnames::TOOL_LEAFBLOWER) {
			//	//ImGui::Separator();
			//	//ImGui::Combo("Mode", &blower_current, leafBlowerModes, IM_ARRAYSIZE(leafBlowerModes));
			//	//toolgun::LBMode = (toolgun::leafblowerModes)blower_current;
			//	//ImGui::Checkbox("Show rays", &toolgun::showRayHitPos);
			//	//ImGui::SliderInt("Rays", &toolgun::leafBlowerRayCount, 0, 500);
			//	//ImGui::SliderFloat("FOV", &toolgun::leafBlowerFOV, 0.01f, 0.5f, "%.2f");
			//	//ImGui::Separator();
			//}

			if (ImGui::Button("Slicer", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_SLICE; };
			if (nToolgun::currentTool == toolnames::TOOL_SLICE) {
				ImGui::Separator();
				ImGui::SliderInt("Resolution", &nToolgun::instance_slice->slicer_resolution, 64, 1024);
				ImGui::SliderFloat("maxDist", &nToolgun::instance_slice->slicer_maxDist, 1.f, 1000.f, "%.2f");
				if (ImGui::RadioButton("Horizontal", nToolgun::instance_slice->slicerHorizontal)) {
					nToolgun::instance_slice->slicerHorizontal = true;
				}
				if (ImGui::RadioButton("Vertical", !nToolgun::instance_slice->slicerHorizontal)) {
					nToolgun::instance_slice->slicerHorizontal = false;
				}
				ImGui::Separator();
			}
		
			if (ImGui::Button("Camera", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_CAMERA; };
			if (nToolgun::currentTool == toolnames::TOOL_CAMERA) {
				ImGui::Separator();
				ImGui::SliderInt("Resolution X", &nToolgun::instance_camera->resolutionX, 32, 512);
				ImGui::SliderInt("Resolution Y", &nToolgun::instance_camera->resolutionY, 32, 512);
				ImGui::SliderFloat("FOV", &threadCamera::universialFov, 1.f, 20.f, "%.1f");
				if (ImGui::Button("Spawn physCamera", { 150, 0 })) {
					physCamera::spawnCameraObject();
				}
				if (ImGui::Button("Destroy physCamera", { 150, 0 })) {
					physCamera::destroyCamera();
				}
				if (ImGui::Button("Update")) {
					for (threadCamera::KMCamera* kmc : threadCamera::gameCameras) {
						kmc->setResolution(nToolgun::instance_camera->resolutionX, nToolgun::instance_camera->resolutionY);
					}
				}

				//ImGui::Checkbox("Monochrome", &camera::mono);
				//ImGui::Checkbox("Enable transparency", &camera::transparency);
				//ImGui::Checkbox("Save image", &toolgun::takeSnapshot);
				//ImGui::Checkbox("(Staged) show progress", &camera::showImageProgress);
				//ImGui::SliderInt("(Staged) max pixels / frame", &camera::staged_maxPixelsPerFrame, 100, 10000);
				//if (ImGui::RadioButton("Interlaced", camera::mode == camera::cameraMode::interlaced)) {
				//	camera::mode = camera::cameraMode::interlaced;
				//}
				//if (ImGui::RadioButton("Staged", camera::mode == camera::cameraMode::staged)) {
				//	camera::mode = camera::cameraMode::staged;
				//}
				//if (ImGui::RadioButton("Fullframe", camera::mode == camera::cameraMode::fullframe)) {
				//	camera::mode = camera::cameraMode::fullframe;
				//}
				ImGui::Separator();
			}

			//if (ImGui::Button("DebugObject", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_DEBUG; };
			//if (nToolgun::currentTool == toolnames::TOOL_DEBUG) {
			//	ImGui::Separator();
			//	ImGui::Text("DebugObject tab");
			//	ImGui::Separator();
			//}

			if (ImGui::Button("testing (dev stuff dont use)", ImVec2(ImGui::GetWindowWidth() - 16, 20))) { nToolgun::currentTool = toolnames::TOOL_DEV; };
			if (nToolgun::currentTool == toolnames::TOOL_DEV) {
				ImGui::Separator();
				ImGui::Text("Testing tab");
				ImGui::Separator();
			}

		}
		else {
			if (ImGui::CollapsingHeader("debug")) {
				if (ImGui::Button("Spawn DebugCube")) {
					objectTesting::spawnDebugObject();
				}
				if (ImGui::Button("Free DebugCube")) {
					objectTesting::destroyDebugObject();
				}
			}

			if (ImGui::CollapsingHeader("misc")) {
				ImGui::InputFloat3("TP pos (X:Y:Z)", teleportTargetPosition, 2);

				if (ImGui::Button("TP")) {
					glb::player->position = { teleportTargetPosition[0], teleportTargetPosition[1], teleportTargetPosition[2] };
				}

				ImGui::SliderFloat("Vox scale", &spawner::voxScale, 0.1f, 2.0f, "%.2f");
				ImGui::Checkbox("Info popup", &displayInfoLabel);
				ImGui::Checkbox("Godmode", &mods::godmode);
				ImGui::Checkbox("Show bounds", &showBounds);
				ImGui::Checkbox("Show bodies", &showBodes);
				ImGui::Checkbox("Show shapes", &showShapes);
				ImGui::Checkbox("Run when unfocused", &mods::dontLockWhenOutOfFocus);
				ImGui::Checkbox("Remove map boundary", &mods::removeWalls);
				//if (ImGui::Checkbox("Remove plank length limit", &miscPatches::plankPatch)) {
				//	miscPatches::updatePlankPatch();
				//}
				//if (ImGui::CollapsingHeader("objects")) {
				//	if (ImGui::Button("Spawn lantern")) {
				//		lantern::spawnLantern();
				//	}


				//	ImGui::SliderFloat("pointSize", &lantern::a1, 0.1f, 255.f, "%.2f");
				//	ImGui::SliderFloat("shadows", &lantern::a2, 0.1f, 255.f, "%.2f");
				//	ImGui::SliderFloat("brightness", &lantern::a3, 0.1f, 255.f, "%.2f");
				//	ImGui::SliderFloat("fog", &lantern::a4, 0.1f, 255.f, "%.2f");
				//	ImGui::Spacing();
				//	ImGui::SliderFloat("R", &lantern::lightR, 0.1f, 255.f, "%.2f");
				//	ImGui::SliderFloat("G", &lantern::lightG, 0.1f, 255.f, "%.2f");
				//	ImGui::SliderFloat("B", &lantern::lightB, 0.1f, 255.f, "%.2f");
				//}
				ImGui::SliderFloat("Cracker power", &c4::firecrackerExplosionSize, 0.f, 5.f, "%.1f");
			}

			if (ImGui::CollapsingHeader("minimap")) {
				ImGui::Checkbox("Enabled", &lidar::enabled);
				ImGui::SliderFloat("Zoom", &lidar::zoom, 1.f, 100.f, "%2.f");
				ImGui::Checkbox("Colour", &lidar::colour);
			}

			if (ImGui::CollapsingHeader("c4")) {
				ImGui::Checkbox("Global C4 detonation", &mods::c4_global_detonation);
				ImGui::SliderInt("C4 size", &c4::selectedBombSizeInt, 1, 3);
			}

			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::Checkbox("Seperate thread", &threadCamera::runInSeperateThread);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted("Run the camera routine in a seperate thread.\nFast, but sometimes unstable.");
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}
				ImGui::Checkbox("Stochastic sampling", &threadCamera::stochastic);
				if (ImGui::RadioButton("Colour", (threadCamera::method == threadCamera::cameraType::CT_Colour))) {
					threadCamera::method = threadCamera::cameraType::CT_Colour;
				}
				if (ImGui::RadioButton("Monochrome", (threadCamera::method == threadCamera::cameraType::CT_Monochrome))) {
					threadCamera::method = threadCamera::cameraType::CT_Monochrome;
				}
				if (ImGui::RadioButton("Distance", (threadCamera::method == threadCamera::cameraType::CT_Dist))) {
					threadCamera::method = threadCamera::cameraType::CT_Dist;
				}

				if (ImGui::Button("Spawn physCamera", { 150, 0 })) {
					physCamera::spawnCameraObject();
				}
				if (ImGui::Button("Destroy physCamera", { 150, 0 })) {
					physCamera::destroyCamera();
				}


			}

			//if (ImGui::CollapsingHeader("environment (buggy)")) {
			//	if (ImGui::Button("Set night")) {
			//		envOptions::setNight();
			//	}

			//	if (ImGui::Button("All lights off")) {
			//		for (TDLight* tdl : *(glb::scene->m_Lights)) {
			//			((TDLight*)tdl)->m_Enabled = false;
			//		}
			//	}

			//	if (ImGui::Button("All lights on")) {
			//		for (TDLight* tdl : *(glb::scene->m_Lights)) {
			//			((TDLight*)tdl)->m_Enabled = true;
			//		}
			//	}

			//	ImGui::SliderInt("New res", &newRes, 1, 1024);

			//	if (ImGui::Button("List screens")) {
			//		for (TDScreen* tds : *(glb::scene->m_Screens)) {

			//			//((TDScreen*)tds)->m_Resolution = { newRes, newRes };
			//			//glb::tdUpdateScreen(((TDScreen*)tds));
			//			std::cout << "Screen @ " << ((TDScreen*)tds) << " RES: " << &((TDScreen*)tds)->m_Resolution << " SIZE: " << std::to_string(((TDScreen*)tds)->m_Resolution.x) << " : " << std::to_string(((TDScreen*)tds)->m_Resolution.y) << " SCRIPT: " << (TDScreen*)tds->m_Script.c_str() << std::endl;
			//		}
			//	}

			//	//ImGui::InputText("Skybox", tempSkyboxPath, 128);
			//	ImGui::SliderFloat("Sun brightness", &glb::scene->pEnvironment->m_SunBrightness, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("Sun spread", &glb::scene->pEnvironment->m_SunSpread, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("Sun length", &glb::scene->pEnvironment->m_SunLength, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("Sun fog", &glb::scene->pEnvironment->m_FogScale, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("Sun glare", &glb::scene->pEnvironment->m_SunGlare, 0.f, 255.f, "%.2f");
			//	ImGui::Spacing();
			//	ImGui::SliderFloat("sun R", &glb::scene->pEnvironment->m_SunColorTint.x, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("sun G", &glb::scene->pEnvironment->m_SunColorTint.y, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("sun B", &glb::scene->pEnvironment->m_SunColorTint.z, 0.f, 255.f, "%.2f");
			//	ImGui::Spacing();
			//	ImGui::SliderFloat("Exposure max", &glb::scene->pEnvironment->m_Exposure.x, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("Exposure min", &glb::scene->pEnvironment->m_Exposure.y, 0.f, 255.f, "%.2f");
			//	ImGui::Spacing();
			//	ImGui::SliderFloat("Ambience", &glb::scene->pEnvironment->m_Ambient, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("Brightness", &glb::scene->pEnvironment->m_Brightness, 0.f, 255.f, "%.2f");
			//	ImGui::SliderFloat("rain", &glb::scene->pEnvironment->m_Rain, 0.f, 1.f, "%.2f");
			//	ImGui::SliderFloat("wetness", &glb::scene->pEnvironment->m_Wetness, 0.f, 1.f, "%.2f");
			//	ImGui::SliderFloat("puddle ammount", &glb::scene->pEnvironment->m_PuddleAmount, 0.f, 1.f, "%.2f");
			//	ImGui::SliderFloat("puddle size", &glb::scene->pEnvironment->m_Puddlesize, 0.f, 1.f, "%.2f");
			//	ImGui::SliderFloat("slippery", &glb::scene->pEnvironment->m_Slippery, 0.f, 1.f, "%.2f");
			//	ImGui::SliderFloat("sky rotation", &glb::scene->pEnvironment->m_SkyboxRot, 0.f, 255.f, "%.2f");
			//	ImGui::Checkbox("Nightlight", &glb::scene->pEnvironment->m_Nightlight);
			//	if (ImGui::Button("Update pEnvironment") && glb::game->State == gameState::ingame) {
			//		glb::oEnvUpdate((uintptr_t)glb::scene->pEnvironment);
			//	}
			//	if (ImGui::Button("Sleep all bodies")) {
			//		for (TDBody* tdb : *(glb::scene->m_Bodies)) {
			//			tdb->countDown = 0x00;
			//			tdb->isAwake = false;
			//		}
			//	}

			//}

			/*if (ImGui::CollapsingHeader("Smoker (buggy)")) {
				if (ImGui::Button("Spawn smoker")) {
					smoker::spawnSmoker();
				}

				ImGui::SliderFloat("Velo X", &smoker::velocity.x, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("Velo Y", &smoker::velocity.y, 0.f, 10.f, "%.2f");
				ImGui::SliderFloat("Velo Z", &smoker::velocity.z, 0.f, 10.f, "%.2f");
				ImGui::SliderFloat("Lifetime", &smoker::lifetime, 0.f, 10.f, "%.2f");

				ImGui::SliderFloat("U01", &smoker::pInfo.U01, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U02", &smoker::pInfo.U02, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U03", &smoker::pInfo.U03, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U04", &smoker::pInfo.U04, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U05", &smoker::pInfo.U05, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U06", &smoker::pInfo.U06, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U07", &smoker::pInfo.U07, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U08", &smoker::pInfo.U08, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U09", &smoker::pInfo.U09, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U10", &smoker::pInfo.U10, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U11", &smoker::pInfo.U11, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U12", &smoker::pInfo.U12, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U13", &smoker::pInfo.U13, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U14", &smoker::pInfo.U14, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U15", &smoker::pInfo.U15, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U16", &smoker::pInfo.U16, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U17", &smoker::pInfo.U17, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U18", &smoker::pInfo.U18, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U19", &smoker::pInfo.U19, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U20", &smoker::pInfo.U20, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U21", &smoker::pInfo.U21, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U22", &smoker::pInfo.U22, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U23", &smoker::pInfo.U23, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U24", &smoker::pInfo.U24, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U25", &smoker::pInfo.U25, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U26", &smoker::pInfo.U26, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U27", &smoker::pInfo.U27, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U28", &smoker::pInfo.U28, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U29", &smoker::pInfo.U29, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U30", &smoker::pInfo.U30, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U31", &smoker::pInfo.U31, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U32", &smoker::pInfo.U32, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U33", &smoker::pInfo.U33, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U34", &smoker::pInfo.U34, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U35", &smoker::pInfo.U35, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U36", &smoker::pInfo.U36, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U37", &smoker::pInfo.U37, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U38", &smoker::pInfo.U38, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U39", &smoker::pInfo.U39, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U40", &smoker::pInfo.U40, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U41", &smoker::pInfo.U41, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U42", &smoker::pInfo.U42, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U43", &smoker::pInfo.U43, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U44", &smoker::pInfo.U44, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U45", &smoker::pInfo.U45, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U46", &smoker::pInfo.U46, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U47", &smoker::pInfo.U47, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U48", &smoker::pInfo.U48, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U49", &smoker::pInfo.U49, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U50", &smoker::pInfo.U50, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U51", &smoker::pInfo.U51, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U52", &smoker::pInfo.U52, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U53", &smoker::pInfo.U53, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U54", &smoker::pInfo.U54, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U55", &smoker::pInfo.U55, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U56", &smoker::pInfo.U56, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U57", &smoker::pInfo.U57, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U58", &smoker::pInfo.U58, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U59", &smoker::pInfo.U59, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U60", &smoker::pInfo.U60, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U61", &smoker::pInfo.U61, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U62", &smoker::pInfo.U62, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U63", &smoker::pInfo.U63, 0.f, 5.f, "%.2f");
				ImGui::SliderFloat("U64", &smoker::pInfo.U64, 0.f, 5.f, "%.2f");
			}*/
		}

		ImGui::EndTabBar();

		ImGui::Separator();

		ImGui::EndChild();
		ImGui::PopStyleVar();


		//ImGui::Text("UI test");
		//sprintf_s(health, 64, "HP: %.4f", glb::player->health);
		//ImGui::Text(health);
		//sprintf_s(position, 64, "X: %.4f Y: %.4f Z: %.4f", glb::player->position.x, glb::player->position.y, glb::player->position.z);
		//ImGui::Text(position);
		//ImGui::Text(glb::player->heldItemName);

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

		//if (nToolgun::currentTool == toolnames::TOOL_DEBUG && nToolgun::holdingToolgun) {
		//	ImGui::Separator();
		//	ImGui::Text("Shape : %p", nToolgun::instance_debug->tShape);
		//	ImGui::Text("Body   : %p", toolgun::dbgObject.tBody);
		//}

		ImGui::End();
	}

	mods::execMods();


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

