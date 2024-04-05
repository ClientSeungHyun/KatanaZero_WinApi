#pragma once
#include "Scene.h"

class CBossStage02 :
    public CScene
{
public:
    CBossStage02();
    virtual ~CBossStage02();

public:
    virtual void            Initialize()    override;
    virtual int             Update()        override;
    virtual void            Late_Update()   override;
    virtual void            Render(HDC hDC) override;
    virtual void            Release()       override;
};

