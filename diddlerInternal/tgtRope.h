#pragma once
#include "tgtBase.h"
#include "global.h"
#include "types.h"
#include "TDObjects.h"

struct tgt_rope : public tgt {
    float ropeSlack = 0.f;
    float ropeStrength = 1.f;
    float ropeMaxStretch = 0.f;
    td::Color ropeColor = { 1.f, 1.f, 1.f, 1.f };
    bool ropeAttackOnce = true;
    bool ropeToolFirstPos = true;
    td::Vec3 ropeFirstPos = {};
    td::Vec3 ropeSecondPos = {};
    TDShape* shape1 = 0;
    TDShape* shape2 = 0;
    td::Vec3 translatedPoint1 = {};
    td::Vec3 translatedPoint2 = {};
    td::Vec3 worldPos1 = {};
    td::Vec3 worldPos2 = {};

	tgt_rope();
	DWORD exec() override;
};