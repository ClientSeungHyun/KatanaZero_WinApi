#pragma once
#include "Scene.h"

class CStageScene02 :
    public CScene
{
public:
    CStageScene02();
    virtual ~CStageScene02();

public:
    virtual void            Initialize()    override;
    virtual int             Update()        override;
    virtual void            Late_Update()   override;
    virtual void            Render(HDC hDC) override;
    virtual void            Release()       override;
};

