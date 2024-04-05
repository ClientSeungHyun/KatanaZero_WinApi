#include "PlayerState.h"

CPlayerState::CPlayerState()
{
}

CPlayerState::~CPlayerState()
{
}

void CPlayerState::Jump()
{
	if (CKeyMgr::Get_Instance()->Get_KeyState('W') == KEY_DOWN && dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsGround())
	{
		m_pPlayerFsm->ChangeState(JUMP);
	}
}

void CPlayerState::Attack()
{
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_LBUTTON) == KEY_DOWN) 
	{
		m_pPlayerFsm->ChangeState(ATTACK);
	}
}
