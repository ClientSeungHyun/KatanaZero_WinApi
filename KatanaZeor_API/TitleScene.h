#pragma once
#include "Scene.h"
class CTitleScene : public CScene
{
public:
	CTitleScene();
	virtual ~CTitleScene();

public:
	void					Set_StartClick(bool bClick) { m_bIsStartClicked = bClick; }
	void					Set_BloodStartClick(bool bClick) { m_bIsBloodStartClicked = bClick; }

public:
	virtual void			Initialize()	override;
	virtual int				Update()		override;
	virtual void			Late_Update()	override;
	virtual void			Render(HDC hDC) override;
	virtual void			Release()		 override;

private:
	DWORD					m_dwBackUpTime;
	int						m_dwZeroDelayTime;

	bool					m_bIsTitleLoad;
	bool					m_bIsStartClicked;
	bool					m_bIsBloodStartClicked;

};

