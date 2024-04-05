#include "Player.h"
#include "PlayerFSM.h"
#include "Boss.h"
#include "Enemy.h"

#include "PlayerIdle.h"
#include "PlayerRun.h"
#include "PlayerJump.h"
#include "CPlayerFall.h"
#include "PlayerAttack.h"
#include "PlayerRoll.h"
#include "CPlayerWallGrab.h"
#include "PlayerFlip.h"
#include "PlayerHurt.h"

#include "BaseUI.h"
#include "CInvenUI.h"
#include "TimerUI.h"
#include "CBatterUI.h"

#include "ItemKnife.h"
#include "BaseBullet.h"
#include "CLaserBullet.h"
#include "Pomp.h"

#include "BmpMgr.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

#include "TimeMgr.h"

#include "RigidBody.h"
#include "Collider.h"

#include "Caretaker.h"

#include "StageScene01.h"

#include "AbstractFactory.h"
#include "HitEffect.h"

#include "BomoBullet.h"

CPlayer::CPlayer() :m_bIsGround(false), m_bIsFall(false), m_bIsWallGrab(false), m_iHp(1), m_pItem(nullptr), m_iBulletTimeGage(9)
, m_pGroundEffect(nullptr), m_bIsNextStage(false), m_fLaserDisappear(0.f), m_bIsHitLaser(false), m_bIsDownJump(false), m_eState(IDLE),
m_bIsDance(false), m_dwDanceTime(0), m_bIsSpecialAttack(false), m_dwBulletTimeLimit(0), m_pPlayerFSM(nullptr)
{
	m_eId = OBJ_PLAYER;
	m_eRenderId = GAMEOBJECT;
}

CPlayer::~CPlayer()
{
	Release();
}

void		CPlayer::Initialize()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, this);
	Create_Collider();
	Create_RigidBody();

	m_pCareTaker = new CCaretaker;
	Set_StageCol();

#pragma region 이미지 불러오기
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_idle.bmp", L"Player_Idle", VEC2(70, 70), VEC2(30, 70), VEC2(0, 0), 10, 100, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_idle_to_run.bmp", L"Player_idle_to_run", VEC2(84, 64), VEC2(30, 64), VEC2(0, 0), 3, 80, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_run.bmp", L"Player_run", VEC2(88, 64), VEC2(40, 64), VEC2(0, 0), 9, 60, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_run_to_idle.bmp", L"Player_run_to_Idle", VEC2(80, 70), VEC2(30, 70), VEC2(0, 0), 4, 80, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_jump.bmp", L"Player_jump", VEC2(60, 84), VEC2(30, 73), VEC2(10, 0), 3, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_grab_wall.bmp", L"Player_grab_wall", VEC2(78, 100), VEC2(20, 70), VEC2(-15, 0), 3, 200, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_attack.bmp", L"Player_attack", VEC2(120, 82), VEC2(30, 75), VEC2(10, 0), 3, 50, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_roll.bmp", L"Player_roll", VEC2(94, 64), VEC2(30, 64), VEC2(10, 0), 6, 60, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_fall.bmp", L"Player_fall", VEC2(82, 96), VEC2(30, 75), VEC2(0, 0), 3, 100, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_flip.bmp", L"Player_flip", VEC2(96, 88), VEC2(25, 70), VEC2(25, 0), 10, 30, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_slash_basic.bmp", L"Player_slash", VEC2(210, 60), VEC2(90, 80), VEC2(30, 0), 5, 40, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_hurtfly.bmp", L"Player_hurtfly", VEC2(100, 66), VEC2(100, 66), VEC2(30, 0), 3, 40, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_song.bmp", L"Player_song", VEC2(72, 78), VEC2(30, 70), VEC2(30, 0), 30, 70, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_dance.bmp", L"Player_dance", VEC2(63, 80), VEC2(30, 70), VEC2(30, 0), 11, 60, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_walk.bmp", L"Player_walk", VEC2(70, 74), VEC2(30, 70), VEC2(30, 0), 7, 130, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_hurtground.bmp", L"Player_hurtground", VEC2(114, 46), VEC2(114, 46), VEC2(30, 0), 5, 70, GetTickCount());

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Effect/player_jumpdust.bmp", L"Player_jumpdust", VEC2(42, 80), VEC2(30, 30), VEC2(0, 0), 4, 30, GetTickCount());
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player_hurtground.bmp", L"Player_hurtground", VEC2(114, 46), VEC2(0, 0), VEC2(0, 0), 5, 100, GetTickCount());
#pragma endregion 이미지 불러오기

	if (CSceneMgr::Get_Instance()->Get_CurScene() != SC_ENDING)
	{
		CUI* pUI = new CUI;
		pUI->Initialize();
		pUI->Set_Size(1344, 49);
		pUI->Set_Pos(0, 0);
		pUI->Set_FrameKey(L"HUD");
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pUI);
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CInvenUI>::Create());
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CTimerUI>::Create());
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, CAbstractFactory<CBatterUI>::Create());

		for (int i = 0; i < 8; ++i)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_SHADOW, CAbstractFactory<CPlayerShadow>::Create_Shadow(100 - (i * 10), 100 + (i * 20)));
		}
	}

	CTimeMgr::Get_Instance()->Set_WorldTime(0.f);
	Init_Player();
}

