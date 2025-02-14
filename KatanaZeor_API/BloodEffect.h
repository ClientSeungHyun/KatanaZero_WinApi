#pragma once
#include "Obj.h"
class CBloodEffect :
    public CObj
{
public:
    CBloodEffect();
    virtual ~CBloodEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

