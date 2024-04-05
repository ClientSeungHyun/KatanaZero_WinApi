#pragma once
#include "MyBmp.h"

class CMemento
{
public:
	CMemento();
	~CMemento();

public:
	void		Save_State(VEC2 vPos, VEC2 vLookDir, CMyBmp* aniBmp, bool bDead, bool bIsActive, FRAME tFrame );

	VEC2		Get_Pos() { return m_vPos; }
	VEC2		Get_LookDir() { return m_vLookDir; }
	CMyBmp*		Get_Bmp() { return m_AniBmp; }
	bool		Get_IsDead() { return m_bDead; }
	bool		Get_bIsActive(){ return m_bIsActive; }
	FRAME		Get_Frame() { return m_tFrame; }

private:
	VEC2		m_vPos;
	CMyBmp*		m_AniBmp;
	VEC2		m_vLookDir;

	bool		m_bDead;
	bool		m_bIsActive;

	FRAME		m_tFrame;
};

