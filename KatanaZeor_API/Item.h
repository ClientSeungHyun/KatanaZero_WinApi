#pragma once
#include "Obj.h"
class CItem :
    public CObj
{
public:
    CItem();
    virtual ~CItem();

public:
    ITEMTYPE        Get_ItemType() { return m_eItemType; }
    void        Set_IsThrow(bool bThrow) { m_bIsThrow = bThrow; }

public:
    void            Initialize() override;
    int             Update() override;
    void            Late_Update() override;
    void            Render(HDC hDC) override;
    void            Release() override;

    virtual	void		OnCollisionEnter(CObj* _pOther);
    virtual	void		OnCollisionStay(CObj* _pOther);
    virtual	void		OnCollisionExit(CObj* _pOther);

public:
    void            Throw();
    void            TimeBakcDone();

protected:
    ITEMTYPE        m_eItemType;

    bool            m_bIsInPlayer;
    bool            m_bIsThrow;

    float           m_fThrowSpeed;
    float           m_fAngle;
    float           m_fDiagonal;

    POINT           m_tPoint[3];
};

