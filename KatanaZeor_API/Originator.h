#pragma once
#include "Obj.h"
#include "Player.h"

class CMemento;

class COriginator
{
public:
	COriginator();
	~COriginator();

public:
	//void			Save_State(const list<CObj*> ObjList);
	void			Save_Player(CPlayer* copyPlayer);

	CMemento		SaveState_Memento();

private:
	//list<CObj*>		m_ObjList[OBJ_END];
	CPlayer		m_pCopyPlayer;
};

