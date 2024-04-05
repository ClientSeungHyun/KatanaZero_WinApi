#include "BossDie.h"

#include "ObjMgr.h"
#include "Collider.h"
#include "SoundMgr.h"

CBossDie::CBossDie():m_bDieGround(false), m_bSneakGround(false)
{
	m_eState = BOSS_DIE;
}

CBossDie::~CBossDie()
{
}

void CBossDie::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_DIE);

	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(false);

	CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(1.0f,0.f));
	CSoundMgr::Get_Instance()->StopAll();
	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_dieground"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
	CTimeMgr::Get_Instance()->Set_TimeScale(0.2f);
	//CObjMgr::Get_Instance()->Get_Boss()->Get_RigidBody()->AddForce(VEC2(200.f * CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x, 0.f));

	CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_floorhit_01.wav", BOSS_DIESOUND, g_fEffectSound);
}

void CBossDie::Update()
{
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && !m_bDieGround)
	{
		
		m_bDieGround = true;
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"boss_dead"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame();
		CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x, CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y + 20.f);
		CTimeMgr::Get_Instance()->Set_TimeScale(1.0f);
	}

	if (m_bDieGround)
	{
		CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(true);
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Pos().x >= CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(1.0f, 0.f));
		}
		else
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(-1.0f, 0.f));
		}

		CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x + CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x * 0.1f);
	}

	
}

void CBossDie::Release()
{
}
