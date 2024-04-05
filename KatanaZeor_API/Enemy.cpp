#include "Enemy.h"
#include "Player.h"
#include "BaseBullet.h"
#include "Effect.h"
#include "HitEffect.h"
#include "BloodEffect.h"
#include "BloodBGEffect.h"

#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Collider.h"
#include "RigidBody.h"

#include "Caretaker.h"

CEnemy::CEnemy() : m_bIsGround(true), m_pDetectCollider(nullptr), m_bIsFind(false), m_bIsAttack(false), m_bIsPatrol(false),
m_dwDetectEffectTime(0), m_bIsChase(false), m_bIsBlood(false), m_pBlood(nullptr), m_bIsAttacking(false)
{
	m_eId = OBJ_ENEMY;
}

CEnemy::~CEnemy()
{
	Release();
}

void CEnemy::Initialize()
{

}

int CEnemy::Update()
{
	//과거 회귀
	for (int i = 0; i < 2; ++i)
	{
		if (m_pCareTaker->Get_ListSize() > 0)
		{
			Set_MementoState(m_pCareTaker->TimeBakc());
		}
	}

    return 0;
}

void CEnemy::Late_Update()
{
	m_pCareTaker->Add(Save_State());
}

void CEnemy::Render(HDC hDC)
{
}

void CEnemy::Release()
{
	if (m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);

	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

	if(m_pDetectCollider)
		Safe_Delete<CCollider*>(m_pDetectCollider);
	
	if (m_pCareTaker)
		Safe_Delete<CCaretaker*>(m_pCareTaker);

	if(m_pEffect)
		Safe_Delete<CObj*>(m_pEffect);
}

void CEnemy::OnCollisionEnter(CObj* _pOther)
{
	if (_pOther->Get_Id() == OBJ_BULLET && _pOther->Get_IsActive() && !m_bDead)
	{
		if (m_eType == SHIELDCOP)
			return;
		if (dynamic_cast<CBullet*>(_pOther)->Get_Type() == BASE)
		{
			if (dynamic_cast<CBaseBullet*>(_pOther)->Get_IsReflect())
			{
				VEC2 dir{ -_pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
				dir.Normalize();

				if (!m_bIsBlood)
				{
					CObj* pBloodEffect = new CBloodEffect;
					pBloodEffect->Set_Pos(m_vPos.x, m_vPos.y);
					pBloodEffect->Set_LookDir(m_vLookDir);
					pBloodEffect->Initialize();
					CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodEffect);

					//blood 이펙트
					CObj* pBloodBGEffect = new CBloodBGEffect;
					pBloodBGEffect->Set_Pos(m_vPos.x - m_vLookDir.x * 30.f, m_vPos.y - 30.f);
					pBloodBGEffect->Set_LookDir(m_vLookDir);
					pBloodBGEffect->Initialize();
					CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodBGEffect);

					m_bIsBlood = true;
				}

				m_pRigidBody->AddForce(VEC2(dir.x * 100000.f, -300.f));
				m_bIsGround = false;
				m_bDead = true;
				_pOther->Set_IsActive(false);

				//hit 이펙트
				CObj* pHitEffect = new CHitEffect;
				pHitEffect->Set_Pos(m_vPos.x, m_vPos.y);
				pHitEffect->Initialize();
				CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pHitEffect);

				switch (m_eType)
				{
				case BOLD:
					m_pPrevAniBmp = m_pAniBmp;
					m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_hurt");
					break;

				case GRUNT:
					m_pPrevAniBmp = m_pAniBmp;
					m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_grunt_hurt");
					break;

				case POMP:
					m_pPrevAniBmp = m_pAniBmp;
					m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_hurt");
					break;

				default:
					break;
				}
				Set_Frame();
			}
		}
	}

	//플레이어 추적
	if (_pOther->Get_Id() == OBJ_PLAYER)
	{
		if (abs(_pOther->Get_Pos().y - m_vPos.y) > 100.f)
			return;

		if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_IsSpecialAttack())
		{
			return;
		}

		if (!m_pEffect && !m_bIsFind)
		{
			m_pEffect = new CEffect();
			dynamic_cast<CEffect*>(m_pEffect)->Set_EffectId(EFFECT_END);

			m_pEffect->Initialize();
			m_pEffect->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Enemy_follow"));
			m_pEffect->Set_Frame();
			m_pEffect->Set_IsActive(true);
			m_pEffect->Set_Pos(m_vPos.x - 15 * m_vLookDir.x, m_vPos.y - 50);
			m_pEffect->Set_LookDir(VEC2(1.f, 0.f));

			m_dwDetectEffectTime = GetTickCount();
			m_bIsFind = true;
			GetCollisionSet()->erase(_pOther);
		}
	}

	//검이나 아이템에 맞음
	if (((_pOther->Get_Id() == OBJ_EFFECT && dynamic_cast<CEffect*>(_pOther)->Get_EfeectId() == EFFECT_PLAYERATTACK) || _pOther->Get_Id() == OBJ_ITEM) && _pOther->Get_IsActive() && !m_bDead)
	{
		if (m_eType == POMP || m_eType == SHIELDCOP) return;

		if (_pOther->Get_Id() == OBJ_ITEM)
		{
			_pOther->Set_Dead();
		}

		VEC2 dir{ _pOther->Get_LookDir().x ,-_pOther->Get_LookDir().y };
		dir.Normalize();

		CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", ENEMY_DIE1, g_fEffectSound);

		//hit 이펙트
		CObj* pHitEffect = new CHitEffect;
		pHitEffect->Set_Pos(m_vPos.x, m_vPos.y);
		pHitEffect->Initialize();
		CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pHitEffect);

		if (!m_bIsBlood)
		{
			CObj* pBloodEffect = new CBloodEffect;
			pBloodEffect->Set_Pos(m_vPos.x, m_vPos.y);
			pBloodEffect->Set_LookDir(m_vLookDir);
			pBloodEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodEffect);

			//blood 이펙트
			CObj* pBloodBGEffect = new CBloodBGEffect;
			pBloodBGEffect->Set_Pos(m_vPos.x - m_vLookDir.x * 30.f, m_vPos.y - 30.f);
			pBloodBGEffect->Set_LookDir(m_vLookDir);
			pBloodBGEffect->Initialize();
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodBGEffect);

			m_bIsBlood = true;
		}

		switch (m_eType)
		{
		case BOLD:
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_hurt");
			break;

		case GRUNT:
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_grunt_hurt");
			break;

		case POMP:
			m_pPrevAniBmp = m_pAniBmp;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_hurt");
			break;

		default:
			break;
		}
		Set_Frame();

		m_pRigidBody->AddForce(VEC2(dir.x * 100000.f, -200.f));
		m_bIsGround = false;
		if (m_pTarget->Get_CameraPos().x >= m_vCameraPos.x)
		{
			m_vLookDir.x = 1.f;
		}
		else
		{
			m_vLookDir.x = -1.f;
		}

		m_bDead = true;
	}

}

