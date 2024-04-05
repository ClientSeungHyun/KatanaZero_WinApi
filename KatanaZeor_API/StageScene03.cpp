#include "StageScene03.h"
#include "Player.h"
#include "ItemKnife.h"
#include "ItemExplosive.h"
#include "UI.h"
#include "CPlayerText.h"

#include "Bold.h"
#include "Grunt.h"
#include "Pomp.h"
#include "Shieldcop.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Camera.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

#include "Originator.h"

CStageScene03::CStageScene03()
{
}

CStageScene03::~CStageScene03()
{
	Release();
}

void CStageScene03::Initialize()
{
	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_Pos(200.f, 900.f);
	pPlayer->Initialize();

	//16
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(1240.f, 900.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(2660.f, 900.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(3000.f, 900.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(1400.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(1700.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(1900.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(2100.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(2400.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(3200.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(3000.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(2800.f, 1140.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CBold>::Create_Monseter(350.f, 610.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CShieldcop>::Create_Monseter(700.f, 610.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CShieldcop>::Create_Monseter(3000.f, 610.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CShieldcop>::Create_Monseter(3300.f, 610.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CShieldcop>::Create_Monseter(3600.f, 610.f, false));

	//아이템 배치
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItemKnife>::Create(507.f, 907.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, CAbstractFactory<CItemExplosive>::Create(1600.f, 1198.f));

	//카메라 설정
	CCamera::Get_Instance()->SetLookAtPos(700.f, 800.f);
	CCamera::Get_Instance()->SetMaxPos(VEC2(3328, 1292));
	CCamera::Get_Instance()->SetMinPos(VEC2(700, 0));
	CCamera::Get_Instance()->SetTarget(CObjMgr::Get_Instance()->Get_Player());

	m_iEnemyCount = CObjMgr::Get_Instance()->Get_EnemyList()->size();

	if (!m_pText)
	{
		m_pText = CAbstractFactory<CPlayerText>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pText);
	}

	CSceneMgr::Get_Instance()->Set_SceneState(SC_READY_COMPLETE);
}

int CStageScene03::Update()
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

void CStageScene03::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	//CCamera::Get_Instance()->Late_Update();

}

void CStageScene03::Render(HDC hDC)
{
	VEC2 vWindowPos = { 0,0 };
	VEC2 vRenderPos = CCamera::Get_Instance()->GetRenderPos(vWindowPos.x, vWindowPos.y);



	if (!CSceneMgr::Get_Instance()->Get_IsSlow())
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Stage03");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 4000, 1492, hBackGroundDc, 0, 0, SRCCOPY);
	}
	else
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Stage03_slow");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 4000, 1492, hBackGroundDc, 0, 0, SRCCOPY);
	}


	CObjMgr::Get_Instance()->Render(hDC);
}

void CStageScene03::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
}
