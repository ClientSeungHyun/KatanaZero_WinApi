#pragma once
#include "Obj.h"

class CPlayerShadow :
	public CObj
{
public:
	CPlayerShadow();
	virtual ~CPlayerShadow();

public:
	void Set_ShadowTime(DWORD shadowTime) { m_dwShadowTime = shadowTime; }
	void Set_Alpha(int iAlpha) { m_iAlpha = iAlpha; }

public:
	virtual void Initialize(void) override;
	virtual int  Update(void) override;
	virtual void Late_Update(void)override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:
	void Motion_Change();

private:
	CObj* m_pPlayer;

	DWORD    m_dwDestroyTime;
	DWORD    m_dwShadowTime;

	bool	 m_bRender = false;

	BLENDFUNCTION m_BlendFunction;
	int			  m_iAlpha;

	HDC		m_AlphaDC;
};

