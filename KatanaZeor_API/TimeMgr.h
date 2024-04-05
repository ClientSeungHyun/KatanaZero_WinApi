#pragma once
#include "Define.h"

class CTimeMgr
{
private:
	CTimeMgr();
	~CTimeMgr();

public:
	float			Get_DeltaTime() { return m_fDeltaTime; }

	float			Get_TimeScale() { return m_fTimeScale; }
	void			Set_TimeScale(float fTimeScale) { m_fTimeScale = fTimeScale; }
	
	float			Get_WorldTime() { return m_fWorldTime; }
	void			Set_WorldTime(float fTime) { m_fWorldTime = fTime; }

	int				Get_TimeGage() { return m_iTimeGage; }
	void			Set_TimeGage(int iTime) { m_iTimeGage = iTime; }

public:
	void			Initialize();
	void			Update();

private:
	LARGE_INTEGER	m_llCurCount;   
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	float			m_fDeltaTime;
	float			m_fAcc;			//누적
	float			m_fWorldTime;

	float			m_fTimeScale;

	int				m_iCallCount;	//초당 호출 횟수
	int				m_iFPS;

	int				m_iTimeGage;

public:
	static CTimeMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTimeMgr;

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
	static CTimeMgr* m_pInstance;
};

