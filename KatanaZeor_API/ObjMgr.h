#pragma once

#include "Obj.h"
#include "Define.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	CObj*		Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	list<CObj*>* Get_PlayerList() { return &m_ObjList[OBJ_PLAYER]; }
	bool		Get_IsPlayerEmpty() { return m_ObjList[OBJ_PLAYER].empty(); }
	CObj*		Get_Boss() { return m_ObjList[OBJ_BOSS].front(); }
	list<CObj*>* Get_EnemyList() { return &m_ObjList[OBJ_ENEMY]; }
	list<CObj*>* Get_ShadowList() { return &m_ObjList[OBJ_SHADOW]; }
	CObj*		Get_Target(OBJID eID, CObj* pObj);

	CObj*		Get_BackUI() { return m_ObjList[OBJ_UI].back(); }

	list<CObj*> Get_ObjList() { return *m_ObjList; }


public:
	void		Add_Object(OBJID eID, CObj* pObj);
	void		Update();		
	void		Late_Update();
	void		Render(HDC hDC);		
	void		Release();

	void		Delete_ID(OBJID eID);
	void		Delete_All();


private:
	list<CObj*>	m_ObjList[OBJ_END];
	list<CObj*>	m_RenderList[RENDER_END];

public:
	static CObjMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;

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

private:
	static CObjMgr*		m_pInstance;

};

