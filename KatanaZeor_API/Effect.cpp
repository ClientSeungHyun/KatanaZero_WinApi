#include "Effect.h"
#include "BaseBullet.h"
#include "Boss.h"
#include "Player.h"
#include "ReflectEffect.h"

#include "Camera.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "RigidBody.h"
#include "Collider.h"
#include "SoundMgr.h"

CEffect::CEffect()
{
	m_eRenderId = EFFECT_LATER;
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	Create_Collider();

	m_eId = OBJ_EFFECT;
}

int CEffect::Update()
{
	return 0;
}

void CEffect::Late_Update()
{
	__super::Move_Frame();
	m_pCollider->Late_Update();

	if (m_bIsActive)
	{
		m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
		m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));
	}

}

void CEffect::Render(HDC hDC)
{
	if (m_bIsActive)
	{
		m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

		HDC	hMemDC = m_pAniBmp->Get_MemDC();

		GdiTransparentBlt(hDC, // �������� �׸��� �׸� dc
			(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // ���� ���� ��ġ�� left
			(int)(m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f),			 // ���� ���� ��ġ�� top
			(int)m_pAniBmp->Get_ImageSize().x,			 // ���� ���� ���� ����
			(int)m_pAniBmp->Get_ImageSize().y,			 // ���� ���� ���� ����
			hMemDC,					// ��Ʈ���� ������ �ִ� dc
			m_tFrame.iFrameStart * (int)m_pAniBmp->Get_ImageSize().x,						// ����� ��Ʈ���� ���� x��ǥ
			((int)(m_vLookDir.x + 2) % 3) * (int)m_pAniBmp->Get_ImageSize().y,						// ����� ��Ʈ���� ���� y��ǥ
			(int)m_pAniBmp->Get_ImageSize().x,			// ����� ��Ʈ���� ���� ������
			(int)m_pAniBmp->Get_ImageSize().y,			// ����� ��Ʈ���� ���� ������
			RGB(255, 0, 255));	// ������ �ȼ��� ���� ��

		
	}
	m_pCollider->Render(hDC);
}

void CEffect::Release()
{
	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);
}

void CEffect::OnCollisionEnter(CObj* _pOther)
{
	//�Ѿ� ƨ�ܳ���
	if (_pOther->Get_Id() == OBJ_BULLET && m_bIsActive && m_eEffectID == EFFECT_PLAYERATTACK)
	{
		if (dynamic_cast<CBullet*>(_pOther)->Get_Type() == BASE)
		{
			if (!dynamic_cast<CBaseBullet*>(_pOther)->Get_IsReflect())
			{
				dynamic_cast<CBaseBullet*>(_pOther)->Set_BulletPower(dynamic_cast<CBaseBullet*>(_pOther)->Get_BulletPower() * -1);
				dynamic_cast<CBaseBullet*>(_pOther)->Set_IsReflect(true);

				//hit ����Ʈ
				CObj* pReflectEffect = new CReflectEffect;
				pReflectEffect->Set_Pos(m_vPos.x, m_vPos.y);
				pReflectEffect->Initialize();
				CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pReflectEffect);

				CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundMgr::Get_Instance()->PlaySound(L"slash_bullet.wav", SOUND_EFFECT, g_fEffectSound);
			}
		}
	}

	if (_pOther->Get_Id() == OBJ_PLAYER )
	{
		if (m_eEffectID == EFFECT_ENEMYATTACK && m_bIsActive && m_pCollider->Get_IsActive())
		{
			_pOther->Set_Dead();
			CSoundMgr::Get_Instance()->PlaySound(L"playerdie.wav", SOUND_DIE, g_fEffectSound);
			dynamic_cast<CPlayer*>(_pOther)->Change_StateHURT();
		}
	}

	if (_pOther->Get_Id() == OBJ_BOSS)
	{
		if (m_eEffectID == EFFECT_PLAYERATTACK && m_bIsActive && _pOther->Get_Collider()->Get_IsActive())
		{
			dynamic_cast<CBoss*>(_pOther)->Boss_Hit();
			//���� �߰��ϱ�
		}
	}
}

void CEffect::OnCollisionStay(CObj* _pOther)
{
	////�Ѿ� ƨ�ܳ���
	//if (_pOther->Get_Id() == OBJ_BULLET && m_bIsActive && !dynamic_cast<CBaseBullet*>(_pOther)->Get_IsReflect() && m_eEffectID == EFFECT_PLAYERATTACK)
	//{
	//	dynamic_cast<CBaseBullet*>(_pOther)->Set_BulletPower(dynamic_cast<CBaseBullet*>(_pOther)->Get_BulletPower() * -1);
	//	dynamic_cast<CBaseBullet*>(_pOther)->Set_IsReflect(true);

	//	CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
	//	CSoundMgr::Get_Instance()->PlaySound(L"slash_bullet.wav", SOUND_EFFECT, g_fEffectSound);
	//}

	//if (_pOther->Get_Id() == OBJ_PLAYER && m_bIsActive && m_eEffectID == ENEMY_ATTACK)
	//{
	//	_pOther->Set_Dead();
	//	m_pCollisionOthers.erase(_pOther);
	//}
}
