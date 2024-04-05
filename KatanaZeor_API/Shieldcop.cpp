#include "Shieldcop.h"
#include "BaseBullet.h"
#include "Effect.h"
#include "HitEffect.h"
#include "BloodBGEffect.h"
#include "BloodEffect.h"
#include "Player.h"

#include "Caretaker.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

#include "Camera.h"

#include "Collider.h"
#include "RigidBody.h"

CShieldcop::CShieldcop() :m_bIsKnockback(false), m_iBulletCount(0), m_bIsChangDir(false)
{
	for (int i = 0; i < 3; ++i)
	{
		m_pBullet[i] = nullptr;
	}

	m_eType = SHIELDCOP;
	m_eRenderId = ENEMY;
}

CShieldcop::~CShieldcop()
{
}

void CShieldcop::Initialize()
{
	Create_Collider();
	Create_DetectCollider();
	Create_RigidBody();
	Set_StageCol();

	m_pCareTaker = new CCaretaker;

	//애니메이션용 insert 이미지 크기, 콜라이더 크기, 콜라이더 offset, 프레임 개수
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Shieldcop/Enemy_shieldcop_idle.bmp", L"Enemy_shieldcop_idle", VEC2(62, 74), VEC2(30, 70), VEC2(-15, 0), 5, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Shieldcop/Enemy_shieldcop_run.bmp", L"Enemy_shieldcop_run", VEC2(74, 76), VEC2(40, 70), VEC2(0, 0), 9, 80, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Shieldcop/Enemy_shieldcop_walk.bmp", L"Enemy_shieldcop_walk", VEC2(52, 74), VEC2(40, 70), VEC2(0, 0), 9, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Shieldcop/Enemy_shieldcop_hurt.bmp", L"Enemy_shieldcop_hurt", VEC2(137, 74), VEC2(40, 70), VEC2(0, 0), 13, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Shieldcop/Enemy_shieldcop_bash.bmp", L"Enemy_shieldcop_bash", VEC2(112, 74), VEC2(40, 70), VEC2(0, 0), 5, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Shieldcop/Enemy_shieldcop_turn.bmp", L"Enemy_shieldcop_turn", VEC2(62, 74), VEC2(40, 70), VEC2(0, 0), 7, 300, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Enemy_follow.bmp", L"Enemy_follow", VEC2(16, 20), VEC2(0, 0), VEC2(0, 0), 0, 200, GetTickCount());

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");
	Set_Frame();

	m_iHp = 1;
	m_vLookDir = { -1.0f, 0.f };
	m_iBulletCount = 0;

	m_pRigidBody->Set_VelocityLimit(VEC2(500.f, 1000.f));
	m_pRigidBody->Set_Friction(2000.f);
	m_pRigidBody->ResetGravityAccel();

	m_dwShootTime = GetTickCount();
	m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	m_pDetectCollider->Set_Offset(VEC2(10.f, 0.f));
	m_pDetectCollider->Set_Size(VEC2(500.f, 100.f));

	m_bIsPatrol = true;
	m_fPatrolDistance = 200.f;

	m_eState = ENEMY_IDLE;
}

