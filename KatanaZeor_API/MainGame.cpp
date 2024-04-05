#include "stdafx.h"
#include "MainGame.h"
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


CMainGame::CMainGame() : m_iFPS(0), m_dwTime(GetTickCount()), m_dwTimeBackTime(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	g_fEffectSound = 1.f;
	g_fBackgGroundSound = 1.f;

	CTimeMgr::Get_Instance()->Initialize();

	CMouse::Get_Instance()->Initialize();

	//CObjMgr::Get_Instance()->Add_Object(OBJ_MOUSE, CAbstractFactory<CMouse>::Create());

	CSceneMgr::Get_Instance()->Scene_Change(SC_LOADING);
	m_dwTimeBackTime = 0;
}

void CMainGame::Update()
{
	CKeyMgr::Get_Instance()->Update();
	CMouse::Get_Instance()->Update();

#pragma region æ¿ ƒ°∆Æ≈∞
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_F9) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_TITLE);
	}
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_F1) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE1);
	}
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_F2) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE2);
	}
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_F3) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE3);
	}
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_F4) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_BOSS1);
	}
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_F5) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_BOSS2);
	}
#pragma endregion æ¿ ƒ°∆Æ≈∞

	if (CKeyMgr::Get_Instance()->Get_KeyState('Z') == KEY_DOWN)
	{
		g_bIsTimeBack = true;
		m_dwTimeBackTime = GetTickCount();
		CObjMgr::Get_Instance()->Delete_ID(OBJ_BG);
		CSoundMgr::Get_Instance()->PlaySound(L"Rewind.wav", SOUND_REWIND, g_fEffectSound);
	}

	if (g_bIsTimeBack)
	{
		CTimeMgr::Get_Instance()->Set_WorldTime(0.f);
	}

	if (m_dwTimeBackTime + 5000 < GetTickCount() && m_dwTimeBackTime)
	{
		g_bIsTimeBack = false;
		m_dwTimeBackTime = 0;
	}

	CCamera::Get_Instance()->Update();
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	
	CSceneMgr::Get_Instance()->Late_Update();
	CTimeMgr::Get_Instance()->Update();

	if (!CObjMgr::Get_Instance()->Get_IsPlayerEmpty())
	{
		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsNextStage())
		{
			CSceneMgr::Get_Instance()->Scene_Change(SCENEID(CSceneMgr::Get_Instance()->Get_CurScene() + 1));
			CSoundMgr::Get_Instance()->PlaySound(L"sound_transition_begin.wav", SCENE_CHANGE, g_fEffectSound);
			CSceneMgr::Get_Instance()->Set_SceneState(SC_READY);
			dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsNextStage(false);
			
		}
	}
}

void CMainGame::Render()
{
	HDC hMemdc = CreateCompatibleDC(m_hDC); 
	HBITMAP hBitmap = CreateCompatibleBitmap(m_hDC, WINCX, WINCY); 
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemdc, hBitmap); 

	CSceneMgr::Get_Instance()->Render(hMemdc);
	CCamera::Get_Instance()->Render(hMemdc);
	CMouse::Get_Instance()->Render(hMemdc);

	BitBlt(m_hDC, (int)0, (int)0, 1394, 784, hMemdc, 0, 0, SRCCOPY);

	SelectObject(hMemdc, hOldBitmap); // ±‚¡∏ ∫Ò∆Æ∏  º±≈√
	DeleteDC(hMemdc);
	DeleteObject(hBitmap);
	DeleteObject(hOldBitmap);
}

void CMainGame::Release()
{
	CBmpMgr::Destroy();
	CKeyMgr::Destroy();
	CSceneMgr::Destroy();
	CTimeMgr::Destroy();
	CCamera::Destroy();
	CObjMgr::Destroy();
	CSoundMgr::Destroy_Instance();
	CMouse::Destroy();

	ReleaseDC(g_hWnd, m_hDC);


}
