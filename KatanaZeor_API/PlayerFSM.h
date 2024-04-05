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
	vector<CPlayerState*> m_vecState;	//��� ���¸� ���� �ִ� ���� - �ε��� ���ٿ�

	CPlayerState* m_pNowState;			//���� ����
	CPlayerState* m_pPreState;			//���� ����
};

