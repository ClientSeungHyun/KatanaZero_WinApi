#pragma once
#include "Obj.h"

class CHitEffect :
    public CObj
{
public:
    CHitEffect();
    virtual ~CHitEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_Angle(float fAngle);

private:
    POINT		m_tPoint[3];

    VEC2        m_vLookDir;

    float       m_fEffectAngle;
    float       m_fEffectSpeed;

    float       m_fDirAngle;

    DWORD		m_dwDeadTime;
    CObj*       m_pPlayer;
};

