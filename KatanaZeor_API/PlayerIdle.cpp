#include "Player.h"
#include "PlayerIdle.h"

#include "PlayerFSM.h"

#include "RigidBody.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"

CPlayerIdle::CPlayerIdle()
{
	m_eState = IDLE;
}

CPlayerIdle::~CPlayerIdle()
{
}

void			CPlayerIdle::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(IDLE);

	switch (m_pPlayerFsm->Get_PreState()->Get_State())
	{
	case RUN:
		CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_run_to_Idle"));
		CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);

		break;
	default:
		break;
	}

}

void			CPlayerIdle::Update()
{
	if (m_pPlayerFsm->Get_PreState()->Get_State() == RUN)
	{
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone)
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle"));
			CObjMgr::Get_Instance()->Get_Player()->Set_Frame();
		}
	}
	else if(CObjMgr::Get_Instance()->Get_Player()->Get_AniBmp() != CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle"))
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle"));
		CObjMgr::Get_Instance()->Get_Player()->Set_Frame();
	}


	//¸¶Âû·Â
	CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->CalcFriction();

	//ÁÂ¿ì µ¿½Ã¿¡ ´­¸² Ã³¸®
	if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING && CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)
	{
		CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->Set_Velocity(VEC2(0.f, 0.f));

	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING)	//ÁÂÃø
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(-1.0f, 0.f));
		m_pPlayerFsm->ChangeState(RUN);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)		//¿ìÃø
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(1.0f, 0.f));
		m_pPlayerFsm->ChangeState(RUN);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('S') == KEY_DOWN)
	{
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsDownJump(true);
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsGround(false);
		m_pPlayerFsm->ChangeState(FALL);
	}

	Attack();
	Jump();
}

void			CPlayerIdle::Release()
{
	
}
