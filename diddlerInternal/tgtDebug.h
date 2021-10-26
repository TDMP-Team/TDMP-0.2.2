#pragma once
#include "tgtBase.h"

struct tgt_debug: public tgt {
	tgt_debug();
	DWORD exec() override;
};