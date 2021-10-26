#pragma once
#include "tgtBase.h"

struct tgt_remove : public tgt {
	bool spawnOnce = true;

	tgt_remove();
	DWORD exec() override;
};