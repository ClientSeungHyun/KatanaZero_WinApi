#pragma once
#include "Obj.h"

#include "Memento.h"

class CPlayerFSM;
class CPlayerState;
class CCaretaker;
class CEffect;
class CEnemy;

class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    bool                    Get_IsDead() { return m_bDead; }
    void                    Set_IsDead(bool bIsDead) { m_bDead = bIsDead; }

    bool                    Get_IsGround() { return m_bIsGround; }
    void                    Set_IsGround(bool bIsGround) { m_bIsGround = bIsGround; }

    bool                    Get_IsFall() { return m_bIsFall; }
    void                    Set_IsFall(bool bIsFall) { m_bIsFall = bIsFall; }

    bool                    Get_IsWallGrab() { return m_bIsWallGrab; }
    void                    Set_IsWallGrab(bool bIsWallGrab) { m_bIsWallGrab = bIsWallGrab; }

    bool                    Get_IsHitRaser() { return m_bIsHitLaser; }
    void                    Set_IsHitRaser(bool bHit) { m_bIsHitLaser = bHit; }

    bool                    Get_IsSpecialAttack() { return m_bIsSpecialAttack; }

    void                    Set_IsDownJump(bool bDownJump) { m_bIsDownJump = bDownJump; }

    PLAYERSTATE             Get_State() { return m_eState; }
    void                    Set_State(PLAYERSTATE eState) { m_eState = eState; }

    int                     Get_BulletTimeGage() { return m_iBulletTimeGage; }

    CObj*                   Get_PlayerItem() { return m_pItem; }

    bool                    Get_IsNextStage() { return m_bIsNextStage; }
    void                    Set_IsNextStage(bool bNextStage) { m_bIsNextStage = bNextStage; }

    CPlayerFSM*             Get_PlayerFSM() { return m_pPlayerFSM; }

public:
    virtual void            Initialize()    override;
    virtual int             Update()        override;
    virtual void            Late_Update()   override;
    virtual void            Render(HDC hDC) override;
    virtual void            Release()       override;

    virtual	void		    OnCollisionEnter(CObj* _pOther);
    virtual	void	    	OnCollisionStay(CObj* _pOther);
    virtual	void		    OnCollisionExit(CObj* _pOther);

public:
    void                    BulletTime();
    void                    ThrowItem();
    VEC2                    Calculate_AttackDir();
    void                    Select_BGSong();

    void                    Change_StateHURT();
    void                    Laser_Die();

    void                    TimeBakcDone();
    void                    Init_Player();

private:
    void                    PixelCollisionLeft();
    void                    PixelCollisionRight();
    void                    PixelCollisionBottom();
    void                    PixelCollisionTop();

protected:
    CObj*                   m_pItem;
    CObj*                   m_pGroundEffect;

    int                     m_iHp;
    int                     m_iBulletTimeGage;

    float                   m_fLaserDisappear;

    DWORD                   m_dwBulletTimeLimit;
    DWORD                   m_dwDanceTime;

    bool                    m_bIsGround;
    bool                    m_bIsFall;
    bool                    m_bIsWallGrab;
    bool                    m_bIsNextStage;
    bool                    m_bIsHitLaser;
    bool                    m_bIsDownJump;
    bool                    m_bIsDance;
    bool                    m_bIsSpecialAttack;

    PLAYERSTATE             m_eState;           //플레이어 현재 상태
    CPlayerFSM*             m_pPlayerFSM;       //플레이어 유한 상태 기계

    vector<CObj*>  m_vecShdow;
    vector<CObj*>   m_vecSpecialAttack;
};

