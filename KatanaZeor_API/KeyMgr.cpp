#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr*	CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	for (int i = 0; i < (int)KEY_NOTHING; ++i)
	{
		m_tKeyState[i].eState = KEY_NOTHING;
		m_tKeyState[i].bPrevPush = false;
	}
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Update()
{
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)VK_MAX; ++i)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (m_tKeyState[i].bPrevPush)
				{
					m_tKeyState[i].eState = KEY_PRESSING;
				}
				else
				{
					m_tKeyState[i].eState = KEY_DOWN;
				}

				m_tKeyState[i].bPrevPush = true;
			}
			else
			{
				if (m_tKeyState[i].bPrevPush)
				{
					m_tKeyState[i].eState = KEY_UP;
					m_tKeyState[i].bPrevPush = false;
				}
				else
				{
					m_tKeyState[i].eState = KEY_NOTHING;
				}
			}
		}
	}
}
