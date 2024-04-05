#pragma once
#include "Define.h"
#include "Player.h"

#include "RigidBody.h"

#include "ObjMgr.h"
#include "BmpMgr.h"
#include "PlayerFSM.h"
#include "KeyMgr.h"
#include "SoundMgr.h"


class CPlayerState
{
public:
	CPlayerState();
	virtual ~CPlayerState();

public:
	PLAYERSTATE				Get_State() { return m_eState; }

	void					SetFSM(CPlayerFSM* pFSM) { m_pPlayerFsm = pFSM; }

public:
	virtual void			Initialize()	PURE;
	virtual void			Update()		PURE;
	virtual void			Release()		PURE;

public:
	void					Jump();
	void					Attack();


protected:
    CPlayerFSM*				m_pPlayerFsm;
    PLAYERSTATE				m_eState;
};

