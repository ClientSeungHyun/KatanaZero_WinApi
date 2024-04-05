#include "Memento.h"

#include "MyBmp.h"

CMemento::CMemento()
{
	
}

CMemento::~CMemento()
{
}

void CMemento::Save_State(VEC2 vPos, VEC2 vLookDir, CMyBmp* aniBmp, bool bDead, bool bIsActive, FRAME tFrame)
{
	m_vPos = vPos;
	m_vLookDir = vLookDir;
	m_AniBmp = aniBmp;
	m_bDead = bDead;
	m_bIsActive = bIsActive;
	m_tFrame = tFrame;
}