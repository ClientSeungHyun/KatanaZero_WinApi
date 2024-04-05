#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "Camera.h"

CObjMgr*		CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}


CObj * CObjMgr::Get_Target(OBJID eID, CObj * pObj)
{
	if(m_ObjList[eID].empty())
		return nullptr;

	CObj*	pTarget = nullptr;

	float	fDistance(0.f);

	for (auto& iter : m_ObjList[eID])
	{
		if(iter->Get_Dead())
			continue;

		float	fWidth	  = pObj->Get_Pos().x - iter->Get_Pos().x;
		float	fHeight	  = pObj->Get_Pos().y - iter->Get_Pos().y;
		float	fDiagonal = sqrt(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			pTarget = iter;
			fDistance = fDiagonal;
		}
	}

	return pTarget;
}

void CObjMgr::Add_Object(OBJID eID, CObj * pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	//CCamera::Get_Instance()->Update();

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int		iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& Obj : m_ObjList[i])
		{
			Obj->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID eID = Obj->Get_RenderId();
			m_RenderList[eID].push_back(Obj);
		}
	}

	if (!g_bIsTimeBack)
	{
		if (!m_ObjList[OBJ_PLAYER].empty() && dynamic_cast<CPlayer*>(m_ObjList[OBJ_PLAYER].front())->Get_State() != HURT)
		{
			CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ITEM]);
			CCollisionMgr::Collision_Rect(m_ObjList[OBJ_TRAP], m_ObjList[OBJ_PLAYER]);

			if (dynamic_cast<CPlayer*>(m_ObjList[OBJ_PLAYER].front())->Get_State() != ROLL && dynamic_cast<CPlayer*>(m_ObjList[OBJ_PLAYER].front())->Get_State() != FLIP)
			{
				CCollisionMgr::Collision_Sphere(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ENEMY]);
				CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_BULLET]);
				CCollisionMgr::Collision_Rect(m_ObjList[OBJ_ENEMY], m_ObjList[OBJ_PLAYER]);
				CCollisionMgr::Collision_Rect(m_ObjList[OBJ_EFFECT], m_ObjList[OBJ_PLAYER]);
			}

		}
		CCollisionMgr::Collision_Rect(m_ObjList[OBJ_EFFECT], m_ObjList[OBJ_BOSS]);
		CCollisionMgr::Collision_Rect(m_ObjList[OBJ_EFFECT], m_ObjList[OBJ_BULLET]);
		CCollisionMgr::Collision_Rect(m_ObjList[OBJ_EFFECT], m_ObjList[OBJ_ENEMY]);
		CCollisionMgr::Collision_Rect(m_ObjList[OBJ_ITEM], m_ObjList[OBJ_ENEMY]);
		CCollisionMgr::Collision_Rect(m_ObjList[OBJ_ENEMY], m_ObjList[OBJ_BULLET]);
	}
}

void CObjMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->Get_Pos().y < pSrc->Get_Pos().y;
			});

		for (auto& iter : m_RenderList[i])
		{
			if (CSceneMgr::Get_Instance()->Get_CurScene() != SC_TITLE)
			{
				if (i == GAMEOBJECT || i == ENEMY)
				{
					iter->Render(hDC);
				}
				else
				{
					if (CSceneMgr::Get_Instance()->Get_SceneState() <= SC_READY_COMPLETE || CSceneMgr::Get_Instance()->Get_CurScene() == SC_TITLE)
					{
						continue;
					}

					iter->Render(hDC);
				}
			}
			else
			{
				iter->Render(hDC);
			}

		}

		m_RenderList[i].clear();
	}

}

void CObjMgr::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

void CObjMgr::Delete_All()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_PLAYER)
			continue;

		for (auto& pObj : m_ObjList[i])
			Safe_Delete(pObj);

		m_ObjList[i].clear();
	}
}

