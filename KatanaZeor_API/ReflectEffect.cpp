#include "ReflectEffect.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Camera.h"

CReflectEffect::CReflectEffect()
{
    m_eRenderId = BACKGROUND;
}

CReflectEffect::~CReflectEffect()
{
}

void CReflectEffect::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Enemy/Pomp/Enemy_pomp_reflect.bmp", L"Enemy_reflect", VEC2(147, 140), VEC2(0, 0), VEC2(0, 0), 5, 200, GetTickCount());
    m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_reflect");
	Set_Frame(false);

	CCamera::Get_Instance()->StartShakeCamera();
	m_vLookDir = { 1.0f,0.f };
}

int CReflectEffect::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

	if (m_tFrame.isPlayDone)
	{
		m_bDead = true;
	}

    Move_Frame();

    return 0;
}

void CReflectEffect::Late_Update()
{
}

void CReflectEffect::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC hReflect = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
		(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
		hReflect,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// 출력할 비트맵의 시작 y좌표
		(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
		(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값
}

void CReflectEffect::Release()
{
}
