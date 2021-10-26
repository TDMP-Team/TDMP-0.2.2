
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Raycaster.h"
#include "drawCube.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"

namespace lidar {
	ImGuiWindowFlags infoBoxFlags;

	float currentOffset = 0.f;
	int currentPtr = 0;
	float records[125] = {};
	float zoom = 10.f;
	bool colour = false;
	bool enabled = true;
	const float pi = 3.1415926535f;

	int resolution = 500;

	ImVec2 getPoint(ImVec2 midpoint, float angle, float rot, float length) {
		float delta_X = cos(angle + rot);
		float delta_Y = sin(angle + rot);
		return { midpoint.x + (delta_X * length), midpoint.y + (delta_Y * length) };
	}

	void drawLidarWindow(td::Vec2 size) {
		ImU32 white32 = ImColor(ImVec4(1.0f, 1.0f, 1.f, 1.0f));
		ImU32 red32 = ImColor(ImVec4(1.0f, 0.0f, 0.f, 1.0f));

		td::Color redT{ 1.f , 0.f, 0.f, 1.f };

		infoBoxFlags = 0;
		infoBoxFlags |= ImGuiWindowFlags_NoMove;
		infoBoxFlags |= ImGuiWindowFlags_NoResize;
		infoBoxFlags |= ImGuiWindowFlags_NoCollapse;
		infoBoxFlags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::SetNextWindowPos(ImVec2(5, size.y + 10));
		ImGui::SetNextWindowSize({ size.x, size.x });
		ImGui::SetNextWindowBgAlpha(0.35f);

		ImGui::Begin("Lidar", (bool*)false, infoBoxFlags);
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 midpoint = { ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() / 2), ImGui::GetWindowPos().y + (ImGui::GetWindowHeight() / 2) };

		currentOffset = 0.f;

		for (int i = 0; i < resolution; i++) {
			RaycastFilter filter{ 0 };
			filter.m_Mask = -1;
			glm::quat camera_rotation_tl = glm::quat(glm::vec3(0, (currentOffset + glb::player->camYaw), 0));
			glm::vec3 raycast_dir_tl = camera_rotation_tl * glm::vec3(0, 0, -1);
			
			raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tl.x, raycast_dir_tl.y, raycast_dir_tl.z }, &filter);
			td::Vec3 target = rd.worldPos;

			float step = ((pi * 2) / resolution);

			currentOffset += step;

			ImVec2 endPoint = getPoint(midpoint, (1.5 * pi) - (step * i), 0.f, rd.distance * zoom);

			if (colour) {
				drawList->AddCircleFilled(endPoint, 1.5f, ImColor(ImVec4(rd.palette.m_Color.m_R, rd.palette.m_Color.m_G, rd.palette.m_Color.m_B, 1.0f)));
			}
			else {
				drawList->AddCircleFilled(endPoint, 1.5f, white32);
			}

		}

		drawList->AddCircleFilled(midpoint, 3.f, red32);
		drawList->AddLine({ midpoint.x - 0.5f, midpoint.y }, { midpoint.x - 0.5f, midpoint.y - 10.f }, red32, 2.f);



		ImGui::End();
	}
}
