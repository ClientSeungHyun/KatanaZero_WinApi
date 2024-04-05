#pragma once

#include "Define.h"

class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC			Get_MemDC() { return m_hMemDC; }
	HBITMAP		Get_Bmp() { return m_hBitmap; }

	void		Set_ImageSize(VEC2 vSize) { m_vImageSize = vSize; }
	VEC2		Get_ImageSize() { return m_vImageSize; }

	void		Set_ColliderSize(VEC2 vSize) { m_vColliderSize = vSize; }
	VEC2		Get_ColliderSize() { return m_vColliderSize; }

	void		Set_ColliderOffset(VEC2 vOffset) { m_vColliderOffset = vOffset; }
	VEC2		Get_ColliderOffset() { return m_vColliderOffset; }

	int			Get_EndFrame(){ return m_iEndFrame; }
	void		Set_EndFrame(int iFrame) { m_iEndFrame = iFrame; }

	int			Get_RealEndFrame() { return m_iRealEndFrame; }

	int			Get_StartFrame() { return m_iStartFrame; }
	void		Set_StartFrame(int iFrame) { m_iStartFrame = iFrame; }

	DWORD		Get_Time() { return m_dwTime; }
	void		Set_Time(DWORD dwTime) { m_dwTime = dwTime; }

	DWORD		Get_Speed() { return m_dwSpeed; }
	void		Set_Speed(DWORD dwSpeed) { m_dwSpeed = dwSpeed; }

	bool		Get_IsLoop() { return m_bIsLoop; }
	void		Set_IsLoop(bool bIsLoop) { m_bIsLoop = bIsLoop; }

	bool		Get_IsPlayDone() { return m_bIsPlayDone; }
	void		Set_IsPlayDone(bool bIsPlayDone) { m_bIsPlayDone = bIsPlayDone; }

	const TCHAR* Get_FilePath() { return m_pFilePath; }

public:
	void		Load_Bmp(const TCHAR* pFilePath);
	void		Set_AniInfo(VEC2 vSize, VEC2 vColliderSize, VEC2 vColliderOffset, int iEndFrame, DWORD dwSpeed, DWORD dwTime);
	void		Reset_AniInfo();


	COLORREF	Get_Pixel(int x, int y);
	void		Release();

private:
	HDC			m_hMemDC;

	VEC2		m_vImageSize;
	VEC2		m_vColliderSize;
	VEC2		m_vColliderOffset;
	int			m_iStartFrame = 0;
	int			m_iEndFrame;
	int			m_iRealEndFrame;

	bool		m_bIsLoop;		//반복 여부 기본적으로 true
	bool		m_bIsPlayDone;
	
	const TCHAR*		m_pFilePath;

	DWORD		m_dwTime;
	DWORD		m_dwSpeed;

	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

