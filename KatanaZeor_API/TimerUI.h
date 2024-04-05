#pragma once
#include "Obj.h"

class CUI;

class CTimerUI :
    public CObj
{
public:
    CTimerUI();
    virtual ~CTimerUI();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    CUI* m_pTimerGageUI;
    
    float   m_iTimeGage;

};

