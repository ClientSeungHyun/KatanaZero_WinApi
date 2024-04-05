#include "BossStage01.h"
#include "Player.h"
#include "Boss.h"
#include "BombBulletEffect.h"
#include "CPlayerText.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Camera.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CBossStage01::CBossStage01():m_dwExplosionTime(0), m_bIsDestroyGround(false), m_iExplosionCount(0)
{
}

CBossStage01::~CBossStage01()
{
	Release();
}

void CBossStage01::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1.bmp", L"BossStage01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1_Destroy.bmp", L"BossStage01_Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1_slow.bmp", L"BossStage01_slow");

	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_Pos(253.f, 485.f);
	pPlayer->Initialize();

	CBoss* pBoss = new CBoss;
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, pBoss);
	pBoss->Initialize();
	pBoss->Set_Pos(1044, 470);

	//카메라 설정
	CCamera::Get_Instance()->SetLookAtPos(WINCX * 0.5f, WINCY * 0.5f);
	CCamera::Get_Instance()->SetMaxPos(VEC2(WINCX * 0.5f, WINCY * 0.5f));
	CCamera::Get_Instance()->SetMinPos(VEC2(WINCX * 0.5f, WINCY * 0.5f));

	if (!m_pText)
	{
		m_pText = CAbstractFactory<CPlayerText>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pText);
	}

	m_dwExplosionTime = GetTickCount();
}

int CBossStage01::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (m_bIsClear)
	{
		if (m_dwExplosionTime + 50 < GetTickCount() && m_iExplosionCount < 40)
		{
			VEC2 vBombPos = { 150.f + +float(m_iExplosionCount * 40) , 600.f };
			CObj* pExplosion = new CBombBulletEffect;
			pExplosion->Set_Pos(vBombPos.x , vBombPos.y);
			pExplosion->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pExplosion);
			pExplosion->Set_IsActive(false);

			m_iExplosionCount++;
			m_dwExplosionTime = GetTickCount();
		}


		//폭발 종료 시 배경 이미지 교체
		if (m_iExplosionCount >= 30)
		{
			m_bIsDestroyGround = true;
		}
	}
	
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Dead() && !g_bIsTimeBack)
	{
		m_pText->Set_IsActive(true);
	}
	else
	{
		m_pText->Set_IsActive(false);
	}

	return 0;
}

void CBossStage01::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CBossStage01::Render(HDC hDC)
{
	VEC2 vWindowPos = { 0,0 };
	VEC2 vRenderPos = CCamera::Get_Instance()->GetRenderPos(vWindowPos.x, vWindowPos.y);

	if (!m_bIsDestroyGround)
	{
		if (!CSceneMgr::Get_Instance()->Get_IsSlow())
		{
			HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"BossStage01");
			BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 1394, 784, hBackGroundDc, 0, 0, SRCCOPY);
		}
		else
		{
			HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"BossStage01_slow");
			BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 1394, 784, hBackGroundDc, 0, 0, SRCCOPY);
		}
	}
	else
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"BossStage01_Destroy");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 1394, 784, hBackGroundDc, 0, 0, SRCCOPY);
	}

	CObjMgr::Get_Instance()->Render(hDC);
}

void CBossStage01::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
}
