#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Enemy.h"

#include "Collider.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*> _Dst, list<CObj*> _Src)
{
	RECT	rcRect{};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (!Dst->IsHaveCollision() || !Src->IsHaveCollision())
				return;

			if (IntersectRect(&rcRect, (Dst->Get_Collider()->Get_Rect()), (Src->Get_Collider()->Get_Rect())))
			{
				//어떤거라도 활성화되있지 않으면 충돌되지 않은 상태임
				if (!Dst->Get_Collider()->Get_IsActive() || !Src->Get_Collider()->Get_IsActive() || !Dst->Get_IsActive() || !Src->Get_IsActive())
				{
					Dst->OnCollisionExit(Src);
					Src->OnCollisionExit(Dst);
					Dst->GetCollisionSet()->erase(Src);
					Src->GetCollisionSet()->erase(Dst);
					continue;
				}

				if (Dst->GetCollisionSet()->find(Src) == Dst->GetCollisionSet()->end())
				{
					Dst->OnCollisionEnter(Src);
					Src->OnCollisionEnter(Dst);
					Dst->GetCollisionSet()->insert(Src);
					Src->GetCollisionSet()->insert(Dst);
				}
				else if (Dst->GetCollisionSet()->find(Src) != Dst->GetCollisionSet()->end())
				{
					Dst->OnCollisionStay(Src);
					Src->OnCollisionStay(Dst);
				}
			}
			else
			{
				if (Dst->GetCollisionSet()->find(Src) != Dst->GetCollisionSet()->end())
				{
					Dst->OnCollisionExit(Src);
					Src->OnCollisionExit(Dst);
					Dst->GetCollisionSet()->erase(Src);
					Src->GetCollisionSet()->erase(Dst);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dst, list<CObj*> _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				if (Dst->GetCollisionSet()->find(Src) == Dst->GetCollisionSet()->end())
				{
					Dst->OnCollisionEnter(Src);
					Src->OnCollisionEnter(Dst);
					Dst->GetCollisionSet()->insert(Src);
					Src->GetCollisionSet()->insert(Dst);
				}
				else if (Dst->GetCollisionSet()->find(Src) != Dst->GetCollisionSet()->end())
				{
					Dst->OnCollisionStay(Src);
					Src->OnCollisionStay(Dst);
				}
			}
			else
			{
				if (Dst->GetCollisionSet()->find(Src) != Dst->GetCollisionSet()->end())
				{
					Dst->OnCollisionExit(Src);
					Src->OnCollisionExit(Dst);
					Dst->GetCollisionSet()->erase(Src);
					Src->GetCollisionSet()->erase(Dst);
				}
			}
		}

	}
}

bool CCollisionMgr::Check_Sphere(CObj * pDst, CObj * pSrc)
{
	float	fWidth  = abs(pDst->Get_Collider()->Get_RenderPos().x -  dynamic_cast<CEnemy*>(pSrc)->Get_DetectCollider()->Get_RenderPos().x);
	float	fHeight	= abs(pDst->Get_Collider()->Get_RenderPos().y - dynamic_cast<CEnemy*>(pSrc)->Get_DetectCollider()->Get_RenderPos().y);

	float	fDiagonal = sqrt(fWidth * fWidth + fHeight  * fHeight);

	float	fRadius = (pDst->Get_Collider()->Get_Size().x + dynamic_cast<CEnemy*>(pSrc)->Get_DetectCollider()->Get_Size().x) * 0.5f;

	if(fDiagonal)
		return fRadius >= fDiagonal;

	return false;
}
