#pragma once
#include "EnemyState.h"

class CEnemyIdle :
    public CEnemyState
{
public:
    CEnemyIdle();
    virtual ~CEnemyIdle();

public:
    void Initialize() override;
    void Update() override;
    void Release() override;
};

