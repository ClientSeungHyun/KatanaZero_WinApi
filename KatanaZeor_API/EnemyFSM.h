#pragma once
#include "Define.h"

class CEnemyState;

class CEnemyFSM
{
public:
	CEnemyFSM();
	~CEnemyFSM();

public:
	CEnemyState* Get_State() { return m_pNowState; }
	CEnemyState* Get_PreState() { return m_pPreState; }

public:
	void				Update();
	void				Release();

public:
	void				AddState(CEnemyState* pState);

	void				ChangeState(ENEMYSTATE eState);
	void				Set_State(ENEMYSTATE eState);

private:
	vector<CEnemyState*> m_vecState;	//��� ���¸� ���� �ִ� ���� - �ε��� ���ٿ�

	CEnemyState* m_pNowState;			//���� ����
	CEnemyState* m_pPreState;			//���� ����
};

