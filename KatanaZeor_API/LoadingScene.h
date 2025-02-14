#pragma once
#include "Scene.h"

class CLoadingScene :
    public CScene
{
public:
    CLoadingScene();
    virtual ~CLoadingScene();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

