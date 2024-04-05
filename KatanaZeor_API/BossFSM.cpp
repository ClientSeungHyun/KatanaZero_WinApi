#include "BossFSM.h"
#include "BossState.h"

CBossFSM::CBossFSM()
{
}

CBossFSM::~CBossFSM()
{
	Release();
}

void CBossFSM::Update()
{
	m_pNowState->Update();
}

void CBossFSM::Release()
{
	for (auto& iter : m_vecState)
	{
		Safe_Delete<CBossState*>(iter);
	}
}

void CBossFSM::AddState(CBossState* pState)
{
	pState->SetFSM(this);
	m_vecState.push_back(pState);
}

void CBossFSM::ChangeState(BOSSSTATE eState)
{
	m_pPreState = m_pNowState;
	m_pNowState->Release();

	for (int i = 0; i < m_vecState.size(); ++i)
	{
		if (eState == m_vecState[i]->Get_State())
		{
			m_pNowState = m_vecState[i];
			m_pNowState->Initialize();
			return;
		}
	}
}

void CBossFSM::Set_State(BOSSSTATE eState)
{
	for (int i = 0; i < m_vecState.size(); ++i)
	{
		if (eState == m_vecState[i]->Get_State())
		{
			m_pNowState = m_vecState[i];
			m_pPreState = m_vecState[i];
			m_pNowState->Initialize();
			return;
		}
	}
}
