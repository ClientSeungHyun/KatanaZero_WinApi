#pragma once
#include "Enemy.h"

class CShieldcop :
    public CEnemy
{
public:
    enum CSHIELSTATE{SHIELD_IDLE, SHIELD_RUN, SHIELD_ATTACK, SHIELD_TURN, SHIELD_END};
public:
    CShieldcop();
    virtual ~CShieldcop();

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

    bool            m_bIsKnockback;
    bool            m_bIsChangDir;

    DWORD           m_dwShootTime;
    DWORD           m_dKnockbakcTime;
    DWORD           m_dwChangeDirTime;

    CSHIELSTATE     m_ePrevState;
    CSHIELSTATE     m_eCurState;
};

