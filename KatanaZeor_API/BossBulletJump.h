#pragma once
#include "BossState.h"

class CBossBulletJump :
    public CBossState
{
public:
    CBossBulletJump();
    virtual ~CBossBulletJump();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

public:
    int             m_ShootCount;
    DWORD           m_dwShootDelayTime;

    float           m_fJumpAngle;
    float           m_fGravity;

    bool            m_bIsPreJump;
    bool            m_bIsJump;
    bool            m_bIsWallGrab;
    bool            m_bIsShootEnd;
    bool            m_bIsLand;

    VEC2            m_vWallGrabPoint;
    VEC2            m_vLandingPoint;
};

