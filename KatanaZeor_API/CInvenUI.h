#pragma once
#include "Obj.h"
class CInvenUI :
    public CObj
{
public:
    CInvenUI();
    virtual ~CInvenUI();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

