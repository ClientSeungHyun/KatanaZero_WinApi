#pragma once
#include "Scene.h"

class CStageScene01 :
    public CScene
{
public:
    CStageScene01();
    virtual ~CStageScene01();

public:


public:
    virtual void            Initialize()    override;
    virtual int             Update()        override;
    virtual void            Late_Update()   override;
    virtual void            Render(HDC hDC) override;
    virtual void            Release()       override;
};

