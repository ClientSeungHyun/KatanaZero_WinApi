#pragma once
#include "Define.h"

class CBossState;

class CBossFSM
{
public:
	CBossFSM();
	~CBossFSM();

public:
	CBossState* Get_State() { return m_pNowState; }
	CBossState* Get_PreState() { return m_pPreState; }

public:
	void				Update();
	void				Release();

public:
	void				AddState(CBossState* pState);

	void				ChangeState(BOSSSTATE eState);
	void				Set_State(BOSSSTATE eState);

private:
	vector<CBossState*> m_vecState;	//모든 상태를 갖고 있는 벡터 - 인덱스 접근용

	CBossState* m_pNowState;			//지금 상태
	CBossState* m_pPreState;			//이전 상태
};

