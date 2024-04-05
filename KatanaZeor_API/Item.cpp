#include "Item.h"
#include "Collider.h"

CItem::CItem() :m_bIsInPlayer(false), m_bIsThrow(false), m_fThrowSpeed(15.f), m_eItemType(ITEM_END)
{
    ZeroMemory(&m_tPoint, sizeof(POINT));
}

CItem::~CItem()
{
}

void CItem::Initialize()
{
}

int CItem::Update()
{
	return 0;
}

void CItem::Late_Update()
{
}

void CItem::Render(HDC hDC)
{
}

void CItem::Release()
{
}

void CItem::OnCollisionEnter(CObj* _pOther)
{
}

void CItem::OnCollisionStay(CObj* _pOther)
{
}

void CItem::OnCollisionExit(CObj* _pOther)
{
}

void        CItem::Throw()
{
    if (m_bIsThrow)
    {
        m_fAngle += 20.f;
        m_pCollider->Set_IsActive(true);

        m_vPos.x += m_vLookDir.x * m_fThrowSpeed * CTimeMgr::Get_Instance()->Get_TimeScale();
        m_vPos.y += m_vLookDir.y * m_fThrowSpeed * CTimeMgr::Get_Instance()->Get_TimeScale();

        // аб ╩С╢э
        m_tPoint[0].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);
        m_tPoint[0].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);

        // ©Л ╩С╢э		     
        m_tPoint[1].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);
        m_tPoint[1].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);

        // аб го╢э		    
        m_tPoint[2].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
        m_tPoint[2].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
    }

}

void CItem::TimeBakcDone()
{
    m_bDead = false;
    m_fAngle = 0.f;
    m_bIsActive = true;
    m_bIsThrow = false;
    m_pCollider->Set_IsActive(true);

    // аб ╩С╢э
    m_tPoint[0].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);
    m_tPoint[0].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);

    // ©Л ╩С╢э		     
    m_tPoint[1].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);
    m_tPoint[1].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);

    // аб го╢э		    
    m_tPoint[2].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
    m_tPoint[2].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
}
