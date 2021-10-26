#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "windows.h"
#include <vector>
#include <time.h>
#include "console.h"
#include <iostream>
#include "Global.h"
#include "noclip.h"
#include <string>
#include "Raycaster.h"
#include <regex>
#include "objectSpawner.h"

std::vector<std::string> splitCommand(std::string target)
{
	std::stringstream ss(target);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	for (std::string& str : vstrings) {
		str = std::string(str.c_str()); //need this or the last arg has random trailing spaces that you cant delete with string.erase

	}

	return vstrings;
}

std::vector<std::string> splitPath(std::string target)
{
	std::replace(target.begin(), target.end(), '/', ' ');
	std::replace(target.begin(), target.end(), '\\', ' ');
	std::stringstream ss(target);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);

	for (std::string& str : vstrings) {
		str = std::string(str.c_str()); //need this or the last arg has random trailing spaces that you cant delete with string.erase

	}

	return vstrings;
}

struct consoleLine {
	std::string text = "";
	DWORD format = MSG_GENERIC;
};

//std::vector<argument> getArgs(char* str, int strSize) {
//	std::vector<argument> responce = {};
//	int lastSplit = 0;
//
//	for (int i = 0; i < strSize - 1; i++) {
//		char c = *(char*)(str + i);
//		if ((c == ' ' && (i - lastSplit) != 0) || i == strSize - 2) {
//			int thisArgSize = i - lastSplit;
//			char* thisArg = new char[(thisArgSize) + 1];
//			memcpy(thisArg, (char*)(str + lastSplit), thisArgSize);
//			thisArg[thisArgSize] = '\0';
//			responce.push_back({ thisArg, thisArgSize });
//			lastSplit = i + 1;
//		}
//	}
//
//	return responce;
//}
//const char* getCommand(char* fullCommand, int strSize) {
//	int len = 0;
//	for (int i = 0; i < strSize - 1; i++) {
//		char c = *(char*)(fullCommand + i);
//		if (c == ' ') {
//			len = i;
//			break;
//		}
//	}
//
//	if (len != 0) {
//		char* response = new char[len + 1];
//		memcpy(response, fullCommand, len);
//		response[len] = '\0';
//		return response;
//	}
//	else {
//		return fullCommand;
//	}
//
//}

struct command {
	std::string commandT = "";
	std::string helpText = "";
	virtual DWORD exec(std::vector<std::string> args) = 0;
};

std::vector<command*> commands = {};
std::vector<consoleLine> consoleItems = {};
std::vector<std::string> sentCommands = {};

struct comm_help : public command {
	comm_help() {
		commandT = "help";
		helpText = "Displays help | Args: None";
	}

	DWORD exec(std::vector<std::string> args) override {
		console::writeConsole("===Help===", MSG_GENERIC);

		for (command* c : commands) {
			console::writeConsole("[" + c->commandT + " | " + c->helpText + "]", MSG_GENERIC);
		}

		return COMM_OK;
	}
};

struct comm_noclip: public command {
	comm_noclip() {
		commandT = "noclip";
		helpText = "Toggles noclip mode | Args: None";
	}

	DWORD exec(std::vector<std::string> args) override {
		noclip::ToggleNoclip();
		if (noclip::inNoclip) {
			console::writeConsole("Noclip enabled", MSG_GENERIC);
		}
		else {
			console::writeConsole("Noclip disabled", MSG_GENERIC);
		}

		return COMM_OK;
	}
};

struct comm_explode : public command {
	comm_explode() {
		commandT = "explode";
		helpText = "Creates an explosion | Args: X, Y, Z, Power";
	}

