#include "BloodEffect.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Camera.h"

CBloodEffect::CBloodEffect()
{
	m_eRenderId = BACKGROUND;
}

CBloodEffect::~CBloodEffect()
{
	Release();
}

void CBloodEffect::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/effect/effect_blood_10x2.bmp", L"EFFECT_BLOOD", VEC2(80, 79), VEC2(0, 0), VEC2(80, 79), 9, 50, GetTickCount());

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"EFFECT_BLOOD");
	Set_Frame(false);
}

int CBloodEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();



	return 0;
}

void CBloodEffect::Late_Update()
{
}

void CBloodEffect::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC hBloodDC = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
		(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
		hBloodDC,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// 출력할 비트맵의 시작 y좌표
		(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
		(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

}

void CBloodEffect::Release()
{
}
