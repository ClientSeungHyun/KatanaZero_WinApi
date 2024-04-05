#include "BossHurt.h"

#include "Collider.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CBossHurt::CBossHurt()
{
	m_eState = BOSS_HURT;
}

CBossHurt::~CBossHurt()
{
}

void CBossHurt::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_HURT);

	CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir() * -1);
	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Hurtrecover"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
	CObjMgr::Get_Instance()->Get_Boss()->Get_RigidBody()->AddForce(VEC2(200.f * CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x, 0.f));

	CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_hurt_3.wav", BOSS_DIESOUND, g_fEffectSound);

	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(false);
	m_dwDisappearTime = 0;
}

void CBossHurt::Update()
{
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone)
	{
		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1)
		{
			if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_Hp() == 1 || dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_Hp() == 2)
			{
				if (CObjMgr::Get_Instance()->Get_Boss()->Get_IsActive())
				{
					dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Create_Enemy();
				}
				CObjMgr::Get_Instance()->Get_Boss()->Set_IsActive(false);

				for (auto iter = CObjMgr::Get_Instance()->Get_EnemyList()->begin(); iter != CObjMgr::Get_Instance()->Get_EnemyList()->end(); ++iter)
				{
					if (!(*iter)->Get_Dead())
						return;

					if(!m_dwDisappearTime)
						m_dwDisappearTime = GetTickCount();
				}

				if (m_dwDisappearTime + 2000 < GetTickCount())
				{
					CObjMgr::Get_Instance()->Get_Boss()->Get_RigidBody()->CalcFriction();
					CObjMgr::Get_Instance()->Get_Boss()->Get_RigidBody()->Set_Velocity(VEC2(0.f, 0.f));
					m_pBossFsm->ChangeState(BOSS_DISAPPEAR);
				}
			}

			if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_Hp() == 0)
			{

				CObjMgr::Get_Instance()->Get_Boss()->Set_IsActive(false);
				CSceneMgr::Get_Instance()->Get_NowScene()->Set_IsClear(true);
			}
		}


		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS2)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Get_RigidBody()->CalcFriction();
			CObjMgr::Get_Instance()->Get_Boss()->Get_RigidBody()->Set_Velocity(VEC2(0.f, 0.f));
			m_pBossFsm->ChangeState(BOSS_DISAPPEAR);
		}

	}
}

void CBossHurt::Release()
{
}
