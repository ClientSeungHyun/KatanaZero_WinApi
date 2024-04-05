#include "PlayerRoll.h"

CPlayerRoll::CPlayerRoll()
{
	m_eState = ROLL;
}

CPlayerRoll::~CPlayerRoll()
{
}

void CPlayerRoll::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(ROLL); 

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_roll"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);

	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"player_roll.wav", SOUND_EFFECT, g_fEffectSound);
}

void CPlayerRoll::Update()
{
	CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->Set_VelocityX(500.f * CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x);


	if (CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone)
	{
		if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING)	//ÁÂÃø
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(-1.0f, 0.f));
			m_pPlayerFsm->ChangeState(RUN);
		}
		else if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)		//¿ìÃø
		{
			CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(1.0f, 0.f));
			m_pPlayerFsm->ChangeState(RUN);
		}
		else
		{
			m_pPlayerFsm->ChangeState(IDLE);
		}
		
	}

	Attack();
}

void CPlayerRoll::Release()
{
}
