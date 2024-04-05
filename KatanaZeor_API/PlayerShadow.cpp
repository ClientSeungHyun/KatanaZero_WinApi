#include "PlayerShadow.h"
#include "PlayerState.h"
#include "PlayerFSM.h"
#include "Boss.h"

#include "Camera.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "TimeMgr.h"

CPlayerShadow::CPlayerShadow():m_bRender(false), m_iAlpha(0), m_pPlayer(nullptr), m_dwShadowTime(0), m_dwDestroyTime(0), m_AlphaDC(nullptr)
{
	m_eRenderId = SHADOW;
	ZeroMemory(&m_BlendFunction, sizeof(BLENDFUNCTION));
}

CPlayerShadow::~CPlayerShadow()
{
}

void CPlayerShadow::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Shadow/player_run_shadow.bmp", L"Player_run_shadow", VEC2(88, 64), VEC2(40, 64), VEC2(0, 0), 9, 60, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Shadow/player_roll_shadow.bmp", L"Player_roll_shadow", VEC2(94, 64), VEC2(30, 64), VEC2(10, 0), 6, 60, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Shadow/player_fall_shadow.bmp", L"Player_fall_shadow", VEC2(82, 96), VEC2(30, 75), VEC2(10, 0), 3, 100, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Shadow/player_flip_shadow.bmp", L"Player_flip_shadow", VEC2(96, 88), VEC2(25, 70), VEC2(20, 0), 10, 30, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Shadow/player_attack_shadow.bmp", L"Player_attack_shadow", VEC2(120, 82), VEC2(30, 75), VEC2(10, 0), 3, 50, GetTickCount());

	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();

	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.AlphaFormat = 0;

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_run_shadow");
	Set_Frame();

	m_dwDestroyTime = GetTickCount();
}

int CPlayerShadow::Update(void)
{
	m_BlendFunction.SourceConstantAlpha--;

	if (!CObjMgr::Get_Instance()->Get_IsPlayerEmpty())
		m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	else
		m_pPlayer = nullptr;


	if (m_pPlayer && !g_bIsTimeBack)
	{
		if (m_dwDestroyTime + (m_dwShadowTime * (int)(1 / CTimeMgr::Get_Instance()->Get_TimeScale())) < GetTickCount())
		{
			m_vPos.x = m_pPlayer->Get_Pos().x;
			m_vPos.y = m_pPlayer->Get_Pos().y;
			m_vLookDir = dynamic_cast<CPlayer*>(m_pPlayer)->Get_LookDir();
			m_dwDestroyTime = GetTickCount();
		}
	}

	Motion_Change();
	Move_Frame();

	return OBJ_NOEVENT;
}

void CPlayerShadow::Late_Update(void)
{
}

void CPlayerShadow::Render(HDC hDC)
{
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS2)
	{
		if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_State() == BOSS_REALDIE)
		{
			return;
		}
	}
	
	if (m_bRender && !CObjMgr::Get_Instance()->Get_Player()->Get_Dead() && !g_bIsTimeBack )
	{
		m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

		HDC	hMemDC = m_pAniBmp->Get_MemDC();

		HDC AlphaDC = CreateCompatibleDC(hMemDC);
		HBITMAP TempDC = CreateCompatibleBitmap(hMemDC, WINCX, WINCY);
		HBITMAP	OldTempDC = (HBITMAP)SelectObject(AlphaDC, TempDC);
		m_BlendFunction.SourceConstantAlpha = m_iAlpha;

		GdiTransparentBlt(AlphaDC,
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f),
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),
			(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
			(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
			hDC,
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
			(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
			(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
			RGB(255, 0, 255));

		GdiTransparentBlt(AlphaDC, // 최종적인 그림을 그릴 dc
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
			(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
			(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
			hMemDC,					// 비트맵을 가지고 있는 dc
			m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
			((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// 출력할 비트맵의 시작 y좌표
			(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
			(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
			RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

		GdiAlphaBlend(hDC,
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
			(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
			(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
			AlphaDC,
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
			(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
			(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
			m_BlendFunction);

		DeleteObject(SelectObject(AlphaDC, OldTempDC));
		DeleteDC(AlphaDC);
	}

	
}

void CPlayerShadow::Release(void)
{
}

void CPlayerShadow::Motion_Change()
{
	PLAYERSTATE eCurState = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerFSM()->Get_State()->Get_State();
	PLAYERSTATE ePrevState = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerFSM()->Get_PreState()->Get_State();;

	if (eCurState != ePrevState)
	{
		switch (eCurState)
		{
		case RUN:
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_run_shadow");
			m_bRender = true;
			break;

		case ROLL:
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_roll_shadow");
			m_bRender = true;
			break;

		case ATTACK:
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_attack_shadow");
			m_bRender = true;
			break;

		case FALL:
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_fall_shadow");
			m_bRender = true;
			break;

		case FLIP:
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_flip_shadow");
			m_bRender = true;
			break;

		default:
			m_bRender = false;
			break;
		}
	}
}
