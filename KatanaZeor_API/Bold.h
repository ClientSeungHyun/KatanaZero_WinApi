#pragma once
#include "Enemy.h"

class CBold :
    public CEnemy
{
public:
    CBold();
    virtual ~CBold();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);

private:
    CObj*           m_pBullet[3];
    int             m_iBulletCount;

    DWORD           m_dwShootTime;
};

