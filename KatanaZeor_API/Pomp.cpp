#include "Pomp.h"
#include "Effect.h"
#include "BloodEffect.h"
#include "HitEffect.h"
#include "BloodBGEffect.h"
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
CPomp::CPomp() :m_bIsBounceAble(false), m_bISBounce(false), m_dwBounceTime(0), m_pBounceEffect(nullptr), m_dwAttackTime(0)
{
	m_eType = POMP;
	m_eRenderId = ENEMY;
}

CPomp::~CPomp()
{
}

void CPomp::Initialize()
{
	Create_Collider();
	Create_DetectCollider();
	Create_RigidBody();
	Set_StageCol();

	m_pCareTaker = new CCaretaker;

	//애니메이션용 insert 이미지 크기, 콜라이더 크기, 콜라이더 offset, 프레임 개수
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_idle.bmp", L"Enemy_pomp_idle", VEC2(66, 84), VEC2(30, 80), VEC2(0, 0), 7, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_walk.bmp", L"Enemy_pomp_walk", VEC2(62, 86), VEC2(30, 80), VEC2(0, 0), 9, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_run.bmp", L"Enemy_pomp_run", VEC2(66, 80), VEC2(30, 80), VEC2(0, 0), 9, 40, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_hurt.bmp", L"Enemy_pomp_hurt", VEC2(102, 82), VEC2(40, 80), VEC2(0, 0), 14, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_attack.bmp", L"Enemy_pomp_attack", VEC2(92, 86), VEC2(20, 72), VEC2(30, 0), 5, 100, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_reflect.bmp", L"Enemy_reflect", VEC2(147, 140), VEC2(0, 0), VEC2(0, 0), 5, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Enemy_follow.bmp", L"Enemy_follow", VEC2(16, 20), VEC2(0, 0), VEC2(0, 0), 0, 200, GetTickCount());

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_idle");
	Set_Frame();

	m_iHp = 1;
	m_vLookDir = { 1.0f, 0.f };

	m_pRigidBody->Set_VelocityLimit(VEC2(500.f, 1000.f));
	m_pRigidBody->Set_Friction(2000.f);
	m_pRigidBody->ResetGravityAccel();

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();

	m_pDetectCollider->Set_Offset(VEC2(10.f, 0.f));
	m_pDetectCollider->Set_Size(VEC2(500.f, 100.f));

	m_bIsPatrol = true;
	m_fPatrolDistance = 200.f;

	m_pBounceEffect = new CEffect();
	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, m_pBounceEffect);
	m_pBounceEffect->Initialize();
	m_pBounceEffect->Set_IsActive(false);
	dynamic_cast<CEffect*>(m_pBounceEffect)->Set_EffectId(EFFECT_END);
	m_pBounceEffect->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Enemy_reflect"));
	m_pBounceEffect->Set_Frame();

	m_dwAttackTime = GetTickCount();
	m_bIsFind = false;
	m_eState = ENEMY_IDLE;
}

