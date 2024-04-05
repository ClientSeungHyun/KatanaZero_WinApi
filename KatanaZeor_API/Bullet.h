#pragma once
#include "Obj.h"

class CBullet :
    public CObj
{
public:
    CBullet();
    virtual ~CBullet();
    
public:
    BULLETTYPE    Get_Type() { return m_eType; }
    void          Set_Type(BULLETTYPE eType) { m_eType = eType; }

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


protected:
    BULLETTYPE  m_eType;
};

