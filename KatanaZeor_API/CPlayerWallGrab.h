#pragma once
#include "PlayerState.h"

class CPlayerWallGrab :
    public CPlayerState
{
public:
    CPlayerWallGrab();
    virtual ~CPlayerWallGrab();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

private:
    VEC2        m_vStartPos;
};

