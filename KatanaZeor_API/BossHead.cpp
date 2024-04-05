#include "BossHead.h"
#include "Boss.h"

#include "Collider.h"
#include "RigidBody.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Camera.h"

CBossHead::CBossHead() :m_bIsGround(false)
{
	m_eRenderId = GAMEOBJECT;
	m_eId = OBJ_HEAD;
}

CBossHead::~CBossHead()
{
	Release();
}

void CBossHead::Initialize()
{
	Create_Collider();
	Create_RigidBody();
	Set_StageCol();

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/boss_headfly.bmp", L"Boss_headfly", VEC2(40, 42), VEC2(30, 30), VEC2(3, 0), 6, 100, GetTickCount());
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Boss/boss_headground.bmp", L"Boss_headground", VEC2(90, 28), VEC2(28, 28), VEC2(3, 0), 7, 50, GetTickCount());
	m_vLookDir = { 1.0f,0.f };

    m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_headfly");
	m_pRigidBody->AddForce(VEC2(100000.f, -30000.f));
    Set_Frame();
}

int CBossHead::Update()
{
    Move_Frame();

    return 0;
}

void CBossHead::Late_Update()
{
	m_pCollider->Late_Update();
	m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
	m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

	m_pRigidBody->Set_IsGround(m_bIsGround);
	

	if (m_pAniBmp == CBmpMgr::Get_Instance()->Find_Image(L"Boss_headfly"))
	{
		PixelCollisionBottom();
		m_pRigidBody->Late_Update();
	}

	if (m_pAniBmp == CBmpMgr::Get_Instance()->Find_Image(L"Boss_headground"))
	{
		if(m_tFrame.isPlayDone)
			dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Set_State(BOSS_REALDIE);
	}

}

void CBossHead::Render(HDC hDC)
{
    m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

    HDC hHeadDC = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // 복사 받을 위치의 top
		(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
		(int)m_pAniBmp->Get_ImageSize().y,			 // 복사 받을 세로 길이
		hHeadDC,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// 출력할 비트맵의 시작 y좌표
		(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
		(int)m_pAniBmp->Get_ImageSize().y,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

	m_pCollider->Render(hDC);
}

void CBossHead::Release()
{
	if (m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);

	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);
}

void		CBossHead::PixelCollisionBottom()
{
	for (int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f) + 5; width <= (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f) - 5; ++width)
	{
		int height = (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) + 5;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Boss_headground");
			Set_Frame(false);
			m_vPos.y += 10;
			m_pRigidBody->Set_VelocityY(0.f);
			m_bIsGround = true;
			m_pRigidBody->CalcFriction();

			break;
		}
	}

	for (int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f); height <= (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 10; ++height)
	{
		int width = (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f) + 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_vPos.x -= 10;
			m_pRigidBody->Set_VelocityX(0.f);
			break;
		}
	}

	for (int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f); height <= (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 10; ++height)
	{
		int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f) - 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_vPos.x += 10;
			m_pRigidBody->Set_VelocityX(0.f);
			break;
		}
	}

}