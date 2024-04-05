#include "CBatterUI.h"
#include "Player.h"
#include "UI.h"

#include "BmpMgr.h"
#include "ObjMgr.h"

CBatterUI::CBatterUI() :m_iBulletTimeGage(0)
{
	m_eRenderId = UI;
	m_eId = OBJ_UI;
}

CBatterUI::~CBatterUI()
{
}

void CBatterUI::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BattertyBar.bmp", L"BattertyBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BattertyGage.bmp", L"BattertyGage");

	m_vSize = { 140, 36 };
	m_vPos = { 5, 5 };

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"BattertyBar");

	m_bIsActive = true;

    for (int i = 0; i < 9; ++i)
    {
        CUI* BatteryGageUI = new CUI;
        BatteryGageUI->Set_Size(20, 24);
        BatteryGageUI->Set_Pos((float)(16 + (i * 12)), 10.f);
        BatteryGageUI->Set_FrameKey(L"BattertyGage");
        BatteryGageUI->Set_IsActive(true);
        m_BatteryGageUIList.push_back(BatteryGageUI);

        CObjMgr::Get_Instance()->Add_Object(OBJ_UI, BatteryGageUI);
    }
}

int CBatterUI::Update()
{ 
    m_iBulletTimeGage = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_BulletTimeGage();

    for (auto& iter : m_BatteryGageUIList)
    {
        if(iter->Get_Pos().x >= m_iBulletTimeGage*12+ 16)
            iter->Set_IsActive(false);
        else
            iter->Set_IsActive(true);
    }

	return 0;
}

void CBatterUI::Late_Update()
{
}

void CBatterUI::Render(HDC hDC)
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

void CBatterUI::Release()
{
}