	DWORD exec(std::vector<std::string> args) override {
		try {
			if (args.size() < 4) {
				return COMM_ERROR_ARGS;
			}

			float X;
			float Y;
			float Z;

			if (args[1] + args[2] + args[3] == "~~~") {
				raycaster::rayData rd = raycaster::castRayPlayer();
				X = rd.worldPos.x;
				Y = rd.worldPos.y;
				Z = rd.worldPos.z;
			}
			else {
				X = std::stof(args[1]);
				Y = std::stof(args[2]);
				Z = std::stof(args[3]);
			}

			float P = std::stof(args[4]);

			td::Vec3 centerpoint = { X, Y, Z };
			glb::TDcreateExplosion((uintptr_t)glb::scene, &centerpoint, P);
			console::writeConsole("Created explosion @ " + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z), MSG_GENERIC);
			return COMM_OK;
		}
		catch(const std::invalid_argument& e){
			return COMM_ERROR_ARGS;
		}
	}
};

struct comm_tp : public command {
	comm_tp() {
		commandT = "tp";
		helpText = "Teleports the player to a location | Args: X, Y, Z";
	}

	DWORD exec(std::vector<std::string> args) override {
		try {
			if (args.size() < 4) {
				return COMM_ERROR_ARGS;
			}

			float X;
			float Y;
			float Z;

			if (args[1] + args[2] + args[3] == "~~~") {
				raycaster::rayData rd = raycaster::castRayPlayer();
				X = rd.worldPos.x;
				Y = rd.worldPos.y;
				Z = rd.worldPos.z;
			}
			else {
				X = std::stof(args[1]);
				Y = std::stof(args[2]);
				Z = std::stof(args[3]);
			}

			if (noclip::inNoclip) {
				glb::player->cameraPosition = { X, Y, Z };
			}
			else {
				glb::player->position = { X, Y, Z };
			}

			console::writeConsole("Teleported to: " + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z), MSG_GENERIC);
			return COMM_OK;
		}
		catch (const std::invalid_argument& e) {
			return COMM_ERROR_ARGS;
		}
	}
};

struct comm_spawn : public command {
	comm_spawn() {
		commandT = "spawn";
		helpText = "Spawns the supplied .vox file where the player is looking | Args: Path";
	}

	DWORD exec(std::vector<std::string> args) override {
		try {
			if (args.size() < 2) {
				return COMM_ERROR_ARGS;
			}

			std::filesystem::path f{ args[1] };

			if (!std::filesystem::exists(f)) {
				console::writeConsole("Path not found: " + args[1], MSG_ERROR);
				return COMM_ERROR_EXEC;
			}

			std::string voxFileExt = args[1].substr(args[1].size() - 4, 4);
			if (voxFileExt != ".vox") {
				console::writeConsole("Invalid path: " + args[1], MSG_ERROR);
				return COMM_ERROR_EXEC;
			}

			raycaster::rayData rd = raycaster::castRayPlayer();

			td::small_string ssPath = td::small_string(args[1].c_str());
			spawner::placeFreeObject(args[1]);
			std::vector<std::string> sPath = splitPath(args[1]);
			console::writeConsole("Spawned " + sPath.back() + " @ " + std::to_string(rd.worldPos.x) + ", " + std::to_string(rd.worldPos.y) + ", " + std::to_string(rd.worldPos.z), MSG_GENERIC);

			return COMM_OK;
		}
		catch (const std::invalid_argument& e) {
			return COMM_ERROR_ARGS;
		}
	}
};

struct comm_health : public command {
	comm_health() {
		commandT = "health";
		helpText = "Sets the players health | Args: Health";
	}

	DWORD exec(std::vector<std::string> args) override {
		try {
			if (args.size() < 2) {
				return COMM_ERROR_ARGS;
			}

			float H = std::stof(args[1]);
			glb::player->health = H;

			console::writeConsole("Set player health to: " + std::to_string(H), MSG_GENERIC);
			return COMM_OK;
		}
		catch (const std::invalid_argument& e) {
			return COMM_ERROR_ARGS;
		}
	}
};

struct comm_clear : public command {
	comm_clear() {
		commandT = "clear";
		helpText = "Clears the console | Args: None";
	}

	DWORD exec(std::vector<std::string> args) override {
		consoleItems.clear();
		return COMM_OK;
	}
};

