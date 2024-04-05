#pragma once
#include "UI.h"

class CBaseUI :
    public CUI
{
public:
    CBaseUI();
    virtual ~CBaseUI();

public:
    virtual void        Initialize() override;
    virtual int         Update() override;
    virtual void        Late_Update() override;
    virtual void        Render(HDC hDC) override;
    virtual void        Release() override;


};

