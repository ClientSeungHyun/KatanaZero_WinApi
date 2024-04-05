#pragma once
#include "Bullet.h"

class CBomoBullet :
    public CBullet
{
public:
    CBomoBullet();
    virtual ~CBomoBullet();

public:
    void        Set_DelayTime(int iDelay) { m_iDelayTime = iDelay; }
    void        Set_MoveTime(DWORD dwTime) { m_dwMoveTime = dwTime; }

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);

private:
    float       m_fRandomAngle;
    float       m_fRandomPowerX;
    float       m_fRandomPowerY;

    int         m_iDelayTime;
    DWORD       m_dwMoveTime;

    DWORD       m_dwDestroyTime;
    bool        m_bIsCreate;
};

