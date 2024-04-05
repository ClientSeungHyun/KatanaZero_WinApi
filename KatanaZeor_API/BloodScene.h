#pragma once
#include "Scene.h"

class CBloodScene :
    public CScene
{
public:
    CBloodScene();
    virtual ~CBloodScene();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void                    Create_Enemy();

private:
    CObj*       m_pEnemy;
    VEC2        m_vTextPos[2];

    int         m_iScore;
    int         m_iLevel;

    bool        m_bIsDead;

    DWORD       m_dwCreateEnemyTime;
    DWORD       m_dwCreateDuration;
    DWORD       m_dwScoreTime;
};

