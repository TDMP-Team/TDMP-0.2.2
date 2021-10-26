#pragma once
#include "tgtBase.h"

struct tgt_dev : public tgt {
	bool spawnOnce = true;

	tgt_dev();
	DWORD exec() override;
};