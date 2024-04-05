#pragma once
#include "Obj.h"
class CBloodBGEffect :
    public CObj
{
public:
    CBloodBGEffect();
    virtual ~CBloodBGEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

