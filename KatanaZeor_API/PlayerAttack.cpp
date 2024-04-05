#include "PlayerAttack.h"
#include "Effect.h"
#include "Camera.h"

CPlayerAttack::CPlayerAttack() :m_pEffect(nullptr)
{
	m_eState = ATTACK;
	srand((unsigned)time(NULL));
}

CPlayerAttack::~CPlayerAttack()
{
}

void CPlayerAttack::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(ATTACK);

	m_vAttackDir = Calculate_AttackDir();
	m_fAttackPower = 300;

	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_RigidBody()->Set_Velocity(VEC2(m_vAttackDir.x * m_fAttackPower, m_vAttackDir.y * m_fAttackPower));
	
	if (m_vAttackDir.x >= 0)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(1, 0.f));
	}
	else
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_LookDir(VEC2(-1, 0.f));
	}
	
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsGround(false);

	if (!m_pEffect)
	{
		m_pEffect = new CEffect();
		dynamic_cast<CEffect*>(m_pEffect)->Set_EffectId(EFFECT_PLAYERATTACK);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, m_pEffect);
		m_pEffect->Initialize();
		m_pEffect->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_slash"));
		dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_EffectOBj(m_pEffect);
	}
	m_pEffect->Set_IsActive(true);
	m_pEffect->Set_Pos(CObjMgr::Get_Instance()->Get_Player()->Get_Pos().x + m_vAttackDir.x * 50, CObjMgr::Get_Instance()->Get_Player()->Get_Pos().y + m_vAttackDir.y * 50);
	m_pEffect->Set_Frame(false);

	CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_attack"));
	CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);
	CSoundMgr::Get_Instance()->StopSound(SOUND_ATTACK);

	int r = rand() % 3;

	switch (r)
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"slash_1.wav", SOUND_ATTACK, g_fEffectSound);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"slash_2.wav", SOUND_ATTACK, g_fEffectSound);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"slash_3.wav", SOUND_ATTACK, g_fEffectSound);
		break;
	default:
		break;
	}
	m_iAttackCount = 1;
}

void CPlayerAttack::Update()
{
	m_pEffect->Set_Pos(CObjMgr::Get_Instance()->Get_Player()->Get_Pos().x + m_vAttackDir.x * 50, CObjMgr::Get_Instance()->Get_Player()->Get_Pos().y + m_vAttackDir.y * 50);
	m_pEffect->Set_LookDir(CObjMgr::Get_Instance()->Get_Player()->Get_LookDir());
	
	if(m_pEffect->Get_Frame().isPlayDone)
		m_pEffect->Set_IsActive(false);

	//한 번 재생이 끝났으면
	if (CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone)
	{
		m_pPlayerFsm->ChangeState(FALL);
		m_pEffect->Set_IsActive(false);
	}
}

void CPlayerAttack::Release()
{
}

//마우스 방향 가져오기
VEC2		CPlayerAttack::Calculate_AttackDir()
{
	POINT pt;
	VEC2 vPlayerPos = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_CameraPos();
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	VEC2 vDir = { pt.x - vPlayerPos.x, pt.y - vPlayerPos.y };

	////바닥으로  공격하면 x축만 힘을 받도록 바꿔줌
	//if (vPlayerPos.y < pt.y)
	//	vDir.y = 0.f;
	vDir.Normalize();

	return vDir;
}