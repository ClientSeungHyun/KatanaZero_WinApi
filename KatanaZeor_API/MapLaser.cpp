#include "MapLaser.h"

#include "Collider.h"
#include "RigidBody.h"
#include "Camera.h"

#include "Player.h"
#include "BmpMgr.h"
#include "SoundMgr.h"

CMapLaser::CMapLaser() :m_IsLaserOn(false), m_dwOnTime(0)
{
	m_eRenderId = BACKGROUND;
}

CMapLaser::~CMapLaser()
{
	Release();
}

void CMapLaser::Initialize()
{
	Create_Collider();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/StageLaser_NoCollision.bmp", L"Laser_NoCollision", VEC2(4, 750), VEC2(4,0), VEC2(0, 0), 3, 10, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/StageLaser_Collision.bmp", L"Laser_Collision", VEC2(6, 2000), VEC2(6, 2000), VEC2(0, 0), 1, 10, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/Laser_On_Down.bmp", L"Laser_On_Down");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/Laser_Off_Down.bmp", L"Laser_Off_Down");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/Laser_On_Up.bmp", L"Laser_On_Up");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/Laser_Off_Up.bmp", L"Laser_Off_Up");

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_NoCollision");
	m_vLookDir = { 1.0f,0.f };
	Set_Frame();

	m_pUpBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_Off_Up");
	m_pDownBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_Off_Down");

	m_pCollider->Set_IsActive(true);
	m_eId = OBJ_TRAP;

}

int CMapLaser::Update()
{
	if (m_IsLaserOn)
	{
		m_pUpBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_On_Up");
		m_pDownBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_On_Down");

		if (m_dwOnTime + 100 < GetTickCount())
		{
			m_IsLaserOn = false; 
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_NoCollision");
			Set_Frame();
		}
	}
	else if(!m_IsLaserOn)
	{
		m_pUpBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_Off_Up");
		m_pDownBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_Off_Down");
	}


	Move_Frame();

	return 0;
}

void CMapLaser::Late_Update()
{
	m_pCollider->Late_Update();

	m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
	m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x,m_vSize.y));
}

void CMapLaser::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC hLaserDC = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_vSize.y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
		(int)m_vSize.y,			 // 복사 받을 세로 길이
		hLaserDC,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
		((int)(m_vLookDir.x + 2) % 3) * (int)m_vSize.y,						// 출력할 비트맵의 시작 y좌표
		(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
		(int)m_vSize.y,			// 출력할 비트맵의 세로 사이즈
		RGB(0, 0, 0));	// 제거할 픽셀의 색상 값


	

	//m_pCollider->Render(hDC);

}

void CMapLaser::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);
}

void CMapLaser::OnCollisionEnter(CObj* _pOther)
{
	CSoundMgr::Get_Instance()->PlaySound(L"sound_laser_explosion.wav", SOUND_PLAYSONG, g_fEffectSound);
	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Laser_Collision");
	Set_Frame();

	m_IsLaserOn = true;
	m_dwOnTime = GetTickCount();

	if (_pOther->Get_Id() == OBJ_PLAYER )
	{
		if (dynamic_cast<CPlayer*>(_pOther)->Get_State() != ROLL && dynamic_cast<CPlayer*>(_pOther)->Get_State() != FLIP)
		{
			_pOther->Get_RigidBody()->Set_Velocity(VEC2(0.f, 0.f));
			dynamic_cast<CPlayer*>(_pOther)->Set_IsHitRaser(true);
		}
	}

	if (_pOther->Get_Id() == OBJ_ENEMY)
	{
		_pOther->Get_RigidBody()->Set_Velocity(VEC2(0.f, 0.f));
		_pOther->Set_Dead();
	}
}

void CMapLaser::OnCollisionStay(CObj* _pOther)
{
}

void CMapLaser::OnCollisionExit(CObj* _pOther)
{
}
