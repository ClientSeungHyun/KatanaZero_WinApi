#include "stdafx.h"
#include "SceneMgr.h"


CSceneMgr*		CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_pScene(nullptr), m_ePreScene(SC_END), m_eCurScene(SC_TITLE), m_eSceneState(SC_NOTHING), m_bIsSlow(false)
{

}


CSceneMgr::~CSceneMgr()
{
	Release();
}


void CSceneMgr::Scene_Change(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		if(eID != SC_TITLE && eID != SC_ENDING)
			m_eSceneState = SC_READY;

		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOADING:
			m_pScene = new CLoadingScene;
			break;

		case SC_TITLE:
			m_pScene = new CTitleScene;
			break;

		case SC_STAGE1:
			m_pScene = new CStageScene01;
			break;

		case SC_STAGE2:
			m_pScene = new CStageScene02;
			break;

		case SC_STAGE3:
			m_pScene = new CStageScene03;
			break;

		case SC_BOSS1:
			m_pScene = new CBossStage01;
			break;
			
		case SC_BOSS2:
			m_pScene = new CBossStage02;
			break;

		case SC_ENDING:
			m_pScene = new CEndingScene;
			break;

		case SC_BLOOD:
			m_pScene = new CBloodScene;;
			break;
		}
		
		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}


void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
