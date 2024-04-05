#pragma once
#include "PlayerState.h"

class CPlayerAttack :
    public CPlayerState
{
public:
    CPlayerAttack();
    virtual ~CPlayerAttack();

public:
    void            Initialize() override;
    void            Update() override;
    void            Release() override;

public:
    VEC2            Calculate_AttackDir();

private:
    float           m_fAttackPower;
    VEC2            m_vAttackDir;

    int             m_iAttackCount;

    CObj*        m_pEffect;
};

