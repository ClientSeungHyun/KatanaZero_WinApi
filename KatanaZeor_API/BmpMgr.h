#pragma once

#include "MyBmp.h"

class CBmpMgr
{
private:
	CBmpMgr();
	CBmpMgr(const CBmpMgr& _rhs);
	~CBmpMgr();

public:
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey, VEC2 vImageSize, VEC2 vColliderSize, VEC2 vColliderOffset, int iFrameCount, DWORD dwSpeed, DWORD dwTime);
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);


	void		Delete_Bmp(const TCHAR* pImgKey);

	CMyBmp*		Find_Image(const TCHAR* pImgKey);
	HDC			Find_DC(const TCHAR* pImgKey);
	void		Release();

public:
	static CBmpMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CBmpMgr;

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
	static CBmpMgr*		m_pInstance;
	map<const TCHAR*, CMyBmp*>		m_mapBit;
};

