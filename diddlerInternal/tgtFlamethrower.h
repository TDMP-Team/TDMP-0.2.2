#pragma once
#include "tgtBase.h"

struct tgt_flamethrower : public tgt {
	float flRadius = 5.f;
	int chance = 50;

	tgt_flamethrower();
	DWORD exec() override;
};