int CShieldcop::Update()
{
	if (CSceneMgr::Get_Instance()->Get_SceneState() != SC_PLAYING)
		return 0;
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsSpecialAttack())
	{
		return 0;
	}

	VEC2 vTargetDistance{ m_pTarget->Get_CameraPos().x - m_vCameraPos.x, m_pTarget->Get_CameraPos().y - m_vCameraPos.y };

	if (!m_bDead && !g_bIsTimeBack)
	{
		if (m_bIsKnockback)
		{
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");
			Set_Frame();
			m_pRigidBody->CalcFriction();
			if (m_dKnockbakcTime + 500 < GetTickCount())
			{
				m_bIsKnockback = false;
				//m_dKnockbakcTime = 0;
			}
			m_bIsFind = true;
			m_bIsAttack = false;
			return 0;
		}

		if (!m_bIsFind)
		{
			if (m_bIsPatrol)
			{
				//순찰 몬스터
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_walk");
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
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");
				Set_Frame();
			}
		}

		if (m_pTarget->Get_Pos().x >= m_vPos.x && m_vLookDir.x != 1.f && m_bIsFind)
		{
			if (!m_bIsChangDir)
			{
				m_dwChangeDirTime = GetTickCount();
				m_bIsChangDir = true;
			}

			if (m_dwChangeDirTime + 1500 < GetTickCount())
			{
				m_vLookDir.x = 1.f;
				m_dwShootTime = GetTickCount();
				m_bIsChangDir = false;
			}
			return 0;
		}
		else if (m_pTarget->Get_Pos().x < m_vPos.x && m_vLookDir.x != -1.f&& m_bIsFind)
		{
			if (!m_bIsChangDir)
			{
				m_dwChangeDirTime = GetTickCount();
				m_bIsChangDir = true;
			}

			if (m_dwChangeDirTime + 1000 < GetTickCount())
			{
				m_vLookDir.x = -1.f;
				m_dwShootTime = GetTickCount();
				m_bIsChangDir = false;
			}
			return 0;
		}

		if (m_bIsFind && !m_bIsAttack)
		{
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_run");
			Set_Frame();
			
			if (m_dwDetectEffectTime + 600 < GetTickCount())
			{
				Safe_Delete<CObj*>(m_pEffect);
				m_dwDetectEffectTime = 0;
			}

		
			if (vTargetDistance.Length() <= 230.f)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");
				Set_Frame();
				m_bIsAttack = true;
				m_pRigidBody->Set_VelocityX(0.f);
				m_dwShootTime = GetTickCount();
			}
			else
			{
				m_bIsAttack = false;
				m_pRigidBody->Set_VelocityX(230.f * m_vLookDir.x);
			}
		}

		if (m_bIsAttack)
		{
			//총알을 계속해서 재사용
			if (m_dwShootTime + (1000 * (1 / CTimeMgr::Get_Instance()->Get_TimeScale())) < GetTickCount() && !m_bIsAttacking)
			{
				m_bIsChase = false;
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");

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
				m_iBulletCount++;
				m_bIsAttacking = true;
			}
		
			if (vTargetDistance.Length() > 360.f)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");
				Set_Frame();

				m_pRigidBody->Set_VelocityX(0.f);
				m_dwShootTime = GetTickCount();
				m_bIsAttacking = false;
				m_bIsAttack = false;
			}
			if (m_tFrame.isPlayDone && m_bIsAttacking)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_idle");
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
				m_bIsKnockback = false;
			}
		}
	}
	else  if (m_bDead)//죽은 상태
	{
		if (m_pEffect)
		{
			Safe_Delete<CObj*>(m_pEffect);
			m_dwDetectEffectTime = 0;
		}
	}

	if (m_pEffect)
		m_pEffect->Set_Pos(m_vPos.x - 15 * m_vLookDir.x, m_vPos.y - 50);

	if (m_dwDetectEffectTime + 600 < GetTickCount() && m_dwDetectEffectTime)
	{
		Safe_Delete<CObj*>(m_pEffect);
		m_dwDetectEffectTime = 0;
	}

	if(m_bIsMoveFrame)
		__super::Move_Frame();

	return 0;
}

void CShieldcop::Late_Update()
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

void CShieldcop::Render(HDC hDC)
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
		RGB(0, 0, 0));	// 제거할 픽셀의 색상 값

	m_pCollider->Render(hDC);
	m_pDetectCollider->Render(hDC);

	if (m_pEffect && !m_bDead)
		m_pEffect->Render(hDC);
}

void CShieldcop::Release()
{
}

