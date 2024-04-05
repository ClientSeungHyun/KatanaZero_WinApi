#pragma once
#include "Scene.h"

class CEndingScene :
    public CScene
{
public:
    CEndingScene();
    virtual ~CEndingScene();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
};

