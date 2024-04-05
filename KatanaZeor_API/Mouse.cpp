#include "stdafx.h"
#include "Mouse.h"

#include "BmpMgr.h"
#include "Camera.h"

CMouse* CMouse::m_pInstance = nullptr;

CMouse::CMouse()
{
	m_eRenderId = UI;
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/effect/cursor.bmp", L"Cursor");

	m_vSize.x = 50.f;
	m_vSize.y = 50.f;
}

int CMouse::Update()
{
	POINT	ptMouse1{}, ptMouse2{};
	RECT	rcMouse;
	POINT pt;
	GetClientRect(g_hWnd, &rcMouse);

	ptMouse1.x = rcMouse.left;
	ptMouse1.y = rcMouse.top;
	ptMouse2.x = rcMouse.right;
	ptMouse2.y = rcMouse.bottom;

	ClientToScreen(g_hWnd, &ptMouse1);
	ClientToScreen(g_hWnd, &ptMouse2);

	rcMouse.left = ptMouse1.x;
	rcMouse.top = ptMouse1.y;
	rcMouse.right = ptMouse2.x;
	rcMouse.bottom = ptMouse2.y;

	//ClipCursor(&rcMouse);

	GetCursorPos(&pt);

	ScreenToClient(g_hWnd, &pt);

	m_vPos.x = (float)pt.x;
	m_vPos.y = (float)pt.y;

	__super::Update_Rect();

	ShowCursor(false);

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_DC(L"Cursor");

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vPos.x - m_vSize.x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vPos.y - m_vSize.y * 0.5f),			 // 복사 받을 위치의 top
		(int)50,			 // 복사 받을 가로 길이
		(int)50,			 // 복사 받을 세로 길이
		hMemDC,					// 비트맵을 가지고 있는 dc
		0,						// 출력할 비트맵의 시작 x좌표
		0,						// 출력할 비트맵의 시작 y좌표
		(int)50,			// 출력할 비트맵의 가로 사이즈
		(int)50,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값
}

void CMouse::Release()
{
}
