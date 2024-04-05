#pragma once
#include "Obj.h"

class CEffect :
    public CObj
{
public:
    CEffect();
    virtual ~CEffect();

public:
    EFFECTID        Get_EfeectId() { return m_eEffectID; }
    void            Set_EffectId(EFFECTID eID) { m_eEffectID = eID; }

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);

private:

    EFFECTID        m_eEffectID;
};