int			CPlayer::Update()
{
	if (CSceneMgr::Get_Instance()->Get_SceneState() == SC_READY)
		return 0;

	if (CSceneMgr::Get_Instance()->Get_SceneState() == SC_READY_COMPLETE)
	{
		if (CKeyMgr::Get_Instance()->Get_KeyState(VK_LBUTTON) == KEY_DOWN && m_pAniBmp != CBmpMgr::Get_Instance()->Find_Image(L"Player_song"))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"song_play.wav", SOUND_PLAYSONG, g_fEffectSound);
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_song");
			Set_Frame(false);
			CTimeMgr::Get_Instance()->Set_WorldTime(0.f);

			return 0;
		}

		if (m_pAniBmp == CBmpMgr::Get_Instance()->Find_Image(L"Player_song") && m_tFrame.isPlayDone)
		{
			Select_BGSong();
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle");
			Set_Frame();
			CSceneMgr::Get_Instance()->Set_SceneState(SC_PLAYING);
		}

		return 0;
	}

	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_ENDING)
	{
		if (!m_bIsDance)
		{
			m_bIsDance = true;
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_dance");
			Set_Frame();
		}
		__super::Move_Frame();
		return 0;
	}

#pragma region 스페셜어택
	if (CKeyMgr::Get_Instance()->Get_KeyState('F') == KEY_PRESSING && m_iBulletTimeGage >= 5)
	{
		if (!m_bIsSpecialAttack)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW);
			CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Enter.wav", SOUND_SLOW, g_fEffectSound);
			CSoundMgr::Get_Instance()->SetPause(SOUND_BGM, true);

			CSoundMgr::Get_Instance()->PlaySound(L"SlowWalk.mp3", SOUND_SATTACK, g_fEffectSound);

			CSceneMgr::Get_Instance()->Set_IsSlow(true);

			m_pRigidBody->CalcFriction();
			m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_walk");
			m_bIsSpecialAttack = true;
			Set_Frame();
		}

		m_pRigidBody->Set_VelocityX(70.f * m_vLookDir.x);
		return 0;
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState('F') == KEY_UP && m_bIsSpecialAttack)
	{
		if (!m_vecSpecialAttack.empty())
		{
			for (auto iter = m_vecSpecialAttack.begin(); iter != m_vecSpecialAttack.end(); ++iter)
			{
				if (fabsf((*iter)->Get_Pos().y - m_vPos.y) <= 100.f)
					dynamic_cast<CEnemy*>(*iter)->EnemyDie();
			}
			//hit 이펙트
			CObj* pHitEffect = new CHitEffect;
			pHitEffect->Set_Pos(CCamera::Get_Instance()->GetLookAtPos().x, m_vPos.y);
			pHitEffect->Initialize();
			dynamic_cast<CHitEffect*>(pHitEffect)->Set_Angle(0.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ_BG, pHitEffect);

			m_vecSpecialAttack.clear();
		}

		CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW);
		CSoundMgr::Get_Instance()->StopSound(SOUND_SATTACK);
		CSoundMgr::Get_Instance()->SetPause(SOUND_BGM, false);

		CSceneMgr::Get_Instance()->Set_IsSlow(false);

		m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle");
		m_bIsSpecialAttack = false;
		Set_Frame();

		m_iBulletTimeGage = 0.f;
		m_dwBulletTimeLimit = GetTickCount();
	}
