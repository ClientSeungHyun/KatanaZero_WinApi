#include "Caretaker.h"

CCaretaker::CCaretaker()
{
}

CCaretaker::~CCaretaker()
{
}

void CCaretaker::Clear_MementoList()
{
	m_MementoList.clear();
}

CMemento		CCaretaker::TimeBakc()
{
	CMemento memento = m_MementoList.back();
	m_MementoList.pop_back();
	
	return memento;
}
