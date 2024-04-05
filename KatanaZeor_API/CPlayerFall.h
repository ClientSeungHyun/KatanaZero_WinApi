#pragma once
#include "PlayerState.h"

class CPlayerFall :
    public CPlayerState
{
public:
    CPlayerFall();
    virtual ~CPlayerFall();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

