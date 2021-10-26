#pragma once
#include "wireObjectBase.h"

namespace wireObjects {
	class wirePlacementIntBus : public wireObj {
	public:

		wirePlacementIntBus();
		DWORD init(TDShape* housing, int memoryVal) override;
		DWORD exec() override;
	};
}