#include "BossLaserGround.h"
#include "Camera.h"
#include "CLaserBullet.h"
#include "Collider.h"

CBossLaserGround::CBossLaserGround() : m_dwAimTime(0), m_dwShootTime(0), m_pLaserBullet(nullptr)
{
	m_eState = BOSS_LASERGROUND;
}

CBossLaserGround::~CBossLaserGround()
{
	Release();
}

void CBossLaserGround::Initialize()
{
	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_LASERGROUND);
	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(true);

	m_dwAimTime = 0;
	m_dwShootTime = 0;

	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Drawgun"));
	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);

	m_vEffectPos = CObjMgr::Get_Instance()->Get_Boss()->Get_Pos();

	if (!m_pLaserBullet)
	{
		m_pLaserBullet = new CLaserBullet;
		m_pLaserBullet->Initialize();
		dynamic_cast<CLaserBullet*>(m_pLaserBullet)->Set_bIsGroundLaser(true);
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, m_pLaserBullet);
	}

	m_pLaserBullet->Set_IsActive(false);
}

void CBossLaserGround::Update()
{
	//ÃÑÀ» ²¨³Â´Ù
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && !m_dwAimTime)
	{
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Aim"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);

		m_pLaserBullet->Set_Pos(m_vEffectPos.x + (540 * CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x), m_vEffectPos.y - 8);
		dynamic_cast<CLaserBullet*>(m_pLaserBullet)->Set_ShootPos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos());
		m_pLaserBullet->Set_IsActive(true);

		m_dwAimTime = GetTickCount();
	}

	if (!m_pLaserBullet->Get_IsActive() && !m_pLaserBullet->Get_Collider()->Get_IsActive() && m_dwAimTime)
	{
		m_pBossFsm->ChangeState(BOSS_DISAPPEAR);
	}

}

void CBossLaserGround::Release()
{
}
