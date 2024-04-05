#pragma once
#include "Obj.h"

class CBossHead :
    public CObj
{
public:
    CBossHead();
    virtual ~CBossHead();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void PixelCollisionBottom();

private:
    bool        m_bIsGround;
};

