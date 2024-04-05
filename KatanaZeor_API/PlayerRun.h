#pragma once
#include "PlayerState.h"

class CPlayerRun :
    public CPlayerState
{
public:
    CPlayerRun();
    virtual ~CPlayerRun();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

