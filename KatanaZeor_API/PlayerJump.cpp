#include "PlayerJump.h"
#include "Effect.h"

CPlayerJump::CPlayerJump():m_pJumpEffect(nullptr)
{
	m_eState = JUMP;
}

CPlayerJump::~CPlayerJump()
{
}

void CPlayerJump::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(JUMP);
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsGround(false);

	m_fJumpMax = 120.f;
	m_fJumpSpeed = 20.f;

	m_fStartPosY = CObjMgr::Get_Instance()->Get_Player()->Get_Pos().y;
	
	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"player_jump.wav", SOUND_EFFECT, g_fEffectSound);

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_jump"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);

	if (!m_pJumpEffect)
	{
		m_pJumpEffect = new CEffect();
		dynamic_cast<CEffect*>(m_pJumpEffect)->Set_EffectId(EFFECT_END);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, m_pJumpEffect);
		m_pJumpEffect->Initialize();
		m_pJumpEffect->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_jumpdust"));
	}
	m_pJumpEffect->Set_IsActive(true);
	m_pJumpEffect->Set_Pos(CObjMgr::Get_Instance()->Get_Player()->Get_Pos().x, CObjMgr::Get_Instance()->Get_Player()->Get_Pos().y + CObjMgr::Get_Instance()->Get_Player()->Get_Size().y * 0.5f);
	m_pJumpEffect->Set_Frame(false);
	m_pJumpEffect->Set_LookDir(VEC2(1.0f,0.f));
}

void CPlayerJump::Update()
{
	if (m_pJumpEffect->Get_Frame().isPlayDone)
		m_pJumpEffect->Set_IsActive(false);

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

	//하락 조건 추가하기(천장에 닿으면 isfall true) 다른 조건 정하기
	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsFall())
	{
		//CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_jumpdust"));
		//CObjMgr::Get_Instance()->Get_Player()->Set_Frame();
	}

	if (m_fJumpSpeed <= m_fJumpMax)
	{
		m_fJumpSpeed += (8 * CTimeMgr::Get_Instance()->Get_TimeScale());
		CObjMgr::Get_Instance()->Get_Player()->Set_PosY((m_fStartPosY - m_fJumpSpeed) );
		m_fMaxPosY = CObjMgr::Get_Instance()->Get_Player()->Get_Pos().y;
		m_dwJumpTime = GetTickCount();
	}
	else
	{
		m_pPlayerFsm->ChangeState(FALL);
	}

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsFall())
	{
		m_pPlayerFsm->ChangeState(FALL);
	}
	
	Attack();
}

void CPlayerJump::Release()
{
}
