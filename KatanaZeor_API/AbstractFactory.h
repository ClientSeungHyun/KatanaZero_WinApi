#pragma once

#include "Obj.h"
#include "PlayerShadow.h"
#include "Enemy.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj*		Create()
	{
		CObj*		pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj*		Create(float _fX, float _fY)
	{
		CObj*		pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		return pObj;
	}


	static CObj* Create(float _fX, float _fY, float _fCX, float _fCY)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Size(_fCX, _fCY);

		return pObj;
	}

	static CObj* Create_Monseter(float _fX, float _fY, bool bIsPatrol)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		dynamic_cast<CEnemy*>(pObj)->Set_StartPos(_fX, _fY);
		dynamic_cast<CEnemy*>(pObj)->Set_IsPatrol(bIsPatrol);

		return pObj;
	}

	static CObj* Create_Shadow(int iAlpha, DWORD dwShadowTime)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		dynamic_cast<CPlayerShadow*>(pObj)->Set_Alpha(iAlpha);
		dynamic_cast<CPlayerShadow*>(pObj)->Set_ShadowTime(dwShadowTime);

		return pObj;
	}

public:
	CAbstractFactory(){}
	~CAbstractFactory(){}
};

