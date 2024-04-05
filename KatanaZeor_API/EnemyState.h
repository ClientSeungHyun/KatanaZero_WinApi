#pragma once
#include "Define.h"
#include "EnemyFSM.h"
#include "Boss.h"

#include "RigidBody.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "PlayerFSM.h"
#include "KeyMgr.h"

class CEnemyState
{
public:
	CEnemyState();
	virtual ~CEnemyState();

public:
	ENEMYSTATE				Get_State() { return m_eState; }

	void					SetFSM(CEnemyFSM* pFSM) { m_pEnemyFsm = pFSM; }

public:
	virtual void			Initialize()	PURE;
	virtual void			Update()		PURE;
	virtual void			Release()		PURE;

public:


protected:
	CEnemyFSM*				m_pEnemyFsm;
	ENEMYSTATE				m_eState;
};

