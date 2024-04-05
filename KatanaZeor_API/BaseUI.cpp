#include "BaseUI.h"

#include "BmpMgr.h"

CBaseUI::CBaseUI()
{
}

CBaseUI::~CBaseUI()
{
}

void CBaseUI::Initialize()
{
	Set_Size(1344, 49);
	Set_Pos(0, 0);
	Set_FrameKey(L"HUD");
}

int CBaseUI::Update()
{
	__super::Update_Child();
	return 0;
}

void CBaseUI::Late_Update()
{
	__super::Late_Update_Child();
}

void CBaseUI::Render(HDC hDC)
{
    HDC	hMemDC = CBmpMgr::Get_Instance()->Find_DC(m_pFrameKey);
    //// 비트맵 출력 함수
    //BitBlt(hDC, m_vCameraPos.x, m_vCameraPos.y, m_vSize.x, m_vSize.y, hMemDC, 0, 0, SRCCOPY);

    GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
        (int)(m_vPos.x), // 복사 받을 위치의 left
        (int)(m_vPos.y),			 // 복사 받을 위치의 top
        (int)m_vSize.x,			 // 복사 받을 가로 길이
        (int)m_vSize.y,			 // 복사 받을 세로 길이
        hMemDC,					// 비트맵을 가지고 있는 dc
        (int)0,						// 출력할 비트맵의 시작 x좌표
        (int)0,						// 출력할 비트맵의 시작 y좌표
        (int)m_vSize.x,			// 출력할 비트맵의 가로 사이즈
        (int)m_vSize.y,			// 출력할 비트맵의 세로 사이즈
        RGB(255, 255, 255));	// 제거할 픽셀의 색상 값
	__super::Render(hDC);
}

void CBaseUI::Release()
{
}
