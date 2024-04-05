#pragma once
#include "Obj.h"
class CMouse : public CObj
{
public:
	CMouse();
	virtual ~CMouse();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	static CMouse* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CMouse;

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
	static CMouse* m_pInstance;
};

