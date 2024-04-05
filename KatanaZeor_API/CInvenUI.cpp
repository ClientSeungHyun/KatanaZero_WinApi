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
        GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
            (int)(m_vPos.x), // ���� ���� ��ġ�� left
            (int)(m_vPos.y),			 // ���� ���� ��ġ�� top
            (int)m_vSize.x,			 // ���� ���� ���� ����
            (int)m_vSize.y,			 // ���� ���� ���� ����
            hMemDC,					// ��Ʈ���� ������ �ִ� dc
            (int)0,						// ����� ��Ʈ���� ���� x��ǥ
            (int)0,						// ����� ��Ʈ���� ���� y��ǥ
            (int)m_vSize.x,			// ����� ��Ʈ���� ���� ������
            (int)m_vSize.y,			// ����� ��Ʈ���� ���� ������
            RGB(255, 255, 255));	// ������ �ȼ��� ���� ��
    }
}

void CInvenUI::Release()
{
}
