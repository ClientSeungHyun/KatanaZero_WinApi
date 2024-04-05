#pragma once
#include "Enemy.h"

class CEffect;

class CPomp :
    public CEnemy
{
public:
    CPomp();
    virtual ~CPomp();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);

private:
    CEffect*        m_pBounceEffect;
    DWORD           m_dwAttackTime;
    DWORD           m_dwBounceTime;

    bool            m_bIsBounceAble;
    bool            m_bISBounce;
};

