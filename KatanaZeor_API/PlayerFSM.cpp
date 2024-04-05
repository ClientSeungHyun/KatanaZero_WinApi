#include "PlayerFSM.h"
#include "PlayerState.h"

CPlayerFSM::CPlayerFSM() :m_pNowState(nullptr), m_pPreState(nullptr)
{
}

CPlayerFSM::~CPlayerFSM()
{
	Release();
}

void CPlayerFSM::Update()
{
	m_pNowState->Update();
}

void CPlayerFSM::Release()
{
	for (auto& iter : m_vecState)
	{
		Safe_Delete<CPlayerState*>(iter);
	}
}

void CPlayerFSM::AddState(CPlayerState* pState)
{
	pState->SetFSM(this);
	m_vecState.push_back(pState);
}

void CPlayerFSM::ChangeState(PLAYERSTATE eState)
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

void CPlayerFSM::Set_State(PLAYERSTATE eState)
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
