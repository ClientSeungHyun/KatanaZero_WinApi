#include "BossDisappear.h"

#include "SceneMgr.h"
#include "Collider.h"
#include "BmpMgr.h"

CBossDisappear::CBossDisappear()
{
	m_eState = BOSS_DISAPPEAR;
	srand((unsigned)time(NULL));
}

CBossDisappear::~CBossDisappear()
{
}

void CBossDisappear::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_DISAPPEAR);
	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_OutPattern"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);

	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(false);

	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1)
	{
		m_vRightPos = { 1044, 470 };
		m_vLeftPos = { 240, 470 };
	}
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS2)
	{
		m_vRightPos = { 1044, 530 };
		m_vLeftPos = { 245, 530 };
	}

	Choice_Pattern();
	m_dwAppearTime = GetTickCount();
}

void CBossDisappear::Update()
{
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone)
	{
		if (m_eNextState == BOSS_BULLETJUMP)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vRightPos.x, m_vRightPos.y);

			//CObjMgr::Get_Instance()->Get_Boss()->Set_IsActive(true);
			m_pBossFsm->ChangeState(BOSS_BULLETJUMP);
		}

		if (m_eNextState == BOSS_LASERGROUND)
		{
			if (rand() % 2)
			{
				CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vRightPos.x, m_vRightPos.y);
				CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(-1.f, 0.f));
			}
			else
			{
				CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vLeftPos.x, m_vLeftPos.y);
				CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(1.f, 0.f));
			}

			CObjMgr::Get_Instance()->Get_Boss()->Set_IsActive(true);
			m_pBossFsm->ChangeState(BOSS_LASERGROUND);
		}

		if (m_eNextState == BOSS_LASERBLINK)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_IsActive(true);
			m_pBossFsm->ChangeState(BOSS_LASERBLINK);
		}

		if (m_eNextState == BOSS_SHOOTBOMB)
		{
			if (rand() % 2)
			{
				CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vRightPos.x, m_vRightPos.y);
				CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(-1.f, 0.f));
			}
			else
			{
				CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vLeftPos.x, m_vLeftPos.y);
				CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(1.f, 0.f));
			}

			CObjMgr::Get_Instance()->Get_Boss()->Set_IsActive(true);
			m_pBossFsm->ChangeState(BOSS_SHOOTBOMB);
		}
	}
}

void CBossDisappear::Release()
{
}

void CBossDisappear::Choice_Pattern()
{
	int iRandPattern = rand() % 4;

	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1)
		iRandPattern = 0;

	if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_Hp() > 2 && iRandPattern == 2)
		iRandPattern -= 1;



	switch (iRandPattern)
	{
	case 0:
		m_eNextState = BOSS_LASERGROUND;
		break;

	case 1:
		m_eNextState = BOSS_LASERBLINK;
		break;

	case 2:
		m_eNextState = BOSS_SHOOTBOMB;
		break;

	case 3:
		m_eNextState = BOSS_BULLETJUMP;
		break;

	default:
		break;
	}
	
}
