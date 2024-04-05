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

	GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
		(int)(m_vPos.x - m_vSize.x * 0.5f), // ���� ���� ��ġ�� left
		(int)(m_vPos.y - m_vSize.y * 0.5f),			 // ���� ���� ��ġ�� top
		(int)50,			 // ���� ���� ���� ����
		(int)50,			 // ���� ���� ���� ����
		hMemDC,					// ��Ʈ���� ������ �ִ� dc
		0,						// ����� ��Ʈ���� ���� x��ǥ
		0,						// ����� ��Ʈ���� ���� y��ǥ
		(int)50,			// ����� ��Ʈ���� ���� ������
		(int)50,			// ����� ��Ʈ���� ���� ������
		RGB(255, 0, 255));	// ������ �ȼ��� ���� ��
}

void CMouse::Release()
{
}
