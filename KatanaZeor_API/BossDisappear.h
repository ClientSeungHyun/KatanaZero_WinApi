#pragma once
#include "BossState.h"

class CBossDisappear :
    public CBossState
{
public:
    CBossDisappear();
    virtual ~CBossDisappear();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

public:
    void Choice_Pattern();

private:
    VEC2        m_vRightPos;
    VEC2        m_vLeftPos;

    BOSSSTATE   m_eNextState;

    DWORD       m_dwAppearTime;
};

