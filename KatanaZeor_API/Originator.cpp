#include "Originator.h"
#include "Memento.h"

COriginator::COriginator()
{
}

COriginator::~COriginator()
{
}

void COriginator::Save_Player(CPlayer* copyPlayer)
{
	m_pCopyPlayer = *copyPlayer;
}

CMemento COriginator::SaveState_Memento()
{
	CMemento newMemento;
	newMemento.Save_Player(m_pCopyPlayer);
	
		return newMemento;
	return CMemento();
}

//void COriginator::Save_State(const list<CObj*> ObjList)
//{
//	copy(ObjList.begin(), ObjList.end(), back_inserter(*m_ObjList));
//}
