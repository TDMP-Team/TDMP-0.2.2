#pragma once

#include "tgtBase.h"
#include "global.h"

struct tgt_weld : public tgt {
    bool weldStageOne = true;
    bool weldAttackOnce = true;
    float postCompletionHighlight = 1.f;

    TDShape* weldTargetShape1 = 0;
    TDBody* weldTargetBody1 = 0;
    TDShape* weldTargetShape2 = 0;
    TDBody* weldTargetBody2 = 0;
    td::Vec3 weldFirstPos = {};
    td::Vec3 weldSecondPos = {};
    TDBody* postCompletionBody = 0;

	tgt_weld();
	DWORD exec() override;
};