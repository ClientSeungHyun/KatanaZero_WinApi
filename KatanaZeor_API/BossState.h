#pragma once
#include "Define.h"
#include "BossFSM.h"
#include "Boss.h"

#include "RigidBody.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "PlayerFSM.h"
#include "KeyMgr.h"

class CBossState
{
public:
	CBossState();
	virtual ~CBossState();

public:
	BOSSSTATE				Get_State() { return m_eState; }

	void					SetFSM(CBossFSM* pFSM) { m_pBossFsm = pFSM; }

public:
	virtual void			Initialize()	PURE;
	virtual void			Update()		PURE;
	virtual void			Release()		PURE;

public:


protected:
	CBossFSM*				m_pBossFsm;
	BOSSSTATE				m_eState;
};

