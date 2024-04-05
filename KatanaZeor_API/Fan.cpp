#include "Fan.h"

#include "Collider.h"
#include "RigidBody.h"
#include "Camera.h"

#include "Player.h"
#include "BmpMgr.h"

CFan::CFan()
{
    m_eRenderId = BACKGROUND;
}

CFan::~CFan()
{
	Release();
}

void CFan::Initialize()
{
    Create_Collider();

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/Fan.bmp", L"Fan", VEC2(72, 190), VEC2(72, 190), VEC2(0, 0), 31, 10, GetTickCount());
	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Fan");
	m_vLookDir = { 1.0f,0.f };
	Set_Frame();

    m_eId = OBJ_TRAP;
}

int CFan::Update()
{
	if (1 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart <= 31)
	{
		m_pCollider->Set_IsActive(false);
	}
	else
	{
		m_pCollider->Set_IsActive(true);
	}

	Move_Frame();

    return 0;
}

void CFan::Late_Update()
{
    m_pCollider->Late_Update();

    m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
    m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));
}

void CFan::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hMemDC = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // ���� ���� ��ġ�� left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // ���� ���� ��ġ�� top
		(int)m_pAniBmp->Get_ImageSize().x,			 // ���� ���� ���� ����
		(int)m_pAniBmp->Get_ImageSize().y,			 // ���� ���� ���� ����
		hMemDC,					// ��Ʈ���� ������ �ִ� dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// ����� ��Ʈ���� ���� x��ǥ
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// ����� ��Ʈ���� ���� y��ǥ
		(int)m_pAniBmp->Get_ImageSize().x,			// ����� ��Ʈ���� ���� ������
		(int)m_pAniBmp->Get_ImageSize().y,			// ����� ��Ʈ���� ���� ������
		RGB(255, 255, 255));	// ������ �ȼ��� ���� ��

	m_pCollider->Render(hDC);
}

void CFan::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);
}

void CFan::OnCollisionEnter(CObj* _pOther)
{
	_pOther->Get_RigidBody()->AddForce(VEC2(-900000.f, -50.f));
	_pOther->Get_RigidBody()->Set_VelocityX(0.f);

	if (_pOther->Get_Id() == OBJ_PLAYER)
	{
		dynamic_cast<CPlayer*>(_pOther)->Change_StateHURT();
	}
}

void CFan::OnCollisionStay(CObj* _pOther)
{
}

void CFan::OnCollisionExit(CObj* _pOther)
{
}
