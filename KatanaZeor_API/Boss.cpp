#include "Boss.h"
#include "BossFSM.h"
#include "BossHead.h"

#include "Bold.h"
#include "Grunt.h"
#include "Pomp.h"
#include "Shieldcop.h"

#include "BossIdle.h"
#include "BossLaserGround.h"
#include "BossBulletJump.h"
#include "BossShootBomb.h"
#include "BossDisappear.h"
#include "BossBlinkLaser.h"
#include "BossHurt.h"
#include "BossDie.h"

#include "Effect.h"
#include "HitEffect.h"
#include "BloodBGEffect.h"
#include "BloodEffect.h"

#include "RigidBody.h"
#include "Collider.h"
#include "Caretaker.h"
#include "Camera.h"

#include "BmpMgr.h"
#include "SoundMgr.h"
#include "AbstractFactory.h"

CBoss::CBoss() :m_bIsRenderEffect(false), m_eState(BOSS_IDLE), m_iHp(3),m_pBossFSM(nullptr), m_bIsBlood(false)
{
	m_eRenderId = ENEMY;
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize()
{
	Create_Collider();
	Create_RigidBody();

	m_pCareTaker = new CCaretaker;

	//애니메이션용 insert 이미지 크기, 콜라이더 크기, 콜라이더 offset, 프레임 개수
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_idle.bmp", L"Boss_Idle", VEC2(44, 88), VEC2(30, 85), VEC2(3, 0), 10, 100, GetTickCount()); 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_drawgun.bmp", L"Boss_Drawgun", VEC2(76, 86), VEC2(30, 85), VEC2(3, 0), 7, 80, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_aim.bmp", L"Boss_Aim", VEC2(78, 104), VEC2(30, 90), VEC2(-10, 0), 0, 99999, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_prejump.bmp", L"Boss_Prejump", VEC2(66, 86), VEC2(30, 90), VEC2(-10, 0), 2, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_jump.bmp", L"Boss_Jump", VEC2(54, 88), VEC2(30, 90), VEC2(-10, 0), 0, 9900, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_wallgrab.bmp", L"Boss_Wallgrab", VEC2(60, 86), VEC2(30, 90), VEC2(0, 0), 2, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_walljump.bmp", L"Boss_WallJump", VEC2(108, 124), VEC2(30, 90), VEC2(0, 0), 6, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_land.bmp", L"Boss_Land", VEC2(66, 86), VEC2(30, 90), VEC2(0, 0), 3, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_takeoutgun.bmp", L"Boss_TakeoutGun", VEC2(94, 86), VEC2(30, 90), VEC2(0, 0), 6, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_shoot.bmp", L"Boss_Shoot", VEC2(136, 92), VEC2(30, 90), VEC2(-30, 0), 7, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_predash.bmp", L"Boss_Predash", VEC2(114, 86), VEC2(30, 90), VEC2(-15, 0), 7, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_dash.bmp", L"Boss_Dash", VEC2(102, 100), VEC2(30, 90), VEC2(-0, 0), 0, 99999, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_dashend.bmp", L"Boss_Dashend", VEC2(100, 92), VEC2(30, 90), VEC2(0, 0), 9, 150, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_outpattern.bmp", L"Boss_OutPattern", VEC2(72, 62), VEC2(30, 90), VEC2(0, 0), 3, 50, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_teleport_ground.bmp", L"Boss_Teleportground", VEC2(82, 70), VEC2(36, 60), VEC2(0, 0), 7, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_teleport_sky.bmp", L"Boss_Teleportsky", VEC2(56, 90), VEC2(36, 60), VEC2(0, 0), 7, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_hurtrecover.bmp", L"Boss_Hurtrecover", VEC2(140, 106), VEC2(30, 90), VEC2(-15, 0), 12, 150, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/boss_dieground.bmp", L"Boss_dieground", VEC2(116, 82), VEC2(30, 90), VEC2(-15, 0), 7, 400, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/boss_dead.bmp", L"boss_dead", VEC2(112, 42), VEC2(30, 90), VEC2(0, 30), 18, 300, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/boss_nohead.bmp", L"boss_nohead", VEC2(120, 44), VEC2(30, 90), VEC2(0, 30), 5, 100, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_aimlazer.bmp", L"Boss_Aimlazer", VEC2(1000, 2), VEC2(1000, 3), VEC2(0, 0), 0, 99999, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_aimlazer2.bmp", L"Boss_Aimlazer2", VEC2(2, 1000), VEC2(3, 1000), VEC2(0, 0), 0, 99999, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_lazer3.bmp", L"Boss_Lazer3", VEC2(2000, 50), VEC2(2000, 40), VEC2(0, 0), 0, 99999, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/Boss_lazer5.bmp", L"Boss_Lazer5", VEC2(50, 2000), VEC2(40, 2000), VEC2(0, 0), 0, 99999, GetTickCount());

	Init_Boss();

	m_eId = OBJ_BOSS;
}

