#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

#include "TitleScene.h"

CMyButton::CMyButton()
{
	m_eRenderId = UI;
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_vSize.x = 167.f;
	m_vSize.y = 48.f;

	m_pFrameKey = L"Start";
}

int CMyButton::Update()
{

	__super::Update_Rect();

	return 0;
}

void CMyButton::Late_Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->Get_KeyState(VK_LBUTTON) == KEY_UP)
		{
			if (!lstrcmp(L"Btn_New", m_pFrameKey))
			{
				dynamic_cast<CTitleScene*>(CSceneMgr::Get_Instance()->Get_NowScene())->Set_StartClick(true);
			}
			else if (!lstrcmp(L"Btn_Blood", m_pFrameKey))
			{
				dynamic_cast<CTitleScene*>(CSceneMgr::Get_Instance()->Get_NowScene())->Set_BloodStartClick(true);
			}

			return;
		}

		if (!lstrcmp(L"Btn_New", m_pFrameKey))
			m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;

}

void CMyButton::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_DC(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vSize.x,
		(int)m_vSize.y,
		hMemDC,
		m_iDrawID * (int)m_vSize.x,
		0,
		(int)m_vSize.x,
		(int)m_vSize.y,
		RGB(255, 0, 255));
}

void CMyButton::Release()
{
}
