#include "BloodScene.h"
#include "Player.h"

#include "Bold.h"
#include "Grunt.h"
#include "Pomp.h"
#include "Shieldcop.h"

#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

CBloodScene::CBloodScene() :m_dwCreateDuration(0), m_dwCreateEnemyTime(0), m_iScore(0), m_iLevel(0), m_bIsDead(false), m_dwScoreTime(0)
{
	srand((unsigned)time(NULL));
}

CBloodScene::~CBloodScene()
{
	Release();
}

void CBloodScene::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1.bmp", L"BossStage01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage1_slow.bmp", L"BossStage01_slow");

	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_Pos(253.f, 485.f);
	pPlayer->Initialize();

	//카메라 설정
	CCamera::Get_Instance()->SetLookAtPos(WINCX * 0.5f, WINCY * 0.5f);
	CCamera::Get_Instance()->SetMaxPos(VEC2(WINCX * 0.5f, WINCY * 0.5f));
	CCamera::Get_Instance()->SetMinPos(VEC2(WINCX * 0.5f, WINCY * 0.5f));

	m_dwCreateEnemyTime = GetTickCount();
	m_dwCreateDuration = 10000;
}

int CBloodScene::Update()
{
	CTimeMgr::Get_Instance()->Set_WorldTime(0.f);


	if (m_bIsDead)
	{
		if (CKeyMgr::Get_Instance()->Get_KeyState(VK_LBUTTON) == KEY_DOWN)
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_TITLE);
		}

		return 0;
	}

	CObjMgr::Get_Instance()->Update();

	if (CObjMgr::Get_Instance()->Get_Player()->Get_Dead())
	{
		if (!m_dwScoreTime)
		{
			m_dwScoreTime = GetTickCount();
		}

		if (m_dwScoreTime + 2000 < GetTickCount())
		{
			m_bIsDead = true;
		}

		return 0;
	}


	if (m_dwCreateEnemyTime + m_dwCreateDuration < GetTickCount() && !m_bIsDead)
	{
		Create_Enemy();
		m_dwCreateEnemyTime = GetTickCount();
	}

    return 0;
}

void CBloodScene::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

	for (auto iter = CObjMgr::Get_Instance()->Get_EnemyList()->begin(); iter != CObjMgr::Get_Instance()->Get_EnemyList()->end(); )
	{
		if ((*iter)->Get_Dead())
		{
			iter = CObjMgr::Get_Instance()->Get_EnemyList()->erase(iter);
			m_iScore++;
		}
		else
		{
			++iter;
		}
	}
}

void CBloodScene::Render(HDC hDC)
{
	VEC2 vWindowPos = { 0,0 };
	VEC2 vRenderPos = CCamera::Get_Instance()->GetRenderPos(vWindowPos.x, vWindowPos.y);

	HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"BossStage01");


	HDC hMemdc = CreateCompatibleDC(hDC); // 화면dc(그릴 윈도우) 기반 메모리 DC생성 
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, WINCX, WINCY); // 비트맵 생성
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemdc, hBitmap); // 비트맵 선택

	BitBlt(hMemdc, (int)vRenderPos.x, (int)vRenderPos.y, 1394, 784, hBackGroundDc, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hMemdc);

	if (CSceneMgr::Get_Instance()->Get_SceneState() == SC_PLAYING)
	{
		HFONT hFont;
		HFONT hOldFont;

		if(m_bIsDead)
		{ 
			hFont = CreateFont(50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
			hOldFont = (HFONT)SelectObject(hMemdc, hFont);
			m_vTextPos[0] = { 550, 200 };
			m_vTextPos[1] = { 550, 300 };
		}
		else
		{
			hFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
			hOldFont = (HFONT)SelectObject(hMemdc, hFont);
			m_vTextPos[0] = { 150, 10 };
			m_vTextPos[1] = { 1070, 10 };
		}

		TCHAR szLevelText[1024];
		TCHAR szScoreText[1024];

		SetBkMode(hMemdc, TRANSPARENT);
		wsprintf(szLevelText, TEXT("LEVEL : %d"), m_iLevel);
		SetTextColor(hMemdc, RGB(255, 255, 255));
		TextOut(hMemdc, (int)m_vTextPos[0].x, (int)m_vTextPos[0].y, szLevelText, lstrlen(szLevelText));

		wsprintf(szScoreText, TEXT("SCORE : %d"), m_iScore);
		SetTextColor(hMemdc, RGB(255, 255, 255));
		TextOut(hMemdc, (int)m_vTextPos[1].x, (int)m_vTextPos[1].y, szScoreText, lstrlen(szScoreText));

		// 이전에 선택되었던 글꼴 복원
		SelectObject(hDC, hOldFont);
		// 생성한 글꼴 삭제
		DeleteObject(hFont);
	}
	BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 1394, 784, hMemdc, 0, 0, SRCCOPY);

	SelectObject(hMemdc, hOldBitmap); // 기존 비트맵 선택
	DeleteDC(hMemdc);
	DeleteObject(hBitmap);
	DeleteObject(hOldBitmap);
}

void CBloodScene::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);

	Safe_Delete<CObj*>(m_pEnemy);
}

void CBloodScene::Create_Enemy()
{
	m_pEnemy = nullptr;
	float fDoorPos[4] = {230.f ,530.f ,900.f ,1160.f };

	for (int i = 0; i < 4 + m_iLevel; ++i)
	{
		int randEnemy = rand() % 100;
		int randPos = rand() % 90;

		switch (randEnemy % 4)
		{
		case 0:
			m_pEnemy = new CGrunt;
			break;

		case 1:
			m_pEnemy = new CPomp;
			break;

		case 2:
			m_pEnemy = new CShieldcop;
			break;

		case 3:
			m_pEnemy = new CBold;
			break;

		default:
			continue;
		}

		m_pEnemy->Initialize();
		m_pEnemy->Set_Pos(fDoorPos[randPos % 4], 470.f);
		dynamic_cast<CEnemy*>(m_pEnemy)->Set_StartPos(fDoorPos[randPos % 4], 470.f);
		dynamic_cast<CEnemy*>(m_pEnemy)->Set_IsPatrol(false);
		CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, m_pEnemy);
	}

	m_pEnemy = nullptr;
	m_iLevel++;
}