#pragma endregion 스페셜어택

	if (!g_bIsTimeBack)
	{
		BulletTime();
		ThrowItem();
		m_pPlayerFSM->Update();
	}
	else
	{
		//과거 회귀
		for (int i = 0; i < 20; ++i)
		{
			if (m_pCareTaker->Get_ListSize() > 0)
			{
				m_fLaserDisappear = 0.f;
				m_bIsHitLaser = false;
				Set_MementoState(m_pCareTaker->TimeBakc());
			}
			else
			{
				TimeBakcDone();
			}
		}
	}

	Laser_Die();

	if (m_pEffect && m_eState != ATTACK)
		m_pEffect->Set_Pos(m_vPos.x, m_vPos.y);

	return 0;
}

void		CPlayer::Late_Update()
{
	__super::Move_Frame();

	CSoundMgr::Get_Instance()->SetPlayeSpeed();
	m_pRigidBody->Late_Update();
	m_pCollider->Late_Update();

	m_pCollider->Set_IsActive(true);

	if (!g_bIsTimeBack && CSceneMgr::Get_Instance()->Get_SceneState() == SC_PLAYING)
	{
		m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset() * m_vLookDir.x);
		m_pCollider->Set_Size(VEC2(m_pAniBmp->Get_ColliderSize().x, m_pAniBmp->Get_ColliderSize().y));

		m_pCareTaker->Add(Save_State());

		m_pRigidBody->Set_IsFall(m_bIsFall);
	}


	if (!g_bIsTimeBack)
	{
		PixelCollisionBottom();
		PixelCollisionTop();
		PixelCollisionLeft();
		PixelCollisionRight();
	}
}

void		CPlayer::Render(HDC hDC)
{
	m_vCameraPos = CCamera::Get_Instance()->GetRenderPos(m_vPos);

	HDC	hMemDC = m_pAniBmp->Get_MemDC();

	GdiTransparentBlt(hDC, // 최종적인 그림을 그릴 dc
		(int)(m_vCameraPos.x - m_pAniBmp->Get_ImageSize().x * 0.5f), // 복사 받을 위치의 left
		(int)m_vCameraPos.y - m_pAniBmp->Get_ImageSize().y * 0.5f + m_fLaserDisappear,			 // 복사 받을 위치의 top
		(int)m_pAniBmp->Get_ImageSize().x,			 // 복사 받을 가로 길이
		m_pAniBmp->Get_ImageSize().y - m_fLaserDisappear,			 // 복사 받을 세로 길이
		hMemDC,					// 비트맵을 가지고 있는 dc
		m_tFrame.iFrameStart * m_pAniBmp->Get_ImageSize().x,						// 출력할 비트맵의 시작 x좌표
		((int)(m_vLookDir.x + 2) % 3) * m_pAniBmp->Get_ImageSize().y + m_fLaserDisappear,						// 출력할 비트맵의 시작 y좌표
		(int)m_pAniBmp->Get_ImageSize().x,			// 출력할 비트맵의 가로 사이즈
		m_pAniBmp->Get_ImageSize().y - m_fLaserDisappear,			// 출력할 비트맵의 세로 사이즈
		RGB(255, 0, 255));	// 제거할 픽셀의 색상 값

	//m_pCollider->Render(hDC);
}

void		CPlayer::Release()
{
	if (m_pRigidBody)
		Safe_Delete<CRigidBody*>(m_pRigidBody);

	if (m_pCollider)
		Safe_Delete<CCollider*>(m_pCollider);

	if (m_pCareTaker)
		Safe_Delete<CCaretaker*>(m_pCareTaker);

	if(m_pPlayerFSM)
		Safe_Delete<CPlayerFSM*>(m_pPlayerFSM);
}

