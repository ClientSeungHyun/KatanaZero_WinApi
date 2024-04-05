#pragma once
#include "BossState.h"

class CBossDie :
    public CBossState
{
public:
    CBossDie();
    virtual ~CBossDie();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

private:
    bool            m_bDieGround;
    bool            m_bSneakGround;
};