int CBoss::Update()
{
	if (CSceneMgr::Get_Instance()->Get_SceneState() != SC_PLAYING)
		return 0;

	if (m_eState == BOSS_REALDIE)
		return 0;

	if (!g_bIsTimeBack)
	{
			m_pBossFSM->Update();
	}
	else
	{
		for (int i = 0; i < 20; ++i)
		{
			if (m_pCareTaker->Get_ListSize() > 0)
			{
				Set_MementoState(m_pCareTaker->TimeBakc());
			}
			else
			{
				TimeBakcDone();
			}
		}
	}

	return 0;
}

void CBoss::Late_Update()
{
	__super::Move_Frame();

	if (m_eState != BOSS_REALDIE)
	{
		m_pCollider->Late_Update();

		m_pRigidBody->Late_Update();
		if (!g_bIsTimeBack)
		{

			m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
			m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

			m_pCareTaker->Add(Save_State());
		}
	}
}

void CBoss::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hMemDC = m_pAniBmp->Get_MemDC();

	if (m_bIsActive)
	{
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
	}

}

void CBoss::Release()
{
	if (m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);

	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

	if (m_pBossFSM)
		Safe_Delete<CBossFSM*>(m_pBossFSM);

	if (m_pCareTaker)
		Safe_Delete<CCaretaker*>(m_pCareTaker);
}

void CBoss::OnCollisionEnter(CObj* _pOther)
{
	//검에 맞음
	if (_pOther->Get_Id() == OBJ_EFFECT && _pOther->Get_IsActive() && !m_bDead && dynamic_cast<CEffect*>(_pOther)->Get_EfeectId() == EFFECT_PLAYERATTACK)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", ENEMY_DIE1, g_fEffectSound);

		//hit 이펙트
		CObj* pHitEffect = new CHitEffect;
		pHitEffect->Set_Pos(m_vPos.x, m_vPos.y);
		pHitEffect->Initialize();
		CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pHitEffect);

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


		if (m_iHp <= -1)
		{
			m_eState = BOSS_REALDIE;
			m_pCollider->Set_IsActive(false);

			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"boss_nohead");
			m_pRigidBody->CalcFriction();
			Set_Frame(false);

			CObj* pHead = new CBossHead;
			pHead->Set_Pos(m_vPos.x, m_vPos.y - 50.f);
			pHead->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pHead);
			return;
		}

		VEC2 dir{ _pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
		dir.Normalize();
		m_iHp--;
		m_pCollider->Set_IsActive(false);

		if (m_iHp > 0)
			Boss_Hit();

		if (m_iHp <= 0 && CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS2)
			Boss_Die();
	}
}

void CBoss::OnCollisionStay(CObj* _pOther)
{
}

void CBoss::OnCollisionExit(CObj* _pOther)
{
}

void CBoss::Init_Boss()
{
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1)
		m_iHp = 3;
	else
		m_iHp = 4;

	m_vLookDir = { -1.0f, 0.f };

	m_pRigidBody->Set_VelocityLimit(VEC2(500.f, 1000.f));
	m_pRigidBody->Set_Friction(2000.f);
	m_pRigidBody->ResetGravityAccel();

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Idle");

	m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset());
	m_pCollider->Set_Size(m_pAniBmp->Get_ColliderSize());

	//FSM 객체 생성 및 상태 추가
	m_pBossFSM = new CBossFSM;
	m_pBossFSM->AddState(new CBossIdle);
	m_pBossFSM->AddState(new CBossLaserGround);
	m_pBossFSM->AddState(new CBossBulletJump);
	m_pBossFSM->AddState(new CBossShootBomb);
	m_pBossFSM->AddState(new CBossDisappear);
	m_pBossFSM->AddState(new CBossBlinkLaser);
	m_pBossFSM->AddState(new CBossHurt);
	m_pBossFSM->AddState(new CBossDie);

	m_pBossFSM->Set_State(BOSS_IDLE);
}

void CBoss::Boss_Hit()
{
	m_pBossFSM->ChangeState(BOSS_HURT);  
}

void CBoss::Boss_Die()
{
	m_iHp--;
	m_pBossFSM->ChangeState(BOSS_DIE);
}

void CBoss::TimeBakcDone()
{
	if(CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1)
		m_iHp = 3;
	else
		m_iHp = 4;

	m_pCollisionOthers.clear();
	m_bDead = false;

	if (m_pCareTaker->Get_ListSize() > 0)
	{
		Set_MementoState(m_pCareTaker->Get_First());
	}

	m_bIsBlood = false;
	m_pCareTaker->Clear_MementoList();
	m_pBossFSM->Set_State(BOSS_IDLE);
}

void CBoss::Create_Enemy()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(230.f, 470.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CPomp>::Create_Monseter(530.f, 470.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CShieldcop>::Create_Monseter(900.f, 470.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(1160.f, 470.f, false));
}
