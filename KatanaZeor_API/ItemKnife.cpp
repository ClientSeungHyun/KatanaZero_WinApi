#include "ItemKnife.h"

#include "BmpMgr.h"
#include "Camera.h"

#include "Caretaker.h"
#include "Collider.h"

CItemKnife::CItemKnife() 
{
    m_eId = OBJ_ITEM;
    m_eItemType = ITEM_KNEIF;
    m_eRenderId = GAMEOBJECT;
}

CItemKnife::~CItemKnife()
{
    Release();
}

void CItemKnife::Initialize()
{
    m_pCareTaker = new CCaretaker;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Knife.bmp", L"Knife");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Knife_Plg.bmp", L"Knife_Plg");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Knife_Reset.bmp", L"Knife_Reset");
    m_vSize = { 42, 42 };
    Create_Collider();

    m_pAniBmp =  CBmpMgr::Get_Instance()->Find_Image(L"Knife");

    m_pCollider->Set_Size(m_vSize);
    m_pCollider->Set_Offset(VEC2(0,0));

    m_fDiagonal = sqrt((m_vSize.x / 2.f) * (m_vSize.x / 2.f) + (m_vSize.y / 2.f) * (m_vSize.y / 2.f));

    m_fAngle = 0.f;
    m_vLookDir = { 1.0f,.0f };
    m_tFrame = { 0,0,0,0,0 };
    // �� ���
    m_tPoint[0].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);
    m_tPoint[0].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 135.f) * PI / 180.f) * m_fDiagonal);

    // �� ���		     
    m_tPoint[1].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);
    m_tPoint[1].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 45.f) * PI / 180.f) * m_fDiagonal);

    // �� �ϴ�		    
    m_tPoint[2].x = LONG((m_vSize.x / 2.f) + cos((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
    m_tPoint[2].y = LONG((m_vSize.y / 2.f) - sin((m_fAngle + 225.f) * PI / 180.f) * m_fDiagonal);
}

int         CItemKnife::Update()
{
    if (m_bDead)
    {
        m_bIsActive = false;
    }

    if (!g_bIsTimeBack)
    {
        Throw();
    }
    else
    {
        //���� ȸ��
        for (int i = 0; i < 20; ++i)
        {
            if (m_pCareTaker->Get_ListSize() > 0)
            {
                Set_MementoState(m_pCareTaker->TimeBakc());
            }
            else
            {
                TimeBakcDone();
            }
        }
    }

    return 0;
}

void        CItemKnife::Late_Update()
{
    m_pCollider->Late_Update();
    m_pCollider->Set_Size(m_vSize);
    m_pCollider->Set_Offset(VEC2(0, 0));
    if (!g_bIsTimeBack && CSceneMgr::Get_Instance()->Get_SceneState() == SC_PLAYING)
    {

        m_pCareTaker->Add(Save_State());
    }


}

void        CItemKnife::Render(HDC hDC)
{
    if (m_bIsActive)
    {
        m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos.x, m_vPos.y);

        HDC	hMemDC = m_pAniBmp->Get_MemDC();
        HDC	hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Knife_Plg")->Get_MemDC();
        HDC	hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Knife_Reset")->Get_MemDC();

        PlgBlt(hPlgDC,	// ȸ�� ��ų �̹����� ��� �׸� ���ΰ�
            m_tPoint, // �׸��� �׸� �簢���� ��ǥ ����Ʈ 3��, �迭�� ������ �� ���, �� ���, �� �ϴ�
            hMemDC,	// ��� �ִ� �̹����� ȸ�� ���� �׸� ���ΰ�
            0, 0,		// ��� ���� �׸� ���ΰ� 
            (int)m_vSize.x,	// �׸� �̹����� ����, ���� ������
            (int)m_vSize.y,
            NULL, NULL, NULL);

        GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
            (int)(m_vCameraPos.x - m_vSize.x * 0.5f), // ���� ���� ��ġ�� left
            (int)(m_vCameraPos.y - m_vSize.y * 0.5f),			 // ���� ���� ��ġ�� top
            (int)m_vSize.x,			 // ���� ���� ���� ����
            (int)m_vSize.y,			 // ���� ���� ���� ����
            hPlgDC,					// ��Ʈ���� ������ �ִ� dc
            (int)0,						// ����� ��Ʈ���� ���� x��ǥ
            (int)0,						// ����� ��Ʈ���� ���� y��ǥ
            (int)m_vSize.x,			// ����� ��Ʈ���� ���� ������
            (int)m_vSize.y,			// ����� ��Ʈ���� ���� ������
            RGB(255, 255, 255));	// ������ �ȼ��� ���� ��

        // ������ �� �ؽ�ó ���
        BitBlt(hPlgDC,
            0, 0,
            (int)m_vSize.x, m_vSize.y,
            hResetDC,
            0, 0,
            SRCCOPY);

        m_pCollider->Render(hDC);
    }

}

void        CItemKnife::Release()
{
    if (m_pCollider)
        Safe_Delete<CCollider*>(m_pCollider);

    if (m_pCareTaker)
        Safe_Delete<CCaretaker*>(m_pCareTaker);
}

