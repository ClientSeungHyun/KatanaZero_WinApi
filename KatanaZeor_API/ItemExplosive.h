#pragma once
#include "Item.h"

class CItemExplosive :
    public CItem
{
public:
    CItemExplosive();
    virtual ~CItemExplosive();


public:
    virtual void        Initialize() override;
    virtual int         Update() override;
    virtual void        Late_Update() override;
    virtual void        Render(HDC hDC) override;
    virtual void        Release() override;
};

