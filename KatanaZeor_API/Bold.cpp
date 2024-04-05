#include "Bold.h"
#include "BaseBullet.h"
#include "BloodEffect.h"
#include "BloodBGEffect.h"
#include "Player.h"

#include "Caretaker.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"

#include "Camera.h"

#include "Collider.h"
#include "RigidBody.h"

CBold::CBold() :m_dwShootTime(0), m_iBulletCount(0)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pBullet[i] = nullptr;
	}

	m_eType = BOLD;
	m_eRenderId = ENEMY;
}

CBold::~CBold()
{
}

void CBold::Initialize()
{
	Create_Collider();
	Create_DetectCollider();
	Create_RigidBody();
	Set_StageCol();

	m_pCareTaker = new CCaretaker;

	//애니메이션용 insert 이미지 크기, 콜라이더 크기, 콜라이더 offset, 프레임 개수
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/Enemy_bold_Idle.bmp", L"Enemy_bold_Idle", VEC2(72, 70), VEC2(30, 70), VEC2(-15, 0), 7, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/Enemy_bold_walk.bmp", L"Enemy_bold_walk", VEC2(68, 76), VEC2(40, 70), VEC2(0, 0), 6, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/Enemy_bold_run.bmp", L"Enemy_bold_run", VEC2(90, 78), VEC2(40, 70), VEC2(0, 0), 9, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Bold/Enemy_bold_hurt.bmp", L"Enemy_bold_hurt", VEC2(74, 64), VEC2(40, 70), VEC2(0, 0), 13, 100, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Enemy_follow.bmp", L"Enemy_follow", VEC2(16, 20), VEC2(0,0), VEC2(0, 0), 0, 200, GetTickCount());
	
	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_walk");
	Set_Frame();

	m_iHp = 1;
	m_vLookDir = { -1.0f, 0.f };
	m_iBulletCount = 0;

	m_pRigidBody->Set_VelocityLimit(VEC2(500.f, 1000.f));
	m_pRigidBody->Set_Friction(2000.f);
	m_pRigidBody->ResetGravityAccel();

	m_dwShootTime = GetTickCount();
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	m_pDetectCollider->Set_Offset(VEC2(10.f,0.f));
	m_pDetectCollider->Set_Size(VEC2(500.f, 100.f));
	m_bIsPatrol = true;
	m_fPatrolDistance = 100.f;

	//m_eState = ENEMY_IDLE;
}

