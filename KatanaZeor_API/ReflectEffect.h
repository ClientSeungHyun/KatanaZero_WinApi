#pragma once
#include "Obj.h"
class CReflectEffect :
    public CObj
{
public:
    CReflectEffect();
    virtual ~CReflectEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


};

