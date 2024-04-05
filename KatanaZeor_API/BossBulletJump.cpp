#include "BossBulletJump.h"
#include "BaseBullet.h"

#include "Camera.h"
#include "Collider.h"
#include "SoundMgr.h"

CBossBulletJump::CBossBulletJump() :m_dwShootDelayTime(0), m_ShootCount(15), m_bIsPreJump(false), m_bIsShootEnd(false), m_bIsWallGrab(false)
, m_bIsJump(false), m_bIsLand(false)
{
	m_eState = BOSS_BULLETJUMP;
}

CBossBulletJump::~CBossBulletJump()
{
	Release();
}

void CBossBulletJump::Initialize()
{

	m_dwShootDelayTime = 0;
	m_ShootCount = 15;
	m_bIsPreJump = false;
	m_bIsJump = false;
	m_bIsShootEnd = false;
	m_bIsWallGrab = false;
	m_bIsLand = false;
	m_fJumpAngle = 55.f;
	m_fGravity = 0.f;

	m_vWallGrabPoint = { 1150.f,345.f };
	m_vLandingPoint = { 400.f, 530.f };

	dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_BULLETJUMP);

	CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Prejump"));
	CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(false);

	CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);

	CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(-1.0f, 0.f));
}

void CBossBulletJump::Update()
{
	if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && !m_bIsPreJump)
	{
		m_bIsPreJump = true;
		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_jump_01.wav", BOSS_JUMP, g_fEffectSound);
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Jump"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame();
		CObjMgr::Get_Instance()->Get_Boss()->Get_AniBmp()->Set_Time(GetTickCount());
	}
	else if (m_bIsPreJump && !m_bIsJump && CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x >= m_vWallGrabPoint.x )
	{
		m_bIsJump = true;
		CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(CObjMgr::Get_Instance()->Get_Boss()->Get_LookDir().x * -1, 0.f));
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Wallgrab"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
	}
	else if (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && m_bIsJump && !m_bIsWallGrab)
	{
		CObjMgr::Get_Instance()->Get_Boss()->Get_Collider()->Set_IsActive(true);
		m_bIsWallGrab = true;
		m_dwShootDelayTime = GetTickCount();
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_WallJump"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
	}


	//점프 움직임
	if (m_bIsPreJump && !m_bIsJump && CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x != m_vWallGrabPoint.x && CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y != m_vWallGrabPoint.y)
	{

		CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x + cosf(m_fJumpAngle * (PI / 180.f)) * (10 * CTimeMgr::Get_Instance()->Get_TimeScale()));
		CObjMgr::Get_Instance()->Get_Boss()->Set_PosY(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y - sinf(m_fJumpAngle * (PI / 180.f)) * (10 * CTimeMgr::Get_Instance()->Get_TimeScale()));
		
		if (CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x >= m_vWallGrabPoint.x)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_Pos(m_vWallGrabPoint.x, m_vWallGrabPoint.y);
		}
	}

	// 움직임
	if (m_bIsWallGrab && CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x != m_vLandingPoint.x && CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y != m_vLandingPoint.y)
	{
		m_fGravity += ((float)0.40 * CTimeMgr::Get_Instance()->Get_TimeScale());
		m_fJumpAngle = 30;
		//포물선 움직임 추가하기
		CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x - cosf(m_fJumpAngle * (PI / 180.f)) * (16 * CTimeMgr::Get_Instance()->Get_TimeScale()));
		CObjMgr::Get_Instance()->Get_Boss()->Set_PosY(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y - (sinf(m_fJumpAngle * (PI / 180.f)) * 20 * CTimeMgr::Get_Instance()->Get_TimeScale()) + m_fGravity * CTimeMgr::Get_Instance()->Get_TimeScale());
		
		if (CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y >= m_vLandingPoint.y)
		{
			CObjMgr::Get_Instance()->Get_Boss()->Set_PosY(m_vLandingPoint.y);
		}

		CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_gatling_01.wav", BOSS_GATLING, g_fEffectSound);
		//총알 발사
		if ( m_ShootCount >= 0 && (CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().iFrameStart == 2 || CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().iFrameStart == 3))
		{
			CObj* bullet = new CBaseBullet;
			dynamic_cast<CBaseBullet*>(bullet)->Set_Angle(320.f - (m_ShootCount * 7));
			bullet->Initialize();
			bullet->Set_Pos(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x, CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().y);

			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, bullet);
			m_ShootCount--;
			m_dwShootDelayTime = GetTickCount();
		}
	}

	if (m_ShootCount <= 0 && CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone && !m_bIsLand)
	{
		m_bIsLand = true;
		CObjMgr::Get_Instance()->Get_Boss()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Boss_Land"));
		CObjMgr::Get_Instance()->Get_Boss()->Set_Frame(false);
		CObjMgr::Get_Instance()->Get_Boss()->Set_LookDir(VEC2(1.0f, 0.f));
	}

	if (m_bIsLand && CObjMgr::Get_Instance()->Get_Boss()->Get_Frame().isPlayDone)
	{
		CObjMgr::Get_Instance()->Get_Boss()->Set_PosX(CObjMgr::Get_Instance()->Get_Boss()->Get_Pos().x - 10);
		m_pBossFsm->ChangeState(BOSS_LASERGROUND);
	}
}

void CBossBulletJump::Release()
{
}
