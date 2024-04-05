#pragma once
#include "Obj.h"

class CMapLaser :
    public CObj
{
public:
    CMapLaser();
    virtual ~CMapLaser();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);
    virtual	void		OnCollisionExit(CObj* _pOther);

private:
    bool                m_IsLaserOn;

    CMyBmp*             m_pUpBmp;
    CMyBmp*             m_pDownBmp;

    DWORD               m_dwOnTime;
};

