#include "LoadingScene.h"
#include "Player.h"
#include "Mouse.h"

#include "AbstractFactory.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "Camera.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CLoadingScene::CLoadingScene()
{
}

CLoadingScene::~CLoadingScene()
{
}

void CLoadingScene::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/LoadingScene.bmp", L"Loading");
	HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Loading");

	HDC hDC = GetDC(g_hWnd);
	BitBlt(hDC, 0, 0, 1344, 744, hBackGroundDc, 0, 0, SRCCOPY);

	DeleteDC(hDC);

	CSoundMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage1_bg_collision.bmp", L"Stage01_Col");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage1_bg_render.bmp", L"Stage01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage1_bg_slow.bmp", L"Stage01_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage2_bg_collision.bmp", L"Stage02_Col");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage2_bg_render.bmp", L"Stage02");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage2_bg_slow.bmp", L"Stage02_slow"); 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage3_bg_collision.bmp", L"Stage03_Col");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage3_bg_render.bmp", L"Stage03");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/stage3_bg_render_slow.bmp", L"Stage03_slow");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1_collision.bmp", L"BossStage01_Col");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1.bmp", L"BossStage1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage2_collision.bmp", L"BossStage02_Col");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage2.bmp", L"BossStage2");

	/*if (CSceneMgr::Get_Instance()->Get_MapVec(SC_STAGE1).empty())
	{
		for (int i = 0; i <= 4640; ++i)
		{
			for (int j = 0; j <= 812; ++j)
			{
				COLORREF color = GetPixel(CBmpMgr::Get_Instance()->Find_DC(L"Stage01_Col"), i, j);
				CSceneMgr::Get_Instance()->Get_MapVec(SC_STAGE1).push_back(color);
			}
		}

		CSceneMgr::Get_Instance()->Set_MapHeight(SC_STAGE1, 813);
	}

	if (CSceneMgr::Get_Instance()->Get_MapVec(SC_STAGE2).empty())
	{
		for (int i = 0; i <= 2829; ++i)
		{
			for (int j = 0; j <= 2031; ++j)
			{
				COLORREF color = GetPixel(CBmpMgr::Get_Instance()->Find_DC(L"Stage02_Col"), i, j);
				CSceneMgr::Get_Instance()->Get_MapVec(SC_STAGE2).push_back(color);
			}
		}

		CSceneMgr::Get_Instance()->Set_MapHeight(SC_STAGE2, 2032);
	}

	if (CSceneMgr::Get_Instance()->Get_MapVec(SC_STAGE3).empty())
	{
		for (int i = 0; i <= 4000; ++i)
		{
			for (int j = 0; j <= 1492; ++j)
			{
				COLORREF color = GetPixel(CBmpMgr::Get_Instance()->Find_DC(L"Stage03_Col"), i, j);
				CSceneMgr::Get_Instance()->Get_MapVec(SC_STAGE3).push_back(color);
			}
		}

		CSceneMgr::Get_Instance()->Set_MapHeight(SC_STAGE3, 1493);
	}*/

	if (CSceneMgr::Get_Instance()->Get_MapVec(SC_BOSS1).empty())
	{
		for (int i = 0; i <= 1394; ++i)
		{
			for (int j = 0; j <= 784; ++j)
			{
				COLORREF color = GetPixel(CBmpMgr::Get_Instance()->Find_DC(L"BossStage01_Col"), i, j);
				CSceneMgr::Get_Instance()->Get_MapVec(SC_BOSS1).push_back(color);
			}
		}

		CSceneMgr::Get_Instance()->Set_MapHeight(SC_BOSS1, 785);
	}

	if (CSceneMgr::Get_Instance()->Get_MapVec(SC_BOSS2).empty())
	{
		for (int i = 0; i <= 1344; ++i)
		{
			for (int j = 0; j <= 784; ++j)
			{
				COLORREF color = GetPixel(CBmpMgr::Get_Instance()->Find_DC(L"BossStage02_Col"), i, j);
				CSceneMgr::Get_Instance()->Get_MapVec(SC_BOSS2).push_back(color);
			}
		}

		CSceneMgr::Get_Instance()->Set_MapHeight(SC_BOSS2, 785);
	}
}

int CLoadingScene::Update()
{
	CSceneMgr::Get_Instance()->Scene_Change(SC_TITLE);
    return 0;
}

void CLoadingScene::Late_Update()
{
}

void CLoadingScene::Render(HDC hDC)
{

}

void CLoadingScene::Release()
{
}
