#include "CLaserBullet.h"
#include "Boss.h"

#include "Camera.h"
#include "BmpMgr.h"
#include "Collider.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CLaserBullet::CLaserBullet():m_bIsGroundLaser(false), m_dwAimTime(0), m_dwShootTime(0)
{
	m_eId = OBJ_BULLET;
	m_eType = LASER;
	m_eRenderId = GAMEOBJECT;
}

CLaserBullet::~CLaserBullet()
{
	Release();
}

void CLaserBullet::Initialize()
{
	Create_Collider();
	
	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Aimlazer");
	m_bIsActive = true;
	m_pCollider->Set_IsActive(false);
}

int CLaserBullet::Update()
{
	if (m_bIsActive)
	{
		if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_State() == HURT)
		{
			m_pCollider->Set_IsActive(false);
			m_bIsActive = false;
			m_dwAimTime = 0;
			m_dwShootTime = 0;

			return 0;
		}

		//레이저 조준
		if (m_bIsGroundLaser && !m_dwAimTime)
		{
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Aimlazer");
			m_dwAimTime = GetTickCount();
			CSoundMgr::Get_Instance()->StopSound(BOSS_AIM);
			CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_lockon_01.wav", BOSS_AIM, g_fEffectSound);
		}
		else if(!m_bIsGroundLaser && !m_dwAimTime)
		{
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Aimlazer2");
			m_dwAimTime = GetTickCount();
			CSoundMgr::Get_Instance()->StopSound(BOSS_AIM);
			CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_lockon_01.wav", BOSS_AIM, g_fEffectSound);
		}

		//레이저 발사
		if (m_dwAimTime + 350 < GetTickCount() && !m_dwShootTime)
		{
			m_pCollider->Set_IsActive(true);
			if (m_bIsGroundLaser)
			{
				m_vPos.x = m_vShootPos.x + (1040 * CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x);
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Lazer3");
			}
			else if (!m_bIsGroundLaser)
			{
				m_vPos.y = m_vShootPos.y + 1040 ;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Lazer5");
			}

			m_dwShootTime = GetTickCount();
			CSoundMgr::Get_Instance()->StopSound(BOSS_LASERSHOOT);
			CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntresslaser_shot_1.wav", BOSS_LASERSHOOT, g_fEffectSound);
		}

		//레이저 끄기
		if (m_dwShootTime + 150 < GetTickCount() && m_pCollider->Get_IsActive())
		{
			m_pCollider->Set_IsActive(false);
			m_bIsActive = false;
			m_dwAimTime = 0;
			m_dwShootTime = 0;
		}
	}
	return 0;
}

void CLaserBullet::Late_Update()
{
	m_pCollider->Late_Update();
	m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
	m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

}

void CLaserBullet::Render(HDC hDC)
{
	if (m_bIsActive)
	{
		m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

		HDC	hMemDC = m_pAniBmp->Get_MemDC();

		GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
			(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
			(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
			hMemDC,					// 비트맵을 가지고 있는 dc
			(int)0,						// 출력할 비트맵의 시작 x좌표
			(int)0,						// 출력할 비트맵의 시작 y좌표
			(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
			(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
			RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

		//m_pCollider->Render(hDC);
	}
}

void CLaserBullet::Release()
{
	if (m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);

	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);
}
