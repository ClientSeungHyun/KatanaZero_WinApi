#pragma once
#include "Bullet.h"

class CBombBulletEffect :
    public CBullet
{
public:
    CBombBulletEffect();
    virtual ~CBombBulletEffect();

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
    DWORD       m_dwRangeTime;
    bool        m_bIsBomb;
};

