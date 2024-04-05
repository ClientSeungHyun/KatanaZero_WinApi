#include "TimerUI.h"
#include "Item.h"
#include "UI.h"
#include "Player.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

CTimerUI::CTimerUI()
{
    m_eRenderId = UI;
    m_eId = OBJ_UI;
}

CTimerUI::~CTimerUI()
{
}

void CTimerUI::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Timer_Down.bmp", L"TimerDown");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Timer_Up.bmp", L"TimerUp");

    m_vSize = { 271, 46 };
    m_vPos = { 520, 0 };

    m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"TimerDown");

    m_bIsActive = true;

    m_pTimerGageUI = new CUI;
    m_pTimerGageUI->Set_Size(m_iTimeGage, 27);
    m_pTimerGageUI->Set_Pos(561, 5);
    m_pTimerGageUI->Set_FrameKey(L"TimerUp");
    m_pTimerGageUI->Set_IsActive(true);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pTimerGageUI);
}

int CTimerUI::Update()
{
    //m_pTimerGageUI->Set_Size(m_iTimeGage - 0.01f, 5);
    m_pTimerGageUI->Set_Size(CTimeMgr::Get_Instance()->Get_TimeGage() - CTimeMgr::Get_Instance()->Get_WorldTime(), 27);

    if (CTimeMgr::Get_Instance()->Get_TimeGage() - CTimeMgr::Get_Instance()->Get_WorldTime() <= 0 && dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_State() != HURT)
    {
        dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Change_StateHURT();
    }

    return 0;
}

void CTimerUI::Late_Update()
{
}

void CTimerUI::Render(HDC hDC)
{
    HDC	hMemDC = m_pAniBmp->Get_MemDC();

    if (m_bIsActive)
    {
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
    }
}

void CTimerUI::Release()
{
}