int CBold::Update()
{
	if (CSceneMgr::Get_Instance()->Get_SceneState() != SC_PLAYING)
		return 0;

	VEC2 vTargetDistance{ m_pTarget->Get_CameraPos().x - m_vCameraPos.x, m_pTarget->Get_CameraPos().y - m_vCameraPos.y };

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsSpecialAttack())
	{
		return 0;
	}

	if (!m_bDead && !g_bIsTimeBack)
	{
		if (!m_bIsFind)
		{
			if (m_bIsPatrol)
			{
				//순찰 몬스터
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_walk");
				Set_Frame();

				m_vPos.x = m_vPos.x + m_vLookDir.x * 50.f * CTimeMgr::Get_Instance()->Get_DeltaTime() * CTimeMgr::Get_Instance()->Get_TimeScale();

				if (m_vPos.x > m_vStartPos.x + m_fPatrolDistance || m_vPos.x < m_vStartPos.x - m_fPatrolDistance)
				{
					m_vLookDir.x *= -1;
				}
			}
			else if (!m_bIsPatrol)
			{
				//가만히 있는 몬스터
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_Idle");
				Set_Frame();
			}
		}

		if (m_bIsFind && !m_bIsAttack)
		{
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_run");
			Set_Frame();

			if (m_dwDetectEffectTime + 600 < GetTickCount())
			{
				Safe_Delete<CObj*>(m_pEffect);
				m_dwDetectEffectTime = 0;
			}

			if (vTargetDistance.Length() <= 350.f)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_Idle");
				Set_Frame();
				m_bIsAttack = true;
				m_pRigidBody->Set_VelocityX(0.f);

			}
			else
			{
				m_bIsAttack = false;
				m_pRigidBody->Set_VelocityX(200.f * m_vLookDir.x);
			}
	
		}

		if (m_bIsAttack)
		{
			if (m_pTarget->Get_Pos().x >= m_vPos.x && m_vLookDir.x)
			{
				m_vLookDir.x = 1.f;
			}
			else
			{
				m_vLookDir.x = -1.f;
			}

			if (m_dwShootTime + (3000 * (1 / CTimeMgr::Get_Instance()->Get_TimeScale())) < GetTickCount() && !m_bIsAttacking)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_Idle");
				Set_Frame(false);

				if (!m_pBullet[m_iBulletCount % 3])
				{
					m_pBullet[m_iBulletCount % 3] = new CBaseBullet;
					m_pBullet[m_iBulletCount % 3]->Initialize();
					m_pBullet[m_iBulletCount % 3]->Set_IsActive(false);
					CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, m_pBullet[m_iBulletCount % 3]);
				}

				m_pBullet[m_iBulletCount % 3]->Set_Pos(m_vPos.x, m_vPos.y);
				dynamic_cast<CBaseBullet*>(m_pBullet[m_iBulletCount % 3])->Set_Target(m_pTarget);
				m_pBullet[m_iBulletCount % 3]->Set_IsActive(true);
				m_dwShootTime = GetTickCount();
				m_iBulletCount++;
				m_bIsAttacking = true;
			}

			if (vTargetDistance.Length() > 360.f)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_Idle");
				Set_Frame();

				m_pRigidBody->Set_VelocityX(0.f);
				m_dwShootTime = GetTickCount();
				m_bIsAttacking = false;
				m_bIsAttack = false;
			}
			if (m_tFrame.isPlayDone && m_bIsAttacking)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_Idle");
				Set_Frame();

				m_bIsAttacking = false;
				m_bIsAttack = false;
			}
		}
	}
	else if (g_bIsTimeBack)		//과거 회귀
	{
		for (int i = 0; i < 20; ++i)
		{
			if (m_pCareTaker->Get_ListSize() > 0)
			{
				Set_MementoState(m_pCareTaker->TimeBakc());
				Safe_Delete<CObj*>(m_pEffect);
			}

			if (m_pCareTaker->Get_ListSize() <= 0)
			{
				TimeBakcDone();
			}
		}
	}
	else  if(m_bDead)//죽은 상태
	{
		if (m_pEffect)
		{
			Safe_Delete<CObj*>(m_pEffect);
			m_dwDetectEffectTime = 0;
		}
	}


	if (m_bIsMoveFrame)
		__super::Move_Frame();

	return 0;
}

void CBold::Late_Update()
{
	m_pRigidBody->Late_Update();
	m_pCollider->Late_Update();

	m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
	m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

	m_pDetectCollider->Late_Update();

	if (!g_bIsTimeBack && CSceneMgr::Get_Instance()->Get_SceneState() == SC_PLAYING)
	{
		m_pCareTaker->Add(Save_State());
	}

	PixelCollisionBottom();
}

void CBold::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hMemDC = m_pAniBmp->Get_MemDC();
	
	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
		(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
		hMemDC,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// 출력할 비트맵의 시작 y좌표
		(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
		(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

	m_pCollider->Render(hDC);
	m_pDetectCollider->Render(hDC);

	if(m_pEffect && !m_bDead)
		m_pEffect->Render(hDC);
}

void CBold::Release()
{
}

void CBold::OnCollisionEnter(CObj* _pOther)
{
	__super::OnCollisionEnter(_pOther);
}

void CBold::OnCollisionStay(CObj* _pOther)
{
	__super::OnCollisionStay(_pOther);
}




