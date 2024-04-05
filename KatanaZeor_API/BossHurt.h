#pragma once
#include "BossState.h"

class CBossHurt :
    public CBossState
{
public:
    CBossHurt();
    virtual ~CBossHurt();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;

private:
    DWORD   m_dwDisappearTime;
};

