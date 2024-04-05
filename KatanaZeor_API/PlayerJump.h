#pragma once
#include "PlayerState.h"

class CEffect;

class CPlayerJump :
    public CPlayerState
{
public:
    CPlayerJump();
    virtual ~CPlayerJump();
    
public:
    void Initialize() override;
    void Update() override;
    void Release() override;

private:
    float           m_fJumpMax;
    float           m_fJumpSpeed;
    float           m_fStartPosY;
    float           m_fMaxPosY;

    DWORD           m_dwJumpTime;

    CObj*           m_pJumpEffect;

};

