#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr() :m_llCurCount{}, m_llPrevCount{}, m_fDeltaTime(0.f)
, m_llFrequency{}, m_iCallCount(0), m_fAcc(0.f), m_fTimeScale(1.0f), m_fWorldTime(0.f), m_iFPS(0), m_iTimeGage(227)
{
}

CTimeMgr::~CTimeMgr()
{
}

void		CTimeMgr::Initialize()
{
	QueryPerformanceCounter(&m_llPrevCount);		//ī��Ʈ �� �޾ƿ���
	QueryPerformanceFrequency(&m_llFrequency);		//�ʴ� ī��Ʈ�� �󸶳� ���� �߻��ϴ��� ���ϱ� 
	m_fTimeScale = 1.0f;
}

void		CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// �� �����Ӹ��� �󸶳� count�� ���̰� ������ �˻�
	m_fDeltaTime = (float)((m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart) ;
	m_llPrevCount = m_llCurCount;

	++m_iCallCount;
	m_fAcc += (m_fDeltaTime );		//�帥 �ð�
	m_fWorldTime += (m_fDeltaTime);

	if (m_fAcc >= 1.f)
	{
		m_iFPS = m_iCallCount;
		m_fAcc = 0;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf(szBuffer, 255,L"FPS : %d, DT : %f, WorldTime : %f", m_iFPS, m_fDeltaTime, m_fWorldTime);
		SetWindowText(g_hWnd, szBuffer);
	}
}
