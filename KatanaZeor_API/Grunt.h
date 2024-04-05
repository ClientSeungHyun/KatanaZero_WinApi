#pragma once
#include "Enemy.h"

class CGrunt :
    public CEnemy
{
public:
    CGrunt();
    virtual ~CGrunt();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);

private:
    CObj*           m_pAttactEffect;

    DWORD           m_dwAttackTime;
};

