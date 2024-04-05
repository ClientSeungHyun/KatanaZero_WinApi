#include "CPlayerWallGrab.h"

CPlayerWallGrab::CPlayerWallGrab()
{
	m_eState = WALL_GRAB;
}

CPlayerWallGrab::~CPlayerWallGrab()
{
}

void CPlayerWallGrab::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(WALL_GRAB);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsGround(false);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsFall(false);

	if (m_pPlayerFsm->Get_PreState()->Get_State() == JUMP)
	{

	}

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_grab_wall"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);

	m_vStartPos = CObjMgr::Get_Instance()->Get_Player()->Get_Pos();
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_RigidBody()->Set_VelocityY(0.f);
	CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->ResetGravityAccel();
}

void CPlayerWallGrab::Update()
{

	//키 상태 처리
	if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING && CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x == -1)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_PosX(m_vStartPos.x - 11);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_UP && CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x == -1)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(-1.0f, 0.f));
		m_pPlayerFsm->ChangeState(FALL);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING && CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x == 1)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_PosX(m_vStartPos.x + 11);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_UP && CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x == 1)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(1.0f, 0.f));
		m_pPlayerFsm->ChangeState(FALL);
	}


	//flip으로 전환
	if (CKeyMgr::Get_Instance()->Get_KeyState('W') == KEY_DOWN)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsWallGrab(false);
		m_pPlayerFsm->ChangeState(FLIP);
	}

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsGround())
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsWallGrab(false);
		m_pPlayerFsm->ChangeState(IDLE);
	}

}

void CPlayerWallGrab::Release()
{
}
