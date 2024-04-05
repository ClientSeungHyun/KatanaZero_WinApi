#pragma once
#include "Scene.h"

class CStageScene03 :
    public CScene
{
public:
    CStageScene03();
    virtual ~CStageScene03();

public:
    virtual void            Initialize()    override;
    virtual int             Update()        override;
    virtual void            Late_Update()   override;
    virtual void            Render(HDC hDC) override;
    virtual void            Release()       override;
};

