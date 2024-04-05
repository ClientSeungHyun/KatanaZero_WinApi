#pragma once
#include "PlayerState.h"

class CPlayerIdle :
    public CPlayerState
{
public:
    CPlayerIdle();
    virtual ~CPlayerIdle();


public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

