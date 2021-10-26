#pragma once

#include "tgtBase.h"
#include "global.h"

struct tgt_explode : public tgt {
    int bulletCount = 1;
    float explosionFov = 0.f;
    float explosionPower = 2.f;

    tgt_explode();
    DWORD exec() override;
};