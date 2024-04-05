#include "BombBulletEffect.h"
#include "Player.h"

#include "BmpMgr.h"
#include "Camera.h"
#include "Collider.h"
#include "SoundMgr.h"

CBombBulletEffect::CBombBulletEffect():m_bIsBomb(false)
{
	m_eId = OBJ_BULLET;
	m_eType = BOMB;
	m_eRenderId = GAMEOBJECT;
}

CBombBulletEffect::~CBombBulletEffect()
{
	Release();
}

void CBombBulletEffect::Initialize()
{
	Create_Collider();
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_circle.bmp", L"Boss_Circle", VEC2(274, 274), VEC2(260, 260), VEC2(0, 0), 0, 50, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/effect/BombBulletEffect.bmp", L"BombBulletEffect", VEC2(270, 270), VEC2(270,270), VEC2(0, 0), 9, 50, GetTickCount());
	m_vLookDir = { 1.0f,0.f };

	CSoundMgr::Get_Instance()->StopSound(BOSS_CIRCLE);
	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntressbomb_armed_01.wav", BOSS_CIRCLE, g_fEffectSound);
	
	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Circle");
	m_pCollider->Set_IsActive(false);

	m_dwRangeTime = GetTickCount();
;}

int CBombBulletEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bIsBomb && m_dwRangeTime + 1000 < GetTickCount())
	{
		CSoundMgr::Get_Instance()->StopSound(BOSS_EXPLOSION);
		CSoundMgr::Get_Instance()->PlaySound(L"explosion1.wav", BOSS_EXPLOSION, g_fEffectSound);
		m_bIsBomb = true;
		m_pCollider->Set_IsActive(true);
		m_bIsActive = true;
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"BombBulletEffect");
		Set_Frame(false);
	}

	if (m_bIsBomb && m_tFrame.isPlayDone)
	{
		m_bDead = true;
	}

	Move_Frame();

    return 0;
}

void CBombBulletEffect::Late_Update()
{
	m_pCollider->Late_Update();
	m_pCollider->Set_Offset(VEC2(m_pAniBmp->Get_ColliderOffset().x * m_vLookDir.x, m_pAniBmp->Get_ColliderOffset().y * m_vLookDir.y));
	m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

}

void CBombBulletEffect::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC hBoombDC = m_pAniBmp->Get_MemDC();

	if (m_bIsActive)
	{
		if (hBoombDC == CBmpMgr::Get_Instance()->Find_Image(L"Boss_Circle")->Get_MemDC())
		{
			GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
				(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // ���� ���� ��ġ�� left
				(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // ���� ���� ��ġ�� top
				(int)m_pAniBmp->Get_ImageSize().x,			 // ���� ���� ���� ����
				(int)m_pAniBmp->Get_ImageSize().y,			 // ���� ���� ���� ����
				hBoombDC,					// ��Ʈ���� ������ �ִ� dc
				0,						// ����� ��Ʈ���� ���� x��ǥ
				0,						// ����� ��Ʈ���� ���� y��ǥ
				(int)m_pAniBmp->Get_ImageSize().x,			// ����� ��Ʈ���� ���� ������
				(int)m_pAniBmp->Get_ImageSize().y,			// ����� ��Ʈ���� ���� ������
				RGB(255, 0, 255));	// ������ �ȼ��� ���� ��
		}
		else
		{

			GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
				(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // ���� ���� ��ġ�� left
				(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // ���� ���� ��ġ�� top
				(int)m_pAniBmp->Get_ImageSize().x,			 // ���� ���� ���� ����
				(int)m_pAniBmp->Get_ImageSize().y,			 // ���� ���� ���� ����
				hBoombDC,					// ��Ʈ���� ������ �ִ� dc
				m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// ����� ��Ʈ���� ���� x��ǥ
				((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// ����� ��Ʈ���� ���� y��ǥ
				(int)m_pAniBmp->Get_ImageSize().x,			// ����� ��Ʈ���� ���� ������
				(int)m_pAniBmp->Get_ImageSize().y,			// ����� ��Ʈ���� ���� ������
				RGB(0, 0, 0));	// ������ �ȼ��� ���� ��
		}
		m_pCollider->Render(hDC);
	}
}

void CBombBulletEffect::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);
}

void CBombBulletEffect::OnCollisionEnter(CObj* _pOther)
{
	
}

void CBombBulletEffect::OnCollisionStay(CObj* _pOther)
{
}

void CBombBulletEffect::OnCollisionExit(CObj* _pOther)
{
}
