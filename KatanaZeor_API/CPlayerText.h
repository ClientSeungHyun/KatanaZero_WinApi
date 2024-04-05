#pragma once
#include "Obj.h"

class CPlayerText :
    public CObj
{
public:
    CPlayerText();
    virtual ~CPlayerText();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