void		CPlayer::OnCollisionEnter(CObj* _pOther)
{
	if (_pOther->Get_Id() == OBJ_ITEM && CKeyMgr::Get_Instance()->Get_KeyState(VK_RBUTTON) == KEY_DOWN)
	{
		if (m_pItem == nullptr)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"pick.wav", SOUND_PICK, g_fEffectSound);
			m_pItem = _pOther;
			m_pItem->Set_IsActive(false);
			m_pItem->Get_Collider()->Set_IsActive(false);
		}
	}

	//불릿 타격
	if (_pOther->Get_Id() == OBJ_BULLET && _pOther->Get_Collider()->Get_IsActive())
	{
		if (dynamic_cast<CBullet*>(_pOther)->Get_Type() == BASE)
		{
			if (!dynamic_cast<CBaseBullet*>(_pOther)->Get_IsReflect())
			{
				Change_StateHURT();
			}
		}

		if (dynamic_cast<CBullet*>(_pOther)->Get_Type() == LASER || dynamic_cast<CBullet*>(_pOther)->Get_Type() == BOMB)
		{
			Change_StateHURT();
		}
	}

	if (_pOther->Get_Id() == OBJ_ENEMY)
	{
		if (m_bIsSpecialAttack && !_pOther->Get_Dead())
		{
			if(_pOther->Get_Collider()->Get_IsRectCollider() != false)
				m_vecSpecialAttack.push_back(_pOther);
		}

		if (dynamic_cast<CEnemy*>(_pOther)->Get_Type() == POMP)
		{
			if ((dynamic_cast<CPomp*>(_pOther)->Get_IsAttack()))
			{
				if (_pOther->Get_AniBmp() == CBmpMgr::Get_Instance()->Find_Image(L"Enemy_pomp_attack"))
				{
					if (_pOther->Get_LookDir().x == 1)
					{
						if (_pOther->Get_Frame().iFrameStart == 4 || _pOther->Get_Frame().iFrameStart == 5)
						{
							Change_StateHURT();
						}
					}
					else
					{
						if (_pOther->Get_Frame().iFrameStart == 0 || _pOther->Get_Frame().iFrameStart == 1)
						{
							Change_StateHURT();
						}
					}
				}
			}
		}
	}
}

void		CPlayer::OnCollisionStay(CObj* _pOther)
{
	if (_pOther->Get_Id() == OBJ_ITEM && CKeyMgr::Get_Instance()->Get_KeyState(VK_RBUTTON) == KEY_DOWN)
	{
		if (m_pItem == nullptr)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"pick.wav", SOUND_PICK, g_fEffectSound);
			m_pItem = _pOther;
			m_pItem->Set_IsActive(false);
			m_pItem->Get_Collider()->Set_IsActive(false);
		}
	}
}

void		CPlayer::OnCollisionExit(CObj* _pOther)
{
}

void CPlayer::PixelCollisionLeft()
{
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1 && dynamic_cast<CBossStage01*>(CSceneMgr::Get_Instance()->Get_NowScene())->Get_IsDestroy())
	{
		return;
	}

	for (int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f); height <= (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 10; ++height)
	{
		int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f) - 1;
		int slope = (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 8;	//더 아래쪽

		BYTE slR = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + slope]);
		BYTE slG = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + slope]);
		BYTE slB = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + slope]);

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 0)
		{

			if (CKeyMgr::Get_Instance()->Get_KeyState('A') == KEY_PRESSING)
			{

				m_bIsWallGrab = true;
			}
			else
			{
				m_bIsWallGrab = false;
			}

			m_vPos.x += 8;
			m_pRigidBody->Set_VelocityX(0.f);
			break;
		}

		//더 아래쪽이 닿았음
		if (slR == 255 && slG == 0 && slB == 255)
		{
			if (r == 255 && g == 0 && b == 255)
			{

			}
			else
			{
				m_vPos.y -= 8;
				break;
			}
		}

		if (r == 255 && g == 0 && b == 255)
		{
			m_vPos.x += 10;
			m_pRigidBody->Set_VelocityX(0.f);
			break;
		}
	}
}

void		CPlayer::PixelCollisionRight()
{
	for (int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f); height <= (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 20; ++height)
	{
		int width = (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f) + 1;
		int slope = (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f) - 8;	//더 아래쪽

		BYTE slR = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + slope]);
		BYTE slG = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + slope]);
		BYTE slB = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + slope]);

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);



		//다음 스테이지 벽
		if (r == 0 && g == 0 && b == 255)
		{
			if (CSceneMgr::Get_Instance()->Get_NowScene()->Get_IsClear())
			{
				m_vPos.x -= 10;
				m_bIsNextStage = true;
			}
			else
			{
				m_vPos.x -= 10;
				m_pRigidBody->Set_VelocityX(0.f);
			}

			break;
		}

		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1 && dynamic_cast<CBossStage01*>(CSceneMgr::Get_Instance()->Get_NowScene())->Get_IsDestroy())
		{
			return;
		}

		if (r == 255 && g == 0 && b == 0)
		{
			if (CKeyMgr::Get_Instance()->Get_KeyState('D') == KEY_PRESSING)
			{
				m_bIsWallGrab = true;
			}
			else
			{
				m_bIsWallGrab = false;
			}
				m_vPos.x -= 8;
				m_pRigidBody->Set_VelocityX(0.f);
				break;
		}

		//더 아래쪽이 닿았음
		if (slR == 255 && slG == 0 && slB == 255)
		{
			if (r == 255 && g == 0 && b == 255)
			{

			}
			else
			{
				m_vPos.y -= 8;
				break;
			}
		}
		//일반 벽
		if (r == 255 && g == 0 && b == 255)
		{
			m_vPos.x -= 8;
			m_pRigidBody->Set_VelocityX(0.f);

			break;
		}
	}
}

