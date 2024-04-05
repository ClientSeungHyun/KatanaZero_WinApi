#include "StageScene02.h"
#include "Player.h"
#include "ItemKnife.h"
#include "UI.h"
#include "CPlayerText.h"
#include "Fan.h"
#include "MapLaser.h"

#include "Grunt.h"


#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Camera.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

#include "Originator.h"

CStageScene02::CStageScene02()
{
}

CStageScene02::~CStageScene02()
{
	Release();
}

void CStageScene02::Initialize()
{
	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_Pos(190, 1695);
	pPlayer->Initialize();

	CObjMgr::Get_Instance()->Add_Object(OBJ_TRAP, CAbstractFactory<CFan>::Create(2320.f, 697.f));
	CObjMgr::Get_Instance()->Add_Object(OBJ_TRAP, CAbstractFactory<CMapLaser>::Create(730.f, 1370.f , 4.f, 750.f));

	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(1100.f, 1170.f, false));
	CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, CAbstractFactory<CGrunt>::Create_Monseter(1400.f, 1170.f, false));
	
	//카메라 설정
	CCamera::Get_Instance()->SetLookAtPos(760, 1500);
	CCamera::Get_Instance()->SetMaxPos(VEC2(2000, 1500));
	CCamera::Get_Instance()->SetMinPos(VEC2(740, 500));

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

int CStageScene02::Update()
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

void CStageScene02::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

}

void CStageScene02::Render(HDC hDC)
{
		VEC2 vWindowPos = { 0,0 };
		VEC2 vRenderPos = CCamera::Get_Instance()->GetRenderPos(m_vStartPos);


		if (!CSceneMgr::Get_Instance()->Get_IsSlow())
		{
			HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Stage02");
			BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 2829, 1931, hBackGroundDc, 0, 0, SRCCOPY);
		}
		else
		{
			HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Stage02_slow");
			BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 2829, 1931, hBackGroundDc, 0, 0, SRCCOPY);
		}
	

		
			CObjMgr::Get_Instance()->Render(hDC);
		
}

void CStageScene02::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
}