namespace console {
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
	bool consoleOpen = false;
	bool firstFocus = false;
	char InputBuf[256];
	int commandReversePtr = -1;

	int TextEditCallback(ImGuiInputTextCallbackData* data) {
		//copied from imgui_demo.cpp because i got so fucking pissed off at trying to make this myself that i just could not be fucking bothered.
		//it crashed so fucking much i swear to god. 
		//i cut my fucking finger on my sharp ass keyboard from getting mad. 
		//fuck you cpp, fuck you imgui, fuck you teardown.
		//it still doesnt work properly but i dont fucking care.
		//you fix it. 

		const int prev_history_pos = commandReversePtr;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (commandReversePtr == -1) {
				commandReversePtr = sentCommands.size() - 1;
			}
			else if (commandReversePtr > 0) {
				commandReversePtr--;
			}
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (commandReversePtr != -1){
				if (++commandReversePtr >= sentCommands.size()) {
					commandReversePtr = -1;
				}
			}
		}

		if (prev_history_pos != commandReversePtr)
		{
			const char* history_str = (commandReversePtr >= 0) ? sentCommands[commandReversePtr].c_str() : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str);
		}

		return 0;
	}

	void writeConsole(std::string text, DWORD messageType) {
		char* timeBuffer = new char[25];
		time_t now = time(0);
		struct tm  tstruct;
		localtime_s(&tstruct, &now);
		strftime(timeBuffer, 25, "%X", &tstruct);
		std::string consoleOutput = std::string(timeBuffer) + " | " + text;
		consoleItems.push_back({ consoleOutput, messageType });
		free(timeBuffer);
	}

	void drawConsole() {
		if (commands.size() == 0) {
			commands.push_back(new comm_clear());
			commands.push_back(new comm_help());
			commands.push_back(new comm_noclip());
			commands.push_back(new comm_explode());
			commands.push_back(new comm_health());
			commands.push_back(new comm_tp());
			commands.push_back(new comm_spawn());
		}
		
		if (consoleOpen) {
			ImGui::Begin("Console");
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

			for (consoleLine c : consoleItems) {

				if (c.format == MSG_WARNING) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.67f, 0.0f, 1.0f)); }
				else if (c.format == MSG_ERROR) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); }
				else if (c.format == MSG_BACKGROUND) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); }

				ImGui::TextUnformatted(c.text.c_str());

				if (c.format != MSG_GENERIC) { ImGui::PopStyleColor(); }
			}

			if ((ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
				ImGui::SetScrollHereY(1.0f);
			}

			ImGui::EndChild();
			ImGui::Separator();

			bool reclaimFocus = false;
			if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallback) && InputBuf[0] != 0x00) {
				char* bufferCopy = new char[256];
				memcpy(bufferCopy, InputBuf, 256);
				std::string inputBufferString(bufferCopy, 256);
				console::writeConsole(">" + inputBufferString, MSG_BACKGROUND);
				sentCommands.push_back(inputBufferString);
				commandReversePtr = -1;

				std::vector<std::string> commArgs = splitCommand(inputBufferString);
				bool vaidCommand = false;
				for (command* commandBlock : commands) {
					if ((commandBlock->commandT == commArgs[0].substr(0, commandBlock->commandT.size()))) {
						vaidCommand = true;
						DWORD response = commandBlock->exec(commArgs);
						if (response == COMM_ERROR_ARGS) {
							console::writeConsole("Exec failed (invalid arguments)", MSG_ERROR);
						}
					}
				}

				if (!vaidCommand) {
					console::writeConsole("Invalid command: " + commArgs[0], MSG_WARNING);
				}

				strcpy_s(InputBuf, "");
				FillMemory(InputBuf, 256, 0x00);
	
				reclaimFocus = true;
			}

			if (reclaimFocus || firstFocus) { ImGui::SetKeyboardFocusHere(-1); firstFocus = false; }
		}
	}
}