void		CPlayer::PixelCollisionBottom()
{
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1 && dynamic_cast<CBossStage01*>(CSceneMgr::Get_Instance()->Get_NowScene())->Get_IsDestroy())
	{
		return;
	}

	for (int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f) + 5; width <= (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f) - 5; ++width)
	{
		int height = (int)(m_pCollider->Get_Pos().y + m_pCollider->Get_Size().y * 0.5f )+ 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_pRigidBody->Set_VelocityY(0.f);
			m_vPos.y -= 1;
			m_bIsFall = false;
			m_bIsGround = true;
			m_bIsWallGrab = false;
			m_bIsDownJump = false;
			
			break;
		}
		else
		{
			if(m_eState != JUMP && m_eState != WALL_GRAB && m_eState != FLIP)
				m_bIsFall = true;
		}
		
		if (r == 0 && g == 255 && b == 0 && !m_bIsDownJump) 
		{
			m_pRigidBody->Set_VelocityY(0.f);
			m_vPos.y -= 1;

			m_bIsGround = true;
			m_bIsFall = false;
			m_bIsWallGrab = false;

			break;
		}
	}
}

void		CPlayer::PixelCollisionTop()
{
	for (int width = (int)(m_pCollider->Get_Pos().x - m_pCollider->Get_Size().x * 0.5f) + 5; width <= (int)(m_pCollider->Get_Pos().x + m_pCollider->Get_Size().x * 0.5f) - 5; ++width)
	{
		int height = (int)(m_pCollider->Get_Pos().y - m_pCollider->Get_Size().y * 0.5f) - 1;

		BYTE r = GetRValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE g = GetGValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);
		BYTE b = GetBValue(m_vecStageCol[CSceneMgr::Get_Instance()->Get_MapHeight(CSceneMgr::Get_Instance()->Get_CurScene()) * width + height]);

		if (r == 255 && g == 0 && b == 255)
		{
			m_vPos.y += 8;
			m_pRigidBody->Set_IsGround(false);
			m_bIsFall = true;
			break;
		}
	}
}

void		CPlayer::BulletTime()
{
	if (CKeyMgr::Get_Instance()->Get_KeyState(VK_SHIFT) == KEY_DOWN)
	{
		m_dwBulletTimeLimit = GetTickCount();
		CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW);
		CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Enter.wav", SOUND_SLOW, g_fEffectSound);



		CSceneMgr::Get_Instance()->Set_IsSlow(true);
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState(VK_SHIFT) == KEY_PRESSING && m_iBulletTimeGage > 0)
	{
		CTimeMgr::Get_Instance()->Set_TimeScale(0.2f);
		if (m_dwBulletTimeLimit + 1000 < GetTickCount())
		{
			--m_iBulletTimeGage;
			m_dwBulletTimeLimit = GetTickCount();
		}
	}
	else if (CKeyMgr::Get_Instance()->Get_KeyState(VK_SHIFT) == KEY_UP )
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_SLOW);
		CSoundMgr::Get_Instance()->PlaySound(L"Slowmo_Exit.wav", SOUND_SLOW, g_fEffectSound);
		CSceneMgr::Get_Instance()->Set_IsSlow(false);
		CTimeMgr::Get_Instance()->Set_TimeScale(1.f);
	}
	else 
	{
		if (m_iBulletTimeGage < 9)
		{
			if (m_dwBulletTimeLimit + 1000 < GetTickCount())
			{
				++m_iBulletTimeGage;
				m_dwBulletTimeLimit = GetTickCount();
			}
		}
	}
}

void		CPlayer::ThrowItem()
{
	if (m_pItem)
	{
		if (CKeyMgr::Get_Instance()->Get_KeyState(VK_RBUTTON) == KEY_DOWN)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"throw.wav", SOUND_THROW, g_fEffectSound);
			m_pItem->Set_Pos(m_vPos.x, m_vPos.y);
			m_pItem->Set_LookDir(Calculate_AttackDir());
			m_pItem->Set_IsActive(true);
			dynamic_cast<CItem*>(m_pItem)->Set_IsThrow(true);
			m_pItem = nullptr;
		}
	}
}

