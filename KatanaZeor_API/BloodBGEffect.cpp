#include "BloodBGEffect.h"

#include "BmpMgr.h"
#include "Camera.h"

CBloodBGEffect::CBloodBGEffect()
{
	m_eRenderId = BACKGROUND;
}

CBloodBGEffect::~CBloodBGEffect()
{
}

void CBloodBGEffect::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/effect/effect_blood_remain4_L.bmp", L"BLOOD_REMAIN_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/effect/effect_blood_remain4_R.bmp", L"BLOOD_REMAIN_R");
	
	m_vSize = { 144.f,144.f };

	if (m_vLookDir.x == -1.f)
	{
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"BLOOD_REMAIN_R");
	}
	else if (m_vLookDir.x == 1.f)
	{
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"BLOOD_REMAIN_L");
	}
}

int CBloodBGEffect::Update()
{
	if (m_bDead)
		return OBJ_NOEVENT;

	return 0;
}

void CBloodBGEffect::Late_Update()
{
}

void CBloodBGEffect::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hBloodDC = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
		(int)(m_vCameraPos.x - m_vSize.x * 0.5f), // ���� ���� ��ġ�� left
		(int)(m_vCameraPos.y - m_vSize.y * 0.5f),			 // ���� ���� ��ġ�� top
		(int)m_vSize.x,			 // ���� ���� ���� ����
		(int)m_vSize.y,			 // ���� ���� ���� ����
		hBloodDC,					// ��Ʈ���� ������ �ִ� dc
		0,						// ����� ��Ʈ���� ���� x��ǥ
		0,						// ����� ��Ʈ���� ���� y��ǥ
		(int)m_vSize.x,			// ����� ��Ʈ���� ���� ������
		(int)m_vSize.y,			// ����� ��Ʈ���� ���� ������
		RGB(255, 0, 255));	// ������ �ȼ��� ���� ��

}

void CBloodBGEffect::Release()
{
}
