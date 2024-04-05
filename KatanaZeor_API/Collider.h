#pragma once
#include "Define.h"

class CObj;
class CEnemy;

class CCollider
{
public:
    CCollider();
    ~CCollider();

private:
    CObj* m_pOwner;

public:
    CObj*           Get_Owner() { return m_pOwner; };

    VEC2            Get_Pos() { return m_vPos; }
    void            Set_Pos(VEC2 vPos) { m_vPos = vPos; }

    VEC2            Get_RenderPos() { return m_vRenderPos; }

    VEC2            Get_Size() { return m_vSize; }
    void            Set_Size(VEC2 vSize) { m_vSize = vSize; }

    VEC2            Get_Offset() { return m_vOffset; }
    void            Set_Offset(VEC2 vOffset) { m_vOffset = vOffset; }

    RECT*           Get_Rect() { return &m_tRect; }

    bool            Get_IsActive() { return m_bIsActive; }
    void            Set_IsActive(bool bActive) { m_bIsActive = bActive; }

    bool            Get_IsRectCollider() { return m_bIsRectCollider; }
    void            Set_IsRectCollider(bool bRect) { m_bIsRectCollider = bRect; }

public:
    void            Late_Update();
    void            Render(HDC hDC);

public:
    friend class CObj;
    friend class CEnemy;

private:
    VEC2            m_vPos;
    VEC2            m_vSize;
    VEC2            m_vOffset;

    VEC2            m_vRenderPos;

    RECT            m_tRect;

    bool            m_bIsActive;
    bool            m_bIsRectCollider;
};

