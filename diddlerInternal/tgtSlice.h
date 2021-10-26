#pragma once
#include "tgtBase.h"

struct tgt_slice: public tgt {
	float slicer_maxDist = 10.f;
	int slicer_resolution = 500;
	bool slicerHorizontal = true;

	tgt_slice();
	DWORD exec() override;
};