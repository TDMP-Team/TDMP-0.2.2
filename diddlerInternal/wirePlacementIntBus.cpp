#include "wireObjectBase.h"
#include "wirePlacementIntBus.h"
#include "drawCube.h"

namespace wireObjects {
	wirePlacementIntBus::wirePlacementIntBus() {

	}

	DWORD wirePlacementIntBus::init(TDShape* housing, int memoryVal) {
		this->housing = housing;
		this->memory = memoryVal;

		wireNode* _yellow = new wireNode();
		_yellow->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Yellow, { 0.25f, 0.35f, 0.05f });
		this->nodes.push_back(_yellow);

		wireNode* _red = new wireNode();
		_red->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Red, { 0.05f, 0.15f, 0.05f });
		this->nodes.push_back(_red);

		wireNode* _green = new wireNode();
		_green->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Green, { 0.05f, 0.35f, 0.05f });
		this->nodes.push_back(_green);

		wireNode* _blue = new wireNode();
		_blue->init(this, wireObjects::nodeType::NT_Numeric, wireObjects::nodeColour::Blue, { 0.05f, 0.55f, 0.05f });
		this->nodes.push_back(_blue);

		return 0x01;
	}

	DWORD wirePlacementIntBus::exec() {
		if (!housing || housing->isBroken) { this->disconnectAll(); this->destroyed = true; }
		if (this->destroyed) { return 0x00; }

		int inputValue = nodes[0]->getClientValue(); //get the value on YELLOW
		nodes[1]->setValue(inputValue); //set YELLOW value on RED
		nodes[2]->setValue(inputValue); //set YELLOW value on GREEN
		nodes[3]->setValue(inputValue); //set YELLOW value on BLUE

		return 0x01;
	}
}