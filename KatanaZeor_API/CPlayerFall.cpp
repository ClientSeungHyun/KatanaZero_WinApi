#include "CPlayerFall.h"

CPlayerFall::CPlayerFall()
{
	m_eState = FALL;
}

CPlayerFall::~CPlayerFall()
{
}

void CPlayerFall::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(FALL);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsFall(true);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsGround(false);

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_fall"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame();
}

void CPlayerFall::Update()
{

	if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(-1.0f, 0.f));
		CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->Set_VelocityX(-300.f);

		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsWallGrab())
		{
			m_pPlayerFsm->ChangeState(WALL_GRAB);
		}
	}

	if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(1.0f, 0.f));
		CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->Set_VelocityX(300.f);

		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsWallGrab())
		{
			m_pPlayerFsm->ChangeState(WALL_GRAB);
		}
	}

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsGround() == true)
	{
		CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone = true;

		if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING || CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)
		{
			m_pPlayerFsm->ChangeState(RUN);
		}
		else
		{
			m_pPlayerFsm->ChangeState(IDLE);
		}
	}

	Attack();
}

void CPlayerFall::Release()
{
}
