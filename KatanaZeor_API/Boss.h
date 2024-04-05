#pragma once
#include "Obj.h"

class CBossFSM;
class CBossState;

class CBoss :
    public CObj
{
public:
    CBoss();
    virtual ~CBoss();

public:
    BOSSSTATE           Get_State() { return m_eState; }
    void                Set_State(BOSSSTATE eState) { m_eState = eState; }

    int                 Get_Hp() { return m_iHp; }
    void                Set_Hp(int iHp) { m_iHp = iHp; }

public:
    void                Initialize() override;
    int                 Update() override;
    void                Late_Update() override;
    void                Render(HDC hDC) override;
    void                Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);
    virtual	void		OnCollisionExit(CObj* _pOther);

public:
    void                    Init_Boss();
    void                    Boss_Hit();
    void                    Boss_Die();
    void                    TimeBakcDone();

    void                    Create_Enemy();

private:
    int                 m_iHp;

    bool                m_bIsRenderEffect;
    bool                m_bIsBlood;

    BOSSSTATE           m_eState;  
    CBossFSM*           m_pBossFSM;      
};

