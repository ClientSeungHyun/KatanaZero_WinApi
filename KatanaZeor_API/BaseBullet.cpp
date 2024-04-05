#include "BaseBullet.h"

#include "BmpMgr.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Camera.h"

#include "Caretaker.h"

CBaseBullet::CBaseBullet() : m_fAngle(30.f), m_vTargetPos(0, 0), m_fDiagonal(0.f), m_fBulletPower(0.f), m_bIsReflect(false)
{
	ZeroMemory(&m_tPoint, sizeof(POINT));

	m_eId = OBJ_BULLET;
	m_eType = BASE;
	m_eRenderId = GAMEOBJECT;
}

CBaseBullet::~CBaseBullet()
{
	Release();
}

void CBaseBullet::Initialize()
{
	Create_Collider();
	Create_RigidBody();
	m_pCareTaker = new CCaretaker;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BaseBullet.bmp", L"BaseBullet", VEC2(48, 48), VEC2(48, 48), VEC2(0, 0), 10, 30, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BaseBullet_Plg.bmp", L"BaseBullet_Plg", VEC2(48, 48), VEC2(100, 100), VEC2(10, 0), 10, 30, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BaseBullet_Reset.bmp", L"BaseBullet_Reset", VEC2(48, 48), VEC2(100, 100), VEC2(10, 0), 10, 30, GetTickCount());

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"BaseBullet");
	m_bIsActive = true;
	m_pCollider->Set_IsActive(true);

	m_fBulletPower = 900.f;
	m_bIsReflect = false;

	m_fDiagonal = sqrt((m_pAniBmp->Get_ImageSize().x / 2.f) * (m_pAniBmp->Get_ImageSize().x / 2.f) + (m_pAniBmp->Get_ImageSize().y / 2.f) * (m_pAniBmp->Get_ImageSize().y / 2.f));

	// 좌 상단
	m_tPoint[0].x = LONG((m_pAniBmp->Get_ImageSize().x / 2.f) + cos((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);
	m_tPoint[0].y = LONG((m_pAniBmp->Get_ImageSize().y / 2.f) - sin((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);

	// 우 상단		
	m_tPoint[1].x = LONG((m_pAniBmp->Get_ImageSize().x / 2.f) + cos((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);
	m_tPoint[1].y = LONG((m_pAniBmp->Get_ImageSize().y / 2.f) - sin((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);

	// 좌 하단		
	m_tPoint[2].x = LONG((m_pAniBmp->Get_ImageSize().x / 2.f) + cos((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
	m_tPoint[2].y = LONG((m_pAniBmp->Get_ImageSize().y / 2.f) - sin((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);

}

int CBaseBullet::Update()
{
	if (m_bIsActive && !g_bIsTimeBack)
	{
		if (m_vPos.x < CCamera::Get_Instance()->GetLookAtPos().x - WINCX * 0.5f ||
			m_vPos.x > CCamera::Get_Instance()->GetLookAtPos().x + WINCX * 0.5f ||
			m_vPos.y < CCamera::Get_Instance()->GetLookAtPos().y - WINCY * 0.5f ||
			m_vPos.y > CCamera::Get_Instance()->GetLookAtPos().y + WINCY * 0.5f)
		{
			m_bIsActive = false;
			m_fBulletPower = 1200.f;
			m_bIsReflect = false;
		}


		m_pRigidBody->Set_Velocity(VEC2(cos(m_fAngle * PI / 180.f) * m_fBulletPower, -sin(m_fAngle * PI / 180.f) * m_fBulletPower));
	}
	else if (!m_bIsActive)
	{
		m_fBulletPower = 1200.f;
		m_bIsReflect = false;
	}

	if (g_bIsTimeBack)
	{
		//과거 회귀
		for (int i = 0; i < 20; ++i)
		{
			if (m_pCareTaker->Get_ListSize() > 0)
			{
				Set_MementoState(m_pCareTaker->TimeBakc());
			}

			if (m_pCareTaker->Get_ListSize() <= 0)
			{
				TimeBakcDone();
			}
		}
	}
	return 0;
}

void CBaseBullet::Late_Update()
{
	if (m_bIsActive)
	{
		m_pRigidBody->Move();

		m_pCollider->Late_Update();
		m_pCollider->Set_Offset(VEC2(m_pAniBmp->Get_ColliderOffset().x * m_vLookDir.x, m_pAniBmp->Get_ColliderOffset().y * m_vLookDir.y));
		m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));
	}

	if (!g_bIsTimeBack && CSceneMgr::Get_Instance()->Get_SceneState() == SC_PLAYING)
	{
		m_pCareTaker->Add(Save_State());
	}

	
	//m_pCollider->Set_Pos(VEC2(m_vPos.x ,m_vPos.y ));
}

void CBaseBullet::Render(HDC hDC)
{
	if (m_bIsActive)
	{
		m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

		HDC	hMemDC = m_pAniBmp->Get_MemDC();
		HDC	hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"BaseBullet_Plg")->Get_MemDC();
		HDC	hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"BaseBullet_Reset")->Get_MemDC();

		PlgBlt(hPlgDC,	// 회전 시킬 이미지를 어디에 그릴 것인가
			m_tPoint, // 그림을 그릴 사각형의 좌표 포인트 3개, 배열의 순서는 좌 상단, 우 상단, 좌 하단
			hMemDC,	// 어디에 있는 이미지를 회전 시켜 그릴 것인가
			0, 0,		// 어디서 부터 그릴 것인가 
			(int)m_pAniBmp->Get_ImageSize().x,	// 그릴 이미지의 가로, 세로 사이즈
			(int)m_pAniBmp->Get_ImageSize().y,
			NULL, NULL, NULL);

		GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
			(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
			(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
			hPlgDC,					// 비트맵을 가지고 있는 dc
			(int)0,						// 출력할 비트맵의 시작 x좌표
			(int)0,						// 출력할 비트맵의 시작 y좌표
			(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
			(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
			RGB(255, 255, 255));	// 제거할 픽셀의 색상 값

		// 가리기 용 텍스처 출력
		BitBlt(hPlgDC,
			0, 0,
			(int)m_pAniBmp->Get_ImageSize().x, (int)m_pAniBmp->Get_ImageSize().y,
			hResetDC,
			0, 0,
			SRCCOPY);
	}
}

void CBaseBullet::Release()
{
	if (m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);

	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

	if (m_pCareTaker)
		Safe_Delete<CCaretaker*>(m_pCareTaker);
}


void CBaseBullet::Set_Target(CObj* pTarget)
{
	m_vTargetPos = pTarget->Get_Pos();

	m_vLookDir = { m_vTargetPos.x - (m_vPos.x ), m_vTargetPos.y - m_vPos.y };
	m_vLookDir.Normalize();

	m_fAngle = -atan2f(m_vLookDir.y, m_vLookDir.x) * (180.f / PI);

	m_fDiagonal = sqrt((m_pAniBmp->Get_ImageSize().x / 2.f) * (m_pAniBmp->Get_ImageSize().x / 2.f) + (m_pAniBmp->Get_ImageSize().y / 2.f) * (m_pAniBmp->Get_ImageSize().y / 2.f));

	// 좌 상단
	m_tPoint[0].x = LONG((m_pAniBmp->Get_ImageSize().x / 2.f) + cos((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);
	m_tPoint[0].y = LONG((m_pAniBmp->Get_ImageSize().y / 2.f) - sin((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);

	// 우 상단		
	m_tPoint[1].x = LONG((m_pAniBmp->Get_ImageSize().x / 2.f) + cos((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);
	m_tPoint[1].y = LONG((m_pAniBmp->Get_ImageSize().y / 2.f) - sin((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);

	// 좌 하단		
	m_tPoint[2].x = LONG((m_pAniBmp->Get_ImageSize().x / 2.f) + cos((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
	m_tPoint[2].y = LONG((m_pAniBmp->Get_ImageSize().y / 2.f) - sin((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);

}

void CBaseBullet::TimeBakcDone()
{
	m_bDead = false;
	m_bIsActive = false;
	m_bIsReflect = false;
	m_fBulletPower = 1200.f;

	m_pCollisionOthers.clear();
}
