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

	GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // ���� ���� ��ġ�� left
		(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // ���� ���� ��ġ�� top
		(int)m_pAniBmp->Get_ImageSize().x,			 // ���� ���� ���� ����
		(int)m_pAniBmp->Get_ImageSize().y,			 // ���� ���� ���� ����
		hBloodDC,					// ��Ʈ���� ������ �ִ� dc
		m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// ����� ��Ʈ���� ���� x��ǥ
		((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// ����� ��Ʈ���� ���� y��ǥ
		(int)m_pAniBmp->Get_ImageSize().x,			// ����� ��Ʈ���� ���� ������
		(int)m_pAniBmp->Get_ImageSize().y,			// ����� ��Ʈ���� ���� ������
		RGB(255, 0, 255));	// ������ �ȼ��� ���� ��

}

void CBloodEffect::Release()
{
}