int CPomp::Update()
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
		if (!m_bIsFind)
		{
			if (m_bIsPatrol)
			{
				//순찰 몬스터
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_walk");
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
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_idle");
				Set_Frame();
			}
		}

		if (m_bIsFind && !m_bIsAttack && !m_bISBounce )
		{
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_run");
			Set_Frame();

			if (m_pTarget->Get_Pos().x >= m_vPos.x && m_vLookDir.x)
			{
				m_vLookDir.x = 1.f;
			}
			else
			{
				m_vLookDir.x = -1.f;
			}

			if (vTargetDistance.Length() <= 80.f)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_idle");
				Set_Frame();
				m_bIsAttack = true;
				m_pRigidBody->Set_VelocityX(0.f);
				m_dwAttackTime = GetTickCount();

			}
			else
			{
				m_bIsAttack = false;
				m_pRigidBody->Set_VelocityX(200.f * m_vLookDir.x);
			}
		}

		if (m_bIsAttack)
		{
			if (m_dwAttackTime + (1500 * (1 / CTimeMgr::Get_Instance()->Get_TimeScale())) < GetTickCount() && !m_bIsAttacking)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_attack");
				Set_Frame(false);

				m_bIsAttacking = true;
				m_dwAttackTime = GetTickCount();
			}

			if (m_bIsAttacking)
			{
				if (m_vLookDir.x == 1.f)
				{
					if (m_tFrame.iFrameStart >= 2)
					{
						m_bIsBounceAble = true;
					}
					else
					{
						m_bIsBounceAble = false;
					}

					if (m_tFrame.iFrameStart >= 4)
					{
						m_pCollider->Set_IsActive(true);
					}
				}
				else if (m_vLookDir.x == -1.f)
				{
					if (m_tFrame.iFrameStart <= 3)
					{
						m_bIsBounceAble = true;
					}
					else 
					{
						m_bIsBounceAble = false;
					}

					if (m_tFrame.iFrameStart <= 1)
					{
						m_pCollider->Set_IsActive(true);
					}
				}
			}

			if (vTargetDistance.Length() > 85.f)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_idle");
				Set_Frame();

				m_bIsAttack = false;
				m_pRigidBody->Set_VelocityX(0.f);
				m_dwAttackTime = GetTickCount();
				m_bIsAttacking = false;
				m_bIsBounceAble = false;
			}
			if (m_tFrame.isPlayDone && m_bIsAttacking)
			{
				m_pPrevAniBmp = m_pAniBmp;
				m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_idle");
				Set_Frame();

				m_pBounceEffect->Set_IsActive(false);
				m_bIsBounceAble = false;
				m_bIsAttacking = false;
			}
		}

		if (m_bISBounce)
		{
			if (m_dwBounceTime + 2000 < GetTickCount())
			{
				m_bISBounce = false;
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
				m_bIsBounceAble = true;
				m_bISBounce = false;
				m_dwBounceTime = 0;
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

		if (!m_pBlood && !m_bIsBlood)
		{ 		//피 이펙트1
			m_pBlood = new CBloodEffect;
			m_pBlood->Set_LookDir(-m_vLookDir);
			m_pBlood->Initialize();
			m_pBlood->Set_Frame(false);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, m_pBlood);

			//blood 이펙트
			CObj* pBloodBGEffect = new CBloodBGEffect;
			pBloodBGEffect->Set_Pos(m_vPos.x - m_vLookDir.x * 30.f, m_vPos.y - 30.f);
			pBloodBGEffect->Set_LookDir(m_vLookDir);
			pBloodBGEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodBGEffect);

			m_bIsBlood = true;
		}

		if (m_pBlood)
		{
			m_pBlood->Set_Pos(m_vPos.x, m_vPos.y - 30);
			if (m_pBlood->Get_Frame().isPlayDone)
			{
				m_pBlood->Set_Dead();
				m_pBlood = nullptr;
			}
		}
	}

	if (m_dwDetectEffectTime + 600 < GetTickCount() && m_dwDetectEffectTime)
	{
		Safe_Delete<CObj*>(m_pEffect);
		m_dwDetectEffectTime = 0;
	}

	if(m_pEffect)
		m_pEffect->Set_Pos(m_vPos.x - 15 * m_vLookDir.x, m_vPos.y - 50);

	if(m_bIsMoveFrame)
		__super::Move_Frame();

	return 0;
}

void CPomp::Late_Update()
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

void CPomp::Render(HDC hDC)
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

	if (m_pEffect && !m_bDead)
		m_pEffect->Render(hDC);
}

void CPomp::Release()
{
}

void CPomp::OnCollisionEnter(CObj* _pOther)
{
	__super::OnCollisionEnter(_pOther);
	//검에 맞음
	if (((_pOther->Get_Id() == OBJ_EFFECT && dynamic_cast<CEffect*>(_pOther)->Get_EfeectId() == EFFECT_PLAYERATTACK) || _pOther->Get_Id() == OBJ_ITEM) && _pOther->Get_IsActive() && !m_bDead)
	{
		if (m_bIsBounceAble)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"swordcrash.wav", ENEMY_EFFECT1, g_fEffectSound);

			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_hurt");
			Set_Frame(false);

			VEC2 dir{ _pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
			dir.Normalize();

			m_pBounceEffect->Set_Frame(false);
			m_pBounceEffect->Set_LookDir(m_vLookDir);
			m_pBounceEffect->Set_Pos(m_vPos.x, m_vPos.y);
			m_pBounceEffect->Set_IsActive(true);

			m_pRigidBody->AddForce(VEC2(dir.x * 200000.f, -300.f));
			m_bIsGround = false;
			m_bISBounce = true;
			m_bIsBounceAble = false;
			m_bIsAttack = false;
			m_bIsAttacking = false;

			m_dwBounceTime = GetTickCount();
			return;
		}
		else if(!m_bIsBounceAble)
		{
			VEC2 dir{ _pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
			dir.Normalize();

			m_pRigidBody->AddForce(VEC2(dir.x * 200000.f, -500.f));
			m_bIsGround = false;
			m_bDead = true;

			CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", ENEMY_DIE1, g_fEffectSound);

			//hit 이펙트
			CObj* pHitEffect = new CHitEffect;
			pHitEffect->Set_Pos(m_vPos.x, m_vPos.y);
			pHitEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pHitEffect);

			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_hurt");
			Set_Frame(false);
		}

	}
}

void CPomp::OnCollisionStay(CObj* _pOther)
{
	__super::OnCollisionStay(_pOther);
}
