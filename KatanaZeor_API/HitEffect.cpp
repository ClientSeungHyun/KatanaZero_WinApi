#include "HitEffect.h"
#include "Player.h"
#include "Camera.h"

#include "Collider.h"

#include "ObjMgr.h"
#include "BmpMgr.h"

CHitEffect::CHitEffect()
{
	m_eRenderId = EFFECT_LATER;
}

CHitEffect::~CHitEffect()
{
	Release();
}

void CHitEffect::Initialize()
{
	Create_Collider();
	m_pCollider->Set_IsActive(false);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/Player_hit_lazer.bmp", L"HitEffect");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/Player_hit_lazer_Plg.bmp", L"HitEffect_Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../image/Player_hit_lazer_Reset.bmp", L"HitEffect_Reset");

	m_fEffectSpeed = 100.f;
	m_dwDeadTime = GetTickCount();
	m_vSize = { 1400.f, 1400.f };

	POINT pt;
	VEC2 vPlayerPos = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_Pos();
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_fEffectAngle = atan2f(pt.y - m_vPos.y, pt.x - m_vPos.x) * (180.f / PI);
	m_fDirAngle = atan2f(vPlayerPos.y - pt.y, vPlayerPos.x - pt.x) * (180.f / PI);

	float	fDiagonal = (float)sqrtf(pow(m_vSize.x * 0.5f, 2) + pow(m_vSize.y * 0.5f, 2)); 

	// 좌 상단
	m_tPoint[0].x = LONG((m_vSize.x * 0.5f) + cosf((m_fEffectAngle + 135.f) * PI / 180.f) * fDiagonal);
	m_tPoint[0].y = LONG((m_vSize.y * 0.5f) - sinf((m_fEffectAngle + 135.f) * (PI / 180.f)) * fDiagonal);
					
	// 우 상단		 
	m_tPoint[1].x = LONG((m_vSize.x * 0.5f) + cosf((m_fEffectAngle + 45.f) * PI / 180.f) * fDiagonal);
	m_tPoint[1].y = LONG((m_vSize.y * 0.5f) - sinf((m_fEffectAngle + 45.f) * PI / 180.f) * fDiagonal);
					
	// 좌 하단					
	m_tPoint[2].x = LONG((m_vSize.x * 0.5f) + cosf((m_fEffectAngle + 225.f) * PI / 180.f) * fDiagonal);
	m_tPoint[2].y = LONG((m_vSize.y * 0.5f) - sinf((m_fEffectAngle + 225.f) * PI / 180.f) * fDiagonal);

	m_vPos.x += cosf(m_fDirAngle * (PI / 180.f)) * m_fEffectSpeed;
	m_vPos.y -= sinf(m_fDirAngle * (PI / 180.f)) * m_fEffectSpeed;

	CCamera::Get_Instance()->StartShakeCamera();
}

int CHitEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dwDeadTime + 100 < GetTickCount())
	{
		m_bDead = true;
	}
	return 0;
}

void CHitEffect::Late_Update()
{
}

void CHitEffect::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"HitEffect")->Get_MemDC();
	HDC	hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"HitEffect_Plg")->Get_MemDC();
	HDC	hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"HitEffect_Reset")->Get_MemDC();

	PlgBlt(hPlgDC,	// 회전 시킬 이미지를 어디에 그릴 것인가
		m_tPoint, // 그림을 그릴 사각형의 좌표 포인트 3개, 배열의 순서는 좌 상단, 우 상단, 좌 하단
		hMemDC,	// 어디에 있는 이미지를 회전 시켜 그릴 것인가
		0, 0,		// 어디서 부터 그릴 것인가 
		(int)m_vSize.x,	// 그릴 이미지의 가로, 세로 사이즈
		(int)m_vSize.y,
		NULL, NULL, NULL);

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_vSize.x*0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_vSize.y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_vSize.x,			 // 복사 받을 가로 길이
		(int)m_vSize.y,			 // 복사 받을 세로 길이
		hPlgDC,					// 비트맵을 가지고 있는 dc
		(int)0,						// 출력할 비트맵의 시작 x좌표
		(int)0,						// 출력할 비트맵의 시작 y좌표
		(int)m_vSize.x,			// 출력할 비트맵의 가로 사이즈
		(int)m_vSize.y,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 255, 255));	// 제거할 픽셀의 색상 값

	// 가리기 용 텍스처 출력
	BitBlt(hPlgDC,
		0, 0,
		(int)m_vSize.x, (int)m_vSize.y,
		hResetDC,
		0, 0,
		SRCCOPY);

}

void CHitEffect::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

}

void CHitEffect::Set_Angle(float fAngle)
{
	m_fEffectAngle = fAngle;

	float	fDiagonal = (float)sqrtf(pow(m_vSize.x * 0.5f, 2) + pow(m_vSize.y * 0.5f, 2));

	// 좌 상단
	m_tPoint[0].x = LONG((m_vSize.x * 0.5f) + cosf((m_fEffectAngle + 135.f) * PI / 180.f) * fDiagonal);
	m_tPoint[0].y = LONG((m_vSize.y * 0.5f) - sinf((m_fEffectAngle + 135.f) * (PI / 180.f)) * fDiagonal);

	// 우 상단		 
	m_tPoint[1].x = LONG((m_vSize.x * 0.5f) + cosf((m_fEffectAngle + 45.f) * PI / 180.f) * fDiagonal);
	m_tPoint[1].y = LONG((m_vSize.y * 0.5f) - sinf((m_fEffectAngle + 45.f) * PI / 180.f) * fDiagonal);

	// 좌 하단					
	m_tPoint[2].x = LONG((m_vSize.x * 0.5f) + cosf((m_fEffectAngle + 225.f) * PI / 180.f) * fDiagonal);
	m_tPoint[2].y = LONG((m_vSize.y * 0.5f) - sinf((m_fEffectAngle + 225.f) * PI / 180.f) * fDiagonal);
}
