#pragma once
#include "Global.h"
#include "windows.h"
#include <vector>
#include <string>
#include "types.h"
#include "glm/glm.hpp"

namespace wireObjects {

	class wireNode;
	class wireObj;

	/*
	Each wire object can have multiple nodes.
	Nodes must authenticate with eachother and both agree they are connected to eachother.
	Each node can only have one connection.
	Nodes are bi-directional, each one can read its connected partners int value. 
	Numeric nodes can hold any intiger value, bool nodes will only hold 0 or 1.
	All nodes are both inputs and outputs, though each object can specify how its nodes are used.
	Each node has one public int called "memory" that can be read from and written to outside of the nodes exec function. 
	*/

	enum class nodeType {
		NT_Bool,
		NT_Numeric
	};

	enum class nodeResponse {
		NR_Ok,
		NR_Busy,
		NR_TypeMissmatch,
		NR_Pebcak,
		NR_Generic,
	};

	//Each node is assigned a colour, this is the colour of each voxel on the physical wire object
	enum class nodeColour : int {
		Red = 0,		//255, 000, 000
		Green = 1,		//000, 255, 000
		Blue = 2,		//000, 000, 255
		Yellow = 3,		//255, 255, 051
		Magenta = 4,	//255, 000, 255
		Cyan = 5,		//000, 255, 255
		Black = 6,		//050, 050, 050
		White = 7		//255, 255, 255
	};

	static td::Color nodeColourActive[] = {
		{255, 000, 000, 255}, 
		{000, 255, 000, 255},
		{000, 255, 255, 255},
		{255, 255, 051, 255},
		{255, 000, 255, 255},
		{000, 255, 255, 255},
		{050, 050, 050, 255},
		{255, 255, 255, 255}
	};

	class wireNode {
		nodeType type;
		int value = 0;
		wireObj* parent = 0;
		wireNode* connection = 0;
		nodeColour colour;
		bool activeConnection = false;
		bool isInit = false;
		glm::vec3 relPosition = { 0, 0, 0 };

	public:
		nodeType getType() { return type; }
		nodeColour getColour() { return colour; }
		glm::vec3 getPosition() { return relPosition; }
		int getValue() { return value; }
		int setValue(int val) { value = val; return val; }

		void dispose() {
			this->disconnect();
			delete(this);
		}

		bool init(wireObj* parent, nodeType type, nodeColour colour, glm::vec3 position){
			if (!isInit) {
				this->parent = parent;
				this->type = type;
				this->relPosition = position;
				this->colour = colour;
				this->isInit = true;
				return true;
			}
			return false;
		}

		int getClientValue() {
			if (connection && activeConnection) {
				return connection->getValue();
			}
			else {
				return 0;
			}
		}

		nodeResponse connect(wireNode* client) {
			if (activeConnection) {
				return wireObjects::nodeResponse::NR_Busy;
			}
			else if (client == this) {
				return wireObjects::nodeResponse::NR_Pebcak;
			}
			else if (client->getType() != this->getType()) {
				return wireObjects::nodeResponse::NR_TypeMissmatch;
			}
			else {
				connection = client;
				return wireObjects::nodeResponse::NR_Ok;
			}
		}

		void disconnect() {
			if (connection && activeConnection) {
				activeConnection = false;
				connection = 0;
				connection->disconnect();
			}
		}
	};

	nodeResponse connectNodes(wireNode* nodeA, wireNode* nodeB);

	class wireObj {
	public:
		bool destroyed = false;
		TDShape* housing = 0;
		int memory = 0;
		std::vector<wireNode*> nodes = {};

		bool getClosestNode(glm::vec3 worldPos, wireNode** out);
		DWORD drawNodes(wireNode* selected = 0);
		virtual DWORD init(TDShape* housing, int memoryVal) = 0;	//run when the object is created
		virtual DWORD exec() = 0;									//run every frame

		void disconnectAll() {
			for (wireNode* cNode : nodes) {
				cNode->disconnect();
			}
		}

		void dispose() {
			destroyed = true;
			for (wireNode* cNode : nodes) {
				cNode->dispose();
			}
			delete(this);
		}
	};
}