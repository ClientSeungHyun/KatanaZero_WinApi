#include "BossBlinkLaser.h"
#include "CLaserBullet.h"

#include "Collider.h"
#include "Collider.h"

CBossBlinkLaser::CBossBlinkLaser()
{
	m_eState = BOSS_LASERBLINK;
}

CBossBlinkLaser::~CBossBlinkLaser()
{
}

void CBossBlinkLaser::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_LASERBLINK);

	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(false);
	//초기화
	if (m_vTeleportPosList.empty())
	{
		//상 4개
		m_vTeleportPosList.push_back(VEC2(220, 250));
		m_vTeleportPosList.push_back(VEC2(1105, 250));
		m_vTeleportPosList.push_back(VEC2(480, 250));
		m_vTeleportPosList.push_back(VEC2(900, 250));
		//좌우
		m_vTeleportPosList.push_back(VEC2(190, 535));
		m_vTeleportPosList.push_back(VEC2(1140, 535));

		for (int i = 0; i < 3; ++i)
		{
			m_pLaserBullet[i] = new CLaserBullet;
			m_pLaserBullet[i]->Initialize();
			m_pLaserBullet[i]->Set_IsActive(false);
			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, m_pLaserBullet[i]);
		}
	}

	m_iTeleportCount = 0;
	CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vTeleportPosList[m_iTeleportCount].x, m_vTeleportPosList[m_iTeleportCount].y);
	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Teleportsky"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
}

void CBossBlinkLaser::Update()
{
	if ((int)CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x <= 200)
		CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(1.f, 0.f));
	else
		CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(-1.f, 0.f));

	//발사 경로
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().iFrameStart == 2)
	{
		if (m_vTeleportPosList[m_iTeleportCount].y == 535)
		{
			dynamic_cast<CLaserBullet*>(m_pLaserBullet[m_iTeleportCount % 3])->Set_bIsGroundLaser(true);
			m_pLaserBullet[m_iTeleportCount % 3]->Set_Pos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x + (540 * CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x), CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y - 8);
			dynamic_cast<CLaserBullet*>(m_pLaserBullet[m_iTeleportCount % 3])->Set_ShootPos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos());
		}

		if (m_vTeleportPosList[m_iTeleportCount].y == 250)
		{
			dynamic_cast<CLaserBullet*>(m_pLaserBullet[m_iTeleportCount % 3])->Set_bIsGroundLaser(false);
			m_pLaserBullet[m_iTeleportCount % 3]->Set_Pos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x + 10, CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y + 540.f);
			dynamic_cast<CLaserBullet*>(m_pLaserBullet[m_iTeleportCount % 3])->Set_ShootPos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos());
		}

		m_pLaserBullet[m_iTeleportCount % 3]->Set_IsActive(true);
	}

	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && m_iTeleportCount < m_vTeleportPosList.size() - 1)
	{
		m_iTeleportCount += 1;
		if ((int)m_vTeleportPosList[m_iTeleportCount].y == 535)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Teleportground"));
			CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
		}
		else
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Teleportsky"));
			CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
		}

		//레이저 발사 위치를 초반에 받아올 수 있도록 조정하기
		CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vTeleportPosList[m_iTeleportCount].x, m_vTeleportPosList[m_iTeleportCount].y);
	}
	else if (m_iTeleportCount == 5 && CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone)
	{
		m_pBossFsm->ChangeState(BOSS_DISAPPEAR);
	}
}

void CBossBlinkLaser::Release()
{
}