void CEnemy::OnCollisionStay(CObj* _pOther)
{
	
}

void CEnemy::OnCollisionExit(CObj* _pOther)
{
}

void CEnemy::EnemyDie()
{
	CSoundMgr::Get_Instance()->PlaySound(L"sound_enemy_death_sword_01.wav", ENEMY_DIE1, g_fEffectSound);

	if (!m_bIsBlood)
	{
		CObj* pBloodEffect = new CBloodEffect;
		pBloodEffect->Set_Pos(m_vPos.x, m_vPos.y);
		pBloodEffect->Set_LookDir(m_vLookDir);
		pBloodEffect->Initialize();
		CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodEffect);

		//blood 이펙트
		CObj* pBloodBGEffect = new CBloodBGEffect;
		pBloodBGEffect->Set_Pos(m_vPos.x - m_vLookDir.x * 30.f, m_vPos.y - 30.f);
		pBloodBGEffect->Set_LookDir(m_vLookDir);
		pBloodBGEffect->Initialize();
		CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pBloodBGEffect);

		m_bIsBlood = true;
	}

	switch (m_eType)
	{
	case BOLD:
		m_pPrevAniBmp = m_pAniBmp;
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_bold_hurt");
		break;

	case GRUNT:
		m_pPrevAniBmp = m_pAniBmp;
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_grunt_hurt");
		break;

	case POMP:
		m_pPrevAniBmp = m_pAniBmp;
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_hurt");
		break;

	case SHIELDCOP:
		m_pPrevAniBmp = m_pAniBmp;
		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Enemy_shieldcop_hurt");
		break;

	default:
		break;
	}
	Set_Frame(false);

	if (m_pTarget->Get_CameraPos().x >= m_vCameraPos.x)
	{
		m_vLookDir.x = 1.f;
	}
	else
	{
		m_vLookDir.x = -1.f;
	}

	m_pRigidBody->AddForce(VEC2(200.f, -10.f));
	m_bIsGround = false;
	m_bDead = true;
}

void CEnemy::Create_DetectCollider()
{
		if (!m_pDetectCollider)
		{
			m_pDetectCollider = new CCollider();
			m_pDetectCollider->m_pOwner = this;
			m_pDetectCollider->Set_IsRectCollider(false);
		}
}

void CEnemy::PixelCollisionBottom()
{
	//천장 
	for (int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f); width <= (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f - 5); ++width)
	{
		int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f) + 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_pRigidBody->Set_IsGround(true);

			m_bIsGround = true;
		}

		if (r == 0 && g == 255 && b == 0 )
		{
			m_pRigidBody->Set_IsGround(true);

			m_bIsGround = true;
		}
	}

	//바닥
	for (int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f) + 5; width <= (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f)- 5; ++width)
	{
		int height = (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) + 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_pRigidBody->Set_VelocityX(0.f);
			m_pRigidBody->Set_VelocityY(0.f);
			m_vPos.y -= 1;

			m_bIsGround = true;
			break;
		}
		m_pRigidBody->Set_IsGround(m_bIsGround);
	}

	//우측
	for (int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f); height <= (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 20; ++height)
	{
		int width = (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f) + 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		//일반 벽
		if (r == 255 && g == 0 && b == 255)
		{
			m_vPos.x -= 8;
			m_pRigidBody->Set_VelocityX(0.f);

			break;
		}
	}

	//좌측
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

void CEnemy::TimeBakcDone()
{
	m_pCollisionOthers.clear();
	m_pRigidBody->ResetGravityAccel();

	m_bDead = false;
	m_bIsFind = false;
	m_bIsAttack = false;
	m_bIsGround = false;
	m_bIsMoveFrame = true;
	m_bIsBlood = false;

	if (m_pBlood)
	{
		m_pBlood->Set_Dead();
		m_pBlood = nullptr;
	}
	
	if (m_pCareTaker->Get_ListSize() > 0)
	{
		Set_MementoState(m_pCareTaker->Get_First());
	}

	m_pCareTaker->Clear_MementoList();
	m_vPos = m_vStartPos;
}
