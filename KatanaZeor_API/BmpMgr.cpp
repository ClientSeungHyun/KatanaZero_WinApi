#include "stdafx.h"
#include "BmpMgr.h"

CBmpMgr*		CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::CBmpMgr(const CBmpMgr& _rhs)
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey, VEC2 vImageSize, VEC2 vColliderSize, VEC2 vColliderOffset, int iFrameCount, DWORD dwSpeed, DWORD dwTime)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp*		pMyBmp = new CMyBmp;
		pMyBmp->Load_Bmp(pFilePath);
		pMyBmp->Set_AniInfo(vImageSize, vColliderSize, vColliderOffset, iFrameCount, dwSpeed, dwTime);

		m_mapBit.insert({ pImgKey, pMyBmp });
	}

}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp* pMyBmp = new CMyBmp;
		pMyBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pMyBmp });
	}

}

void CBmpMgr::Delete_Bmp(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
	{
		return;
	}

	m_mapBit.erase(iter);
}

CMyBmp* CBmpMgr::Find_Image(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;


	return iter->second;
}

HDC CBmpMgr::Find_DC(const TCHAR * pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;
	

	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), DeleteMap());
	m_mapBit.clear();
}
