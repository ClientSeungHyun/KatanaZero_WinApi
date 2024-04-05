#include "stdafx.h"
#include "Obj.h"
#include "TitleScene.h"
#include "MyButton.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CTitleScene::CTitleScene() :m_bIsTitleLoad(false), m_bIsStartClicked(false), m_dwBackUpTime(0), m_dwZeroDelayTime(0), m_bIsBloodStartClicked(false)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	srand((unsigned)time(NULL));
}

CTitleScene::~CTitleScene()
{
	Release();
}

void CTitleScene::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Title/Title0.bmp", L"Title");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Title/Title1.bmp", L"Title_zero");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Title/btn_New.bmp", L"Btn_New");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Title/btn_blood.bmp", L"Btn_Blood");

	/*pObj = CAbstractFactory<CMyButton>::Create(400.f, 600.f, 167.f, 48.f);
	dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Btn_Load");
	CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);*/

	m_vStartPos = { 0,0 };
	m_dwBackUpTime = 0;
	m_dwZeroDelayTime = 0;

	m_bIsStartClicked = false;
	m_bIsBloodStartClicked = false;
	
	CSoundMgr::Get_Instance()->PlayBGM(L"song_rainonbrick.ogg", g_fBackgGroundSound);
}

int CTitleScene::Update()
{
	if (m_vStartPos.y > -800.f )
		m_vStartPos.y -= 10.f;
	else
	{
		if (!m_bIsTitleLoad)
		{
			m_bIsTitleLoad = true;

			CObj* pObj = CAbstractFactory<CMyButton>::Create(650.f, 450.f, 129.f, 48.f);
			dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Btn_New");
			CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

			pObj = CAbstractFactory<CMyButton>::Create(650.f, 520.f, 167.f, 48.f);
			dynamic_cast<CMyButton*>(pObj)->Set_FrameKey(L"Btn_Blood");
			CObjMgr::Get_Instance()->Add_Object(OBJ_BUTTON, pObj);

			m_dwBackUpTime = GetTickCount();
		}

		if (m_bIsStartClicked && m_vStartPos.y < -1500.f)
		{
			m_vStartPos.y -= 10;
		}
		
		if(m_bIsStartClicked && m_vStartPos.y >= -1500.f)
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_STAGE1);
		}
		else if (m_bIsBloodStartClicked && m_vStartPos.y >= -1500.f)
		{
			CSceneMgr::Get_Instance()->Scene_Change(SC_BLOOD);
		}


		if (m_dwBackUpTime + m_dwZeroDelayTime + 200 < GetTickCount() && m_dwBackUpTime)
		{
			m_dwBackUpTime = GetTickCount();
			m_dwZeroDelayTime = rand() % 200 + 1000;
		}
	}
	
	
	CObjMgr::Get_Instance()->Update();

	return 0;
}

void CTitleScene::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CTitleScene::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_DC(L"Title");
	HDC	hZeroDC = CBmpMgr::Get_Instance()->Find_DC(L"Title_zero");

	BitBlt(hDC, (int)m_vStartPos.x, (int)m_vStartPos.y, WINCX, WINCY - (int)m_vStartPos.y, hMemDC, 0, 0, SRCCOPY);

	if (m_bIsTitleLoad && m_dwBackUpTime + m_dwZeroDelayTime > GetTickCount())
	{
		BitBlt(hDC, (int)m_vStartPos.x, (int)m_vStartPos.y, WINCX, WINCY - (int)m_vStartPos.y, hZeroDC, 0, 0, SRCCOPY);
	}

	CObjMgr::Get_Instance()->Render(hDC);
}

void CTitleScene::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ_BUTTON);
	CSoundMgr::Get_Instance()->StopAll();
}
