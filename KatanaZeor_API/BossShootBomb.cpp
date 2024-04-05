#include "BossShootBomb.h"

#include "Collider.h"
#include "BomoBullet.h"

CBossShootBomb::CBossShootBomb()
{
	m_eState = BOSS_SHOOTBOMB;
}

CBossShootBomb::~CBossShootBomb()
{
	Release();
}

void CBossShootBomb::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_SHOOTBOMB);
	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(true);

	m_iDelayTime = 500;
	m_iShootCount = 3;
	m_bIsShooting = false;

	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_TakeoutGun"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);

	CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x - 10);
	if (m_BossBulletList.size() < 3)
	{
		for (int i = 0; i < 3; ++i)
		{
			CObj* b = new CBomoBullet;
			b->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, b);
			m_BossBulletList.push_back(b);
		}
	}
	m_iMoveforRender = false;
}

void CBossShootBomb::Update()
{
	//ÃÑÀ» ²¨³Â´Ù
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && m_iShootCount > 0)
	{
		m_bIsShooting = true;
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Shoot"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);

		if (!m_iMoveforRender)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x + 30 * (CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x));
			m_iMoveforRender = true;
		}
	}

	if ((CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().iFrameStart == 0 || CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().iFrameStart == 7) && m_iShootCount >= 0 && m_bIsShooting)
	{
		m_BossBulletList[m_iShootCount-1]->Set_IsActive(true);
		m_BossBulletList[m_iShootCount-1]->Set_Pos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x - 20 * (CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x), CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y);
		dynamic_cast<CBomoBullet*>(m_BossBulletList[m_iShootCount - 1])->Set_MoveTime(GetTickCount());
		dynamic_cast<CBomoBullet*>(m_BossBulletList[m_iShootCount-1])->Set_DelayTime(m_iDelayTime);
		m_iDelayTime -= 100;
		m_iShootCount--;
		m_bIsShooting = false;
	}

	if(m_iShootCount <= 0 && CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone)
	{
		CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x + 40);
		m_pBossFsm->ChangeState(BOSS_IDLE);
	}
}

void CBossShootBomb::Release()
{
}
