#pragma once
#include "Define.h"

class CObj;
class CUI;

class CCamera 
{
public:CCamera();
    virtual ~CCamera();

public:
	VEC2 GetLookAtPos() { return m_tLookAtPos; }
    void SetLookAtPos(VEC2 _tLook) { m_tLookAtPos = _tLook; }
    void SetLookAtPos(float _x, float _y){ m_tLookAtPos.x = _x; m_tLookAtPos.y = _y;}

	//타겟 설정
    void SetTarget(CObj* _pTarget) { m_pTarget = _pTarget; }

	void SetMaxPos(VEC2 _vPos) { m_vMaxPos = _vPos; }
	void SetMinPos(VEC2 _vPos) { m_vMinPos = _vPos; }

	bool Get_IsShaking() { return m_bIsCameraShake; }

public:
	//렌더될 위치값 계산
	VEC2 GetRenderPos(VEC2 _vObjPos);
	VEC2 GetRenderPos(float _x, float _y);

	VEC2 GetWindowPos(VEC2 _vObjPos);
	VEC2 GetWindowPos(float _x, float _y);

	void StartShakeCamera();

	void ShakeCamera();

public:
    void Initialize();
    int Update();
    void Render(HDC hDC);

public:
    void CalculateDiff();

private:
    VEC2    m_tLookAtPos;
	VEC2	m_vShakePos;
	VEC2	m_vOriginPos;
    CObj*   m_pTarget;

	VEC2	m_tDiff;

	VEC2	m_vMaxPos;
	VEC2	m_vMinPos;

	int		m_iCameraShake;
	bool	m_bIsCameraShake;
	DWORD	m_dwShakeTime;

	FRAME	m_tNoiseFrame;

	int		m_iSceneChange;
	int		m_iLastAlpha;

public:
	static CCamera* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCamera;
			m_pInstance->Initialize();
		}

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
	static CCamera* m_pInstance;
};

