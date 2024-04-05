#pragma once
#include "BossState.h"

class CBossIdle :
    public CBossState
{
public:
    CBossIdle();
    ~CBossIdle();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

