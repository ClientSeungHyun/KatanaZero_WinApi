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

	GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // ���� ���� ��ġ�� left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // ���� ���� ��ġ�� top
		(int)m_pAniBmp->Get_ImageSize().x,			 // ���� ���� ���� ����
		(int)m_pAniBmp->Get_ImageSize().y,			 // ���� ���� ���� ����
		hReflect,					// ��Ʈ���� ������ �ִ� dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// ����� ��Ʈ���� ���� x��ǥ
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// ����� ��Ʈ���� ���� y��ǥ
		(int)m_pAniBmp->Get_ImageSize().x,			// ����� ��Ʈ���� ���� ������
		(int)m_pAniBmp->Get_ImageSize().y,			// ����� ��Ʈ���� ���� ������
		RGB(255, 0, 255));	// ������ �ȼ��� ���� ��
}

void CReflectEffect::Release()
{
}
