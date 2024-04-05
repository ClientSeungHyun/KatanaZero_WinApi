#include "BossStage02.h"
#include "Player.h"
#include "Boss.h"
#include "CPlayerText.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "Camera.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CBossStage02::CBossStage02()
{
}

CBossStage02::~CBossStage02()
{
	Release();
}

void CBossStage02::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage2.bmp", L"BossStage02");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/StageBg/BossStage2_slow.bmp", L"BossStage02_slow");

	CPlayer* pPlayer = new CPlayer;
	pPlayer->Set_Pos(250, 540);
	pPlayer->Initialize();

	CBoss* pBoss = new CBoss;
	CObjMgr::Get_Instance()->Add_Object(OBJ_BOSS, pBoss);
	pBoss->Initialize();
	pBoss->Set_Pos(1044, 530);

	//카메라 설정
	CCamera::Get_Instance()->SetLookAtPos(672.f, 392.f);
	CCamera::Get_Instance()->SetTarget(nullptr);
	
	if (!m_pText)
	{
		m_pText = CAbstractFactory<CPlayerText>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pText);
	}

	CSceneMgr::Get_Instance()->Set_SceneState(SC_READY_COMPLETE);
	CTimeMgr::Get_Instance()->Set_WorldTime(0.f);
}

int CBossStage02::Update()
{

	CObjMgr::Get_Instance()->Update();

	if (CObjMgr::Get_Instance()->Get_Player()->Get_Dead() && !g_bIsTimeBack)
	{
		m_pText->Set_IsActive(true);
	}
	else
	{
		m_pText->Set_IsActive(false);
	}

	return 0;
}

void CBossStage02::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CBossStage02::Render(HDC hDC)
{
	VEC2 vWindowPos = { 0,0 };
	VEC2 vRenderPos = CCamera::Get_Instance()->GetRenderPos(vWindowPos.x, vWindowPos.y);

	if (!CSceneMgr::Get_Instance()->Get_IsSlow())
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"BossStage02");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 3200, 2000, hBackGroundDc, 0, 0, SRCCOPY);
	}
	else
	{
		HDC hBackGroundDc = CBmpMgr::Get_Instance()->Find_DC(L"BossStage02_slow");
		BitBlt(hDC, (int)vRenderPos.x, (int)vRenderPos.y, 3200, 2000, hBackGroundDc, 0, 0, SRCCOPY);
	}

	CObjMgr::Get_Instance()->Render(hDC);
}

void CBossStage02::Release()
{
	CObjMgr::Get_Instance()->Destroy();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
}