VEC2		CPlayer::Calculate_AttackDir()
{
	POINT pt;
	VEC2 vPlayerPos = m_vCameraPos;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	VEC2 vDir = { pt.x - vPlayerPos.x, pt.y - vPlayerPos.y };

	//바닥으로  공격하면 x축만 힘을 받도록 바꿔줌
	if (vPlayerPos.y < pt.y)
		vDir.y = 0.f;

	vDir.Normalize();

	return vDir;
}

void CPlayer::Select_BGSong()
{
	switch (CSceneMgr::Get_Instance()->Get_CurScene())
		{
		case SC_STAGE1:
			CSoundMgr::Get_Instance()->PlayBGM(L"song_bunker.mp3", g_fBackgGroundSound);
			break;

		case SC_STAGE2:
			CSoundMgr::Get_Instance()->PlayBGM(L"song_katanazero.ogg", g_fBackgGroundSound);
			break;

		case SC_STAGE3:
			CSoundMgr::Get_Instance()->PlayBGM(L"song_youwillneverknow.ogg", g_fBackgGroundSound);
			break;

		case SC_BOSS1:
			CSoundMgr::Get_Instance()->PlayBGM(L"song_bossbattle.ogg", g_fBackgGroundSound);
			break;
			
		case SC_BOSS2:
			CSoundMgr::Get_Instance()->PlayBGM(L"song_lastStage.mp3", g_fBackgGroundSound);
			break;

		case SC_BLOOD:
			CSoundMgr::Get_Instance()->PlayBGM(L"song_Meat_Grinder.mp3", g_fBackgGroundSound);
			break;
		}
}

void CPlayer::Change_StateHURT()
{
	m_pPlayerFSM->ChangeState(HURT);
}

void CPlayer::Laser_Die()
{
	if (m_bIsHitLaser)
	{
		m_bIsFall = false;
		m_fLaserDisappear += 1.f;
		m_pRigidBody->CalcFriction();
		m_pRigidBody->Set_Velocity(VEC2(0.f, 0.f));
		m_bDead = true;
	}
}

void		CPlayer::TimeBakcDone()
{
	m_pCollisionOthers.clear();
	m_pRigidBody->ResetGravityAccel();
	m_pRigidBody->CalcFriction();

	m_bDead = false;
	m_bIsGround = true;
	m_bIsMoveFrame = true;
	m_bIsFall = false;
	m_pItem = nullptr;

	//CCamera::Get_Instance()->SetTarget(this);

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle");
	Set_Frame();
	m_pPlayerFSM->ChangeState(IDLE);

	if (m_pCareTaker->Get_ListSize() > 0)
	{
		Set_MementoState(m_pCareTaker->Get_First());
	}


	//보스 스테이지 1이면 몬스터 전부 지우기
	if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_BOSS1)
	{
		CObjMgr::Get_Instance()->Delete_ID(OBJ_ENEMY);
	}
	m_pCareTaker->Clear_MementoList();
}

void		CPlayer::Init_Player()
{
	m_iHp = 1;
	m_vLookDir = { 1.0f, 0.f };
	m_bIsFall = true;

	m_pRigidBody->Set_VelocityLimit(VEC2(500.f, 1000.f));
	m_pRigidBody->Set_Friction(2000.f);
	m_pRigidBody->ResetGravityAccel();

	m_pAniBmp = CBmpMgr::Get_Instance()->Find_Image(L"Player_Idle");
	Set_Frame();

	m_pCollider->Set_Offset(m_pAniBmp->Get_ColliderOffset());
	m_pCollider->Set_Size(m_pAniBmp->Get_ColliderSize());

	m_vLookDir.x = 1;

	//FSM 객체 생성 및 상태 추가
	m_pPlayerFSM = new CPlayerFSM;
	m_pPlayerFSM->AddState(new CPlayerIdle);
	m_pPlayerFSM->AddState(new CPlayerRun);
	m_pPlayerFSM->AddState(new CPlayerJump);
	m_pPlayerFSM->AddState(new CPlayerFall);
	m_pPlayerFSM->AddState(new CPlayerAttack);
	m_pPlayerFSM->AddState(new CPlayerRoll);
	m_pPlayerFSM->AddState(new CPlayerWallGrab);
	m_pPlayerFSM->AddState(new CPlayerFlip);
	m_pPlayerFSM->AddState(new CPlayerHurt);

	//더 추가할거
	/*문 차기
	웅크리기(여유있으면)*/

	m_pPlayerFSM->Set_State(IDLE);
	m_pPlayerFSM->ChangeState(IDLE);
}
