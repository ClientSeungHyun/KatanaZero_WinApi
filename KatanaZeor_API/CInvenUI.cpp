#include "CInvenUI.h"
#include "Player.h"
#include "Item.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

CInvenUI::CInvenUI()
{
    m_eRenderId = UI;
    m_eId = OBJ_UI;
}

CInvenUI::~CInvenUI()
{
}

void CInvenUI::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Inven_Hand.bmp", L"Inven_Hand");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Inven_Knief.bmp", L"Inven_Knief");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Inven_Fire.bmp", L"Inven_Fire");

    m_vSize = { 106, 46 };
    m_vPos = { 1225, 2 };

    m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Inven_Hand");

    m_bIsActive = true;
}

int CInvenUI::Update()
{
    return 0;
}

void CInvenUI::Late_Update()
{
    if (static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_PlayerItem() == nullptr)
    {
        m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Inven_Hand");
    }
    else
    {
        switch (static_cast<CItem*>(static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_PlayerItem())->Get_ItemType())
        {
        case ITEM_KNEIF:
            m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Inven_Knief");
            break;

        case ITEM_FIRE:
            m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Inven_Fire");
            break;

        default:
            break;
        }
    }
}

void CInvenUI::Render(HDC hDC)
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

void CInvenUI::Release()
{
}
