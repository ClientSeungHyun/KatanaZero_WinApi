#include "stdafx.h"
#include "MyBmp.h"

#include "Camera.h"
#include "TimeMgr.h"

CMyBmp::CMyBmp():m_iStartFrame(0), m_bIsLoop(true), m_bIsPlayDone(false)
{
}


CMyBmp::~CMyBmp()
{
}

void CMyBmp::Load_Bmp(const TCHAR * pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	// 화면DC와 호환되는 DC를 할당
	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL, // 프로그램 인스턴스 핸들
							pFilePath,  // 이미지 파일의 경로
							IMAGE_BITMAP,  // 어떤 타입을 읽어올지 결정
							0,		// 이미지의 가로, 세로 크기
							0, 
							LR_LOADFROMFILE | LR_CREATEDIBSECTION);
							//LR_LOADFROMFILE : 파일로부터 이미지를 읽어들인다는 옵션
							// LR_CREATEDIBSECTION : 읽어온 파일을 DIB 형태로 변환
	if (m_hBitmap == 0)
		::MessageBox(g_hWnd, pFilePath, L"Image load failed", NULL);
	// SelectObject : 준비한 DC의 GDI 오브젝트를 불러 비트맵을 선택하게 만드는 함수
	// 현재 이 코드는 GDI 오브젝트를 선택하기 전 기존에 가지고 있던 오브젝트를 반환하게 만든다.
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}
void CMyBmp::Set_AniInfo(VEC2 vImageSize, VEC2 vColliderSize, VEC2 vColliderOffset, int iEndFrame, DWORD dwSpeed, DWORD dwTime)
{
	m_vImageSize = vImageSize;
	m_vColliderSize = vColliderSize;
	m_vColliderOffset = vColliderOffset;
	m_iEndFrame = iEndFrame;
	m_dwSpeed = dwSpeed;
	m_dwTime = dwTime;

	m_iRealEndFrame = iEndFrame;
}

void CMyBmp::Reset_AniInfo()
{
	m_iStartFrame = 0;
	m_iEndFrame = m_iRealEndFrame;
	m_dwTime = GetTickCount();
	m_bIsLoop = true;
	m_bIsPlayDone = false;
}


COLORREF CMyBmp::Get_Pixel(int x, int y)
{
	//VEC2 newPos = CCamera::Get_Instance()->GetRenderPos(
	return ::GetPixel(m_hMemDC, x, y);


}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
