#include "PlayerFlip.h"

CPlayerFlip::CPlayerFlip()
{
	m_eState = FLIP;
}

CPlayerFlip::~CPlayerFlip()
{
}

void CPlayerFlip::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(FLIP);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsFall(false);

	m_fFlipAngel = 50;
	//바라보는 방향을 반대로 하기
	CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(CObjMgr::Get_Instance()->Get_Player()->Get_LookDir() * -1);

	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsFall(false);

	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"player_roll.wav", SOUND_EFFECT, g_fEffectSound);

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_flip"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);

}

void CPlayerFlip::Update()
{

	//x방향 이동
	CObjMgr::Get_Instance()->Get_Player()->Set_PosX(CObjMgr::Get_Instance()->Get_Player()->Get_Pos().x +
		cosf(m_fFlipAngel * (PI / 180.f) * CTimeMgr::Get_Instance()->Get_DeltaTime() * CTimeMgr::Get_Instance()->Get_TimeScale()) *13 * 
		CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x);

	//y방향 이동
	CObjMgr::Get_Instance()->Get_Player()->Set_PosY(CObjMgr::Get_Instance()->Get_Player()->Get_Pos().y -
		(sinf(m_fFlipAngel * (PI / 180.f) * CTimeMgr::Get_Instance()->Get_DeltaTime() * CTimeMgr::Get_Instance()->Get_TimeScale()) *500));
	if (CKeyMgr::Get_Instance()->Get_KeyState('W') == KEY_PRESSING &&
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsWallGrab())
	{
		m_pPlayerFsm->ChangeState(WALL_GRAB);
	}

	if (CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone)
	{
		m_pPlayerFsm->ChangeState(FALL);
	}

	Attack();
}

void CPlayerFlip::Release()
{
}