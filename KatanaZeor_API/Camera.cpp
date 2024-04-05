#include "stdafx.h"
#include "Camera.h"
#include "Obj.h"
#include "Boss.h"

#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SoundMgr.h"

#include "UI.h"

CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera() : m_pTarget(nullptr), m_bIsCameraShake(false), m_iSceneChange(-3100), m_iLastAlpha(0)
{
	ZeroMemory(&m_vMaxPos, sizeof(VEC2));
	ZeroMemory(&m_vMinPos, sizeof(VEC2));
}

CCamera::~CCamera()
{
}



VEC2 CCamera::GetRenderPos(VEC2 _vObjPos)
{
	return _vObjPos - m_tDiff;
}

VEC2 CCamera::GetRenderPos(float _x, float _y)
{
	return VEC2(_x - m_tDiff.x, _y - m_tDiff.y);
}

VEC2 CCamera::GetWindowPos(VEC2 _vObjPos)
{
	return _vObjPos + m_tDiff;
}

VEC2 CCamera::GetWindowPos(float _x, float _y)
{
	return VEC2(m_tDiff.x+_x, m_tDiff.y + _y);
}

void CCamera::StartShakeCamera()
{
	if (!m_bIsCameraShake)
	{
		m_bIsCameraShake = true;
		m_dwShakeTime = GetTickCount();
		m_vOriginPos = m_tLookAtPos;
	}
}

void CCamera::ShakeCamera()
{
	if (m_dwShakeTime + 100 > GetTickCount())
	{
		m_vShakePos.x = ((sinf((float)GetTickCount()) * 50));
		m_vShakePos.y = ((cosf((float)GetTickCount()) * 50));
	}

	if (m_dwShakeTime + 150 < GetTickCount())
	{
		m_vShakePos = { 0.f,0.f };
		m_bIsCameraShake = false;
		m_tLookAtPos = m_vOriginPos;
	}
	
}

void CCamera::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CameraEffect/Camera_Black.bmp", L"Camera_Black");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CameraEffect/Camera_Noise.bmp", L"Camera_Noise");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CameraEffect/Camera_Scenechange.bmp", L"Camera_Scenechange");

	m_tNoiseFrame = { 0,5,5,0,50,GetTickCount(),true,false };
}

int CCamera::Update()
{
	if (m_bIsCameraShake)
		ShakeCamera();

	if (m_pTarget)
	{

		if ((m_pTarget->m_vPos.x <= m_vMaxPos.x && m_pTarget->m_vPos.x >= m_vMinPos.x))
		{

			m_tLookAtPos.x = m_pTarget->m_vPos.x;
		}
		if ((m_pTarget->m_vPos.y <= m_vMaxPos.y && m_pTarget->m_vPos.y >= m_vMinPos.y))
		{
			m_tLookAtPos.y = m_pTarget->m_vPos.y;
		}
	}

	m_tLookAtPos.x += m_vShakePos.x;
	m_tLookAtPos.y += m_vShakePos.y;

	//ȭ�� �ѱ��
	if (CSceneMgr::Get_Instance()->Get_SceneState() == SC_READY && CSceneMgr::Get_Instance()->Get_CurScene() != SC_TITLE)
	{
			m_iSceneChange += 50;

			if (m_iSceneChange > 0 && m_iSceneChange < 55)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"sound_transition_end.wav", SCENE_CHANGE, g_fEffectSound);
			}

		if (m_iSceneChange > 3000)
		{
			m_iSceneChange = -3000;
			CSceneMgr::Get_Instance()->Set_SceneState(SC_READY_COMPLETE);
		}
	}

	//���� ���� ���ĺ��� ����
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS2)
	{
		if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_State() == BOSS_REALDIE)
		{
			if (m_iLastAlpha < 254)
			{
				m_iLastAlpha += 1;
			}
			else
			{
				CSceneMgr::Get_Instance()->Scene_Change(SC_ENDING);
			}
		}
		else
		{
			m_iLastAlpha = 0;
		}
	}

	CalculateDiff();

	//if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	//{
	//	m_tLookAtPos.y -= 10.f;
	//}
	//if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	//{
	//	m_tLookAtPos.x-= 10.f;
	//}
	//if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	//{
	//	m_tLookAtPos.y += 10.f;
	//}
	//if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	//{
	//	m_tLookAtPos.x += 10.f;
	//}

	return 0;
}

void CCamera::Render(HDC hDC)
{
	BLENDFUNCTION bf{};

	bf.BlendOp = 0;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_OVER;

	
	//�ð� ȸ��
	if (g_bIsTimeBack)
	{
		HDC hNoiseDC = CBmpMgr::Get_Instance()->Find_DC(L"Camera_Noise");
		bf.SourceConstantAlpha = 90;

		if (m_tNoiseFrame.dwTime + m_tNoiseFrame.dwSpeed < GetTickCount())
		{
			m_tNoiseFrame.iFrameStart = m_tNoiseFrame.iFrameStart + 1;
			m_tNoiseFrame.dwTime = GetTickCount();

			if (m_tNoiseFrame.iFrameStart > m_tNoiseFrame.iFrameRealEnd)
			{
				m_tNoiseFrame.iFrameStart = 0;
			}
		}

		GdiAlphaBlend(
			hDC,
			0,
			0,	//���簡 ���۵� ��ġ
			1344, 
			744,
			hNoiseDC,
			m_tNoiseFrame.iFrameStart * 1344,
			0,
			1344, 
			744,
			bf);
	}

	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS2)
	{
		if (dynamic_cast<CBoss*>(CObjMgr::Get_Instance()->Get_Boss())->Get_State() == BOSS_REALDIE)
		{
			HDC hNoiseDC = CBmpMgr::Get_Instance()->Find_DC(L"Camera_Black");
			bf.SourceConstantAlpha = m_iLastAlpha;

			GdiAlphaBlend(
				hDC,
				0,
				0,	//���簡 ���۵� ��ġ
				1344,
				744,
				hNoiseDC,
				0,
				0,
				1344,
				744,
				bf);
		}
	}

	//�� �ε�
	if (CSceneMgr::Get_Instance()->Get_SceneState() == SC_READY)
	{
		HDC hSceneChangeDC = CBmpMgr::Get_Instance()->Find_DC(L"Camera_Scenechange");

		GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
			m_iSceneChange, // ���� ���� ��ġ�� left
			0,			 // ���� ���� ��ġ�� top
			3000,			 // ���� ���� ���� ����
			744,			 // ���� ���� ���� ����
			hSceneChangeDC,					// ��Ʈ���� ������ �ִ� dc
			0,						// ����� ��Ʈ���� ���� x��ǥ
			0,						// ����� ��Ʈ���� ���� y��ǥ
			3000,			// ����� ��Ʈ���� ���� ������
			744,			// ����� ��Ʈ���� ���� ������
			RGB(255, 255, 255));	// ������ �ȼ��� ���� ��
	}
	
}

void CCamera::CalculateDiff()
{
	VEC2 vCenter(WINCX * 0.5f, WINCY * 0.5f);

	m_tDiff = (m_tLookAtPos - vCenter);
}