void CShieldcop::OnCollisionEnter(CObj* _pOther)
{
	__super::OnCollisionEnter(_pOther); 
	
	if (_pOther->Get_Id() == OBJ_BULLET && _pOther->Get_IsActive())
	{
		if (dynamic_cast<CBullet*>(_pOther)->Get_Type() == BASE)
		{
			if (dynamic_cast<CBaseBullet*>(_pOther)->Get_IsReflect())
			{
				VEC2 dir{ -_pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
				dir.Normalize();


				//방패를 들고 있는 방향
				if ((dir.x > 0 && m_vLookDir.x == -1) || (dir.x < 0 && m_vLookDir.x == 1))
				{
					m_pRigidBody->AddForce(VEC2(dir.x * 200000.f, 0));
					_pOther->Set_IsActive(false);
					m_bIsGround = false;
					m_bIsKnockback = true;
					m_dKnockbakcTime = GetTickCount();
					return;
				}

				m_pRigidBody->AddForce(VEC2(dir.x * 200000.f, -30000.f));
				_pOther->Set_IsActive(false);
				m_bIsGround = false;
				m_bDead = true;
			}
		}
	}

	//방패용으로 수정
	if (((_pOther->Get_Id() == OBJ_EFFECT && dynamic_cast<CEffect*>(_pOther)->Get_EfeectId() == EFFECT_PLAYERATTACK) || _pOther->Get_Id() == OBJ_ITEM) && _pOther->Get_IsActive() && !m_bDead)
	{
		VEC2 dir{ _pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
		dir.Normalize();

		if (_pOther->Get_Id() == OBJ_ITEM)
		{
			_pOther->Set_Dead();
		}

		if ((dir.x > 0 && m_vLookDir.x == -1) || (dir.x < 0 && m_vLookDir.x == 1))
		{
			m_pRigidBody->AddForce(VEC2(dir.x * 200000.f, 0.f));
			m_dKnockbakcTime = GetTickCount();
			m_bIsKnockback = true;
			return;
		}

		//죽는 상황
		CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", ENEMY_DIE1, g_fEffectSound);

		//hit 이펙트
		CObj* pHitEffect = new CHitEffect;
		pHitEffect->Set_Pos(m_vPos.x, m_vPos.y);
		pHitEffect->Initialize();
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pHitEffect);

		if (!m_bIsBlood)
		{
			CObj* pBloodEffect = new CBloodEffect;
			pBloodEffect->Set_Pos(m_vPos.x, m_vPos.y);
			pBloodEffect->Set_LookDir(m_vLookDir);
			pBloodEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodEffect);

			//blood 이펙트
			CObj* pBloodBGEffect = new CBloodBGEffect;
			pBloodBGEffect->Set_Pos(m_vPos.x - m_vLookDir.x * 30.f, m_vPos.y - 30.f);
			pBloodBGEffect->Set_LookDir(m_vLookDir);
			pBloodBGEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodBGEffect);

			m_bIsBlood = true;
		}

		m_pPrevAniBmp = m_pAniBmp;
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_hurt");
		Set_Frame(false);

		m_bIsGround = false;
		m_bDead = true;
	}
}

void CShieldcop::OnCollisionStay(CObj* _pOther)
{
}


//dir change
//if (m_pTarget->Get_CameraPos().x >= m_vCameraPos.x && m_vLookDir.x != 1.f)
//{
//	if (!m_bIsChangDir)
//	{
//		m_bIsChangDir = true;
//		m_pPrevAniBmp = m_pAniBmp;
//		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_turn");
//
//		m_vLookDir.x = 1.f;
//		Set_Frame(false);
//	}
//	if (m_tFrame.isPlayDone)
//	{
//		m_bIsChangDir = false;
//		m_dwShootTime = GetTickCount();
//	}
//	return 0;
//}
//else if (m_pTarget->Get_CameraPos().x <= m_vCameraPos.x && m_vLookDir.x != -1.f)
//{
//	if (!m_bIsChangDir)
//	{
//		m_bIsChangDir = true;
//		m_pPrevAniBmp = m_pAniBmp;
//		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_turn");
//
//		m_vLookDir.x = -1.f;
//		Set_Frame(false);
//	}
//	if (m_tFrame.isPlayDone)
//	{
//		m_bIsChangDir = false;
//		m_dwShootTime = GetTickCount();
//	}
//	return 0;
//}