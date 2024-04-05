#pragma once
#include "Obj.h"

class CBmpMgr;

class CUI :
    public CObj
{
public:
    CUI();
    virtual ~CUI();

public:
    CUI*                Get_Parent() { return m_pParentUI; }
    void                AddChild(CUI* pUI) { m_vecChildUI.push_back(pUI); pUI->m_pParentUI = this; }

    void                Set_Image(const TCHAR* pImgKey) { m_pFrameKey = pImgKey; }

public:
    virtual void        Initialize() override;
    virtual int         Update() override;
    virtual void        Late_Update() override;
    virtual void        Render(HDC hDC) override;
    virtual void        Release() override;

protected:
    void                Update_Child();
    void                Late_Update_Child();
    void                Render_Child(HDC hDC);

protected:
    vector<CUI*>        m_vecChildUI;
    CUI*                m_pParentUI;

    vector<CUI*>        m_BatteryGageUIList;
    CUI*                m_pInvenUI;
    int                 m_iBulletTimeGage;

};

