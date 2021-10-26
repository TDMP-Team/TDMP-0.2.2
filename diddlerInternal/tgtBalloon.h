#pragma once
#include "tgtBase.h"

struct tgt_balloon : public tgt {
	float balloonPower = 10.f;
	bool spawnOnce = true;

	tgt_balloon();
	DWORD exec() override;
};