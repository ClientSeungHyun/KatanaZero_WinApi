#include "BomoBullet.h"
#include "Effect.h"
#include "BombBulletEffect.h"

#include "BmpMgr.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Camera.h"
#include "TimeMgr.h"
#include "ObjMgr.h"


CBomoBullet::CBomoBullet() : m_bIsCreate(false), m_dwDestroyTime(0)
{
	srand((unsigned)time(NULL));
	m_eId = OBJ_BULLET;
	m_eType = BOMB;
	m_eRenderId = GAMEOBJECT;
}

CBomoBullet::~CBomoBullet()
{
	Release();
}

void CBomoBullet::Initialize()
{
	Create_Collider();
	Create_RigidBody();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_bomb.bmp", L"Boss_Bomb", VEC2(20, 20), VEC2(20, 20), VEC2(0, 0), 1, 100, GetTickCount());

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Bomb");
	m_bIsActive = false;
	m_bIsCreate = false;
	m_pCollider->Set_IsActive(false);

}

int CBomoBullet::Update()
{
	if (m_bIsActive)
	{
		m_fRandomAngle = (float)(rand() % 70 + 120);

		m_fRandomPowerX = (float)(rand() % 30 + 5);
		m_fRandomPowerY = (float)(rand() % 20 + 5);

		if (m_dwMoveTime + m_iDelayTime > GetTickCount())
		{
			m_vPos.x = m_vPos.x - cos(m_fRandomAngle * (PI / 180.f)) * m_fRandomPowerX * CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x * CTimeMgr::Get_Instance()->Get_TimeScale();
			m_vPos.y = m_vPos.y - sin(m_fRandomAngle * (PI / 180.f)) * m_fRandomPowerY * CTimeMgr::Get_Instance()->Get_TimeScale();
		}
		else if(!m_bIsCreate && m_dwMoveTime + m_iDelayTime < GetTickCount())
		{
			CObj* pBombEffect = new CBombBulletEffect;;
			pBombEffect->Set_Pos(m_vPos.x, m_vPos.y);
			pBombEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBombEffect);
			m_bIsCreate = true;
			m_dwDestroyTime = GetTickCount();
		}

		if (m_bIsCreate && m_dwDestroyTime + 1000 < GetTickCount())
		{
			m_bIsCreate = false;
			m_bIsActive = false;
		}

		m_pCollider->Set_IsActive(m_bIsActive);
	}
    return 0;
}

void CBomoBullet::Late_Update()
{
	if (m_bIsActive)
	{
		m_pCollider->Late_Update();
		m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
		m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

		m_pRigidBody->Move();
	}
}

void CBomoBullet::Render(HDC hDC)
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

void CBomoBullet::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

	if(m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);
}

void CBomoBullet::OnCollisionEnter(CObj* _pOther)
{
}

void CBomoBullet::OnCollisionStay(CObj* _pOther)
{
}
