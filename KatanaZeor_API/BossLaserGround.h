#pragma once
#include "BossState.h"

class CBossLaserGround :
    public CBossState
{
public:
    CBossLaserGround();
    virtual ~CBossLaserGround();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

private:
    CObj*   m_pLaserBullet;
    VEC2    m_vEffectPos;

    DWORD   m_dwAimTime;
    DWORD   m_dwShootTime;

};

