#pragma once
#include "BossState.h"

class CBossBlinkLaser :
    public CBossState
{
public:
    CBossBlinkLaser();
    virtual ~CBossBlinkLaser();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

public:
    vector<VEC2>    m_vTeleportPosList;
    CObj*           m_pLaserBullet[3];

    int             m_iTeleportCount;
    int             m_iLaserCount;
};

