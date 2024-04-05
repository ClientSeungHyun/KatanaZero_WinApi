#pragma once

#include "Define.h"

#include "LoadingScene.h"
#include "TitleScene.h"
#include "StageScene01.h"
#include "StageScene02.h"
#include "StageScene03.h"
#include "BossStage01.h"
#include "BossStage02.h"
#include "EndingScene.h"
#include "BloodScene.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	static CSceneMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

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

public:
	void			Scene_Change(SCENEID eID);
	void			Update();
	void			Late_Update();
	void			Render(HDC hDC);
	void			Release();

	SCENEID			Get_CurScene() { return m_eCurScene; }
	SCENEID			Get_PrevScene() { return m_ePreScene; }

	CScene*			Get_NowScene() { return m_pScene; }

	vector<COLORREF>& Get_MapVec(SCENEID eID) { return m_vecStageCol[eID]; };

	void			Set_MapHeight(SCENEID eID, int iHeight) { m_arrStageHeight[eID] = iHeight; }
	int&			Get_MapHeight(SCENEID eID) 
	{ 
		if (eID == SC_BLOOD) 
			eID = SC_BOSS1; 
		return m_arrStageHeight[eID]; 
	}

	const TCHAR*	Get_CurSceneColliderKey() { return m_pScene->Get_FrameKey(); }

	SCENESTATE		Get_SceneState() { return m_eSceneState; }
	void			Set_SceneState(SCENESTATE eState) { m_eSceneState = eState; }

	bool			Get_IsSlow() { return m_bIsSlow; }
	void			Set_IsSlow(bool bSlow) { m_bIsSlow = bSlow; }

private:
	static CSceneMgr*		m_pInstance;
	CScene*					m_pScene;

	SCENEID					m_ePreScene;
	SCENEID					m_eCurScene;

	vector<COLORREF>           m_vecStageCol[SC_END];
	int							m_arrStageHeight[SC_END];

	SCENESTATE	m_eSceneState;

	bool		m_bIsSlow;
};

