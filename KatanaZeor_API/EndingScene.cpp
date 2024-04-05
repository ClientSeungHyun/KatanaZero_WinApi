#include "EndingScene.h"
#include "Player.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
#include "KeyMgr.h"

CEndingScene::CEndingScene()
{
}

CEndingScene::~CEndingScene()
{
	Release();
}

void CEndingScene::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/EndText.bmp", L"EndText");

	for (int i = 0; i < 20; ++i)
	{
		CPlayer* pPlayer = new CPlayer;
		pPlayer->Set_Pos(float(100 + i * 60), WINCY * 0.5f + 200.f);
		pPlayer->Initialize();
	}

	CSoundMgr::Get_Instance()->PlayBGM(L"Neighbors_Ever.mp3", g_fBackgGroundSound);
}

int CEndingScene::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_LBUTTON) == KEY_DOWN)
	{
		CSceneMgr::Get_Instance()->Scene_Change(SC_TITLE);
	}

    return 0;
}

void CEndingScene::Late_Update()
{
	//CObjMgr::Get_Instance()->Late_Update();
}

void CEndingScene::Render(HDC hDC)
{
	HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"Camera_Black");
	HDC hTextDc = CBmpMgr::Get_Instance()->Find_DC(L"EndText");

	BitBlt(hDC, 0, 0, 1344, 744, hBackGroundDc, 0, 0, SRCCOPY);

	GdiTransparentBlt(hDC,
		250,
		100,
		767,
		307,
		hTextDc,
		0,
		0,
		(int)767,
		(int)307,
		RGB(0, 0, 0));

	for (auto iter = CObjMgr::Get_Instance()->Get_PlayerList()->begin(); iter != CObjMgr::Get_Instance()->Get_PlayerList()->end(); ++iter)
	{
		(*iter)->Render(hDC);
	}

	//CObjMgr::Get_Instance()->Render(hDC);
}

void CEndingScene::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
}
