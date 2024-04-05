#pragma once

#include "Define.h"

class CUI;
class CObj;

class COriginator;
class CCareTaker;

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	const TCHAR*			Get_FrameKey() { return m_pFrameKey; }

	bool					Get_IsClear() { return m_bIsClear; }
	void					Set_IsClear(bool bClear) { m_bIsClear = bClear; }

public:
	virtual		void		Initialize()	PURE;
	virtual		int			Update()		PURE;
	virtual		void		Late_Update()	PURE;
	virtual		void		Render(HDC hDC)	PURE;
	virtual		void		Release()		PURE;


protected:
	VEC2					m_vStartPos;

	CObj*					m_pText;

	FRAME					m_tFrame;
	const TCHAR*			m_pFrameKey = L"";

	bool					m_bIsClear;
	int						m_iEnemyCount;
};

