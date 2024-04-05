#pragma once
#include "PlayerState.h"

class CPlayerRoll :
    public CPlayerState
{
public:
    CPlayerRoll();
    virtual ~CPlayerRoll();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

