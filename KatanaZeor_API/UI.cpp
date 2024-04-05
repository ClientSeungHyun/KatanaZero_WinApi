#include "UI.h"
#include "Player.h"
#include "ItemKnife.h"

#include "BmpMgr.h"
#include "Camera.h"
#include "ObjMgr.h"

CUI::CUI():m_pParentUI(nullptr)
{
    m_eRenderId = UI;
}

CUI::~CUI()
{
    Release();
}

void		CUI::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/HUD.bmp", L"HUD");


    /*

    CUI* pTimerGageUI = new CUI;
    pTimerGageUI->Set_Size(227, 27);
    pTimerGageUI->Set_Pos(561, 5);
    pTimerGageUI->Set_FrameKey(L"TimerUp");

    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pTimerUI);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pTimerGageUI);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pBatteryBarUI);
    CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pInvenUI);
    AddChild(pTimerUI);
    pTimerUI->AddChild(pTimerGageUI);

    AddChild(pBatteryBarUI);

    AddChild(m_pInvenUI);

    for (int i = 0; i < 9; ++i)
    {
        CUI* BatteryGageUI = new CUI;
        BatteryGageUI->Set_Size(20, 24);
        BatteryGageUI->Set_Pos(16 + (i * 12), 10);
        BatteryGageUI->Set_FrameKey(L"BattertyGage");
        BatteryGageUI->Set_IsActive(true);
        pBatteryBarUI->AddChild(pTimerUI);
        m_BatteryGageUIList.push_back(BatteryGageUI);

        CObjMgr::Get_Instance()->Add_Object(OBJ_UI, BatteryGageUI);
    }*/

    m_eId = OBJ_UI;
}

int			CUI::Update()
{
    /*m_iBulletTimeGage = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_BulletTimeGage();

    for (auto& iter : m_BatteryGageUIList)
    {
        if(iter->Get_Pos().x >= m_iBulletTimeGage*12+ 16)
            iter->Set_IsActive(false);
        else
            iter->Set_IsActive(true);
    }*/


	return 0;
}

void		CUI::Late_Update()
{
    /*if (Get_Parent())
    {
        VEC2 vParentPos = Get_Parent()->Get_Pos();
        m_vPos += vParentPos;
    }*/

    //Late_Update_Child();
}

void		CUI::Render(HDC hDC)
{
    HDC	hMemDC = CBmpMgr::Get_Instance()->Find_DC(m_pFrameKey);
    //// ��Ʈ�� ��� �Լ�
    //BitBlt(hDC, m_vCameraPos.x, m_vCameraPos.y, m_vSize.x, m_vSize.y, hMemDC, 0, 0, SRCCOPY);

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

    //Render_Child(hDC);
}

void		CUI::Release()
{
   /* for (auto& iter : m_vecChildUI)
    {
        Safe_Delete<CUI*>(iter);
    }*/
}

void CUI::Update_Child()
{
    for (int i = 0; i < m_vecChildUI.size(); ++i)
    {
        m_vecChildUI[i]->Update();
    }
}

void CUI::Late_Update_Child()
{
    for (auto iter : m_vecChildUI)
    {
        iter->Late_Update_Child();
    }
}

void CUI::Render_Child(HDC hDC)
{
    for (auto iter : m_vecChildUI)
    {
        iter->Render(hDC);
    }
}

