#pragma once
#include "Scene.h"

class CBossStage01 :
    public CScene
{
public:
    CBossStage01();
    virtual ~CBossStage01();

public:
    bool                    Get_IsDestroy() { return m_bIsDestroyGround; }

public:
    virtual void            Initialize()    override;
    virtual int             Update()        override;
    virtual void            Late_Update()   override;
    virtual void            Render(HDC hDC) override;
    virtual void            Release()       override;

private:
    int                     m_iExplosionCount;
    DWORD                   m_dwExplosionTime;

    bool                    m_bIsDestroyGround;
};

