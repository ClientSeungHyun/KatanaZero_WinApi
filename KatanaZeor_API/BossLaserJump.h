#pragma once
#include "BossState.h"

class CBossLaserJump :
    public CBossState
{
    // CBossState을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
};

