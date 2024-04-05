#pragma once

#include "Define.h"
#include "TimeMgr.h"
#include "SceneMgr.h"

class CMyBmp;
class CCamera;
class CRigidBody;
class CCollider;
class CMemento;
class CCaretaker;

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	CRigidBody*			Get_RigidBody() { return m_pRigidBody; }
	CCollider*			Get_Collider() { return m_pCollider; }

	VEC2				Get_Pos() { return m_vPos; }
	virtual void		Set_Pos(float x, float y) { m_vPos.x = x; m_vPos.y = y; }
	void				Set_PosX(float x) { m_vPos.x = x; }
	void				Set_PosY(float y) { m_vPos.y = y; }

	VEC2				Get_CameraPos() { return m_vCameraPos; }

	VEC2				Get_Size() { return m_vSize; }
	void				Set_Size(float x, float y) { m_vSize.x = x; m_vSize.y = y; }
	bool				Get_Dead() { return m_bDead; }
	void				Set_Dead() { m_bDead = true; }

	void				Set_StageCol() 
	{ 
		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BLOOD)
		{
			m_vecStageCol = CSceneMgr::Get_Instance()->Get_MapVec(SC_BOSS1);
		}
		else
		{
			m_vecStageCol = CSceneMgr::Get_Instance()->Get_MapVec(CSceneMgr::Get_Instance()->Get_CurScene());
		}
	}
	void				Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }

	void				Set_ColliderBmp(CMyBmp* bmp) { m_pColliderBmp = bmp; }

	bool				Get_IsActive() { return m_bIsActive; }
	void				Set_IsActive(bool bIsActive) { m_bIsActive = bIsActive; }

	VEC2				Get_LookDir() { return m_vLookDir; }
	void				Set_LookDir(VEC2 vDir) { m_vLookDir = vDir; }

	OBJID				Get_Id() { return m_eId; }
	RENDERID			Get_RenderId() { return m_eRenderId; }

	CMyBmp*				Get_AniBmp() { return m_pAniBmp; }
	void				Set_AniBmp(CMyBmp* bmp) { m_pAniBmp = bmp; }

	CObj*				Get_EffectObj() { return m_pEffect; }
	void				Set_EffectOBj(CObj* pEffect) { m_pEffect = pEffect; }

	FRAME&				Get_Frame() { return m_tFrame; }
	void				Set_Frame(bool bIsLoop = true);

	bool				IsHaveCollision() 
	{
		if (m_pCollider) return true;
		else return false;
	}

	unordered_set<CObj*>* GetCollisionSet() { return &m_pCollisionOthers; }

public:
	virtual	void		Initialize()	PURE;
	virtual	int			Update()		PURE;
	virtual	void		Late_Update()	PURE;
	virtual	void		Render(HDC hDC) PURE;
	virtual	void		Release()		PURE;

	virtual	void		OnCollisionEnter(CObj* _pOther) {};
	virtual	void		OnCollisionStay(CObj* _pOther) {}
	virtual	void		OnCollisionExit(CObj* _pOther) {};

public:
	void				Create_RigidBody();
	void				Create_Collider();

	void				Update_Rect();
	void				Move_Frame();

	CMemento                Save_State();
	void                    Set_MementoState(CMemento memento);

	friend				CCamera;

protected:
	CRigidBody*			m_pRigidBody;
	CCollider*			m_pCollider;
	CCaretaker*			m_pCareTaker;
	CObj*				m_pEffect;

	vector<COLORREF>       m_vecStageCol;

	RECT				m_tRect;
	VEC2				m_vPos;
	VEC2				m_vCameraPos;
	VEC2				m_vSize;
	VEC2                m_vLookDir;     //플레이어가 바라보는 방향 벡터

	vector<VEC2*>		m_vAniSize;

	CMyBmp*				m_pColliderBmp;
	CMyBmp*				m_pAniBmp;
	CMyBmp*				m_pPrevAniBmp;
	bool				m_bDead;
	bool				m_bIsMoveFrame;
	bool				m_bIsActive;

	FRAME				m_tFrame;

	int					m_iFrameX;

	OBJID				m_eId;
	RENDERID			m_eRenderId;
	const TCHAR*		m_pFrameKey = L"";

	unordered_set<CObj*> m_pCollisionOthers;
};

