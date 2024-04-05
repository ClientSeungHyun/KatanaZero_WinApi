#pragma once

#include "Define.h"

typedef struct tagKeyInfo
{
	KEYSTATE	eState;
	bool		bPrevPush;
}KEYINFO;

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	KEYSTATE			Get_KeyState(int _iKey) { return m_tKeyState[_iKey].eState; }

public:
	static CKeyMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyMgr;

		return m_pInstance;
	}
	static void		Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void				Update();

private:
	static CKeyMgr*		m_pInstance;
	KEYINFO				m_tKeyState[VK_MAX];



};

