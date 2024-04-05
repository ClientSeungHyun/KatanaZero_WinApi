#include "PlayerHurt.h"

CPlayerHurt::CPlayerHurt()
{
	m_eState = HURT;
}

CPlayerHurt::~CPlayerHurt()
{
}

void CPlayerHurt::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(HURT);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsFall(true);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsGround(false);

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_hurtfly"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame();

	VEC2 vFlyDir = CObjMgr::Get_Instance()->Get_Player()->Get_LookDir();


	if (CTimeMgr::Get_Instance()->Get_TimeGage() - CTimeMgr::Get_Instance()->Get_WorldTime() <= 0)
	{
		CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->AddForce(VEC2(0.f, -10000.f));
	}
	else
	{
		CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->AddForce(VEC2(vFlyDir.x * -200000.f, -20000.f));
	}

	CSoundMgr::Get_Instance()->PlaySound(L"playerdie.wav", SOUND_DIE, g_fEffectSound);
	m_bIsDead = false;
}

void CPlayerHurt::Update()
{
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsGround() == true && !m_bIsDead)
	{
		m_bIsDead = true;
		CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone = true;
		CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_hurtground"));
		CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);
	}
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsGround() == true && m_bIsDead)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_Dead();
		CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->CalcFriction();
	}
}

void CPlayerHurt::Release()
{
}
