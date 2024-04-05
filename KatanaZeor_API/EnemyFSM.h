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
	vector<CEnemyState*> m_vecState;	//모든 상태를 갖고 있는 벡터 - 인덱스 접근용

	CEnemyState* m_pNowState;			//지금 상태
	CEnemyState* m_pPreState;			//이전 상태
};

