#include "CPlayerText.h"

#include "BmpMgr.h"

CPlayerText::CPlayerText()
{
    m_eRenderId = UI;
}

CPlayerText::~CPlayerText()
{
}

void CPlayerText::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DeadText.bmp", L"DeadText");
	m_pFrameKey = L"DeadText";

	m_vSize.x =224.f;
	m_vSize.y = 106.f;
}

int CPlayerText::Update()
{
	__super::Update_Rect();

    return 0;
}

void CPlayerText::Late_Update()
{
}

void CPlayerText::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_DC(m_pFrameKey);

	if (m_bIsActive)
	{
		GdiTransparentBlt(hDC,
			600,
			300,
			(int)m_vSize.x,
			(int)m_vSize.y,
			hMemDC,
			0,
			0,
			(int)m_vSize.x,
			(int)m_vSize.y,
			RGB(255, 255, 255));
	}
}

void CPlayerText::Release()
{
}
