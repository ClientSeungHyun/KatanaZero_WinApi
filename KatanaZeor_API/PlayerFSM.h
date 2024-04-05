#pragma once
#include"Define.h"

class CPlayerState;

class CPlayerFSM
{
public:
	CPlayerFSM();
	~CPlayerFSM();

public:
	CPlayerState*		Get_State() { return m_pNowState; }
	CPlayerState*		Get_PreState() { return m_pPreState; }

public:
	void				Update();
	void				Release();

public:
	void				AddState(CPlayerState* pState);

	void				ChangeState(PLAYERSTATE eState);
	void				Set_State(PLAYERSTATE eState);

private:
	vector<CPlayerState*> m_vecState;	//모든 상태를 갖고 있는 벡터 - 인덱스 접근용

	CPlayerState* m_pNowState;			//지금 상태
	CPlayerState* m_pPreState;			//이전 상태
};

