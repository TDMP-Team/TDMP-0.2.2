#include "Raycaster.h"
#include "objectSpawner.h"
#include "maths.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "drawCube.h"
#include "toolgun.h"
#include "Global.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "dotProjector.h"
#include "camera.h"
#include "threadedCamera.h"
#include "TDObjects.h"

#define rndLvl 16

namespace threadCamera {

	std::vector<KMCamera*> gameCameras = {};
	bool runInSeperateThread = true;
	bool stochastic = false;
	float universialFov = 8.f;
	cameraType method = CT_Colour;

	void updateGameCameras() {
		for (threadCamera::KMCamera* kmc : threadCamera::gameCameras) {
			if (kmc->cameraActive) {
				kmc->updateImage();
			}
		}
	}

	float randFloat(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	void drawCameraWndw(KMCamera* camera) {
		if (!camera) { return; }
		//if (camera->isDestroyed()) { return; }

		ImGui::Begin("Camera");
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 posMin = ImGui::GetWindowContentRegionMin();
		ImVec2 posMax = ImGui::GetWindowContentRegionMax();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 adjustedPosMin = ImVec2(windowPos.x + posMin.x, windowPos.y + posMin.y);
		ImVec2 adjustedPosMax = ImVec2(windowPos.x + posMax.x, windowPos.y + posMax.y);
		drawList->AddImage((void*)camera->createImage(), adjustedPosMin, adjustedPosMax);
		ImGui::Text("TIME: %.2f MS", camera->getLastFrameTime());
		ImGui::End();
	}

	int mapToIntRange(float minVal, float maxVal, float thisVal, int minInt, int maxInt) {

		int vec = ((maxVal) - (minVal));
		if (vec <= 0) { vec = 1; }
		int pxVal = 255 - ((thisVal - minVal) * 255 / vec);

		if (pxVal < 0) { pxVal = 0; }
		if (pxVal > 255) { pxVal = 255; }

		return pxVal;
	}

	KMCamera::KMCamera(glm::quat rot, glm::vec3 pos, glm::vec3 forwv, glm::vec3 upv, int resX, int resY) {
		this->cameraActive = false;

		glGenTextures(1, (GLuint*)camTexture);
		resolutionX = resX;
		resolutionY = resY;

		bufferA = new pixel[resolutionX * resolutionY];
		bufferB = new pixel[resolutionX * resolutionY];
		bufferShow = bufferA;
		bufferWrite = bufferB;
		bufferDistances = new float[resolutionX * resolutionY];

		FillMemory(bufferA, (resolutionX * resolutionY) * 4, 0x00);
		FillMemory(bufferB, (resolutionX * resolutionY) * 4, 0x00);

		rotation = rot;
		position = pos;
		forward = forwv;
		up = upv;

		gameCameras.push_back(this);
	}

	void KMCamera::updateCameraSpecs(glm::quat rot, glm::vec3 pos, glm::vec3 forwv, glm::vec3 upv) {

		//std::cout << "RW: " << std::to_string(rot.w) << " RX : " << std::to_string(rot.x) << " RY : " << std::to_string(rot.y) << " RZ : " << std::to_string(rot.z) << std::endl;
		//std::cout << "PX : " << std::to_string(pos.x) << " PY : " << std::to_string(pos.y) << " PZ : " << std::to_string(pos.z) << std::endl;
		//std::cout << "===" << std::endl;

		td::Color blue{ 0.f, 0.f, 1.f, 1.f };

		rotation = rot;
		position = pos;
		forward = forwv;
		up = upv;

		//printf_s("Time delta: %.02f\n", glb::game->m_fElapsedTime);
		//drawCube(math::v3_glm2td(pos), 0.02f, blue);
	}

	void* KMCamera::createImage() {
		//if (cameraDestroyed) {
		//	return 0;
		//}

		glBindTexture(GL_TEXTURE_2D, (GLuint)camTexture);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolutionX, resolutionY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)bufferShow);
		return camTexture;
	}

	void KMCamera::swapBuffers() {
		pixel* tmp = bufferShow;
		bufferShow = bufferWrite;
		bufferWrite = tmp;
	}

	void KMCamera::setResolution(int X, int Y) {
		nextResolutionX = X;
		nextResolutionY = Y;
		bufferUpdateNeeded = true;
	}

	std::chrono::high_resolution_clock execTimer;
	std::chrono::steady_clock::time_point FRAMESTART;
	std::chrono::steady_clock::time_point FRAMEEND;

	float KMCamera::getLastFrameTime() {
		return lastFrameExecTime;
	}

	float KMCamera::getFps() {
		return (1000.f / lastFrameExecTime);
	}

	void KMCamera::destroy() {
		cameraDestroyed = true;
	}
	
	bool KMCamera::isDestroyed() {
		return cameraDestroyed;
	}

	float KMCamera::updateImage() {
		rcf.m_RejectTransparent = true;

		td::Color blue{ 1.f, 0.f, 0.f, 1.f };
		//drawCube(math::v3_glm2td(position), 0.02f, blue);

		float pxSizeX = (universialFov / resolutionX);
		float pxSizeY = (universialFov / resolutionY);
		int pxPointer = 0;

		if (bufferUpdateNeeded) {
			resolutionX = nextResolutionX;
			resolutionY = nextResolutionX;
			bufferUpdateNeeded = false;

			delete(bufferA);
			delete(bufferB);
			bufferA = new pixel[resolutionX * resolutionY];
			bufferB = new pixel[resolutionX * resolutionY];
			bufferShow = bufferA;
			bufferWrite = bufferB;
			bufferDistances = new float[resolutionX * resolutionY];

			FillMemory(bufferA, (resolutionX * resolutionY) * 4, 0x00);
			FillMemory(bufferB, (resolutionX * resolutionY) * 4, 0x00);
		}

		if (cameraDestroyed) {
			FRAMESTART = execTimer.now();
			DCF++;
			for (int y = 0; y < resolutionY; y++) {
				//byte rLineShade = (byte)(rand() % 255);
				for (int x = resolutionX; x > 0; x--) {
					byte shade = (byte)(rand() % 128);
					bufferWrite[pxPointer] = { shade, shade, shade, 0xFF };
					pxPointer++;
				}
			}

			swapBuffers();
			FRAMEEND = execTimer.now();
			lastFrameExecTime = (std::chrono::duration_cast<std::chrono::microseconds>(FRAMEEND - FRAMESTART).count() / 1000.f);
			return lastFrameExecTime;
		}
		else {
			DCF = 0;
		}

		FRAMESTART = execTimer.now();
		raycast_dir_bl = rotation * forward;
		raycaster::rayData rd = raycaster::castRayManual(math::v3_glm2td(position), math::v3_glm2td(raycast_dir_bl), &rcf);
		glTarget = glm::vec3(rd.worldPos.x, rd.worldPos.y, rd.worldPos.z);
		vmatrix = glm::lookAt(position, glTarget, up);
		pmatrix = glm::perspective(50.f, 1.f, 1.f, 1000.f);
		invProjMat = glm::inverse(pmatrix);
		invViewMat = glm::inverse(vmatrix);

		float frameMaxDist = 0;
		float frameMinDist = INT32_MAX;

		for (int y = 0; y < resolutionY; y++) {
			for (int x = resolutionX; x > 0; x--) {
				float xNoise = 0;
				float yNoise = 0;

				if (stochastic) {
					xNoise = randFloat(-(pxSizeX / 3.f), (pxSizeX / 3.f));
					yNoise = randFloat(-(pxSizeY / 3.f), (pxSizeY / 3.f));
				}

				float comX = (universialFov / 2.f) - ((x * pxSizeX) - (pxSizeX / 2.f)) + xNoise;
				float comY = (universialFov / 2.f) - ((y * pxSizeY) + (pxSizeY / 2.f)) + yNoise;

				glm::vec2 ray_nds = glm::vec2(comX, comY);
				glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
				glm::vec4 eyeCoords = invProjMat * ray_clip;
				eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

				glm::vec4 rayWorld = invViewMat * eyeCoords;
				glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

				raycaster::rayData rd = raycaster::castRayManual(math::v3_glm2td(position), math::v3_glm2td(rayDirection), &rcf);

				if (rd.distance > 0.f && rd.distance < 1000.f && rd.distance < frameMinDist) {
					frameMinDist = rd.distance;
				}
				if (rd.distance > 0.f && rd.distance < 1000.f && rd.distance > frameMaxDist) {
					frameMaxDist = rd.distance;
				}

				int iColourR = 0;
				int iColourG = 0;
				int iColourB = 0;

				if (rd.successful && rd.distance < 1000.f) {
					if (threadCamera::method == CT_Colour) {
						if (rd.distance > 0.f) {
							int iThisDist = (rd.distance) / 2;
							int iPxNoise = rand() % rndLvl;
							iColourR = (iPxNoise + (rd.palette.m_Color.m_R * 255) - iThisDist);
							if (iColourR < 0) { iColourR = 0; }
							if (iColourR > 255) { iColourR = 255; }
							iColourG = (iPxNoise + (rd.palette.m_Color.m_G * 255) - iThisDist);
							if (iColourG < 0) { iColourG = 0; }
							if (iColourG > 255) { iColourG = 255; }
							iColourB = (iPxNoise + (rd.palette.m_Color.m_B * 255) - iThisDist);
							if (iColourB < 0) { iColourB = 0; }
							if (iColourB > 255) { iColourB = 255; }
						}
						else {
							iColourR = 0;
							iColourG = 77;
							iColourB = 77;
						}

						bufferWrite[pxPointer] = { (byte)iColourR,(byte)iColourG, (byte)iColourB, 0xFF };
					}
					else if (threadCamera::method == CT_Monochrome) {
						int iThisDist = (rd.distance) / 2;
						int iPxNoise = rand() % rndLvl;
						iColourR = (iPxNoise + (rd.palette.m_Color.m_R * 255) - iThisDist);
						if (iColourR < 0) { iColourR = 0; }
						if (iColourR > 255) { iColourR = 255; }
						iColourG = (iPxNoise + (rd.palette.m_Color.m_G * 255) - iThisDist);
						if (iColourG < 0) { iColourG = 0; }
						if (iColourG > 255) { iColourG = 255; }
						iColourB = (iPxNoise + (rd.palette.m_Color.m_B * 255) - iThisDist);
						if (iColourB < 0) { iColourB = 0; }
						if (iColourB > 255) { iColourB = 255; }

						int avg = (iColourR + iColourG + iColourB) / 3;
						iColourR = iColourG = iColourB = avg;

						bufferWrite[pxPointer] = { (byte)iColourR,(byte)iColourG, (byte)iColourB, 0xFF };
					}
				}
				else {
					int iPxNoise = rand() % rndLvl;
					bufferWrite[pxPointer] = {(byte)iPxNoise, (byte)(iPxNoise + 77), (byte)(iPxNoise + 77), 0xFF };
				}
				

				bufferDistances[pxPointer] = rd.distance;

				pxPointer++;
			}
		}

		//printf_s("Min: %0.2f, Max: %0.2f\n", frameMinDist, frameMaxDist);

		pxPointer = 0;
		if (threadCamera::method == CT_Dist) {
			for (int y = 0; y < resolutionY; y++) {
				for (int x = resolutionX; x > 0; x--) {
					float thisDist = bufferDistances[pxPointer];
					int thisVal = mapToIntRange(frameMinDist, frameMaxDist, thisDist, 0, 255);
					bufferWrite[pxPointer] = { (byte)thisVal,(byte)thisVal, (byte)thisVal, 0xFF };
					pxPointer++;
				}
			}
		}

		swapBuffers();
		FRAMEEND = execTimer.now();
		lastFrameExecTime = (std::chrono::duration_cast<std::chrono::microseconds>(FRAMEEND - FRAMESTART).count() / 1000.f);
		Sleep(1);
		return lastFrameExecTime;
	}

}