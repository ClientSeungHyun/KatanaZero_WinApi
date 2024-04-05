#pragma once
#include "PlayerState.h"

class CPlayerHurt :
    public CPlayerState
{
public:
    CPlayerHurt();
    virtual ~CPlayerHurt();

public:
    void            Initialize() override;
    void            Update() override;
    void            Release() override;

private:
    bool            m_bIsDead;
};

