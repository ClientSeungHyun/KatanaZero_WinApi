#pragma once
#include "Obj.h"

class CBatterUI :
    public CObj
{
public:
    CBatterUI();
    virtual ~CBatterUI();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    vector<CUI*>        m_BatteryGageUIList;
    int                 m_iBulletTimeGage;
};

