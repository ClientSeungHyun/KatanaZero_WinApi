#include "BossIdle.h"

#include "Collider.h"


CBossIdle::CBossIdle()
{
	m_eState = BOSS_IDLE;
}

CBossIdle::~CBossIdle()
{
}

void CBossIdle::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_IDLE);
	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(true);
	
	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Idle"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame();
}

void CBossIdle::Update()
{
	if (CKeyMgr::Get_Instance()->Get_KeyState('L') == KEY_DOWN)
	{
		m_pBossFsm->ChangeState(BOSS_DISAPPEAR);
	}
}

void CBossIdle::Release()
{
}
