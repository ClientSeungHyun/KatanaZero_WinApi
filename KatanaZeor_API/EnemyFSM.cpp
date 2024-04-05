#include "EnemyFSM.h"
#include "EnemyState.h"

CEnemyFSM::CEnemyFSM()
{
}

CEnemyFSM::~CEnemyFSM()
{
	Release();
}

void CEnemyFSM::Update()
{
	m_pNowState->Update();
}

void CEnemyFSM::Release()
{
	for (auto& iter : m_vecState)
	{
		Safe_Delete<CEnemyState*>(iter);
	}
}

void CEnemyFSM::AddState(CEnemyState* pState)
{
	pState->SetFSM(this);
	m_vecState.push_back(pState);
}

void CEnemyFSM::ChangeState(ENEMYSTATE eState)
{
	m_pPreState = m_pNowState;
	m_pNowState->Release();

	for (UINT i = 0; i < m_vecState.size(); ++i)
	{
		if (eState == m_vecState[i]->Get_State())
		{
			m_pNowState = m_vecState[i];
			m_pNowState->Initialize();
			return;
		}
	}
}

void CEnemyFSM::Set_State(ENEMYSTATE eState)
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
