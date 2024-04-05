#pragma once
#include "Bullet.h"

class CLaserBullet :public CBullet
{
public:
	CLaserBullet();
	virtual ~CLaserBullet();

public:
	void	Set_bIsGroundLaser(bool m_bLaser) { m_bIsGroundLaser = m_bLaser; }
	void	Set_ShootPos(VEC2 vPos) { m_vShootPos = vPos; }
	bool    Get_IsReflect() { return m_bIsReflect; }
	void    Set_IsReflect(bool bReflect) { m_bIsReflect = bReflect; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	bool		m_bIsGroundLaser;
	VEC2		m_vShootPos;

	DWORD		m_dwAimTime;
	DWORD		m_dwShootTime;

	bool		m_bIsReflect;
};

