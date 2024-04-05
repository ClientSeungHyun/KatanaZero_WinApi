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
			GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
				(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
				(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
				(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
				(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
				hBoombDC,					// 비트맵을 가지고 있는 dc
				0,						// 출력할 비트맵의 시작 x좌표
				0,						// 출력할 비트맵의 시작 y좌표
				(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
				(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
				RGB(255, 0, 255));	// 제거할 픽셀의 색상 값
		}
		else
		{

			GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
				(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
				(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
				(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
				(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
				hBoombDC,					// 비트맵을 가지고 있는 dc
				m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
				((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// 출력할 비트맵의 시작 y좌표
				(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
				(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
				RGB(0, 0, 0));	// 제거할 픽셀의 색상 값
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
