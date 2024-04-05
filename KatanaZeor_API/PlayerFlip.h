#pragma once
#include "PlayerState.h"
class CPlayerFlip :
    public CPlayerState
{
public:
    CPlayerFlip();
    virtual ~CPlayerFlip();

public:
    void            Initialize() override;
    void            Update() override;
    void            Release() override;

private:
    float           m_fFlipAngel;
};

