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
    //// ��Ʈ�� ��� �Լ�
    //BitBlt(hDC, m_vCameraPos.x, m_vCameraPos.y, m_vSize.x, m_vSize.y, hMemDC, 0, 0, SRCCOPY);

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
	__super::Render(hDC);
}

void CBaseUI::Release()
{
}
