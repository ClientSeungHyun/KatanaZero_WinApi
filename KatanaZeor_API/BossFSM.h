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
	vector<CBossState*> m_vecState;	//��� ���¸� ���� �ִ� ���� - �ε��� ���ٿ�

	CBossState* m_pNowState;			//���� ����
	CBossState* m_pPreState;			//���� ����
};

