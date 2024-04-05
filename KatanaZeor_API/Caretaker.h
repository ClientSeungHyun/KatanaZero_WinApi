#pragma once
#include"Memento.h"

class CCaretaker
{
public:
	CCaretaker();
	~CCaretaker();

public:
	void Add(CMemento memState) { m_MementoList.push_back(memState); }
	int Get_ListSize() { return m_MementoList.size(); }
	void Clear_MementoList();
	CMemento Get_First() { return m_MementoList.front(); }

	CMemento TimeBakc();

private:
	list<CMemento>		m_MementoList;
};

