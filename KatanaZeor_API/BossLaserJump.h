#pragma once
#include "BossState.h"

class CBossLaserJump :
    public CBossState
{
    // CBossState��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Release() override;
};

