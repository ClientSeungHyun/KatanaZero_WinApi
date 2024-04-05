#include "Obj.h"
#include "MyBmp.h"
#include "Camera.h"

#include "RigidBody.h"
#include "BmpMgr.h"
#include "Collider.h"

#include "Caretaker.h"
#include "Memento.h"

CObj::CObj() : m_bDead(false), m_pRigidBody(nullptr), m_pAniBmp(nullptr), m_pCollider(nullptr), m_bIsActive(true),
m_pCareTaker(nullptr), m_pColliderBmp(nullptr), m_pEffect(nullptr), m_eId(OBJ_END), m_bIsMoveFrame(true), m_pPrevAniBmp(nullptr),
m_eRenderId(RENDER_END)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::~CObj()
{
}


void CObj::Set_Frame(bool bIsLoop)
{
	if (bIsLoop)
	{
		if (m_pPrevAniBmp)
		{
			if (m_pAniBmp->Get_MemDC() == m_pPrevAniBmp->Get_MemDC())
				return;
		}
	}

	m_tFrame.iFrameStart = 0;

	if(m_vLookDir.x == -1 && m_eId != OBJ_BOSS)
		m_tFrame.iFrameStart = m_pAniBmp->Get_RealEndFrame();


	m_tFrame.isLoop = bIsLoop;

	m_tFrame.iFrameEnd = m_pAniBmp->Get_EndFrame();
	m_tFrame.iFrameRealEnd = m_pAniBmp->Get_RealEndFrame();
	m_tFrame.dwSpeed = m_pAniBmp->Get_Speed();
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.isPlayDone = false;
}

void		CObj::Create_RigidBody()
{
	if (!m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody();
		m_pRigidBody->m_pOwner = this;
		m_pRigidBody->Initialize();
	}
}

void		CObj::Create_Collider()
{
	if (!m_pCollider)
	{
		m_pCollider = new CCollider();
		m_pCollider->m_pOwner = this;
	}
}

void		CObj::Update_Rect()
{
	m_tRect.left = long(m_vPos.x - (m_vSize.x * 0.5f));
	m_tRect.top = long(m_vPos.y - (m_vSize.y * 0.5f));
	m_tRect.right = long(m_vPos.x + (m_vSize.x * 0.5f));
	m_tRect.bottom = long(m_vPos.y + (m_vSize.y * 0.5f));
}

void CObj::Move_Frame()
{//오른 방향
	DWORD dwSpeed; 

	if (CSceneMgr::Get_Instance()->Get_IsSlow())
	{
		dwSpeed = m_tFrame.dwSpeed * 3;
	}
	else
	{
		dwSpeed = m_tFrame.dwSpeed;
	}

	if (m_pAniBmp == CBmpMgr::Get_Instance()->Find_Image(L"Fan"))
	{
		if (CSceneMgr::Get_Instance()->Get_IsSlow())
		{
			if (m_tFrame.dwTime + 60 < GetTickCount())
			{
				m_tFrame.iFrameStart = m_tFrame.iFrameStart + 1;
				m_tFrame.dwTime = GetTickCount();
			}
			if (m_tFrame.iFrameStart > m_tFrame.iFrameRealEnd)
			{
				m_tFrame.iFrameStart = 0;
			}

			return;
		}

		for (int i = 1; i <= 5; ++i)
		{
			m_tFrame.iFrameStart = m_tFrame.iFrameStart + 1;
		}
		if (m_tFrame.iFrameStart > m_tFrame.iFrameRealEnd)
		{
			m_tFrame.iFrameStart = 0;
		}

		return;
	}

		if (m_vLookDir.x == 1)
		{
			if (m_tFrame.dwTime + dwSpeed < GetTickCount())
			{
				m_tFrame.iFrameStart = m_tFrame.iFrameStart + 1;
				m_tFrame.dwTime = GetTickCount();

				if (m_tFrame.iFrameStart > m_tFrame.iFrameRealEnd )
				{
					m_tFrame.isPlayDone = true;	//한 번 재생 끝
				}

				//isLoop가 true면 다시 처음으로
				if (m_tFrame.isLoop && m_tFrame.iFrameStart > m_tFrame.iFrameRealEnd && !m_bDead)
				{
					m_tFrame.iFrameStart = 0;
					m_tFrame.isPlayDone = false;	
				}
				else if (!m_tFrame.isLoop && m_tFrame.isPlayDone || (m_bDead && m_tFrame.isPlayDone))
				{
					m_tFrame.iFrameStart = m_tFrame.iFrameRealEnd;

					if (m_bDead)
						m_bIsMoveFrame = false;
				}
			}
		}
		else if (m_vLookDir.x == -1)
		{
			if (m_tFrame.dwTime + dwSpeed < GetTickCount())
			{
				m_tFrame.iFrameEnd = m_tFrame.iFrameEnd - 1;
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
				m_tFrame.dwTime = GetTickCount();

				if (m_tFrame.iFrameStart <= 0 )
				{
					m_tFrame.isPlayDone = true;	//한 번 재생 끝
				}

				if (m_tFrame.isLoop && m_tFrame.isPlayDone && !m_bDead)
				{
					m_tFrame.iFrameEnd = m_tFrame.iFrameRealEnd;
					m_tFrame.isPlayDone = false;
				}
				else if (!m_tFrame.isLoop && m_tFrame.isPlayDone || (m_bDead && m_tFrame.isPlayDone))
				{
					m_tFrame.iFrameStart = 0;
					if (m_bDead)
						m_bIsMoveFrame = false;
				}
			}
		}
	}

CMemento		CObj::Save_State()
{
	CMemento newMemento;

	newMemento.Save_State(m_vPos, m_vLookDir, m_pAniBmp, m_bDead, m_bIsActive, m_tFrame);

	return newMemento;
}

void		CObj::Set_MementoState(CMemento memento)
{
		m_vPos = memento.Get_Pos();
		m_vLookDir = memento.Get_LookDir();
		m_pAniBmp = memento.Get_Bmp();
		m_bIsActive = memento.Get_bIsActive();
		m_bDead = memento.Get_IsDead();
		m_tFrame = memento.Get_Frame();
}
