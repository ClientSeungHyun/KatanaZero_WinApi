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

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_vSize.x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_vSize.y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_vSize.x,			 // 복사 받을 가로 길이
		(int)m_vSize.y,			 // 복사 받을 세로 길이
		hBloodDC,					// 비트맵을 가지고 있는 dc
		0,						// 출력할 비트맵의 시작 x좌표
		0,						// 출력할 비트맵의 시작 y좌표
		(int)m_vSize.x,			// 출력할 비트맵의 가로 사이즈
		(int)m_vSize.y,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

}

void CBloodBGEffect::Release()
{
}
