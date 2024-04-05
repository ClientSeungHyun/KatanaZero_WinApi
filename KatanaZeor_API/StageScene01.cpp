#include "StageScene01.h"
#include "Player.h"
#include "PlayerShadow.h"
#include "Bold.h"
#include "Grunt.h"
#include "Pomp.h"
#include "Shieldcop.h"

#include "CInvenUI.h"
#include "CPlayerText.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Camera.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CStageScene01::CStageScene01()
{
}

CStageScene01::~CStageScene01()
{
	Release();
}

void CStageScene01::Initialize()
{
	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_Pos(200.f, 495.f);
	pPlayer->Initialize();

#pragma region 몬스터 생성
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(900.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CPomp>::Create_Monseter(1200.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(1300.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(1500.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CPomp>::Create_Monseter(1700.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(1800.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(2100.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CPomp>::Create_Monseter(2400.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(2900.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CPomp>::Create_Monseter(3300.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CPomp>::Create_Monseter(3500.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(3900.f, 490.f, true));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(4100.f, 490.f, true));
#pragma endregion 몬스터 생성

	CCamera::Get_Instance()->SetLookAtPos(700.f, 400.f);
	CCamera::Get_Instance()->SetMaxPos(VEC2(3950, 450));
	CCamera::Get_Instance()->SetMinPos(VEC2(700, 0));
	CCamera::Get_Instance()->SetTarget(CObjMgr::Get_Instance()->Get_Player());

	m_vStartPos = { 0,0 };

	m_iEnemyCount = CObjMgr::Get_Instance()->Get_EnemyList()->size();

	if (!m_pText)
	{
		m_pText = CAbstractFactory<CPlayerText>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pText);
	}

	CSceneMgr::Get_Instance()->Set_SceneState(SC_READY_COMPLETE);
}

int CStageScene01::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (m_iEnemyCount)
	{
		for (auto iter = CObjMgr::Get_Instance()->Get_EnemyList()->begin(); iter != CObjMgr::Get_Instance()->Get_EnemyList()->end(); ++iter)
		{
			if ((*iter)->Get_Dead())
			{
				m_iEnemyCount--;
			}
			else
			{
				m_iEnemyCount = CObjMgr::Get_Instance()->Get_EnemyList()->size();
				break;
			}
		}
	}
	else
	{
		//클리어 화살표 생성
		m_bIsClear = true;
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

void CStageScene01::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStageScene01::Render(HDC hDC)
{

	VEC2 vRenderPos = CCamera::Get_Instance()->GetRenderPos(m_vStartPos);

	if (!CSceneMgr::Get_Instance()->Get_IsSlow())
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Stage01");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 4640, 812, hBackGroundDc, 0, 0, SRCCOPY);
	}
	else
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Stage01_slow");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 4640, 812, hBackGroundDc, 0, 0, SRCCOPY);
	}

	CObjMgr::Get_Instance()->Render(hDC);

}

void CStageScene01::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
}
