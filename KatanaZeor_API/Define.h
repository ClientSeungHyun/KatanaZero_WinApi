#pragma once
#include "assert.h"
#include "stdafx.h"

extern	HWND		g_hWnd;

extern float g_fBackgGroundSound;
extern float g_fEffectSound;

extern bool g_bIsTimeBack;

class CTimeMgr;

#define			WINCX		1344
#define			WINCY		680

#define			PURE		= 0

#define			OBJ_NOEVENT		0
#define			OBJ_DEAD		1

#define			PI			3.141592f

#define			VK_MAX		0xff

#define			DT			CTimeMgr::Get_Instance()->Get_DeltaTime();

enum KEYSTATE{KEY_PRESSING, KEY_DOWN, KEY_UP, KEY_NOTHING};
enum OBJID {  OBJ_BLOCK, OBJ_BULLET, OBJ_ITEM, OBJ_ENEMY, OBJ_BOSS, OBJ_EFFECT, OBJ_BG, OBJ_TRAP, OBJ_PLAYER, OBJ_UI, OBJ_BUTTON, OBJ_MOUSE, OBJ_SHADOW, OBJ_CAMERA, OBJ_HEAD, OBJ_END };
enum RENDERID { BACKGROUND, EFFECT_FIRST, ENEMY, GAMEOBJECT, EFFECT_LATER, UI, SHADOW, RENDER_END };
enum EFFECTID { EFFECT_PLAYERATTACK, EFFECT_ENEMYATTACK, EFFECT_END };
enum SCENEID { SC_LOADING, SC_TITLE, SC_MENU, SC_STAGE1, SC_STAGE2, SC_STAGE3, SC_BOSS1, SC_BOSS2, SC_ENDING, SC_BLOOD, SC_END };
enum SCENESTATE{SC_READY, SC_READY_COMPLETE, SC_PLAYING, SC_NOTHING };
enum PLAYERSTATE { IDLE, RUN, JUMP, FALL, ATTACK, ROLL, FLIP, HURT, SNEAK, WALL_GRAB, DOORKICK, PL_END };
enum BOSSSTATE { BOSS_IDLE, BOSS_LASERGROUND, BOSS_BULLETJUMP, BOSS_SHOOTBOMB, BOSS_DISAPPEAR, BOSS_LASERBLINK, BOSS_HURT, BOSS_DIE, BOSS_REALDIE, BOSS_END };
enum ENEMYSTATE { ENEMY_IDLE, ENEMY_RUN, ENEMY_WALK, ENEMY_DIE, ENEMY_ATTACK, ENEMY_END };
enum ENEMYTYPE { GRUNT, BOLD, POMP, SHIELDCOP };
enum ITEMTYPE { ITEM_KNEIF, ITEM_FIRE, ITEM_END };
enum BULLETTYPE{BASE, LASER, BOMB};

enum CHANNELID
{
	SCENE_CHANGE,
	SOUND_EFFECT,
	SOUND_REWIND,
	SOUND_ATTACK,
	SOUND_SATTACK,
	SOUND_DIE,
	SOUND_BGM,
	SOUND_SLOW,
	SOUND_THROW,
	SOUND_PICK,
	SOUND_PLAYSONG,
	ENEMY_EFFECT1,
	ENEMY_EFFECT2,
	ENEMY_EFFECT3,
	ENEMY_EFFECT4,
	ENEMY_DIE1,
	ENEMY_DIE2,
	ENEMY_DIE3,
	ENEMY_DIE4,
	ENEMY_SHOOT1,
	ENEMY_SHOOT2,
	ENEMY_SHOOT3,
	BOSS_JUMP,
	BOSS_GATLING,
	BOSS_AIM,
	BOSS_LASERSHOOT,
	BOSS_EFFECT,
	BOSS_CIRCLE,
	BOSS_EXPLOSION,
	BOSS_DIESOUND,
	MAXCHANNEL
};

typedef struct tagPoint
{
	float	fX;
	float	fY;

	tagPoint() { ZeroMemory(this, sizeof(tagPoint)); }
	tagPoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

}LINEPOINT;

typedef struct tagPixel
{
	BYTE r;
	BYTE g;
	BYTE b;
}PIXEL;

typedef struct tagLine
{
	LINEPOINT		tLPoint;
	LINEPOINT		tRPoint;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& _tLPoint, LINEPOINT& _tRPoint)
		: tLPoint(_tLPoint), tRPoint(_tRPoint) {}

}LINE;

typedef struct tagFrame
{
	int		iFrameStart;	// �� �� �ε������� ������ ���ΰ�
	int		iFrameEnd;
	int		iFrameRealEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;

	bool	isLoop;
	bool	isPlayDone;

}FRAME;



template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

template<typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

struct DeleteObj
{
	template<typename T>
	void		operator()(T& Obj)
	{
		if (Obj)
		{
			delete Obj;
			Obj = nullptr;
		}
	}
};

struct DeleteMap
{
	template<typename T>
	void		operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};


class CStringCmp
{

public:
	CStringCmp(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool operator()(T& MyPair)
	{
		return !lstrcmp(m_pString, MyPair.first);
	}

private:
	const	TCHAR*		m_pString;
};

typedef struct Vector
{
	Vector() {}
	Vector(float x, float y) : x(x), y(y) {}
	Vector(POINT pt) :x((float)pt.x), y((float)pt.y) {}

	//0�������� Ȯ��(����, ũ�Ⱑ ���� ����)
	bool IsZero()
	{
		if (x == 0.f && y == 0.f)
			return true;

		return false;
	}

	//���ϱ�
	const Vector operator+(float value) const
	{
		return Vector(x + value, y + value);
	}

	//���ϱ�
	const Vector operator+(const Vector& other) const
	{
		return Vector(x + other.x, y + other.y);
	}

	//����
	Vector operator-(const Vector& other)
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}
	
	//���̳ʽ� ���̱� ����
	Vector operator-()
	{
		return Vector(-x, -y);
	}

	//���ϱ�
	Vector operator*(float value)
	{
		Vector ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}

	//������(���ͳ���)
	Vector operator /(Vector _vOther)
	{
		//0���� ������ �ʵ��� ���� ó��
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return  Vector(x / _vOther.x, y / _vOther.y);
	}

	//������(�Ǽ���)
	Vector operator /(float _f)
	{
		//0���� ������ �ʵ��� ���� ó��
		assert(!(0.f == _f));
		return  Vector(x / _f, y / _f);
	}

	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator-=(float value)
	{
		x -= value;
		y -= value;
	}
	void operator*=(float value)
	{
		x *= value;
		y *= value;
	}

	//������ ����
	float LengthSqured()
	{
		return x * x + y * y;
	}

	//������ ����
	float Length()
	{
		return ::sqrt(LengthSqured());
	}

	//����ȭ(ũ��� ������ ���� ���ͷ� �������)
	Vector& Normalize()
	{
		float fLen;
		fLen = Length();
		if (fLen < 0.0001f)
			fLen = 0.0001f;
		//assert(fLen != 0.f);
		x /= fLen;
		y /= fLen;
		return *this;
	}
	
	//����
	float Dot(Vector other)
	{
		return x * other.x + y * other.y;
	}

	//����
	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	float x = 0;
	float y = 0;
}VEC2;