#pragma once
#include "Bullet.h"

class CBaseBullet :
    public CBullet
{
public:
    CBaseBullet();
    virtual ~CBaseBullet();

public:
    void    Set_Angle(float fAngle) { m_fAngle = fAngle; }

    float    Get_BulletPower() { return m_fBulletPower; }
    void     Set_BulletPower(float fPower) { m_fBulletPower = fPower; }

    bool    Get_IsReflect() { return m_bIsReflect; }
    void    Set_IsReflect(bool bReflect) { m_bIsReflect = bReflect; }

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_Target(CObj* pTarget);
    void    TimeBakcDone();

private:
    VEC2 m_vTargetPos;

    float m_fAngle;
    float m_fDiagonal;
    float m_fBulletPower;

    bool m_bIsReflect;

    POINT m_tPoint[3];
};

