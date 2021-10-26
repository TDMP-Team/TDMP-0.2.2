#pragma once
#include "tgtBase.h"

struct tgt_leafblower : public tgt {
	tgt_leafblower();
	DWORD exec() override;
};