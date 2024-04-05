#pragma once
#include "Obj.h"

class CEnemy :
    public CObj
{
public:
    CEnemy();
    virtual ~CEnemy();

public:
    CCollider*  Get_DetectCollider() { return m_pDetectCollider; }
    bool        Get_IsAttack() { return m_bIsAttack; }
    ENEMYTYPE   Get_Type() { return m_eType; }

    virtual void		Set_Pos(float x, float y) { m_vPos.x = x; m_vPos.y = y;   }
    void                Set_StartPos(float x, float y){ m_vStartPos.x = x; m_vStartPos.y = y; }
    void                Set_IsPatrol(bool bPatrol) { m_bIsPatrol = bPatrol; }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);
    virtual	void		OnCollisionExit(CObj* _pOther);

public:
    virtual void        EnemyDie();

    void                Create_DetectCollider();
    void		        PixelCollisionBottom();

    void                TimeBakcDone();

protected:
    CObj*               m_pTarget;
    CCollider*          m_pDetectCollider;  //�÷��̾� ������ �ݶ��̴�
    CObj*               m_pBlood;

    VEC2                m_vStartPos;        //set ���� ���߿� �� �밡��

    DWORD               m_dwDetectEffectTime;

    int                 m_iHp;
    float               m_fPatrolDistance;

    bool                m_bIsGround;
    bool                m_bIsPatrol;
    bool                m_bIsFind;
    bool                m_bIsAttack;
    bool                m_bIsAttacking;
    bool                m_bIsChase;
    bool                m_bIsBlood;

    ENEMYTYPE           m_eType;
    ENEMYSTATE          m_eState;

};

