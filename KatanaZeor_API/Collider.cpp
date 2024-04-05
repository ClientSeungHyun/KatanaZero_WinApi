#include "Collider.h"

#include "Obj.h"

#include "Camera.h"

CCollider::CCollider() :m_pOwner(nullptr), m_bIsActive(true), m_bIsRectCollider(true)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CCollider::~CCollider()
{
}

void CCollider::Late_Update()
{
	m_vRenderPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	VEC2 vObjPos = m_pOwner->Get_Pos();
	m_vPos = vObjPos + m_vOffset;
	m_tRect.left = (LONG)(m_vRenderPos.x - m_vSize.x * 0.5f);
	m_tRect.right = (LONG)(m_vRenderPos.x + m_vSize.x * 0.5f);
	m_tRect.top = (LONG)(m_vRenderPos.y - m_vSize.y * 0.5f);
	m_tRect.bottom = (LONG)(m_vRenderPos.y + m_vSize.y * 0.5f);

}

void CCollider::Render(HDC hDC)
{
	/*if (m_bIsActive)
	{
		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		RECT rcCollider{ m_vRenderPos.x - m_vSize.x * 0.5f, m_vRenderPos.y - m_vSize.y * 0.5f, m_vRenderPos.x + m_vSize.x * 0.5f, m_vRenderPos.y + m_vSize.y * 0.5f };


		FrameRect(hDC, &rcCollider, myBrush);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
	}*/
	
}
