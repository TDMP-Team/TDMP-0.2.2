#pragma once
#include "tgtBase.h"

struct tgt_damage : public tgt {
	float maxRange = 100.f;
	float holeSize = 0.2f;
	bool fireConst = true;
	bool fireOnceActive = true;
	int holesPerFrame = 10;
	float flRadius = 8.f;
	int res = 10;

	tgt_damage();
	DWORD exec() override;
};