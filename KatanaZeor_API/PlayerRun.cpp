#include "Player.h"
#include "PlayerRun.h"
#include "PlayerFSM.h"

#include "RigidBody.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"


CPlayerRun::CPlayerRun()
{
	m_eState = RUN;
}

CPlayerRun::~CPlayerRun()
{
}

void CPlayerRun::Initialize()
{
	dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_State(RUN);
	//dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_IsFall(true);

	switch (m_pPlayerFsm->Get_PreState()->Get_State())
	{
	case IDLE:
		//idle to run ���
		CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_idle_to_run"));
		CObjMgr::Get_Instance()->Get_Player()->Set_Frame(false);

		break;
	default:
		break;
	}

}

void CPlayerRun::Update()
{
	//�ӵ� �����ϱ�
	CSoundMgr::Get_Instance()->PlaySound(L"player_running_1.wav", SOUND_EFFECT, g_fEffectSound);

	//CSoundMgr::Get_Instance()->SetPlayeSpeed(SOUND_EFFECT, 2.f);

	if (CObjMgr::Get_Instance()->Get_Player()->Get_Frame().isPlayDone)
	{
		CObjMgr::Get_Instance()->Get_Player()->Set_AniBmp(CBmpMgr::Get_Instance()->Find_Image(L"Player_run"));
		CObjMgr::Get_Instance()->Get_Player()->Set_Frame();
	}

	

	//�̵�
	CObjMgr::Get_Instance()->Get_Player()->Get_RigidBody()->Set_VelocityX(350.f * CObjMgr::Get_Instance()->Get_Player()->Get_LookDir().x);

	//���� ó��
	if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING && CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)
	{
		m_pPlayerFsm->ChangeState(IDLE);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_UP)	//����
	{
		m_pPlayerFsm->ChangeState(IDLE);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_UP)		//����
	{
		m_pPlayerFsm->ChangeState(IDLE);
	}
	
	//������
	if (CKeyMgr::Get_Instance()->Get_KeyState('S') == KEY_DOWN) 
	{
		m_pPlayerFsm->ChangeState(ROLL);
	}

	Attack();
	Jump();
}


void CPlayerRun::Release()
{
}
