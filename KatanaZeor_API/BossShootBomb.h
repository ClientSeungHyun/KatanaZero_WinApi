#pragma once
#include "BossState.h"

class CBossShootBomb :
    public CBossState
{
public:
    CBossShootBomb();
    virtual ~CBossShootBomb();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

private:
    vector<CObj*>     m_BossBulletList;

    int             m_iDelayTime;
    int             m_iShootCount;

    bool            m_iMoveforRender;
    bool            m_bIsShooting;
};

